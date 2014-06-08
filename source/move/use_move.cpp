// Use moves
// Copyright (C) 2014 David Stone
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

#include "use_move.hpp"

#include <algorithm>
#include <cassert>

#include "move.hpp"
#include "moves.hpp"

#include "../ability.hpp"
#include "../block.hpp"
#include "../damage.hpp"
#include "../heal.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../stat/stat.hpp"

#include "../type/effectiveness.hpp"
#include "../type/type.hpp"

namespace technicalmachine {
namespace {

auto use_move(Team & user, Team & target, Weather & weather, Variable const & variable, bool damage_is_known) -> damage_type;
auto calculate_real_damage(Team const & user, Team const & target, Weather const & weather, Variable const & variable, bool const damage_is_known) -> damage_type;
auto call_other_move (ActivePokemon & user) -> void;
template<typename Predicate>
auto cure_all_status(Team & user, Predicate const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			get_status(pokemon) = Status{};
		}
	}
}
auto do_effects_before_moving (Pokemon & user, Team & target) -> void;
auto do_damage (ActivePokemon & user, ActivePokemon & target, damage_type damage) -> void;
auto do_side_effects (Team & user, Team & target, Weather & weather, Variable const & variable, damage_type damage) -> void;
auto absorb_hp(Pokemon & user, Pokemon const & target, damage_type damage) -> void;
auto belly_drum(ActivePokemon & user) -> void;
auto can_confuse_with_chatter(Species pokemon) -> bool;
auto clear_field(Team & user, Pokemon const & target) -> void;
auto confusing_stat_boost(ActivePokemon & target, StatNames stat, bounded::checked_integer<1, 2> stages) -> void;
auto curse(ActivePokemon & user, ActivePokemon & target) -> void;
auto equalize(HP & hp1, HP & hp2) -> void;
auto phaze(Team & user, Team & target, Weather & weather, Variable const & variable) -> void;
auto rest(Pokemon & user) -> void;
auto struggle(Pokemon & user) -> void;
auto swap_items(Pokemon & user, Pokemon & target) -> void;
auto tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather, Variable const & variable) -> void;
auto use_swallow(ActivePokemon & user) -> void;

template<Status::Statuses status>
auto fang_side_effects(Pokemon & user, ActivePokemon & target, Weather const & weather, Variable const & variable) {
	switch (variable.value().value()) {
		case 0:
			break;
		case 1:
			Status::apply<status>(user, target, weather);
			break;
		case 2:
			target.flinch();
			break;
		case 3:	
			Status::apply<status>(user, target, weather);
			target.flinch();
			break;
		default:
			assert(false);
			break;
	}
}

template<Status::Statuses status>
auto recoil_status(Pokemon & user, Pokemon & target, Weather const & weather, damage_type const damage, Variable const & variable) {
	recoil(user, damage, 3_bi);
	if (variable.effect_activates()) {
		Status::apply<status>(user, target, weather);
	}
}

auto calls_other_move(Moves const move) {
	switch (move) {
//		case Moves::Nature_Power:
		case Moves::Assist:
		case Moves::Copycat:
		case Moves::Me_First:
		case Moves::Metronome:
		case Moves::Mirror_Move:
		case Moves::Sleep_Talk:
			return true;
		default:
			return false;
	}
}

}	// namespace

auto call_move(Team & user_team, Team & target_team, Weather & weather, Variable const & variable, bool const damage_is_known) -> damage_type {
	auto & user = user_team.pokemon();
	auto const & target = target_team.pokemon();
	user.update_before_move();
	if (can_execute_move (user, target, weather)) {
		user.lower_pp(get_ability(target));
		if (calls_other_move(user.move())) {
			call_other_move (user);
		}
		if (!user.missed()) {
			return use_move(user_team, target_team, weather, variable, damage_is_known);
		}
	}
	return 0_bi;
}

