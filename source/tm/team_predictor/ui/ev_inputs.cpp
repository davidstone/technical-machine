// Class to abstract UI of getting EVs for the team builder
// Copyright (C) 2018 David Stone
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

#include <tm/team_predictor/ui/ev_inputs.hpp>
#include <string>
#include <tm/team_predictor/ui/input_constants.hpp>

namespace technicalmachine {

EVInput::EVInput(int const button_number, int const ev, char const * label):
	input(left_padding + pokemon_indent + ev_input_width * (ev), y_position(button_number), ev_input_width, input_height, label)
	{
}

EV EVInput::value() const {
	std::string const str = input.value();
	return EV(!str.empty() ? bounded::to_integer<EV::value_type>(str) : 0_bi);
}

EVInputs::EVInputs(int const button_number):
	hp(button_number, 0, "EVs"),
	atk(button_number, 1),
	def(button_number, 2),
	spa(button_number, 3),
	spd(button_number, 4),
	spe(button_number, 5)
	{
}

}	// namespace technicalmachine
