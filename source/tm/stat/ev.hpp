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

#include <bounded/integer.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct EV {
	static constexpr auto max = 252_bi;
	static constexpr auto max_total = 508_bi;
	using value_type = bounded::checked_integer<0, static_cast<int>(max)>;
	using total_type = bounded::checked_integer<0, static_cast<int>(max_total)>;

	constexpr explicit EV(value_type evs):
		m_value(evs) {
	}
	constexpr auto value() const -> bounded::integer<0, static_cast<int>(max)> {
		return m_value;
	}
private:
	bounded::clamped_integer<0, static_cast<int>(max)> m_value;
};

constexpr auto compare(EV const lhs, EV const rhs) {
	return compare(lhs.value(), rhs.value());
}
constexpr auto compare(EV const lhs, EV::value_type const rhs) {
	return compare(lhs.value(), rhs);
}
constexpr auto compare(EV::value_type const lhs, EV const rhs) {
	return compare(lhs, rhs.value());
}
constexpr auto operator==(EV const lhs, EV const rhs) {
	return lhs.value() == rhs.value();
}
constexpr auto operator==(EV const lhs, EV::value_type const rhs) {
	return lhs.value() == rhs;
}
constexpr auto operator==(EV::value_type const lhs, EV const rhs) {
	return lhs == rhs.value();
}

template<typename Max>
constexpr auto ev_range(Max const max) {
	return containers::transform(containers::inclusive_integer_range(0_bi, max, 4_bi), bounded::construct_return<EV>);
}

constexpr auto ev_range() {
	return ev_range(EV::max);
}

}	// namespace technicalmachine
