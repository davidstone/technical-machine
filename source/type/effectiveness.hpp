// Effectiveness of a type
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

#ifndef TYPE__EFFECTIVENESS_HPP_
#define TYPE__EFFECTIVENESS_HPP_

#include "type.hpp"
#include "../rational.hpp"

#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
class Pokemon;

class Effectiveness {
public:
	Effectiveness(Type type, Pokemon const & defender);
	Effectiveness(Type::Types attacking, Type::Types defending);
	auto is_super_effective() const -> bool;
	auto is_not_very_effective() const -> bool;
	auto has_no_effect() const -> bool;

private:
	Effectiveness(Type::Types attacking, Type::Types defending1, Type::Types defending2);
	using SingleType = bounded_rational<bounded_integer::native_integer<0, 2>, bounded_integer::native_integer<1, 2>>;
	using Product = decltype(std::declval<SingleType>() * std::declval<SingleType>());
	using container_type = bounded_integer::array<SingleType, 2>;

	container_type m_effectiveness;

	template<typename Integer>
	friend auto operator*(Integer const number, Effectiveness const & e) -> decltype(number * std::declval<container_type>()[0_bi] * std::declval<container_type>()[1_bi]) {
		return number * e.m_effectiveness[0_bi] * e.m_effectiveness[1_bi];
	}
};

template<typename Integer>
auto operator*=(Integer & number, Effectiveness const & effectiveness) {
	return number = number * effectiveness;
}
template<typename Integer>
auto operator*(Effectiveness const & effectiveness, Integer const number) {
	return number * effectiveness;
}

}	// namespace technicalmachine
#endif	// TYPE__EFFECTIVENESS_HPP_
