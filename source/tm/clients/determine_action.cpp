// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.determine_action;

import tm.clients.visible_state;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.move_probability;
import tm.evaluate.score_moves;
import tm.evaluate.scored_move;
import tm.evaluate.state;

import tm.move.move_name;
import tm.move.is_switch;

import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.team;

import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.get_both_actions;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto log_move_scores(
	AnalysisLogger & logger,
	ScoredMoves const moves,
	Team<generation> const & ai
) -> void {
	for (auto const move : moves) {
		if (is_switch(move.name)) {
			logger << "Switch to " << to_string(ai.pokemon(to_replacement(move.name)).species());
		} else {
			logger << "Use " << to_string(move.name);
		}
		logger << " for an expected score of " << static_cast<std::int64_t>(move.score) << '\n';
	}
}

template<Generation generation>
auto log_foe_move_probabilities(
	AnalysisLogger & logger,
	MoveProbabilities const moves,
	Team<generation> const & foe
) -> void {
	for (auto const move : moves) {
		logger << "Predicted " << move.probability * 100.0 << "% chance of ";
		if (is_switch(move.name)) {
			logger << "switching to " << to_string(foe.pokemon(to_replacement(move.name)).species());
		} else {
			logger << "using " << to_string(move.name);
		}
		logger << '\n';
	}
}

template<Generation generation>
auto predicted_state(
	VisibleState<generation> const & state,
	UsageStats const & usage_stats,
	Depth const depth
) -> State<generation> {
	return State<generation>(
		Team<generation>(state.ai),
		most_likely_team(usage_stats, state.foe),
		state.environment,
		depth
	);
}

export template<Generation generation>
auto determine_action(
	VisibleState<generation> const & visible,
	AnalysisLogger & logger,
	UsageStats const & usage_stats,
	Evaluate<generation> const evaluate,
	Depth const depth
) -> MoveName {
	if (visible.ai.size() == 0_bi or visible.foe.size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}
	auto const state = predicted_state(visible, usage_stats, depth);

	auto log_team = [&](std::string_view const label, Team<generation> const & team) {
		logger << label << "'s " << to_string(team) << '\n';
	};
	log_team("AI", state.ai);
	log_team("Predicted Foe", state.foe);
	logger << std::flush;

	logger << "Evaluating to a depth of " << state.depth.general << ", " << state.depth.single << "...\n";
	auto const start = std::chrono::steady_clock::now();

	auto [foe_moves, ai_selections] = get_both_actions(
		state.foe,
		state.ai,
		state.environment,
		evaluate
	);
	containers::sort(foe_moves, [](MoveProbability const lhs, MoveProbability const rhs) {
		return lhs.probability > rhs.probability;
	});
	log_foe_move_probabilities(logger, foe_moves, state.foe);

	auto scored_moves = score_moves(
		state,
		ai_selections,
		foe_moves,
		evaluate
	);
	auto const finish = std::chrono::steady_clock::now();
	logger << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
	containers::sort(scored_moves, [](ScoredMove const lhs, ScoredMove const rhs) {
		return lhs.score > rhs.score;
	});
	log_move_scores(logger, scored_moves, state.ai);
	logger << std::flush;
	return containers::front(scored_moves).name;
}

} // namespace technicalmachine
