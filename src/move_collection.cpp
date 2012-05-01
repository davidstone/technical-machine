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

#include "move_collection.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include "move.hpp"
#include "reorder_moves.hpp"

namespace technicalmachine {

MoveCollection::MoveCollection (unsigned const team_size) {
	container.reserve ((team_size > 1) ? team_size + 1 : 1);
	add (Move (Move::STRUGGLE, 0));
	// A Pokemon has a new "Switch" move for each Pokemon in the party.
	if (team_size > 1) {
		for (unsigned count = 0; count != team_size; ++count) {
			add (Move (Move::from_replacement (count), 0));
		}
	}
}

unsigned MoveCollection::number_of_regular_moves () const {
	unsigned n = 0;
	while (operator()(n).name != Move::STRUGGLE)
		++n;
	return n;
}

// Skips Struggle and switches
void MoveCollection::for_each_regular_move (std::function<void(Move const &)> const & f) const {
	for (auto move = container.cbegin(); move->name != Move::STRUGGLE; ++move) {
		f (*move);
	}
}
void MoveCollection::for_each_regular_move (std::function<void(Move &)> const & f) {
	for (auto move = container.begin(); move->name != Move::STRUGGLE; ++move) {
		f (*move);
	}
}

bool MoveCollection::regular_move_exists (std::function<bool(Move const &)> const & condition) const {
	return find_if(condition) != nullptr;
}

bool MoveCollection::a_regular_move_is_selectable () const {
	return regular_move_exists ([](Move const & move) {
		return move.selectable;
	});
}

Move::Moves MoveCollection::name_of_last_used_move () const {
	Move const * move_ptr = find_if ([] (Move const & move) {
		return move.was_used_last();
	});
	return (move_ptr != nullptr) ? move_ptr->name : Move::END;
}

Move const * MoveCollection::find_if (std::function<bool(Move const &)> const & condition) const {
	for (auto move = container.cbegin(); move->name != Move::STRUGGLE; ++move) {
		if (condition (*move))
			return &*move;
	}
	return nullptr;
}
Move * MoveCollection::find_if (std::function<bool(Move &)> const & condition) {
	for (auto move = container.begin(); move->name != Move::STRUGGLE; ++move) {
		if (condition (*move))
			return &*move;
	}
	return nullptr;
}

std::vector<std::pair<int64_t, size_t>> MoveCollection::create_ordered_container (bool const ai) const {
	return reorder (container, ai);
}

std::vector<Move::Moves> MoveCollection::legal_switches (uint8_t const pokemon_index) const {
	std::vector<Move::Moves> switches;
	for (unsigned n = 0; n != container.size(); ++n) {
		if (n != pokemon_index)
			switches.push_back (Move::from_replacement(n));
	}
	return switches;
}

// Temporary function until I rework my detailed stats data structures
uint8_t MoveCollection::size () const {
	return container.size();
}

}	// namespace technicalmachine
