// Stats data structures
// Copyright 2010 David Stone
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

#include <map>

struct stats {
	unsigned char base;
	unsigned char iv;		// 0 through 31
	unsigned char ev;		// 0 through 63
	char stage;	// -6 though 6
	short stat;		// Current HP or last calculated value for other stats
	short max;		// Max HP only
};

enum natures { ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID };

void set_nature_map (std::map <std::string, natures> &natures_map) {
	natures_map["Adamant"] = ADAMANT;
	natures_map["Bashful"] = BASHFUL;
	natures_map["Bold"] = BOLD;
	natures_map["Brave"] = BRAVE;
	natures_map["Calm"] = CALM;
	natures_map["Careful"] = CAREFUL;
	natures_map["Docile"] = DOCILE;
	natures_map["Gentle"] = GENTLE;
	natures_map["Hardy"] = HARDY;
	natures_map["Hasty"] = HASTY;
	natures_map["Impish"] = IMPISH;
	natures_map["Jolly"] = JOLLY;
	natures_map["Lax"] = LAX;
	natures_map["Lonely"] = LONELY;
	natures_map["Mild"] = MILD;
	natures_map["Modest"] = MODEST;
	natures_map["Naive"] = NAIVE;
	natures_map["Naughty"] = NAUGHTY;
	natures_map["Quiet"] = QUIET;
	natures_map["Quirky"] = QUIRKY;
	natures_map["Rash"] = RASH;
	natures_map["Relaxed"] = RELAXED;
	natures_map["Sassy"] = SASSY;
	natures_map["Serious"] = SERIOUS;
	natures_map["Timid"] = TIMID;
}

#endif
