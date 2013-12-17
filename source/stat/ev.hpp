// EVs
// Copyright (C) 2013 David Stone
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

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded_integer::literal;

class EV {
public:
	static constexpr auto max = 252;
	static constexpr auto max_total = 508;
	using value_type = bounded_integer::checked_integer<0, max>;
	using total_type = bounded_integer::checked_integer<0, max_total>;
	explicit EV(value_type evs);
	bounded_integer::native_integer<0, max> value() const;
	bool is_maxed() const;
	void add(value_type evs);
private:
	bounded_integer::clamped_integer<0, max> m_value;
};

}	// namespace technicalmachine
#endif	// STAT__EV_HPP_
