// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.test.gender;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.gender;

import tm.gender;

namespace technicalmachine {

static_assert(test_enum_string<Gender>());

} // namespace technicalmachine