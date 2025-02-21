// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.make_party;

import tm.clients.party;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

export constexpr auto make_party(std::string_view const party_str) -> Party {
	if (party_str == "p1"sv) {
		return Party(0_bi);
	} else if (party_str == "p2"sv) {
		return Party(1_bi);
	} else {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid player id: "sv, party_str));
	}
}

} // namespace technicalmachine::ps
