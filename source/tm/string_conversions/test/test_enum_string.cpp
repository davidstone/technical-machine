module;

#include <bounded/assert.hpp>

export module tm.string_conversions.test.test_enum_string;

import tm.string_conversions.from_string;

import containers;

namespace technicalmachine {

export template<typename Enum>
constexpr auto test_enum_string() -> bool {
	for (auto const original : containers::enum_range<Enum>()) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		BOUNDED_ASSERT(original == result);
	}
	return true;
}

} // namespace technicalmachine