namespace {

auto is_sound_based(Moves const move) {
	switch (move) {
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::GrassWhistle:
		case Moves::Growl:
		case Moves::Heal_Bell:
		case Moves::Hyper_Voice:
		case Moves::Metal_Sound:
		case Moves::Perish_Song:
		case Moves::Roar:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Snore:
		case Moves::Supersonic:
		case Moves::Uproar:
			return true;
		default:
			return false;
	}
}

auto use_move(Team & user, Team & target, Weather & weather, Variable const & variable, bool const damage_is_known) -> damage_type {
	Moves const move = user.pokemon().move();
	// TODO: Add targeting information and only block the move if the target is
	// immune.
	if (get_ability(target.pokemon()).blocks_sound_moves() and is_sound_based(move) and
			!(move == Moves::Heal_Bell or move == Moves::Perish_Song)) {
		return 0_bi;
	}

	do_effects_before_moving (user.pokemon(), target);

	auto const damage = calculate_real_damage(user, target, weather, variable, damage_is_known);
	do_damage (user.pokemon(), target.pokemon(), damage);
	user.pokemon().increment_move_use_counter();

	do_side_effects(user, target, weather, variable, damage);

	return damage;
}

constexpr auto breaks_screens(Moves const move) {
	return move == Moves::Brick_Break;
}

auto do_effects_before_moving (Pokemon & user, Team & target) -> void {
	if (breaks_screens(user.move())) {
		target.screens.shatter();
	}
	else if (is_usable_while_frozen(user.move())) {
		if (is_frozen(get_status(user))) {
			get_status(user) = Status{};
		}
	}
}

auto calculate_real_damage(Team const & user, Team const & target, Weather const & weather, Variable const & variable, bool const damage_is_known) -> damage_type {
	if (!is_damaging(user.pokemon().move())) {
		return 0_bi;
	}
	if (damage_is_known) {
		return target.pokemon().damaged();
	}

	return damage_calculator(user, target, weather, variable);
}

auto do_damage(ActivePokemon & user, ActivePokemon & target, damage_type const damage) -> void {
	if (damage == 0_bi) {
		return;
	}
	target.direct_damage(damage);
	if (causes_recoil(get_item(user))) {
		heal(user, make_rational(-1_bi, 10_bi));
	}
}

auto do_side_effects(Team & user_team, Team & target_team, Weather & weather, Variable const & variable, damage_type const damage) -> void {
	auto & target = target_team.pokemon();
	auto & user = user_team.pokemon();
	Moves const move = user.move();
	switch (move) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			absorb_hp(user, target, damage);
			break;
		case Moves::Acid:
		case Moves::Bug_Buzz:
		case Moves::Earth_Power:
		case Moves::Energy_Ball:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
		case Moves::Luster_Purge:
		case Moves::Psychic:
		case Moves::Shadow_Ball:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::SPD, -1_bi);
			}
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			boost(user.stage(), StatNames::DEF, 2_bi);
			break;
		case Moves::Acupressure:
			boost(user.stage(), static_cast<StatNames>(variable.value()), 2_bi);
			break;
		case Moves::Agility:
		case Moves::Rock_Polish:
			boost(user.stage(), StatNames::SPE, 2_bi);
			break;
		case Moves::Air_Slash:
		case Moves::Astonish:
		case Moves::Bite:
		case Moves::Bone_Club:
		case Moves::Dark_Pulse:
		case Moves::Dragon_Rush:
		case Moves::Extrasensory:
		case Moves::Fake_Out:
		case Moves::Headbutt:
		case Moves::Hyper_Fang:
		case Moves::Iron_Head:
		case Moves::Needle_Arm:
		case Moves::Rock_Slide:
		case Moves::Rolling_Kick:
		case Moves::Snore:
		case Moves::Stomp:
		case Moves::Twister:
		case Moves::Waterfall:
		case Moves::Zen_Headbutt:
			if (variable.effect_activates()) {
				target.flinch();
			}
			break;
		case Moves::Amnesia:
			boost(user.stage(), StatNames::SPD, 2_bi);
			break;
		case Moves::AncientPower:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			if (variable.effect_activates()) {
				boost_regular(user.stage(), 1_bi);
			}
			break;
		case Moves::Aqua_Ring:
			user.activate_aqua_ring();
			break;
		case Moves::Aurora_Beam:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::ATK, -1_bi);
			}
			break;
		case Moves::Aromatherapy:
			cure_all_status(user_team, [](Pokemon const &) { return true; });
			break;
		case Moves::Attract:
			if (multiplier(get_gender(user), get_gender(target)) == -1_bi) {
				target.attract();
			}
			break;
		case Moves::Baton_Pass:
			user.baton_pass();
			break;
		case Moves::Belly_Drum:
			belly_drum(user);
			break;
		case Moves::Bide:
			user.use_bide(target);
			break;
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Magma_Storm:
		case Moves::Sand_Tomb:
		case Moves::Whirlpool:
		case Moves::Wrap:
			target.partially_trap();
			break;
		case Moves::Blast_Burn:
		case Moves::Frenzy_Plant:
		case Moves::Giga_Impact:
		case Moves::Hydro_Cannon:
		case Moves::Hyper_Beam:
		case Moves::Roar_of_Time:
		case Moves::Rock_Wrecker:
			user.use_recharge_move();
			break;
		case Moves::Blaze_Kick:
		case Moves::Ember:
		case Moves::Fire_Blast:
		case Moves::Fire_Punch:
		case Moves::Flame_Wheel:
		case Moves::Flamethrower:
		case Moves::Heat_Wave:
		case Moves::Lava_Plume:
		case Moves::Sacred_Fire:
			if (variable.effect_activates()) {
				Status::apply<Status::burn>(user, target, weather);
			}
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (variable.effect_activates()) {
				Status::apply<Status::freeze>(user, target, weather);
			}
			break;
		case Moves::Block:
		case Moves::Mean_Look:
		case Moves::Spider_Web:
			target.fully_trap();
			break;
		case Moves::Body_Slam:
		case Moves::Discharge:
		case Moves::DragonBreath:
		case Moves::Force_Palm:
		case Moves::Lick:
		case Moves::Secret_Power:
		case Moves::Spark:
		case Moves::Thunder:
		case Moves::Thunderbolt:
		case Moves::ThunderPunch:
		case Moves::ThunderShock:
			if (variable.effect_activates()) {
				Status::apply<Status::paralysis>(user, target, weather);
			}
			break;
		case Moves::Bounce: {
			bool const vanished = user.bounce();
			if (vanished and variable.effect_activates()) {
				Status::apply<Status::paralysis>(user, target, weather);
			}
			break;
		}
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			recoil(user, damage, 3_bi);
			break;
		case Moves::Bubble:
		case Moves::BubbleBeam:
		case Moves::Constrict:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::SPE, -1_bi);
			}
			break;
		case Moves::Bug_Bite:			// Fix
		case Moves::Pluck:
			break;
		case Moves::Bulk_Up:
			boost_physical(user.stage(), 1_bi);
			break;
		case Moves::Calm_Mind:
			boost_special(user.stage(), 1_bi);
			break;
		case Moves::Camouflage:
			break;
		case Moves::Captivate:
			if (multiplier(get_gender(user), get_gender(target)) == -1_bi) {
				boost(target.stage(), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Charge:
			user.charge();
			boost(user.stage(), StatNames::SPD, 1_bi);
			break;
		case Moves::Charge_Beam:
			if (variable.effect_activates()) {
				boost(user.stage(), StatNames::SPA, 1_bi);
			}
			break;
		case Moves::Charm:
		case Moves::FeatherDance:
			boost(target.stage(), StatNames::ATK, -2_bi);
			break;
		case Moves::Chatter:
			if (can_confuse_with_chatter(user) and variable.effect_activates()) {
				target.confuse();
			}
			break;
		case Moves::Close_Combat:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Confuse_Ray:
		case Moves::Supersonic:
		case Moves::Sweet_Kiss:
		case Moves::Teeter_Dance:
			target.confuse();
			break;
		case Moves::Confusion:
		case Moves::Dizzy_Punch:
		case Moves::DynamicPunch:
		case Moves::Psybeam:
		case Moves::Rock_Climb:
		case Moves::Signal_Beam:
		case Moves::Water_Pulse:
			if (variable.effect_activates()) {
				target.confuse();
			}
			break;
		case Moves::Conversion:		// Fix
			break;
		case Moves::Conversion_2:	// Fix
			break;
		case Moves::Cosmic_Power:
		case Moves::Defend_Order:
			boost_defensive(user.stage(), 1_bi);
			break;
		case Moves::Crunch:
		case Moves::Crush_Claw:
		case Moves::Iron_Tail:
		case Moves::Rock_Smash:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::DEF, -1_bi);
			}
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			boost(target.stage(), StatNames::SPE, -2_bi);
			break;
		case Moves::Counter:
			if (is_physical(target.move())) {
				target.indirect_damage(user.damaged() * 2_bi);
			}
			break;
		case Moves::Covet:
		case Moves::Thief:
			steal(get_item(user), get_item(target));
			break;
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smog:
			if (variable.effect_activates()) {
				Status::apply<Status::poison>(user, target, weather);
			}
			break;
		case Moves::Curse:
			curse(user, target);
			break;
		case Moves::Dark_Void:
		case Moves::GrassWhistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
		case Moves::Sleep_Powder:
		case Moves::Spore:
			Status::apply<Status::sleep>(user, target, weather);
			break;
		case Moves::Defense_Curl:
			boost(user.stage(), StatNames::DEF, 1_bi);
			user.defense_curl();
			break;
		case Moves::Defog:
			weather.fog = false;
			boost(target.stage(), StatNames::EVA, -1_bi);
			break;
		case Moves::Destiny_Bond:
			user.use_destiny_bond();
			break;
		case Moves::Detect:
		case Moves::Protect:
			user.protect();
			break;
		case Moves::Dig:
			user.dig();
			break;
		case Moves::Disable:
			target.disable();
			break;
		case Moves::Dive:
			user.dive();
			break;
		case Moves::Doom_Desire:	// Fix
		case Moves::Future_Sight:
			break;
		case Moves::Double_Team:
		case Moves::Minimize:
			boost(user.stage(), StatNames::EVA, 1_bi);
			break;
		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			boost(user.stage(), StatNames::SPA, -2_bi);
			break;
		case Moves::Dragon_Dance:
			boost(user.stage(), StatNames::ATK, 1_bi);
			boost(user.stage(), StatNames::SPE, 1_bi);
			break;
		case Moves::Dream_Eater:
			if (is_sleeping(get_status(target))) {
				absorb_hp(user, target, damage);
			}
			break;
		case Moves::Embargo:
			target.activate_embargo();
			break;
		case Moves::Encore:
			target.activate_encore();
			break;
		case Moves::Endure:
			user.endure();
			break;
		case Moves::Explosion:
		case Moves::Selfdestruct:
			user.faint();
			break;
		case Moves::Fake_Tears:
		case Moves::Metal_Sound:
			boost(target.stage(), StatNames::SPD, -2_bi);
			break;
		case Moves::Feint:
			target.break_protect();
			break;
		case Moves::Fire_Fang:
			fang_side_effects<Status::burn>(user, target, weather, variable);
			break;
		case Moves::Flare_Blitz:
			recoil_status<Status::burn>(user, target, weather, damage, variable);
			break;
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::SmokeScreen:
			boost(target.stage(), StatNames::ACC, -1_bi);
			break;
		case Moves::Flatter:
			confusing_stat_boost(target, StatNames::SPA, 1_bi);
			break;
		case Moves::Fling:
			remove(get_item(user));
			break;
		case Moves::Fly:
			user.fly();
			break;
		case Moves::Focus_Energy:
			user.focus_energy();
			break;
		case Moves::Focus_Punch:		// Fix
			break;
		case Moves::Follow_Me:		// Fix
			break;
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
			target.identify();
			break;
		case Moves::Gastro_Acid:		// Fix
			break;
		case Moves::Glare:
		case Moves::Stun_Spore:
		case Moves::Thunder_Wave:
		case Moves::Zap_Cannon:
			Status::apply<Status::paralysis>(user, target, weather);
			break;
		case Moves::Gravity:
			weather.set_gravity();
			break;
		case Moves::Growl:
			boost(target.stage(), StatNames::ATK, -1_bi);
			break;
		case Moves::Growth:
			boost(user.stage(), StatNames::SPA, 1_bi);
			break;
		case Moves::Grudge:		// Fix
			break;
		case Moves::Guard_Swap:
			swap_defensive(user.stage(), target.stage());
			break;
		case Moves::Hail:
			weather.set_hail(extends_hail(get_item(user)));
			break;
		case Moves::Hammer_Arm:
			boost(user.stage(), StatNames::SPE, -1_bi);
			break;
		case Moves::Harden:
		case Moves::Withdraw:
			boost(user.stage(), StatNames::DEF, 1_bi);
			break;
		case Moves::Haze:
			user.stage() = Stage{};
			target.stage() = Stage{};
			break;
		case Moves::Head_Smash:
			recoil(user, damage, 2_bi);
			break;
		case Moves::Heal_Bell:
			cure_all_status(user_team, [](Pokemon const & pokemon) {
				return !get_ability(pokemon).blocks_sound_moves();
			});
			break;
		case Moves::Heal_Block:
			target.activate_heal_block();
			break;
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Recover:
		case Moves::Slack_Off:
		case Moves::Softboiled:
			heal(user, make_rational(1_bi, 2_bi));
			break;
		case Moves::Healing_Wish:		// Fix
			break;
		case Moves::Heart_Swap: {
			using std::swap;
			swap(user.stage(), target.stage());
			break;
		}
		case Moves::Hi_Jump_Kick:		// Fix
		case Moves::Jump_Kick:
			break;
		case Moves::Howl:
		case Moves::Metal_Claw:
		case Moves::Meditate:
		case Moves::Meteor_Mash:
			if (variable.effect_activates()) {
				boost(user.stage(), StatNames::ATK, 1_bi);
			}
			break;
		case Moves::Ice_Ball:		// Fix
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			fang_side_effects<Status::freeze>(user, target, weather, variable);
			break;
		case Moves::Icy_Wind:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
		case Moves::String_Shot:
			boost(target.stage(), StatNames::SPE, -1_bi);
			break;
		case Moves::Imprison:
			user.imprison();
			break;
		case Moves::Ingrain:
			user.ingrain();
			break;
		case Moves::Knock_Off:		// Fix
			break;
		case Moves::Leech_Seed:
			target.hit_with_leech_seed();
			break;
		case Moves::Leer:
		case Moves::Tail_Whip:
			boost(target.stage(), StatNames::DEF, -1_bi);
			break;
		case Moves::Light_Screen:
			user_team.screens.activate_light_screen(extends_light_screen(get_item(user)));
			break;
		case Moves::Lock_On:
		case Moves::Mind_Reader:
			user.lock_on_to();
			break;
		case Moves::Lucky_Chant:
			user_team.screens.activate_lucky_chant();
			break;
		case Moves::Lunar_Dance:		// Fix
			break;
		case Moves::Magic_Coat:		// Fix
			break;
		case Moves::Magnet_Rise:
			user.activate_magnet_rise();
			break;
		case Moves::Me_First:		// Fix
			break;
		case Moves::Memento:
			boost_offensive(target.stage(), -2_bi);
			user.faint();
			break;
		case Moves::Metal_Burst:
			target.indirect_damage(user.damaged() * 3_bi / 2_bi);
			break;
		case Moves::Mimic:		// Fix
			break;
		case Moves::Miracle_Eye:		// Fix
			break;
		case Moves::Mirror_Coat:
			if (is_special(target.move())) {
				target.indirect_damage(user.damaged() * 2_bi);
			}
			break;
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Octazooka:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::ACC, -1_bi);
			}
			break;
		case Moves::Mist:
			user_team.screens.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::SPA, -1_bi);
			}
			break;
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis: {
			auto const amount = [& weather]() {
				using Numerator = bounded::integer<1, 2>;
				using Denominator = bounded::integer<2, 4>;
				using Result = bounded_rational<Numerator, Denominator>;
				if (weather.sun()) {
					return Result(2_bi, 3_bi);
				} else if (weather.hail() or weather.rain() or weather.sand()) {
					return Result(1_bi, 4_bi);
				} else {
					return Result(1_bi, 2_bi);
				}
			};
			heal(user, amount());
			break;
		}
		case Moves::Mud_Sport:
			user.activate_mud_sport();
			break;
		case Moves::Nasty_Plot:
		case Moves::Tail_Glow:
			boost(user.stage(), StatNames::SPA, 2_bi);
			break;
		case Moves::Nightmare:
			target.give_nightmares();
			break;
		case Moves::Outrage:
		case Moves::Petal_Dance:
		case Moves::Thrash:
			user.activate_rampage();
			break;
		case Moves::Pain_Split:
			equalize(get_hp(user), get_hp(target));
			break;
		case Moves::Perish_Song:
			user.activate_perish_song();
			target.activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (variable.effect_activates()) {
				Status::apply<Status::poison_toxic>(user, target, weather);
			}
			break;
		case Moves::Poison_Gas:
		case Moves::PoisonPowder:
			Status::apply<Status::poison>(user, target, weather);
			break;
		case Moves::Power_Swap:
			swap_offensive(user.stage(), target.stage());
			break;
		case Moves::Power_Trick:
			user.activate_power_trick();
			break;
		case Moves::Present:
			if (variable.present_heals()) {
				get_hp(target) += 80_bi;
			}
			break;
		case Moves::Psych_Up:
			user.stage() = target.stage();
			break;
		case Moves::Psycho_Shift:
			if (is_clear(get_status(target))) {
				Status::shift(user, target, weather);
			}
			break;
		case Moves::Rage:		// Fix
			break;
		case Moves::Rain_Dance:
			weather.set_rain(extends_rain(get_item(user)));
			break;
		case Moves::Rapid_Spin:
			clear_field(user_team, target);
			break;
		case Moves::Razor_Wind:	// Fix
			break;
		case Moves::Recycle:		// Fix
			break;
		case Moves::Reflect:
			user_team.screens.activate_reflect(extends_reflect(get_item(user)));
			break;
		case Moves::Refresh:
			get_status(user) = Status{};
			break;
		case Moves::Rest:
			rest(user);
			break;
		case Moves::Roar:
		case Moves::Whirlwind:
			phaze(user_team, target_team, weather, variable);
			break;
		case Moves::Role_Play:		// Fix
			break;
		case Moves::Roost:
			user.roost();
			heal(user, make_rational(1_bi, 2_bi));
			break;
		case Moves::Safeguard:
			user_team.screens.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.set_sand(extends_sand(get_item(user)));
			break;
		case Moves::Screech:
			boost(target.stage(), StatNames::DEF, -2_bi);
			break;
		case Moves::Seed_Flare:
			if (variable.effect_activates()) {
				boost(target.stage(), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Shadow_Force:
			user.shadow_force();
			break;
		case Moves::Sharpen:
			boost(user.stage(), StatNames::ATK, 1_bi);
			break;
		case Moves::Sketch:		// Fix
			break;
		case Moves::Skill_Swap:		// Fix
			break;
		case Moves::Skull_Bash: // Fix
			break;
		case Moves::Sky_Attack:
			if (variable.effect_activates()) {
				target.flinch();
			}
			break;
		case Moves::SmellingSalt:
			if (boosts_smellingsalt(get_status(target))) {
				get_status(target) = Status{};
			}
			break;
		case Moves::Snatch:	// Fix
			break;
		case Moves::SolarBeam:		// Fix
			break;
		case Moves::Spikes:
			target_team.entry_hazards.add_spikes();
			break;
		case Moves::Spit_Up:
			user.release_stockpile();
			break;
		case Moves::Spite:		// Fix
			break;
		case Moves::Stealth_Rock:
			target_team.entry_hazards.add_stealth_rock();
			break;
		case Moves::Steel_Wing:
			if (variable.effect_activates()) {
				boost(user.stage(), StatNames::DEF, 1_bi);
			}
			break;
		case Moves::Stockpile:
			user.increment_stockpile();
			break;
		case Moves::Struggle:
			struggle(user);
			break;
		case Moves::Submission:
		case Moves::Take_Down:
			recoil(user, damage, 4_bi);
			break;
		case Moves::Substitute:
			user.use_substitute();		
			break;
		case Moves::Sunny_Day:
			weather.set_sun(extends_sun(get_item(user)));
			break;
		case Moves::Superpower:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Swagger:
			confusing_stat_boost(target, StatNames::ATK, 2_bi);
			break;
		case Moves::Swallow:
			use_swallow(user);
			break;
		case Moves::Sweet_Scent:
			boost(target.stage(), StatNames::EVA, -1_bi);
			break;
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			user_team.all_pokemon().replacement_from_switch();
			switchpokemon (user_team, target_team, weather);
			break;
		case Moves::Switcheroo:
		case Moves::Trick:
			swap_items(user, target);
			break;
		case Moves::Swords_Dance:
			boost(user.stage(), StatNames::ATK, 2_bi);
			break;
		case Moves::Tailwind:
			user_team.screens.activate_tailwind();
			break;
		case Moves::Taunt:
			target.taunt();
			break;
		case Moves::Thunder_Fang:
			fang_side_effects<Status::paralysis>(user, target, weather, variable);
			break;
		case Moves::Tickle:
			boost_physical(target.stage(), -1_bi);
			break;
		case Moves::Torment:
			target.torment();
			break;
		case Moves::Toxic:
			Status::apply<Status::poison_toxic>(user, target, weather);
			break;
		case Moves::Toxic_Spikes:
			target_team.entry_hazards.add_toxic_spikes();
			break;
		case Moves::Transform:		// Fix
			break;
		case Moves::Tri_Attack:
			tri_attack_status(user, target, weather, variable);
			break;
		case Moves::Trick_Room:
			weather.set_trick_room ();
			break;
		case Moves::U_turn:
			user.u_turn();
			break;
		case Moves::Uproar:
			// TODO: make this make sense
			weather.set_uproar (static_cast<int8_t> (variable.value()));
			user.use_uproar();
			break;
		case Moves::Volt_Tackle:
			recoil_status<Status::paralysis>(user, target, weather, damage, variable);
			break;
		case Moves::Wake_Up_Slap:
			if (is_sleeping(get_status(target))) {
				get_status(target) = Status{};
			}
			break;
		case Moves::Water_Sport:
			user.activate_water_sport();
			break;
		case Moves::Will_O_Wisp:
			Status::apply<Status::burn>(user, target, weather);
			break;
		case Moves::Wish:
			user_team.wish.activate();
			break;
		case Moves::Worry_Seed:		// Fix
			break;
		case Moves::Yawn:
			target.hit_with_yawn();
			break;
		default:
			break;
	}
}

