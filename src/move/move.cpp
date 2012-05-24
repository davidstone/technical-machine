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

#include "moves.hpp"

#include "../evaluate.hpp"
#include "../team.hpp"
#include "../type.hpp"
#include "../weather.hpp"
#undef SING

namespace technicalmachine {
namespace {

constexpr uint8_t indeterminate_power = 0xFF;
constexpr uint8_t variable_power = indeterminate_power - 1;
constexpr uint16_t perfect_accuracy = 0xFFFF;
Type get_type (Moves move);
uint8_t get_base_power (Moves move);
uint16_t get_accuracy (Moves move);
int8_t get_priority(Moves move);

}	// unnamed namespace

Move::Move (Moves const move, unsigned const pp_ups, unsigned const size) :
	variable (move, size),
	name (move),
	pp(move, pp_ups),
	score (-Score::VICTORY - 1),
	accuracy (get_accuracy (move)),
	r (100),
	cached_base_power(get_base_power(move)),
	cached_type(get_type(move)),
	cached_priority(get_priority(move)),
	cached_classification(move) {
}

void Move::reset () {
	disable.reset();
	times_used.reset();
}

bool Move::is_damaging() const {
	return base_power() != 0;
}

bool Move::is_physical() const {
	return cached_classification.is_physical();
}

bool Move::is_special() const {
	return cached_classification.is_special();
}

bool Move::can_critical_hit() const {
	switch (base_power()) {
		case 0:
		case indeterminate_power:
			return false;
		default:
			return true;
	}
}

uint64_t Move::hash () const {
	return static_cast<uint64_t>(name) + static_cast<uint64_t>(Moves::END) *
			(pp.hash() + pp.max_hash() *
			(disable.hash() + disable.max_hash() *
			times_used.hash()));
}

uint64_t Move::max_hash() const {
	return times_used.hash() * disable.max_hash() * pp.max_hash() * static_cast<uint64_t>(Moves::END);
}

bool operator== (Move const & lhs, Move const & rhs) {
	return lhs.name == rhs.name and
			lhs.disable == rhs.disable and
			lhs.pp == rhs.pp and
			lhs.times_used == rhs.times_used;
}

bool operator!= (Move const & lhs, Move const & rhs) {
	return !(lhs == rhs);
}

Type Move::type() const {
	return cached_type;
}

void Move::set_type(Type::Types const t) {
	assert(name == Moves::HIDDEN_POWER);
	cached_type = t;
}

uint8_t Move::base_power() const {
	return cached_base_power;
}

int8_t Move::priority() const {
	return cached_priority;
}

bool Move::is_switch (Moves const name) {
	return Moves::SWITCH0 <= name and name <= Moves::SWITCH5;
}

bool Move::is_switch () const {
	return is_switch (name);
}

Moves Move::from_replacement (unsigned const replacement) {
	return static_cast<Moves>(replacement + static_cast<unsigned>(Moves::SWITCH0));
}

unsigned Move::to_replacement (Moves const name) {
	assert (is_switch(name));
	return static_cast<unsigned>(name) - static_cast<unsigned>(Moves::SWITCH0);
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
	return type().get_effectiveness(pokemon) > 0 and (type() != Type::GROUND or grounded (target, pokemon, weather));
}

bool Move::has_follow_up_decision () const {
	switch (name) {
		case Moves::BATON_PASS:
		case Moves::U_TURN:
			return true;
		default:
			return false;
	}
}

bool Move::calls_other_move () const {
	switch (name) {
//		case Moves::NATURE_POWER:
		case Moves::ASSIST:
		case Moves::COPYCAT:
		case Moves::ME_FIRST:
		case Moves::METRONOME:
		case Moves::MIRROR_MOVE:
		case Moves::SLEEP_TALK:
			return true;
		default:
			return false;
	}
}

bool Move::cannot_ko () const {
	return name == Moves::FALSE_SWIPE;
}

bool Move::breaks_screens () const {
	return name == Moves::BRICK_BREAK;
}

void Move::increment_use_counter() {
	times_used.increment();
}

bool Move::was_used_last () const {
	return times_used.was_used_last();
}

unsigned Move::fury_cutter_power() const {
	return times_used.fury_cutter_power();
}

unsigned Move::momentum_move_power() const {
	return times_used.momentum_move_power();
}

unsigned Move::triple_kick_power() const {
	return times_used.triple_kick_power();
}

Rational Move::metronome_boost() const {
	return times_used.metronome_boost();
}

bool Move::is_struggle_or_switch () const {
	return pp.has_unlimited_pp();
}

bool Move::is_phaze (Moves name) {
	switch (name) {
		case Moves::ROAR:
		case Moves::WHIRLWIND:
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
		case Moves::HEAL_ORDER:
		case Moves::MILK_DRINK:
		case Moves::MOONLIGHT:
		case Moves::MORNING_SUN:
		case Moves::RECOVER:
		case Moves::REST:
		case Moves::ROOST:
		case Moves::SLACK_OFF:
		case Moves::SOFTBOILED:
		case Moves::SWALLOW:
		case Moves::SYNTHESIS:
		case Moves::WISH:
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
		case Moves::BOUNCE:
		case Moves::FLY:
		case Moves::HI_JUMP_KICK:
		case Moves::JUMP_KICK:
		case Moves::MAGNET_RISE:
		case Moves::SPLASH:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_iron_fist () const {
	switch (name) {
		case Moves::BULLET_PUNCH:
		case Moves::COMET_PUNCH:
		case Moves::DIZZY_PUNCH:
		case Moves::DRAIN_PUNCH:
		case Moves::DYNAMICPUNCH:
		case Moves::FIRE_PUNCH:
		case Moves::FOCUS_PUNCH:
		case Moves::HAMMER_ARM:
		case Moves::ICE_PUNCH:
		case Moves::MACH_PUNCH:
		case Moves::MEGA_PUNCH:
		case Moves::METEOR_MASH:
		case Moves::SHADOW_PUNCH:
		case Moves::SKY_UPPERCUT:
		case Moves::THUNDERPUNCH:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_reckless() const {
	switch (name) {
		case Moves::BRAVE_BIRD:
		case Moves::DOUBLE_EDGE:
		case Moves::FLARE_BLITZ:
		case Moves::HEAD_SMASH:
		case Moves::SUBMISSION:
		case Moves::TAKE_DOWN:
		case Moves::VOLT_TACKLE:
		case Moves::WOOD_HAMMER:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_sleeping () const {
	switch (name) {
		case Moves::SLEEP_TALK:
		case Moves::SNORE:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_frozen () const {
	switch (name) {
		case Moves::FLAME_WHEEL:
		case Moves::SACRED_FIRE:
			return true;
		default:
			return false;
	}
}

bool Move::is_sound_based () const {
	switch (name) {
		case Moves::BUG_BUZZ:
		case Moves::CHATTER:
		case Moves::GRASSWHISTLE:
		case Moves::GROWL:
		case Moves::HEAL_BELL:
		case Moves::HYPER_VOICE:
		case Moves::METAL_SOUND:
		case Moves::PERISH_SONG:
		case Moves::ROAR:
		case Moves::SCREECH:
		case Moves::SING:
		case Moves::SNORE:
		case Moves::SUPERSONIC:
		case Moves::UPROAR:
			return true;
		default:
			return false;
	}
}

bool Move::is_self_KO () const {
	switch (name) {
		case Moves::EXPLOSION:
		case Moves::SELFDESTRUCT:
			return true;
		default:
			return false;
	}
}

bool Move::cannot_miss () const {
	return get_accuracy (name) == perfect_accuracy;
}

namespace {

int8_t get_priority(Moves const move) {
	switch (move) {
		case Moves::SWITCH0:
		case Moves::SWITCH1:
		case Moves::SWITCH2:
		case Moves::SWITCH3:
		case Moves::SWITCH4:
		case Moves::SWITCH5:
			return 6;
		case Moves::HELPING_HAND:
			return 5;
		case Moves::MAGIC_COAT:
		case Moves::SNATCH:
			return 4;
		case Moves::DETECT:
		case Moves::ENDURE:
		case Moves::FOLLOW_ME:
		case Moves::PROTECT:
			return 3;
		case Moves::FEINT:
			return 2;
		case Moves::AQUA_JET:
		case Moves::BIDE:
		case Moves::BULLET_PUNCH:
		case Moves::EXTREMESPEED:
		case Moves::FAKE_OUT:
		case Moves::ICE_SHARD:
		case Moves::MACH_PUNCH:
		case Moves::QUICK_ATTACK:
		case Moves::SHADOW_SNEAK:
		case Moves::SUCKER_PUNCH:
		case Moves::VACUUM_WAVE:
			return 1;
		case Moves::VITAL_THROW:
			return -1;
		case Moves::FOCUS_PUNCH:
			return -2;
		case Moves::AVALANCHE:
		case Moves::REVENGE:
			return -3;
		case Moves::COUNTER:
		case Moves::MIRROR_COAT:
			return -4;
		case Moves::ROAR:
		case Moves::WHIRLWIND:
			return -5;
		case Moves::TRICK_ROOM:
			return -6;
		default:
			return 0;
	}
}

Type get_type (Moves const move) {
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
	return Type(move_type[static_cast<unsigned>(move)]);
}

uint8_t get_base_power (Moves const move) {
	static constexpr uint8_t get_power [] = {
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
		variable_power,		// FLAIL
		60,		// FLAME_WHEEL
		95,		// FLAMETHROWER
		120,		// FLARE_BLITZ
		0,		// FLASH
		80,		// FLASH_CANNON
		0,		// FLATTER
		variable_power,		// FLING
		90,		// FLY
		120,		// FOCUS_BLAST
		0,		// FOCUS_ENERGY
		150,		// FOCUS_PUNCH
		0,		// FOLLOW_ME
		60,		// FORCE_PALM
		0,		// FORESIGHT
		150,		// FRENZY_PLANT
		variable_power,		// FRUSTRATION
		15,		// FURY_ATTACK
		10,		// FURY_CUTTER
		18,		// FURY_SWIPES
		80,		// FUTURE_SIGHT
		0,		// GASTRO_ACID
		60,		// GIGA_DRAIN
		150,		// GIGA_IMPACT
		0,		// GLARE
		variable_power,		// GRASS_KNOT
		0,		// GRASSWHISTLE
		0,		// GRAVITY
		0,		// GROWL
		0,		// GROWTH
		0,		// GRUDGE
		0,		// GUARD_SWAP
		indeterminate_power,		// GUILLOTINE
		120,		// GUNK_SHOT
		40,		// GUST
		variable_power,		// GYRO_BALL
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
		variable_power,		// HIDDEN_POWER
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
		variable_power,		// LOW_KICK
		0,		// LUCKY_CHANT
		0,		// LUNAR_DANCE
		70,		// LUSTER_PURGE
		40,		// MACH_PUNCH
		0,		// MAGIC_COAT
		60,		// MAGICAL_LEAF
		120,		// MAGMA_STORM
		60,		// MAGNET_BOMB
		0,		// MAGNET_RISE
		variable_power,		// MAGNITUDE
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
		variable_power,		// NATURAL_GIFT
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
		variable_power,		// PRESENT
		0,		// PROTECT
		65,		// PSYBEAM
		0,		// PSYCH_UP
		90,		// PSYCHIC_MOVE
		140,		// PSYCHO_BOOST
		70,		// PSYCHO_CUT
		0,		// PSYCHO_SHIFT
		indeterminate_power,		// PSYWAVE
		variable_power,		// PUNISHMENT
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
		variable_power,		// RETURN
		60,		// REVENGE
		variable_power,		// REVERSAL
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
		variable_power,		// SPIT_UP
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
		variable_power,		// TRUMP_CARD
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
	return get_power [static_cast<unsigned>(move)];
}

uint16_t get_accuracy (Moves const move) {
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
	return get_accuracy [static_cast<unsigned>(move)];
}

}	// unnamed namespace

}	// namespace technicalmachine
