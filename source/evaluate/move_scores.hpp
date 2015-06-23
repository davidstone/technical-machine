// Hold move scores to allow efficient reordering
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

#include <vector>
#include "../move/moves.hpp"
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
class Pokemon;

class MoveScores {
public:
	explicit MoveScores(Pokemon const & pokemon);
	double at(Species species, Moves move) const;
	double & at(Species species, Moves move);
private:
	using key_type = std::pair<Species, Moves>;
	using container_type = std::vector<std::pair<key_type, double>>;
	container_type m_scores;
};

}	// namespace technicalmachine
