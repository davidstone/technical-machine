// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.team;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.known_pokemon;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.other_pokemon;
import tm.pokemon.pokemon;
export import tm.pokemon.pokemon_collection;
import tm.pokemon.seen_pokemon;

import tm.activate_ability_on_switch;
import tm.apply_entry_hazards;
import tm.any_team;
import tm.compress;
import tm.entry_hazards;
import tm.environment;
import tm.generation;
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

template<typename PokemonType>
using bool_type = std::conditional_t<
	any_real_pokemon<PokemonType>,
	bool,
	std::bool_constant<any_known_pokemon<PokemonType>>
>;

template<typename Target, typename From>
constexpr auto convert_bool_type(From const value) -> Target {
	static_assert(!std::same_as<From, Target>);
	if constexpr (std::same_as<From, bool>) {
		return Target();
	} else {
		return Target(value);
	}
}

template<any_pokemon PokemonType>
struct TeamImpl {
	constexpr explicit TeamImpl(containers::initializer_range<PokemonCollection<PokemonType>> auto && source, bool_type<PokemonType> me = {}) requires(!any_seen_pokemon<PokemonType>):
		m_all_pokemon(OPERATORS_FORWARD(source)),
		m_me(me)
	{
	}
	template<std::size_t source_size> requires(!any_seen_pokemon<PokemonType>)
	constexpr explicit TeamImpl(containers::c_array<PokemonType, source_size> && source, bool_type<PokemonType> me = {}):
		m_all_pokemon(std::move(source)),
		m_me(me)
	{
	}
	template<bounded::explicitly_convertible_to<PokemonType> OtherPokemonType>
	constexpr explicit TeamImpl(TeamImpl<OtherPokemonType> const & other):
		m_all_pokemon(other.all_pokemon()),
		m_flags(other.flags()),
		m_screens(other.screens()),
		m_wish(other.wish()),
		m_entry_hazards(other.entry_hazards()),
		m_me(convert_bool_type<bool_type<PokemonType>>(other.is_me()))
	{
	}

	constexpr explicit TeamImpl(TeamSize const team_size) requires any_seen_pokemon<PokemonType>:
		m_all_pokemon(team_size)
	{
	}

	constexpr auto all_pokemon() const -> auto const & {
		return m_all_pokemon;
	}
	constexpr auto all_pokemon() -> auto & {
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

	constexpr auto pokemon() const {
		return AnyActivePokemon<PokemonType>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon() {
		return AnyMutableActivePokemon<PokemonType>(all_pokemon()(), m_flags);
	}
	constexpr auto pokemon(TeamIndex const index) const -> PokemonType const & {
		return all_pokemon()(index);
	}
	constexpr auto pokemon(TeamIndex const index) -> PokemonType & {
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

	constexpr auto add_pokemon(PokemonType pokemon) -> PokemonType & requires any_seen_pokemon<PokemonType> {
		return all_pokemon().add(std::move(pokemon));
	}


	constexpr auto reset_start_of_turn() -> void {
		m_flags.reset_start_of_turn();
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
		replacement_pokemon.switch_in(environment);
		apply(m_entry_hazards, replacement_pokemon, environment);
		if (replacement_pokemon.hp().current() != 0_bi) {
			activate_ability_on_switch(replacement_pokemon, other, environment);
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

	constexpr auto is_me() const {
		return m_me;
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
	[[no_unique_address]] bool_type<PokemonType> m_me;
};

#define TM_EXPLICIT_INSTANTIATION_IMPL(PokemonType) \
	template struct TeamImpl<PokemonType>

#define TM_EXPLICIT_INSTANTIATION(generation) \
	TM_EXPLICIT_INSTANTIATION_IMPL(Pokemon<generation>); \
	TM_EXPLICIT_INSTANTIATION_IMPL(KnownPokemon<generation>); \
	TM_EXPLICIT_INSTANTIATION_IMPL(SeenPokemon<generation>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TM_EXPLICIT_INSTANTIATION);

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
