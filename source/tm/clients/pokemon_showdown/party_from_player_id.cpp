// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.party_from_player_id;

import tm.clients.ps.parse_pokemon_identity;

import tm.clients.party;

import std_module;

namespace technicalmachine::ps {

export constexpr auto party_from_player_id(std::string_view const player_id) -> Party {
	return parse_pokemon_identity(player_id).party;
}

} // namespace technicalmachine::ps
