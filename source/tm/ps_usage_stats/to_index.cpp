// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.to_index;

import bounded;
import numeric_traits;

namespace technicalmachine::ps_usage_stats {

export template<typename T>
constexpr auto to_index(T const value) {
	return bounded::integer(value) - bounded::constant<numeric_traits::min_value<T>>;
}

} // namespace technicalmachine::ps_usage_stats
