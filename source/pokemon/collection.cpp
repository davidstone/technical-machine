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

#include <cassert>

namespace technicalmachine {

void PokemonCollection::remove_active(containers::index_type<PokemonCollection> const index_of_replacement) {
	assert(index() != index_of_replacement);
	containers::erase(static_cast<PokemonContainer &>(*this), begin() + index());
	--true_size;
	// We don't need any bounds checking here because we've already established
	// that index_of_replacement is greater than index(), so it cannot be 0,
	// which is the only value that could get this out of bounds.
	set_index((index() > index_of_replacement) ?
		index_of_replacement :
		containers::index_type<PokemonCollection>(index_of_replacement - 1_bi, bounded::non_check)
	);
	for (auto & pokemon : *this) {
		all_moves(pokemon).remove_switch();
	}
}

}	// namespace technicalmachine
