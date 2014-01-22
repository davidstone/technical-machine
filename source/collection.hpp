// Allows safe access to the active Pokemon / move
// Copyright (C) 2012 David Stone
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

#include <cstdint>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace technicalmachine {

class InvalidCollectionIndex : public std::out_of_range {
public:
	InvalidCollectionIndex (unsigned index, size_t size, std::string const & name):
		out_of_range ("Attempted to access element " + std::to_string (index) + " in a container of size " + std::to_string (size) + " with elements of type " + name + "\n")
		{
	}
};

namespace detail {

template<typename Container>
class BaseCollection;

template<typename Container>
bool operator==(BaseCollection<Container> const & lhs, BaseCollection<Container> const & rhs) {
	return lhs.container == rhs.container;
}

template<typename Container>
class BaseCollection {
public:
	using container_type = Container;
	using index_type = uint8_t;
	using value_type = typename container_type::value_type;
	template<typename... Args>
	constexpr BaseCollection(Args &&... args) :
		container(std::forward<Args>(args)...),
		current_index (0) {
	}
	constexpr value_type const & operator() (index_type const specified_index) const {
		return unchecked_value (check_range(specified_index));
	}
	constexpr value_type const & operator() () const {
		return unchecked_value (index());
	}
	constexpr bool is_empty() const {
		return container.empty();
	}
	template<class... Args>
	void add(Args&&... args) {
		container.emplace_back(std::forward<Args>(args)...);
	}
	void set_index (index_type const new_index) {
		current_index = check_range (new_index);
	}
	void reset_index () {
		current_index = 0;
	}
	constexpr index_type index() const {
		return current_index;
	}
	friend bool operator== <container_type>(BaseCollection<container_type> const & lhs, BaseCollection<container_type> const & rhs);
protected:
	static constexpr index_type check_range(index_type const new_index, index_type const max_index) {
		return (new_index < max_index) ? new_index : throw InvalidCollectionIndex(new_index, max_index, typeid(value_type).name());
	}
	constexpr index_type check_range (index_type const new_index) const {
		return check_range(new_index, container.size());
	}
	constexpr value_type const & unchecked_value (index_type const specified_index) const {
		return container [specified_index];
	}
	container_type container;
	index_type current_index;
};
}	// namespace detail

}	// namespace technicalmachine
#endif	// COLLECTION_HPP_
