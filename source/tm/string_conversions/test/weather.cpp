export module tm.string_conversions.test.weather;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.weather;

import tm.weather;

namespace technicalmachine {

static_assert(test_enum_string<Weather>());

} // namespace technicalmachine
