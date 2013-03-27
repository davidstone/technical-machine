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

#include "battle_settings.hpp"

#include <cstdint>
#include <vector>

#include "inmessage.hpp"

namespace technicalmachine {
namespace po {
namespace {

uint32_t vector_to_clauses (std::vector <uint32_t> const & clauses);

}	// unnamed namespace

BattleSettings::BattleSettings (uint32_t const battle_clauses, uint8_t const battle_mode):
	::technicalmachine::BattleSettings(),
	clauses (battle_clauses),
	mode (static_cast <Mode> (battle_mode)) {
}

BattleSettings::BattleSettings (std::vector <uint32_t> const & battle_clauses, uint8_t const battle_mode):
	::technicalmachine::BattleSettings(),
	clauses (vector_to_clauses (battle_clauses)),
	mode (static_cast <Mode> (battle_mode)) {
}

namespace {

uint32_t vector_to_clauses (std::vector <uint32_t> const & clauses) {
	uint32_t clause = 0;
	for (uint32_t const value : clauses)
		clause |= value;
	return clause;
}

}	// unnamed namespace

bool BattleSettings::active (Clauses const check) const {
	return clauses & check;
}

bool BattleSettings::are_acceptable () const {
	return active (SLEEP_CLAUSE) and active (SPECIES_CLAUSE) and !active (CHALLENGE_CUP) and !active (REARRANGE_TEAMS) and mode == SINGLES;
}

BattleConfiguration::BattleConfiguration (InMessage & msg):
	generation (msg.read_byte ()),
	mode (msg.read_byte ()),
	id1 (msg.read_int ()),
	id2 (msg.read_int ()),
	clauses (msg.read_int ()),
	settings (clauses, mode) {
}

}	// namespace po
}	// namespace technicalmachine
