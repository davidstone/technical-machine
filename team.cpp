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

unsigned long Team::hash () const {
	unsigned long hash = 0;
	for (std::vector<Pokemon>::const_iterator it = pokemon.set.begin(); it != pokemon.set.end(); ++it)
		hash ^= it->hash();
	// Accuracy and Evasion are +6 to account for negative stat boosts
//	std::cout << size + 6 * pokemon.index + 36 * vanish + 216 * bide_damage + 77328 * substitute + 13764384 * bide + 41293152 * confused + 206465760 * embargo + 1032328800 * encore + 8258630400 * heal_block + 41293152000 * magnet_rise + 206465760000 * partial_trap + 1651726080000 * perish_song + 4955178240000 * rampage + 14865534720000 * slow_start + 44596604160000 * stockpile + 178386416640000 * taunt + 535159249920000 * toxic + 8562547998720000 * uproar + 42812739993600000 * yawn + 85625479987200000 * aqua_ring + 171250959974400000 * attract + 342501919948800000 * charge + 685003839897600000 * curse + 1370007679795200000 * defense_curl + 2740015359590400000 * destiny_bond + 5480030719180800000 * ff + 10960061438361600000 * focus_energy + 21920122876723200000 * gastro_acid + 43840245753446400000 * identified + 87680491506892800000 * imprison + 175360983013785600000 * ingrain + 350721966027571200000 * leech_seed + 701443932055142400000 * loaf + 1402887864110284800000 * lock_on + 2805775728220569600000 * minimize + 5611551456441139200000 * mud_sport + 11223102912882278400000 * nightmare + 22446205825764556800000 * power_trick + 44892411651529113600000 * recharging + 89784823303058227200000 * torment + 179569646606116454400000 * trapped + 359139293212232908800000 * water_sport + 718278586424465817600000 * (accuracy + 6) + 9337621623518055628800000 * (evasion + 6) + 121389081105734723174400000 * counter + 364167243317204169523200000 * light_screen + 2913337946537633356185600000 * lucky_chant + 14566689732688166780928000000 * mist + 72833448663440833904640000000 * reflect + 582667589307526671237120000000 * safeguard + 2913337946537633356185600000000 * tailwind + 8740013839612900068556800000000 * wish + 17480027679225800137113600000000 * spikes + 69920110716903200548454400000000 * toxic_spikes + 209760332150709601645363200000000 * stealth_rock + 419520664301419203290726400000000 * hash << '\n';
//	std::cout << size + 6 * (pokemon.index + 6 * (vanish + 6 * (bide_damage + 358 * (substitute + 178 * (bide + 3 * (confused + 5 * (embargo + 5 * (encore + 8 * (heal_block + 5 * (magnet_rise + 5 * (partial_trap + 8 * (perish_song + 3 * (rampage + 3 * (slow_start + 3 * (stockpile + 4 * (taunt + 3 * (toxic + 16 * (uproar + 5 * (yawn + 2 * (aqua_ring + 2 * (attract + 2 * (charge + 2 * (curse + 2 * (defense_curl + 2 * (destiny_bond + 2 * (ff + 2 * (focus_energy + 2 * (gastro_acid + 2 * (identified + 2 * (imprison + 2 * (ingrain + 2 * (leech_seed + 2 * (loaf + 2 * (lock_on + 2 * (minimize + 2 * (mud_sport + 2 * (nightmare + 2 * (power_trick + 2 * (recharging + 2 * (torment + 2 * (trapped + 2 * (water_sport + 2 * ((accuracy + 6) + 13 * ((evasion + 6) + 13 * (counter + 3 * (light_screen + 8 * (lucky_chant + 5 * (mist + 5 * (reflect + 8 * (safeguard + 5 * (tailwind + 3 * (wish + 2 * (spikes + 4 * (toxic_spikes + 3 * (stealth_rock + 2 * hash))))))))))))))))))))))))))))))))))))))))))))))))))))))) << '\n';
	return size + 6 * (pokemon.index + 6 * (vanish + 6 * (bide_damage + 358 * (substitute + 178 * (bide + 3 * (confused + 5 * (embargo + 5 * (encore + 8 * (heal_block + 5 * (magnet_rise + 5 * (partial_trap + 8 * (perish_song + 3 * (rampage + 3 * (slow_start + 3 * (stockpile + 4 * (taunt + 3 * (toxic + 16 * (uproar + 5 * (yawn + 2 * (aqua_ring + 2 * (attract + 2 * (charge + 2 * (curse + 2 * (defense_curl + 2 * (destiny_bond + 2 * (ff + 2 * (focus_energy + 2 * (gastro_acid + 2 * (identified + 2 * (imprison + 2 * (ingrain + 2 * (leech_seed + 2 * (loaf + 2 * (lock_on + 2 * (minimize + 2 * (mud_sport + 2 * (nightmare + 2 * (power_trick + 2 * (recharging + 2 * (torment + 2 * (trapped + 2 * (water_sport + 2 * ((accuracy + 6) + 13 * ((evasion + 6) + 13 * (counter + 3 * (light_screen + 8 * (lucky_chant + 5 * (mist + 5 * (reflect + 8 * (safeguard + 5 * (tailwind + 3 * (wish + 2 * (spikes + 4 * (toxic_spikes + 3 * (stealth_rock + 2 * hash)))))))))))))))))))))))))))))))))))))))))))))))))))))));
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
		output += it->get_name();
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
