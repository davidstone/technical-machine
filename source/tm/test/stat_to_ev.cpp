// Copyright (C) 2020 David Stone
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

#include <tm/test/stat_to_ev.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

void test_hp() {
	auto const base_stats = BaseStats(Generation::four, Species::Charizard);
	constexpr auto level = Level(78_bi);
	constexpr auto iv = IV(29_bi);
	constexpr auto ev = EV(128_bi);
	auto const hp = HP(base_stats, level, iv, ev).max();
	auto const computed_ev = hp_to_ev(base_stats, level, hp, iv);
	BOUNDED_ASSERT(computed_ev == ev);
}

void test_attack() {
	auto const base_stats = BaseStats(Generation::four, Species::Charizard);
	constexpr auto level = Level(37_bi);
	constexpr auto iv = IV(13_bi);
	constexpr auto ev = EV(176_bi);
	constexpr auto nature = Nature::Adamant;
	constexpr auto stat_name = RegularStat::atk;
	auto const stat_value = initial_stat(stat_name, base_stats.atk(), nature, iv, ev, level);
	auto const computed_ev = stat_to_ev(stat_value, nature, stat_name, base_stats.atk(), iv, level);
	BOUNDED_ASSERT(computed_ev == ev);
}

} // namespace

void stat_to_ev_tests() {
	std::cout << "Stat to EV tests.\n";
	test_hp();
	test_attack();
	std::cout << "Stat to EV tests passed.\n\n";
}

} // namespace technicalmachine
