// Load Pokemon Lab teams
// Copyright (C) 2012 David Stone
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

#include "read_team_file.hpp"

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../pokemon.hpp"
#include "../stat.hpp"
#include "../team.hpp"

namespace technicalmachine {
namespace pl {
namespace {

static Move load_move (boost::property_tree::ptree const & pt, unsigned foe_size) {
	std::string const name_str = pt.get <std::string> ("");
	Move::Moves const name = Move::from_string (name_str);
	int const pp_ups = pt.get <int> ("<xmlattr>.pp-up");
	return Move (name, pp_ups, foe_size);
}

static Stat & lookup_stat (Pokemon & pokemon, std::string const & name) {
	if (name == "HP")
		return pokemon.hp;
	else if (name == "Atk")
		return pokemon.atk;
	else if (name == "Def")
		return pokemon.def;
	else if (name == "Spd")
		return pokemon.spe;
	else if (name == "SpAtk")
		return pokemon.spa;
	else if (name == "SpDef")
		return pokemon.spd;
	else
		throw InvalidStat (name);
}

static void load_stats (Pokemon & pokemon, boost::property_tree::ptree const & pt) {
	std::string const name = pt.get <std::string> ("<xmlattr>.name");
	Stat & stat = lookup_stat (pokemon, name);
	stat.iv = pt.get <unsigned> ("<xmlattr>.iv");
	stat.ev = pt.get <unsigned> ("<xmlattr>.ev") / 4;
}

static Pokemon load_pokemon (boost::property_tree::ptree const & pt, unsigned foe_size, unsigned my_size) {
	std::string const species_str = pt.get <std::string> ("<xmlattr>.species");
	Pokemon pokemon (Pokemon::from_string (species_str), my_size);
	pokemon.nickname = pt.get <std::string> ("nickname");
	if (pokemon.nickname.empty ())
		pokemon.nickname = species_str;
	pokemon.level = pt.get <uint8_t> ("level");
	pokemon.happiness = pt.get <int> ("happiness");
	std::string const gender_str = pt.get <std::string> ("gender");
	pokemon.gender = Gender (gender_str);
	std::string const nature_str = pt.get <std::string> ("nature");
	pokemon.nature = Nature (nature_str);
	std::string const item_str = pt.get <std::string> ("item");
	pokemon.item = Item (item_str);
	std::string const ability_str = pt.get <std::string> ("ability");
	pokemon.ability = Ability (ability_str);
	
	unsigned n = 0;
	for (boost::property_tree::ptree::value_type const & value : pt.get_child ("moveset")) {
		Move const move = load_move (value.second, foe_size);
		pokemon.move.set.insert (pokemon.move.set.begin () + n, move);
		++n;
	}
	
	for (auto const & value : pt.get_child ("stats"))
		load_stats (pokemon, value.second);
	
	return pokemon;
}

}	// anonymous namespace

void load_team (Team & team, std::string const & file_name, unsigned foe_size) {
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);
	
	auto const all_pokemon = pt.get_child ("shoddybattle");
	team.size = all_pokemon.size ();
	for (auto const & value : all_pokemon) {
		Pokemon const pokemon = load_pokemon (value.second, foe_size, team.size);
		team.pokemon.set.push_back (pokemon);
	}
}

}	// namespace pl
}	// namespace technicalmachine
