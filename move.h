// Moves data structure
// Copyright 2010 David Stone
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

#include <map>
#include "type.h"

// I use PSYCHIC_MOVE (along with PSYCHIC_TYPE) instead of just making one of them PSYCHIC and the other PSYCHIC_WHATEVER so that I will never get confused as to what to type. Attempting to use PSYCHIC will create an error on compile, allowing me to fix it. Same for METRONOME with the move vs. item, and the HAIL move vs. weather.

enum moves_list { ABSORB, ACID, ACID_ARMOR, ACUPRESSURE, AERIAL_ACE, AEROBLAST, AGILITY, AIR_CUTTER, AIR_SLASH, AMNESIA, ANCIENTPOWER, AQUA_JET, AQUA_RING, AQUA_TAIL, ARM_THRUST, AROMATHERAPY, ASSIST, ASSURANCE, ASTONISH, ATTACK_ORDER, ATTRACT, AURA_SPHERE, AURORA_BEAM, AVALANCHE, BARRAGE, BARRIER, BATON_PASS, BEAT_UP, BELLY_DRUM, BIDE, BIND, BITE, BLAST_BURN, BLAZE_KICK, BLIZZARD, BLOCK, BODY_SLAM, BONE_CLUB, BONE_RUSH, BONEMERANG, BOUNCE, BRAVE_BIRD, BRICK_BREAK, BRINE, BUBBLE, BUBBLEBEAM, BUG_BITE, BUG_BUZZ, BULK_UP, BULLET_PUNCH, BULLET_SEED, CALM_MIND, CAMOUFLAGE, CAPTIVATE, CHARGE, CHARGE_BEAM, CHARM, CHATTER, CLAMP, CLOSE_COMBAT, COMET_PUNCH, CONFUSE_RAY, CONFUSION, CONSTRICT, CONVERSION, CONVERSION2, COPYCAT, COSMIC_POWER, COTTON_SPORE, COUNTER, COVET, CRABHAMMER, CROSS_CHOP, CROSS_POISON, CRUNCH, CRUSH_CLAW, CRUSH_GRIP, CURSE, CUT, DARK_PULSE, DARK_VOID, DEFEND_ORDER, DEFENSE_CURL, DEFOG, DESTINY_BOND, DETECT, DIG, DISABLE, DISCHARGE, DIVE, DIZZY_PUNCH, DOOM_DESIRE, DOUBLE_HIT, DOUBLE_KICK, DOUBLE_TEAM, DOUBLE_EDGE, DOUBLESLAP, DRACO_METEOR, DRAGON_CLAW, DRAGON_DANCE, DRAGON_PULSE, DRAGON_RAGE, DRAGON_RUSH, DRAGONBREATH, DRAIN_PUNCH, DREAM_EATER, DRILL_PECK, DYNAMICPUNCH, EARTH_POWER, EARTHQUAKE, EGG_BOMB, EMBARGO, EMBER, ENCORE, ENDEAVOR, ENDURE, ENERGY_BALL, ERUPTION, EXPLOSION, EXTRASENSORY, EXTREMESPEED, FACADE, FAINT_ATTACK, FAKE_OUT, FAKE_TEARS, FALSE_SWIPE, FEATHERDANCE, FEINT, FIRE_BLAST, FIRE_FANG, FIRE_PUNCH, FIRE_SPIN, FISSURE, FLAIL, FLAME_WHEEL, FLAMETHROWER, FLARE_BLITZ, FLASH, FLASH_CANNON, FLATTER, FLING, FLY, FOCUS_BLAST, FOCUS_ENERGY, FOCUS_PUNCH, FOLLOW_ME, FORCE_PALM, FORESIGHT, FRENZY_PLANT, FRUSTRATION, FURY_ATTACK, FURY_CUTTER, FURY_SWIPES, FUTURE_SIGHT, GASTRO_ACID, GIGA_DRAIN, GIGA_IMPACT, GLARE, GRASS_KNOT, GRASSWHISTLE, GRAVITY, GROWL, GROWTH, GRUDGE, GUARD_SWAP, GUILLOTINE, GUNK_SHOT, GUST, GYRO_BALL, HAIL_MOVE, HAMMER_ARM, HARDEN, HAZE, HEAD_SMASH, HEADBUTT, HEAL_BELL, HEAL_BLOCK, HEAL_ORDER, HEALING_WISH, HEART_SWAP, HEAT_WAVE, HELPING_HAND, HI_JUMP_KICK, HIDDEN_POWER, HORN_ATTACK, HORN_DRILL, HOWL, HYDRO_CANNON, HYDRO_PUMP, HYPER_BEAM, HYPER_FANG, HYPER_VOICE, HYPNOSIS, ICE_BALL, ICE_BEAM, ICE_FANG, ICE_PUNCH, ICE_SHARD, ICICLE_SPEAR, ICY_WIND, IMPRISON, INGRAIN, IRON_DEFENSE, IRON_HEAD, IRON_TAIL, JUDGMENT, JUMP_KICK, KARATE_CHOP, KINESIS, KNOCK_OFF, LAST_RESORT, LAVA_PLUME, LEAF_BLADE, LEAF_STORM, LEECH_LIFE, LEECH_SEED, LEER, LICK, LIGHT_SCREEN, LOCK_ON, LOVELY_KISS, LOW_KICK, LUCKY_CHANT, LUNAR_DANCE, LUSTER_PURGE, MACH_PUNCH, MAGIC_COAT, MAGICAL_LEAF, MAGMA_STORM, MAGNET_BOMB, MAGNET_RISE, MAGNITUDE, ME_FIRST, MEAN_LOOK, MEDITATE, MEGA_DRAIN, MEGA_KICK, MEGA_PUNCH, MEGAHORN, MEMENTO, METAL_BURST, METAL_CLAW, METAL_SOUND, METEOR_MASH, METRONOME_MOVE, MILK_DRINK, MIMIC, MIND_READER, MINIMIZE, MIRACLE_EYE, MIRROR_COAT, MIRROR_MOVE, MIRROR_SHOT, MIST, MIST_BALL, MOONLIGHT, MORNING_SUN, MUD_BOMB, MUD_SHOT, MUD_SPORT, MUD_SLAP, MUDDY_WATER, NASTY_PLOT, NATURAL_GIFT, NATURE_POWER, NEEDLE_ARM, NIGHT_SHADE, NIGHT_SLASH, NIGHTMARE, OCTAZOOKA, ODOR_SLEUTH, OMINOUS_WIND, OUTRAGE, OVERHEAT, PAIN_SPLIT, PAY_DAY, PAYBACK, PECK, PERISH_SONG, PETAL_DANCE, PIN_MISSILE, PLUCK, POISON_FANG, POISON_GAS, POISON_JAB, POISON_STING, POISON_TAIL, POISONPOWDER, POUND, POWDER_SNOW, POWER_GEM, POWER_SWAP, POWER_TRICK, POWER_WHIP, PRESENT, PROTECT, PSYBEAM, PSYCH_UP, PSYCHIC_MOVE, PSYCHO_BOOST, PSYCHO_CUT, PSYCHO_SHIFT, PSYWAVE, PUNISHMENT, PURSUIT, QUICK_ATTACK, RAGE, RAIN_DANCE, RAPID_SPIN, RAZOR_LEAF, RAZOR_WIND, RECOVER, RECYCLE, REFLECT, REFRESH, REST, RETURN, REVENGE, REVERSAL, ROAR, ROAR_OF_TIME, ROCK_BLAST, ROCK_CLIMB, ROCK_POLISH, ROCK_SLIDE, ROCK_SMASH, ROCK_THROW, ROCK_TOMB, ROCK_WRECKER, ROLE_PLAY, ROLLING_KICK, ROLLOUT, ROOST, SACRED_FIRE, SAFEGUARD, SAND_TOMB, SAND_ATTACK, SANDSTORM, SCARY_FACE, SCRATCH, SCREECH, SECRET_POWER, SEED_BOMB, SEED_FLARE, SEISMIC_TOSS, SELFDESTRUCT, SHADOW_BALL, SHADOW_CLAW, SHADOW_FORCE, SHADOW_PUNCH, SHADOW_SNEAK, SHARPEN, SHEER_COLD, SHOCK_WAVE, SIGNAL_BEAM, SILVER_WIND, SING, SKETCH, SKILL_SWAP, SKULL_BASH, SKY_ATTACK, SKY_UPPERCUT, SLACK_OFF, SLAM, SLASH, SLEEP_POWDER, SLEEP_TALK, SLUDGE, SLUDGE_BOMB, SMELLINGSALT, SMOG, SMOKESCREEN, SNATCH, SNORE, SOFTBOILED, SOLARBEAM, SONICBOOM, SPACIAL_REND, SPARK, SPIDER_WEB, SPIKE_CANNON, SPIKES, SPIT_UP, SPITE, SPLASH, SPORE, STEALTH_ROCK, STEEL_WING, STOCKPILE, STOMP, STONE_EDGE, STRENGTH, STRING_SHOT, STRUGGLE, STUN_SPORE, SUBMISSION, SUBSTITUTE, SUCKER_PUNCH, SUNNY_DAY, SUPER_FANG, SUPERPOWER, SUPERSONIC, SURF, SWAGGER, SWALLOW, SWEET_KISS, SWEET_SCENT, SWIFT, SWITCHEROO, SWORDS_DANCE, SYNTHESIS, TACKLE, TAIL_GLOW, TAIL_WHIP, TAILWIND, TAKE_DOWN, TAUNT, TEETER_DANCE, TELEPORT, THIEF, THRASH, THUNDER, THUNDER_FANG, THUNDER_WAVE, THUNDERBOLT, THUNDERPUNCH, THUNDERSHOCK, TICKLE, TORMENT, TOXIC, TOXIC_SPIKES, TRANSFORM, TRI_ATTACK, TRICK, TRICK_ROOM, TRIPLE_KICK, TRUMP_CARD, TWINEEDLE, TWISTER, U_TURN, UPROAR, VACUUM_WAVE, VICEGRIP, VINE_WHIP, VITAL_THROW, VOLT_TACKLE, WAKE_UP_SLAP, WATER_GUN, WATER_PULSE, WATER_SPORT, WATER_SPOUT, WATERFALL, WEATHER_BALL, WHIRLPOOL, WHIRLWIND, WILL_O_WISP, WING_ATTACK, WISH, WITHDRAW, WOOD_HAMMER, WORRY_SEED, WRAP, WRING_OUT, X_SCISSOR, YAWN, ZAP_CANNON, ZEN_HEADBUTT };

