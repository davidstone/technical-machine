// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_status;

import tm.status.status_name;

import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export constexpr auto parse_status(std::string_view const str) -> StatusName {
	return
		(str == ""sv) ? StatusName::clear :
		(str == "fnt"sv) ? StatusName::clear :
		(str == "brn"sv) ? StatusName::burn :
		(str == "frz"sv) ? StatusName::freeze :
		(str == "par"sv) ? StatusName::paralysis :
		(str == "psn"sv) ? StatusName::poison :
		(str == "tox"sv) ? StatusName::toxic :
		(str == "slp"sv) ? StatusName::sleep :
//		(str == ""sv) ? StatusName::rest :
		throw std::runtime_error("Received an invalid status");
}

} // namespace technicalmachine::ps
