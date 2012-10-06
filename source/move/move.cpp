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
#include "../rational.hpp"

#include "../pokemon/active_pokemon.hpp"

#include "../string_conversions/conversion.hpp"

#include "../type/type.hpp"
#undef SING

namespace technicalmachine {
namespace {

constexpr uint8_t indeterminate_power = 0xFF;
constexpr uint8_t variable_power = indeterminate_power - 1;
Type get_type (Moves move);
uint8_t get_base_power (Moves move);

}	// unnamed namespace

Move::Move (Moves const move, unsigned const pp_ups, unsigned const size) :
	variable (move, size),
	name (move),
	pp(move, pp_ups),
	score (-Score::VICTORY - 1),
	cached_accuracy(move),
	cached_base_power(get_base_power(move)),
	cached_type(get_type(move)),
	cached_priority(move),
	cached_classification(move) {
}

void Move::reset () {
	disable.reset();
	times_used.reset();
}

bool Move::selectable() const {
	return cached_selectable;
}

void Move::selectable(bool const b) {
	cached_selectable = b;
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

bool Move::is_blocked_by_taunt() const {
	return !is_damaging();
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

unsigned Move::base_power() const {
	return cached_base_power;
}

Priority Move::priority() const {
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

bool Move::affects_target(ActivePokemon const & target, Weather const & weather) const {
	return type().get_effectiveness(target) > 0 and (type() != Type::Ground or grounded (target, weather));
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

uint8_t Move::accuracy() const {
	return cached_accuracy();
}

bool Move::can_miss () const {
	return cached_accuracy.can_miss();
}

namespace {

Type get_type (Moves const move) {
	static constexpr Type::Types move_type [] = {
		Type::Grass,		// ABSORB
		Type::Poison,		// ACID
		Type::Poison,		// ACID_ARMOR
		Type::Normal,		// ACUPRESSURE
		Type::Flying,		// AERIAL_ACE
		Type::Flying,		// AEROBLAST
		Type::Psychic,	// AGILITY
		Type::Flying,		// AIR_CUTTER
		Type::Flying,		// AIR_SLASH
		Type::Psychic,	// AMNESIA
		Type::Rock,		// ANCIENTPOWER
		Type::Water,		// AQUA_JET
		Type::Water,		// AQUA_RING
		Type::Water,		// AQUA_TAIL
		Type::Fighting,	// ARM_THRUST
		Type::Grass,		// AROMATHERAPY
		Type::Normal,		// ASSIST
		Type::Dark,		// ASSURANCE
		Type::Ghost,		// ASTONISH
		Type::Bug,		// ATTACK_ORDER
		Type::Normal,		// ATTRACT
		Type::Fighting,	// AURA_SPHERE
		Type::Ice,		// AURORA_BEAM
		Type::Ice,		// AVALANCHE
		Type::Normal,		// BARRAGE
		Type::Psychic,	// BARRIER
		Type::Normal,		// BATON_PASS
		Type::Dark,		// BEAT_UP
		Type::Normal,		// BELLY_DRUM
		Type::Normal,		// BIDE
		Type::Normal,		// BIND
		Type::Dark,		// BITE
		Type::Fire,		// BLAST_BURN
		Type::Fire,		// BLAZE_KICK
		Type::Ice,		// BLIZZARD
		Type::Normal,		// BLOCK
		Type::Normal,		// BODY_SLAM
		Type::Ground,		// BONE_CLUB
		Type::Ground,		// BONE_RUSH
		Type::Ground,		// BONEMERANG
		Type::Flying,		// BOUNCE
		Type::Flying,		// BRAVE_BIRD
		Type::Fighting,	// BRICK_BREAK
		Type::Water,		// BRINE
		Type::Water,		// BUBBLE
		Type::Water,		// BUBBLEBEAM
		Type::Bug,		// Bug_BITE
		Type::Bug,		// Bug_BUZZ
		Type::Fighting,	// BULK_UP
		Type::Steel,		// BULLET_PUNCH
		Type::Grass,		// BULLET_SEED
		Type::Psychic,	// CALM_MIND
		Type::Normal,		// CAMOUFLAGE
		Type::Normal,		// CAPTIVATE
		Type::Electric,	// CHARGE
		Type::Electric,	// CHARGE_BEAM
		Type::Normal,		// CHARM
		Type::Flying,		// CHATTER
		Type::Water,		// CLAMP
		Type::Fighting,	// CLOSE_COMBAT
		Type::Normal,		// COMET_PUNCH
		Type::Ghost,		// CONFUSE_RAY
		Type::Psychic,	// CONFUSION
		Type::Normal,		// CONSTRICT
		Type::Normal,		// CONVERSION
		Type::Normal,		// CONVERSION_2
		Type::Normal,		// COPYCAT
		Type::Psychic,	// COSMIC_POWER
		Type::Grass,		// COTTON_SPORE
		Type::Fighting,	// COUNTER
		Type::Normal,		// COVET
		Type::Water,		// CRABHAMMER
		Type::Fighting,	// CROSS_CHOP
		Type::Poison,		// CROSS_Poison
		Type::Dark,		// CRUNCH
		Type::Normal,		// CRUSH_CLAW
		Type::Normal,		// CRUSH_GRIP
		Type::Typeless,	// CURSE
		Type::Normal,		// CUT
		Type::Dark,		// Dark_PULSE
		Type::Dark,		// Dark_VOID
		Type::Bug,		// DEFEND_ORDER
		Type::Normal,		// DEFENSE_CURL
		Type::Flying,		// DEFOG
		Type::Ghost,		// DESTINY_BOND
		Type::Fighting,	// DETECT
		Type::Ground,		// DIG
		Type::Normal,		// DISABLE
		Type::Electric,	// DISCHARGE
		Type::Water,		// DIVE
		Type::Normal,		// DIZZY_PUNCH
		Type::Steel,		// DOOM_DESIRE
		Type::Normal,		// DOUBLE_HIT
		Type::Fighting,	// DOUBLE_KICK
		Type::Normal,		// DOUBLE_TEAM
		Type::Normal,		// DOUBLE_EDGE
		Type::Normal,		// DOUBLESLAP
		Type::Dragon,		// DRACO_METEOR
		Type::Dragon,		// Dragon_CLAW
		Type::Dragon,		// Dragon_DANCE
		Type::Dragon,		// Dragon_PULSE
		Type::Dragon,		// Dragon_RAGE
		Type::Dragon,		// Dragon_RUSH
		Type::Dragon,		// DRAGONBREATH
		Type::Fighting,	// DRAIN_PUNCH
		Type::Psychic,	// DREAM_EATER
		Type::Flying,		// DRILL_PECK
		Type::Fighting,	// DYNAMICPUNCH
		Type::Ground,		// EARTH_POWER
		Type::Ground,		// EARTHQUAKE
		Type::Normal,		// EGG_BOMB
		Type::Dark,		// EMBARGO
		Type::Fire,		// EMBER
		Type::Normal,		// ENCORE
		Type::Normal,		// ENDEAVOR
		Type::Normal,		// ENDURE
		Type::Grass,		// ENERGY_BALL
		Type::Fire,		// ERUPTION
		Type::Normal,		// EXPLOSION
		Type::Psychic,	// EXTRASENSORY
		Type::Normal,		// EXTREMESPEED
		Type::Normal,		// FACADE
		Type::Dark,		// FAINT_ATTACK
		Type::Normal,		// FAKE_OUT
		Type::Dark,		// FAKE_TEARS
		Type::Normal,		// FALSE_SWIPE
		Type::Flying,		// FEATHERDANCE
		Type::Normal,		// FEINT
		Type::Fire,		// Fire_BLAST
		Type::Fire,		// Fire_FANG
		Type::Fire,		// Fire_PUNCH
		Type::Fire,		// Fire_SPIN
		Type::Ground,		// FISSURE
		Type::Normal,		// FLAIL
		Type::Fire,		// FLAME_WHEEL
		Type::Fire,		// FLAMETHROWER
		Type::Fire,		// FLARE_BLITZ
		Type::Normal,		// FLASH
		Type::Steel,		// FLASH_CANNON
		Type::Dark,		// FLATTER
		Type::Dark,		// FLING
		Type::Flying,		// FLY
		Type::Fighting,	// FOCUS_BLAST
		Type::Normal,		// FOCUS_ENERGY
		Type::Fighting,	// FOCUS_PUNCH
		Type::Normal,		// FOLLOW_ME
		Type::Fighting,	// FORCE_PALM
		Type::Normal,		// FORESIGHT
		Type::Grass,		// FRENZY_PLANT
		Type::Normal,		// FRUSTRATION
		Type::Normal,		// FURY_ATTACK
		Type::Bug,		// FURY_CUTTER
		Type::Normal,		// FURY_SWIPES
		Type::Psychic,	// FUTURE_SIGHT
		Type::Poison,		// GASTRO_ACID
		Type::Grass,		// GIGA_DRAIN
		Type::Normal,		// GIGA_IMPACT
		Type::Normal,		// GLARE
		Type::Grass,		// Grass_KNOT
		Type::Grass,		// GRASSWHISTLE
		Type::Psychic,	// GRAVITY
		Type::Normal,		// GROWL
		Type::Normal,		// GROWTH
		Type::Ghost,		// GRUDGE
		Type::Psychic,	// GUARD_SWAP
		Type::Normal,		// GUILLOTINE
		Type::Poison,		// GUNK_SHOT
		Type::Flying,		// GUST
		Type::Steel,		// GYRO_BALL
		Type::Ice,		// HAIL
		Type::Fighting,	// HAMMER_ARM
		Type::Normal,		// HARDEN
		Type::Ice,		// HAZE
		Type::Rock,		// HEAD_SMASH
		Type::Normal,		// HEADBUTT
		Type::Normal,		// HEAL_BELL
		Type::Psychic,	// HEAL_BLOCK
		Type::Bug,		// HEAL_ORDER
		Type::Psychic,	// HEALING_WISH
		Type::Psychic,	// HEART_SWAP
		Type::Fire,		// HEAT_WAVE
		Type::Normal,		// HELPING_HAND
		Type::Fighting,	// HI_JUMP_KICK
		Type::Normal,		// HIDDEN_POWER
		Type::Normal,		// HORN_ATTACK
		Type::Normal,		// HORN_DRILL
		Type::Normal,		// HOWL
		Type::Water,		// HYDRO_CANNON
		Type::Water,		// HYDRO_PUMP
		Type::Normal,		// HYPER_BEAM
		Type::Normal,		// HYPER_FANG
		Type::Normal,		// HYPER_VOICE
		Type::Psychic,	// HYPNOSIS
		Type::Ice,		// Ice_BALL
		Type::Ice,		// Ice_BEAM
		Type::Ice,		// Ice_FANG
		Type::Ice,		// Ice_PUNCH
		Type::Ice,		// Ice_SHARD
		Type::Ice,		// ICICLE_SPEAR
		Type::Ice,		// ICY_WIND
		Type::Psychic,	// IMPRISON
		Type::Grass,		// INGRAIN
		Type::Steel,		// IRON_DEFENSE
		Type::Steel,		// IRON_HEAD
		Type::Steel,		// IRON_TAIL
		Type::Normal,		// JUDGMENT
		Type::Fighting,	// JUMP_KICK
		Type::Fighting,	// KARATE_CHOP
		Type::Psychic,	// KINESIS
		Type::Dark,		// KNOCK_OFF
		Type::Normal,		// LAST_RESORT
		Type::Fire,		// LAVA_PLUME
		Type::Grass,		// LEAF_BLADE
		Type::Grass,		// LEAF_STORM
		Type::Bug,		// LEECH_LIFE
		Type::Grass,		// LEECH_SEED
		Type::Normal,		// LEER
		Type::Ghost,		// LICK
		Type::Psychic,	// LIGHT_SCREEN
		Type::Normal,		// LOCK_ON
		Type::Normal,		// LOVELY_KISS
		Type::Fighting,	// LOW_KICK
		Type::Normal,		// LUCKY_CHANT
		Type::Psychic,	// LUNAR_DANCE
		Type::Psychic,	// LUSTER_PURGE
		Type::Fighting,	// MACH_PUNCH
		Type::Psychic,	// MAGIC_COAT
		Type::Grass,		// MAGICAL_LEAF
		Type::Fire,		// MAGMA_STORM
		Type::Steel,		// MAGNET_BOMB
		Type::Electric,	// MAGNET_RISE
		Type::Ground,		// MAGNITUDE
		Type::Normal,		// ME_FIRST
		Type::Normal,		// MEAN_LOOK
		Type::Psychic,	// MEDITATE
		Type::Grass,		// MEGA_DRAIN
		Type::Normal,		// MEGA_KICK
		Type::Normal,		// MEGA_PUNCH
		Type::Bug,		// MEGAHORN
		Type::Dark,		// MEMENTO
		Type::Steel,		// METAL_BURST
		Type::Steel,		// METAL_CLAW
		Type::Steel,		// METAL_SOUND
		Type::Steel,		// METEOR_MASH
		Type::Normal,		// METRONOME_MOVE
		Type::Normal,		// MILK_DRINK
		Type::Normal,		// MIMIC
		Type::Normal,		// MIND_READER
		Type::Normal,		// MINIMIZE
		Type::Psychic,	// MIRACLE_EYE
		Type::Psychic,	// MIRROR_COAT
		Type::Flying,		// MIRROR_MOVE
		Type::Steel,		// MIRROR_SHOT
		Type::Ice,		// MIST
		Type::Psychic,	// MIST_BALL
		Type::Normal,		// MOONLIGHT
		Type::Normal,		// MORNING_SUN
		Type::Ground,		// MUD_BOMB
		Type::Ground,		// MUD_SHOT
		Type::Ground,		// MUD_SLAP
		Type::Ground,		// MUD_SPORT
		Type::Water,		// MUDDY_Water
		Type::Dark,		// NASTY_PLOT
		Type::Normal,		// NATURAL_GIFT
		Type::Normal,		// NATURE_POWER
		Type::Grass,		// NEEDLE_ARM
		Type::Ghost,		// NIGHT_SHADE
		Type::Dark,		// NIGHT_SLASH
		Type::Ghost,		// NIGHTMARE
		Type::Water,		// OCTAZOOKA
		Type::Normal,		// ODOR_SLEUTH
		Type::Ghost,		// OMINOUS_WIND
		Type::Dragon,		// OUTRAGE
		Type::Fire,		// OVERHEAT
		Type::Normal,		// PAIN_SPLIT
		Type::Normal,		// PAY_DAY
		Type::Dark,		// PAYBACK
		Type::Flying,		// PECK
		Type::Normal,		// PERISH_SONG
		Type::Grass,		// PETAL_DANCE
		Type::Bug,		// PIN_MISSILE
		Type::Flying,		// PLUCK
		Type::Poison,		// Poison_FANG
		Type::Poison,		// Poison_GAS
		Type::Poison,		// Poison_JAB
		Type::Poison,		// Poison_STING
		Type::Poison,		// Poison_TAIL
		Type::Poison,		// POISONPOWDER
		Type::Normal,		// POUND
		Type::Ice,		// POWDER_SNOW
		Type::Rock,		// POWER_GEM
		Type::Psychic,	// POWER_SWAP
		Type::Psychic,	// POWER_TRICK
		Type::Grass,		// POWER_WHIP
		Type::Normal,		// PRESENT
		Type::Normal,		// PROTECT
		Type::Psychic,	// PSYBEAM
		Type::Normal,		// PSYCH_UP
		Type::Psychic,	// Psychic_MOVE
		Type::Psychic,	// PSYCHO_BOOST
		Type::Psychic,	// PSYCHO_CUT
		Type::Psychic,	// PSYCHO_SHIFT
		Type::Psychic,	// PSYWAVE
		Type::Dark,		// PUNISHMENT
		Type::Dark,		// PURSUIT
		Type::Normal,		// QUICK_ATTACK
		Type::Normal,		// RAGE
		Type::Water,		// RAIN_DANCE
		Type::Normal,		// RAPID_SPIN
		Type::Grass,		// RAZOR_LEAF
		Type::Normal,		// RAZOR_WIND
		Type::Normal,		// RECOVER
		Type::Normal,		// RECYCLE
		Type::Psychic,	// REFLECT
		Type::Normal,		// REFRESH
		Type::Psychic,	// REST
		Type::Normal,		// RETURN
		Type::Fighting,	// REVENGE
		Type::Fighting,	// REVERSAL
		Type::Normal,		// ROAR
		Type::Dragon,		// ROAR_OF_TIME
		Type::Rock,		// Rock_BLAST
		Type::Normal,		// Rock_CLIMB
		Type::Rock,		// Rock_POLISH
		Type::Rock,		// Rock_SLIDE
		Type::Fighting,	// Rock_SMASH
		Type::Rock,		// Rock_THROW
		Type::Rock,		// Rock_TOMB
		Type::Rock,		// Rock_WRECKER
		Type::Psychic,	// ROLE_PLAY
		Type::Fighting,	// ROLLING_KICK
		Type::Rock,		// ROLLOUT
		Type::Flying,		// ROOST
		Type::Fire,		// SACRED_Fire
		Type::Normal,		// SAFEGUARD
		Type::Ground,		// SAND_ATTACK
		Type::Ground,		// SAND_TOMB
		Type::Rock,		// SANDSTORM
		Type::Normal,		// SCARY_FACE
		Type::Normal,		// SCRATCH
		Type::Normal,		// SCREECH
		Type::Normal,		// SECRET_POWER
		Type::Grass,		// SEED_BOMB
		Type::Grass,		// SEED_FLARE
		Type::Fighting,	// SEISMIC_TOSS
		Type::Normal,		// SELFDESTRUCT
		Type::Ghost,		// SHADOW_BALL
		Type::Ghost,		// SHADOW_CLAW
		Type::Ghost,		// SHADOW_FORCE
		Type::Ghost,		// SHADOW_PUNCH
		Type::Ghost,		// SHADOW_SNEAK
		Type::Normal,		// SHARPEN
		Type::Ice,		// SHEER_COLD
		Type::Electric,	// SHOCK_WAVE
		Type::Bug,		// SIGNAL_BEAM
		Type::Bug,		// SILVER_WIND
		Type::Normal,		// SING
		Type::Normal,		// SKETCH
		Type::Psychic,	// SKILL_SWAP
		Type::Normal,		// SKULL_BASH
		Type::Flying,		// SKY_ATTACK
		Type::Fighting,	// SKY_UPPERCUT
		Type::Normal,		// SLACK_OFF
		Type::Normal,		// SLAM
		Type::Normal,		// SLASH
		Type::Grass,		// SLEEP_POWDER
		Type::Normal,		// SLEEP_TALK
		Type::Poison,		// SLUDGE
		Type::Poison,		// SLUDGE_BOMB
		Type::Normal,		// SMELLINGSALT
		Type::Poison,		// SMOG
		Type::Normal,		// SMOKESCREEN
		Type::Dark,		// SNATCH
		Type::Normal,		// SNORE
		Type::Normal,		// SOFTBOILED
		Type::Grass,		// SOLARBEAM
		Type::Normal,		// SONICBOOM
		Type::Dragon,		// SPACIAL_REND
		Type::Electric,	// SPARK
		Type::Bug,		// SPIDER_WEB
		Type::Normal,		// SPIKE_CANNON
		Type::Ground,		// SPIKES
		Type::Normal,		// SPIT_UP
		Type::Ghost,		// SPITE
		Type::Normal,		// SPLASH
		Type::Grass,		// SPORE
		Type::Rock,		// STEALTH_Rock
		Type::Steel,		// Steel_WING
		Type::Normal,		// STOCKPILE
		Type::Normal,		// STOMP
		Type::Rock,		// STONE_EDGE
		Type::Normal,		// STRENGTH
		Type::Bug,		// STRING_SHOT
		Type::Typeless,	// STRUGGLE
		Type::Grass,		// STUN_SPORE
		Type::Fighting,	// SUBMISSION
		Type::Normal,		// SUBSTITUTE
		Type::Dark,		// SUCKER_PUNCH
		Type::Fire,		// SUNNY_DAY
		Type::Normal,		// SUPER_FANG
		Type::Fighting,	// SUPERPOWER
		Type::Normal,		// SUPERSONIC
		Type::Water,		// SURF
		Type::Normal,		// SWAGGER
		Type::Normal,		// SWALLOW
		Type::Normal,		// SWEET_KISS
		Type::Normal,		// SWEET_SCENT
		Type::Normal,		// SWIFT
		Type::Typeless,	// SWITCH0
		Type::Typeless,	// SWITCH1
		Type::Typeless,	// SWITCH2
		Type::Typeless,	// SWITCH3
		Type::Typeless,	// SWITCH4
		Type::Typeless,	// SWITCH5
		Type::Dark,		// SWITCHEROO
		Type::Normal,		// SWORDS_DANCE
		Type::Grass,		// SYNTHESIS
		Type::Normal,		// TACKLE
		Type::Bug,		// TAIL_GLOW
		Type::Normal,		// TAIL_WHIP
		Type::Flying,		// TAILWIND
		Type::Normal,		// TAKE_DOWN
		Type::Dark,		// TAUNT
		Type::Normal,		// TEETER_DANCE
		Type::Psychic,	// TELEPORT
		Type::Dark,		// THIEF
		Type::Normal,		// THRASH
		Type::Electric,	// THUNDER
		Type::Electric,	// THUNDER_FANG
		Type::Electric,	// THUNDER_WAVE
		Type::Electric,	// THUNDERBOLT
		Type::Electric,	// THUNDERPUNCH
		Type::Electric,	// THUNDERSHOCK
		Type::Normal,		// TICKLE
		Type::Dark,		// TORMENT
		Type::Poison,		// TOXIC
		Type::Poison,		// TOXIC_SPIKES
		Type::Normal,		// TRANSFORM
		Type::Normal,		// TRI_ATTACK
		Type::Psychic,	// TRICK
		Type::Psychic,	// TRICK_ROOM
		Type::Fighting,	// TRIPLE_KICK
		Type::Normal,		// TRUMP_CARD
		Type::Bug,		// TWINEEDLE
		Type::Dragon,		// TWISTER
		Type::Bug,		// U_TURN
		Type::Normal,		// UPROAR
		Type::Fighting,	// VACUUM_WAVE
		Type::Normal,		// VICEGRIP
		Type::Grass,		// VINE_WHIP
		Type::Fighting,	// VITAL_THROW
		Type::Electric,	// VOLT_TACKLE
		Type::Fighting,	// WAKE_UP_SLAP
		Type::Water,		// Water_GUN
		Type::Water,		// Water_PULSE
		Type::Water,		// Water_SPORT
		Type::Water,		// Water_SPOUT
		Type::Water,		// WATERFALL
		Type::Normal,		// WEATHER_BALL
		Type::Water,		// WHIRLPOOL
		Type::Normal,		// WHIRLWIND
		Type::Fire,		// WILL_O_WISP
		Type::Flying,		// WING_ATTACK
		Type::Normal,		// WISH
		Type::Water,		// WITHDRAW
		Type::Grass,		// WOOD_HAMMER
		Type::Grass,		// WORRY_SEED
		Type::Normal,		// WRAP
		Type::Normal,		// WRING_OUT
		Type::Bug,		// X_SCISSOR
		Type::Normal,		// YAWN
		Type::Electric,	// ZAP_CANNON
		Type::Psychic		// ZEN_HEADBUTT
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
		60,		// Bug_BITE
		90,		// Bug_BUZZ
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
		70,		// CROSS_Poison
		80,		// CRUNCH
		75,		// CRUSH_CLAW
		80,		// CRUSH_GRIP
		0,		// CURSE
		50,		// CUT
		80,		// Dark_PULSE
		0,		// Dark_VOID
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
		80,		// Dragon_CLAW
		0,		// Dragon_DANCE
		90,		// Dragon_PULSE
		indeterminate_power,		// Dragon_RAGE
		100,		// Dragon_RUSH
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
		120,		// Fire_BLAST
		65,		// Fire_FANG
		75,		// Fire_PUNCH
		15,		// Fire_SPIN
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
		variable_power,		// Grass_KNOT
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
		30,		// Ice_BALL
		95,		// Ice_BEAM
		65,		// Ice_FANG
		75,		// Ice_PUNCH
		40,		// Ice_SHARD
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
		95,		// MUDDY_Water
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
		50,		// Poison_FANG
		0,		// Poison_GAS
		80,		// Poison_JAB
		15,		// Poison_STING
		50,		// Poison_TAIL
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
		90,		// Psychic_MOVE
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
		25,		// Rock_BLAST
		90,		// Rock_CLIMB
		0,		// Rock_POLISH
		75,		// Rock_SLIDE
		40,		// Rock_SMASH
		50,		// Rock_THROW
		50,		// Rock_TOMB
		150,		// Rock_WRECKER
		0,		// ROLE_PLAY
		60,		// ROLLING_KICK
		30,		// ROLLOUT
		0,		// ROOST
		100,		// SACRED_Fire
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
		0,		// STEALTH_Rock
		70,		// Steel_WING
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
		40,		// Water_GUN
		60,		// Water_PULSE
		0,		// Water_SPORT
		150,		// Water_SPOUT
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

}	// unnamed namespace

std::string Move::to_string() const {
	return ::technicalmachine::to_string(name);
}

}	// namespace technicalmachine
