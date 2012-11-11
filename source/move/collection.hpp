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
#include "moves_forward.hpp"
#include "ranked.hpp"

namespace technicalmachine {
class SharedMoves;

class MoveCollection : public detail::BaseCollection<Move, MoveContainer> {
		typedef detail::BaseCollection<Move, MoveContainer> Base;
	public:
		using Base::index_type;
		MoveCollection (SharedMoves & s);
		index_type number_of_regular_moves () const;
		template<class... Args>
		void add(Args&&... args) {
			Base::add(std::forward<Args>(args)...);
			current_index = number_of_regular_moves() - 1;
		}
		void for_each (std::function<void(Move const &)> const & f) const;
		void for_each (std::function<void(Move &)> const & f);
		// Skips Struggle and switches
		void for_each_regular_move (std::function<void(Move const &)> const & f) const;
		void for_each_regular_move (std::function<void(Move &)> const & f);
		using Base::set_index;
		bool set_index_if_found(Moves name);
		void set_index(Moves name);
		void set_switch_index(unsigned replacement);
		// nullptr if not found
		Move const * find (Moves name) const;
		Move * find (Moves name);
		bool regular_move_exists (std::function<bool(Move const &)> const & condition) const;
		bool exists (Moves name) const;
		bool a_regular_move_is_selectable () const;
		// Move::END if none
		Moves name_of_last_used_move () const;
		bool moved_since_switch() const;
		std::vector<RankedMove> create_ordered_container (bool ai) const;
		index_type size () const;
		void update_shared_moves(SharedMoves & s);
		typedef uint64_t hash_type;
		hash_type hash() const;
		hash_type max_hash() const;
};
}	// namespace technicalmachine
#endif	// MOVE__COLLECTION_HPP_
