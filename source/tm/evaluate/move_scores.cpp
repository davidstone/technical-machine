// Hold move scores to allow efficient reordering
// Copyright (C) 2016 David Stone
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

#include <tm/evaluate/move_scores.hpp>
#include <tm/evaluate/evaluate.hpp>
#include <tm/pokemon/pokemon.hpp>

namespace technicalmachine {
using namespace bounded::literal;

MoveScores::MoveScores(Pokemon const & pokemon):
	// Set the score of all foe moves to an illegally high value, so that they
	// get sorted last. If they didn't even need to be checked for their
	// complete value before, they probably still don't need to be.
	//
	// If this is for my team, then it doesn't matter what I set the scores to,
	// because I evaluate every move of mine and give it a score. Therefore,
	// this works in all situations.
	m_scores(containers::transform(all_moves(pokemon), [](Move const move) {
		return value_type(move.name(), static_cast<double>(victory + 1_bi));
	}))
{
}

}	// namespace technicalmachine
