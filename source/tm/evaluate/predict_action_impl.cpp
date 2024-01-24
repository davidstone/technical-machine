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
import tm.evaluate.action_probability;
import tm.evaluate.scored_action;
import tm.evaluate.state;

import tm.move.action;
import tm.move.legal_selections;

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
auto predict_action(Team<generation> const & team, LegalSelections const selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) -> ActionProbabilities {
	auto all_equally_likely = [&] {
		auto const possible_moves = double(containers::size(selections));
		return ActionProbabilities(containers::transform(
			selections,
			[=](Action const action) { return ActionProbability(action, 1.0 / possible_moves); }
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
	auto const score_only = containers::transform(scores, &ScoredAction::score);
	auto const min_value = *containers::min_element(score_only);
	auto adjust_score = [=](double const score) {
		return std::pow(score - min_value, 3.0);
	};
	auto const total_score = containers::sum(containers::transform(score_only, adjust_score));
	if (total_score == 0.0) {
		return all_equally_likely();
	}
	return ActionProbabilities(containers::filter(
		containers::transform(
			scores,
			[=](ScoredAction const sa) {
				return ActionProbability(sa.action, adjust_score(sa.score) / total_score);
			}
		),
		[](ActionProbability const ap) { return ap.probability > 0.0; }
	));
}

#define INSTANTIATION(generation) \
	template auto predict_action(Team<generation> const & team, LegalSelections selections, Team<generation> const & other, LegalSelections other_selections, Environment environment, Evaluate<generation> evaluate, Depth depth) -> ActionProbabilities

TM_FOR_EACH_GENERATION(INSTANTIATION);

} // namespace technicalmachine
