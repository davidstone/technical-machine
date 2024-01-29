// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.evaluate.predict_selection;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.expectiminimax;
import tm.evaluate.predicted;
import tm.evaluate.scored_selection;
import tm.evaluate.state;

import tm.move.legal_selections;
import tm.move.selection;

import tm.environment;
import tm.generation;
import tm.team;
import tm.team_is_empty;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto predict_selection(Team<generation> const & team, LegalSelections const selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) -> AllPredicted {
	auto all_equally_likely = [&] {
		auto const possible_moves = double(containers::size(selections));
		return AllPredicted(containers::transform(
			selections,
			[=](Selection const selection) { return Predicted(selection, 1.0 / possible_moves); }
		));
	};
	if (containers::size(selections) == 1_bi) {
		return all_equally_likely();
	}
	auto const scores = expectiminimax(
		State<generation>(team, other, environment, depth),
		selections,
		other_selections,
		evaluate
	);
	// TODO: this is not the right way to weight move scores
	auto const score_only = containers::transform(scores, &ScoredSelection::score);
	auto const min_value = *containers::min_element(score_only);
	auto adjust_score = [=](double const score) {
		return std::pow(score - min_value, 3.0);
	};
	auto const total_score = containers::sum(containers::transform(score_only, adjust_score));
	if (total_score == 0.0) {
		return all_equally_likely();
	}
	return AllPredicted(containers::filter(
		containers::transform(
			scores,
			[=](ScoredSelection const scored) {
				return Predicted(scored.selection, adjust_score(scored.score) / total_score);
			}
		),
		[](Predicted const predicted) {
			return predicted.probability > 0.0;
		}
	));
}

#define INSTANTIATION(generation) \
	template auto predict_selection(Team<generation> const & team, LegalSelections selections, Team<generation> const & other, LegalSelections other_selections, Environment environment, Evaluate<generation> evaluate, Depth depth) -> AllPredicted

TM_FOR_EACH_GENERATION(INSTANTIATION);

} // namespace technicalmachine
