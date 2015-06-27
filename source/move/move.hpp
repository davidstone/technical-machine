// Moves data structure
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

#pragma once

#include <bounded_integer/bounded_integer.hpp>
#include "moves.hpp"
#include "pp.hpp"

namespace technicalmachine {
using namespace bounded::literal;

struct Ability;

struct Move {
	static constexpr auto class_name = "Move";
	// move must not be Moves::END
	explicit Move(Moves move, Pp::pp_ups_type pp_ups = 3_bi);
	operator Moves() const;

	auto pp() const -> Pp const &;
	auto decrement_pp(Ability const & target) -> void;

private:
	Moves m_name;
	Pp m_pp;
};

auto operator==(Move const & lhs, Move const & rhs) -> bool;
auto operator!=(Move const & lhs, Move const & rhs) -> bool;

auto is_regular(Moves move) -> bool;

auto is_physical(Moves move) -> bool;
auto is_special(Moves move) -> bool;

auto is_damaging(Moves move) -> bool;
auto is_phaze(Moves name) -> bool;

auto is_usable_while_frozen(Moves move) -> bool;

inline auto hash(Move const & move) noexcept {
	return hash(static_cast<Moves>(move), move.pp());
}

}	// namespace technicalmachine
