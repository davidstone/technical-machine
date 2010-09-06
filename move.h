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

void move_type (moves &move) {
	if (move.name == ATTACK_ORDER or move.name == BUG_BITE or move.name == BUG_BUZZ or move.name == DEFEND_ORDER or move.name == FURY_CUTTER or move.name == HEAL_ORDER or move.name == LEECH_LIFE or move.name == MEGAHORN or move.name == PIN_MISSILE or move.name == SIGNAL_BEAM or move.name == SILVER_WIND or move.name == SPIDER_WEB or move.name == STRING_SHOT or move.name == TAIL_GLOW or move.name == TWINEEDLE or move.name == U_TURN or move.name == X_SCISSOR)
		move.type = BUG;

	else if (move.name == ASSURANCE or move.name == BEAT_UP or move.name == BITE or move.name == CRUNCH or move.name == DARK_PULSE or move.name == DARK_VOID or move.name == EMBARGO or move.name == FAINT_ATTACK or move.name == FAKE_TEARS or move.name == FLATTER or move.name == FLING or move.name == KNOCK_OFF or move.name == MEMENTO or move.name == NASTY_PLOT or move.name == NIGHT_SLASH or move.name == PAYBACK or move.name == PUNISHMENT or move.name == PURSUIT or move.name == SNATCH or move.name == SUCKER_PUNCH or move.name == SWITCHEROO or move.name == TAUNT or move.name == THIEF or move.name == TORMENT)
		move.type = DARK;

	else if (move.name == DRACO_METEOR or move.name == DRAGONBREATH or move.name == DRAGON_CLAW or move.name == DRAGON_DANCE or move.name == DRAGON_PULSE or move.name == DRAGON_RAGE or move.name == DRAGON_RUSH or move.name == OUTRAGE or move.name == ROAR_OF_TIME or move.name == SPACIAL_REND or move.name == TWISTER)
		move.type = DRAGON;

	else if (move.name == CHARGE or move.name == CHARGE_BEAM or move.name == DISCHARGE or move.name == MAGNET_RISE or move.name == SHOCK_WAVE or move.name == SPARK or move.name == THUNDER or move.name == THUNDERBOLT or move.name == THUNDER_FANG or move.name == THUNDERPUNCH or move.name == THUNDERSHOCK or move.name == THUNDER_WAVE or move.name == VOLT_TACKLE or move.name == ZAP_CANNON)
		move.type = ELECTRIC;

	else if (move.name == ARM_THRUST or move.name == AURA_SPHERE or move.name == BRICK_BREAK or move.name == BULK_UP or move.name == CLOSE_COMBAT or move.name == COUNTER or move.name == CROSS_CHOP or move.name == DETECT or move.name == DOUBLE_KICK or move.name == DRAIN_PUNCH or move.name == DYNAMICPUNCH or move.name == FOCUS_BLAST or move.name == FOCUS_PUNCH or move.name == FORCE_PALM or move.name == HAMMER_ARM or move.name == HI_JUMP_KICK or move.name == JUMP_KICK or move.name == KARATE_CHOP or move.name == LOW_KICK or move.name == MACH_PUNCH or move.name == REVENGE or move.name == REVERSAL or move.name == ROCK_SMASH or move.name == ROLLING_KICK or move.name == SEISMIC_TOSS or move.name == SKY_UPPERCUT or move.name == SUBMISSION or move.name == SUPERPOWER or move.name == TRIPLE_KICK or move.name == VACUUM_WAVE or move.name == VITAL_THROW or move.name == WAKE_UP_SLAP)
		move.type = FIGHTING;

	else if (move.name == BLAST_BURN or move.name == BLAZE_KICK or move.name == EMBER or move.name == ERUPTION or move.name == FIRE_BLAST or move.name == FIRE_FANG or move.name == FIRE_PUNCH or move.name == FIRE_SPIN or move.name == FLAMETHROWER or move.name == FLAME_WHEEL or move.name == FLARE_BLITZ or move.name == HEAT_WAVE or move.name == LAVA_PLUME or move.name == MAGMA_STORM or move.name == OVERHEAT or move.name == SACRED_FIRE or move.name == SUNNY_DAY or move.name == WILL_O_WISP)
		move.type = FIRE;

	else if (move.name == AERIAL_ACE or move.name == AEROBLAST or move.name == AIR_CUTTER or move.name == AIR_SLASH or move.name == BOUNCE or move.name == BRAVE_BIRD or move.name == CHATTER or move.name == DEFOG or move.name == DRILL_PECK or move.name == FEATHERDANCE or move.name == FLY or move.name == GUST or move.name == MIRROR_MOVE or move.name == PECK or move.name == PLUCK or move.name == ROOST or move.name == SKY_ATTACK or move.name == TAILWIND or move.name == WING_ATTACK)
		move.type = FLYING;

	else if (move.name == ASTONISH or move.name == CONFUSE_RAY or move.name == DESTINY_BOND or move.name == GRUDGE or move.name == LICK or move.name == NIGHTMARE or move.name == NIGHT_SHADE or move.name == OMINOUS_WIND or move.name == SHADOW_BALL or move.name == SHADOW_CLAW or move.name == SHADOW_FORCE or move.name == SHADOW_PUNCH or move.name == SHADOW_SNEAK or move.name == SPITE)
		move.type = GHOST;

	else if (move.name == ABSORB or move.name == AROMATHERAPY or move.name == BULLET_SEED or move.name == COTTON_SPORE or move.name == ENERGY_BALL or move.name == FRENZY_PLANT or move.name == GIGA_DRAIN or move.name == GRASS_KNOT or move.name == GRASSWHISTLE or move.name == INGRAIN or move.name == LEAF_BLADE or move.name == LEAF_STORM or move.name == LEECH_SEED or move.name == MAGICAL_LEAF or move.name == MEGA_DRAIN or move.name == NEEDLE_ARM or move.name == PETAL_DANCE or move.name == POWER_WHIP or move.name == RAZOR_LEAF or move.name == SEED_BOMB or move.name == SEED_FLARE or move.name == SLEEP_POWDER or move.name == SOLARBEAM or move.name == SPORE or move.name == STUN_SPORE or move.name == SYNTHESIS or move.name == VINE_WHIP or move.name == WOOD_HAMMER or move.name == WORRY_SEED)
		move.type = GRASS;

	else if (move.name == BONE_CLUB or move.name == BONEMERANG or move.name == BONE_RUSH or move.name == DIG or move.name == EARTH_POWER or move.name == EARTHQUAKE or move.name == FISSURE or move.name == MAGNITUDE or move.name == MUD_BOMB or move.name == MUD_SHOT or move.name == MUD_SLAP or move.name == MUD_SPORT or move.name == SAND_ATTACK or move.name == SAND_TOMB or move.name == SPIKES)
		move.type = GROUND;

	else if (move.name == AURORA_BEAM or move.name == AVALANCHE or move.name == BLIZZARD or move.name == HAIL_MOVE or move.name == HAZE or move.name == ICE_BALL or move.name == ICE_BEAM or move.name == ICE_FANG or move.name == ICE_PUNCH or move.name == ICE_SHARD or move.name == ICICLE_SPEAR or move.name == ICY_WIND or move.name == MIST or move.name == POWDER_SNOW or move.name == SHEER_COLD)
		move.type = ICE;

	else if (move.name == ACID or move.name == ACID_ARMOR or move.name == CROSS_POISON or move.name == GASTRO_ACID or move.name == GUNK_SHOT or move.name == POISON_FANG or move.name == POISON_GAS or move.name == POISON_JAB or move.name == POISONPOWDER or move.name == POISON_STING or move.name == POISON_TAIL or move.name == SLUDGE or move.name == SLUDGE_BOMB or move.name == SMOG or move.name == TOXIC or move.name == TOXIC_SPIKES)
		move.type = POISON;

	else if (move.name == AGILITY or move.name == AMNESIA or move.name == BARRIER or move.name == CALM_MIND or move.name == CONFUSION or move.name == COSMIC_POWER or move.name == DREAM_EATER or move.name == EXTRASENSORY or move.name == FUTURE_SIGHT or move.name == GRAVITY or move.name == GUARD_SWAP or move.name == HEAL_BLOCK or move.name == HEALING_WISH or move.name == HEART_SWAP or move.name == HYPNOSIS or move.name == IMPRISON or move.name == KINESIS or move.name == LIGHT_SCREEN or move.name == LUNAR_DANCE or move.name == LUSTER_PURGE or move.name == MAGIC_COAT or move.name == MEDITATE or move.name == MIRACLE_EYE or move.name == MIRROR_COAT or move.name == MIST_BALL or move.name == POWER_SWAP or move.name == POWER_TRICK or move.name == PSYBEAM or move.name == PSYCHIC_MOVE or move.name == PSYCHO_BOOST or move.name == PSYCHO_CUT or move.name == PSYCHO_SHIFT or move.name == PSYWAVE or move.name == REFLECT or move.name == REST or move.name == ROLE_PLAY or move.name == SKILL_SWAP or move.name == TELEPORT or move.name == TRICK or move.name == TRICK_ROOM or move.name == ZEN_HEADBUTT)
		move.type = PSYCHIC_TYPE;

	else if (move.name == ANCIENTPOWER or move.name == HEAD_SMASH or move.name == POWER_GEM or move.name == ROCK_BLAST or move.name == ROCK_POLISH or move.name == ROCK_SLIDE or move.name == ROCK_THROW or move.name == ROCK_TOMB or move.name == ROCK_WRECKER or move.name == ROLLOUT or move.name == SANDSTORM or move.name == STEALTH_ROCK or move.name == STONE_EDGE)
		move.type = ROCK;

	else if (move.name == BULLET_PUNCH or move.name == DOOM_DESIRE or move.name == FLASH_CANNON or move.name == GYRO_BALL or move.name == IRON_DEFENSE or move.name == IRON_HEAD or move.name == IRON_TAIL or move.name == MAGNET_BOMB or move.name == METAL_BURST or move.name == METAL_CLAW or move.name == METAL_SOUND or move.name == METEOR_MASH or move.name == MIRROR_SHOT or move.name == STEEL_WING)
		move.type = STEEL;

	else if (move.name == AQUA_JET or move.name == AQUA_RING or move.name == AQUA_TAIL or move.name == BRINE or move.name == BUBBLE or move.name == BUBBLEBEAM or move.name == CLAMP or move.name == CRABHAMMER or move.name == DIVE or move.name == HYDRO_CANNON or move.name == HYDRO_PUMP or move.name == MUDDY_WATER or move.name == OCTAZOOKA or move.name == RAIN_DANCE or move.name == SURF or move.name == WATERFALL or move.name == WATER_GUN or move.name == WATER_PULSE or move.name == WATER_SPORT or move.name == WATER_SPOUT or move.name == WHIRLPOOL or move.name == WITHDRAW)
		move.type = WATER;

	else if (move.name == CURSE or move.name == STRUGGLE)
		move.type = TYPELESS;
	
	else
		move.type = NORMAL;
	//	else if (move.name == ACUPRESSURE or move.name == ASSIST or move.name == ATTRACT or move.name == BARRAGE or move.name == BATON_PASS or move.name == BELLY_DRUM or move.name == BIDE or move.name == BIND or move.name == BLOCK or move.name == BODY_SLAM or move.name == CAMOUFLAGE or move.name == CAPTIVATE or move.name == CHARM or move.name == COMET_PUNCH or move.name == CONSTRICT or move.name == CONVERSION or move.name == CONVERSION_2 or move.name == COPYCAT or move.name == COVET or move.name == CRUSH_CLAW or move.name == CRUSH_GRIP or move.name == CUT or move.name == DEFENSE_CURL or move.name == DISABLE or move.name == DIZZY_PUNCH or move.name == DOUBLE_EDGE or move.name == DOUBLE_HIT or move.name == DOUBLESLAP or move.name == DOUBLE_TEAM or move.name == EGG_BOMB or move.name == ENCORE or move.name == ENDEAVOR or move.name == ENDURE or move.name == EXPLOSION or move.name == EXTREMESPEED or move.name == FACADE or move.name == FAKE_OUT or move.name == FALSE_SWIPE or move.name == FEINT or move.name == FLAIL or move.name == FLASH or move.name == FOCUS_ENERGY or move.name == FOLLOW_ME or move.name == FORESIGHT or move.name == FRUSTRATION or move.name == FURY_ATTACK or move.name == FURY_SWIPES or move.name == GIGA_IMPACT or move.name == GLARE or move.name == GROWL or move.name == GROWTH or move.name == GUILLOTINE or move.name == HARDEN or move.name == HEADBUTT or move.name == HEAL_BELL or move.name == HELPING_HAND or move.name == HIDDEN_POWER or move.name == HORN_ATTACK or move.name == HORN_DRILL or move.name == HOWL or move.name == HYPER_BEAM or move.name == HYPER_FANG or move.name == HYPER_VOICE or move.name == JUDGMENT or move.name == LAST_RESORT or move.name == LEER or move.name == LOCK_ON or move.name == LOVELY_KISS or move.name == LUCKY_CHANT or move.name == MEAN_LOOK or move.name == ME_FIRST or move.name == MEGA_KICK or move.name == MEGA_PUNCH or move.name == METRONOME_MOVE or move.name == MILK_DRINK or move.name == MIMIC or move.name == MIND_READER or move.name == MINIMIZE or move.name == MOONLIGHT or move.name == MORNING_SUN or move.name == NATURAL_GIFT or move.name == NATURE_POWER or move.name == ODOR_SLEUTH or move.name == PAIN_SPLIT or move.name == PAY_DAY or move.name == PERISH_SONG or move.name == POUND or move.name == PRESENT or move.name == PROTECT or move.name == PSYCH_UP or move.name == QUICK_ATTACK or move.name == RAGE or move.name == RAPID_SPIN or move.name == RAZOR_WIND or move.name == RECOVER or move.name == RECYCLE or move.name == REFRESH or move.name == RETURN or move.name == ROAR or move.name == ROCK_CLIMB or move.name == SAFEGUARD or move.name == SCARY_FACE or move.name == SCRATCH or move.name == SCREECH or move.name == SECRET_POWER or move.name == SELFDESTRUCT or move.name == SHARPEN or move.name == SING or move.name == SKETCH or move.name == SKULL_BASH or move.name == SLACK_OFF or move.name == SLAM or move.name == SLASH or move.name == SLEEP_TALK or move.name == SMELLINGSALT or move.name == SMOKESCREEN or move.name == SNORE or move.name == SOFTBOILED or move.name == SONICBOOM or move.name == SPIKE_CANNON or move.name == SPIT_UP or move.name == SPLASH or move.name == STOCKPILE or move.name == STOMP or move.name == STRENGTH or move.name == STRUGGLE or move.name == SUBSTITUTE or move.name == SUPER_FANG or move.name == SUPERSONIC or move.name == SWAGGER or move.name == SWALLOW or move.name == SWEET_KISS or move.name == SWEET_SCENT or move.name == SWIFT or move.name == SWORDS_DANCE or move.name == TACKLE or move.name == TAIL_WHIP or move.name == TAKE_DOWN or move.name == TEETER_DANCE or move.name == THRASH or move.name == TICKLE or move.name == TRANSFORM or move.name == TRI_ATTACK or move.name == TRUMP_CARD or move.name == UPROAR or move.name == VICEGRIP or move.name == WEATHER_BALL or move.name == WHIRLWIND or move.name == WISH or move.name == WRAP or move.name == WRING_OUT or move.name == YAWN)
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
