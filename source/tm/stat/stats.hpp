// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Level;

struct Stats {
	Stats(BaseStats const base, CombinedStats const inputs, Level const level):
		m_hp(base, level, inputs.dvs_or_ivs.hp, inputs.evs.hp),
		m_stats{
			initial_stat(RegularStat::atk, base, inputs, level),
			initial_stat(RegularStat::def, base, inputs, level),
			initial_stat(RegularStat::spa, base, inputs, level),
			initial_stat(RegularStat::spd, base, inputs, level),
			initial_stat(RegularStat::spe, base, inputs, level)
		}
	{
	}

	auto const & hp() const {
		return m_hp;
	}
	auto & hp() {
		return m_hp;
	}

	auto const & operator[](RegularStat const stat) const {
		return m_stats[bounded::integer(stat)];
	}
	auto & operator[](RegularStat const stat) {
		return m_stats[bounded::integer(stat)];
	}

	friend auto operator==(Stats, Stats) -> bool = default;
private:
	HP m_hp;
	containers::array<InitialStat, 5> m_stats;
};

inline auto initial_stats(BaseStats const base_stats, Level const level, CombinedStats const stats) {
	auto calculate_stat = [=](RegularStat const stat_name) {
		return initial_stat(stat_name, base_stats, stats, level);
	};
	return GenericStats<HP::max_type, InitialStat>{
		HP(base_stats, level, stats.dvs_or_ivs.hp, stats.evs.hp).max(),
		calculate_stat(RegularStat::atk),
		calculate_stat(RegularStat::def),
		calculate_stat(RegularStat::spa),
		calculate_stat(RegularStat::spd),
		calculate_stat(RegularStat::spe),
	};
}

}	// namespace technicalmachine
