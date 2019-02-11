// Copyright (C) 2019 David Stone
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

#include <tm/wish.hpp>
#include <tm/heal.hpp>
#include <tm/rational.hpp>

namespace technicalmachine {
using namespace bounded::literal;

auto Wish::activate() -> void {
	if (!is_active()) {
		static constexpr auto turn_delay = 1_bi;
		m_turns_until_activation = counter_type(turn_delay);
	}
}

auto Wish::decrement(MutableActivePokemon pokemon) -> void {
	if (!m_turns_until_activation) {
		return;
	}
	if (*m_turns_until_activation == 0_bi) {
		m_turns_until_activation = bounded::none;
		heal(pokemon, rational(1_bi, 2_bi));
	} else {
		--*m_turns_until_activation;
	}
}

}	// namespace technicalmachine
