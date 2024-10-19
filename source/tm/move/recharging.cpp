// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.recharging;

namespace technicalmachine {

export struct Recharging {
	friend auto operator==(Recharging, Recharging) -> bool = default;
};

} // namespace technicalmachine