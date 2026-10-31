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
