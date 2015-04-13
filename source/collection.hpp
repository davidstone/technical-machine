// Allows safe access to the active Pokemon / move
// Copyright (C) 2014 David Stone
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

#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include <cstddef>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

class InvalidCollectionIndex : public std::out_of_range {
public:
	template<typename Index, typename Size>
	InvalidCollectionIndex(Index const index, Size const size, std::string const & name):
		out_of_range("Attempted to access element " + bounded::to_string(index) + " in a container of size " + bounded::to_string(size) + " with elements of type " + name + "\n")
		{
	}
};

template<typename Iterator>
class Range {
public:
	using iterator = Iterator;
	constexpr Range(iterator first, iterator last):
		m_first(std::move(first)),
		m_last(std::move(last)) {
	}
	constexpr auto begin() const {
		return m_first;
	}
	constexpr auto end() const {
		return m_last;
	}
private:
	iterator m_first;
	iterator m_last;
};

template<typename Iterator>
constexpr auto size(Range<Iterator> const & range) {
	return range.end() - range.begin();
}

template<typename Iterator>
constexpr auto make_range(Iterator first, Iterator last) {
	return Range<Iterator>(first, last);
}

namespace detail {

template<typename Container>
class Collection;

template<typename Container>
bool operator==(Collection<Container> const & lhs, Collection<Container> const & rhs) {
	return lhs.container == rhs.container;
}

template<typename Container>
class Collection {
public:
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
		return (new_index < container.size()) ? new_index : throw InvalidCollectionIndex(new_index, container.size(), typeid(value_type).name());
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
#endif	// COLLECTION_HPP_
