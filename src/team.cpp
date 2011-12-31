// Load teams
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

#include "team.hpp"

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "ability.hpp"
#include "gender.hpp"
#include "item.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "switch.hpp"
#include "status.hpp"
#include "teampredictor.hpp"
#include "type.hpp"

#include "pokemon_lab/file.hpp"
#include "pokemon_online/file.hpp"

namespace technicalmachine {

static void open_directory_and_add_files (boost::filesystem::path const & team_file, std::vector<boost::filesystem::path> & files);

Team::Team (bool isme, unsigned size) :
	vanish (LANDED),
	damage (0),
	bide_damage (0),
	substitute (0),
	chance_to_hit (100),
	stage ({}),
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
	protect (false),
	recharging (false),
	// Initial switch mechanics are the same as replacing a fainted Pokemon
	replacing (true),
	roost (false),
	shed_skin (false),
	torment (false),
	trapped (false),
	u_turning (false),
	water_sport (false),
	
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
		boost::filesystem::path team_file;
		std::ifstream settings ("settings/settings.txt");
		std::string const comment = "//";
		std::string const delimiter = ": ";
		for (getline (settings, line); !settings.eof(); getline (settings, line)) {
			if (line.substr (0, comment.length ()) != comment and !line.empty ()) {
				size_t found = line.find (delimiter);
				std::string const data = line.substr (0, found);
				if (data == "username") {
					player = line.substr (found + delimiter.length());
					boost::algorithm::trim (player);
				}
				else if (data == "team") {
					std::string team_file_name = line.substr (found + delimiter.length());
					boost::algorithm::trim (team_file_name);
					team_file = team_file_name;
				}
			}
		}
		settings.close();
		std::vector <boost::filesystem::path> files;
		open_directory_and_add_files (team_file, files);
		srand (static_cast <unsigned> (time (0)));
		team_file = files [(rand () % files.size ())];
		load (team_file.string(), size);
		for (Pokemon & member : pokemon.set)
			member.new_hp = member.hp.max;
	}
}

void open_directory_and_add_files (boost::filesystem::path const & team_file, std::vector<boost::filesystem::path> & files) {
	if (boost::filesystem::is_directory (team_file)) {
		for (boost::filesystem::directory_iterator it (team_file); it != boost::filesystem::directory_iterator (); ++it)
			open_directory_and_add_files (it->path (), files);
	}
	else if (boost::filesystem::is_regular_file (team_file))
		files.push_back (team_file);
}

bool Team::is_switching_to_self () const {
	return pokemon.set [pokemon().move().name - Move::SWITCH0].name == pokemon().name;
}

bool Team::seen_pokemon (Species name) {
	for (replacement = 0; replacement != pokemon.set.size(); ++replacement) {
		if (name == at_replacement().name)
			return true;
	}
	return false;
}

void Team::add_pokemon (Species name, std::string const & nickname, int level, Gender gender) {
	Pokemon member (name, size);
	member.level = level;
	member.gender = gender;

	member.nickname = nickname;

	pokemon.set.push_back (member);
	replacement = pokemon.set.size() - 1;
	pokemon.set.back().load();
}

