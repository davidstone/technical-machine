// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

#include <bounded/assert.hpp>

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.string_conversions.nature;

import tm.team_predictor.ev_optimizer.compute_minimal_spread;
import tm.team_predictor.ev_optimizer.defensive;
import tm.team_predictor.ev_optimizer.offensive;
import tm.team_predictor.ev_optimizer.optimize_evs;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;
import tm.team_predictor.ev_optimizer.speed;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

TEST_CASE("Optimize already optimized EVs", "[EV Optimizer]") {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Metagross;
	constexpr auto level = Level(100_bi);
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>();
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = false;
	constexpr auto ivs_and_evs = CombinedStatsFor<generation>{
		Nature::Adamant,
		max_dvs_or_ivs<special_style_for(generation)>,
		EVs(
			EV(252_bi),
			EV(96_bi),
			EV(96_bi),
			EV(0_bi),
			EV(4_bi),
			EV(60_bi)
		)
	};
	constexpr auto base_stats = BaseStats(generation, species);
	constexpr auto stats = Stats<stat_style_for(generation)>(base_stats, level, ivs_and_evs);

	CHECK(compute_minimal_spread(base_stats, stats, level, hidden_power, include_attack, include_special_attack) == ivs_and_evs);
	auto const optimized = optimize_evs(ivs_and_evs, species, level, hidden_power, include_attack, include_special_attack);
	CHECK(optimized == ivs_and_evs);
}

TEST_CASE("Optimize EVs below level 100", "[EV Optimizer]") {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Masquerain;
	constexpr auto level = Level(83_bi);
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>();
	constexpr auto include_attack = false;
	constexpr auto include_special_attack = true;
	constexpr auto ivs_and_evs = CombinedStatsFor<generation>{
		Nature::Modest,
		max_dvs_or_ivs<special_style_for(generation)>,
		EVs(
			EV(192_bi),
			EV(0_bi),
			EV(8_bi),
			EV(120_bi),
			EV(60_bi),
			EV(128_bi)
		)
	};
	[[maybe_unused]] auto const optimized = optimize_evs(ivs_and_evs, species, level, hidden_power, include_attack, include_special_attack);
}

TEST_CASE("Optimize generation 2 EVs", "[EV Optimizer]") {
	constexpr auto generation = Generation::two;
	constexpr auto species = Species::Mew;
	constexpr auto level = Level(100_bi);
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>();
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = true;
	constexpr auto ivs_and_evs = CombinedStatsFor<generation>();
	auto const base_stats = BaseStats(generation, species);
	auto const stats = Stats<stat_style_for(generation)>(base_stats, level, ivs_and_evs);
	CHECK(compute_minimal_spread(base_stats, stats, level, hidden_power, include_attack, include_special_attack) == ivs_and_evs);
	auto const optimized = optimize_evs(ivs_and_evs, species, level, hidden_power, include_attack, include_special_attack);
	CHECK(optimized == ivs_and_evs);
}

} // namespace
} // namespace technicalmachine
