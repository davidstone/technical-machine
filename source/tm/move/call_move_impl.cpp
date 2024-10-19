// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

module tm.move.call_move;

import tm.move.activate_when_hit_item;
import tm.move.actual_damage;
import tm.move.affects_target;
import tm.move.category;
import tm.move.end_of_attack;
import tm.move.executed_move;
import tm.move.known_move;
import tm.move.move;
import tm.move.move_is_unsuccessful;
import tm.move.move_name;
import tm.move.other_action;
import tm.move.target;
import tm.move.used_move;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.change_hp;
import tm.pokemon.get_hidden_power_type;
import tm.pokemon.potentially_selectable_moves;
import tm.pokemon.substitute;

import tm.stat.stage;
import tm.stat.stat_names;

import tm.status.status;
import tm.status.status_name;

import tm.type.effectiveness;
import tm.type.move_type;

import tm.ability;
import tm.ability_blocks_move;
import tm.any_team;
import tm.can_execute_move;
import tm.contact_ability_effect;
import tm.environment;
import tm.generation;
import tm.heal;
import tm.item;
import tm.other_team;
import tm.rational;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto breaks_screens(MoveName const move) {
	return move == MoveName::Brick_Break;
}

auto do_effects_before_moving(MoveName const move, any_mutable_active_pokemon auto user, any_team auto & other) {
	if (breaks_screens(move)) {
		other.shatter_screens();
	} else if (thaws_user(move)) {
		if (is_frozen(user.status())) {
			user.clear_status();
		}
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

constexpr auto activate_target_ability(any_mutable_active_pokemon auto const user, any_mutable_active_pokemon auto const other, Environment const environment, ContactAbilityEffect const effect) {
	switch (effect) {
		case ContactAbilityEffect::nothing:
			break;
		case ContactAbilityEffect::burn:
			user.set_status(StatusName::burn, environment);
			break;
		case ContactAbilityEffect::infatuation:
			user.attract(other, environment);
			break;
		case ContactAbilityEffect::paralysis:
			user.set_status(StatusName::paralysis, environment);
			break;
		case ContactAbilityEffect::poison:
			user.set_status(StatusName::poison, environment);
			break;
		case ContactAbilityEffect::sleep:
			user.set_status(StatusName::sleep, environment);
			break;
	}
}

template<any_team UserTeam>
auto use_move(UserTeam & user, ExecutedMove<UserTeam> const executed, Target const target, OtherTeam<UserTeam> & other, OtherAction const other_action, Environment & environment, ActualDamage const actual_damage) -> void {
	constexpr auto generation = generation_from<UserTeam>;
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	do_effects_before_moving(executed.move.name, user_pokemon, other);

	if (targets_foe_specifically(target) and !affects_target(executed.move, other_pokemon.as_const(), environment)) {
		return;
	}

	auto const damaging = is_damaging(executed.move.name);
	auto const effectiveness = Effectiveness(generation, executed.move.type, other_pokemon.types());
	auto const weakened = damaging and activate_when_hit_item(executed.move, other_pokemon, environment, effectiveness);
	auto const damage = actual_damage.value(user, executed, weakened, other, other_action, environment);
	BOUNDED_ASSERT(damaging or damage == 0_bi);

	auto const effects = static_cast<bool>(other_pokemon.substitute()) ?
		substitute_interaction(generation, executed.move.name) :
		Substitute::bypassed;
	if (effects == Substitute::causes_failure) {
		return;
	}

	// Should this check if we did any damage or if the move is damaging?
	auto const damage_done = damage != 0_bi ?
		other_pokemon.direct_damage(executed.move.name, user_pokemon, environment, damage) :
		0_bi;

	if (effects == Substitute::absorbs) {
		return;
	}
	executed.side_effect(user, other, environment, damage_done);

	// Should this check if we did any damage or if the move is damaging?
	if (damage_done != 0_bi) {
		auto const item = user_pokemon.item(environment);
		// TODO: Doom Desire / Future Sight are not handled correctly
		if (item == Item::Life_Orb) {
			heal(user_pokemon, environment, rational(-1_bi, 10_bi));
		} else if (item == Item::Shell_Bell) {
			change_hp(user_pokemon, environment, bounded::max(damage_done / 8_bi, 1_bi));
		}

		// TODO: When do target abilities activate?
		activate_target_ability(user_pokemon, other_pokemon, environment, executed.contact_ability_effect);
	}
}

constexpr auto find_move(auto const moves, MoveName const move_name) -> Move {
	auto const maybe_move = containers::maybe_find(moves, move_name);
	BOUNDED_ASSERT(maybe_move);
	return *maybe_move;
}

auto handle_ability_blocks_move(any_mutable_active_pokemon auto const target, Environment const environment) {
	switch (target.ability()) {
		case Ability::Flash_Fire:
			target.activate_flash_fire();
			break;
		case Ability::Volt_Absorb:
			heal(target, environment, rational(1_bi, 4_bi));
			break;
		case Ability::Water_Absorb:
		case Ability::Dry_Skin:
			heal(target, environment, rational(1_bi, 4_bi));
			break;
		case Ability::Motor_Drive:
			target.stages()[BoostableStat::spe] += 1_bi;
			break;
		default:
			break;
	}
}

template<any_team UserTeam>
auto try_use_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherAction const other_action, Environment & environment, bool const clear_status, ActualDamage const actual_damage, bool const is_fully_paralyzed) -> void {
	constexpr auto generation = generation_from<UserTeam>;
	auto user_pokemon = user.pokemon();

	auto unsuccessfully_use_move = [&] {
		user_pokemon.unsuccessfully_use_move(move.executed);
	};

	auto const found_move = find_move(
		potentially_selectable_moves(generation, user.pokemon().regular_moves()),
		move.selected
	);
	auto other_pokemon = other.pokemon();
	auto const was_asleep = is_sleeping(user_pokemon.status());
	user_pokemon.advance_status_from_move(clear_status);
	// Need the side-effect from recharge
	auto const is_recharging = user_pokemon.recharge();
	if (!can_attempt_move_execution(user_pokemon.as_const(), found_move, other_pokemon.as_const(), was_asleep)) {
		unsuccessfully_use_move();
		return;
	}
	if (user_pokemon.flinched()) {
		if (boosts_speed_when_flinched(user_pokemon.ability())) {
			user_pokemon.stages()[BoostableStat::spe] += 1_bi;
		}
		return;
	}
	if (!can_execute_move(user_pokemon.as_const(), found_move, environment, is_recharging, is_fully_paralyzed)) {
		unsuccessfully_use_move();
		return;
	}
	user_pokemon.advance_confusion();

	auto const other_ability = other_pokemon.ability();

	if (!user_pokemon.last_used_move().locked_in()) {
		auto const uses_extra_pp = other_ability == Ability::Pressure;
		user_pokemon.reduce_pp(move.selected, environment, BOUNDED_CONDITIONAL(uses_extra_pp, 2_bi, 1_bi));
	}

	// TODO: What happens if we Sleep Talk Trump Card?
	// TODO: Make sure this does not happen because we missed due to a vanishing
	// state
	if (move.miss) {
		unsuccessfully_use_move();
		if (user_pokemon.item(environment) == Item::Blunder_Policy) {
			user_pokemon.stages()[BoostableStat::spe] += 2_bi;
			user_pokemon.remove_item();
		}
		return;
	}

	auto const target = move_target(generation, move.executed);

	if (move_is_unsuccessful(target, move.executed, user_pokemon.damaged(), other_pokemon.hp().current(), other_ability, other_action, other_pokemon.last_used_move().is_protecting())) {
		unsuccessfully_use_move();
		return;
	}

	auto const known_move = KnownMove{
		move.executed,
		move_type(generation, move.executed, get_hidden_power_type(user_pokemon))
	};

	auto const released_damage = user_pokemon.successfully_use_move(move.executed, other_ability, environment);

	if (ability_blocks_move(generation, other_ability, known_move, other_pokemon.status().name(), other_pokemon.types())) {
		handle_ability_blocks_move(other_pokemon, environment);
		return;
	}

	auto const executed_move = ExecutedMove<UserTeam>{
		known_move,
		found_move.pp(),
		move.side_effect,
		move.critical_hit,
		move.contact_ability_effect
	};
	use_move(user, executed_move, target, other, other_action, environment, actual_damage);
	if (released_damage) {
		other_pokemon.direct_damage(move.executed, user_pokemon, environment, *released_damage * 2_bi);
	}
}

template<any_team UserTeam>
auto call_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherAction const other_action, Environment & environment, bool const clear_status, ActualDamage const actual_damage, bool const is_fully_paralyzed) -> void {
	BOUNDED_ASSERT(user.pokemon().hp().current() != 0_bi);
	try_use_move(user, move, other, other_action, environment, clear_status, actual_damage, is_fully_paralyzed);
	end_of_attack(user.pokemon(), other.pokemon(), environment);
}

#define INSTANTIATE_ONE(UserTeam) \
	template auto call_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherAction const other_action, Environment & environment, bool const clear_status, ActualDamage const actual_damage, bool const is_fully_paralyzed) -> void

#define INSTANTIATE_ALL(generation) \
	INSTANTIATE_ONE(Team<generation>); \
	INSTANTIATE_ONE(KnownTeam<generation>); \
	INSTANTIATE_ONE(SeenTeam<generation>)

TM_FOR_EACH_GENERATION(INSTANTIATE_ALL);

} // namespace technicalmachine
