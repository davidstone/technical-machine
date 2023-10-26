// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.clients.ps.battle_parser;

import tm.clients.ps.battle_interface;
import tm.clients.ps.chat;
import tm.clients.ps.end_of_turn_state;
import tm.clients.ps.inmessage;
import tm.clients.ps.make_party;
import tm.clients.ps.move_state;
import tm.clients.ps.parse_details;
import tm.clients.ps.parse_effect_source;
import tm.clients.ps.parse_set_hp_message;
import tm.clients.ps.parse_status;
import tm.clients.ps.parse_switch;
import tm.clients.ps.party_from_player_id;
import tm.clients.ps.slot_memory;

import tm.clients.battle_manager;
import tm.clients.make_battle_manager;
import tm.clients.make_battle_manager_inputs;
import tm.clients.party;
import tm.clients.result;
import tm.clients.turn_count;
import tm.clients.write_team;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.move.is_switch;
import tm.move.legal_selections;
import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.status.clears_status;
import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.weather;

import tm.team_predictor.usage_stats;

import tm.ability;
import tm.buffer_view;
import tm.gender;
import tm.generation;
import tm.item;
import tm.visible_hp;
import tm.weather;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

auto move_response_impl(std::string_view const id, bool const is_switch, auto const switch_move, auto const move_index) -> containers::string {
	return containers::concatenate<containers::string>(
		id,
		is_switch ?
			containers::concatenate_view("|/choose switch "sv, containers::to_string(switch_move())) :
			containers::concatenate_view("|/choose move "sv, containers::to_string(move_index() + 1_bi))
	);
}

