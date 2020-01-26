// Collection of moves with index indicating current move
// Copyright (C) 2018 David Stone
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

#include <tm/move/container.hpp>
#include <tm/move/moves.hpp>

#include <tm/string_conversions/move.hpp>

#include <containers/algorithms/all_any_none.hpp>

#include <bounded/assert.hpp>

namespace technicalmachine {

namespace {

auto add_seen_move_impl(MoveContainer & container, Generation const generation, Moves const move, auto... maybe_pp) {
	if (move == Moves::Pass or move == Moves::Struggle or move == Moves::Hit_Self) {
		return;
	}
	if (containers::any_equal(container, move)) {
		return;
	}
	if (size(container.regular()) == max_moves_per_pokemon) {
		auto message = std::string("Tried to add too many moves. Already have: ");
		for (auto const existing_move : container.regular()) {
			message += to_string(existing_move.name());
			message += ", ";
		}
		message += "-- Tried to add ";
		message += to_string(move);
		throw std::runtime_error(message);
	}
	container.add(Move(generation, move, maybe_pp...));
}


} // namespace

auto MoveContainer::add(Move const move) -> Move & {
	BOUNDED_ASSERT(containers::none_equal(m_regular, move.name()));
	return containers::push_back(m_regular, move);
}



auto add_seen_move(MoveContainer & container, Generation const generation, Moves const move) -> void {
	add_seen_move_impl(container, generation, move);
}

auto add_seen_move(MoveContainer & container, Generation const generation, Moves const move, PP::pp_ups_type pp_ups) -> void {
	add_seen_move_impl(container, generation, move, pp_ups);
}

} // namespace technicalmachine