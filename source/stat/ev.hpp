// EVs
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

#ifndef STAT__EV_HPP_
#define STAT__EV_HPP_

#include <cstdint>

namespace technicalmachine {

class EV {
	public:
		explicit EV(unsigned evs);
		unsigned value() const;
		void set_value(unsigned evs);
		// A point increases your stat by 1 at level 100. Smallest useful unit
		unsigned points() const;
	private:
		uint8_t internal;
};

}	// namespace technicalmachine
#endif	// STAT__EV_HPP_
