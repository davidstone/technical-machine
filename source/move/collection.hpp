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

#ifndef MOVE__COLLECTION_HPP_
#define MOVE__COLLECTION_HPP_

#include "../collection.hpp"

#include "container.hpp"
#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "moves_forward.hpp"

#include <bounded_integer/integer_range.hpp>
#include <bounded_integer/optional.hpp>

#include <vector>

namespace technicalmachine {
using namespace bounded::literal;

class MoveScores;

class MoveCollection : public detail::Collection<MoveContainer> {
	using Base = detail::Collection<MoveContainer>;
public:
	using Base::index_type;
	using Base::size_type;
	MoveCollection(TeamSize my_team_size);

	auto regular_move() const -> Move const &;
	auto regular_move() -> Move &;
	auto regular_move(RegularMoveIndex index) const -> Move const &;
	auto regular_move(RegularMoveIndex index) -> Move &;
	auto number_of_regular_moves() const -> RegularMoveSize;

	template<class... Args>
	auto add(Args&&... args) -> void {
		Base::add(std::forward<Args>(args)...);
		current_index = number_of_regular_moves() - 1_bi;
	}
	template<typename Function>
	auto for_each(Function && f) const -> void {
		for (auto const move_index : bounded::integer_range(container.size())) {
			f(container[move_index]);
		}
	}

	using Base::set_index;
	auto set_index_if_found(Moves name) -> bool;
	auto set_index(Moves name) -> void;
	// nullptr if not found
	auto find (Moves name) const -> Move const *;
	auto find (Moves name) -> Move *;
	template<typename Function>
	auto regular_move_exists(Function && condition) const -> bool {
		return container.find_if(std::forward<Function>(condition)) != nullptr;
	}
	using Base::index;
	auto index(Moves name) const -> bounded::optional<RegularMoveIndex>;
	auto size() const -> size_type;
	auto remove_switch() -> void;
	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	auto max_hash() const -> hash_type;
};
}	// namespace technicalmachine
#endif	// MOVE__COLLECTION_HPP_
