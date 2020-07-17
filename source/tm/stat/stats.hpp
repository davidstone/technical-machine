// Copyright (C) 2015 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
	Stats(BaseStats const base, CombinedStats<IVAndEV> const inputs, Level const level):
		m_hp(base, level, inputs.hp.iv, inputs.hp.ev),
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

	friend auto operator==(Stats const &, Stats const &) -> bool = default;
private:
	HP m_hp;
	containers::array<InitialStat, 5> m_stats;
};

inline auto initial_stats(BaseStats const base_stats, Level const level, CombinedStats<IVAndEV> const stats) {
	auto calculate_stat = [=](RegularStat const stat_name) {
		return initial_stat(stat_name, base_stats, stats, level);
	};
	return GenericStats<HP::max_type, InitialStat>{
		HP(base_stats, level, stats.hp.iv, stats.hp.ev).max(),
		calculate_stat(RegularStat::atk),
		calculate_stat(RegularStat::def),
		calculate_stat(RegularStat::spa),
		calculate_stat(RegularStat::spd),
		calculate_stat(RegularStat::spe),
	};
}

}	// namespace technicalmachine
