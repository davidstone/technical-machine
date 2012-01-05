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

#include "file.hpp"

#include <fstream>
#include <string>

#include "conversion.hpp"
#include "../ability.hpp"
#include "../item.hpp"
#include "../move.hpp"
#include "../pokemon.hpp"
#include "../species.hpp"
#include "../team.hpp"

namespace technicalmachine {
namespace po {

static void load_pokemon (Team & team, std::ifstream & file, unsigned size);
static int converter (std::string const & data, std::string const & end, std::string const & line);

void load_team (Team & team, std::string const & name, unsigned size) {
	std::ifstream file (name);
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
	int const species_id = converter ("Num=\"", "\"", line);
	int const forme = converter ("Forme=\"", "\"", line);
	Pokemon member (id_to_species (species_id, forme), team.size);

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
	if (member.name != Species::END and member.move.set.size() != 0)
		team.pokemon.set.push_back (member);
}

int converter (std::string const & data, std::string const & end, std::string const & line) {
	size_t const x = data.length();
	size_t const a = line.find (data);
	size_t const b = line.find (end, a + x);
	return std::stoi (line.substr (a + x, b - a - x));
}

} // namespace po
} // namespace technicalmachine
