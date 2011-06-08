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

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "stat.h"
#include "statfunction.h"
#include "switch.h"
#include "team.h"
#include "teampredictor.h"
#include "type.h"

#include "pokemon_lab/file.h"
#include "pokemon_online/file.h"

namespace technicalmachine {

Team::Team (bool isme, Map const &map) :
	vanish (LANDED),
	damage (0),
	bide_damage (0),
	substitute (0),
	bide (0),
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
	me (isme) {
	if (me) {
		std::string line;
		std::string team_file;
		std::ifstream settings ("settings.txt");
		for (getline (settings, line); !settings.eof(); getline (settings, line)) {
			size_t found = line.find ('\t');
			if (line.substr (0, found) == "username")
				player = line.substr (found + 1);
			else if (line.substr (0, found) == "team")
				team_file = line.substr (found + 1);
		}
		settings.close();

		load ("teams/" + team_file, map);
	}
}

// Warning: Almost everything you see here is a hack.

// I do no error checking because I assume Pokelab's teams will always be in the proper format. This must be changed if I ever allow arbitary teams to be used.

void Team::load (std::string const &name, Map const &map) {
	if (name.substr (name.length() - 3) == ".tp")
		po::load_team (*this, name);
	else			// if (name.substr (name.length() - 4) == ".sbt")
		pl::load_team (*this, name, map);
	for (std::vector<Pokemon>::iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it)
		it->load();
}

bool Team::operator== (Team const &other) const {
	if (this->pokemon.set.size() != other.pokemon.set.size())
		return false;
	for (size_t n = 0; n != this->pokemon.set.size(); ++n) {
		if (this->pokemon.set [n] != other.pokemon.set [n])
			return false;
	}
	return this->pokemon->name == other.pokemon->name and this->vanish == other.vanish and this->bide == other.bide and this->confused == other.confused and this->embargo == other.embargo and this->encore == other.encore and this->heal_block == other.heal_block and this->magnet_rise == other.magnet_rise and this->partial_trap == other.partial_trap and this->perish_song == other.perish_song and this->rampage == other.rampage and this->slow_start == other.slow_start and this->stockpile == other.stockpile and this->taunt == other.taunt and this->toxic == other.toxic and this->uproar == other.yawn and this->aqua_ring == other.aqua_ring and this->attract == other.attract and this->charge == other.charge and this->curse == other.curse and this->defense_curl == other.defense_curl and this->destiny_bond == other.destiny_bond and this->ff == other.ff and this->focus_energy == other.focus_energy and this->identified == other.identified and this->imprison == other.imprison and this->ingrain == other.ingrain and this->leech_seed == other.leech_seed and this->loaf == other.loaf and this->lock_on == other.lock_on and this->minimize == other.minimize and this->mud_sport == other.mud_sport and this->nightmare == other.nightmare and this->torment == other.torment and this->trapped == other.trapped and this->water_sport == other.water_sport and this->accuracy == other.accuracy and this->evasion == other.evasion and this->counter == other.counter and this->light_screen == other.light_screen and this->lucky_chant == other.lucky_chant and this->mist == other.mist and this->reflect == other.reflect and this->safeguard == other.safeguard and this->tailwind == other.tailwind and this->wish == other.wish and this->spikes == other.spikes and this->toxic_spikes == other.toxic_spikes and this->stealth_rock == other.stealth_rock and this->me == other.me;
}

Pokemon& Team::at_replacement () {
	return pokemon.set [replacement];
}
Pokemon const & Team::at_replacement () const {
	return pokemon.set [replacement];
}

void Team::output (std::string &output) {
	if (me)
		output = "AI";
	else
		output = "Foe";
	output += " team:\n";
	output += player + ":\n";
	for (std::vector<Pokemon>::const_iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it) {
		output += pokemon_name [it->name];
		output += " (" + boost::lexical_cast<std::string> (100.0 * static_cast<double> (it->hp.stat) / static_cast<double> (it->hp.max)) + "% HP)";
		output += " @ " + item_name [it->item];
		output += " ** " + it->nickname + '\n';
		if (it->ability != END_ABILITY)
			output += "\tAbility: " + ability_name [it->ability] + '\n';
		for (std::vector<Move>::const_iterator move = it->move.set.begin(); move->name != STRUGGLE; ++move)
			output += "\t- " + move->get_name() + "\n";
	}
}

}
