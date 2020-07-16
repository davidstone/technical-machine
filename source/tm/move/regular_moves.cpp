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

#include <bounded/assert.hpp>

namespace technicalmachine {

auto RegularMoves::push_back(Move const move) -> Move & {
	BOUNDED_ASSERT(containers::none_equal(m_moves, move.name()));
	return containers::push_back(m_moves, move);
}

namespace {

auto add_seen_move_impl(RegularMoves & moves, Generation const generation, Moves const move_name, auto... maybe_pp) {
	switch (move_name) {
		case Moves::Pass:
		case Moves::Struggle:
		case Moves::Hit_Self:
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return;
		default:
			break;
	}
	if (containers::any_equal(moves, move_name)) {
		return;
	}
	if (containers::size(moves) == max_moves_per_pokemon) {
		auto message = std::string("Tried to add too many moves. Already have: ");
		for (auto const existing_move : moves) {
			message += to_string(existing_move.name());
			message += ", ";
		}
		message += "-- Tried to add ";
		message += to_string(move_name);
		throw std::runtime_error(message);
	}
	moves.push_back(Move(generation, move_name, maybe_pp...));
}

} // namespace

auto add_seen_move(RegularMoves & moves, Generation const generation, Moves const move_name) -> void {
	add_seen_move_impl(moves, generation, move_name);
}

auto add_seen_move(RegularMoves & moves, Generation const generation, Moves const move_name, PP::pp_ups_type pp_ups) -> void {
	add_seen_move_impl(moves, generation, move_name, pp_ups);
}

} // namespace technicalmachine