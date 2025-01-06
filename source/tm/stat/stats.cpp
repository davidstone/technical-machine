// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stats;

import tm.pokemon.level;

import tm.stat.hp;
import tm.stat.initial_stat;
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
