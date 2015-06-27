// Allows safe access to the active Pokemon / move
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

#pragma once

#include "range.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Move;
struct Pokemon;

struct InvalidCollectionIndex final : std::out_of_range {
	template<typename Index, typename Size>
	InvalidCollectionIndex(Index const index, Size const size, std::string const & name):
		out_of_range("Attempted to access element " + bounded::to_string(index) + " in a container of size " + bounded::to_string(size) + " with elements of type " + name + "\n")
		{
	}
};

namespace detail {

template<typename Container>
struct Collection;

template<typename Container>
bool operator==(Collection<Container> const & lhs, Collection<Container> const & rhs) {
	return lhs.container == rhs.container;
}

template<typename Container>
struct Collection {
	using container_type = Container;
	using index_type = typename container_type::index_type;
	template<typename... Args>
	constexpr Collection(Args &&... args) :
		container(std::forward<Args>(args)...),
		m_current_index(0_bi) {
	}
	
	auto begin() const {
		return container.begin();
	}
	auto begin() {
		return container.begin();
	}
	auto end() const {
		return container.end();
	}
	auto end() {
		return container.end();
	}
	
	constexpr decltype(auto) operator() (index_type const specified_index) const {
		return unchecked_value(check_range(specified_index));
	}
	constexpr decltype(auto) operator() () const {
		return unchecked_value(index());
	}
	constexpr bool is_empty() const {
		return container.empty();
	}
	void set_index(index_type const new_index) {
		m_current_index = check_range (new_index);
	}
	void reset_index() {
		m_current_index = 0_bi;
	}
	constexpr index_type index() const {
		return m_current_index;
	}
	friend bool operator== <container_type>(Collection const & lhs, Collection const & rhs);
protected:
	constexpr index_type check_range(index_type const new_index) const {
		using value_type = std::decay_t<decltype(container[new_index])>;
		return (new_index < container.size()) ?
			new_index :
			throw InvalidCollectionIndex(new_index, container.size(), value_type::class_name);
	}
	constexpr decltype(auto) unchecked_value(index_type const specified_index) const {
		return container[specified_index];
	}
	container_type container;
private:
	index_type m_current_index;
};
}	// namespace detail

}	// namespace technicalmachine
