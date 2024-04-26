// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.team_predictor.ev_optimizer.individual;

import tm.pokemon.level;

import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature_effect;
import tm.stat.stat_to_ev;

import containers;
import std_module;
import tv;

namespace technicalmachine {

export struct Individual {
	NatureEffect nature_effect;
	IV iv;
	EV ev;
	friend auto operator==(Individual, Individual) -> bool = default;
};

export constexpr auto possible(Level const level, auto inputs, NatureEffect const effect, auto const target) {
	auto const pre_iv = stat_to_ev_at_least_pre_iv(target, inputs.base, level, effect);
	return containers::remove_none(containers::transform(
		std::move(inputs.ivs),
		[=](IV const iv) -> tv::optional<Individual> {
			auto const ev = pre_iv(iv);
			return BOUNDED_CONDITIONAL(ev, Individual(effect, iv, *ev), tv::none);
		}
	));
};

} // namespace technicalmachine
