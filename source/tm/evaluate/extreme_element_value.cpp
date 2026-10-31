module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.evaluate.extreme_element_value;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {

template<containers::range Range>
constexpr auto extreme_element_value(Range && range, auto const compare) {
	BOUNDED_ASSERT(!containers::is_empty(range));
	auto first = containers::begin(range);
	auto const last = containers::end(range);
	// Could be rewritten to use `uninitialized<T>`, but it's trickier
	auto storage = tv::optional<containers::range_reference_t<Range>>(*first);
	++first;
	for (; first != last; ++first) {
		auto && value = *first;
		if (compare(value, *storage)) {
			storage.emplace(bounded::value_to_function(OPERATORS_FORWARD(value)));
		}
	}
	return *std::move(storage);
}

export template<containers::range Range>
constexpr auto max_element_value(Range && range) {
	return extreme_element_value(OPERATORS_FORWARD(range), std::greater());
}

export template<containers::range Range>
constexpr auto min_element_value(Range && range) {
	return extreme_element_value(OPERATORS_FORWARD(range), std::less());
}

} // namespace technicalmachine
