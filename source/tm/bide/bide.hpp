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

#include <tm/bide/damage.hpp>
#include <tm/bide/duration.hpp>

#include <tm/operators.hpp>
#include <tm/stat/hp.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Bide {
	constexpr auto add_damage(HP::current_type const damage) {
		m_damage.add(damage);
	}
	[[nodiscard]] constexpr auto advance_one_turn() {
		return BOUNDED_CONDITIONAL(m_duration.advance_one_turn(), m_damage.release(), bounded::none);
	}

	friend auto operator==(Bide const &, Bide const &) -> bool = default;

private:
	BideDamage m_damage;
	BideDuration m_duration;
};

}	// namespace technicalmachine
