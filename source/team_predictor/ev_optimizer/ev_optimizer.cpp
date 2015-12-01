// Optimize EVs and nature to remove waste
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

#include "ev_optimizer.hpp"

#include "combine.hpp"
#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include "../../enum_range.hpp"

#include "../../pokemon/pokemon.hpp"

#include <containers/algorithms/count.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <utility>
#include <vector>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

EV::total_type ev_sum(Pokemon const & pokemon) {
	auto const ev_sum = [&](EV::total_type const sum, StatNames const stat) {
		return sum + get_stat(pokemon, stat).ev().value();
	};
	static constexpr auto regular = regular_stats();
	return std::accumulate(regular.begin(), regular.end(), EV::total_type(get_hp(pokemon).ev().value()), ev_sum);
}

}	// namespace

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	minimize_evs(pokemon);
	while (ev_sum(pokemon) < EV::max_total) {
		pad_random_evs(pokemon, random_engine);
		minimize_evs(pokemon);
	}
}

void minimize_evs(Pokemon & pokemon) {
	OffensiveEVs offensive(pokemon);
	DefensiveEVs defensive(pokemon);
	SpeedEVs speed(pokemon);
	combine(offensive, defensive, speed, pokemon);
}

void pad_random_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	// A value of 0 is the divider between stats. A value of 1 means to add one
	// more EV point (4 EVs) to the current stat. I use a random_shuffle to
	// create a uniform distribution of available stats non-maxed EVs. I repeat
	// the process in case a stat is overfilled.
	while (ev_sum(pokemon) < EV::max_total) {
		auto const hp_is_full = get_hp(pokemon).ev().value() == EV::max;
		auto const regular = regular_stats();
		auto checker = [&](auto const stat_name) { return get_stat(pokemon, stat_name).ev().value() != EV::max; };
		auto const non_full_stats =
			bounded::checked_integer<0, 5>(containers::count_if(regular.begin(), regular.end(), checker)) +
			BOUNDED_CONDITIONAL(hp_is_full, 0_bi, 1_bi);

		static constexpr auto number_of_stats = 6;
		static constexpr auto maximum_full_stats = 2;
		assert(non_full_stats >= bounded::constant<number_of_stats - maximum_full_stats>);

		auto const extra_evs = EV::max_total - ev_sum(pokemon);

		std::vector<bounded::integer<0, 1>> shuffled(static_cast<std::size_t>(extra_evs + non_full_stats - 1_bi), 1_bi);
		std::fill(shuffled.begin(), shuffled.begin() + static_cast<int>(non_full_stats - 1_bi), 0_bi);
		std::shuffle(shuffled.begin(), shuffled.end(), random_engine);

		auto find = [&](auto const it) { return std::find(it, shuffled.end(), 0_bi); };
		auto new_ev = [&](auto const stat, auto const distance) {
			return EV(bounded::clamped_integer<0, EV::max.value()>(distance + stat.ev().value().value()));
		};

		auto add_hp = [&]() {
			auto & hp = get_hp(pokemon);
			if (hp.ev().value() == EV::max) {
				return shuffled.begin();
			}
			auto const it = find(shuffled.begin());
			set_hp_ev(pokemon, new_ev(hp, it - shuffled.begin()));
			return it;
		};
		auto it = add_hp();
		if (it == shuffled.end()) {
			continue;
		}
		++it;
		for (auto const stat_name : enum_range<StatNames, StatNames::NORMAL_END>) {
			auto const previous = it;
			it = find(previous);
			// I use clamped here because I expect there to be some extra EVs
			// assigned to some stats, which is why I put this in a loop.
			set_stat_ev(pokemon, stat_name, new_ev(get_stat(pokemon, stat_name), it - previous));

			if (it == shuffled.end()) {
				break;
			}
			++it;
		}
		assert(it == shuffled.end());
	}
}

}	// namespace technicalmachine
