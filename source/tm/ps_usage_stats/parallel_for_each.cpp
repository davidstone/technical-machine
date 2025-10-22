// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/forward.hpp>

export module tm.ps_usage_stats.parallel_for_each;

import tm.ps_usage_stats.thread_count;

import concurrent;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {

template<typename T, typename Function>
struct Worker {
	Worker(Function function):
		m_function(std::move(function)),
		m_queue(1000),
		m_thread([&](std::stop_token token) {
			auto buffer = typename Queue::container_type();
			while (!token.stop_requested()) {
				buffer = m_queue.pop_all(token, std::move(buffer));
				for (auto const & result : buffer) {
					m_function(result);
				}
				buffer.clear();
			}
			for (auto const & result : m_queue.try_pop_all()) {
				m_function(result);
			}
		})
	{
	}
	auto add_work(auto && value) -> void {
		m_queue.push(OPERATORS_FORWARD(value));
	}

private:
	[[no_unique_address]] Function m_function;
	using Queue = concurrent::blocking_queue<T>;
	Queue m_queue;
	std::jthread m_thread;
};

export template<containers::range Range, typename Function>
auto parallel_for_each(
	ThreadCount const thread_count,
	Range && inputs,
	Function function
) -> void {
	auto it = containers::begin(inputs);
	auto const last = containers::end(inputs);
	if (it == last) {
		return;
	}

	using value_type = containers::range_value_t<Range>;
	auto workers = containers::dynamic_array(
		containers::generate_n(
			thread_count,
			[&] { return Worker<value_type, Function>(function); }
		)
	);

	while (true) {
		for (auto & worker : workers) {
			worker.add_work(*it);
			++it;
			if (it == last) {
				return;
			}
		}
	}
}

} // namespace technicalmachine::ps_usage_stats