// Various states a Pokemon can be in due to vanishing moves.

enum Vanish { LANDED, BOUNCED, DUG, DIVED, FLOWN, SHADOW_FORCED };

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
	char times_used;
	char variable;		// Used for moves of variable power. Magnitude = 4-10, Psywave = 5-15, Present = 0-4 (0 = heal)
};

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
NORMAL,		// DOUBLE_EDGE
NORMAL,		// DOUBLE_HIT
FIGHTING,	// DOUBLE_KICK
NORMAL,		// DOUBLE_TEAM
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
ICE,		// HAIL_MOVE
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
NORMAL,		// STRUGGLE
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

void move_priority (moves &move) {
	if (move.name == HELPING_HAND)
		move.priority = 5;
	else if (move.name == MAGIC_COAT or move.name == SNATCH)
		move.priority = 4;
	else if (move.name == DETECT or move.name == ENDURE or move.name == FOLLOW_ME or move.name == PROTECT)
		move.priority = 3;
	else if (move.name ==  FEINT)
		move.priority = 2;
	else if (move.name == AQUA_JET or move.name == BIDE or move.name == BULLET_PUNCH or move.name == EXTREMESPEED or move.name == FAKE_OUT or move.name == ICE_SHARD or move.name == MACH_PUNCH or move.name == QUICK_ATTACK or move.name == SHADOW_SNEAK or move.name == SUCKER_PUNCH or move.name == VACUUM_WAVE)
		move.priority = 1;
	else if (move.name == VITAL_THROW)
		move.priority = -1;
	else if (move.name == FOCUS_PUNCH)
		move.priority = -2;
	else if (move.name == AVALANCHE or move.name == REVENGE)
		move.priority = -3;
	else if (move.name == COUNTER or move.name == MIRROR_COAT)
		move.priority = -4;
	else if (move.name == ROAR  or move.name == WHIRLWIND)
		move.priority = -5;
	else if (move.name == TRICK_ROOM)
		move.priority = -6;
	else
		move.priority = 0;
}

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
0,		// CRUSH_GRIP	
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
0,		// DRAGON_RAGE	
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
0,		// ENDEAVOR	
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
0,		// FISSURE	
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
0,		// GUILLOTINE	
120,		// GUNK_SHOT	
40,		// GUST	
0,		// GYRO_BALL	
0,		// HAIL_MOVE	
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
0,		// HORN_DRILL	
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
0,		// NIGHT_SHADE	
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
0,		// PSYWAVE	
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
0,		// SEISMIC_TOSS	
200,		// SELFDESTRUCT	
80,		// SHADOW_BALL	
70,		// SHADOW_CLAW	
120,		// SHADOW_FORCE	
60,		// SHADOW_PUNCH	
40,		// SHADOW_SNEAK	
0,		// SHARPEN	
0,		// SHEER_COLD	
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
0,		// SONICBOOM	
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
0,		// SUPER_FANG	
120,		// SUPERPOWER	
0,		// SUPERSONIC	
95,		// SURF	
0,		// SWAGGER	
0,		// SWALLOW	
0,		// SWEET_KISS	
0,		// SWEET_SCENT	
60,		// SWIFT	
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
0,		// WRING_OUT	
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
1,		// Struggle 
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

