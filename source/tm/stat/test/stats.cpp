// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.test.stats;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.generation;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto stats = Stats<StatStyle::gen1>(
	BaseStats(Generation::one, Species::Chansey),
	Level(100_bi),
	CombinedStatsFor<Generation::one>()
);

static_assert(stats.hp().max() == 703_bi);

} // namespace
} // namespace technicalmachine
