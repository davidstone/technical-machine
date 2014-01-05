// Pokemon Showdown battle logic
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

#ifndef POKEMON_SHOWDOWN__BATTLE_HPP_
#define POKEMON_SHOWDOWN__BATTLE_HPP_

#include <cstdint>
#include <random>
#include <string>
#include "../battle.hpp"

namespace technicalmachine {
namespace ps {

class Battle : public ::technicalmachine::Battle {
public:
	template<typename ... Args>
	Battle(Args &&... args):
		::technicalmachine::Battle::Battle(std::forward<Args>(args)...) {
	}
private:
	VisibleFoeHP max_damage_precision() const override;
};

}	// namespace ps
}	// namespace technicalmachine
#endif	// POKEMON_SHOWDOWN__BATTLE_HPP_
