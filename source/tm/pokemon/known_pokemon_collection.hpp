// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/known_pokemon.hpp>
#include <tm/pokemon/pokemon_not_found.hpp>

#include <tm/operators.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/algorithms/erase.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/begin_end.hpp>
#include <containers/push_back.hpp>
#include <containers/range_value_t.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct KnownPokemonCollection {
	using Container = containers::static_vector<KnownPokemon<generation>, max_pokemon_per_team.value()>;

	constexpr explicit KnownPokemonCollection(containers::initializer_range<KnownPokemonCollection> auto && source):
		m_container(OPERATORS_FORWARD(source))
	{
	}
	template<std::size_t source_size>
	constexpr explicit KnownPokemonCollection(containers::c_array<KnownPokemon<generation>, source_size> && source):
		m_container(std::move(source))
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
	
	constexpr auto set_index(TeamIndex const new_index) -> void {
		check_range(new_index);
		m_index = new_index;
	}
	constexpr auto index() const {
		return m_index;
	}

	constexpr decltype(auto) operator()(TeamIndex const specified_index) const {
		check_range(specified_index);
		return m_container[specified_index];
	}
	constexpr decltype(auto) operator()(TeamIndex const specified_index) {
		check_range(specified_index);
		return m_container[specified_index];
	}
	constexpr decltype(auto) operator()() const {
		return m_container[index()];
	}
	constexpr decltype(auto) operator()() {
		return m_container[index()];
	}

	constexpr auto remove_active(TeamIndex const index_of_replacement) -> void {
		auto const original_index = index();
		BOUNDED_ASSERT(original_index != index_of_replacement);
		containers::erase(m_container, begin() + original_index);
		set_index((index_of_replacement < original_index) ?
			index_of_replacement :
			TeamIndex(index_of_replacement - 1_bi, bounded::non_check)
		);
	}

	friend auto operator==(KnownPokemonCollection const &, KnownPokemonCollection const &) -> bool = default;
private:
	constexpr auto check_range(TeamIndex const new_index [[maybe_unused]]) const -> void {
		BOUNDED_ASSERT(new_index < size());
	}

	Container m_container;
	TeamIndex m_index = 0_bi;
};

BOUNDED_COMMON_ARITHMETIC

template<Generation generation>
constexpr auto find_index(KnownPokemonCollection<generation> const & collection, Species const species) {
	auto const it = containers::find_if(collection, [=](KnownPokemon<generation> const pokemon) { return pokemon.species() == species; });
	if (it == containers::end(collection)) {
		throw PokemonNotFound(species);
	}
	return bounded::assume_in_range<TeamIndex>(it - containers::begin(collection));
}

} // namespace technicalmachine
