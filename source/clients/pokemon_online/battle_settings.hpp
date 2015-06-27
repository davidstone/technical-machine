// Pokemon Online battle settings
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

#pragma once

#include <cstdint>
#include <vector>
#include "../battle_settings.hpp"

namespace technicalmachine {
namespace po {

struct InMessage;

struct BattleSettings : ::technicalmachine::BattleSettings {
	enum Clauses : uint32_t {
		sleep_CLAUSE = 1 << 0,
		freeze_CLAUSE = 1 << 1,
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
	BattleSettings (uint32_t battle_clauses, uint8_t battle_mode);
	BattleSettings (std::vector <uint32_t> const & battle_clauses, uint8_t battle_mode);
	bool are_acceptable () const;
	uint32_t const clauses;
	Mode const mode;
private:
	bool active (Clauses const check) const;
};

struct BattleConfiguration {
	BattleConfiguration (InMessage & msg);
private:
	uint8_t const generation;
	uint8_t const mode;
	uint32_t const id1;
	uint32_t const id2;
	uint32_t const clauses;
	BattleSettings const settings;
};

}	// namespace po
}	// namespace technicalmachine
