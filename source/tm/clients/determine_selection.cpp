// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.determine_selection;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.predicted;
import tm.evaluate.score_selections;
import tm.evaluate.scored_selection;
import tm.evaluate.state;

import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.team;

import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.get_both_selections;
import tm.team;
import tm.visible_state;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
auto log_selection(std::ostream & stream, Selection const selection, Team<generation> const & team) {
	tv::visit(selection, tv::overload(
		[&](Switch const switch_) {
			stream << "Switch to " << to_string(team.pokemon(switch_.value()).species());
		},
		[&](MoveName const move) {
			stream << "Use " << to_string(move);
		},
		[&](Pass) {
			stream << "Pass";
		}
	));
}

template<Generation generation>
auto log_move_scores(
	std::ostream & stream,
	ScoredSelections const scored_selections,
	Team<generation> const & ai
) -> void {
	for (auto const scored : scored_selections) {
		log_selection(stream, scored.selection, ai);
		stream << " for an expected score of " << static_cast<std::int64_t>(scored.score) << '\n';
	}
}

template<Generation generation>
auto log_foe_move_probabilities(
	std::ostream & stream,
	AllPredicted const all_predicted,
	Team<generation> const & foe
) -> void {
	for (auto const predicted : all_predicted) {
		stream << "Predicted " << predicted.probability * 100.0 << "% chance: ";
		log_selection(stream, predicted.selection, foe);
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
auto determine_selection(
	VisibleState<generation> const & visible,
	std::ostream & stream,
	UsageStats const & usage_stats,
	Evaluate<generation> const evaluate,
	Depth const depth
) -> Selection {
	if (visible.ai.size() == 0_bi or visible.foe.size() == 0_bi) {
		throw std::runtime_error("Tried to determine a selection with an empty team.");
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

	auto [foe_moves, ai_selections] = get_both_selections(
		state.foe,
		state.ai,
		state.environment,
		evaluate
	);
	containers::sort(foe_moves, [](Predicted const lhs, Predicted const rhs) {
		return lhs.probability > rhs.probability;
	});
	log_foe_move_probabilities(stream, foe_moves, state.foe);
	stream << std::flush;

	auto scored_selections = score_selections(
		state,
		ai_selections,
		foe_moves,
		evaluate
	);
	auto const finish = std::chrono::steady_clock::now();
	stream << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
	containers::sort(scored_selections, [](ScoredSelection const lhs, ScoredSelection const rhs) {
		return lhs.score > rhs.score;
	});
	log_move_scores(stream, scored_selections, state.ai);
	stream << std::flush;
	return containers::front(scored_selections).selection;
}

export auto determine_selection(
	GenerationGeneric<VisibleState> const & generic_state,
	std::ostream & stream,
	UsageStats const & usage_stats,
	GenerationGeneric<Evaluate> const generic_evaluate,
	Depth const depth
) -> Selection {
	return tv::visit(generic_state, generic_evaluate, tv::overload(
		[&]<Generation generation>(VisibleState<generation> const & state, Evaluate<generation> const evaluate) -> Selection {
			return determine_selection(
				state,
				stream,
				usage_stats,
				evaluate,
				depth
			);
		},
		[](auto const &, auto) -> Selection {
			std::unreachable();
		}
	));
}

} // namespace technicalmachine
