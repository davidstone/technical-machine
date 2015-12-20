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

#pragma once

#include "../operators.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Party {
	// -1 indicates the message does not apply to a party
	using value_type = bounded::checked_integer<-1, 2>;
	Party();
	explicit Party(value_type initial);
	auto value() const -> value_type;
private:
	value_type m_party;
};

auto operator==(Party lhs, Party rhs) -> bool;

auto set_if_unknown(Party & party, Party new_party) -> void;
auto other(Party const party) -> Party;

}	// namespace technicalmachine
