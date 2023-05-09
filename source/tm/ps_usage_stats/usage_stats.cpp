// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.ps_usage_stats.usage_stats;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.species;

import tm.ps_usage_stats.header;

import tm.stat.calculate;

import tm.ability;
import tm.environment;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps_usage_stats {

export using SpeedDistribution = containers::array<double, bounded::number_of<InitialSpeed>>;

// Getting the full data set of correlations would require free memory measured
// in TiB. Instead we restrict ourselves to correlations among the top N most
// used moves per Pokemon. However, it's impossible to know in advance what this
// set will be, which means we need to run two full passes over the data -- the
// first determines lower-cardinality data that informs what data to gather for
// the higher-cardinality second pass.
export struct UsageStats {
	auto add(GenerationGeneric<Team> const & t, double const weight) & -> void {
		tv::visit(t, [&]<typename TeamType>(TeamType const & team) {
			for (auto const & pokemon : team.all_pokemon()) {
				auto & per_species = m_all[bounded::integer(pokemon.species())];
				per_species.total += weight;
				per_species.abilities[bounded::integer(pokemon.initial_ability())] += weight;
				per_species.items[bounded::integer(pokemon.item(false, false))] += weight;
				for (auto const move : pokemon.regular_moves()) {
					per_species.moves[bounded::integer(move.name())] += weight;
				}
				auto const calculated_speed = calculate_speed(TeamType({pokemon}), Ability::Honey_Gather, Environment());
				containers::at(per_species.speed, calculated_speed) += weight;
			}
		});
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
		return moves(species)[bounded::integer(move_name)];
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
		containers::array<double, bounded::number_of<Ability>> abilities{};
		containers::array<double, bounded::number_of<Item>> items{};
		containers::array<double, bounded::number_of<MoveName>> moves{};
		SpeedDistribution speed{};
	};
	containers::array<PerSpecies, bounded::number_of<Species>> m_all;
	double m_total_teams = 0.0;
};

template<typename T>
struct LockedAccess {
	auto locked() & {
		struct Result {
			std::scoped_lock<std::mutex> lock;
			T & data;
		};
		return Result{
			std::scoped_lock(m_mutex),
			m_data
		};
	}
	constexpr auto unlocked() const -> T const & {
		return m_data;
	}

private:
	mutable std::mutex m_mutex;
	T m_data;
};

struct LocalTopMoves {
	MoveName move;
	double value;
};

auto get_most_used(containers::array<double, bounded::number_of<MoveName>> const & moves, double const percent_threshold) -> containers::vector<LocalTopMoves> {
	auto const total_sum = containers::sum(moves);
	if (total_sum == 0.0) {
		return {};
	}
	auto const usage_threshold = percent_threshold * total_sum;
	auto top_moves = containers::vector<LocalTopMoves>(containers::transform(
		containers::enum_range<MoveName>(),
		[&](MoveName const move) { return LocalTopMoves{move, moves[bounded::integer(move)]}; }
	));
	auto current_sum = 0.0;
	containers::ska_sort(top_moves, [](LocalTopMoves const x) { return -x.value; });
	auto it = containers::begin(top_moves);
	while (true) {
		BOUNDED_ASSERT(it != containers::end(top_moves));
		current_sum += it->value;
		++it;
		if (current_sum >= usage_threshold) {
			break;
		}
	}
	containers::erase_to_end(top_moves, it);
	return top_moves;
}

struct PerTeammate {
	double usage = 0.0;
	containers::flat_map<MoveName, double> other_moves;
};

using Teammates = containers::array<PerTeammate, bounded::number_of<Species>>;

struct MoveData {
	Teammates teammates;
	containers::array<double, bounded::number_of<MoveName>> moves = {};
	containers::array<double, bounded::number_of<Item>> items = {};
	containers::array<double, bounded::number_of<Ability>> abilities = {};
	SpeedDistribution speed = {};
};

auto populate_correlation(auto & correlations, auto const key, double const weight) {
	auto & mapped = containers::keyed_insert(correlations, key, 0.0).iterator->mapped;
	mapped += weight;
}

auto populate_teammate_correlations(Teammates & teammates, auto const & team, auto const & pokemon1, double const weight) -> void {
	auto is_different = [&](auto const & pokemon2) { return std::addressof(pokemon1) != std::addressof(pokemon2); };
	for (auto const & pokemon2 : containers::filter(team.all_pokemon(), is_different)) {
		auto & mapped = teammates[bounded::integer(pokemon2.species())];
		mapped.usage += weight;
		for (auto const move2 : pokemon2.regular_moves()) {
			populate_correlation(mapped.other_moves, move2.name(), weight);
		}
	}
}

auto populate_move_correlations(auto & correlations, RegularMoves const moves, MoveName const move1, double const weight) -> void {
	auto is_different = [=](Move const move2) { return move2.name() != move1; };
	for (auto const move2 : containers::filter(moves, is_different)) {
		correlations[bounded::integer(move2.name())] += weight;
	}
}

template<typename TeamType>
auto populate_correlations(MoveData & data, TeamType const & team, auto const & pokemon, MoveName const move_name, double const weight) -> void {
	populate_teammate_correlations(data.teammates, team, pokemon, weight);
	populate_move_correlations(data.moves, pokemon.regular_moves(), move_name, weight);
	data.items[bounded::integer(pokemon.item(false, false))] += weight;
	data.abilities[bounded::integer(pokemon.initial_ability())] += weight;
	auto const calculated_speed = calculate_speed(TeamType({pokemon}), Ability::Honey_Gather, Environment());
	containers::at(data.speed, calculated_speed) += weight;
}

export struct Correlations {
	using PerTeammate = PerTeammate;
	using Teammates = Teammates;
	using MoveData = MoveData;
	using TopMoves = containers::flat_map<MoveName, std::unique_ptr<LockedAccess<MoveData>>>;
	struct PerSpecies {
		TopMoves top_moves;
		std::unique_ptr<LockedAccess<Teammates>> teammates = std::make_unique<LockedAccess<Teammates>>();
	};

	explicit Correlations(UsageStats const & usage_stats) {
		constexpr auto threshold = 0.95;
		for (auto const species : containers::enum_range<Species>()) {
			m_data[bounded::integer(species)].top_moves = TopMoves(
				containers::assume_unique,
				containers::transform(get_most_used(usage_stats.moves(species), threshold), [](LocalTopMoves const moves) {
					return containers::range_value_t<TopMoves>{moves.move, std::make_unique<LockedAccess<MoveData>>()};
				})
			);
		}
	}

	auto add(GenerationGeneric<Team> const & t, double const weight) & -> void {
		tv::visit(t, [&](auto const & team) {
			for (auto const & pokemon : team.all_pokemon()) {
				auto & per_species = m_data[bounded::integer(pokemon.species())];
				populate_teammate_correlations(per_species.teammates->locked().data, team, pokemon, weight);
				for (auto const move : pokemon.regular_moves()) {
					auto const move_name = move.name();
					auto const maybe_correlations = containers::lookup(per_species.top_moves, move_name);
					if (!maybe_correlations) {
						continue;
					}
					auto & correlations = *maybe_correlations;
					populate_correlations(correlations->locked().data, team, pokemon, move_name, weight);
				}
			}
		});
	}

	constexpr auto top_moves(Species const species) const -> TopMoves const & {
		return m_data[bounded::integer(species)].top_moves;
	}
	constexpr auto teammates(Species const species) const -> Teammates const & {
		return m_data[bounded::integer(species)].teammates->unlocked();
	}

private:
	containers::array<PerSpecies, bounded::number_of<Species>> m_data;
};

} // namespace technicalmachine::ps_usage_stats
