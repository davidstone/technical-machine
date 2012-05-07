// Load teams
// Copyright (C) 2012 David Stone
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
#include <fstream>
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
#include "team_predictor.hpp"
#include "type.hpp"

#include "pokemon_lab/read_team_file.hpp"
#include "pokemon_online/read_team_file.hpp"

#include <iostream>

namespace technicalmachine {

namespace {

std::vector<boost::filesystem::path> open_directory_and_add_files (boost::filesystem::path const & team_file);

}	// unnamed namespace

// I copy + pasted this constructor as a temporary measure. When GCC 4.7 is
// released and there is support for delegating constructors and non-static
// data member initializers, I will replace this with one of (or both of) those
// facilities.

Team::Team () :
	damage (0),
	bide_damage (0),
	chance_to_hit (100),
	stage ({{}}),
	vanish (LANDED),
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
	substitute (0),
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
	flash_fire (false),
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
	me_first (false),
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

	me (false)
	{
}

Team::Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name) :
	damage (0),
	bide_damage (0),
	chance_to_hit (100),
	stage ({{}}),
	vanish (LANDED),
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
	substitute (0),
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
	flash_fire (false),
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
	me_first (false),
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

	me (true)
	{
	boost::filesystem::path team_file = team_file_name;
	std::vector <boost::filesystem::path> const files = open_directory_and_add_files (team_file);
	std::uniform_int_distribution <size_t> distribution (0, files.size () - 1);
	team_file = files [distribution (random_engine)];
	load (team_file.string(), foe_size);
	pokemon.for_each([](Pokemon & member) {
		member.set_hidden_power_type();
		member.calculate_initial_hp();
		member.new_hp = member.hp.max;
	});
}

