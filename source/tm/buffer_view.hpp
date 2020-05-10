// Pokemon Showdown incoming messages
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

#include <string_view>

namespace technicalmachine {

constexpr auto separator_size(char) {
	return 1U;
}
constexpr auto separator_size(std::string_view const separator) {
	return separator.size();
}

constexpr auto split(std::string_view str, auto const separator) {
	auto const index = str.find(separator);
	if (index == std::string_view::npos) {
		return std::pair(str, std::string_view{});
	}
	return std::pair(str.substr(0, index), str.substr(index + separator_size(separator)));
}

// TODO: Maybe something with iterators?
struct BufferViewBase {
	explicit constexpr BufferViewBase(std::string_view buffer):
		m_buffer(buffer)
	{
	}
	
	constexpr auto next(auto const separator) -> std::string_view {
		auto const [first, second] = split(m_buffer, separator);
		m_buffer = second;
		return first;
	}
	constexpr auto remainder() const {
		return m_buffer;
	}
	
private:
	std::string_view m_buffer;
};

template<typename Separator>
struct BufferView {
	static_assert(std::is_same_v<Separator, char> or std::is_same_v<Separator, std::string_view>);

	constexpr BufferView(std::string_view buffer, Separator const separator):
		m_buffer(buffer),
		m_separator(separator)
	{
	}
	
	constexpr auto next(auto const separator) -> std::string_view {
		return m_buffer.next(separator);
	}
	constexpr auto next() -> std::string_view {
		return next(m_separator);
	}
	constexpr auto remainder() const {
		return m_buffer.remainder();
	}
	
private:
	BufferViewBase m_buffer;
	Separator m_separator;
};

BufferView(std::string_view, char const *) -> BufferView<std::string_view>;

} // namespace technicalmachine
