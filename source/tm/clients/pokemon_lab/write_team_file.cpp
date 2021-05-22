// Write Pokemon Lab teams
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/string_conversions/move.hpp>

namespace technicalmachine {
namespace pl {

auto write_move(Move const move, boost::property_tree::ptree & pt) -> void {
	boost::property_tree::ptree & m = pt.add("moveset.move", to_string(move.name()));
	// TODO: replace with real PP ups logic
	m.put ("<xmlattr>.pp-up", 3);
}

auto to_simulator_string(SplitSpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP";
		case SplitSpecialPermanentStat::atk: return "Atk";
		case SplitSpecialPermanentStat::def: return "Def";
		case SplitSpecialPermanentStat::spa: return "SpAtk";
		case SplitSpecialPermanentStat::spd: return "SpDef";
		case SplitSpecialPermanentStat::spe: return "Spd";
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
