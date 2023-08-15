// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.status_can_apply;

import tm.pokemon.any_pokemon;
import tm.pokemon.is_type;

import tm.status.blocks_status;
import tm.status.status;
import tm.status.status_name;
import tm.status.team_has_status;

import tm.any_team;
import tm.environment;

namespace technicalmachine {

constexpr auto status_is_clausable(StatusName const status) {
	switch (status) {
		case StatusName::burn:
		case StatusName::paralysis:
		case StatusName::poison:
		case StatusName::toxic:
		case StatusName::rest:
		case StatusName::clear:
			return false;
		case StatusName::freeze:
		case StatusName::sleep:
			return true;
	}
}

export constexpr auto status_can_apply_ignoring_current_status(StatusName const status, any_active_pokemon auto const user, any_team auto const & target, Environment const environment, auto const... immune_types) {
	auto const target_pokemon = target.pokemon();
	return
		!blocks_status(target_pokemon.ability(), user.ability(), status, environment) and
		(... and !is_type(target_pokemon, immune_types)) and
		(!status_is_clausable(status) or !team_has_status(target, status)) and
		(status != StatusName::sleep or (!user.last_used_move().is_uproaring() and !target_pokemon.last_used_move().is_uproaring()));
}

export constexpr auto status_can_apply(StatusName const status, any_active_pokemon auto const user, any_team auto const & target, Environment const environment, auto const... immune_types) {
	return
		is_clear(target.pokemon().status()) and
		status_can_apply_ignoring_current_status(status, user, target, environment, immune_types...);
}

} // namespace technicalmachine
