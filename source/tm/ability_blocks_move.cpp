// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ability_blocks_move;

import tm.move.category;
import tm.move.known_move;
import tm.move.move_name;
import tm.move.target;

import tm.status.status_name;

import tm.type.effectiveness;
import tm.type.pokemon_types;
import tm.type.type;

import tm.ability;
import tm.generation;

namespace technicalmachine {

export constexpr auto ability_blocks_move(Generation const generation, Ability const ability, KnownMove const move, StatusName const target_status, PokemonTypes const target_types) -> bool {
	auto absorb_ability_activates = [=](Type const absorbed_type) {
		return move_targets_foe(generation, move.name) and move.type == absorbed_type;
	};

	switch (ability) {
		case Ability::Flash_Fire:
			if (generation <= Generation::four and target_status == StatusName::freeze) {
				return false;
			}
			return
				absorb_ability_activates(Type::Fire) and
				(generation >= Generation::four or move.name != MoveName::Will_O_Wisp);
		case Ability::Volt_Absorb:
			return
				absorb_ability_activates(Type::Electric) and
				(generation >= Generation::four or move.name != MoveName::Thunder_Wave);
		case Ability::Water_Absorb:
		case Ability::Dry_Skin:
			return absorb_ability_activates(Type::Water);
		case Ability::Wonder_Guard:
			if (!is_damaging(move.name)) {
				return false;
			}
			if (Effectiveness(generation, move.type, target_types).is_super_effective()) {
				return false;
			}
			switch (move.name) {
				case MoveName::Beat_Up:
					return generation >= Generation::five;
				case MoveName::Fire_Fang:
					return generation != Generation::four;
				case MoveName::Hit_Self:
				case MoveName::Struggle:
					return false;
				default:
					return true;
			}
		case Ability::Motor_Drive:
			return absorb_ability_activates(Type::Electric);
		default:
			return false;
	}
}

} // namespace technicalmachine
