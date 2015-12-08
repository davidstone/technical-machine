// Copyright (C) 2015 David Stone
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

}	// namespace

Team::Team(TeamSize const initial_size, bool team_is_me) :
	m_all_pokemon(initial_size),
	me(team_is_me)
	{
}

Team::Team(std::mt19937 & random_engine, boost::filesystem::path const & team_file) :
	m_all_pokemon(6_bi), // This size gets corrected later
	me(true)
	{
	std::vector <boost::filesystem::path> const files = open_directory_and_add_files (team_file);
	assert(files.size() > 0);
	std::uniform_int_distribution <size_t> distribution (0, files.size () - 1);
	load(files[distribution(random_engine)]);
}

Pokemon const & Team::pokemon(containers::index_type<PokemonCollection> const index) const {
	return all_pokemon()(index);
}
Pokemon & Team::pokemon(containers::index_type<PokemonCollection> const index) {
	return all_pokemon()(index);
}

Pokemon const & Team::replacement() const {
	return all_pokemon().at_replacement();
}
Pokemon & Team::replacement() {
	return all_pokemon().at_replacement();
}

PokemonCollection const & Team::all_pokemon() const {
	return m_all_pokemon;
}

PokemonCollection & Team::all_pokemon() {
	return m_all_pokemon;
}

TeamSize Team::number_of_seen_pokemon() const {
	return containers::size(all_pokemon());
}
TeamSize Team::size() const {
	return all_pokemon().real_size();
}

bool Team::is_me() const {
	return me;
}

void Team::reset_between_turns() {
	m_flags.reset_between_turns();
	for (auto & member : all_pokemon()) {
		all_moves(member).reset_index();
	}
	all_pokemon().initialize_replacement();
}

auto Team::reset_end_of_turn() -> void {
	m_flags.reset_end_of_turn();
}

auto Team::reset_switch() -> void {
	m_flags.reset_switch();
}

void Team::clear_field() {
	pokemon().clear_field();
	entry_hazards = EntryHazards{};
}

void Team::move(bool const value) {
	pokemon().set_moved(value);
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

}	// namespace

void Team::load(boost::filesystem::path const & team_file) {
	// I do no error checking because I assume my team files will always be in
	// the proper format. This must be changed if I ever allow arbitary teams
	// to be used.

	auto const extension = boost::filesystem::extension(team_file);
	if (extension == ".tp") {
		po::load_team(*this, team_file);
	} else if (extension == ".sbt") {
		pl::load_team(*this, team_file);
	} else {
		throw InvalidTeamFileFormat(team_file);
	}
}

bool operator== (Team const & lhs, Team const & rhs) {
	return
		lhs.all_pokemon() == rhs.all_pokemon() and
		lhs.m_flags == rhs.m_flags and
		lhs.screens == rhs.screens and
		lhs.wish == rhs.wish and
		lhs.entry_hazards == rhs.entry_hazards and
		lhs.me == rhs.me;
}

std::string to_string(Team const & team, bool const include_owner) {
	std::string output;
	if (include_owner) {
		output += team.who() + "'s team:\n";
	}
	for (auto const & member : team.all_pokemon()) {
		output += to_string(member, include_owner) + '\n';
	}
	return output;
}

std::string Team::who() const {
	return is_me() ? "AI" : "Foe";
}

}	// namespace technicalmachine
