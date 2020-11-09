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

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct IV {
	static constexpr auto max = 31;
	using value_type = bounded::integer<0, max>;
	constexpr explicit IV(value_type const iv):
		m_value(iv) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator==(IV const &, IV const &) -> bool = default;
private:
	value_type m_value;
};

constexpr auto default_iv(Generation const generation) {
	return generation <= Generation::two ? IV(30_bi) : IV(31_bi);
}

}	// namespace technicalmachine
