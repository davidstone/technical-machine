// Pokemon Online battle logic
// Copyright (C) 2012 David Stone
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

#ifndef POKEMON_ONLINE_BATTLE_HPP_
#define POKEMON_ONLINE_BATTLE_HPP_

#include <random>
#include <string>
#include <vector>
#include "outmessage.hpp"
#include "../battle.hpp"

namespace technicalmachine {
namespace po {

class Client;
class InMessage;

class Battle : public GenericBattle {
	private:
		OutMessage action;
	public:
		Battle (std::random_device::result_type seed, std::string const & opponent, int const battle_depth);
		Battle (std::random_device::result_type seed, std::string const & opponent, int const battle_depth, Team const & team);
		void handle_message (Client & client, uint32_t battle_id, uint8_t command, uint8_t player, InMessage & msg);
	private:
		unsigned get_max_damage_precision () const;
		uint8_t get_target () const;
		int16_t ai_change_in_hp (int16_t remaining_hp) const;
		int16_t foe_change_in_hp (int16_t remaining_hp) const;
		static constexpr unsigned pokemon_per_team = 6;
		static constexpr unsigned moves_per_pokemon = 4;
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE_BATTLE_HPP_
