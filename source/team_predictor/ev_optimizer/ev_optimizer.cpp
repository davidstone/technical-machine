// Optimize EVs and nature to remove waste
// Copyright (C) 2014 David Stone
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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>

#include "combine.hpp"
#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include "../../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
using namespace bounded_integer::literal;

using sum_type = bounded_integer::checked_integer<0, EV::max_total>;
sum_type ev_sum(Pokemon const & pokemon) {
	auto const ev_sum = [&](sum_type const sum, StatNames const stat) {
		return sum + get_stat(pokemon, stat).ev.value();
	};
	return std::accumulate(std::begin(regular_stats()), std::end(regular_stats()), sum_type(get_hp(pokemon).ev.value()), ev_sum);
}

void add_non_full_evs(std::vector<EV *> & evs, EV & ev);

}	// unnamed namespace

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
		std::vector<EV *> evs;
		add_non_full_evs(evs, get_hp(pokemon).ev);
		for (auto const stat : regular_stats()) {
			add_non_full_evs(evs, get_stat(pokemon, stat).ev);
		}
		auto const extra_evs = bounded_integer::make_bounded<EV::max_total>() - ev_sum(pokemon);
		static constexpr auto number_of_stats = 6;
		static constexpr auto maximum_full_stats = 2;
		bounded_integer::checked_integer<number_of_stats - maximum_full_stats, number_of_stats> size(evs.size());
		std::vector<bounded_integer::checked_integer<0, 1>> shuffled(extra_evs.value() + evs.size() - 1, 1_bi);
		std::fill(std::begin(shuffled), std::begin(shuffled) + static_cast<int>(evs.size()) - 1, 0_bi);
		std::shuffle(std::begin(shuffled), std::end(shuffled), random_engine);
		auto it = shuffled.begin();
		for (auto const & ev : evs) {
			auto const prior = it;
			it = std::find(prior, std::end(shuffled), 0_bi);
			// I use clamped here because I expect there to be some extra EVs
			// assigned to some stats, which is why I put this in a loop.
			ev->add(static_cast<bounded_integer::clamped_integer<0, EV::max>>(std::distance(prior, it)));
			++it;
		}
	}
}

namespace {

void add_non_full_evs(std::vector<EV *> & evs, EV & ev) {
	if (!ev.is_maxed()) {
		evs.emplace_back(&ev);
	}
}

}	// unnamed namespace

}	// namespace technicalmachine
