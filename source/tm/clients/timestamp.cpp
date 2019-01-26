// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
