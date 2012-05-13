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

#include "ability.hpp"
#include "block.hpp"
#include "damage.hpp"
#include "heal.hpp"
#include "move.hpp"
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

class ResetIndex {
	public:
		ResetIndex (Pokemon & pokemon):
			reset (pokemon),
			index (pokemon.move.index()) {
		}
		~ResetIndex () {
			reset.move.set_index(index);
		}
		ResetIndex (ResetIndex const & r) = delete;
		ResetIndex & operator= (ResetIndex const & r) = delete;
	private:
		Pokemon & reset;
		uint8_t const index;
};

}	// unnamed namespace

unsigned call_move (Team & user, Team & target, Weather & weather, unsigned const log_damage) {
	user.destiny_bond = false;
	user.lock_on = false;
	user.moved = true;
	if (can_execute_move (user, target, weather)) {
		lower_pp (user, target.pokemon());
//		ResetIndex const reset_index (user.pokemon());
		if (user.pokemon().move().calls_other_move())
			call_other_move (user);
		if (!user.miss)
			return use_move (user, target, weather, log_damage);
	}
	// There seems to be some sort of bug related to moves that do damage not
	// hitting the target that causes some old damage amount to remain. Should
	// look into this later.
	// assert (log_damage == 0 or log_damage == -1u);
	return 0;
}

