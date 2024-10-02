// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.team;

import tm.move.end_of_attack;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.known_pokemon;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.other_pokemon;
import tm.pokemon.pokemon;
export import tm.pokemon.pokemon_collection;
import tm.pokemon.seen_pokemon;

import tm.stat.stat_style;

import tm.activate_ability_on_switch;
import tm.apply_entry_hazards;
import tm.any_team;
import tm.compress;
import tm.entry_hazards;
import tm.environment;
import tm.generation;
import tm.initial_team;
import tm.item;
import tm.other_team;
import tm.screens;
import tm.wish;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

template<any_pokemon PokemonType>
struct TeamImpl {
	constexpr explicit TeamImpl(containers::initializer_range<PokemonCollection<PokemonType>> auto && source) requires(!any_seen_pokemon<PokemonType>):
		m_all_pokemon(OPERATORS_FORWARD(source))
	{
	}
	template<std::size_t source_size> requires(!any_seen_pokemon<PokemonType>)
	constexpr explicit TeamImpl(containers::c_array<PokemonType, source_size> && source):
		m_all_pokemon(std::move(source))
	{
	}
	template<bounded::explicitly_convertible_to<PokemonType> OtherPokemonType>
	constexpr explicit TeamImpl(TeamImpl<OtherPokemonType> const & other):
		m_all_pokemon(other.all_pokemon()),
		m_flags(other.flags()),
		m_screens(other.screens()),
		m_wish(other.wish()),
		m_entry_hazards(other.entry_hazards())
	{
	}

	constexpr explicit TeamImpl(TeamSize const team_size) requires any_seen_pokemon<PokemonType>:
		m_all_pokemon(team_size)
	{
	}

	constexpr explicit TeamImpl(InitialTeam<special_style_for(generation_from<PokemonType>)> const & other) requires(!any_seen_pokemon<PokemonType>):
		m_all_pokemon(other)
	{
	}

	constexpr auto all_pokemon() const [[clang::lifetimebound]] -> auto const & {
		return m_all_pokemon;
	}
	constexpr auto all_pokemon() [[clang::lifetimebound]] -> auto & {
		return m_all_pokemon;
	}
	constexpr auto flags() const {
		return m_flags;
	}
	constexpr auto screens() const {
		return m_screens;
	}
	constexpr auto wish() const {
		return m_wish;
	}

