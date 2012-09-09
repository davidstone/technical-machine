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
#include "../status.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../pokemon/pokemon.hpp"

#include "../stat/stat.hpp"

#include "../type/type.hpp"

#undef SING

namespace technicalmachine {
namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, bool damage_is_known);
unsigned calculate_real_damage(Team & user, Team & target, Weather const & weather, bool const damage_is_known);
void call_other_move (Team & user);
void cure_all_status(Team & user, std::function<bool(Pokemon const &)> const & predicate);
void do_effects_before_moving (Pokemon & user, Team & target);
void do_damage (Team & user, Team & target, unsigned damage);
void do_side_effects (Team & user, Team & target, Weather & weather, unsigned damage);
void absorb_hp(Pokemon & user, Pokemon const & target, unsigned damage);
void belly_drum(Team & user);
void clear_field(Team & user, Team const & target);
void confusing_stat_boost(Move const & move, Team & target, Stat::Stats stat, int stages);
void curse(Team & user, Team & target);
void equalize(Stat & hp1, Stat & hp2);
void phaze(Team & user, Team & target, Weather & weather);
void rest(Pokemon & user);
void struggle(Pokemon & user);
void swap_items(Pokemon & user, Pokemon & target);
void tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather);

template<Status::Statuses status>
void fang_side_effects(Pokemon & user, Team & target, Weather const & weather) {
	switch (user.move().variable().value()) {
		case 0:
			break;
		case 1:
			Status::apply<status>(user, target.pokemon(), weather);
			break;
		case 2:
			target.flinch();
			break;
		case 3:	
			Status::apply<status>(user, target.pokemon(), weather);
			target.flinch();
			break;
		default:
			assert(false);
			break;
	}
}

template<Status::Statuses status>
void recoil_status(Pokemon & user, Pokemon & target, Weather const & weather, unsigned const damage) {
	recoil(user, damage, 3);
	if (user.move().variable().effect_activates())
		Status::apply<status>(user, target, weather);
}

}	// unnamed namespace

unsigned call_move (Team & user, Team & target, Weather & weather, bool const damage_is_known) {
	user.update_before_move();
	if (can_execute_move (user, target, weather)) {
		user.lower_pp(target.pokemon().ability);
		if (user.pokemon().move().calls_other_move())
			call_other_move (user);
		if (!user.missed())
			return use_move (user, target, weather, damage_is_known);
	}
	return 0;
}

namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, bool const damage_is_known) {
	Move & move = user.pokemon().move();
	// TODO: Add targeting information and only block the move if the target is
	// immune.
	if (target.pokemon().ability.blocks_sound_moves() and move.is_sound_based() and
			!(move.name == Moves::HEAL_BELL or move.name == Moves::PERISH_SONG))
		return 0;
	Stat::calculate_speed (user, weather);
	Stat::calculate_speed (target, weather);

	do_effects_before_moving (user.pokemon(), target);

	unsigned const damage = calculate_real_damage(user, target, weather, damage_is_known);
	do_damage (user, target, damage);
	move.increment_use_counter();

	do_side_effects (user, target, weather, damage);

	return damage;
}

void do_effects_before_moving (Pokemon & user, Team & target) {
	if (user.move().breaks_screens()) {
		target.screens.shatter();
	}
	else if (user.move().is_usable_while_frozen()) {
		if (user.status.is_frozen())
			user.status.clear();
	}
}

unsigned calculate_real_damage(Team & user, Team & target, Weather const & weather, bool const damage_is_known) {
	if (!user.pokemon().move().is_damaging())
		return 0;
	if (damage_is_known)
		return target.damaged();

	calculate_defending_stat (user, target, weather);
	calculate_attacking_stat (user, weather);
	return damage_calculator(user, target, weather);
}

void do_damage(Team & user, Team & target, unsigned const damage) {
	if (damage == 0)
		return;
	damage_side_effect (target.pokemon(), damage);
	if (user.pokemon().item.causes_recoil())
		heal (user.pokemon(), -10);
	target.do_damage(damage);
}

