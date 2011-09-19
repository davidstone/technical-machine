// Load teams
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "switch.h"
#include "team.h"
#include "teampredictor.h"
#include "type.h"

#include "pokemon_lab/file.h"
#include "pokemon_online/file.h"

namespace technicalmachine {

Team::Team (bool isme, Map const & map, unsigned size) :
	vanish (LANDED),
	damage (0),
	bide_damage (0),
	substitute (0),
	bide (0),
	chance_to_hit (100),
	confused (0),
	embargo (0),
	encore (0),
	heal_block (0),
	magnet_rise (0),
	partial_trap (0),
	perish_song (0),
	rampage (0),
	slow_start (0),
	stockpile (0),
	taunt (0),
	toxic (0),
	uproar (0),
	yawn (0),
	aqua_ring (false),
	attract (false),
	awaken (false),
	ch (false),
	charge (false),
	curse (false),
	damaged (false),
	defense_curl (false),
	destiny_bond (false),
	endure (false),
	ff (false),
	flinch (false),
	focus_energy (false),
	fully_paralyzed (false),
	gastro_acid (false),
	hitself (false),
	identified (false),
	imprison (false),
	ingrain (false),
	leech_seed (false),
	loaf (false),
	lock_on (false),
	mf (false),
	minimize (false),
	miss (false),
	moved (false),
	mud_sport (false),
	nightmare (false),
	pass (false),
	power_trick (false),
	protect (false),
	recharging (false),
	replacing (false),
	roost (false),
	shed_skin (false),
	torment (false),
	trapped (false),
	water_sport (false),
	accuracy (0),
	evasion (0),
	
	counter (0),

	light_screen (0),
	lucky_chant (0),
	mist (0),
	reflect (0),
	safeguard (0),
	tailwind (0),

	wish (0),

	spikes (0),
	toxic_spikes (0),
	stealth_rock (false),

