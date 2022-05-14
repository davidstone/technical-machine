// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <string_view>

namespace technicalmachine {

inline constexpr auto usage_stats_magic_string = std::string_view("tm usage stats");

using UsageStatsVersion = bounded::integer<0, 65535>;

// Regieleki with a Choice Scarf
using InitialSpeed = bounded::integer<0, bounded::normalize<822>>;

} // namespace technicalmachine
