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

#include <algorithm>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace technicalmachine {

class InvalidCollectionIndex : public std::out_of_range {
	public:
		InvalidCollectionIndex (unsigned index, unsigned size, std::string const & name):
			out_of_range ("Attempted to access element " + std::to_string (index) + " in a container of size " + std::to_string (size) + " with elements of type " + name + "\n")
			{
		}
};

namespace detail {
template<typename T> class BaseCollection;
}	// namespace detail

template<typename T>
bool operator==(detail::BaseCollection<T> const & lhs, detail::BaseCollection<T> const & rhs) {
	return lhs.container == rhs.container;
}

// BaseCollection is defined only as a base class to Pokemon, Move, and Variable
// collections. Do not use directly.
namespace detail {
template<typename T>
class BaseCollection {
	public:
		typedef std::vector<T> container_type;
		typedef T value_type;
		constexpr BaseCollection () :
			current_index (0) {
		}
		constexpr BaseCollection (container_type const & pre_set) :
			container (pre_set),
			current_index (0) {
		}
		constexpr T const & operator() (uint8_t const specified_index) const {
			return unchecked_value (check_range(specified_index));
		}
		T & operator() (uint8_t const specified_index) {
			return unchecked_value (check_range(specified_index));
		}
		constexpr T const & operator() () const {
			return unchecked_value (index());
		}
		T & operator() () {
			return unchecked_value (index());
		}
		constexpr bool is_empty() const {
			return container.empty();
		}
		void add (T const & element) {
			container.push_back (element);
		}
		void insert (uint8_t const position, T const & element) {
			container.insert (container.begin() + position, element);
		}
		void insert (T const & element) {
			insert (index(), element);
		}
		void pop_back () {
			container.pop_back();
			reset_index();
		}
		void set_index (uint8_t const new_index) {
			current_index = check_range (new_index);
		}
		void reset_index () {
			current_index = 0;
		}
		constexpr uint8_t index() const {
			return current_index;
		}
		void for_each (typename std::function<void(T &)> const & f) {
			for (T & element : container) {
				f (element);
			}
		}
		void for_each (typename std::function<void(T const &)> const & f) const {
			for (T const & element : container) {
				f (element);
			}
		}
		unsigned count_if (typename std::function<bool(T const &)> const & f) const {
			return std::count_if(container.begin(), container.end(), f);
		}
		friend bool operator==<T>(BaseCollection<T> const & lhs, BaseCollection<T> const & rhs);
	protected:
		uint8_t check_range (uint8_t const new_index) const {
			if (new_index < container.size())
				return new_index;
			else
				throw InvalidCollectionIndex (new_index, container.size(), typeid(T).name());
		}
		constexpr T const & unchecked_value (uint8_t const specified_index) const {
			return container [specified_index];
		}
		T & unchecked_value (uint8_t const specified_index) {
			return container [specified_index];
		}
		container_type container;
		uint8_t current_index;
};
}	// namespace detail

}	// namespace technicalmachine
#endif	// COLLECTION_HPP_
