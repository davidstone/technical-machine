// Class that handles which party I am
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

#ifndef CLIENTS__PARTY_HPP_
#define CLIENTS__PARTY_HPP_

#include <cstdint>

namespace technicalmachine {

class Party {
	public:
		typedef uint8_t value_type;
		Party();
		explicit Party(value_type initial);
		void set_if_unknown (Party new_party);
		value_type operator()() const;
		friend bool operator==(Party lhs, Party rhs);
	private:
		value_type party;
};

bool operator!=(Party lhs, Party rhs);

}	// namespace technicalmachine
#endif	// CLIENTS__PARTY_HPP_
