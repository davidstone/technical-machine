// Pokemon functions
// Copyright (C) 2013 David Stone
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

#include <cassert>
#include <cstdint>
#include <string>

#include <boost/format.hpp>

#include "species.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../rational.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

#include "../stat/stat.hpp"

#include "../string_conversions/ability.hpp"
#include "../string_conversions/item.hpp"
#include "../string_conversions/move.hpp"
#include "../string_conversions/nature.hpp"
#include "../string_conversions/pokemon.hpp"
#include "../string_conversions/status.hpp"

namespace technicalmachine {

Pokemon::Pokemon (unsigned const my_team_size, Species const species, uint8_t set_level, Gender set_gender, std::string const & set_nickname, uint8_t set_happiness) : 
	move(my_team_size),
	current_type(species),
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	nickname(set_nickname);
	#endif
	stats(species),

	m_species(species),
	m_gender(set_gender),

	m_level(set_level),

	m_happiness(set_happiness)
	{
	calculate_initial_hp(*this);
}

Pokemon::Pokemon(unsigned const my_team_size, Species const species, uint8_t const set_level, Gender const set_gender, Item const & set_item, Ability const & set_ability, Nature const & set_nature, std::string const & set_nickname, uint8_t set_happiness):
	Pokemon::Pokemon(my_team_size, species, set_level, set_gender, set_nickname, set_happiness)
	{
	m_item = set_item;
	m_ability = set_ability;
	m_nature = set_nature;
}

Pokemon::operator Species() const {
	return m_species;
}


void switch_in(Pokemon & pokemon) {
	pokemon.seen.make_visible();
}

void calculate_initial_hp(Pokemon & pokemon) {
	get_stat(pokemon, Stat::HP).calculate_initial_hp(get_level(pokemon));
}

void Pokemon::remove_switch() {
	move.remove_switch();
}

uint8_t Pokemon::index_of_first_switch () const {
	uint8_t index = 0;
	while (!is_switch(move(index)))
		++index;
	return index;
}

Rational hp_ratio(Pokemon const & pokemon) {
	auto const & hp = get_stat(pokemon, Stat::HP);
	return Rational(hp.stat, hp.max);
}

std::string Pokemon::get_nickname () const {
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		return nickname;
	#else
		return to_string(m_species);
	#endif
}

Ability const & get_ability(Pokemon const & pokemon) {
	return pokemon.m_ability;
}
Ability & get_ability(Pokemon & pokemon) {
	return pokemon.m_ability;
}

Gender const & get_gender(Pokemon const & pokemon) {
	return pokemon.m_gender;
}
Gender & get_gender(Pokemon & pokemon) {
	return pokemon.m_gender;
}

Item const & get_item(Pokemon const & pokemon) {
	return pokemon.m_item;
}
Item & get_item(Pokemon & pokemon) {
	return pokemon.m_item;
}

Nature const & get_nature(Pokemon const & pokemon) {
	return pokemon.m_nature;
}
Nature & get_nature(Pokemon & pokemon) {
	return pokemon.m_nature;
}

Stat const & get_stat(Pokemon const & pokemon, Stat::Stats const index_stat) {
	return pokemon.stats[index_stat];
}
Stat & get_stat(Pokemon & pokemon, Stat::Stats const index_stat) {
	return pokemon.stats[index_stat];
}

Status const & get_status(Pokemon const & pokemon) {
	return pokemon.m_status;
}
Status & get_status(Pokemon & pokemon) {
	return pokemon.m_status;
}

TypeCollection const & get_type(Pokemon const & pokemon) {
	return pokemon.current_type;
}

void Pokemon::change_type(Type::Types const new_type) {
	current_type.change_type(new_type);
}

unsigned get_level(Pokemon const & pokemon) {
	return pokemon.m_level();
}

unsigned Pokemon::happiness() const {
	return m_happiness;
}

Pokemon::hash_type Pokemon::hash() const {
	auto const & hp = get_stat(*this, Stat::HP);
	return static_cast<hash_type>(m_species) + number_of_species *
			(m_item.name + Item::END *
			(m_status.hash() + Status::max_hash() *
			((hp.stat - 1u) + hp.max *	// - 1 because you can't have 0 HP
			(seen.hash() + seen.max_hash() *
			move.hash()))));
}

Pokemon::hash_type Pokemon::max_hash() const {
	return number_of_species * Item::END * Status::max_hash() * get_stat(*this, Stat::HP).max * seen.max_hash() * move.max_hash();
}

bool operator== (Pokemon const & lhs, Pokemon const & rhs) {
	// Species clause is assumed, and Pokemon will only be compared for equality
	// on the same team, so the same species implies many other things are the
	// same
	assert(illegal_inequality_check(lhs, rhs));
	return lhs.move == rhs.move and
			lhs.m_species == rhs.m_species and
			lhs.m_status == rhs.m_status and
			get_stat(lhs, Stat::HP).stat == get_stat(rhs, Stat::HP).stat and
			lhs.m_item == rhs.m_item and
			lhs.seen == rhs.seen;
}

bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs) {
	if (lhs.m_species != rhs.m_species)
		return true;
	return lhs.m_ability == rhs.m_ability and
			lhs.m_gender == rhs.m_gender and
			lhs.m_nature == rhs.m_nature and
			lhs.m_level == rhs.m_level and
			lhs.happiness() == rhs.happiness();
}

bool operator!= (Pokemon const & lhs, Pokemon const & rhs) {
	return !(lhs == rhs);
}

std::string to_string(Pokemon const & pokemon, bool const include_nickname) {
	std::string output = to_string(static_cast<Species>(pokemon));
	double const d_per_cent_hp = 100.0 * hp_ratio(pokemon);
	std::string const per_cent_hp = str(boost::format("%.1f") % d_per_cent_hp);
	output += " (" + per_cent_hp + "% HP)";
	output += " @ " + to_string(get_item(pokemon).name);
	if (include_nickname and pokemon.get_nickname() != to_string(static_cast<Species>(pokemon))) {
		output += " ** " + pokemon.get_nickname();
	}
	output += '\n';
	if (get_ability(pokemon).is_set()) {
		output += "\tAbility: " + to_string(get_ability(pokemon).name()) + '\n';
	}
	if (!get_status(pokemon).is_clear()) {
		output += "\tStatus: " + to_string(get_status(pokemon).name()) + '\n';
	}
	output += "\tNature: " + to_string(get_nature(pokemon).name) + '\n';
	output += "\t";
	auto const add_stat = [&](Stat const & stat, std::string const & stat_name) {
		if (stat_name != "HP") {
			output += " / ";
		}
		output += std::to_string(stat.ev.value()) + " " + stat_name;
	};
	static std::pair<Stat::Stats, std::string> const stats [] = {
		{ Stat::HP, "HP" },
		{ Stat::ATK, "Atk" },
		{ Stat::DEF, "Def" },
		{ Stat::SPA, "SpA" },
		{ Stat::SPD, "SpD" },
		{ Stat::SPE, "Spe" },
	};
	for (auto const stat : stats) {
		add_stat(get_stat(pokemon, stat.first), stat.second);
	}
	pokemon.move.for_each_regular_move([& output](Move const & move) {
		output += "\n\t- " + to_string(move);
	});
	return output;
}

}	// namespace technicalmachine
