// Use moves
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

#include "use_move.hpp"

#include <algorithm>
#include <cassert>

#include "is_switch.hpp"
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

// I could potentially treat this as negative recoil
auto absorb_hp(Pokemon & user, Pokemon const & target, damage_type const damage) -> void {
	auto & hp = get_hp(user);
	auto const absorbed = damage / 2_bi;
	if (damages_leechers(get_ability(target))) {
		hp -= absorbed;
	} else {
		hp += absorbed;
	}
}


template<typename Predicate>
auto cure_all_status(Team & user, Predicate const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			get_status(pokemon) = Status{};
		}
	}
}


auto belly_drum(MutableActivePokemon user) {
	HP & hp = get_hp(user);
	if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
		hp -= hp.max() / 2_bi;
		boost(stage(user), StatNames::ATK, 12_bi);
	}
}


auto can_confuse_with_chatter(Species const pokemon) {
	return pokemon == Species::Chatot;
}


auto curse(MutableActivePokemon user, MutableActivePokemon target) {
	if (is_type(user, Type::Ghost, is_roosting(user)) and !blocks_secondary_damage(get_ability(user))) {
		if (!is_cursed(target)) {
			user.indirect_damage(get_hp(user).max() / 2_bi);
			target.curse();
		}
	} else {
		auto & stat_stage = stage(user);
		boost_physical(stat_stage, 1_bi);
		boost(stat_stage, StatNames::SPE, -1_bi);
	}
}


auto fang_side_effects(Pokemon & user, MutableActivePokemon target, Weather const weather, Variable const & variable, Statuses const status) {
	switch (variable.value.value()) {
		case 0:
			break;
		case 1:
			apply(status, user, target, weather);
			break;
		case 2:
			target.flinch();
			break;
		case 3:	
			apply(status, user, target, weather);
			target.flinch();
			break;
		default:
			assert(false);
			break;
	}
}


auto recoil_status(Pokemon & user, Pokemon & target, Weather const weather, damage_type const damage, Variable const & variable, Statuses const status) {
	recoil(user, damage, 3_bi);
	if (effect_activates(variable)) {
		apply(status, user, target, weather);
	}
}


auto confusing_stat_boost(MutableActivePokemon target, StatNames const stat, bounded::checked_integer<1, 2> const stages) {
	boost(stage(target), stat, stages);
	target.confuse();
}


auto equalize(HP & hp1, HP & hp2) {
	auto const temp = (hp1.current() + hp2.current()) / 2_bi;
	hp1 = temp;
	hp2 = temp;
}


auto clear_field(Team & user, Moves const move, Pokemon const & target) {
	auto const type = get_type(move, user.pokemon());
	if (!Effectiveness(type, target).has_no_effect()) {
		user.clear_field();
	}
}


auto active_pokemon_can_be_phazed(Team const & team) {
	return !ingrained(team.pokemon()) and !blocks_phazing(get_ability(team.pokemon())) and size(team.all_pokemon()) > 1_bi;
}

auto phaze(Team & user, Team & target, Weather & weather, Variable const & variable) {
	if (active_pokemon_can_be_phazed(target)) {
		switch_pokemon(target, user, weather, phaze_index(variable, target.all_pokemon().index()));
		target.move();
	}
}


auto rest(Pokemon & user) {
	HP & hp = get_hp(user);
	if (hp.current() != hp.max()) {
		hp = hp.max();
		get_status(user).rest();
	}
}


auto struggle(Pokemon & user) {
	auto & hp = get_hp(user);
	hp -= hp.max() / 4_bi;
}


auto use_swallow(MutableActivePokemon user) {
	auto const stockpiles = user.release_stockpile();
	if (stockpiles == 0_bi) {
		return;
	}
	heal(user, swallow_healing(bounded::integer<1, Stockpile::max>(stockpiles)));
}


auto swap_items(Pokemon & user, Pokemon & target) {
	// Add support for abilities that block Trick / Switcheroo
	if (!blocks_trick(get_item(user)) and !blocks_trick(get_item(target))) {
		using std::swap;
		swap(get_item(user), get_item(target));
	}
}