export struct BattleParser final : BattleInterface {
	BattleParser(
		AnalysisLogger analysis_logger,
		tv::optional<WriteTeam> write_team,
		containers::string id_,
		containers::string username,
		UsageStats const & usage_stats,
		GenerationGeneric<BattleManagerInputs> inputs,
		Party party,
		Depth const depth,
		std::mt19937 random_engine
	):
		m_slot_memory(tv::visit(inputs, [](auto const & i) { return i.teams.ai.size(); })),
		m_battle_manager(make_battle_manager(
			std::move(analysis_logger),
			std::move(write_team),
			usage_stats,
			std::move(inputs),
			depth,
			std::move(random_engine)
		)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_party(party)
	{
	}

	auto handle_message(InMessage message) -> tv::optional<containers::string> final {
		if (handle_chat_message(message)) {
			return tv::none;
		}

		// Documented at
		// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
		// under the section "Battle progress"
		auto const type = message.type();
		if (type == "") {
			maybe_use_previous_move();
		} else if (type == "-ability") {
			auto const party = party_from_player_id(message.pop());
			auto const ability = from_string<Ability>(message.pop());
			auto const source = parse_from_source(message.pop());
			maybe_commit_switch(party);
			tv::visit(source, tv::overload(
				[&](MainEffect) { set_value_on_pokemon(party, ability); },
				[&](Ability const source_ability) {
					switch (source_ability) {
						case Ability::Trace:
							set_value_on_pokemon(party, source_ability);
							set_value_on_pokemon(other(party), ability);
							break;
						default:
							throw std::runtime_error("Invalid ability to get another ability");
					}
				},
				[](auto) { throw std::runtime_error("Invalid ability source"); }
			));
		} else if (type == "-activate") {
			auto const party = party_from_player_id(message.pop());
			auto const [category, source] = split_view(message.pop(), ": "sv);
			auto const details = message.pop();
			maybe_commit_switch(party);
			return tv::visit(parse_effect_source(category, source), tv::overload(
				[](MainEffect) -> tv::optional<containers::string> {
					throw std::runtime_error("Unexpected -activate source MainEffect");
				},
				[](FromConfusion) -> tv::optional<containers::string> { return tv::none; },
				[](FromEntryHazards) -> tv::optional<containers::string> { throw std::runtime_error("Unexpected -activate source FromEntryHazards"); },
				[](FromMiscellaneous) -> tv::optional<containers::string> { return tv::none; },
				[](FromMove) -> tv::optional<containers::string> { return tv::none; },
				[](FromRecoil) -> tv::optional<containers::string> {
					throw std::runtime_error("Unexpected -activate source FromRecoil");
				},
				[&](FromSubstitute) -> tv::optional<containers::string> {
					if (details == "[damage]") {
						m_move_state.damage_substitute(other(party));
						// TODO: Why
						return handle_delayed_switch(other(party));
					}
					return tv::none;
				},
				[&](Ability const ability) -> tv::optional<containers::string> {
					set_value_on_pokemon(party, ability);
					switch (ability) {
						case Ability::Forewarn:
							m_battle_manager->active_has(
								party != m_party,
								from_string<MoveName>(details)
							);
							break;
						case Ability::Shed_Skin:
							m_end_of_turn_state.shed_skin(party);
							break;
						default:
							break;
					}
					return tv::none;
				},
				[&](Item const item) -> tv::optional<containers::string> {
					set_value_on_pokemon(party, item);
					return tv::none;
				}
			));
		} else if (type == "-anim") {
	#if 0
			auto const party = party_from_player_id(message.pop());
			// Solar Beam is the only example I have of this
			auto const move? = message.pop();
			auto const target? = message.pop();
	#endif
		} else if (type == "-boost") {
	#if 0
			auto const pokemon = message.pop();
			auto const stat = message.pop();
			auto const amount = message.pop();
	#endif
		} else if (type == "cant") {
			maybe_use_previous_move();
			// TODO: Figure out how to solve this in general...
			auto const party = party_from_player_id(message.pop());
			auto const reason = message.pop();
			if (reason == "flinch") {
				m_move_state.flinch(party);
			} else if (reason == "Focus Punch") {
			} else if (reason == "frz") {
				m_move_state.frozen_solid(party);
			} else if (reason == "par") {
				m_move_state.fully_paralyze(party);
			} else if (reason == "slp") {
				m_move_state.still_asleep(party);
			} else if (reason == "recharge") {
				m_move_state.recharge(party);
			} else {
				std::cerr << "Received unknown \"cant\" reason: " << reason;
				if (!message.remainder().empty()) {
					std::cerr << '|' << message.remainder();
				}
				std::cerr << '\n';
			}
		} else if (type == "-center") {
		} else if (type == "-clearallboost") {
			// We already know what Haze does
		} else if (type == "-crit") {
			auto const user = other(party_from_player_id(message.pop()));
			m_move_state.critical_hit(user);
		} else if (type == "-curestatus") {
			if (m_ignore_next_cure_status) {
				m_ignore_next_cure_status = false;
				return tv::none;
			}
			auto const party = party_from_player_id(message.pop());
			auto const status = parse_status(message.pop());
			auto const source = parse_from_source(message.pop());
			tv::visit(source, tv::overload(
				[&](MainEffect) {
					if (m_battle_manager->is_end_of_turn()) {
						if (status == StatusName::freeze) {
							m_end_of_turn_state.thaw(party);
						} else {
							m_end_of_turn_state.set_expected(party, StatusName::clear);
						}
						return;
					}
					auto const move_name = m_move_state.executed_move();
					auto const move_cured_status = move_name and (
						m_move_state.party() == party or
						m_battle_manager->cures_target_status(party == m_party, *move_name)
					);
					if (move_cured_status) {
						m_move_state.status_from_move(party, StatusName::clear);
					} else {
						maybe_use_previous_move();
						switch (status) {
							case StatusName::freeze:
								m_move_state.thaw(party);
								break;
							case StatusName::sleep:
								m_move_state.awaken(party, m_battle_manager->generation());
								break;
							default:
								throw std::runtime_error("Spontaneously recovered from status");
						}

					}
				},
				[&](Ability const ability) {
					set_value_on_pokemon(party, ability);
					switch (ability) {
						case Ability::Natural_Cure:
							break;
						case Ability::Shed_Skin:
							m_end_of_turn_state.set_expected(party, StatusName::clear);
							break;
						default:
							throw std::runtime_error("Unexpected ability cured status");
					}
				},
				[](auto) { throw std::runtime_error("Unexpected -curestatus source"); }
			));
		} else if (type == "-cureteam") {
	#if 0
			auto const pokemon = message.pop();
	#endif
		} else if (type == "-damage") {
			return handle_damage(message);
		} else if (type == "detailschange" or type == "-formechange") {
	#if 0
			auto const pokemon = message.pop();
			// message.remainder() == DETAILS|HP STATUS
	#endif
		} else if (type == "-end") {
			// Tells me if the substitute is destroyed, probably other stuff ending
			auto const party = party_from_player_id(message.pop());
			auto const source = message.pop();
			if (source == "Substitute") {
				m_move_state.break_substitute(other(party));
				return handle_delayed_switch(other(party));
			}
		} else if (type == "-endability") {
	#if 0
			auto const pokemon = message.pop();
			auto const ability = message.pop();
	#endif
		} else if (type == "-enditem") {
			auto const party = party_from_player_id(message.pop());
			auto const item = from_string<Item>(message.pop());
			set_value_on_pokemon(party, item);
			for (auto const status : containers::enum_range<StatusName>()) {
				if (clears_status(item, status)) {
					m_ignore_next_cure_status = true;
					queue_hp_or_status_checks(party, StatusName::clear);
					break;
				}
			}
		} else if (type == "error") {
			if (message.remainder() != "[Invalid choice] There's nothing to choose") {
				return random_move_response();
			}
		} else if (type == "-fail") {
	#if 0
			auto const party = party_from_player_id(message.pop());
			// There can be more to this message, but nothing we care about
	#endif
		} else if (type == "faint") {
			auto const party = party_from_player_id(message.pop());
			auto const from_entry_hazards = std::exchange(m_replacement_fainted_from_entry_hazards, false);
			auto const requires_response =
				m_battle_manager->generation() <= Generation::three and
				party == m_party and
				!m_battle_manager->ai_is_on_last_pokemon() and
				(!m_battle_manager->is_end_of_turn() or from_entry_hazards);
			if (from_entry_hazards or party != m_move_state.party()) {
				maybe_use_previous_move();
			}
			if (requires_response) {
				m_replacing_fainted = true;
				return move_response(m_battle_manager->determine_action());
			}
		} else if (type == "-fieldactivate") {
			// move: Perish Song
		} else if (type == "-fieldend") {
	#if 0
			// Grassy Terrain, Gravity, Trick Room
			auto const condition = message.pop();
	#endif
		} else if (type == "-fieldstart") {
	#if 0
			// Grassy Terrain, Gravity, Trick Room
			auto const condition = message.pop();
	#endif
		} else if (type == "-heal") {
			auto const parsed = parse_set_hp_message(message);
			auto const party = parsed.party;
			tv::visit(parsed.source, tv::overload(
				[](MainEffect) {},
				[](FromConfusion) { throw std::runtime_error("Confusion cannot heal"); },
				[](FromEntryHazards) { throw std::runtime_error("Entry hazards cannot heal"); },
				[](FromMiscellaneous) {},
				[](FromMove) {},
				[](FromRecoil) { throw std::runtime_error("Recoil cannot heal"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot heal"); },
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
			queue_hp_or_status_checks(party, parsed.hp);
			queue_hp_or_status_checks(party, parsed.status);
		} else if (type == "-hint") {
			// message.remainder() == MESSAGE
		} else if (type == "-hitcount") {
			auto const party = party_from_player_id(message.pop());
			auto const count = bounded::to_integer<2, 5>(message.pop());
			static_cast<void>(party);
			static_cast<void>(count);
			// TODO: Implement multi-hit moves
		} else if (type == "-immune") {
			auto const party = party_from_player_id(message.pop());
			auto const source = parse_from_source(message.pop());
			tv::visit(source, tv::overload(
				// TODO: Validate that the type should be immune
				[](MainEffect) {},
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause immunity"); },
				[](FromEntryHazards) { throw std::runtime_error("Entry hazards cannot cause immunity"); },
				[](FromMiscellaneous) { throw std::runtime_error("Miscellaneous effects cannot cause immunity"); },
				[](FromMove) { throw std::runtime_error("MoveName cannot cause immunity"); },
				[](FromRecoil) { throw std::runtime_error("Recoil cannot cause immunity"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot cause immunity"); },
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
		} else if (type == "inactive") {
			// Pokemon Showdown consistently sends
			// |inactive|Time left: X sec this turn | Y sec total
			// when it is the user's turn to make a decision. However, this message
			// arrives only if the timer is on. Technical Machine turns on the timer
			// by default, but I think I want to support playing without a timer, so
			// I do not use this easy trigger (the only consistent trigger to know
			// when the server is expecting a response).
			// message.remainder() == MESSAGE
		} else if (type == "inactiveoff") {
			// message.remainder() == MESSAGE
			// Timer is off
		} else if (type == "-item") {
			auto const party = party_from_player_id(message.pop());
			auto const item = from_string<Item>(message.pop());
			set_value_on_pokemon(party, item);
		} else if (type == "-mega") {
	#if 0
			auto const pokemon = message.pop();
			auto const megastone = message.pop();
	#endif
		} else if (type == "-message") {
		} else if (type == "-miss") {
			auto const user_party = party_from_player_id(message.pop());
			m_move_state.miss(user_party);
		} else if (type == "move") {
			auto const party = party_from_player_id(message.pop());
			auto const move = from_string<MoveName>(message.pop());
	#if 0
			// target is sent only for moves that target one Pokemon
			auto const target = message.pop();
	#endif
			if (m_move_state.party() != party) {
				maybe_use_previous_move();
			}
			m_move_state.use_move(party, move);
			if (party == m_party and move == MoveName::Baton_Pass and !m_battle_manager->ai_is_on_last_pokemon()) {
				maybe_use_previous_move();
				return move_response(m_battle_manager->determine_action());
			}
		} else if (type == "-mustrecharge") {
			// After moves like Hyper Beam
		} else if (type == "-notarget") {
			// When you use a move, but there is no one to target
		} else if (type == "-nothing") {
			// Splash
		} else if (type == "-prepare") {
			// From moves like SolarBeam on the charge turn
		} else if (type == "request") {
	#if 0
			auto const json = message.remainder();
	#endif
		} else if (type == "-resisted") {
			// message.remainder() == POKEMON
		} else if (type == "-setboost") {
			// Belly Drum
		} else if (type == "-sethp") {
			// TODO: This should verify that the received HP matches the actual
			// HP. The problem is that we tend to get this message too soon, so
			// we need to defer checking until some time later.
			auto const parsed = parse_set_hp_message(message);
			static_cast<void>(parsed);
		} else if (type == "-sideend") {
	#if 0
			// Light Screen, Reflect, etc.
			auto const side = message.pop();
			auto const condition = message.pop();
	#endif
		} else if (type == "-sidestart") {
	#if 0
			// Light Screen, Reflect, etc.
			auto const side = message.pop();
			auto const condition = message.pop();
	#endif
		} else if (type == "-supereffective") {
			// message.remainder() == POKEMON
		} else if (type == "switch") {
			if (!m_ai_switch and !m_foe_switch) {
				maybe_use_previous_move();
			}
			auto const parsed = handle_switch_or_drag(message);
			auto & switch_ = get_switch(parsed.party);
			if (switch_) {
				throw std::runtime_error("Tried to switch in the same Pokemon twice");
			}
			tv::insert(switch_, Switch{parsed.move, parsed.hp, parsed.status});
		} else if (type == "drag") {
			auto const parsed = handle_switch_or_drag(message);
			m_move_state.phazed_in(other(parsed.party), to_replacement(parsed.move));
			m_move_state.set_expected(parsed.party, parsed.hp);
			m_move_state.set_expected(parsed.party, parsed.status);
		} else if (type == "replace") {
	#if 0
			// Illusion ended
			auto const pokemon = message.pop();
			// message.remainder() == DETAILS|HP STATUS
	#endif
		} else if (type == "-singleturn") {
			// Received for things like Protect that last the rest of the turn
		} else if (type == "-start") {
			auto const party = party_from_player_id(message.pop());
			auto const first_part_of_source = message.pop();
			auto parse_remainder_of_source = [&] {
				if (first_part_of_source == "typechange") {
					auto const changed_type = message.pop();
					static_cast<void>(changed_type);
					return parse_from_source(message.pop());
				} else {
					auto const [source_type, string_source] = split_view(first_part_of_source, ':');
					return parse_effect_source(source_type, string_source);
				}
			};
			auto const source = parse_remainder_of_source();
			tv::visit(source, tv::overload(
				[](MainEffect) { throw std::runtime_error("Unexpected -start source MainEffect"); },
				[&](FromConfusion) {
					auto const how = message.pop();
					if (how == "[fatigue]") {
						m_end_of_turn_state.lock_in_ends(party);
					} else {
						m_move_state.confuse(other(party));
					}
				},
				[](FromEntryHazards) {},
				[](FromMiscellaneous) {},
				[](FromMove) {},
				[](FromRecoil) { throw std::runtime_error("Unexpected -start source FromRecoil"); },
				[](FromSubstitute) {},
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
		} else if (type == "-status") {
			auto const party = party_from_player_id(message.pop());
			auto const status = parse_status(message.pop());
			auto const source = parse_from_source(message.pop());
			maybe_commit_switch(party);
			tv::visit(source, tv::overload(
				[&](MainEffect) { m_move_state.status_from_move(party, status); },
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
				[](FromEntryHazards) { },
				[&](FromMiscellaneous) { m_move_state.status_from_move(other(party), status); },
				[&](FromMove) { m_move_state.status_from_move(party, status); },
				[](FromRecoil) { throw std::runtime_error("Recoil cannot cause another status"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot cause another status"); },
				[&](Ability const ability) {
					set_value_on_pokemon(other(party), ability);
					m_move_state.status_from_ability(party, ability, status);
				},
				[&](Item const item) {
					set_value_on_pokemon(party, item);
					m_move_state.set_expected(party, status);
				}
			));
		} else if (type == "swap") {
	#if 0
			auto const pokemon = message.pop();
			auto const position = message.pop();
			// position here is "0", "1", or "2"
	#endif
		} else if (type == "t:") {
			// message.remainder() == Seconds since 1970
		} else if (type == "tie") {
			m_battle_manager->complete(Result::tied);
		} else if (type == "-transform") {
			// message.remainder() == POKEMON|SPECIES
		} else if (type == "turn") {
			maybe_use_previous_move();
			m_replacement_fainted_from_entry_hazards = false;
			auto const turn_count = bounded::to_integer<TurnCount>(message.pop());
			m_battle_manager->begin_turn(turn_count);
			return move_response(m_battle_manager->determine_action());
		} else if (type == "-unboost") {
	#if 0
			auto const pokemon = message.pop();
			auto const stat = message.pop();
			auto const amount = message.pop();
	#endif
		} else if (type == "upkeep") {
			auto const end_of_turn_state = m_end_of_turn_state.complete();
			// TODO: If there is no party, we have no information on relative
			// ordering, so saying the user did or did not go first could mess
			// up future code that attempts to determine relative speed.
			bool const ai_went_first = end_of_turn_state.first_party == m_party;
			m_battle_manager->end_turn(ai_went_first, end_of_turn_state.first.flags, end_of_turn_state.last.flags);
			m_battle_manager->weather_is(end_of_turn_state.weather);
			try_correct_hp_and_status(ai_went_first, end_of_turn_state.first.hp, end_of_turn_state.first.status);
			try_correct_hp_and_status(!ai_went_first, end_of_turn_state.last.hp, end_of_turn_state.last.status);
			if (m_battle_manager->ai_is_fainted()) {
				m_replacing_fainted = true;
				return move_response(m_battle_manager->determine_action());
			}
		} else if (type == "-weather") {
			auto const weather = from_string<Weather>(message.pop());
			m_end_of_turn_state.active_weather(weather);
			[[maybe_unused]] auto const from_or_upkeep_or_nothing = message.pop(' ');
			auto const category = message.pop(": "sv);
			auto const source = message.pop();
			tv::visit(parse_effect_source(category, source), tv::overload(
				[&](Ability const ability) {
					[[maybe_unused]] auto const of = message.pop(' ');
					auto const party = party_from_player_id(message.pop());
					maybe_commit_switch(party);
					set_value_on_pokemon(party, ability);
				},
				[](auto) { }
			));
		} else if (type == "win") {
			auto const winning_username = message.pop();
			m_battle_manager->complete(winning_username == m_username ? Result::won : Result::lost);
		} else {
			std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
		}
		return tv::none;
	}

	auto id() const -> std::string_view final {
		return m_id;
	}
	auto completed() const -> BattleInterface::Complete final {
		return m_battle_manager->completed() ? BattleInterface::Complete::finish : BattleInterface::Complete::none;
	}

private:
	auto handle_damage(InMessage message) -> tv::optional<containers::string> {
		auto const parsed = parse_set_hp_message(message);
		auto move_damage = [&](Party const party) {
			m_move_state.damage(party, parsed.hp);
		};
		auto const party = parsed.party;
		maybe_commit_switch(party);
		auto response = tv::visit(parsed.source, tv::overload(
			[&](MainEffect) -> tv::optional<containers::string> {
				if (m_move_state.move_damaged_self(party)) {
					return tv::none;
				}
				move_damage(other(party));
				return handle_delayed_switch(other(party));
			},
			[&](FromConfusion) -> tv::optional<containers::string> {
				// TODO: Technically you cannot select Hit Self, you just execute
				// it. This matters for things like priority or determining whether
				// Sucker Punch succeeds. As a workaround for now, say the user
				// selected Struggle.
				// When hitting self in confusion, we get -activate then -damage
				maybe_use_previous_move();
				m_move_state.use_move(party, MoveName::Struggle);
				m_move_state.use_move(party, MoveName::Hit_Self);
				move_damage(party);
				return tv::none;
			},
			[&](FromEntryHazards) -> tv::optional<containers::string> {
				if (parsed.hp.current.value() == 0_bi) {
					m_replacement_fainted_from_entry_hazards = true;
				}
				return tv::none;
			},
			[](FromMiscellaneous) -> tv::optional<containers::string> { return tv::none; },
			[](FromMove) -> tv::optional<containers::string> { return tv::none; },
			[&](FromRecoil) -> tv::optional<containers::string> {
				m_move_state.recoil(party);
				return tv::none;
			},
			[](FromSubstitute) -> tv::optional<containers::string> {
				return tv::none;
			},
			[&](auto const value) -> tv::optional<containers::string> {
				set_value_on_pokemon(party, value);
				return tv::none;
			}
		));
		queue_hp_or_status_checks(party, parsed.hp);
		queue_hp_or_status_checks(party, parsed.status);
		return response;
	}

	struct SwitchOrDragResult {
		Party party;
		Species species;
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto handle_switch_or_drag(InMessage message) -> SwitchOrDragResult {
		auto parsed = parse_switch(message);
		auto const data_is_for_ai = parsed.party == m_party;
		auto const move = [&] {
			if (data_is_for_ai) {
				auto const result = m_battle_manager->ai_has(
					parsed.species,
					parsed.nickname,
					parsed.level,
					parsed.gender
				);
				auto const switch_index = to_replacement(result);
				if (m_replacing_fainted) {
					m_slot_memory.replace_fainted(switch_index);
					m_replacing_fainted = false;
				} else {
					m_slot_memory.switch_to(switch_index);
				}
				return result;
			} else {
				return m_battle_manager->foe_has(
					parsed.species,
					parsed.nickname,
					parsed.level,
					parsed.gender
				);
			}
		}();

		return SwitchOrDragResult{parsed.party, parsed.species, move, parsed.hp, parsed.status};
	}

	auto set_value_on_pokemon(Party const party, auto const value) -> void {
		auto const for_ai = party == m_party;
		if (auto const replacing = m_move_state.switch_index()) {
			m_battle_manager->replacement_has(for_ai, *replacing, value);
		} else {
			m_battle_manager->active_has(for_ai, value);
		}
	}

	auto queue_hp_or_status_checks(Party const party, auto const value) -> void {
		if (m_move_state.party()) {
			m_move_state.set_expected(party, value);
		} else {
			m_end_of_turn_state.set_expected(party, value);
		}
	}

	struct Switch {
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto get_switch(Party const party) -> tv::optional<Switch> & {
		return party == m_party ? m_ai_switch : m_foe_switch;
	}

	auto maybe_commit_switch(Party const party) -> void {
		auto & switch_ = get_switch(party);
		if (!switch_) {
			return;
		}
		auto const move_party = m_move_state.party();
		if (move_party and *move_party != party) {
			maybe_use_previous_move_impl();
		}
		m_move_state.use_move(party, switch_->move);
		m_move_state.set_expected(party, switch_->hp);
		m_move_state.set_expected(party, switch_->status);
		switch_ = tv::none;
	}
	auto maybe_use_previous_move() -> void {
		maybe_use_previous_move_impl();
		maybe_commit_switch(m_party);
		maybe_commit_switch(other(m_party));
		maybe_use_previous_move_impl();
	}
	auto maybe_use_previous_move_impl() -> void {
		auto const maybe_data = m_move_state.complete();
		if (!maybe_data) {
			return;
		}
		auto & data = *maybe_data;
		auto const data_is_for_ai = data.party == m_party;
		m_battle_manager->use_move(data_is_for_ai, data.move, data.user_status_was_cleared); 
		try_correct_hp_and_status(data_is_for_ai, data.user.hp, data.user.status);
		try_correct_hp_and_status(!data_is_for_ai, data.other.hp, data.other.status);
	}

	auto try_correct_hp_and_status(bool const is_ai, tv::optional<VisibleHP> const hp, tv::optional<StatusName> const status, auto... maybe_index) -> void {
		if (hp) {
			m_battle_manager->correct_hp(is_ai, *hp, maybe_index...);
			if (hp->current == CurrentVisibleHP(0_bi)) {
				return;
			}
		}
		if (status) {
			m_battle_manager->correct_status(is_ai, *status, maybe_index...);
		}
	}

	auto move_response(MoveName const move) const -> containers::string {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto switch_move = [&]{ return m_slot_memory[to_replacement(move)]; };
		auto move_index = [&]{ return m_battle_manager->move_index(move); };
		return move_response_impl(id(), is_switch(move), switch_move, move_index);
	}
	auto random_move_response() -> containers::string {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto distribution = std::uniform_int_distribution<int>(0, static_cast<int>(maximum_possible_selections));
		auto const result = distribution(m_battle_manager->random_engine());

		auto switch_move = [=]{ return bounded::assume_in_range<TeamIndex>(result - max_moves_per_pokemon); };
		auto move_index = [=]{ return bounded::assume_in_range<containers::index_type<RegularMoves>>(result); };
		auto const is_switch = result >= max_moves_per_pokemon;
		return move_response_impl(id(), is_switch, switch_move, move_index);
	}

	auto handle_delayed_switch(Party const party) -> tv::optional<containers::string> {
		if (party != m_party) {
			return tv::none;
		}
		auto const executed_move = m_move_state.executed_move();
		if (!executed_move) {
			return tv::none;
		}
		// TODO: What is the correct check here
		if (!is_delayed_switch(*executed_move) or m_battle_manager->foe_is_fainted() or m_battle_manager->ai_is_on_last_pokemon()) {
			return tv::none;
		}
		maybe_use_previous_move();
		return move_response(m_battle_manager->determine_action());
	}

	SlotMemory m_slot_memory;
	std::unique_ptr<BattleManager> m_battle_manager;

	containers::string m_id;
	containers::string m_username;

	Party m_party;
	MoveState m_move_state;
	EndOfTurnState m_end_of_turn_state;

	tv::optional<Switch> m_ai_switch;
	tv::optional<Switch> m_foe_switch;

	bool m_ignore_next_cure_status = false;
	bool m_replacing_fainted = false;
	bool m_replacement_fainted_from_entry_hazards = false;
};

} // namespace technicalmachine::ps
