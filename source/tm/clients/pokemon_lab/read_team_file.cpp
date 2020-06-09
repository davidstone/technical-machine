// Load Pokemon Lab teams
// Copyright (C) 2018 David Stone
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

#include <tm/clients/pokemon_lab/read_team_file.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/stat.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/generation.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>
#include <string_view>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace pl {
namespace {

auto stat_from_simulator_string(std::string_view const str) {
	using Storage = containers::array<containers::map_value_type<std::string_view, RegularStat>, 5>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "Atk", RegularStat::atk },
			{ "Def", RegularStat::def },
			{ "SpAtk", RegularStat::spa },
			{ "SpDef", RegularStat::spd },
			{ "Spd", RegularStat::spe }
		}}
	);
	return converter.at(str);
}

auto load_stats(Generation const generation, Pokemon & pokemon, boost::property_tree::ptree const & pt) {
	auto const stat_name = pt.get<std::string>("<xmlattr>.name");
	auto const iv = IV(pt.get<IV::value_type>("<xmlattr>.iv"));
	auto const ev = EV(pt.get<EV::value_type>("<xmlattr>.ev"));
	if (stat_name == "HP") {
		set_hp_ev(generation, pokemon, iv, ev);
	} else {
		set_stat_ev(pokemon, stat_from_simulator_string(stat_name), iv, ev);
	}
}

auto species_from_simulator_string(std::string_view const str) {
	using Storage = containers::array<containers::map_value_type<std::string_view, Species>, 16>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "Deoxys", Species::Deoxys_Normal },
			{ "Deoxys-e", Species::Deoxys_Speed },
			{ "Deoxys-f", Species::Deoxys_Attack },
			{ "Deoxys-l", Species::Deoxys_Defense },
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
		}}
	);
	auto const it = converter.find(str);
	return (it != end(converter)) ? it->mapped() : from_string<Species>(str);
}

auto load_pokemon(Generation const generation, boost::property_tree::ptree const & pt, Team & team) {
	auto const species_str = pt.get <std::string>("<xmlattr>.species");
	auto const species = species_from_simulator_string(species_str);
	// auto const nickname_temp = pt.get <std::string>("nickname");
	// auto const nickname = !nickname_temp.empty() ? nickname_temp : species_str;
	auto const level = Level(pt.get<Level::value_type>("level"));
	auto const gender = Gender(from_string<Gender>(pt.get<std::string>("gender")));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("happiness"));
	auto const nature = from_string<Nature>(pt.get<std::string>("nature"));
	auto const item = from_string<Item>(pt.get<std::string>("item"));
	auto const ability = Ability(from_string<Ability>(pt.get<std::string>("ability")));
	auto & pokemon = team.add_pokemon(generation, species, level, gender, item, ability, nature, happiness);
	
	for (boost::property_tree::ptree::value_type const & value : pt.get_child("moveset")) {
		auto const name = from_string<Moves>(value.second.get_value<std::string>());
		auto const pp_ups = value.second.get<PP::pp_ups_type>("<xmlattr>.pp-up");
		add_seen_move(all_moves(pokemon), generation, name, pp_ups);
	}
	for (auto const & value : pt.get_child("stats")) {
		load_stats(generation, pokemon, value.second);
	}
}

}	// namespace

Team load_team(Generation const generation, std::filesystem::path const & team_file) {
	boost::property_tree::ptree pt;
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("shoddybattle");
	constexpr bool is_me = true;
	auto team = Team(static_cast<TeamSize>(all_pokemon.size()), is_me);
	for (auto const & value : all_pokemon) {
		load_pokemon(generation, value.second, team);
	}
	team.all_pokemon().reset_index();
	return team;
}

}	// namespace pl
}	// namespace technicalmachine
