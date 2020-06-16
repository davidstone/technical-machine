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

#include <tm/pokemon/collection.hpp>

#include <bounded/assert.hpp>

namespace technicalmachine {

void PokemonCollection::remove_active(containers::index_type<PokemonCollection> const index_of_replacement) {
	auto const original_index = index();
	BOUNDED_ASSERT(original_index != index_of_replacement);
	containers::erase(m_container, begin(m_container) + original_index);
	--m_real_size;
	set_index((index_of_replacement < original_index) ?
		index_of_replacement :
		containers::index_type<PokemonCollection>(index_of_replacement - 1_bi, bounded::non_check)
	);
	for (auto & pokemon : m_container) {
		pokemon.all_moves().remove_switch();
	}
}

}	// namespace technicalmachine
