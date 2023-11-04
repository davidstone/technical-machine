// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_result;

import containers;
import tv;

namespace technicalmachine::ps {

export struct BattleContinues {
};

export struct BattleResponseNeeded {
	containers::string response;
};

export struct BattleStarted {
};

export struct BattleFinished {
};

export using BattleMessageResult = tv::variant<
	BattleContinues,
	BattleResponseNeeded,
	BattleStarted,
	BattleFinished
>;

} // namespace technicalmachine::ps
