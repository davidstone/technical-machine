// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.determine_action;

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
import tm.generation_generic;
import tm.get_both_actions;
import tm.team;
import tm.visible_state;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto log_move_scores(
	std::ostream & stream,
	ScoredMoves const moves,
	Team<generation> const & ai
) -> void {
	for (auto const move : moves) {
		if (is_switch(move.name)) {
			stream << "Switch to " << to_string(ai.pokemon(to_replacement(move.name)).species());
		} else {
			stream << "Use " << to_string(move.name);
		}
		stream << " for an expected score of " << static_cast<std::int64_t>(move.score) << '\n';
	}
}

template<Generation generation>
auto log_foe_move_probabilities(
	std::ostream & stream,
	MoveProbabilities const moves,
	Team<generation> const & foe
) -> void {
	for (auto const move : moves) {
		stream << "Predicted " << move.probability * 100.0 << "% chance of ";
		if (is_switch(move.name)) {
			stream << "switching to " << to_string(foe.pokemon(to_replacement(move.name)).species());
		} else {
			stream << "using " << to_string(move.name);
		}
		stream << '\n';
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

template<Generation generation>
auto determine_action(
	VisibleState<generation> const & visible,
	std::ostream & stream,
	UsageStats const & usage_stats,
	Evaluate<generation> const evaluate,
	Depth const depth
) -> MoveName {
	if (visible.ai.size() == 0_bi or visible.foe.size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}
	auto const state = predicted_state(visible, usage_stats, depth);

	auto log_team = [&](std::string_view const label, Team<generation> const & team) {
		stream << label << "'s " << to_string(team) << '\n';
	};
	log_team("AI", state.ai);
	log_team("Predicted Foe", state.foe);
	stream << std::flush;

	stream << "Evaluating to a depth of " << state.depth.general << ", " << state.depth.single << "...\n";
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
	log_foe_move_probabilities(stream, foe_moves, state.foe);
	stream << std::flush;

	auto scored_moves = score_moves(
		state,
		ai_selections,
		foe_moves,
		evaluate
	);
	auto const finish = std::chrono::steady_clock::now();
	stream << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
	containers::sort(scored_moves, [](ScoredMove const lhs, ScoredMove const rhs) {
		return lhs.score > rhs.score;
	});
	log_move_scores(stream, scored_moves, state.ai);
	stream << std::flush;
	return containers::front(scored_moves).name;
}

export auto determine_action(
	GenerationGeneric<VisibleState> const & generic_state,
	std::ostream & stream,
	UsageStats const & usage_stats,
	GenerationGeneric<Evaluate> const generic_evaluate,
	Depth const depth
) -> MoveName {
	return tv::visit(generic_state, generic_evaluate, tv::overload(
		[&]<Generation generation>(VisibleState<generation> const & state, Evaluate<generation> const evaluate) -> MoveName {
			return determine_action(
				state,
				stream,
				usage_stats,
				evaluate,
				depth
			);
		},
		[](auto const &, auto) -> MoveName {
			std::unreachable();
		}
	));
}

} // namespace technicalmachine
