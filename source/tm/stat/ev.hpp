// EVs
// Copyright (C) 2018 David Stone
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

#include <bounded/detail/conditional.hpp>
#include <bounded/detail/construct_destroy.hpp>
#include <bounded/integer.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct EV {
	static constexpr auto max = 252_bi;
	using value_type = bounded::integer<0, static_cast<int>(max)>;

	constexpr explicit EV(value_type evs):
		m_value(evs) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator<=>(EV const &, EV const &) = default;

private:
	value_type m_value;
};

constexpr auto operator<=>(EV const lhs, EV::value_type const rhs) {
	return lhs.value() <=> rhs;
}
constexpr auto operator==(EV const lhs, EV::value_type const rhs) -> bool {
	return lhs.value() == rhs;
}

constexpr auto ev_range(auto const max) {
	return containers::transform(containers::inclusive_integer_range(0_bi, max, 4_bi), bounded::construct_return<EV>);
}

constexpr auto ev_range() {
	return ev_range(EV::max);
}

constexpr auto max_total_evs(Generation const generation) {
	return
		BOUNDED_CONDITIONAL(generation <= Generation::two, 252_bi * 6_bi,
		508_bi
	);
}

}	// namespace technicalmachine
