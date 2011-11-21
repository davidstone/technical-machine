// Load Pokemon Online teams
// Copyright (C) 2011 David Stone
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

#include "file.h"
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "conversion.h"
#include "../ability.h"
#include "../item.h"
#include "../move.h"
#include "../pokemon.h"
#include "../species.h"
#include "../team.h"

namespace technicalmachine {
namespace po {

static void load_pokemon (Team & team, std::ifstream & file, unsigned size);
static unsigned converter (std::string const & data, std::string const & end, std::string const & line);

void load_team (Team & team, std::string const & name, unsigned size) {
	std::ifstream file (name.c_str());
	std::string line;
	getline (file, line);
	for (unsigned n = 0; n != 6; ++n)
		load_pokemon (team, file, size);
	file.close ();
	team.size = team.pokemon.set.size ();
}

void load_pokemon (Team & team, std::ifstream & file, unsigned size) {
	std::string line;
	getline (file, line);
	getline (file, line);
	Pokemon member (id_to_species (converter ("Num=\"", "\"", line)), team.size);
	int forme = converter ("Forme=\"", "\"", line);
	switch (member.name) {
		case DEOXYS_A:
			switch (forme) {
				case 0:
					member.name = DEOXYS_M;
					break;
				// case 1: is assumed
				case 1:
					break;
				case 2:
					member.name = DEOXYS_D;
					break;
				case 3:
					member.name = DEOXYS_S;
					break;
			}
			break;
		case GIRATINA_A:
			// Giratina-O has a forme value of 1
			member.name = static_cast<Species> (member.name + forme);
			break;
		case ROTOM:
			switch (forme) {
				case 1:
					member.name = ROTOM_C;
					break;
				case 2:
					member.name = ROTOM_H;
					break;
				case 3:
					member.name = ROTOM_F;
					break;
				case 4:
					member.name = ROTOM_W;
					break;
				case 5:
					member.name = ROTOM_S;
					break;
			}
			break;
		case SHAYMIN_L:
			// Shaymin-S has a forme value of 1
			member.name = static_cast<Species> (member.name + forme);
			break;
		case WORMADAM_P:
			// Wormadam-P is 0, Wormadam-S is 1, Wormadam-T is 2
			member.name = static_cast<Species> (member.name + forme);
		default:
			break;
	}

	member.ability.name = id_to_ability (converter ("Ability=\"", "\"", line));

	member.item.name = id_to_item (converter ("Item=\"", "\"", line));
	
	member.nature.name = static_cast <Nature::Natures> (converter ("Nature=\"", "\"", line));
	
	member.level = converter ("Lvl=\"", "\"", line);
	
	std::string const data = "Nickname=\"";
	size_t const x = data.length();
	size_t const a = line.find (data);
	size_t const b = line.find ("\"", a + x);
	member.nickname = line.substr (a + x, b - a - x);
	
	member.happiness = converter ("Happiness=\"", "\"", line);
	
	member.gender.from_simulator_int (converter ("Gender=\"", "\"", line));

	for (unsigned n = 0; n != 4; ++n) {
		getline (file, line);
		Move move (id_to_move (converter ("<Move>", "</Move>", line)), 3, size);
		member.move.set.push_back (move);
	}
	getline (file, line);
	member.hp.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.atk.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.def.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spa.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spd.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spe.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.hp.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.atk.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.def.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spa.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spd.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spe.ev = converter ("<EV>", "</EV>", line);
	if (member.name != END_SPECIES and member.move.set.size() != 0)
		team.pokemon.set.push_back (member);
}

unsigned converter (std::string const & data, std::string const & end, std::string const & line) {
	size_t const x = data.length();
	size_t const a = line.find (data);
	size_t const b = line.find (end, a + x);
	return boost::lexical_cast<unsigned> (line.substr (a + x, b - a - x));
}

} // namespace po
} // namespace technicalmachine
