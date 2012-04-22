// Template specialization for Active
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

#ifndef ACTIVE_MOVE_HPP_
#define ACTIVE_MOVE_HPP_

#include "active.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include "move.hpp"
#include "reorder_moves.hpp"

namespace technicalmachine {

template<>
class Active<Move> : public detail::BaseActive<Move> {
	public:
		Active (unsigned const team_size) {
			container.reserve ((team_size > 1) ? team_size + 1 : 1);
			container.push_back (Move (Move::STRUGGLE, 0, 0));
			// A Pokemon has a new "Switch" move for each Pokemon in the party.
			if (team_size > 1) {
				for (unsigned count = 0; count != team_size; ++count) {
					container.push_back (Move (Move::from_replacement (count), 0, 0));
				}
			}
		}
		Active (typename detail::BaseActive<Move>::container_type const & pre_set):
			detail::BaseActive<Move>(pre_set) {
		}
		// Skips Struggle and switches
		void for_each_regular_move (typename std::function<void(Move &)> const & f) {
			for (auto move = container.begin(); move->name != Move::STRUGGLE; ++move) {
				f (*move);
			}
		}
		void for_each_regular_move (typename std::function<void(Move const &)> const & f) const {
			for (auto move = container.cbegin(); move->name != Move::STRUGGLE; ++move) {
				f (*move);
			}
		}
		bool regular_move_exists (typename std::function<bool(Move const &)> const & condition) const {
			return find_if(condition) != nullptr;
		}
		bool a_regular_move_is_selectable () const {
			return regular_move_exists ([](Move const & move) {
				return move.selectable;
			});
		}
		// Move::END if none
		Move::Moves name_of_last_used_move () const {
			Move const * move_ptr = find_if ([] (Move const & move) {
				return move.was_used_last();
			});
			return (move_ptr != nullptr) ? move_ptr->name : Move::END;
		}
		// nullptr if not found
		Move * find_if (typename std::function<bool(Move &)> const & condition) {
			for (auto move = container.begin(); move->name != Move::STRUGGLE; ++move) {
				if (condition (*move))
					return &*move;
			}
			return nullptr;
		}
		Move const * find_if (typename std::function<bool(Move const &)> const & condition) const {
			for (auto move = container.cbegin(); move->name != Move::STRUGGLE; ++move) {
				if (condition (*move))
					return &*move;
			}
			return nullptr;
		}
		std::vector<std::pair<int64_t, size_t>> create_ordered_container (bool const ai) const {
			return reorder (container, ai);
		}
};

}	// namespace technicalmachine
#endif	// ACTIVE_MOVE_HPP_
