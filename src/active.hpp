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

#include <cstdint>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include <iostream>

namespace technicalmachine {

class InvalidActiveIndex;

template <class T>
class Active {
	public:
		// All Pokemon on the team, all moves on the Pokemon, etc.
		std::vector<T> set;
		uint8_t index;
		Active () :
			index (0) {
		}
		Active (std::vector<T> const & pre_set) :
			set (pre_set),
			index (0) {
		}
		#ifndef NDEBUG
		T & operator() () {
			if (index >= set.size()) {
				std::cout << "index: " << index << '\n';
				std::cout << "set.size(): " << set.size() << '\n';
				std::cout << "type: " << typeid(T).name() << '\n';
			}
			return set [index];
		}
		T const & operator() () const {
			if (index >= set.size()) {
				std::cout << "index: " << index << '\n';
				std::cout << "set.size(): " << set.size() << '\n';
				std::cout << "type: " << typeid(T).name() << '\n';
			}
			return set [index];
		}
		#else	// NDEBUG
		T & operator() () {
			if (index < set.size())
				return set [index];
			else
				throw InvalidActiveIndex (index, set.size(), typeid(T).name());
		}
		T const & operator() () const {
			if (index < set.size())
				return set [index];
			else
				throw InvalidActiveIndex (index, set.size(), typeid(T).name());
		}
		#endif	// NDEBUG
		void add (T name) {
			set.insert (set.begin() + index, name);
		}
		void remove_active () {
			set.erase (set.begin() + index);
		}
};

class InvalidActiveIndex : public std::logic_error {
	public:
		InvalidActiveIndex (unsigned index, size_t size, std::string const & name):
			logic_error ("Attempted to access element " + std::to_string (index) + " in a set of size " + std::to_string (size) + " with elements of type " + name + "\n")
			{
		}
};

}	// namespace technicalmachine
#endif	// ACTIVE_HPP_
