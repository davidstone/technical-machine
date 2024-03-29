// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.battle_continues;

namespace technicalmachine {

export struct BattleContinues {
	friend auto operator==(BattleContinues, BattleContinues) -> bool = default;
};

} // namespace technicalmachine
