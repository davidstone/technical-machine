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

#include "../../team.hpp"

#include "../../move/move.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/invalid_stat.hpp"
#include "../../stat/stat.hpp"

#include "../../string_conversions/conversion.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
namespace pl {
namespace {

static Move load_move (boost::property_tree::ptree const & pt, unsigned foe_size) {
	std::string const name_str = pt.get_value<std::string>();
	Moves const name = from_string<Moves>(name_str);
	unsigned const pp_ups = pt.get <unsigned> ("<xmlattr>.pp-up");
	return Move(name, pp_ups, foe_size);
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

void load_pokemon (boost::property_tree::ptree const & pt, Team & team, unsigned foe_size) {
	std::string const species_str = pt.get <std::string> ("<xmlattr>.species");
	std::string const nickname_temp = pt.get <std::string> ("nickname");
	std::string const nickname = !nickname_temp.empty() ? nickname_temp : species_str;
	uint8_t const level = pt.get <uint8_t> ("level");
	Gender const gender(from_string<Gender::Genders>(pt.get<std::string>("gender")));
	uint8_t const happiness = pt.get<uint8_t>("happiness");
	Nature const nature(pt.get<std::string>("nature"));
	Item const item(pt.get<std::string>("item"));
	Ability const ability(pt.get<std::string>("ability"));
	team.add_pokemon(from_string<Species>(species_str), level, gender, item, ability, nature, nickname, happiness);
	Pokemon & pokemon = team.all_pokemon().at_replacement();
	
	for (boost::property_tree::ptree::value_type const & value : pt.get_child ("moveset"))
		pokemon.move.add (load_move (value.second, foe_size));
	
	for (auto const & value : pt.get_child ("stats"))
		load_stats (pokemon, value.second);
}

}	// anonymous namespace

void load_team (Team & team, std::string const & file_name, unsigned foe_size) {
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);
	
	auto const all_pokemon = pt.get_child ("shoddybattle");
	team.all_pokemon().initialize_size(all_pokemon.size());
	for (auto const & value : all_pokemon)
		load_pokemon (value.second, team, foe_size);
	team.all_pokemon().reset_index();
}

}	// namespace pl
}	// namespace technicalmachine