uint64_t Team::hash () const {
	uint64_t hash = 0;
	for (Pokemon const & member : pokemon.set)
		hash ^= member.hash();
	return size + 6 * (pokemon.index + 6 * (vanish + 6 * (stage [Stat::ATK] + 13 * (stage [Stat::DEF] + 13 * (stage [Stat::SPA] + 13 * (stage [Stat::SPD] + 13 * (stage [Stat::SPE] + 13 * (stage [Stat::ACC] + 13 * (stage [Stat::EVA] + 13 * (bide_damage + 358 * (substitute + 178 * (bide + 3 * (confused + 5 * (embargo + 5 * (encore + 8 * (heal_block + 5 * (magnet_rise + 5 * (partial_trap + 8 * (perish_song + 3 * (rampage + 3 * (slow_start + 3 * (stockpile + 4 * (taunt + 3 * (toxic + 16 * (uproar + 5 * (yawn + 2 * (aqua_ring + 2 * (attract + 2 * (charge + 2 * (curse + 2 * (defense_curl + 2 * (destiny_bond + 2 * (ff + 2 * (focus_energy + 2 * (gastro_acid + 2 * (identified + 2 * (imprison + 2 * (ingrain + 2 * (leech_seed + 2 * (loaf + 2 * (lock_on + 2 * (minimize + 2 * (mud_sport + 2 * (nightmare + 2 * (power_trick + 2 * (recharging + 2 * (torment + 2 * (trapped + 2 * (water_sport + 2 * (counter + 3 * (light_screen + 8 * (lucky_chant + 5 * (mist + 5 * (reflect + 8 * (safeguard + 5 * (tailwind + 3 * (wish + 2 * (spikes + 4 * (toxic_spikes + 3 * (stealth_rock + 2 * hash))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
}

void Team::load (std::string const & name, unsigned size) {
	// I do no error checking because I assume my team files will always be in
	// the proper format. This must be changed if I ever allow arbitary teams
	// to be used.

	if (name.substr (name.length() - 3) == ".tp")
		po::load_team (*this, name, size);
	else if (name.substr (name.length() - 4) == ".sbt")
		pl::load_team (*this, name, size);
	else
		std::cerr << "Unsupported file format.\n";
	for (Pokemon & member : pokemon.set)
		member.load();
}

bool Team::operator== (Team const & other) const {
	if (pokemon.set.size() != other.pokemon.set.size())
		return false;
	for (size_t n = 0; n != pokemon.set.size(); ++n) {
		if (pokemon.set [n] != other.pokemon.set [n])
			return false;
	}
	for (Stat::Stats stat = Stat::ATK; stat != Stat::END_STAT; stat = static_cast <Stat::Stats> (stat + 1)) {
		if (stage [stat] != other.stage [stat])
			return false;
	}
	return pokemon().name == other.pokemon().name and vanish == other.vanish and bide == other.bide and confused == other.confused and embargo == other.embargo and encore == other.encore and heal_block == other.heal_block and magnet_rise == other.magnet_rise and partial_trap == other.partial_trap and perish_song == other.perish_song and rampage == other.rampage and slow_start == other.slow_start and stockpile == other.stockpile and taunt == other.taunt and toxic == other.toxic and uproar == other.yawn and aqua_ring == other.aqua_ring and attract == other.attract and charge == other.charge and curse == other.curse and defense_curl == other.defense_curl and destiny_bond == other.destiny_bond and ff == other.ff and focus_energy == other.focus_energy and identified == other.identified and imprison == other.imprison and ingrain == other.ingrain and leech_seed == other.leech_seed and loaf == other.loaf and lock_on == other.lock_on and minimize == other.minimize and mud_sport == other.mud_sport and nightmare == other.nightmare and torment == other.torment and trapped == other.trapped and water_sport == other.water_sport and counter == other.counter and light_screen == other.light_screen and lucky_chant == other.lucky_chant and mist == other.mist and reflect == other.reflect and safeguard == other.safeguard and tailwind == other.tailwind and wish == other.wish and spikes == other.spikes and toxic_spikes == other.toxic_spikes and stealth_rock == other.stealth_rock and me == other.me;
}

Pokemon& Team::at_replacement () {
	return pokemon.set [replacement];
}
Pokemon const & Team::at_replacement () const {
	return pokemon.set [replacement];
}

void Team::output (std::string & output) const {
	if (me)
		output = "AI";
	else
		output = "Foe";
	output += " team:\n";
	output += player + ":\n";
	for (Pokemon const & member : pokemon.set) {
		output += member.to_string();
		std::string const per_cent_hp = boost::lexical_cast <std::string> (boost::format ("%.1f") % (100.0 * static_cast<double> (member.hp.stat) / static_cast<double> (member.hp.max)));
		output += " (" + per_cent_hp + "% HP)";
		output += " @ " + member.item.to_string ();
		output += " ** " + member.nickname + '\n';
		if (member.ability.is_set ())
			output += "\tAbility: " + member.ability.to_string () + '\n';
		if (member.status.name != Status::NO_STATUS)
			output += "\tStatus: " + member.status.to_string () + "\n";
		for (std::vector<Move>::const_iterator move = member.move.set.begin(); move->name != Move::STRUGGLE; ++move)
			output += "\t- " + move->to_string() + "\n";
	}
}

}
