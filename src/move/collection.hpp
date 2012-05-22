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

#ifndef MOVE__COLLECTION_HPP_
#define MOVE__COLLECTION_HPP_

#include "../collection.hpp"

#include <cstdint>
#include <functional>
#include <vector>

#include "container.hpp"
#include "move.hpp"
#include "ranked.hpp"

namespace technicalmachine {
class SharedMoves;

class MoveCollection : public detail::BaseCollection<Move, MoveContainer> {
	public:
		MoveCollection (SharedMoves & s);
		unsigned number_of_regular_moves () const;
		void for_each (std::function<void(Move const &)> const & f) const;
		void for_each (std::function<void(Move &)> const & f);
		// Skips Struggle and switches
		void for_each_regular_move (std::function<void(Move const &)> const & f) const;
		void for_each_regular_move (std::function<void(Move &)> const & f);
		bool set_index_if_found(Move::Moves name);
		// nullptr if not found
		Move const * find_if (std::function<bool(Move const &)> const & condition) const;
		Move * find_if (std::function<bool(Move &)> const & condition);
		bool regular_move_exists (std::function<bool(Move const &)> const & condition) const;
		bool regular_move_exists (Move::Moves name) const;
		bool a_regular_move_is_selectable () const;
		// Move::END if none
		Move::Moves name_of_last_used_move () const;
		std::vector<RankedMove> create_ordered_container (bool ai) const;
		uint8_t size () const;
		uint8_t regular_size () const;
		void update_shared_moves(SharedMoves & s);
};
}	// namespace technicalmachine
#endif	// MOVE__COLLECTION_HPP_
