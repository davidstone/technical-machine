// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.thread_count;

import bounded;

namespace technicalmachine::ps_usage_stats {

export using ThreadCount = bounded::integer<1, 10'000>;

} // namespace technicalmachine::ps_usage_stats
