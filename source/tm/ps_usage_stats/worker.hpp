// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/thread/scoped_thread.hpp>

#include <concurrent/queue.hpp>

#include <utility>

namespace technicalmachine::ps_usage_stats {

template<typename T, typename Function>
struct Worker {
	Worker(Function function):
		m_function(std::move(function)),
		m_queue(1000),
		m_thread([&] {
			while (true) {
				for (auto const & result : m_queue.pop_all()) {
					m_function(result);
				}
			}
		})
	{
	}
	~Worker() {
		for (auto const & result : m_queue.try_pop_all()) {
			m_function(result);
		}
	}
	auto add_work(auto && value) -> void {
		m_queue.push(OPERATORS_FORWARD(value));
	}

private:
	Function m_function;
	concurrent::blocking_queue<T> m_queue;
	boost::scoped_thread<boost::interrupt_and_join_if_joinable> m_thread;
};

template<typename T, typename Function>
auto make_worker(Function function) {
	return Worker<T, Function>(std::move(function));
}

} // namespace technicalmachine::ps_usage_stats
