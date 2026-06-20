// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.split_offsets;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct split_offsets {
	template<typename View>
	constexpr split_offsets(View const buffer, containers::range_value_t<View> const delimiter):
		first(bounded::increase_min<0>(containers::find(buffer, delimiter) - containers::begin(buffer))),
		discard(1_bi)
	{
	}

	template<typename View, typename Delimiter> requires std::same_as<containers::range_value_t<View>, containers::range_value_t<Delimiter>>
	constexpr split_offsets(View const buffer, Delimiter const delimiter):
		first(bounded::increase_min<0>(std::search(
			containers::make_legacy_iterator(containers::begin(buffer)),
			containers::make_legacy_iterator(containers::end(buffer)),
			containers::make_legacy_iterator(containers::begin(delimiter)),
			containers::make_legacy_iterator(containers::end(delimiter))
		).base() - containers::begin(buffer))),
		discard(containers::size(delimiter))
	{
	}

	containers::array_size_type<char> first;
	containers::array_size_type<char> discard;
};

} // namespace technicalmachine