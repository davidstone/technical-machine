// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.blocks_status;

import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.weather;

namespace technicalmachine {

export constexpr bool blocks_status(Ability const ability, Ability const other_ability, StatusName const status, Weather const weather) {
	auto const ability_blocked = other_ability == Ability::Mold_Breaker;
	switch (status) {
		case StatusName::burn:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Leaf_Guard:
					return weather == Weather::sun;
				case Ability::Water_Veil:
					return true;
				default:
					return false;
			}
		case StatusName::freeze:
			return weather == Weather::sun or (!ability_blocked and ability == Ability::Magma_Armor);
		case StatusName::paralysis:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Leaf_Guard:
					return weather == Weather::sun;
				case Ability::Limber:
					return true;
				default:
					return false;
			}
		case StatusName::poison:
		case StatusName::toxic:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Immunity:
					return true;
				case Ability::Leaf_Guard:
					return weather == Weather::sun;
				default:
					return false;
			}
		case StatusName::sleep:
		case StatusName::rest:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Insomnia:
				case Ability::Vital_Spirit:
					return true;
				case Ability::Leaf_Guard:
					return weather == Weather::sun;
				default:
					return false;
			}
		case StatusName::clear:
			return false;
	}
}

export constexpr auto blocks_status(Type const type, StatusName const status) {
	switch (status) {
		case StatusName::burn:
			return type == Type::Fire;
		case StatusName::freeze:
			return type == Type::Ice;
		case StatusName::poison:
		case StatusName::toxic:
			return type == Type::Poison or type == Type::Steel;
		default:
			return false;
	}
}

} // namespace technicalmachine
