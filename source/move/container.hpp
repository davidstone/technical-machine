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

#include <algorithm>
#include <vector>

namespace technicalmachine {

class MoveContainer {
public:
	using value_type = Move;
	using size_type = MoveSize;
	using index_type = MoveIndex;
	MoveContainer(TeamSize my_team_size);
	Move const & operator[](index_type index) const;
	Move const & regular_move(RegularMoveIndex index) const;
	Move & regular_move(RegularMoveIndex index);
	static constexpr bool empty() {
		// A move container is never empty, it always contains at least Struggle
		return false;
	}
	template<class... Args>
	void emplace_back(Args&&... args) {
		// The only moves that are ever added are regular moves. Shared
		// moves are just a reference to a collection at the Team level.
		regular.emplace_back(std::forward<Args>(args)...);
	}
	// Skips Struggle and switches
	template<typename Function>
	void for_each_regular_move(Function && f) const {
		std::for_each(regular.begin(), regular.end(), f);
	}
	template<typename Function>
	void for_each_regular_move(Function && f) {
		std::for_each(regular.begin(), regular.end(), f);
	}
	template<typename Function>
	void for_each_shared(Function && f) const {
		for (auto const n : bounded::range(shared.size())) {
			f(shared[n]);
		}
	}
	size_type size() const;
	RegularMoveSize number_of_regular_moves() const;
	template<typename Function>
	Move const * find_if(Function const & condition) const {
		auto const it = std::find_if(regular.begin(), regular.end(), condition);
		return (it != regular.end()) ? &*it : nullptr;
	}
	template<typename Function>
	Move * find_if(Function const & condition) {
		auto const it = std::find_if(regular.begin(), regular.end(), condition);
		return (it != regular.end()) ? &*it : nullptr;
	}
	void remove_switch();
	friend bool operator==(MoveContainer const & lhs, MoveContainer const & rhs);
private:
	Move const & unchecked_regular_move(RegularMoveIndex index) const;
	Move & unchecked_regular_move(RegularMoveIndex index);
	std::vector<Move> regular;
	SharedMoves shared;
};

}	// namespace technicalmachine
#endif	// MOVE__CONTAINER_HPP_
