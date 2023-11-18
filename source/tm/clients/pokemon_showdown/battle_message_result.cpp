// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_result;

import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;

import bounded;
import tv;

namespace technicalmachine::ps {

export struct BattleContinues {};
export using BattleResponseSwitch = bounded::integer<1, bounded::normalize<max_pokemon_per_team>>;
export struct BattleResponseError {};
export struct BattleStarted {};
export struct BattleFinished {};

export using BattleMessageResult = tv::variant<
	BattleContinues,
	MoveName,
	BattleResponseSwitch,
	BattleResponseError,
	BattleStarted,
	BattleFinished
>;

} // namespace technicalmachine::ps
