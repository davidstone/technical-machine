// Load Pokemon Lab teams
// Copyright (C) 2011 David Stone
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

#include "file.hpp"

#include <stdexcept>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../pokemon.hpp"
#include "../stat.hpp"
#include "../team.hpp"

namespace technicalmachine {
namespace pl {
namespace {

static void load_move (Pokemon & pokemon, boost::property_tree::ptree const & pt, unsigned size) {
	std::string const name_str = pt.get <std::string> ("");
	Move::Moves const name = Move::from_string (name_str);
	int const pp_ups = pt.get <int> ("<xmlattr>.pp-up");
	Move const move (name, pp_ups, size);
	pokemon.move.set.insert (pokemon.move.set.begin (), move);
}

class InvalidStat : public std::runtime_error {
	public:
		explicit InvalidStat (std::string const & stat_string):
			std::runtime_error ("Invalid stat of " + stat_string + " requested.\n")
			{
		}
};

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
	stat.ev = pt.get <unsigned> ("<xmlattr>.ev");
}

static void load_pokemon (Team & team, boost::property_tree::ptree const & pt, unsigned size) {
	std::string const species_str = pt.get <std::string> ("<xmlattr>.species");
	Pokemon pokemon (Pokemon::from_string (species_str), team.size);
	pokemon.nickname = pt.get <std::string> ("nickname");
	if (pokemon.nickname.empty ())
		pokemon.nickname = species_str;
	pokemon.level = pt.get <int> ("level");
	pokemon.happiness = pt.get <int> ("happiness");
	std::string const gender_str = pt.get <std::string> ("gender");
	pokemon.gender.set_name_from_string (gender_str);
	std::string const nature_str = pt.get <std::string> ("nature");
	pokemon.nature.set_name_from_string (nature_str);
	std::string const item_str = pt.get <std::string> ("item");
	pokemon.item.set_name_from_string (item_str);
	std::string const ability_str = pt.get <std::string> ("ability");
	pokemon.ability.set_name_from_string (ability_str);
	
	for (boost::property_tree::ptree::value_type const & value : pt.get_child ("moveset"))
		load_move (pokemon, value.second, size);
	
	for (auto const & value : pt.get_child ("stats"))
		load_stats (pokemon, value.second);
	
	team.pokemon.set.push_back (pokemon);
}

static unsigned team_size (std::string const & name) {
	std::ifstream file (name);
	std::string line;
	unsigned size = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.find ("<pokemon species") != std::string::npos)
			++size;
	}
	file.close();
	return size;
}

}	// anonymous namespace

void load_team (Team & team, std::string const & file_name, unsigned size) {
	team.size = team_size (file_name);
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);

	for (auto const & value : pt.get_child ("shoddybattle"))
		load_pokemon (team, value.second, size);
}

}	// namespace pl
}	// namespace technicalmachine
