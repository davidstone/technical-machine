// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.add_to_workers;

import containers;

namespace technicalmachine::ps_usage_stats {

export auto add_to_workers(auto && inputs, auto & workers) {
	auto it = containers::begin(inputs);
	auto const last = containers::end(inputs);
	while (true) {
		for (auto & worker : workers) {
			if (it == last) {
				break;
			}
			worker.add_work(*it);
			++it;
		}
		if (it == last) {
			break;
		}
	}
}

} // namespace technicalmachine::ps_usage_stats
