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

#include "move_scores.hpp"
#include "evaluate.hpp"
#include "../pokemon/pokemon.hpp"

#include <containers/legacy_iterator.hpp>

#include <algorithm>
#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

MoveScores::MoveScores(Pokemon const & pokemon) {
	// Set the score of all foe moves to an illegally high value, so that they
	// get sorted last. If they didn't even need to be checked for their
	// complete value before, they probably still don't need to be.
	//
	// If this is for my team, then it doesn't matter what I set the scores to,
	// because I evaluate every move of mine and give it a score. Therefore,
	// this works in all situations.
	for (auto const move : all_moves(pokemon)) {
		constexpr auto initial = static_cast<double>(victory + 1_bi);
		m_scores.emplace_back(move, initial);
	}
	std::sort(containers::legacy_iterator(begin(m_scores)), containers::legacy_iterator(end(m_scores)));
}

namespace {

template<typename Container>
auto & search(Container & container, Moves const move) {
	auto compare = [](auto const & element, auto const & requested) { return element.first < requested; };
	auto const it = std::lower_bound(
		containers::legacy_iterator(begin(container)),
		containers::legacy_iterator(end(container)),
		move,
		compare
	);
	assert(it.base() != end(container));
	assert(it->first == move);
	return it->second;
}

}	// namespace

double MoveScores::get(Moves const move) const {
	return search(m_scores, move);
}
void MoveScores::set(Moves const move, double const value) {
	search(m_scores, move) = value;
}

}	// namespace technicalmachine
