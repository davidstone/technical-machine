// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

template<any_team TeamType>
constexpr auto compress_battle(TeamType const & ai, TeamType const & foe, Weather const weather) {
	auto const compressed_ai = compress(ai);
	auto const compressed_foe = compress(foe);
	if constexpr (generation_from<TeamType> == Generation::one) {
		static_assert(bounded::tuple_size<decltype(compressed_ai)> == 3_bi);
		static_assert(std::same_as<decltype(compressed_ai), decltype(compressed_foe)>);
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
		static_assert(std::same_as<decltype(compressed_ai), decltype(compressed_foe)>);
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
