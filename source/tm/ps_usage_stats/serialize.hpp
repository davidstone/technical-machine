// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <ostream>

namespace technicalmachine::ps_usage_stats {

struct UsageStats;
struct Correlations;

auto serialize(std::ostream &, Generation, UsageStats const & usage_stats, Correlations const & correlations) -> void;

} // namespace technicalmachine::ps_usage_stats
