// class Move
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

#include "move.hpp"

#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>
#undef SING

#include "evaluate.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

constexpr uint8_t unlimited_pp = 0xFF;
constexpr uint16_t indeterminate_power = 0xFFFF;
constexpr uint16_t perfect_accuracy = 0xFFFF;
Type get_type (Move::Moves move);
bool is_physical (Move::Moves move);
uint16_t base_power (Move::Moves move);
uint8_t get_pp (Move::Moves move);
uint16_t get_probability (Move::Moves move);
uint16_t get_accuracy (Move::Moves move);

}	// anonymous namespace

Move::Move (Moves const move, int const pp_ups, unsigned const size) :
	score (-Score::VICTORY - 1),
	variable (get_variable (move, size)),
	name (move),
	basepower (base_power (move)),
	type (get_type (move)),
	physical (is_physical (move)),
	accuracy (get_accuracy (move)),
	disable (0),
	pp_max (get_pp (move) * (5 + pp_ups) / 5),
	pp (pp_max),
	priority (get_priority ()),
	r (100),
	times_used (0) {
}

void Move::reset () {
	disable = 0;
	times_used = 0;
}

uint64_t Move::hash () const {
	return static_cast<uint64_t> (
			name + END *
			(disable + 7 *
			(pp + pp_max *
			times_used)));
}

int8_t Move::get_priority () {
	switch (name) {
		case SWITCH0:
		case SWITCH1:
		case SWITCH2:
		case SWITCH3:
		case SWITCH4:
		case SWITCH5:
			return 6;
		case HELPING_HAND:
			return 5;
		case MAGIC_COAT:
		case SNATCH:
			return 4;
		case DETECT:
		case ENDURE:
		case FOLLOW_ME:
		case PROTECT:
			return 3;
		case FEINT:
			return 2;
		case AQUA_JET:
		case BIDE:
		case BULLET_PUNCH:
		case EXTREMESPEED:
		case FAKE_OUT:
		case ICE_SHARD:
		case MACH_PUNCH:
		case QUICK_ATTACK:
		case SHADOW_SNEAK:
		case SUCKER_PUNCH:
		case VACUUM_WAVE:
			return 1;
		case VITAL_THROW:
			return -1;
		case FOCUS_PUNCH:
			return -2;
		case AVALANCHE:
		case REVENGE:
			return -3;
		case COUNTER:
		case MIRROR_COAT:
			return -4;
		case ROAR:
		case WHIRLWIND:
			return -5;
		case TRICK_ROOM:
			return -6;
		default:
			return 0;
	}
}

std::vector<std::pair<uint16_t, uint16_t>> Move::get_variable (Moves name, unsigned size) {
	std::vector<std::pair<uint16_t, uint16_t>> set;
	std::pair<uint16_t, uint16_t> effect;
	effect.first = 0;
	unsigned last = 0;
	bool simple_range = true;
	effect.second = get_probability (name);
	switch (name) {
		case ACUPRESSURE:
			last = 6;
			break;
		case BIND:
		case CLAMP:
		case FIRE_SPIN:
		case MAGMA_STORM:
		case SAND_TOMB:
		case WHIRLPOOL:
		case WRAP:
			simple_range = false;
			for (effect.first = 2; effect.first <= 5; ++effect.first) {
				if (effect.first == 4)
					effect.second /= 3;
				set.push_back (effect);
			}
			break;
		case ENCORE:
			effect.first = 4;
			last = 8;
			break;
		case FIRE_FANG:
		case ICE_FANG:
		case THUNDER_FANG:
			simple_range = false;
			effect.second = max_probability - 2u * max_probability / 10u + max_probability / 100u;
			set.push_back (effect);
			effect.first = 1;
			effect.second = max_probability / 10;
			set.push_back (effect);
			effect.first = 2;
			set.push_back (effect);
			effect.first = 3;
			effect.second = effect.second / 10;
			set.push_back (effect);
			break;
		case MAGNITUDE:
			simple_range = false;
			effect.first = 10;
			effect.second = max_probability * 5u / 100u;
			set.push_back (effect);
			effect.first = 30;
			effect.second = max_probability * 10u / 100u;
			set.push_back (effect);
			effect.first = 50;
			effect.second = max_probability * 20u / 100u;
			set.push_back (effect);
			effect.first = 70;
			effect.second = max_probability * 30u / 100u;
			set.push_back (effect);
			effect.first = 90;
			effect.second = max_probability * 20u / 100u;
			set.push_back (effect);
			effect.first = 110;
			effect.second = max_probability * 10u / 100u;
			set.push_back (effect);
			effect.first = 150;
			effect.second = max_probability * 5u / 100u;
			set.push_back (effect);
			break;
		case OUTRAGE:
		case PETAL_DANCE:
		case THRASH:
			effect.first = 2;
			last = 3;
			break;
		case PRESENT:
			simple_range = false;
			while (effect.first <= 120) {
				set.push_back (effect);
				effect.first += 40;
			}
			break;
		case PSYWAVE:
			effect.first = 5;
			last = 15;
			break;
		case ROAR:
		case WHIRLWIND:
			effect.second = (size > 2) ? max_probability / (size - 1) : max_probability;
			last = size - 2;
			break;
		case TAUNT:
			effect.first = 2;
			last = 3;
			break;
		case TRI_ATTACK:
			last = 3;
			break;
		default:
			simple_range = false;
			while (effect.first <= 1) {
				effect.second = max_probability - effect.second;
				if (effect.second != 0)
					set.push_back (effect);
				++effect.first;
			}
			break;
	}
	if (simple_range) {
		while (effect.first <= last) {
			set.push_back (effect);
			++effect.first;
		}
	}
	return set;
}

bool Move::operator== (Move const & other) const {
	return name == other.name and
			disable == other.disable and
			pp == other.pp and
			times_used == other.times_used;
}

bool Move::operator!= (Move const & other) const {
	return !(*this == other);
}

bool Move::is_switch (Moves name) {
	return SWITCH0 <= name and name <= SWITCH5;
}

bool Move::is_switch () const {
	return is_switch (name);
}

Move::Moves Move::from_replacement (unsigned replacement) {
	return static_cast <Moves> (replacement + SWITCH0);
}

unsigned Move::to_replacement (Moves name) {
	assert (SWITCH0 <= name and name <= SWITCH5);
	return static_cast<unsigned> (name - SWITCH0);
}

unsigned Move::to_replacement () const {
	return to_replacement (name);
}

bool Move::affects_target (Team const & target, Weather const & weather) const {
	return affects_pokemon (target, target.pokemon(), weather);
}

bool Move::affects_replacement (Team const & target, Weather const & weather) const {
	return affects_pokemon (target, target.pokemon.at_replacement(), weather);
}

bool Move::affects_pokemon (Team const & target, Pokemon const & pokemon, Weather const & weather) const {
	return type.get_effectiveness(pokemon) > 0 and (type != Type::GROUND or grounded (target, pokemon, weather));
}

bool Move::has_follow_up_decision () const {
	switch (name) {
		case BATON_PASS:
		case U_TURN:
			return true;
		default:
			return false;
	}
}

bool Move::calls_other_move () const {
	switch (name) {
//		case Move::NATURE_POWER:
		case Move::ASSIST:
		case Move::COPYCAT:
		case Move::ME_FIRST:
		case Move::METRONOME:
		case Move::MIRROR_MOVE:
		case Move::SLEEP_TALK:
			return true;
		default:
			return false;
	}
}

bool Move::is_out_of_pp () const {
	return pp == 0;
}

bool Move::cannot_ko () const {
	return name == FALSE_SWIPE;
}

bool Move::breaks_screens () const {
	return name == BRICK_BREAK;
}

bool Move::was_used_last () const {
	return times_used != 0;
}

bool Move::is_struggle_or_switch () const {
	return pp_max == unlimited_pp;
}

bool Move::is_phaze (Moves name) {
	switch (name) {
		case ROAR:
		case WHIRLWIND:
			return true;
		default:
			return false;
	}
}

bool Move::is_phaze () const {
	return is_phaze (name);
}

bool Move::is_healing (Moves name) {
	switch (name) {
		case HEAL_ORDER:
		case MILK_DRINK:
		case MOONLIGHT:
		case MORNING_SUN:
		case RECOVER:
		case REST:
		case ROOST:
		case SLACK_OFF:
		case SOFTBOILED:
		case SWALLOW:
		case SYNTHESIS:
		case WISH:
			return true;
		default:
			return false;
	}
}

bool Move::is_healing () const {
	return is_healing (name);
}

