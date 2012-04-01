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

#include <cassert>

#include "ability.hpp"
#include "block.hpp"
#include "damage.hpp"
#include "heal.hpp"
#include "move.hpp"
#include "move_power.hpp"
#include "pokemon.hpp"
#include "stat.hpp"
#include "status.hpp"
#include "switch.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"
#undef SING

namespace technicalmachine {
namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, unsigned log_damage);
void call_other_move (Team & user);
void do_effects_before_moving (Pokemon & user, Team & target);
void do_damage (Team & user, Team & target, unsigned damage);
void do_side_effects (Team & user, Team & target, Weather & weather, unsigned damage);
void lower_pp (Team & user, Pokemon const & target);

}	// unnamed namespace

unsigned call_move (Team & user, Team & target, Weather & weather, unsigned log_damage) {
	unsigned damage = (log_damage == -1u) ? 0 : log_damage;
	user.destiny_bond = false;
	user.lock_on = false;
	user.moved = true;
	bool const execute = can_execute_move (user, target, weather);
	if (execute) {
		lower_pp (user, target.pokemon());
		size_t index = user.pokemon().move.index;
		switch (user.pokemon().move().name) {
//			case Move::NATURE_POWER:
//				break;
			case Move::ASSIST:
			case Move::COPYCAT:
			case Move::ME_FIRST:
			case Move::METRONOME:
			case Move::MIRROR_MOVE:
			case Move::SLEEP_TALK:
				call_other_move (user);
				// fall through
			default:
				if (!user.miss)
					damage = use_move (user, target, weather, log_damage);
				break;
		}
		user.pokemon().move.index = index;
	}
	return damage;
}

namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, unsigned log_damage) {
	calculate_speed (user, weather);
	calculate_speed (target, weather);
	move_power (user, target, weather);
	unsigned damage = 0;
	
	if (user.pokemon().move().basepower != 0) {
		do_effects_before_moving (user.pokemon(), target);
		if (log_damage == -1u) {
			calculate_defending_stat (user, target, weather);
			calculate_attacking_stat (user, weather);
			damage = damage_calculator (user, target, weather);
		}
		else {
			damage = log_damage;
		}
		if (damage != 0)
			do_damage (user, target, damage);
	}
	++user.pokemon().move().times_used;

	do_side_effects (user, target, weather, damage);

	return damage;
}

void do_effects_before_moving (Pokemon & user, Team & target) {
	if (user.move().name == Move::BRICK_BREAK) {
		target.reflect = 0;
		target.light_screen = 0;
	}
	else if (user.move().is_usable_while_frozen ()) {
		if (user.status.name == Status::FREEZE)
			user.status.clear ();
	}
}

void do_damage (Team & user, Team & target, unsigned damage) {
	damage_side_effect (target.pokemon(), damage);
	if (user.pokemon().item.causes_recoil())
		heal (user.pokemon(), -10);
	if (target.pokemon().hp.stat > 0) {
		if (target.bide != 0)
			target.bide_damage += damage;
	}
}

