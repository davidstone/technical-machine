// Class to abstract UI of getting Nature for the team builder
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

#include "nature_input.hpp"
#include "input_constants.hpp"
#include "../../string_conversions/conversion.hpp"
#include "../../string_conversions/invalid_string_conversion.hpp"

namespace technicalmachine {
namespace {
constexpr int nature_input_width = 80;
}

NatureInput::NatureInput(int const button_number):
	input(left_padding + pokemon_indent, y_position(button_number), nature_input_width, input_height, "Nature")
	{
}

Nature NatureInput::value() const {
	try {
		return from_string<Nature>(input.value());
	}
	catch (InvalidFromStringConversion const &) {
		return Nature::END;
	}
}

}	// namespace technicalmachine
