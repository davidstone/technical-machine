// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/apply_entry_hazards.hpp>
#include <tm/entry_hazards.hpp>
#include <tm/operators.hpp>
#include <tm/screens.hpp>
#include <tm/wish.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/known_pokemon.hpp>
#include <tm/pokemon/seen_pokemon_collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <operators/forward.hpp>

#include <string_view>

namespace technicalmachine {
struct Weather;

template<Generation generation>
struct SeenTeam {
	explicit SeenTeam(TeamSize const initial_size) :
		m_all_pokemon(initial_size)
	{
	}

	auto const & all_pokemon() const {
		return m_all_pokemon;
	}
	auto & all_pokemon() {
		return m_all_pokemon;
	}
	auto flags() const {
		return m_flags;
	}
	auto screens() const {
		return m_screens;
	}
	auto wish() const {
		return m_wish;
	}

	auto pokemon() const -> AnyActivePokemon<SeenPokemon<generation>> {
		return AnyActivePokemon(all_pokemon()(), m_flags);
	}
	auto pokemon() -> AnyMutableActivePokemon<SeenPokemon<generation>> {
		return AnyMutableActivePokemon(all_pokemon()(), m_flags);
	}
	auto pokemon(TeamIndex const index) const -> SeenPokemon<generation> const & {
		return all_pokemon()(index);
	}
	auto pokemon(TeamIndex const index) -> SeenPokemon<generation> & {
		return all_pokemon()(index);
	}

	auto add_pokemon(SeenPokemon<generation> pokemon) -> SeenPokemon<generation> & {
		return all_pokemon().add(std::move(pokemon));
	}

	auto number_of_seen_pokemon() const -> TeamSize {
		return containers::size(all_pokemon());
	}
	auto size() const -> TeamSize {
		return all_pokemon().real_size();
	}
	
	auto reset_start_of_turn() -> void {
		m_flags.reset_start_of_turn();
	}
	void clear_field() {
		pokemon().clear_field();
		m_entry_hazards = {};
	}

	auto switch_pokemon(AnyMutableActivePokemon<KnownPokemon<generation>> other, Weather & weather, TeamIndex const replacement) -> void {
		auto original_pokemon = pokemon();
		original_pokemon.switch_out();
		if constexpr (generation == Generation::one) {
			shatter_screens();
		}

		if (original_pokemon.hp().current() != 0_bi) {
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
		if (replacement_pokemon.hp().current() != 0_bi) {
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

	friend auto operator==(SeenTeam const &, SeenTeam const &) -> bool = default;

private:
	SeenPokemonCollection<generation> m_all_pokemon;
	ActivePokemonFlags<generation> m_flags;
	Screens<generation> m_screens;
	[[no_unique_address]] Wish<generation> m_wish;
	[[no_unique_address]] EntryHazards<generation> m_entry_hazards;
};

template<Generation generation>
constexpr auto team_has_status(SeenTeam<generation> const & target, Statuses const status) {
	return containers::any(target.all_pokemon(), [=](SeenPokemon<generation> const & pokemon) {
		return pokemon.status().name() == status;
	});
}

template<Generation generation>
auto switch_decision_required(SeenTeam<generation> const & team) {
	if (team.size() == 1_bi) {
		return false;
	}
	auto const pokemon = team.pokemon();
	return pokemon.hp().current() == 0_bi or pokemon.last_used_move().switch_decision_required();
}

} // namespace technicalmachine
