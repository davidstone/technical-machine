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
#include <utility>

namespace technicalmachine {

class VariableCollection : public detail::BaseCollection<std::pair<uint16_t, uint16_t>> {
	public:
		VariableCollection ();
		VariableCollection (detail::BaseCollection<std::pair<uint16_t, uint16_t>>::container_type const & pre_set);
		void set_phaze_index (uint8_t const pokemon_index, uint8_t const new_index);
		uint8_t phaze_index (uint8_t const pokemon_index) const;
};
}	// namespace technicalmachine
#endif	// VARIABLE_COLLECTION_HPP_
