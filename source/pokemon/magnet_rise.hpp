// Class that handles Magnet Rise
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

#ifndef Magnet_RISE_HPP_
#define Magnet_RISE_HPP_

#include "../hash.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

class MagnetRise {
private:
	using duration_type = bounded::integer<0, 5>;
public:
	auto is_active() const -> bool;
	auto activate() -> void;
	auto decrement() -> void;
	auto turns_remaining() const -> duration_type;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_turns_remaining);
	}
	constexpr auto max_hash() const noexcept {
		return technicalmachine::max_hash(m_turns_remaining);
	}
private:
	bounded::equivalent_type<duration_type, bounded::clamp_policy> m_turns_remaining = 0_bi;
};

bool operator== (MagnetRise lhs, MagnetRise rhs);
bool operator!= (MagnetRise lhs, MagnetRise rhs);

constexpr auto hash(MagnetRise magnet_rise) noexcept {
	return magnet_rise.hash();
}
constexpr auto max_hash(MagnetRise magnet_rise) noexcept {
	return magnet_rise.max_hash();
}


}	// namespace technicalmachine
#endif	// Magnet_RISE_HPP_
