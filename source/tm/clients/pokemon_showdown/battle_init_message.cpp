// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_init_message;

import tm.clients.ps.switch_message;

import tm.clients.party;

import tm.pokemon.max_pokemon_per_team;

import tm.generation;

import bounded;
import containers;

namespace technicalmachine::ps {
using namespace bounded::literal;

export struct BattleInitMessage {
	struct Team {
		[[no_unique_address]] SwitchMessage starter;
		[[no_unique_address]] TeamSize size;
	};
	Generation generation;
	containers::array<Team, 2_bi> team;
};

} // namespace technicalmachine::ps
