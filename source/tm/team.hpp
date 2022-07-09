// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/apply_entry_hazards.hpp>
#include <tm/any_team.hpp>
#include <tm/compress.hpp>
#include <tm/entry_hazards.hpp>
#include <tm/known_team.hpp>
#include <tm/operators.hpp>
#include <tm/screens.hpp>
#include <tm/seen_team.hpp>
#include <tm/wish.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <operators/forward.hpp>

#include <string_view>

namespace technicalmachine {
struct Weather;

template<Generation generation>
struct Team {
	explicit Team(PokemonContainer<generation> all_pokemon_, bool team_is_me = false) :
		m_all_pokemon(all_pokemon_),
		me(team_is_me)
	{
	}
	template<std::size_t size>
	explicit Team(containers::c_array<Pokemon<generation>, size> && all_pokemon_, bool team_is_me = false):
		Team(PokemonContainer<generation>(std::move(all_pokemon_)), team_is_me)
	{
	}

	explicit Team(KnownTeam<generation> const & other):
		m_all_pokemon(other.all_pokemon()),
		m_flags(other.flags()),
		m_screens(other.screens()),
		m_wish(other.wish()),
		m_entry_hazards(other.entry_hazards()),
		me(true)
	{
	}
	explicit Team(SeenTeam<generation> const & other):
		m_all_pokemon(other.all_pokemon()),
		m_flags(other.flags()),
		m_screens(other.screens()),
		m_wish(other.wish()),
		m_entry_hazards(other.entry_hazards()),
		me(false)
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

	TeamSize size() const {
		return containers::size(all_pokemon());
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

constexpr auto team_has_status(any_team auto const & target, StatusName const status) {
	return containers::any(target.all_pokemon(), [=](auto const & pokemon) {
		return pokemon.status().name() == status;
	});
}

auto switch_decision_required(any_team auto const & team) {
	if (team.size() == 1_bi) {
		return false;
	}
	auto const pokemon = team.pokemon();
	return pokemon.hp().current() == 0_bi or pokemon.last_used_move().switch_decision_required();
}

} // namespace technicalmachine
