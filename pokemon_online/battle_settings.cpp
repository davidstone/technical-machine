// Pokemon Online battle settings
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

#include <cstdint>
#include "battle_settings.h"

namespace technicalmachine {
namespace po {

BattleSettings::BattleSettings (uint32_t const battle_clauses, uint8_t battle_mode):
	network::GenericBattleSettings (),
	clauses (static_cast <Clauses> (battle_clauses)),
	mode (static_cast <Mode> (battle_mode)) {
}

bool BattleSettings::active (Clauses const check) const {
	return clauses & check;
}

bool BattleSettings::are_acceptable () const {
	return active (SLEEP_CLAUSE) and active (SPECIES_CLAUSE) and !active (CHALLENGE_CUP) and !active (REARRANGE_TEAMS) and mode == SINGLES;
}

} // namespace po
} // namespace technicalmachine