namespace {

unsigned use_move (Team & user, Team & target, Weather & weather, unsigned const log_damage) {
	Move & move = user.pokemon().move();
	// TODO: Add targeting information and only block the move if the target is
	// immune.
	if (target.pokemon().ability.blocks_sound_moves() and move.is_sound_based() and
			!(move.name == Move::HEAL_BELL or move.name == Move::PERISH_SONG))
		return 0;
	Stat::calculate_speed (user, weather);
	Stat::calculate_speed (target, weather);
	unsigned damage = 0;
	
	if (move.is_damaging()) {
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
	++move.times_used;

	do_side_effects (user, target, weather, damage);

	return damage;
}

void do_effects_before_moving (Pokemon & user, Team & target) {
	if (user.move().breaks_screens()) {
		target.reflect.clear();
		target.light_screen.clear();
	}
	else if (user.move().is_usable_while_frozen()) {
		if (user.status.is_frozen())
			user.status.clear();
	}
}

void do_damage (Team & user, Team & target, unsigned damage) {
	damage_side_effect (target.pokemon(), damage);
	if (user.pokemon().item.causes_recoil())
		heal (user.pokemon(), -10);
	if (target.pokemon().hp.stat > 0) {
		if (target.bide)
			target.bide_damage += damage;
	}
}

void do_side_effects (Team & user, Team & target, Weather & weather, unsigned damage) {
	Move & move = user.pokemon().move();
	switch (move.name) {
		case Move::DREAM_EATER:
			if (!target.pokemon().status.is_sleeping ())
				break;
		case Move::ABSORB:
		case Move::DRAIN_PUNCH:
		case Move::GIGA_DRAIN:
		case Move::LEECH_LIFE:
		case Move::MEGA_DRAIN:
			if (target.pokemon().ability.damages_leechers()) {
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
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPD, -1);
			break;
		case Move::ACID_ARMOR:
		case Move::BARRIER:
		case Move::IRON_DEFENSE:
			user.stage.boost(Stat::DEF, 2);
			break;
		case Move::ACUPRESSURE:
			// TODO: this doesn't properly account for stats maxing out
			user.stage.boost(static_cast<Stat::Stats>(move.variable().value()), 2);
			break;
		case Move::AGILITY:
		case Move::ROCK_POLISH:
			user.stage.boost(Stat::SPE, 2);
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
			if (move.variable().effect_activates())
				target.flinch = true;
			break;
		case Move::AMNESIA:
			user.stage.boost(Stat::SPD, 2);
			break;
		case Move::ANCIENTPOWER:
		case Move::OMINOUS_WIND:
		case Move::SILVER_WIND:
			if (move.variable().effect_activates())
				user.stage.boost_regular(1);
			break;
		case Move::AQUA_RING:
			user.aqua_ring = true;
			break;
		case Move::AURORA_BEAM:
			if (!move.variable().effect_activates())
				break;
		case Move::GROWL:
			target.stage.boost(Stat::ATK, -1);
			break;
		case Move::AROMATHERAPY: {
			user.pokemon.for_each ([](Pokemon & pokemon) {
				pokemon.status.clear ();
			});
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
				user.stage.maximize_attack();
			}
			break;
		case Move::BIDE:
			if (!user.bide) {
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
			if (!target.partial_trap)
				target.partial_trap = move.variable().value();
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
			if (!move.variable().effect_activates())
				break;
		case Move::WILL_O_WISP:
			Status::burn (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::BLIZZARD:
		case Move::ICE_BEAM:
		case Move::ICE_PUNCH:
		case Move::POWDER_SNOW:
			if (move.variable().effect_activates())
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
			if (!move.variable().effect_activates())
				break;
		case Move::GLARE:
		case Move::STUN_SPORE:
		case Move::THUNDER_WAVE:
		case Move::ZAP_CANNON:
			Status::paralyze (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::BOUNCE:
			user.vanish = (user.vanish == LANDED) ? BOUNCED : LANDED;
			break;
		case Move::BRAVE_BIRD:
		case Move::DOUBLE_EDGE:
		case Move::WOOD_HAMMER:
			recoil (user.pokemon(), damage, 3);
			break;
		case Move::BUBBLE:
		case Move::BUBBLEBEAM:
		case Move::CONSTRICT:
			if (!move.variable().effect_activates())
				break;
		case Move::ICY_WIND:
		case Move::MUD_SHOT:
		case Move::ROCK_TOMB:
		case Move::STRING_SHOT:
			target.stage.boost(Stat::SPE, -1);
			break;
		case Move::BUG_BITE:			// Fix
		case Move::PLUCK:
			break;
		case Move::BULK_UP:
			user.stage.boost_physical(1);
			break;
		case Move::CALM_MIND:
			user.stage.boost_special(1);
			break;
		case Move::CAMOUFLAGE:
			break;
		case Move::CAPTIVATE:
			if (user.pokemon().gender.multiplier (target.pokemon().gender) == -1)
				target.stage.boost(Stat::SPD, -2);
			break;
		case Move::CHARGE:
			user.charge = true;
			user.stage.boost(Stat::SPD, 1);
			break;
		case Move::CHARGE_BEAM:
			if (!move.variable().effect_activates())
				break;
		case Move::GROWTH:
			user.stage.boost(Stat::SPA, 1);
			break;
		case Move::CHARM:
		case Move::FEATHERDANCE:
			target.stage.boost(Stat::ATK, -2);
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
			if (!move.variable().effect_activates())
				break;
		case Move::CONFUSE_RAY:
		case Move::SUPERSONIC:
		case Move::SWEET_KISS:
		case Move::TEETER_DANCE:
			if (!target.pokemon().ability.blocks_confusion() and !target.confused)
				target.confused = move.variable().value();
			break;
		case Move::CLOSE_COMBAT:
			user.stage.boost_physical(-1);
			break;
		case Move::CONVERSION:		// Fix
			break;
		case Move::CONVERSION2:	// Fix
			break;
		case Move::COSMIC_POWER:
		case Move::DEFEND_ORDER:
			user.stage.boost_defensive(1);
			break;
		case Move::CRUNCH:
		case Move::CRUSH_CLAW:
		case Move::IRON_TAIL:
		case Move::ROCK_SMASH:
			if (!move.variable().effect_activates())
				break;
		case Move::LEER:
		case Move::TAIL_WHIP:
			target.stage.boost(Stat::DEF, -1);
			break;
		case Move::COTTON_SPORE:
		case Move::SCARY_FACE:
			target.stage.boost(Stat::SPE, -2);
			break;
		case Move::COUNTER:
			if (target.pokemon().move().is_physical())
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
			if (!move.variable().effect_activates())
				break;
		case Move::POISON_GAS:
		case Move::POISONPOWDER:
			Status::poison (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::CURSE:
			if (is_type (user, Type::GHOST) and !user.pokemon().ability.blocks_secondary_damage()) {
				if (!target.curse) {
					if (user.pokemon().hp.max <= 3)
						--user.pokemon().hp.stat;
					else
						damage_side_effect (user.pokemon(), user.pokemon().hp.max / 2);
					target.curse = true;
				}
			}
			else {
				user.stage.boost_physical(1);
				user.stage.boost(Stat::SPE, -1);
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
			user.stage.boost(Stat::DEF, 1);
			user.defense_curl = true;
			break;
		case Move::DEFOG:
			weather.fog = false;
		// Fall through
		case Move::SWEET_SCENT:
			target.stage.boost(Stat::EVA, -1);
			break;
		case Move::DESTINY_BOND:
			user.destiny_bond = true;
			break;
		case Move::DETECT:
		case Move::PROTECT:
			user.protect = true;
			break;
		case Move::DIG:
			user.vanish = (user.vanish == LANDED) ? DUG : LANDED;
			break;
		case Move::DISABLE:		// Fix
			break;
		case Move::DIVE:
			user.vanish = (user.vanish == LANDED) ? DIVED : LANDED;
			break;
		case Move::DOOM_DESIRE:	// Fix
		case Move::FUTURE_SIGHT:
			break;
		case Move::DOUBLE_TEAM:
		case Move::MINIMIZE:
			user.stage.boost(Stat::EVA, 1);
			break;
		case Move::DRACO_METEOR:
		case Move::LEAF_STORM:
		case Move::OVERHEAT:
		case Move::PSYCHO_BOOST:
			user.stage.boost(Stat::SPA, -2);
			break;
		case Move::DRAGON_DANCE:
			user.stage.boost(Stat::ATK, 1);
			user.stage.boost(Stat::SPE, 1);
			break;
		case Move::EMBARGO:
			target.embargo = 5;
			break;
		case Move::ENCORE:
			if (!target.encore)
				target.encore = move.variable().value();
			break;
		case Move::ENDURE:
			user.endure = true;
			break;
		case Move::EXPLOSION:
		case Move::SELFDESTRUCT:
			user.pokemon().hp.stat = 0;
			break;
		case Move::SEED_FLARE:
			if (!move.variable().effect_activates())
				break;
		case Move::FAKE_TEARS:
		case Move::METAL_SOUND:
			target.stage.boost(Stat::SPD, -2);
			break;
		case Move::FEINT:
			target.protect = false;
			break;
		case Move::FIRE_FANG:
			switch (move.variable().value()) {
				case 0:
					break;
				case 1:
					Status::burn (user.pokemon(), target.pokemon(), weather);
					break;
				case 2:
					target.flinch = true;
					break;
				case 3:	
					Status::burn (user.pokemon(), target.pokemon(), weather);
					target.flinch = true;
					break;
				default:
					assert (false);
					break;
			}
			break;
		case Move::MIRROR_SHOT:
		case Move::MUD_BOMB:
		case Move::MUDDY_WATER:
		case Move::OCTAZOOKA:
			if (!move.variable().effect_activates())
				break;
		case Move::FLASH:
		case Move::KINESIS:
		case Move::MUD_SLAP:
		case Move::SAND_ATTACK:
		case Move::SMOKESCREEN:
			target.stage.boost(Stat::ACC, -1);
			break;
		case Move::FLATTER:
			target.stage.boost(Stat::SPA, 1);
			if (!target.pokemon().ability.blocks_confusion() and !target.confused)
				target.confused = move.variable().value();
			break;
		case Move::FLING:
			user.pokemon().item.remove();
			break;
		case Move::FLY:
			user.vanish = (user.vanish == LANDED) ? FLOWN : LANDED;
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
			Stage::swap_defensive(user.stage, target.stage);
			break;
		case Move::HAIL:
			weather.set_hail (user.pokemon().item.extends_hail());
			break;
		case Move::HAMMER_ARM:
			user.stage.boost(Stat::SPE, -1);
			break;
		case Move::STEEL_WING:
			if (!move.variable().effect_activates())
				break;
		case Move::HARDEN:
		case Move::WITHDRAW:
			user.stage.boost(Stat::DEF, 1);
			break;
		case Move::HAZE:
			user.stage.reset();
			target.stage.reset();
			break;
		case Move::HEAD_SMASH:
			recoil (user.pokemon(), damage, 2);
			break;
		case Move::HEAL_BELL: {
			user.pokemon.for_each([](Pokemon & pokemon) {
				if (!pokemon.ability.blocks_sound_moves())
					pokemon.status.clear ();
			});
			break;
		}
		case Move::HEAL_BLOCK:
			if (!target.heal_block)
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
			using std::swap;
			swap(user.stage, target.stage);
			break;
		case Move::HI_JUMP_KICK:		// Fix
		case Move::JUMP_KICK:
			break;
		case Move::HOWL:
		case Move::MEDITATE:
		case Move::METEOR_MASH:
			if (!move.variable().effect_activates())
				break;
		case Move::SHARPEN:
			user.stage.boost(Stat::ATK, 1);
			break;
		case Move::ICE_BALL:		// Fix
		case Move::ROLLOUT:
			break;
		case Move::ICE_FANG:
			switch (move.variable().value()) {
				case 0:
					break;
				case 1:
					Status::freeze (user.pokemon(), target.pokemon(), weather);
					break;
				case 2:
					target.flinch = true;
					break;
				case 3:	
					Status::freeze (user.pokemon(), target.pokemon(), weather);
					target.flinch = true;
					break;
				default:
					assert (false);
					break;
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
			user.light_screen.activate(user.pokemon().item.extends_light_screen());
			break;
		case Move::LOCK_ON:
		case Move::MIND_READER:
			user.lock_on = true;
			break;
		case Move::LUCKY_CHANT:
			user.lucky_chant.activate();
			break;
		case Move::LUNAR_DANCE:		// Fix
			break;
		case Move::MAGIC_COAT:		// Fix
			break;
		case Move::MAGNET_RISE:
			if (!user.magnet_rise)
				user.magnet_rise = 5;
			break;
		case Move::ME_FIRST:		// Fix
			break;
		case Move::MEMENTO:
			target.stage.boost_offensive(-2);
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
			if (target.pokemon().move().is_special())
				damage_side_effect (target.pokemon(), user.damage * 2u);
			break;
		case Move::MIST:
			user.mist.activate();
			break;
		case Move::MIST_BALL:
			if (move.variable().effect_activates())
				target.stage.boost(Stat::SPA, -1);
			break;
		case Move::MOONLIGHT:
		case Move::MORNING_SUN:
		case Move::SYNTHESIS:
			if (weather.sun())
				heal (user.pokemon(), 3, 2);
			else if (weather.hail() or weather.rain() or weather.sand())
				heal (user.pokemon(), 4);
			else
				heal (user.pokemon(), 2);
			break;
		case Move::MUD_SPORT:
			user.mud_sport = true;
			break;
		case Move::NASTY_PLOT:
		case Move::TAIL_GLOW:
			user.stage.boost(Stat::SPA, 2);
			break;
		case Move::NIGHTMARE:
			target.nightmare = true;
			break;
		case Move::OUTRAGE:
		case Move::PETAL_DANCE:
		case Move::THRASH:
			if (!user.rampage)
				user.rampage = move.variable().value();
			break;
		case Move::PAIN_SPLIT: {
			auto & user_hp = user.pokemon().hp;
			auto & target_hp = target.pokemon().hp;
			user_hp.stat = (user_hp.stat + target_hp.stat) / 2;
			target_hp.stat = std::min (user_hp.stat, target_hp.max);
			user_hp.stat = std::min (user_hp.stat, user_hp.max);
			break;
		}
		case Move::PERISH_SONG:
			if (!user.perish_song)
				user.perish_song = 3;
			if (!target.perish_song)
				target.perish_song = 3;
			break;
		case Move::POISON_FANG:
			if (!move.variable().effect_activates())
				break;
		case Move::TOXIC:
			Status::poison_toxic (user.pokemon(), target.pokemon(), weather);
			break;
		case Move::POWER_SWAP:
			Stage::swap_offensive(user.stage, target.stage);
			break;
		case Move::POWER_TRICK:
			user.power_trick = !user.power_trick;
			break;
		case Move::PRESENT:
			if (move.variable().present_heals()) {
				target.pokemon().hp.stat += 80;
				target.pokemon().hp.stat = std::min (target.pokemon().hp.stat, target.pokemon().hp.max);
			}
			break;
		case Move::PSYCH_UP:
			user.stage = target.stage;
			break;
		case Move::PSYCHO_SHIFT:
			if (target.pokemon().status.is_clear())
				Status::shift(user.pokemon(), target.pokemon(), weather);
			break;
		case Move::RAGE:		// Fix
			break;
		case Move::RAIN_DANCE:
			weather.set_rain (user.pokemon().item.extends_rain());
			break;
		case Move::RAPID_SPIN:
			if (move.type().get_effectiveness(target.pokemon()) > 0) {
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
			user.reflect.activate(user.pokemon().item.extends_reflect());
			break;
		case Move::REFRESH:
			user.pokemon().status.clear ();
			break;
		case Move::REST:
			if (user.pokemon().hp.stat != user.pokemon().hp.max) {
				user.pokemon().hp.stat = user.pokemon().hp.max;
				user.pokemon().status.rest();
			}
			break;
		case Move::ROAR:
		case Move::WHIRLWIND:
			if (target.can_be_phazed()) {
				uint8_t const index = move.variable.phaze_index(target.pokemon.index());
				target.pokemon.set_replacement(index);
				switchpokemon (target, user, weather);
				target.moved = true;
			}
			break;
		case Move::ROLE_PLAY:		// Fix
			break;
		case Move::SAFEGUARD:
			user.safeguard.activate();
			break;
		case Move::SANDSTORM:
			weather.set_sand (user.pokemon().item.extends_sand());
			break;
		case Move::SCREECH:
			target.stage.boost(Stat::DEF, -2);
			break;
		case Move::SHADOW_FORCE:
			user.vanish = (user.vanish == LANDED) ? SHADOW_FORCED : LANDED;
			break;
		case Move::SKETCH:		// Fix
			break;
		case Move::SKILL_SWAP:		// Fix
			break;
		case Move::SKULL_BASH: // Fix
			break;
		case Move::SKY_ATTACK:	// Fix
			if (move.variable().effect_activates()) {}
			break;
		case Move::SMELLINGSALT:
			if (target.pokemon().status.boosts_smellingsalt())
				target.pokemon().status.clear();
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
			weather.set_sun (user.pokemon().item.extends_sun());
			break;
		case Move::SUPERPOWER:
			user.stage.boost_physical(-1);
			break;
		case Move::SWAGGER:
			target.stage.boost(Stat::ATK, 2);
			if (target.pokemon().ability.name != Ability::OWN_TEMPO and !target.confused)
				target.confused = move.variable().value();
		case Move::SWALLOW:		// Fix
			break;
		case Move::SWITCH0:
		case Move::SWITCH1:
		case Move::SWITCH2:
		case Move::SWITCH3:
		case Move::SWITCH4:
		case Move::SWITCH5:
			user.pokemon.replacement_from_switch();
			switchpokemon (user, target, weather);
			break;
		case Move::SWITCHEROO:
		case Move::TRICK:
			if (!user.pokemon().item.blocks_trick () and !target.pokemon().item.blocks_trick()) {
				using std::swap;
				swap (user.pokemon().item, target.pokemon().item);
			}
			break;
		case Move::SWORDS_DANCE:
			user.stage.boost(Stat::ATK, 2);
			break;
		case Move::TAILWIND:
			user.tailwind.activate();
			break;
		case Move::TAUNT:
			if (!target.taunt)
				target.taunt = move.variable().value();
			break;
		case Move::THUNDER_FANG:
			switch (move.variable().value()) {
				case 0:
					break;
				case 1:
					Status::paralyze (user.pokemon(), target.pokemon(), weather);
					break;
				case 2:
					target.flinch = true;
					break;
				case 3:	
					Status::paralyze (user.pokemon(), target.pokemon(), weather);
					target.flinch = true;
					break;
				default:
					assert (false);
					break;
			}
			break;
		case Move::TICKLE:
			target.stage.boost_physical(-1);
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
			switch (move.variable().value()) {
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
			weather.set_uproar (static_cast<int8_t> (move.variable().value()));
			user.uproar = move.variable().value();
			break;
		case Move::WAKE_UP_SLAP:
			if (target.pokemon().status.is_sleeping ())
				target.pokemon().status.clear ();
			break;
		case Move::WATER_SPORT:
			user.water_sport = true;
			break;
		case Move::WISH:
			if (!user.wish)
				user.wish = 2;
			break;
		case Move::WORRY_SEED:		// Fix
			break;
		case Move::YAWN:
			if (!target.yawn)
				target.yawn = 2;
		default:
			break;
	}
}

void lower_pp (Team & user, Pokemon const & target) {
	Move & move = user.pokemon().move();
	if (!move.is_struggle_or_switch() and !user.bide) {
		if (target.ability.name == Ability::PRESSURE and 2 <= move.pp)
			move.pp -= 2;
		else
			--move.pp;
	}
}

void call_other_move (Team & user) {
	user.pokemon().move.set_index(user.called_move);
}

}	// unnamed namespace
}	// namespace technicalmachine
