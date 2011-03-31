// Moves data structure
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MOVE_H_
#define MOVE_H_

#include <string>
#include <vector>
#include "type.h"

namespace tm {

// I use PSYCHIC_MOVE (along with PSYCHIC_TYPE) instead of just making one of them PSYCHIC and the other PSYCHIC_WHATEVER so that I will never get confused as to what to type. Attempting to use PSYCHIC will create an error on compile, allowing me to fix it. Same for METRONOME with the move vs. item.

enum moves_list { ABSORB, ACID, ACID_ARMOR, ACUPRESSURE, AERIAL_ACE, AEROBLAST, AGILITY, AIR_CUTTER, AIR_SLASH, AMNESIA, ANCIENTPOWER, AQUA_JET, AQUA_RING, AQUA_TAIL, ARM_THRUST, AROMATHERAPY, ASSIST, ASSURANCE, ASTONISH, ATTACK_ORDER, ATTRACT, AURA_SPHERE, AURORA_BEAM, AVALANCHE, BARRAGE, BARRIER, BATON_PASS, BEAT_UP, BELLY_DRUM, BIDE, BIND, BITE, BLAST_BURN, BLAZE_KICK, BLIZZARD, BLOCK, BODY_SLAM, BONE_CLUB, BONE_RUSH, BONEMERANG, BOUNCE, BRAVE_BIRD, BRICK_BREAK, BRINE, BUBBLE, BUBBLEBEAM, BUG_BITE, BUG_BUZZ, BULK_UP, BULLET_PUNCH, BULLET_SEED, CALM_MIND, CAMOUFLAGE, CAPTIVATE, CHARGE, CHARGE_BEAM, CHARM, CHATTER, CLAMP, CLOSE_COMBAT, COMET_PUNCH, CONFUSE_RAY, CONFUSION, CONSTRICT, CONVERSION, CONVERSION2, COPYCAT, COSMIC_POWER, COTTON_SPORE, COUNTER, COVET, CRABHAMMER, CROSS_CHOP, CROSS_POISON, CRUNCH, CRUSH_CLAW, CRUSH_GRIP, CURSE, CUT, DARK_PULSE, DARK_VOID, DEFEND_ORDER, DEFENSE_CURL, DEFOG, DESTINY_BOND, DETECT, DIG, DISABLE, DISCHARGE, DIVE, DIZZY_PUNCH, DOOM_DESIRE, DOUBLE_HIT, DOUBLE_KICK, DOUBLE_TEAM, DOUBLE_EDGE, DOUBLESLAP, DRACO_METEOR, DRAGON_CLAW, DRAGON_DANCE, DRAGON_PULSE, DRAGON_RAGE, DRAGON_RUSH, DRAGONBREATH, DRAIN_PUNCH, DREAM_EATER, DRILL_PECK, DYNAMICPUNCH, EARTH_POWER, EARTHQUAKE, EGG_BOMB, EMBARGO, EMBER, ENCORE, ENDEAVOR, ENDURE, ENERGY_BALL, ERUPTION, EXPLOSION, EXTRASENSORY, EXTREMESPEED, FACADE, FAINT_ATTACK, FAKE_OUT, FAKE_TEARS, FALSE_SWIPE, FEATHERDANCE, FEINT, FIRE_BLAST, FIRE_FANG, FIRE_PUNCH, FIRE_SPIN, FISSURE, FLAIL, FLAME_WHEEL, FLAMETHROWER, FLARE_BLITZ, FLASH, FLASH_CANNON, FLATTER, FLING, FLY, FOCUS_BLAST, FOCUS_ENERGY, FOCUS_PUNCH, FOLLOW_ME, FORCE_PALM, FORESIGHT, FRENZY_PLANT, FRUSTRATION, FURY_ATTACK, FURY_CUTTER, FURY_SWIPES, FUTURE_SIGHT, GASTRO_ACID, GIGA_DRAIN, GIGA_IMPACT, GLARE, GRASS_KNOT, GRASSWHISTLE, GRAVITY, GROWL, GROWTH, GRUDGE, GUARD_SWAP, GUILLOTINE, GUNK_SHOT, GUST, GYRO_BALL, HAIL, HAMMER_ARM, HARDEN, HAZE, HEAD_SMASH, HEADBUTT, HEAL_BELL, HEAL_BLOCK, HEAL_ORDER, HEALING_WISH, HEART_SWAP, HEAT_WAVE, HELPING_HAND, HI_JUMP_KICK, HIDDEN_POWER, HORN_ATTACK, HORN_DRILL, HOWL, HYDRO_CANNON, HYDRO_PUMP, HYPER_BEAM, HYPER_FANG, HYPER_VOICE, HYPNOSIS, ICE_BALL, ICE_BEAM, ICE_FANG, ICE_PUNCH, ICE_SHARD, ICICLE_SPEAR, ICY_WIND, IMPRISON, INGRAIN, IRON_DEFENSE, IRON_HEAD, IRON_TAIL, JUDGMENT, JUMP_KICK, KARATE_CHOP, KINESIS, KNOCK_OFF, LAST_RESORT, LAVA_PLUME, LEAF_BLADE, LEAF_STORM, LEECH_LIFE, LEECH_SEED, LEER, LICK, LIGHT_SCREEN, LOCK_ON, LOVELY_KISS, LOW_KICK, LUCKY_CHANT, LUNAR_DANCE, LUSTER_PURGE, MACH_PUNCH, MAGIC_COAT, MAGICAL_LEAF, MAGMA_STORM, MAGNET_BOMB, MAGNET_RISE, MAGNITUDE, ME_FIRST, MEAN_LOOK, MEDITATE, MEGA_DRAIN, MEGA_KICK, MEGA_PUNCH, MEGAHORN, MEMENTO, METAL_BURST, METAL_CLAW, METAL_SOUND, METEOR_MASH, METRONOME_MOVE, MILK_DRINK, MIMIC, MIND_READER, MINIMIZE, MIRACLE_EYE, MIRROR_COAT, MIRROR_MOVE, MIRROR_SHOT, MIST, MIST_BALL, MOONLIGHT, MORNING_SUN, MUD_BOMB, MUD_SHOT, MUD_SPORT, MUD_SLAP, MUDDY_WATER, NASTY_PLOT, NATURAL_GIFT, NATURE_POWER, NEEDLE_ARM, NIGHT_SHADE, NIGHT_SLASH, NIGHTMARE, OCTAZOOKA, ODOR_SLEUTH, OMINOUS_WIND, OUTRAGE, OVERHEAT, PAIN_SPLIT, PAY_DAY, PAYBACK, PECK, PERISH_SONG, PETAL_DANCE, PIN_MISSILE, PLUCK, POISON_FANG, POISON_GAS, POISON_JAB, POISON_STING, POISON_TAIL, POISONPOWDER, POUND, POWDER_SNOW, POWER_GEM, POWER_SWAP, POWER_TRICK, POWER_WHIP, PRESENT, PROTECT, PSYBEAM, PSYCH_UP, PSYCHIC_MOVE, PSYCHO_BOOST, PSYCHO_CUT, PSYCHO_SHIFT, PSYWAVE, PUNISHMENT, PURSUIT, QUICK_ATTACK, RAGE, RAIN_DANCE, RAPID_SPIN, RAZOR_LEAF, RAZOR_WIND, RECOVER, RECYCLE, REFLECT, REFRESH, REST, RETURN, REVENGE, REVERSAL, ROAR, ROAR_OF_TIME, ROCK_BLAST, ROCK_CLIMB, ROCK_POLISH, ROCK_SLIDE, ROCK_SMASH, ROCK_THROW, ROCK_TOMB, ROCK_WRECKER, ROLE_PLAY, ROLLING_KICK, ROLLOUT, ROOST, SACRED_FIRE, SAFEGUARD, SAND_TOMB, SAND_ATTACK, SANDSTORM, SCARY_FACE, SCRATCH, SCREECH, SECRET_POWER, SEED_BOMB, SEED_FLARE, SEISMIC_TOSS, SELFDESTRUCT, SHADOW_BALL, SHADOW_CLAW, SHADOW_FORCE, SHADOW_PUNCH, SHADOW_SNEAK, SHARPEN, SHEER_COLD, SHOCK_WAVE, SIGNAL_BEAM, SILVER_WIND, SING, SKETCH, SKILL_SWAP, SKULL_BASH, SKY_ATTACK, SKY_UPPERCUT, SLACK_OFF, SLAM, SLASH, SLEEP_POWDER, SLEEP_TALK, SLUDGE, SLUDGE_BOMB, SMELLINGSALT, SMOG, SMOKESCREEN, SNATCH, SNORE, SOFTBOILED, SOLARBEAM, SONICBOOM, SPACIAL_REND, SPARK, SPIDER_WEB, SPIKE_CANNON, SPIKES, SPIT_UP, SPITE, SPLASH, SPORE, STEALTH_ROCK, STEEL_WING, STOCKPILE, STOMP, STONE_EDGE, STRENGTH, STRING_SHOT, STRUGGLE, STUN_SPORE, SUBMISSION, SUBSTITUTE, SUCKER_PUNCH, SUNNY_DAY, SUPER_FANG, SUPERPOWER, SUPERSONIC, SURF, SWAGGER, SWALLOW, SWEET_KISS, SWEET_SCENT, SWIFT, SWITCH1, SWITCH2, SWITCH3, SWITCH4, SWITCH5, SWITCH6, SWITCHEROO, SWORDS_DANCE, SYNTHESIS, TACKLE, TAIL_GLOW, TAIL_WHIP, TAILWIND, TAKE_DOWN, TAUNT, TEETER_DANCE, TELEPORT, THIEF, THRASH, THUNDER, THUNDER_FANG, THUNDER_WAVE, THUNDERBOLT, THUNDERPUNCH, THUNDERSHOCK, TICKLE, TORMENT, TOXIC, TOXIC_SPIKES, TRANSFORM, TRI_ATTACK, TRICK, TRICK_ROOM, TRIPLE_KICK, TRUMP_CARD, TWINEEDLE, TWISTER, U_TURN, UPROAR, VACUUM_WAVE, VICEGRIP, VINE_WHIP, VITAL_THROW, VOLT_TACKLE, WAKE_UP_SLAP, WATER_GUN, WATER_PULSE, WATER_SPORT, WATER_SPOUT, WATERFALL, WEATHER_BALL, WHIRLPOOL, WHIRLWIND, WILL_O_WISP, WING_ATTACK, WISH, WITHDRAW, WOOD_HAMMER, WORRY_SEED, WRAP, WRING_OUT, X_SCISSOR, YAWN, ZAP_CANNON, ZEN_HEADBUTT, END_MOVE };

struct moves {
	moves_list name;
	types type;
	short basepower;
	short power;
	bool ch;
	bool execute;
	bool physical;
	bool selectable;
	char accuracy;		// A number between 0 (1?) and 100, according to poccil.
	char disable;			// Number of turns left on this move being Disabled (4-7)
	char pp;
	char pp_max;			// PP after all PP ups are applied
	char priority;
	char r;					// The random number (85 through 100)
	char score;				// What score will this move give?
	char times_used;
	// effect is used for side effects of moves. example: Fire Fang: effect == 1 = burn, effect == 2 = flinch, effect == 3 = both.
	char effect;
	// probability determines the odds that the effect does work. For moves without a side effect, probability == 0
	double probability;
	// variable is used for moves with a variable power / length / other integer range. Moves of variable power: Magnitude = 4-10, Psywave = 5-15, Present = 0-4 (0 = heal).
	std::vector<unsigned char> range;
	std::vector<unsigned char>::iterator variable;
	
