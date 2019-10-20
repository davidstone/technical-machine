// Copyright (C) 2018 David Stone
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

#include <tm/operators.hpp>
#include <tm/stat/hp.hpp>

#include <bounded/integer.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {
enum class Generation;
enum class Moves : std::uint16_t;
using namespace bounded::literal;

struct Substitute {
private:
	using hp_type = decltype(std::declval<HP::current_type>() / 4_bi);
public:
	constexpr auto create(HP::current_type const total_hp) -> hp_type {
		if (static_cast<bool>(*this)) {
			return 0_bi;
		}
		m_hp = total_hp / 4_bi;
		return m_hp;
	}

	constexpr auto damage(auto const damage_done) {
		m_hp -= damage_done;
	}

	constexpr auto hp() const -> hp_type {
		return m_hp;
	}

	explicit constexpr operator bool() const {
		return hp() != 0_bi;
	}

private:
	// TODO: Use change_policy
	bounded::clamped_integer<
		static_cast<int>(bounded::min_value<hp_type>),
		static_cast<int>(bounded::max_value<hp_type>)
	> m_hp = 0_bi;
};

constexpr auto operator==(Substitute const lhs, Substitute const rhs) {
	return lhs.hp() == rhs.hp();
}

auto blocked_by_substitute(Generation generation, Moves move) -> bool;
auto damage_blocked_by_substitute(Generation generation, Moves move) -> bool;

}	// namespace technicalmachine
