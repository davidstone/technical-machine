// Data structure for unknowns
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef UNKNOWN_H_
#define UNKNOWN_H_

#include <cstdint>
#include <vector>

namespace technicalmachine {

struct Unknown {
//	std::vector<moves> moveset;
//	uint8_t ability;
	uint8_t item;
	uint8_t nature;
//	uint8_t hpiv;
	uint8_t hpev;
//	uint8_t atkiv;
	uint8_t atkev;
//	uint8_t defiv;
//	uint8_t defev;
//	uint8_t spaiv;
//	uint8_t spaev;
//	uint8_t spdiv;
//	uint8_t spdev;
//	uint8_t speiv;
	uint8_t speev;
};

void unknown (std::vector<Unknown> &hidden);

}
#endif
