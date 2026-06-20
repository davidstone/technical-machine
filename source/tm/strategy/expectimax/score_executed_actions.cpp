// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.score_executed_actions;

import tm.evaluate.depth;
import tm.evaluate.score;
import tm.evaluate.selector;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.known_move;
import tm.move.move_name;
import tm.move.other_action;
import tm.move.pass;
import tm.move.selection;
import tm.move.side_effects;
import tm.move.switch_;
import tm.move.used_move;

import tm.pokemon.get_hidden_power_type;

import tm.stat.chance_to_hit;

import tm.status.status_name;

import tm.strategy.expectimax.moved;

import tm.type.move_type;

import tm.contact_ability_effect;
import tm.critical_hit_probability;
import tm.generation;
import tm.probability;
import tm.state;
import tm.team;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

struct SelectedAndExecuted {
	MoveName selected;
	MoveName executed;
};

constexpr auto paralysis_probability(StatusName const status) -> Probability {
	return status == StatusName::paralysis ? Probability(0.25) : Probability(0.0);
}

template<Generation generation>
auto execute_move(
	State<generation> const & state,
	Selector const select,
	SelectedAndExecuted const move,
	OtherAction const other_action,
	Depth const depth,
	auto const continuation
) -> Score {
	auto const selected = select(state);
	auto const user_pokemon = selected.selection.pokemon();
	auto const other_pokemon = selected.other.pokemon();
	auto const side_effects = possible_side_effects(move.executed, user_pokemon, selected.other, state.environment);
	auto const status = user_pokemon.status();
	auto const probability_of_clearing_status = status.probability_of_clearing(generation, user_pokemon.ability());
	auto const specific_chance_to_hit = chance_to_hit(
		user_pokemon,
		KnownMove(
			move.executed,
			move_type(generation, move.executed, get_hidden_power_type(user_pokemon))
		),
		other_pokemon,
		state.environment,
		moved(other_pokemon)
	);
	auto const ch_probability = critical_hit_probability(user_pokemon, move.executed, other_pokemon.ability(), state.environment);
	auto const chance_to_be_paralyzed = paralysis_probability(user_pokemon.status().name());
	auto const action_end_probability = user_pokemon.last_used_move().action_end_probability();
	return generic_flag_branch(probability_of_clearing_status, [&](bool const clear_status) {
		return generic_flag_branch(specific_chance_to_hit, [&](bool const hits) {
			return generic_flag_branch(chance_to_be_paralyzed, [&](bool const is_fully_paralyzed) {
				return generic_flag_branch(action_end_probability, [&](bool const action_ends) {
					return containers::sum(containers::transform(
						side_effects,
						[&](auto const & side_effect) {
							return side_effect.probability * generic_flag_branch(
								hits ? ch_probability : Probability(0.0),
								[&](bool const critical_hit) {
									auto copy = state;
									auto const selected_copy = select(copy);
									// TODO: https://github.com/davidstone/technical-machine/issues/24
									constexpr auto contact_ability_effect = ContactAbilityEffect::nothing;
									call_move(
										selected_copy.selection,
										UsedMove<Team<generation>>(
											move.selected,
											move.executed,
											critical_hit,
											!hits,
											action_ends,
											contact_ability_effect,
											side_effect.function
										),
										selected_copy.other,
										other_action,
										copy.environment,
										clear_status,
										ActualDamage::Unknown{},
										is_fully_paralyzed
									);
									if (auto const won = win(copy.ai, copy.foe)) {
										return *won + Score(double(depth.remaining_general()));
									}
									return continuation(copy);
								}
							);
						}
					));
				});
			});
		});
	});
}

export template<Generation generation>
auto score_executed_actions(
	State<generation> const & state,
	Selector const select,
	Selection const selection,
	OtherAction const other_action,
	Depth const depth,
	auto const continuation
) -> Score {
	return tv::visit(selection, tv::overload(
		[&](Switch const switch_) -> Score {
			return execute_switch(state, select, switch_, depth, continuation);
		},
		[&](MoveName const selected) -> Score {
			auto const & team = select(state).selection;
			if (team.pokemon().hp().current() == 0_bi) {
				return continuation(state);
			}
			auto const executed_moves = possible_executed_moves(selected, team);
			auto const sum = containers::sum(containers::transform(
				executed_moves,
				[&](MoveName const executed) {
					return execute_move(
						state,
						select,
						SelectedAndExecuted(selected, executed),
						other_action,
						depth,
						continuation
					);
				}
			));
			return sum / double(containers::size(executed_moves));
		},
		[&](Pass) -> Score {
			return continuation(state);
		}
	));
}

} // namespace technicalmachine
