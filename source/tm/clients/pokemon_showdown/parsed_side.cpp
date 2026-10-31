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