void do_side_effects (Team & user, Team & target, Weather & weather, unsigned const damage) {
	Move & move = user.pokemon().move();
	switch (move.name) {
		case Moves::ABSORB:
		case Moves::DRAIN_PUNCH:
		case Moves::GIGA_DRAIN:
		case Moves::LEECH_LIFE:
		case Moves::MEGA_DRAIN:
			absorb_hp(user.pokemon(), target.pokemon(), damage);
			break;
		case Moves::ACID:
		case Moves::BUG_BUZZ:
		case Moves::EARTH_POWER:
		case Moves::ENERGY_BALL:
		case Moves::FLASH_CANNON:
		case Moves::FOCUS_BLAST:
		case Moves::LUSTER_PURGE:
		case Moves::PSYCHIC:
		case Moves::SHADOW_BALL:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPD, -1);
			break;
		case Moves::ACID_ARMOR:
		case Moves::BARRIER:
		case Moves::IRON_DEFENSE:
			user.stage.boost(Stat::DEF, 2);
			break;
		case Moves::ACUPRESSURE:
			// TODO: this doesn't properly account for stats maxing out
			user.stage.boost(static_cast<Stat::Stats>(move.variable().value()), 2);
			break;
		case Moves::AGILITY:
		case Moves::ROCK_POLISH:
			user.stage.boost(Stat::SPE, 2);
			break;
		case Moves::AIR_SLASH:
		case Moves::ASTONISH:
		case Moves::BITE:
		case Moves::BONE_CLUB:
		case Moves::DARK_PULSE:
		case Moves::DRAGON_RUSH:
		case Moves::EXTRASENSORY:
		case Moves::FAKE_OUT:
		case Moves::HEADBUTT:
		case Moves::HYPER_FANG:
		case Moves::IRON_HEAD:
		case Moves::NEEDLE_ARM:
		case Moves::ROCK_SLIDE:
		case Moves::ROLLING_KICK:
		case Moves::SNORE:
		case Moves::STOMP:
		case Moves::TWISTER:
		case Moves::WATERFALL:
		case Moves::ZEN_HEADBUTT:
			if (move.variable().effect_activates())
				target.flinch();
			break;
		case Moves::AMNESIA:
			user.stage.boost(Stat::SPD, 2);
			break;
		case Moves::ANCIENTPOWER:
		case Moves::OMINOUS_WIND:
		case Moves::SILVER_WIND:
			if (move.variable().effect_activates())
				user.stage.boost_regular(1);
			break;
		case Moves::AQUA_RING:
			user.activate_aqua_ring();
			break;
		case Moves::AURORA_BEAM:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::ATK, -1);
			break;
		case Moves::AROMATHERAPY:
			cure_all_status(user, [](Pokemon const & pokemon) { return true; });
			break;
		case Moves::ATTRACT:
			if (user.pokemon().gender.multiplier (target.pokemon().gender) == -1)
				target.attract();
			break;
		case Moves::BATON_PASS:
			user.baton_pass();
			break;
		case Moves::BELLY_DRUM:
			belly_drum(user);
			break;
		case Moves::BIDE:
			user.use_bide(target.pokemon());
			break;
		case Moves::BIND:
		case Moves::CLAMP:
		case Moves::FIRE_SPIN:
		case Moves::MAGMA_STORM:
		case Moves::SAND_TOMB:
		case Moves::WHIRLPOOL:
		case Moves::WRAP:
			target.partially_trap(move.variable().value());
			break;
		case Moves::BLAST_BURN:
		case Moves::FRENZY_PLANT:
		case Moves::GIGA_IMPACT:
		case Moves::HYDRO_CANNON:
		case Moves::HYPER_BEAM:
		case Moves::ROAR_OF_TIME:
		case Moves::ROCK_WRECKER:
			user.use_recharge_move();
			break;
		case Moves::BLAZE_KICK:
		case Moves::EMBER:
		case Moves::FIRE_BLAST:
		case Moves::FIRE_PUNCH:
		case Moves::FLAME_WHEEL:
		case Moves::FLAMETHROWER:
		case Moves::HEAT_WAVE:
		case Moves::LAVA_PLUME:
		case Moves::SACRED_FIRE:
			if (move.variable().effect_activates())
				Status::apply<Status::BURN>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::BLIZZARD:
		case Moves::ICE_BEAM:
		case Moves::ICE_PUNCH:
		case Moves::POWDER_SNOW:
			if (move.variable().effect_activates())
				Status::apply<Status::FREEZE>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::BLOCK:
		case Moves::MEAN_LOOK:
		case Moves::SPIDER_WEB:
			target.fully_trap();
			break;
		case Moves::BODY_SLAM:
		case Moves::DISCHARGE:
		case Moves::DRAGONBREATH:
		case Moves::FORCE_PALM:
		case Moves::LICK:
		case Moves::SECRET_POWER:
		case Moves::SPARK:
		case Moves::THUNDER:
		case Moves::THUNDERBOLT:
		case Moves::THUNDERPUNCH:
		case Moves::THUNDERSHOCK:
			if (move.variable().effect_activates())
				Status::apply<Status::PARALYSIS>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::BOUNCE:		// TODO: add paralysis
			user.bounce();
			break;
		case Moves::BRAVE_BIRD:
		case Moves::DOUBLE_EDGE:
		case Moves::WOOD_HAMMER:
			recoil (user.pokemon(), damage, 3);
			break;
		case Moves::BUBBLE:
		case Moves::BUBBLEBEAM:
		case Moves::CONSTRICT:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPE, -1);
			break;
		case Moves::BUG_BITE:			// Fix
		case Moves::PLUCK:
			break;
		case Moves::BULK_UP:
			user.stage.boost_physical(1);
			break;
		case Moves::CALM_MIND:
			user.stage.boost_special(1);
			break;
		case Moves::CAMOUFLAGE:
			break;
		case Moves::CAPTIVATE:
			if (user.pokemon().gender.multiplier (target.pokemon().gender) == -1)
				target.stage.boost(Stat::SPD, -2);
			break;
		case Moves::CHARGE:
			user.charged();
			user.stage.boost(Stat::SPD, 1);
			break;
		case Moves::CHARGE_BEAM:
			if (move.variable().effect_activates())
				user.stage.boost(Stat::SPA, 1);
			break;
		case Moves::CHARM:
		case Moves::FEATHERDANCE:
			target.stage.boost(Stat::ATK, -2);
			break;
		case Moves::CHATTER:
			if (user.pokemon().can_use_chatter() and move.variable().effect_activates())
				target.confuse();
			break;
		case Moves::CLOSE_COMBAT:
			user.stage.boost_physical(-1);
			break;
		case Moves::CONFUSE_RAY:
		case Moves::SUPERSONIC:
		case Moves::SWEET_KISS:
		case Moves::TEETER_DANCE:
			target.confuse();
			break;
		case Moves::CONFUSION:
		case Moves::DIZZY_PUNCH:
		case Moves::DYNAMICPUNCH:
		case Moves::PSYBEAM:
		case Moves::ROCK_CLIMB:
		case Moves::SIGNAL_BEAM:
		case Moves::WATER_PULSE:
			if (move.variable().effect_activates())
				target.confuse();
			break;
		case Moves::CONVERSION:		// Fix
			break;
		case Moves::CONVERSION2:	// Fix
			break;
		case Moves::COSMIC_POWER:
		case Moves::DEFEND_ORDER:
			user.stage.boost_defensive(1);
			break;
		case Moves::CRUNCH:
		case Moves::CRUSH_CLAW:
		case Moves::IRON_TAIL:
		case Moves::ROCK_SMASH:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::DEF, -1);
			break;
		case Moves::COTTON_SPORE:
		case Moves::SCARY_FACE:
			target.stage.boost(Stat::SPE, -2);
			break;
		case Moves::COUNTER:
			if (target.pokemon().move().is_physical())
				damage_side_effect (target.pokemon(), user.damaged() * 2u);
			break;
		case Moves::COVET:
		case Moves::THIEF:
			user.pokemon().item.steal (target.pokemon().item);
			break;
		case Moves::CROSS_POISON:
		case Moves::GUNK_SHOT:
		case Moves::POISON_JAB:
		case Moves::POISON_STING:
		case Moves::POISON_TAIL:
		case Moves::SLUDGE:
		case Moves::SLUDGE_BOMB:
		case Moves::SMOG:
			if (move.variable().effect_activates())
				Status::apply<Status::POISON>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::CURSE:
			curse(user, target);
			break;
		case Moves::DARK_VOID:
		case Moves::GRASSWHISTLE:
		case Moves::HYPNOSIS:
		case Moves::LOVELY_KISS:
		case Moves::SING:
		case Moves::SLEEP_POWDER:
		case Moves::SPORE:
			Status::apply<Status::SLEEP>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::DEFENSE_CURL:
			user.stage.boost(Stat::DEF, 1);
			user.defense_curl();
			break;
		case Moves::DEFOG:
			weather.fog = false;
			target.stage.boost(Stat::EVA, -1);
			break;
		case Moves::DESTINY_BOND:
			user.use_destiny_bond();
			break;
		case Moves::DETECT:
		case Moves::PROTECT:
			user.protect();
			break;
		case Moves::DIG:
			user.dig();
			break;
		case Moves::DISABLE:		// Fix
			break;
		case Moves::DIVE:
			user.dive();
			break;
		case Moves::DOOM_DESIRE:	// Fix
		case Moves::FUTURE_SIGHT:
			break;
		case Moves::DOUBLE_TEAM:
		case Moves::MINIMIZE:
			user.stage.boost(Stat::EVA, 1);
			break;
		case Moves::DRACO_METEOR:
		case Moves::LEAF_STORM:
		case Moves::OVERHEAT:
		case Moves::PSYCHO_BOOST:
			user.stage.boost(Stat::SPA, -2);
			break;
		case Moves::DRAGON_DANCE:
			user.stage.boost(Stat::ATK, 1);
			user.stage.boost(Stat::SPE, 1);
			break;
		case Moves::DREAM_EATER:
			if (target.pokemon().status.is_sleeping ())
				absorb_hp(user.pokemon(), target.pokemon(), damage);
			break;
		case Moves::EMBARGO:
			target.embargo = 5;
			break;
		case Moves::ENCORE:
			if (!target.encore)
				target.encore = move.variable().value();
			break;
		case Moves::ENDURE:
			user.endure();
			break;
		case Moves::EXPLOSION:
		case Moves::SELFDESTRUCT:
			user.pokemon().hp.stat = 0;
			break;
		case Moves::FAKE_TEARS:
		case Moves::METAL_SOUND:
			target.stage.boost(Stat::SPD, -2);
			break;
		case Moves::FEINT:
			target.break_protect();
			break;
		case Moves::FIRE_FANG:
			fang_side_effects<Status::BURN>(user.pokemon(), target, weather);
			break;
		case Moves::FLARE_BLITZ:
			recoil_status<Status::BURN>(user.pokemon(), target.pokemon(), weather, damage);
			break;
		case Moves::FLASH:
		case Moves::KINESIS:
		case Moves::MUD_SLAP:
		case Moves::SAND_ATTACK:
		case Moves::SMOKESCREEN:
			target.stage.boost(Stat::ACC, -1);
			break;
		case Moves::FLATTER:
			confusing_stat_boost(move, target, Stat::SPA, 1);
			break;
		case Moves::FLING:
			user.pokemon().item.remove();
			break;
		case Moves::FLY:
			user.fly();
			break;
		case Moves::FOCUS_ENERGY:
			user.focus_energy();
			break;
		case Moves::FOCUS_PUNCH:		// Fix
			break;
		case Moves::FOLLOW_ME:		// Fix
			break;
		case Moves::FORESIGHT:
		case Moves::ODOR_SLEUTH:
			target.identify();
			break;
		case Moves::GASTRO_ACID:		// Fix
			break;
		case Moves::GLARE:
		case Moves::STUN_SPORE:
		case Moves::THUNDER_WAVE:
		case Moves::ZAP_CANNON:
			Status::apply<Status::PARALYSIS>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::GRAVITY:
			weather.set_gravity();
			break;
		case Moves::GROWL:
			target.stage.boost(Stat::ATK, -1);
			break;
		case Moves::GROWTH:
			user.stage.boost(Stat::SPA, 1);
			break;
		case Moves::GRUDGE:		// Fix
			break;
		case Moves::GUARD_SWAP:
			Stage::swap_defensive(user.stage, target.stage);
			break;
		case Moves::HAIL:
			weather.set_hail (user.pokemon().item.extends_hail());
			break;
		case Moves::HAMMER_ARM:
			user.stage.boost(Stat::SPE, -1);
			break;
		case Moves::HARDEN:
		case Moves::WITHDRAW:
			user.stage.boost(Stat::DEF, 1);
			break;
		case Moves::HAZE:
			user.stage.reset();
			target.stage.reset();
			break;
		case Moves::HEAD_SMASH:
			recoil (user.pokemon(), damage, 2);
			break;
		case Moves::HEAL_BELL:
			cure_all_status(user, [](Pokemon const & pokemon) {
				return !pokemon.ability.blocks_sound_moves();
			});
			break;
		case Moves::HEAL_BLOCK:
			if (!target.heal_block)
				target.heal_block = 5;
			break;
		case Moves::HEAL_ORDER:
		case Moves::MILK_DRINK:
		case Moves::RECOVER:
		case Moves::SLACK_OFF:
		case Moves::SOFTBOILED:
			heal (user.pokemon(), 2);
			break;
		case Moves::HEALING_WISH:		// Fix
			break;
		case Moves::HEART_SWAP:
			using std::swap;
			swap(user.stage, target.stage);
			break;
		case Moves::HI_JUMP_KICK:		// Fix
		case Moves::JUMP_KICK:
			break;
		case Moves::HOWL:
		case Moves::METAL_CLAW:
		case Moves::MEDITATE:
		case Moves::METEOR_MASH:
			if (move.variable().effect_activates())
				user.stage.boost(Stat::ATK, 1);
			break;
		case Moves::ICE_BALL:		// Fix
		case Moves::ROLLOUT:
			break;
		case Moves::ICE_FANG:
			fang_side_effects<Status::FREEZE>(user.pokemon(), target, weather);
			break;
		case Moves::ICY_WIND:
		case Moves::MUD_SHOT:
		case Moves::ROCK_TOMB:
		case Moves::STRING_SHOT:
			target.stage.boost(Stat::SPE, -1);
			break;
		case Moves::IMPRISON:
			user.imprison();
			break;
		case Moves::INGRAIN:
			user.ingrain();
			break;
		case Moves::KNOCK_OFF:		// Fix
			break;
		case Moves::LEECH_SEED:
			target.hit_with_leech_seed();
			break;
		case Moves::LEER:
		case Moves::TAIL_WHIP:
			target.stage.boost(Stat::DEF, -1);
			break;
		case Moves::LIGHT_SCREEN:
			user.screens.activate_light_screen(user.pokemon().item.extends_light_screen());
			break;
		case Moves::LOCK_ON:
		case Moves::MIND_READER:
			user.lock_on_to();
			break;
		case Moves::LUCKY_CHANT:
			user.screens.activate_lucky_chant();
			break;
		case Moves::LUNAR_DANCE:		// Fix
			break;
		case Moves::MAGIC_COAT:		// Fix
			break;
		case Moves::MAGNET_RISE:
			if (!user.magnet_rise)
				user.magnet_rise = 5;
			break;
		case Moves::ME_FIRST:		// Fix
			break;
		case Moves::MEMENTO:
			target.stage.boost_offensive(-2);
			user.pokemon().hp.stat = 0;
			break;
		case Moves::METAL_BURST:
			damage_side_effect (target.pokemon(), user.damaged() * 3u / 2);
			break;
		case Moves::MIMIC:		// Fix
			break;
		case Moves::MIRACLE_EYE:		// Fix
			break;
		case Moves::MIRROR_COAT:
			if (target.pokemon().move().is_special())
				damage_side_effect (target.pokemon(), user.damaged() * 2u);
			break;
		case Moves::MIRROR_SHOT:
		case Moves::MUD_BOMB:
		case Moves::MUDDY_WATER:
		case Moves::OCTAZOOKA:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::ACC, -1);
			break;
		case Moves::MIST:
			user.screens.activate_mist();
			break;
		case Moves::MIST_BALL:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPA, -1);
			break;
		case Moves::MOONLIGHT:
		case Moves::MORNING_SUN:
		case Moves::SYNTHESIS:
			if (weather.sun())
				heal (user.pokemon(), 3, 2);
			else if (weather.hail() or weather.rain() or weather.sand())
				heal (user.pokemon(), 4);
			else
				heal (user.pokemon(), 2);
			break;
		case Moves::MUD_SPORT:
			user.activate_mud_sport();
			break;
		case Moves::NASTY_PLOT:
		case Moves::TAIL_GLOW:
			user.stage.boost(Stat::SPA, 2);
			break;
		case Moves::NIGHTMARE:
			target.give_nightmares();
			break;
		case Moves::OUTRAGE:
		case Moves::PETAL_DANCE:
		case Moves::THRASH:
			if (!user.rampage)
				user.rampage = move.variable().value();
			break;
		case Moves::PAIN_SPLIT:
			equalize(user.pokemon().hp, target.pokemon().hp);
			break;
		case Moves::PERISH_SONG:
			user.activate_perish_song();
			target.activate_perish_song();
			break;
		case Moves::POISON_FANG:
			if (move.variable().effect_activates())
				Status::apply<Status::POISON_TOXIC>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::POISON_GAS:
		case Moves::POISONPOWDER:
			Status::apply<Status::POISON>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::POWER_SWAP:
			Stage::swap_offensive(user.stage, target.stage);
			break;
		case Moves::POWER_TRICK:
			user.activate_power_trick();
			break;
		case Moves::PRESENT:
			if (move.variable().present_heals()) {
				Stat & hp = target.pokemon().hp;
				hp.stat += 80;
				hp.stat = std::min(hp.stat, hp.max);
			}
			break;
		case Moves::PSYCH_UP:
			user.stage = target.stage;
			break;
		case Moves::PSYCHO_SHIFT:
			if (target.pokemon().status.is_clear())
				Status::shift(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::RAGE:		// Fix
			break;
		case Moves::RAIN_DANCE:
			weather.set_rain (user.pokemon().item.extends_rain());
			break;
		case Moves::RAPID_SPIN:
			clear_field(user, target);
			break;
		case Moves::RAZOR_WIND:	// Fix
			break;
		case Moves::RECYCLE:		// Fix
			break;
		case Moves::REFLECT:
			user.screens.activate_reflect(user.pokemon().item.extends_reflect());
			break;
		case Moves::REFRESH:
			user.pokemon().status.clear ();
			break;
		case Moves::REST:
			rest(user.pokemon());
			break;
		case Moves::ROAR:
		case Moves::WHIRLWIND:
			phaze(user, target, weather);
			break;
		case Moves::ROLE_PLAY:		// Fix
			break;
		case Moves::ROOST:
			user.roost();
			heal (user.pokemon(), 2);
			break;
		case Moves::SAFEGUARD:
			user.screens.activate_safeguard();
			break;
		case Moves::SANDSTORM:
			weather.set_sand (user.pokemon().item.extends_sand());
			break;
		case Moves::SCREECH:
			target.stage.boost(Stat::DEF, -2);
			break;
		case Moves::SEED_FLARE:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPD, -2);
			break;
		case Moves::SHADOW_FORCE:
			user.shadow_force();
			break;
		case Moves::SHARPEN:
			user.stage.boost(Stat::ATK, 1);
			break;
		case Moves::SKETCH:		// Fix
			break;
		case Moves::SKILL_SWAP:		// Fix
			break;
		case Moves::SKULL_BASH: // Fix
			break;
		case Moves::SKY_ATTACK:	// Fix
			if (move.variable().effect_activates()) {}
			break;
		case Moves::SMELLINGSALT:
			if (target.pokemon().status.boosts_smellingsalt())
				target.pokemon().status.clear();
			break;
		case Moves::SNATCH:	// Fix
			break;
		case Moves::SOLARBEAM:		// Fix
			break;
		case Moves::SPIKES:
			target.entry_hazards.add_spikes();
			break;
		case Moves::SPIT_UP:		// Fix
			break;
		case Moves::SPITE:		// Fix
			break;
		case Moves::STEALTH_ROCK:
			target.entry_hazards.add_stealth_rock();
			break;
		case Moves::STEEL_WING:
			if (move.variable().effect_activates())
				user.stage.boost(Stat::DEF, 1);
			break;
		case Moves::STOCKPILE:		// Fix
			break;
		case Moves::STRUGGLE:
			struggle(user.pokemon());
			break;
		case Moves::SUBMISSION:
		case Moves::TAKE_DOWN:
			recoil (user.pokemon(), damage, 4);
			break;
		case Moves::SUBSTITUTE:
			user.substitute();		
			break;
		case Moves::SUNNY_DAY:
			weather.set_sun (user.pokemon().item.extends_sun());
			break;
		case Moves::SUPERPOWER:
			user.stage.boost_physical(-1);
			break;
		case Moves::SWAGGER:
			confusing_stat_boost(move, target, Stat::ATK, 2);
			break;
		case Moves::SWALLOW:		// Fix
			break;
		case Moves::SWEET_SCENT:
			target.stage.boost(Stat::EVA, -1);
			break;
		case Moves::SWITCH0:
		case Moves::SWITCH1:
		case Moves::SWITCH2:
		case Moves::SWITCH3:
		case Moves::SWITCH4:
		case Moves::SWITCH5:
			user.pokemon.replacement_from_switch();
			switchpokemon (user, target, weather);
			break;
		case Moves::SWITCHEROO:
		case Moves::TRICK:
			swap_items(user.pokemon(), target.pokemon());
			break;
		case Moves::SWORDS_DANCE:
			user.stage.boost(Stat::ATK, 2);
			break;
		case Moves::TAILWIND:
			user.screens.activate_tailwind();
			break;
		case Moves::TAUNT:
			target.taunt();
			break;
		case Moves::THUNDER_FANG:
			fang_side_effects<Status::PARALYSIS>(user.pokemon(), target, weather);
			break;
		case Moves::TICKLE:
			target.stage.boost_physical(-1);
			break;
		case Moves::TORMENT:
			target.torment();
			break;
		case Moves::TOXIC:
			Status::apply<Status::POISON_TOXIC>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::TOXIC_SPIKES:
			target.entry_hazards.add_toxic_spikes();
			break;
		case Moves::TRANSFORM:		// Fix
			break;
		case Moves::TRI_ATTACK:
			tri_attack_status(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::TRICK_ROOM:
			weather.set_trick_room ();
			break;
		case Moves::U_TURN:
			user.u_turn();
			break;
		case Moves::UPROAR:
			// TODO: make this make sense
			weather.set_uproar (static_cast<int8_t> (move.variable().value()));
			user.use_uproar();
			break;
		case Moves::VOLT_TACKLE:
			recoil_status<Status::PARALYSIS>(user.pokemon(), target.pokemon(), weather, damage);
			break;
		case Moves::WAKE_UP_SLAP:
			if (target.pokemon().status.is_sleeping ())
				target.pokemon().status.clear ();
			break;
		case Moves::WATER_SPORT:
			user.activate_water_sport();
			break;
		case Moves::WILL_O_WISP:
			Status::apply<Status::BURN>(user.pokemon(), target.pokemon(), weather);
			break;
		case Moves::WISH:
			user.wish.activate();
			break;
		case Moves::WORRY_SEED:		// Fix
			break;
		case Moves::YAWN:
			target.hit_with_yawn();
			break;
		default:
			break;
	}
}

