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

#include <cstdint>
#include <string>
#include <utility>
#undef SING

#include "active.h"
#include "type.h"

namespace technicalmachine {

class Move {
	public:
		enum Moves { ABSORB, ACID, ACID_ARMOR, ACUPRESSURE, AERIAL_ACE, AEROBLAST, AGILITY, AIR_CUTTER, AIR_SLASH, AMNESIA, ANCIENTPOWER, AQUA_JET, AQUA_RING, AQUA_TAIL, ARM_THRUST, AROMATHERAPY, ASSIST, ASSURANCE, ASTONISH, ATTACK_ORDER, ATTRACT, AURA_SPHERE, AURORA_BEAM, AVALANCHE, BARRAGE, BARRIER, BATON_PASS, BEAT_UP, BELLY_DRUM, BIDE, BIND, BITE, BLAST_BURN, BLAZE_KICK, BLIZZARD, BLOCK, BODY_SLAM, BONE_CLUB, BONE_RUSH, BONEMERANG, BOUNCE, BRAVE_BIRD, BRICK_BREAK, BRINE, BUBBLE, BUBBLEBEAM, BUG_BITE, BUG_BUZZ, BULK_UP, BULLET_PUNCH, BULLET_SEED, CALM_MIND, CAMOUFLAGE, CAPTIVATE, CHARGE, CHARGE_BEAM, CHARM, CHATTER, CLAMP, CLOSE_COMBAT, COMET_PUNCH, CONFUSE_RAY, CONFUSION, CONSTRICT, CONVERSION, CONVERSION2, COPYCAT, COSMIC_POWER, COTTON_SPORE, COUNTER, COVET, CRABHAMMER, CROSS_CHOP, CROSS_POISON, CRUNCH, CRUSH_CLAW, CRUSH_GRIP, CURSE, CUT, DARK_PULSE, DARK_VOID, DEFEND_ORDER, DEFENSE_CURL, DEFOG, DESTINY_BOND, DETECT, DIG, DISABLE, DISCHARGE, DIVE, DIZZY_PUNCH, DOOM_DESIRE, DOUBLE_HIT, DOUBLE_KICK, DOUBLE_TEAM, DOUBLE_EDGE, DOUBLESLAP, DRACO_METEOR, DRAGON_CLAW, DRAGON_DANCE, DRAGON_PULSE, DRAGON_RAGE, DRAGON_RUSH, DRAGONBREATH, DRAIN_PUNCH, DREAM_EATER, DRILL_PECK, DYNAMICPUNCH, EARTH_POWER, EARTHQUAKE, EGG_BOMB, EMBARGO, EMBER, ENCORE, ENDEAVOR, ENDURE, ENERGY_BALL, ERUPTION, EXPLOSION, EXTRASENSORY, EXTREMESPEED, FACADE, FAINT_ATTACK, FAKE_OUT, FAKE_TEARS, FALSE_SWIPE, FEATHERDANCE, FEINT, FIRE_BLAST, FIRE_FANG, FIRE_PUNCH, FIRE_SPIN, FISSURE, FLAIL, FLAME_WHEEL, FLAMETHROWER, FLARE_BLITZ, FLASH, FLASH_CANNON, FLATTER, FLING, FLY, FOCUS_BLAST, FOCUS_ENERGY, FOCUS_PUNCH, FOLLOW_ME, FORCE_PALM, FORESIGHT, FRENZY_PLANT, FRUSTRATION, FURY_ATTACK, FURY_CUTTER, FURY_SWIPES, FUTURE_SIGHT, GASTRO_ACID, GIGA_DRAIN, GIGA_IMPACT, GLARE, GRASS_KNOT, GRASSWHISTLE, GRAVITY, GROWL, GROWTH, GRUDGE, GUARD_SWAP, GUILLOTINE, GUNK_SHOT, GUST, GYRO_BALL, HAIL, HAMMER_ARM, HARDEN, HAZE, HEAD_SMASH, HEADBUTT, HEAL_BELL, HEAL_BLOCK, HEAL_ORDER, HEALING_WISH, HEART_SWAP, HEAT_WAVE, HELPING_HAND, HI_JUMP_KICK, HIDDEN_POWER, HORN_ATTACK, HORN_DRILL, HOWL, HYDRO_CANNON, HYDRO_PUMP, HYPER_BEAM, HYPER_FANG, HYPER_VOICE, HYPNOSIS, ICE_BALL, ICE_BEAM, ICE_FANG, ICE_PUNCH, ICE_SHARD, ICICLE_SPEAR, ICY_WIND, IMPRISON, INGRAIN, IRON_DEFENSE, IRON_HEAD, IRON_TAIL, JUDGMENT, JUMP_KICK, KARATE_CHOP, KINESIS, KNOCK_OFF, LAST_RESORT, LAVA_PLUME, LEAF_BLADE, LEAF_STORM, LEECH_LIFE, LEECH_SEED, LEER, LICK, LIGHT_SCREEN, LOCK_ON, LOVELY_KISS, LOW_KICK, LUCKY_CHANT, LUNAR_DANCE, LUSTER_PURGE, MACH_PUNCH, MAGIC_COAT, MAGICAL_LEAF, MAGMA_STORM, MAGNET_BOMB, MAGNET_RISE, MAGNITUDE, ME_FIRST, MEAN_LOOK, MEDITATE, MEGA_DRAIN, MEGA_KICK, MEGA_PUNCH, MEGAHORN, MEMENTO, METAL_BURST, METAL_CLAW, METAL_SOUND, METEOR_MASH, METRONOME, MILK_DRINK, MIMIC, MIND_READER, MINIMIZE, MIRACLE_EYE, MIRROR_COAT, MIRROR_MOVE, MIRROR_SHOT, MIST, MIST_BALL, MOONLIGHT, MORNING_SUN, MUD_BOMB, MUD_SHOT, MUD_SPORT, MUD_SLAP, MUDDY_WATER, NASTY_PLOT, NATURAL_GIFT, NATURE_POWER, NEEDLE_ARM, NIGHT_SHADE, NIGHT_SLASH, NIGHTMARE, OCTAZOOKA, ODOR_SLEUTH, OMINOUS_WIND, OUTRAGE, OVERHEAT, PAIN_SPLIT, PAY_DAY, PAYBACK, PECK, PERISH_SONG, PETAL_DANCE, PIN_MISSILE, PLUCK, POISON_FANG, POISON_GAS, POISON_JAB, POISON_STING, POISON_TAIL, POISONPOWDER, POUND, POWDER_SNOW, POWER_GEM, POWER_SWAP, POWER_TRICK, POWER_WHIP, PRESENT, PROTECT, PSYBEAM, PSYCH_UP, PSYCHIC, PSYCHO_BOOST, PSYCHO_CUT, PSYCHO_SHIFT, PSYWAVE, PUNISHMENT, PURSUIT, QUICK_ATTACK, RAGE, RAIN_DANCE, RAPID_SPIN, RAZOR_LEAF, RAZOR_WIND, RECOVER, RECYCLE, REFLECT, REFRESH, REST, RETURN, REVENGE, REVERSAL, ROAR, ROAR_OF_TIME, ROCK_BLAST, ROCK_CLIMB, ROCK_POLISH, ROCK_SLIDE, ROCK_SMASH, ROCK_THROW, ROCK_TOMB, ROCK_WRECKER, ROLE_PLAY, ROLLING_KICK, ROLLOUT, ROOST, SACRED_FIRE, SAFEGUARD, SAND_TOMB, SAND_ATTACK, SANDSTORM, SCARY_FACE, SCRATCH, SCREECH, SECRET_POWER, SEED_BOMB, SEED_FLARE, SEISMIC_TOSS, SELFDESTRUCT, SHADOW_BALL, SHADOW_CLAW, SHADOW_FORCE, SHADOW_PUNCH, SHADOW_SNEAK, SHARPEN, SHEER_COLD, SHOCK_WAVE, SIGNAL_BEAM, SILVER_WIND, SING, SKETCH, SKILL_SWAP, SKULL_BASH, SKY_ATTACK, SKY_UPPERCUT, SLACK_OFF, SLAM, SLASH, SLEEP_POWDER, SLEEP_TALK, SLUDGE, SLUDGE_BOMB, SMELLINGSALT, SMOG, SMOKESCREEN, SNATCH, SNORE, SOFTBOILED, SOLARBEAM, SONICBOOM, SPACIAL_REND, SPARK, SPIDER_WEB, SPIKE_CANNON, SPIKES, SPIT_UP, SPITE, SPLASH, SPORE, STEALTH_ROCK, STEEL_WING, STOCKPILE, STOMP, STONE_EDGE, STRENGTH, STRING_SHOT, STRUGGLE, STUN_SPORE, SUBMISSION, SUBSTITUTE, SUCKER_PUNCH, SUNNY_DAY, SUPER_FANG, SUPERPOWER, SUPERSONIC, SURF, SWAGGER, SWALLOW, SWEET_KISS, SWEET_SCENT, SWIFT, SWITCH0, SWITCH1, SWITCH2, SWITCH3, SWITCH4, SWITCH5, SWITCHEROO, SWORDS_DANCE, SYNTHESIS, TACKLE, TAIL_GLOW, TAIL_WHIP, TAILWIND, TAKE_DOWN, TAUNT, TEETER_DANCE, TELEPORT, THIEF, THRASH, THUNDER, THUNDER_FANG, THUNDER_WAVE, THUNDERBOLT, THUNDERPUNCH, THUNDERSHOCK, TICKLE, TORMENT, TOXIC, TOXIC_SPIKES, TRANSFORM, TRI_ATTACK, TRICK, TRICK_ROOM, TRIPLE_KICK, TRUMP_CARD, TWINEEDLE, TWISTER, U_TURN, UPROAR, VACUUM_WAVE, VICEGRIP, VINE_WHIP, VITAL_THROW, VOLT_TACKLE, WAKE_UP_SLAP, WATER_GUN, WATER_PULSE, WATER_SPORT, WATER_SPOUT, WATERFALL, WEATHER_BALL, WHIRLPOOL, WHIRLWIND, WILL_O_WISP, WING_ATTACK, WISH, WITHDRAW, WOOD_HAMMER, WORRY_SEED, WRAP, WRING_OUT, X_SCISSOR, YAWN, ZAP_CANNON, ZEN_HEADBUTT, END_MOVE };

