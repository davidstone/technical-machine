// IVs
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

#ifndef STAT__IV_HPP_
#define STAT__IV_HPP_

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

class IV {
public:
	static constexpr unsigned max = 31u;
	using value_type = bounded::checked_integer<0, max>;
	constexpr explicit IV(value_type iv) noexcept:
		m_value(iv) {
	}
	constexpr auto value() const noexcept -> bounded::integer<0, max> {
		return m_value;
	}
private:
	value_type m_value;
};

}	// namespace technicalmachine
#endif	// STAT__IV_HPP_
