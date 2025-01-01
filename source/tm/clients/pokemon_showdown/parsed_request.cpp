// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_request;

import tm.clients.ps.parsed_side;
import tm.clients.ps.parsed_team;

import tm.clients.party;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.pp;

import containers;
import std_module;

namespace technicalmachine::ps {

export struct ParsedMove {
	MoveName move;
	PP::current_type pp;
	bool enabled;

	friend auto operator==(ParsedMove, ParsedMove) -> bool = default;
};

export using ParsedMoves = containers::static_vector<ParsedMove, max_moves_per_pokemon>;

export enum class SwitchPossibilities {
	maybe_trapped,
	trapped,
	forced,
	allowed
};

export struct ParsedRequest {
	ParsedMoves moves;
	SwitchPossibilities switches;
	Party party;
	ParsedTeam team;
	friend auto operator==(ParsedRequest, ParsedRequest) -> bool = default;
};

export constexpr auto parsed_side_to_request(ParsedSide side) -> ParsedRequest {
	return ParsedRequest(
		ParsedMoves(),
		SwitchPossibilities::maybe_trapped,
		std::move(side).party,
		std::move(side).team,
	);
}

} // namespace technicalmachine::ps
