// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.team_predictor.ev_optimizer.speed;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.nature_effect;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_style;

import tm.team_predictor.ev_optimizer.individual;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;
import tm.team_predictor.ev_optimizer.speed;

import tm.generation;

import bounded;
import containers;
import tv;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

namespace simple {
	constexpr auto generation = Generation::four;
	constexpr auto level = Level(100_bi);
	constexpr auto base_spe = 30_bi;
	constexpr auto original_value = initial_stat(base_spe, level, NatureEffect::neutral, IV(31_bi), EV(76_bi));
	constexpr auto possible_ivs = possible_optimized_ivs(possible_dvs_or_ivs(tv::optional<HiddenPower<generation>>()).spe());
	constexpr auto calculated = SpeedEVs(level, {base_spe, possible_ivs, original_value});

	constexpr auto expected = containers::array({
		Individual(NatureEffect::negative, IV(31_bi), EV(128_bi)),
		Individual(NatureEffect::negative, IV(30_bi), EV(132_bi)),
		Individual(NatureEffect::neutral, IV(31_bi), EV(76_bi)),
		Individual(NatureEffect::neutral, IV(30_bi), EV(80_bi)),
		Individual(NatureEffect::positive, IV(31_bi), EV(36_bi)),
		Individual(NatureEffect::positive, IV(30_bi), EV(40_bi)),
	});

	static_assert(containers::equal(calculated, expected));
}

} // namespace
} // namespace technicalmachine
