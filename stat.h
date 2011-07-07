// Stats data structures
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STAT_H_
#define STAT_H_

#include "species.h"

namespace technicalmachine {

enum stats { HP, ATK, DEF, SPA, SPD, SPE };

class Stat {
	public:
		unsigned char base;
		char iv;			// 0 through 31
		char ev;			// 0 through 63
		char stage;		// -6 though 6
		int max;		// Max HP only
		int stat;		// Current HP or last calculated value for other stats
	
		Stat (species name, int level, stats stat);
		void boost (int n);
	private:
		static unsigned char const base_stat [][6];
};

enum natures { ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, END_NATURE };

}
#endif
