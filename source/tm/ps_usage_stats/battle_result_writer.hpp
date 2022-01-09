// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/battle_result.hpp>

#include <filesystem>
#include <fstream>
#include <memory>

namespace technicalmachine::ps_usage_stats {

struct BattleResult;

struct BattleResultWriter {
	BattleResultWriter(std::filesystem::path const & path):
		m_file(path)
	{
		m_file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
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
