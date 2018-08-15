// Range struct
// Copyright (C) 2016 David Stone
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

#include <iterator>
#include <utility>

namespace technicalmachine {

template<typename Iterator, typename Sentinel = Iterator>
struct range_view {
	using value_type = typename std::iterator_traits<Iterator>::value_type;

	using size_type = bounded::integer<
		0,
		static_cast<std::uintmax_t>(std::numeric_limits<typename std::iterator_traits<Iterator>::difference_type>::max())
	>;
	
	using const_iterator = Iterator;
	using iterator = Iterator;

	constexpr range_view(Iterator first, Sentinel last):
		m_begin(first),
		m_end(last)
	{
	}
	constexpr explicit range_view(std::pair<Iterator, Sentinel> pair):
		range_view(pair.first, pair.second)
	{
	}
	
	constexpr Iterator begin() const {
		return m_begin;
	}
	constexpr Sentinel end() const {
		return m_end;
	}
	
private:
	Iterator m_begin;
	Sentinel m_end;
};

}	// namespace technicalmachine
