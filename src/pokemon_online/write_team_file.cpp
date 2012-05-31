// Write Pokemon Online teams
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
#include <utility>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "conversion.hpp"
#include "../pokemon.hpp"
#include "../stat.hpp"
#include "../team.hpp"

#include "../move/move.hpp"

namespace technicalmachine {
namespace po {
namespace {

void write_move (Move const & move, boost::property_tree::ptree & pt) {
	pt.add ("Move", move_to_id (move.name));
}

void write_blank_move (boost::property_tree::ptree & pt) {
	pt.add ("Move", 0);
}

void write_stats (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	pt.add ("DV", pokemon.hp.iv);
	pt.add ("DV", pokemon.atk.iv);
	pt.add ("DV", pokemon.def.iv);
	pt.add ("DV", pokemon.spa.iv);
	pt.add ("DV", pokemon.spd.iv);
	pt.add ("DV", pokemon.spe.iv);
	pt.add ("EV", pokemon.hp.ev * 4);
	pt.add ("EV", pokemon.atk.ev * 4);
	pt.add ("EV", pokemon.def.ev * 4);
	pt.add ("EV", pokemon.spa.ev * 4);
	pt.add ("EV", pokemon.spd.ev * 4);
	pt.add ("EV", pokemon.spe.ev * 4);
}

void write_blank_stats (boost::property_tree::ptree & pt) {
	for (unsigned n = 0; n != 6; ++n)
		pt.add ("DV", 31);
	for (unsigned n = 0; n != 6; ++n)
		pt.add ("EV", 0);
}

void write_pokemon (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & member = pt.add ("Pokemon", "");
	member.put ("<xmlattr>.Item", item_to_id (pokemon.item.name));
	member.put ("<xmlattr>.Ability", ability_to_id (pokemon.ability.name));
	std::pair<unsigned, unsigned> const ids = species_to_id (pokemon.name);
	member.put ("<xmlattr>.Num", ids.first);
	member.put ("<xmlattr>.Nature", nature_to_id (pokemon.nature.name));
	member.put ("<xmlattr>.Shiny", 0);
	member.put ("<xmlattr>.Nickname", pokemon.get_nickname());
	member.put ("<xmlattr>.Gen", 4);
	member.put ("<xmlattr>.Forme", ids.second);
	member.put ("<xmlattr>.Happiness", pokemon.happiness());
	member.put ("<xmlattr>.Lvl", pokemon.level);
	member.put ("<xmlattr>.Gender", gender_to_id (pokemon.gender.gender));

	unsigned n = 0;
	pokemon.move.for_each_regular_move([&](Move const & move) {
		write_move (move, member);
		++n;
	});
	for (; n < 4; ++n)
		write_blank_move (member);

	write_stats (pokemon, member);
}

void write_blank_pokemon (boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & member = pt.add ("Pokemon", "");
	member.put ("<xmlattr>.Item", 0);
	member.put ("<xmlattr>.Ability", 0);
	member.put ("<xmlattr>.Num", 0);
	member.put ("<xmlattr>.Nature", 0);
	member.put ("<xmlattr>.Shiny", 0);
	member.put ("<xmlattr>.Nickname", "Missingno");
	member.put ("<xmlattr>.Gen", 4);
	member.put ("<xmlattr>.Forme", 0);
	member.put ("<xmlattr>.Happiness", 0);
	member.put ("<xmlattr>.Lvl", 100);
	member.put ("<xmlattr>.Gender", 0);

	for (unsigned n = 0; n != 4; ++n)
		write_blank_move (member);

	write_blank_stats (member);
}

}	// anonymous namespace

void write_team (Team & team, std::string const & file_name) {
	boost::property_tree::ptree pt;
	boost::property_tree::xml_writer_settings<char> settings ('\t', 1);
	boost::property_tree::ptree & t = pt.add ("Team", "");
	t.put ("<xmlattr>.version", 1);
	t.put ("<xmlattr>.gen", 4);
	t.put ("<xmlattr>.defaultTier", "");
	boost::property_tree::ptree & trainer = t.add ("Trainer", "TM");
	trainer.put ("<xmlattr>.loseMsg", "");
	trainer.put ("<xmlattr>.avatar", 1);
	trainer.put ("<xmlattr>.winMsg", "");
	trainer.put ("<xmlattr>.infoMsg", "");
	team.pokemon.for_each([&](Pokemon const & pokemon) {
		write_pokemon (pokemon, t);
	});
	for (unsigned n = team.pokemon.size (); n < 6; ++n)
		write_blank_pokemon (t);
	write_xml (file_name, pt, std::locale (), settings);
}

}	// namespace po
}	// namespace technicalmachine
