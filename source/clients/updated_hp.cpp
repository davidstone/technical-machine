// Keep track of changes in HP to make sure TM has the same view as the server
// Copyright (C) 2013 David Stone
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
		add(team.is_me(), pokemon, static_cast<unsigned>(get_stat(pokemon, StatNames::HP).max));
	}
}

UpdatedHP::mapped_type::mapped_type(unsigned set_new_hp, unsigned set_damage):
	m_new_hp(set_new_hp),
	m_damage(set_damage),
	m_fainted(false) {
}
unsigned UpdatedHP::mapped_type::new_hp() const {
	return m_new_hp;
}
unsigned UpdatedHP::mapped_type::damage() const {
	return m_damage;
}
bool UpdatedHP::mapped_type::is_fainted() const {
	return m_fainted;
}
void UpdatedHP::mapped_type::change_hp(unsigned set_new_hp) {
	m_new_hp = set_new_hp;
}
void UpdatedHP::mapped_type::direct_damage(unsigned set_damage) {
	m_damage = set_damage;
}
void UpdatedHP::mapped_type::faint() {
	m_fainted = true;
}
void UpdatedHP::mapped_type::reset() {
	m_damage = 0;
}

void UpdatedHP::reset_between_turns() {
	for (auto & value : container) {
		value.second.reset();
	}
}

void UpdatedHP::add(bool const is_me, Species const species, unsigned const max_precision) {
	key_type const key(is_me, species);
	mapped_type const mapped(max_precision, 0);
	auto const result = container.insert(container_type::value_type(key, mapped));
	// This implementation only works if Species Clause is in effect
	assert(result.second);
	// Do not warn if asserts are disabled
	static_cast<void>(result);
}

void UpdatedHP::update(bool const is_me, Species const species, unsigned const value) {
	container.at(key_type(is_me, species)).change_hp(value);
}

void UpdatedHP::direct_damage(bool is_me, Species const species, unsigned set_damage) {
	container.at(key_type(is_me, species)).direct_damage(set_damage);
}

void UpdatedHP::faint(bool is_me, Species const species) {
	container.at(key_type(is_me, species)).faint();
}

unsigned UpdatedHP::get(bool const is_me, Species const species) const {
	return container.at(key_type(is_me, species)).new_hp();
}

unsigned UpdatedHP::damage(bool is_me, Species const species) const {
	return container.at(key_type(is_me, species)).damage();
}

bool UpdatedHP::is_fainted(bool is_me, Species const species) const {
	return container.at(key_type(is_me, species)).is_fainted();
}

} // namespace technicalmachine
