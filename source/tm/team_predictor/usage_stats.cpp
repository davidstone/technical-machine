// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/forward.hpp>

export module tm.team_predictor.usage_stats;

import tm.move.move_name;

import tm.pokemon.species;

import tm.ps_usage_stats.header;

import tm.team_predictor.usage_stats_probabilities;

import tm.ability;
import tm.generation;
import tm.item;
import tm.read_bytes;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

struct FileReader {
	explicit FileReader(std::istream & stream):
		m_stream(stream)
	{
	}
	template<typename T>
	auto read() {
		return read_bytes<T>(m_stream);
	}
private:
	std::istream & m_stream;
};

template<bounded::bounded_integer T>
auto checked_read(FileReader & reader) {
	return bounded::check_in_range<T>(reader.read<typename T::underlying_type>());
}

template<typename T> requires std::is_enum_v<T>
auto checked_read(FileReader & reader) {
	return T(checked_read<decltype(bounded::integer(T()))>(reader));
}

template<std::floating_point T>
auto checked_read(FileReader & reader) {
	auto const value = reader.read<T>();
	if (value < 0.0 or !std::isfinite(value)) {
		throw std::runtime_error("Invalid floating point value");
	}
	return value;
}

template<typename T>
auto read_count_for(FileReader & reader) {
	using Count = bounded::integer<0, bounded::normalize<bounded::number_of<T>>>;
	return checked_read<Count>(reader);
}

template<typename Key>
struct CheckedRead {
	explicit constexpr CheckedRead(FileReader & reader):
		m_reader(reader)
	{
	}
	constexpr auto operator()() const {
		return checked_read<Key>(m_reader);
	}
private:
	std::reference_wrapper<FileReader> m_reader;
};
template<typename Key>
auto read_map(FileReader & reader) {
	return containers::generate_n(read_count_for<Key>(reader), CheckedRead<Key>(reader));
}

auto checked_header_read(FileReader & reader) -> void {
	auto str = reader.read<decltype(usage_stats_magic_string)>();
	if (str != usage_stats_magic_string) {
		throw std::runtime_error("Invalid magic string");
	}
	if (checked_read<UsageStatsVersion>(reader) != 0_bi) {
		throw std::runtime_error("Invalid version");
	}
}

template<typename Container>
constexpr auto checked_insert(Container & container, typename Container::key_type const key, auto && mapped) -> auto & {
	auto const result = container.lazy_insert(key, OPERATORS_FORWARD(mapped));
	if (!result.inserted) {
		throw std::runtime_error("Duplicate values in stats file");
	}
	return containers::get_mapped(*result.iterator);
}

struct SpeedDistribution {
};

template<std::same_as<SpeedDistribution>>
auto checked_read(FileReader & reader) {
	for ([[maybe_unused]] auto const value : read_map<InitialSpeed>(reader)) {
		checked_read<double>(reader);
	}
	return SpeedDistribution();
}

template<typename T>
auto read_inner_probabilities(FileReader & reader, double const species_weight) {
	using Data = UsageStatsProbabilities::Data<T>;
	using Map = Data::Map;
	auto const elements = read_map<T>(reader);
	return Data(containers::transform(elements, [&](T const key) {
		auto const relative_weight = checked_read<double>(reader);
		return containers::range_value_t<Map>{
			key,
			static_cast<float>(species_weight * relative_weight)
		};
	}));
}

template<std::same_as<UsageStatsProbabilities::Map>>
auto checked_read(FileReader & reader) {
	auto const all_species = read_map<Species>(reader);
	return UsageStatsProbabilities::Map(containers::transform(all_species, [&](Species const species) {
		auto const weight = checked_read<double>(reader);
		return containers::range_value_t<UsageStatsProbabilities::Map>{
			species,
			{
				read_inner_probabilities<MoveName>(reader, weight),
				UsageStatsProbabilities::Data<Item>(),
				UsageStatsProbabilities::Data<Ability>()
			}
		};
	}));
}

} // namespace

