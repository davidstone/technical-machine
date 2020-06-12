// All 'normal' stats that a Pokemon has
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
#include <tm/stat/hp.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Level;

struct Stats {
	Stats(Generation const generation, BaseStats const base, Level const level):
		m_hp(base, level, default_iv(generation), EV(0_bi)),
		m_stats{
			Stat(base.atk(), default_iv(generation), EV(0_bi)),
			Stat(base.def(), default_iv(generation), EV(0_bi)),
			Stat(base.spa(), default_iv(generation), EV(0_bi)),
			Stat(base.spd(), default_iv(generation), EV(0_bi)),
			Stat(base.spe(), default_iv(generation), EV(0_bi))
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
	containers::array<Stat, 5> m_stats;
};

}	// namespace technicalmachine
