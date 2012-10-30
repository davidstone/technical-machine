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

#ifndef TEAM_PREDICTOR__UI__NATURE_INPUT_HPP_
#define TEAM_PREDICTOR__UI__NATURE_INPUT_HPP_

#include <FL/Fl_Input.H>
#include "../../stat/nature.hpp"

namespace technicalmachine {

class NatureInput {
	public:
		explicit NatureInput(int button_number);
		Nature::Natures value() const;
	private:
		Fl_Input input;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__UI__NATURE_INPUT_HPP_
