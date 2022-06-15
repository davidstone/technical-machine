// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/ps_usage_stats/header.hpp>

#include <tm/stat/ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/load_json_from_file.hpp>
#include <tm/get_directory.hpp>

#include <bounded/to_integer.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <bit>
#include <iostream>
#include <stdexcept>
#include <utility>


namespace technicalmachine {
namespace {

struct FileReader {
	explicit FileReader(std::istream & stream):
		m_stream(stream)
	{
	}
	auto read(auto const size) {
		auto buffer = containers::array<std::byte, size>();
		auto const ptr = reinterpret_cast<char *>(std::addressof(buffer));
		m_stream.read(ptr, static_cast<std::streamsize>(size));
		return buffer;
	}
	auto is_done() const {
		return m_stream.eof();
	}
private:
	std::istream & m_stream;
};

template<typename T>
auto read_as(FileReader & reader) {
	return std::bit_cast<T>(reader.read(bounded::size_of<T>));
}

template<bounded::bounded_integer T>
auto checked_read(FileReader & reader) {
	return bounded::check_in_range<T>(read_as<typename T::underlying_type>(reader));
}

template<typename T> requires std::is_enum_v<T>
auto checked_read(FileReader & reader) {
	return T(checked_read<decltype(bounded::integer(T()))>(reader));
}

template<std::floating_point T>
auto checked_read(FileReader & reader) {
	auto const value = read_as<T>(reader);
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
auto read_map(FileReader & reader) {
	return containers::generate_n(read_count_for<Key>(reader), [&] {
		return checked_read<Key>(reader);
	});
}

auto checked_header_read(FileReader & reader) -> void {
	auto str = reader.read(containers::size(usage_stats_magic_string));
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
	using Map = typename UsageStatsProbabilities::Data<T>::Map;
	auto const elements = read_map<T>(reader);
	return UsageStatsProbabilities::Data<T>(containers::transform(elements, [&](T const key) {
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
				UsageStatsProbabilities::Data<Item>(),
				read_inner_probabilities<Moves>(reader, weight)
			}
		};
	}));
}

} // namespace

auto UsageStats::make(std::istream && stream) -> UsageStats {
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
		auto skip_abilities = [&] {
			if (generation >= Generation::three) {
				for ([[maybe_unused]] auto const & value : read_map<Ability>(reader)) {
					checked_read<double>(reader);
				}
			}
		};
		auto & for_this_species = checked_insert(
			per_species_probabilities,
			species,
			bounded::construct_return<UsageStatsProbabilities::Inner>
		);
		auto & probabilities_assuming_species = checked_insert(
			probabilities,
			species,
			bounded::construct_return<UsageStatsProbabilities::Inner>
		);
		for (auto const & move_name : read_map<Moves>(reader)) {
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
			auto moves = read_inner_probabilities<Moves>(reader, species_weight);
			auto items = read_items();
			skip_abilities();
			checked_insert(
				teammates,
				species,
				[&] {
					return UsageStatsProbabilities::Inner{
						std::move(items),
						std::move(moves)
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
		skip_abilities();
		checked_insert(data, species, [&] {
			return PerSpecies{
				std::move(used_moves),
				UsageStatsProbabilities(std::move(per_species_probabilities))
			};
		});
	}
	return UsageStats(std::move(data), UsageStatsProbabilities(std::move(probabilities)));
}

namespace {

auto stats_for_generation(Generation const generation) {
	std::cout << "Loading stats for generation " << to_string(generation) << '\n' << std::flush;
	return UsageStats::make(open_file(get_usage_stats_directory() / to_string(generation) / "OU.tmus"));
}

} // namespace

AllUsageStats::AllUsageStats():
	m_all_stats{
		stats_for_generation(Generation::one),
		stats_for_generation(Generation::two),
		stats_for_generation(Generation::three),
		stats_for_generation(Generation::four),
		stats_for_generation(Generation::five),
		stats_for_generation(Generation::six),
		stats_for_generation(Generation::seven),
		stats_for_generation(Generation::eight)
	}
{
	static_assert(numeric_traits::max_value<Generation> == Generation::eight);
	std::cout << "Done loading stats\n" << std::flush;
}

} // namespace technicalmachine
