// Keep track of changes in HP to make sure TM has the same view as the server
// Copyright (C) 2012 David Stone
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

#include "updated_hp.hpp"
#include <cassert>
#include "../pokemon/pokemon.hpp"
#include "../team.hpp"

namespace technicalmachine {

UpdatedHP::UpdatedHP(Team const & team) {
	for (auto const & pokemon : team.all_pokemon()) {
		add(team.is_me(), pokemon, pokemon.hp().max);
	}
}

void UpdatedHP::reset_between_turns() {
	for (auto & value : container) {
		value.second.second = 0;
	}
}

void UpdatedHP::add(bool const is_me, Pokemon const & pokemon, unsigned const max_precision) {
	auto const key = std::make_pair(is_me, pokemon.name());
	auto const mapped = std::make_pair(max_precision, 0);
	auto const result = container.insert(std::make_pair(key, mapped));
	// This implementation only works if Species Clause is in effect
	assert(result.second);
}

void UpdatedHP::update(bool const is_me, Pokemon const & pokemon, unsigned const value) {
	auto const key = std::make_pair(is_me, pokemon.name());
	container.at(key).first = value;
}

void UpdatedHP::direct_damage(bool is_me, Pokemon const & pokemon, unsigned set_damage) {
	auto const key = std::make_pair(is_me, pokemon.name());
	container.at(key).second = set_damage;
}

unsigned UpdatedHP::get(bool const is_me, Pokemon const & pokemon) const {
	auto const key = std::make_pair(is_me, pokemon.name());
	return container.at(key).first;
}

unsigned UpdatedHP::damage(bool is_me, Pokemon const & pokemon) const {
	auto const key = std::make_pair(is_me, pokemon.name());
	return container.at(key).second;
}

} // namespace technicalmachine
