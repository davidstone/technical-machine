// Class that represents the duration left on Confusion
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

#include "confusion.hpp"
#include "pokemon.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned inactive = 0xFF;
constexpr unsigned min_duration = 2;
constexpr unsigned max_duration = 5;
}	// unnamed namespace

Confusion::Confusion() :
	turns_spent_confused(inactive),
	is_hitting_self(false)
	{
}

bool Confusion::is_active() const {
	return turns_spent_confused != inactive;
}

void Confusion::activate() {
	if (is_active())
		return;
	turns_spent_confused = 0;
}

namespace {
void register_self_hit(Pokemon & pokemon) {
	// TODO: write this
}
}	// unnamed namespace

void Confusion::do_turn(Pokemon & pokemon) {
	increment();
	if (is_active() and is_hitting_self)
		register_self_hit(pokemon);
}

void Confusion::increment() {
	if (!is_active())
		return;
	++turns_spent_confused;
	if (turns_spent_confused == max_duration)
		reset();
}

void Confusion::reset() {
	turns_spent_confused = inactive;
	is_hitting_self = false;
}

void Confusion::end_of_turn_reset() {
	is_hitting_self = false;
}

void Confusion::hit_self() {
	is_hitting_self = true;
}

Confusion::hash_type Confusion::hash() const {
	return turns_spent_confused;
}

Confusion::hash_type Confusion::max_hash() {
	return max_duration;
}

bool operator== (Confusion const & lhs, Confusion const & rhs) {
	return lhs.turns_spent_confused == rhs.turns_spent_confused;
}

bool operator!= (Confusion const & lhs, Confusion const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
