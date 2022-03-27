// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/battle_result.hpp>

#include <bounded/optional.hpp>

#include <filesystem>

namespace technicalmachine::ps_usage_stats {

auto parse_log(std::filesystem::path const & path) -> bounded::optional<BattleResult>;

} // namespace technicalmachine::ps_usage_stats
