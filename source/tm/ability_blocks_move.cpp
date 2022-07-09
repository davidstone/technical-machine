// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ability_blocks_move.hpp>

#include <tm/move/category.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>
#include <tm/type/effectiveness.hpp>

namespace technicalmachine {

auto ability_blocks_move(Generation const generation, Ability const ability, KnownMove const move, StatusName const target_status, PokemonTypes const target_types) -> bool {
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
				(generation >= Generation::four or move.name != Moves::Will_O_Wisp);
		case Ability::Volt_Absorb:
			return
				absorb_ability_activates(Type::Electric) and
				(generation >= Generation::four or move.name != Moves::Thunder_Wave);
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
				case Moves::Beat_Up:
					return generation >= Generation::five;
				case Moves::Fire_Fang:
					return generation != Generation::four;
				case Moves::Hit_Self:
				case Moves::Struggle:
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
