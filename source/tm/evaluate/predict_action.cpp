// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.predict_action;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.expectiminimax;
import tm.evaluate.scored_move;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import tm.block;
import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct MoveProbability {
	MoveName name;
	double probability;
	friend auto operator==(MoveProbability, MoveProbability) -> bool = default;
};
export using MoveProbabilities = containers::static_vector<MoveProbability, maximum_possible_selections>;

export template<Generation generation>
auto predict_action(Team<generation> const & team, LegalSelections const team_selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> const evaluate) -> MoveProbabilities {
	auto all_equally_likely = [&] {
		auto const possible_moves = double(containers::size(team_selections));
		return MoveProbabilities(containers::transform(
			team_selections,
			[=](MoveName const move) { return MoveProbability{move, 1.0 / possible_moves}; }
		));
	};
	if (containers::size(team_selections) == 1_bi) {
		return all_equally_likely();
	}
	auto const scores = expectiminimax(team, team_selections, other, other_selections, environment, evaluate, Depth(1_bi, 0_bi));
	// TODO: this is not the right way to weight move scores
	auto const score_only = containers::transform(scores, &ScoredMove::score);
	auto const min_value = *containers::min_element(score_only);
	auto adjust_score = [=](double const score) {
		return std::pow(score - min_value, 3.0);
	};
	auto const total_score = containers::sum(containers::transform(score_only, adjust_score));
	if (total_score == 0.0) {
		return all_equally_likely();
	}
	return MoveProbabilities(containers::filter(
		containers::transform(
			scores,
			[=](ScoredMove const move) {
				return MoveProbability{move.name, adjust_score(move.score) / total_score};
			}
		),
		[](MoveProbability const move) { return move.probability > 0.0; }
	));
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto predict_action(Team<generation> const &, LegalSelections, Team<generation> const &, LegalSelections, Environment, Evaluate<generation>) -> MoveProbabilities

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine