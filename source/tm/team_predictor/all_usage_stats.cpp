// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.team_predictor.all_usage_stats;

import tm.string_conversions.generation;

import tm.team_predictor.usage_stats;

import tm.generation;
import tm.get_directory;
import tm.open_file;

import bounded;
import containers;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export auto stats_for_generation(Generation const generation) -> UsageStats {
	auto const generation_str = to_string(generation);
	std::cout << "Loading stats for generation " << generation_str << '\n' << std::flush;
	return UsageStats::make(open_binary_file_for_reading(get_usage_stats_directory() / generation_str / "OU.tmus"));
}

export using StatsForGeneration = containers::trivial_inplace_function<UsageStats(Generation) const, 0>;

export struct AllUsageStats {
	explicit AllUsageStats(StatsForGeneration const get_stats_for_generation):
		m_all_stats{
			get_stats_for_generation(Generation::one),
			get_stats_for_generation(Generation::two),
			get_stats_for_generation(Generation::three),
			get_stats_for_generation(Generation::four),
			get_stats_for_generation(Generation::five),
			get_stats_for_generation(Generation::six),
			get_stats_for_generation(Generation::seven),
			get_stats_for_generation(Generation::eight)
		}
	{
		static_assert(numeric_traits::max_value<Generation> == Generation::eight);
		std::cout << "Done loading stats\n" << std::flush;
	}
	auto operator[](Generation const generation) const -> UsageStats const & {
		return m_all_stats[bounded::integer(generation) - 1_bi];
	}

private:
	containers::array<UsageStats, bounded::number_of<Generation>> m_all_stats;
};

} // namespace technicalmachine
