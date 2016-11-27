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
#include <iostream>

namespace technicalmachine {
namespace {

#if 0

struct Hash {
	double value;
	uint64_t hash;
	unsigned depth;
	constexpr Hash():
		value(0.0),
		hash(0),
		depth(0)
	{
	}
	constexpr Hash(uint64_t hash_, unsigned depth_):
		value(0.0),
		hash(hash_),
		depth(depth_)
	{
	}
};

#endif

}	// namespace

double transposition(Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	if (depth == 0) {
		return static_cast<double>(evaluate(ai, foe, weather));
	}

#if 0
	// This long-form hash should be unique within a game.
	auto current = Hash(hash(ai, foe, weather).first, depth);

	using Table = containers::array<Hash, 2048>;
	static auto table = Table{};
	// All of these casting gymnastics are because the bounded::integer library
	// is not yet complete with support for integers larger than can fit in
	// std::intmax_t.
	auto & saved = at(table, static_cast<std::intmax_t>(current.hash % static_cast<std::size_t>(size(table))), bounded::non_check);
	// I verify that saved == current because hash_table_lookup only checks
	// against shortened hashes for speed and memory reasons. I need the
	// additional check to minimize the chances of a collision.
	if (saved.depth >= current.depth and saved.hash == current.hash) {
		return saved.value;
	}
#endif
	Moves phony = Moves::END;
	// If I can't find it, continue evaluation as normal.
	auto const value = select_type_of_move(ai, foe, weather, depth, evaluate, phony);
#if 0
	current.value = value;
	
	// Since I didn't find any stored value at the same hash as the
	// current state, or the value I found was for a shallower depth,
	// add the new value to my table.
	saved = current;
#endif
	return value;
}

}	// namespace technicalmachine
