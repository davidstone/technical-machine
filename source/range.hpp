// Range struct
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

#ifndef TECHNICALMACHINE_RANGE_HPP_
#define TECHNICALMACHINE_RANGE_HPP_

#include <utility>

namespace technicalmachine {

template<typename Iterator>
struct Range {
	using iterator = Iterator;
	constexpr Range(iterator first, iterator last):
		m_first(std::move(first)),
		m_last(std::move(last)) {
	}
	constexpr auto begin() const {
		return m_first;
	}
	constexpr auto end() const {
		return m_last;
	}
private:
	iterator m_first;
	iterator m_last;
};

template<typename Iterator>
constexpr auto size(Range<Iterator> const & range) {
	return end(range) - begin(range);
}

template<typename Iterator>
constexpr auto make_range(Iterator first, Iterator last) {
	return Range<Iterator>(first, last);
}

}	// namespace technicalmachine
#endif	// TECHNICALMACHINE_RANGE_HPP_
