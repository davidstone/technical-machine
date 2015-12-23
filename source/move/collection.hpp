// Collection of moves with index indicating current move
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

#include "container.hpp"
#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "moves.hpp"

#include "../collection.hpp"
#include "../operators.hpp"

#include <bounded/integer_range.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct MoveCollection : detail::Collection<MoveContainer> {
	explicit MoveCollection(TeamSize my_team_size);
	
	auto regular() const {
		return make_range(regular_begin(), regular_end());
	}
	auto regular() {
		return make_range(regular_begin(), regular_end());
	}

	auto add(Moves move, Pp::pp_ups_type pp_ups = 3_bi) -> void;
	using MoveContainer::remove_switch;
};

inline auto hash(MoveCollection const & collection) noexcept {
	return hash_range<RegularMoveSize>(collection.regular().begin(), collection.regular().end());
}

auto index(MoveCollection const & moves, Moves name) -> bounded::optional<RegularMoveIndex>;
auto set_index(MoveCollection & moves, Moves name) -> void;
auto regular_move(MoveCollection const & moves) -> Move;
auto regular_move(MoveCollection & moves) -> Move &;

}	// namespace technicalmachine
