// Collection of moves with index indicating current move
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

#pragma once

#include "../collection.hpp"

#include "container.hpp"
#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "moves.hpp"

#include <bounded_integer/integer_range.hpp>
#include <bounded_integer/optional.hpp>

#include <vector>

namespace technicalmachine {
using namespace bounded::literal;

struct MoveScores;

struct MoveCollection : detail::Collection<MoveContainer> {
	using detail::Collection<MoveContainer>::index_type;
	explicit MoveCollection(TeamSize my_team_size);
	
	auto regular() const {
		return make_range(container.regular_begin(), container.regular_end());
	}
	auto regular() {
		return make_range(container.regular_begin(), container.regular_end());
	}

	auto add(Moves move, Pp::pp_ups_type pp_ups = 3_bi) -> void;
	auto size() const {
		return container.size();
	}
	auto remove_switch() -> void;
};

inline auto hash(MoveCollection const & collection) noexcept {
	return hash_range<RegularMoveSize>(collection.regular().begin(), collection.regular().end());
}

auto index(MoveCollection const & moves, Moves name) -> bounded::optional<RegularMoveIndex>;
auto set_index(MoveCollection & moves, Moves name) -> void;
auto regular_move(MoveCollection const & moves) -> Move const &;
auto regular_move(MoveCollection & moves) -> Move &;

}	// namespace technicalmachine
