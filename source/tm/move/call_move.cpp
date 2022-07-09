// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/call_move.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/category.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/type/effectiveness.hpp>
#include <tm/type/type.hpp>

#include <tm/ability.hpp>
#include <tm/ability_blocks_move.hpp>
#include <tm/any_team.hpp>
#include <tm/block.hpp>
#include <tm/contact_ability_effect.hpp>
#include <tm/end_of_turn.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/known_team.hpp>
#include <tm/rational.hpp>
#include <tm/seen_team.hpp>
#include <tm/status_name.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/maybe_find.hpp>
#include <containers/range_value_t.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <utility>

namespace technicalmachine {
namespace {

constexpr auto breaks_screens(Moves const move) {
	return move == Moves::Brick_Break;
}

auto do_effects_before_moving(Moves const move, any_mutable_active_pokemon auto user, any_team auto & other) {
	if (breaks_screens(move)) {
		other.shatter_screens();
	} else if (thaws_user(move)) {
		if (is_frozen(user.status())) {
			user.clear_status();
		}
	}
}


constexpr auto move_fails(Moves const move, bool const user_damaged, Ability const other_ability, OtherMove const other_move) {
	switch (move) {
		case Moves::Boomburst:
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::Clanging_Scales:
		case Moves::Clangorous_Soulblaze:
		case Moves::Confide:
		case Moves::Disarming_Voice:
		case Moves::Echoed_Voice:
		case Moves::Grass_Whistle:
		case Moves::Growl:
		case Moves::Hyper_Voice:
		case Moves::Metal_Sound:
		case Moves::Noble_Roar:
		case Moves::Parting_Shot:
		case Moves::Relic_Song:
		case Moves::Roar:
		case Moves::Round:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Snarl:
		case Moves::Snore:
		case Moves::Sparkling_Aria:
		case Moves::Supersonic:
		case Moves::Uproar:
			return blocks_sound_moves(other_ability);
		case Moves::Explosion:
		case Moves::Mind_Blown:
		case Moves::Self_Destruct:
			return other_ability == Ability::Damp;
		case Moves::Focus_Punch:
			return user_damaged;
		case Moves::Sucker_Punch:
			return !other_move.future_move_is_damaging();
		default:
			return false;
	}
}

// Returns whether the attack is weakened by the item
template<any_mutable_active_pokemon DefenderPokemon>
auto activate_when_hit_item(KnownMove const move, DefenderPokemon const defender, Weather const weather, Effectiveness const effectiveness) -> bool {
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

constexpr auto targets_foe_specifically(Target const target) {
	switch (target) {
		case Target::adjacent_foe:
		case Target::adjacent:
		case Target::any:
			return true;
		case Target::user:
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::field:
		case Target::all_adjacent_foes:
		case Target::all_adjacent:
		case Target::all:
			return false;
	}
}

constexpr auto activate_target_ability(any_mutable_active_pokemon auto const user, any_mutable_active_pokemon auto const other, Weather const weather, ContactAbilityEffect const effect) {
	switch (effect) {
		case ContactAbilityEffect::nothing:
			break;
		case ContactAbilityEffect::burn:
			user.set_status(StatusName::burn, weather);
			break;
		case ContactAbilityEffect::infatuation:
			user.attract(other, weather);
			break;
		case ContactAbilityEffect::paralysis:
			user.set_status(StatusName::paralysis, weather);
			break;
		case ContactAbilityEffect::poison:
			user.set_status(StatusName::poison, weather);
			break;
		case ContactAbilityEffect::sleep:
			user.set_status(StatusName::sleep, weather);
			break;
	}
}

template<any_team UserTeam, any_team OtherTeamType>
auto use_move(UserTeam & user, ExecutedMove<UserTeam> const executed, Target const target, OtherTeamType & other, OtherMove const other_move, Weather & weather, ActualDamage const actual_damage) -> void {
	constexpr auto generation = generation_from<UserTeam>;
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	do_effects_before_moving(executed.move.name, user_pokemon, other);

	if (targets_foe_specifically(target) and !affects_target(executed.move, other_pokemon.as_const(), weather)) {
		return;
	}

	auto const damaging = is_damaging(executed.move.name);
	auto const effectiveness = Effectiveness(generation, executed.move.type, other_pokemon.types());
	auto const weakened = damaging and activate_when_hit_item(executed.move, other_pokemon, weather, effectiveness);
	auto const damage = actual_damage.value(user, executed, weakened, other, other_move, weather);
	BOUNDED_ASSERT(damaging or damage == 0_bi);

	auto const effects = static_cast<bool>(other_pokemon.substitute()) ?
		substitute_interaction(generation, executed.move.name) :
		Substitute::bypassed;
	if (effects == Substitute::causes_failure) {
		return;
	}

	// Should this check if we did any damage or if the move is damaging?
	auto const damage_done = damage != 0_bi ?
		other_pokemon.direct_damage(executed.move.name, user_pokemon, weather, damage) :
		0_bi;

	if (effects == Substitute::absorbs) {
		return;
	}
	constexpr auto hit_self_combination = std::same_as<UserTeam, OtherTeamType> and !std::same_as<UserTeam, Team<generation_from<UserTeam>>>;
	if constexpr (hit_self_combination) {
		BOUNDED_ASSERT(executed.move.name == Moves::Hit_Self);
	} else {
		executed.side_effect(user, other, weather, damage_done);
	}

	// Should this check if we did any damage or if the move is damaging?
	if (damage_done != 0_bi) {
		auto const item = user_pokemon.item(weather);
		// TODO: Doom Desire / Future Sight are not handled correctly
		if (item == Item::Life_Orb) {
			heal(user_pokemon, weather, rational(-1_bi, 10_bi));
		} else if (item == Item::Shell_Bell) {
			change_hp(user_pokemon, weather, bounded::max(damage_done / 8_bi, 1_bi));
		}

		// TODO: When do target abilities activate?
		if constexpr (hit_self_combination) {
			BOUNDED_ASSERT(executed.contact_ability_effect == ContactAbilityEffect::nothing);
		} else {
			activate_target_ability(user_pokemon, other_pokemon, weather, executed.contact_ability_effect);
		}
	}
}

template<Generation generation>
auto find_move(MoveContainer<generation> const container, Moves const move_name) -> Move {
	auto const maybe_move = containers::maybe_find(container, move_name);
	BOUNDED_ASSERT(maybe_move);
	return *maybe_move;
}

constexpr auto blocked_by_protect(Target const target, Moves const move) {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::any:
		case Target::all_adjacent:
			return true;
		case Target::adjacent:
			switch (move) {
				case Moves::Feint:
				case Moves::Hyperspace_Fury:
				case Moves::Hyperspace_Hole:
				case Moves::Phantom_Force:
				case Moves::Shadow_Force:
					return false;
				default:
					return true;
			}
	}
}

constexpr auto fails_against_fainted(Target const target) {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::all_adjacent_foes:
		case Target::foe_field:
		case Target::all_adjacent:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::adjacent:
		case Target::any:
			return true;
	}
}

auto handle_ability_blocks_move(any_mutable_active_pokemon auto const target, Weather const weather) {
	switch (target.ability()) {
		case Ability::Flash_Fire:
			target.activate_flash_fire();
			break;
		case Ability::Volt_Absorb:
			heal(target, weather, rational(1_bi, 4_bi));
			break;
		case Ability::Water_Absorb:
		case Ability::Dry_Skin:
			heal(target, weather, rational(1_bi, 4_bi));
			break;
		case Ability::Motor_Drive:
			target.stages()[BoostableStat::spe] += 1_bi;
			break;
		default:
			break;
	}
}

template<any_team UserTeam>
auto try_use_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherMove const other_move, Weather & weather, bool const clear_status, ActualDamage const actual_damage) -> void {
	constexpr auto generation = generation_from<UserTeam>;
	auto user_pokemon = user.pokemon();

	auto unsuccessfully_use_move = [&] {
		user_pokemon.unsuccessfully_use_move(move.executed);
	};

	auto const found_move = find_move(all_moves(user_pokemon.as_const(), user.size()), move.selected);
	auto other_pokemon = other.pokemon();
	auto const was_asleep = is_sleeping(user_pokemon.status());
	if (!is_switch(move.selected)) {
		user_pokemon.advance_status_from_move(clear_status);
	}
	// Need the side-effect from recharge
	auto const is_recharging = user_pokemon.recharge();
	if (!can_attempt_move_execution(user_pokemon.as_const(), found_move, other_pokemon.as_const(), was_asleep)) {
		unsuccessfully_use_move();
		return;
	}
	user_pokemon.handle_confusion();
	if (move.executed != Moves::Hit_Self and user_pokemon.flinched()) {
		if (boosts_speed_when_flinched(user_pokemon.ability())) {
			user_pokemon.stages()[BoostableStat::spe] += 1_bi;
		}
	}
	if (!can_execute_move(user_pokemon.as_const(), found_move, weather, is_recharging)) {
		unsuccessfully_use_move();
		return;
	}

	auto const other_ability = other_pokemon.ability();

	if (move.executed != Moves::Hit_Self and !user_pokemon.last_used_move().is_locked_in_by_move()) {
		auto const uses_extra_pp = other_ability == Ability::Pressure;
		user_pokemon.reduce_pp(move.selected, weather, BOUNDED_CONDITIONAL(uses_extra_pp, 2_bi, 1_bi));
	}

	// TODO: What happens if we Sleep Talk Trump Card?
	// TODO: Make sure this does not happen because we missed due to a vanishing
	// state
	if (move.miss) {
		unsuccessfully_use_move();
		if (user_pokemon.item(weather) == Item::Blunder_Policy) {
			user_pokemon.stages()[BoostableStat::spe] += 2_bi;
			user_pokemon.remove_item();
		}
		return;
	}

	auto const target = move_target(generation, move.executed);

	auto const unsuccessful =
		move_fails(move.executed, user_pokemon.damaged(), other_ability, other_move) or
		(other_pokemon.hp().current() == 0_bi and fails_against_fainted(target)) or
		(other_pokemon.last_used_move().is_protecting() and blocked_by_protect(target, move.executed));
	if (unsuccessful) {
		unsuccessfully_use_move();
		return;
	}

	auto const known_move = KnownMove{
		move.executed,
		get_type(generation, move.executed, get_hidden_power_type(user_pokemon))
	};
	if (ability_blocks_move(generation, other_pokemon.ability(), known_move, other_pokemon.status().name(), other_pokemon.types())) {
		handle_ability_blocks_move(other_pokemon, weather);
	} else {
		auto const executed_move = ExecutedMove<UserTeam>{
			known_move,
			found_move.pp(),
			move.side_effect,
			move.critical_hit,
			move.contact_ability_effect
		};
		if (executed_move.move.name == Moves::Hit_Self) {
			use_move(user, executed_move, target, user, other_move, weather, actual_damage);
		} else {
			use_move(user, executed_move, target, other, other_move, weather, actual_damage);
		}
	}
	user_pokemon.successfully_use_move(move.executed);
}

template<any_mutable_active_pokemon UserPokemon>
void end_of_attack(UserPokemon const user_pokemon, OtherMutableActivePokemon<UserPokemon> const other_pokemon, Weather const weather) {
	if constexpr (generation_from<UserPokemon> == Generation::two) {
		user_pokemon.status_and_leech_seed_effects(other_pokemon, weather);
		handle_curse(user_pokemon, weather);
	}
}

} // namespace

template<any_team UserTeam>
auto call_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherMove const other_move, Weather & weather, bool const clear_status, ActualDamage const actual_damage) -> void {
	if (move.selected == Moves::Pass) {
		return;
	}
	try_use_move(user, move, other, other_move, weather, clear_status, actual_damage);
	end_of_attack(user.pokemon(), other.pokemon(), weather);
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(UserTeam) \
	template auto call_move(UserTeam & user, UsedMove<UserTeam> move, OtherTeam<UserTeam> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(Team<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenTeam<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownTeam<generation>)

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