Team::Team(Team const & other):
	pokemon (other.pokemon),
	shared_moves(other.shared_moves),
	damage(other.damage),
	bide_damage(other.bide_damage),
	chance_to_hit(other.chance_to_hit),
	stage(other.stage),
	vanish(other.vanish),
	bide(other.bide),
	confused(other.confused),
	embargo(other.embargo),
	encore(other.encore),
	heal_block(other.heal_block),
	magnet_rise(other.magnet_rise),
	partial_trap(other.partial_trap),
	perish_song(other.perish_song),
	rampage(other.rampage),
	slow_start(other.slow_start),
	stockpile(other.stockpile),
	substitute(other.substitute),
	taunt(other.taunt),
	toxic(other.toxic),
	uproar(other.uproar),
	yawn(other.yawn),
	aqua_ring(other.aqua_ring),
	attract(other.attract),
	awaken(other.awaken),
	ch(other.ch),
	charge(other.charge),
	curse(other.curse),
	damaged(other.damaged),
	defense_curl(other.defense_curl),
	destiny_bond(other.destiny_bond),
	endure(other.endure),
	flash_fire(other.flash_fire),
	flinch(other.flinch),
	focus_energy(other.focus_energy),
	fully_paralyzed(other.fully_paralyzed),
	gastro_acid(other.gastro_acid),
	hitself(other.hitself),
	identified(other.identified),
	imprison(other.imprison),
	ingrain(other.ingrain),
	leech_seed(other.leech_seed),
	loaf(other.loaf),
	lock_on(other.lock_on),
	me_first(other.me_first),
	minimize(other.minimize),
	miss(other.miss),
	moved(other.moved),
	mud_sport(other.mud_sport),
	nightmare(other.nightmare),
	pass(other.pass),
	power_trick(other.power_trick),
	protect(other.protect),
	recharging(other.recharging),
	replacing(other.replacing),
	roost(other.roost),
	shed_skin(other.shed_skin),
	torment(other.torment),
	trapped(other.trapped),
	u_turning(other.u_turning),
	water_sport(other.water_sport),
	counter(other.counter),
	light_screen(other.light_screen),
	lucky_chant(other.lucky_chant),
	mist(other.mist),
	reflect(other.reflect),
	safeguard(other.safeguard),
	tailwind(other.tailwind),
	wish(other.wish),
	spikes(other.spikes),
	toxic_spikes(other.toxic_spikes),
	stealth_rock(other.stealth_rock),
	called_move(other.called_move),
	me(other.me) {
	pokemon.for_each([& shared_moves](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

Team::Team(Team && other):
	pokemon(std::move(other.pokemon)),
	shared_moves(std::move(other.shared_moves)),
	damage(std::move(other.damage)),
	bide_damage(std::move(other.bide_damage)),
	chance_to_hit(std::move(other.chance_to_hit)),
	stage(std::move(other.stage)),
	vanish(std::move(other.vanish)),
	bide(std::move(other.bide)),
	confused(std::move(other.confused)),
	embargo(std::move(other.embargo)),
	encore(std::move(other.encore)),
	heal_block(std::move(other.heal_block)),
	magnet_rise(std::move(other.magnet_rise)),
	partial_trap(std::move(other.partial_trap)),
	perish_song(std::move(other.perish_song)),
	rampage(std::move(other.rampage)),
	slow_start(std::move(other.slow_start)),
	stockpile(std::move(other.stockpile)),
	substitute(std::move(other.substitute)),
	taunt(std::move(other.taunt)),
	toxic(std::move(other.toxic)),
	uproar(std::move(other.uproar)),
	yawn(std::move(other.yawn)),
	aqua_ring(std::move(other.aqua_ring)),
	attract(std::move(other.attract)),
	awaken(std::move(other.awaken)),
	ch(std::move(other.ch)),
	charge(std::move(other.charge)),
	curse(std::move(other.curse)),
	damaged(std::move(other.damaged)),
	defense_curl(std::move(other.defense_curl)),
	destiny_bond(std::move(other.destiny_bond)),
	endure(std::move(other.endure)),
	flash_fire(std::move(other.flash_fire)),
	flinch(std::move(other.flinch)),
	focus_energy(std::move(other.focus_energy)),
	fully_paralyzed(std::move(other.fully_paralyzed)),
	gastro_acid(std::move(other.gastro_acid)),
	hitself(std::move(other.hitself)),
	identified(std::move(other.identified)),
	imprison(std::move(other.imprison)),
	ingrain(std::move(other.ingrain)),
	leech_seed(std::move(other.leech_seed)),
	loaf(std::move(other.loaf)),
	lock_on(std::move(other.lock_on)),
	me_first(std::move(other.me_first)),
	minimize(std::move(other.minimize)),
	miss(std::move(other.miss)),
	moved(std::move(other.moved)),
	mud_sport(std::move(other.mud_sport)),
	nightmare(std::move(other.nightmare)),
	pass(std::move(other.pass)),
	power_trick(std::move(other.power_trick)),
	protect(std::move(other.protect)),
	recharging(std::move(other.recharging)),
	replacing(std::move(other.replacing)),
	roost(std::move(other.roost)),
	shed_skin(std::move(other.shed_skin)),
	torment(std::move(other.torment)),
	trapped(std::move(other.trapped)),
	u_turning(std::move(other.u_turning)),
	water_sport(std::move(other.water_sport)),
	counter(std::move(other.counter)),
	light_screen(std::move(other.light_screen)),
	lucky_chant(std::move(other.lucky_chant)),
	mist(std::move(other.mist)),
	reflect(std::move(other.reflect)),
	safeguard(std::move(other.safeguard)),
	tailwind(std::move(other.tailwind)),
	wish(std::move(other.wish)),
	spikes(std::move(other.spikes)),
	toxic_spikes(std::move(other.toxic_spikes)),
	stealth_rock(std::move(other.stealth_rock)),
	called_move(std::move(other.called_move)),
	me(std::move(other.me)) {
	pokemon.for_each([& shared_moves](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

void Team::add_pokemon (Species name) {
	pokemon.add(Pokemon(name, shared_moves));
}

void Team::add_pokemon (Species name, std::string const & nickname, unsigned level, Gender gender) {
	pokemon.add(name, nickname, level, gender, shared_moves);
}

void Team::remove_pokemon () {
	pokemon.remove_active();
	if (pokemon.is_empty())
		return;
	shared_moves.remove_switch();
}

bool Team::can_be_phazed () const {
	return !ingrain and !pokemon().ability.blocks_phazing() and pokemon.size() > 1;
}

namespace {

std::vector<boost::filesystem::path> open_directory_and_add_files (boost::filesystem::path const & team_file) {
	std::vector<boost::filesystem::path> files;
	if (boost::filesystem::is_directory (team_file)) {
		for (boost::filesystem::directory_iterator it (team_file); it != boost::filesystem::directory_iterator (); ++it) {
			auto const temp = open_directory_and_add_files (it->path ());
			files.insert (files.end(), temp.begin(), temp.end());
		}
	}
	else if (boost::filesystem::is_regular_file (team_file))
		files.push_back (team_file);
	return files;
}

}	// unnamed namespace

uint64_t Team::hash () const {
	uint64_t current_hash = 0;
	// Should probably think of a better way to combine Pokemon hashes than xor
	pokemon.for_each([& current_hash](Pokemon const & member) {
		current_hash ^= member.hash();
	});
	// hash is in the innermost nested parentheses, so all of the arguments
	// are promoted to uint64_t
	constexpr unsigned max_size = 6;
	return static_cast<unsigned> (pokemon.real_size() - 1) + max_size *
			(pokemon.index() + pokemon.real_size() *
			(vanish + Vanish::END_VANISH *
			(static_cast<unsigned> (stage [Stat::ATK] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::DEF] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::SPA] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::SPD] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::SPE] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::ACC] + 6) + (6 + 6 + 1) *
			(static_cast<unsigned> (stage [Stat::EVA] + 6) + (6 + 6 + 1) *
			(((bide_damage < 714 / 2) ? bide_damage : 714u / 2) + (714 / 2 + 1) *
			(bide + 3 *
			(confused + 5 *
			(embargo + 5 *
			(encore + 8 *
			(heal_block + 5 *
			(magnet_rise + 5 *
			(partial_trap + 8 *
			(perish_song + 3 *
			(rampage + 3 *
			(slow_start + 3 *
			(stockpile + 4 *
			(substitute + (714 / 4 + 1) *
			(taunt + 3 *
			(toxic + 16 *
			(uproar + 5 *
			(yawn + 2 *
			(aqua_ring + 2 *
			(attract + 2 *
			(charge + 2 *
			(curse + 2 *
			(defense_curl + 2 *
			(destiny_bond + 2 *
			(flash_fire + 2 *
			(focus_energy + 2 *
			(gastro_acid + 2 *
			(identified + 2 *
			(imprison + 2 *
			(ingrain + 2 *
			(leech_seed + 2 *
			(loaf + 2 *
			(lock_on + 2 *
			(minimize + 2 *
			(mud_sport + 2 *
			(nightmare + 2 *
			(power_trick + 2 *
			(recharging + 2 *
			(torment + 2 *
			(trapped + 2 *
			(water_sport + 2 *
			(counter + 3 *
			(light_screen + 8 *
			(lucky_chant + 5 *
			(mist + 5 *
			(reflect + 8 *
			(safeguard + 5 *
			(tailwind + 3 *
			(wish + 2 *
			(spikes + 4 *
			(toxic_spikes + 3 *
			(stealth_rock + 2 *
			current_hash))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
}

void Team::load (std::string const & name, unsigned other_size) {
	// I do no error checking because I assume my team files will always be in
	// the proper format. This must be changed if I ever allow arbitary teams
	// to be used.

	size_t const dot = name.rfind ('.');
	// I include '.' in the extension to protect against malformed file names.
	// If a file does not have a '.', I don't want to try to make a substr from
	// std::string::npos + 1.
	std::string extension = name.substr (dot);
	boost::algorithm::to_lower (extension);
	if (extension == ".tp")
		po::load_team (*this, name, other_size);
	else if (extension == ".sbt")
		pl::load_team (*this, name, other_size);
	else	// TODO: throw
		std::cerr << "Unsupported file format: " + extension + ".\n";
}

bool Team::operator== (Team const & other) const {
	return pokemon().name == other.pokemon().name and
			pokemon == other.pokemon and
			stage == other.stage and
			vanish == other.vanish and
			bide == other.bide and
			confused == other.confused and
			embargo == other.embargo and
			encore == other.encore and
			heal_block == other.heal_block and
			magnet_rise == other.magnet_rise and
			partial_trap == other.partial_trap and
			perish_song == other.perish_song and
			rampage == other.rampage and
			slow_start == other.slow_start and
			stockpile == other.stockpile and
			taunt == other.taunt and
			toxic == other.toxic and
			uproar == other.yawn and
			aqua_ring == other.aqua_ring and
			attract == other.attract and
			charge == other.charge and
			curse == other.curse and
			defense_curl == other.defense_curl and
			destiny_bond == other.destiny_bond and
			flash_fire == other.flash_fire and
			focus_energy == other.focus_energy and
			identified == other.identified and
			imprison == other.imprison and
			ingrain == other.ingrain and
			leech_seed == other.leech_seed and
			loaf == other.loaf and
			lock_on == other.lock_on and
			minimize == other.minimize and
			mud_sport == other.mud_sport and
			nightmare == other.nightmare and
			torment == other.torment and
			trapped == other.trapped and
			water_sport == other.water_sport and
			counter == other.counter and
			light_screen == other.light_screen and
			lucky_chant == other.lucky_chant and
			mist == other.mist and
			reflect == other.reflect and
			safeguard == other.safeguard and
			tailwind == other.tailwind and
			wish == other.wish and
			spikes == other.spikes and
			toxic_spikes == other.toxic_spikes and
			stealth_rock == other.stealth_rock and
			me == other.me;
}

std::string Team::to_string () const {
	std::string output = me ? "AI" : "Foe";
	output += "'s team:\n";
	pokemon.for_each([& output](Pokemon const & member) {
		output += member.to_string();
		double const d_per_cent_hp = 100.0 * member.hp.stat / member.hp.max;
		std::string const per_cent_hp = boost::lexical_cast <std::string> (boost::format ("%.1f") % d_per_cent_hp);
		output += " (" + per_cent_hp + "% HP)";
		output += " @ " + member.item.to_string ();
		output += " ** " + member.get_nickname() + '\n';
		if (member.ability.is_set ())
			output += "\tAbility: " + member.ability.to_string () + '\n';
		if (!member.status.is_clear())
			output += "\tStatus: " + member.status.to_string () + '\n';
		member.move.for_each_regular_move([& output](Move const & move) {
			output += "\t- " + move.to_string() + "\n";
		});
	});
	return output;
}

}	// namespace technicalmachine
