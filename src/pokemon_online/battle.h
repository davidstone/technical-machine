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

#ifndef POKEMON_ONLINE_BATTLE_H_
#define POKEMON_ONLINE_BATTLE_H_

#include <string>
#include <vector>
#include "outmessage.h"
#include "../battle.h"

namespace technicalmachine {
namespace po {

class Client;
class InMessage;

class Battle : public GenericBattle {
	private:
		OutMessage action;
	public:
		Battle (std::string const & opponent, int const battle_depth);
		Battle (std::string const & opponent, int const battle_depth, Team const & team);
		void handle_message (Client & client, uint32_t battle_id, uint8_t command, uint8_t player, InMessage & msg);
//		void update_active_print (Log & log, std::vector <std::string> const & arguments);
//		void handle_set_move (uint8_t pokemon, uint8_t move_slot, int16_t new_move, uint8_t pp, uint8_t max_pp);
	private:
		unsigned get_max_damage_precision () const;
		uint8_t get_target () const;
		static constexpr unsigned pokemon_per_team = 6;
		static constexpr unsigned moves_per_pokemon = 4;
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE_BATTLE_H_
