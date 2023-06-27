// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.team_has_status;

import tm.status.status_name;

import tm.any_team;

import containers;

namespace technicalmachine {

export constexpr auto team_has_status(any_team auto const & target, StatusName const status) {
	return containers::any(target.all_pokemon(), [=](auto const & pokemon) {
		return pokemon.status().name() == status;
	});
}

} // namespace technicalmachine
