// Pokemon Lab battle
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

#ifndef POKEMON_LAB_BATTLE_H_
#define POKEMON_LAB_BATTLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "../battle.h"
#include "../species.h"

namespace technicalmachine {
namespace pl {

class Client;

class Battle : public GenericBattle {
	public:
		Battle (std::string const & opponent, int battle_depth);
		void handle_request_action (Client & client, uint32_t field_id, uint8_t slot, uint8_t index, bool replace, std::vector <uint8_t> const & switches, bool can_switch, bool forced, std::vector <uint8_t> const & moves);
		void handle_print (uint8_t category, int16_t message_id, std::vector <std::string> const & arguments);
		void update_active_print (Log & log, std::vector <std::string> const & arguments);
		void handle_set_move (uint8_t pokemon, uint8_t move_slot, int16_t new_move, uint8_t pp, uint8_t max_pp);
};

} // namespace pl
} // namespace technicalmachine
#endif // POKEMON_LAB_BATTLE_H_
