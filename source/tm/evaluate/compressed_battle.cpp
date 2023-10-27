// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.compressed_battle;

import tm.evaluate.state;

import tm.compress;
import tm.generation;

import bounded;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
constexpr auto compress_battle(State<generation> const & state) {
	auto const compressed_ai = compress(state.ai);
	auto const compressed_foe = compress(state.foe);
	if constexpr (generation == Generation::one) {
		static_assert(tv::tuple_size<decltype(compressed_ai)> == 3_bi);
		static_assert(std::same_as<decltype(compressed_ai), decltype(compressed_foe)>);
		return tv::tuple(
			compressed_ai[0_bi],
			compressed_ai[1_bi],
			compress_combine(
				compressed_ai[2_bi],
				state.environment
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
				state.environment
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
using CompressedBattle = decltype(compress_battle(bounded::declval<State<generation>>()));

} // namespace technicalmachine
