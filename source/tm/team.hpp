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
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <operators/forward.hpp>

#include <string_view>

namespace technicalmachine {
enum class Generation : std::uint8_t;
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
		return all_pokemon().add(OPERATORS_FORWARD(args)...);
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

	auto reset_start_of_turn() -> void {
		m_flags.reset_start_of_turn();
	}
	void clear_field() {
		pokemon().clear_field();
		m_entry_hazards = EntryHazards{};
	}

	auto switch_pokemon(Generation const generation, MutableActivePokemon other, Weather & weather, TeamIndex const replacement) -> void {
		auto original_pokemon = pokemon();
		original_pokemon.switch_out();
		if (generation == Generation::one) {
			shatter_screens();
		}

		if (original_pokemon.hp() != 0_bi) {
			all_pokemon().set_index(replacement);
		} else {
			all_pokemon().remove_active(replacement);
			// If the last Pokemon is fainted; there is nothing left to do.
			if (empty(all_pokemon())) {
				return;
			}
		}

		auto const replacement_pokemon = pokemon();
		replacement_pokemon.switch_in(generation, weather);
		apply(generation, m_entry_hazards, replacement_pokemon, weather);
		if (replacement_pokemon.hp() != 0_bi) {
			activate_ability_on_switch(generation, replacement_pokemon, other, weather);
		}
	}

	auto decrement_screens() & -> void {
		m_screens.decrement();
	}
	auto light_screen() const {
		return m_screens.light_screen();
	}
	auto reflect() const {
		return m_screens.reflect();
	}
	auto lucky_chant() const {
		return m_screens.lucky_chant();
	}
	auto mist() const {
		return m_screens.mist();
	}
	auto safeguard() const {
		return m_screens.safeguard();
	}
	auto tailwind() const {
		return m_screens.tailwind();
	}
	auto activate_light_screen(Generation const generation, Weather const weather) & {
		m_screens.activate_light_screen(extends_light_screen(pokemon().item(generation, weather)));
	}
	auto activate_reflect(Generation const generation, Weather const weather) & {
		m_screens.activate_reflect(extends_reflect(pokemon().item(generation, weather)));
	}
	auto activate_lucky_chant() & {
		m_screens.activate_lucky_chant();
	}
	auto activate_mist() & {
		m_screens.activate_mist();
	}
	auto activate_safeguard() & {
		m_screens.activate_safeguard();
	}
	auto activate_tailwind() & {
		m_screens.activate_tailwind();
	}
	auto shatter_screens() & -> void {
		m_screens.shatter();
	}

	auto wish_is_active() const -> bool {
		return m_wish.is_active();
	}
	auto activate_wish() & -> void {
		m_wish.activate();
	}
	auto decrement_wish(Generation const generation, Weather const weather) & -> void {
		m_wish.decrement(generation, pokemon(), weather);
	}

	auto entry_hazards() const {
		return m_entry_hazards;
	}
	auto add_spikes() & {
		m_entry_hazards.add_spikes();
	}
	auto add_stealth_rock() & {
		m_entry_hazards.add_stealth_rock();
	}
	auto add_toxic_spikes() & {
		m_entry_hazards.add_toxic_spikes();
	}

	friend auto operator==(Team const &, Team const &) -> bool = default;

private:
	PokemonCollection m_all_pokemon;
	ActivePokemonFlags m_flags;
	Screens m_screens;
	Wish m_wish;
	EntryHazards m_entry_hazards;
	bool me;
};

}	// namespace technicalmachine
