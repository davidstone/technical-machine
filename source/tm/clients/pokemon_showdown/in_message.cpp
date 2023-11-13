// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.in_message;

import tm.buffer_view;

import containers;
import std_module;

namespace technicalmachine::ps {

using namespace std::string_view_literals;

export struct InMessage {
	constexpr InMessage(std::string_view const room, std::string_view const data):
		InMessage(room, DelimitedBufferView(data, '|'))
	{
	}

	constexpr auto room() const {
		return m_room;
	}
	constexpr auto type() const {
		return m_type;
	}
	constexpr auto pop(auto const separator) {
		return m_view.pop(separator);
	}
	constexpr auto pop() {
		return m_view.pop();
	}
	constexpr auto remainder() const {
		return m_view.remainder();
	}

private:
	constexpr InMessage(std::string_view const room, DelimitedBufferView<std::string_view> view):
		m_room(room),
		m_type([&] {
			// Because messages start with a '|', discard first empty string
			auto const discarded = view.pop();
			if (!discarded.empty()) {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Expected empty string, got "sv,
					discarded
				));
			}
			return view.pop();
		}()),
		m_view(view)
	{
	}

	std::string_view m_room;
	std::string_view m_type;
	DelimitedBufferView<std::string_view> m_view;
};

} // namespace technicalmachine::ps
