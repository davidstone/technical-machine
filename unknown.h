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

#include <vector>

namespace technicalmachine {

struct Unknown {
//	std::vector<moves> moveset;
//	unsigned char ability;
	unsigned char item;
	unsigned char nature;
//	unsigned char hpiv;
	unsigned char hpev;
//	unsigned char atkiv;
	unsigned char atkev;
//	unsigned char defiv;
//	unsigned char defev;
//	unsigned char spaiv;
//	unsigned char spaev;
//	unsigned char spdiv;
//	unsigned char spdev;
//	unsigned char speiv;
	unsigned char speev;
};

void unknown (std::vector<Unknown> &hidden);

}
#endif
