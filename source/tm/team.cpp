// Copyright (C) 2018 David Stone
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

#include <tm/team.hpp>

#include <tm/ability.hpp>
#include <tm/damage.hpp>
#include <tm/files_in_directory.hpp>
#include <tm/item.hpp>
#include <tm/status.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/clients/invalid_team_file_format.hpp>

#include <tm/clients/pokemon_lab/read_team_file.hpp>
#include <tm/clients/pokemon_online/read_team_file.hpp>

#include <cstdint>
#include <filesystem>
#include <string>

namespace technicalmachine {

Team::Team(TeamSize const initial_size, bool team_is_me) :
	m_all_pokemon(initial_size),
	me(team_is_me)
{
}

Pokemon const & Team::pokemon(containers::index_type<PokemonCollection> const index) const {
	return all_pokemon()(index);
}
Pokemon & Team::pokemon(containers::index_type<PokemonCollection> const index) {
	return all_pokemon()(index);
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

auto Team::reset_end_of_turn() -> void {
	m_flags.reset_end_of_turn();
}

auto Team::reset_switch() -> void {
	m_flags.reset_switch();
	auto p = pokemon();
	get_status(p).handle_switch(get_ability(p));
}

void Team::clear_field() {
	pokemon().clear_field();
	entry_hazards = EntryHazards{};
}

Team load_team_from_file(std::mt19937 & random_engine, std::filesystem::path const & path) {
	auto const files = recursive_files_in_path(path);
	if (empty(files)) {
		throw std::runtime_error(path.string() + " does not contain any team files.");
	}
	auto distribution = std::uniform_int_distribution<std::intmax_t>(0, static_cast<std::intmax_t>(size(files) - 1_bi));
	auto const file = files[containers::index_type<decltype(files)>(distribution(random_engine))];

	auto const extension = file.extension();
	if (extension == ".tp") {
		return po::load_team(file);
	} else if (extension == ".sbt") {
		return pl::load_team(file);
	} else {
		throw InvalidTeamFileFormat(file);
	}
}

containers::string to_string(Team const & team, bool const include_owner) {
	containers::string output;
	if (include_owner) {
		output = containers::concatenate<containers::string>(std::move(output), team.who(), std::string_view("'s team:\n"));
	}
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::single_element_range('\n')
		);
	}
	return output;
}

std::string_view Team::who() const {
	return is_me() ? "AI" : "Foe";
}

}	// namespace technicalmachine
