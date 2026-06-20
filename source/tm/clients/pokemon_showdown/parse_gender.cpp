// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_gender;

import tm.gender;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

export constexpr auto parse_gender(containers::string_view const str) -> Gender {
	return
		str == "F"_s ? Gender::female :
		str == "M"_s ? Gender::male :
		Gender::genderless;
}

} // namespace technicalmachine::ps
