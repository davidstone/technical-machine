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

UpdatedHP::mapped_type::mapped_type(unsigned set_new_hp, unsigned set_damage):
	m_new_hp(set_new_hp),
	m_damage(set_damage),
	m_fainted(false)
	{
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

void UpdatedHP::add(bool const is_me, Pokemon const & pokemon, unsigned const max_precision) {
	auto const key = std::make_pair(is_me, pokemon.name());
	mapped_type const mapped(max_precision, 0);
	auto const result = container.insert(std::make_pair(key, mapped));
	// This implementation only works if Species Clause is in effect
	assert(result.second);
	// Do not warn if asserts are disabled
	static_cast<void>(result);
}

void UpdatedHP::update(bool const is_me, Pokemon const & pokemon, unsigned const value) {
	auto const key = std::make_pair(is_me, pokemon.name());
	container.at(key).change_hp(value);
}

void UpdatedHP::direct_damage(bool is_me, Pokemon const & pokemon, unsigned set_damage) {
	auto const key = std::make_pair(is_me, pokemon.name());
	container.at(key).direct_damage(set_damage);
}

void UpdatedHP::faint(bool is_me, Pokemon const & pokemon) {
	auto const key = std::make_pair(is_me, pokemon.name());
	container.at(key).faint();
}

unsigned UpdatedHP::get(bool const is_me, Pokemon const & pokemon) const {
	auto const key = std::make_pair(is_me, pokemon.name());
	return container.at(key).new_hp();
}

unsigned UpdatedHP::damage(bool is_me, Pokemon const & pokemon) const {
	auto const key = std::make_pair(is_me, pokemon.name());
	return container.at(key).damage();
}

bool UpdatedHP::is_fainted(bool is_me, Pokemon const & pokemon) const {
	auto const key = std::make_pair(is_me, pokemon.name());
	return container.at(key).is_fainted();
}

} // namespace technicalmachine
