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

#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/team.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>

namespace technicalmachine {
namespace pl {
namespace {

void write_move (Move const move, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & m = pt.add("moveset.move", to_string(move.name()));
	// TODO: replace with real PP ups logic
	m.put ("<xmlattr>.pp-up", 3);
}

void write_stat(std::string_view const name, IV const iv, EV const ev, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & s = pt.add("stats.stat", "");
	s.put("<xmlattr>.name", name);
	s.put("<xmlattr>.iv", iv.value());
	s.put("<xmlattr>.ev", ev.value());
}

constexpr auto to_simulator_string(PermanentStat const stat) -> std::string_view {
	switch (stat) {
		case PermanentStat::hp: return "HP";
		case PermanentStat::atk: return "Atk";
		case PermanentStat::def: return "Def";
		case PermanentStat::spa: return "SpAtk";
		case PermanentStat::spd: return "SpDef";
		case PermanentStat::spe: return "Spd";
	}
}

void write_stats(Generation const generation, Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	auto const stats = calculate_ivs_and_evs(generation, pokemon);
	for (auto const stat_name : {PermanentStat::hp, PermanentStat::atk, PermanentStat::def, PermanentStat::spe, PermanentStat::spa, PermanentStat::spd}) {
		auto const stat = stats[stat_name];
		write_stat(to_simulator_string(stat_name), stat.iv, stat.ev, pt);
	}
}

std::string_view to_simulator_string(Species const species) {
	switch (species) {
		case Species::Deoxys_Normal: return "Deoxys";
		case Species::Deoxys_Attack: return "Deoxys-f";
		case Species::Deoxys_Defense: return "Deoxys-l";
		case Species::Deoxys_Speed: return "Deoxys-e";
		case Species::Giratina_Altered: return "Giratina";
		case Species::Giratina_Origin: return "Giratina-o";
		case Species::Rotom_Mow: return "Rotom-c";
		case Species::Rotom_Frost: return "Rotom-f";
		case Species::Rotom_Heat: return "Rotom-h";
		case Species::Rotom_Fan: return "Rotom-s";
		case Species::Rotom_Wash: return "Rotom-w";
		case Species::Shaymin_Land: return "Shaymin";
		case Species::Shaymin_Sky: return "Shaymin-s";
		case Species::Wormadam_Plant: return "Wormadam";
		case Species::Wormadam_Sandy: return "Wormadam-g";
		case Species::Wormadam_Trash: return "Wormadam-s";
		default: return to_string(species);
	}
}

std::string_view to_simulator_string(Gender const gender) {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::genderless: return "No Gender";
		case Gender::male: return "Male";
	}
}

void write_pokemon(Generation const generation, Pokemon const & pokemon, boost::property_tree::ptree & pt) {
	boost::property_tree::ptree & member = pt.add ("pokemon", "");
	member.put("<xmlattr>.species", to_simulator_string(pokemon.species()));
	member.put ("nickname", "");
	member.put ("level", pokemon.level()());
	member.put ("happiness", pokemon.happiness());
	member.put ("gender", to_simulator_string(pokemon.gender()));
	member.put ("nature", to_string(pokemon.nature()));
	member.put ("item", to_string(pokemon.unmodified_item()));
	member.put ("ability", to_string(pokemon.initial_ability()));
	for (auto const & move : pokemon.regular_moves()) {
		write_move(move, member);
	}
	write_stats(generation, pokemon, member);
}

}	// namespace

void write_team(Generation const generation, Team const & team, std::filesystem::path const & file_name) {
	boost::property_tree::ptree pt;
	boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type> settings('\t', 1);
	boost::property_tree::ptree & t = pt.add ("shoddybattle", "");
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon(generation, pokemon, t);
	}
	write_xml(file_name.string(), pt, std::locale (), settings);
}

}	// namespace pl
}	// namespace technicalmachine