		Moves name;
		Type type;
		int64_t score;
		int16_t basepower;
		int16_t power;
		bool execute;
		bool physical;
		bool selectable;
		int8_t accuracy;		// A number between 0 (1?) and 100, according to poccil. -1 means never miss
		int8_t disable;			// Number of turns left on this move being Disabled (4-7)
		int8_t pp_max;			// PP after all PP ups are applied
		int8_t pp;
		int8_t priority;
		int8_t r;					// The random number (85 through 100)
		int8_t times_used;
		// variable is used for moves with a variable power / length / other integer range. Moves of variable power: Magnitude = 4-10, Psywave = 5-15, Present = 0-4 (0 = heal). It is also used to determine whether random effects activate. First value is the magnitude of the effect, second value is the probability.
		Active <std::pair <uint16_t, uint16_t> > variable;

		static std::string const name_to_string [];
		static uint16_t const max_probability;

		Move (Moves move, int pp_ups, unsigned size);
		uint64_t hash() const;
		std::string get_name () const;			// Gets the string version of the name.
		static Moves name_from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
		bool operator== (Move const & other) const;
		bool operator!= (Move const & other) const;
		bool is_switch () const;
		static bool is_switch (Moves name);
		bool is_struggle_or_switch () const;
		bool is_phaze () const;
		static bool is_phaze (Moves name);
		bool is_healing () const;
		static bool is_healing (Moves name);
		bool is_blocked_by_gravity () const;
		bool is_usable_while_sleeping () const;
		bool is_usable_while_frozen () const;
		bool is_self_KO () const;
		void get_magnitude (unsigned magnitude);
		static bool compare_scores (Move const & first, Move const & second);
	private:
		void set_priority ();
		void set_variable (unsigned size);

		static Type const move_type [];
		static bool const is_physical [];
		static int16_t const base_power [];
		static int8_t const get_pp [];
		static uint16_t const get_probability [];
		static int16_t const get_accuracy [];
};

// Various states a Pokemon can be in due to vanishing moves.

enum Vanish { LANDED, BOUNCED, DUG, DIVED, FLOWN, SHADOW_FORCED };

class Pokemon;
class Team;
class Weather;

unsigned usemove (Team & user, Team & target, Weather & weather, unsigned log_damage = -1);
unsigned usemove2 (Team & user, Team & target, Weather & weather, unsigned log_damage);
void call_other_move (Team & user);
void do_damage (Team & user, Team & target, unsigned damage);
void lower_pp (Team & user, Pokemon const & target);

}

#endif		// MOVE_H_
