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

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include <iostream>

namespace technicalmachine {

class InvalidActiveIndex : public std::logic_error {
	public:
		InvalidActiveIndex (unsigned index, size_t size, std::string const & name):
			logic_error ("Attempted to access element " + std::to_string (index) + " in a set of size " + std::to_string (size) + " with elements of type " + name + "\n")
			{
		}
};

template <class T>
class Active {
	public:
		Active () :
			current_index (0) {
		}
		Active (std::vector<T> const & pre_set) :
			set (pre_set),
			current_index (0) {
		}
		#ifndef NDEBUG
		T & operator() (uint8_t const specified_index) {
			if (specified_index >= set.size()) {
				InvalidActiveIndex ex (specified_index, set.size(), typeid(T).name());
				std::cerr << ex.what();
			}
			return set [specified_index];
		}
		T const & operator() (uint8_t const specified_index) const {
			if (specified_index >= set.size()) {
				InvalidActiveIndex ex (specified_index, set.size(), typeid(T).name());
				std::cerr << ex.what();
			}
			return set [specified_index];
		}
		#else	// NDEBUG
		T & operator() (uint8_t const specified_index) {
			if (specified_index < set.size())
				return set [specified_index];
			else
				throw InvalidActiveIndex (specified_index, set.size(), typeid(T).name());
		}
		T const & operator() (uint8_t const specified_index) const {
			if (specified_index < set.size())
				return set [specified_index];
			else
				throw InvalidActiveIndex (specified_index, set.size(), typeid(T).name());
		}
		#endif	// NDEBUG
		T & operator() () {
			return operator() (current_index);
		}
		T const & operator() () const {
			return operator() (current_index);
		}
		void add (T const & name) {
			set.insert (set.begin() + current_index, name);
		}
		void remove_active () {
			set.erase (set.begin() + current_index);
		}
		void set_index (uint8_t const new_index) {
			assert (new_index < set.size());
			current_index = new_index;
		}
		void reset_index () {
			current_index = 0;
		}
		uint8_t index() const {
			return current_index;
		}
		// All Pokemon on the team, all moves on the Pokemon, etc.
		std::vector<T> set;
	private:
		uint8_t current_index;
};

}	// namespace technicalmachine
#endif	// ACTIVE_HPP_
