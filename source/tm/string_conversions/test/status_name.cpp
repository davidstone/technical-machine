export module tm.string_conversions.test.status_name;

import tm.status.status_name;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.status_name;

namespace technicalmachine {

static_assert(test_enum_string<StatusName>());

} // namespace technicalmachine