export struct UsageStats {
	static auto make(std::istream && stream) -> UsageStats {
		auto reader = FileReader(stream);
		checked_header_read(reader);
		// TODO: Confirm this is the expected generation?
		auto const generation = checked_read<Generation>(reader);
		auto data = Data();
		auto probabilities = UsageStatsProbabilities::Map();

		for (auto const species : read_map<Species>(reader)) {
			auto const species_weight = checked_read<double>(reader);
			checked_read<SpeedDistribution>(reader);
			auto per_species_probabilities = checked_read<UsageStatsProbabilities::Map>(reader);
			auto used_moves = UsedMoves();
			auto read_items = [&] {
				return generation >= Generation::two ?
					read_inner_probabilities<Item>(reader, species_weight) :
					UsageStatsProbabilities::Data<Item>();
			};
			auto read_abilities = [&] {
				return generation >= Generation::three ?
					read_inner_probabilities<Ability>(reader, species_weight) :
					UsageStatsProbabilities::Data<Ability>();
			};
			auto & for_this_species = checked_insert(
				per_species_probabilities,
				species,
				bounded::construct<UsageStatsProbabilities::Inner>
			);
			auto & probabilities_assuming_species = checked_insert(
				probabilities,
				species,
				bounded::construct<UsageStatsProbabilities::Inner>
			);
			for (auto const & move_name : read_map<MoveName>(reader)) {
				auto const move_weight = checked_read<double>(reader);
				checked_insert(
					for_this_species.moves,
					move_name,
					bounded::value_to_function(static_cast<float>(move_weight))
				);
				[[maybe_unused]] auto const speed_distribution = checked_read<SpeedDistribution>(reader);
				auto teammates = checked_read<UsageStatsProbabilities::Map>(reader);
				auto const weight = species_weight * move_weight;
				checked_insert(
					probabilities_assuming_species.moves,
					move_name,
					bounded::value_to_function(static_cast<float>(weight))
				);
				auto moves = read_inner_probabilities<MoveName>(reader, species_weight);
				auto items = read_items();
				auto abilities = read_abilities();
				checked_insert(
					teammates,
					species,
					[&] {
						return UsageStatsProbabilities::Inner{
							std::move(moves),
							std::move(items),
							std::move(abilities)
						};
					}
				);
				checked_insert(
					used_moves,
					move_name,
					[&] { return UsageStatsProbabilities(std::move(teammates)); }
				);
			}
			for_this_species.items = read_items();
			probabilities_assuming_species.items = for_this_species.items;
			for_this_species.abilities = read_abilities();
			probabilities_assuming_species.abilities = for_this_species.abilities;
			checked_insert(data, species, [&] {
				return PerSpecies{
					std::move(used_moves),
					UsageStatsProbabilities(std::move(per_species_probabilities))
				};
			});
		}
		return UsageStats(std::move(data), UsageStatsProbabilities(std::move(probabilities)));
	}

	auto assuming() const -> UsageStatsProbabilities const & {
		return m_probabilities;
	}
	auto assuming(Species const species) const -> UsageStatsProbabilities const * {
		auto const per_species = containers::lookup(m_data, species);
		return per_species ? std::addressof(per_species->probabilities) : nullptr;
	}
	auto assuming(Species const species, MoveName const moves) const -> UsageStatsProbabilities const * {
		auto const per_species = containers::lookup(m_data, species);
		return per_species ?
			containers::lookup(per_species->used_moves, moves) :
			nullptr;
	}

private:
	using UsedMoves = containers::flat_map<MoveName, UsageStatsProbabilities>;
	struct PerSpecies {
		UsedMoves used_moves;
		UsageStatsProbabilities probabilities;
	};
	using Data = containers::flat_map<Species, PerSpecies>;

	explicit UsageStats(Data data, UsageStatsProbabilities probabilities):
		m_data(std::move(data)),
		m_probabilities(std::move(probabilities))
	{
	}

	Data m_data;
	UsageStatsProbabilities m_probabilities;
};

} // namespace technicalmachine
