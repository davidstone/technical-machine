// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_interface;

import tm.clients.ps.inmessage;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {

export struct BattleInterface {
	virtual auto id() const -> std::string_view = 0;
	virtual auto handle_message(InMessage message) -> tv::optional<containers::string> = 0;
	enum class Complete {
		none,
		start,
		finish
	};
	virtual auto completed() const -> Complete = 0;
	virtual ~BattleInterface() = default;
};

} // namespace technicalmachine::ps
