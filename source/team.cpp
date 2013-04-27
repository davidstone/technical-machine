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
	me(false)
	{
}

Team::Team(std::mt19937 & random_engine, std::string const & team_file_name) :
	me(true)
	{
	boost::filesystem::path team_file = team_file_name;
	std::vector <boost::filesystem::path> const files = open_directory_and_add_files (team_file);
	assert(files.size() > 0);
	std::uniform_int_distribution <size_t> distribution (0, files.size () - 1);
	team_file = files [distribution (random_engine)];
	load(team_file.string());
	for (auto & member : all_pokemon()) {
		member.set_hidden_power_type();
		member.calculate_initial_hp();
	}
}

ActivePokemon const & Team::pokemon() const {
	return active_pokemon;
}

ActivePokemon & Team::pokemon() {
	return active_pokemon;
}

Pokemon const & Team::pokemon(PokemonCollection::index_type const index) const {
	return all_pokemon()(index);
}
Pokemon & Team::pokemon(PokemonCollection::index_type const index) {
	return all_pokemon()(index);
}

Pokemon const & Team::replacement() const {
	return all_pokemon().at_replacement();
}
Pokemon & Team::replacement() {
	return all_pokemon().at_replacement();
}

void Team::remove_pokemon () {
	all_pokemon().remove_active();
}

PokemonCollection const & Team::all_pokemon() const {
	return active_pokemon.all_pokemon();
}

PokemonCollection & Team::all_pokemon() {
	return active_pokemon.all_pokemon();
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

void Team::reset_between_turns() {
	pokemon().reset_between_turns();
	for (auto & member : all_pokemon()) {
		member.move.reset_index();
	}
	all_pokemon().initialize_replacement();
}

void Team::clear_field() {
	pokemon().clear_field();
	entry_hazards.clear();
}

void Team::move(bool const value) {
	pokemon().set_moved(value);
}

void Team::update_chance_to_hit(Team const & target, Weather const & weather, bool const target_moved) {
	pokemon().update_chance_to_hit(target.pokemon(), weather, target_moved);
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
	hash_type current_hash = active_pokemon.hash();
	current_hash *= entry_hazards.max_hash();
	current_hash += entry_hazards.hash();
	current_hash *= wish.max_hash();
	current_hash += wish.hash();
	current_hash *= screens.max_hash();
	current_hash += screens.hash();
	return current_hash;
}

void Team::load(std::string const & name) {
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
		po::load_team(*this, name);
	else if (extension == ".sbt")
		pl::load_team(*this, name);
	else
		throw InvalidTeamFileFormat(name);
}

bool operator== (Team const & lhs, Team const & rhs) {
	return
			lhs.active_pokemon == rhs.active_pokemon and
			lhs.screens == rhs.screens and
			lhs.wish == rhs.wish and
			lhs.entry_hazards == rhs.entry_hazards and
			lhs.me == rhs.me;
}

bool operator!= (Team const & lhs, Team const & rhs) {
	return !(lhs == rhs);
}

std::string to_string(Team const & team, bool const include_owner) {
	std::string output;
	if (include_owner) {
		output += team.who() + "'s team:\n";
	}
	for (auto const & member : team.all_pokemon()) {
		output += member.to_string();
		double const d_per_cent_hp = 100.0 * member.current_hp();
		std::string const per_cent_hp = boost::lexical_cast <std::string> (boost::format ("%.1f") % d_per_cent_hp);
		output += " (" + per_cent_hp + "% HP)";
		output += " @ " + member.item().to_string ();
		output += include_owner ? (" ** " + member.get_nickname() + '\n') : "\n";
		if (member.ability().is_set()) {
			output += "\tAbility: " + member.ability().to_string() + '\n';
		}
		if (!member.status().is_clear()) {
			output += "\tStatus: " + member.status().to_string() + '\n';
		}
		output += "\tNature: " + member.nature().to_string() + '\n';
		output += "\t";
		auto const add_stat = [&](Stat const & stat, std::string const & stat_name) {
			if (stat_name != "HP") {
				output += " / ";
			}
			output += std::to_string(stat.ev.value()) + " " + stat_name;
		};
		static std::pair<Stat::Stats, std::string> const stats [] = {
			{ Stat::HP, "HP" },
			{ Stat::ATK, "Atk" },
			{ Stat::DEF, "Def" },
			{ Stat::SPA, "SpA" },
			{ Stat::SPD, "SpD" },
			{ Stat::SPE, "Spe" },
		};
		for (auto const stat : stats) {
			add_stat(member.stat(stat.first), stat.second);
		}
		member.move.for_each_regular_move([& output](Move const & move) {
			output += "\n\t- " + move.to_string();
		});
	}
	return output;
}

std::string Team::who() const {
	return is_me() ? "AI" : "Foe";
}

}	// namespace technicalmachine
