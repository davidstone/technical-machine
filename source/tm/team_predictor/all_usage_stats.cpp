// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

auto stats_for_generation(Generation const generation) -> UsageStats {
	return UsageStats::make(open_binary_file_for_reading(
		get_usage_stats_directory() / to_string(generation) / "OU.tmus"
	));
}

export using StatsForGeneration = containers::trivial_inplace_function<
	auto(Generation) const -> UsageStats,
	0
>;

export struct AllUsageStats {
	explicit AllUsageStats(StatsForGeneration const make):
		m_all_stats{
			make(Generation::one),
			make(Generation::two),
			make(Generation::three),
			make(Generation::four),
			make(Generation::five),
			make(Generation::six),
			make(Generation::seven),
			make(Generation::eight)
		}
	{
		static_assert(numeric_traits::max_value<Generation> == Generation::eight);
	}
	AllUsageStats():
		AllUsageStats(stats_for_generation)
	{
	}
	auto operator[](Generation const generation) const -> UsageStats const & {
		return m_all_stats[bounded::integer(generation) - 1_bi];
	}

private:
	containers::array<UsageStats, bounded::number_of<Generation>> m_all_stats;
};

} // namespace technicalmachine