	replacement (0),
	size (6),
	me (isme)
{
	if (me) {
		std::string line;
		std::string team_file;
		std::ifstream settings ("settings.txt");
		for (getline (settings, line); !settings.eof(); getline (settings, line)) {
			std::string const delimiter = ": ";
			size_t found = line.find (delimiter);
			if (line.substr (0, found) == "username")
				player = line.substr (found + delimiter.length());
			else if (line.substr (0, found) == "team")
				team_file = line.substr (found + delimiter.length());
		}
		settings.close();

		load ("teams/" + team_file, map, size);
		for (std::vector <Pokemon>::iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it)
			it->new_hp = it->hp.max;
	}
}

bool Team::is_switching_to_self () const {
	return pokemon.set [pokemon->move->name - Move::SWITCH0].name == pokemon->name;
}

uint64_t Team::hash () const {
	uint64_t hash = 0;
	for (std::vector<Pokemon>::const_iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it)
		hash ^= it->hash();
	// Accuracy and Evasion are +6 to account for negative stat boosts
	return size + 6 * (pokemon.index + 6 * (vanish + 6 * (bide_damage + 358 * (substitute + 178 * (bide + 3 * (confused + 5 * (embargo + 5 * (encore + 8 * (heal_block + 5 * (magnet_rise + 5 * (partial_trap + 8 * (perish_song + 3 * (rampage + 3 * (slow_start + 3 * (stockpile + 4 * (taunt + 3 * (toxic + 16 * (uproar + 5 * (yawn + 2 * (aqua_ring + 2 * (attract + 2 * (charge + 2 * (curse + 2 * (defense_curl + 2 * (destiny_bond + 2 * (ff + 2 * (focus_energy + 2 * (gastro_acid + 2 * (identified + 2 * (imprison + 2 * (ingrain + 2 * (leech_seed + 2 * (loaf + 2 * (lock_on + 2 * (minimize + 2 * (mud_sport + 2 * (nightmare + 2 * (power_trick + 2 * (recharging + 2 * (torment + 2 * (trapped + 2 * (water_sport + 2 * ((accuracy + 6) + 13 * ((evasion + 6) + 13 * (counter + 3 * (light_screen + 8 * (lucky_chant + 5 * (mist + 5 * (reflect + 8 * (safeguard + 5 * (tailwind + 3 * (wish + 2 * (spikes + 4 * (toxic_spikes + 3 * (stealth_rock + 2 * hash)))))))))))))))))))))))))))))))))))))))))))))))))))))));
}

// Warning: Almost everything you see here is a hack.

// I do no error checking because I assume Pokelab's teams will always be in the proper format. This must be changed if I ever allow arbitary teams to be used.

void Team::load (std::string const & name, Map const & map, unsigned size) {
	if (name.substr (name.length() - 3) == ".tp")
		po::load_team (*this, name, size);
	else if (name.substr (name.length() - 4) == ".sbt")
		pl::load_team (*this, name, map, size);
	else
		std::cerr << "Unsupported file format.\n";
	for (std::vector<Pokemon>::iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it)
		it->load();
}

bool Team::operator== (Team const & other) const {
	if (pokemon.set.size() != other.pokemon.set.size())
		return false;
	for (size_t n = 0; n != pokemon.set.size(); ++n) {
		if (pokemon.set [n] != other.pokemon.set [n])
			return false;
	}
	return pokemon->name == other.pokemon->name and vanish == other.vanish and bide == other.bide and confused == other.confused and embargo == other.embargo and encore == other.encore and heal_block == other.heal_block and magnet_rise == other.magnet_rise and partial_trap == other.partial_trap and perish_song == other.perish_song and rampage == other.rampage and slow_start == other.slow_start and stockpile == other.stockpile and taunt == other.taunt and toxic == other.toxic and uproar == other.yawn and aqua_ring == other.aqua_ring and attract == other.attract and charge == other.charge and curse == other.curse and defense_curl == other.defense_curl and destiny_bond == other.destiny_bond and ff == other.ff and focus_energy == other.focus_energy and identified == other.identified and imprison == other.imprison and ingrain == other.ingrain and leech_seed == other.leech_seed and loaf == other.loaf and lock_on == other.lock_on and minimize == other.minimize and mud_sport == other.mud_sport and nightmare == other.nightmare and torment == other.torment and trapped == other.trapped and water_sport == other.water_sport and accuracy == other.accuracy and evasion == other.evasion and counter == other.counter and light_screen == other.light_screen and lucky_chant == other.lucky_chant and mist == other.mist and reflect == other.reflect and safeguard == other.safeguard and tailwind == other.tailwind and wish == other.wish and spikes == other.spikes and toxic_spikes == other.toxic_spikes and stealth_rock == other.stealth_rock and me == other.me;
}

Pokemon& Team::at_replacement () {
	return pokemon.set [replacement];
}
Pokemon const & Team::at_replacement () const {
	return pokemon.set [replacement];
}

void Team::output (std::string & output) {
	if (me)
		output = "AI";
	else
		output = "Foe";
	output += " team:\n";
	output += player + ":\n";
	for (std::vector<Pokemon>::const_iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it) {
		output += it->get_name();
		output += " (" + boost::lexical_cast<std::string> (100.0 * static_cast<double> (it->hp.stat) / static_cast<double> (it->hp.max)) + "% HP)";
		output += " @ " + it->item.get_name ();
		output += " ** " + it->nickname + '\n';
		if (it->ability.is_set ())
			output += "\tAbility: " + it->ability.get_name () + '\n';
		if (it->status != NO_STATUS)
			output += "\tStatus: " + boost::lexical_cast<std::string> (it->status) + '\n';
		for (std::vector<Move>::const_iterator move = it->move.set.begin(); move->name != Move::STRUGGLE; ++move)
			output += "\t- " + move->get_name() + "\n";
	}
}

}
