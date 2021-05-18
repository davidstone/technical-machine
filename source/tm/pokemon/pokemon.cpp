// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/initial_stat.hpp>

namespace technicalmachine {

template<Generation generation>
Pokemon<generation>::Pokemon(Species const species, Level const level, Gender const gender) : 
	Pokemon::Pokemon(
		species,
		level,
		gender,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{default_iv(generation), EV(0_bi)},
			{default_iv(generation), EV(0_bi)},
			{default_iv(generation), EV(0_bi)},
			{default_iv(generation), EV(0_bi)},
			{default_iv(generation), EV(0_bi)},
			{default_iv(generation), EV(0_bi)},
		},
		RegularMoves(),
		Happiness()
	)
{
	m_ability_is_known = false;
	m_item_is_known = false;
	m_nature_is_known = false;
}

template<Generation generation>
auto Pokemon<generation>::set_ev(PermanentStat const stat_name, IV const iv, EV const ev) -> void {
	auto const base_stats = BaseStats(generation, species());
	if (stat_name == PermanentStat::hp) {
		m_stats.hp() = HP(base_stats, level(), iv, ev);
	} else {
		auto const regular = RegularStat(stat_name);
		auto & stat = m_stats[regular];
		stat = initial_stat(regular, base_stats[regular], nature(), iv, ev, level());
	}
}

template struct Pokemon<Generation::one>;
template struct Pokemon<Generation::two>;
template struct Pokemon<Generation::three>;
template struct Pokemon<Generation::four>;
template struct Pokemon<Generation::five>;
template struct Pokemon<Generation::six>;
template struct Pokemon<Generation::seven>;
template struct Pokemon<Generation::eight>;

} // namespace technicalmachine