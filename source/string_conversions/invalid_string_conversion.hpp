// Invalid string conversion classes
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

#include <boost/utility/string_ref.hpp>

#include <stdexcept>

namespace technicalmachine {

// I make the FromString a runtime error because I only convert from a string
// that I get from 'outside' the program. It may also represent a logic error,
// but it is definitely a runtime error.

class InvalidFromStringConversion : public std::runtime_error {
public:
	InvalidFromStringConversion(boost::string_ref const target, boost::string_ref const requested);
};

}	// namespace technicalmachine

