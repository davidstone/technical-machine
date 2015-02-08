// Hold all moves for easy use by certain moves that call other moves
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

#include "global_move.hpp"

#include "move.hpp"
#include "moves.hpp"

#include <bounded_integer/array.hpp>

#include <utility>

namespace technicalmachine {
namespace {

using underlying = std::underlying_type_t<Moves>;

template<underlying... integers>
auto all_moves(std::integer_sequence<underlying, integers...>) noexcept {
	return bounded::array<Move, number_of_moves.value()>{ Move(static_cast<Moves>(integers))... };
}

}	// namespace

auto global_move(Moves const name) -> Move const & {
	static auto const all = all_moves(std::make_integer_sequence<underlying, static_cast<underlying>(number_of_moves)>{});
	return all.at(name);
}

}	// namespace technicalmachine
