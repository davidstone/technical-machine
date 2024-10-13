// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.ps_usage_stats.usage_stats;

import tm.clients.ps.parsed_team;

import tm.move.move_name;
import tm.move.move_names;

import tm.pokemon.species;

import tm.ps_usage_stats.header;

import tm.stat.calculate_speed;
import tm.stat.stage;

import tm.ability;
import tm.generation_generic;
import tm.item;
import tm.to_index;
import tm.usage_for;
import tm.weather;

import bounded;
import concurrent;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine::ps_usage_stats {

export using SpeedDistribution = UsageFor<InitialSpeed>;

constexpr auto calculate_speed(ps::ParsedPokemon const pokemon) {
	constexpr auto is_unburdened = false;
	constexpr auto slow_start_is_active = false;
	constexpr auto tailwind = false;
	return calculate_speed(
		pokemon.stats.spe,
		pokemon.species,
		pokemon.item,
		pokemon.ability,
		pokemon.status,
		Stages(),
		is_unburdened,
		slow_start_is_active,
		tailwind,
		Weather::clear
	);
}

// Getting the full data set of correlations would require free memory measured
// in TiB. Instead we restrict ourselves to correlations among the top N most
// used moves per Pokemon. However, it's impossible to know in advance what this
// set will be, which means we need to run two full passes over the data -- the
// first determines lower-cardinality data that informs what data to gather for
// the higher-cardinality second pass.
export struct UsageStats {
	auto add(ps::ParsedTeam const & team, double const weight) & -> void {
		for (auto const & pokemon : team) {
			auto & per_species = m_all[bounded::integer(pokemon.species)];
			per_species.total += weight;
			per_species.abilities[bounded::integer(pokemon.ability)] += weight;
			per_species.items[bounded::integer(pokemon.item)] += weight;
			for (auto const move : pokemon.moves) {
				per_species.moves[to_index(move)] += weight;
			}
			auto const calculated_speed = calculate_speed(pokemon);
			containers::at(per_species.speed, calculated_speed) += weight;
		}
		m_total_teams += weight;
	}

	constexpr auto get_total(Species const species) const {
		return m_all[bounded::integer(species)].total;
	}
	constexpr auto get(Species const species, Ability const ability) const {
		auto const & per_species = m_all[bounded::integer(species)];
		return per_species.abilities[bounded::integer(ability)];
	}
	constexpr auto get(Species const species, Item const item) const {
		auto const & per_species = m_all[bounded::integer(species)];
		return per_species.items[bounded::integer(item)];
	}
	constexpr auto const & moves(Species const species) const {
		return m_all[bounded::integer(species)].moves;
	}
	constexpr auto get(Species const species, MoveName const move_name) const {
		return moves(species)[to_index(move_name)];
	}
	constexpr auto const & speed_distribution(Species const species) const {
		return m_all[bounded::integer(species)].speed;
	}
	constexpr auto total_teams() const {
		return m_total_teams;
	}

private:
	struct PerSpecies {
		double total = 0.0;
		UsageFor<Ability> abilities{};
		UsageFor<Item> items{};
		UsageFor<MoveName> moves{};
		SpeedDistribution speed{};
	};
	UsageFor<Species, PerSpecies> m_all;
	double m_total_teams = 0.0;
};

auto get_used(UsageFor<MoveName> const & moves) {
	return containers::filter(
		containers::enum_range<MoveName>(),
		[&](MoveName const move) { return moves[to_index(move)] != 0.0; }
	);
}

struct PerTeammate {
	double usage = 0.0;
	containers::flat_map<MoveName, double> other_moves;
};

using Teammates = UsageFor<Species, PerTeammate>;

struct MoveData {
	Teammates teammates;
	UsageFor<MoveName> moves = {};
	UsageFor<Item> items = {};
	UsageFor<Ability> abilities = {};
	SpeedDistribution speed = {};
};

auto populate_correlation(containers::flat_map<MoveName, double> & correlations, MoveName const key, double const weight) {
	auto & mapped = containers::keyed_insert(correlations, key, 0.0).iterator->mapped;
	mapped += weight;
}

auto populate_teammate_correlations(Teammates & teammates, ps::ParsedTeam const & team, ps::ParsedPokemon const & pokemon1, double const weight) -> void {
	auto is_different = [&](ps::ParsedPokemon const & pokemon2) {
		return std::addressof(pokemon1) != std::addressof(pokemon2);
	};
	for (auto const & pokemon2 : containers::filter(team, is_different)) {
		auto & mapped = teammates[bounded::integer(pokemon2.species)];
		mapped.usage += weight;
		for (auto const move2 : pokemon2.moves) {
			populate_correlation(mapped.other_moves, move2, weight);
		}
	}
}

auto populate_move_correlations(UsageFor<MoveName> & correlations, MoveNames const moves, MoveName const move1, double const weight) -> void {
	auto is_different = [=](MoveName const move2) { return move2 != move1; };
	for (auto const move2 : containers::filter(moves, is_different)) {
		correlations[to_index(move2)] += weight;
	}
}

auto populate_correlations(MoveData & data, ps::ParsedTeam const & team, ps::ParsedPokemon const & pokemon, MoveName const move_name, double const weight) -> void {
	populate_teammate_correlations(data.teammates, team, pokemon, weight);
	populate_move_correlations(data.moves, pokemon.moves, move_name, weight);
	data.items[bounded::integer(pokemon.item)] += weight;
	data.abilities[bounded::integer(pokemon.ability)] += weight;
	auto const calculated_speed = calculate_speed(pokemon);
	containers::at(data.speed, calculated_speed) += weight;
}

export struct Correlations {
	using PerTeammate = PerTeammate;
	using Teammates = Teammates;
	using MoveData = MoveData;
	using TopMoves = containers::flat_map<MoveName, std::unique_ptr<concurrent::locked_access<MoveData>>>;
	struct PerSpecies {
		TopMoves top_moves;
		std::unique_ptr<concurrent::locked_access<Teammates>> teammates = std::make_unique<concurrent::locked_access<Teammates>>();
	};

	explicit Correlations(UsageStats const & usage_stats) {
		for (auto const species : containers::enum_range<Species>()) {
			m_data[bounded::integer(species)].top_moves = TopMoves(
				containers::assume_sorted_unique,
				containers::transform(get_used(usage_stats.moves(species)), [](MoveName const move) {
					return containers::range_value_t<TopMoves>{
						move,
						std::make_unique<concurrent::locked_access<MoveData>>()
					};
				})
			);
		}
	}

	auto add(ps::ParsedTeam const & team, double const weight) & -> void {
		for (auto const & pokemon : team) {
			auto & per_species = m_data[bounded::integer(pokemon.species)];
			{
				auto const locked = per_species.teammates->locked();
				populate_teammate_correlations(locked.value(), team, pokemon, weight);
			}
			for (auto const move : pokemon.moves) {
				auto const maybe_correlations = containers::lookup(per_species.top_moves, move);
				if (!maybe_correlations) {
					continue;
				}
				auto & correlations = *maybe_correlations;
				{
					auto const locked = correlations->locked();
					populate_correlations(locked.value(), team, pokemon, move, weight);
				}
			}
		}
	}

	constexpr auto top_moves(Species const species) const -> TopMoves const & {
		return m_data[bounded::integer(species)].top_moves;
	}
	constexpr auto teammates(Species const species) const -> Teammates const & {
		return m_data[bounded::integer(species)].teammates->unlocked();
	}

private:
	UsageFor<Species, PerSpecies> m_data;
};

} // namespace technicalmachine::ps_usage_stats
