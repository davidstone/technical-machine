// Iterate over all values of an enum
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

#include "hash.hpp"
#include "operators.hpp"

#include <bounded/integer_range.hpp>

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace technicalmachine {

using namespace bounded::literal;

template<typename Enum>
struct enum_iterator {
private:
	using base = typename bounded::integer_range_type<bounded::equivalent_type<Enum>>::const_iterator;
public:
	using index_type = typename base::index_type;
	using value_type = Enum;
	using difference_type = typename base::difference_type;
	using iterator_category = typename base::iterator_category;
	
	explicit constexpr enum_iterator(Enum const e) noexcept:
		m_it(e)  {
	}

	constexpr auto operator*() const noexcept {
		return static_cast<Enum>(*m_it);
	}
	constexpr auto operator[](index_type const index) const {
		return static_cast<Enum>(m_it[index]);
	}
	constexpr auto operator->() const noexcept {
		return m_it.operator->();
	}
	
	friend constexpr auto operator-(enum_iterator const lhs, difference_type const rhs) {
		return enum_iterator(lhs.m_it - rhs);
	}
	
	friend constexpr auto operator+(enum_iterator const lhs, difference_type const rhs) {
		return enum_iterator(lhs.m_it + rhs);
	}
	
	friend constexpr auto operator==(enum_iterator const lhs, enum_iterator const rhs) noexcept {
		return lhs.m_it == rhs.m_it;
	}
	friend constexpr auto operator<(enum_iterator const lhs, enum_iterator const rhs) noexcept {
		return lhs.m_it < rhs.m_it;
	}

private:
	explicit constexpr enum_iterator(base it) noexcept:
		m_it(std::move(it)) {
	}
	base m_it;
};

template<typename Enum, Enum end = Enum::END>
constexpr auto enum_range = bounded::range_type<enum_iterator<Enum>>(static_cast<Enum>(0), end);

}	// namespace technicalmachine
