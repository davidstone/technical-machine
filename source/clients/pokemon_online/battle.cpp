// Pokemon Online battle logic
// Copyright (C) 2015 David Stone
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

#include "battle.hpp"

#include "client.hpp"
#include "conversion.hpp"
#include "inmessage.hpp"
#include "outmessage.hpp"

#include "../timestamp.hpp"

#include "../../team.hpp"

#include "../../pokemon/max_pokemon_per_team.hpp"
#include "../../pokemon/pokemon.hpp"

#include <bounded_integer/array.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace technicalmachine {
namespace po {
namespace {

struct Todo {
	explicit Todo (InMessage & in_msg, std::string const & str):
		msg (in_msg) {
		std::cerr << str + '\n';
	}
	explicit Todo (InMessage & in_msg):
		msg (in_msg) {
	}
	~Todo() {
		msg.read_remaining_bytes();
	}
private:
	InMessage & msg;
};

void parse_battle_chat(InMessage & msg) {
	Todo const t (msg);
	std::cout << timestamp() << ": " << msg.read_string() << '\n';
}

}	// namespace

void Battle::handle_message (Client & client, uint32_t battle_id, uint8_t command, Party const party, InMessage & msg) {
	enum {
		SEND_OUT = 0,
		WITHDRAW = 1,		// "SendBack"
		USE_ATTACK = 2,
		OFFER_CHOICE = 3,
		BEGIN_TURN = 4,
		PP_CHANGE = 5,
		HP_CHANGE = 6,
		KO = 7,
		EFFECTIVENESS = 8,
		MISS = 9,
		CRITICAL_HIT = 10,
		NUMBER_OF_HITS = 11,
		STAT_CHANGE = 12,
		STATUS_CHANGE = 13,
		STATUS_MESSAGE = 14,
		FAILED = 15,
		BATTLE_CHAT = 16,
		MOVE_MESSAGE = 17,
		ITEM_MESSAGE = 18,
		NO_TARGET = 19,
		FLINCH = 20,
		RECOIL = 21,
		WEATHER_MESSAGE = 22,
		STRAIGHT_DAMAGE = 23,
		ABILITY_MESSAGE = 24,
		ABS_STATUS_CHANGE = 25, 
		SUBSTITUTE = 26,
		BATTLE_END = 27,
		BLANK_MESSAGE = 28,
		CANCEL_MOVE = 29,
		CLAUSE = 30,
		DYNAMIC_INFO = 31,
		DYNAMIC_STATS = 32,
		SPECTATING = 33,
		SPECTATOR_CHAT = 34,
		ALREADY_STATUSED = 35,
		TEMPORARY_POKEMON_CHANGE = 36,
		CLOCK_START = 37,
		CLOCK_STOP = 38,
		RATED = 39,
		TIER_SECTION = 40,
		END_MESSAGE = 41,
		POINT_ESTIMATE = 42,
		MAKE_YOUR_CHOICE = 43,
		AVOID = 44,
		REARRANGE_TEAM = 45,
		SPOT_SHIFTS = 46
	};
	switch (command) {
		case BEGIN_TURN:
			parse_begin_turn(msg);
			break;
		case SEND_OUT:
			parse_send_out(msg, party);
			break;
		case WITHDRAW:
			// Do nothing.
			break;
		case USE_ATTACK:
			parse_use_attack(msg, party);
			break;
		case STRAIGHT_DAMAGE:
			parse_straight_damage(msg);
			break;
		case HP_CHANGE:
			parse_hp_change(msg, party);
			break;
		case PP_CHANGE:
			parse_pp_change(msg);
			break;
		case KO: {
			constexpr uint8_t slot = 0;
			handle_fainted (party, slot);
			break;
		}
		case EFFECTIVENESS:
			parse_effectiveness(msg);
			break;
		case AVOID:
		case MISS:
			std::cerr << ((command == AVOID) ? "AVOID\n" : "MISS\n");
			handle_miss(party);
			break;
		case NO_TARGET:
			// "But there was no target..."
			break;
		case FAILED:
			// "But it failed!"
			break;
		case CRITICAL_HIT:
			handle_critical_hit(party);
			break;
		case NUMBER_OF_HITS:
			parse_number_of_hits(msg);
			break;
		case STAT_CHANGE:
			parse_stat_change(msg);
			break;
		case STATUS_MESSAGE:
			parse_status_message(msg);
			break;
		case STATUS_CHANGE:
			parse_status_change(msg);
			break;
		case ABS_STATUS_CHANGE:
			parse_abs_status_change(msg);
			break;
		case ALREADY_STATUSED:
			parse_already_statused(msg);
			break;
		case FLINCH:
			handle_flinch(party);
			break;
		case RECOIL:
			parse_recoil(msg);
			break;
		case WEATHER_MESSAGE:
			parse_weather_message(msg);
			break;
		case ABILITY_MESSAGE:
			parse_ability_message(msg, party);
			break;
		case ITEM_MESSAGE:
			parse_item_message(msg, party);
			break;
		case MOVE_MESSAGE:
			parse_move_message(msg);
			break;
		case SUBSTITUTE:
			parse_substitute(msg);
			break;
		case CLAUSE:
			std::cerr << "CLAUSE\n";
			break;
		case DYNAMIC_INFO:
			parse_dynamic_info(msg);
			break;
		case DYNAMIC_STATS:
			parse_dynamic_stats(msg);
			break;
		case SPECTATING:
			parse_spectating(msg);
			break;
		case TEMPORARY_POKEMON_CHANGE:
			parse_temporary_pokemon_change(msg);
			break;
		case CLOCK_START:
			parse_clock_start(msg);
			break;
		case CLOCK_STOP:
			parse_clock_stop(msg);
			break;
		case RATED:
			handle_rated(client, msg);
			break;
		case TIER_SECTION:
			parse_tier_section(msg);
			break;
		case BLANK_MESSAGE:
			// Apparently this is used to tell the client to print a new line...
			break;
		case BATTLE_CHAT:
		case END_MESSAGE:
			std::cerr << ((command == BATTLE_CHAT) ? "BATTLE_CHAT\n" : "END_MESSAGE\n");
			parse_battle_chat(msg);
			break;
		case SPECTATOR_CHAT:
			parse_spectator_chat(client, msg, battle_id);
			break;
		case POINT_ESTIMATE:
			parse_point_estimate(msg);
			break;
		case OFFER_CHOICE:
			parse_offer_choice(client, msg, battle_id);
			break;
		case MAKE_YOUR_CHOICE:
			handle_make_your_choice(client);
			break;
		case CANCEL_MOVE:
			handle_cancel_move();
			break;
		case REARRANGE_TEAM:
			parse_rearrange_team(msg);
			break;
		case SPOT_SHIFTS:
			parse_spot_shifts(msg);
			break;
		case BATTLE_END:
			parse_battle_end(msg);
			break;
		default:
			std::cerr << "Unknown battle message " << command << '\n';
			break;
	}
}

void Battle::parse_begin_turn(InMessage & msg) const {
	uint32_t const turn = msg.read_int();
	handle_begin_turn(static_cast<uint16_t>(turn));
}

void Battle::parse_send_out (InMessage & msg, Party const party) {
	bool const is_silent = msg.read_byte();
	static_cast<void>(is_silent);
	uint8_t const index = msg.read_byte();
	uint16_t const species_id = msg.read_short();
	uint8_t const forme_id = msg.read_byte();
	Species const species = id_to_species(species_id, forme_id);
	std::string const nickname = msg.read_string();
	uint8_t const hp_percent = msg.read_byte();
	// TODO: use hp_percent to verify things are good
	static_cast<void>(hp_percent);
	uint32_t const full_status = msg.read_int();
	// TODO: use full_status to verify things are good
	static_cast<void>(full_status);
	Gender const gender(id_to_gender(msg.read_byte()));
	bool const shiny = msg.read_byte();
	static_cast<void>(shiny);
	Level const level(bounded::checked_integer<Level::min, Level::max>(msg.read_byte()));
	uint8_t const slot = 0;
	handle_send_out(party, slot, index, nickname, species, gender, level);
	if (is_me(party)) {
		slot_memory_bring_to_front();
	}
}

void Battle::parse_use_attack (InMessage & msg, Party const party) {
	std::cerr << is_me(party) << " USE ATTACK\n";
	uint16_t const attack = msg.read_short ();
	constexpr uint8_t slot = 0;
	handle_use_move (party, slot, id_to_move(attack));
	last_attacker = party;
}

void Battle::parse_straight_damage(InMessage & msg) {
	auto const damage = UpdatedHP::VisibleHP(msg.read_short());
	constexpr uint8_t slot = 0;
	handle_direct_damage(other(last_attacker), slot, damage);
}

void Battle::parse_hp_change (InMessage & msg, Party const party) {
	auto const remaining_hp = UpdatedHP::VisibleHP(msg.read_short());
	uint8_t const slot = 0;
	handle_hp_change(party, slot, remaining_hp);
}

void Battle::parse_pp_change (InMessage & msg) {
	uint8_t const move = msg.read_byte();
	uint8_t const new_pp = msg.read_byte();
	// TODO: use these to verify things are still good
	static_cast<void>(move);
	static_cast<void>(new_pp);
}

void Battle::parse_effectiveness(InMessage & msg) {
	// TODO: Hidden Power.
	uint8_t const effectiveness = msg.read_byte();
	// 0, 1, 2, 4, 8, 16
	static_cast<void>(effectiveness);
}

void Battle::parse_number_of_hits(InMessage & msg) {
	// TODO: parse this
	uint8_t const number = msg.read_byte();
	static_cast<void>(number);
}

void Battle::parse_stat_change(InMessage & msg) {
	std::cerr << "STAT_CHANGE\n";
	// I think stat may work just as well unsigned.
	int8_t const stat = static_cast<int8_t>(msg.read_byte());
	int8_t const boost = static_cast<int8_t>(msg.read_byte());
	// TODO: Parse for Accupressure
	static_cast<void>(stat);
	static_cast<void>(boost);
}

void Battle::parse_status_message (InMessage & msg) {
	Todo const t(msg, "STATUS_MESSAGE");
	int8_t const status = static_cast<int8_t>(msg.read_byte());
	static_cast<void>(status);
}

void Battle::parse_status_change (InMessage & msg) {
	Todo const t(msg, "STATUS_CHANGE");
	int8_t const status = static_cast<int8_t> (msg.read_byte());
	// bool const multiple_turns = msg.read_byte ();
	// Includes things like confusion
	static_cast<void>(status);
}

void Battle::parse_abs_status_change (InMessage & msg) {
	uint8_t const index = msg.read_byte();
	if (index >= max_pokemon_per_team) {
		std::cerr << "Invalid Pokemon index.\n";
		return;
	}
	int8_t const status = static_cast<int8_t> (msg.read_byte());
	static_cast<void>(status);
}

void Battle::parse_already_statused (InMessage & msg) {
	uint8_t const status = msg.read_byte ();
	static_cast<void>(status);
}

void Battle::parse_recoil (InMessage & msg) {
	std::cerr << "RECOIL\n";
	bool const damaging = msg.read_byte ();
	if (damaging) {
		// is hit with recoil
	}
	else {
		// had its energy drained
	}
}

void Battle::parse_weather_message (InMessage & msg) {
	std::cerr << "WEATHER_MESSAGE\n";
	int8_t const wstatus = static_cast<int8_t> (msg.read_byte());
	int8_t const weather_var = static_cast<int8_t> (msg.read_byte());
	static_cast<void>(wstatus);
	static_cast<void>(weather_var);
}

void Battle::parse_ability_message (InMessage & msg, Party const party) {
	uint16_t const ability = msg.read_short ();
	uint8_t const part = msg.read_byte ();
	handle_ability_message(party, battle_id_to_ability(ability, part));
	#if 0
	int8_t const type = msg.read_byte ();
	int8_t const foe = msg.read_byte ();
	int16_t const other = msg.read_short ();
	#endif
}

void Battle::parse_item_message (InMessage & msg, Party const party) {
	uint16_t const item_id = msg.read_short ();
	uint8_t const part = msg.read_byte ();
	handle_item_message(party, battle_id_to_item(item_id, part));
	// int8_t const foe = msg.read_byte ();
	// int16_t const berry = msg.read_short ();
	// int16_t const other = msg.read_short ();
}

void Battle::parse_move_message (InMessage &) {
	std::cerr << "MOVE_MESSAGE\n";
	#if 0
	uint16_t const move = msg.read_short ();
	uint8_t const part = msg.read_byte ();
	int8_t const type = msg.read_byte ();
	int8_t const foe = msg.read_byte ();
	int16_t const other = msg.read_short ();
	std::string const q = msg.read_string ();		// No idea what this is
	#endif
}

void Battle::parse_substitute (InMessage & msg) {
	std::cerr << "SUBSTITUTE\n";
	bool const substitute = msg.read_byte ();
	static_cast<void>(substitute);
}

namespace {

auto parse_boosts(InMessage & msg) {
	bounded::array<int8_t, 7> boosts;
	for (int8_t & boost : boosts)
		boost = static_cast<int8_t>(msg.read_byte());
	return boosts;
}

}	// namespace

void Battle::parse_dynamic_info (InMessage & msg) {
	auto const boosts = parse_boosts(msg);
	enum DynamicFlags {
		Spikes=1,
		SpikesLV2=2,
		SpikesLV3=4,
		StealthRock=8,
		ToxicSpikes=16,
		ToxicSpikesLV2=32
	};
	// I don't think I need any of this
	static_cast<void>(boosts);
	uint8_t const flags = msg.read_byte ();
	static_cast<void>(flags);
}

void Battle::parse_dynamic_stats (InMessage & msg) {
	for (unsigned n = 0; n != 5; ++n) {
		auto const something = static_cast<int16_t> (msg.read_short());
		static_cast<void>(something);
	}
}

void Battle::parse_spectating (InMessage & msg) {
	bool const joining = msg.read_byte ();
	uint32_t const user_id = msg.read_int ();
	static_cast<void>(joining);
	static_cast<void>(user_id);
}

void Battle::parse_temporary_pokemon_change (InMessage & msg) {
	Todo const t (msg, "TEMPORARY_POKEMON_CHANGE");
	enum {
		TEMP_MOVE = 0,
		DEF_MOVE = 1,
		TEMP_PP = 2,
		TEMP_SPRITE = 3,
		DEFINITE_FORM = 4,
		AESTHETIC_FORM = 5
	};
	uint8_t const code = msg.read_byte ();
	switch (code) {
		case TEMP_MOVE:
		case DEF_MOVE: {
			std::cerr << "MOVE\n";
			int8_t const slot = static_cast<int8_t> (msg.read_byte ());
			static_cast<void>(slot);
			break;
		}
		case TEMP_PP: {
			std::cerr << "TEMP_PP\n";
			int8_t const slot = static_cast<int8_t> (msg.read_byte ());
			int8_t const pp = static_cast<int8_t> (msg.read_byte ());
			static_cast<void>(slot);
			static_cast<void>(pp);
			break;
		}
		case TEMP_SPRITE: {
			std::cerr << "TEMP_SPRITE\n";
			// ???
			break;
		}
		case DEFINITE_FORM: {
			std::cerr << "DEFINITE_FORM\n";
			int8_t const pokemon = static_cast<int8_t> (msg.read_byte ());
			int16_t const form = static_cast<int16_t> (msg.read_short ());
			static_cast<void>(pokemon);
			static_cast<void>(form);
			break;
		}
		case AESTHETIC_FORM: {
			std::cerr << "AESTHETIC_FORM\n";
			int16_t form = static_cast<int16_t> (msg.read_short ());
			static_cast<void>(form);
			break;
		}
		default:
			std::cerr << "Unknown TEMPORARY_POKEMON_CHANGE code: " << code << '\n';
			break;
	}
}

void Battle::parse_clock_start (InMessage & msg) {
	uint16_t const remaining_time = msg.read_short ();
	static_cast<void>(remaining_time);
}

void Battle::parse_clock_stop (InMessage & msg) {
	Todo const t (msg, "CLOCK_STOP");
	uint16_t const remaining_time = msg.read_short ();
	static_cast<void>(remaining_time);
}

void Battle::handle_rated (Client & client, InMessage & msg) {
	client.write_team();
	bool const rated = msg.read_byte ();
	static_cast<void>(rated);
}

void Battle::parse_tier_section (InMessage & msg) {
	std::string const tier = msg.read_string ();
	static_cast<void>(tier);
}

void Battle::parse_spectator_chat (Client const & client, InMessage & msg, uint32_t const battle_id) {
	std::cerr << "SPECTATOR_CHAT\n";
	uint32_t const user_id = msg.read_int ();
	std::string const message = msg.read_string ();
	client.handle_channel_message (battle_id, client.get_user_name (user_id), message);
}

void Battle::parse_point_estimate (InMessage & msg) {
	Todo const t (msg, "POINT_ESTIMATE");
}

void Battle::parse_offer_choice (Client & client, InMessage & msg, uint32_t const battle_id) {
	int8_t const num_slot = static_cast<int8_t> (msg.read_byte());
	static_cast<void>(num_slot);
	bool const can_switch = msg.read_byte();
	bool const can_attack = msg.read_byte();
	static_cast<void>(can_attack);
	std::vector<uint8_t> attacks_allowed(moves_per_pokemon());
	std::generate(std::begin(attacks_allowed), std::end(attacks_allowed), [&]() { return msg.read_byte(); });
	handle_request_action(client.detailed(), client.evaluation_constants(), action, battle_id, can_switch, attacks_allowed);
}

void Battle::handle_make_your_choice (Client & client) {
	client.send_message(action);
	action.reset_action_code();
}

void Battle::handle_cancel_move () {
	std::cerr << "CANCEL_MOVE\n";
	// Humans make foolish mistakes. Technical Machine does not.
}

void Battle::parse_rearrange_team (InMessage & msg) {
	Todo const t (msg, "REARRANGE_TEAM");
	for (unsigned n = 0; n != max_pokemon_per_team; ++n) {
		uint16_t const species_id = msg.read_short ();
		uint8_t const form_id = msg.read_byte ();
		uint8_t const level = msg.read_byte ();
		uint8_t const gender = msg.read_byte ();
		bool const item = msg.read_byte ();
		static_cast<void>(species_id);
		static_cast<void>(form_id);
		static_cast<void>(level);
		static_cast<void>(gender);
		static_cast<void>(item);
	}
}

void Battle::parse_spot_shifts (InMessage & msg) {
	std::cerr << "SPOT_SHIFTS\n";
	// Spinda is quite the Pokemon!
	int8_t const s1 = static_cast<int8_t> (msg.read_byte ());
	int8_t const s2 = static_cast<int8_t> (msg.read_byte ());
	bool const silent = msg.read_byte ();
	static_cast<void>(s1);
	static_cast<void>(s2);
	static_cast<void>(silent);
}

void Battle::parse_battle_end (InMessage & msg) {
	// Forfeit, Win, Tie, Close seems to be the four options in order
	uint8_t const result = msg.read_byte ();
	static_cast<void>(result);
}

VisibleFoeHP Battle::max_damage_precision() const {
	return 100_bi;
}

}	// namespace po
}	// namespace technicalmachine
