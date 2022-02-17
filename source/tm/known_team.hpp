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
#include <tm/pokemon/known_pokemon_collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/seen_pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/any_team.hpp>
#include <tm/compress.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <operators/forward.hpp>

#include <string_view>

namespace technicalmachine {
struct Weather;

template<Generation generation>
struct KnownTeam {
	constexpr explicit KnownTeam(containers::initializer_range<KnownPokemonCollection<generation>> auto && source):
		m_all_pokemon(OPERATORS_FORWARD(source))
	{
	}
	template<std::size_t source_size>
	constexpr explicit KnownTeam(containers::c_array<KnownPokemon<generation>, source_size> && source):
		m_all_pokemon(std::move(source))
	{
	}
	explicit KnownTeam(Team<generation> const & other):
		m_all_pokemon(containers::transform(other.all_pokemon(), [](Pokemon<generation> pokemon) {
			return KnownPokemon<generation>(pokemon);
		})),
		m_flags(other.flags()),
		m_screens(other.screens()),
		m_wish(other.wish()),
		m_entry_hazards(other.entry_hazards())
	{
	}

	constexpr auto const & all_pokemon() const {
		return m_all_pokemon;
	}
	constexpr auto & all_pokemon() {
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

	constexpr auto pokemon() const {
		return AnyActivePokemon<KnownPokemon<generation>>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon() {
		return AnyMutableActivePokemon<KnownPokemon<generation>>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon(TeamIndex const index) const -> KnownPokemon<generation> const & {
		return all_pokemon()(index);
	}
	constexpr auto pokemon(TeamIndex const index) -> KnownPokemon<generation> & {
		return all_pokemon()(index);
	}

	constexpr auto size() const -> TeamSize {
		return containers::size(all_pokemon());
	}

	constexpr auto reset_start_of_turn() -> void {
		m_flags.reset_start_of_turn();
	}
	constexpr auto clear_field() -> void {
		pokemon().clear_field();
		m_entry_hazards = {};
	}

	constexpr auto switch_pokemon(AnyMutableActivePokemon<SeenPokemon<generation>> other, Weather & weather, TeamIndex const replacement) -> void {
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

	constexpr auto decrement_screens() & -> void {
		m_screens.decrement();
	}
	constexpr auto light_screen() const {
		return m_screens.light_screen();
	}
	constexpr auto reflect() const {
		return m_screens.reflect();
	}
	constexpr auto lucky_chant() const {
		return m_screens.lucky_chant();
	}
	constexpr auto mist() const {
		return m_screens.mist();
	}
	constexpr auto safeguard() const {
		return m_screens.safeguard();
	}
	constexpr auto tailwind() const {
		return m_screens.tailwind();
	}
	constexpr auto activate_light_screen(Weather const weather) & {
		m_screens.activate_light_screen(extends_light_screen(pokemon().item(weather)));
	}
	constexpr auto activate_reflect(Weather const weather) & {
		m_screens.activate_reflect(extends_reflect(pokemon().item(weather)));
	}
	constexpr auto activate_lucky_chant() & {
		m_screens.activate_lucky_chant();
	}
	constexpr auto activate_mist() & {
		m_screens.activate_mist();
	}
	constexpr auto activate_safeguard() & {
		m_screens.activate_safeguard();
	}
	constexpr auto activate_tailwind() & {
		m_screens.activate_tailwind();
	}
	constexpr auto shatter_screens() & -> void {
		m_screens.shatter();
	}

	constexpr auto activate_wish() & -> void {
		m_wish.activate();
	}
	constexpr auto decrement_wish(Weather const weather) & -> void {
		m_wish.decrement(pokemon(), weather);
	}

	constexpr auto entry_hazards() const {
		return m_entry_hazards;
	}
	constexpr auto add_spikes() & {
		m_entry_hazards.add_spikes();
	}
	constexpr auto add_stealth_rock() & {
		m_entry_hazards.add_stealth_rock();
	}
	constexpr auto add_toxic_spikes() & {
		m_entry_hazards.add_toxic_spikes();
	}

	friend auto operator==(KnownTeam const &, KnownTeam const &) -> bool = default;

private:
	KnownPokemonCollection<generation> m_all_pokemon;
	ActivePokemonFlags<generation> m_flags;
	Screens<generation> m_screens;
	[[no_unique_address]] Wish<generation> m_wish;
	[[no_unique_address]] EntryHazards<generation> m_entry_hazards;
};

template<Generation generation>
constexpr auto team_has_status(KnownTeam<generation> const & target, Statuses const status) {
	return containers::any(target.all_pokemon(), [=](KnownPokemon<generation> const & pokemon) {
		return pokemon.status().name() == status;
	});
}

template<Generation generation>
auto switch_decision_required(KnownTeam<generation> const & team) {
	if (team.size() == 1_bi) {
		return false;
	}
	auto const pokemon = team.pokemon();
	return pokemon.hp().current() == 0_bi or pokemon.last_used_move().switch_decision_required();
}

} // namespace technicalmachine
