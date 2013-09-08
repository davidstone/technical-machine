// Hold move scores to allow efficient reordering
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

#include "move_scores.hpp"
#include "evaluate.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
constexpr auto initial = Evaluate::victory + 1;
}

MoveScores::MoveScores(Pokemon const & pokemon) {
	// Set the score of all foe moves to an illegally high value, so that they
	// get sorted last. If they didn't even need to be checked for their
	// complete value before, they probably still don't need to be.
	//
	// If this is for my team, then it doesn't matter what I set the scores to,
	// because I evaluate every move of mine and give it a score. Therefore,
	// this works in all situations.
	pokemon.move.for_each([&](Move const & move) {
		auto const key = std::make_pair(pokemon.name(), move.name());
		auto const inserted = scores.insert(std::make_pair(key, initial));
		if (inserted.second) {
			inserted.first->second = initial;
		}
	});
}

int64_t const & MoveScores::at(Species const species, Moves const name) const {
	return scores.at(std::make_pair(species, name));
}
int64_t & MoveScores::at(Species const species, Moves const name) {
	return scores.at(std::make_pair(species, name));
}

}	// namespace technicalmachine
