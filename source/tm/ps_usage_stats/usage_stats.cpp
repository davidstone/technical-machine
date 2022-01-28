// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/stat/calculate.hpp>

#include <bounded/detail/variant/visit.hpp>

#include <containers/algorithms/binary_search.hpp>
#include <containers/algorithms/keyed_insert.hpp>
#include <containers/algorithms/is_sorted.hpp>
#include <containers/at.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine::ps_usage_stats {

auto UsageStats::add(GenerationGeneric<Team> const & t, double const weight) & -> void {
	bounded::visit(t, [&](auto const & team) {
		for (auto const & pokemon : team.all_pokemon()) {
			auto & per_species = containers::at(m_all, pokemon.species());
			per_species.total += weight;
			containers::at(per_species.abilities, pokemon.initial_ability()) += weight;
			containers::at(per_species.items, pokemon.item(false, false)) += weight;
			for (auto const move : pokemon.regular_moves()) {
				containers::at(per_species.moves, move.name()) += weight;
			}
			auto const calculated_speed = calculate_speed(Team({pokemon}), Ability::Honey_Gather, Weather());
			per_species.speed[bounded::check_in_range(calculated_speed, 1_bi, max_initial_speed)] += weight;
		}
	});
	m_total_teams += weight;
}

struct LocalTopMoves {
	Moves move;
	double value;
};

constexpr auto compare_count = [](LocalTopMoves const lhs, LocalTopMoves const rhs) {
	if (auto const cmp = lhs.value <=> rhs.value; cmp != 0) {
		return cmp > 0;
	}
	return lhs.move > rhs.move;
};

constexpr decltype(auto) legacy_algorithm(containers::range auto && range, auto compare, auto algorithm) {
	algorithm(
		containers::legacy_iterator(containers::begin(range)),
		containers::legacy_iterator(containers::end(range)),
		compare
	);
}

constexpr auto get_top_n(containers::array<double, number_of<Moves>> const & moves, auto const n) {
	auto top_moves = containers::static_vector<LocalTopMoves, n>();
	for (auto const move : containers::enum_range<Moves>()) {
		auto const value = containers::at(moves, move);
		if (value == 0) {
			continue;
		}
		if (containers::size(top_moves) < n) {
			containers::push_back(top_moves, LocalTopMoves{move, value});
			if (containers::size(top_moves) == n) {
				legacy_algorithm(top_moves, compare_count, [](auto && ... args) {
					std::make_heap(OPERATORS_FORWARD(args)...);
				});
			}
		} else if (value > containers::back(top_moves).value) {
			legacy_algorithm(top_moves, compare_count, [](auto && ... args) {
				std::pop_heap(OPERATORS_FORWARD(args)...);
			});
			containers::back(top_moves) = LocalTopMoves{move, value};
			legacy_algorithm(top_moves, compare_count, [](auto && ... args) {
				std::push_heap(OPERATORS_FORWARD(args)...);
			});
		}
	}
	return top_moves;
}

Correlations::Correlations(UsageStats const & usage_stats) {
	for (auto const species : containers::enum_range<Species>()) {
		containers::at(m_top_moves, species) = TopMoves(
			containers::assume_unique,
			containers::transform(get_top_n(usage_stats.moves(species), top_n_cutoff), [](LocalTopMoves const moves) {
				return containers::range_value_t<TopMoves>{moves.move, {}};
			})
		);
	}
}

namespace {

auto populate_correlation(auto & correlations, auto const key, double const weight) {
	auto & mapped = containers::keyed_insert(correlations, key, 0.0).iterator->mapped;
	mapped += weight;
}

auto populate_teammate_correlations(auto & correlations, auto const & team, auto const & pokemon1, double const weight) -> void {
	auto is_different = [&](auto const & pokemon2) { return std::addressof(pokemon1) != std::addressof(pokemon2); };
	for (auto const & pokemon2 : containers::filter(team.all_pokemon(), is_different)) {
		auto & mapped = containers::at(correlations, pokemon2.species());
		mapped.usage += weight;
		for (auto const move2 : pokemon2.regular_moves()) {
			populate_correlation(mapped.other_moves, move2.name(), weight);
		}
	}
}

auto populate_move_correlations(auto & correlations, RegularMoves const moves, Moves const move1, double const weight) -> void {
	auto is_different = [=](Move const move2) { return move2.name() != move1; };
	for (auto const move2 : containers::filter(moves, is_different)) {
		containers::at(correlations, move2.name()) += weight;
	}
}

} // namespace

auto Correlations::add(GenerationGeneric<Team> const & t, double const weight) & -> void {
	bounded::visit(t, [&](auto const & team) {
		for (auto const & pokemon : team.all_pokemon()) {
			auto & per_species = containers::at(m_top_moves, pokemon.species());
			for (auto const move : pokemon.regular_moves()) {
				auto const move_name = move.name();
				auto const correlations = containers::lookup(per_species, move_name);
				if (!correlations) {
					continue;
				}
				auto lock = correlations->lock();
				populate_teammate_correlations(correlations->teammates(), team, pokemon, weight);
				populate_move_correlations(correlations->moves(), pokemon.regular_moves(), move_name, weight);
				containers::at(correlations->items(), pokemon.item(false, false)) += weight;
				containers::at(correlations->abilities(), pokemon.initial_ability()) += weight;
			}
		}
	});
}

} // namespace technicalmachine::ps_usage_stats
