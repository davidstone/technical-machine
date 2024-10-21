// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

module tm.clients.ps.battle_message_handler;

import tm.clients.ps.end_of_turn_state;
import tm.clients.ps.end_of_turn_state_builder;
import tm.clients.ps.move_state;
import tm.clients.ps.move_state_builder;
import tm.clients.ps.parsed_message;
import tm.clients.ps.slot_memory;
import tm.clients.ps.switch_message;

import tm.clients.client_battle;
import tm.clients.make_client_battle;
import tm.clients.party;
import tm.clients.teams;
import tm.clients.turn_count;

import tm.move.move_name;
import tm.move.switch_;

import tm.pokemon.max_pokemon_per_team;

import tm.status.status_name;
import tm.status.toxic_resets_on_switch;

import tm.ability;
import tm.generation;
import tm.generation_generic;
import tm.visible_hp;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;

BattleMessageHandler::BattleMessageHandler(Party party, GenerationGeneric<Teams> teams):
	m_slot_memory(tv::visit(teams, [](auto const & t) { return t.ai.size(); })),
	m_client_battle(make_client_battle(std::move(teams))),
	m_party(party)
{
}

struct Nothing {
};

// Double switches first report that both Pokemon are switching, then it gives
// information about each switch.
using Switches = containers::static_vector<SwitchState, 2_bi>;

constexpr auto find_switch(auto & switches, Party const party) {
	return containers::maybe_find_if(switches, [=](SwitchState const s) {
		return s.party == party;
	});
}

// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
auto BattleMessageHandler::handle_message(std::span<ParsedMessage const> const block) -> Result {
	using ActionBuilder = tv::variant<
		Nothing,
		MoveStateBuilder,
		Switches,
		HitSelf
	>;
	auto action_builder = ActionBuilder(Nothing());
	auto end_of_turn_state = EndOfTurnStateBuilder();

	auto set_value_on_pokemon = [&](Party const party, auto const value) -> void {
		auto const for_ai = party == m_party;
		auto const index = tv::visit(action_builder, tv::overload(
			[](MoveStateBuilder const & move_builder) -> tv::optional<TeamIndex> {
				return move_builder.phaze_index();
			},
			[=](Switches const switches) -> tv::optional<TeamIndex> {
				auto const ptr = find_switch(switches, party);
				return BOUNDED_CONDITIONAL(ptr, ptr->index, tv::none);
			},
			[](auto) -> tv::optional<TeamIndex> {
				return tv::none;
			}
		));
		if (index) {
			m_client_battle->replacement_has(for_ai, *index, value);
		} else {
			m_client_battle->active_has(for_ai, value);
		}
	};

	auto const do_action = tv::overload(
		[](Nothing) {},
		[&](MoveStateBuilder & builder) -> void {
			if (auto const move_state = builder.complete()) {
				use_move(*move_state);
			}
		},
		[&](Switches const switches) {
			// TODO: Timing for both sides replacing a fainting Pokemon
			for (auto const switch_ : containers::reversed(switches)) {
				use_switch(switch_);
			}
		},
		[&](HitSelf const hit_self) {
			hit_self_in_confusion(hit_self);
		}
	);
	auto use_previous_action = [&] -> void {
		tv::visit(action_builder, do_action);
		action_builder = Nothing();
	};

	auto get_move_builder = [&] -> MoveStateBuilder & {
		return tv::visit(action_builder, tv::overload(
			[](MoveStateBuilder & builder) -> MoveStateBuilder & {
				return builder;
			},
			[](auto) -> MoveStateBuilder & {
				throw std::runtime_error("Tried to get a MoveStateBuilder from the wrong state.");
			}
		));
	};

	auto make_move_builder = [&] -> MoveStateBuilder & {
		BOUNDED_ASSERT(action_builder.index() == bounded::type<Nothing>);
		return action_builder.emplace(bounded::construct<MoveStateBuilder>);
	};

	struct Unknown {
	};
	auto result = tv::variant<Unknown, TurnCount, BattleFinished>(Unknown());

	for (auto const element : block) {
		tv::visit(element, tv::overload(
			[&](SeparatorMessage) {
				// This is either the first message or a transition to end of turn
				use_previous_action();
			},
			[&](AbilityMessage const message) {
				set_value_on_pokemon(message.party, message.ability);
				if (message.other_ability) {
					set_value_on_pokemon(other(message.party), *message.other_ability);
				}
			},
			[&](ForewarnMessage const message) {
				set_value_on_pokemon(message.party, Ability::Forewarn);
				m_client_battle->active_has(message.party != m_party, message.other_move);
			},
			[&](ShedSkinMessage const message) {
				set_value_on_pokemon(message.party, Ability::Shed_Skin);
				require_is_end_of_turn();
				end_of_turn_state.shed_skin(message.party);
			},
			[&](ItemMessage const message) {
				set_value_on_pokemon(message.party, message.item);
			},
			[&](DamageSubstituteMessage const message) {
				get_move_builder().damage_substitute(other(message.party));
			},
			[&](DestroySubstituteMessage const message) {
				get_move_builder().break_substitute(other(message.party));
			},
			[&](FlinchMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.flinch(message.party);
			},
			[](FocusPunchMessage) {
				// TODO
			},
			[&](FrozenSolidMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.frozen_solid(message.party);
			},
			[&](FullyParalyzedMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.fully_paralyze(message.party);
			},
			[&](PartiallyTrappedMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.partial_trap(message.party);
			},
			[&](StillAsleepMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.still_asleep(message.party);
			},
			[&](RechargingMessage const message) {
				use_previous_action();
				auto & move_builder = make_move_builder();
				move_builder.recharge(message.party);
			},
			[&](CriticalHitMessage const message) {
				get_move_builder().critical_hit(message.party);
			},
			[&](StatusClearMessage const message) {
				auto const party = message.party;
				if (m_client_battle->is_end_of_turn()) {
					if (message.status == StatusName::freeze) {
						end_of_turn_state.thaw(party);
					} else {
						end_of_turn_state.set_expected(party, StatusName::clear);
					}
				} else {
					auto natural_status_recovery = [&] {
						switch (message.status) {
							case StatusName::freeze:
								use_previous_action();
								make_move_builder().thaw(party);
								break;
							case StatusName::sleep:
								use_previous_action();
								make_move_builder().awaken(party, m_client_battle->generation());
								break;
							default:
								throw std::runtime_error("Spontaneously recovered from status");
						}
					};
					tv::visit(action_builder, tv::overload(
						[&](MoveStateBuilder & builder) {
							auto const move_name = builder.executed_move();
							auto const move_cured_status = move_name and (
								builder.party() == party or
								m_client_battle->cures_target_status(party == m_party, *move_name)
							);
							if (move_cured_status) {
								builder.status_from_move(party, StatusName::clear);
							} else {
								do_action(builder);
								natural_status_recovery();
							}
						},
						[&](auto const previous) {
							do_action(previous);
							natural_status_recovery();
						}
					));
				}
			},
			[&](DamageMessage const message) {
				auto const party = message.party;
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						if (builder.move_damaged_self(party)) {
							return;
						}
						builder.damage(other(party), message.hp);
						builder.set_expected(party, message.status);
						builder.set_expected(party, message.hp);
					},
					[](auto) {
						throw std::runtime_error("Unattached damage message");
					}
				));
			},
			[&](HitSelfMessage const message) {
				use_previous_action();
				tv::visit(action_builder, tv::overload(
					[&](Nothing) {
						action_builder.emplace([&] {
							return HitSelf(message.party, message.status, message.hp);
						});
					},
					[](auto) {
						throw std::runtime_error("Tried to hit self and take another action");
					}
				));
			},
			[&](RecoilMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						builder.recoil(message.party);
						builder.set_expected(message.party, message.status);
						builder.set_expected(message.party, message.hp);
					},
					[](auto) {
						throw std::runtime_error("Unattached recoil message");
					}
				));
			},
			[](ConfusionEndedMessage) {
				// TODO
			},
			[](TauntEndedMessage) {
				// TODO
			},
			[&](HPMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						builder.set_expected(message.party, message.status);
						builder.set_expected(message.party, message.hp);
					},
					[&](Switches & switches) {
						auto const ptr = find_switch(switches, message.party);
						if (!ptr) {
							throw std::runtime_error("Invalid switch for HP Message");
						}
						ptr->status = message.status;
						ptr->hp = message.hp;
					},
					[&](auto const previous) {
						do_action(previous);
						require_is_end_of_turn();
						end_of_turn_state.set_expected(message.party, message.status);
						end_of_turn_state.set_expected(message.party, message.hp);
					}
				));
			},
			[&](MoveMessage const message) {
				use_previous_action();
				make_move_builder().use_move(message.party, message.move, message.miss);
			},
			[](EffectivenessMessage) {
				// TODO
			},
			[](ScreenEndMessage) {
				// TODO
			},
			[&](SwitchMessage const message) {
				auto & switches = tv::visit(action_builder, tv::overload(
					[&](Switches & s) -> Switches & {
						if (find_switch(s, message.party)) {
							throw std::runtime_error("Tried to switch in the same Pokemon twice");
						}
						return s;
					},
					[&](auto & previous) -> Switches & {
						do_action(previous);
						return action_builder.emplace(bounded::construct<Switches>);
					}
				));
				auto const index = handle_switch_message(message);
				containers::emplace_back(
					switches,
					message.party,
					index,
					message.status,
					message.hp
				);
			},
			[&](PhazeMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						auto const index = handle_switch_message(message);
						builder.phazed_in(other(message.party), index);
						builder.set_expected(message.party, message.status);
						builder.set_expected(message.party, message.hp);
					},
					[](auto) {
						throw std::runtime_error("Received phaze information without a phaze move");
					}
				));
			},
			[&](RampageEndMessage const message) {
				require_is_end_of_turn();
				end_of_turn_state.lock_in_ends(message.party);
			},
			[&](StartConfusionMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						// TODO: Rampage moves?
						builder.confuse(other(message.party));
					},
					[](auto) {
						throw std::runtime_error("Start confusion without a move");
					}
				));
			},
			[&](MoveStatus const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						builder.status_from_move(message.party, message.status);
					},
					[&](Switches & switches) {
						auto const ptr = find_switch(switches, message.party);
						if (!ptr) {
							throw std::runtime_error("Status changed from switching without a switch");
						}
						if (message.status != StatusName::poison) {
							throw std::runtime_error("Status changed from switching and it wasn't poison");
						}
						if (!toxic_resets_on_switch(m_client_battle->generation())) {
							throw std::runtime_error("Toxic poison changed to poison from switching in later generations");
						}
						ptr->status = message.status;
					},
					[](auto) {
						throw std::runtime_error("Move status without a move");
					}
				));
			},
			[&](AbilityStatusMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](MoveStateBuilder & builder) {
						set_value_on_pokemon(message.party, message.ability);
						builder.status_from_ability(
							other(message.party),
							message.ability,
							message.status
						);
					},
					[](auto) {
						throw std::runtime_error("Unattached ability status message");
					}
				));
			},
			[&](TurnMessage const message) {
				tv::visit(action_builder, tv::overload(
					[&](Nothing) {
						if (m_client_battle->is_end_of_turn()) {
							handle_end_of_turn(end_of_turn_state.complete());
						}
					},
					[](MoveStateBuilder) {
						throw std::runtime_error("Should not have a move state builder at the start of a turn");
					},
					[&](Switches const switches) {
						do_action(switches);
						action_builder = Nothing();
					},
					[](HitSelf) {
						throw std::runtime_error("Should not be hitting self at the start of a turn");
					}
				));
				action_builder = Nothing();
				result = message.count;
			},
			[&](WeatherMessage const message) {
				require_is_end_of_turn();
				end_of_turn_state.active_weather(message.weather);
			},
			[&](BattleFinishedMessage) {
				result = BattleFinished();
			}
		));
	}
	auto handle_previous_data = [&] {
		use_previous_action();
		if (m_client_battle->is_end_of_turn()) {
			handle_end_of_turn(end_of_turn_state.complete());
		}
	};
	return tv::visit(result, tv::overload(
		[&](Unknown) -> Result {
			handle_previous_data();
			auto state = m_client_battle->state();
			auto const is_finished = tv::visit(state, [](auto const & s) {
				return team_is_empty(s.ai) or team_is_empty(s.foe);
			});
			if (is_finished) {
				return BattleFinished();
			}
			return ActionRequired(std::move(state), m_slot_memory);
		},
		[&](TurnCount const turn_count) -> Result {
			handle_previous_data();
			return StartOfTurn(
				ActionRequired(m_client_battle->state(), m_slot_memory),
				turn_count
			);
		},
		[](BattleFinished) -> Result {
			return BattleFinished();
		}
	));
}

