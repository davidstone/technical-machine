// Copyright (C) 2014 David Stone
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

#include <bounded/integer.hpp>
#include <iosfwd>

namespace technicalmachine {
using namespace bounded::literal;

struct Happiness {
	using value_type = bounded::checked_integer<0, 255>;
	explicit Happiness(value_type value = 255_bi);
	// Needed to send out to simulators
	value_type operator()() const;
	friend bounded::integer<0, 102> return_power(Happiness happiness);
private:
	value_type m_value;	
};

bounded::integer<0, 102> frustration_power(Happiness happiness);

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out, Happiness const happiness) {
	return out << happiness();
}

}	// namespace technicalmachine
