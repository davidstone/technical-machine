// Handles when Bide activates
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

#include "bide.hpp"

#include <cassert>

namespace technicalmachine {

BideDuration::BideDuration():
	turns_until_activation(0)
	{
}

namespace {
constexpr Bide::hash_type max_duration = 2;
}	// unnamed namespace

void BideDuration::activate() {
	turns_until_activation = max_duration;
}

BideDuration::operator bool() const {
	return turns_until_activation != 0;
}

bool BideDuration::decrement() {
	assert(this->operator bool());
	--turns_until_activation;
	return this->operator bool();
}

BideDuration::hash_type BideDuration::hash() const {
	return turns_until_activation;
}

BideDuration::hash_type BideDuration::max_hash() {
	return max_duration + 1;
}

bool operator== (BideDuration const & lhs, BideDuration const & rhs) {
	return lhs.turns_until_activation == rhs.turns_until_activation;
}

bool operator!= (BideDuration const & lhs, BideDuration const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
