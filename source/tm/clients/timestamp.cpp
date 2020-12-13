// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/timestamp.hpp>

#include <cstring>
#include <ctime>
#include <mutex>

namespace technicalmachine {
namespace {

std::tm thread_safe_localtime(std::time_t const time) {
	static auto mutex = std::mutex{};
	auto lock = std::lock_guard(mutex);
	return *std::localtime(std::addressof(time));
}

}	// namespace

std::string timestamp() {
	std::string result;
	result.resize(std::strlen("2000-01-01 12:34:56"));
	auto const time = thread_safe_localtime(std::time(nullptr));
	std::strftime(result.data(), result.size(), "%Y-%m-%d %H:%M:%S", std::addressof(time));
	return result;
}

}	// namespace technicalmachine
