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

#include <tm/pokemon/species.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>

#include <tm/stat/stat.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/status.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <boost/format.hpp>

#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>

namespace technicalmachine {

Pokemon::Pokemon(TeamSize const my_team_size, Species const species, Level const level, Gender const gender, Item const & item, Ability const & ability, Nature const & nature, Happiness const happiness):
	m_moves(my_team_size),
	current_type(species),
	stats(species, level),

	m_species(species),
	m_item(item),
	m_ability(ability),
	m_gender(gender),
	m_nature(nature),

	m_level(level),

	m_happiness(happiness),
	
	m_has_been_seen(false),

	m_ability_is_known(true),
	m_item_is_known(true),
	m_nature_is_known(true)
{
}

Pokemon::Pokemon(TeamSize const my_team_size, Species const species, Level const level, Gender const gender, Happiness const happiness) : 
	Pokemon::Pokemon(my_team_size, species, level, gender, Item::No_Item, Ability::Honey_Gather, Nature::Hardy, happiness)
	{
	m_ability_is_known = false;
	m_item_is_known = false;
	m_nature_is_known = false;
}

Pokemon::operator Species() const {
	return m_species;
}


void Pokemon::change_type(Type const new_type) {
	current_type.change_type(new_type);
}

auto Pokemon::has_been_seen() const -> bool {
	return m_has_been_seen;
}

bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs) {
	if (lhs.m_species != rhs.m_species)
		return true;
	return lhs.m_ability == rhs.m_ability and
			lhs.m_gender == rhs.m_gender and
			lhs.m_nature == rhs.m_nature and
			lhs.m_level == rhs.m_level;
}

containers::string to_string(Pokemon const & pokemon) {
	double const per_cent_hp = 100.0 * static_cast<double>(hp_ratio(pokemon));

	auto const output_status = !is_clear(get_status(pokemon));
	
	#define TECHNICALMACHINE_STAT(stat, str) \
		std::string_view(" / "), std::string_view(bounded::to_string(get_stat(pokemon, stat).ev().value())), std::string_view(" " str)
	
	auto moves_to_string = [&]{
		containers::string output;
		for (auto const & move : regular_moves(pokemon)) {
			output = containers::concatenate<containers::string>(std::move(output), std::string_view("\n\t- "), to_string(move));
		}
		return output;
	};

	return containers::concatenate<containers::string>(
		to_string(static_cast<Species>(pokemon)),
		std::string_view(" ("), std::string_view((boost::format("%.1f") % per_cent_hp).str()), std::string_view("% HP) @ "),
		to_string(get_item(pokemon)),
		std::string_view("\n\tAbility: "), to_string(get_ability(pokemon)), std::string_view("\n"),
		(output_status ? containers::concatenate<containers::string>(std::string_view("\tStatus: "), to_string(get_status(pokemon).name()), std::string_view("\n")) : containers::string("")),
		std::string_view("\tNature: "), to_string(get_nature(pokemon)),
		std::string_view("\n\t"),
		std::string_view(bounded::to_string(get_hp(pokemon).ev().value())), std::string_view(" HP"),
		TECHNICALMACHINE_STAT(StatNames::ATK, "Atk"),
		TECHNICALMACHINE_STAT(StatNames::DEF, "Def"),
		TECHNICALMACHINE_STAT(StatNames::SPA, "SpA"),
		TECHNICALMACHINE_STAT(StatNames::SPD, "SpD"),
		TECHNICALMACHINE_STAT(StatNames::SPE, "Spe"),
		moves_to_string()
	);
	#undef TECHNICALMACHINE_STAT
}

}	// namespace technicalmachine
