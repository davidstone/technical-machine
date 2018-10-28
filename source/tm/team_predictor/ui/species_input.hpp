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

#pragma once

#include <fltk/FL/Fl_Input.H>
#include <tm/pokemon/species_forward.hpp>

namespace technicalmachine {

struct SpeciesInput {
	explicit SpeciesInput(int button_number);
	Species value() const;
private:
	Fl_Input input;
};

}	// namespace technicalmachine
