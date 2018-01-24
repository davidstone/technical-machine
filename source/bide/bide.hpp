// Handles bide damage and when it activates
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

#include "damage.hpp"
#include "duration.hpp"

#include "../operators.hpp"
#include "../stat/hp.hpp"

namespace technicalmachine {

struct Bide {
	constexpr auto is_active() const {
		return static_cast<bool>(m_duration);
	}
	constexpr auto activate() {
		m_duration.activate();
	}
	constexpr auto add_damage(damage_type const damage) {
		if (is_active()) {
			m_damage.add(damage);
		}
	}
	constexpr auto decrement() {
		return BOUNDED_CONDITIONAL(m_duration.decrement(), m_damage.release(), 0_bi);
	}

	friend constexpr auto compare(Bide const lhs, Bide const rhs) {
		auto as_tuple = [](auto const value) {
			return containers::make_tuple(value.m_damage, value.m_duration);
		};
		return compare(as_tuple(lhs), as_tuple(rhs));
	}

private:
	BideDamage m_damage;
	BideDuration m_duration;
};

}	// namespace technicalmachine
