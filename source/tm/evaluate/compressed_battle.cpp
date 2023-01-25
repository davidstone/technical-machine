// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.compressed_battle;

import tm.any_team;
import tm.compress;
import tm.generation;
import tm.team;
import tm.weather;

import bounded;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<any_team TeamType>
constexpr auto compress_battle(TeamType const & ai, TeamType const & foe, Weather const weather) {
	auto const compressed_ai = compress(ai);
	auto const compressed_foe = compress(foe);
	if constexpr (generation_from<TeamType> == Generation::one) {
		static_assert(tv::tuple_size<decltype(compressed_ai)> == 3_bi);
		static_assert(std::same_as<decltype(compressed_ai), decltype(compressed_foe)>);
		return tv::tuple(
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
		static_assert(tv::tuple_size<decltype(compressed_ai)> == 4_bi);
		static_assert(std::same_as<decltype(compressed_ai), decltype(compressed_foe)>);
		return tv::tuple(
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

export template<Generation generation>
using CompressedBattle = decltype(compress_battle(bounded::declval<Team<generation>>(), bounded::declval<Team<generation>>(), bounded::declval<Weather>()));

} // namespace technicalmachine
