// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/buffer_view.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {

using namespace std::string_view_literals;

struct InMessage {
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

} // namespace ps
} // namespace technicalmachine
