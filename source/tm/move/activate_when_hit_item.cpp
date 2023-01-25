// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.move.activate_when_hit_item;

import tm.move.category;
import tm.move.known_move;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.known_pokemon;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;
import tm.pokemon.substitute;

import tm.stat.stage;
import tm.stat.stat_names;

import tm.type.effectiveness;
import tm.type.type;

import tm.generation;
import tm.heal;
import tm.item;
import tm.rational;
import tm.weather;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

// Returns whether the attack is weakened by the item
export template<any_mutable_active_pokemon DefenderPokemon>
constexpr auto activate_when_hit_item(KnownMove const move, DefenderPokemon const defender, Weather const weather, Effectiveness const effectiveness) -> bool {
	constexpr auto generation = generation_from<DefenderPokemon>;
	auto substitute = [&] {
		return defender.substitute() and substitute_interaction(generation, move.name) != Substitute::bypassed;
	};
	auto resistance_berry = [&](Type const resisted) {
		if (move.type != resisted or substitute()) {
			return false;
		}
		defender.remove_item();
		return true;
	};
	auto se_resistance_berry = [&](Type const resisted) {
		if (!effectiveness.is_super_effective()) {
			return false;
		}
		return resistance_berry(resisted);
	};
	constexpr auto max_stage = numeric_traits::max_value<Stage::value_type>;
	auto stat_boost = [&](BoostableStat const stat) {
		auto & stage = defender.stages()[stat];
		if (stage != max_stage and !substitute()) {
			stage += 1_bi;
			defender.remove_item();
		}
	};
	auto stat_boost_move_type = [&](Type const type, BoostableStat const stat) {
		if (move.type == type) {
			stat_boost(stat);
		}
		return false;
	};
	switch (defender.item(weather)) {
		case Item::Absorb_Bulb:
			return stat_boost_move_type(Type::Water, BoostableStat::spa);
		case Item::Air_Balloon:
			defender.remove_item();
			return false;
		case Item::Babiri_Berry:
			return se_resistance_berry(Type::Steel);
		case Item::Cell_Battery:
			return stat_boost_move_type(Type::Electric, BoostableStat::atk);
		case Item::Charti_Berry:
			return se_resistance_berry(Type::Rock);
		case Item::Chople_Berry:
			return se_resistance_berry(Type::Fighting);
		case Item::Chilan_Berry:
			return resistance_berry(Type::Normal);
		case Item::Coba_Berry:
			return se_resistance_berry(Type::Flying);
		case Item::Colbur_Berry:
			return se_resistance_berry(Type::Dark);
		case Item::Enigma_Berry:
			if (effectiveness.is_super_effective() and generation >= Generation::four and !substitute()) {
				defender.remove_item();
				heal(defender, weather, rational(1_bi, 4_bi));
			}
			return false;
		case Item::Haban_Berry:
			return se_resistance_berry(Type::Dragon);
		case Item::Kasib_Berry:
			return se_resistance_berry(Type::Ghost);
		case Item::Kebia_Berry:
			return se_resistance_berry(Type::Poison);
		case Item::Kee_Berry:
			if (is_physical(generation, move)) {
				stat_boost(BoostableStat::def);
			}
			return false;
		case Item::Luminous_Moss:
			return stat_boost_move_type(Type::Water, BoostableStat::spd);
		case Item::Maranga_Berry:
			if (is_special(generation, move)) {
				stat_boost(BoostableStat::spd);
			}
			return false;
		case Item::Occa_Berry:
			return se_resistance_berry(Type::Fire);
		case Item::Passho_Berry:
			return se_resistance_berry(Type::Water);
		case Item::Payapa_Berry:
			return se_resistance_berry(Type::Psychic);
		case Item::Rindo_Berry:
			return se_resistance_berry(Type::Grass);
		case Item::Roseli_Berry:
			return se_resistance_berry(Type::Fairy);
		case Item::Shuca_Berry:
			return se_resistance_berry(Type::Ground);
		case Item::Snowball:
			return stat_boost_move_type(Type::Ice, BoostableStat::atk);
		case Item::Tanga_Berry:
			return se_resistance_berry(Type::Bug);
		case Item::Wacan_Berry:
			return se_resistance_berry(Type::Electric);
		case Item::Weakness_Policy:
			if (effectiveness.is_super_effective() and !substitute()) {
				auto & stages = defender.stages();
				if (stages[BoostableStat::atk] != max_stage and stages[BoostableStat::spa] != max_stage) {
					boost_offensive(stages, 2_bi);
					defender.remove_item();
				}
			}
			return false;
		case Item::Yache_Berry:
			return se_resistance_berry(Type::Ice);
		default:
			return false;
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(DefenderPokemon) \
	template auto activate_when_hit_item(KnownMove const move, DefenderPokemon const defender, Weather const weather, Effectiveness const effectiveness) -> bool

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyMutableActivePokemon<Pokemon<generation>>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyMutableActivePokemon<SeenPokemon<generation>>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyMutableActivePokemon<KnownPokemon<generation>>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
