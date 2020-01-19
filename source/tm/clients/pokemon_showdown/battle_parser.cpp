// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>

#include <tm/clients/log_foe_team.hpp>

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/moves.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/team.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>
#include <bounded/detail/overload.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>

namespace technicalmachine {
namespace ps {

namespace {

constexpr auto party_from_player_id(std::string_view const player_id) {
	return make_party(player_id.substr(0, 2));
}

template<typename Container>
constexpr auto container_index(Container const & container, auto const value, std::string_view const text) {
	auto const it = containers::find(container, value);
	if (it == end(container)) {
		throw std::runtime_error(std::string(text) + std::string(to_string(value)));
	}
	return static_cast<containers::index_type<Container>>(it - begin(container));
}

auto get_move_index(Pokemon const & pokemon, Moves const move) {
	return container_index(all_moves(pokemon).regular(), move, "Pokemon does not know ");
}
auto get_species_index(Team const & team, Species const species) {
	return container_index(team.all_pokemon(), species, "Team does not have a ");
}

auto const & get_team(Battle const & battle, Party const party) {
	return battle.is_me(party) ? battle.ai() : battle.foe();
}

constexpr auto parse_status(std::string_view const str) {
	return
		(str == "") ? Statuses::clear :
		(str == "fnt") ? Statuses::clear :
		(str == "brn") ? Statuses::burn :
		(str == "frz") ? Statuses::freeze :
		(str == "par") ? Statuses::paralysis :
		(str == "psn") ? Statuses::poison :
		(str == "tox") ? Statuses::toxic :
		(str == "slp") ? Statuses::sleep :
//		(str == "") ? Statuses::rest :
		throw std::runtime_error("Received an invalid status");
}

constexpr auto parse_hp_and_status(std::string_view const hp_and_status) {
	auto const [hp_fraction, status] = split(hp_and_status, ' ');
	return HPAndStatus{ParsedHP(hp_fraction), parse_status(status)};
}

struct FromMove{};
struct MainEffect{};
struct FromConfusion{};
struct FromMiscellaneous{};
struct FromRecoil{};

using EffectSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous, FromRecoil>;

constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) {
	return
		(type == "") ? EffectSource(MainEffect{}) :
		(type == "item") ? EffectSource(from_string<Item>(source)) :
		(type == "ability") ? EffectSource(from_string<Ability>(source)) :
		(type == "move") ? EffectSource(FromMove{}) :
		(type == "confusion") ? EffectSource(FromConfusion{}) :
		(type == "Recoil") ? EffectSource(FromRecoil{}) :
		(
			type == "brn" or
			type == "psn" or
			type == "tox" or
			type == "drain" or
			type == "Encore" or
			type == "Leech Seed" or
			type == "Spikes" or
			type == "Stealth Rock" or
			type == "Hail" or
			type == "Sandstorm" or
			type == "Substitute"
		) ? EffectSource(FromMiscellaneous{}) :
		throw std::runtime_error("Unhandled effect source type: " + std::string(type));
}

auto parse_from_source(InMessage message) {
	// "[from]" or nothing
	auto const from [[maybe_unused]] = message.next(' ');
	auto const [type, source] = split(message.next(), ':');
	return parse_effect_source(type, source);
}

auto parse_hp_message(InMessage message) {
	struct Message {
		Party party;
		ParsedHP hp;
		Statuses status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.next());
	auto const hp_and_status = parse_hp_and_status(message.next());
	auto const source = parse_from_source(message);
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

auto parse_set_hp_message(InMessage message) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct MessagePokemon {
		Party party;
		ParsedHP hp;
		Statuses status;
	};
	struct Message {
		MessagePokemon pokemon1;
		MessagePokemon pokemon2;
		EffectSource source;
	};
	auto const party1 = party_from_player_id(message.next());
	auto const hp_and_status1 = parse_hp_and_status(message.next());
	auto const party2 = party_from_player_id(message.next());
	auto const hp_and_status2 = parse_hp_and_status(message.next());
	auto const source = parse_from_source(message);
	
	return Message{
		{party1, hp_and_status1.hp, hp_and_status1.status},
		{party2, hp_and_status2.hp, hp_and_status2.status},
		source
	};
}

struct AllowedHP {
	HP::current_type min;
	HP::current_type value;
	HP::current_type max;
};

auto to_real_hp(bool const is_me, HP const actual_hp, ParsedHP const visible_hp) {
	if (visible_hp.current == 0_bi) {
		return AllowedHP{0_bi, 0_bi, 0_bi};
	}
	constexpr auto max_visible_foe_hp = 100_bi;
	auto const max_hp = actual_hp.max();
	auto const expected_max_visible_hp = BOUNDED_CONDITIONAL(is_me, max_hp, max_visible_foe_hp);
	if (expected_max_visible_hp != visible_hp.max) {
		throw std::runtime_error("Received an invalid max HP. Expected: " + bounded::to_string(expected_max_visible_hp) + " but got " + bounded::to_string(visible_hp.max));
	}
	auto compute_value = [=](auto const visible_current) {
		return HP::current_type(bounded::max(1_bi, max_hp * visible_current / visible_hp.max));
	};
	if (is_me) {
		auto const value = compute_value(visible_hp.current);
		return AllowedHP{value, value, value};
	} else {
		// TODO: Put in correct bounds on this for the foe Pokemon
		return AllowedHP{
			compute_value(visible_hp.current - 1_bi),
			compute_value(visible_hp.current),
			compute_value(visible_hp.current + 1_bi)
		};
	}
}

auto correct_hp(HP const original_hp, bool const is_me, ParsedHP const visible_hp) {
	auto const current_hp = original_hp.current();
	auto const seen_hp = to_real_hp(is_me, original_hp, visible_hp);
	if (seen_hp.min > current_hp or seen_hp.max < current_hp) {
		// TODO: Find a better way to sync this up with server messages. Find a
		// better way to fail unit tests if this happens.
		std::cerr << "HP out of sync with server messages\n";
	}
	return seen_hp.value;
}

void validate_status(Statuses const original_status, Statuses const visible_status) {
	auto const normalized_original_status = (original_status == Statuses::rest) ? Statuses::sleep : original_status;
	if (normalized_original_status != visible_status) {
		throw std::runtime_error(
			"Status out of sync with server messages: expected " +
			std::string(to_string(original_status)) +
			" but received " +
			std::string(to_string(visible_status))
		);
	}
}

// TODO: What happens here if a Pokemon has a pinch item?
void correct_hp_and_status(bool const is_me, Pokemon & pokemon, HPAndStatus const hp_and_status) {
	auto const [visible_hp, visible_status] = hp_and_status;
	auto const original_hp = get_hp(pokemon);
	pokemon.set_hp(correct_hp(original_hp, is_me, visible_hp));
	if (visible_hp.current == 0_bi) {
		return;
	}
	validate_status(get_status(pokemon).name(), visible_status);
}

} // namespace


void BattleParser::handle_message(InMessage message) {
	m_battle_logger.log(message);
	
	if (handle_chat_message(message)) {
		return;
	}

	auto const type = message.type();
	if (type.empty() or (type.front() != '-' and type != "drag" and type != "move")) {
		maybe_use_previous_move();
	}

	// Documented at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
	// under the section "Battle progress"
	if (type == "") {
		// Do nothing. This message is used to indicate the end of a series.
	} else if (type == "-ability") {
		auto const party = party_from_player_id(message.next());
		auto const ability = from_string<Ability>(message.next());
		m_battle.set_value_on_active(party, ability);
	} else if (type == "-activate") {
		auto const party = party_from_player_id(message.next());
		auto const what = message.next();
		auto const details = message.next();
		if (what == "Substitute") {
			if (details == "[damage]") {
				m_move_state.damage_substitute(other(party));
				handle_u_turn(other(party));
			}
		} else {
			// what = ability: Forewarn
			// details = Earthquake
			// remainder() = [of] p1a: Excadrill
		}
	} else if (type == "-anim") {
#if 0
		auto const party = party_from_player_id(message.next());
		// Solar Beam is the only example I have of this
		auto const move? = message.next();
		auto const target? = message.next();
#endif
	} else if (type == "-boost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (type == "callback") {
		// Looks like it's just another form of "cant", but it is probably used
		// for more things than just that
	} else if (type == "cant") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == "REASON" or "REASON|MOVE"
#endif
	} else if (type == "-center") {
	} else if (type == "-crit") {
		auto const user = other(party_from_player_id(message.next()));
		m_move_state.critical_hit(user);
	} else if (type == "-curestatus") {
		auto const party = party_from_player_id(message.next());
		if (m_move_state.party() != party) {
			maybe_use_previous_move();
			m_move_state.clear_status(party);
		}
	} else if (type == "-cureteam") {
#if 0
		auto const pokemon = message.next();
#endif
	} else if (type == "-damage") {
		handle_damage(message);
	} else if (type == "detailschange" or type == "-formechange") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (type == "-end") {
		// Tells me if the substitute is destroyed, probably other stuff ending
		auto const party = party_from_player_id(message.next());
		auto const source = message.next();
		if (source == "Substitute") {
			m_move_state.break_substitute(other(party));
			handle_u_turn(other(party));
		}
	} else if (type == "-endability") {
#if 0
		auto const pokemon = message.next();
		auto const ability = message.next();
#endif
	} else if (type == "-enditem") {
		auto const party = party_from_player_id(message.next());
		auto const item = from_string<Item>(message.next());
		m_battle.set_value_on_active(party, item);
	} else if (type == "error") {
		if (message.remainder() != "[Invalid choice] There's nothing to choose") {
			send_random_move();
		}
	} else if (type == "-fail") {
#if 0
		auto const party = party_from_player_id(message.next());
		// There can be more to this message, but nothing we care about
#endif
	} else if (type == "faint") {
		constexpr auto slot = 0;
		auto const party = party_from_player_id(message.next());
		m_battle.handle_fainted(party, slot);
		if (m_battle.is_me(party) and m_battle.ai().size() != 1_bi) {
			m_replacing_fainted = true;
			send_move(determine_action());
		}
	} else if (type == "-fieldend") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
#endif
	} else if (type == "-fieldstart") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
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
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "-hint") {
		// message.remainder() == MESSAGE
	} else if (type == "-hitcount") {
		auto const party = party_from_player_id(message.next());
		auto const count = bounded::to_integer<2, 5>(message.next());
		static_cast<void>(party);
		static_cast<void>(count);
		// TODO: Implement multi-hit moves
	} else if (type == "-immune") {
		auto const party = party_from_player_id(message.next());
		auto const source = parse_from_source(message);
		bounded::visit(source, bounded::overload(
			// TODO: Validate that the type should be immune
			[](MainEffect) {},
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause immunity"); },
			[](FromMiscellaneous) { throw std::runtime_error("Miscellaneous effects cannot cause immunity"); },
			[](FromMove) { throw std::runtime_error("Moves cannot cause immunity"); },
			[](FromRecoil) { throw std::runtime_error("Recoil cannot cause immunity"); },
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "inactive") {
		// message.remainder() == MESSAGE
		// Timer is on
	} else if (type == "inactiveoff") {
		// message.remainder() == MESSAGE
		// Timer is off
	} else if (type == "-item") {
		auto const party = party_from_player_id(message.next());
		auto const item = from_string<Item>(message.next());
		m_battle.set_value_on_active(party, item);
	} else if (type == "-mega") {
#if 0
		auto const pokemon = message.next();
		auto const megastone = message.next();
#endif
	} else if (type == "-message") {
	} else if (type == "-miss") {
		auto const user_party = party_from_player_id(message.next());
		m_move_state.miss(user_party);
	} else if (type == "move") {
		auto const party = party_from_player_id(message.next());
		auto const move = from_string<Moves>(message.next());
#if 0
		// target is sent only for moves that target one Pokemon
		auto const target = message.next();
#endif
		if (m_move_state.party() != party) {
			maybe_use_previous_move();
		}
		m_move_state.use_move(party, move);
		if (m_battle.is_me(party) and move == Moves::Baton_Pass and m_battle.ai().size() != 1_bi) {
			maybe_use_previous_move();
			send_move(determine_action());
		}
	} else if (type == "-notarget") {
		// When you use a move, but there is no one to target
	} else if (type == "player") {
		// At the end of a battle, I received this with a body of "p1|"
	} else if (type == "-prepare") {
		// From moves like SolarBeam on the charge turn
	} else if (type == "request") {
#if 0
		auto const json = message.remainder();
#endif
	} else if (type == "-resisted") {
		// message.remainder() == POKEMON
	} else if (type == "-sethp") {
		auto const parsed = parse_set_hp_message(message);
		static_cast<void>(parsed);
	} else if (type == "-sideend") {
#if 0
		// Light Screen, Reflect, etc.
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (type == "-sidestart") {
#if 0
		// Light Screen, Reflect, etc.
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (type == "-supereffective") {
		// message.remainder() == POKEMON
	} else if (type == "switch" or type == "drag") {
		auto const parsed = parse_switch(message);

		auto const is_me = m_battle.is_me(parsed.party);
		if (is_me) {
			auto const index = get_species_index(m_battle.ai(), parsed.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		constexpr auto slot = 0;
		auto const move = m_battle.find_or_add_pokemon(parsed.party, slot, parsed.species, parsed.level, parsed.gender);
		correct_hp_and_status(
			is_me,
			m_battle.active_pokemon(parsed.party, to_replacement(move)),
			HPAndStatus{parsed.hp, parsed.status}
		);
		if (type == "drag") {
			m_move_state.phaze_index(other(parsed.party), get_team(m_battle, parsed.party), parsed.species);
		} else {
			m_move_state.use_move(parsed.party, move);
		}
	} else if (type == "replace") {
#if 0
		// Illusion ended
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (type == "-singleturn") {
		// Received for things like Protect that last the rest of the turn
	} else if (type == "-start") {
		auto const party = party_from_player_id(message.next());
		auto const first_part_of_source = message.next();
		auto parse_remainder_of_source = [&]{
			if (first_part_of_source == "typechange") {
				auto const changed_type = message.next();
				static_cast<void>(changed_type);
				return parse_from_source(message);
			} else {
				auto const [source_type, string_source] = split(first_part_of_source, ':');
				return parse_effect_source(source_type, string_source);
			}
		};
		auto const source = parse_remainder_of_source();
		bounded::visit(source, bounded::overload(
			[](MainEffect) { throw std::runtime_error("Unexpected -start source MainEffect"); },
			[&](FromConfusion) {
				auto const how = message.next();
				if (how == "[fatigue]") {
					// TODO
				} else {
					m_move_state.confuse();
				}
			},
			[](FromMiscellaneous) {},
			[](FromMove) {},
			[](FromRecoil) { throw std::runtime_error("Unexpected -start source FromRecoil"); },
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "-status") {
		auto const party = party_from_player_id(message.next());
		auto const status = parse_status(message.next());
		auto const source = parse_from_source(message);
		bounded::visit(source, bounded::overload(
			[&](MainEffect) { m_move_state.status(party, status); },
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
			[&](FromMiscellaneous) { m_move_state.status(other(party), status); },
			[&](FromMove) { m_move_state.status(party, status); },
			[](FromRecoil) { throw std::runtime_error("Recoil cannot cause another status"); },
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "swap") {
#if 0
		auto const pokemon = message.next();
		auto const position = message.next();
		// position here is "0", "1", or "2"
#endif
	} else if (type == "tie") {
		m_completed = true;
	} else if (type == "-transform") {
		// message.remainder() == POKEMON|SPECIES
	} else if (type == "turn") {
		auto const turn = bounded::to_integer<0, bounded::max_value<std::uint32_t>>(message.next());
		m_analysis_logger << std::string(20, '=') << "\nBegin turn " << turn << '\n';
		m_battle.handle_begin_turn(turn);
		send_move(determine_action());
	} else if (type == "-unboost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (type == "upkeep") {
	} else if (type == "-weather") {
#if 0
		auto const weather = message.next();
#endif
	} else if (type == "win") {
		if (m_log_foe_teams) {
			auto const won = m_username == message.next();
			if (!won) {
				log_foe_team(m_battle.generation(), m_usage_stats, m_battle.foe(), m_random_engine);
			}
		}
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
	}
}

void BattleParser::handle_damage(InMessage message) {
	auto const parsed = parse_hp_message(message);
	auto const hp_and_status = HPAndStatus{parsed.hp, parsed.status};
	auto move_damage = [&](Party const party) {
		m_move_state.damage(party, hp_and_status);
	};
	auto const party = parsed.party;
	bounded::visit(parsed.source, bounded::overload(
		[&](MainEffect) {
			if (m_move_state.move_damages_self(party)) {
				return;
			}
			move_damage(other(party));
			handle_u_turn(other(party));
		},
		[&](FromConfusion) {
			// TODO: Technically you cannot select Hit Self, you just execute
			// it. This matters for things like priority or determining whether
			// Sucker Punch succeeds. As a workaround for now, say the user
			// selected Struggle.
			// When hitting self in confusion, we get -activate then -damage
			maybe_use_previous_move();
			m_move_state.use_move(party, Moves::Struggle);
			m_move_state.use_move(party, Moves::Hit_Self);
			move_damage(party);
		},
		[](FromMiscellaneous) {},
		[](FromMove) {},
		[&](FromRecoil) { m_move_state.recoil(party); },
		[&](auto const value) { m_battle.set_value_on_active(party, value); }
	));
	m_move_state.hp_change(party, hp_and_status);
}

namespace {

auto hp_to_damage(Pokemon const & pokemon, HP::current_type const new_hp) {
	auto const old_hp = get_hp(pokemon).current();
	if (new_hp > old_hp) {
		std::cerr << "Took negative damage\n";
		return HP::current_type(0_bi);
		// throw std::runtime_error("Took negative damage");
	}
	return HP::current_type(old_hp - new_hp);
}

auto const & select_pokemon(Team const & team, Moves const move) {
	return is_switch(move) ? team.pokemon(to_replacement(move)) : team.pokemon();
}

auto compute_damage(Battle const & battle, Party const user_party, Moves const move, HPAndStatus const damage) {
	auto const damaged_party = move == Moves::Hit_Self ? user_party : other(user_party);
	auto const is_me = battle.is_me(damaged_party);
	auto const & team = is_me ? battle.ai() : battle.foe();
	auto const & pokemon = select_pokemon(team, move);
	auto const new_hp = to_real_hp(is_me, get_hp(pokemon), damage.hp);
	return hp_to_damage(pokemon, new_hp.value);
}

constexpr auto causes_recoil(Moves const move) {
	switch (move) {
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Flare_Blitz:
		case Moves::Head_Smash:
		case Moves::Submission:
		case Moves::Take_Down:
		case Moves::Volt_Tackle:
		case Moves::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

} // namespace

void BattleParser::maybe_use_previous_move() {
	auto const maybe_data = m_move_state.complete();
	if (!maybe_data) {
		return;
	}
	auto const data = *maybe_data;
	
	if (causes_recoil(data.move.executed) and !data.recoil) {
		// TODO: This could also be Magic Guard
		m_battle.set_value_on_active(data.party, Ability::Rock_Head);
	}
	
	auto const other_pokemon = get_team(m_battle, other(data.party)).pokemon();

	constexpr auto slot = 0;
	struct LocalDamage {
		ActualDamage value;
		bool did_any_damage;
	};
	auto const damage = bounded::visit(data.damage, bounded::overload(
		[](std::monostate) {
			return LocalDamage{ActualDamage::Known{0_bi}, false};
		},
		[&](HPAndStatus const hp_and_status) -> LocalDamage {
			auto const value = compute_damage(m_battle, data.party, data.move.executed, hp_and_status);
			return LocalDamage{
				ActualDamage::Known{value},
				value != 0_bi
			};
		},
		[&](MoveState::SubstituteDamaged) -> LocalDamage {
			return LocalDamage{ActualDamage::Capped{get_hp(other_pokemon).max() / 4_bi}, true};
		},
		[&](MoveState::SubstituteBroke) -> LocalDamage {
			return LocalDamage{ActualDamage::Known{other_pokemon.substitute().hp()}, true};
		}
	));
	
	auto const other_move = other_pokemon.moved() ?
		OtherMove([&]{
			auto const move = other_pokemon.last_used_move().name();
			auto const type = get_type(m_battle.generation(), move, get_hidden_power(other_pokemon).type());
			return KnownMove{move, type};
		}()) :
		OtherMove(FutureMove{
			data.move.executed == Moves::Sucker_Punch and damage.did_any_damage
		});

	m_battle.handle_use_move(data.party, slot, data.move, data.clear_status, damage.value, other_move);

	auto const user_is_me = m_battle.is_me(data.party);
	if (data.user_hp_and_status) {
		correct_hp_and_status(
			user_is_me,
			m_battle.active_pokemon(data.party),
			*data.user_hp_and_status
		);
	}
	if (data.other_hp_and_status) {
		correct_hp_and_status(
			!user_is_me,
			m_battle.active_pokemon(other(data.party)),
			*data.other_hp_and_status
		);
	}
}

Moves BattleParser::determine_action() {
	if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}

	m_analysis_logger << to_string(m_battle.ai()) << '\n';
	m_analysis_logger << "Seen " << to_string(m_battle.foe()) << '\n';
	auto predicted = predict_team(m_battle.generation(), m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
	m_analysis_logger << "Predicted " << to_string(predicted) << '\n' << std::flush;

	return expectiminimax(m_battle.generation(), m_battle.ai(), predicted, m_battle.weather(), m_evaluate, Depth(m_depth, 1U), m_analysis_logger);
}

void BattleParser::send_move(Moves const move) {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	using std::to_string;
	auto switch_move = [&]{ return to_string(m_slot_memory[to_replacement(move)]); };
	auto move_index = [&]{ return to_string(get_move_index(m_battle.ai().pokemon(), move) + 1_bi); };
	m_send_message(m_id + (is_switch(move) ? "|/switch " + switch_move() : "|/move " + move_index()));
}

void BattleParser::send_random_move() {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	auto distribution = std::uniform_int_distribution(1, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team));
	auto const result = distribution(m_random_engine);

	auto switch_move = [=]{ return std::to_string(result - max_moves_per_pokemon); };
	auto move_index = [=]{ return std::to_string(result); };
	auto const is_switch = result > max_moves_per_pokemon;
	m_send_message(m_id + (is_switch ? "|/switch " + switch_move() : "|/move " + move_index()));
}

auto parse_switch(InMessage message) -> ParsedSwitch {
	auto const party = party_from_player_id(message.next());
	auto const details = parse_details(message.next());
	auto const [visible_hp, status] = parse_hp_and_status(message.next());
	return ParsedSwitch{party, details.species, details.level, details.gender, visible_hp, status};
}

void BattleParser::handle_u_turn(Party const party) {
	if (m_battle.is_me(party) and m_move_state.executed_move() == Moves::U_turn and get_hp(m_battle.foe().pokemon()).current() == 0_bi and m_battle.ai().size() != 1_bi) {
		maybe_use_previous_move();
		send_move(determine_action());
	}
}

}	// namespace ps
}	// namespace technicalmachine
