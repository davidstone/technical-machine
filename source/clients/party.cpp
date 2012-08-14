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

#include "party.hpp"

namespace technicalmachine {
namespace {
constexpr Party::value_type unknown_party = 255;
}	// unnamed namespace

Party::Party():
	party(unknown_party)
	{
}

Party::Party(value_type const initial):
	party(initial)
	{
}

bool operator==(Party const lhs, Party const rhs) {
	return lhs.party == rhs.party;
}

bool operator!=(Party const lhs, Party const rhs) {
	return !(lhs == rhs);
}

void Party::set_if_unknown (Party const new_party) {
 	if (party == unknown_party)
		*this = new_party;
}

Party::value_type Party::operator()() const {
	return party;
}

} // namespace technicalmachine
