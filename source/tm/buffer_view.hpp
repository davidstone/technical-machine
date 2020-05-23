// Copyright (C) 2020 David Stone
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

#include <containers/algorithms/find.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace technicalmachine {

template<typename View>
constexpr auto split_impl(View buffer, std::size_t separator_size, auto const position) {
	if (position == buffer.end()) {
		return std::pair(buffer, View());
	}
	auto const offset = static_cast<std::size_t>(position - buffer.begin());
	return std::pair(
		View(buffer.data(), offset),
		View(buffer.data() + offset + separator_size, buffer.size() - separator_size - offset)
	);
}

template<typename View>
constexpr auto split(View const buffer, typename View::value_type const separator) {
	return split_impl(buffer, 1U, containers::find(buffer, separator));
}

template<typename View, typename Separator> requires std::is_same_v<typename View::value_type, typename Separator::value_type>
constexpr auto split(View const buffer, Separator const separator) {
	return split_impl(buffer, separator.size(), std::search(buffer.begin(), buffer.end(), separator.begin(), separator.end()));
}

// TODO: Maybe something with iterators?
template<typename View>
struct BufferView {
	static_assert(std::is_nothrow_copy_constructible_v<View>);

	explicit constexpr BufferView(View const buffer):
		m_buffer(buffer)
	{
	}
	
	constexpr auto next(auto const separator) {
		auto const [first, second] = split(m_buffer, separator);
		m_buffer = second;
		return first;
	}
	constexpr auto remainder() const {
		return m_buffer;
	}
	
private:
	View m_buffer;
};

template<typename View, typename Separator = typename View::value_type>
struct DelimitedBufferView {
	static_assert(std::is_nothrow_copy_constructible_v<View>);

	constexpr DelimitedBufferView(View const buffer, Separator const separator):
		m_buffer(buffer),
		m_separator(separator)
	{
	}
	
	constexpr auto next(auto const separator) {
		return m_buffer.next(separator);
	}
	constexpr auto next() {
		return next(m_separator);
	}
	constexpr auto remainder() const {
		return m_buffer.remainder();
	}
	
private:
	BufferView<View> m_buffer;
	Separator m_separator;
};

} // namespace technicalmachine
