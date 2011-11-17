// Pokemon Online battle logic
// Copyright (C) 2011 David Stone
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

#include "battle.h"
#include <string>
#include <vector>
#include "connect.h"
#include "conversion.h"
#include "inmessage.h"
#include "outmessage.h"
#include "../pokemon.h"
#include "../team.h"

namespace technicalmachine {
namespace po {

Battle::Battle (std::string const & opponent, int battle_depth):
	GenericBattle::GenericBattle (opponent, battle_depth) {
	attacks_allowed.reserve (4);
}

enum Command {
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

enum Temporary_Pokemon_Change {
	TEMP_MOVE = 0,
	DEF_MOVE = 1,
	TEMP_PP = 2,
	TEMP_SPRITE = 3,
	DEFINITE_FORM = 4,
	AESTHETIC_FORM = 5
};

void Battle::handle_message (Client & client, uint32_t battle_id, uint8_t command, uint8_t player, InMessage & msg) {
	switch (command) {
		case BEGIN_TURN: {
			uint32_t const turn = msg.read_int ();
			handle_begin_turn (turn);
			break;
		}
		case SEND_OUT: {
			std::cerr << "SEND_OUT\n";
			bool const is_silent = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			Species const species = id_to_species (msg.read_short ());
			uint8_t const forme_id = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			uint8_t const hp_percent = msg.read_byte ();
			uint32_t const full_status = msg.read_int ();
			Gender gender;
			gender.from_simulator_int (msg.read_byte ());
			bool const shiny = msg.read_byte ();
			uint8_t const level = msg.read_byte ();
			uint8_t const slot = 0;
			handle_send_out (player, slot, index, nickname, species, gender, level);
			if (player == 0) {
				for (unsigned n = 0; n != slot_memory.size (); ++n) {
					if (ai.at_replacement ().name == slot_memory [n])
						std::swap (slot_memory [0], slot_memory [n]);
				}
			}
			break;
		}
		case WITHDRAW: {
			// Do nothing.
			break;
		}
		case USE_ATTACK: {
			std::cerr << "USE_ATTACK\n";
			uint32_t const attack = msg.read_short ();
			handle_use_move (player, 0, attack);
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case STRAIGHT_DAMAGE: {
			std::cerr << "STRAIGHT_DAMAGE\n";
			int16_t damage = msg.read_short ();
			std::cerr << "damage: " << damage << '\n';
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case HP_CHANGE: {
			int16_t const remaining_hp = msg.read_short ();
			int16_t const change_in_hp = (player == 0) ? ai.at_replacement ().hp.stat : foe.at_replacement ().hp.stat;
			uint8_t const slot = 0;
			int16_t const denominator = (player == 0) ? ai.at_replacement ().hp.max : 100;
			handle_health_change (player, slot, change_in_hp, remaining_hp, denominator);
			break;
		}
		case PP_CHANGE: {
			std::cerr << "PP_CHANGE\n";
			uint8_t const move = msg.read_byte ();
			uint8_t const new_pp = msg.read_byte ();
			break;
		}
		case KO: {
			uint8_t const slot = 0;
			handle_fainted (player, slot);
			break;
		}
		case EFFECTIVENESS: {
			// This matters for Hidden Power.
			// Do nothing with this for now.
			uint8_t const effectiveness = msg.read_byte ();
			// 0, 1, 2, 4, 8, 16
			break;
		}
		case AVOID:
		case MISS: {
			if (command == AVOID)
				std::cerr << "AVOID\n";
			else
				std::cerr << "MISS\n";
			log.active->miss = true;
			break;
		}
		case NO_TARGET: {
			// "But there was no target..."
			break;
		}
		case FAILED: {
			// "But it failed!"
			break;
		}
		case CRITICAL_HIT: {
			std::cerr << "CH\n";
			log.active->ch = true;
			break;
		}
		case NUMBER_OF_HITS: {
			uint8_t const number = msg.read_byte ();
			break;
		}
		case STAT_CHANGE: {
			std::cerr << "STAT_CHANGE\n";
			int8_t const stat = msg.read_byte ();
			int8_t const boost = msg.read_byte ();
			break;
		}
		case STATUS_MESSAGE: {
			std::cerr << "STATUS_MESSAGE\n";
			int8_t const status = msg.read_byte ();
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case STATUS_CHANGE: {
			std::cerr << "STATUS_CHANGE\n";
			int8_t const status = msg.read_byte ();
//			bool const multiple_turns = msg.read_byte ();
			// Includes things like confusion
			break;
		}
		case ABS_STATUS_CHANGE: {
			uint8_t const index = msg.read_byte ();
			if (index >= 6) {
				std::cerr << "Invalid Pokemon index.\n";
				break;
			}
			int8_t const status = msg.read_byte ();
			break;
		}
		case ALREADY_STATUSED: {
			uint8_t const status = msg.read_byte ();
			break;
		}
		case FLINCH: {
			std::cerr << "FLINCH\n";
			log.active->at_replacement().move->variable.index = 1;
			break;
		}
		case RECOIL: {
			std::cerr << "RECOIL\n";
			bool const damaging = msg.read_byte ();
			if (damaging) {
				// is hit with recoil
			}
			else {
				// had its energy drained
			}
			break;
		}
		case WEATHER_MESSAGE: {
			std::cerr << "WEATHER_MESSAGE\n";
			int8_t const wstatus = msg.read_byte ();
			int8_t const weather = msg.read_byte ();
			break;
		}
		case ABILITY_MESSAGE: {
			std::cerr << "ABILITY_MESSAGE\n";
			uint16_t const ability = msg.read_short ();
//			uint8_t const part = msg.read_byte ();
//			int8_t const type = msg.read_byte ();
//			int8_t const foe = msg.read_byte ();
//			int16_t const other = msg.read_short ();
			std::cerr << "End of ABILITY_MESSAGE\n";
			break;
		}
		case ITEM_MESSAGE: {
			std::cerr << "ITEM_MESSAGE\n";
/*			uint16_t const item = msg.read_short ();
			uint8_t const part = msg.read_byte ();
			int8_t const foe = msg.read_byte ();
			int16_t const berry = msg.read_short ();
			int16_t const other = msg.read_short ();
*/			break;
		}
		case MOVE_MESSAGE: {
			std::cerr << "MOVE_MESSAGE\n";
/*			uint16_t const move = msg.read_short ();
			uint8_t const part = msg.read_byte ();
			int8_t const type = msg.read_byte ();
			int8_t const foe = msg.read_byte ();
			int16_t const other = msg.read_short ();
			std::string const q = msg.read_string ();		// No idea what this is
*/			break;
		}
		case SUBSTITUTE: {
			std::cerr << "SUBSTITUTE\n";
			bool const substitute = msg.read_byte ();
			break;
		}
		case CLAUSE: {
			std::cerr << "CLAUSE\n";
			break;
		}
		case DYNAMIC_INFO: {
			std::cerr << "Dynamic Info\n";
//			for (unsigned n = 0; n != 7; ++n)
//				
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case DYNAMIC_STATS: {
			for (unsigned n = 0; n != 5; ++n)
				int16_t const something = msg.read_short ();
			break;
		}
		case SPECTATING: {
			bool const joining = msg.read_byte ();
			uint32_t const user_id = msg.read_int ();
			break;
		}
		case TEMPORARY_POKEMON_CHANGE: {
			std::cerr << "TEMPORARY_POKEMON_CHANGE\n";
			uint8_t const code = msg.read_byte ();
			switch (code) {
				case TEMP_MOVE:
				case DEF_MOVE: {
					std::cerr << "MOVE\n";
					int8_t const slot = msg.read_byte ();
					while (msg.index != msg.buffer.size ())
						std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
					break;
				}
				case TEMP_PP: {
					std::cerr << "TEMP_PP\n";
					int8_t const slot = msg.read_byte ();
					int8_t const pp = msg.read_byte ();
					while (msg.index != msg.buffer.size ())
						std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
					break;
				}
				case TEMP_SPRITE: {
					std::cerr << "TEMP_SPRITE\n";
					// ???
					while (msg.index != msg.buffer.size ())
						std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
					break;
				}
				case DEFINITE_FORM: {
					std::cerr << "DEFINITE_FORM\n";
					int8_t const pokemon = msg.read_byte ();
					int16_t const form =msg.read_short ();
					while (msg.index != msg.buffer.size ())
						std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
					break;
				}
				case AESTHETIC_FORM: {
					std::cerr << "AESTHETIC_FORM\n";
					int16_t form = msg.read_short ();
					while (msg.index != msg.buffer.size ())
						std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
					break;
				}
				default:
					std::cerr << "Unknown TEMPORARY_POKEMON_CHANGE code: " << code << '\n';
					break;
			}
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case CLOCK_START: {
			uint16_t const remaining_time = msg.read_short ();
			break;
		}
		case CLOCK_STOP: {
			std::cerr << "CLOCK_STOP\n";
			uint16_t const remaining_time = msg.read_short ();
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case RATED: {
			bool const rated = msg.read_byte ();
			break;
		}
		case TIER_SECTION: {
			std::string const tier = msg.read_string ();
			break;
		}
		case BLANK_MESSAGE: {
			// Apparently this is used to tell the client to print a new line...
			break;
		}
		case BATTLE_CHAT:
		case END_MESSAGE: {
			if (command == BATTLE_CHAT)
				std::cerr << "BATTLE_CHAT\n";
			else
				std::cerr << "END_MESSAGE\n";
			std::string const message = msg.read_string ();
			std::cout << message + "\n";
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case SPECTATOR_CHAT: {
			std::cerr << "SPECTATOR_CHAT\n";
			uint32_t const user_id = msg.read_int ();
			std::string const message = msg.read_string ();
			client.handle_channel_message (battle_id, client.get_user_name (user_id), message);
			break;
		}
		case POINT_ESTIMATE: {
			std::cerr << "POINT_ESTIMATE\n";
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case OFFER_CHOICE: {
			std::cerr << "OFFER_CHOICE\n";
			num_slot = msg.read_byte ();
			can_switch = msg.read_byte ();
			can_attack = msg.read_byte ();
			for (unsigned n = 0; n != 4; ++n)
				attacks_allowed [n] = msg.read_byte ();
			break;
		}
		case MAKE_YOUR_CHOICE: {
			std::cerr << "MAKE_YOUR_CHOICE\n";
			handle_request_action (client, battle_id);
			break;
		}
		case CANCEL_MOVE: {
			std::cerr << "CANCEL_MOVE\n";
			// Humans make foolish mistakes. Technical Machine does not.
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case REARRANGE_TEAM: {
			std::cerr << "REARRANGE_TEAM\n";
			for (unsigned n = 0; n != 6; ++n) {
				int16_t const species_id = msg.read_short ();
				int8_t const form_id = msg.read_byte ();
				int8_t const level = msg.read_byte ();
				int8_t const gender = msg.read_byte ();
				bool const item = msg.read_byte ();
			}
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case SPOT_SHIFTS: {
			std::cerr << "SPOT_SHIFTS\n";
			// Spinda is quite the Pokemon!
			int8_t s1 = msg.read_byte ();
			int8_t s2 = msg.read_byte ();
			bool silent = msg.read_byte ();
			break;
		}
		case BATTLE_END: {
			std::cerr << "BATTLE_END\n";
			// Forfeit, Win, Tie, Close seems to be the four options in order
			int8_t const result = msg.read_byte ();
			break;
		}
		default:
			std::cerr << "Unknown battle message " << command << '\n';
			break;
	}
}

/*
	uint32_t const battle_id = msg.read_int ();
	std::cerr << "battle_id: " << static_cast <int> (battle_id) << '\n';
	uint8_t const player_slot = msg.read_byte ();
	std::cerr << "player_slot: " << static_cast <int> (player_slot) << '\n';
	uint8_t const command = msg.read_byte ();
	std::cerr << "command: " << static_cast <int> (command) << '\n';
	switch (command) {
		case CANCEL:
		case CENTER_MOVE:
		case DRAW:
			break;
		case SWITCH:
			break;
		case ATTACK:
			break;
		case REARRANGE:
			break;
		default:
			std::cerr << "Unknown battle message command.\n";
			break;
	}
*/

void Battle::handle_request_action (Client & client, uint32_t battle_id) {
	update_from_previous_turn (client, battle_id);
	OutMessage msg (OutMessage::BATTLE_MESSAGE);
	Move::Moves move = determine_action (client);
	if (Move::is_switch (move))
		msg.write_switch (battle_id, switch_slot (move));
	else {
		uint8_t move_index = 0;
		while (ai.pokemon->move.set [move_index].name != move)
			++move_index;
		uint8_t const target = 0;
		msg.write_move (battle_id, move_index, target);
	}
	msg.send (*client.socket);
	if (!ai.replacing)
		log.initialize_turn (ai, foe);
}

} // namespace po
} // namespace technicalmachine
