// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.activate_ability_on_switch;

import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;

import tm.stat.calculate;
import tm.stat.stage;
import tm.stat.stat_names;

import tm.ability;
import tm.environment;
import tm.generation;
import tm.item;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

export template<any_mutable_active_pokemon PokemonType>
constexpr void activate_ability_on_switch(PokemonType const switcher, OtherMutableActivePokemon<PokemonType> const other, Environment & environment) {
	constexpr auto generation = generation_from<PokemonType>;
	auto const switcher_ability = switcher.ability();
	switch (switcher_ability) {
		case Ability::Download: {
			// TODO: Should not take into account items, abilities, or Wonder Room
			constexpr auto is_self_ko = false;
			auto const defense = calculate_defense(other.as_const(), environment, is_self_ko);
			auto const special_defense = calculate_special_defense(other.as_const(), switcher_ability, environment);
			auto const boosted_stat = defense >= special_defense ? BoostableStat::spa : BoostableStat::atk;
			switcher.stages()[boosted_stat] += 1_bi;
			break;
		}
		case Ability::Drizzle:
			environment.activate_rain_from_ability(generation, extends_rain(switcher.item(environment)));
			break;
		case Ability::Drought:
			environment.activate_sun_from_ability(generation, extends_sun(switcher.item(environment)));
			break;
		case Ability::Forecast:
			break;
		case Ability::Intimidate: {
			if (blocks_intimidate(generation, other.ability())) {
				break;
			}
			auto & attack = other.stages()[BoostableStat::atk];
			if (attack == numeric_traits::min_value<Stage::value_type>) {
				break;
			}
			attack -= 1_bi;
			auto & speed = other.stages()[BoostableStat::spe];
			if (other.item(environment) == Item::Adrenaline_Orb and speed != numeric_traits::max_value<Stage::value_type>) {
				speed += 1_bi;
				other.remove_item();
			}
			break;
		}
		case Ability::Sand_Stream:
			environment.activate_sand_from_ability(generation, extends_sand(switcher.item(environment)));
			break;
		case Ability::Snow_Warning:
			environment.activate_hail_from_ability(generation, extends_hail(switcher.item(environment)));
			break;
		case Ability::Trace: {
			auto const other_ability = other.ability();
			if (traceable(generation, other_ability)) {
				switcher.set_ability(other_ability);
			}
			break;
		}
		default:
			break;
	}
}

} // namespace technicalmachine
