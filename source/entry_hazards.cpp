// Entry hazards
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

#include "entry_hazards.hpp"

#include <cstdint>

#include "heal.hpp"
#include "team.hpp"

#include "pokemon/active_pokemon.hpp"

#include "type/effectiveness.hpp"

namespace technicalmachine {
namespace {

constexpr unsigned max_spikes_layers = 3;
constexpr unsigned max_toxic_spikes_layers = 2;
constexpr unsigned max_stealth_rock_layers = 1;
constexpr EntryHazards::hash_type max_spikes = max_spikes_layers + 1;
constexpr EntryHazards::hash_type max_toxic_spikes = max_toxic_spikes_layers + 1;
constexpr EntryHazards::hash_type max_stealth_rock = max_stealth_rock_layers + 1;

bool removes_toxic_spikes(ActivePokemon const & switcher);

}	// unnamed namespace

EntryHazards::EntryHazards() :
	spikes(0),
	toxic_spikes(0),
	stealth_rock(false)
	{
}

void EntryHazards::clear() {
	spikes = 0;
	toxic_spikes = 0;
	stealth_rock = 0;
}

void EntryHazards::add_spikes() {
	if (spikes < max_spikes_layers)
		++spikes;
}

void EntryHazards::add_toxic_spikes() {
	if (toxic_spikes < max_toxic_spikes_layers)
		++toxic_spikes;
}

void EntryHazards::add_stealth_rock() {
	if (!stealth_rock)
		stealth_rock = true;
}

EntryHazards::hash_type EntryHazards::hash() const {
	return spikes + max_spikes *
			(toxic_spikes + max_toxic_spikes *
			stealth_rock);
}

EntryHazards::hash_type EntryHazards::max_hash() {
	return max_spikes * max_toxic_spikes * max_stealth_rock;
}

void EntryHazards::apply(Team & switcher, Weather const & weather) {
	EntryHazards & entry_hazards = switcher.entry_hazards;
	if (switcher.pokemon().ability().blocks_secondary_damage())
		return;

	if (grounded(switcher.pokemon(), weather)) {
		if (entry_hazards.toxic_spikes) {
			if (removes_toxic_spikes(switcher.pokemon()))
				entry_hazards.toxic_spikes = 0;
			else
				apply_toxic_spikes(switcher, weather);
		}
		if (entry_hazards.spikes)
			drain(switcher.pokemon(), Rational(entry_hazards.spikes + 1u, 16));
	}
	if (entry_hazards.stealth_rock) {
		drain(switcher.pokemon(), Rational(1, 8) * Effectiveness::stealth_rock_effectiveness(switcher.pokemon()));
	}
}

bool operator== (EntryHazards const lhs, EntryHazards const rhs) {
	return lhs.spikes == rhs.spikes and
			lhs.toxic_spikes == rhs.toxic_spikes and
			lhs.stealth_rock == rhs.stealth_rock;
}
bool operator!= (EntryHazards const lhs, EntryHazards const rhs) {
	return !(lhs == rhs);
}

void EntryHazards::apply_toxic_spikes(Team & switcher, Weather const & weather) {
	if (switcher.entry_hazards.toxic_spikes == 1)
		Status::apply<Status::POISON>(switcher.pokemon(), weather);
	else
		Status::apply<Status::POISON_TOXIC>(switcher.pokemon(), weather);
}

namespace {

bool removes_toxic_spikes(ActivePokemon const & switcher) {
	return is_type(switcher, Type::Poison);
}
}	// unnamed namespace
}	// namespace technicalmachine
