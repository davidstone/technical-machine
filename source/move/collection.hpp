// Collection of moves with index indicating current move
// Copyright (C) 2017 David Stone
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
	
	using detail::Collection<MoveContainer>::regular;
	
	using MoveContainer::remove_switch;

	template<typename... MaybePP>
	auto & emplace_back(Moves const move, MaybePP... maybe_pp) {
		auto const new_index = size(regular());
		auto & result = detail::Collection<MoveContainer>::emplace_back(move, maybe_pp...);
		set_index(static_cast<containers::index_type<MoveCollection>>(new_index));
		return result;
	}
};

auto index(MoveCollection const & moves, Moves name) -> bounded::optional<RegularMoveIndex>;
auto set_index(MoveCollection & moves, Moves name) -> void;

inline auto & add_seen_move(MoveCollection & moves, Moves const move) {
	auto const regular = moves.regular();
	auto const it = containers::find(begin(regular), end(regular), move);
	if (it != end(regular)) {
		moves.set_index(static_cast<containers::index_type<MoveCollection>>(it - begin(regular)));
		return *it;
	}
	return moves.emplace_back(move);
}


}	// namespace technicalmachine