	constexpr auto pokemon() const [[clang::lifetimebound]] {
		return AnyActivePokemon<PokemonType>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon() [[clang::lifetimebound]] {
		return AnyMutableActivePokemon<PokemonType>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon(TeamIndex const index) const [[clang::lifetimebound]] -> PokemonType const & {
		return all_pokemon()(index);
	}
	constexpr auto pokemon(TeamIndex const index) [[clang::lifetimebound]] -> PokemonType & {
		return all_pokemon()(index);
	}

	constexpr auto number_of_seen_pokemon() const -> TeamSize requires any_seen_pokemon<PokemonType> {
		return containers::size(all_pokemon());
	}
	constexpr auto size() const -> TeamSize {
		if constexpr (any_seen_pokemon<PokemonType>) {
			return all_pokemon().real_size();
		} else {
			return containers::size(all_pokemon());
		}
	}

	constexpr auto add_pokemon(PokemonType pokemon) [[clang::lifetimebound]] -> PokemonType & requires any_seen_pokemon<PokemonType> {
		return all_pokemon().add(std::move(pokemon));
	}


	constexpr auto reset_end_of_turn() -> void {
		m_flags.reset_end_of_turn();
	}
	constexpr auto clear_field() -> void {
		pokemon().clear_field();
		m_entry_hazards = {};
	}

	constexpr auto switch_pokemon(AnyMutableActivePokemon<OtherPokemon<PokemonType>> other, Environment & environment, TeamIndex const replacement) -> void {
		auto original_pokemon = pokemon();
		original_pokemon.switch_out();
		if constexpr (generation == Generation::one) {
			shatter_screens();
		}

		auto const replacing_fainted = original_pokemon.hp().current() == 0_bi;
		if (replacing_fainted) {
			all_pokemon().remove_active(replacement);
			// If the last Pokemon is fainted; there is nothing left to do.
			if (containers::is_empty(all_pokemon())) {
				return;
			}
		} else {
			all_pokemon().set_index(replacement);
		}

		auto const replacement_pokemon = pokemon();
		replacement_pokemon.switch_in(environment, replacing_fainted);
		apply(m_entry_hazards, replacement_pokemon, other.ability(), environment);
		if (replacement_pokemon.hp().current() != 0_bi) {
			activate_ability_on_switch(replacement_pokemon, other, environment);
		}
		if (!replacing_fainted) {
			end_of_attack(replacement_pokemon, other, environment);
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
	constexpr auto activate_light_screen(Environment const environment) & -> void {
		m_screens.activate_light_screen(extends_light_screen(pokemon().item(environment)));
	}
	constexpr auto activate_reflect(Environment const environment) & -> void {
		m_screens.activate_reflect(extends_reflect(pokemon().item(environment)));
	}
	constexpr auto activate_lucky_chant() & -> void {
		m_screens.activate_lucky_chant();
	}
	constexpr auto activate_mist() & -> void {
		m_screens.activate_mist();
	}
	constexpr auto activate_safeguard() & -> void {
		m_screens.activate_safeguard();
	}
	constexpr auto activate_tailwind() & -> void {
		m_screens.activate_tailwind();
	}
	constexpr auto shatter_screens() & -> void {
		m_screens.shatter();
	}

	constexpr auto activate_wish() & -> void {
		m_wish.activate();
	}
	constexpr auto decrement_wish(Environment const environment) & -> void {
		m_wish.decrement(pokemon(), environment);
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

	friend auto operator==(TeamImpl const &, TeamImpl const &) -> bool = default;

	friend constexpr auto compress(TeamImpl const & team) requires any_real_pokemon<PokemonType> {
		auto const compressed_pokemon = compress(team.m_all_pokemon);
		auto const compressed_flags = compress(team.m_flags);
		if constexpr (generation == Generation::one) {
			static_assert(tv::tuple_size<decltype(compressed_pokemon)> == 2_bi);
			static_assert(tv::tuple_size<decltype(compressed_flags)> == 1_bi);
			return tv::tuple(
				compress_combine(
					compressed_pokemon[0_bi],
					team.m_entry_hazards,
					team.m_wish
				),
				compressed_pokemon[1_bi],
				compress_combine(
					compressed_flags[0_bi],
					team.m_screens
				)
			);
		} else {
			static_assert(tv::tuple_size<decltype(compressed_pokemon)> == 3_bi);
			static_assert(tv::tuple_size<decltype(compressed_flags)> == 2_bi);
			return tv::tuple(
				compress_combine(
					compressed_pokemon[0_bi],
					team.m_screens,
					team.m_entry_hazards,
					team.m_wish
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
	static constexpr auto generation = generation_from<PokemonType>;

	PokemonCollection<PokemonType> m_all_pokemon;
	ActivePokemonFlags<generation> m_flags;
	Screens<generation> m_screens;
	[[no_unique_address]] Wish<generation> m_wish;
	[[no_unique_address]] EntryHazards<generation> m_entry_hazards;
};

export template<Generation generation>
using Team = TeamImpl<Pokemon<generation>>;

export template<Generation generation>
using KnownTeam = TeamImpl<KnownPokemon<generation>>;

export template<Generation generation>
using SeenTeam = TeamImpl<SeenPokemon<generation>>;

template<Generation generation>
struct OtherTeamImpl<Team<generation>> {
	using type = Team<generation>;
};
template<Generation generation>
struct OtherTeamImpl<KnownTeam<generation>> {
	using type = SeenTeam<generation>;
};
template<Generation generation>
struct OtherTeamImpl<SeenTeam<generation>> {
	using type = KnownTeam<generation>;
};

template<Generation generation>
constexpr auto is_real_team<Team<generation>> = true;

template<Generation generation>
constexpr auto is_known_team<KnownTeam<generation>> = true;

template<Generation generation>
constexpr auto is_seen_team<SeenTeam<generation>> = true;

template<typename PokemonType>
constexpr auto generation_from<TeamImpl<PokemonType>> = generation_from<PokemonType>;

} // namespace technicalmachine