void do_side_effects (Team & user, Team & target, Weather & weather, unsigned damage) {
	switch (user.pokemon().move().name) {
		case Move::DREAM_EATER:
			if (!target.pokemon().status.is_sleeping ())
				break;
		case Move::ABSORB:
		case Move::DRAIN_PUNCH:
		case Move::GIGA_DRAIN:
		case Move::LEECH_LIFE:
		case Move::MEGA_DRAIN:
			if (target.pokemon().ability.name == Ability::LIQUID_OOZE) {
				if (damage <= 3)
					--user.pokemon().hp.stat;
				else
					damage_side_effect (user.pokemon(), damage / 2);
			}
			else {
				if (damage <= 3)
					++user.pokemon().hp.stat;
				else {
					user.pokemon().hp.stat += damage / 2;
					if (user.pokemon().hp.stat > user.pokemon().hp.max)
						user.pokemon().hp.stat = user.pokemon().hp.max;
				}
			}
		case Move::ACID:
		case Move::BUG_BUZZ:
		case Move::EARTH_POWER:
		case Move::ENERGY_BALL:
		case Move::FLASH_CANNON:
		case Move::FOCUS_BLAST:
		case Move::LUSTER_PURGE:
		case Move::PSYCHIC:
		case Move::SHADOW_BALL:
			if (user.pokemon().move().variable().first != 0)
				Stat::boost (target.stage [Stat::SPD], -1);
			break;
		case Move::ACID_ARMOR:
		case Move::BARRIER:
		case Move::IRON_DEFENSE:
			Stat::boost (user.stage [Stat::DEF], 2);
			break;
		case Move::ACUPRESSURE:		// fix
			Stat::boost (user.stage [user.pokemon().move().variable().first], 2);
			break;
		case Move::AGILITY:
		case Move::ROCK_POLISH:
			Stat::boost (user.stage [Stat::SPE], 2);
			break;
		case Move::AIR_SLASH:
		case Move::ASTONISH:
		case Move::BITE:
		case Move::BONE_CLUB:
		case Move::DARK_PULSE:
		case Move::DRAGON_RUSH:
		case Move::EXTRASENSORY:
		case Move::FAKE_OUT:
		case Move::HEADBUTT:
		case Move::HYPER_FANG:
		case Move::IRON_HEAD:
		case Move::NEEDLE_ARM:
		case Move::ROCK_SLIDE:
		case Move::ROLLING_KICK:
		case Move::SNORE:
		case Move::STOMP:
		case Move::TWISTER:
		case Move::WATERFALL:
		case Move::ZEN_HEADBUTT:
			if (user.pokemon().move().variable().first != 0)
				target.flinch = true;
			break;
		case Move::AMNESIA:
			Stat::boost (user.stage [Stat::SPD], 2);
			break;
		case Move::ANCIENTPOWER:
		case Move::OMINOUS_WIND:
		case Move::SILVER_WIND:
			if (user.pokemon().move().variable().first != 0) {
				for (Stat::Stats stat = Stat::ATK; stat <= Stat::SPE; stat = static_cast <Stat::Stats> (stat + 1))
					Stat::boost (user.stage [stat], 1);
			}
			break;
		case Move::AQUA_RING:
			user.aqua_ring = true;
			break;
		case Move::AURORA_BEAM:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::GROWL:
			Stat::boost (target.stage [Stat::ATK], -1);
			break;
		case Move::AROMATHERAPY: {
			for (Pokemon & pokemon : user.pokemon.set)
				pokemon.status.clear ();
			break;
		}
		case Move::ATTRACT:
			if (user.pokemon().gender.multiplier (target.pokemon().gender) == -1)
				target.attract = true;
			break;
		case Move::BATON_PASS:
			user.pass = true;
			break;
		case Move::BELLY_DRUM:
			if (user.pokemon().hp.stat > user.pokemon().hp.max / 2 and user.pokemon().hp.stat > 1) {
				user.pokemon().hp.stat -= user.pokemon().hp.max / 2;
				user.stage [Stat::ATK] = 6;
			}
			break;
		case Move::BIDE:
			if (user.bide == 0) {
				user.bide = 2;
				user.bide_damage = 0;
			}
			else {
				if (user.bide == 1)
					damage_side_effect (target.pokemon(), user.bide_damage * 2u);
				--user.bide;
			}
			break;
		case Move::BIND:
		case Move::CLAMP:
		case Move::FIRE_SPIN:
		case Move::MAGMA_STORM:
		case Move::SAND_TOMB:
		case Move::WHIRLPOOL:
		case Move::WRAP:
			if (target.partial_trap == 0)
				target.partial_trap = user.pokemon().move().variable().first;
			break;
		case Move::BLAST_BURN:
		case Move::FRENZY_PLANT:
		case Move::GIGA_IMPACT:
		case Move::HYDRO_CANNON:
		case Move::HYPER_BEAM:
		case Move::ROAR_OF_TIME:
		case Move::ROCK_WRECKER:
			user.recharging = true;
			break;
		case Move::FLARE_BLITZ:
			recoil (user.pokemon(), damage, 3);
		case Move::BLAZE_KICK:
		case Move::EMBER:
		case Move::FIRE_BLAST:
		case Move::FIRE_PUNCH:
		case Move::FLAME_WHEEL:
		case Move::FLAMETHROWER:
		case Move::HEAT_WAVE:
		case Move::LAVA_PLUME:
		case Move::SACRED_FIRE:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::WILL_O_WISP:
			Status::burn (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::BLIZZARD:
		case Move::ICE_BEAM:
		case Move::ICE_PUNCH:
		case Move::POWDER_SNOW:
			if (user.pokemon().move().variable().first != 0)
				Status::freeze (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::BLOCK:
		case Move::MEAN_LOOK:
		case Move::SPIDER_WEB:
			target.trapped = true;
			break;
		case Move::VOLT_TACKLE:
			recoil (user.pokemon(), damage, 3);
		case Move::BODY_SLAM:
		case Move::DISCHARGE:
		case Move::DRAGONBREATH:
		case Move::FORCE_PALM:
		case Move::LICK:
		case Move::SECRET_POWER:
		case Move::SPARK:
		case Move::THUNDER:
		case Move::THUNDERBOLT:
		case Move::THUNDERPUNCH:
		case Move::THUNDERSHOCK:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::GLARE:
		case Move::STUN_SPORE:
		case Move::THUNDER_WAVE:
		case Move::ZAP_CANNON:
			Status::paralyze (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::BOUNCE:
			if (user.vanish == LANDED)
				user.vanish = BOUNCED;
			else
				user.vanish = LANDED;
			break;
		case Move::BRAVE_BIRD:
		case Move::DOUBLE_EDGE:
		case Move::WOOD_HAMMER:
			recoil (user.pokemon(), damage, 3);
			break;
		case Move::BUBBLE:
		case Move::BUBBLEBEAM:
		case Move::CONSTRICT:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::ICY_WIND:
		case Move::MUD_SHOT:
		case Move::ROCK_TOMB:
		case Move::STRING_SHOT:
			Stat::boost (target.stage [Stat::SPE], -1);
			break;
		case Move::BUG_BITE:			// Fix
		case Move::PLUCK:
			break;
		case Move::BULK_UP:
			Stat::boost (user.stage [Stat::ATK], 1);
			Stat::boost (user.stage [Stat::DEF], 1);
			break;
		case Move::CALM_MIND:
			Stat::boost (user.stage [Stat::SPA], 1);
			Stat::boost (user.stage [Stat::SPD], 1);
			break;
		case Move::CAMOUFLAGE:
			break;
		case Move::CAPTIVATE:
			if (user.pokemon().gender.multiplier (target.pokemon().gender) == -1)
				Stat::boost (target.stage [Stat::SPD], -2);
			break;
		case Move::CHARGE:
			user.charge = true;
			Stat::boost (user.stage [Stat::SPD], 1);
			break;
		case Move::CHARGE_BEAM:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::GROWTH:
			Stat::boost (user.stage [Stat::SPA], 1);
			break;
		case Move::CHARM:
		case Move::FEATHERDANCE:
			Stat::boost (target.stage [Stat::ATK], -2);
			break;
		case Move::CHATTER:
			if (user.pokemon().name != CHATOT)
				break;
		case Move::CONFUSION:
		case Move::DIZZY_PUNCH:
		case Move::DYNAMICPUNCH:
		case Move::PSYBEAM:
		case Move::ROCK_CLIMB:
		case Move::SIGNAL_BEAM:
		case Move::WATER_PULSE:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::CONFUSE_RAY:
		case Move::SUPERSONIC:
		case Move::SWEET_KISS:
		case Move::TEETER_DANCE:
			if (target.pokemon().ability.name != Ability::OWN_TEMPO and target.confused == 0)
				target.confused = user.pokemon().move().variable().first;
			break;
		case Move::CLOSE_COMBAT:
			Stat::boost (user.stage [Stat::ATK], -1);
			Stat::boost (user.stage [Stat::DEF], -1);
			break;
		case Move::CONVERSION:		// Fix
			break;
		case Move::CONVERSION2:	// Fix
			break;
		case Move::COSMIC_POWER:
		case Move::DEFEND_ORDER:
			Stat::boost (user.stage [Stat::DEF], 1);
			Stat::boost (user.stage [Stat::SPD], 1);
			break;
		case Move::CRUNCH:
		case Move::CRUSH_CLAW:
		case Move::IRON_TAIL:
		case Move::ROCK_SMASH:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::LEER:
		case Move::TAIL_WHIP:
			Stat::boost (target.stage [Stat::DEF], -1);
			break;
		case Move::COTTON_SPORE:
		case Move::SCARY_FACE:
			Stat::boost (target.stage [Stat::SPE], -2);
			break;
		case Move::COUNTER:
			if (target.pokemon().move().physical)
				damage_side_effect (target.pokemon(), user.damage * 2u);
			break;
		case Move::COVET:
		case Move::THIEF:
			user.pokemon().item.steal (target.pokemon().item);
			break;
		case Move::CROSS_POISON:
		case Move::GUNK_SHOT:
		case Move::POISON_JAB:
		case Move::POISON_STING:
		case Move::POISON_TAIL:
		case Move::SLUDGE:
		case Move::SLUDGE_BOMB:
		case Move::SMOG:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::POISON_GAS:
		case Move::POISONPOWDER:
			Status::poison (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::CURSE:
			if (is_type (user, Type::GHOST) and user.pokemon().ability.name != Ability::MAGIC_GUARD) {
				if (!target.curse) {
					if (user.pokemon().hp.max <= 3)
						--user.pokemon().hp.stat;
					else
						damage_side_effect (user.pokemon(), user.pokemon().hp.max / 2);
					target.curse = true;
				}
			}
			else {
				Stat::boost (user.stage [Stat::ATK], 1);
				Stat::boost (user.stage [Stat::DEF], 1);
				Stat::boost (user.stage [Stat::SPE], -1);
			}
			break;
		case Move::DARK_VOID:
		case Move::GRASSWHISTLE:
		case Move::HYPNOSIS:
		case Move::LOVELY_KISS:
		case Move::SING:
		case Move::SLEEP_POWDER:
		case Move::SPORE:
			Status::sleep (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::DEFENSE_CURL:
			Stat::boost (user.stage [Stat::DEF], 1);
			user.defense_curl = true;
			break;
		case Move::DEFOG:
			weather.fog = false;
		// Fall through
		case Move::SWEET_SCENT:
			Stat::boost (target.stage [Stat::EVA], -1);
			break;
		case Move::DESTINY_BOND:
			user.destiny_bond = true;
			break;
		case Move::DETECT:
		case Move::PROTECT:
			user.protect = true;
			break;
		case Move::DIG:
			if (user.vanish == LANDED)
				user.vanish = DUG;
			else
				user.vanish = LANDED;
			break;
		case Move::DISABLE:		// Fix
			break;
		case Move::DIVE:
			if (user.vanish == LANDED)
				user.vanish = DIVED;
			else
				user.vanish = LANDED;
			break;
		case Move::DOOM_DESIRE:	// Fix
		case Move::FUTURE_SIGHT:
			break;
		case Move::DOUBLE_TEAM:
		case Move::MINIMIZE:
			Stat::boost (user.stage [Stat::EVA], 1);
			break;
		case Move::DRACO_METEOR:
		case Move::LEAF_STORM:
		case Move::OVERHEAT:
		case Move::PSYCHO_BOOST:
			Stat::boost (user.stage [Stat::SPA], -2);
			break;
		case Move::DRAGON_DANCE:
			Stat::boost (user.stage [Stat::ATK], 1);
			Stat::boost (user.stage [Stat::SPE], 1);
			break;
		case Move::EMBARGO:
			target.embargo = 5;
			break;
		case Move::ENCORE:
			if (target.encore == 0)
				target.encore = user.pokemon().move().variable().first;
			break;
		case Move::ENDURE:
			user.endure = true;
			break;
		case Move::EXPLOSION:
		case Move::SELFDESTRUCT:
			user.pokemon().hp.stat = 0;
			break;
		case Move::SEED_FLARE:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::FAKE_TEARS:
		case Move::METAL_SOUND:
			Stat::boost (target.stage [Stat::SPD], -2);
			break;
		case Move::FEINT:
			target.protect = false;
			break;
		case Move::FIRE_FANG:
			if (user.pokemon().move().variable().first != 0) {
				if (user.pokemon().move().variable().first != 2)
					Status::burn (user.pokemon(), target.pokemon(), weather);
				if (user.pokemon().move().variable().first != 1)
					target.flinch = true;
			}
			break;
		case Move::MIRROR_SHOT:
		case Move::MUD_BOMB:
		case Move::MUDDY_WATER:
		case Move::OCTAZOOKA:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::FLASH:
		case Move::KINESIS:
		case Move::MUD_SLAP:
		case Move::SAND_ATTACK:
		case Move::SMOKESCREEN:
			Stat::boost (target.stage [Stat::ACC], -1);
			break;
		case Move::FLATTER:
			Stat::boost (target.stage [Stat::SPA], 1);
			if (target.pokemon().ability.name != Ability::OWN_TEMPO and target.confused == 0)
				target.confused = user.pokemon().move().variable().first;
			break;
		case Move::FLING:
			user.pokemon().item.remove();
			break;
		case Move::FLY:
			if (user.vanish == LANDED)
				user.vanish = FLOWN;
			else
				user.vanish = LANDED;
			break;
		case Move::FOCUS_ENERGY:
			user.focus_energy = true;
			break;
		case Move::FOCUS_PUNCH:		// Fix
			break;
		case Move::FOLLOW_ME:		// Fix
			break;
		case Move::FORESIGHT:
		case Move::ODOR_SLEUTH:
			target.identified = true;
			break;
		case Move::GASTRO_ACID:		// Fix
			break;
		case Move::GRAVITY:
			weather.set_gravity();
			break;
		case Move::GRUDGE:		// Fix
			break;
		case Move::GUARD_SWAP:
			std::swap (user.stage [Stat::DEF], target.stage [Stat::DEF]);
			std::swap (user.stage [Stat::SPD], target.stage [Stat::SPD]);
			break;
		case Move::HAIL:
			if (user.pokemon().item.extends_hail())
				weather.set_hail (8);
			else
				weather.set_hail (5);
			break;
		case Move::HAMMER_ARM:
			Stat::boost (user.stage [Stat::SPE], -1);
			break;
		case Move::STEEL_WING:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::HARDEN:
		case Move::WITHDRAW:
			Stat::boost (user.stage [Stat::DEF], 1);
			break;
		case Move::HAZE:
			for (Stat::Stats stat = Stat::ATK; stat != Stat::END; stat = static_cast <Stat::Stats> (stat + 1))
				user.stage [stat] = 0;
			for (Stat::Stats stat = Stat::ATK; stat != Stat::END; stat = static_cast <Stat::Stats> (stat + 1))
				target.stage [stat] = 0;
			break;
		case Move::HEAD_SMASH:
			recoil (user.pokemon(), damage, 2);
			break;
		case Move::HEAL_BELL: {
			for (Pokemon & pokemon : user.pokemon.set) {
				if (pokemon.ability.name != Ability::SOUNDPROOF)
					pokemon.status.clear ();
			}
			break;
		}
		case Move::HEAL_BLOCK:
			if (target.heal_block == 0)
				target.heal_block = 5;
			break;
		case Move::ROOST:
			user.roost = true;
		// Fall through
		case Move::HEAL_ORDER:
		case Move::MILK_DRINK:
		case Move::RECOVER:
		case Move::SLACK_OFF:
		case Move::SOFTBOILED:
			heal (user.pokemon(), 2);
			break;
		case Move::HEALING_WISH:		// Fix
			break;
		case Move::HEART_SWAP:
			for (Stat::Stats stat = Stat::ATK; stat != Stat::END; stat = static_cast <Stat::Stats> (stat + 1))
				std::swap (user.stage [stat], target.stage [stat]);
			break;
		case Move::HI_JUMP_KICK:		// Fix
		case Move::JUMP_KICK:
			break;
		case Move::HOWL:
		case Move::MEDITATE:
		case Move::METEOR_MASH:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::SHARPEN:
			Stat::boost (user.stage [Stat::ATK], 1);
			break;
		case Move::ICE_BALL:		// Fix
		case Move::ROLLOUT:
			break;
		case Move::ICE_FANG:
			if (user.pokemon().move().variable().first != 0) {
				if (user.pokemon().move().variable().first != 2)
					Status::freeze (user.pokemon(), target.pokemon(), weather);
				if (user.pokemon().move().variable().first != 1)
					target.flinch = true;
			}
			break;
		case Move::IMPRISON:
			user.imprison = true;
			break;
		case Move::INGRAIN:
			user.ingrain = true;
			break;
		case Move::KNOCK_OFF:		// Fix
			break;
		case Move::LEECH_SEED:
			target.leech_seed = true;
			break;
		case Move::LIGHT_SCREEN:
			if (user.light_screen == 0) {
				if (user.pokemon().item.extends_light_screen())
					user.light_screen = 8;
				else
					user.light_screen = 5;
			}
			break;
		case Move::LOCK_ON:
		case Move::MIND_READER:
			user.lock_on = true;
			break;
		case Move::LUCKY_CHANT:
			if (user.lucky_chant == 0)
				user.lucky_chant = 5;
			break;
		case Move::LUNAR_DANCE:		// Fix
			break;
		case Move::MAGIC_COAT:		// Fix
			break;
		case Move::MAGNET_RISE:
			if (user.magnet_rise == 0)
				user.magnet_rise = 5;
			break;
		case Move::ME_FIRST:		// Fix
			break;
		case Move::MEMENTO:
			Stat::boost (target.stage [Stat::ATK], -2);
			Stat::boost (target.stage [Stat::SPA], -2);
			user.pokemon().hp.stat = 0;
			break;
		case Move::METAL_BURST:
			damage_side_effect (target.pokemon(), user.damage * 3u / 2);
			break;
		case Move::METAL_CLAW:
		case Move::MIMIC:		// Fix
			break;
		case Move::MIRACLE_EYE:		// Fix
			break;
		case Move::MIRROR_COAT:
			if (!target.pokemon().move().physical)
				damage_side_effect (target.pokemon(), user.damage * 2u);
			break;
		case Move::MIST:
			user.mist = 5;
			break;
		case Move::MIST_BALL:
			if (user.pokemon().move().variable().first != 0)
				Stat::boost (target.stage [Stat::SPA], -1);
			break;
		case Move::MOONLIGHT:
		case Move::MORNING_SUN:
		case Move::SYNTHESIS:
			if (weather.sun)
				heal (user.pokemon(), 3, 2);
			else if (weather.hail or weather.rain or weather.sand)
				heal (user.pokemon(), 4);
			else
				heal (user.pokemon(), 2);
			break;
		case Move::MUD_SPORT:
			user.mud_sport = true;
			break;
		case Move::NASTY_PLOT:
		case Move::TAIL_GLOW:
			Stat::boost (user.stage [Stat::SPA], 2);
			break;
		case Move::NIGHTMARE:
			target.nightmare = true;
			break;
		case Move::OUTRAGE:
		case Move::PETAL_DANCE:
		case Move::THRASH:
			if (user.rampage == 0)
				user.rampage = user.pokemon().move().variable().first;
			break;
		case Move::PAIN_SPLIT:
			user.pokemon().hp.stat = (user.pokemon().hp.stat + target.pokemon().hp.stat) / 2;
			target.pokemon().hp.stat = user.pokemon().hp.stat;
			if (user.pokemon().hp.stat > user.pokemon().hp.max)
				user.pokemon().hp.stat = user.pokemon().hp.max;
			if (target.pokemon().hp.stat > target.pokemon().hp.max)
				target.pokemon().hp.stat = target.pokemon().hp.max;
			break;
		case Move::PERISH_SONG:
			if (user.perish_song == 0)
				user.perish_song = 3;
			if (target.perish_song == 0)
				target.perish_song = 3;
			break;
		case Move::POISON_FANG:
			if (user.pokemon().move().variable().first == 0)
				break;
		case Move::TOXIC:
			Status::poison_toxic (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::POWER_SWAP:
			std::swap (user.stage [Stat::ATK], target.stage [Stat::ATK]);
			std::swap (user.stage [Stat::SPA], target.stage [Stat::SPA]);
			break;
		case Move::POWER_TRICK:
			if (user.power_trick)
				user.power_trick = false;
			else
				user.power_trick = true;
			break;
		case Move::PRESENT:
			if (user.pokemon().move().variable().first == 0) {
				target.pokemon().hp.stat += 80;
				if (target.pokemon().hp.stat > target.pokemon().hp.max)
					target.pokemon().hp.stat = target.pokemon().hp.max;
			}
			break;
		case Move::PSYCH_UP:
			for (Stat::Stats stat = Stat::ATK; stat != Stat::END; stat = static_cast <Stat::Stats> (stat + 1))
				user.stage [stat] = target.stage [stat];
			break;
		case Move::PSYCHO_SHIFT:
			if (target.pokemon().status.name == Status::NO_STATUS) {
				switch (user.pokemon().status.name) {
					case Status::BURN:
						Status::burn (user.pokemon(), target.pokemon(), weather);
						break;
					case Status::PARALYSIS:
						Status::paralyze (user.pokemon(), target.pokemon(), weather);
						break;
					case Status::POISON:
						Status::poison (user.pokemon(), target.pokemon(), weather);
						break;
					case Status::POISON_TOXIC:
						Status::poison_toxic (user.pokemon(), target.pokemon(), weather);
						break;
					case Status::REST:		// Fix
					case Status::SLEEP:
						Status::sleep (user.pokemon(), target.pokemon(), weather);
						break;
					default:
						break;
				}
				user.pokemon().status.clear ();
			}
			break;
		case Move::RAGE:		// Fix
			break;
		case Move::RAIN_DANCE:
			if (user.pokemon().item.extends_rain())
				weather.set_rain (8);
			else
				weather.set_rain (5);
			break;
		case Move::RAPID_SPIN:
			if (get_effectiveness (user.pokemon().move().type, target.pokemon()) > 0) {
				user.spikes = 0;
				user.stealth_rock = false;
				user.toxic_spikes = 0;
				user.leech_seed = false;
				user.partial_trap = false;
			}
			break;
		case Move::RAZOR_WIND:	// Fix
			break;
		case Move::RECYCLE:		// Fix
			break;
		case Move::REFLECT:
			if (user.reflect == 0) {
				if (user.pokemon().item.extends_reflect())
					user.reflect = 8;
				else
					user.reflect = 5;
			}
			break;
		case Move::REFRESH:
			user.pokemon().status.clear ();
			break;
		case Move::REST:
			if (user.pokemon().hp.stat != user.pokemon().hp.max) {
				user.pokemon().hp.stat = user.pokemon().hp.max;
				user.pokemon().status.name = Status::REST;
				user.pokemon().sleep = 3;
			}
			break;
		case Move::ROAR:
		case Move::WHIRLWIND:
			if ((target.pokemon().ability.name != Ability::SOUNDPROOF or user.pokemon().move().name != Move::ROAR) and !target.ingrain and target.pokemon().ability.name != Ability::SUCTION_CUPS) {
				if (target.pokemon.set.size() > 1) {
					target.replacement = user.pokemon().move().variable().first;
					switchpokemon (target, user, weather);
					target.moved = true;
				}
			}
			break;
		case Move::ROLE_PLAY:		// Fix
			break;
		case Move::SAFEGUARD:
			if (user.safeguard == 0)
				user.safeguard = 5;
			break;
		case Move::SANDSTORM:
			if (user.pokemon().item.extends_sand())
				weather.set_sand (8);
			else
				weather.set_sand (5);
			break;
		case Move::SCREECH:
			Stat::boost (target.stage [Stat::DEF], -2);
			break;
		case Move::SHADOW_FORCE:
			if (user.vanish == LANDED)
				user.vanish = SHADOW_FORCED;
			else
				user.vanish = LANDED;
			break;
		case Move::SKETCH:		// Fix
			break;
		case Move::SKILL_SWAP:		// Fix
			break;
		case Move::SKULL_BASH: // Fix
			break;
		case Move::SKY_ATTACK:
			if (user.pokemon().move().variable().first != 0) {}
				break;
		case Move::SMELLINGSALT:
			if (target.pokemon().status.name == Status::PARALYSIS)
				target.pokemon().status.clear ();
			break;
		case Move::SNATCH:	// Fix
			break;
		case Move::SOLARBEAM:		// Fix
			break;
		case Move::SPIKES:
			if (target.spikes < 3)
				++target.spikes;
			break;
		case Move::SPIT_UP:		// Fix
			break;
		case Move::SPITE:		// Fix
			break;
		case Move::STEALTH_ROCK:
			target.stealth_rock = true;
			break;
		case Move::STOCKPILE:		// Fix
			break;
		case Move::STRUGGLE:
			if (user.pokemon().hp.max <= 7)
				--user.pokemon().hp.stat;
			else
				damage_side_effect (user.pokemon(), user.pokemon().hp.max / 4);
			break;
		case Move::SUBMISSION:
		case Move::TAKE_DOWN:
			recoil (user.pokemon(), damage, 4);
			break;
		case Move::SUBSTITUTE:
			if (!user.substitute and user.pokemon().hp.stat > user.pokemon().hp.max / 4) {
				user.substitute = user.pokemon().hp.max / 4;
				user.pokemon().hp.stat -= user.pokemon().hp.max / 4;
			}
			break;
		case Move::SUNNY_DAY:
			if (user.pokemon().item.extends_sun())
				weather.set_sun (8);
			else
				weather.set_sun (5);
			break;
		case Move::SUPERPOWER:
			Stat::boost (user.stage [Stat::ATK], -1);
			Stat::boost (user.stage [Stat::DEF], -1);
			break;
		case Move::SWAGGER:
			Stat::boost (target.stage [Stat::ATK], 2);
			if (target.pokemon().ability.name != Ability::OWN_TEMPO and target.confused == 0)
				target.confused = user.pokemon().move().variable().first;
		case Move::SWALLOW:		// Fix
			break;
		case Move::SWITCH0:
		case Move::SWITCH1:
		case Move::SWITCH2:
		case Move::SWITCH3:
		case Move::SWITCH4:
		case Move::SWITCH5:
			user.replacement = user.pokemon().move().to_replacement ();
			switchpokemon (user, target, weather);
			break;
		case Move::SWITCHEROO:
		case Move::TRICK:
			if (!user.pokemon().item.blocks_trick () and !target.pokemon().item.blocks_trick ())
				std::swap (user.pokemon().item, target.pokemon().item);
			break;
		case Move::SWORDS_DANCE:
			Stat::boost (user.stage [Stat::ATK], 2);
			break;
		case Move::TAILWIND:
			if (user.tailwind == 0)
				user.tailwind = 3;
			break;
		case Move::TAUNT:
			if (target.taunt == 0)
				target.taunt = user.pokemon().move().variable().first;
			break;
		case Move::THUNDER_FANG:
			if (user.pokemon().move().variable().first != 0) {
				if (user.pokemon().move().variable().first != 2)
					Status::paralyze (user.pokemon(), target.pokemon(), weather);
				if (user.pokemon().move().variable().first != 1)
					target.flinch = true;
			}
			break;
		case Move::TICKLE:
			Stat::boost (target.stage [Stat::ATK], -1);
			Stat::boost (target.stage [Stat::DEF], -1);
			break;
		case Move::TORMENT:
			target.torment = true;
			break;
		case Move::TOXIC_SPIKES:
			if (target.toxic_spikes < 2)
				++target.toxic_spikes;
			break;
		case Move::TRANSFORM:		// Fix
			break;
		case Move::TRI_ATTACK:
			switch (user.pokemon().move().variable().first) {
				case 1:
					Status::burn (user.pokemon(), target.pokemon(), weather);
					break;
				case 2:
					Status::freeze (user.pokemon(), target.pokemon(), weather);
					break;
				case 3:
					Status::paralyze (user.pokemon(), target.pokemon(), weather);
					break;
				default:
					assert (false);
					break;
			}
			break;
		case Move::TRICK_ROOM:
			weather.set_trick_room ();
			break;
		case Move::U_TURN:
			user.u_turning = true;
			break;
		case Move::UPROAR:
			user.uproar = user.pokemon().move().variable().first;
			weather.set_uproar (static_cast<int8_t> (user.uproar));
			break;
		case Move::WAKE_UP_SLAP:
			if (target.pokemon().status.is_sleeping ())
				target.pokemon().status.clear ();
			break;
		case Move::WATER_SPORT:
			user.water_sport = true;
			break;
		case Move::WISH:
			if (user.wish == 0)
				user.wish = 2;
			break;
		case Move::WORRY_SEED:		// Fix
			break;
		case Move::YAWN:
			if (target.yawn == 0)
				target.yawn = 2;
		default:
			break;
	}
}

void lower_pp (Team & user, Pokemon const & target) {
	if (!user.pokemon().move().is_struggle_or_switch() and user.bide == 0) {
		if (target.ability.name == Ability::PRESSURE and 2 <= user.pokemon().move().pp)
			user.pokemon().move().pp -= 2;
		else
			--user.pokemon().move().pp;
	}
}

void call_other_move (Team & user) {
	user.pokemon().move.index = user.called_move;
}

}	// unnamed namespace
}	// namespace technicalmachine
