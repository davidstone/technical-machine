export module tm.ps_usage_stats.thread_count;

import bounded;
import containers;

namespace technicalmachine::ps_usage_stats {

export using ThreadCount = bounded::integer<1, 10'000>;
export using ThreadIndex = containers::to_index_type<ThreadCount>;

} // namespace technicalmachine::ps_usage_stats
