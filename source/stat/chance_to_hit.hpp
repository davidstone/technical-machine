// Chance to hit calculations
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

#ifndef STAT__CHANCE_TO_HIT_HPP_
#define STAT__CHANCE_TO_HIT_HPP_

#include "../rational.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

class ActivePokemon;
class Weather;

namespace detail_chance_to_hit {

constexpr auto min = 0_bi;
constexpr auto max = 100_bi;

}	// namespace detail_chance_to_hit

using ChanceToHit = bounded_rational<
	bounded::integer<detail_chance_to_hit::min.value(), detail_chance_to_hit::max.value()>,
	bounded::integer<detail_chance_to_hit::max.value(), detail_chance_to_hit::max.value()>
>;

auto chance_to_hit(ActivePokemon const & user, ActivePokemon const & target, Weather weather, bool target_moved) -> ChanceToHit;

}	// namespace technicalmachine
#endif	// STAT__CHANCE_TO_HIT_HPP_
