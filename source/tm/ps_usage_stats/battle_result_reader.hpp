// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/battle_result.hpp>

#include <containers/algorithms/generate.hpp>
#include <containers/array.hpp>

#include <bit>
#include <filesystem>
#include <fstream>

namespace technicalmachine::ps_usage_stats {

inline auto compute_number_of_battles(std::filesystem::path const & path) {
	auto const file_size = std::filesystem::file_size(path);
	constexpr auto element_size = bounded::size_of<BattleResult>;
	if (file_size % element_size != 0_bi) {
		throw std::runtime_error("Invalid file size");
	}
	return bounded::integer(file_size) / element_size;
}

inline auto battle_result_reader(std::filesystem::path const & path) {
	auto open_file = [&] {
		auto file = std::ifstream(path);
		file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		return file;
	};
	return containers::generate_n(
		compute_number_of_battles(path),
		[file = open_file()]() mutable {
			if (!file) {
				throw std::runtime_error("Inconsistent file size");
			}
			containers::array<std::byte, sizeof(BattleResult)> buffer;
			file.read(reinterpret_cast<char *>(std::addressof(buffer)), static_cast<std::streamsize>(sizeof(buffer)));
			return std::bit_cast<BattleResult>(buffer);
		}
	);
}

} // namespace technicalmachine::ps_usage_stats