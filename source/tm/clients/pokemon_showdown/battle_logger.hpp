// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/open_file.hpp>

#include <filesystem>
#include <fstream>

namespace technicalmachine {
namespace ps {

struct BattleLogger {
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

} // namespace ps
} // namespace technicalmachine
