// Moves specific to one Pokemon and shared moves (Struggle and switches)
// Copyright (C) 2012 David Stone
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

#include <cstdint>
#include <functional>
#include <vector>
#include "move.hpp"

namespace technicalmachine {
class SharedMoves;

class MoveContainer {
	public:
		MoveContainer(SharedMoves & s);
		MoveContainer(MoveContainer const & other);
		MoveContainer(MoveContainer && other);
		MoveContainer & operator=(MoveContainer const & other);
		MoveContainer & operator=(MoveContainer && other);
		Move const & operator[](uint8_t index) const;
		Move & operator[](uint8_t index);
		bool empty() const;
		template<class... Args>
		void emplace_back(Args&&... args) {
			// The only moves that are ever added are regular moves. Shared
			// moves are just a reference to a collection at the Team level.
			regular.emplace_back(std::forward<Args>(args)...);
		}
		// Skips Struggle and switches
		void for_each_regular_move (std::function<void(Move const &)> const & f) const;
		void for_each_regular_move (std::function<void(Move &)> const & f);
		void for_each_shared (std::function<void(Move const &)> const & f) const;
		void for_each_shared (std::function<void(Move &)> const & f);
		uint8_t size() const;
		uint8_t number_of_regular_moves() const;
		std::vector<Move> concatenate() const;
		Move const * find_if (std::function<bool(Move const &)> const & condition) const;
		Move * find_if (std::function<bool(Move const &)> const & condition);
		void update_shared_moves(SharedMoves & s);
		friend bool operator==(MoveContainer const & lhs, MoveContainer const & rhs);
	private:
		std::vector<Move> regular;
		SharedMoves * shared;
};

}	// namespace technicalmachine
#endif	// MOVE__CONTAINER_HPP_
