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
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "ability.hpp"
#include "damage.hpp"
#include "item.hpp"
#include "rational.hpp"
#include "status.hpp"

#include "pokemon/pokemon.hpp"

#include "clients/invalid_team_file_format.hpp"

#include "clients/pokemon_lab/read_team_file.hpp"
#include "clients/pokemon_online/read_team_file.hpp"

namespace technicalmachine {

namespace {

std::vector<boost::filesystem::path> open_directory_and_add_files (boost::filesystem::path const & team_file);

}	// unnamed namespace

Team::Team () :
	me(false)
	{
}

Team::Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name) :
	me(true)
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
	damage_done_to_active(other.damage_done_to_active),
	bide(other.bide),
	stage(other.stage),
	vanish(other.vanish),
	cached_chance_to_hit(other.cached_chance_to_hit),
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
	active_substitute(other.active_substitute),
	m_taunt(other.m_taunt),
	toxic(other.toxic),
	uproar(other.uproar),
	yawn(other.yawn),
	aqua_ring(other.aqua_ring),
	attracted(other.attracted),
	awakening(other.awakening),
	ch(other.ch),
	charge(other.charge),
	cursed(other.cursed),
	used_defense_curl(other.used_defense_curl),
	destiny_bond(other.destiny_bond),
	enduring(other.enduring),
	flash_fire(other.flash_fire),
	flinched_this_turn(other.flinched_this_turn),
	focusing_energy(other.focusing_energy),
	fully_paralyzed(other.fully_paralyzed),
	fully_trapped(other.fully_trapped),
	gastro_acid(other.gastro_acid),
	hitself(other.hitself),
	identified(other.identified),
	is_imprisoned(other.is_imprisoned),
	ingrain_active(other.ingrain_active),
	leech_seed(other.leech_seed),
	loaf(other.loaf),
	lock_on(other.lock_on),
	me_first(other.me_first),
	minimize(other.minimize),
	miss(other.miss),
	has_moved(other.has_moved),
	mud_sport(other.mud_sport),
	nightmares(other.nightmares),
	pass(other.pass),
	power_trick(other.power_trick),
	protecting(other.protecting),
	recharge_lock_in(other.recharge_lock_in),
	is_replacing(other.is_replacing),
	roosting(other.roosting),
	shedding_skin(other.shedding_skin),
	is_tormented(other.is_tormented),
	u_turning(other.u_turning),
	water_sport(other.water_sport),
	counter(other.counter),
	screens(other.screens),
	wish(other.wish),
	entry_hazards(other.entry_hazards),
	called_move(other.called_move),
	me(other.me) {
	pokemon.for_each([this](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

Team::Team(Team && other):
	pokemon(std::move(other.pokemon)),
	shared_moves(std::move(other.shared_moves)),
	damage_done_to_active(std::move(other.damage_done_to_active)),
	bide(std::move(other.bide)),
	stage(std::move(other.stage)),
	vanish(std::move(other.vanish)),
	cached_chance_to_hit(std::move(other.cached_chance_to_hit)),
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
	active_substitute(std::move(other.active_substitute)),
	m_taunt(std::move(other.m_taunt)),
	toxic(std::move(other.toxic)),
	uproar(std::move(other.uproar)),
	yawn(std::move(other.yawn)),
	aqua_ring(std::move(other.aqua_ring)),
	attracted(std::move(other.attracted)),
	awakening(std::move(other.awakening)),
	ch(std::move(other.ch)),
	charge(std::move(other.charge)),
	cursed(std::move(other.cursed)),
	used_defense_curl(std::move(other.used_defense_curl)),
	destiny_bond(std::move(other.destiny_bond)),
	enduring(std::move(other.enduring)),
	flash_fire(std::move(other.flash_fire)),
	flinched_this_turn(std::move(other.flinched_this_turn)),
	focusing_energy(std::move(other.focusing_energy)),
	fully_paralyzed(std::move(other.fully_paralyzed)),
	fully_trapped(std::move(other.fully_trapped)),
	gastro_acid(std::move(other.gastro_acid)),
	hitself(std::move(other.hitself)),
	identified(std::move(other.identified)),
	is_imprisoned(std::move(other.is_imprisoned)),
	ingrain_active(std::move(other.ingrain_active)),
	leech_seed(std::move(other.leech_seed)),
	loaf(std::move(other.loaf)),
	lock_on(std::move(other.lock_on)),
	me_first(std::move(other.me_first)),
	minimize(std::move(other.minimize)),
	miss(std::move(other.miss)),
	has_moved(std::move(other.has_moved)),
	mud_sport(std::move(other.mud_sport)),
	nightmares(std::move(other.nightmares)),
	pass(std::move(other.pass)),
	power_trick(std::move(other.power_trick)),
	protecting(std::move(other.protecting)),
	recharge_lock_in(std::move(other.recharge_lock_in)),
	is_replacing(std::move(other.is_replacing)),
	roosting(std::move(other.roosting)),
	shedding_skin(std::move(other.shedding_skin)),
	is_tormented(std::move(other.is_tormented)),
	u_turning(std::move(other.u_turning)),
	water_sport(std::move(other.water_sport)),
	counter(std::move(other.counter)),
	screens(std::move(other.screens)),
	wish(std::move(other.wish)),
	entry_hazards(std::move(other.entry_hazards)),
	called_move(std::move(other.called_move)),
	me(std::move(other.me)) {
	pokemon.for_each([this](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

Team & Team::operator= (Team const & other) = default;
Team & Team::operator= (Team && other) = default;

void Team::remove_pokemon () {
	pokemon.remove_active();
	if (pokemon.is_empty())
		return;
	shared_moves.remove_switch();
}

void Team::reset_end_of_turn() {
	damage_done_to_active = 0;
	enduring = false;
	flinched_this_turn = false;
	has_moved = false;
	me_first = false;
	loaf = !loaf;
	protecting = false;
	is_replacing = false;
}

void Team::reset_switch() {
	if (!pass) {
		aqua_ring = false;
		cursed = false;
		focusing_energy = false;
		gastro_acid = false;
		ingrain_active = false;
		leech_seed = false;
		lock_on = false;
		power_trick = false;
		confused = 0;
		embargo = 0;
		magnet_rise = 0;
		perish_song = 0;
		stage.reset();
		active_substitute.destroy();
	}
	attracted = false;
	charge = false;
	used_defense_curl = false;
	destiny_bond = false;
	flash_fire = false;
	flinched_this_turn = false;
	fully_trapped = false;
	identified = false;
	is_imprisoned = false;
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	loaf = false;
	minimize = false;
	me_first = false;
	mud_sport = false;
	nightmares = false;
	pass = false;
	roosting = false;
	is_tormented = false;
	u_turning = false;
	water_sport = false;
	bide.reset();
	damage_done_to_active = 0;
	encore = 0;
	heal_block = 0;
	partial_trap = 0;
	rampage = 0;
	slow_start = 0;
	stockpile = 0;
	m_taunt.reset();
	toxic.reset();
	uproar.reset();
	// Whirlwind can hit Flying Pokemon, so it needs to be reset
	vanish.reset();
	yawn.reset();

	pokemon().move.for_each([](Move & move) {
		move.reset();
	});
}

void Team::reset_between_turns() {
	pokemon.for_each ([](Pokemon & pokemon)->void {
		pokemon.move.reset_index();
	});
	ch = false;
	fully_paralyzed = false;
	hitself = false;
	miss = false;
	pokemon.initialize_replacement();
}

void Team::update_before_move() {
	destiny_bond = false;
	lock_on = false;
	has_moved = true;
}

void Team::clear_field() {
	entry_hazards.clear();
	leech_seed = false;
	partial_trap = false;
}

bool Team::is_loafing() const {
	return pokemon().ability.is_loafing(loaf);
}

bool Team::aqua_ring_is_active() const {
	return aqua_ring;
}

void Team::activate_aqua_ring() {
	aqua_ring = true;
}

void Team::attract() {
	attracted = true;
}

void Team::awaken(bool const value) {
	awakening = value;
}

void Team::baton_pass() {
	pass = true;
}

bool Team::cannot_be_koed() const {
	return enduring;
}

void Team::charged() {
	charge = true;
}

bool Team::charge_boosted() const {
	return charge and pokemon().move().type() == Type::ELECTRIC;
}

bool Team::critical_hit() const {
	return ch;
}

void Team::set_critical_hit(bool const value) {
	ch = value;
}

void Team::curse() {
	cursed = true;
}

bool Team::is_cursed() const {
	return cursed;
}

void Team::defense_curl() {
	used_defense_curl = true;
}

void Team::use_destiny_bond() {
	destiny_bond = true;
}

bool Team::defense_curled() const {
	return used_defense_curl;
}

void Team::endure() {
	enduring = true;
}

bool Team::flash_fire_is_active() const {
	return flash_fire;
}

void Team::activate_flash_fire() {
	flash_fire = true;
}

bool Team::flinched() const {
	return flinched_this_turn;
}

void Team::flinch() {
	flinched_this_turn = true;
}

void Team::focus_energy() {
	focusing_energy = true;
}

void Team::fully_trap() {
	fully_trapped = true;
}

void Team::identify() {
	identified = true;
}

bool Team::imprisoned() const {
	return is_imprisoned;
}

void Team::imprison() {
	is_imprisoned = true;
}

bool Team::ingrained() const {
	return ingrain_active;
}

void Team::ingrain() {
	ingrain_active = true;
}

bool Team::is_fully_paralyzed() const {
	return fully_paralyzed;
}

bool Team::leech_seeded() const {
	return leech_seed;
}

void Team::hit_with_leech_seed() {
	leech_seed = true;
}

bool Team::locked_on() const {
	return lock_on;
}

void Team::lock_on_to() {
	lock_on = true;
}

bool Team::me_first_is_active() const {
	return me_first;
}

bool Team::minimized() const {
	return minimize;
}

bool Team::missed() const {
	return miss;
}

void Team::set_miss(bool const value) {
	miss = value;
}

void Team::move(bool const value) {
	has_moved = value;
}

bool Team::moved() const {
	return has_moved;
}

void Team::activate_mud_sport() {
	mud_sport = true;
}

bool Team::nightmare() const {
	return nightmares;
}

void Team::give_nightmares() {
	nightmares = true;
}

bool Team::power_trick_is_active() const {
	return power_trick;
}

void Team::activate_power_trick() {
	power_trick = !power_trick;
}

void Team::protect() {
	protecting = true;
}

void Team::break_protect() {
	protecting = false;
}

bool Team::recharging() const {
	return recharge_lock_in;
}

bool Team::recharge() {
	bool const return_value = recharging();
	recharge_lock_in = false;
	return return_value;
}

void Team::use_recharge_move() {
	recharge_lock_in = true;
}

bool Team::replacing() const {
	return is_replacing;
}

void Team::not_replacing() {
	is_replacing = false;
}

bool Team::is_roosting() const {
	return roosting;
}

void Team::roost() {
	roosting = true;
}

bool Team::shed_skin_activated() const {
	return shedding_skin;
}

void Team::shed_skin(bool const value) {
	shedding_skin = value;
}

bool Team::sport_is_active(Move const & foe_move) const {
	Type const & type = foe_move.type();
	if (type == Type::ELECTRIC)
		return mud_sport;
	else if (type == Type::FIRE)
		return water_sport;
	else
		return false;
}

bool Team::switch_decision_required() const {
	return pass or u_turning;
}

bool Team::trapped() const {
	return !fully_trapped and !partial_trap and !ingrained();
}

bool Team::tormented() const {
	return is_tormented;
}

void Team::torment() {
	is_tormented = true;
}

void Team::taunt() {
	m_taunt.activate();
}

bool Team::is_taunted() const {
	return m_taunt.is_active();
}

void Team::increment_taunt() {
	m_taunt.increment();
}

Rational Team::toxic_ratio() const {
	return toxic.ratio_drained();
}

void Team::increment_toxic() {
	toxic.increment();
}

void Team::u_turn() {
	u_turning = true;
}

void Team::increment_uproar() {
	uproar.increment();
}

void Team::use_uproar() {
	uproar.increment();
}

void Team::activate_water_sport() {
	water_sport = true;
}

bool Team::decrement_yawn() {
	return yawn.decrement();
}

void Team::hit_with_yawn() {
	yawn.activate();
}

void Team::use_bide(Pokemon & target) {
	if (!bide.is_active()) {
		bide.activate();
	}
	else {
		unsigned const bide_damage = bide.decrement();
		if (bide_damage != 0)
			damage_side_effect(target, bide_damage * 2);
	}
}

void Team::substitute() {
	if (!active_substitute and pokemon().can_use_substitute()) {
		active_substitute.create(pokemon().hp.max);
		pokemon().hp.stat -= pokemon().hp.max / 4;
	}
}

bool Team::is_hitting_self() const {
	return hitself;
}

bool Team::is_locked_in_to_bide() const {
	return bide.is_active();
}

void Team::lower_pp(Ability const & target) {
	if (!is_locked_in_to_bide())
		pokemon().move().pp.decrement(target);
}

void Team::activate_perish_song() {
	if (!perish_song)
		perish_song = 3;
}

void Team::increase_sleep_counter() {
	pokemon().status.increase_sleep_counter(pokemon().ability, awakening);
}

bool Team::can_be_phazed () const {
	return !ingrained() and !pokemon().ability.blocks_phazing() and pokemon.size() > 1;
}

unsigned Team::damaged() const {
	return damage_done_to_active;
}

void Team::do_damage(unsigned const damage) {
	damage_done_to_active = damage;
	bide.add_damage(damage);
}

bool Team::has_switched() const {
	return moved() and pokemon().move().is_switch();
}

void Team::update_chance_to_hit(Team const & target, Weather const & weather, bool const target_moved) {
	cached_chance_to_hit.update(*this, target, weather, target_moved);
}

ChanceToHit::value_type Team::chance_to_hit() const {
	return cached_chance_to_hit();
}

ChanceToHit::value_type Team::chance_to_miss() const {
	return cached_chance_to_hit.inverse();
}

bool Team::can_miss() const {
	return cached_chance_to_hit.can_miss();
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
		files.emplace_back(team_file);
	return files;
}

}	// unnamed namespace

uint64_t Team::hash () const {
	uint64_t current_hash = pokemon.hash();
	current_hash *= entry_hazards.max_hash();
	current_hash += entry_hazards.hash();
	current_hash *= wish.max_hash();
	current_hash += wish.hash();
	current_hash *= screens.max_hash();
	current_hash += screens.hash();
	current_hash *= active_substitute.max_hash();
	current_hash += active_substitute.hash();
	current_hash *= m_taunt.max_hash();
	current_hash += m_taunt.hash();
	current_hash *= toxic.max_hash();
	current_hash += toxic.hash();
	current_hash *= uproar.max_hash();
	current_hash += uproar.hash();
	current_hash *= yawn.max_hash();
	current_hash += yawn.hash();
	current_hash *= bide.max_hash();
	current_hash += bide.hash();
	current_hash *= stage.max_hash();
	current_hash += stage.hash();
	current_hash *= vanish.max_hash();
	current_hash += vanish.hash();
	current_hash *= 8;
	current_hash += encore;
	current_hash *= 8;
	current_hash += partial_trap;
	current_hash *= 5;
	current_hash += confused;
	current_hash *= 5;
	current_hash += embargo;
	current_hash *= 5;
	current_hash += heal_block;
	current_hash *= 5;
	current_hash += magnet_rise;
	current_hash *= 4;
	current_hash += stockpile;
	current_hash *= 3;
	current_hash += counter;
	current_hash *= 3;
	current_hash += perish_song;
	current_hash *= 3;
	current_hash += rampage;
	current_hash *= 3;
	current_hash += slow_start;
	current_hash *= 2;
	current_hash += aqua_ring;
	current_hash *= 2;
	current_hash += attracted;
	current_hash *= 2;
	current_hash += charge;
	current_hash *= 2;
	current_hash += cursed;
	current_hash *= 2;
	current_hash += used_defense_curl;
	current_hash *= 2;
	current_hash += destiny_bond;
	current_hash *= 2;
	current_hash += flash_fire;
	current_hash *= 2;
	current_hash += focusing_energy;
	current_hash *= 2;
	current_hash += fully_trapped;
	current_hash *= 2;
	current_hash += gastro_acid;
	current_hash *= 2;
	current_hash += identified;
	current_hash *= 2;
	current_hash += is_imprisoned;
	current_hash *= 2;
	current_hash += ingrain_active;
	current_hash *= 2;
	current_hash += leech_seed;
	current_hash *= 2;
	current_hash += loaf;
	current_hash *= 2;
	current_hash += lock_on;
	current_hash *= 2;
	current_hash += minimize;
	current_hash *= 2;
	current_hash += mud_sport;
	current_hash *= 2;
	current_hash += nightmares;
	current_hash *= 2;
	current_hash += power_trick;
	current_hash *= 2;
	current_hash += recharge_lock_in;
	current_hash *= 2;
	current_hash += is_tormented;
	current_hash *= 2;
	current_hash += water_sport;
	current_hash *= pokemon.real_size();
	current_hash += pokemon.index();
	constexpr unsigned max_size = 6;
	current_hash *= max_size;
	current_hash += static_cast<uint64_t>(pokemon.real_size() - 1);
	return current_hash;
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
	else
		throw InvalidTeamFileFormat(name);
}

bool operator== (Team const & lhs, Team const & rhs) {
	return lhs.pokemon().name == rhs.pokemon().name and
			lhs.pokemon == rhs.pokemon and
			lhs.stage == rhs.stage and
			lhs.vanish == rhs.vanish and
			lhs.bide == rhs.bide and
			lhs.confused == rhs.confused and
			lhs.embargo == rhs.embargo and
			lhs.encore == rhs.encore and
			lhs.heal_block == rhs.heal_block and
			lhs.magnet_rise == rhs.magnet_rise and
			lhs.partial_trap == rhs.partial_trap and
			lhs.perish_song == rhs.perish_song and
			lhs.rampage == rhs.rampage and
			lhs.slow_start == rhs.slow_start and
			lhs.stockpile == rhs.stockpile and
			lhs.m_taunt == rhs.m_taunt and
			lhs.toxic == rhs.toxic and
			lhs.uproar == rhs.uproar and
			lhs.yawn == rhs.yawn and
			lhs.aqua_ring == rhs.aqua_ring and
			lhs.attracted == rhs.attracted and
			lhs.charge == rhs.charge and
			lhs.cursed == rhs.cursed and
			lhs.used_defense_curl == rhs.used_defense_curl and
			lhs.destiny_bond == rhs.destiny_bond and
			lhs.flash_fire == rhs.flash_fire and
			lhs.focusing_energy == rhs.focusing_energy and
			lhs.identified == rhs.identified and
			lhs.is_imprisoned == rhs.is_imprisoned and
			lhs.ingrain_active == rhs.ingrain_active and
			lhs.leech_seed == rhs.leech_seed and
			lhs.loaf == rhs.loaf and
			lhs.lock_on == rhs.lock_on and
			lhs.minimize == rhs.minimize and
			lhs.mud_sport == rhs.mud_sport and
			lhs.nightmares == rhs.nightmares and
			lhs.is_tormented == rhs.is_tormented and
			lhs.fully_trapped == rhs.fully_trapped and
			lhs.water_sport == rhs.water_sport and
			lhs.counter == rhs.counter and
			lhs.screens == rhs.screens and
			lhs.wish == rhs.wish and
			lhs.entry_hazards == rhs.entry_hazards and
			lhs.me == rhs.me;
}

bool operator!= (Team const & lhs, Team const & rhs) {
	return !(lhs == rhs);
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