bool Move::is_blocked_by_gravity () const {
	switch (name) {
		case BOUNCE:
		case FLY:
		case HI_JUMP_KICK:
		case JUMP_KICK:
		case MAGNET_RISE:
		case SPLASH:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_iron_fist () const {
	switch (name) {
		case BULLET_PUNCH:
		case COMET_PUNCH:
		case DIZZY_PUNCH:
		case DRAIN_PUNCH:
		case DYNAMICPUNCH:
		case FIRE_PUNCH:
		case FOCUS_PUNCH:
		case HAMMER_ARM:
		case ICE_PUNCH:
		case MACH_PUNCH:
		case MEGA_PUNCH:
		case METEOR_MASH:
		case SHADOW_PUNCH:
		case SKY_UPPERCUT:
		case THUNDERPUNCH:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_reckless() const {
	switch (name) {
		case BRAVE_BIRD:
		case DOUBLE_EDGE:
		case FLARE_BLITZ:
		case HEAD_SMASH:
		case SUBMISSION:
		case TAKE_DOWN:
		case VOLT_TACKLE:
		case WOOD_HAMMER:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_sleeping () const {
	switch (name) {
		case SLEEP_TALK:
		case SNORE:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_frozen () const {
	switch (name) {
		case FLAME_WHEEL:
		case SACRED_FIRE:
			return true;
		default:
			return false;
	}
}

bool Move::is_sound_based () const {
	switch (name) {
		case BUG_BUZZ:
		case CHATTER:
		case GRASSWHISTLE:
		case GROWL:
		case HEAL_BELL:
		case HYPER_VOICE:
		case METAL_SOUND:
		case PERISH_SONG:
		case ROAR:
		case SCREECH:
		case SING:
		case SNORE:
		case SUPERSONIC:
		case UPROAR:
			return true;
		default:
			return false;
	}
}

bool Move::is_self_KO () const {
	switch (name) {
		case EXPLOSION:
		case SELFDESTRUCT:
			return true;
		default:
			return false;
	}
}

bool Move::cannot_miss () const {
	return get_accuracy (name) == perfect_accuracy;
}

uint16_t const Move::max_probability = 840;

namespace {

Type get_type (Move::Moves move) {
	static constexpr Type::Types move_type [] = {
		Type::GRASS,		// ABSORB
		Type::POISON,		// ACID
		Type::POISON,		// ACID_ARMOR
		Type::NORMAL,		// ACUPRESSURE
		Type::FLYING,		// AERIAL_ACE
		Type::FLYING,		// AEROBLAST
		Type::PSYCHIC,	// AGILITY
		Type::FLYING,		// AIR_CUTTER
		Type::FLYING,		// AIR_SLASH
		Type::PSYCHIC,	// AMNESIA
		Type::ROCK,		// ANCIENTPOWER
		Type::WATER,		// AQUA_JET
		Type::WATER,		// AQUA_RING
		Type::WATER,		// AQUA_TAIL
		Type::FIGHTING,	// ARM_THRUST
		Type::GRASS,		// AROMATHERAPY
		Type::NORMAL,		// ASSIST
		Type::DARK,		// ASSURANCE
		Type::GHOST,		// ASTONISH
		Type::BUG,		// ATTACK_ORDER
		Type::NORMAL,		// ATTRACT
		Type::FIGHTING,	// AURA_SPHERE
		Type::ICE,		// AURORA_BEAM
		Type::ICE,		// AVALANCHE
		Type::NORMAL,		// BARRAGE
		Type::PSYCHIC,	// BARRIER
		Type::NORMAL,		// BATON_PASS
		Type::DARK,		// BEAT_UP
		Type::NORMAL,		// BELLY_DRUM
		Type::NORMAL,		// BIDE
		Type::NORMAL,		// BIND
		Type::DARK,		// BITE
		Type::FIRE,		// BLAST_BURN
		Type::FIRE,		// BLAZE_KICK
		Type::ICE,		// BLIZZARD
		Type::NORMAL,		// BLOCK
		Type::NORMAL,		// BODY_SLAM
		Type::GROUND,		// BONE_CLUB
		Type::GROUND,		// BONE_RUSH
		Type::GROUND,		// BONEMERANG
		Type::FLYING,		// BOUNCE
		Type::FLYING,		// BRAVE_BIRD
		Type::FIGHTING,	// BRICK_BREAK
		Type::WATER,		// BRINE
		Type::WATER,		// BUBBLE
		Type::WATER,		// BUBBLEBEAM
		Type::BUG,		// BUG_BITE
		Type::BUG,		// BUG_BUZZ
		Type::FIGHTING,	// BULK_UP
		Type::STEEL,		// BULLET_PUNCH
		Type::GRASS,		// BULLET_SEED
		Type::PSYCHIC,	// CALM_MIND
		Type::NORMAL,		// CAMOUFLAGE
		Type::NORMAL,		// CAPTIVATE
		Type::ELECTRIC,	// CHARGE
		Type::ELECTRIC,	// CHARGE_BEAM
		Type::NORMAL,		// CHARM
		Type::FLYING,		// CHATTER
		Type::WATER,		// CLAMP
		Type::FIGHTING,	// CLOSE_COMBAT
		Type::NORMAL,		// COMET_PUNCH
		Type::GHOST,		// CONFUSE_RAY
		Type::PSYCHIC,	// CONFUSION
		Type::NORMAL,		// CONSTRICT
		Type::NORMAL,		// CONVERSION
		Type::NORMAL,		// CONVERSION_2
		Type::NORMAL,		// COPYCAT
		Type::PSYCHIC,	// COSMIC_POWER
		Type::GRASS,		// COTTON_SPORE
		Type::FIGHTING,	// COUNTER
		Type::NORMAL,		// COVET
		Type::WATER,		// CRABHAMMER
		Type::FIGHTING,	// CROSS_CHOP
		Type::POISON,		// CROSS_POISON
		Type::DARK,		// CRUNCH
		Type::NORMAL,		// CRUSH_CLAW
		Type::NORMAL,		// CRUSH_GRIP
		Type::TYPELESS,	// CURSE
		Type::NORMAL,		// CUT
		Type::DARK,		// DARK_PULSE
		Type::DARK,		// DARK_VOID
		Type::BUG,		// DEFEND_ORDER
		Type::NORMAL,		// DEFENSE_CURL
		Type::FLYING,		// DEFOG
		Type::GHOST,		// DESTINY_BOND
		Type::FIGHTING,	// DETECT
		Type::GROUND,		// DIG
		Type::NORMAL,		// DISABLE
		Type::ELECTRIC,	// DISCHARGE
		Type::WATER,		// DIVE
		Type::NORMAL,		// DIZZY_PUNCH
		Type::STEEL,		// DOOM_DESIRE
		Type::NORMAL,		// DOUBLE_HIT
		Type::FIGHTING,	// DOUBLE_KICK
		Type::NORMAL,		// DOUBLE_TEAM
		Type::NORMAL,		// DOUBLE_EDGE
		Type::NORMAL,		// DOUBLESLAP
		Type::DRAGON,		// DRACO_METEOR
		Type::DRAGON,		// DRAGON_CLAW
		Type::DRAGON,		// DRAGON_DANCE
		Type::DRAGON,		// DRAGON_PULSE
		Type::DRAGON,		// DRAGON_RAGE
		Type::DRAGON,		// DRAGON_RUSH
		Type::DRAGON,		// DRAGONBREATH
		Type::FIGHTING,	// DRAIN_PUNCH
		Type::PSYCHIC,	// DREAM_EATER
		Type::FLYING,		// DRILL_PECK
		Type::FIGHTING,	// DYNAMICPUNCH
		Type::GROUND,		// EARTH_POWER
		Type::GROUND,		// EARTHQUAKE
		Type::NORMAL,		// EGG_BOMB
		Type::DARK,		// EMBARGO
		Type::FIRE,		// EMBER
		Type::NORMAL,		// ENCORE
		Type::NORMAL,		// ENDEAVOR
		Type::NORMAL,		// ENDURE
		Type::GRASS,		// ENERGY_BALL
		Type::FIRE,		// ERUPTION
		Type::NORMAL,		// EXPLOSION
		Type::PSYCHIC,	// EXTRASENSORY
		Type::NORMAL,		// EXTREMESPEED
		Type::NORMAL,		// FACADE
		Type::DARK,		// FAINT_ATTACK
		Type::NORMAL,		// FAKE_OUT
		Type::DARK,		// FAKE_TEARS
		Type::NORMAL,		// FALSE_SWIPE
		Type::FLYING,		// FEATHERDANCE
		Type::NORMAL,		// FEINT
		Type::FIRE,		// FIRE_BLAST
		Type::FIRE,		// FIRE_FANG
		Type::FIRE,		// FIRE_PUNCH
		Type::FIRE,		// FIRE_SPIN
		Type::GROUND,		// FISSURE
		Type::NORMAL,		// FLAIL
		Type::FIRE,		// FLAME_WHEEL
		Type::FIRE,		// FLAMETHROWER
		Type::FIRE,		// FLARE_BLITZ
		Type::NORMAL,		// FLASH
		Type::STEEL,		// FLASH_CANNON
		Type::DARK,		// FLATTER
		Type::DARK,		// FLING
		Type::FLYING,		// FLY
		Type::FIGHTING,	// FOCUS_BLAST
		Type::NORMAL,		// FOCUS_ENERGY
		Type::FIGHTING,	// FOCUS_PUNCH
		Type::NORMAL,		// FOLLOW_ME
		Type::FIGHTING,	// FORCE_PALM
		Type::NORMAL,		// FORESIGHT
		Type::GRASS,		// FRENZY_PLANT
		Type::NORMAL,		// FRUSTRATION
		Type::NORMAL,		// FURY_ATTACK
		Type::BUG,		// FURY_CUTTER
		Type::NORMAL,		// FURY_SWIPES
		Type::PSYCHIC,	// FUTURE_SIGHT
		Type::POISON,		// GASTRO_ACID
		Type::GRASS,		// GIGA_DRAIN
		Type::NORMAL,		// GIGA_IMPACT
		Type::NORMAL,		// GLARE
		Type::GRASS,		// GRASS_KNOT
		Type::GRASS,		// GRASSWHISTLE
		Type::PSYCHIC,	// GRAVITY
		Type::NORMAL,		// GROWL
		Type::NORMAL,		// GROWTH
		Type::GHOST,		// GRUDGE
		Type::PSYCHIC,	// GUARD_SWAP
		Type::NORMAL,		// GUILLOTINE
		Type::POISON,		// GUNK_SHOT
		Type::FLYING,		// GUST
		Type::STEEL,		// GYRO_BALL
		Type::ICE,		// HAIL
		Type::FIGHTING,	// HAMMER_ARM
		Type::NORMAL,		// HARDEN
		Type::ICE,		// HAZE
		Type::ROCK,		// HEAD_SMASH
		Type::NORMAL,		// HEADBUTT
		Type::NORMAL,		// HEAL_BELL
		Type::PSYCHIC,	// HEAL_BLOCK
		Type::BUG,		// HEAL_ORDER
		Type::PSYCHIC,	// HEALING_WISH
		Type::PSYCHIC,	// HEART_SWAP
		Type::FIRE,		// HEAT_WAVE
		Type::NORMAL,		// HELPING_HAND
		Type::FIGHTING,	// HI_JUMP_KICK
		Type::NORMAL,		// HIDDEN_POWER
		Type::NORMAL,		// HORN_ATTACK
		Type::NORMAL,		// HORN_DRILL
		Type::NORMAL,		// HOWL
		Type::WATER,		// HYDRO_CANNON
		Type::WATER,		// HYDRO_PUMP
		Type::NORMAL,		// HYPER_BEAM
		Type::NORMAL,		// HYPER_FANG
		Type::NORMAL,		// HYPER_VOICE
		Type::PSYCHIC,	// HYPNOSIS
		Type::ICE,		// ICE_BALL
		Type::ICE,		// ICE_BEAM
		Type::ICE,		// ICE_FANG
		Type::ICE,		// ICE_PUNCH
		Type::ICE,		// ICE_SHARD
		Type::ICE,		// ICICLE_SPEAR
		Type::ICE,		// ICY_WIND
		Type::PSYCHIC,	// IMPRISON
		Type::GRASS,		// INGRAIN
		Type::STEEL,		// IRON_DEFENSE
		Type::STEEL,		// IRON_HEAD
		Type::STEEL,		// IRON_TAIL
		Type::NORMAL,		// JUDGMENT
		Type::FIGHTING,	// JUMP_KICK
		Type::FIGHTING,	// KARATE_CHOP
		Type::PSYCHIC,	// KINESIS
		Type::DARK,		// KNOCK_OFF
		Type::NORMAL,		// LAST_RESORT
		Type::FIRE,		// LAVA_PLUME
		Type::GRASS,		// LEAF_BLADE
		Type::GRASS,		// LEAF_STORM
		Type::BUG,		// LEECH_LIFE
		Type::GRASS,		// LEECH_SEED
		Type::NORMAL,		// LEER
		Type::GHOST,		// LICK
		Type::PSYCHIC,	// LIGHT_SCREEN
		Type::NORMAL,		// LOCK_ON
		Type::NORMAL,		// LOVELY_KISS
		Type::FIGHTING,	// LOW_KICK
		Type::NORMAL,		// LUCKY_CHANT
		Type::PSYCHIC,	// LUNAR_DANCE
		Type::PSYCHIC,	// LUSTER_PURGE
		Type::FIGHTING,	// MACH_PUNCH
		Type::PSYCHIC,	// MAGIC_COAT
		Type::GRASS,		// MAGICAL_LEAF
		Type::FIRE,		// MAGMA_STORM
		Type::STEEL,		// MAGNET_BOMB
		Type::ELECTRIC,	// MAGNET_RISE
		Type::GROUND,		// MAGNITUDE
		Type::NORMAL,		// ME_FIRST
		Type::NORMAL,		// MEAN_LOOK
		Type::PSYCHIC,	// MEDITATE
		Type::GRASS,		// MEGA_DRAIN
		Type::NORMAL,		// MEGA_KICK
		Type::NORMAL,		// MEGA_PUNCH
		Type::BUG,		// MEGAHORN
		Type::DARK,		// MEMENTO
		Type::STEEL,		// METAL_BURST
		Type::STEEL,		// METAL_CLAW
		Type::STEEL,		// METAL_SOUND
		Type::STEEL,		// METEOR_MASH
		Type::NORMAL,		// METRONOME_MOVE
		Type::NORMAL,		// MILK_DRINK
		Type::NORMAL,		// MIMIC
		Type::NORMAL,		// MIND_READER
		Type::NORMAL,		// MINIMIZE
		Type::PSYCHIC,	// MIRACLE_EYE
		Type::PSYCHIC,	// MIRROR_COAT
		Type::FLYING,		// MIRROR_MOVE
		Type::STEEL,		// MIRROR_SHOT
		Type::ICE,		// MIST
		Type::PSYCHIC,	// MIST_BALL
		Type::NORMAL,		// MOONLIGHT
		Type::NORMAL,		// MORNING_SUN
		Type::GROUND,		// MUD_BOMB
		Type::GROUND,		// MUD_SHOT
		Type::GROUND,		// MUD_SLAP
		Type::GROUND,		// MUD_SPORT
		Type::WATER,		// MUDDY_WATER
		Type::DARK,		// NASTY_PLOT
		Type::NORMAL,		// NATURAL_GIFT
		Type::NORMAL,		// NATURE_POWER
		Type::GRASS,		// NEEDLE_ARM
		Type::GHOST,		// NIGHT_SHADE
		Type::DARK,		// NIGHT_SLASH
		Type::GHOST,		// NIGHTMARE
		Type::WATER,		// OCTAZOOKA
		Type::NORMAL,		// ODOR_SLEUTH
		Type::GHOST,		// OMINOUS_WIND
		Type::DRAGON,		// OUTRAGE
		Type::FIRE,		// OVERHEAT
		Type::NORMAL,		// PAIN_SPLIT
		Type::NORMAL,		// PAY_DAY
		Type::DARK,		// PAYBACK
		Type::FLYING,		// PECK
		Type::NORMAL,		// PERISH_SONG
		Type::GRASS,		// PETAL_DANCE
		Type::BUG,		// PIN_MISSILE
		Type::FLYING,		// PLUCK
		Type::POISON,		// POISON_FANG
		Type::POISON,		// POISON_GAS
		Type::POISON,		// POISON_JAB
		Type::POISON,		// POISON_STING
		Type::POISON,		// POISON_TAIL
		Type::POISON,		// POISONPOWDER
		Type::NORMAL,		// POUND
		Type::ICE,		// POWDER_SNOW
		Type::ROCK,		// POWER_GEM
		Type::PSYCHIC,	// POWER_SWAP
		Type::PSYCHIC,	// POWER_TRICK
		Type::GRASS,		// POWER_WHIP
		Type::NORMAL,		// PRESENT
		Type::NORMAL,		// PROTECT
		Type::PSYCHIC,	// PSYBEAM
		Type::NORMAL,		// PSYCH_UP
		Type::PSYCHIC,	// PSYCHIC_MOVE
		Type::PSYCHIC,	// PSYCHO_BOOST
		Type::PSYCHIC,	// PSYCHO_CUT
		Type::PSYCHIC,	// PSYCHO_SHIFT
		Type::PSYCHIC,	// PSYWAVE
		Type::DARK,		// PUNISHMENT
		Type::DARK,		// PURSUIT
		Type::NORMAL,		// QUICK_ATTACK
		Type::NORMAL,		// RAGE
		Type::WATER,		// RAIN_DANCE
		Type::NORMAL,		// RAPID_SPIN
		Type::GRASS,		// RAZOR_LEAF
		Type::NORMAL,		// RAZOR_WIND
		Type::NORMAL,		// RECOVER
		Type::NORMAL,		// RECYCLE
		Type::PSYCHIC,	// REFLECT
		Type::NORMAL,		// REFRESH
		Type::PSYCHIC,	// REST
		Type::NORMAL,		// RETURN
		Type::FIGHTING,	// REVENGE
		Type::FIGHTING,	// REVERSAL
		Type::NORMAL,		// ROAR
		Type::DRAGON,		// ROAR_OF_TIME
		Type::ROCK,		// ROCK_BLAST
		Type::NORMAL,		// ROCK_CLIMB
		Type::ROCK,		// ROCK_POLISH
		Type::ROCK,		// ROCK_SLIDE
		Type::FIGHTING,	// ROCK_SMASH
		Type::ROCK,		// ROCK_THROW
		Type::ROCK,		// ROCK_TOMB
		Type::ROCK,		// ROCK_WRECKER
		Type::PSYCHIC,	// ROLE_PLAY
		Type::FIGHTING,	// ROLLING_KICK
		Type::ROCK,		// ROLLOUT
		Type::FLYING,		// ROOST
		Type::FIRE,		// SACRED_FIRE
		Type::NORMAL,		// SAFEGUARD
		Type::GROUND,		// SAND_ATTACK
		Type::GROUND,		// SAND_TOMB
		Type::ROCK,		// SANDSTORM
		Type::NORMAL,		// SCARY_FACE
		Type::NORMAL,		// SCRATCH
		Type::NORMAL,		// SCREECH
		Type::NORMAL,		// SECRET_POWER
		Type::GRASS,		// SEED_BOMB
		Type::GRASS,		// SEED_FLARE
		Type::FIGHTING,	// SEISMIC_TOSS
		Type::NORMAL,		// SELFDESTRUCT
		Type::GHOST,		// SHADOW_BALL
		Type::GHOST,		// SHADOW_CLAW
		Type::GHOST,		// SHADOW_FORCE
		Type::GHOST,		// SHADOW_PUNCH
		Type::GHOST,		// SHADOW_SNEAK
		Type::NORMAL,		// SHARPEN
		Type::ICE,		// SHEER_COLD
		Type::ELECTRIC,	// SHOCK_WAVE
		Type::BUG,		// SIGNAL_BEAM
		Type::BUG,		// SILVER_WIND
		Type::NORMAL,		// SING
		Type::NORMAL,		// SKETCH
		Type::PSYCHIC,	// SKILL_SWAP
		Type::NORMAL,		// SKULL_BASH
		Type::FLYING,		// SKY_ATTACK
		Type::FIGHTING,	// SKY_UPPERCUT
		Type::NORMAL,		// SLACK_OFF
		Type::NORMAL,		// SLAM
		Type::NORMAL,		// SLASH
		Type::GRASS,		// SLEEP_POWDER
		Type::NORMAL,		// SLEEP_TALK
		Type::POISON,		// SLUDGE
		Type::POISON,		// SLUDGE_BOMB
		Type::NORMAL,		// SMELLINGSALT
		Type::POISON,		// SMOG
		Type::NORMAL,		// SMOKESCREEN
		Type::DARK,		// SNATCH
		Type::NORMAL,		// SNORE
		Type::NORMAL,		// SOFTBOILED
		Type::GRASS,		// SOLARBEAM
		Type::NORMAL,		// SONICBOOM
		Type::DRAGON,		// SPACIAL_REND
		Type::ELECTRIC,	// SPARK
		Type::BUG,		// SPIDER_WEB
		Type::NORMAL,		// SPIKE_CANNON
		Type::GROUND,		// SPIKES
		Type::NORMAL,		// SPIT_UP
		Type::GHOST,		// SPITE
		Type::NORMAL,		// SPLASH
		Type::GRASS,		// SPORE
		Type::ROCK,		// STEALTH_ROCK
		Type::STEEL,		// STEEL_WING
		Type::NORMAL,		// STOCKPILE
		Type::NORMAL,		// STOMP
		Type::ROCK,		// STONE_EDGE
		Type::NORMAL,		// STRENGTH
		Type::BUG,		// STRING_SHOT
		Type::TYPELESS,	// STRUGGLE
		Type::GRASS,		// STUN_SPORE
		Type::FIGHTING,	// SUBMISSION
		Type::NORMAL,		// SUBSTITUTE
		Type::DARK,		// SUCKER_PUNCH
		Type::FIRE,		// SUNNY_DAY
		Type::NORMAL,		// SUPER_FANG
		Type::FIGHTING,	// SUPERPOWER
		Type::NORMAL,		// SUPERSONIC
		Type::WATER,		// SURF
		Type::NORMAL,		// SWAGGER
		Type::NORMAL,		// SWALLOW
		Type::NORMAL,		// SWEET_KISS
		Type::NORMAL,		// SWEET_SCENT
		Type::NORMAL,		// SWIFT
		Type::TYPELESS,	// SWITCH0
		Type::TYPELESS,	// SWITCH1
		Type::TYPELESS,	// SWITCH2
		Type::TYPELESS,	// SWITCH3
		Type::TYPELESS,	// SWITCH4
		Type::TYPELESS,	// SWITCH5
		Type::DARK,		// SWITCHEROO
		Type::NORMAL,		// SWORDS_DANCE
		Type::GRASS,		// SYNTHESIS
		Type::NORMAL,		// TACKLE
		Type::BUG,		// TAIL_GLOW
		Type::NORMAL,		// TAIL_WHIP
		Type::FLYING,		// TAILWIND
		Type::NORMAL,		// TAKE_DOWN
		Type::DARK,		// TAUNT
		Type::NORMAL,		// TEETER_DANCE
		Type::PSYCHIC,	// TELEPORT
		Type::DARK,		// THIEF
		Type::NORMAL,		// THRASH
		Type::ELECTRIC,	// THUNDER
		Type::ELECTRIC,	// THUNDER_FANG
		Type::ELECTRIC,	// THUNDER_WAVE
		Type::ELECTRIC,	// THUNDERBOLT
		Type::ELECTRIC,	// THUNDERPUNCH
		Type::ELECTRIC,	// THUNDERSHOCK
		Type::NORMAL,		// TICKLE
		Type::DARK,		// TORMENT
		Type::POISON,		// TOXIC
		Type::POISON,		// TOXIC_SPIKES
		Type::NORMAL,		// TRANSFORM
		Type::NORMAL,		// TRI_ATTACK
		Type::PSYCHIC,	// TRICK
		Type::PSYCHIC,	// TRICK_ROOM
		Type::FIGHTING,	// TRIPLE_KICK
		Type::NORMAL,		// TRUMP_CARD
		Type::BUG,		// TWINEEDLE
		Type::DRAGON,		// TWISTER
		Type::BUG,		// U_TURN
		Type::NORMAL,		// UPROAR
		Type::FIGHTING,	// VACUUM_WAVE
		Type::NORMAL,		// VICEGRIP
		Type::GRASS,		// VINE_WHIP
		Type::FIGHTING,	// VITAL_THROW
		Type::ELECTRIC,	// VOLT_TACKLE
		Type::FIGHTING,	// WAKE_UP_SLAP
		Type::WATER,		// WATER_GUN
		Type::WATER,		// WATER_PULSE
		Type::WATER,		// WATER_SPORT
		Type::WATER,		// WATER_SPOUT
		Type::WATER,		// WATERFALL
		Type::NORMAL,		// WEATHER_BALL
		Type::WATER,		// WHIRLPOOL
		Type::NORMAL,		// WHIRLWIND
		Type::FIRE,		// WILL_O_WISP
		Type::FLYING,		// WING_ATTACK
		Type::NORMAL,		// WISH
		Type::WATER,		// WITHDRAW
		Type::GRASS,		// WOOD_HAMMER
		Type::GRASS,		// WORRY_SEED
		Type::NORMAL,		// WRAP
		Type::NORMAL,		// WRING_OUT
		Type::BUG,		// X_SCISSOR
		Type::NORMAL,		// YAWN
		Type::ELECTRIC,	// ZAP_CANNON
		Type::PSYCHIC		// ZEN_HEADBUTT
	};
	return Type (move_type [move]);
}

bool is_physical (Move::Moves move) {
	static constexpr bool is_physical [] = {
		false,		// Absorb
		false,		// Acid
		false,		// Acid Armor
		false,		// Acupressure
		true,		// Aerial Ace
		false,		// Aeroblast
		false,		// Agility
		false,		// Air Cutter
		false,		// Air Slash
		false,		// Amnesia
		false,		// AncientPower
		true,		// Aqua Jet
		false,		// Aqua Ring
		true,		// Aqua Tail
		true,		// Arm Thrust
		false,		// Aromatherapy
		false,		// Assist
		true,		// Assurance
		true,		// Astonish
		true,		// Attack Order
		false,		// Attract
		false,		// Aura Sphere
		false,		// Aurora Beam
		true,		// Avalanche
		true,		// Barrage
		false,		// Barrier
		false,		// Baton Pass
		true,		// Beat Up
		false,		// Belly Drum
		true,		// Bide
		true,		// Bind
		true,		// Bite
		false,		// Blast Burn
		true,		// Blaze Kick
		false,		// Blizzard
		false,		// Block
		true,		// Body Slam
		true,		// Bone Club
		true,		// Bone Rush
		true,		// Bonemerang
		true,		// Bounce
		true,		// Brave Bird
		true,		// Brick Break
		false,		// Brine
		false,		// Bubble
		false,		// BubbleBeam
		true,		// Bug Bite
		false,		// Bug Buzz
		false,		// Bulk Up
		true,		// Bullet Punch
		true,		// Bullet Seed
		false,		// Calm Mind
		false,		// Camouflage
		false,		// Captivate
		false,		// Charge
		false,		// Charge Beam
		false,		// Charm
		false,		// Chatter
		true,		// Clamp
		true,		// Close Combat
		true,		// Comet Punch
		false,		// Confuse Ray
		false,		// Confusion
		true,		// Constrict
		false,		// Conversion
		false,		// Conversion 2
		false,		// Copycat
		false,		// Cosmic Power
		false,		// Cotton Spore
		true,		// Counter
		true,		// Covet
		true,		// Crabhammer
		true,		// Cross Chop
		true,		// Cross Poison
		true,		// Crunch
		true,		// Crush Claw
		true,		// Crush Grip
		false,		// Curse
		true,		// Cut
		false,		// Dark Pulse
		false,		// Dark Void
		false,		// Defend Order
		false,		// Defense Curl
		false,		// Defog
		false,		// Destiny Bond
		false,		// Detect
		true,		// Dig
		false,		// Disable
		false,		// Discharge
		true,		// Dive
		true,		// Dizzy Punch
		false,		// Doom Desire
		true,		// Double Hit
		true,		// Double Kick
		false,		// Double Team
		true,		// Double-Edge
		true,		// DoubleSlap
		false,		// Draco Meteor
		true,		// Dragon Claw
		false,		// Dragon Dance
		false,		// Dragon Pulse
		false,		// Dragon Rage
		true,		// Dragon Rush
		false,		// DragonBreath
		true,		// Drain Punch
		false,		// Dream Eater
		true,		// Drill Peck
		true,		// DynamicPunch
		false,		// Earth Power
		true,		// Earthquake
		true,		// Egg Bomb
		false,		// Embargo
		false,		// Ember
		false,		// Encore
		true,		// Endeavor
		false,		// Endure
		false,		// Energy Ball
		false,		// Eruption
		true,		// Explosion
		false,		// Extrasensory
		true,		// ExtremeSpeed
		true,		// Facade
		true,		// Faint Attack
		true,		// Fake Out
		false,		// Fake Tears
		true,		// False Swipe
		false,		// FeatherDance
		true,		// Feint
		false,		// Fire Blast
		true,		// Fire Fang
		true,		// Fire Punch
		false,		// Fire Spin
		true,		// Fissure
		true,		// Flail
		true,		// Flame Wheel
		false,		// Flamethrower
		true,		// Flare Blitz
		false,		// Flash
		false,		// Flash Cannon
		false,		// Flatter
		true,		// Fling
		true,		// Fly
		false,		// Focus Blast
		false,		// Focus Energy
		true,		// Focus Punch
		false,		// Follow Me
		true,		// Force Palm
		false,		// Foresight
		false,		// Frenzy Plant
		true,		// Frustration
		true,		// Fury Attack
		true,		// Fury Cutter
		true,		// Fury Swipes
		false,		// Future Sight
		false,		// Gastro Acid
		false,		// Giga Drain
		true,		// Giga Impact
		false,		// Glare
		false,		// Grass Knot
		false,		// GrassWhistle
		false,		// Gravity
		false,		// Growl
		false,		// Growth
		false,		// Grudge
		false,		// Guard Swap
		true,		// Guillotine
		true,		// Gunk Shot
		false,		// Gust
		true,		// Gyro Ball
		false,		// Hail
		true,		// Hammer Arm
		false,		// Harden
		false,		// Haze
		true,		// Head Smash
		true,		// Headbutt
		false,		// Heal Bell
		false,		// Heal Block
		false,		// Heal Order
		false,		// Healing Wish
		false,		// Heart Swap
		false,		// Heat Wave
		false,		// Helping Hand
		true,		// Hi Jump Kick
		false,		// Hidden Power
		true,		// Horn Attack
		true,		// Horn Drill
		false,		// Howl
		false,		// Hydro Cannon
		false,		// Hydro Pump
		false,		// Hyper Beam
		true,		// Hyper Fang
		false,		// Hyper Voice
		false,		// Hypnosis
		true,		// Ice Ball
		false,		// Ice Beam
		true,		// Ice Fang
		true,		// Ice Punch
		true,		// Ice Shard
		true,		// Icicle Spear
		false,		// Icy Wind
		false,		// Imprison
		false,		// Ingrain
		false,		// Iron Defense
		true,		// Iron Head
		true,		// Iron Tail
		false,		// Judgment
		true,		// Jump Kick
		true,		// Karate Chop
		false,		// Kinesis
		true,		// Knock Off
		true,		// Last Resort
		false,		// Lava Plume
		true,		// Leaf Blade
		false,		// Leaf Storm
		true,		// Leech Life
		false,		// Leech Seed
		false,		// Leer
		true,		// Lick
		false,		// Light Screen
		false,		// Lock-On
		false,		// Lovely Kiss
		true,		// Low Kick
		false,		// Lucky Chant
		false,		// Lunar Dance
		false,		// Luster Purge
		true,		// Mach Punch
		false,		// Magic Coat
		false,		// Magical Leaf
		false,		// Magma Storm
		true,		// Magnet Bomb
		false,		// Magnet Rise
		true,		// Magnitude
		false,		// Me First
		false,		// Mean Look
		false,		// Meditate
		false,		// Mega Drain
		true,		// Mega Kick
		true,		// Mega Punch
		true,		// Megahorn
		false,		// Memento
		true,		// Metal Burst
		true,		// Metal Claw
		false,		// Metal Sound
		true,		// Meteor Mash
		false,		// Metronome
		false,		// Milk Drink
		false,		// Mimic
		false,		// Mind Reader
		false,		// Minimize
		false,		// Miracle Eye
		false,		// Mirror Coat
		false,		// Mirror Move
		false,		// Mirror Shot
		false,		// Mist
		false,		// Mist Ball
		false,		// Moonlight
		false,		// Morning Sun
		false,		// Mud Bomb
		false,		// Mud Shot
		false,		// Mud-Slap
		false,		// Mud Sport
		false,		// Muddy Water
		false,		// Nasty Plot
		true,		// Natural Gift
		false,		// Nature Power
		true,		// Needle Arm
		false,		// Night Shade
		true,		// Night Slash
		false,		// Nightmare
		false,		// Octazooka
		false,		// Odor Sleuth
		false,		// Ominous Wind
		true,		// Outrage
		false,		// Overheat
		false,		// Pain Split
		true,		// Pay Day
		true,		// Payback
		true,		// Peck
		false,		// Perish Song
		false,		// Petal Dance
		true,		// Pin Missile
		true,		// Pluck
		true,		// Poison Fang
		false,		// Poison Gas
		true,		// Poison Jab
		true,		// Poison Sting
		true,		// Poison Tail
		false,		// PoisonPowder
		true,		// Pound
		false,		// Powder Snow
		false,		// Power Gem
		false,		// Power Swap
		false,		// Power Trick
		true,		// Power Whip
		true,		// Present
		false,		// Protect
		false,		// Psybeam
		false,		// Psych Up
		false,		// Psychic
		false,		// Psycho Boost
		true,		// Psycho Cut
		false,		// Psycho Shift
		false,		// Psywave
		true,		// Punishment
		true,		// Pursuit
		true,		// Quick Attack
		true,		// Rage
		false,		// Rain Dance
		true,		// Rapid Spin
		true,		// Razor Leaf
		false,		// Razor Wind
		false,		// Recover
		false,		// Recycle
		false,		// Reflect
		false,		// Refresh
		false,		// Rest
		true,		// Return
		true,		// Revenge
		true,		// Reversal
		false,		// Roar
		false,		// Roar of Time
		true,		// Rock Blast
		true,		// Rock Climb
		false,		// Rock Polish
		true,		// Rock Slide
		true,		// Rock Smash
		true,		// Rock Throw
		true,		// Rock Tomb
		true,		// Rock Wrecker
		false,		// Role Play
		true,		// Rolling Kick
		true,		// Rollout
		false,		// Roost
		true,		// Sacred Fire
		false,		// Safeguard
		false,		// Sand-Attack
		true,		// Sand Tomb
		false,		// Sandstorm
		false,		// Scary Face
		true,		// Scratch
		false,		// Screech
		true,		// Secret Power
		true,		// Seed Bomb
		false,		// Seed Flare
		true,		// Seismic Toss
		true,		// Selfdestruct
		false,		// Shadow Ball
		true,		// Shadow Claw
		true,		// Shadow Force
		true,		// Shadow Punch
		true,		// Shadow Sneak
		false,		// Sharpen
		false,		// Sheer Cold
		false,		// Shock Wave
		false,		// Signal Beam
		false,		// Silver Wind
		false,		// Sing
		false,		// Sketch
		false,		// Skill Swap
		true,		// Skull Bash
		true,		// Sky Attack
		true,		// Sky Uppercut
		false,		// Slack Off
		true,		// Slam
		true,		// Slash
		false,		// Sleep Powder
		false,		// Sleep Talk
		false,		// Sludge
		false,		// Sludge Bomb
		true,		// SmellingSalt
		false,		// Smog
		false,		// SmokeScreen
		false,		// Snatch
		false,		// Snore
		false,		// Softboiled
		false,		// SolarBeam
		false,		// SonicBoom
		false,		// Spacial Rend
		true,		// Spark
		false,		// Spider Web
		true,		// Spike Cannon
		false,		// Spikes
		false,		// Spit Up
		false,		// Spite
		false,		// Splash
		false,		// Spore
		false,		// Stealth Rock
		true,		// Steel Wing
		false,		// Stockpile
		true,		// Stomp
		true,		// Stone Edge
		true,		// Strength
		false,		// String Shot
		true,		// Struggle
		false,		// Stun Spore
		true,		// Submission
		false,		// Substitute
		true,		// Sucker Punch
		false,		// Sunny Day
		true,		// Super Fang
		true,		// Superpower
		false,		// Supersonic
		false,		// Surf
		false,		// Swagger
		false,		// Swallow
		false,		// Sweet Kiss
		false,		// Sweet Scent
		false,		// Swift
		false,		// Switch0
		false,		// Switch1
		false,		// Switch2
		false,		// Switch3
		false,		// Switch4
		false,		// Switch5
		false,		// Switcheroo
		false,		// Swords Dance
		false,		// Synthesis
		true,		// Tackle
		false,		// Tail Glow
		false,		// Tail Whip
		false,		// Tailwind
		true,		// Take Down
		false,		// Taunt
		false,		// Teeter Dance
		false,		// Teleport
		true,		// Thief
		true,		// Thrash
		false,		// Thunder
		true,		// Thunder Fang
		false,		// Thunder Wave
		false,		// Thunderbolt
		true,		// ThunderPunch
		false,		// ThunderShock
		false,		// Tickle
		false,		// Torment
		false,		// Toxic
		false,		// Toxic Spikes
		false,		// Transform
		false,		// Tri Attack
		false,		// Trick
		false,		// Trick Room
		true,		// Triple Kick
		false,		// Trump Card
		true,		// Twineedle
		false,		// Twister
		true,		// U-turn
		false,		// Uproar
		false,		// Vacuum Wave
		true,		// ViceGrip
		true,		// Vine Whip
		true,		// Vital Throw
		true,		// Volt Tackle
		true,		// Wake-Up Slap
		false,		// Water Gun
		false,		// Water Pulse
		false,		// Water Sport
		false,		// Water Spout
		true,		// Waterfall
		false,		// Weather Ball
		false,		// Whirlpool
		false,		// Whirlwind
		false,		// Will-O-Wisp
		true,		// Wing Attack
		false,		// Wish
		false,		// Withdraw
		true,		// Wood Hammer
		false,		// Worry Seed
		true,		// Wrap
		false,		// Wring Out
		true,		// X-Scissor
		false,		// Yawn
		false,		// Zap Cannon
		true		// Zen Headbutt
	};
	return is_physical [move];
}

uint16_t base_power (Move::Moves move) {
	static constexpr uint16_t get_power [] = {
		20,		// ABSORB
		40,		// ACID
		0,		// ACID_ARMOR
		0,		// ACUPRESSURE
		60,		// AERIAL_ACE
		100,		// AEROBLAST
		0,		// AGILITY
		55,		// AIR_CUTTER
		75,		// AIR_SLASH
		0,		// AMNESIA
		60,		// ANCIENTPOWER
		40,		// AQUA_JET
		0,		// AQUA_RING
		90,		// AQUA_TAIL
		15,		// ARM_THRUST
		0,		// AROMATHERAPY
		0,		// ASSIST
		50,		// ASSURANCE
		30,		// ASTONISH
		90,		// ATTACK_ORDER
		0,		// ATTRACT
		90,		// AURA_SPHERE
		65,		// AURORA_BEAM
		60,		// AVALANCHE
		15,		// BARRAGE
		0,		// BARRIER
		0,		// BATON_PASS
		10,		// BEAT_UP
		0,		// BELLY_DRUM
		0,		// BIDE
		15,		// BIND
		60,		// BITE
		150,		// BLAST_BURN
		85,		// BLAZE_KICK
		120,		// BLIZZARD
		0,		// BLOCK
		85,		// BODY_SLAM
		65,		// BONE_CLUB
		25,		// BONE_RUSH
		50,		// BONEMERANG
		85,		// BOUNCE
		120,		// BRAVE_BIRD
		75,		// BRICK_BREAK
		65,		// BRINE
		20,		// BUBBLE
		65,		// BUBBLEBEAM
		60,		// BUG_BITE
		90,		// BUG_BUZZ
		0,		// BULK_UP
		40,		// BULLET_PUNCH
		10,		// BULLET_SEED
		0,		// CALM_MIND
		0,		// CAMOUFLAGE
		0,		// CAPTIVATE
		0,		// CHARGE
		50,		// CHARGE_BEAM
		0,		// CHARM
		60,		// CHATTER
		35,		// CLAMP
		120,		// CLOSE_COMBAT
		18,		// COMET_PUNCH
		0,		// CONFUSE_RAY
		50,		// CONFUSION
		10,		// CONSTRICT
		0,		// CONVERSION
		0,		// CONVERSION2
		0,		// COPYCAT
		0,		// COSMIC_POWER
		0,		// COTTON_SPORE
		indeterminate_power,		// COUNTER
		40,		// COVET
		90,		// CRABHAMMER
		100,		// CROSS_CHOP
		70,		// CROSS_POISON
		80,		// CRUNCH
		75,		// CRUSH_CLAW
		80,		// CRUSH_GRIP
		0,		// CURSE
		50,		// CUT
		80,		// DARK_PULSE
		0,		// DARK_VOID
		0,		// DEFEND_ORDER
		0,		// DEFENSE_CURL
		0,		// DEFOG
		0,		// DESTINY_BOND
		0,		// DETECT
		80,		// DIG
		0,		// DISABLE
		80,		// DISCHARGE
		80,		// DIVE
		70,		// DIZZY_PUNCH
		120,		// DOOM_DESIRE
		35,		// DOUBLE_HIT
		30,		// DOUBLE_KICK
		0,		// DOUBLE_TEAM
		120,		// DOUBLE_EDGE
		15,		// DOUBLESLAP
		140,		// DRACO_METEOR
		80,		// DRAGON_CLAW
		0,		// DRAGON_DANCE
		90,		// DRAGON_PULSE
		indeterminate_power,		// DRAGON_RAGE
		100,		// DRAGON_RUSH
		60,		// DRAGONBREATH
		60,		// DRAIN_PUNCH
		100,		// DREAM_EATER
		80,		// DRILL_PECK
		100,		// DYNAMICPUNCH
		90,		// EARTH_POWER
		100,		// EARTHQUAKE
		100,		// EGG_BOMB
		0,		// EMBARGO
		40,		// EMBER
		0,		// ENCORE
		indeterminate_power,		// ENDEAVOR
		0,		// ENDURE
		80,		// ENERGY_BALL
		150,		// ERUPTION
		250,		// EXPLOSION
		80,		// EXTRASENSORY
		80,		// EXTREMESPEED
		70,		// FACADE
		60,		// FAINT_ATTACK
		40,		// FAKE_OUT
		0,		// FAKE_TEARS
		40,		// FALSE_SWIPE
		0,		// FEATHERDANCE
		50,		// FEINT
		120,		// FIRE_BLAST
		65,		// FIRE_FANG
		75,		// FIRE_PUNCH
		15,		// FIRE_SPIN
		indeterminate_power,		// FISSURE
		1,		// FLAIL
		60,		// FLAME_WHEEL
		95,		// FLAMETHROWER
		120,		// FLARE_BLITZ
		0,		// FLASH
		80,		// FLASH_CANNON
		0,		// FLATTER
		1,		// FLING
		90,		// FLY
		120,		// FOCUS_BLAST
		0,		// FOCUS_ENERGY
		150,		// FOCUS_PUNCH
		0,		// FOLLOW_ME
		60,		// FORCE_PALM
		0,		// FORESIGHT
		150,		// FRENZY_PLANT
		1,		// FRUSTRATION
		15,		// FURY_ATTACK
		10,		// FURY_CUTTER
		18,		// FURY_SWIPES
		80,		// FUTURE_SIGHT
		0,		// GASTRO_ACID
		60,		// GIGA_DRAIN
		150,		// GIGA_IMPACT
		0,		// GLARE
		1,		// GRASS_KNOT
		0,		// GRASSWHISTLE
		0,		// GRAVITY
		0,		// GROWL
		0,		// GROWTH
		0,		// GRUDGE
		0,		// GUARD_SWAP
		indeterminate_power,		// GUILLOTINE
		120,		// GUNK_SHOT
		40,		// GUST
		1,		// GYRO_BALL
		0,		// HAIL
		100,		// HAMMER_ARM
		0,		// HARDEN
		0,		// HAZE
		150,		// HEAD_SMASH
		70,		// HEADBUTT
		0,		// HEAL_BELL
		0,		// HEAL_BLOCK
		0,		// HEAL_ORDER
		0,		// HEALING_WISH
		0,		// HEART_SWAP
		100,		// HEAT_WAVE
		0,		// HELPING_HAND
		100,		// HI_JUMP_KICK
		1,		// HIDDEN_POWER
		65,		// HORN_ATTACK
		indeterminate_power,		// HORN_DRILL
		0,		// HOWL
		150,		// HYDRO_CANNON
		120,		// HYDRO_PUMP
		150,		// HYPER_BEAM
		80,		// HYPER_FANG
		90,		// HYPER_VOICE
		0,		// HYPNOSIS
		30,		// ICE_BALL
		95,		// ICE_BEAM
		65,		// ICE_FANG
		75,		// ICE_PUNCH
		40,		// ICE_SHARD
		10,		// ICICLE_SPEAR
		55,		// ICY_WIND
		0,		// IMPRISON
		0,		// INGRAIN
		0,		// IRON_DEFENSE
		80,		// IRON_HEAD
		100,		// IRON_TAIL
		100,		// JUDGMENT
		85,		// JUMP_KICK
		50,		// KARATE_CHOP
		0,		// KINESIS
		20,		// KNOCK_OFF
		130,		// LAST_RESORT
		80,		// LAVA_PLUME
		90,		// LEAF_BLADE
		140,		// LEAF_STORM
		20,		// LEECH_LIFE
		0,		// LEECH_SEED
		0,		// LEER
		20,		// LICK
		0,		// LIGHT_SCREEN
		0,		// LOCK_ON
		0,		// LOVELY_KISS
		1,		// LOW_KICK
		0,		// LUCKY_CHANT
		0,		// LUNAR_DANCE
		70,		// LUSTER_PURGE
		40,		// MACH_PUNCH
		0,		// MAGIC_COAT
		60,		// MAGICAL_LEAF
		120,		// MAGMA_STORM
		60,		// MAGNET_BOMB
		0,		// MAGNET_RISE
		1,		// MAGNITUDE
		0,		// ME_FIRST
		0,		// MEAN_LOOK
		0,		// MEDITATE
		40,		// MEGA_DRAIN
		120,		// MEGA_KICK
		80,		// MEGA_PUNCH
		120,		// MEGAHORN
		0,		// MEMENTO
		indeterminate_power,		// METAL_BURST
		50,		// METAL_CLAW
		0,		// METAL_SOUND
		100,		// METEOR_MASH
		0,		// METRONOME
		0,		// MILK_DRINK
		0,		// MIMIC
		0,		// MIND_READER
		0,		// MINIMIZE
		0,		// MIRACLE_EYE
		indeterminate_power,		// MIRROR_COAT
		0,		// MIRROR_MOVE
		65,		// MIRROR_SHOT
		0,		// MIST
		70,		// MIST_BALL
		0,		// MOONLIGHT
		0,		// MORNING_SUN
		65,		// MUD_BOMB
		55,		// MUD_SHOT
		0,		// MUD_SPORT
		20,		// MUD_SLAP
		95,		// MUDDY_WATER
		0,		// NASTY_PLOT
		1,		// NATURAL_GIFT
		0,		// NATURE_POWER
		60,		// NEEDLE_ARM
		indeterminate_power,		// NIGHT_SHADE
		70,		// NIGHT_SLASH
		0,		// NIGHTMARE
		65,		// OCTAZOOKA
		0,		// ODOR_SLEUTH
		60,		// OMINOUS_WIND
		120,		// OUTRAGE
		140,		// OVERHEAT
		0,		// PAIN_SPLIT TODO: ???
		40,		// PAY_DAY
		50,		// PAYBACK
		35,		// PECK
		0,		// PERISH_SONG
		90,		// PETAL_DANCE
		14,		// PIN_MISSILE
		60,		// PLUCK
		50,		// POISON_FANG
		0,		// POISON_GAS
		80,		// POISON_JAB
		15,		// POISON_STING
		50,		// POISON_TAIL
		0,		// POISONPOWDER
		40,		// POUND
		40,		// POWDER_SNOW
		70,		// POWER_GEM
		0,		// POWER_SWAP
		0,		// POWER_TRICK
		120,		// POWER_WHIP
		1,		// PRESENT
		0,		// PROTECT
		65,		// PSYBEAM
		0,		// PSYCH_UP
		90,		// PSYCHIC_MOVE
		140,		// PSYCHO_BOOST
		70,		// PSYCHO_CUT
		0,		// PSYCHO_SHIFT
		indeterminate_power,		// PSYWAVE
		1,		// PUNISHMENT
		40,		// PURSUIT
		40,		// QUICK_ATTACK
		20,		// RAGE
		0,		// RAIN_DANCE
		20,		// RAPID_SPIN
		55,		// RAZOR_LEAF
		80,		// RAZOR_WIND
		0,		// RECOVER
		0,		// RECYCLE
		0,		// REFLECT
		0,		// REFRESH
		0,		// REST
		1,		// RETURN
		60,		// REVENGE
		1,		// REVERSAL
		0,		// ROAR
		150,		// ROAR_OF_TIME
		25,		// ROCK_BLAST
		90,		// ROCK_CLIMB
		0,		// ROCK_POLISH
		75,		// ROCK_SLIDE
		40,		// ROCK_SMASH
		50,		// ROCK_THROW
		50,		// ROCK_TOMB
		150,		// ROCK_WRECKER
		0,		// ROLE_PLAY
		60,		// ROLLING_KICK
		30,		// ROLLOUT
		0,		// ROOST
		100,		// SACRED_FIRE
		0,		// SAFEGUARD
		15,		// SAND_TOMB
		0,		// SAND_ATTACK
		0,		// SANDSTORM
		0,		// SCARY_FACE
		40,		// SCRATCH
		0,		// SCREECH
		70,		// SECRET_POWER
		80,		// SEED_BOMB
		120,		// SEED_FLARE
		indeterminate_power,		// SEISMIC_TOSS
		200,		// SELFDESTRUCT
		80,		// SHADOW_BALL
		70,		// SHADOW_CLAW
		120,		// SHADOW_FORCE
		60,		// SHADOW_PUNCH
		40,		// SHADOW_SNEAK
		0,		// SHARPEN
		indeterminate_power,		// SHEER_COLD
		60,		// SHOCK_WAVE
		75,		// SIGNAL_BEAM
		60,		// SILVER_WIND
		0,		// SING
		0,		// SKETCH
		0,		// SKILL_SWAP
		100,		// SKULL_BASH
		140,		// SKY_ATTACK
		85,		// SKY_UPPERCUT
		0,		// SLACK_OFF
		80,		// SLAM
		70,		// SLASH
		0,		// SLEEP_POWDER
		0,		// SLEEP_TALK
		65,		// SLUDGE
		90,		// SLUDGE_BOMB
		60,		// SMELLINGSALT
		20,		// SMOG
		0,		// SMOKESCREEN
		0,		// SNATCH
		40,		// SNORE
		0,		// SOFTBOILED
		60,		// SOLARBEAM	(I double the power if it's not raining, rather than halve it if it is)
		indeterminate_power,		// SONICBOOM
		100,		// SPACIAL_REND
		65,		// SPARK
		0,		// SPIDER_WEB
		20,		// SPIKE_CANNON
		0,		// SPIKES
		1,		// SPIT_UP
		0,		// SPITE
		0,		// SPLASH
		0,		// SPORE
		0,		// STEALTH_ROCK
		70,		// STEEL_WING
		0,		// STOCKPILE
		65,		// STOMP
		100,		// STONE_EDGE
		80,		// STRENGTH
		0,		// STRING_SHOT
		50,		// STRUGGLE
		0,		// STUN_SPORE
		80,		// SUBMISSION
		0,		// SUBSTITUTE
		80,		// SUCKER_PUNCH
		0,		// SUNNY_DAY
		indeterminate_power,		// SUPER_FANG
		120,		// SUPERPOWER
		0,		// SUPERSONIC
		95,		// SURF
		0,		// SWAGGER
		0,		// SWALLOW
		0,		// SWEET_KISS
		0,		// SWEET_SCENT
		60,		// SWIFT
		0,		// SWITCH0
		0,		// SWITCH1
		0,		// SWITCH2
		0,		// SWITCH3
		0,		// SWITCH4
		0,		// SWITCH5
		0,		// SWITCHEROO
		0,		// SWORDS_DANCE
		0,		// SYNTHESIS
		35,		// TACKLE
		0,		// TAIL_GLOW
		0,		// TAIL_WHIP
		0,		// TAILWIND
		90,		// TAKE_DOWN
		0,		// TAUNT
		0,		// TEETER_DANCE
		0,		// TELEPORT
		40,		// THIEF
		90,		// THRASH
		120,		// THUNDER
		65,		// THUNDER_FANG
		0,		// THUNDER_WAVE
		95,		// THUNDERBOLT
		75,		// THUNDERPUNCH
		40,		// THUNDERSHOCK
		0,		// TICKLE
		0,		// TORMENT
		0,		// TOXIC
		0,		// TOXIC_SPIKES
		0,		// TRANSFORM
		80,		// TRI_ATTACK
		0,		// TRICK
		0,		// TRICK_ROOM
		10,		// TRIPLE_KICK
		1,		// TRUMP_CARD
		25,		// TWINEEDLE
		40,		// TWISTER
		70,		// U_TURN
		50,		// UPROAR
		40,		// VACUUM_WAVE
		55,		// VICEGRIP
		35,		// VINE_WHIP
		70,		// VITAL_THROW
		120,		// VOLT_TACKLE
		60,		// WAKE_UP_SLAP
		40,		// WATER_GUN
		60,		// WATER_PULSE
		0,		// WATER_SPORT
		150,		// WATER_SPOUT
		80,		// WATERFALL
		50,		// WEATHER_BALL
		15,		// WHIRLPOOL
		0,		// WHIRLWIND
		0,		// WILL_O_WISP
		60,		// WING_ATTACK
		0,		// WISH
		0,		// WITHDRAW
		120,		// WOOD_HAMMER
		0,		// WORRY_SEED
		15,		// WRAP
		80,		// WRING_OUT
		80,		// X_SCISSOR
		0,		// YAWN
		120,		// ZAP_CANNON
		80		// ZEN_HEADBUTT
	};
	return get_power [move];
}

uint8_t get_pp (Move::Moves move) {
	static constexpr uint8_t get_pp [] = {
		25,		// Absorb
		30,		// Acid
		40,		// Acid Armor
		30,		// Acupressure
		20,		// Aerial Ace
		5,		// Aeroblast
		30,		// Agility
		25,		// Air Cutter
		20,		// Air Slash
		20,		// Amnesia
		5,		// AncientPower
		20,		// Aqua Jet
		20,		// Aqua Ring
		10,		// Aqua Tail
		20,		// Arm Thrust
		5,		// Aromatherapy
		20,		// Assist
		10,		// Assurance
		15,		// Astonish
		15,		// Attack Order
		15,		// Attract
		20,		// Aura Sphere
		20,		// Aurora Beam
		10,		// Avalanche
		20,		// Barrage
		30,		// Barrier
		40,		// Baton Pass
		10,		// Beat Up
		10,		// Belly Drum
		10,		// Bide
		20,		// Bind
		25,		// Bite
		5,		// Blast Burn
		10,		// Blaze Kick
		5,		// Blizzard
		5,		// Block
		15,		// Body Slam
		20,		// Bone Club
		10,		// Bone Rush
		10,		// Bonemerang
		5,		// Bounce
		15,		// Brave Bird
		15,		// Brick Break
		10,		// Brine
		30,		// Bubble
		20,		// BubbleBeam
		20,		// Bug Bite
		10,		// Bug Buzz
		20,		// Bulk Up
		30,		// Bullet Punch
		30,		// Bullet Seed
		20,		// Calm Mind
		20,		// Camouflage
		20,		// Captivate
		20,		// Charge
		10,		// Charge Beam
		20,		// Charm
		20,		// Chatter
		10,		// Clamp
		5,		// Close Combat
		15,		// Comet Punch
		10,		// Confuse Ray
		25,		// Confusion
		35,		// Constrict
		30,		// Conversion
		30,		// Conversion2
		20,		// Copycat
		20,		// Cosmic Power
		40,		// Cotton Spore
		20,		// Counter
		40,		// Covet
		10,		// Crabhammer
		5,		// Cross Chop
		20,		// Cross Poison
		15,		// Crunch
		10,		// Crush Claw
		5,		// Crush Grip
		10,		// Curse
		30,		// Cut
		15,		// Dark Pulse
		10,		// Dark Void
		10,		// Defend Order
		40,		// Defense Curl
		15,		// Defog
		5,		// Destiny Bond
		5,		// Detect
		10,		// Dig
		20,		// Disable
		15,		// Discharge
		10,		// Dive
		10,		// Dizzy Punch
		5,		// Doom Desire
		10,		// Double Hit
		30,		// Double Kick
		15,		// Double Team
		15,		// Double-Edge
		10,		// DoubleSlap
		5,		// Draco Meteor
		15,		// Dragon Claw
		20,		// Dragon Dance
		10,		// Dragon Pulse
		10,		// Dragon Rage
		10,		// Dragon Rush
		20,		// DragonBreath
		5,		// Drain Punch
		15,		// Dream Eater
		20,		// Drill Peck
		5,		// DynamicPunch
		10,		// Earth Power
		10,		// Earthquake
		10,		// Egg Bomb
		15,		// Embargo
		25,		// Ember
		5,		// Encore
		5,		// Endeavor
		10,		// Endure
		10,		// Energy Ball
		5,		// Eruption
		5,		// Explosion
		30,		// Extrasensory
		5,		// ExtremeSpeed
		20,		// Facade
		20,		// Faint Attack
		10,		// Fake Out
		20,		// Fake Tears
		40,		// False Swipe
		15,		// FeatherDance
		10,		// Feint
		5,		// Fire Blast
		15,		// Fire Fang
		15,		// Fire Punch
		15,		// Fire Spin
		5,		// Fissure
		15,		// Flail
		25,		// Flame Wheel
		15,		// Flamethrower
		15,		// Flare Blitz
		20,		// Flash
		10,		// Flash Cannon
		15,		// Flatter
		10,		// Fling
		15,		// Fly
		5,		// Focus Blast
		30,		// Focus Energy
		20,		// Focus Punch
		20,		// Follow Me
		10,		// Force Palm
		5,		// Foresight
		5,		// Frenzy Plant
		20,		// Frustration
		20,		// Fury Attack
		20,		// Fury Cutter
		15,		// Fury Swipes
		15,		// Future Sight
		10,		// Gastro Acid
		10,		// Giga Drain
		5,		// Giga Impact
		30,		// Glare
		20,		// Grass Knot
		15,		// GrassWhistle
		5,		// Gravity
		40,		// Growl
		40,		// Growth
		5,		// Grudge
		10,		// Guard Swap
		5,		// Guillotine
		5,		// Gunk Shot
		35,		// Gust
		5,		// Gyro Ball
		10,		// Hail
		10,		// Hammer Arm
		30,		// Harden
		30,		// Haze
		5,		// Head Smash
		15,		// Headbutt
		5,		// Heal Bell
		15,		// Heal Block
		10,		// Heal Order
		10,		// Healing Wish
		10,		// Heart Swap
		10,		// Heat Wave
		20,		// Helping Hand
		20,		// Hi Jump Kick
		15,		// Hidden Power
		25,		// Horn Attack
		5,		// Horn Drill
		40,		// Howl
		5,		// Hydro Cannon
		5,		// Hydro Pump
		5,		// Hyper Beam
		15,		// Hyper Fang
		10,		// Hyper Voice
		20,		// Hypnosis
		20,		// Ice Ball
		10,		// Ice Beam
		15,		// Ice Fang
		15,		// Ice Punch
		30,		// Ice Shard
		30,		// Icicle Spear
		15,		// Icy Wind
		10,		// Imprison
		20,		// Ingrain
		15,		// Iron Defense
		15,		// Iron Head
		15,		// Iron Tail
		10,		// Judgment
		25,		// Jump Kick
		25,		// Karate Chop
		15,		// Kinesis
		20,		// Knock Off
		5,		// Last Resort
		15,		// Lava Plume
		15,		// Leaf Blade
		5,		// Leaf Storm
		15,		// Leech Life
		10,		// Leech Seed
		30,		// Leer
		30,		// Lick
		30,		// Light Screen
		5,		// Lock-On
		10,		// Lovely Kiss
		20,		// Low Kick
		30,		// Lucky Chant
		10,		// Lunar Dance
		5,		// Luster Purge
		30,		// Mach Punch
		15,		// Magic Coat
		20,		// Magical Leaf
		5,		// Magma Storm
		20,		// Magnet Bomb
		10,		// Magnet Rise
		30,		// Magnitude
		20,		// Me First
		5,		// Mean Look
		40,		// Meditate
		15,		// Mega Drain
		5,		// Mega Kick
		20,		// Mega Punch
		10,		// Megahorn
		10,		// Memento
		10,		// Metal Burst
		35,		// Metal Claw
		40,		// Metal Sound
		10,		// Meteor Mash
		10,		// Metronome
		10,		// Milk Drink
		10,		// Mimic
		40,		// Mind Reader
		20,		// Minimize
		40,		// Miracle Eye
		20,		// Mirror Coat
		20,		// Mirror Move
		10,		// Mirror Shot
		30,		// Mist
		5,		// Mist Ball
		5,		// Moonlight
		5,		// Morning Sun
		10,		// Mud Bomb
		15,		// Mud Shot
		15,		// Mud Sport
		10,		// Mud-Slap
		10,		// Muddy Water
		20,		// Nasty Plot
		15,		// Natural Gift
		20,		// Nature Power
		15,		// Needle Arm
		15,		// Night Shade
		15,		// Night Slash
		15,		// Nightmare
		10,		// Octazooka
		40,		// Odor Sleuth
		5,		// Ominous Wind
		15,		// Outrage
		5,		// Overheat
		20,		// Pain Split
		20,		// Pay Day
		10,		// Payback
		35,		// Peck
		5,		// Perish Song
		20,		// Petal Dance
		20,		// Pin Missile
		20,		// Pluck
		15,		// Poison Fang
		40,		// Poison Gas
		20,		// Poison Jab
		35,		// Poison Sting
		25,		// Poison Tail
		35,		// PoisonPowder
		35,		// Pound
		25,		// Powder Snow
		20,		// Power Gem
		10,		// Power Swap
		10,		// Power Trick
		10,		// Power Whip
		15,		// Present
		10,		// Protect
		20,		// Psybeam
		10,		// Psych Up
		10,		// Psychic
		5,		// Psycho Boost
		20,		// Psycho Cut
		10,		// Psycho Shift
		15,		// Psywave
		5,		// Punishment
		20,		// Pursuit
		30,		// Quick Attack
		20,		// Rage
		5,		// Rain Dance
		40,		// Rapid Spin
		25,		// Razor Leaf
		10,		// Razor Wind
		10,		// Recover
		10,		// Recycle
		20,		// Reflect
		20,		// Refresh
		10,		// Rest
		20,		// Return
		10,		// Revenge
		15,		// Reversal
		20,		// Roar
		5,		// Roar Of Time
		10,		// Rock Blast
		20,		// Rock Climb
		20,		// Rock Polish
		10,		// Rock Slide
		15,		// Rock Smash
		15,		// Rock Throw
		10,		// Rock Tomb
		5,		// Rock Wrecker
		10,		// Role Play
		15,		// Rolling Kick
		20,		// Rollout
		10,		// Roost
		5,		// Sacred Fire
		25,		// Safeguard
		15,		// Sand Tomb
		15,		// Sand-Attack
		10,		// Sandstorm
		10,		// Scary Face
		35,		// Scratch
		40,		// Screech
		20,		// Secret Power
		15,		// Seed Bomb
		5,		// Seed Flare
		20,		// Seismic Toss
		5,		// Selfdestruct
		15,		// Shadow Ball
		15,		// Shadow Claw
		5,		// Shadow Force
		20,		// Shadow Punch
		30,		// Shadow Sneak
		30,		// Sharpen
		5,		// Sheer Cold
		20,		// Shock Wave
		15,		// Signal Beam
		5,		// Silver Wind
		15,		// Sing
		1,		// Sketch
		10,		// Skill Swap
		15,		// Skull Bash
		5,		// Sky Attack
		15,		// Sky Uppercut
		10,		// Slack Off
		20,		// Slam
		20,		// Slash
		15,		// Sleep Powder
		10,		// Sleep Talk
		20,		// Sludge
		10,		// Sludge Bomb
		10,		// SmellingSalt
		20,		// Smog
		20,		// SmokeScreen
		10,		// Snatch
		15,		// Snore
		10,		// Softboiled
		10,		// SolarBeam
		20,		// SonicBoom
		5,		// Spacial Rend
		20,		// Spark
		10,		// Spider Web
		15,		// Spike Cannon
		20,		// Spikes
		10,		// Spit Up
		10,		// Spite
		40,		// Splash
		15,		// Spore
		20,		// Stealth Rock
		25,		// Steel Wing
		20,		// Stockpile
		20,		// Stomp
		5,		// Stone Edge
		15,		// Strength
		40,		// String Shot
		unlimited_pp,		// Struggle
		30,		// Stun Spore
		25,		// Submission
		10,		// Substitute
		5,		// Sucker Punch
		5,		// Sunny Day
		10,		// Super Fang
		5,		// Superpower
		20,		// Supersonic
		15,		// Surf
		15,		// Swagger
		10,		// Swallow
		10,		// Sweet Kiss
		20,		// Sweet Scent
		20,		// Swift
		unlimited_pp,		// Switch0
		unlimited_pp,		// Switch1
		unlimited_pp,		// Switch2
		unlimited_pp,		// Switch3
		unlimited_pp,		// Switch4
		unlimited_pp,		// Switch5
		10,		// Switcheroo
		30,		// Swords Dance
		5,		// Synthesis
		35,		// Tackle
		20,		// Tail Glow
		30,		// Tail Whip
		30,		// Tailwind
		20,		// Take Down
		20,		// Taunt
		20,		// Teeter Dance
		20,		// Teleport
		10,		// Thief
		20,		// Thrash
		10,		// Thunder
		15,		// Thunder Fang
		20,		// Thunder Wave
		15,		// Thunderbolt
		15,		// ThunderPunch
		30,		// ThunderShock
		20,		// Tickle
		15,		// Torment
		10,		// Toxic
		20,		// Toxic Spikes
		10,		// Transform
		10,		// Tri Attack
		10,		// Trick
		5,		// Trick Room
		10,		// Triple Kick
		5,		// Trump Card
		20,		// Twineedle
		20,		// Twister
		20,		// U-turn
		10,		// Uproar
		30,		// Vacuum Wave
		30,		// ViceGrip
		15,		// Vine Whip
		10,		// Vital Throw
		15,		// Volt Tackle
		10,		// Wake-Up Slap
		25,		// Water Gun
		20,		// Water Pulse
		15,		// Water Sport
		5,		// Water Spout
		15,		// Waterfall
		10,		// Weather Ball
		15,		// Whirlpool
		20,		// Whirlwind
		15,		// Will-O-Wisp
		35,		// Wing Attack
		10,		// Wish
		40,		// Withdraw
		15,		// Wood Hammer
		10,		// Worry Seed
		20,		// Wrap
		5,		// Wring Out
		15,		// X-Scissor
		10,		// Yawn
		5,		// Zap Cannon
		15		// Zen Headbutt
	};
	return get_pp [move];
}

uint16_t get_probability (Move::Moves move) {
// Chance (out of max_probability) to activate side-effect
	static constexpr uint16_t get_probability [] = {
		0,		// Absorb
		84,		// Acid
		0,		// Acid Armor
		140,		// Acupressure
		0,		// Aerial Ace
		0,		// Aeroblast
		0,		// Agility
		0,		// Air Cutter
		252,		// Air Slash
		0,		// Amnesia
		84,		// AncientPower
		0,		// Aqua Jet
		0,		// Aqua Ring
		0,		// Aqua Tail
		0,		// Arm Thrust
		0,		// Aromatherapy
		0,		// Assist
		0,		// Assurance
		252,		// Astonish
		0,		// Attack Order
		0,		// Attract
		0,		// Aura Sphere
		84,		// Aurora Beam
		0,		// Avalanche
		0,		// Barrage
		0,		// Barrier
		0,		// Baton Pass
		0,		// Beat Up
		0,		// Belly Drum
		0,		// Bide
		315,		// Bind
		252,		// Bite
		0,		// Blast Burn
		84,		// Blaze Kick
		84,		// Blizzard
		0,		// Block
		252,		// Body Slam
		84,		// Bone Club
		0,		// Bone Rush
		0,		// Bonemerang
		252,		// Bounce
		0,		// Brave Bird
		0,		// Brick Break
		0,		// Brine
		84,		// Bubble
		84,		// BubbleBeam
		0,		// Bug Bite
		84,		// Bug Buzz
		0,		// Bulk Up
		0,		// Bullet Punch
		0,		// Bullet Seed
		0,		// Calm Mind
		0,		// Camouflage
		0,		// Captivate
		0,		// Charge
		588,		// Charge Beam
		0,		// Charm
		0,		// Chatter
		315,		// Clamp
		0,		// Close Combat
		0,		// Comet Punch
		0,		// Confuse Ray
		84,		// Confusion
		84,		// Constrict
		0,		// Conversion
		0,		// Conversion2
		0,		// Copycat
		0,		// Cosmic Power
		0,		// Cotton Spore
		0,		// Counter
		0,		// Covet
		0,		// Crabhammer
		0,		// Cross Chop
		84,		// Cross Poison
		168,		// Crunch
		420,		// Crush Claw
		0,		// Crush Grip
		0,		// Curse
		0,		// Cut
		168,		// Dark Pulse
		0,		// Dark Void
		0,		// Defend Order
		0,		// Defense Curl
		0,		// Defog
		0,		// Destiny Bond
		0,		// Detect
		0,		// Dig
		0,		// Disable
		252,		// Discharge
		0,		// Dive
		168,		// Dizzy Punch
		0,		// Doom Desire
		0,		// Double Hit
		0,		// Double Kick
		0,		// Double Team
		0,		// Double-Edge
		0,		// DoubleSlap
		0,		// Draco Meteor
		0,		// Dragon Claw
		0,		// Dragon Dance
		0,		// Dragon Pulse
		0,		// Dragon Rage
		168,		// Dragon Rush
		252,		// DragonBreath
		0,		// Drain Punch
		0,		// Dream Eater
		0,		// Drill Peck
		840,		// DynamicPunch
		84,		// Earth Power
		0,		// Earthquake
		0,		// Egg Bomb
		0,		// Embargo
		84,		// Ember
		168,		// Encore
		0,		// Endeavor
		0,		// Endure
		84,		// Energy Ball
		0,		// Eruption
		0,		// Explosion
		84,		// Extrasensory
		0,		// ExtremeSpeed
		0,		// Facade
		0,		// Faint Attack
		840,		// Fake Out
		0,		// Fake Tears
		0,		// False Swipe
		0,		// FeatherDance
		0,		// Feint
		84,		// Fire Blast
		84,		// Fire Fang
		84,		// Fire Punch
		315,		// Fire Spin
		0,		// Fissure
		0,		// Flail
		84,		// Flame Wheel
		84,		// Flamethrower
		84,		// Flare Blitz
		0,		// Flash
		84,		// Flash Cannon
		0,		// Flatter
		0,		// Fling
		0,		// Fly
		84,		// Focus Blast
		0,		// Focus Energy
		0,		// Focus Punch
		0,		// Follow Me
		252,		// Force Palm
		0,		// Foresight
		0,		// Frenzy Plant
		0,		// Frustration
		0,		// Fury Attack
		0,		// Fury Cutter
		0,		// Fury Swipes
		0,		// Future Sight
		0,		// Gastro Acid
		0,		// Giga Drain
		0,		// Giga Impact
		0,		// Glare
		0,		// Grass Knot
		0,		// GrassWhistle
		0,		// Gravity
		0,		// Growl
		0,		// Growth
		0,		// Grudge
		0,		// Guard Swap
		0,		// Guillotine
		252,		// Gunk Shot
		0,		// Gust
		0,		// Gyro Ball
		0,		// Hail
		0,		// Hammer Arm
		0,		// Harden
		0,		// Haze
		0,		// Head Smash
		252,		// Headbutt
		0,		// Heal Bell
		0,		// Heal Block
		0,		// Heal Order
		0,		// Healing Wish
		0,		// Heart Swap
		84,		// Heat Wave
		0,		// Helping Hand
		0,		// Hi Jump Kick
		0,		// Hidden Power
		0,		// Horn Attack
		0,		// Horn Drill
		0,		// Howl
		0,		// Hydro Cannon
		0,		// Hydro Pump
		0,		// Hyper Beam
		84,		// Hyper Fang
		0,		// Hyper Voice
		0,		// Hypnosis
		0,		// Ice Ball
		84,		// Ice Beam
		84,		// Ice Fang
		84,		// Ice Punch
		0,		// Ice Shard
		0,		// Icicle Spear
		0,		// Icy Wind
		0,		// Imprison
		0,		// Ingrain
		0,		// Iron Defense
		252,		// Iron Head
		252,		// Iron Tail
		0,		// Judgment
		0,		// Jump Kick
		0,		// Karate Chop
		0,		// Kinesis
		0,		// Knock Off
		0,		// Last Resort
		252,		// Lava Plume
		0,		// Leaf Blade
		0,		// Leaf Storm
		0,		// Leech Life
		0,		// Leech Seed
		0,		// Leer
		252,		// Lick
		0,		// Light Screen
		0,		// Lock-On
		0,		// Lovely Kiss
		0,		// Low Kick
		0,		// Lucky Chant
		0,		// Lunar Dance
		420,		// Luster Purge
		0,		// Mach Punch
		0,		// Magic Coat
		0,		// Magical Leaf
		315,		// Magma Storm
		0,		// Magnet Bomb
		0,		// Magnet Rise
		0,		// Magnitude
		0,		// Me First
		0,		// Mean Look
		0,		// Meditate
		0,		// Mega Drain
		0,		// Mega Kick
		0,		// Mega Punch
		0,		// Megahorn
		0,		// Memento
		0,		// Metal Burst
		84,		// Metal Claw
		0,		// Metal Sound
		168,		// Meteor Mash
		0,		// Metronome
		0,		// Milk Drink
		0,		// Mimic
		0,		// Mind Reader
		0,		// Minimize
		0,		// Miracle Eye
		0,		// Mirror Coat
		0,		// Mirror Move
		252,		// Mirror Shot
		0,		// Mist
		420,		// Mist Ball
		0,		// Moonlight
		0,		// Morning Sun
		252,		// Mud Bomb
		0,		// Mud Shot
		0,		// Mud Sport
		0,		// Mud-Slap
		252,		// Muddy Water
		0,		// Nasty Plot
		0,		// Natural Gift
		0,		// Nature Power
		252,		// Needle Arm
		0,		// Night Shade
		0,		// Night Slash
		0,		// Nightmare
		420,		// Octazooka
		0,		// Odor Sleuth
		84,		// Ominous Wind
		420,		// Outrage
		0,		// Overheat
		0,		// Pain Split
		0,		// Pay Day
		0,		// Payback
		0,		// Peck
		0,		// Perish Song
		420,		// Petal Dance
		0,		// Pin Missile
		0,		// Pluck
		252,		// Poison Fang
		0,		// Poison Gas
		252,		// Poison Jab
		252,		// Poison Sting
		84,		// Poison Tail
		0,		// PoisonPowder
		0,		// Pound
		84,		// Powder Snow
		0,		// Power Gem
		0,		// Power Swap
		0,		// Power Trick
		0,		// Power Whip
		210,		// Present
		0,		// Protect
		84,		// Psybeam
		0,		// Psych Up
		84,		// Psychic
		0,		// Psycho Boost
		0,		// Psycho Cut
		0,		// Psycho Shift
		0,		// Psywave
		0,		// Punishment
		0,		// Pursuit
		0,		// Quick Attack
		0,		// Rage
		0,		// Rain Dance
		0,		// Rapid Spin
		0,		// Razor Leaf
		0,		// Razor Wind
		0,		// Recover
		0,		// Recycle
		0,		// Reflect
		0,		// Refresh
		0,		// Rest
		0,		// Return
		0,		// Revenge
		0,		// Reversal
		0,		// Roar
		0,		// Roar Of Time
		0,		// Rock Blast
		168,		// Rock Climb
		0,		// Rock Polish
		252,		// Rock Slide
		420,		// Rock Smash
		0,		// Rock Throw
		0,		// Rock Tomb
		0,		// Rock Wrecker
		0,		// Role Play
		252,		// Rolling Kick
		0,		// Rollout
		0,		// Roost
		420,		// Sacred Fire
		0,		// Safeguard
		315,		// Sand Tomb
		0,		// Sand-Attack
		0,		// Sandstorm
		0,		// Scary Face
		0,		// Scratch
		0,		// Screech
		0,		// Secret Power
		0,		// Seed Bomb
		336,		// Seed Flare
		0,		// Seismic Toss
		0,		// Selfdestruct
		168,		// Shadow Ball
		0,		// Shadow Claw
		0,		// Shadow Force
		0,		// Shadow Punch
		0,		// Shadow Sneak
		0,		// Sharpen
		0,		// Sheer Cold
		0,		// Shock Wave
		84,		// Signal Beam
		84,		// Silver Wind
		0,		// Sing
		0,		// Sketch
		0,		// Skill Swap
		0,		// Skull Bash
		252,		// Sky Attack
		0,		// Sky Uppercut
		0,		// Slack Off
		0,		// Slam
		0,		// Slash
		0,		// Sleep Powder
		0,		// Sleep Talk
		252,		// Sludge
		252,		// Sludge Bomb
		0,		// SmellingSalt
		336,		// Smog
		0,		// SmokeScreen
		0,		// Snatch
		252,		// Snore
		0,		// Softboiled
		0,		// SolarBeam
		0,		// SonicBoom
		0,		// Spacial Rend
		252,		// Spark
		0,		// Spider Web
		0,		// Spike Cannon
		0,		// Spikes
		0,		// Spit Up
		0,		// Spite
		0,		// Splash
		0,		// Spore
		0,		// Stealth Rock
		84,		// Steel Wing
		0,		// Stockpile
		252,		// Stomp
		0,		// Stone Edge
		0,		// Strength
		0,		// String Shot
		0,		// Struggle
		0,		// Stun Spore
		0,		// Submission
		0,		// Substitute
		0,		// Sucker Punch
		0,		// Sunny Day
		0,		// Super Fang
		0,		// Superpower
		0,		// Supersonic
		0,		// Surf
		0,		// Swagger
		0,		// Swallow
		0,		// Sweet Kiss
		0,		// Sweet Scent
		0,		// Swift
		0,		// Switch0
		0,		// Switch1
		0,		// Switch2
		0,		// Switch3
		0,		// Switch4
		0,		// Switch5
		0,		// Switcheroo
		0,		// Swords Dance
		0,		// Synthesis
		0,		// Tackle
		0,		// Tail Glow
		0,		// Tail Whip
		0,		// Tailwind
		0,		// Take Down
		420,		// Taunt
		0,		// Teeter Dance
		0,		// Teleport
		0,		// Thief
		420,		// Thrash
		252,		// Thunder
		84,		// Thunder Fang
		0,		// Thunder Wave
		84,		// Thunderbolt
		84,		// ThunderPunch
		84,		// ThunderShock
		0,		// Tickle
		0,		// Torment
		0,		// Toxic
		0,		// Toxic Spikes
		0,		// Transform
		168,		// Tri Attack
		0,		// Trick
		0,		// Trick Room
		0,		// Triple Kick
		0,		// Trump Card
		168,		// Twineedle
		168,		// Twister
		0,		// U-turn
		0,		// Uproar
		0,		// Vacuum Wave
		0,		// ViceGrip
		0,		// Vine Whip
		0,		// Vital Throw
		84,		// Volt Tackle
		0,		// Wake-Up Slap
		0,		// Water Gun
		168,		// Water Pulse
		0,		// Water Sport
		0,		// Water Spout
		168,		// Waterfall
		0,		// Weather Ball
		315,		// Whirlpool
		0,		// Whirlwind
		0,		// Will-O-Wisp
		0,		// Wing Attack
		0,		// Wish
		0,		// Withdraw
		0,		// Wood Hammer
		0,		// Worry Seed
		315,		// Wrap
		0,		// Wring Out
		0,		// X-Scissor
		0,		// Yawn
		0,		// Zap Cannon
		168		// Zen Headbut
	};
	return get_probability [move];
}

uint16_t get_accuracy (Move::Moves move) {
	static constexpr uint16_t get_accuracy [] = {
		100,		// Absorb
		100,		// Acid
		perfect_accuracy,		// Acid Armor
		perfect_accuracy,		// Acupressure
		perfect_accuracy,		// Aerial Ace
		95,		// Aeroblast
		perfect_accuracy,		// Agility
		95,		// Air Cutter
		95,		// Air Slash
		perfect_accuracy,		// Amnesia
		100,		// AncientPower
		100,		// Aqua Jet
		perfect_accuracy,		// Aqua Ring
		90,		// Aqua Tail
		100,		// Arm Thrust
		perfect_accuracy,		// Aromatherapy
		perfect_accuracy,		// Assist
		100,		// Assurance
		100,		// Astonish
		100,		// Attack Order
		100,		// Attract
		perfect_accuracy,		// Aura Sphere
		100,		// Aurora Beam
		100,		// Avalanche
		85,		// Barrage
		perfect_accuracy,		// Barrier
		perfect_accuracy,		// Baton Pass
		100,		// Beat Up
		perfect_accuracy,		// Belly Drum
		perfect_accuracy,		// Bide
		75,		// Bind
		100,		// Bite
		90,		// Blast Burn
		90,		// Blaze Kick
		70,		// Blizzard
		perfect_accuracy,		// Block
		100,		// Body Slam
		85,		// Bone Club
		80,		// Bone Rush
		90,		// Bonemerang
		85,		// Bounce
		100,		// Brave Bird
		100,		// Brick Break
		100,		// Brine
		100,		// Bubble
		100,		// BubbleBeam
		100,		// Bug Bite
		100,		// Bug Buzz
		perfect_accuracy,		// Bulk Up
		100,		// Bullet Punch
		100,		// Bullet Seed
		perfect_accuracy,		// Calm Mind
		perfect_accuracy,		// Camouflage
		100,		// Captivate
		perfect_accuracy,		// Charge
		90,		// Charge Beam
		100,		// Charm
		100,		// Chatter
		75,		// Clamp
		100,		// Close Combat
		85,		// Comet Punch
		100,		// Confuse Ray
		100,		// Confusion
		100,		// Constrict
		perfect_accuracy,		// Conversion
		perfect_accuracy,		// Conversion2
		perfect_accuracy,		// Copycat
		perfect_accuracy,		// Cosmic Power
		85,		// Cotton Spore
		100,		// Counter
		100,		// Covet
		85,		// Crabhammer
		80,		// Cross Chop
		100,		// Cross Poison
		100,		// Crunch
		95,		// Crush Claw
		100,		// Crush Grip
		perfect_accuracy,		// Curse
		95,		// Cut
		100,		// Dark Pulse
		80,		// Dark Void
		perfect_accuracy,		// Defend Order
		perfect_accuracy,		// Defense Curl
		perfect_accuracy,		// Defog
		perfect_accuracy,		// Destiny Bond
		perfect_accuracy,		// Detect
		100,		// Dig
		80,		// Disable
		100,		// Discharge
		100,		// Dive
		100,		// Dizzy Punch
		85,		// Doom Desire
		90,		// Double Hit
		100,		// Double Kick
		perfect_accuracy,		// Double Team
		100,		// Double-Edge
		85,		// DoubleSlap
		90,		// Draco Meteor
		100,		// Dragon Claw
		perfect_accuracy,		// Dragon Dance
		100,		// Dragon Pulse
		100,		// Dragon Rage
		75,		// Dragon Rush
		100,		// DragonBreath
		100,		// Drain Punch
		100,		// Dream Eater
		100,		// Drill Peck
		50,		// DynamicPunch
		100,		// Earth Power
		100,		// Earthquake
		75,		// Egg Bomb
		100,		// Embargo
		100,		// Ember
		100,		// Encore
		100,		// Endeavor
		perfect_accuracy,		// Endure
		100,		// Energy Ball
		100,		// Eruption
		100,		// Explosion
		100,		// Extrasensory
		100,		// ExtremeSpeed
		100,		// Facade
		perfect_accuracy,		// Faint Attack
		100,		// Fake Out
		100,		// Fake Tears
		100,		// False Swipe
		100,		// FeatherDance
		100,		// Feint
		85,		// Fire Blast
		95,		// Fire Fang
		100,		// Fire Punch
		70,		// Fire Spin
		30,		// Fissure
		100,		// Flail
		100,		// Flame Wheel
		100,		// Flamethrower
		100,		// Flare Blitz
		100,		// Flash
		100,		// Flash Cannon
		100,		// Flatter
		100,		// Fling
		95,		// Fly
		70,		// Focus Blast
		perfect_accuracy,		// Focus Energy
		100,		// Focus Punch
		perfect_accuracy,		// Follow Me
		100,		// Force Palm
		perfect_accuracy,		// Foresight
		90,		// Frenzy Plant
		100,		// Frustration
		85,		// Fury Attack
		95,		// Fury Cutter
		80,		// Fury Swipes
		90,		// Future Sight
		100,		// Gastro Acid
		100,		// Giga Drain
		90,		// Giga Impact
		75,		// Glare
		100,		// Grass Knot
		55,		// GrassWhistle
		perfect_accuracy,		// Gravity
		100,		// Growl
		perfect_accuracy,		// Growth
		perfect_accuracy,		// Grudge
		perfect_accuracy,		// Guard Swap
		30,		// Guillotine
		70,		// Gunk Shot
		100,		// Gust
		100,		// Gyro Ball
		perfect_accuracy,		// Hail
		90,		// Hammer Arm
		perfect_accuracy,		// Harden
		perfect_accuracy,		// Haze
		80,		// Head Smash
		100,		// Headbutt
		perfect_accuracy,		// Heal Bell
		100,		// Heal Block
		perfect_accuracy,		// Heal Order
		perfect_accuracy,		// Healing Wish
		perfect_accuracy,		// Heart Swap
		90,		// Heat Wave
		perfect_accuracy,		// Helping Hand
		90,		// Hi Jump Kick
		100,		// Hidden Power
		100,		// Horn Attack
		30,		// Horn Drill
		perfect_accuracy,		// Howl
		90,		// Hydro Cannon
		80,		// Hydro Pump
		90,		// Hyper Beam
		90,		// Hyper Fang
		100,		// Hyper Voice
		60,		// Hypnosis
		90,		// Ice Ball
		100,		// Ice Beam
		95,		// Ice Fang
		100,		// Ice Punch
		100,		// Ice Shard
		100,		// Icicle Spear
		95,		// Icy Wind
		perfect_accuracy,		// Imprison
		perfect_accuracy,		// Ingrain
		perfect_accuracy,		// Iron Defense
		100,		// Iron Head
		75,		// Iron Tail
		100,		// Judgment
		95,		// Jump Kick
		100,		// Karate Chop
		80,		// Kinesis
		100,		// Knock Off
		100,		// Last Resort
		100,		// Lava Plume
		100,		// Leaf Blade
		90,		// Leaf Storm
		100,		// Leech Life
		90,		// Leech Seed
		100,		// Leer
		100,		// Lick
		perfect_accuracy,		// Light Screen
		perfect_accuracy,		// Lock-On
		75,		// Lovely Kiss
		100,		// Low Kick
		perfect_accuracy,		// Lucky Chant
		perfect_accuracy,		// Lunar Dance
		100,		// Luster Purge
		100,		// Mach Punch
		perfect_accuracy,		// Magic Coat
		perfect_accuracy,		// Magical Leaf
		70,		// Magma Storm
		perfect_accuracy,		// Magnet Bomb
		perfect_accuracy,		// Magnet Rise
		100,		// Magnitude
		perfect_accuracy,		// Me First
		perfect_accuracy,		// Mean Look
		perfect_accuracy,		// Meditate
		100,		// Mega Drain
		75,		// Mega Kick
		85,		// Mega Punch
		85,		// Megahorn
		100,		// Memento
		100,		// Metal Burst
		95,		// Metal Claw
		85,		// Metal Sound
		85,		// Meteor Mash
		perfect_accuracy,		// Metronome
		perfect_accuracy,		// Milk Drink
		perfect_accuracy,		// Mimic
		perfect_accuracy,		// Mind Reader
		perfect_accuracy,		// Minimize
		perfect_accuracy,		// Miracle Eye
		100,		// Mirror Coat
		perfect_accuracy,		// Mirror Move
		85,		// Mirror Shot
		perfect_accuracy,		// Mist
		100,		// Mist Ball
		perfect_accuracy,		// Moonlight
		perfect_accuracy,		// Morning Sun
		85,		// Mud Bomb
		95,		// Mud Shot
		perfect_accuracy,		// Mud Sport
		100,		// Mud-Slap
		85,		// Muddy Water
		perfect_accuracy,		// Nasty Plot
		100,		// Natural Gift
		perfect_accuracy,		// Nature Power
		100,		// Needle Arm
		100,		// Night Shade
		100,		// Night Slash
		100,		// Nightmare
		85,		// Octazooka
		perfect_accuracy,		// Odor Sleuth
		100,		// Ominous Wind
		100,		// Outrage
		90,		// Overheat
		perfect_accuracy,		// Pain Split
		100,		// Pay Day
		100,		// Payback
		100,		// Peck
		perfect_accuracy,		// Perish Song
		100,		// Petal Dance
		85,		// Pin Missile
		100,		// Pluck
		100,		// Poison Fang
		55,		// Poison Gas
		100,		// Poison Jab
		100,		// Poison Sting
		100,		// Poison Tail
		75,		// PoisonPowder
		100,		// Pound
		100,		// Powder Snow
		100,		// Power Gem
		perfect_accuracy,		// Power Swap
		perfect_accuracy,		// Power Trick
		85,		// Power Whip
		90,		// Present
		perfect_accuracy,		// Protect
		100,		// Psybeam
		perfect_accuracy,		// Psych Up
		100,		// Psychic
		90,		// Psycho Boost
		100,		// Psycho Cut
		90,		// Psycho Shift
		80,		// Psywave
		100,		// Punishment
		100,		// Pursuit
		100,		// Quick Attack
		100,		// Rage
		perfect_accuracy,		// Rain Dance
		100,		// Rapid Spin
		95,		// Razor Leaf
		100,		// Razor Wind
		perfect_accuracy,		// Recover
		perfect_accuracy,		// Recycle
		perfect_accuracy,		// Reflect
		perfect_accuracy,		// Refresh
		perfect_accuracy,		// Rest
		100,		// Return
		100,		// Revenge
		100,		// Reversal
		100,		// Roar
		90,		// Roar Of Time
		80,		// Rock Blast
		85,		// Rock Climb
		perfect_accuracy,		// Rock Polish
		90,		// Rock Slide
		100,		// Rock Smash
		90,		// Rock Throw
		80,		// Rock Tomb
		90,		// Rock Wrecker
		perfect_accuracy,		// Role Play
		85,		// Rolling Kick
		90,		// Rollout
		perfect_accuracy,		// Roost
		95,		// Sacred Fire
		perfect_accuracy,		// Safeguard
		70,		// Sand Tomb
		100,		// Sand-Attack
		perfect_accuracy,		// Sandstorm
		90,		// Scary Face
		100,		// Scratch
		85,		// Screech
		100,		// Secret Power
		100,		// Seed Bomb
		85,		// Seed Flare
		100,		// Seismic Toss
		100,		// Selfdestruct
		100,		// Shadow Ball
		100,		// Shadow Claw
		100,		// Shadow Force
		perfect_accuracy,		// Shadow Punch
		100,		// Shadow Sneak
		perfect_accuracy,		// Sharpen
		30,		// Sheer Cold
		perfect_accuracy,		// Shock Wave
		100,		// Signal Beam
		100,		// Silver Wind
		55,		// Sing
		perfect_accuracy,		// Sketch
		perfect_accuracy,		// Skill Swap
		100,		// Skull Bash
		90,		// Sky Attack
		90,		// Sky Uppercut
		perfect_accuracy,		// Slack Off
		75,		// Slam
		100,		// Slash
		75,		// Sleep Powder
		perfect_accuracy,		// Sleep Talk
		100,		// Sludge
		100,		// Sludge Bomb
		100,		// SmellingSalt
		70,		// Smog
		100,		// SmokeScreen
		perfect_accuracy,		// Snatch
		100,		// Snore
		perfect_accuracy,		// Softboiled
		100,		// SolarBeam
		90,		// SonicBoom
		95,		// Spacial Rend
		100,		// Spark
		perfect_accuracy,		// Spider Web
		100,		// Spike Cannon
		perfect_accuracy,		// Spikes
		100,		// Spit Up
		100,		// Spite
		perfect_accuracy,		// Splash
		100,		// Spore
		perfect_accuracy,		// Stealth Rock
		90,		// Steel Wing
		perfect_accuracy,		// Stockpile
		100,		// Stomp
		80,		// Stone Edge
		100,		// Strength
		95,		// String Shot
		perfect_accuracy,		// Struggle
		75,		// Stun Spore
		80,		// Submission
		perfect_accuracy,		// Substitute
		100,		// Sucker Punch
		perfect_accuracy,		// Sunny Day
		90,		// Super Fang
		100,		// Superpower
		55,		// Supersonic
		100,		// Surf
		90,		// Swagger
		perfect_accuracy,		// Swallow
		75,		// Sweet Kiss
		100,		// Sweet Scent
		perfect_accuracy,		// Swift
		perfect_accuracy,		// Switch0
		perfect_accuracy,		// Switch1
		perfect_accuracy,		// Switch2
		perfect_accuracy,		// Switch3
		perfect_accuracy,		// Switch4
		perfect_accuracy,		// Switch5
		100,		// Switcheroo
		perfect_accuracy,		// Swords Dance
		perfect_accuracy,		// Synthesis
		95,		// Tackle
		perfect_accuracy,		// Tail Glow
		100,		// Tail Whip
		perfect_accuracy,		// Tailwind
		85,		// Take Down
		100,		// Taunt
		100,		// Teeter Dance
		perfect_accuracy,		// Teleport
		100,		// Thief
		100,		// Thrash
		70,		// Thunder
		95,		// Thunder Fang
		100,		// Thunder Wave
		100,		// Thunderbolt
		100,		// ThunderPunch
		100,		// ThunderShock
		100,		// Tickle
		100,		// Torment
		85,		// Toxic
		perfect_accuracy,		// Toxic Spikes
		perfect_accuracy,		// Transform
		100,		// Tri Attack
		100,		// Trick
		perfect_accuracy,		// Trick Room
		90,		// Triple Kick
		perfect_accuracy,		// Trump Card
		100,		// Twineedle
		100,		// Twister
		100,		// U-turn
		100,		// Uproar
		100,		// Vacuum Wave
		100,		// ViceGrip
		100,		// Vine Whip
		perfect_accuracy,		// Vital Throw
		100,		// Volt Tackle
		100,		// Wake-Up Slap
		100,		// Water Gun
		100,		// Water Pulse
		perfect_accuracy,		// Water Sport
		100,		// Water Spout
		100,		// Waterfall
		100,		// Weather Ball
		70,		// Whirlpool
		100,		// Whirlwind
		75,		// Will-O-Wisp
		100,		// Wing Attack
		perfect_accuracy,		// Wish
		perfect_accuracy,		// Withdraw
		100,		// Wood Hammer
		100,		// Worry Seed
		85,		// Wrap
		100,		// Wring Out
		100,		// X-Scissor
		perfect_accuracy,		// Yawn
		50,		// Zap Cannon
		90		// Zen Headbutt
	};
	return get_accuracy [move];
}

}	// anonymous namespace

void Move::get_magnitude (unsigned const magnitude) {
	variable.set_index (magnitude - 4);
}

}	// namespace technicalmachine
