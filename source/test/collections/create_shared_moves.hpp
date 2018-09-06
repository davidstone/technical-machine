// Basis for some move tests
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

#include "../../move/is_switch.hpp"
#include "../../move/move.hpp"
#include "../../move/moves.hpp"
#include "../../pokemon/collection.hpp"
#include "../../pokemon/max_pokemon_per_team.hpp"

#include <bounded/integer.hpp>

#include <containers/integer_range.hpp>
#include <containers/vector/vector.hpp>

namespace technicalmachine {
using namespace bounded::literal;

inline auto create_shared_moves(TeamSize const team_size) {
	containers::vector<Moves> shared ({ Moves::Struggle });
	if (team_size != 1_bi) {
		for (auto const n : containers::integer_range(team_size)) {
			shared.emplace_back(to_switch(n));
		}
	}
	return shared;
}

}	// namespace technicalmachine
