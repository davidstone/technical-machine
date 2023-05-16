// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.battle_logger;

import tm.clients.ps.inmessage;

import tm.open_file;

import std_module;

namespace technicalmachine::ps {

export struct BattleLogger {
	explicit BattleLogger(std::filesystem::path const & path, std::string_view const battle_id):
		m_file(open_text_file_for_writing(path / "server_messages.txt"))
	{
		m_file << '>' << battle_id << '\n';
	}

	void log(InMessage const message) {
		m_file << '|' << message.type() << '|' << message.remainder() << '\n' << std::flush;
	}

private:
	std::ofstream m_file;
};

} // namespace technicalmachine::ps
