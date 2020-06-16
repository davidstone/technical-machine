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

#include <containers/algorithms/sort.hpp>

namespace technicalmachine {
using namespace bounded::literal;

MoveScores::MoveScores(StaticVectorMove const legal_selections, bool const ai):
	m_scores(
		containers::transform(legal_selections, [=](Moves const move) {
			return value_type{move, ai ? static_cast<double>(-victory - 1_bi) : static_cast<double>(victory + 1_bi)};
		})
	)
{
}

auto MoveScores::ordered_moves(bool const ai) const -> StaticVectorMove {
	auto intermediate = containers::static_vector<value_type, static_cast<int>(bounded::max_value<MoveSize>)>(m_scores);
	auto compare = [=](value_type const lhs, value_type const rhs) {
		return ai ? lhs.score > rhs.score : lhs.score < rhs.score;
	};
	containers::sort(intermediate, compare);
	return StaticVectorMove(containers::transform(intermediate, [](value_type const value) { return value.move_name; }));
}

} // namespace technicalmachine