auto tri_attack_status(Variable const & variable) {
	switch (variable.value.value()) {
		case 1: return Statuses::burn;
		case 2: return Statuses::freeze;
		case 3: return Statuses::paralysis;
		default: assert(false);
	}
}


auto do_side_effects(Team & user_team, Moves const move, Team & target, bounded::optional<UsedMove> const target_move, Weather & weather, Variable const & variable, damage_type const damage) {
	auto user = user_team.pokemon();
	switch (move) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			absorb_hp(user, target.pokemon(), damage);
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
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::SPD, -1_bi);
			}
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			boost(stage(user), StatNames::DEF, 2_bi);
			break;
		case Moves::Acupressure:
			boost(stage(user), static_cast<StatNames>(variable.value), 2_bi);
			break;
		case Moves::Agility:
		case Moves::Rock_Polish:
			boost(stage(user), StatNames::SPE, 2_bi);
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
			if (effect_activates(variable)) {
				target.pokemon().flinch();
			}
			break;
		case Moves::Amnesia:
			boost(stage(user), StatNames::SPD, 2_bi);
			break;
		case Moves::AncientPower:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			if (effect_activates(variable)) {
				boost_regular(stage(user), 1_bi);
			}
			break;
		case Moves::Aqua_Ring:
			user.activate_aqua_ring();
			break;
		case Moves::Aurora_Beam:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::ATK, -1_bi);
			}
			break;
		case Moves::Aromatherapy:
			cure_all_status(user_team, [](Pokemon const &) { return true; });
			break;
		case Moves::Attract:
			if (multiplier(get_gender(user), get_gender(target.pokemon())) == -1_bi) {
				target.pokemon().attract();
			}
			break;
		case Moves::Baton_Pass:
			user.baton_pass();
			break;
		case Moves::Belly_Drum:
			belly_drum(user);
			break;
		case Moves::Bide:
			user.use_bide(target.pokemon());
			break;
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Magma_Storm:
		case Moves::Sand_Tomb:
		case Moves::Whirlpool:
		case Moves::Wrap:
			target.pokemon().partially_trap();
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
			if (effect_activates(variable)) {
				apply(Statuses::burn, user, target.pokemon(), weather);
			}
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (effect_activates(variable)) {
				apply(Statuses::freeze, user, target.pokemon(), weather);
			}
			break;
		case Moves::Block:
		case Moves::Mean_Look:
		case Moves::Spider_Web:
			target.pokemon().fully_trap();
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
			if (effect_activates(variable)) {
				apply(Statuses::paralysis, user, target.pokemon(), weather);
			}
			break;
		case Moves::Bounce:
			if (user.bounce() and effect_activates(variable)) {
				apply(Statuses::paralysis, user, target.pokemon(), weather);
			}
			break;
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			recoil(user, damage, 3_bi);
			break;
		case Moves::Bubble:
		case Moves::BubbleBeam:
		case Moves::Constrict:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::SPE, -1_bi);
			}
			break;
		case Moves::Bug_Bite:			// Fix
		case Moves::Pluck:
			break;
		case Moves::Bulk_Up:
			boost_physical(stage(user), 1_bi);
			break;
		case Moves::Calm_Mind:
			boost_special(stage(user), 1_bi);
			break;
		case Moves::Camouflage:
			break;
		case Moves::Captivate:
			if (multiplier(get_gender(user), get_gender(target.pokemon())) == -1_bi) {
				boost(stage(target.pokemon()), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Charge:
			user.charge();
			boost(stage(user), StatNames::SPD, 1_bi);
			break;
		case Moves::Charge_Beam:
			if (effect_activates(variable)) {
				boost(stage(user), StatNames::SPA, 1_bi);
			}
			break;
		case Moves::Charm:
		case Moves::FeatherDance:
			boost(stage(target.pokemon()), StatNames::ATK, -2_bi);
			break;
		case Moves::Chatter:
			if (can_confuse_with_chatter(user) and effect_activates(variable)) {
				target.pokemon().confuse();
			}
			break;
		case Moves::Close_Combat:
			boost_physical(stage(user), -1_bi);
			break;
		case Moves::Confuse_Ray:
		case Moves::Supersonic:
		case Moves::Sweet_Kiss:
		case Moves::Teeter_Dance:
			target.pokemon().confuse();
			break;
		case Moves::Confusion:
		case Moves::Dizzy_Punch:
		case Moves::DynamicPunch:
		case Moves::Psybeam:
		case Moves::Rock_Climb:
		case Moves::Signal_Beam:
		case Moves::Water_Pulse:
			if (effect_activates(variable)) {
				target.pokemon().confuse();
			}
			break;
		case Moves::Conversion:		// Fix
			break;
		case Moves::Conversion_2:	// Fix
			break;
		case Moves::Cosmic_Power:
		case Moves::Defend_Order:
			boost_defensive(stage(user), 1_bi);
			break;
		case Moves::Crunch:
		case Moves::Crush_Claw:
		case Moves::Iron_Tail:
		case Moves::Rock_Smash:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::DEF, -1_bi);
			}
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			boost(stage(target.pokemon()), StatNames::SPE, -2_bi);
			break;
		case Moves::Counter:
			if (target_move and is_physical(target_move->move)) {
				target.pokemon().direct_damage(static_cast<damage_type>(target_move->damage * 2_bi));
			}
			break;
		case Moves::Covet:
		case Moves::Thief:
			steal(get_item(user), get_item(target.pokemon()));
			break;
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smog:
			if (effect_activates(variable)) {
				apply(Statuses::poison, user, target.pokemon(), weather);
			}
			break;
		case Moves::Curse:
			curse(user, target.pokemon());
			break;
		case Moves::Dark_Void:
		case Moves::GrassWhistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
		case Moves::Sleep_Powder:
		case Moves::Spore:
			apply(Statuses::sleep, user, target.pokemon(), weather);
			break;
		case Moves::Defense_Curl:
			boost(stage(user), StatNames::DEF, 1_bi);
			user.defense_curl();
			break;
		case Moves::Defog:
			weather.deactivate_fog();
			boost(stage(target.pokemon()), StatNames::EVA, -1_bi);
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
			// TODO: This should work on a move the Pokemon used the previous
			// turn
			if (target_move) {
				target.pokemon().disable(target_move->move);
			}
			break;
		case Moves::Dive:
			user.dive();
			break;
		case Moves::Doom_Desire:	// Fix
		case Moves::Future_Sight:
			break;
		case Moves::Double_Team:
		case Moves::Minimize:
			boost(stage(user), StatNames::EVA, 1_bi);
			break;
		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			boost(stage(user), StatNames::SPA, -2_bi);
			break;
		case Moves::Dragon_Dance:
			boost(stage(user), StatNames::ATK, 1_bi);
			boost(stage(user), StatNames::SPE, 1_bi);
			break;
		case Moves::Dream_Eater:
			if (is_sleeping(get_status(target.pokemon()))) {
				absorb_hp(user, target.pokemon(), damage);
			}
			break;
		case Moves::Embargo:
			target.pokemon().activate_embargo();
			break;
		case Moves::Encore:
			target.pokemon().activate_encore();
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
			boost(stage(target.pokemon()), StatNames::SPD, -2_bi);
			break;
		case Moves::Feint:
			target.pokemon().break_protect();
			break;
		case Moves::Fire_Fang:
			fang_side_effects(user, target.pokemon(), weather, variable, Statuses::burn);
			break;
		case Moves::Flare_Blitz:
			recoil_status(user, target.pokemon(), weather, damage, variable, Statuses::burn);
			break;
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::SmokeScreen:
			boost(stage(target.pokemon()), StatNames::ACC, -1_bi);
			break;
		case Moves::Flatter:
			confusing_stat_boost(target.pokemon(), StatNames::SPA, 1_bi);
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
			target.pokemon().identify();
			break;
		case Moves::Gastro_Acid:		// Fix
			break;
		case Moves::Glare:
		case Moves::Stun_Spore:
		case Moves::Thunder_Wave:
		case Moves::Zap_Cannon:
			apply(Statuses::paralysis, user, target.pokemon(), weather);
			break;
		case Moves::Gravity:
			weather.activate_gravity();
			break;
		case Moves::Growl:
			boost(stage(target.pokemon()), StatNames::ATK, -1_bi);
			break;
		case Moves::Growth:
			boost(stage(user), StatNames::SPA, 1_bi);
			break;
		case Moves::Grudge:		// Fix
			break;
		case Moves::Guard_Swap:
			swap_defensive(stage(user), stage(target.pokemon()));
			break;
		case Moves::Hail:
			weather.activate_hail(extends_hail(get_item(user)));
			break;
		case Moves::Hammer_Arm:
			boost(stage(user), StatNames::SPE, -1_bi);
			break;
		case Moves::Harden:
		case Moves::Withdraw:
			boost(stage(user), StatNames::DEF, 1_bi);
			break;
		case Moves::Haze:
			stage(user) = Stage{};
			stage(target.pokemon()) = Stage{};
			break;
		case Moves::Head_Smash:
			recoil(user, damage, 2_bi);
			break;
		case Moves::Heal_Bell:
			cure_all_status(user_team, [](Pokemon const & pokemon) {
				return !blocks_sound_moves(get_ability(pokemon));
			});
			break;
		case Moves::Heal_Block:
			target.pokemon().activate_heal_block();
			break;
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Recover:
		case Moves::Slack_Off:
		case Moves::Softboiled:
			heal(user, rational(1_bi, 2_bi));
			break;
		case Moves::Healing_Wish:		// Fix
			break;
		case Moves::Heart_Swap:
			std::swap(stage(user), stage(target.pokemon()));
			break;
		case Moves::Hi_Jump_Kick:		// Fix
		case Moves::Jump_Kick:
			break;
		case Moves::Howl:
		case Moves::Metal_Claw:
		case Moves::Meditate:
		case Moves::Meteor_Mash:
			if (effect_activates(variable)) {
				boost(stage(user), StatNames::ATK, 1_bi);
			}
			break;
		case Moves::Ice_Ball:		// Fix
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			fang_side_effects(user, target.pokemon(), weather, variable, Statuses::freeze);
			break;
		case Moves::Icy_Wind:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
		case Moves::String_Shot:
			boost(stage(target.pokemon()), StatNames::SPE, -1_bi);
			break;
		case Moves::Imprison:
			user.use_imprison();
			break;
		case Moves::Ingrain:
			user.ingrain();
			break;
		case Moves::Knock_Off:		// Fix
			break;
		case Moves::Leech_Seed:
			target.pokemon().hit_with_leech_seed();
			break;
		case Moves::Leer:
		case Moves::Tail_Whip:
			boost(stage(target.pokemon()), StatNames::DEF, -1_bi);
			break;
		case Moves::Light_Screen:
			user_team.screens.activate_light_screen(extends_light_screen(get_item(user)));
			break;
		case Moves::Lock_On:
		case Moves::Mind_Reader:
			user.use_lock_on();
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
			boost_offensive(stage(target.pokemon()), -2_bi);
			user.faint();
			break;
		case Moves::Metal_Burst:
			if (target_move) {
				target.pokemon().direct_damage(static_cast<damage_type>(target_move->damage * 3_bi / 2_bi));
			}
			break;
		case Moves::Mimic:		// Fix
			break;
		case Moves::Miracle_Eye:		// Fix
			break;
		case Moves::Mirror_Coat:
			if (target_move and is_special(target_move->move)) {
				target.pokemon().direct_damage(static_cast<damage_type>(target_move->damage * 2_bi));
			}
			break;
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Octazooka:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::ACC, -1_bi);
			}
			break;
		case Moves::Mist:
			user_team.screens.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::SPA, -1_bi);
			}
			break;
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis: {
			auto const amount = [weather]() {
				using Numerator = bounded::integer<1, 2>;
				using Denominator = bounded::integer<2, 4>;
				using Result = rational<Numerator, Denominator>;
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
			boost(stage(user), StatNames::SPA, 2_bi);
			break;
		case Moves::Nightmare:
			target.pokemon().give_nightmares();
			break;
		case Moves::Outrage:
		case Moves::Petal_Dance:
		case Moves::Thrash:
			user.activate_rampage();
			break;
		case Moves::Pain_Split:
			equalize(get_hp(user), get_hp(target.pokemon()));
			break;
		case Moves::Perish_Song:
			user.activate_perish_song();
			target.pokemon().activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (effect_activates(variable)) {
				apply(Statuses::poison_toxic, user, target.pokemon(), weather);
			}
			break;
		case Moves::Poison_Gas:
		case Moves::PoisonPowder:
			apply(Statuses::poison, user, target.pokemon(), weather);
			break;
		case Moves::Power_Swap:
			swap_offensive(stage(user), stage(target.pokemon()));
			break;
		case Moves::Power_Trick:
			user.activate_power_trick();
			break;
		case Moves::Present:
			if (present_heals(variable)) {
				get_hp(target.pokemon()) += 80_bi;
			}
			break;
		case Moves::Psych_Up:
			stage(user) = stage(target.pokemon());
			break;
		case Moves::Psycho_Shift:
			if (is_clear(get_status(target.pokemon()))) {
				shift_status(user, target.pokemon(), weather);
			}
			break;
		case Moves::Rage:		// Fix
			break;
		case Moves::Rain_Dance:
			weather.activate_rain(extends_rain(get_item(user)));
			break;
		case Moves::Rapid_Spin:
			clear_field(user_team, move, target.pokemon());
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
			phaze(user_team, target, weather, variable);
			break;
		case Moves::Role_Play:		// Fix
			break;
		case Moves::Roost:
			user.roost();
			heal(user, rational(1_bi, 2_bi));
			break;
		case Moves::Safeguard:
			user_team.screens.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.activate_sand(extends_sand(get_item(user)));
			break;
		case Moves::Screech:
			boost(stage(target.pokemon()), StatNames::DEF, -2_bi);
			break;
		case Moves::Seed_Flare:
			if (effect_activates(variable)) {
				boost(stage(target.pokemon()), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Shadow_Force:
			user.shadow_force();
			break;
		case Moves::Sharpen:
			boost(stage(user), StatNames::ATK, 1_bi);
			break;
		case Moves::Sketch:		// Fix
			break;
		case Moves::Skill_Swap:		// Fix
			break;
		case Moves::Skull_Bash: // Fix
			break;
		case Moves::Sky_Attack:
			if (effect_activates(variable)) {
				target.pokemon().flinch();
			}
			break;
		case Moves::SmellingSalt:
			if (boosts_smellingsalt(get_status(target.pokemon()))) {
				get_status(target.pokemon()) = Status{};
			}
			break;
		case Moves::Snatch:	// Fix
			break;
		case Moves::SolarBeam:		// Fix
			break;
		case Moves::Spikes:
			target.entry_hazards.add_spikes();
			break;
		case Moves::Spit_Up:
			user.release_stockpile();
			break;
		case Moves::Spite:		// Fix
			break;
		case Moves::Stealth_Rock:
			target.entry_hazards.add_stealth_rock();
			break;
		case Moves::Steel_Wing:
			if (effect_activates(variable)) {
				boost(stage(user), StatNames::DEF, 1_bi);
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
			weather.activate_sun(extends_sun(get_item(user)));
			break;
		case Moves::Superpower:
			boost_physical(stage(user), -1_bi);
			break;
		case Moves::Swagger:
			confusing_stat_boost(target.pokemon(), StatNames::ATK, 2_bi);
			break;
		case Moves::Swallow:
			use_swallow(user);
			break;
		case Moves::Sweet_Scent:
			boost(stage(target.pokemon()), StatNames::EVA, -1_bi);
			break;
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			switch_pokemon(user_team, target, weather, to_replacement(move));
			break;
		case Moves::Switcheroo:
		case Moves::Trick:
			swap_items(user, target.pokemon());
			break;
		case Moves::Swords_Dance:
			boost(stage(user), StatNames::ATK, 2_bi);
			break;
		case Moves::Tailwind:
			user_team.screens.activate_tailwind();
			break;
		case Moves::Taunt:
			target.pokemon().taunt();
			break;
		case Moves::Thunder_Fang:
			fang_side_effects(user, target.pokemon(), weather, variable, Statuses::paralysis);
			break;
		case Moves::Tickle:
			boost_physical(stage(target.pokemon()), -1_bi);
			break;
		case Moves::Torment:
			target.pokemon().torment();
			break;
		case Moves::Toxic:
			apply(Statuses::poison_toxic, user, target.pokemon(), weather);
			break;
		case Moves::Toxic_Spikes:
			target.entry_hazards.add_toxic_spikes();
			break;
		case Moves::Transform:		// Fix
			break;
		case Moves::Tri_Attack:
			apply(tri_attack_status(variable), user, target.pokemon(), weather);
			break;
		case Moves::Trick_Room:
			weather.activate_trick_room();
			break;
		case Moves::U_turn:
			user.u_turn();
			break;
		case Moves::Uproar:
			// TODO: make this make sense
			weather.activate_uproar(variable.value);
			user.use_uproar();
			break;
		case Moves::Volt_Tackle:
			recoil_status(user, target.pokemon(), weather, damage, variable, Statuses::paralysis);
			break;
		case Moves::Wake_Up_Slap:
			if (is_sleeping(get_status(target.pokemon()))) {
				get_status(target.pokemon()) = Status{};
			}
			break;
		case Moves::Water_Sport:
			user.activate_water_sport();
			break;
		case Moves::Will_O_Wisp:
			apply(Statuses::burn, user, target.pokemon(), weather);
			break;
		case Moves::Wish:
			user_team.wish.activate();
			break;
		case Moves::Worry_Seed:		// Fix
			break;
		case Moves::Yawn:
			target.pokemon().hit_with_yawn();
			break;
		default:
			break;
	}
}


auto lower_pp(MutableActivePokemon user, Move const move, Ability const target) {
	if (is_regular(move) and !is_locked_in_to_bide(user)) {
		auto const regular_moves = all_moves(user).regular();
		auto const it = containers::find(begin(regular_moves), end(regular_moves), move);
		assert(it != regular_moves.end());
		it->decrement_pp(target);
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

auto call_other_move(MutableActivePokemon) {
	// TODO: implement
}


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


constexpr auto breaks_screens(Moves const move) {
	return move == Moves::Brick_Break;
}

auto do_effects_before_moving(Moves const move, Status & user_status, Team & target) {
	if (breaks_screens(move)) {
		target.screens.shatter();
	} else if (is_usable_while_frozen(move)) {
		if (is_frozen(user_status)) {
			user_status = Status{};
		}
	}
}


auto do_damage(MutableActivePokemon user, MutableActivePokemon target, damage_type const damage) {
	target.direct_damage(damage);
	if (causes_recoil(get_item(user))) {
		heal(user, rational(-1_bi, 10_bi));
	}
}


auto use_move(Team & user, Move const move, bool const user_damaged, Team & target, bounded::optional<UsedMove> const target_move, bool const target_damaged, Weather & weather, Variable const & variable, bool const critical_hit, bounded::optional<damage_type> const known_damage) -> void {
	// TODO: Add targeting information and only block the move if the target is
	// immune.
	if (blocks_sound_moves(get_ability(target.pokemon())) and is_sound_based(move) and !(move == Moves::Heal_Bell or move == Moves::Perish_Song)) {
		return;
	}

	do_effects_before_moving(move, get_status(user.pokemon()), target);

	auto const damage = known_damage ?
		*known_damage :
		is_damaging(move) ? damage_calculator(user, move, user_damaged, target, target_damaged, weather, variable, critical_hit) : 0_bi;
	if (damage != 0_bi) {
		do_damage(user.pokemon(), target.pokemon(), damage);
	}
	user.pokemon().increment_move_use_counter(move);
	do_side_effects(user, move, target, target_move, weather, variable, damage);
}


}	// namespace

auto call_move(Team & user, Move const move, bool const user_damaged, Team & target, bounded::optional<UsedMove> const target_move, bool const target_damaged, Weather & weather, Variable const & variable, bool const missed, bool const awakens, bool const critical_hit, bounded::optional<damage_type> const known_damage) -> void {
	assert(!moved(user.pokemon()));
	user.move();
	auto user_pokemon = user.pokemon();
	auto target_pokemon = target.pokemon();
	user_pokemon.update_before_move();
	if (!can_execute_move(user_pokemon, move, target_pokemon, weather, awakens)) {
		return;
	}

	lower_pp(user_pokemon, move, get_ability(target_pokemon));

	if (calls_other_move(move)) {
		call_other_move(user_pokemon);
	}
	if (!missed) {
		use_move(user, move, user_damaged, target, target_move, target_damaged, weather, variable, critical_hit, known_damage);
	}
}

}	// namespace technicalmachine
