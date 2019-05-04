// Copyright (C) 2019 David Stone
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

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/legacy_iterator.hpp>

#include <stdexcept>

namespace technicalmachine {
using namespace bounded::literal;

template<typename LHS, typename RHS>
constexpr auto round_up_divide(LHS const lhs, RHS const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

constexpr auto hp_to_ev(Species const species, Level const level, HP::max_type const stat) {
	auto const stat_range = containers::transform(ev_range(), [=](EV const ev) { return HP(species, level, ev).max(); });
	auto const it = std::lower_bound(containers::legacy_iterator(begin(stat_range)), containers::legacy_iterator(end(stat_range)), stat);
	if (it.base() == end(stat_range)) {
		throw std::runtime_error("No valid HP EV for a given stat value");
	}
	return *it.base().base();
}

// `target` is not just bounded::integer<4, 614> because this function is also
// used in the EV optimizer, where values outside the legal range are regularly
// encountered as part of speculative computation.
template<typename Integer>
constexpr auto stat_to_ev(Integer const target, Nature const nature, StatNames const stat_name, Stat::base_type const base, IV const iv, Level const level) {
	return bounded::max(0_bi, (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi);
}


using StatValue = bounded::integer<4, 614>;
constexpr auto calculate_evs(Species const species, Level const level, GenericStats<HP::max_type, StatValue> const stats) -> CombinedStats {
	// TODO: Give the correct IVs for the Hidden Power type
	
	auto base_stat = [=](StatNames const stat) { return Stat(species, stat).base(); };
	
	auto const base_attack = base_stat(StatNames::ATK);
	auto const base_defense = base_stat(StatNames::DEF);
	auto const base_special_attack = base_stat(StatNames::SPA);
	auto const base_special_defense = base_stat(StatNames::SPD);
	auto const base_speed = base_stat(StatNames::SPE);
	
	auto to_ev = [](auto const integer) { return EV(EV::value_type(integer)); };
	auto const hp_ev = hp_to_ev(species, level, stats.hp);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const attack_ev = stat_to_ev(stats.attack, nature, StatNames::ATK, base_attack, IV(31_bi), level);
		if (attack_ev > EV::max) {
			continue;
		}
		auto const defense_ev = stat_to_ev(stats.defense, nature, StatNames::DEF, base_defense, IV(31_bi), level);
		if (defense_ev > EV::max) {
			continue;
		}
		auto const special_attack_ev = stat_to_ev(stats.special_attack, nature, StatNames::SPA, base_special_attack, IV(31_bi), level);
		if (special_attack_ev > EV::max) {
			continue;
		}
		auto const special_defense_ev = stat_to_ev(stats.special_defense, nature, StatNames::SPD, base_special_defense, IV(31_bi), level);
		if (special_defense_ev > EV::max) {
			continue;
		}
		auto const speed_ev = stat_to_ev(stats.speed, nature, StatNames::SPE, base_speed, IV(31_bi), level);
		if (speed_ev > EV::max) {
			continue;
		}

		auto const combined = CombinedStats{nature, hp_ev, to_ev(attack_ev), to_ev(defense_ev), to_ev(special_attack_ev), to_ev(special_defense_ev), to_ev(speed_ev)};
		if (ev_sum(combined) > EV::max_total) {
			continue;
		}
		
		return combined;
	}
	throw std::runtime_error("No Nature + EV combination combines to give the received stats");
}

}	// namespace technicalmachine
