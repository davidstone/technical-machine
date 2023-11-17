// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.split_offsets;

import containers;
import std_module;

namespace technicalmachine {

export struct split_offsets {
	template<typename View>
	constexpr split_offsets(View const buffer, containers::range_value_t<View> const delimiter):
		first(static_cast<std::size_t>(containers::find(buffer, delimiter) - containers::begin(buffer))),
		discard(1U)
	{
	}

	template<typename View, typename Delimiter> requires std::same_as<containers::range_value_t<View>, containers::range_value_t<Delimiter>>
	constexpr split_offsets(View const buffer, Delimiter const delimiter):
		first(static_cast<std::size_t>(std::search(buffer.begin(), buffer.end(), delimiter.begin(), delimiter.end()) - buffer.begin())),
		discard(delimiter.size())
	{
	}

	std::size_t first;
	std::size_t discard;
};

} // namespace technicalmachine