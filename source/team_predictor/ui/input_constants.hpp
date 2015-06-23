// Constants related to the size of various inputs
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

namespace technicalmachine {

constexpr int padding = 5;
constexpr int left_padding = 70;
constexpr int input_width = 120;
constexpr int input_height = 30;
constexpr int pokemon_indent = 20;
constexpr int ev_input_width = 60;

// Returns the y_position of the nth button
inline int y_position(int const button_number) {
	return (1 + button_number) * padding + button_number * input_height;
}

}	// namespace technicalmachine