const std::string move_name [] = { "Absorb", "Acid", "Acid Armor", "Acupressure", "Aerial Ace", "Aeroblast", "Agility", "Air Cutter", "Air Slash", "Amnesia", "AncientPower", "Aqua Jet", "Aqua Ring", "Aqua Tail", "Arm Thrust", "Aromatherapy", "Assist", "Assurance", "Astonish", "Attack Order", "Attract", "Aura Sphere", "Aurora Beam", "Avalanche", "Barrage", "Barrier", "Baton Pass", "Beat Up", "Belly Drum", "Bide", "Bind", "Bite", "Blast Burn", "Blaze Kick", "Blizzard", "Block", "Body Slam", "Bone Club", "Bone Rush", "Bonemerang", "Bounce", "Brave Bird", "Brick Break", "Brine", "Bubble", "BubbleBeam", "Bug Bite", "Bug Buzz", "Bulk Up", "Bullet Punch", "Bullet Seed", "Calm Mind", "Camouflage", "Captivate", "Charge", "Charge Beam", "Charm", "Chatter", "Clamp", "Close Combat", "Comet Punch", "Confuse Ray", "Confusion", "Constrict", "Conversion", "Conversion2", "Copycat", "Cosmic Power", "Cotton Spore", "Counter", "Covet", "Crabhammer", "Cross Chop", "Cross Poison", "Crunch", "Crush Claw", "Crush Grip", "Curse", "Cut", "Dark Pulse", "Dark Void", "Defend Order", "Defense Curl", "Defog", "Destiny Bond", "Detect", "Dig", "Disable", "Discharge", "Dive", "Dizzy Punch", "Doom Desire", "Double Hit", "Double Kick", "Double Team", "Double-Edge", "DoubleSlap", "Draco Meteor", "Dragon Claw", "Dragon Dance", "Dragon Pulse", "Dragon Rage", "Dragon Rush", "DragonBreath", "Drain Punch", "Dream Eater", "Drill Peck", "DynamicPunch", "Earth Power", "Earthquake", "Egg Bomb", "Embargo", "Ember", "Encore", "Endeavor", "Endure", "Energy Ball", "Eruption", "Explosion", "Extrasensory", "ExtremeSpeed", "Facade", "Faint Attack", "Fake Out", "Fake Tears", "False Swipe", "FeatherDance", "Feint", "Fire Blast", "Fire Fang", "Fire Punch", "Fire Spin", "Fissure", "Flail", "Flame Wheel", "Flamethrower", "Flare Blitz", "Flash", "Flash Cannon", "Flatter", "Fling", "Fly", "Focus Blast", "Focus Energy", "Focus Punch", "Follow Me", "Force Palm", "Foresight", "Frenzy Plant", "Frustration", "Fury Attack", "Fury Cutter", "Fury Swipes", "Future Sight", "Gastro Acid", "Giga Drain", "Giga Impact", "Glare", "Grass Knot", "GrassWhistle", "Gravity", "Growl", "Growth", "Grudge", "Guard Swap", "Guillotine", "Gunk Shot", "Gust", "Gyro Ball", "Hail", "Hammer Arm", "Harden", "Haze", "Head Smash", "Headbutt", "Heal Bell", "Heal Block", "Heal Order", "Healing Wish", "Heart Swap", "Heat Wave", "Helping Hand", "Hi Jump Kick", "Hidden Power", "Horn Attack", "Horn Drill", "Howl", "Hydro Cannon", "Hydro Pump", "Hyper Beam", "Hyper Fang", "Hyper Voice", "Hypnosis", "Ice Ball", "Ice Beam", "Ice Fang", "Ice Punch", "Ice Shard", "Icicle Spear", "Icy Wind", "Imprison", "Ingrain", "Iron Defense", "Iron Head", "Iron Tail", "Judgment", "Jump Kick", "Karate Chop", "Kinesis", "Knock Off", "Last Resort", "Lava Plume", "Leaf Blade", "Leaf Storm", "Leech Life", "Leech Seed", "Leer", "Lick", "Light Screen", "Lock-On", "Lovely Kiss", "Low Kick", "Lucky Chant", "Lunar Dance", "Luster Purge", "Mach Punch", "Magic Coat", "Magical Leaf", "Magma Storm", "Magnet Bomb", "Magnet Rise", "Magnitude", "Me First", "Mean Look", "Meditate", "Mega Drain", "Mega Kick", "Mega Punch", "Megahorn", "Memento", "Metal Burst", "Metal Claw", "Metal Sound", "Meteor Mash", "Metronome", "Milk Drink", "Mimic", "Mind Reader", "Minimize", "Miracle Eye", "Mirror Coat", "Mirror Move", "Mirror Shot", "Mist", "Mist Ball", "Moonlight", "Morning Sun", "Mud Bomb", "Mud Shot", "Mud Sport", "Mud-Slap", "Muddy Water", "Nasty Plot", "Natural Gift", "Nature Power", "Needle Arm", "Night Shade", "Night Slash", "Nightmare", "Octazooka", "Odor Sleuth", "Ominous Wind", "Outrage", "Overheat", "Pain Split", "Pay Day", "Payback", "Peck", "Perish Song", "Petal Dance", "Pin Missile", "Pluck", "Poison Fang", "Poison Gas", "Poison Jab", "Poison Sting", "Poison Tail", "PoisonPowder", "Pound", "Powder Snow", "Power Gem", "Power Swap", "Power Trick", "Power Whip", "Present", "Protect", "Psybeam", "Psych Up", "Psychic", "Psycho Boost", "Psycho Cut", "Psycho Shift", "Psywave", "Punishment", "Pursuit", "Quick Attack", "Rage", "Rain Dance", "Rapid Spin", "Razor Leaf", "Razor Wind", "Recover", "Recycle", "Reflect", "Refresh", "Rest", "Return", "Revenge", "Reversal", "Roar", "Roar Of Time", "Rock Blast", "Rock Climb", "Rock Polish", "Rock Slide", "Rock Smash", "Rock Throw", "Rock Tomb", "Rock Wrecker", "Role Play", "Rolling Kick", "Rollout", "Roost", "Sacred Fire", "Safeguard", "Sand Tomb", "Sand-Attack", "Sandstorm", "Scary Face", "Scratch", "Screech", "Secret Power", "Seed Bomb", "Seed Flare", "Seismic Toss", "Selfdestruct", "Shadow Ball", "Shadow Claw", "Shadow Force", "Shadow Punch", "Shadow Sneak", "Sharpen", "Sheer Cold", "Shock Wave", "Signal Beam", "Silver Wind", "Sing", "Sketch", "Skill Swap", "Skull Bash", "Sky Attack", "Sky Uppercut", "Slack Off", "Slam", "Slash", "Sleep Powder", "Sleep Talk", "Sludge", "Sludge Bomb", "SmellingSalt", "Smog", "SmokeScreen", "Snatch", "Snore", "Softboiled", "SolarBeam", "SonicBoom", "Spacial Rend", "Spark", "Spider Web", "Spike Cannon", "Spikes", "Spit Up", "Spite", "Splash", "Spore", "Stealth Rock", "Steel Wing", "Stockpile", "Stomp", "Stone Edge", "Strength", "String Shot", "Struggle", "Stun Spore", "Submission", "Substitute", "Sucker Punch", "Sunny Day", "Super Fang", "Superpower", "Supersonic", "Surf", "Swagger", "Swallow", "Sweet Kiss", "Sweet Scent", "Swift", "Switcheroo", "Swords Dance", "Synthesis", "Tackle", "Tail Glow", "Tail Whip", "Tailwind", "Take Down", "Taunt", "Teeter Dance", "Teleport", "Thief", "Thrash", "Thunder", "Thunder Fang", "Thunder Wave", "Thunderbolt", "ThunderPunch", "ThunderShock", "Tickle", "Torment", "Toxic", "Toxic Spikes", "Transform", "Tri Attack", "Trick", "Trick Room", "Triple Kick", "Trump Card", "Twineedle", "Twister", "U-turn", "Uproar", "Vacuum Wave", "ViceGrip", "Vine Whip", "Vital Throw", "Volt Tackle", "Wake-Up Slap", "Water Gun", "Water Pulse", "Water Sport", "Water Spout", "Waterfall", "Weather Ball", "Whirlpool", "Whirlwind", "Will-O-Wisp", "Wing Attack", "Wish", "Withdraw", "Wood Hammer", "Worry Seed", "Wrap", "Wring Out", "X-Scissor", "Yawn", "Zap Cannon", "Zen Headbutt" };

