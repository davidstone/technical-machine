// class Move
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

#include "move.hpp"

#include <cstddef>
#include <cstdint>

#include "base_power.hpp"
#include "classification.hpp"
#include "moves.hpp"

namespace technicalmachine {

Move::Move(Moves const move, Pp::pp_ups_type const pp_ups) :
	m_name(move),
	m_pp(move, pp_ups) {
}

Move::operator Moves() const {
	return m_name;
}

auto Move::pp() const -> Pp const & {
	return m_pp;
}

auto Move::decrement_pp(Ability const & target) -> void {
	m_pp.decrement(target);
}

auto is_damaging(Moves const move) -> bool {
	auto const power = base_power(move);
	return !static_cast<bool>(power) or *power != 0_bi;
}

auto is_physical(Moves const move) -> bool {
	return Classification(move).is_physical();
}

auto is_special(Moves const move) -> bool {
	return Classification(move).is_special();
}

uint64_t Move::hash () const {
	return pp().hash() + pp().max_hash() * static_cast<uint64_t>(m_name);
}

uint64_t Move::max_hash() const {
	return pp().max_hash() * static_cast<uint64_t>(Moves::END);
}

auto operator==(Move const & lhs, Move const & rhs) -> bool {
	return
		static_cast<Moves>(lhs) == static_cast<Moves>(rhs) and
		lhs.pp() == rhs.pp()
	;
}

auto operator!=(Move const & lhs, Move const & rhs) -> bool {
	return !(lhs == rhs);
}

auto is_switch(Moves const move) -> bool {
	static_assert(static_cast<unsigned>(Moves::Switch0) == 0, "Switching is not the first Move enum.");
	return move <= Moves::Switch5;
}

auto is_phaze(Moves const move) -> bool {
	switch (move) {
		case Moves::Roar:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

auto is_usable_while_frozen(Moves const move) -> bool {
	switch (move) {
		case Moves::Flame_Wheel:
		case Moves::Sacred_Fire:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
