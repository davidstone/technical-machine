// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/stat_to_ev.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/initial_stat.hpp>
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
	constexpr auto generation = Generation::four;
	auto const base_stats = BaseStats(generation, Species::Charizard);
	constexpr auto level = Level(37_bi);
	constexpr auto iv = IV(13_bi);
	constexpr auto ev = EV(176_bi);
	constexpr auto nature = Nature::Adamant;
	constexpr auto stat_name = SplitSpecialRegularStat::atk;
	auto const stat_value = initial_stat<generation>(stat_name, base_stats.atk(), level, nature, iv, ev);
	auto const computed_ev = stat_to_ev(stat_value, stat_name, base_stats.atk(), level, nature, iv);
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
