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

#include <tm/type/type.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Pokemon;

struct Effectiveness {
	Effectiveness(Generation generation, Type type, Pokemon const & defender);
	Effectiveness(Generation generation, Type attacking, Type defending);
	auto is_super_effective() const -> bool;
	auto is_not_very_effective() const -> bool;
	auto has_no_effect() const -> bool;

private:
	auto product() const {
		return m_first * m_second;
	}

	friend auto operator*(Effectiveness const lhs, auto const rhs) {
		return rhs * lhs.product();
	}

	Effectiveness(Generation generation, Type attacking, Type defending1, Type defending2);

	using SingleType = rational<bounded::integer<0, 2>, bounded::integer<1, 2>>;
	SingleType m_first;
	SingleType m_second;
};

auto operator*(auto const lhs, Effectiveness const rhs) {
	return rhs * lhs;
}

}	// namespace technicalmachine
