// Copyright (C) 2017 David Stone
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

#include "max_moves_per_pokemon.hpp"
#include "moves.hpp"
#include "pp.hpp"

#include "../operators.hpp"

#include <bounded/integer.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Move {
	constexpr explicit Move(Moves const move, Pp::pp_ups_type const pp_ups = 3_bi) :
		m_name(move),
		m_pp(move, pp_ups)
	{
	}

	constexpr operator Moves() const {
		return m_name;
	}

	constexpr auto pp() const {
		return m_pp;
	}

	constexpr auto decrement_pp(Ability const & target) {
		m_pp.decrement(target);
	}

private:
	Moves m_name;
	Pp m_pp;
};

constexpr auto compare(Move const lhs, Move const rhs) {
	BOUNDED_COMPARE_ONE_MEMBER(pp());
	return bounded::compare(static_cast<Moves>(lhs), static_cast<Moves>(rhs));
}
constexpr auto compare(Move const lhs, Moves const rhs) {
	return bounded::compare(static_cast<Moves>(lhs), rhs);
}
constexpr auto compare(Moves const lhs, Move const rhs) {
	return bounded::compare(lhs, static_cast<Moves>(rhs));
}

auto is_regular(Moves move) -> bool;

auto is_physical(Moves move) -> bool;
auto is_special(Moves move) -> bool;

auto is_damaging(Moves move) -> bool;
auto is_phaze(Moves name) -> bool;

auto is_usable_while_frozen(Moves move) -> bool;

using StaticVectorMove = containers::static_vector<Move, static_cast<std::intmax_t>(std::numeric_limits<MoveSize>::max())>;

}	// namespace technicalmachine
