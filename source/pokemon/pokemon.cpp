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

#include "pokemon.hpp"

#include "species.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../operators.hpp"
#include "../rational.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"

#include "../stat/stat.hpp"

#include "../string_conversions/ability.hpp"
#include "../string_conversions/item.hpp"
#include "../string_conversions/move.hpp"
#include "../string_conversions/nature.hpp"
#include "../string_conversions/pokemon.hpp"
#include "../string_conversions/status.hpp"

#include <containers/array/make_array.hpp>

#include <boost/format.hpp>

#include <cassert>
#include <cstdint>
#include <string>

namespace technicalmachine {

Pokemon::Pokemon(TeamSize const my_team_size, Species const species, Level const level, Gender const gender, Item const & item, Ability const & ability, Nature const & nature, std::string const & set_nickname, Happiness const happiness):
	m_moves(my_team_size),
	current_type(species),
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	nickname(set_nickname);
	#endif
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
	static_cast<void>(set_nickname);
}

Pokemon::Pokemon(TeamSize const my_team_size, Species const species, Level const level, Gender const gender, std::string const & set_nickname, Happiness const happiness) : 
	Pokemon::Pokemon(my_team_size, species, level, gender, Item::No_Item, Ability::Honey_Gather, Nature::Hardy, set_nickname, happiness)
	{
	m_ability_is_known = false;
	m_item_is_known = false;
	m_nature_is_known = false;
}

Pokemon::operator Species() const {
	return m_species;
}


std::string const & Pokemon::get_nickname() const {
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		return nickname;
	#else
		return to_string(m_species);
	#endif
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

std::string to_string(Pokemon const & pokemon, bool const include_nickname) {
	std::string output = to_string(static_cast<Species>(pokemon));
	double const d_per_cent_hp = 100.0 * static_cast<double>(hp_ratio(pokemon));
	std::string const per_cent_hp = (boost::format("%.1f") % d_per_cent_hp).str();
	output += " (" + per_cent_hp + "% HP)";
	output += " @ " + to_string(get_item(pokemon));
	if (include_nickname and pokemon.get_nickname() != to_string(static_cast<Species>(pokemon))) {
		output += " ** " + pokemon.get_nickname();
	}
	output += '\n';
	output += "\tAbility: " + to_string(get_ability(pokemon)) + '\n';
	if (!is_clear(get_status(pokemon))) {
		output += "\tStatus: " + to_string(get_status(pokemon).name()) + '\n';
	}
	output += "\tNature: " + to_string(get_nature(pokemon)) + '\n';
	output += "\t";
	auto const add_stat = [&](Stat const & stat, std::string const & stat_name) {
		output += " / " + bounded::to_string(stat.ev().value()) + " " + stat_name;
	};
	struct Pair {
		StatNames name;
		std::string str;
	};
	static auto const stats = containers::make_array(
		Pair{StatNames::ATK, "Atk"},
		Pair{StatNames::DEF, "Def"},
		Pair{StatNames::SPA, "SpA"},
		Pair{StatNames::SPD, "SpD"},
		Pair{StatNames::SPE, "Spe"}
	);
	output += bounded::to_string(get_hp(pokemon).ev().value()) + " HP";
	for (auto const & stat : stats) {
		add_stat(get_stat(pokemon, stat.name), stat.str);
	}
	for (auto const & move : regular_moves(pokemon)) {
		output += "\n\t- " + to_string(move);
	}
	return output;
}

}	// namespace technicalmachine
