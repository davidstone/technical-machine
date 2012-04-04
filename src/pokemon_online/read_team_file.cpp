// Load Pokemon Online teams
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

#include <stdexcept>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "conversion.hpp"
#include "../ability.hpp"
#include "../item.hpp"
#include "../move.hpp"
#include "../pokemon.hpp"
#include "../species.hpp"
#include "../stat.hpp"
#include "../team.hpp"

#include <iostream>

namespace technicalmachine {
namespace po {
namespace {

static Move load_move (boost::property_tree::ptree const & pt, unsigned foe_size) {
	unsigned const id = pt.get <unsigned> ("");
	Move::Moves name = id_to_move (id);
	constexpr unsigned pp_ups = 3;
	return Move (name, pp_ups, foe_size);
}

static Stat & lookup_stat (Pokemon & pokemon, unsigned n) {
	switch (n) {
		case 0:
			return pokemon.hp;
		case 1:
			return pokemon.atk;
		case 2:
			return pokemon.def;
		case 3:
			return pokemon.spa;
		case 4:
			return pokemon.spd;
		case 5:
			return pokemon.spe;
		default:
			throw InvalidStat (std::to_string (n));
	}
}

static Pokemon load_pokemon (boost::property_tree::ptree const & pt, unsigned foe_size, unsigned my_size) {
	unsigned const id = pt.get <unsigned> ("<xmlattr>.Num");
	unsigned const forme = pt.get <unsigned> ("<xmlattr>.Forme");
	Pokemon pokemon (id_to_species (id, forme), my_size);
	pokemon.set_nickname (pt.get <std::string> ("<xmlattr>.Nickname"));
	unsigned const item = pt.get <unsigned> ("<xmlattr>.Item");
	pokemon.item.name = id_to_item (item);
	unsigned const ability = pt.get <unsigned> ("<xmlattr>.Ability");
	pokemon.ability.name = id_to_ability (ability);
	unsigned const nature = pt.get <unsigned> ("<xmlattr>.Nature");
	pokemon.nature.name = id_to_nature (nature);
	pokemon.happiness = pt.get <unsigned> ("<xmlattr>.Happiness");
	pokemon.level = pt.get <unsigned> ("<xmlattr>.Lvl");
	unsigned const gender = pt.get <unsigned> ("<xmlattr>.Gender");
	pokemon.gender = id_to_gender (gender);

	unsigned n = 0;
	for (auto const & value : pt.get_child ("")) {
		if (value.first == "Move") {
			Move const move = load_move (value.second, foe_size);
			pokemon.move.set.insert (pokemon.move.set.begin () + n, move);
			n < 3 ? ++n : n = 0;
		}
		else if (value.first == "DV") {
			Stat & stat = lookup_stat (pokemon, n);
			stat.iv = value.second.get <unsigned> ("");
			n < 5 ? ++n : n = 0;
		}
		else if (value.first == "EV") {
			Stat & stat = lookup_stat (pokemon, n);
			stat.ev = value.second.get <unsigned> ("") / 4;
			n < 5 ? ++n : n = 0;
		}
	}
	return pokemon;
}

}	// anonymous namespace

void load_team (Team & team, std::string const & file_name, unsigned foe_size) {
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);
	
	team.size = 6;
	auto const all_pokemon = pt.get_child ("Team");
	for (auto const & value : all_pokemon) {
		if (value.first == "Pokemon") {
			Pokemon const pokemon = load_pokemon (value.second, foe_size, team.size);
			team.pokemon.set.push_back (pokemon);
		}
	}
	team.size = team.pokemon.set.size ();
}

}	// namespace po
}	// namespace technicalmachine
