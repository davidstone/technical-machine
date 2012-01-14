// Determine when transpositions occur
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

#include "transposition.hpp"

#include <cstdint>

#include "evaluate.hpp"
#include "expectiminimax.hpp"
#include "move.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

class Hash {
public:
	uint64_t ai;
	uint64_t foe;
	uint64_t weather;
	int64_t value;
	int depth;
	Hash ();
	Hash (uint64_t ai_hash, uint64_t foe_hash, uint64_t weather_hash, int depth_current);
	bool operator== (Hash const & other) const;
	bool operator!= (Hash const & other) const;
};

Hash::Hash ():
	depth (0) {
}

Hash::Hash (uint64_t ai_hash, uint64_t foe_hash, uint64_t weather_hash, int depth_current):
	ai (ai_hash),
	foe (foe_hash),
	weather (weather_hash),
	depth (depth_current) {
}

bool Hash::operator== (Hash const & other) const {
	return ai == other.ai and foe == other.foe and weather == other.weather;
}

bool Hash::operator!= (Hash const & other) const {
	return !(*this == other);
}

Hash & hash_table_lookup (Hash const & current) {
	// This creates a short-form hash that I use to look up the full hash, the
	// score, and the depth of search used to get that score. It is later
	// checked against the long form (which should never have collisions in a
	// single battle). I also later check the depth to make sure it is at least
	// as deep.
	
	// The dimension is used to limit the size of the table.
	
	// TODO: In theory, I could make this per battle. The long-form hashes are
	// almost certain to have no collisions in a single battle (because certain
	// things such as Pokemon species are fixed), but I haven't verified
	// minimal collisions between battles. However, it's also possible that if
	// TM uses the same team, the current foe could be using a similar enough
	// team to a previous foe to where there may actually be collisions between
	// battles. I also need to investigate to see which of these is more likely
	// and find the relative merits of each strategy (per battle or the current
	// static array).
	static constexpr unsigned ai_dimension = 256;
	static constexpr unsigned foe_dimension = 256;
	static constexpr unsigned weather_dimension = 32;
	static Hash table [ai_dimension][foe_dimension][weather_dimension] = {};
	
	unsigned const ai_position = current.ai % ai_dimension;
	unsigned const foe_position = current.foe % foe_dimension;
	unsigned const weather_position = current.weather % weather_dimension;
	return table [ai_position] [foe_position] [weather_position];
}

}	// anonymous namespace

int64_t transposition (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score) {
	int64_t value;
	if (depth == 0) {
		value = score.evaluate (ai, foe, weather);
	}
	else {
		// This long-form hash should be unique within a game.
		Hash current (ai.hash(), foe.hash(), weather.hash(), depth);
		Hash & saved = hash_table_lookup (current);
		// I verify that saved == current because hash_table_lookup only checks
		// against shortened hashes for speed and memory reasons. I need the
		// additional check to minimize the chances of a collision.
		if (saved.depth >= current.depth and saved == current)
			value = saved.value;
		else {
			Move::Moves phony = Move::END;
			// If I can't find it, continue evaluation as normal.
			value = select_move_branch (ai, foe, weather, depth, score, phony);
			current.value = value;
			
			// Since I didn't find any stored value at the same hash as the
			// current state, or the value I found was for a shallower depth,
			// add the new value to my table.
			saved = current;
		}
	}
	return value;
}

}	// namespace technicalmachine
