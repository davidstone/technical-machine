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
import tm.binary_file_reader;
import tm.generation;
import tm.item;
import tm.weight;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto check_header(std::istream & stream) -> void {
	auto str = read_bytes(stream, containers::size(usage_stats_magic_string));
	if (str != usage_stats_magic_string) {
		throw std::runtime_error("Invalid magic string");
	}
	if (read<UsageStatsVersion>(stream) != 0_bi) {
		throw std::runtime_error("Invalid version");
	}
}

struct SpeedDistribution {
};

auto read_speed_distribution(std::istream & stream) -> SpeedDistribution {
	auto const distribution = map_reader<InitialSpeed>(stream, [&] { return read_weight(stream); });
	for ([[maybe_unused]] auto const value : distribution) {
	}
	return SpeedDistribution();
}

template<typename T>
auto read_inner_probabilities(std::istream & stream, Weight<double> const species_weight) {
	return UsageStatsProbabilities::Data<T>(map_reader<T>(stream, [&] {
		auto const relative_weight = read_weight(stream);
		return Weight<float>(species_weight * relative_weight);
	}));
}

auto read_probabilities_map(std::istream & stream) -> UsageStatsProbabilities::Map {
	return UsageStatsProbabilities::Map(map_reader<Species>(stream, [&] {
		auto const weight = read_weight(stream);
		return UsageStatsProbabilities::Inner(
			read_inner_probabilities<MoveName>(stream, weight),
			UsageStatsProbabilities::Data<Item>(),
			UsageStatsProbabilities::Data<Ability>()
		);
	}));
}

template<typename Key>
auto read_map(std::istream & stream) {
	using Count = bounded::integer<0, bounded::normalize<bounded::number_of<Key>>>;
	return containers::generate_n(
		read<Count>(stream),
		[&] { return read<Key>(stream); }
	);
}

auto read_items(
	std::istream & stream,
	Generation const generation,
	Weight<double> const species_weight
) -> UsageStatsProbabilities::Data<Item> {
	return generation >= Generation::two ?
		read_inner_probabilities<Item>(stream, species_weight) :
		UsageStatsProbabilities::Data<Item>();
}

auto read_abilities(
	std::istream & stream,
	Generation const generation,
	Weight<double> const species_weight
) -> UsageStatsProbabilities::Data<Ability> {
	return generation >= Generation::three ?
		read_inner_probabilities<Ability>(stream, species_weight) :
		UsageStatsProbabilities::Data<Ability>();
}

template<typename Container>
constexpr auto checked_insert(Container & container, typename Container::key_type const key, auto && mapped) -> auto & {
	auto const result = container.lazy_insert(key, OPERATORS_FORWARD(mapped));
	if (!result.inserted) {
		throw std::runtime_error("Duplicate values in stats file");
	}
	return containers::get_mapped(*result.iterator);
}

struct ReadMoveData {
	[[no_unique_address]] Weight<double> weight;
	[[no_unique_address]] SpeedDistribution speed_distribution;
	[[no_unique_address]] UsageStatsProbabilities::Map teammates;
};

auto move_reader(std::istream & stream, Generation const generation, Species const species, Weight<double> const species_weight) {
	return map_reader<MoveName>(stream, [=, &stream] {
		auto const weight = read_weight(stream);
		auto speed_distribution = read_speed_distribution(stream);
		auto teammates = read_probabilities_map(stream);
		checked_insert(
			teammates,
			species,
			[&] {
				return UsageStatsProbabilities::Inner{
					read_inner_probabilities<MoveName>(stream, species_weight),
					read_items(stream, generation, species_weight),
					read_abilities(stream, generation, species_weight)
				};
			}
		);
		return ReadMoveData{
			weight,
			std::move(speed_distribution),
			std::move(teammates)
		};
	});
}

} // namespace

export struct UsageStats {
	static auto make(std::istream && stream) -> UsageStats {
		check_header(stream);
		// TODO: Confirm this is the expected generation?
		auto const generation = read<Generation>(stream);
		auto data = Data();
		auto probabilities = UsageStatsProbabilities::Map();

		for (auto const species : read_map<Species>(stream)) {
			auto const species_weight = read_weight(stream);
			[[maybe_unused]] auto speed_distribution = read_speed_distribution(stream);
			auto per_species_probabilities = read_probabilities_map(stream);
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
			auto used_moves = UsedMoves();
			for (auto move : move_reader(stream, generation, species, species_weight)) {
				checked_insert(
					for_this_species.moves,
					move.key,
					bounded::value_to_function(Weight<float>(move.mapped.weight))
				);
				checked_insert(
					probabilities_assuming_species.moves,
					move.key,
					bounded::value_to_function(Weight<float>(species_weight * move.mapped.weight))
				);
				checked_insert(
					used_moves,
					move.key,
					[&] { return UsageStatsProbabilities(std::move(move.mapped.teammates)); }
				);
			}
			for_this_species.items = read_items(stream, generation, species_weight);
			probabilities_assuming_species.items = for_this_species.items;
			for_this_species.abilities = read_abilities(stream, generation, species_weight);
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
