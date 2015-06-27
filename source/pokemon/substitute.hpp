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

#pragma once

#include "../hash.hpp"
#include "../stat/hp.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

struct Substitute {
private:
	using hp_type = bounded::equivalent_type<decltype(std::declval<HP::current_type>() / 4_bi), bounded::null_policy>;
public:
	auto create(HP::current_type total_hp) -> hp_type;
	template<typename Damage>
	auto damage(Damage const damage_done) -> void {
		m_hp -= damage_done;
	}
	constexpr auto hp() const noexcept -> hp_type {
		return m_hp;
	}
	explicit operator bool() const;
private:
	bounded::equivalent_type<hp_type, bounded::clamp_policy> m_hp = 0_bi;
};
bool operator== (Substitute const & lhs, Substitute const & rhs);
bool operator!= (Substitute const & lhs, Substitute const & rhs);

constexpr auto hash(Substitute const substitute) noexcept {
	return hash(substitute.hp());
}


}	// namespace technicalmachine
