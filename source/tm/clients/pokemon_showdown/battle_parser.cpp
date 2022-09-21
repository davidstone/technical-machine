// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/battle_parser.hpp>

#include <tm/clients/pokemon_showdown/chat.hpp>

#include <tm/move/move_name.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move_name.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/weather.hpp>

#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>
#include <bounded/overload.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/index_type.hpp>
#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
namespace ps {

using namespace std::string_view_literals;

namespace {

constexpr auto party_from_player_id(std::string_view const player_id) {
	return parse_identity(player_id).party;
}

constexpr auto parse_status(std::string_view const str) {
	return
		(str == "") ? StatusName::clear :
		(str == "fnt") ? StatusName::clear :
		(str == "brn") ? StatusName::burn :
		(str == "frz") ? StatusName::freeze :
		(str == "par") ? StatusName::paralysis :
		(str == "psn") ? StatusName::poison :
		(str == "tox") ? StatusName::toxic :
		(str == "slp") ? StatusName::sleep :
//		(str == "") ? StatusName::rest :
		throw std::runtime_error("Received an invalid status");
}

struct HPAndStatus {
	VisibleHP hp;
	StatusName status;
};

constexpr auto parse_hp_and_status(std::string_view const hp_and_status) {
	auto const [hp_fraction, status] = split_view(hp_and_status, ' ');
	return HPAndStatus{parse_hp(hp_fraction), parse_status(status)};
}

struct FromMove {};
struct MainEffect {};
struct FromConfusion {};
struct FromMiscellaneous {};
struct FromRecoil {};
struct FromSubstitute {};

using EffectSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous, FromRecoil, FromSubstitute>;

constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) -> EffectSource {
	return
		(type == "") ? EffectSource(MainEffect{}) :
		(type == "item") ? EffectSource(from_string<Item>(source)) :
		(type == "ability") ? EffectSource(from_string<Ability>(source)) :
		(type == "move") ? EffectSource(FromMove{}) :
		(type == "confusion") ? EffectSource(FromConfusion{}) :
		(type == "Recoil") ? EffectSource(FromRecoil{}) :
		(type == "Substitute") ? EffectSource(FromSubstitute{}) :
		(
			type == "brn" or
			type == "psn" or
			type == "tox" or
			type == "drain" or
			type == "perish3" or
			type == "perish2" or
			type == "perish1" or
			type == "perish0" or
			type == "trapped" or
			type == "Encore" or
			type == "Hail" or
			type == "Leech Seed" or
			type == "Light Screen" or
			type == "Protect" or // Includes Detect
			type == "Spikes" or
			type == "Stealth Rock" or
			type == "Reflect" or
			type == "Sandstorm"
		) ? EffectSource(FromMiscellaneous{}) :
		throw std::runtime_error(containers::concatenate<std::string>(std::string_view("Unhandled effect source type: "), type));
}

constexpr auto parse_from_source(std::string_view const message) -> EffectSource {
	// [from]
	auto const [bracketed_text_or_nothing, remainder] = split_view(message, ' ');
	auto const [type, source] = split_view(remainder, ':');
	return parse_effect_source(type, source);
}

constexpr auto parse_hp_message(InMessage message) {
	struct Message {
		Party party;
		VisibleHP hp;
		StatusName status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message.pop());
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

constexpr auto parse_set_hp_message(InMessage message) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct Message {
		Party party;
		VisibleHP hp;
		StatusName status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message.pop());

	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

} // namespace

auto BattleParser::handle_message(InMessage message) -> bounded::optional<containers::string> {
	m_battle_logger.log(message);

	if (handle_chat_message(message)) {
		return bounded::none;
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
		bounded::visit(source, bounded::overload(
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
		return bounded::visit(parse_effect_source(category, source), bounded::overload(
			[](MainEffect) -> bounded::optional<containers::string> {
				throw std::runtime_error("Unexpected -activate source MainEffect");
			},
			[](FromConfusion) -> bounded::optional<containers::string> { return bounded::none; },
			[](FromMiscellaneous) -> bounded::optional<containers::string> { return bounded::none; },
			[](FromMove) -> bounded::optional<containers::string> { return bounded::none; },
			[](FromRecoil) -> bounded::optional<containers::string> {
				throw std::runtime_error("Unexpected -activate source FromRecoil");
			},
			[&](FromSubstitute) -> bounded::optional<containers::string> {
				if (details == "[damage]") {
					m_move_state.damage_substitute(other(party));
					// TODO: Why
					return handle_delayed_switch(other(party));
				}
				return bounded::none;
			},
			[&](Ability const ability) -> bounded::optional<containers::string> {
				set_value_on_pokemon(party, ability);
				switch (ability) {
					case Ability::Forewarn:
						m_battle_manager->active_has(
							!is_ai(party),
							from_string<MoveName>(details)
						);
						break;
					case Ability::Shed_Skin:
						m_end_of_turn_state.shed_skin(party);
						break;
					default:
						break;
				}
				return bounded::none;
			},
			[&](Item const item) -> bounded::optional<containers::string> {
				set_value_on_pokemon(party, item);
				return bounded::none;
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
			return bounded::none;
		}
		auto const party = party_from_player_id(message.pop());
		auto const status = parse_status(message.pop());
		auto const source = parse_from_source(message.pop());
		bounded::visit(source, bounded::overload(
			[&](MainEffect) {
				if (m_battle_manager->is_end_of_turn()) {
					m_end_of_turn_state.set_expected(party, StatusName::clear);
					return;
				}
				auto const move_name = m_move_state.executed_move();
				auto const move_cured_status = move_name and (
					m_move_state.party() == party or
					m_battle_manager->cures_target_status(is_ai(party), *move_name)
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
		auto const maybe_replace = [&] {
			if (!m_battle_manager->is_end_of_turn()) {
				return true;
			}
			return std::exchange(m_already_replaced_fainted_end_of_turn, true);
		};
		auto const requires_response =
			m_battle_manager->generation() <= Generation::three and
			is_ai(party) and
			!m_battle_manager->ai_is_on_last_pokemon() and
			maybe_replace();
		maybe_use_previous_move();
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
		auto const parsed = parse_hp_message(message);
		auto const party = parsed.party;
		bounded::visit(parsed.source, bounded::overload(
			[](MainEffect) {},
			[](FromConfusion) { throw std::runtime_error("Confusion cannot heal"); },
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
		bounded::visit(source, bounded::overload(
			// TODO: Validate that the type should be immune
			[](MainEffect) {},
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause immunity"); },
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
		if (is_ai(party) and move == MoveName::Baton_Pass and !m_battle_manager->ai_is_on_last_pokemon()) {
			maybe_use_previous_move();
			return move_response(m_battle_manager->determine_action());
		}
	} else if (type == "-mustrecharge") {
		// After moves like Hyper Beam
	} else if (type == "-notarget") {
		// When you use a move, but there is no one to target
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
		bounded::insert(switch_, Switch{parsed.move, parsed.hp, parsed.status});
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
		bounded::visit(source, bounded::overload(
			[](MainEffect) { throw std::runtime_error("Unexpected -start source MainEffect"); },
			[&](FromConfusion) {
				auto const how = message.pop();
				if (how == "[fatigue]") {
					m_end_of_turn_state.lock_in_ends(party);
				} else {
					m_move_state.confuse(other(party));
				}
			},
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
		bounded::visit(source, bounded::overload(
			[&](MainEffect) { m_move_state.status_from_move(party, status); },
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
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
		m_already_replaced_fainted_end_of_turn = false;
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
		bool const ai_went_first = end_of_turn_state.first_party == m_ai_party;
		m_battle_manager->end_turn(ai_went_first, end_of_turn_state.first.flags, end_of_turn_state.last.flags);
		m_battle_manager->weather_is(end_of_turn_state.weather);
		try_correct_hp_and_status(ai_went_first, end_of_turn_state.first.hp, end_of_turn_state.first.status);
		try_correct_hp_and_status(!ai_went_first, end_of_turn_state.last.hp, end_of_turn_state.last.status);
		if (m_battle_manager->ai_is_fainted()) {
			m_replacing_fainted = true;
			return move_response(m_battle_manager->determine_action());
		}
	} else if (type == "-weather") {
		auto const weather = from_string<NormalWeather>(message.pop());
		m_end_of_turn_state.active_weather(weather);
		[[maybe_unused]] auto const from_or_upkeep_or_nothing = message.pop(' ');
		auto const category = message.pop(": "sv);
		auto const source = message.pop();
		bounded::visit(parse_effect_source(category, source), bounded::overload(
			[&](Ability const ability) {
				[[maybe_unused]] auto const of = message.pop(' ');
				auto const party = party_from_player_id(message.pop());
				maybe_commit_switch(party);
				set_value_on_pokemon(party, ability);
			},
			[](auto) { }
		));
	} else if (type == "win") {
		auto const won = m_username == message.pop();
		m_battle_manager->complete(won ? Result::won : Result::lost);
	} else {
		std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
	}
	return bounded::none;
}

auto BattleParser::handle_damage(InMessage message) -> bounded::optional<containers::string> {
	auto const parsed = parse_hp_message(message);
	auto move_damage = [&](Party const party) {
		m_move_state.damage(party, parsed.hp);
	};
	auto const party = parsed.party;
	maybe_commit_switch(party);
	auto response = bounded::visit(parsed.source, bounded::overload(
		[&](MainEffect) -> bounded::optional<containers::string> {
			if (m_move_state.move_damaged_self(party)) {
				return bounded::none;
			}
			move_damage(other(party));
			return handle_delayed_switch(other(party));
		},
		[&](FromConfusion) -> bounded::optional<containers::string> {
			// TODO: Technically you cannot select Hit Self, you just execute
			// it. This matters for things like priority or determining whether
			// Sucker Punch succeeds. As a workaround for now, say the user
			// selected Struggle.
			// When hitting self in confusion, we get -activate then -damage
			maybe_use_previous_move();
			m_move_state.use_move(party, MoveName::Struggle);
			m_move_state.use_move(party, MoveName::Hit_Self);
			move_damage(party);
			return bounded::none;
		},
		[](FromMiscellaneous) -> bounded::optional<containers::string> { return bounded::none; },
		[](FromMove) -> bounded::optional<containers::string> { return bounded::none; },
		[&](FromRecoil) -> bounded::optional<containers::string> {
			m_move_state.recoil(party);
			return bounded::none;
		},
		[](FromSubstitute) -> bounded::optional<containers::string> {
			return bounded::none;
		},
		[&](auto const value) -> bounded::optional<containers::string> {
			set_value_on_pokemon(party, value);
			return bounded::none;
		}
	));
	queue_hp_or_status_checks(party, parsed.hp);
	queue_hp_or_status_checks(party, parsed.status);
	return response;
}

auto BattleParser::handle_switch_or_drag(InMessage message) -> SwitchOrDragResult {
	auto parsed = parse_switch(message);
	auto const data_is_for_ai = is_ai(parsed.party);
	auto const move = [&] {
		if (data_is_for_ai) {
			auto const result = m_battle_manager->ai_has(
				parsed.species,
				std::move(parsed).nickname,
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
				std::move(parsed).nickname,
				parsed.level,
				parsed.gender
			);
		}
	}();

	return SwitchOrDragResult{parsed.party, parsed.species, move, parsed.hp, parsed.status};
}

auto BattleParser::set_value_on_pokemon(Party const party, auto const value) -> void {
	auto const for_ai = is_ai(party);
	if (auto const replacing = m_move_state.switch_index()) {
		m_battle_manager->replacement_has(for_ai, *replacing, value);
	} else {
		m_battle_manager->active_has(for_ai, value);
	}
}

auto BattleParser::queue_hp_or_status_checks(Party const party, auto const value) -> void {
	if (m_move_state.party()) {
		m_move_state.set_expected(party, value);
	} else {
		m_end_of_turn_state.set_expected(party, value);
	}
}

auto BattleParser::maybe_commit_switch(Party const party) -> void {
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
	switch_ = bounded::none;
}
auto BattleParser::maybe_use_previous_move() -> void {
	maybe_use_previous_move_impl();
	maybe_commit_switch(m_ai_party);
	maybe_commit_switch(other(m_ai_party));
	maybe_use_previous_move_impl();
}
auto BattleParser::maybe_use_previous_move_impl() -> void {
	auto const maybe_data = m_move_state.complete();
	if (!maybe_data) {
		return;
	}
	auto & data = *maybe_data;
	auto const data_is_for_ai = data.party == m_ai_party;
	m_battle_manager->use_move(data_is_for_ai, data.move, data.user_status_was_cleared); 
	try_correct_hp_and_status(data_is_for_ai, data.user.hp, data.user.status);
	try_correct_hp_and_status(!data_is_for_ai, data.other.hp, data.other.status);
}

auto BattleParser::try_correct_hp_and_status(bool const is_ai, bounded::optional<VisibleHP> const hp, bounded::optional<StatusName> const status, auto... maybe_index) -> void {
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

namespace {

auto move_response_impl(std::string_view const id, bool const is_switch, auto const switch_move, auto const move_index) -> containers::string {
	return containers::concatenate<containers::string>(
		id,
		is_switch ?
			containers::concatenate_view("|/switch "sv, to_string(switch_move())) :
			containers::concatenate_view("|/move "sv, to_string(move_index() + 1_bi))
	);
}

} // namespace

auto BattleParser::move_response(MoveName const move) const -> containers::string {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	auto switch_move = [&]{ return m_slot_memory[to_replacement(move)]; };
	auto move_index = [&]{ return m_battle_manager->move_index(move); };
	return move_response_impl(id(), is_switch(move), switch_move, move_index);
}

auto BattleParser::random_move_response() -> containers::string {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	auto distribution = std::uniform_int_distribution(0, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team - 1_bi));
	auto const result = distribution(m_battle_manager->random_engine());

	auto switch_move = [=]{ return bounded::assume_in_range<TeamIndex>(result - max_moves_per_pokemon); };
	auto move_index = [=]{ return bounded::assume_in_range<containers::index_type<RegularMoves>>(result); };
	auto const is_switch = result >= max_moves_per_pokemon;
	return move_response_impl(id(), is_switch, switch_move, move_index);
}

auto BattleParser::handle_delayed_switch(Party const party) -> bounded::optional<containers::string> {
	if (!is_ai(party)) {
		return bounded::none;
	}
	auto const executed_move = m_move_state.executed_move();
	if (!executed_move) {
		return bounded::none;
	}
	// TODO: What is the correct check here
	if (!is_delayed_switch(*executed_move) or m_battle_manager->foe_is_fainted() or m_battle_manager->ai_is_on_last_pokemon()) {
		return bounded::none;
	}
	maybe_use_previous_move();
	return move_response(m_battle_manager->determine_action());
}

auto parse_details(std::string_view const details) -> ParsedDetails {
	auto parser = DelimitedBufferView(details, std::string_view(", "));
	auto const species = from_string<Species>(parser.pop());

	auto const level_or_gender_or_shiny_str = parser.pop();
	auto const has_level = !level_or_gender_or_shiny_str.empty() and level_or_gender_or_shiny_str.front() == 'L';
	auto const level = Level(has_level ?
		bounded::to_integer<Level::value_type>(level_or_gender_or_shiny_str.substr(1)) :
		100_bi
	);

	auto const gender_or_shiny_str = has_level ? parser.pop() : level_or_gender_or_shiny_str;
	auto const gender = 
		BOUNDED_CONDITIONAL(gender_or_shiny_str == "F", Gender::female,
		BOUNDED_CONDITIONAL(gender_or_shiny_str == "M", Gender::male,
		Gender::genderless
	));

	auto const shiny_str = gender != Gender::genderless ? parser.pop() : gender_or_shiny_str;
	auto throw_exception = [&] {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid PS details string: "sv, details));
	};
	if (shiny_str != "shiny" and shiny_str != "") {
		throw_exception();
	}
	if (!parser.remainder().empty()) {
		throw_exception();
	}
	return ParsedDetails{species, level, gender};
}

auto parse_switch(InMessage message) -> ParsedSwitch {
	auto const identity = parse_identity(message.pop());
	auto const details = parse_details(message.pop());
	auto const [visible_hp, status] = parse_hp_and_status(message.pop());
	return ParsedSwitch{identity.party, details.species, identity.nickname, details.level, details.gender, visible_hp, status};
}

} // namespace ps
} // namespace technicalmachine
