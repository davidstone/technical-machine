// Basic container for Pokemon that helps with bounds checking
// Copyright (C) 2015 David Stone
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

#ifndef POKEMON__CONTAINER_HPP_
#define POKEMON__CONTAINER_HPP_

#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"

#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>

#include <boost/optional.hpp>

#include <cassert>
#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

class PokemonContainer {
private:
	using container_type = bounded::array<boost::optional<Pokemon>, max_pokemon_per_team.value()>;

	template<typename Iterator>
	struct basic_iterator {
		friend class PokemonContainer;
		
		using iterator_category = typename Iterator::iterator_category;
		using value_type = typename Iterator::value_type;
		using difference_type = typename Iterator::difference_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;

		constexpr decltype(auto) operator*() const {
			return **m_it;
		}
		constexpr decltype(auto) operator->() const {
			return &operator*();
		}
		
		auto & operator++() {
			++m_it;
			return *this;
		}
		auto operator++(int) {
			auto temp = *this;
			++m_it;
			return temp;
		}
		template<typename Offset>
		constexpr auto operator+(Offset const & offset) const {
			return basic_iterator(m_it + offset);
		}
		template<typename Offset>
		constexpr auto operator-(Offset const & offset) const {
			return basic_iterator(m_it - offset);
		}
		constexpr auto operator-(basic_iterator const other) const {
			return m_it - other.m_it;
		}

		constexpr auto operator==(basic_iterator const other) const {
			return m_it == other.m_it;
		}
		constexpr auto operator!=(basic_iterator const other) const {
			return !(*this == other);
		}
		
		// Convert iterator to const_iterator
		constexpr operator basic_iterator<container_type::const_iterator>() const {
			return { m_it };
		}
		
	private:
		constexpr basic_iterator(Iterator const it):
			m_it(it) {
		}

		Iterator m_it;
	};
	using const_iterator = basic_iterator<container_type::const_iterator>;
	using iterator = basic_iterator<container_type::iterator>;
public:
	using index_type = TeamIndex;

	auto operator[](index_type index) const -> Pokemon const &;

	auto size() const -> TeamSize {
		return static_cast<TeamSize>(base_end() - m_container.begin());
	}
	auto empty() const -> bool {
		return size() == 0_bi;
	}

	auto begin() const {
		return const_iterator(m_container.begin());
	}
	auto begin() {
		return iterator(m_container.begin());
	}
	auto end() const -> const_iterator {
		return const_iterator(base_end());
	}
	auto end() -> iterator {
		return iterator(base_end());
	}

	auto erase(iterator it) -> void;

	template<class... Args>
	auto emplace_back(Args&&... args) {
		auto const it = base_end();
		assert(it != m_container.end());
		it->emplace(std::forward<Args>(args)...);
	}
	friend auto operator==(PokemonContainer const & lhs, PokemonContainer const & rhs) -> bool;
private:
	auto base_end() const -> container_type::const_iterator;
	auto base_end() -> container_type::iterator;
	container_type m_container;
};

}	// namespace technicalmachine
#endif	// POKEMON__CONTAINER_HPP_
