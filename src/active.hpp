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

#ifndef ACTIVE_HPP_
#define ACTIVE_HPP_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include <iostream>

#include <boost/utility/enable_if.hpp>

namespace technicalmachine {

class InvalidActiveIndex : public std::logic_error {
	public:
		InvalidActiveIndex (unsigned index, size_t size, std::string const & name):
			logic_error ("Attempted to access element " + std::to_string (index) + " in a container of size " + std::to_string (size) + " with elements of type " + name + "\n")
			{
		}
};

namespace detail {
template<typename T> class BaseActive;
}	// namespace detail

template<typename T>
bool operator==(detail::BaseActive<T> const & lhs, detail::BaseActive<T> const & rhs) {
	return lhs.container == rhs.container;
}

// BaseActive is defined to allow the addition of a member function to the
// template specialization where T == Move. Do not use outside this header.
namespace detail {
template<typename T>
class BaseActive {
	public:
		typedef std::vector<T> container_type;
		typedef T value_type;
		BaseActive () :
			current_index (0) {
		}
		BaseActive (container_type const & pre_set) :
			container (pre_set),
			current_index (0) {
		}
		// In debug mode, I intentionally access the invalid value so I can get
		// a Valgrind stack trace. This will eventually be replaced with a smart
		// assert.
		#ifndef NDEBUG
		T const & operator() (uint8_t const specified_index) const {
			if (specified_index >= container.size()) {
				InvalidActiveIndex ex (specified_index, container.size(), typeid(T).name());
				std::cerr << ex.what();
			}
			return container [specified_index];
		}
		#else	// NDEBUG
		T const & operator() (uint8_t const specified_index) const {
			if (specified_index < container.size())
				return container [specified_index];
			else
				throw InvalidActiveIndex (specified_index, container.size(), typeid(T).name());
		}
		#endif	// NDEBUG
		T & operator() (uint8_t const specified_index) {
			return const_cast<T &>(const_cast<BaseActive const *>(this)->operator()(specified_index));
		}
		T & operator() () {
			return operator() (current_index);
		}
		T const & operator() () const {
			return operator() (current_index);
		}
		bool is_empty() const {
			return container.empty();
		}
		uint8_t size () const {
			return container.size();
		}
		void add (T const & element) {
			container.push_back (element);
		}
		void insert (uint8_t const position, T const & element) {
			container.insert (container.begin() + position, element);
		}
		void insert (T const & element) {
			insert (current_index, element);
		}
		void remove_active () {
			container.erase (container.begin() + current_index);
		}
		void pop_back () {
			container.pop_back();
			reset_index();
		}
		void set_index (uint8_t const new_index) {
			assert (new_index < container.size());
			current_index = new_index;
		}
		void reset_index () {
			current_index = 0;
		}
		uint8_t index() const {
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
		friend bool operator==<T>(BaseActive<T> const & lhs, BaseActive<T> const & rhs);
	protected:
		// All Pokemon on the team, all moves on the Pokemon, etc.
		container_type container;
		uint8_t current_index;
};
}	// namespace detail

// The general form is just the above functions. I'm not sure how to disable
// this for specialized versions (in case I forget to include their specialized
// header), because I cannot include the specialized type in this header or I
// get circular dependencies.
template<typename T>
class Active : public detail::BaseActive<T> {
	public:
		Active () = default;
		Active (typename detail::BaseActive<T>::container_type const & pre_set):
			detail::BaseActive<T>(pre_set) {
		}
};

}	// namespace technicalmachine
#endif	// ACTIVE_HPP_
