export module tm.clients.ps.parse_pokemon_identity;

import tm.clients.ps.make_party;

import tm.clients.party;

import tm.pokemon.nickname;

import tm.split_view;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace containers::string_literals;

export constexpr auto parse_pokemon_identity(containers::string_view const str) {
	struct result {
		Party party;
		Nickname nickname;
	};
	auto const [party_str, nickname] = split_view(str, ": "_sv);
	return result{make_party(containers::take(party_str, 2_bi)), Nickname(nickname)};
}

} // namespace technicalmachine::ps
