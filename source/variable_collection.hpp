// Collection of move random effects with index indicating current effect
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

#ifndef VARIABLE_COLLECTION_HPP_
#define VARIABLE_COLLECTION_HPP_

#include "collection.hpp"
#include <cstdint>
#include <functional>
#include "variable.hpp"
#include "pokemon/species_forward.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
class Team;

class VariableCollection : public detail::BaseCollection<Variable> {
	public:
		VariableCollection (Moves move, unsigned foe_size);
		void set_phaze_index (Team const & other, Species species);
		uint8_t phaze_index (uint8_t const pokemon_index) const;
		void for_each_index (std::function<void(void)> const & f);
		void remove_phazing (uint8_t foe_size);
		void set_magnitude (unsigned const magnitude);
};
}	// namespace technicalmachine
#endif	// VARIABLE_COLLECTION_HPP_
