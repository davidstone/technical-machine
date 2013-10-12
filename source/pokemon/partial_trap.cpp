// Partial trap timer (things like Wrap and Clamp)
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

#include "partial_trap.hpp"
#include "pokemon.hpp"
#include "../heal.hpp"
#include "../rational.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned normal_duration = 5;
constexpr unsigned extended_duration = 8;
constexpr Rational end_of_turn_damage = Rational(1, 16);
}	// unnamed namespace

PartialTrap::PartialTrap():
	turns_active(0)
	{
}

bool PartialTrap::is_active() const {
	return turns_active != 0;
}

void PartialTrap::activate(bool const extended) {
	if (!is_active())
		turns_active = extended ? extended_duration : normal_duration;
}

void PartialTrap::damage(ActivePokemon & pokemon) {
	if (is_active()) {
		drain(pokemon, end_of_turn_damage);
		--turns_active;
	}
}

void PartialTrap::reset() {
	turns_active = 0;
}

PartialTrap::operator bool() const {
	return is_active();
}

PartialTrap::hash_type PartialTrap::hash() const {
	return turns_active;
}

PartialTrap::hash_type PartialTrap::max_hash() {
	return extended_duration;
}

bool operator== (PartialTrap const & lhs, PartialTrap const & rhs) {
	return lhs.turns_active == rhs.turns_active;
}
bool operator!= (PartialTrap const & lhs, PartialTrap const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
