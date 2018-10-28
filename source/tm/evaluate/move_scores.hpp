// Hold move scores to allow efficient reordering
// Copyright (C) 2015 David Stone
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
#include <tm/move/moves.hpp>

#include <containers/static_vector/static_vector.hpp>

#include <limits>

namespace technicalmachine {
struct Pokemon;

struct MoveScores {
	explicit MoveScores(Pokemon const & pokemon);
	double get(Moves move) const;
	void set(Moves move, double value);
private:
	static constexpr auto capacity = std::numeric_limits<MoveSize>::max();
	containers::static_vector<std::pair<Moves, double>, static_cast<intmax_t>(capacity)> m_scores;
};

}	// namespace technicalmachine