void set_move_map (std::map <std::string, moves_list> &moves_map) {
	moves_map["Absorb"] = ABSORB;
	moves_map["Acid"] = ACID;
	moves_map["Acid Armor"] = ACID_ARMOR;
	moves_map["Acupressure"] = ACUPRESSURE;
	moves_map["Aerial Ace"] = AERIAL_ACE;
	moves_map["Aeroblast"] = AEROBLAST;
	moves_map["Agility"] = AGILITY;
	moves_map["Air Cutter"] = AIR_CUTTER;
	moves_map["Air Slash"] = AIR_SLASH;
	moves_map["Amnesia"] = AMNESIA;
	moves_map["AncientPower"] = ANCIENTPOWER;
	moves_map["Aqua Jet"] = AQUA_JET;
	moves_map["Aqua Ring"] = AQUA_RING;
	moves_map["Aqua Tail"] = AQUA_TAIL;
	moves_map["Arm Thrust"] = ARM_THRUST;
	moves_map["Aromatherapy"] = AROMATHERAPY;
	moves_map["Assist"] = ASSIST;
	moves_map["Assurance"] = ASSURANCE;
	moves_map["Astonish"] = ASTONISH;
	moves_map["Attack Order"] = ATTACK_ORDER;
	moves_map["Attract"] = ATTRACT;
	moves_map["Aura Sphere"] = AURA_SPHERE;
	moves_map["Aurora Beam"] = AURORA_BEAM;
	moves_map["Avalanche"] = AVALANCHE;
	moves_map["Barrage"] = BARRAGE;
	moves_map["Barrier"] = BARRIER;
	moves_map["Baton Pass"] = BATON_PASS;
	moves_map["Beat Up"] = BEAT_UP;
	moves_map["Belly Drum"] = BELLY_DRUM;
	moves_map["Bide"] = BIDE;
	moves_map["Bind"] = BIND;
	moves_map["Bite"] = BITE;
	moves_map["Blast Burn"] = BLAST_BURN;
	moves_map["Blaze Kick"] = BLAZE_KICK;
	moves_map["Blizzard"] = BLIZZARD;
	moves_map["Block"] = BLOCK;
	moves_map["Body Slam"] = BODY_SLAM;
	moves_map["Bone Club"] = BONE_CLUB;
	moves_map["Bone Rush"] = BONE_RUSH;
	moves_map["Bonemerang"] = BONEMERANG;
	moves_map["Bounce"] = BOUNCE;
	moves_map["Brave Bird"] = BRAVE_BIRD;
	moves_map["Brick Break"] = BRICK_BREAK;
	moves_map["Brine"] = BRINE;
	moves_map["Bubble"] = BUBBLE;
	moves_map["BubbleBeam"] = BUBBLEBEAM;
	moves_map["Bug Bite"] = BUG_BITE;
	moves_map["Bug Buzz"] = BUG_BUZZ;
	moves_map["Bulk Up"] = BULK_UP;
	moves_map["Bullet Punch"] = BULLET_PUNCH;
	moves_map["Bullet Seed"] = BULLET_SEED;
	moves_map["Calm Mind"] = CALM_MIND;
	moves_map["Camouflage"] = CAMOUFLAGE;
	moves_map["Captivate"] = CAPTIVATE;
	moves_map["Charge"] = CHARGE;
	moves_map["Charge Beam"] = CHARGE_BEAM;
	moves_map["Charm"] = CHARM;
	moves_map["Chatter"] = CHATTER;
	moves_map["Clamp"] = CLAMP;
	moves_map["Close Combat"] = CLOSE_COMBAT;
	moves_map["Comet Punch"] = COMET_PUNCH;
	moves_map["Confuse Ray"] = CONFUSE_RAY;
	moves_map["Confusion"] = CONFUSION;
	moves_map["Constrict"] = CONSTRICT;
	moves_map["Conversion"] = CONVERSION;
	moves_map["Conversion2"] = CONVERSION2;
	moves_map["Copycat"] = COPYCAT;
	moves_map["Cosmic Power"] = COSMIC_POWER;
	moves_map["Cotton Spore"] = COTTON_SPORE;
	moves_map["Counter"] = COUNTER;
	moves_map["Covet"] = COVET;
	moves_map["Crabhammer"] = CRABHAMMER;
	moves_map["Cross Chop"] = CROSS_CHOP;
	moves_map["Cross Poison"] = CROSS_POISON;
	moves_map["Crunch"] = CRUNCH;
	moves_map["Crush Claw"] = CRUSH_CLAW;
	moves_map["Crush Grip"] = CRUSH_GRIP;
	moves_map["Curse"] = CURSE;
	moves_map["Cut"] = CUT;
	moves_map["Dark Pulse"] = DARK_PULSE;
	moves_map["Dark Void"] = DARK_VOID;
	moves_map["Defend Order"] = DEFEND_ORDER;
	moves_map["Defense Curl"] = DEFENSE_CURL;
	moves_map["Defog"] = DEFOG;
	moves_map["Destiny Bond"] = DESTINY_BOND;
	moves_map["Detect"] = DETECT;
	moves_map["Dig"] = DIG;
	moves_map["Disable"] = DISABLE;
	moves_map["Discharge"] = DISCHARGE;
	moves_map["Dive"] = DIVE;
	moves_map["Dizzy Punch"] = DIZZY_PUNCH;
	moves_map["Doom Desire"] = DOOM_DESIRE;
	moves_map["Double Hit"] = DOUBLE_HIT;
	moves_map["Double Kick"] = DOUBLE_KICK;
	moves_map["Double Team"] = DOUBLE_TEAM;
	moves_map["Double-Edge"] = DOUBLE_EDGE;
	moves_map["DoubleSlap"] = DOUBLESLAP;
	moves_map["Draco Meteor"] = DRACO_METEOR;
	moves_map["Dragon Claw"] = DRAGON_CLAW;
	moves_map["Dragon Dance"] = DRAGON_DANCE;
	moves_map["Dragon Pulse"] = DRAGON_PULSE;
	moves_map["Dragon Rage"] = DRAGON_RAGE;
	moves_map["Dragon Rush"] = DRAGON_RUSH;
	moves_map["DragonBreath"] = DRAGONBREATH;
	moves_map["Drain Punch"] = DRAIN_PUNCH;
	moves_map["Dream Eater"] = DREAM_EATER;
	moves_map["Drill Peck"] = DRILL_PECK;
	moves_map["DynamicPunch"] = DYNAMICPUNCH;
	moves_map["Earth Power"] = EARTH_POWER;
	moves_map["Earthquake"] = EARTHQUAKE;
	moves_map["Egg Bomb"] = EGG_BOMB;
	moves_map["Embargo"] = EMBARGO;
	moves_map["Ember"] = EMBER;
	moves_map["Encore"] = ENCORE;
	moves_map["Endeavor"] = ENDEAVOR;
	moves_map["Endure"] = ENDURE;
	moves_map["Energy Ball"] = ENERGY_BALL;
	moves_map["Eruption"] = ERUPTION;
	moves_map["Explosion"] = EXPLOSION;
	moves_map["Extrasensory"] = EXTRASENSORY;
	moves_map["ExtremeSpeed"] = EXTREMESPEED;
	moves_map["Facade"] = FACADE;
	moves_map["Faint Attack"] = FAINT_ATTACK;
	moves_map["Fake Out"] = FAKE_OUT;
	moves_map["Fake Tears"] = FAKE_TEARS;
	moves_map["False Swipe"] = FALSE_SWIPE;
	moves_map["FeatherDance"] = FEATHERDANCE;
	moves_map["Feint"] = FEINT;
	moves_map["Fire Blast"] = FIRE_BLAST;
	moves_map["Fire Fang"] = FIRE_FANG;
	moves_map["Fire Punch"] = FIRE_PUNCH;
	moves_map["Fire Spin"] = FIRE_SPIN;
	moves_map["Fissure"] = FISSURE;
	moves_map["Flail"] = FLAIL;
	moves_map["Flame Wheel"] = FLAME_WHEEL;
	moves_map["Flamethrower"] = FLAMETHROWER;
	moves_map["Flare Blitz"] = FLARE_BLITZ;
	moves_map["Flash"] = FLASH;
	moves_map["Flash Cannon"] = FLASH_CANNON;
	moves_map["Flatter"] = FLATTER;
	moves_map["Fling"] = FLING;
	moves_map["Fly"] = FLY;
	moves_map["Focus Blast"] = FOCUS_BLAST;
	moves_map["Focus Energy"] = FOCUS_ENERGY;
	moves_map["Focus Punch"] = FOCUS_PUNCH;
	moves_map["Follow Me"] = FOLLOW_ME;
	moves_map["Force Palm"] = FORCE_PALM;
	moves_map["Foresight"] = FORESIGHT;
	moves_map["Frenzy Plant"] = FRENZY_PLANT;
	moves_map["Frustration"] = FRUSTRATION;
	moves_map["Fury Attack"] = FURY_ATTACK;
	moves_map["Fury Cutter"] = FURY_CUTTER;
	moves_map["Fury Swipes"] = FURY_SWIPES;
	moves_map["Future Sight"] = FUTURE_SIGHT;
	moves_map["Gastro Acid"] = GASTRO_ACID;
	moves_map["Giga Drain"] = GIGA_DRAIN;
	moves_map["Giga Impact"] = GIGA_IMPACT;
	moves_map["Glare"] = GLARE;
	moves_map["Grass Knot"] = GRASS_KNOT;
	moves_map["GrassWhistle"] = GRASSWHISTLE;
	moves_map["Gravity"] = GRAVITY;
	moves_map["Growl"] = GROWL;
	moves_map["Growth"] = GROWTH;
	moves_map["Grudge"] = GRUDGE;
	moves_map["Guard Swap"] = GUARD_SWAP;
	moves_map["Guillotine"] = GUILLOTINE;
	moves_map["Gunk Shot"] = GUNK_SHOT;
	moves_map["Gust"] = GUST;
	moves_map["Gyro Ball"] = GYRO_BALL;
	moves_map["Hail"] = HAIL_MOVE;
	moves_map["Hammer Arm"] = HAMMER_ARM;
	moves_map["Harden"] = HARDEN;
	moves_map["Haze"] = HAZE;
	moves_map["Head Smash"] = HEAD_SMASH;
	moves_map["Headbutt"] = HEADBUTT;
	moves_map["Heal Bell"] = HEAL_BELL;
	moves_map["Heal Block"] = HEAL_BLOCK;
	moves_map["Heal Order"] = HEAL_ORDER;
	moves_map["Healing Wish"] = HEALING_WISH;
	moves_map["Heart Swap"] = HEART_SWAP;
	moves_map["Heat Wave"] = HEAT_WAVE;
	moves_map["Helping Hand"] = HELPING_HAND;
	moves_map["Hi Jump Kick"] = HI_JUMP_KICK;
	moves_map["Hidden Power"] = HIDDEN_POWER;
	moves_map["Horn Attack"] = HORN_ATTACK;
	moves_map["Horn Drill"] = HORN_DRILL;
	moves_map["Howl"] = HOWL;
	moves_map["Hydro Cannon"] = HYDRO_CANNON;
	moves_map["Hydro Pump"] = HYDRO_PUMP;
	moves_map["Hyper Beam"] = HYPER_BEAM;
	moves_map["Hyper Fang"] = HYPER_FANG;
	moves_map["Hyper Voice"] = HYPER_VOICE;
	moves_map["Hypnosis"] = HYPNOSIS;
	moves_map["Ice Ball"] = ICE_BALL;
	moves_map["Ice Beam"] = ICE_BEAM;
	moves_map["Ice Fang"] = ICE_FANG;
	moves_map["Ice Punch"] = ICE_PUNCH;
	moves_map["Ice Shard"] = ICE_SHARD;
	moves_map["Icicle Spear"] = ICICLE_SPEAR;
	moves_map["Icy Wind"] = ICY_WIND;
	moves_map["Imprison"] = IMPRISON;
	moves_map["Ingrain"] = INGRAIN;
	moves_map["Iron Defense"] = IRON_DEFENSE;
	moves_map["Iron Head"] = IRON_HEAD;
	moves_map["Iron Tail"] = IRON_TAIL;
	moves_map["Judgment"] = JUDGMENT;
	moves_map["Jump Kick"] = JUMP_KICK;
	moves_map["Karate Chop"] = KARATE_CHOP;
	moves_map["Kinesis"] = KINESIS;
	moves_map["Knock Off"] = KNOCK_OFF;
	moves_map["Last Resort"] = LAST_RESORT;
	moves_map["Lava Plume"] = LAVA_PLUME;
	moves_map["Leaf Blade"] = LEAF_BLADE;
	moves_map["Leaf Storm"] = LEAF_STORM;
	moves_map["Leech Life"] = LEECH_LIFE;
	moves_map["Leech Seed"] = LEECH_SEED;
	moves_map["Leer"] = LEER;
	moves_map["Lick"] = LICK;
	moves_map["Light Screen"] = LIGHT_SCREEN;
	moves_map["Lock-On"] = LOCK_ON;
	moves_map["Lovely Kiss"] = LOVELY_KISS;
	moves_map["Low Kick"] = LOW_KICK;
	moves_map["Lucky Chant"] = LUCKY_CHANT;
	moves_map["Lunar Dance"] = LUNAR_DANCE;
	moves_map["Luster Purge"] = LUSTER_PURGE;
	moves_map["Mach Punch"] = MACH_PUNCH;
	moves_map["Magic Coat"] = MAGIC_COAT;
	moves_map["Magical Leaf"] = MAGICAL_LEAF;
	moves_map["Magma Storm"] = MAGMA_STORM;
	moves_map["Magnet Bomb"] = MAGNET_BOMB;
	moves_map["Magnet Rise"] = MAGNET_RISE;
	moves_map["Magnitude"] = MAGNITUDE;
	moves_map["Me First"] = ME_FIRST;
	moves_map["Mean Look"] = MEAN_LOOK;
	moves_map["Meditate"] = MEDITATE;
	moves_map["Mega Drain"] = MEGA_DRAIN;
	moves_map["Mega Kick"] = MEGA_KICK;
	moves_map["Mega Punch"] = MEGA_PUNCH;
	moves_map["Megahorn"] = MEGAHORN;
	moves_map["Memento"] = MEMENTO;
	moves_map["Metal Burst"] = METAL_BURST;
	moves_map["Metal Claw"] = METAL_CLAW;
	moves_map["Metal Sound"] = METAL_SOUND;
	moves_map["Meteor Mash"] = METEOR_MASH;
	moves_map["Metronome"] = METRONOME_MOVE;
	moves_map["Milk Drink"] = MILK_DRINK;
	moves_map["Mimic"] = MIMIC;
	moves_map["Mind Reader"] = MIND_READER;
	moves_map["Minimize"] = MINIMIZE;
	moves_map["Miracle Eye"] = MIRACLE_EYE;
	moves_map["Mirror Coat"] = MIRROR_COAT;
	moves_map["Mirror Move"] = MIRROR_MOVE;
	moves_map["Mirror Shot"] = MIRROR_SHOT;
	moves_map["Mist"] = MIST;
	moves_map["Mist Ball"] = MIST_BALL;
	moves_map["Moonlight"] = MOONLIGHT;
	moves_map["Morning Sun"] = MORNING_SUN;
	moves_map["Mud Bomb"] = MUD_BOMB;
	moves_map["Mud Shot"] = MUD_SHOT;
	moves_map["Mud Sport"] = MUD_SPORT;
	moves_map["Mud-Slap"] = MUD_SLAP;
	moves_map["Muddy Water"] = MUDDY_WATER;
	moves_map["Nasty Plot"] = NASTY_PLOT;
	moves_map["Natural Gift"] = NATURAL_GIFT;
	moves_map["Nature Power"] = NATURE_POWER;
	moves_map["Needle Arm"] = NEEDLE_ARM;
	moves_map["Night Shade"] = NIGHT_SHADE;
	moves_map["Night Slash"] = NIGHT_SLASH;
	moves_map["Nightmare"] = NIGHTMARE;
	moves_map["Octazooka"] = OCTAZOOKA;
	moves_map["Odor Sleuth"] = ODOR_SLEUTH;
	moves_map["Ominous Wind"] = OMINOUS_WIND;
	moves_map["Outrage"] = OUTRAGE;
	moves_map["Overheat"] = OVERHEAT;
	moves_map["Pain Split"] = PAIN_SPLIT;
	moves_map["Pay Day"] = PAY_DAY;
	moves_map["Payback"] = PAYBACK;
	moves_map["Peck"] = PECK;
	moves_map["Perish Song"] = PERISH_SONG;
	moves_map["Petal Dance"] = PETAL_DANCE;
	moves_map["Pin Missile"] = PIN_MISSILE;
	moves_map["Pluck"] = PLUCK;
	moves_map["Poison Fang"] = POISON_FANG;
	moves_map["Poison Gas"] = POISON_GAS;
	moves_map["Poison Jab"] = POISON_JAB;
	moves_map["Poison Sting"] = POISON_STING;
	moves_map["Poison Tail"] = POISON_TAIL;
	moves_map["PoisonPowder"] = POISONPOWDER;
	moves_map["Pound"] = POUND;
	moves_map["Powder Snow"] = POWDER_SNOW;
	moves_map["Power Gem"] = POWER_GEM;
	moves_map["Power Swap"] = POWER_SWAP;
	moves_map["Power Trick"] = POWER_TRICK;
	moves_map["Power Whip"] = POWER_WHIP;
	moves_map["Present"] = PRESENT;
	moves_map["Protect"] = PROTECT;
	moves_map["Psybeam"] = PSYBEAM;
	moves_map["Psych Up"] = PSYCH_UP;
	moves_map["Psychic"] = PSYCHIC_MOVE;
	moves_map["Psycho Boost"] = PSYCHO_BOOST;
	moves_map["Psycho Cut"] = PSYCHO_CUT;
	moves_map["Psycho Shift"] = PSYCHO_SHIFT;
	moves_map["Psywave"] = PSYWAVE;
	moves_map["Punishment"] = PUNISHMENT;
	moves_map["Pursuit"] = PURSUIT;
	moves_map["Quick Attack"] = QUICK_ATTACK;
	moves_map["Rage"] = RAGE;
	moves_map["Rain Dance"] = RAIN_DANCE;
	moves_map["Rapid Spin"] = RAPID_SPIN;
	moves_map["Razor Leaf"] = RAZOR_LEAF;
	moves_map["Razor Wind"] = RAZOR_WIND;
	moves_map["Recover"] = RECOVER;
	moves_map["Recycle"] = RECYCLE;
	moves_map["Reflect"] = REFLECT;
	moves_map["Refresh"] = REFRESH;
	moves_map["Rest"] = REST;
	moves_map["Return"] = RETURN;
	moves_map["Revenge"] = REVENGE;
	moves_map["Reversal"] = REVERSAL;
	moves_map["Roar"] = ROAR;
	moves_map["Roar Of Time"] = ROAR_OF_TIME;
	moves_map["Rock Blast"] = ROCK_BLAST;
	moves_map["Rock Climb"] = ROCK_CLIMB;
	moves_map["Rock Polish"] = ROCK_POLISH;
	moves_map["Rock Slide"] = ROCK_SLIDE;
	moves_map["Rock Smash"] = ROCK_SMASH;
	moves_map["Rock Throw"] = ROCK_THROW;
	moves_map["Rock Tomb"] = ROCK_TOMB;
	moves_map["Rock Wrecker"] = ROCK_WRECKER;
	moves_map["Role Play"] = ROLE_PLAY;
	moves_map["Rolling Kick"] = ROLLING_KICK;
	moves_map["Rollout"] = ROLLOUT;
	moves_map["Roost"] = ROOST;
	moves_map["Sacred Fire"] = SACRED_FIRE;
	moves_map["Safeguard"] = SAFEGUARD;
	moves_map["Sand Tomb"] = SAND_TOMB;
	moves_map["Sand-Attack"] = SAND_ATTACK;
	moves_map["Sandstorm"] = SANDSTORM;
	moves_map["Scary Face"] = SCARY_FACE;
	moves_map["Scratch"] = SCRATCH;
	moves_map["Screech"] = SCREECH;
	moves_map["Secret Power"] = SECRET_POWER;
	moves_map["Seed Bomb"] = SEED_BOMB;
	moves_map["Seed Flare"] = SEED_FLARE;
	moves_map["Seismic Toss"] = SEISMIC_TOSS;
	moves_map["Selfdestruct"] = SELFDESTRUCT;
	moves_map["Shadow Ball"] = SHADOW_BALL;
	moves_map["Shadow Claw"] = SHADOW_CLAW;
	moves_map["Shadow Force"] = SHADOW_FORCE;
	moves_map["Shadow Punch"] = SHADOW_PUNCH;
	moves_map["Shadow Sneak"] = SHADOW_SNEAK;
	moves_map["Sharpen"] = SHARPEN;
	moves_map["Sheer Cold"] = SHEER_COLD;
	moves_map["Shock Wave"] = SHOCK_WAVE;
	moves_map["Signal Beam"] = SIGNAL_BEAM;
	moves_map["Silver Wind"] = SILVER_WIND;
	moves_map["Sing"] = SING;
	moves_map["Sketch"] = SKETCH;
	moves_map["Skill Swap"] = SKILL_SWAP;
	moves_map["Skull Bash"] = SKULL_BASH;
	moves_map["Sky Attack"] = SKY_ATTACK;
	moves_map["Sky Uppercut"] = SKY_UPPERCUT;
	moves_map["Slack Off"] = SLACK_OFF;
	moves_map["Slam"] = SLAM;
	moves_map["Slash"] = SLASH;
	moves_map["Sleep Powder"] = SLEEP_POWDER;
	moves_map["Sleep Talk"] = SLEEP_TALK;
	moves_map["Sludge"] = SLUDGE;
	moves_map["Sludge Bomb"] = SLUDGE_BOMB;
	moves_map["SmellingSalt"] = SMELLINGSALT;
	moves_map["Smog"] = SMOG;
	moves_map["SmokeScreen"] = SMOKESCREEN;
	moves_map["Snatch"] = SNATCH;
	moves_map["Snore"] = SNORE;
	moves_map["Softboiled"] = SOFTBOILED;
	moves_map["SolarBeam"] = SOLARBEAM;
	moves_map["SonicBoom"] = SONICBOOM;
	moves_map["Spacial Rend"] = SPACIAL_REND;
	moves_map["Spark"] = SPARK;
	moves_map["Spider Web"] = SPIDER_WEB;
	moves_map["Spike Cannon"] = SPIKE_CANNON;
	moves_map["Spikes"] = SPIKES;
	moves_map["Spit Up"] = SPIT_UP;
	moves_map["Spite"] = SPITE;
	moves_map["Splash"] = SPLASH;
	moves_map["Spore"] = SPORE;
	moves_map["Stealth Rock"] = STEALTH_ROCK;
	moves_map["Steel Wing"] = STEEL_WING;
	moves_map["Stockpile"] = STOCKPILE;
	moves_map["Stomp"] = STOMP;
	moves_map["Stone Edge"] = STONE_EDGE;
	moves_map["Strength"] = STRENGTH;
	moves_map["String Shot"] = STRING_SHOT;
	moves_map["Struggle"] = STRUGGLE;
	moves_map["Stun Spore"] = STUN_SPORE;
	moves_map["Submission"] = SUBMISSION;
	moves_map["Substitute"] = SUBSTITUTE;
	moves_map["Sucker Punch"] = SUCKER_PUNCH;
	moves_map["Sunny Day"] = SUNNY_DAY;
	moves_map["Super Fang"] = SUPER_FANG;
	moves_map["Superpower"] = SUPERPOWER;
	moves_map["Supersonic"] = SUPERSONIC;
	moves_map["Surf"] = SURF;
	moves_map["Swagger"] = SWAGGER;
	moves_map["Swallow"] = SWALLOW;
	moves_map["Sweet Kiss"] = SWEET_KISS;
	moves_map["Sweet Scent"] = SWEET_SCENT;
	moves_map["Swift"] = SWIFT;
	moves_map["Switcheroo"] = SWITCHEROO;
	moves_map["Swords Dance"] = SWORDS_DANCE;
	moves_map["Synthesis"] = SYNTHESIS;
	moves_map["Tackle"] = TACKLE;
	moves_map["Tail Glow"] = TAIL_GLOW;
	moves_map["Tail Whip"] = TAIL_WHIP;
	moves_map["Tailwind"] = TAILWIND;
	moves_map["Take Down"] = TAKE_DOWN;
	moves_map["Taunt"] = TAUNT;
	moves_map["Teeter Dance"] = TEETER_DANCE;
	moves_map["Teleport"] = TELEPORT;
	moves_map["Thief"] = THIEF;
	moves_map["Thrash"] = THRASH;
	moves_map["Thunder"] = THUNDER;
	moves_map["Thunder Fang"] = THUNDER_FANG;
	moves_map["Thunder Wave"] = THUNDER_WAVE;
	moves_map["Thunderbolt"] = THUNDERBOLT;
	moves_map["ThunderPunch"] = THUNDERPUNCH;
	moves_map["ThunderShock"] = THUNDERSHOCK;
	moves_map["Tickle"] = TICKLE;
	moves_map["Torment"] = TORMENT;
	moves_map["Toxic"] = TOXIC;
	moves_map["Toxic Spikes"] = TOXIC_SPIKES;
	moves_map["Transform"] = TRANSFORM;
	moves_map["Tri Attack"] = TRI_ATTACK;
	moves_map["Trick"] = TRICK;
	moves_map["Trick Room"] = TRICK_ROOM;
	moves_map["Triple Kick"] = TRIPLE_KICK;
	moves_map["Trump Card"] = TRUMP_CARD;
	moves_map["Twineedle"] = TWINEEDLE;
	moves_map["Twister"] = TWISTER;
	moves_map["U-turn"] = U_TURN;
	moves_map["Uproar"] = UPROAR;
	moves_map["Vacuum Wave"] = VACUUM_WAVE;
	moves_map["ViceGrip"] = VICEGRIP;
	moves_map["Vine Whip"] = VINE_WHIP;
	moves_map["Vital Throw"] = VITAL_THROW;
	moves_map["Volt Tackle"] = VOLT_TACKLE;
	moves_map["Wake-Up Slap"] = WAKE_UP_SLAP;
	moves_map["Water Gun"] = WATER_GUN;
	moves_map["Water Pulse"] = WATER_PULSE;
	moves_map["Water Sport"] = WATER_SPORT;
	moves_map["Water Spout"] = WATER_SPOUT;
	moves_map["Waterfall"] = WATERFALL;
	moves_map["Weather Ball"] = WEATHER_BALL;
	moves_map["Whirlpool"] = WHIRLPOOL;
	moves_map["Whirlwind"] = WHIRLWIND;
	moves_map["Will-O-Wisp"] = WILL_O_WISP;
	moves_map["Wing Attack"] = WING_ATTACK;
	moves_map["Wish"] = WISH;
	moves_map["Withdraw"] = WITHDRAW;
	moves_map["Wood Hammer"] = WOOD_HAMMER;
	moves_map["Worry Seed"] = WORRY_SEED;
	moves_map["Wrap"] = WRAP;
	moves_map["Wring Out"] = WRING_OUT;
	moves_map["X-Scissor"] = X_SCISSOR;
	moves_map["Yawn"] = YAWN;
	moves_map["Zap Cannon"] = ZAP_CANNON;
	moves_map["Zen Headbutt"] = ZEN_HEADBUTT;
}

#endif
