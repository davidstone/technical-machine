// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.pad_evs;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.stat_names;
import tm.stat.stat_style;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto pad_evs(OldGenEVs evs, bool const include_attack, bool) -> OldGenEVs {
	for (auto const stat_name : containers::enum_range<SpecialPermanentStat>()) {
		auto const minimize_stat = stat_name == SpecialPermanentStat::atk and !include_attack;
		evs[stat_name] = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
	}
	return evs;
}

constexpr auto is_not_full(EV const ev) -> bool {
	return ev < EV::useful_max;
}

constexpr auto get_not_full(EV const ev) {
	return is_not_full(ev) ? ev.value() : 0_bi;
}

constexpr auto add_non_full(auto const... evs) {
	return (... + get_not_full(evs));
}

constexpr auto minimal_increment = 4_bi;

constexpr auto useful_value(auto const value) {
	return EV(bounded::assume_in_range<EV::value_type>(value - value % minimal_increment));
}

constexpr auto max = max_total_evs(SpecialStyle::split);

constexpr auto greatest_non_full(EVs & evs) -> EV & {
	auto ref = std::ref(evs.hp());
	auto update = [&](EV & ev) {
		if (ref.get() == EV::useful_max or ev > ref.get()) {
			ref = std::ref(ev);
		}
	};
	update(evs.atk());
	update(evs.def());
	update(evs.spa());
	update(evs.spd());
	update(evs.spe());
	BOUNDED_ASSERT(ref.get() < EV::useful_max);
	return ref.get();
}

constexpr auto used_non_full_evs(EVs const evs) {
	return add_non_full(
		evs.hp(),
		evs.atk(),
		evs.def(),
		evs.spa(),
		evs.spd(),
		evs.spe()
	);
}

constexpr auto proportionate_value(EV const ev, auto const remaining_evs, auto const used_non_full) {
	return ev.value() + ev.value() * remaining_evs  / bounded::increase_min<1>(used_non_full);
}

constexpr auto fill_evs_that_should_fill(EVs evs) -> EVs {
	constexpr auto max_full_evs = max / EV::useful_max;
	for ([[maybe_unused]] auto const _ : containers::integer_range(max_full_evs)) {
		auto const used_non_full = used_non_full_evs(evs);
		auto const all_are_full_or_empty = used_non_full == 0_bi;
		if (all_are_full_or_empty) {
			break;
		}
		auto & ev = greatest_non_full(evs);
		auto const remaining_evs = max - ev_sum(evs);
		auto const new_value = proportionate_value(ev, remaining_evs, used_non_full);
		if (new_value < EV::useful_max) {
			break;
		}
		ev = EV(EV::useful_max);
	}
	return evs;
}

constexpr auto assign_evs_proportionally_to_current(EVs evs, bool const include_attack, bool const include_special_attack) {
	evs = fill_evs_that_should_fill(evs);
	auto const remaining_evs = max - ev_sum(evs);
	auto const used_non_full = used_non_full_evs(evs);
	auto const all_are_full_or_empty = used_non_full == 0_bi;
	if (all_are_full_or_empty) {
		return evs;
	}
	auto update = [&](EV & ev) {
		if (ev == EV::useful_max) {
			return;
		}
		ev = useful_value(proportionate_value(ev, remaining_evs, used_non_full));
	};
	update(evs.hp());
	if (include_attack) {
		update(evs.atk());
	}
	update(evs.def());
	if (include_special_attack) {
		update(evs.spa());
	}
	update(evs.spd());
	update(evs.spe());
	return evs;
}

constexpr auto fill_in_empty_evs_and_fix_rounding(EVs evs, bool const include_attack, bool const include_special_attack) -> EVs {
	auto evs_to_add_to =
		bounded::integer(is_not_full(evs.hp())) +
		(include_attack ? bounded::integer(is_not_full(evs.atk())) : 0_bi) +
		bounded::integer(is_not_full(evs.def())) +
		(include_special_attack ? bounded::integer(is_not_full(evs.spa())) : 0_bi) +
		bounded::integer(is_not_full(evs.spd())) +
		bounded::integer(is_not_full(evs.spe()));
	auto update = [&](EV & ev) {
		if (ev == EV::useful_max) {
			return;
		}
		auto const remaining_evs = max - ev_sum(evs);
		ev = useful_value(ev.value() + remaining_evs / bounded::increase_min<1>(evs_to_add_to));
		--evs_to_add_to;
	};
	if (include_special_attack) {
		update(evs.spa());
	}
	if (include_attack) {
		update(evs.atk());
	}
	update(evs.spd());
	update(evs.def());
	update(evs.hp());
	update(evs.spe());
	BOUNDED_ASSERT(ev_sum(evs) + minimal_increment > max);
	return evs;
}

export constexpr auto pad_evs(EVs const evs, bool const include_attack, bool const include_special_attack) -> EVs {
	auto const updated = assign_evs_proportionally_to_current(evs, include_attack, include_special_attack);
	return fill_in_empty_evs_and_fix_rounding(updated, include_attack, include_special_attack);
}

} // namespace technicalmachine
