// Pokemon Lab Battle settings
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

#ifndef POKEMON_LAB_BATTLE_SETTINGS_H_
#define POKEMON_LAB_BATTLE_SETTINGS_H_

#include <cstdint>
#include "../network/battle_settings.h"

namespace technicalmachine {
namespace pl {

class BattleSettings : public network::GenericBattleSettings {
	private:
		uint8_t generation;
		uint32_t active_party_size;
		uint32_t max_team_length;
	public:
		BattleSettings (uint8_t battle_generation, uint32_t battle_active_party_size, uint32_t battle_max_team_length);
		bool are_acceptable () const;
};

} // namespace pl
} // namespace technicalmachine
#endif // POKEMON_LAB_BATTLE_SETTINGS_H_
