// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.pokemon.pokemon_collection;

import tm.pokemon.any_pokemon;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon_not_found;
import tm.pokemon.species;

import tm.compress;
import tm.exists_if;
import tm.generation;
export import tm.operators;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<any_pokemon PokemonType>
struct PokemonCollection {
	using Container = containers::static_vector<PokemonType, max_pokemon_per_team>;

	constexpr explicit PokemonCollection(TeamSize const real_size) requires any_seen_pokemon<PokemonType>:
		m_real_size(real_size)
	{
	}

	constexpr explicit PokemonCollection(containers::constructor_initializer_range<PokemonCollection> auto && source)
		requires any_known_pokemon<PokemonType> or any_real_pokemon<PokemonType>:
		m_container(OPERATORS_FORWARD(source))
	{
	}
	template<std::size_t source_size> requires any_known_pokemon<PokemonType> or any_real_pokemon<PokemonType>
	constexpr explicit PokemonCollection(containers::c_array<PokemonType, source_size> && source):
		m_container(std::move(source))
	{
	}

	template<typename OtherPokemon> requires any_known_pokemon<PokemonType> or any_real_pokemon<PokemonType>
	constexpr explicit PokemonCollection(PokemonCollection<OtherPokemon> const & other):
		m_container(containers::transform(other, [](OtherPokemon const & pokemon) { return PokemonType(pokemon); })),
		m_index(other.index())
	{
	}

	constexpr auto begin() const {
		return containers::begin(m_container);
	}
	constexpr auto begin() {
		return containers::begin(m_container);
	}
	constexpr auto size() const {
		return containers::size(m_container);
	}
	constexpr auto real_size() const -> TeamSize requires any_seen_pokemon<PokemonType> {
		return m_real_size;
	}

	constexpr auto set_index(TeamIndex const new_index) -> void {
		check_range(new_index);
		m_index = new_index;
	}
	constexpr auto index() const -> TeamIndex {
		return m_index;
	}

	constexpr auto operator()(TeamIndex const specified_index) const & -> PokemonType const & {
		check_range(specified_index);
		return m_container[specified_index];
	}
	constexpr auto operator()(TeamIndex const specified_index) & -> PokemonType & {
		check_range(specified_index);
		return m_container[specified_index];
	}
	constexpr auto operator()() const & -> PokemonType const & {
		return m_container[index()];
	}
	constexpr auto operator()() & -> PokemonType & {
		return m_container[index()];
	}

	constexpr auto add(PokemonType pokemon) -> PokemonType & requires any_seen_pokemon<PokemonType> {
		return containers::push_back(m_container, std::move(pokemon));
	}

	constexpr auto remove_active(TeamIndex const index_of_replacement) -> void {
		auto const original_index = index();
		BOUNDED_ASSERT(original_index != index_of_replacement);
		containers::erase(m_container, begin() + original_index);
		set_index((index_of_replacement < original_index) ?
			index_of_replacement :
			TeamIndex(index_of_replacement - 1_bi, bounded::unchecked)
		);
		if constexpr (any_seen_pokemon<PokemonType>) {
			--m_real_size;
		}
	}

	friend auto operator==(PokemonCollection const &, PokemonCollection const &) -> bool = default;

	friend constexpr auto compress(PokemonCollection const & value) requires any_real_pokemon<PokemonType> {
		using containers::size;
		static_assert(numeric_traits::max_value<decltype(containers::size(value.m_container))> == 6_bi);
		auto const p0 = containers::size(value.m_container) >= 1_bi ? compress(value.m_container[0_bi]) : 0_bi;
		auto const p1 = containers::size(value.m_container) >= 2_bi ? compress(value.m_container[1_bi]) : 0_bi;
		auto const p2 = containers::size(value.m_container) >= 3_bi ? compress(value.m_container[2_bi]) : 0_bi;
		auto const p3 = containers::size(value.m_container) >= 4_bi ? compress(value.m_container[3_bi]) : 0_bi;
		auto const p4 = containers::size(value.m_container) >= 5_bi ? compress(value.m_container[4_bi]) : 0_bi;
		auto const p5 = containers::size(value.m_container) >= 6_bi ? compress(value.m_container[5_bi]) : 0_bi;
		if constexpr (generation_from<PokemonType> == Generation::one) {
			return tv::tuple(
				compress_combine(p0, p1, p2, value.m_index),
				compress_combine(p3, p4, p5)
			);
		} else {
			return tv::tuple(
				compress_combine(p0, p1),
				compress_combine(p2, p3),
				compress_combine(p4, p5, value.m_index)
			);
		}
	}

private:
	constexpr auto check_range(TeamIndex const new_index) const -> void {
		BOUNDED_ASSERT(new_index < size());
	}

	Container m_container;
	TeamIndex m_index = 0_bi;
	// The actual size of the foe's team, not just the Pokemon I've seen
	[[no_unique_address]] IntegerIf<TeamSize, any_seen_pokemon<PokemonType>> m_real_size;
};

} // namespace technicalmachine