// I could potentially treat this as negative recoil
void absorb_hp(Pokemon & user, Pokemon const & target, unsigned const damage) {
	if (target.ability.damages_leechers()) {
		if (damage <= 3)
			--user.hp.stat;
		else
			damage_side_effect (user, damage / 2);
	}
	else {
		if (damage <= 3)
			++user.hp.stat;
		else {
			user.hp.stat += damage / 2;
			if (user.hp.stat > user.hp.max)
				user.hp.stat = user.hp.max;
		}
	}
}

void belly_drum(Team & user) {
	Stat & hp = user.pokemon().hp;
	if (hp.stat > hp.max / 2 and hp.stat > 1) {
		hp.stat -= hp.max / 2;
		user.stage.maximize_attack();
	}
}

void clear_field(Team & user, Team const & target) {
	if (user.pokemon().move().type().get_effectiveness(target.pokemon()) > 0)
		user.clear_field();
}

void confusing_stat_boost(Move const & move, Team & target, Stat::Stats const stat, int const stages) {
	target.stage.boost(stat, stages);
	target.confuse();
}

void cure_all_status(Team & user, std::function<bool(Pokemon const &)> const & predicate) {
	user.pokemon.for_each([& predicate](Pokemon & pokemon) {
		if (predicate(pokemon))
			pokemon.status.clear ();
	});
}

