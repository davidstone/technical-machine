// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.split_view;

import tm.split_offsets;

import containers;
import std_module;

namespace technicalmachine {

export constexpr auto split_view(std::string_view const buffer, auto const delimiter) -> std::pair<std::string_view, std::string_view> {
	auto const offsets = split_offsets(buffer, delimiter);
	if (offsets.first == buffer.size()) {
		return std::pair(buffer, std::string_view());
	}
	return std::pair(
		std::string_view(buffer.data(), offsets.first),
		std::string_view(buffer.data() + offsets.first + offsets.discard, buffer.size() - offsets.discard - offsets.first)
	);
}

} // namespace technicalmachine