// Copyright (C) 2016 David Stone
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

#include <tm/ability.hpp>

#include <cassert>

#include <tm/move/category.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/type/type.hpp>

#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

auto blocks_intimidate(Generation const generation, Ability const ability) -> bool {
	if (generation <= Generation::eight) {
		return false;
	}
	switch (ability) {
		case Ability::Inner_Focus:
		case Ability::Oblivious:
		case Ability::Scrappy:
		case Ability::Own_Tempo:
			return true;
		default:
			return false;
	}
}

auto traceable(Generation const generation, Ability const ability) -> bool {
	switch (ability) {
		case Ability::Multitype:
		case Ability::Illusion:
		case Ability::Zen_Mode:
		case Ability::Imposter:
		case Ability::Stance_Change:
		case Ability::Power_of_Alchemy:
		case Ability::Receiver:
		case Ability::Schooling:
		case Ability::Comatose:
		case Ability::Shields_Down:
		case Ability::Disguise:
		case Ability::RKS_System:
		case Ability::Battle_Bond:
		case Ability::Power_Construct:
		case Ability::Gulp_Missile:
		case Ability::Ice_Face:
		case Ability::Neutralizing_Gas:
		case Ability::Hunger_Switch:
			return false;
		case Ability::Trace:
			return generation <= Generation::three;
		case Ability::Forecast:
			return generation <= Generation::three;
		case Ability::Flower_Gift:
			return generation <= Generation::four;
		default:
			return true;
	}
}

}	// namespace technicalmachine
