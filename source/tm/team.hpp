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

#include <tm/entry_hazards.hpp>
#include <tm/operators.hpp>
#include <tm/screens.hpp>
#include <tm/wish.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <containers/string.hpp>

#include <filesystem>
#include <random>
#include <string_view>
#include <utility>

namespace technicalmachine {
struct Weather;

struct Team {
	explicit Team(TeamSize const initial_size, bool team_is_me = false) :
		m_all_pokemon(initial_size),
		me(team_is_me)
	{
	}

	auto const & all_pokemon() const {
		return m_all_pokemon;
	}
	auto & all_pokemon() {
		return m_all_pokemon;
	}

	auto pokemon() const {
		return ActivePokemon(all_pokemon()(), m_flags);
	}
	auto pokemon() {
		return MutableActivePokemon(all_pokemon()(), m_flags);
	}
	Pokemon const & pokemon(containers::index_type<PokemonCollection> const index) const {
		return all_pokemon()(index);
	}
	Pokemon & pokemon(containers::index_type<PokemonCollection> const index) {
		return all_pokemon()(index);
	}

	template<typename... Args>
	Pokemon & add_pokemon(Args&&... args) {
		return all_pokemon().add(std::forward<Args>(args)...);
	}

	TeamSize number_of_seen_pokemon() const {
		return containers::size(all_pokemon());
	}
	TeamSize size() const {
		return all_pokemon().real_size();
	}
	
	bool is_me() const {
		return me;
	}

	std::string_view who() const {
		return is_me() ? "AI" : "Foe";
	}

	auto reset_end_of_turn() -> void {
		m_flags.reset_end_of_turn();
	}
	auto reset_switch() -> void {
		m_flags.reset_switch();
		auto p = pokemon();
		get_status(p).handle_switch(get_ability(p));
	}
	void clear_field() {
		pokemon().clear_field();
		entry_hazards = EntryHazards{};
	}


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
