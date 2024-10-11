// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.test.stat_names;

import tm.stat.stat_names;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.stat_names;

namespace technicalmachine {

static_assert(test_enum_string<SpecialPermanentStat>());
static_assert(test_enum_string<SplitSpecialPermanentStat>());
static_assert(test_enum_string<SpecialRegularStat>());
static_assert(test_enum_string<SplitSpecialRegularStat>());

} // namespace technicalmachine
