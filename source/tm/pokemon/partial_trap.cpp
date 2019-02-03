// Partial trap timer (things like Wrap and Clamp)
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

#include <tm/pokemon/partial_trap.hpp>
#include <tm/heal.hpp>
#include <tm/rational.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;
// constexpr auto normal_duration = 5_bi;
// constexpr auto extended_duration = 8_bi;
}	// namespace

auto PartialTrap::damage(MutableActivePokemon pokemon) -> void {
	if (is_active()) {
		heal(pokemon, rational(-1_bi, 16_bi));
		m_base.advance_one_turn();
	}
}

}	// namespace technicalmachine