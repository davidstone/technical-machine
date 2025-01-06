// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stats;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;

import tm.exists_if;

import bounded;
import containers;

namespace technicalmachine {

export template<StatStyle stat_style>
struct Stats {
private:
	using Stat = InitialStat<special_style(stat_style)>;

public:
	constexpr Stats(HP const hp_, Stat const atk_, Stat const def_, Stat const spa_, Stat const spd_, Stat const spe_):
		m_hp(hp_),
		m_atk(atk_),
		m_def(def_),
		m_spa(spa_),
		m_spd(spd_),
		m_spe(spe_)
	{
	}
	constexpr Stats(BaseStats const base, Level const level, CombinedStats<special_style(stat_style)> const inputs):
		Stats(
			base.hp(),
			level,
			IV(inputs.dvs_or_ivs.hp()),
			inputs.evs.hp(),
			[=](SplitSpecialRegularStat const stat_name) {
				return initial_stat(
					base[stat_name],
					level,
					to_nature_effect(inputs.nature, stat_name),
					IV(inputs.dvs_or_ivs[stat_name]),
					inputs.evs[stat_name]
				);
			}
		)
	{
	}

	constexpr auto const & hp() const & {
		return m_hp;
	}
	constexpr auto & hp() & {
		return m_hp;
	}
	constexpr auto atk() const {
		return m_atk;
	}
	constexpr auto def() const {
		return m_def;
	}
	constexpr auto spa() const {
		return m_spa;
	}
	constexpr auto spd() const {
		if constexpr (stat_style == StatStyle::gen1) {
			return m_spa;
		} else {
			return m_spd;
		}
	}
	constexpr auto spe() const {
		return m_spe;
	}
	constexpr auto operator[](SplitSpecialRegularStat const stat_name) const {
		return index_stat(*this, stat_name);
	}

	friend auto operator==(Stats, Stats) -> bool = default;

private:
	constexpr Stats(BaseStats::HP const base_hp, Level const level, IV const hp_iv, EV const hp_ev, auto make) :
		m_hp(base_hp, level, hp_iv, hp_ev),
		m_atk(make(SplitSpecialRegularStat::atk)),
		m_def(make(SplitSpecialRegularStat::def)),
		m_spa(make(SplitSpecialRegularStat::spa)),
		m_spd(make(SplitSpecialRegularStat::spd)),
		m_spe(make(SplitSpecialRegularStat::spe))
	{
	}

	HP m_hp;
	Stat m_atk;
	Stat m_def;
	Stat m_spa;
	[[no_unique_address]] ExistsIf<Stat, stat_style != StatStyle::gen1> m_spd;
	Stat m_spe;

	constexpr explicit Stats(bounded::tombstone_tag, auto const make):
		m_hp(make()),
		m_atk(),
		m_def(),
		m_spa(),
		m_spd(),
		m_spe()
	{
	}

	friend bounded::tombstone<Stats<stat_style>>;
	friend bounded::tombstone_member<&Stats<stat_style>::m_hp>;
};

} // namespace technicalmachine

template<technicalmachine::StatStyle stat_style>
struct bounded::tombstone<technicalmachine::Stats<stat_style>> : bounded::tombstone_member<&technicalmachine::Stats<stat_style>::m_hp> {
};
