// Invalid string conversion classes
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

#ifndef STRING_CONVERSIONS__INVALID_STRING_CONVERSION_HPP_
#define STRING_CONVERSIONS__INVALID_STRING_CONVERSION_HPP_

#include <stdexcept>
#include <string>

namespace technicalmachine {

// I make the ToString conversion a logic error because I only convert to a
// string from an internal data structure, so I know the input is only invalid
// if there is a logic error in my program.

class InvalidToStringConversion : public std::logic_error {
	public:
		template <typename Test>
		InvalidToStringConversion (Test original, Test result, std::string const & intermediate):
			std::logic_error (std::to_string (static_cast<unsigned> (original)) + " is seen as " + std::to_string (static_cast<unsigned> (result)) + " with an intermediate string of " + intermediate + ".\n") {
		}
};

// I make the FromString a runtime error because I only convert from a string
// that I get from 'outside' the program. It may also represent a logic error,
// but it is definitely a runtime error.

class InvalidFromStringConversion : public std::runtime_error {
	public:
		InvalidFromStringConversion (std::string const & target, std::string const & requested):
			std::runtime_error ("Invalid conversion from " + requested + " to type " + target + " requested.\n")
			{
		}
};

}	// namespace technicalmachine

#endif	// STRING_CONVERSIONS__INVALID_STRING_CONVERSION_HPP_
