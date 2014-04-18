// Moves specific to one Pokemon and shared moves (Struggle and switches)
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

#ifndef MOVE__CONTAINER_HPP_
#define MOVE__CONTAINER_HPP_

#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "shared.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/integer_range.hpp>

#include <algorithm>
#include <vector>

namespace technicalmachine {

// TODO: add begin, end, regular_end iterators
class MoveContainer {
public:
	using value_type = Move;
	using size_type = MoveSize;
	using index_type = MoveIndex;
	MoveContainer(TeamSize my_team_size);
	auto operator[](index_type index) const -> Move const &;
	// Skips Struggle and switches
	auto regular_move(RegularMoveIndex index) const -> Move const &;
	auto regular_move(RegularMoveIndex index) -> Move &;
	static constexpr auto empty() -> bool {
		// A move container is never empty, it always contains at least Struggle
		return false;
	}
	template<class... Args>
	auto emplace_back(Args&&... args) -> void {
		// The only moves that are ever added are regular moves. Shared
		// moves are just a reference to a collection at the Team level.
		regular.emplace_back(std::forward<Args>(args)...);
	}

	auto size() const -> size_type;
	auto number_of_regular_moves() const -> RegularMoveSize;
	template<typename Function>
	auto find_if(Function const & condition) const -> Move const * {
		auto const it = std::find_if(regular.begin(), regular.end(), condition);
		return (it != regular.end()) ? &*it : nullptr;
	}
	template<typename Function>
	auto find_if(Function const & condition) -> Move * {
		auto const it = std::find_if(regular.begin(), regular.end(), condition);
		return (it != regular.end()) ? &*it : nullptr;
	}
	auto remove_switch() -> void;
	
	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	auto max_hash() const -> hash_type;
	
	friend auto operator==(MoveContainer const & lhs, MoveContainer const & rhs) -> bool;
private:
	auto unchecked_regular_move(RegularMoveIndex index) const -> Move const &;
	auto unchecked_regular_move(RegularMoveIndex index) -> Move &;
	std::vector<Move> regular;
	SharedMoves shared;
};

}	// namespace technicalmachine
#endif	// MOVE__CONTAINER_HPP_
