export module tm.stat.test.make_stats;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.stat_style;
import tm.stat.make_stats;

import tm.generation;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto stats = make_stats<StatStyle::gen1>(
	BaseStats(Generation::one, Species::Chansey),
	Level(100_bi),
	CombinedStatsFor<Generation::one>()
);

static_assert(stats.hp.max() == 703_bi);

} // namespace
} // namespace technicalmachine
