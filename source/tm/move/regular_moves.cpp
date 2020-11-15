// Copyright (C) 2020 David Stone
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

#include <tm/move/regular_moves.hpp>
#include <tm/move/moves.hpp>

#include <tm/string_conversions/move.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/push_back.hpp>

#include <bounded/assert.hpp>

namespace technicalmachine {

auto RegularMoves::push_back(Move const move) -> Move & {
	if (!is_regular(move.name())) {
		throw std::runtime_error(std::string("Tried to add an irregular move ") + std::string(to_string(move.name())));
	}
	if (containers::any_equal(m_moves, move.name())) {
		throw std::runtime_error("Tried to add " + std::string(to_string(move.name())) + "twice");
	}
	if (containers::size(m_moves) == max_moves_per_pokemon) {
		auto message = std::string("Tried to add too many moves. Already have: ");
		for (auto const existing_move : m_moves) {
			message += to_string(existing_move.name());
			message += ", ";
		}
		message += "-- Tried to add ";
		message += to_string(move.name());
		throw std::runtime_error(message);
	}
	return containers::push_back(m_moves, move);
}

auto add_seen_move(RegularMoves & moves, Generation const generation, Moves const move_name) -> void {
	if (containers::any_equal(moves, move_name) or !is_regular(move_name)) {
		return;
	}
	moves.push_back(Move(generation, move_name));
}

} // namespace technicalmachine