	bool operator== (const moves &other) const {
		return this->pp == other.pp and this->times_used == other.times_used;
	}
};

// Various states a Pokemon can be in due to vanishing moves.

enum Vanish { LANDED, BOUNCED, DUG, DIVED, FLOWN, SHADOW_FORCED };

const types move_type [] = {
GRASS,		// ABSORB
POISON,		// ACID
POISON,		// ACID_ARMOR
NORMAL,		// ACUPRESSURE
FLYING,		// AERIAL_ACE
FLYING,		// AEROBLAST
PSYCHIC_TYPE,	// AGILITY
FLYING,		// AIR_CUTTER
FLYING,		// AIR_SLASH
PSYCHIC_TYPE,	// AMNESIA
ROCK,		// ANCIENTPOWER
WATER,		// AQUA_JET
WATER,		// AQUA_RING
WATER,		// AQUA_TAIL
FIGHTING,	// ARM_THRUST
GRASS,		// AROMATHERAPY
NORMAL,		// ASSIST
DARK,		// ASSURANCE
GHOST,		// ASTONISH
BUG,		// ATTACK_ORDER
NORMAL,		// ATTRACT
FIGHTING,	// AURA_SPHERE
ICE,		// AURORA_BEAM
ICE,		// AVALANCHE
NORMAL,		// BARRAGE
PSYCHIC_TYPE,	// BARRIER
NORMAL,		// BATON_PASS
DARK,		// BEAT_UP
NORMAL,		// BELLY_DRUM
NORMAL,		// BIDE
NORMAL,		// BIND
DARK,		// BITE
FIRE,		// BLAST_BURN
FIRE,		// BLAZE_KICK
ICE,		// BLIZZARD
NORMAL,		// BLOCK
NORMAL,		// BODY_SLAM
GROUND,		// BONE_CLUB
GROUND,		// BONE_RUSH
GROUND,		// BONEMERANG
FLYING,		// BOUNCE
FLYING,		// BRAVE_BIRD
FIGHTING,	// BRICK_BREAK
WATER,		// BRINE
WATER,		// BUBBLE
WATER,		// BUBBLEBEAM
BUG,		// BUG_BITE
BUG,		// BUG_BUZZ
FIGHTING,	// BULK_UP
STEEL,		// BULLET_PUNCH
GRASS,		// BULLET_SEED
PSYCHIC_TYPE,	// CALM_MIND
NORMAL,		// CAMOUFLAGE
NORMAL,		// CAPTIVATE
ELECTRIC,	// CHARGE
ELECTRIC,	// CHARGE_BEAM
NORMAL,		// CHARM
FLYING,		// CHATTER
WATER,		// CLAMP
FIGHTING,	// CLOSE_COMBAT
NORMAL,		// COMET_PUNCH
GHOST,		// CONFUSE_RAY
PSYCHIC_TYPE,	// CONFUSION
NORMAL,		// CONSTRICT
NORMAL,		// CONVERSION
NORMAL,		// CONVERSION_2
NORMAL,		// COPYCAT
PSYCHIC_TYPE,	// COSMIC_POWER
GRASS,		// COTTON_SPORE
FIGHTING,	// COUNTER
NORMAL,		// COVET
WATER,		// CRABHAMMER
FIGHTING,	// CROSS_CHOP
POISON,		// CROSS_POISON
DARK,		// CRUNCH
NORMAL,		// CRUSH_CLAW
NORMAL,		// CRUSH_GRIP
TYPELESS,	// CURSE
NORMAL,		// CUT
DARK,		// DARK_PULSE
DARK,		// DARK_VOID
BUG,		// DEFEND_ORDER
NORMAL,		// DEFENSE_CURL
FLYING,		// DEFOG
GHOST,		// DESTINY_BOND
FIGHTING,	// DETECT
GROUND,		// DIG
NORMAL,		// DISABLE
ELECTRIC,	// DISCHARGE
WATER,		// DIVE
NORMAL,		// DIZZY_PUNCH
STEEL,		// DOOM_DESIRE
NORMAL,		// DOUBLE_HIT
FIGHTING,	// DOUBLE_KICK
NORMAL,		// DOUBLE_TEAM
NORMAL,		// DOUBLE_EDGE
NORMAL,		// DOUBLESLAP
DRAGON,		// DRACO_METEOR
DRAGON,		// DRAGON_CLAW
DRAGON,		// DRAGON_DANCE
DRAGON,		// DRAGON_PULSE
DRAGON,		// DRAGON_RAGE
DRAGON,		// DRAGON_RUSH
DRAGON,		// DRAGONBREATH
FIGHTING,	// DRAIN_PUNCH
PSYCHIC_TYPE,	// DREAM_EATER
FLYING,		// DRILL_PECK
FIGHTING,	// DYNAMICPUNCH
GROUND,		// EARTH_POWER
GROUND,		// EARTHQUAKE
NORMAL,		// EGG_BOMB
DARK,		// EMBARGO
FIRE,		// EMBER
NORMAL,		// ENCORE
NORMAL,		// ENDEAVOR
NORMAL,		// ENDURE
GRASS,		// ENERGY_BALL
FIRE,		// ERUPTION
NORMAL,		// EXPLOSION
PSYCHIC_TYPE,	// EXTRASENSORY
NORMAL,		// EXTREMESPEED
NORMAL,		// FACADE
DARK,		// FAINT_ATTACK
NORMAL,		// FAKE_OUT
DARK,		// FAKE_TEARS
NORMAL,		// FALSE_SWIPE
FLYING,		// FEATHERDANCE
NORMAL,		// FEINT
FIRE,		// FIRE_BLAST
FIRE,		// FIRE_FANG
FIRE,		// FIRE_PUNCH
FIRE,		// FIRE_SPIN
GROUND,		// FISSURE
NORMAL,		// FLAIL
FIRE,		// FLAME_WHEEL
FIRE,		// FLAMETHROWER
FIRE,		// FLARE_BLITZ
NORMAL,		// FLASH
STEEL,		// FLASH_CANNON
DARK,		// FLATTER
DARK,		// FLING
FLYING,		// FLY
FIGHTING,	// FOCUS_BLAST
NORMAL,		// FOCUS_ENERGY
FIGHTING,	// FOCUS_PUNCH
NORMAL,		// FOLLOW_ME
FIGHTING,	// FORCE_PALM
NORMAL,		// FORESIGHT
GRASS,		// FRENZY_PLANT
NORMAL,		// FRUSTRATION
NORMAL,		// FURY_ATTACK
BUG,		// FURY_CUTTER
NORMAL,		// FURY_SWIPES
PSYCHIC_TYPE,	// FUTURE_SIGHT
POISON,		// GASTRO_ACID
GRASS,		// GIGA_DRAIN
NORMAL,		// GIGA_IMPACT
NORMAL,		// GLARE
GRASS,		// GRASS_KNOT
GRASS,		// GRASSWHISTLE
PSYCHIC_TYPE,	// GRAVITY
NORMAL,		// GROWL
NORMAL,		// GROWTH
GHOST,		// GRUDGE
PSYCHIC_TYPE,	// GUARD_SWAP
NORMAL,		// GUILLOTINE
POISON,		// GUNK_SHOT
FLYING,		// GUST
STEEL,		// GYRO_BALL
ICE,		// HAIL
FIGHTING,	// HAMMER_ARM
NORMAL,		// HARDEN
ICE,		// HAZE
ROCK,		// HEAD_SMASH
NORMAL,		// HEADBUTT
NORMAL,		// HEAL_BELL
PSYCHIC_TYPE,	// HEAL_BLOCK
BUG,		// HEAL_ORDER
PSYCHIC_TYPE,	// HEALING_WISH
PSYCHIC_TYPE,	// HEART_SWAP
FIRE,		// HEAT_WAVE
NORMAL,		// HELPING_HAND
FIGHTING,	// HI_JUMP_KICK
NORMAL,		// HIDDEN_POWER
NORMAL,		// HORN_ATTACK
NORMAL,		// HORN_DRILL
NORMAL,		// HOWL
WATER,		// HYDRO_CANNON
WATER,		// HYDRO_PUMP
NORMAL,		// HYPER_BEAM
NORMAL,		// HYPER_FANG
NORMAL,		// HYPER_VOICE
PSYCHIC_TYPE,	// HYPNOSIS
ICE,		// ICE_BALL
ICE,		// ICE_BEAM
ICE,		// ICE_FANG
ICE,		// ICE_PUNCH
ICE,		// ICE_SHARD
ICE,		// ICICLE_SPEAR
ICE,		// ICY_WIND
PSYCHIC_TYPE,	// IMPRISON
GRASS,		// INGRAIN
STEEL,		// IRON_DEFENSE
STEEL,		// IRON_HEAD
STEEL,		// IRON_TAIL
NORMAL,		// JUDGMENT
FIGHTING,	// JUMP_KICK
FIGHTING,	// KARATE_CHOP
PSYCHIC_TYPE,	// KINESIS
DARK,		// KNOCK_OFF
NORMAL,		// LAST_RESORT
FIRE,		// LAVA_PLUME
GRASS,		// LEAF_BLADE
GRASS,		// LEAF_STORM
BUG,		// LEECH_LIFE
GRASS,		// LEECH_SEED
NORMAL,		// LEER
GHOST,		// LICK
PSYCHIC_TYPE,	// LIGHT_SCREEN
NORMAL,		// LOCK_ON
NORMAL,		// LOVELY_KISS
FIGHTING,	// LOW_KICK
NORMAL,		// LUCKY_CHANT
PSYCHIC_TYPE,	// LUNAR_DANCE
PSYCHIC_TYPE,	// LUSTER_PURGE
FIGHTING,	// MACH_PUNCH
PSYCHIC_TYPE,	// MAGIC_COAT
GRASS,		// MAGICAL_LEAF
FIRE,		// MAGMA_STORM
STEEL,		// MAGNET_BOMB
ELECTRIC,	// MAGNET_RISE
GROUND,		// MAGNITUDE
NORMAL,		// ME_FIRST
NORMAL,		// MEAN_LOOK
PSYCHIC_TYPE,	// MEDITATE
GRASS,		// MEGA_DRAIN
NORMAL,		// MEGA_KICK
NORMAL,		// MEGA_PUNCH
BUG,		// MEGAHORN
DARK,		// MEMENTO
STEEL,		// METAL_BURST
STEEL,		// METAL_CLAW
STEEL,		// METAL_SOUND
STEEL,		// METEOR_MASH
NORMAL,		// METRONOME_MOVE
NORMAL,		// MILK_DRINK
NORMAL,		// MIMIC
NORMAL,		// MIND_READER
NORMAL,		// MINIMIZE
PSYCHIC_TYPE,	// MIRACLE_EYE
PSYCHIC_TYPE,	// MIRROR_COAT
FLYING,		// MIRROR_MOVE
STEEL,		// MIRROR_SHOT
ICE,		// MIST
PSYCHIC_TYPE,	// MIST_BALL
NORMAL,		// MOONLIGHT
NORMAL,		// MORNING_SUN
GROUND,		// MUD_BOMB
GROUND,		// MUD_SHOT
GROUND,		// MUD_SLAP
GROUND,		// MUD_SPORT
WATER,		// MUDDY_WATER
DARK,		// NASTY_PLOT
NORMAL,		// NATURAL_GIFT
NORMAL,		// NATURE_POWER
GRASS,		// NEEDLE_ARM
GHOST,		// NIGHT_SHADE
DARK,		// NIGHT_SLASH
GHOST,		// NIGHTMARE
WATER,		// OCTAZOOKA
NORMAL,		// ODOR_SLEUTH
GHOST,		// OMINOUS_WIND
DRAGON,		// OUTRAGE
FIRE,		// OVERHEAT
NORMAL,		// PAIN_SPLIT
NORMAL,		// PAY_DAY
DARK,		// PAYBACK
FLYING,		// PECK
NORMAL,		// PERISH_SONG
GRASS,		// PETAL_DANCE
BUG,		// PIN_MISSILE
FLYING,		// PLUCK
POISON,		// POISON_FANG
POISON,		// POISON_GAS
POISON,		// POISON_JAB
POISON,		// POISON_STING
POISON,		// POISON_TAIL
POISON,		// POISONPOWDER
NORMAL,		// POUND
ICE,		// POWDER_SNOW
ROCK,		// POWER_GEM
PSYCHIC_TYPE,	// POWER_SWAP
PSYCHIC_TYPE,	// POWER_TRICK
GRASS,		// POWER_WHIP
NORMAL,		// PRESENT
NORMAL,		// PROTECT
PSYCHIC_TYPE,	// PSYBEAM
NORMAL,		// PSYCH_UP
PSYCHIC_TYPE,	// PSYCHIC_MOVE
PSYCHIC_TYPE,	// PSYCHO_BOOST
PSYCHIC_TYPE,	// PSYCHO_CUT
PSYCHIC_TYPE,	// PSYCHO_SHIFT
PSYCHIC_TYPE,	// PSYWAVE
DARK,		// PUNISHMENT
DARK,		// PURSUIT
NORMAL,		// QUICK_ATTACK
NORMAL,		// RAGE
WATER,		// RAIN_DANCE
NORMAL,		// RAPID_SPIN
GRASS,		// RAZOR_LEAF
NORMAL,		// RAZOR_WIND
NORMAL,		// RECOVER
NORMAL,		// RECYCLE
PSYCHIC_TYPE,	// REFLECT
NORMAL,		// REFRESH
PSYCHIC_TYPE,	// REST
NORMAL,		// RETURN
FIGHTING,	// REVENGE
FIGHTING,	// REVERSAL
NORMAL,		// ROAR
DRAGON,		// ROAR_OF_TIME
ROCK,		// ROCK_BLAST
NORMAL,		// ROCK_CLIMB
ROCK,		// ROCK_POLISH
ROCK,		// ROCK_SLIDE
FIGHTING,	// ROCK_SMASH
ROCK,		// ROCK_THROW
ROCK,		// ROCK_TOMB
ROCK,		// ROCK_WRECKER
PSYCHIC_TYPE,	// ROLE_PLAY
FIGHTING,	// ROLLING_KICK
ROCK,		// ROLLOUT
FLYING,		// ROOST
FIRE,		// SACRED_FIRE
NORMAL,		// SAFEGUARD
GROUND,		// SAND_ATTACK
GROUND,		// SAND_TOMB
ROCK,		// SANDSTORM
NORMAL,		// SCARY_FACE
NORMAL,		// SCRATCH
NORMAL,		// SCREECH
NORMAL,		// SECRET_POWER
GRASS,		// SEED_BOMB
GRASS,		// SEED_FLARE
FIGHTING,	// SEISMIC_TOSS
NORMAL,		// SELFDESTRUCT
GHOST,		// SHADOW_BALL
GHOST,		// SHADOW_CLAW
GHOST,		// SHADOW_FORCE
GHOST,		// SHADOW_PUNCH
GHOST,		// SHADOW_SNEAK
NORMAL,		// SHARPEN
ICE,		// SHEER_COLD
ELECTRIC,	// SHOCK_WAVE
BUG,		// SIGNAL_BEAM
BUG,		// SILVER_WIND
NORMAL,		// SING
NORMAL,		// SKETCH
PSYCHIC_TYPE,	// SKILL_SWAP
NORMAL,		// SKULL_BASH
FLYING,		// SKY_ATTACK
FIGHTING,	// SKY_UPPERCUT
NORMAL,		// SLACK_OFF
NORMAL,		// SLAM
NORMAL,		// SLASH
GRASS,		// SLEEP_POWDER
NORMAL,		// SLEEP_TALK
POISON,		// SLUDGE
POISON,		// SLUDGE_BOMB
NORMAL,		// SMELLINGSALT
POISON,		// SMOG
NORMAL,		// SMOKESCREEN
DARK,		// SNATCH
NORMAL,		// SNORE
NORMAL,		// SOFTBOILED
GRASS,		// SOLARBEAM
NORMAL,		// SONICBOOM
DRAGON,		// SPACIAL_REND
ELECTRIC,	// SPARK
BUG,		// SPIDER_WEB
NORMAL,		// SPIKE_CANNON
GROUND,		// SPIKES
NORMAL,		// SPIT_UP
GHOST,		// SPITE
NORMAL,		// SPLASH
GRASS,		// SPORE
ROCK,		// STEALTH_ROCK
STEEL,		// STEEL_WING
NORMAL,		// STOCKPILE
NORMAL,		// STOMP
ROCK,		// STONE_EDGE
NORMAL,		// STRENGTH
BUG,		// STRING_SHOT
TYPELESS,	// STRUGGLE
GRASS,		// STUN_SPORE
FIGHTING,	// SUBMISSION
NORMAL,		// SUBSTITUTE
DARK,		// SUCKER_PUNCH
FIRE,		// SUNNY_DAY
NORMAL,		// SUPER_FANG
FIGHTING,	// SUPERPOWER
NORMAL,		// SUPERSONIC
WATER,		// SURF
NORMAL,		// SWAGGER
NORMAL,		// SWALLOW
NORMAL,		// SWEET_KISS
NORMAL,		// SWEET_SCENT
NORMAL,		// SWIFT
TYPELESS,	// SWITCH1
TYPELESS,	// SWITCH2
TYPELESS,	// SWITCH3
TYPELESS,	// SWITCH4
TYPELESS,	// SWITCH5
TYPELESS,	// SWITCH6
DARK,		// SWITCHEROO
NORMAL,		// SWORDS_DANCE
GRASS,		// SYNTHESIS
NORMAL,		// TACKLE
BUG,		// TAIL_GLOW
NORMAL,		// TAIL_WHIP
FLYING,		// TAILWIND
NORMAL,		// TAKE_DOWN
DARK,		// TAUNT
NORMAL,		// TEETER_DANCE
PSYCHIC_TYPE,	// TELEPORT
DARK,		// THIEF
NORMAL,		// THRASH
ELECTRIC,	// THUNDER
ELECTRIC,	// THUNDER_FANG
ELECTRIC,	// THUNDER_WAVE
ELECTRIC,	// THUNDERBOLT
ELECTRIC,	// THUNDERPUNCH
ELECTRIC,	// THUNDERSHOCK
NORMAL,		// TICKLE
DARK,		// TORMENT
POISON,		// TOXIC
POISON,		// TOXIC_SPIKES
NORMAL,		// TRANSFORM
NORMAL,		// TRI_ATTACK
PSYCHIC_TYPE,	// TRICK
PSYCHIC_TYPE,	// TRICK_ROOM
FIGHTING,	// TRIPLE_KICK
NORMAL,		// TRUMP_CARD
BUG,		// TWINEEDLE
DRAGON,		// TWISTER
BUG,		// U_TURN
NORMAL,		// UPROAR
FIGHTING,	// VACUUM_WAVE
NORMAL,		// VICEGRIP
GRASS,		// VINE_WHIP
FIGHTING,	// VITAL_THROW
ELECTRIC,	// VOLT_TACKLE
FIGHTING,	// WAKE_UP_SLAP
WATER,		// WATER_GUN
WATER,		// WATER_PULSE
WATER,		// WATER_SPORT
WATER,		// WATER_SPOUT
WATER,		// WATERFALL
NORMAL,		// WEATHER_BALL
WATER,		// WHIRLPOOL
NORMAL,		// WHIRLWIND
FIRE,		// WILL_O_WISP
FLYING,		// WING_ATTACK
NORMAL,		// WISH
WATER,		// WITHDRAW
GRASS,		// WOOD_HAMMER
GRASS,		// WORRY_SEED
NORMAL,		// WRAP
NORMAL,		// WRING_OUT
BUG,		// X_SCISSOR
NORMAL,		// YAWN
ELECTRIC,	// ZAP_CANNON
PSYCHIC_TYPE		// ZEN_HEADBUTT
};

const bool is_physical [] = {
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
false,		// Switch1
false,		// Switch2
false,		// Switch3
false,		// Switch4
false,		// Switch5
false,		// Switch6
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

const short base_power [] = {
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
0,		// COUNTER
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
-1,		// DRAGON_RAGE
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
-1,		// ENDEAVOR
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
-1,		// FISSURE
0,		// FLAIL
60,		// FLAME_WHEEL
95,		// FLAMETHROWER
120,		// FLARE_BLITZ
0,		// FLASH
80,		// FLASH_CANNON
0,		// FLATTER
0,		// FLING
90,		// FLY
120,		// FOCUS_BLAST
0,		// FOCUS_ENERGY
150,		// FOCUS_PUNCH
0,		// FOLLOW_ME
60,		// FORCE_PALM
0,		// FORESIGHT
150,		// FRENZY_PLANT
0,		// FRUSTRATION
15,		// FURY_ATTACK
10,		// FURY_CUTTER
18,		// FURY_SWIPES
80,		// FUTURE_SIGHT
0,		// GASTRO_ACID
60,		// GIGA_DRAIN
150,		// GIGA_IMPACT
0,		// GLARE
0,		// GRASS_KNOT
0,		// GRASSWHISTLE
0,		// GRAVITY
0,		// GROWL
0,		// GROWTH
0,		// GRUDGE
0,		// GUARD_SWAP
-1,		// GUILLOTINE
120,		// GUNK_SHOT
40,		// GUST
0,		// GYRO_BALL
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
0,		// HIDDEN_POWER
65,		// HORN_ATTACK
-1,		// HORN_DRILL
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
0,		// LOW_KICK
0,		// LUCKY_CHANT
0,		// LUNAR_DANCE
70,		// LUSTER_PURGE
40,		// MACH_PUNCH
0,		// MAGIC_COAT
60,		// MAGICAL_LEAF
120,		// MAGMA_STORM
60,		// MAGNET_BOMB
0,		// MAGNET_RISE
0,		// MAGNITUDE
0,		// ME_FIRST
0,		// MEAN_LOOK
0,		// MEDITATE
40,		// MEGA_DRAIN
120,		// MEGA_KICK
80,		// MEGA_PUNCH
120,		// MEGAHORN
0,		// MEMENTO
0,		// METAL_BURST
50,		// METAL_CLAW
0,		// METAL_SOUND
100,		// METEOR_MASH
0,		// METRONOME_MOVE
0,		// MILK_DRINK
0,		// MIMIC
0,		// MIND_READER
0,		// MINIMIZE
0,		// MIRACLE_EYE
0,		// MIRROR_COAT
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
0,		// NATURAL_GIFT
0,		// NATURE_POWER
60,		// NEEDLE_ARM
-1,		// NIGHT_SHADE
70,		// NIGHT_SLASH
0,		// NIGHTMARE
65,		// OCTAZOOKA
0,		// ODOR_SLEUTH
60,		// OMINOUS_WIND
120,		// OUTRAGE
140,		// OVERHEAT
0,		// PAIN_SPLIT
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
0,		// PRESENT
0,		// PROTECT
65,		// PSYBEAM
0,		// PSYCH_UP
90,		// PSYCHIC_MOVE
140,		// PSYCHO_BOOST
70,		// PSYCHO_CUT
0,		// PSYCHO_SHIFT
-1,		// PSYWAVE
0,		// PUNISHMENT
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
0,		// RETURN
60,		// REVENGE
0,		// REVERSAL
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
-1,		// SEISMIC_TOSS
200,		// SELFDESTRUCT
80,		// SHADOW_BALL
70,		// SHADOW_CLAW
120,		// SHADOW_FORCE
60,		// SHADOW_PUNCH
40,		// SHADOW_SNEAK
0,		// SHARPEN
-1,		// SHEER_COLD
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
-1,		// SONICBOOM
100,		// SPACIAL_REND
65,		// SPARK
0,		// SPIDER_WEB
20,		// SPIKE_CANNON
0,		// SPIKES
0,		// SPIT_UP
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
-1,		// SUPER_FANG
120,		// SUPERPOWER
0,		// SUPERSONIC
95,		// SURF
0,		// SWAGGER
0,		// SWALLOW
0,		// SWEET_KISS
0,		// SWEET_SCENT
60,		// SWIFT
0,		// SWITCH1
0,		// SWITCH2
0,		// SWITCH3
0,		// SWITCH4
0,		// SWITCH5
0,		// SWITCH6
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
0,		// TRUMP_CARD
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

const char get_pp [] = {
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
-1,		// Struggle
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
-1,		// Switch1
-1,		// Switch2
-1,		// Switch3
-1,		// Switch4
-1,		// Switch5
-1,		// Switch6
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

const char get_probability [] = {		// Chance (out of 10) to activate side-effect
0,		// Absorb
1,		// Acid
0,		// Acid Armor
0,		// Acupressure
0,		// Aerial Ace
0,		// Aeroblast
0,		// Agility
0,		// Air Cutter
3,		// Air Slash
0,		// Amnesia
1,		// AncientPower
0,		// Aqua Jet
0,		// Aqua Ring
0,		// Aqua Tail
0,		// Arm Thrust
0,		// Aromatherapy
0,		// Assist
0,		// Assurance
3,		// Astonish
0,		// Attack Order
0,		// Attract
0,		// Aura Sphere
1,		// Aurora Beam
0,		// Avalanche
0,		// Barrage
0,		// Barrier
0,		// Baton Pass
0,		// Beat Up
0,		// Belly Drum
0,		// Bide
0,		// Bind
3,		// Bite
0,		// Blast Burn
1,		// Blaze Kick
1,		// Blizzard
0,		// Block
3,		// Body Slam
1,		// Bone Club
0,		// Bone Rush
0,		// Bonemerang
3,		// Bounce
0,		// Brave Bird
0,		// Brick Break
0,		// Brine
1,		// Bubble
1,		// BubbleBeam
0,		// Bug Bite
1,		// Bug Buzz
0,		// Bulk Up
0,		// Bullet Punch
0,		// Bullet Seed
0,		// Calm Mind
0,		// Camouflage
0,		// Captivate
0,		// Charge
7,		// Charge Beam
0,		// Charm
0,		// Chatter
0,		// Clamp
0,		// Close Combat
0,		// Comet Punch
0,		// Confuse Ray
1,		// Confusion
1,		// Constrict
0,		// Conversion
0,		// Conversion2
0,		// Copycat
0,		// Cosmic Power
0,		// Cotton Spore
0,		// Counter
0,		// Covet
0,		// Crabhammer
0,		// Cross Chop
1,		// Cross Poison
2,		// Crunch
5,		// Crush Claw
0,		// Crush Grip
0,		// Curse
0,		// Cut
2,		// Dark Pulse
0,		// Dark Void
0,		// Defend Order
0,		// Defense Curl
0,		// Defog
0,		// Destiny Bond
0,		// Detect
0,		// Dig
0,		// Disable
3,		// Discharge
0,		// Dive
2,		// Dizzy Punch
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
2,		// Dragon Rush
3,		// DragonBreath
0,		// Drain Punch
0,		// Dream Eater
0,		// Drill Peck
0,		// DynamicPunch
1,		// Earth Power
0,		// Earthquake
0,		// Egg Bomb
0,		// Embargo
1,		// Ember
0,		// Encore
0,		// Endeavor
0,		// Endure
1,		// Energy Ball
0,		// Eruption
0,		// Explosion
1,		// Extrasensory
0,		// ExtremeSpeed
0,		// Facade
0,		// Faint Attack
0,		// Fake Out
0,		// Fake Tears
0,		// False Swipe
0,		// FeatherDance
0,		// Feint
1,		// Fire Blast
1,		// Fire Fang
1,		// Fire Punch
0,		// Fire Spin
0,		// Fissure
0,		// Flail
1,		// Flame Wheel
1,		// Flamethrower
1,		// Flare Blitz
0,		// Flash
1,		// Flash Cannon
0,		// Flatter
0,		// Fling
0,		// Fly
1,		// Focus Blast
0,		// Focus Energy
0,		// Focus Punch
0,		// Follow Me
3,		// Force Palm
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
3,		// Gunk Shot
0,		// Gust
0,		// Gyro Ball
0,		// Hail
0,		// Hammer Arm
0,		// Harden
0,		// Haze
0,		// Head Smash
3,		// Headbutt
0,		// Heal Bell
0,		// Heal Block
0,		// Heal Order
0,		// Healing Wish
0,		// Heart Swap
1,		// Heat Wave
0,		// Helping Hand
0,		// Hi Jump Kick
0,		// Hidden Power
0,		// Horn Attack
0,		// Horn Drill
0,		// Howl
0,		// Hydro Cannon
0,		// Hydro Pump
0,		// Hyper Beam
1,		// Hyper Fang
0,		// Hyper Voice
0,		// Hypnosis
0,		// Ice Ball
1,		// Ice Beam
1,		// Ice Fang
1,		// Ice Punch
0,		// Ice Shard
0,		// Icicle Spear
0,		// Icy Wind
0,		// Imprison
0,		// Ingrain
0,		// Iron Defense
3,		// Iron Head
3,		// Iron Tail
0,		// Judgment
0,		// Jump Kick
0,		// Karate Chop
0,		// Kinesis
0,		// Knock Off
0,		// Last Resort
3,		// Lava Plume
0,		// Leaf Blade
0,		// Leaf Storm
0,		// Leech Life
0,		// Leech Seed
0,		// Leer
3,		// Lick
0,		// Light Screen
0,		// Lock-On
0,		// Lovely Kiss
0,		// Low Kick
0,		// Lucky Chant
0,		// Lunar Dance
5,		// Luster Purge
0,		// Mach Punch
0,		// Magic Coat
0,		// Magical Leaf
0,		// Magma Storm
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
1,		// Metal Claw
0,		// Metal Sound
2,		// Meteor Mash
0,		// Metronome
0,		// Milk Drink
0,		// Mimic
0,		// Mind Reader
0,		// Minimize
0,		// Miracle Eye
0,		// Mirror Coat
0,		// Mirror Move
3,		// Mirror Shot
0,		// Mist
5,		// Mist Ball
0,		// Moonlight
0,		// Morning Sun
3,		// Mud Bomb
0,		// Mud Shot
0,		// Mud Sport
0,		// Mud-Slap
3,		// Muddy Water
0,		// Nasty Plot
0,		// Natural Gift
0,		// Nature Power
3,		// Needle Arm
0,		// Night Shade
0,		// Night Slash
0,		// Nightmare
5,		// Octazooka
0,		// Odor Sleuth
1,		// Ominous Wind
0,		// Outrage
0,		// Overheat
0,		// Pain Split
0,		// Pay Day
0,		// Payback
0,		// Peck
0,		// Perish Song
0,		// Petal Dance
0,		// Pin Missile
0,		// Pluck
3,		// Poison Fang
0,		// Poison Gas
3,		// Poison Jab
3,		// Poison Sting
1,		// Poison Tail
0,		// PoisonPowder
0,		// Pound
1,		// Powder Snow
0,		// Power Gem
0,		// Power Swap
0,		// Power Trick
0,		// Power Whip
0,		// Present
0,		// Protect
1,		// Psybeam
0,		// Psych Up
1,		// Psychic
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
2,		// Rock Climb
0,		// Rock Polish
3,		// Rock Slide
5,		// Rock Smash
0,		// Rock Throw
0,		// Rock Tomb
0,		// Rock Wrecker
0,		// Role Play
3,		// Rolling Kick
0,		// Rollout
0,		// Roost
5,		// Sacred Fire
0,		// Safeguard
0,		// Sand Tomb
0,		// Sand-Attack
0,		// Sandstorm
0,		// Scary Face
0,		// Scratch
0,		// Screech
0,		// Secret Power
0,		// Seed Bomb
4,		// Seed Flare
0,		// Seismic Toss
0,		// Selfdestruct
2,		// Shadow Ball
0,		// Shadow Claw
0,		// Shadow Force
0,		// Shadow Punch
0,		// Shadow Sneak
0,		// Sharpen
0,		// Sheer Cold
0,		// Shock Wave
1,		// Signal Beam
1,		// Silver Wind
0,		// Sing
0,		// Sketch
0,		// Skill Swap
0,		// Skull Bash
3,		// Sky Attack
0,		// Sky Uppercut
0,		// Slack Off
0,		// Slam
0,		// Slash
0,		// Sleep Powder
0,		// Sleep Talk
3,		// Sludge
3,		// Sludge Bomb
0,		// SmellingSalt
4,		// Smog
0,		// SmokeScreen
0,		// Snatch
3,		// Snore
0,		// Softboiled
0,		// SolarBeam
0,		// SonicBoom
0,		// Spacial Rend
3,		// Spark
0,		// Spider Web
0,		// Spike Cannon
0,		// Spikes
0,		// Spit Up
0,		// Spite
0,		// Splash
0,		// Spore
0,		// Stealth Rock
1,		// Steel Wing
0,		// Stockpile
3,		// Stomp
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
0,		// Switch1
0,		// Switch2
0,		// Switch3
0,		// Switch4
0,		// Switch5
0,		// Switch6
0,		// Switcheroo
0,		// Swords Dance
0,		// Synthesis
0,		// Tackle
0,		// Tail Glow
0,		// Tail Whip
0,		// Tailwind
0,		// Take Down
0,		// Taunt
0,		// Teeter Dance
0,		// Teleport
0,		// Thief
0,		// Thrash
3,		// Thunder
1,		// Thunder Fang
0,		// Thunder Wave
1,		// Thunderbolt
1,		// ThunderPunch
1,		// ThunderShock
0,		// Tickle
0,		// Torment
0,		// Toxic
0,		// Toxic Spikes
0,		// Transform
2,		// Tri Attack
0,		// Trick
0,		// Trick Room
0,		// Triple Kick
0,		// Trump Card
2,		// Twineedle
2,		// Twister
0,		// U-turn
0,		// Uproar
0,		// Vacuum Wave
0,		// ViceGrip
0,		// Vine Whip
0,		// Vital Throw
1,		// Volt Tackle
0,		// Wake-Up Slap
0,		// Water Gun
2,		// Water Pulse
0,		// Water Sport
0,		// Water Spout
2,		// Waterfall
0,		// Weather Ball
0,		// Whirlpool
0,		// Whirlwind
0,		// Will-O-Wisp
0,		// Wing Attack
0,		// Wish
0,		// Withdraw
0,		// Wood Hammer
0,		// Worry Seed
0,		// Wrap
0,		// Wring Out
0,		// X-Scissor
0,		// Yawn
0,		// Zap Cannon
2		// Zen Headbutt
};

const std::string move_name [] = { "Absorb", "Acid", "Acid Armor", "Acupressure", "Aerial Ace", "Aeroblast", "Agility", "Air Cutter", "Air Slash", "Amnesia", "AncientPower", "Aqua Jet", "Aqua Ring", "Aqua Tail", "Arm Thrust", "Aromatherapy", "Assist", "Assurance", "Astonish", "Attack Order", "Attract", "Aura Sphere", "Aurora Beam", "Avalanche", "Barrage", "Barrier", "Baton Pass", "Beat Up", "Belly Drum", "Bide", "Bind", "Bite", "Blast Burn", "Blaze Kick", "Blizzard", "Block", "Body Slam", "Bone Club", "Bone Rush", "Bonemerang", "Bounce", "Brave Bird", "Brick Break", "Brine", "Bubble", "BubbleBeam", "Bug Bite", "Bug Buzz", "Bulk Up", "Bullet Punch", "Bullet Seed", "Calm Mind", "Camouflage", "Captivate", "Charge", "Charge Beam", "Charm", "Chatter", "Clamp", "Close Combat", "Comet Punch", "Confuse Ray", "Confusion", "Constrict", "Conversion", "Conversion2", "Copycat", "Cosmic Power", "Cotton Spore", "Counter", "Covet", "Crabhammer", "Cross Chop", "Cross Poison", "Crunch", "Crush Claw", "Crush Grip", "Curse", "Cut", "Dark Pulse", "Dark Void", "Defend Order", "Defense Curl", "Defog", "Destiny Bond", "Detect", "Dig", "Disable", "Discharge", "Dive", "Dizzy Punch", "Doom Desire", "Double Hit", "Double Kick", "Double Team", "Double-Edge", "DoubleSlap", "Draco Meteor", "Dragon Claw", "Dragon Dance", "Dragon Pulse", "Dragon Rage", "Dragon Rush", "DragonBreath", "Drain Punch", "Dream Eater", "Drill Peck", "DynamicPunch", "Earth Power", "Earthquake", "Egg Bomb", "Embargo", "Ember", "Encore", "Endeavor", "Endure", "Energy Ball", "Eruption", "Explosion", "Extrasensory", "ExtremeSpeed", "Facade", "Faint Attack", "Fake Out", "Fake Tears", "False Swipe", "FeatherDance", "Feint", "Fire Blast", "Fire Fang", "Fire Punch", "Fire Spin", "Fissure", "Flail", "Flame Wheel", "Flamethrower", "Flare Blitz", "Flash", "Flash Cannon", "Flatter", "Fling", "Fly", "Focus Blast", "Focus Energy", "Focus Punch", "Follow Me", "Force Palm", "Foresight", "Frenzy Plant", "Frustration", "Fury Attack", "Fury Cutter", "Fury Swipes", "Future Sight", "Gastro Acid", "Giga Drain", "Giga Impact", "Glare", "Grass Knot", "GrassWhistle", "Gravity", "Growl", "Growth", "Grudge", "Guard Swap", "Guillotine", "Gunk Shot", "Gust", "Gyro Ball", "Hail", "Hammer Arm", "Harden", "Haze", "Head Smash", "Headbutt", "Heal Bell", "Heal Block", "Heal Order", "Healing Wish", "Heart Swap", "Heat Wave", "Helping Hand", "Hi Jump Kick", "Hidden Power", "Horn Attack", "Horn Drill", "Howl", "Hydro Cannon", "Hydro Pump", "Hyper Beam", "Hyper Fang", "Hyper Voice", "Hypnosis", "Ice Ball", "Ice Beam", "Ice Fang", "Ice Punch", "Ice Shard", "Icicle Spear", "Icy Wind", "Imprison", "Ingrain", "Iron Defense", "Iron Head", "Iron Tail", "Judgment", "Jump Kick", "Karate Chop", "Kinesis", "Knock Off", "Last Resort", "Lava Plume", "Leaf Blade", "Leaf Storm", "Leech Life", "Leech Seed", "Leer", "Lick", "Light Screen", "Lock-On", "Lovely Kiss", "Low Kick", "Lucky Chant", "Lunar Dance", "Luster Purge", "Mach Punch", "Magic Coat", "Magical Leaf", "Magma Storm", "Magnet Bomb", "Magnet Rise", "Magnitude", "Me First", "Mean Look", "Meditate", "Mega Drain", "Mega Kick", "Mega Punch", "Megahorn", "Memento", "Metal Burst", "Metal Claw", "Metal Sound", "Meteor Mash", "Metronome", "Milk Drink", "Mimic", "Mind Reader", "Minimize", "Miracle Eye", "Mirror Coat", "Mirror Move", "Mirror Shot", "Mist", "Mist Ball", "Moonlight", "Morning Sun", "Mud Bomb", "Mud Shot", "Mud Sport", "Mud-Slap", "Muddy Water", "Nasty Plot", "Natural Gift", "Nature Power", "Needle Arm", "Night Shade", "Night Slash", "Nightmare", "Octazooka", "Odor Sleuth", "Ominous Wind", "Outrage", "Overheat", "Pain Split", "Pay Day", "Payback", "Peck", "Perish Song", "Petal Dance", "Pin Missile", "Pluck", "Poison Fang", "Poison Gas", "Poison Jab", "Poison Sting", "Poison Tail", "PoisonPowder", "Pound", "Powder Snow", "Power Gem", "Power Swap", "Power Trick", "Power Whip", "Present", "Protect", "Psybeam", "Psych Up", "Psychic", "Psycho Boost", "Psycho Cut", "Psycho Shift", "Psywave", "Punishment", "Pursuit", "Quick Attack", "Rage", "Rain Dance", "Rapid Spin", "Razor Leaf", "Razor Wind", "Recover", "Recycle", "Reflect", "Refresh", "Rest", "Return", "Revenge", "Reversal", "Roar", "Roar Of Time", "Rock Blast", "Rock Climb", "Rock Polish", "Rock Slide", "Rock Smash", "Rock Throw", "Rock Tomb", "Rock Wrecker", "Role Play", "Rolling Kick", "Rollout", "Roost", "Sacred Fire", "Safeguard", "Sand Tomb", "Sand-Attack", "Sandstorm", "Scary Face", "Scratch", "Screech", "Secret Power", "Seed Bomb", "Seed Flare", "Seismic Toss", "Selfdestruct", "Shadow Ball", "Shadow Claw", "Shadow Force", "Shadow Punch", "Shadow Sneak", "Sharpen", "Sheer Cold", "Shock Wave", "Signal Beam", "Silver Wind", "Sing", "Sketch", "Skill Swap", "Skull Bash", "Sky Attack", "Sky Uppercut", "Slack Off", "Slam", "Slash", "Sleep Powder", "Sleep Talk", "Sludge", "Sludge Bomb", "SmellingSalt", "Smog", "SmokeScreen", "Snatch", "Snore", "Softboiled", "SolarBeam", "SonicBoom", "Spacial Rend", "Spark", "Spider Web", "Spike Cannon", "Spikes", "Spit Up", "Spite", "Splash", "Spore", "Stealth Rock", "Steel Wing", "Stockpile", "Stomp", "Stone Edge", "Strength", "String Shot", "Struggle", "Stun Spore", "Submission", "Substitute", "Sucker Punch", "Sunny Day", "Super Fang", "Superpower", "Supersonic", "Surf", "Swagger", "Swallow", "Sweet Kiss", "Sweet Scent", "Swift", "Switch1", "Switch2", "Switch3", "Switch4", "Switch5", "Switch6", "Switcheroo", "Swords Dance", "Synthesis", "Tackle", "Tail Glow", "Tail Whip", "Tailwind", "Take Down", "Taunt", "Teeter Dance", "Teleport", "Thief", "Thrash", "Thunder", "Thunder Fang", "Thunder Wave", "Thunderbolt", "ThunderPunch", "ThunderShock", "Tickle", "Torment", "Toxic", "Toxic Spikes", "Transform", "Tri Attack", "Trick", "Trick Room", "Triple Kick", "Trump Card", "Twineedle", "Twister", "U-turn", "Uproar", "Vacuum Wave", "ViceGrip", "Vine Whip", "Vital Throw", "Volt Tackle", "Wake-Up Slap", "Water Gun", "Water Pulse", "Water Sport", "Water Spout", "Waterfall", "Weather Ball", "Whirlpool", "Whirlwind", "Will-O-Wisp", "Wing Attack", "Wish", "Withdraw", "Wood Hammer", "Worry Seed", "Wrap", "Wring Out", "X-Scissor", "Yawn", "Zap Cannon", "Zen Headbutt", "End Move" };

}
#endif
