// Nature functions
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

#ifndef STAT__NATURE_HPP_
#define STAT__NATURE_HPP_

#include <cstdint>
#include <bounded_integer/bounded_integer.hpp>

#include "stat_names.hpp"

#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded::literal;

enum class Nature : uint8_t {
	Adamant, Bashful, Bold, Brave, Calm,
	Careful, Docile, Gentle, Hardy, Hasty,
	Impish, Jolly, Lax, Lonely, Mild, Modest,
	Naive, Naughty, Quiet, Quirky, Rash,
	Relaxed, Sassy, Serious, Timid, END
};

auto make_nature(StatNames boost, StatNames drop) -> Nature;
auto is_set(Nature nature) -> bool;

template<StatNames>
auto boosts_stat(Nature nature) -> bool;
template<StatNames>
auto lowers_stat(Nature nature) -> bool;

template<StatNames stat>
auto boost(Nature const nature) {
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_stat<stat>(nature), 11_bi,
		BOUNDED_CONDITIONAL(lowers_stat<stat>(nature), 9_bi, 10_bi)
	);
	static constexpr auto denominator = 10_bi;
	return make_rational(numerator, denominator);
}

auto boosts_attacking_stat(Nature nature) -> bool;
auto boosts_defending_stat(Nature nature) -> bool;
auto lowers_attacking_stat(Nature nature) -> bool;
auto lowers_defending_stat(Nature nature) -> bool;


}	// namespace technicalmachine
#endif	// STAT__NATURE_HPP_
