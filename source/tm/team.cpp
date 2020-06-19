// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/team.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {
namespace {

auto status_clause_applies(Team const & target, Statuses const status) {
	switch (status) {
		case Statuses::freeze:
		case Statuses::sleep:
			return containers::any(target.all_pokemon(), [=](auto const & pokemon) {
				return pokemon.status().name() == status;
			});
		default:
			return false;
	}
}

} // namespace

auto apply_status(Generation const generation, Statuses const status, Team & target, MutableActivePokemon user, Weather const weather, bool uproar) -> void {
	if (status_clause_applies(target, status)) {
		return;
	}
	target.pokemon().apply_status(generation, status, user, weather, uproar);
}

} // namespace technicalmachine