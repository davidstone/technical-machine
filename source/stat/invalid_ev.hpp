// Exception class when initializing EVs to a number greater than 252
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

#ifndef STAT__INVALID_EV_HPP_
#define STAT__INVALID_EV_HPP_

#include <stdexcept>

namespace technicalmachine {

class InvalidEV : public std::runtime_error {
public:
	explicit InvalidEV(unsigned const ev);
};

}	// namespace technicalmachine
#endif	// STAT__INVALID_EV_HPP_
