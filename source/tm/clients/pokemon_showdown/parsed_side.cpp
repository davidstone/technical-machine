// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_side;

import tm.clients.ps.parsed_team;

import tm.clients.party;

namespace technicalmachine::ps {

export struct ParsedSide {
	Party party;
	ParsedTeam team;
	friend auto operator==(ParsedSide, ParsedSide) -> bool = default;
};

} // namespace technicalmachine::ps
