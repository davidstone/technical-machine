// Pokemon Lab Battle settings
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

#include "battle_settings.hpp"
#include <cstdint>

namespace technicalmachine {
namespace pl {

BattleSettings::BattleSettings (uint8_t battle_generation, uint32_t battle_active_party_size, uint32_t battle_max_team_length):
	network::GenericBattleSettings (),
	active_party_size (battle_active_party_size),
	max_team_length (battle_max_team_length),
	generation (battle_generation) {
}

bool BattleSettings::are_acceptable () const {
	return active_party_size == 1 and max_team_length == 6;
}

} // namespace pl
} //namespace technicalmachine
