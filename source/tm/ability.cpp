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

namespace technicalmachine {

bool blocks_switching(Generation, Ability const ability, ActivePokemon const switcher, Weather const weather) {
	switch (ability) {
		case Ability::Shadow_Tag:
			return get_ability(switcher) != Ability::Shadow_Tag;
		case Ability::Arena_Trap:
			return grounded(switcher, weather);
		case Ability::Magnet_Pull:
			return is_type(switcher, Type::Steel);
		default:
			return false;
	}
}

bool boosts_special_attack(Ability const ability, Weather const weather) {
	return ability == Ability::Solar_Power and weather.sun();
}

bool boosts_special_defense(Ability const ability, Weather const weather) {
	return ability == Ability::Flower_Gift and weather.sun();
}

void activate_ability_on_switch(Generation const generation, MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather) {
	switch (get_ability(switcher)) {
		case Ability::Download: {
			auto const defense = calculate_defense(generation, other, weather);
			auto const special_defense = calculate_special_defense(generation, other, weather);
			boost(switcher.stage(), defense >= special_defense ? StatNames::SPA : StatNames::ATK, 1_bi);
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
		case Ability::Intimidate:
			boost(other.stage(), StatNames::ATK, -1_bi);
			break;
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

void weather_healing_ability(MutableActivePokemon pokemon, Weather const weather) {
	switch (get_ability(pokemon)) {
		case Ability::Dry_Skin:
			if (weather.rain()) {
				heal(pokemon, rational(1_bi, 8_bi));
			} else if (weather.sun()) {
				heal(pokemon, rational(-1_bi, 8_bi));
			}
			break;
		case Ability::Hydration:
			if (weather.rain()) {
				clear_status(pokemon);
			}
			break;
		case Ability::Ice_Body:
			if (weather.hail()) {
				heal(pokemon, rational(1_bi, 16_bi));
			}
			break;
		case Ability::Rain_Dish:
			if (weather.rain()) {
				heal(pokemon, rational(1_bi, 16_bi));
			}
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine
