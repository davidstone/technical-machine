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

#include <tm/apply_entry_hazards.hpp>
#include <tm/entry_hazards.hpp>
#include <tm/operators.hpp>
#include <tm/screens.hpp>
#include <tm/wish.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/compress.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <operators/forward.hpp>

#include <string_view>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Weather;

template<Generation generation>
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
		return ActivePokemon<generation>(all_pokemon()(), m_flags);
	}
	auto pokemon() {
		return MutableActivePokemon<generation>(all_pokemon()(), m_flags);
	}
	Pokemon<generation> const & pokemon(TeamIndex const index) const {
		return all_pokemon()(index);
	}
	Pokemon<generation> & pokemon(TeamIndex const index) {
		return all_pokemon()(index);
	}

	Pokemon<generation> & add_pokemon(Pokemon<generation> pokemon) {
		return all_pokemon().add(std::move(pokemon));
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
		m_entry_hazards = {};
	}

	auto switch_pokemon(MutableActivePokemon<generation> other, Weather & weather, TeamIndex const replacement) -> void {
		auto original_pokemon = pokemon();
		original_pokemon.switch_out();
		if constexpr (generation == Generation::one) {
			shatter_screens();
		}

		if (original_pokemon.hp() != 0_bi) {
			all_pokemon().set_index(replacement);
		} else {
			all_pokemon().remove_active(replacement);
			// If the last Pokemon is fainted; there is nothing left to do.
			if (containers::is_empty(all_pokemon())) {
				return;
			}
		}

		auto const replacement_pokemon = pokemon();
		replacement_pokemon.switch_in(weather);
		apply(m_entry_hazards, replacement_pokemon, weather);
		if (replacement_pokemon.hp() != 0_bi) {
			activate_ability_on_switch(replacement_pokemon, other, weather);
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
	auto activate_light_screen(Weather const weather) & {
		m_screens.activate_light_screen(extends_light_screen(pokemon().item(weather)));
	}
	auto activate_reflect(Weather const weather) & {
		m_screens.activate_reflect(extends_reflect(pokemon().item(weather)));
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

	auto activate_wish() & -> void {
		m_wish.activate();
	}
	auto decrement_wish(Weather const weather) & -> void {
		m_wish.decrement(pokemon(), weather);
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

	friend auto compress(Team const & team) {
		auto const compressed_pokemon = compress(team.m_all_pokemon);
		auto const compressed_flags = compress(team.m_flags);
		if constexpr (generation == Generation::one) {
			static_assert(bounded::tuple_size<decltype(compressed_pokemon)> == 2_bi);
			static_assert(bounded::tuple_size<decltype(compressed_flags)> == 1_bi);
			return bounded::tuple(
				compress_combine(
					compressed_pokemon[0_bi],
					team.m_entry_hazards,
					team.m_wish,
					team.me
				),
				compressed_pokemon[1_bi],
				compress_combine(
					compressed_flags[0_bi],
					team.m_screens
				)
			);
		} else {
			static_assert(bounded::tuple_size<decltype(compressed_pokemon)> == 3_bi);
			static_assert(bounded::tuple_size<decltype(compressed_flags)> == 2_bi);
			return bounded::tuple(
				compress_combine(
					compressed_pokemon[0_bi],
					team.m_screens,
					team.m_entry_hazards,
					team.m_wish,
					team.me
				),
				compress_combine(
					compressed_pokemon[1_bi],
					compressed_flags[0_bi]
				),
				compressed_pokemon[2_bi],
				compressed_flags[1_bi]
			);
		}
	}

private:
	PokemonCollection<generation> m_all_pokemon;
	ActivePokemonFlags<generation> m_flags;
	Screens<generation> m_screens;
	[[no_unique_address]] Wish<generation> m_wish;
	[[no_unique_address]] EntryHazards<generation> m_entry_hazards;
	bool me;
};

template<Generation generation>
constexpr auto team_has_status(Team<generation> const & target, Statuses const status) {
	return containers::any(target.all_pokemon(), [=](Pokemon<generation> const & pokemon) {
				return pokemon.status().name() == status;
			});
	}

} // namespace technicalmachine
