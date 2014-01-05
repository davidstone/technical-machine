// class Move
// Copyright (C) 2013 David Stone
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

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "base_power.hpp"
#include "classification.hpp"
#include "moves.hpp"

namespace technicalmachine {

Move::Move(Moves const move, Pp::pp_ups_type const pp_ups) :
	m_name(move),
	pp(move, pp_ups) {
}

Move::operator Moves() const {
	return m_name;
}

bool is_damaging(Moves const move) {
	auto const power = base_power(move);
	return !static_cast<bool>(power) or *power != 0_bi;
}

bool is_physical(Moves const move) {
	return Classification(move).is_physical();
}

bool is_special(Moves const move) {
	return Classification(move).is_special();
}

uint64_t Move::hash () const {
	return pp.hash() + pp.max_hash() * static_cast<uint64_t>(m_name);
}

uint64_t Move::max_hash() const {
	return pp.max_hash() * static_cast<uint64_t>(Moves::END);
}

bool operator== (Move const & lhs, Move const & rhs) {
	return lhs.m_name == rhs.m_name and
			lhs.pp == rhs.pp;
}

bool operator!= (Move const & lhs, Move const & rhs) {
	return !(lhs == rhs);
}

bool is_switch(Moves const move) {
	static_assert(static_cast<unsigned>(Moves::Switch0) == 0, "Switching is not the first Move enum.");
	return move <= Moves::Switch5;
}

Moves from_replacement(unsigned const replacement) {
	return static_cast<Moves>(replacement + static_cast<unsigned>(Moves::Switch0));
}

unsigned to_replacement(Moves const move) {
	assert(is_switch(move));
	return static_cast<unsigned>(move) - static_cast<unsigned>(Moves::Switch0);
}

bool is_phaze(Moves const move) {
	switch (move) {
		case Moves::Roar:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

bool is_usable_while_frozen(Moves const move) {
	switch (move) {
		case Moves::Flame_Wheel:
		case Moves::Sacred_Fire:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
