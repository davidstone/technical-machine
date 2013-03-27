// Pokemon Lab Battle settings
// Copyright (C) 2013 David Stone
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

#ifndef POKEMON_LAB__BATTLE_SETTINGS_HPP_
#define POKEMON_LAB__BATTLE_SETTINGS_HPP_

#include <cstdint>
#include "../battle_settings.hpp"

namespace technicalmachine {
namespace pl {

class BattleSettings : public ::technicalmachine::BattleSettings {
	public:
		BattleSettings(uint8_t battle_generation, uint32_t battle_active_party_size, uint32_t battle_max_team_length);
		bool are_acceptable () const;
	private:
		uint32_t active_party_size;
		uint32_t max_team_length;
		uint8_t generation;
};

}	// namespace pl
}	// namespace technicalmachine
#endif	// POKEMON_LAB__BATTLE_SETTINGS_HPP_
