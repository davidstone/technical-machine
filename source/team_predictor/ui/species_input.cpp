// Class to abstract UI of getting each species for the team builder
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

#include "species_input.hpp"
#include "input_constants.hpp"
#include "../../pokemon/species_forward.hpp"
#include "../../string_conversions/conversion.hpp"

namespace technicalmachine {

SpeciesInput::SpeciesInput(int const button_number):
	input(left_padding, y_position(button_number), input_width, input_height, "Pokemon")
	{
}

Species SpeciesInput::value() const {
	return from_string<Species>(input.value());
}

}	// namespace technicalmachine