// I could potentially treat this as negative recoil
auto absorb_hp(Pokemon & user, Pokemon const & target, damage_type const damage) -> void {
	if (get_ability(target).damages_leechers()) {
		get_hp(user) -= damage / 2_bi;
	}
	else {
		get_hp(user) += damage / 2_bi;
	}
}

auto belly_drum(ActivePokemon & user) -> void {
	HP & hp = get_hp(user);
	if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
		hp -= hp.max() / 2_bi;
		boost(user.stage(), StatNames::ATK, 12_bi);
	}
}

auto can_confuse_with_chatter(Species const pokemon) -> bool {
	return pokemon == Species::Chatot;
}

auto clear_field(Team & user, Pokemon const & target) -> void {
	auto const type = get_type(user.pokemon().move(), user.pokemon());
	if (!Effectiveness(type, target).has_no_effect()) {
		user.clear_field();
	}
}

auto confusing_stat_boost(ActivePokemon & target, StatNames const stat, bounded::checked_integer<1, 2> const stages) -> void {
	boost(target.stage(), stat, stages);
	target.confuse();
}

auto curse(ActivePokemon & user, ActivePokemon & target) -> void {
	if (is_type(user, Type::Ghost) and !get_ability(user).blocks_secondary_damage()) {
		if (!target.is_cursed()) {
			user.indirect_damage(get_hp(user).max() / 2_bi);
			target.curse();
		}
	}
	else {
		boost_physical(user.stage(), 1_bi);
		boost(user.stage(), StatNames::SPE, -1_bi);
	}
}

