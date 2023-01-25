// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.result;

import std_module;

namespace technicalmachine {

export enum class Result {
	won = 1,
	lost = -1,
	tied = 0
};

export constexpr auto to_string(Result const result) -> std::string_view {
	switch (result) {
		case Result::won: return "Won";
		case Result::lost: return "Lost";
		case Result::tied: return "Tied";
	}
}

} // namespace technicalmachine
