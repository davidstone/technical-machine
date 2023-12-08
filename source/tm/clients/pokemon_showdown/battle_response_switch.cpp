// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_response_switch;

import tm.pokemon.max_pokemon_per_team;

import bounded;

namespace technicalmachine::ps {

export using BattleResponseSwitch = bounded::integer<1, bounded::normalize<max_pokemon_per_team>>;

} // namespace technicalmachine::ps
