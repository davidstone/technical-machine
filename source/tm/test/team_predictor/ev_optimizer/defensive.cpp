// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

#include <bounded/assert.hpp>

export module tm.test.team_predictor.ev_optimizer.defensive;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.nature_effect;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_style;

import tm.team_predictor.ev_optimizer.defensive;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

TEST_CASE("Optimize defensive EVs", "[EV Optimizer]") {
	constexpr auto generation = Generation::four;
	constexpr auto base_hp = 100_bi;
	constexpr auto base_def = 100_bi;
	constexpr auto base_spd = 100_bi;
	constexpr auto level = Level(100_bi);
	constexpr auto possible_ivs = possible_dvs_or_ivs(tv::optional<HiddenPower<generation>>());
	constexpr auto hp_ev = EV(252_bi);
	constexpr auto defense_ev = EV(252_bi);
	constexpr auto special_defense_ev = EV(4_bi);
	constexpr auto hp = HP(base_hp, level, IV(31_bi), hp_ev).max();
	constexpr auto defense = initial_stat(base_def, level, NatureEffect::positive, IV(31_bi), defense_ev);
	constexpr auto special_defense = initial_stat(base_spd, level, NatureEffect::neutral, IV(31_bi), special_defense_ev);

	constexpr auto input_hp = DefensiveEVHP{base_hp, possible_optimized_ivs(possible_ivs.hp), hp};
	constexpr auto input_def = DefensiveEVDef{base_def, possible_optimized_ivs(possible_ivs.def), defense};
	constexpr auto input_spd = DefensiveEVSpD{base_spd, possible_optimized_ivs(possible_ivs.spd), special_defense};

	// Too many steps to constant evaluate
	auto const defensive_evs = DefensiveEVs(level, input_hp, input_def, input_spd);
	for (auto const & candidate : defensive_evs) {
		CHECK(candidate.hp.ev == hp_ev);
		CHECK(candidate.def.ev == defense_ev);
		CHECK(candidate.spd.ev >= special_defense_ev);
		CHECK(candidate.def.nature_effect == NatureEffect::positive);
		CHECK(candidate.spd.nature_effect == NatureEffect::neutral);
	}
}

} // namespace
} // namespace technicalmachine
