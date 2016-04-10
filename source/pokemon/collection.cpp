// Collection of Pokemon with index indicating current Pokemon
// Copyright (C) 2016 David Stone
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

#include "collection.hpp"

#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"
#include "pokemon_not_found.hpp"
#include "species.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"

#include <bounded/integer_range.hpp>

#include <cassert>

namespace technicalmachine {

PokemonCollection::PokemonCollection(TeamSize const initial_size):
	current_replacement(0_bi),
	true_size(initial_size) {
}

void PokemonCollection::initialize_size(TeamSize const new_size) {
	true_size = new_size;
}

void PokemonCollection::initialize_replacement () {
	// No need to bounds check because index() already is
	current_replacement = index();
}

void PokemonCollection::set_replacement(containers::index_type<PokemonCollection> const new_index) {
	current_replacement = check_range (new_index);
}

bool PokemonCollection::is_switching_to_self () const {
	return replacement() == index();
}
bool PokemonCollection::is_switching_to_self (Moves const move) const {
	return to_replacement(move) == index();
}

TeamSize PokemonCollection::real_size() const {
	return true_size;
}

containers::index_type<PokemonCollection> PokemonCollection::find_index(Species const name) const {
	for (auto const found_index : bounded::integer_range(size(*this))) {
		if (operator()(found_index) == name)
			return found_index;
	}
	throw PokemonNotFound(name);
}

bool PokemonCollection::seen(Species const name) {
	// In the event of current_replacement == size(), a new Pokemon is added
	// immediately, increasing size() by 1, making this safe.
	for (current_replacement = 0_bi; current_replacement != size(*this); ++current_replacement) {
		if (name == operator()(replacement()))
			return true;
	}
	return false;
}

void PokemonCollection::remove_active () {
	assert(index() != replacement());
	containers::erase(static_cast<PokemonContainer &>(*this), begin() + index());
	decrement_real_size();
	// We don't need any bounds checking here because we've already established
	// that replacement() is greater than index(), so it cannot be 0, which is
	// the only value that could get this out of bounds.
	set_index((index() > replacement()) ? replacement() : containers::index_type<PokemonCollection>(replacement() - 1_bi, bounded::non_check));
	for (auto & pokemon : *this) {
		all_moves(pokemon).remove_switch();
	}
}

void PokemonCollection::decrement_real_size () {
	--true_size;
}

}	// namespace technicalmachine
