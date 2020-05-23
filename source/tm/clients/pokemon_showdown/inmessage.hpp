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

#include <tm/buffer_view.hpp>

#include <iostream>
#include <string_view>

namespace technicalmachine {
namespace ps {

struct InMessage {
	constexpr InMessage(std::string_view const room, std::string_view const data):
		InMessage(room, BufferView(data, '|'))
	{
	}

	constexpr auto room() const {
		return m_room;
	}
	constexpr auto type() const {
		return m_type;
	}
	constexpr auto next(auto const separator) {
		return m_view.next(separator);
	}
	constexpr auto next() {
		return m_view.next();
	}
	constexpr auto remainder() const {
		return m_view.remainder();
	}

private:
	constexpr InMessage(std::string_view const room, BufferView<std::string_view> view):
		m_room(room),
		m_type([&]{
			// Because messages start with a '|', discard first empty string
			auto const discarded = view.next();
			if (!discarded.empty()) {
				std::cerr << "Expected empty string, got " << discarded << '\n';
			}
			return view.next();
		}()),
		m_view(view)
	{
	}

	std::string_view m_room;
	std::string_view m_type;
	BufferView<std::string_view> m_view;
};

}	// namespace ps
}	// namespace technicalmachine
