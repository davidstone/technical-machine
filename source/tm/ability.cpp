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

#include <tm/stat/calculate.hpp>

#include <tm/type/type.hpp>

#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

bool blocks_switching(Generation const generation, Ability const ability, ActivePokemon const switcher, Weather const weather) {
	switch (ability) {
		case Ability::Shadow_Tag:
			return get_ability(switcher) != Ability::Shadow_Tag;
		case Ability::Arena_Trap:
			return
				(generation >= Generation::six and containers::any_equal(switcher.types(), Type::Ghost)) or
				grounded(generation, switcher, weather);
		case Ability::Magnet_Pull:
			return is_type(switcher, Type::Steel);
		default:
			return false;
	}
}

namespace {

constexpr auto blocks_intimidate(Generation const generation, Ability const ability) -> bool {
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

} // namespace

void activate_ability_on_switch(Generation const generation, MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather) {
	auto const switcher_ability = get_ability(switcher);
	switch (switcher_ability) {
		case Ability::Download: {
			// Move is irrelevant here
			constexpr auto move = Moves::Switch0;
			auto const defense = calculate_defense(generation, other, move, switcher_ability, weather);
			auto const special_defense = calculate_special_defense(generation, other, switcher_ability, weather);
			switcher.stage()[defense >= special_defense ? StatNames::SPA : StatNames::ATK] += 1_bi;
			break;
		}
		case Ability::Drizzle:
			weather.activate_rain(Weather::permanent);
			break;
		case Ability::Drought:
			weather.activate_sun(Weather::permanent);
			break;
		case Ability::Forecast:	// TODO: fix this
			break;
		case Ability::Intimidate: {
			if (blocks_intimidate(generation, get_ability(other))) {
				break;
			}
			auto & attack = other.stage()[StatNames::ATK];
			if (attack == bounded::min_value<Stage::value_type>) {
				break;
			}
			attack -= 1_bi;
			auto & speed = other.stage()[StatNames::SPE];
			if (other.item(generation, weather) == Item::Adrenaline_Orb and speed != bounded::max_value<Stage::value_type>) {
				speed += 1_bi;
				other.remove_item();
			}
			break;
		}
		case Ability::Sand_Stream:
			weather.activate_sand(Weather::permanent);
			break;
		case Ability::Snow_Warning:
			weather.activate_hail(Weather::permanent);
			break;
		case Ability::Trace:
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine
