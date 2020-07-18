// Copyright (C) 2016 David Stone
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

#include <tm/compress.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/optional.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation>
struct MutableActivePokemon;

struct Wish {
	constexpr auto is_active() const {
		return static_cast<bool>(m_turns_until_activation);
	}
	constexpr auto activate() & -> void {
		if (!is_active()) {
			constexpr auto turn_delay = 1_bi;
			m_turns_until_activation = counter_type(turn_delay);
		}
	}

	template<Generation generation>
	auto decrement(MutableActivePokemon<generation> pokemon, Weather const weather) & -> void {
		if (!m_turns_until_activation) {
			return;
		}
		if (*m_turns_until_activation == 0_bi) {
			m_turns_until_activation = bounded::none;
			heal(pokemon, weather, rational(1_bi, 2_bi));
		} else {
			--*m_turns_until_activation;
		}
	}

	friend auto operator==(Wish const &, Wish const &) -> bool = default;
	
private:
	using counter_type = bounded::checked_integer<0, 1>;
	bounded::optional<counter_type> m_turns_until_activation = bounded::none;
};

constexpr auto compress(Wish const value) {
	return compress(value.is_active());
}

}	// namespace technicalmachine
