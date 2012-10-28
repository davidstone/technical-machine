// Class to abstract UI of getting each move for the team builder
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

#include "move_inputs.hpp"
#include "input_constants.hpp"
#include "../../string_conversions/conversion.hpp"

namespace technicalmachine {

MoveInput::MoveInput(int const button_number, int const x_position, char const * label):
	input(left_padding + pokemon_indent + x_position * width, y_position(button_number), width, input_height, label)
	{
}

Moves MoveInput::value() const {
	return from_string<Moves>(input.value());
}

MoveInputs::MoveInputs(int const button_number):
	input0(button_number, 0, "Moves"),
	input1(button_number, 1),
	input2(button_number, 2),
	input3(button_number, 3)
	{
}

}	// namespace technicalmachine
