// Load Pokemon Lab teams
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

#include "read_team_file.hpp"

#include <map>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../../team.hpp"

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

#include "../../stat/stat.hpp"

#include "../../string_conversions/conversion.hpp"

namespace technicalmachine {
namespace pl {
namespace {

Move load_move(boost::property_tree::ptree const & pt) {
	Moves const name = from_string<Moves>(pt.get_value<std::string>());
	auto const pp_ups = pt.get<Pp::pp_ups_type>("<xmlattr>.pp-up");
	return Move(name, pp_ups);
}

static Stat & lookup_stat (Pokemon & pokemon, std::string const & name) {
	static std::map<std::string, StatNames> const stats = {
		{ "Atk", StatNames::ATK },
		{ "Def", StatNames::DEF },
		{ "SpAtk", StatNames::SPA },
		{ "SpDef", StatNames::SPD },
		{ "Spd", StatNames::SPE }
	};
	return get_stat(pokemon, stats.at(name));
}

static void load_stats (Pokemon & pokemon, boost::property_tree::ptree const & pt) {
	auto const name = pt.get<std::string>("<xmlattr>.name");
	IV const iv(pt.get<IV::value_type>("<xmlattr>.iv"));
	EV const ev(pt.get<EV::value_type>("<xmlattr>.ev"));
	if (name == "HP") {
		HP & hp = get_hp(pokemon);
		hp.iv = iv;
		hp.ev = ev;
	}
	else {
		Stat & stat = lookup_stat(pokemon, name);
		stat.iv = iv;
		stat.ev = ev;
	}
}

Species from_simulator_string(std::string const & str) {
	static std::map<std::string, Species> const convertor = {
		{ "Deoxys", Species::Deoxys_Mediocre },
		{ "Deoxys-f", Species::Deoxys_Attack },
		{ "Deoxys-l", Species::Deoxys_Defense },
		{ "Deoxys-e", Species::Deoxys_Speed },
		{ "Giratina", Species::Giratina_Altered },
		{ "Giratina-o", Species::Giratina_Origin },
		{ "Rotom-c", Species::Rotom_Mow },
		{ "Rotom-f", Species::Rotom_Frost },
		{ "Rotom-h", Species::Rotom_Heat },
		{ "Rotom-s", Species::Rotom_Fan },
		{ "Rotom-w", Species::Rotom_Wash },
		{ "Shaymin", Species::Shaymin_Land },
		{ "Shaymin-s", Species::Shaymin_Sky },
		{ "Wormadam", Species::Wormadam_Plant },
		{ "Wormadam-g", Species::Wormadam_Sandy },
		{ "Wormadam-s", Species::Wormadam_Trash }
	};
	auto const it = convertor.find(str);
	return (it != convertor.end()) ? it->second : from_string<Species>(str);
}

void load_pokemon (boost::property_tree::ptree const & pt, Team & team) {
	auto const species_str = pt.get <std::string> ("<xmlattr>.species");
	auto const nickname_temp = pt.get <std::string> ("nickname");
	auto const nickname = !nickname_temp.empty() ? nickname_temp : species_str;
	Level const level(pt.get<bounded::checked_integer<Level::min, Level::max>>("level"));
	Gender const gender(from_string<Gender::Genders>(pt.get<std::string>("gender")));
	Happiness const happiness(pt.get<Happiness::value_type>("happiness"));
	Nature const nature(from_string<Nature>(pt.get<std::string>("nature")));
	Item const item(from_string<Item>(pt.get<std::string>("item")));
	Ability const ability(from_string<Ability::Abilities>(pt.get<std::string>("ability")));
	team.add_pokemon(from_simulator_string(species_str), level, gender, item, ability, nature, nickname, happiness);
	Pokemon & pokemon = team.all_pokemon().at_replacement();
	
	for (boost::property_tree::ptree::value_type const & value : pt.get_child ("moveset"))
		all_moves(pokemon).add(load_move(value.second));
	
	for (auto const & value : pt.get_child ("stats"))
		load_stats (pokemon, value.second);
}

}	// namespace

void load_team (Team & team, std::string const & file_name) {
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);
	
	auto const all_pokemon = pt.get_child ("shoddybattle");
	team.all_pokemon().initialize_size(static_cast<TeamSize>(all_pokemon.size()));
	for (auto const & value : all_pokemon) {
		load_pokemon (value.second, team);
	}
	team.all_pokemon().reset_index();
}

}	// namespace pl
}	// namespace technicalmachine
