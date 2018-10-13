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

#pragma once

#include "entry_hazards.hpp"
#include "operators.hpp"
#include "screens.hpp"
#include "wish.hpp"

#include "pokemon/active_pokemon.hpp"
#include "pokemon/active_pokemon_flags.hpp"
#include "pokemon/collection.hpp"
#include "pokemon/species_forward.hpp"

#include <containers/string.hpp>

#include <cstdint>
#include <filesystem>
#include <random>
#include <string>

namespace technicalmachine {
struct Weather;

struct Team {
	explicit Team(TeamSize initial_size, bool is_me = false);
	
	auto pokemon() const {
		return ActivePokemon(all_pokemon()(), m_flags);
	}
	auto pokemon() {
		return MutableActivePokemon(all_pokemon()(), m_flags);
	}
	Pokemon const & pokemon(containers::index_type<PokemonCollection> index) const;
	Pokemon & pokemon(containers::index_type<PokemonCollection> index);

	template<typename... Args>
	Pokemon & add_pokemon(Args&&... args) {
		return all_pokemon().add(std::forward<Args>(args)...);
	}
	PokemonCollection const & all_pokemon() const;
	PokemonCollection & all_pokemon();
	
	TeamSize number_of_seen_pokemon() const;
	TeamSize size() const;
	
	bool is_me() const;
	std::string_view who() const;

	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;
	void clear_field();
	void move(bool value = true);

	friend auto operator==(Team const & lhs, Team const & rhs) {
		return
			lhs.all_pokemon() == rhs.all_pokemon() and
			lhs.m_flags == rhs.m_flags and
			lhs.screens == rhs.screens and
			lhs.wish == rhs.wish and
			lhs.entry_hazards == rhs.entry_hazards and
			lhs.me == rhs.me;
	}

private:
	friend struct Evaluate;
	
	PokemonCollection m_all_pokemon;
	ActivePokemonFlags m_flags;
public:
	Screens screens;
	Wish wish;
	EntryHazards entry_hazards;
private:
	bool me;
};

Team load_team_from_file(std::mt19937 & random_engine, std::filesystem::path const & path);

containers::string to_string(Team const & team, bool include_owner = true);

}	// namespace technicalmachine
