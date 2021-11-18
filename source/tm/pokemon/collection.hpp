// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/pokemon_not_found.hpp>

#include <tm/compress.hpp>
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

#include <stdexcept>

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
using PokemonContainer = containers::static_vector<Pokemon<generation>, max_pokemon_per_team.value()>;

template<Generation generation>
struct PokemonCollection {
	using Container = PokemonContainer<generation>;

	explicit PokemonCollection(TeamSize const initial_size):
		m_real_size(initial_size)
	{
	}
	constexpr explicit PokemonCollection(Container all_pokemon):
		m_container(all_pokemon),
		m_real_size(containers::size(m_container))
	{
	}

	auto begin() const {
		return containers::begin(m_container);
	}
	auto begin() {
		return containers::begin(m_container);
	}
	auto size() const {
		return containers::size(m_container);
	}
	
	void set_index(TeamIndex const new_index) {
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

	auto real_size() const {
		return m_real_size;
	}

	Pokemon<generation> & add(Pokemon<generation> pokemon) {
		check_not_full();
		return containers::push_back(m_container, std::move(pokemon));
	}

	void remove_active(TeamIndex const index_of_replacement) {
		auto const original_index = index();
		BOUNDED_ASSERT(original_index != index_of_replacement);
		containers::erase(m_container, begin() + original_index);
		--m_real_size;
		set_index((index_of_replacement < original_index) ?
			index_of_replacement :
			TeamIndex(index_of_replacement - 1_bi, bounded::non_check)
		);
	}

	friend auto operator==(PokemonCollection const &, PokemonCollection const &) -> bool = default;
	friend auto compress(PokemonCollection const & value) {
		using containers::size;
		static_assert(numeric_traits::max_value<decltype(containers::size(value.m_container))> == 6_bi);
		auto const p0 = containers::size(value.m_container) >= 1_bi ? compress(value.m_container[0_bi]) : 0_bi;
		auto const p1 = containers::size(value.m_container) >= 2_bi ? compress(value.m_container[1_bi]) : 0_bi;
		auto const p2 = containers::size(value.m_container) >= 3_bi ? compress(value.m_container[2_bi]) : 0_bi;
		auto const p3 = containers::size(value.m_container) >= 4_bi ? compress(value.m_container[3_bi]) : 0_bi;
		auto const p4 = containers::size(value.m_container) >= 5_bi ? compress(value.m_container[4_bi]) : 0_bi;
		auto const p5 = containers::size(value.m_container) >= 6_bi ? compress(value.m_container[5_bi]) : 0_bi;
		if constexpr (generation == Generation::one) {
			return bounded::tuple(
				compress_combine(p0, p1, p2, value.m_index),
				compress_combine(p3, p4, p5, value.m_real_size)
			);
		} else {
			return bounded::tuple(
				compress_combine(p0, p1),
				compress_combine(p2, p3),
				compress_combine(p4, p5, value.m_index, value.m_real_size)
			);
		}
	}
private:
	void check_not_full() {
		if (size() == m_real_size) {
			throw std::runtime_error("Tried to add too many Pokemon");
		}
	}
	constexpr void check_range(TeamIndex const new_index [[maybe_unused]]) const {
		BOUNDED_ASSERT(new_index < size());
	}

	Container m_container;
	TeamIndex m_index = 0_bi;
	// The actual size of the foe's team, not just the Pokemon I've seen
	TeamSize m_real_size;
};

BOUNDED_COMMON_ARITHMETIC

template<Generation generation>
inline auto find_index(PokemonCollection<generation> const & collection, Species const species) {
	return bounded::assume_in_range<TeamIndex>(containers::find_if(collection, [=](Pokemon<generation> const pokemon) { return pokemon.species() == species; }) - collection.begin());
}

template<Generation generation>
inline auto find_present_index(PokemonCollection<generation> const & collection, Species const species) {
	auto const index = find_index(collection, species);
	if (index == containers::size(collection)) {
		throw PokemonNotFound(species);
	}
	return index;
}

} // namespace technicalmachine
