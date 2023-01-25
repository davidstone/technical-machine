// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.find_required_index;

import tm.string_conversions.move_name;
import tm.string_conversions.species;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;

export template<containers::range Range>
constexpr auto find_required_index(Range const & range, containers::range_value_t<Range> const & value) -> containers::index_type<Range> {
	auto const it = containers::find(range, value);
	if (it == containers::end(range)) {
		throw std::runtime_error(containers::concatenate<std::string>("Unable to find "sv, to_string(value)));
	}
	return bounded::assume_in_range<containers::index_type<Range>>(it - containers::begin(range));
}

} // namespace technicalmachine