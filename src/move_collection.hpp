// Collection of moves with index indicating current move
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

#ifndef MOVE_COLLECTION_HPP_
#define MOVE_COLLECTION_HPP_

#include "collection.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include "move.hpp"

namespace technicalmachine {

class MoveCollection : public detail::BaseCollection<Move> {
	public:
		MoveCollection (unsigned const team_size);
		unsigned number_of_regular_moves () const;
		// Skips Struggle and switches
		void for_each_regular_move (std::function<void(Move &)> const & f);
		void for_each_regular_move (std::function<void(Move const &)> const & f) const;
		bool regular_move_exists (std::function<bool(Move const &)> const & condition) const;
		bool a_regular_move_is_selectable () const;
		// Move::END if none
		Move::Moves name_of_last_used_move () const;
		// nullptr if not found
		Move * find_if (std::function<bool(Move &)> const & condition);
		Move const * find_if (std::function<bool(Move const &)> const & condition) const;
		std::vector<std::pair<int64_t, size_t>> create_ordered_container (bool const ai) const;
		std::vector<Move::Moves> legal_switches (uint8_t const pokemon_index) const;
		// Temporary function until I rework my detailed stats data structures
		uint8_t size () const;
};
}	// namespace technicalmachine
#endif	// MOVE_COLLECTION_HPP_
