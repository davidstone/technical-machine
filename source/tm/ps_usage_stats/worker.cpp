// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/forward.hpp>

export module tm.ps_usage_stats.worker;

import concurrent_queue;
import std_module;

namespace technicalmachine::ps_usage_stats {

template<typename T, typename Function>
struct Worker {
	Worker(Function function):
		m_function(std::move(function)),
		m_queue(1000),
		m_thread([&](std::stop_token token) {
			while (!token.stop_requested()) {
				for (auto const & result : m_queue.pop_all(token)) {
					m_function(result);
				}
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
	Function m_function;
	concurrent::blocking_queue<T> m_queue;
	std::jthread m_thread;
};

export template<typename T, typename Function>
auto make_worker(Function function) {
	return Worker<T, Function>(std::move(function));
}

} // namespace technicalmachine::ps_usage_stats
