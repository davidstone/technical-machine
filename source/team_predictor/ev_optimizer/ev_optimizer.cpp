// Optimize EVs and nature to remove waste
// Copyright (C) 2012 David Stone
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
#include <array>
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

constexpr unsigned max_evs = 508;

unsigned ev_sum(Pokemon const & pokemon) {
	auto const ev_sum = [&](unsigned const sum, Stat::Stats const stat) {
		return sum + get_stat(pokemon, stat).ev.value();
	};
	return std::accumulate(std::begin(regular_stats()), std::end(regular_stats()), 0u, ev_sum);
}

void add_non_full_stats(std::vector<Stat *> & stats, Stat & stat);

}	// unnamed namespace

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	minimize_evs(pokemon);
	while (ev_sum(pokemon) < max_evs) {
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
	while (ev_sum(pokemon) < max_evs) {
		std::vector<Stat *> stats;
		for (auto const stat : regular_stats()) {
			add_non_full_stats(stats, get_stat(pokemon, stat));
		}
		unsigned const extra_evs = max_evs - ev_sum(pokemon);
		std::vector<uint8_t> shuffled(extra_evs + stats.size() - 1, 1);
		std::fill(std::begin(shuffled), std::begin(shuffled) + static_cast<int>(stats.size()) - 1, 0);
		std::shuffle(std::begin(shuffled), std::end(shuffled), random_engine);
		auto it = shuffled.begin();
		for (auto const & stat : stats) {
			auto const prior = it;
			it = std::find(prior, std::end(shuffled), 0);
			stat->ev.add(static_cast<unsigned>(std::distance(prior, it)));
			++it;
		}
	}
	calculate_initial_hp(pokemon);
}

namespace {

void add_non_full_stats(std::vector<Stat *> & stats, Stat & stat) {
	if (!stat.ev.is_maxed()) {
		stats.emplace_back(&stat);
	}
}

}	// unnamed namespace

}	// namespace technicalmachine
