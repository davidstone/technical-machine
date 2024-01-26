// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.pass;

namespace technicalmachine {

export struct Pass {
	friend constexpr auto operator==(Pass, Pass) -> bool = default;
};

export constexpr auto pass = Pass();

} // namespace technicalmachine
