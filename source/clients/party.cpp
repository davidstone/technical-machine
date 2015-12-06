// Class that handles which party I am
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

#include "party.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto unknown_party = 2_bi;
}	// namespace

Party::Party():
	m_party(unknown_party) {
}

Party::Party(value_type const initial):
	m_party(initial) {
}

auto operator==(Party const lhs, Party const rhs) -> bool {
	return lhs.value() == rhs.value();
}

auto set_if_unknown(Party & party, Party const new_party) -> void {
 	if (party.value() == unknown_party) {
		party = new_party;
	}
}

auto Party::value() const -> value_type{
	return m_party;
}

auto other(Party const party) -> Party {
	return Party(Party::value_type(1_bi - party.value()));
}

} // namespace technicalmachine