auto BattleMessageHandler::use_move(MoveState const data) -> void {
	auto const data_is_for_ai = data.party == m_party;
	m_client_battle->use_move(data_is_for_ai, data.move, data.user_status_was_cleared); 
	try_correct_hp_and_status(data_is_for_ai, data.user.hp, data.user.status);
	try_correct_hp_and_status(!data_is_for_ai, data.other.hp, data.other.status);
}

auto BattleMessageHandler::use_switch(SwitchState const data) -> void {
	auto const data_is_for_ai = data.party == m_party;
	m_client_battle->use_switch(data_is_for_ai, Switch(data.index)); 
	try_correct_hp_and_status(data_is_for_ai, data.hp, data.status);
}

auto BattleMessageHandler::hit_self_in_confusion(HitSelf const data) -> void {
	auto const data_is_for_ai = data.party == m_party;
	m_client_battle->hit_self_in_confusion(data_is_for_ai, data.hp); 
	try_correct_hp_and_status(data_is_for_ai, data.hp, data.status);
}

auto BattleMessageHandler::handle_switch_message(SwitchMessage const message) -> TeamIndex {
	if (message.party == m_party) {
		auto const index = m_client_battle->ai_has(
			message.species,
			message.nickname,
			message.level,
			message.gender
		);
		if (m_client_battle->ai_is_fainted()) {
			m_slot_memory.replace_fainted(index);
		} else {
			m_slot_memory.switch_to(index);
		}
		return index;
	} else {
		return m_client_battle->foe_has(
			message.species,
			message.nickname,
			message.level,
			message.gender,
			message.hp.max
		);
	}
}

auto BattleMessageHandler::handle_end_of_turn(EndOfTurnState const data) -> void {
	// TODO: If there is no party, we have no information on relative ordering,
	// so saying the user did or did not go first could mess up future code that
	// attempts to determine relative speed.
	bool const ai_went_first = data.first_party == m_party;
	m_client_battle->end_turn(ai_went_first, data.first.flags, data.last.flags);
	m_client_battle->weather_is(data.weather);
	try_correct_hp_and_status(ai_went_first, data.first.hp, data.first.status);
	try_correct_hp_and_status(!ai_went_first, data.last.hp, data.last.status);
}

auto BattleMessageHandler::try_correct_hp_and_status(bool const is_ai, tv::optional<VisibleHP> const hp, tv::optional<StatusName> const status) -> void {
	if (hp) {
		m_client_battle->correct_hp(is_ai, *hp);
		if (hp->current == CurrentVisibleHP(0_bi)) {
			return;
		}
	}
	if (status) {
		m_client_battle->correct_status(is_ai, *status);
	}
}

} // namespace technicalmachine::ps
