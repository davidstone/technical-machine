// Write Pokemon Online teams
// Copyright (C) 2014 David Stone
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

#include "conversion.hpp"
#include "stat_order.hpp"

#include "../../team.hpp"

#include "../../move/move.hpp"

#include "../../pokemon/max_pokemon_per_team.hpp"
#include "../../pokemon/pokemon.hpp"

#include "../../stat/stat.hpp"

#include <bounded_integer/integer_range.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>
#include <utility>

namespace technicalmachine {
namespace po {
namespace {
using namespace bounded::literal;
using boost::property_tree::ptree;

void write_move (Move const move, ptree & pt) {
	pt.add("Move", move_to_id(move));
}

void write_blank_move (ptree & pt) {
	pt.add ("Move", 0);
}

void write_stats (Pokemon const & pokemon, ptree & pt) {
	pt.add("DV", get_hp(pokemon).iv().value());
	for (auto const stat : stat_order) {
		pt.add("DV", get_stat(pokemon, stat).iv().value());
	}
	pt.add("EV", get_hp(pokemon).ev().value());
	for (auto const stat : stat_order) {
		pt.add("EV", get_stat(pokemon, stat).ev().value());
	}
}

void write_blank_stats (ptree & pt) {
	for (unsigned n = 0; n != max_pokemon_per_team; ++n)
		pt.add ("DV", 31);
	for (unsigned n = 0; n != max_pokemon_per_team; ++n)
		pt.add ("EV", 0);
}

void write_pokemon (Pokemon const & pokemon, ptree & pt) {
	ptree & member = pt.add ("Pokemon", "");
	member.put("<xmlattr>.Item", item_to_id(get_item(pokemon)));
	member.put ("<xmlattr>.Ability", ability_to_id (get_ability(pokemon).name()));
	std::pair<unsigned, unsigned> const ids = species_to_id(pokemon);
	member.put ("<xmlattr>.Num", ids.first);
	member.put ("<xmlattr>.Nature", nature_to_id(get_nature(pokemon)));
	member.put ("<xmlattr>.Shiny", 0);
	member.put ("<xmlattr>.Nickname", pokemon.get_nickname());
	member.put ("<xmlattr>.Gen", 4);
	member.put ("<xmlattr>.Forme", ids.second);
	member.put ("<xmlattr>.Happiness", get_happiness(pokemon));
	member.put ("<xmlattr>.Lvl", get_level(pokemon)());
	member.put ("<xmlattr>.Gender", gender_to_id (get_gender(pokemon).gender));

	auto const & moves = regular_moves(pokemon);
	for (auto const & move : moves) {
		write_move(move, member);
	}
	for (auto const n : bounded::integer_range(RegularMoveSize(size(moves)), max_moves_per_pokemon)) {
		static_cast<void>(n);
		write_blank_move(member);
	}

	write_stats (pokemon, member);
}

void write_blank_pokemon (ptree & pt) {
	ptree & member = pt.add ("Pokemon", "");
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

void write_team(Team & team, boost::filesystem::path const & file_name) {
	ptree pt;
	boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type> settings('\t', 1);
	ptree & t = pt.add ("Team", "");
	t.put ("<xmlattr>.version", 1);
	t.put ("<xmlattr>.gen", 4);
	t.put ("<xmlattr>.defaultTier", "");
	ptree & trainer = t.add ("Trainer", "TM");
	trainer.put ("<xmlattr>.loseMsg", "");
	trainer.put ("<xmlattr>.avatar", 1);
	trainer.put ("<xmlattr>.winMsg", "");
	trainer.put ("<xmlattr>.infoMsg", "");
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon (pokemon, t);
	}
	for (auto const & unused : bounded::integer_range(team.all_pokemon().size(), max_pokemon_per_team)) {
		static_cast<void>(unused);
		write_blank_pokemon(t);
	}
	write_xml(file_name.string(), pt, std::locale{}, settings);
}

}	// namespace po
}	// namespace technicalmachine
