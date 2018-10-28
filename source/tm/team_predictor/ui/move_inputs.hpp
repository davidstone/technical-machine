// Class to abstract UI of getting each move for the team builder
// Copyright (C) 2015 David Stone
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

#pragma once

#include "../../move/moves.hpp"
#include "../../move/max_moves_per_pokemon.hpp"

#include <containers/array/array.hpp>

#include <fltk/FL/Fl_Input.H>

namespace technicalmachine {

struct MoveInput {
	MoveInput(int button_number, int x_position, char const * label = "");
	Moves value() const;
private:
	static constexpr int width = 90;
	Fl_Input input;
};

struct MoveInputs {
	explicit MoveInputs(int button_number);
	containers::array<MoveInput, max_moves_per_pokemon.value()> value;
};

}	// namespace technicalmachine
