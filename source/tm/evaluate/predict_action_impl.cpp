// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.evaluate.predict_action;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.expectiminimax;
import tm.evaluate.move_probability;
import tm.evaluate.scored_move;

import tm.move.legal_selections;
import tm.move.move_name;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto predict_action(Team<generation> const & team, LegalSelections const team_selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) -> MoveProbabilities {
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
	auto const scores = expectiminimax(
		team,
		team_selections,
		other,
		other_selections,
		environment,
		evaluate,
		depth
	);
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

#define INSTANTIATION(generation) \
	template auto predict_action(Team<generation> const & team, LegalSelections const team_selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> evaluate, Depth const depth) -> MoveProbabilities

TM_FOR_EACH_GENERATION(INSTANTIATION);

} // namespace technicalmachine
