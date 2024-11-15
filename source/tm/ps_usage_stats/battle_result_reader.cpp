// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.battle_result_reader;

import tm.ps_usage_stats.battle_result;

import tm.binary_file_reader;
import tm.open_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
using namespace bounded::literal;

auto compute_number_of_battles(std::filesystem::path const & path) {
	auto const file_size = std::filesystem::file_size(path);
	constexpr auto element_size = bounded::size_of<BattleResult>;
	if (file_size % element_size != 0_bi) {
		throw std::runtime_error("Invalid file size");
	}
	return bounded::integer(file_size) / element_size;
}

export auto battle_result_reader(std::filesystem::path const & path) {
	return containers::generate_n(
		compute_number_of_battles(path),
		[file = open_binary_file_for_reading(path)]() mutable {
			if (!file) {
				throw std::runtime_error("Inconsistent file size");
			}
			return std::bit_cast<BattleResult>(read_bytes(file, bounded::size_of<BattleResult>));
		}
	);
}

} // namespace technicalmachine::ps_usage_stats