auto equalize(HP & hp1, HP & hp2) -> void {
	HP::current_type const temp = (hp1.current() + hp2.current()) / 2_bi;
	hp1 = temp;
	hp2 = temp;
}

auto phaze(Team & user, Team & target, Weather & weather, Variable const & variable) -> void {
	if (target.pokemon().can_be_phazed()) {
		target.all_pokemon().set_replacement(variable.phaze_index(target.pokemon().index()));
		switchpokemon(target, user, weather);
		target.move();
	}
}

auto rest(Pokemon & user) -> void {
	HP & hp = get_hp(user);
	if (hp.current() != hp.max()) {
		hp = hp.max();
		get_status(user).rest();
	}
}

auto struggle(Pokemon & user) -> void {
	get_hp(user) -= get_hp(user).max() / 4_bi;
}

auto swap_items(Pokemon & user, Pokemon & target) -> void {
	// Add support for abilities that block Trick / Switcheroo
	if (!blocks_trick(get_item(user)) and !blocks_trick(get_item(target))) {
		using std::swap;
		swap(get_item(user), get_item(target));
	}
}

auto tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather, Variable const & variable) -> void {
	switch (variable.value().value()) {
		case 1:
			Status::apply<Status::burn>(user, target, weather);
			break;
		case 2:
			Status::apply<Status::freeze>(user, target, weather);
			break;
		case 3:
			Status::apply<Status::paralysis>(user, target, weather);
			break;
		default:
			assert(false);
			break;
	}
}

auto use_swallow(ActivePokemon & user) -> void {
	auto const stockpiles = user.release_stockpile();
	if (stockpiles == 0_bi) {
		return;
	}
	heal(user, swallow_healing(bounded::integer<1, Stockpile::max>(stockpiles)));
}

auto call_other_move (ActivePokemon & user) -> void {
	// TODO: implement
}

}	// unnamed namespace
}	// namespace technicalmachine
