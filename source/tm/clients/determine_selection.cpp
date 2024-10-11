// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.determine_selection;

import tm.evaluate.state;

import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.team;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.get_legal_selections;
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
auto log_move_probabilities(
	std::ostream & stream,
	WeightedSelections const all_predicted,
	Team<generation> const & team
) -> void {
	for (auto const predicted : all_predicted) {
		stream << '\t' << predicted.weight * 100.0 << "% chance: ";
		log_selection(stream, predicted.selection, team);
		stream << '\n';
	}
}

template<Generation generation>
auto predicted_state(
	VisibleState<generation> const & state,
	UsageStats const & usage_stats
) -> State<generation> {
	return State<generation>(
		Team<generation>(state.ai),
		most_likely_team(usage_stats, state.foe),
		state.environment
	);
}

constexpr auto sort_selections(WeightedSelections & selections) -> void {
	containers::sort(selections, [](WeightedSelection const lhs, WeightedSelection const rhs) {
		return lhs.weight > rhs.weight;
	});
}

template<Generation generation>
auto determine_selection(
	VisibleState<generation> const & visible,
	std::ostream & stream,
	UsageStats const & usage_stats,
	Strategy const & strategy,
	std::mt19937 & random_engine
) -> Selection {
	if (visible.ai.size() == 0_bi or visible.foe.size() == 0_bi) {
		throw std::runtime_error("Tried to determine a selection with an empty team.");
	}
	auto const state = predicted_state(visible, usage_stats);

	auto log_team = [&](std::string_view const label, Team<generation> const & team) {
		constexpr auto include_active_marker = true;
		auto const team_str = to_string(team, include_active_marker);
		stream << label << "'s " << team_str << '\n';
	};
	log_team("AI", state.ai);
	log_team("Predicted Foe", state.foe);
	stream << std::flush;

	auto const ai_selections = get_legal_selections(state.ai, state.foe, state.environment);
	auto const foe_selections = get_legal_selections(state.foe, state.ai, state.environment);

	auto const start = std::chrono::steady_clock::now();

	auto result = strategy(
		state.ai,
		ai_selections,
		state.foe,
		foe_selections,
		state.environment
	);

	sort_selections(result.predicted_other);
	stream << "Predicted:\n";
	log_move_probabilities(stream, result.predicted_other, state.foe);
	sort_selections(result.user);
	stream << "Use:\n";
	log_move_probabilities(stream, result.user, state.ai);

	auto const finish = std::chrono::steady_clock::now();
	stream << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds\n";
	stream << std::flush;

	return pick_selection(result.user, random_engine);
}

export auto determine_selection(
	GenerationGeneric<VisibleState> const & generic_state,
	std::ostream & stream,
	AllUsageStats const & all_usage_stats,
	Strategy const & strategy,
	std::mt19937 & random_engine
) -> Selection {
	auto const & usage_stats = all_usage_stats[get_generation(generic_state)];
	return tv::visit(
		generic_state,
		[&]<Generation generation>(VisibleState<generation> const & state) -> Selection {
			return determine_selection(
				state,
				stream,
				usage_stats,
				strategy,
				random_engine
			);
		}
	);
}

} // namespace technicalmachine
