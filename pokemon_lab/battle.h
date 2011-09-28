// Pokemon Lab battle
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_LAB_BATTLE_H_
#define POKEMON_LAB_BATTLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "../analyze_logs.h"
#include "../species.h"
#include "../team.h"
#include "../weather.h"

namespace technicalmachine {

namespace pl {

class BotClient;

class Battle {
	private:
		std::vector <Species> slot_memory;
	public:
		Team ai;
		Team foe;
		Weather weather;
		Log log;
		int depth;
		uint8_t party;
		Battle (std::string const & opponent, int depth_);
		void handle_request_action (BotClient & botclient, uint32_t field_id, uint8_t slot, uint8_t index, bool replace, std::vector <uint8_t> const & switches, bool can_switch, bool forced, std::vector <uint8_t> const & moves);
		void incorrect_hp (Team & team);
		uint8_t switch_slot (Move::Moves move);
		void handle_print (uint8_t category, int16_t message_id, std::vector <std::string> const & arguments);
		void update_active_print (Log & log, std::vector <std::string> const & arguments);
		void handle_use_move (uint8_t party, uint8_t slot, std::string const & nickname, int16_t move_id);
		void handle_withdraw (uint8_t party, uint8_t slot, std::string const & nickname);
		void handle_send_out (uint8_t party, uint8_t slot, uint8_t index, std::string const & nickname, int16_t species_id, int8_t gender_, uint8_t level);
		void handle_health_change (uint8_t party, uint8_t slot, int16_t change_in_health, int16_t remaining_health, int16_t denominator);
		void handle_set_pp (uint8_t party, uint8_t slot, uint8_t pp);
		void handle_fainted (uint8_t party, uint8_t slot, std::string const & nickname);
		void handle_begin_turn (uint16_t turn_count);
		void handle_set_move (uint8_t pokemon, uint8_t move_slot, int16_t new_move, uint8_t pp, uint8_t max_pp);
};

}
}
#endif
