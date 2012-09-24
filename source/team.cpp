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

Team::Team() :
	active_pokemon(m_all_pokemon),
	me(false)
	{
}

Team::Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name) :
	active_pokemon(m_all_pokemon),
	me(true)
	{
	boost::filesystem::path team_file = team_file_name;
	std::vector <boost::filesystem::path> const files = open_directory_and_add_files (team_file);
	std::uniform_int_distribution <size_t> distribution (0, files.size () - 1);
	team_file = files [distribution (random_engine)];
	load (team_file.string(), foe_size);
	all_pokemon().for_each([](Pokemon & member) {
		member.set_hidden_power_type();
		member.calculate_initial_hp();
		member.new_hp = member.hp.max;
	});
}

Team::Team(Team const & other):
	m_all_pokemon (other.m_all_pokemon),
	shared_moves(other.shared_moves),
	active_pokemon(other.active_pokemon),
	screens(other.screens),
	wish(other.wish),
	entry_hazards(other.entry_hazards),
	me(other.me) {
	all_pokemon().for_each([this](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

Team::Team(Team && other):
	m_all_pokemon(std::move(other.m_all_pokemon)),
	shared_moves(std::move(other.shared_moves)),
	active_pokemon(std::move(other.active_pokemon)),
	screens(std::move(other.screens)),
	wish(std::move(other.wish)),
	entry_hazards(std::move(other.entry_hazards)),
	me(std::move(other.me)) {
	all_pokemon().for_each([this](Pokemon & p) {
		p.move.update_shared_moves(shared_moves);
	});
}

Team & Team::operator= (Team const & other) = default;
Team & Team::operator= (Team && other) = default;

ActivePokemon const & Team::pokemon() const {
	return active_pokemon;
}

ActivePokemon & Team::pokemon() {
	return active_pokemon;
}

Pokemon const & Team::pokemon(PokemonCollection::index_type const index) const {
	return m_all_pokemon(index);
}
Pokemon & Team::pokemon(PokemonCollection::index_type const index) {
	return m_all_pokemon(index);
}

Pokemon const & Team::replacement() const {
	return m_all_pokemon.at_replacement();
}
Pokemon & Team::replacement() {
	return m_all_pokemon.at_replacement();
}

void Team::remove_pokemon () {
	all_pokemon().remove_active();
	if (all_pokemon().is_empty())
		return;
	shared_moves.remove_switch();
}

PokemonCollection const & Team::all_pokemon() const {
	return m_all_pokemon;
}

PokemonCollection & Team::all_pokemon() {
	return m_all_pokemon;
}

unsigned Team::number_of_seen_pokemon() const {
	return all_pokemon().size();
}
unsigned Team::size() const {
	return all_pokemon().real_size();
}

bool Team::is_me() const {
	return me;
}

void Team::reset_end_of_turn() {
	pokemon().reset_end_of_turn();
}

void Team::reset_switch() {
	pokemon().reset_switch();

	pokemon().get_pokemon().move.for_each([](Move & move) {
		move.reset();
	});
}

void Team::reset_between_turns() {
	pokemon().reset_between_turns();
	all_pokemon().for_each ([](Pokemon & pokemon)->void {
		pokemon.move.reset_index();
	});
	all_pokemon().initialize_replacement();
}

void Team::update_before_move() {
	pokemon().update_before_move();
}

void Team::clear_field() {
	pokemon().clear_field();
	entry_hazards.clear();
}

bool Team::is_loafing() const {
	return pokemon().is_loafing();
}

bool Team::aqua_ring_is_active() const {
	return pokemon().aqua_ring_is_active();
}

void Team::activate_aqua_ring() {
	pokemon().activate_aqua_ring();
}

void Team::attract() {
	pokemon().attract();
}

void Team::awaken(bool const value) {
	pokemon().awaken(value);
}

void Team::baton_pass() {
	pokemon().baton_pass();
}

void Team::bounce() {
	pokemon().bounce();
}

bool Team::cannot_be_koed() const {
	return pokemon().cannot_be_koed();
}

void Team::charged() {
	pokemon().charge();
}

bool Team::charge_boosted() const {
	return pokemon().charge_boosted() and pokemon().move().type() == Type::ELECTRIC;
}

bool Team::is_confused() const {
	return pokemon().is_confused();
}

void Team::confuse() {
	pokemon().confuse();
}

void Team::handle_confusion() {
	pokemon().handle_confusion();
}

bool Team::critical_hit() const {
	return pokemon().critical_hit();
}

void Team::set_critical_hit(bool const value) {
	pokemon().set_critical_hit(value);
}

void Team::curse() {
	pokemon().curse();
}

bool Team::is_cursed() const {
	return pokemon().is_cursed();
}

void Team::defense_curl() {
	pokemon().defense_curl();
}

void Team::use_destiny_bond() {
	pokemon().use_destiny_bond();
}

bool Team::defense_curled() const {
	return pokemon().defense_curled();
}

void Team::dig() {
	pokemon().dig();
}

void Team::dive() {
	pokemon().dive();
}

void Team::activate_embargo() {
	pokemon().activate_embargo();
}

void Team::decrement_embargo() {
	pokemon().decrement_embargo();
}

bool Team::is_encored() const {
	return pokemon().is_encored();
}

void Team::activate_encore() {
	pokemon().activate_encore();
}

void Team::increment_encore() {
	pokemon().increment_encore();
}

void Team::endure() {
	pokemon().endure();
}

bool Team::flash_fire_is_active() const {
	return pokemon().flash_fire_is_active();
}

void Team::activate_flash_fire() {
	pokemon().activate_flash_fire();
}

bool Team::flinched() const {
	return pokemon().flinched();
}

void Team::flinch() {
	pokemon().flinch();
}

void Team::fly() {
	pokemon().fly();
}

void Team::focus_energy() {
	pokemon().focus_energy();
}

void Team::fully_trap() {
	pokemon().fully_trap();
}

bool Team::heal_block_is_active() const {
	return pokemon().heal_block_is_active();
}

void Team::activate_heal_block() {
	pokemon().activate_heal_block();
}

void Team::decrement_heal_block() {
	pokemon().decrement_heal_block();
}

void Team::identify() {
	pokemon().identify();
}

bool Team::imprisoned() const {
	return pokemon().imprisoned();
}

void Team::imprison() {
	pokemon().imprison();
}

bool Team::ingrained() const {
	return pokemon().ingrained();
}

void Team::ingrain() {
	pokemon().ingrain();
}

bool Team::is_fully_paralyzed() const {
	return pokemon().is_fully_paralyzed();
}

bool Team::leech_seeded() const {
	return pokemon().leech_seeded();
}

void Team::hit_with_leech_seed() {
	pokemon().hit_with_leech_seed();
}

void Team::decrement_lock_in() {
	pokemon().decrement_lock_in();
}

bool Team::locked_on() const {
	return pokemon().locked_on();
}

void Team::lock_on_to() {
	pokemon().lock_on_to();
}

bool Team::magnet_rise_is_active() const {
	return pokemon().magnet_rise_is_active();
}

void Team::activate_magnet_rise() {
	pokemon().activate_magnet_rise();
}

void Team::decrement_magnet_rise() {
	pokemon().decrement_magnet_rise();
}

bool Team::me_first_is_active() const {
	return pokemon().me_first_is_active();
}

bool Team::minimized() const {
	return pokemon().minimized();
}

bool Team::missed() const {
	return pokemon().missed();
}

void Team::set_miss(bool const value) {
	pokemon().set_miss(value);
}

void Team::move(bool const value) {
	pokemon().set_moved(value);
}

bool Team::moved() const {
	return pokemon().moved();
}

void Team::activate_mud_sport() {
	pokemon().activate_mud_sport();
}

bool Team::nightmare() const {
	return pokemon().nightmare();
}

void Team::give_nightmares() {
	pokemon().give_nightmares();
}

void Team::partially_trap(bool const extended) {
	pokemon().partially_trap(extended);
}

void Team::partial_trap_damage() {
	pokemon().partial_trap_damage();
}

void Team::perish_song_turn() {
	bool const faint = pokemon().perish_song_turn();
	if (faint) {
		pokemon().hp().stat = 0;
	}
}

bool Team::power_trick_is_active() const {
	return pokemon().power_trick_is_active();
}

void Team::activate_power_trick() {
	pokemon().activate_power_trick();
}

void Team::protect() {
	pokemon().protect();
}

void Team::break_protect() {
	pokemon().break_protect();
}

void Team::activate_rampage() {
	pokemon().activate_rampage();
}

bool Team::recharging() const {
	return pokemon().recharging();
}

bool Team::recharge() {
	return pokemon().recharge();
}

void Team::use_recharge_move() {
	pokemon().use_recharge_move();
}

bool Team::replacing() const {
	return pokemon().replacing();
}

void Team::not_replacing() {
	pokemon().not_replacing();
}

bool Team::is_roosting() const {
	return pokemon().is_roosting();
}

void Team::roost() {
	pokemon().roost();
}

void Team::shadow_force() {
	pokemon().shadow_force();
}

bool Team::shed_skin_activated() const {
	return pokemon().shed_skin_activated();
}

void Team::shed_skin(bool const value) {
	pokemon().shed_skin(value);
}

bool Team::slow_start_is_active() const {
	return pokemon().slow_start_is_active();
}

bool Team::sport_is_active(Move const & foe_move) const {
	return pokemon().sport_is_active(foe_move);
}

unsigned Team::spit_up_power() const {
	return pokemon().spit_up_power();
}

unsigned Team::positive_stat_boosts() const {
	return pokemon().positive_stat_boosts();
}

void Team::stat_boost(Stat::Stats const stat, int const n) {
	pokemon().stat_boost(stat, n);
}

void Team::stat_boost_physical(int n) {
	pokemon().stat_boost_physical(n);
}

void Team::stat_boost_special(int n) {
	pokemon().stat_boost_special(n);
}

void Team::stat_boost_defensive(int n) {
	pokemon().stat_boost_defensive(n);
}

void Team::stat_boost_offensive(int n) {
	pokemon().stat_boost_offensive(n);
}

void Team::stat_boost_regular(int n) {
	pokemon().stat_boost_regular(n);
}

void Team::reset_stats() {
	pokemon().reset_stats();
}

void Team::copy_stat_boosts(Team const & other) {
	pokemon().copy_stat_boosts(other.active_pokemon);
}

void Team::swap_defensive_stages(Team & lhs, Team & rhs) {
	ActivePokemon::swap_defensive_stages(lhs.active_pokemon, rhs.active_pokemon);
}

void Team::swap_offensive_stages(Team & lhs, Team & rhs) {
	ActivePokemon::swap_offensive_stages(lhs.active_pokemon, rhs.active_pokemon);
}

void Team::swap_stat_boosts(Team & lhs, Team & rhs) {
	ActivePokemon::swap_stat_boosts(lhs.active_pokemon, rhs.active_pokemon);
}

void Team::increment_stockpile() {
	bool const increased = pokemon().increment_stockpile();
	if (increased)
		pokemon().stat_boost_defensive(1);
}

int Team::release_stockpile() {
	int const stages = pokemon().release_stockpile();
	pokemon().stat_boost_defensive(-stages);
	return stages;
}

bool Team::switch_decision_required() const {
	return pokemon().switch_decision_required();
}

bool Team::trapped() const {
	return !pokemon().trapped();
}

bool Team::tormented() const {
	return pokemon().tormented();
}

void Team::torment() {
	pokemon().torment();
}

void Team::taunt() {
	pokemon().taunt();
}

bool Team::is_taunted() const {
	return pokemon().is_taunted();
}

void Team::increment_taunt() {
	pokemon().increment_taunt();
}

Rational Team::toxic_ratio() const {
	return pokemon().toxic_ratio();
}

void Team::increment_toxic() {
	pokemon().increment_toxic();
}

void Team::u_turn() {
	pokemon().u_turn();
}

void Team::use_uproar() {
	pokemon().use_uproar();
}

bool Team::vanish_doubles_power(Moves const move_name) const {
	return pokemon().vanish_doubles_power(move_name);
}

void Team::activate_water_sport() {
	pokemon().activate_water_sport();
}

bool Team::decrement_yawn() {
	return pokemon().decrement_yawn();
}

void Team::hit_with_yawn() {
	pokemon().hit_with_yawn();
}

void Team::use_bide(Pokemon & target) {
	pokemon().use_bide(target);
}

void Team::substitute() {
	pokemon().use_substitute();
}

bool Team::is_locked_in_to_bide() const {
	return pokemon().is_locked_in_to_bide();
}

void Team::lower_pp(Ability const & target) {
	if (!is_locked_in_to_bide())
		pokemon().move().pp.decrement(target);
}

void Team::activate_perish_song() {
	pokemon().activate_perish_song();
}

void Team::increase_sleep_counter() {
	pokemon().increase_sleep_counter();
}

bool Team::can_be_phazed () const {
	return !ingrained() and !pokemon().ability().blocks_phazing() and all_pokemon().size() > 1;
}

unsigned Team::damaged() const {
	return pokemon().damaged();
}

void Team::do_damage(unsigned const damage) {
	pokemon().do_damage(damage);
}

bool Team::has_switched() const {
	return moved() and pokemon().move().is_switch();
}

void Team::update_chance_to_hit(Team const & target, Weather const & weather, bool const target_moved) {
	pokemon().update_chance_to_hit(target.pokemon(), weather, target_moved);
}

ChanceToHit::value_type Team::chance_to_hit() const {
	return pokemon().chance_to_hit();
}

ChanceToHit::value_type Team::chance_to_miss() const {
	return pokemon().chance_to_miss();
}

bool Team::can_miss() const {
	return pokemon().can_miss();
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

Team::hash_type Team::hash () const {
	hash_type current_hash = m_all_pokemon.hash();
	current_hash *= active_pokemon.max_hash();
	current_hash += active_pokemon.hash();
	current_hash *= entry_hazards.max_hash();
	current_hash += entry_hazards.hash();
	current_hash *= wish.max_hash();
	current_hash += wish.hash();
	current_hash *= screens.max_hash();
	current_hash += screens.hash();
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
	return
			lhs.m_all_pokemon == rhs.m_all_pokemon and
			lhs.active_pokemon == rhs.active_pokemon and
			lhs.screens == rhs.screens and
			lhs.wish == rhs.wish and
			lhs.entry_hazards == rhs.entry_hazards and
			lhs.me == rhs.me;
}

bool operator!= (Team const & lhs, Team const & rhs) {
	return !(lhs == rhs);
}

std::string Team::to_string() const {
	std::string output = me ? "AI" : "Foe";
	output += "'s team:\n";
	all_pokemon().for_each([& output](Pokemon const & member) {
		output += member.to_string();
		double const d_per_cent_hp = 100.0 * member.hp.stat / member.hp.max;
		std::string const per_cent_hp = boost::lexical_cast <std::string> (boost::format ("%.1f") % d_per_cent_hp);
		output += " (" + per_cent_hp + "% HP)";
		output += " @ " + member.item().to_string ();
		output += " ** " + member.get_nickname() + '\n';
		if (member.ability().is_set())
			output += "\tAbility: " + member.ability().to_string () + '\n';
		if (!member.status().is_clear())
			output += "\tStatus: " + member.status().to_string () + '\n';
		member.move.for_each_regular_move([& output](Move const & move) {
			output += "\t- " + move.to_string() + "\n";
		});
	});
	return output;
}

}	// namespace technicalmachine
