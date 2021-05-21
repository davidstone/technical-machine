// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/exists_if.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Level;

template<Generation generation>
struct Stats {
private:
	using Stat = InitialStat<generation>;
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
	Stats(BaseStats const base, Level const level, CombinedStats<generation> const inputs):
		Stats(
			base,
			level,
			IV(inputs.dvs_or_ivs.hp()),
			inputs.evs.hp(),
			[=](RegularStat const stat_name) {
				return initial_stat<generation>(
					stat_name,
					base[stat_name],
					level,
					inputs.nature,
					IV(inputs.dvs_or_ivs[stat_name]),
					inputs.evs[stat_name]
				);
			}
		)
	{
	}

	constexpr auto const & hp() const {
		return m_hp;
	}
	constexpr auto & hp() {
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
		if constexpr (generation == Generation::one) {
			return m_spa;
		} else {
			return m_spd;
		}
	}
	constexpr auto spe() const {
		return m_spe;
	}
	constexpr auto operator[](RegularStat const stat_name) const {
		return index_stat(*this, stat_name);
	}

	friend auto operator==(Stats, Stats) -> bool = default;
private:
	Stats(BaseStats const base, Level const level, IV const hp_iv, EV const hp_ev, auto make) :
		m_hp(base, level, hp_iv, hp_ev),
		m_atk(make(RegularStat::atk)),
		m_def(make(RegularStat::def)),
		m_spa(make(RegularStat::spa)),
		m_spd(make(RegularStat::spd)),
		m_spe(make(RegularStat::spe))
	{
	}
	HP m_hp;
	Stat m_atk;
	Stat m_def;
	Stat m_spa;
	[[no_unique_address]] ExistsIf<Stat, generation >= Generation::two> m_spd;
	Stat m_spe;
};

template<Generation generation>
auto with_new_ivs_and_evs(HP const original_hp, BaseStats const base_stats, Level const level, CombinedStats<generation> const stats) {
	auto result = Stats<generation>(base_stats, level, stats);
	result.hp() = result.hp().max() * original_hp.current() / original_hp.max();
	return result;
}

} // namespace technicalmachine
