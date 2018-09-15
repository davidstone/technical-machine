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

#include "../../pokemon/pokemon.hpp"

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/count.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/static_vector/make_static_vector.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <utility>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto ev_sum(Pokemon const & pokemon) {
	auto const ev_value = [&](StatNames const stat) {
		return get_stat(pokemon, stat).ev().value();
	};
	return EV::total_type(containers::accumulate(
		containers::transform(regular_stats(), ev_value),
		get_hp(pokemon).ev().value()
	));
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
		auto checker = [&](auto const stat_name) { return get_stat(pokemon, stat_name).ev().value() == EV::max; };
		auto const full_stats = bounded::checked_integer<0, 2>(
			containers::count_if(regular_stats(), checker) +
			BOUNDED_CONDITIONAL(hp_is_full, 1_bi, 0_bi)
		);

		constexpr auto number_of_stats = 6_bi;

		auto const extra_evs = EV::max_total - ev_sum(pokemon);

		auto const dividers = number_of_stats - full_stats - 1_bi;
		auto shuffled = containers::static_vector(extra_evs + dividers, static_cast<bounded::integer<0, 1>>(1_bi));
		std::fill(begin(shuffled), begin(shuffled) + dividers, 0_bi);
		std::shuffle(data(shuffled), data(shuffled) + size(shuffled), random_engine);

		auto find = [&](auto const it) { return containers::find(it, end(shuffled), 0_bi); };
		auto new_ev = [&](auto const stat, auto const distance) {
			return EV(bounded::clamped_integer<0, EV::max.value()>(distance + stat.ev().value().value()));
		};

		auto add_hp = [&]() {
			auto & hp = get_hp(pokemon);
			if (hp.ev().value() == EV::max) {
				return begin(shuffled);
			}
			auto const it = find(begin(shuffled));
			set_hp_ev(pokemon, new_ev(hp, it - begin(shuffled)));
			return it;
		};
		auto it = add_hp();
		if (it == end(shuffled)) {
			continue;
		}
		++it;
		for (auto const stat_name : containers::enum_range(StatNames::NORMAL_END)) {
			auto const previous = it;
			it = find(previous);
			// I use clamped here because I expect there to be some extra EVs
			// assigned to some stats, which is why I put this in a loop.
			set_stat_ev(pokemon, stat_name, new_ev(get_stat(pokemon, stat_name), it - previous));

			if (it == end(shuffled)) {
				break;
			}
			++it;
		}
		assert(it == end(shuffled));
	}
}

}	// namespace technicalmachine
