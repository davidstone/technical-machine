// Class that handles Magnet Rise
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

#include "magnet_rise.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_duration = 5;
}	// unnamed namespace

MagnetRise::MagnetRise() :
	turns_remaining(0)
	{
}

bool MagnetRise::is_active() const {
	return turns_remaining != 0;
}

void MagnetRise::activate() {
	if (!is_active())
		turns_remaining = max_duration;
}

void MagnetRise::decrement() {
	if (is_active())
		--turns_remaining;
}

void MagnetRise::reset() {
	turns_remaining = 0;
}

MagnetRise::hash_type MagnetRise::hash() const {
	return turns_remaining;
}

MagnetRise::hash_type MagnetRise::max_hash() {
	return max_duration;
}

bool operator== (MagnetRise const & lhs, MagnetRise const & rhs) {
	return lhs.turns_remaining == rhs.turns_remaining;
}

bool operator!= (MagnetRise const & lhs, MagnetRise const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
