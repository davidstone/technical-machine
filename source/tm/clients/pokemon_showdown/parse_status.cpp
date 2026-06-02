// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_status;

import tm.status.status_name;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

export constexpr auto parse_status(std::string_view const str) -> StatusName {
	return
		(str == ""_s) ? StatusName::clear :
		(str == "fnt"_s) ? StatusName::clear :
		(str == "brn"_s) ? StatusName::burn :
		(str == "frz"_s) ? StatusName::freeze :
		(str == "par"_s) ? StatusName::paralysis :
		(str == "psn"_s) ? StatusName::poison :
		(str == "tox"_s) ? StatusName::toxic :
		(str == "slp"_s) ? StatusName::sleep :
//		(str == ""_s) ? StatusName::rest :
		throw std::runtime_error("Received an invalid status");
}

} // namespace technicalmachine::ps
