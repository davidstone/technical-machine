// Use moves
// Copyright (C) 2012 David Stone
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
#include <functional>

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

#include "../stat/stat.hpp"

#include "../type/effectiveness.hpp"
#include "../type/type.hpp"

namespace technicalmachine {
namespace {

unsigned use_move(Team & user, Team & target, Weather & weather, Variable const & variable, bool damage_is_known);
unsigned calculate_real_damage(ActivePokemon & user, Team & target, Weather const & weather, Variable const & variable, bool const damage_is_known);
void call_other_move (ActivePokemon & user);
void cure_all_status(Team & user, std::function<bool(Pokemon const &)> const & predicate);
void do_effects_before_moving (Pokemon & user, Team & target);
void do_damage (ActivePokemon & user, ActivePokemon & target, unsigned damage);
void do_side_effects (Team & user, Team & target, Weather & weather, Variable const & variable, unsigned damage);
void absorb_hp(Pokemon & user, Pokemon const & target, unsigned damage);
void belly_drum(ActivePokemon & user);
void clear_field(Team & user, Pokemon const & target);
void confusing_stat_boost(ActivePokemon & target, Stat::Stats stat, int stages);
void curse(ActivePokemon & user, ActivePokemon & target);
void equalize(Stat & hp1, Stat & hp2);
void phaze(Team & user, Team & target, Weather & weather, Variable const & variable);
void rest(Pokemon & user);
void struggle(Pokemon & user);
void swap_items(Pokemon & user, Pokemon & target);
void tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather, Variable const & variable);
void use_swallow(ActivePokemon & user);

template<Status::Statuses status>
void fang_side_effects(Pokemon & user, ActivePokemon & target, Weather const & weather, Variable const & variable) {
	switch (variable.value()) {
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
void recoil_status(Pokemon & user, Pokemon & target, Weather const & weather, unsigned const damage, Variable const & variable) {
	recoil(user, damage, 3);
	if (variable.effect_activates())
		Status::apply<status>(user, target, weather);
}

}	// unnamed namespace

unsigned call_move (Team & user_team, Team & target_team, Weather & weather, Variable const & variable, bool const damage_is_known) {
	auto & user = user_team.pokemon();
	auto const & target = target_team.pokemon();
	user.update_before_move();
	if (can_execute_move (user, target, weather)) {
		user.lower_pp(target.ability());
		if (user.move().calls_other_move())
			call_other_move (user);
		if (!user.missed())
			return use_move(user_team, target_team, weather, variable, damage_is_known);
	}
	return 0;
}

namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, Variable const & variable, bool const damage_is_known) {
	auto const & move = user.pokemon().move();
	// TODO: Add targeting information and only block the move if the target is
	// immune.
	if (target.pokemon().ability().blocks_sound_moves() and move.is_sound_based() and
			!(move.name == Moves::Heal_Bell or move.name == Moves::Perish_Song))
		return 0;
	calculate_speed (user, weather);
	calculate_speed (target, weather);

	do_effects_before_moving (user.pokemon(), target);

	unsigned const damage = calculate_real_damage(user.pokemon(), target, weather, variable, damage_is_known);
	do_damage (user.pokemon(), target.pokemon(), damage);
	user.pokemon().increment_move_use_counter();

	do_side_effects(user, target, weather, variable, damage);

	return damage;
}

void do_effects_before_moving (Pokemon & user, Team & target) {
	if (user.move().breaks_screens()) {
		target.screens.shatter();
	}
	else if (user.move().is_usable_while_frozen()) {
		if (user.status().is_frozen())
			user.status().clear();
	}
}

unsigned calculate_real_damage(ActivePokemon & user, Team & target, Weather const & weather, Variable const & variable, bool const damage_is_known) {
	if (!user.move().is_damaging())
		return 0;
	if (damage_is_known)
		return target.pokemon().damaged();

	calculate_defending_stat (user, target.pokemon(), weather);
	calculate_attacking_stat (user, weather);
	return damage_calculator(user, target, weather, variable);
}

void do_damage(ActivePokemon & user, ActivePokemon & target, unsigned const damage) {
	if (damage == 0)
		return;
	target.direct_damage(damage);
	if (user.item().causes_recoil())
		drain(user, Rational(1, 10));
}

void do_side_effects(Team & user_team, Team & target_team, Weather & weather, Variable const & variable, unsigned const damage) {
	auto & target = target_team.pokemon();
	auto & user = user_team.pokemon();
	auto const & move = user.move();
	switch (move.name) {
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
			if (variable.effect_activates())
				target.stat_boost(Stat::SPD, -1);
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			user.stat_boost(Stat::DEF, 2);
			break;
		case Moves::Acupressure:
			user.stat_boost(static_cast<Stat::Stats>(variable.value()), 2);
			break;
		case Moves::Agility:
		case Moves::Rock_Polish:
			user.stat_boost(Stat::SPE, 2);
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
			if (variable.effect_activates())
				target.flinch();
			break;
		case Moves::Amnesia:
			user.stat_boost(Stat::SPD, 2);
			break;
		case Moves::AncientPower:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			if (variable.effect_activates())
				user.stat_boost_regular(1);
			break;
		case Moves::Aqua_Ring:
			user.activate_aqua_ring();
			break;
		case Moves::Aurora_Beam:
			if (variable.effect_activates())
				target.stat_boost(Stat::ATK, -1);
			break;
		case Moves::Aromatherapy:
			cure_all_status(user_team, [](Pokemon const & pokemon) { return true; });
			break;
		case Moves::Attract:
			if (user.gender().multiplier(target.gender()) == -1)
				target.attract();
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
			target.partially_trap(variable.value());
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
			if (variable.effect_activates())
				Status::apply<Status::BURN>(user, target, weather);
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (variable.effect_activates())
				Status::apply<Status::FREEZE>(user, target, weather);
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
			if (variable.effect_activates())
				Status::apply<Status::PARALYSIS>(user, target, weather);
			break;
		case Moves::Bounce: {
			bool const vanished = user.bounce();
			if (vanished and variable.effect_activates()) {
				Status::apply<Status::PARALYSIS>(user, target, weather);
			}
			break;
		}
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			recoil (user, damage, 3);
			break;
		case Moves::Bubble:
		case Moves::BubbleBeam:
		case Moves::Constrict:
			if (variable.effect_activates())
				target.stat_boost(Stat::SPE, -1);
			break;
		case Moves::Bug_Bite:			// Fix
		case Moves::Pluck:
			break;
		case Moves::Bulk_Up:
			user.stat_boost_physical(1);
			break;
		case Moves::Calm_Mind:
			user.stat_boost_special(1);
			break;
		case Moves::Camouflage:
			break;
		case Moves::Captivate:
			if (user.gender().multiplier(target.gender()) == -1)
				target.stat_boost(Stat::SPD, -2);
			break;
		case Moves::Charge:
			user.charge();
			user.stat_boost(Stat::SPD, 1);
			break;
		case Moves::Charge_Beam:
			if (variable.effect_activates())
				user.stat_boost(Stat::SPA, 1);
			break;
		case Moves::Charm:
		case Moves::FeatherDance:
			target.stat_boost(Stat::ATK, -2);
			break;
		case Moves::Chatter:
			if (user.can_confuse_with_chatter() and variable.effect_activates())
				target.confuse();
			break;
		case Moves::Close_Combat:
			user.stat_boost_physical(-1);
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
			if (variable.effect_activates())
				target.confuse();
			break;
		case Moves::Conversion:		// Fix
			break;
		case Moves::Conversion_2:	// Fix
			break;
		case Moves::Cosmic_Power:
		case Moves::Defend_Order:
			user.stat_boost_defensive(1);
			break;
		case Moves::Crunch:
		case Moves::Crush_Claw:
		case Moves::Iron_Tail:
		case Moves::Rock_Smash:
			if (variable.effect_activates())
				target.stat_boost(Stat::DEF, -1);
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			target.stat_boost(Stat::SPE, -2);
			break;
		case Moves::Counter:
			if (target.move().is_physical())
				target.indirect_damage(user.damaged() * 2u);
			break;
		case Moves::Covet:
		case Moves::Thief:
			user.item().steal(target.item());
			break;
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smog:
			if (variable.effect_activates())
				Status::apply<Status::POISON>(user, target, weather);
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
			Status::apply<Status::SLEEP>(user, target, weather);
			break;
		case Moves::Defense_Curl:
			user.stat_boost(Stat::DEF, 1);
			user.defense_curl();
			break;
		case Moves::Defog:
			weather.fog = false;
			target.stat_boost(Stat::EVA, -1);
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
		case Moves::Disable:		// Fix
			break;
		case Moves::Dive:
			user.dive();
			break;
		case Moves::Doom_Desire:	// Fix
		case Moves::Future_Sight:
			break;
		case Moves::Double_Team:
		case Moves::Minimize:
			user.stat_boost(Stat::EVA, 1);
			break;
		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			user.stat_boost(Stat::SPA, -2);
			break;
		case Moves::Dragon_Dance:
			user.stat_boost(Stat::ATK, 1);
			user.stat_boost(Stat::SPE, 1);
			break;
		case Moves::Dream_Eater:
			if (target.status().is_sleeping ())
				absorb_hp(user, target, damage);
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
			target.stat_boost(Stat::SPD, -2);
			break;
		case Moves::Feint:
			target.break_protect();
			break;
		case Moves::Fire_Fang:
			fang_side_effects<Status::BURN>(user, target, weather, variable);
			break;
		case Moves::Flare_Blitz:
			recoil_status<Status::BURN>(user, target, weather, damage, variable);
			break;
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::SmokeScreen:
			target.stat_boost(Stat::ACC, -1);
			break;
		case Moves::Flatter:
			confusing_stat_boost(target, Stat::SPA, 1);
			break;
		case Moves::Fling:
			user.item().remove();
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
			Status::apply<Status::PARALYSIS>(user, target, weather);
			break;
		case Moves::Gravity:
			weather.set_gravity();
			break;
		case Moves::Growl:
			target.stat_boost(Stat::ATK, -1);
			break;
		case Moves::Growth:
			user.stat_boost(Stat::SPA, 1);
			break;
		case Moves::Grudge:		// Fix
			break;
		case Moves::Guard_Swap:
			ActivePokemon::swap_defensive_stages(user, target);
			break;
		case Moves::Hail:
			weather.set_hail (user.item().extends_hail());
			break;
		case Moves::Hammer_Arm:
			user.stat_boost(Stat::SPE, -1);
			break;
		case Moves::Harden:
		case Moves::Withdraw:
			user.stat_boost(Stat::DEF, 1);
			break;
		case Moves::Haze:
			user.reset_stats();
			target.reset_stats();
			break;
		case Moves::Head_Smash:
			recoil (user, damage, 2);
			break;
		case Moves::Heal_Bell:
			cure_all_status(user_team, [](Pokemon const & pokemon) {
				return !pokemon.ability().blocks_sound_moves();
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
			heal(user, Rational(1, 2));
			break;
		case Moves::Healing_Wish:		// Fix
			break;
		case Moves::Heart_Swap:
			ActivePokemon::swap_stat_boosts(user, target);
			break;
		case Moves::Hi_Jump_Kick:		// Fix
		case Moves::Jump_Kick:
			break;
		case Moves::Howl:
		case Moves::Metal_Claw:
		case Moves::Meditate:
		case Moves::Meteor_Mash:
			if (variable.effect_activates())
				user.stat_boost(Stat::ATK, 1);
			break;
		case Moves::Ice_Ball:		// Fix
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			fang_side_effects<Status::FREEZE>(user, target, weather, variable);
			break;
		case Moves::Icy_Wind:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
		case Moves::String_Shot:
			target.stat_boost(Stat::SPE, -1);
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
			target.stat_boost(Stat::DEF, -1);
			break;
		case Moves::Light_Screen:
			user_team.screens.activate_light_screen(user.item().extends_light_screen());
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
			target.stat_boost_offensive(-2);
			user.faint();
			break;
		case Moves::Metal_Burst:
			target.indirect_damage(user.damaged() * 3u / 2);
			break;
		case Moves::Mimic:		// Fix
			break;
		case Moves::Miracle_Eye:		// Fix
			break;
		case Moves::Mirror_Coat:
			if (target.move().is_special())
				target.indirect_damage(user.damaged() * 2u);
			break;
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Octazooka:
			if (variable.effect_activates())
				target.stat_boost(Stat::ACC, -1);
			break;
		case Moves::Mist:
			user_team.screens.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (variable.effect_activates())
				target.stat_boost(Stat::SPA, -1);
			break;
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis: {
			auto const amount = [& weather]() {
				if (weather.sun())
					return Rational(2, 3);
				else if (weather.hail() or weather.rain() or weather.sand())
					return Rational(1, 4);
				else
					return Rational(1, 2);
			};
			heal(user, amount());
			break;
		}
		case Moves::Mud_Sport:
			user.activate_mud_sport();
			break;
		case Moves::Nasty_Plot:
		case Moves::Tail_Glow:
			user.stat_boost(Stat::SPA, 2);
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
			equalize(user.hp(), target.hp());
			break;
		case Moves::Perish_Song:
			user.activate_perish_song();
			target.activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (variable.effect_activates())
				Status::apply<Status::POISON_TOXIC>(user, target, weather);
			break;
		case Moves::Poison_Gas:
		case Moves::PoisonPowder:
			Status::apply<Status::POISON>(user, target, weather);
			break;
		case Moves::Power_Swap:
			user.swap_offensive_stages(user, target);
			break;
		case Moves::Power_Trick:
			user.activate_power_trick();
			break;
		case Moves::Present:
			if (variable.present_heals()) {
				Stat & hp = target.hp();
				hp.stat += 80;
				hp.stat = std::min(hp.stat, hp.max);
			}
			break;
		case Moves::Psych_Up:
			user.copy_stat_boosts(target);
			break;
		case Moves::Psycho_Shift:
			if (target.status().is_clear())
				Status::shift(user, target, weather);
			break;
		case Moves::Rage:		// Fix
			break;
		case Moves::Rain_Dance:
			weather.set_rain (user.item().extends_rain());
			break;
		case Moves::Rapid_Spin:
			clear_field(user_team, target);
			break;
		case Moves::Razor_Wind:	// Fix
			break;
		case Moves::Recycle:		// Fix
			break;
		case Moves::Reflect:
			user_team.screens.activate_reflect(user.item().extends_reflect());
			break;
		case Moves::Refresh:
			user.status().clear();
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
			heal(user, Rational(1, 2));
			break;
		case Moves::Safeguard:
			user_team.screens.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.set_sand (user.item().extends_sand());
			break;
		case Moves::Screech:
			target.stat_boost(Stat::DEF, -2);
			break;
		case Moves::Seed_Flare:
			if (variable.effect_activates())
				target.stat_boost(Stat::SPD, -2);
			break;
		case Moves::Shadow_Force:
			user.shadow_force();
			break;
		case Moves::Sharpen:
			user.stat_boost(Stat::ATK, 1);
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
			if (target.status().boosts_smellingsalt())
				target.status().clear();
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
			if (variable.effect_activates())
				user.stat_boost(Stat::DEF, 1);
			break;
		case Moves::Stockpile:
			user.increment_stockpile();
			break;
		case Moves::Struggle:
			struggle(user);
			break;
		case Moves::Submission:
		case Moves::Take_Down:
			recoil (user, damage, 4);
			break;
		case Moves::Substitute:
			user.use_substitute();		
			break;
		case Moves::Sunny_Day:
			weather.set_sun (user.item().extends_sun());
			break;
		case Moves::Superpower:
			user.stat_boost_physical(-1);
			break;
		case Moves::Swagger:
			confusing_stat_boost(target, Stat::ATK, 2);
			break;
		case Moves::Swallow:
			use_swallow(user);
			break;
		case Moves::Sweet_Scent:
			target.stat_boost(Stat::EVA, -1);
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
			user.stat_boost(Stat::ATK, 2);
			break;
		case Moves::Tailwind:
			user_team.screens.activate_tailwind();
			break;
		case Moves::Taunt:
			target.taunt();
			break;
		case Moves::Thunder_Fang:
			fang_side_effects<Status::PARALYSIS>(user, target, weather, variable);
			break;
		case Moves::Tickle:
			target.stat_boost_physical(-1);
			break;
		case Moves::Torment:
			target.torment();
			break;
		case Moves::Toxic:
			Status::apply<Status::POISON_TOXIC>(user, target, weather);
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
			recoil_status<Status::PARALYSIS>(user, target, weather, damage, variable);
			break;
		case Moves::Wake_Up_Slap:
			if (target.status().is_sleeping ())
				target.status().clear ();
			break;
		case Moves::Water_Sport:
			user.activate_water_sport();
			break;
		case Moves::Will_O_Wisp:
			Status::apply<Status::BURN>(user, target, weather);
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
void absorb_hp(Pokemon & user, Pokemon const & target, unsigned const damage) {
	if (target.ability().damages_leechers()) {
		user.apply_damage(damage / 2);
	}
	else {
		user.apply_healing(damage / 2);
	}
}

void belly_drum(ActivePokemon & user) {
	Stat & hp = user.hp();
	if (hp.stat > hp.max / 2 and hp.stat > 1) {
		hp.stat -= hp.max / 2;
		user.stat_boost(Stat::ATK, 12);
	}
}

void clear_field(Team & user, Pokemon const & target) {
	if (!user.pokemon().move().type().get_effectiveness(target).has_no_effect())
		user.clear_field();
}

void confusing_stat_boost(ActivePokemon & target, Stat::Stats const stat, int const stages) {
	target.stat_boost(stat, stages);
	target.confuse();
}

void cure_all_status(Team & user, std::function<bool(Pokemon const &)> const & predicate) {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon))
			pokemon.status().clear();
	}
}

void curse(ActivePokemon & user, ActivePokemon & target) {
	if (is_type(user, Type::Ghost) and !user.ability().blocks_secondary_damage()) {
		if (!target.is_cursed()) {
			user.indirect_damage(user.hp().max / 2);
			target.curse();
		}
	}
	else {
		user.stat_boost_physical(1);
		user.stat_boost(Stat::SPE, -1);
	}
}

void equalize(Stat & hp1, Stat & hp2) {
	Stat::stat_type const temp = (hp1.stat + hp2.stat) / 2;
	hp1.stat = std::min(temp, hp1.max);
	hp2.stat = std::min(temp, hp2.max);
}

void phaze(Team & user, Team & target, Weather & weather, Variable const & variable) {
	if (target.pokemon().can_be_phazed()) {
		uint8_t const index = variable.phaze_index(target.pokemon().index());
		target.all_pokemon().set_replacement(index);
		switchpokemon(target, user, weather);
		target.move();
	}
}

void rest(Pokemon & user) {
	Stat & hp = user.hp();
	if (hp.stat != hp.max) {
		hp.stat = hp.max;
		user.status().rest();
	}
}

void struggle(Pokemon & user) {
	user.apply_damage(user.hp().max / 4);
}

void swap_items(Pokemon & user, Pokemon & target) {
	// Add support for abilities that block Trick / Switcheroo
	if (!user.item().blocks_trick () and !target.item().blocks_trick()) {
		using std::swap;
		swap(user.item(), target.item());
	}
}

void tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather, Variable const & variable) {
	switch (variable.value()) {
		case 1:
			Status::apply<Status::BURN>(user, target, weather);
			break;
		case 2:
			Status::apply<Status::FREEZE>(user, target, weather);
			break;
		case 3:
			Status::apply<Status::PARALYSIS>(user, target, weather);
			break;
		default:
			assert(false);
			break;
	}
}

void use_swallow(ActivePokemon & user) {
	auto const stockpiles = user.release_stockpile();
	if (stockpiles == 0)
		return;
	heal(user, Stockpile::swallow_healing(stockpiles));
}

void call_other_move (ActivePokemon & user) {
	// TODO: implement
}

}	// unnamed namespace
}	// namespace technicalmachine
