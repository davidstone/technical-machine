// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ability.hpp>

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
