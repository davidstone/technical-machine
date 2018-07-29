// Pokemon Showdown incoming messages
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

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {
namespace detail {

static constexpr auto split(std::string_view str, char const token) {
	auto const index = str.find(token);
	if (index == std::string_view::npos) {
		return std::pair(str, std::string_view{});
	}
	return std::pair(str.substr(0, index), str.substr(index + 1));
}

}	// namespace detail

// TODO: Maybe something with iterators?
struct BufferView {
	constexpr BufferView(std::string_view buffer, char const token):
		m_buffer(buffer),
		m_token(token)
	{
	}
	
	constexpr auto next() -> std::string_view {
		auto const [first, second] = detail::split(m_buffer, m_token);
		m_buffer = second;
		return first;
	}
	constexpr auto remainder() const {
		return m_buffer;
	}
	
private:
	std::string_view m_buffer;
	char m_token;
};

struct InMessage {
	explicit constexpr InMessage(BufferView view):
		m_room(view.next()),
		m_type(view.next()),
		m_view(view)
	{
	}

	constexpr auto room() const noexcept {
		return m_room;
	}
	constexpr auto type() const noexcept {
		return m_type;
	}
	constexpr auto next() {
		return m_view.next();
	}
	constexpr auto remainder() const noexcept {
		return m_view.remainder();
	}

private:
	std::string_view m_room;
	std::string_view m_type;
	BufferView m_view;
};

}	// namespace ps
}	// namespace technicalmachine
