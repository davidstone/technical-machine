// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.indirect_status_can_apply;

import tm.pokemon.any_pokemon;

import tm.status.blocks_status;
import tm.status.status;
import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.weather;

import containers;

namespace technicalmachine {

export constexpr auto indirect_status_can_apply(StatusName const status, any_active_pokemon auto const target, Weather const weather) {
	return
		is_clear(target.status()) and
		!blocks_status(target.ability(), Ability::Honey_Gather, status, weather) and
		!containers::any(target.types(), [=](Type const type) { return blocks_status(type, status); });
}

} // namespace technicalmachine
