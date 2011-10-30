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

#ifndef POKEMON_ONLINE_BATTLE_SETTINGS_H_
#define POKEMON_ONLINE_BATTLE_SETTINGS_H_

#include <cstdint>
#include "../network/battle_settings.h"

namespace technicalmachine {
namespace po {

class BattleSettings : public network::GenericBattleSettings {
	private:
		enum Clauses {
			SLEEP_CLAUSE = 1 << 0,
			FREEZE_CLAUSE = 1 << 1,
			DISALLOW_SPECTATORS = 1 << 2,
			ITEM_CLAUSE = 1 << 3,
			CHALLENGE_CUP = 1 << 4,
			NO_TIME_OUT = 1 << 5,
			SPECIES_CLAUSE = 1 << 6,
			REARRANGE_TEAMS = 1 << 7,
			SELF_KO_CLAUSE = 1 << 8
		};
		enum Mode {
			SINGLES = 0,
			DOUBLES = 1,
			TRIPLES = 2,
			ROTATION = 3
		};
		Clauses const clauses;
		Mode const mode;
	public:
		BattleSettings (uint32_t const battle_clauses, uint8_t const battle_mode);
	private:
		bool active (Clauses const check) const;
	public:
		bool are_acceptable () const;
};

} // namespace po
} // namespace technicalmachine
#endif // POKEMON_ONLINE_BATTLE_SETTINGS_H_
