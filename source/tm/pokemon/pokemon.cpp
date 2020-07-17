// Copyright (C) 2016 David Stone
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

#include <tm/pokemon/pokemon.hpp>

namespace technicalmachine {

// TODO: Setting IVs requires modifying the Pokemon after construction, which
// will not update Hidden Power
Pokemon::Pokemon(Generation const generation, Species const species, Level const level, Gender const gender, Item const item, Ability const ability, CombinedStats<IVAndEV> stats, RegularMoves regular_moves_, Happiness const happiness):
	m_regular_moves(regular_moves_),
	m_stats(BaseStats(generation, species), stats, level),

	m_species(species),
	m_item(item),
	m_ability(ability),
	m_gender(gender),
	m_nature(stats.nature),

	m_level(level),

	m_happiness(happiness),

	// TODO: Make this none if there is no way to call Hidden Power
	// TODO: Use the IVs provided
	m_hidden_power([=]{
		constexpr auto dv = DV(15_bi);
		constexpr auto iv = IV(31_bi);
		return generation <= Generation::two ?
			HiddenPower(generation, DVs{dv, dv, dv, dv}) :
			HiddenPower(generation, IVs{iv, iv, iv, iv, iv, iv});
	}()),
	
	m_has_been_seen(false),

	m_ability_is_known(true),
	m_item_is_known(true),
	m_nature_is_known(true)
{
}

Pokemon::Pokemon(Generation const generation, Species const species, Level const level, Gender const gender) : 
	Pokemon::Pokemon(
		generation,
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

} // namespace technicalmachine
