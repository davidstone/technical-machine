// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto parse_pokemon_identity(std::string_view const str) {
	struct result {
		Party party;
		Nickname nickname;
	};
	auto const [party_str, nickname] = split_view(str, ": "_s);
	return result{make_party(party_str.substr(0, 2)), Nickname(nickname)};
}

} // namespace technicalmachine::ps
