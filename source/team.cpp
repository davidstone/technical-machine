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
	active_pokemon(other.active_pokemon),
	stage(other.stage),
	embargo(other.embargo),
	encore(other.encore),
	heal_block(other.heal_block),
	magnet_rise(other.magnet_rise),
	rampage(other.rampage),
	slow_start(other.slow_start),
	stockpile(other.stockpile),
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
	active_pokemon(std::move(other.active_pokemon)),
	stage(std::move(other.stage)),
	embargo(std::move(other.embargo)),
	encore(std::move(other.encore)),
	heal_block(std::move(other.heal_block)),
	magnet_rise(std::move(other.magnet_rise)),
	rampage(std::move(other.rampage)),
	slow_start(std::move(other.slow_start)),
	stockpile(std::move(other.stockpile)),
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
	active_pokemon.reset_end_of_turn();
}

void Team::reset_switch() {
	if (!active_pokemon.is_baton_passing()) {
		embargo = 0;
		magnet_rise = 0;
		stage.reset();
	}
	encore = 0;
	heal_block = 0;
	rampage = 0;
	slow_start = 0;
	stockpile = 0;
	active_pokemon.reset_switch();

	pokemon().move.for_each([](Move & move) {
		move.reset();
	});
}

void Team::reset_between_turns() {
	active_pokemon.reset_between_turns();
	pokemon.for_each ([](Pokemon & pokemon)->void {
		pokemon.move.reset_index();
	});
	pokemon.initialize_replacement();
}

void Team::update_before_move() {
	active_pokemon.update_before_move();
}

void Team::clear_field() {
	active_pokemon.clear_field();
	entry_hazards.clear();
}

bool Team::is_loafing() const {
	return active_pokemon.is_loafing(pokemon().ability);
}

bool Team::aqua_ring_is_active() const {
	return active_pokemon.aqua_ring_is_active();
}

void Team::activate_aqua_ring() {
	active_pokemon.activate_aqua_ring();
}

void Team::attract() {
	active_pokemon.attract();
}

void Team::awaken(bool const value) {
	active_pokemon.awaken(value);
}

void Team::baton_pass() {
	active_pokemon.baton_pass();
}

void Team::bounce() {
	active_pokemon.bounce();
}

bool Team::cannot_be_koed() const {
	return active_pokemon.cannot_be_koed();
}

void Team::charged() {
	active_pokemon.charge();
}

bool Team::charge_boosted() const {
	return active_pokemon.charge_boosted() and pokemon().move().type() == Type::ELECTRIC;
}

bool Team::is_confused() const {
	return active_pokemon.is_confused();
}

void Team::confuse() {
	if (!pokemon().ability.blocks_confusion())
		active_pokemon.confuse();
}

void Team::handle_confusion() {
	active_pokemon.handle_confusion(pokemon());
}

bool Team::critical_hit() const {
	return active_pokemon.critical_hit();
}

void Team::set_critical_hit(bool const value) {
	active_pokemon.set_critical_hit(value);
}

void Team::curse() {
	active_pokemon.curse();
}

bool Team::is_cursed() const {
	return active_pokemon.is_cursed();
}

void Team::defense_curl() {
	active_pokemon.defense_curl();
}

void Team::use_destiny_bond() {
	active_pokemon.use_destiny_bond();
}

bool Team::defense_curled() const {
	return active_pokemon.defense_curled();
}

void Team::dig() {
	active_pokemon.dig();
}

void Team::dive() {
	active_pokemon.dive();
}

void Team::endure() {
	active_pokemon.endure();
}

bool Team::flash_fire_is_active() const {
	return active_pokemon.flash_fire_is_active();
}

void Team::activate_flash_fire() {
	active_pokemon.activate_flash_fire();
}

bool Team::flinched() const {
	return active_pokemon.flinched();
}

void Team::flinch() {
	active_pokemon.flinch();
}

void Team::fly() {
	active_pokemon.fly();
}

void Team::focus_energy() {
	active_pokemon.focus_energy();
}

void Team::fully_trap() {
	active_pokemon.fully_trap();
}

void Team::identify() {
	active_pokemon.identify();
}

bool Team::imprisoned() const {
	return active_pokemon.imprisoned();
}

void Team::imprison() {
	active_pokemon.imprison();
}

bool Team::ingrained() const {
	return active_pokemon.ingrained();
}

void Team::ingrain() {
	active_pokemon.ingrain();
}

bool Team::is_fully_paralyzed() const {
	return active_pokemon.is_fully_paralyzed();
}

bool Team::leech_seeded() const {
	return active_pokemon.leech_seeded();
}

void Team::hit_with_leech_seed() {
	active_pokemon.hit_with_leech_seed();
}

bool Team::locked_on() const {
	return active_pokemon.locked_on();
}

void Team::lock_on_to() {
	active_pokemon.lock_on_to();
}

bool Team::me_first_is_active() const {
	return active_pokemon.me_first_is_active();
}

bool Team::minimized() const {
	return active_pokemon.minimized();
}

bool Team::missed() const {
	return active_pokemon.missed();
}

void Team::set_miss(bool const value) {
	active_pokemon.set_miss(value);
}

void Team::move(bool const value) {
	active_pokemon.move(value);
}

bool Team::moved() const {
	return active_pokemon.moved();
}

void Team::activate_mud_sport() {
	active_pokemon.activate_mud_sport();
}

bool Team::nightmare() const {
	return active_pokemon.nightmare();
}

void Team::give_nightmares() {
	active_pokemon.give_nightmares();
}

