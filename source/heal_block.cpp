// Heal Block class
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

#include "heal_block.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_duration = 5;
}	// unnamed namespace

HealBlock::HealBlock() :
	turns_remaining(0)
	{
}

bool HealBlock::is_active() const {
	return turns_remaining != 0;
}

void HealBlock::activate() {
	turns_remaining = max_duration;
}

void HealBlock::decrement() {
	if (is_active())
		--turns_remaining;
}

void HealBlock::reset() {
	turns_remaining = 0;
}

HealBlock::hash_type HealBlock::hash() const {
	return turns_remaining;
}

HealBlock::hash_type HealBlock::max_hash() {
	return max_duration;
}

bool operator== (HealBlock const & lhs, HealBlock const & rhs) {
	return lhs.turns_remaining == rhs.turns_remaining;
}

bool operator!= (HealBlock const & lhs, HealBlock const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
