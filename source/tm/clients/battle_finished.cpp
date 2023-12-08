// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.battle_finished;

namespace technicalmachine {

export struct BattleFinished {
	friend auto operator==(BattleFinished, BattleFinished) -> bool = default;
};

} // namespace technicalmachine
