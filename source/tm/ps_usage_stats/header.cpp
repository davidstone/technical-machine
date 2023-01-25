// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.header;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

export constexpr auto usage_stats_magic_string = containers::array{
	std::byte('t'),
	std::byte('m'),
	std::byte(' '),
	std::byte('u'),
	std::byte('s'),
	std::byte('a'),
	std::byte('g'),
	std::byte('e'),
	std::byte(' '),
	std::byte('s'),
	std::byte('t'),
	std::byte('a'),
	std::byte('t'),
	std::byte('s')
};

export using UsageStatsVersion = bounded::integer<0, 65535>;

// Regieleki with a Choice Scarf
export using InitialSpeed = bounded::integer<0, bounded::normalize<822>>;

} // namespace technicalmachine
