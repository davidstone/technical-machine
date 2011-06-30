// Determine when transpositions occur
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "evaluate.h"
#include "expectiminimax.h"
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "transposition.h"
#include "weather.h"

namespace technicalmachine {

class Hash {
public:
	unsigned long ai;
	unsigned long foe;
	unsigned long weather;
	long score;
	int depth;
	Hash ();
	Hash (unsigned long ai_hash, unsigned long foe_hash, unsigned long weather_hash, int depth_current);
	bool operator== (Hash const &other) const;
	bool operator!= (Hash const &other) const;
};

Hash::Hash ():
	depth (0) {
}

Hash::Hash (unsigned long ai_hash, unsigned long foe_hash, unsigned long weather_hash, int depth_current):
	ai (ai_hash),
	foe (foe_hash),
	weather (weather_hash),
	depth (depth_current) {
}

bool Hash::operator== (Hash const &other) const {
	return ai == other.ai and foe == other.foe and weather == other.weather;
}

bool Hash::operator!= (Hash const &other) const {
	return !(*this == other);
}

long transposition (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv) {
	long score;
	if (depth == 0)
		score = evaluate (ai, foe, weather, sv);
	else {
		// First, I hash both teams and the weather. These are the long-form hashes that are each the size of an unsigned long, and should be unique within a game. I then take those hashes and divide them modulo their relevant dimension (used to determine the size of the transposition table). These short-form hashes are used as the array index. The shortened hash combinations are used to look up the full hashes, score, and depth.
		Hash hash (ai.hash(), foe.hash(), weather.hash(), depth);
		static unsigned long const ai_dimension = 256;
		static unsigned long const foe_dimension = 256;
		static unsigned long const weather_dimension = 32;
		static Hash table [ai_dimension][foe_dimension][weather_dimension] = {};
		Hash * const transpose = &table [hash.ai % ai_dimension] [hash.foe % foe_dimension] [hash.weather % weather_dimension];
		// If I can find the current state in my transposition table at a depth of at least the current depth, set the score to the stored score.
		if (transpose->depth >= hash.depth and *transpose == hash)
			score = transpose->score;
		else {
			moves_list phony = END_MOVE;
			// If I can't find it, set the score to the evaluation of the state at depth - 1.
			hash.score = tree1 (ai, foe, weather, depth, sv, phony);
		
			// Since I didn't find any stored value at the same hash as the current state, or the value I found was for a shallower depth, add the new value to my table.
			*transpose = hash;
			score = hash.score;
		}
	}
	return score;
}

}
