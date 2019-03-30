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

#include <containers/flat_map.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <limits>

namespace technicalmachine {
enum class Moves : std::uint16_t;
struct Pokemon;

struct MoveScores {
	explicit MoveScores(Pokemon const & pokemon);
	double get(Moves const move) const {
		auto const it = m_scores.find(move);
		BOUNDED_ASSERT(it != end(m_scores));
		return it->mapped();
	}
	void set(Moves const move, double const value) {
		auto const it = m_scores.find(move);
		BOUNDED_ASSERT(it != end(m_scores));
		it->mapped() = value;
	}
private:
	using value_type = containers::map_value_type<Moves, double>;
	containers::basic_flat_map<containers::static_vector<
		value_type,
		static_cast<int>(std::numeric_limits<MoveSize>::max())
	>> m_scores;
};

}	// namespace technicalmachine
