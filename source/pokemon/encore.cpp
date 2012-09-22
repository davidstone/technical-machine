// Encore class
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

#include "encore.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned inactive = 0xFF;
constexpr unsigned min_duration = 4;
constexpr unsigned max_duration = 8;
}	// unnamed namespace

Encore::Encore() :
	turns_active(inactive)
	{
}

bool Encore::is_active() const {
	return turns_active != inactive;
}

void Encore::activate() {
	turns_active = 0;
}

void Encore::increment() {
	if (!is_active())
		return;
	++turns_active;
	if (turns_active == max_duration)
		reset();
}

void Encore::reset() {
	turns_active = inactive;
}

Encore::hash_type Encore::hash() const {
	return turns_active;
}

Encore::hash_type Encore::max_hash() {
	return max_duration;
}

bool operator== (Encore const & lhs, Encore const & rhs) {
	return lhs.turns_active == rhs.turns_active;
}

bool operator!= (Encore const & lhs, Encore const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
