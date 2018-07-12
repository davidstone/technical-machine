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

#include "type.hpp"
#include "../operators.hpp"
#include "../rational.hpp"

#include <bounded/integer.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {
struct Pokemon;

struct Effectiveness {
	Effectiveness(Type type, Pokemon const & defender);
	Effectiveness(Type attacking, Type defending);
	auto is_super_effective() const -> bool;
	auto is_not_very_effective() const -> bool;
	auto has_no_effect() const -> bool;

	template<typename Integer>
	auto operator*(Integer const number) const {
		return number * m_effectiveness[0_bi] * m_effectiveness[1_bi];
	}
private:
	Effectiveness(Type attacking, Type defending1, Type defending2);
	using SingleType = rational<bounded::integer<0, 2>, bounded::integer<1, 2>>;
	using Product = decltype(std::declval<SingleType>() * std::declval<SingleType>());
	using container_type = containers::array<SingleType, 2>;

	container_type m_effectiveness;

};

template<typename Integer>
auto operator*(Integer const number, Effectiveness const & effectiveness) {
	return effectiveness * number;
}

}	// namespace technicalmachine
