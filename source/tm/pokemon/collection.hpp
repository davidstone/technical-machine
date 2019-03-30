// Collection of Pokemon with index indicating current Pokemon
// Copyright (C) 2019 David Stone
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

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/operators.hpp>
#include <tm/range.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/common_container_functions.hpp>
#include <containers/index_type.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <stdexcept>
#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

struct PokemonCollection {
	using Container = containers::static_vector<Pokemon, max_pokemon_per_team.value()>;
	using value_type = Container::value_type;
	using size_type = Container::size_type;
	using const_iterator = Container::const_iterator;

	explicit PokemonCollection(TeamSize const initial_size):
		m_real_size(initial_size)
	{
	}

	friend auto begin(PokemonCollection const & collection) noexcept {
		return begin(collection.m_container);
	}
	friend auto begin(PokemonCollection & collection) noexcept {
		return begin(collection.m_container);
	}
	friend auto end(PokemonCollection const & collection) noexcept {
		return end(collection.m_container);
	}
	friend auto end(PokemonCollection & collection) noexcept {
		return end(collection.m_container);
	}
	
	void set_index(containers::index_type<PokemonCollection> const new_index) {
		check_range(new_index);
		m_index = new_index;
	}
	void reset_index() {
		m_index = 0_bi;
	}
	constexpr auto index() const {
		return m_index;
	}

	constexpr decltype(auto) operator()(containers::index_type<PokemonCollection> const specified_index) const {
		check_range(specified_index);
		return m_container[specified_index];
	}
	constexpr decltype(auto) operator()(containers::index_type<PokemonCollection> const specified_index) {
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

	template<typename... Args>
	Pokemon & add(Args&&... args) {
		if (size(m_container) == m_real_size) {
			throw std::runtime_error("Tried to add too many Pokemon");
		}
		return m_container.emplace_back(m_real_size, std::forward<Args>(args)...);
	}

	void remove_active(containers::index_type<PokemonCollection> index_of_replacement);

private:
	void check_range(containers::index_type<PokemonCollection> const new_index [[maybe_unused]]) const {
		BOUNDED_ASSERT(new_index < containers::size(m_container));
	}

	Container m_container;
	containers::index_type<PokemonCollection> m_index = 0_bi;
	// The actual size of the foe's team, not just the Pokemon I've seen
	TeamSize m_real_size;
};

CONTAINERS_COMMON_USING_DECLARATIONS

}	// namespace technicalmachine
