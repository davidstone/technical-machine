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

export constexpr auto parse_status(containers::string_view const str) -> StatusName {
	return
		(str == ""_sv) ? StatusName::clear :
		(str == "fnt"_sv) ? StatusName::clear :
		(str == "brn"_sv) ? StatusName::burn :
		(str == "frz"_sv) ? StatusName::freeze :
		(str == "par"_sv) ? StatusName::paralysis :
		(str == "psn"_sv) ? StatusName::poison :
		(str == "tox"_sv) ? StatusName::toxic :
		(str == "slp"_sv) ? StatusName::sleep :
//		(str == ""_sv) ? StatusName::rest :
		throw std::runtime_error("Received an invalid status");
}

} // namespace technicalmachine::ps
