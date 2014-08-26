// Hold move scores to allow efficient reordering
// Copyright (C) 2014 David Stone
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

#ifndef EVALUATE__MOVE_SCORES_HPP_
#define EVALUATE__MOVE_SCORES_HPP_

#include <cstdint>
#include <map>
#include "../move/moves_forward.hpp"
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
class Pokemon;

class MoveScores {
public:
	explicit MoveScores(Pokemon const & pokemon);
	double const & at(Species species, Moves name) const;
	double & at(Species species, Moves name);
private:
	using key_type = std::pair<Species, Moves>;
	using container_type = std::map<key_type, double>;
	container_type scores;
};

}	// namespace technicalmachine
#endif	// EVALUATE__MOVE_SCORES_HPP_
