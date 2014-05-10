// Determine when transpositions occur
// Copyright (C) 2014 David Stone
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
#include "move_scores.hpp"

#include "../team.hpp"
#include "../weather.hpp"

#include "../move/moves.hpp"

namespace technicalmachine {
namespace {

class Hash {
public:
	uint64_t ai;
	uint64_t foe;
	int64_t value;
	uint32_t weather;
	unsigned depth;
	constexpr Hash ():
		ai(0),
		foe(0),
		value(0),
		weather(0),
		depth (0) {
	}
	Hash (uint64_t ai_hash, uint64_t foe_hash, uint32_t weather_hash, unsigned depth_current);
	friend bool operator== (Hash const & lhs, Hash const & rhs);
};

Hash::Hash (uint64_t ai_hash, uint64_t foe_hash, uint32_t weather_hash, unsigned depth_current):
	ai (ai_hash),
	foe (foe_hash),
	weather (weather_hash),
	depth (depth_current) {
}

bool operator== (Hash const & lhs, Hash const & rhs) {
	return lhs.ai == rhs.ai and lhs.foe == rhs.foe and lhs.weather == rhs.weather;
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

	// GCC has a memory hog bug that prevents me from making the array much
	// larger than this. http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53650
	// However, this is about the size that I want.
	static constexpr size_t ai_dimension = 256;
	static constexpr size_t foe_dimension = 256;
	static constexpr size_t weather_dimension = 16;
	static Hash table [ai_dimension][foe_dimension][weather_dimension] = {};
	
	unsigned const ai_position = current.ai % ai_dimension;
	unsigned const foe_position = current.foe % foe_dimension;
	unsigned const weather_position = current.weather % weather_dimension;
	return table [ai_position] [foe_position] [weather_position];
}

}	// anonymous namespace

int64_t transposition (Team & ai, Team & foe, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	if (depth == 0) {
		return static_cast<int64_t>(evaluate(ai, foe, weather));
	}
	int64_t value;
	// This long-form hash should be unique within a game.
	Hash current (ai.hash(), foe.hash(), weather.hash(), depth);
	Hash & saved = hash_table_lookup (current);
	// I verify that saved == current because hash_table_lookup only checks
	// against shortened hashes for speed and memory reasons. I need the
	// additional check to minimize the chances of a collision.
	if (saved.depth >= current.depth and saved == current)
		value = saved.value;
	else {
		Moves phony = Moves::END;
		// If I can't find it, continue evaluation as normal.
		value = select_type_of_move(ai, foe, weather, depth, evaluate, phony);
		current.value = value;
		
		// Since I didn't find any stored value at the same hash as the
		// current state, or the value I found was for a shallower depth,
		// add the new value to my table.
		saved = current;
	}
	return value;
}

}	// namespace technicalmachine
