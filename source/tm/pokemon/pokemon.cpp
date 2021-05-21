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
		CombinedStats<generation>{
			Nature::Hardy,
            IVs(
                default_iv(generation),
                default_iv(generation),
                default_iv(generation),
                default_iv(generation),
                default_iv(generation),
                default_iv(generation)
			),
            EVs(
                EV(0_bi),
                EV(0_bi),
                EV(0_bi),
                EV(0_bi),
                EV(0_bi),
                EV(0_bi)
			)
		},
		RegularMoves(),
		Happiness()
	)
{
	m_ability_is_known = false;
	m_item_is_known = false;
	m_nature_is_known = false;
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