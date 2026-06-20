// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.in_message;

import tm.buffer_view;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

export struct InMessage {
	constexpr InMessage(containers::string_view const data):
		InMessage(DelimitedBufferView(data, '|'))
	{
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
	constexpr InMessage(DelimitedBufferView<containers::string_view> view):
		m_type([&] {
			// Because messages start with a '|', discard first empty string
			auto const discarded = view.pop();
			if (!containers::is_empty(discarded)) {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Expected empty string, got "_s,
					discarded
				));
			}
			return view.pop();
		}()),
		m_view(view)
	{
	}

	containers::string_view m_type;
	DelimitedBufferView<containers::string_view> m_view;
};

} // namespace technicalmachine::ps
