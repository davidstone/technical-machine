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

#include <cstdint>
#include <bounded/integer.hpp>

#include "stat_names.hpp"

#include "../enum.hpp"
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

auto boosts_stat(Nature nature, StatNames stat) -> bool;
auto lowers_stat(Nature nature, StatNames stat) -> bool;

inline auto boost(Nature const nature, StatNames const stat) {
	auto const numerator =
		BOUNDED_CONDITIONAL(boosts_stat(nature, stat), 11_bi,
		BOUNDED_CONDITIONAL(lowers_stat(nature, stat), 9_bi,
		10_bi
	));
	static constexpr auto denominator = 10_bi;
	return make_rational(numerator, denominator);
}

auto boosts_attacking_stat(Nature nature) -> bool;
auto boosts_defending_stat(Nature nature) -> bool;
auto lowers_attacking_stat(Nature nature) -> bool;
auto lowers_defending_stat(Nature nature) -> bool;


}	// namespace technicalmachine

namespace std {

template<>
struct hash<technicalmachine::Nature> : public technicalmachine::std_hash<technicalmachine::Nature> {};

template<>
struct numeric_limits<technicalmachine::Nature> : technicalmachine::enum_numeric_limits<technicalmachine::Nature::END> {};

}	// namespace std

