// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ai.parse_args;

import tm.evaluate.depth;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto parse_args(int const argc, char const * const * const argv) -> Depth {
	auto const general_depth = (argc <= 1) ? 2_bi : bounded::to_integer<DepthInt>(argv[1]);
	auto const single_depth = (argc <= 2) ? 0_bi : bounded::to_integer<DepthInt>(argv[2]);
	return Depth(general_depth, single_depth);
}

} // namespace technicalmachine
