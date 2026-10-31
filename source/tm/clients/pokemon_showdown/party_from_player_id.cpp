export module tm.clients.ps.party_from_player_id;

import tm.clients.ps.parse_pokemon_identity;

import tm.clients.party;

import containers;
import std_module;

namespace technicalmachine::ps {

export constexpr auto party_from_player_id(containers::string_view const player_id) -> Party {
	return parse_pokemon_identity(player_id).party;
}

} // namespace technicalmachine::ps
