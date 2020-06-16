// Hold move scores to allow efficient reordering
// Copyright (C) 2019 David Stone
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

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <limits>

namespace technicalmachine {
enum class Moves : std::uint16_t;
struct Pokemon;

struct MoveScores {
	explicit MoveScores(StaticVectorMove legal_selections, bool ai);
	void set(Moves const move_name, double const score) {
		auto const it = containers::find_if(m_scores, [=](value_type const value) {
			return value.move_name == move_name;
		});
		BOUNDED_ASSERT(it != end(m_scores));
		it->score = score;
	}
	auto ordered_moves(bool ai) const -> StaticVectorMove;
private:
	struct value_type {
		Moves move_name;
		double score;
	};
	containers::static_vector<value_type, bounded::detail::builtin_max_value<MoveSize>> m_scores;
};

}	// namespace technicalmachine
