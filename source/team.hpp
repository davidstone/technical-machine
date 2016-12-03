// Copyright (C) 2016 David Stone
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

#pragma once

#include "entry_hazards.hpp"
#include "operators.hpp"
#include "screens.hpp"
#include "wish.hpp"

#include "pokemon/active_pokemon.hpp"
#include "pokemon/active_pokemon_flags.hpp"
#include "pokemon/collection.hpp"
#include "pokemon/species_forward.hpp"

#include <boost/filesystem/path.hpp>

#include <cstdint>
#include <random>
#include <string>

namespace technicalmachine {
struct Ability;
struct Pokemon;
struct Weather;

struct Team {
	explicit Team(TeamSize initial_size, bool is_me = false);
	Team(std::mt19937 & random_engine, boost::filesystem::path const & team_file);
	
	auto pokemon() const {
		return ActivePokemon(all_pokemon()(), m_flags);
	}
	auto pokemon() {
		return MutableActivePokemon(all_pokemon()(), m_flags);
	}
	Pokemon const & pokemon(containers::index_type<PokemonCollection> index) const;
	Pokemon & pokemon(containers::index_type<PokemonCollection> index);
	auto const & replacement() const {
		return all_pokemon()(all_pokemon().replacement());
	}
	auto & replacement() {
		return all_pokemon()(all_pokemon().replacement());
	}

	template<typename... Args>
	void add_pokemon(Args&&... args) {
		all_pokemon().add(std::forward<Args>(args)...);
	}
	PokemonCollection const & all_pokemon() const;
	PokemonCollection & all_pokemon();
	
	TeamSize number_of_seen_pokemon() const;
	TeamSize size() const;
	
	bool is_me() const;
	std::string who() const;

	// Not for variables that give a message at the end of the turn, this is
	// just for some book-keeping variables.
	void reset_between_turns();

	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;
	void clear_field();
	void move(bool value = true);

	friend bool operator== (Team const & lhs, Team const & rhs);

private:
	friend struct Evaluate;
	void load(boost::filesystem::path const & team_file);
	
	PokemonCollection m_all_pokemon;
	ActivePokemonFlags m_flags;
public:
	Screens screens;
	Wish wish;
	EntryHazards entry_hazards;
private:
	bool me;
};

std::string to_string(Team const & team, bool include_owner = true);

}	// namespace technicalmachine
