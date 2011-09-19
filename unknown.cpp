// List of unknowns
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include "unknown.h"
#include "item.h"
#include "stat.h"

namespace technicalmachine {

/*
This creates a list of all possible variables for a given Pokemon. Other functions are then used to remove impossible data points. For instance, the reverse damage calculator removes stat / ability combinations that are not possible for the given damage. The battle log parser removes things based on messages.

Profiling showed that creating a new std::vector in each other function is significantly faster and much less memory intensive than deleting elements from a std::list.
*/

void unknown (std::vector<Unknown> &hidden) {

	Unknown unknown;

	// I need to add both possible abilities for a Pokemon as soon as it comes into play
	
//	unknown.defev = 0;
//	unknown.spaev = 0;
//	unknown.spdev = 0;
	
	for (unknown.item = 0; unknown.item != static_cast<unsigned char> (Item::END_ITEM); ++unknown.item) {
		for (unknown.nature = static_cast<unsigned char> (Stat::NAUGHTY); unknown.nature <= static_cast<unsigned char> (Stat::DOCILE); ++unknown.nature) {		// This is to make output "pretty". Fix later.
			for (unknown.atkev = 0; unknown.atkev <= 63; ++unknown.atkev) {
				for (unknown.speev = 0; unknown.speev <= 63; ++unknown.speev) {
					for (unknown.hpev = 0; unknown.hpev <= 63; ++unknown.hpev) {
						if (unknown.hpev + unknown.atkev + unknown.speev > 127)
							break;
						hidden.push_back (unknown);
					}
				}
			}
		}
	}
}

}
