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
#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/screens.hpp>
#include <tm/wish.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/detail/forward.hpp>

#include <string_view>

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

	Pokemon & add_pokemon(auto && ... args) {
		return all_pokemon().add(BOUNDED_FORWARD(args)...);
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
	void clear_field() {
		pokemon().clear_field();
		entry_hazards = EntryHazards{};
	}

	auto switch_pokemon(Generation const generation, Team & other, Weather & weather, TeamIndex const replacement) -> void {
		m_flags.reset_switch();
		Pokemon & original_pokemon = pokemon();
		switch_out(original_pokemon);

		if (get_hp(original_pokemon) != 0_bi) {
			all_pokemon().set_index(replacement);
		} else {
			all_pokemon().remove_active(replacement);
			// If the last Pokemon is fainted; there is nothing left to do.
			if (empty(all_pokemon())) {
				return;
			}
		}

		auto const replacement_pokemon = pokemon();
		switch_in(replacement_pokemon);
		apply(generation, entry_hazards, replacement_pokemon, weather);
		if (get_hp(replacement_pokemon) != 0_bi) {
			activate_ability_on_switch(replacement_pokemon, other.pokemon(), weather);
		}
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

}	// namespace technicalmachine
