// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.battle_result_writer;

import tm.ps_usage_stats.battle_result;

import tm.open_file;

import std_module;

namespace technicalmachine::ps_usage_stats {

export struct BattleResultWriter {
	BattleResultWriter(std::filesystem::path const & path):
		m_file(open_binary_file_for_writing(path))
	{
	}

	auto operator()(BattleResult const & battle) -> void {
		static_assert(std::is_standard_layout_v<BattleResult>);
		static_assert(std::is_trivially_copyable_v<BattleResult>);
		m_file.write(reinterpret_cast<char const *>(std::addressof(battle)), sizeof(battle));
	}

private:
	std::ofstream m_file;
};

} // namespace technicalmachine::ps_usage_stats