void Team::partially_trap(bool const extended) {
	active_pokemon.partially_trap(extended);
}

void Team::partial_trap_damage() {
	active_pokemon.partial_trap_damage(pokemon());
}

void Team::perish_song_turn() {
	bool const faint = active_pokemon.perish_song_turn();
	if (faint) {
		pokemon().hp.stat = 0;
	}
}

bool Team::power_trick_is_active() const {
	return active_pokemon.power_trick_is_active();
}

void Team::activate_power_trick() {
	active_pokemon.activate_power_trick();
}

void Team::protect() {
	active_pokemon.protect();
}

void Team::break_protect() {
	active_pokemon.break_protect();
}

bool Team::recharging() const {
	return active_pokemon.recharging();
}

bool Team::recharge() {
	return active_pokemon.recharge();
}

void Team::use_recharge_move() {
	active_pokemon.use_recharge_move();
}

bool Team::replacing() const {
	return active_pokemon.replacing();
}

void Team::not_replacing() {
	active_pokemon.not_replacing();
}

bool Team::is_roosting() const {
	return active_pokemon.is_roosting();
}

void Team::roost() {
	active_pokemon.roost();
}

void Team::shadow_force() {
	active_pokemon.shadow_force();
}

bool Team::shed_skin_activated() const {
	return active_pokemon.shed_skin_activated();
}

void Team::shed_skin(bool const value) {
	active_pokemon.shed_skin(value);
}

bool Team::sport_is_active(Move const & foe_move) const {
	return active_pokemon.sport_is_active(foe_move);
}

bool Team::switch_decision_required() const {
	return active_pokemon.switch_decision_required();
}

bool Team::trapped() const {
	return !active_pokemon.trapped();
}

bool Team::tormented() const {
	return active_pokemon.tormented();
}

void Team::torment() {
	active_pokemon.torment();
}

void Team::taunt() {
	active_pokemon.taunt();
}

bool Team::is_taunted() const {
	return active_pokemon.is_taunted();
}

void Team::increment_taunt() {
	active_pokemon.increment_taunt();
}

Rational Team::toxic_ratio() const {
	return active_pokemon.toxic_ratio();
}

void Team::increment_toxic() {
	active_pokemon.increment_toxic();
}

void Team::u_turn() {
	active_pokemon.u_turn();
}

void Team::increment_uproar() {
	active_pokemon.increment_uproar();
}

void Team::use_uproar() {
	active_pokemon.use_uproar();
}

bool Team::vanish_doubles_power(Moves const move_name) const {
	return active_pokemon.vanish_doubles_power(move_name);
}

void Team::activate_water_sport() {
	active_pokemon.activate_water_sport();
}

bool Team::decrement_yawn() {
	return active_pokemon.decrement_yawn();
}

void Team::hit_with_yawn() {
	active_pokemon.hit_with_yawn();
}

void Team::use_bide(Pokemon & target) {
	active_pokemon.use_bide(target);
}

void Team::substitute() {
	if (pokemon().can_use_substitute()) {
		bool const created = active_pokemon.substitute(pokemon().hp.max);
		if (created)
			pokemon().hp.stat -= pokemon().hp.max / 4;
	}
}

bool Team::is_locked_in_to_bide() const {
	return active_pokemon.is_locked_in_to_bide();
}

void Team::lower_pp(Ability const & target) {
	if (!is_locked_in_to_bide())
		pokemon().move().pp.decrement(target);
}

void Team::activate_perish_song() {
	active_pokemon.activate_perish_song();
}

void Team::increase_sleep_counter() {
	active_pokemon.increase_sleep_counter(pokemon());
}

bool Team::can_be_phazed () const {
	return !ingrained() and !pokemon().ability.blocks_phazing() and pokemon.size() > 1;
}

unsigned Team::damaged() const {
	return active_pokemon.damaged();
}

void Team::do_damage(unsigned const damage) {
	active_pokemon.do_damage(damage);
}

bool Team::has_switched() const {
	return moved() and pokemon().move().is_switch();
}

void Team::update_chance_to_hit(Team const & target, Weather const & weather, bool const target_moved) {
	active_pokemon.update_chance_to_hit(*this, target, weather, target_moved);
}

ChanceToHit::value_type Team::chance_to_hit() const {
	return active_pokemon.chance_to_hit();
}

ChanceToHit::value_type Team::chance_to_miss() const {
	return active_pokemon.chance_to_miss();
}

bool Team::can_miss() const {
	return active_pokemon.can_miss();
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
	current_hash *= active_pokemon.max_hash();
	current_hash += active_pokemon.hash();
	current_hash *= entry_hazards.max_hash();
	current_hash += entry_hazards.hash();
	current_hash *= wish.max_hash();
	current_hash += wish.hash();
	current_hash *= screens.max_hash();
	current_hash += screens.hash();
	current_hash *= stage.max_hash();
	current_hash += stage.hash();
	current_hash *= 8;
	current_hash += encore;
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
	current_hash += rampage;
	current_hash *= 3;
	current_hash += slow_start;
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
			lhs.active_pokemon == rhs.active_pokemon and
			lhs.stage == rhs.stage and
			lhs.embargo == rhs.embargo and
			lhs.encore == rhs.encore and
			lhs.heal_block == rhs.heal_block and
			lhs.magnet_rise == rhs.magnet_rise and
			lhs.rampage == rhs.rampage and
			lhs.slow_start == rhs.slow_start and
			lhs.stockpile == rhs.stockpile and
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
