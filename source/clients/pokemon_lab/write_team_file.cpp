// Write Pokemon Lab teams
// Copyright (C) 2015 David Stone
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

#include "../../team.hpp"

#include "../../move/move.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

#include "../../stat/stat.hpp"

#include "../../string_conversions/ability.hpp"
#include "../../string_conversions/gender.hpp"
#include "../../string_conversions/item.hpp"
#include "../../string_conversions/move.hpp"
#include "../../string_conversions/nature.hpp"
#include "../../string_conversions/pokemon.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/utility/string_ref.hpp>

#include <string>
#include <vector>

namespace technicalmachine {
namespace pl {
namespace {

void write_move (Move const move, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & m = pt.add("moveset.move", to_string(move));
	// TODO: replace with real PP ups logic
	m.put ("<xmlattr>.pp-up", 3);
}

template<typename StatType>
void write_stat(StatType const & stat, boost::string_ref const str, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & s = pt.add ("stats.stat", "");
	s.put("<xmlattr>.name", str);
	s.put("<xmlattr>.iv", stat.iv().value());
	s.put("<xmlattr>.ev", stat.ev().value());
}

void write_stats (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	static std::pair<StatNames, boost::string_ref> const stats [] = {
		{ StatNames::ATK, "Atk" },
		{ StatNames::DEF, "Def" },
		{ StatNames::SPE, "Spd" },
		{ StatNames::SPA, "SpAtk" },
		{ StatNames::SPD, "SpDef" }
	};
	write_stat(get_hp(pokemon), "HP", pt);
	for (auto const & stat : stats) {
		write_stat(get_stat(pokemon, stat.first), stat.second, pt);
	}
}

boost::string_ref to_simulator_string(Species const species) {
	switch (species) {
		case Species::Deoxys_Mediocre:
			return "Deoxys";
		case Species::Deoxys_Attack:
			return "Deoxys-f";
		case Species::Deoxys_Defense:
			return "Deoxys-l";
		case Species::Deoxys_Speed:
			return "Deoxys-e";
		case Species::Giratina_Altered:
			return "Giratina";
		case Species::Giratina_Origin:
			return "Giratina-o";
		case Species::Rotom_Mow:
			return "Rotom-c";
		case Species::Rotom_Frost:
			return "Rotom-f";
		case Species::Rotom_Heat:
			return "Rotom-h";
		case Species::Rotom_Fan:
			return "Rotom-s";
		case Species::Rotom_Wash:
			return "Rotom-w";
		case Species::Shaymin_Land:
			return "Shaymin";
		case Species::Shaymin_Sky:
			return "Shaymin-s";
		case Species::Wormadam_Plant:
			return "Wormadam";
		case Species::Wormadam_Sandy:
			return "Wormadam-g";
		case Species::Wormadam_Trash:
			return "Wormadam-s";
		default:
			return to_string(species);
	}
}

void write_pokemon (Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & member = pt.add ("pokemon", "");
	member.put("<xmlattr>.species", to_simulator_string(pokemon));
	member.put ("nickname", pokemon.get_nickname());
	member.put ("level", get_level(pokemon)());
	member.put ("happiness", get_happiness(pokemon));
	member.put ("gender", to_string(get_gender(pokemon).gender));
	member.put ("nature", to_string(get_nature(pokemon)));
	member.put ("item", to_string(get_item(pokemon)));
	member.put ("ability", to_string(get_ability(pokemon).name()));
	for (auto const & move : regular_moves(pokemon)) {
		write_move(move, member);
	}
	write_stats (pokemon, member);
}

}	// namespace

void write_team(Team const & team, boost::filesystem::path const & file_name) {
	boost::property_tree::ptree pt;
	boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type> settings('\t', 1);
	boost::property_tree::ptree & t = pt.add ("shoddybattle", "");
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon (pokemon, t);
	}
	write_xml(file_name.string(), pt, std::locale (), settings);
}

}	// namespace pl
}	// namespace technicalmachine
