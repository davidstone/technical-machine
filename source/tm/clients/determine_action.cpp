// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.determine_action;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.action_probability;
import tm.evaluate.score_actions;
import tm.evaluate.scored_action;
import tm.evaluate.state;

import tm.move.action;
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
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto log_action(std::ostream & stream, Action const action, Team<generation> const & team) {
	tv::visit(action, tv::overload(
		[&](MoveName const move) {
			if (!is_switch(move)) {
				stream << "Use " << to_string(move);
			} else {
				stream << "Switch to " << to_string(team.pokemon(to_replacement(move)).species());
			}
		},
		[](UnusedSwitch) {
		}
	));
}

template<Generation generation>
auto log_move_scores(
	std::ostream & stream,
	ScoredActions const scored_actions,
	Team<generation> const & ai
) -> void {
	for (auto const sa : scored_actions) {
		log_action(stream, sa.action, ai);
		stream << " for an expected score of " << static_cast<std::int64_t>(sa.score) << '\n';
	}
}

template<Generation generation>
auto log_foe_move_probabilities(
	std::ostream & stream,
	ActionProbabilities const action_probabilities,
	Team<generation> const & foe
) -> void {
	for (auto const ap : action_probabilities) {
		stream << "Predicted " << ap.probability * 100.0 << "% chance: ";
		log_action(stream, ap.action, foe);
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
) -> Action {
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
	containers::sort(foe_moves, [](ActionProbability const lhs, ActionProbability const rhs) {
		return lhs.probability > rhs.probability;
	});
	log_foe_move_probabilities(stream, foe_moves, state.foe);
	stream << std::flush;

	auto scored_actions = score_actions(
		state,
		ai_selections,
		foe_moves,
		evaluate
	);
	auto const finish = std::chrono::steady_clock::now();
	stream << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
	containers::sort(scored_actions, [](ScoredAction const lhs, ScoredAction const rhs) {
		return lhs.score > rhs.score;
	});
	log_move_scores(stream, scored_actions, state.ai);
	stream << std::flush;
	return containers::front(scored_actions).action;
}

export auto determine_action(
	GenerationGeneric<VisibleState> const & generic_state,
	std::ostream & stream,
	UsageStats const & usage_stats,
	GenerationGeneric<Evaluate> const generic_evaluate,
	Depth const depth
) -> Action {
	return tv::visit(generic_state, generic_evaluate, tv::overload(
		[&]<Generation generation>(VisibleState<generation> const & state, Evaluate<generation> const evaluate) -> Action {
			return determine_action(
				state,
				stream,
				usage_stats,
				evaluate,
				depth
			);
		},
		[](auto const &, auto) -> Action {
			std::unreachable();
		}
	));
}

} // namespace technicalmachine
