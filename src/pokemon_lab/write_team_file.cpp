// Write Pokemon Lab teams
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

#include "write_team_file.hpp"

#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../move.hpp"
#include "../pokemon.hpp"
#include "../stat.hpp"
#include "../team.hpp"

namespace technicalmachine {
namespace pl {
namespace {

void write_move (Move const & move, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & m = pt.add ("moveset.move", move.to_string ());
	// TODO: replace with real PP ups logic
	m.put ("<xmlattr>.pp-up", 3);
}

void write_stat (Stat const & stat, std::string const & str, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & s = pt.add ("stats.stat", "");
	s.put ("<xmlattr>.name", str);
	s.put ("<xmlattr>.iv", stat.iv);
	s.put ("<xmlattr>.ev", stat.ev * 4);
}

void write_stats (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	write_stat (pokemon.hp, "HP", pt);
	write_stat (pokemon.atk, "Atk", pt);
	write_stat (pokemon.def, "Def", pt);
	write_stat (pokemon.spe, "Spd", pt);
	write_stat (pokemon.spa, "SpAtk", pt);
	write_stat (pokemon.spd, "SpDef", pt);
}

void write_pokemon (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & member = pt.add ("pokemon", "");
	member.put ("<xmlattr>.species", pokemon.to_string ());
	member.put ("nickname", pokemon.get_nickname());
	member.put ("level", pokemon.level);
	member.put ("happiness", pokemon.happiness);
	member.put ("gender", pokemon.gender.to_string ());
	member.put ("nature", pokemon.nature.to_string ());
	member.put ("item", pokemon.item.to_string ());
	member.put ("ability", pokemon.ability.to_string ());
	for (std::vector <Move>::const_iterator move = pokemon.move.set.cbegin (); move->name != Move::STRUGGLE; ++move)
		write_move (*move, member);
	write_stats (pokemon, member);
}

}	// anonymous namespace

void write_team (Team const & team, std::string const & file_name) {
	boost::property_tree::ptree pt;
	boost::property_tree::xml_writer_settings<char> settings ('\t', 1);
	boost::property_tree::ptree & t = pt.add ("shoddybattle", "");
	for (Pokemon const & pokemon : team.pokemon.set)
		write_pokemon (pokemon, t);
	write_xml (file_name, pt, std::locale (), settings);
}

}	// namespace pl
}	// namespace technicalmachine
