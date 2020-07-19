// Copyright (C) 2020 David Stone
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
#include <tm/generation.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

template<Generation generation>
constexpr auto compress_battle(Team<generation> const & ai, Team<generation> const & foe, Weather const weather) {
	auto const compressed_ai = compress(ai);
	auto const compressed_foe = compress(foe);
	if constexpr (generation == Generation::one) {
		static_assert(bounded::tuple_size<decltype(compressed_ai)> == 3_bi);
		static_assert(std::is_same_v<decltype(compressed_ai), decltype(compressed_foe)>);
		return bounded::tuple(
			compressed_ai[0_bi],
			compressed_ai[1_bi],
			compress_combine(
				compressed_ai[2_bi],
				weather
			),
			compressed_foe[0_bi],
			compressed_foe[1_bi],
			compressed_foe[2_bi]
		);
	} else {
		static_assert(bounded::tuple_size<decltype(compressed_ai)> == 4_bi);
		static_assert(std::is_same_v<decltype(compressed_ai), decltype(compressed_foe)>);
		return bounded::tuple(
			compressed_ai[0_bi],
			compressed_ai[1_bi],
			compress_combine(
				compressed_ai[2_bi],
				weather
			),
			compressed_ai[3_bi],
			compressed_foe[0_bi],
			compressed_foe[1_bi],
			compressed_foe[2_bi],
			compressed_foe[3_bi]
		);
	}
}

template<Generation generation>
using CompressedBattle = decltype(compress_battle(std::declval<Team<generation>>(), std::declval<Team<generation>>(), std::declval<Weather>()));

} // namespace technicalmachine
