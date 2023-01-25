// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.depth;

import bounded;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

export using DepthInt = bounded::integer<0, 10'000>;

export struct Depth {
	constexpr Depth(DepthInt const general_, DepthInt const single_):
		general(general_),
		single(single_)
	{
	}
	DepthInt general;
	DepthInt single;
	friend constexpr auto operator<=>(Depth, Depth) = default;
};

export constexpr auto one_level_deeper(Depth depth) -> Depth {
	if (depth.general > 0_bi) {
		--depth.general;
	} else if (depth.single > 0_bi) {
		--depth.single;
	} else {
		std::unreachable();
	}
	return depth;
}

} // namespace technicalmachine