void curse(Team & user, Team & target) {
	Pokemon & pokemon = user.pokemon();
	if (is_type(user, Type::GHOST) and !pokemon.ability.blocks_secondary_damage()) {
		if (!target.is_cursed()) {
			if (pokemon.hp.max <= 3)
				--pokemon.hp.stat;
			else
				damage_side_effect(pokemon, pokemon.hp.max / 2);
			target.curse();
		}
	}
	else {
		user.stage.boost_physical(1);
		user.stage.boost(Stat::SPE, -1);
	}
}

void equalize(Stat & hp1, Stat & hp2) {
	Stat::stat_type const temp = (hp1.stat + hp2.stat) / 2;
	hp1.stat = std::min(temp, hp1.max);
	hp2.stat = std::min(temp, hp2.max);
}

void phaze(Team & user, Team & target, Weather & weather) {
	if (target.can_be_phazed()) {
		uint8_t const index = user.pokemon().move().variable.phaze_index(target.pokemon.index());
		target.pokemon.set_replacement(index);
		switchpokemon(target, user, weather);
		target.move();
	}
}

void rest(Pokemon & user) {
	Stat & hp = user.hp;
	if (hp.stat != hp.max) {
		hp.stat = hp.max;
		user.status.rest();
	}
}

void struggle(Pokemon & user) {
	if (user.hp.max <= 7)
		--user.hp.stat;
	else
		damage_side_effect (user, user.hp.max / 4);
}

void swap_items(Pokemon & user, Pokemon & target) {
	// Add support for abilities that block Trick / Switcheroo
	if (!user.item.blocks_trick () and !target.item.blocks_trick()) {
		using std::swap;
		swap (user.item, target.item);
	}
}

void tri_attack_status(Pokemon & user, Pokemon & target, Weather const & weather) {
	switch (user.move().variable().value()) {
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

void call_other_move (Team & user) {
	user.pokemon().move.set_index(user.called_move);
}

}	// unnamed namespace
}	// namespace technicalmachine
