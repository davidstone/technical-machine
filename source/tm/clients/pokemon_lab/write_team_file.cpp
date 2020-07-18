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

#include <tm/string_conversions/move.hpp>

namespace technicalmachine {
namespace pl {

auto write_move(Move const move, boost::property_tree::ptree & pt) -> void {
	boost::property_tree::ptree & m = pt.add("moveset.move", to_string(move.name()));
	// TODO: replace with real PP ups logic
	m.put ("<xmlattr>.pp-up", 3);
}

auto write_stat(std::string_view const name, IV const iv, EV const ev, boost::property_tree::ptree & pt) -> void {
	boost::property_tree::ptree & s = pt.add("stats.stat", "");
	s.put("<xmlattr>.name", name);
	s.put("<xmlattr>.iv", iv.value());
	s.put("<xmlattr>.ev", ev.value());
}

auto to_simulator_string(PermanentStat const stat) -> std::string_view {
	switch (stat) {
		case PermanentStat::hp: return "HP";
		case PermanentStat::atk: return "Atk";
		case PermanentStat::def: return "Def";
		case PermanentStat::spa: return "SpAtk";
		case PermanentStat::spd: return "SpDef";
		case PermanentStat::spe: return "Spd";
	}
}

auto to_simulator_string(Species const species) -> std::string_view {
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

auto to_simulator_string(Gender const gender) -> std::string_view {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::genderless: return "No Gender";
		case Gender::male: return "Male";
	}
}

} // namespace pl
} // namespace technicalmachine
