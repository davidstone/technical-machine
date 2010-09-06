// Use moves
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MOVE_CPP_
#define MOVE_CPP_

#include "move.h"
#include "ability.h"
#include "damage.cpp"
#include "pokemon.h"
#include "simple.h"
#include "type.h"
#include "weather.h"

void usemove (teams &user, teams &target, weathers &weather, bool hitself);
void usemove2 (teams &user, teams &target, weathers &weather);
void switchpokemon (teams &team, const pokemon &memberin, const weathers &weather);
void blockexecution (teams &user, const teams &target, const weathers &weather, bool hitself);
void blockselection (teams &user, const teams &target, const weathers &weather);
bool block1 (const teams &user, const teams &target);
bool block2 (const teams &user, const weathers &weather);
bool imprison (const teams &user, const teams &target);

void blockselection (teams &user, const teams &target, const weathers &weather) {
	user.active->move->selectable = true;
	if ((block1 (user, target))
	 or (block2 (user, weather))
	 or (user.active->torment and 0 != user.active->move->times_used))
		user.active->move->selectable = false;
	else if (0 != user.active->encore or CHOICE_BAND == user.active->item or CHOICE_SCARF == user.active->item or CHOICE_SPECS == user.active->item) {
		for (std::vector<moves>::iterator it = user.active->moveset.begin(); it != user.active->moveset.end(); ++it) {
			if (it->name != user.active->move->name and it->times_used != 0)
				user.active->move->selectable = false;
		}
	}
}

void blockexecution (teams &user, const teams &target, const weathers &weather, bool hitself) {
	user.active->move->execute = true;
	if (FREEZE == user.active->status and (FLAME_WHEEL != user.active->move->name and SACRED_FIRE != user.active->move->name))
		user.active->move->execute = false;

	else if (SLEEP == user.active->status) {
		if (EARLY_BIRD == user.active->ability and user.active->sleep >= 2)
			user.active->sleep -= 2;
		else
			--user.active->sleep;
		if (0 == user.active->sleep)
			user.active->status = NO_STATUS;
		else if (SLEEP_TALK != user.active->move->name and SNORE != user.active->move->name)
			user.active->move->execute = false;
	}

	if (block1 (user, target)
	 or (TRUANT == user.active->ability and user.active->loaf))
		user.active->move->execute = false;

	if (user.active->move->execute and 0 != user.active->confused) {
		if (hitself) {
			// fix
			user.active->move->execute = false;
		}
		else
			--user.active->confused;
	}
	if (user.active->move->execute and user.active->flinch) {
		if (STEADFAST == user.active->ability)
			statboost (user.active->spe, 1);
		user.active->move->execute = false;
	}
	
	if (block2 (user, weather))
		user.active->move->execute = false;
}

bool block1 (const teams &user, const teams &target) {		// Things that both block selection and block execution in between sleep and confusion
	if ((0 == user.active->move->pp)
	 or (0 != user.active->move->disable)
	 or (0 != user.active->heal_block and (HEAL_ORDER == user.active->move->name or MILK_DRINK == user.active->move->name or MOONLIGHT == user.active->move->name or MORNING_SUN == user.active->move->name or RECOVER == user.active->move->name or REST == user.active->move->name or ROOST == user.active->move->name or SLACK_OFF == user.active->move->name or SOFTBOILED == user.active->move->name or SWALLOW == user.active->move->name or SYNTHESIS == user.active->move->name or WISH == user.active->move->name))
	  or (imprison (user, target)))
		return true;
	return false;
}

bool imprison (const teams &user, const teams &target) {
	if (target.active->imprison) {
		for (std::vector<moves>::iterator it = target.active->moveset.begin(); it != target.active->moveset.end(); ++it) {
			if (user.active->move->name == it->name)
				return true;
		}
	}
	return false;
}

bool block2 (const teams &user, const weathers &weather) {		// Things that both block selection and block execution after flinching
	if ((0 != user.active->taunt and 0 == user.active->move->basepower)
	 or (0 < weather.gravity and (BOUNCE == user.active->move->name or FLY == user.active->move->name or HI_JUMP_KICK == user.active->move->name or JUMP_KICK == user.active->move->name or MAGNET_RISE == user.active->move->name or SPLASH == user.active->move->name)))
		return true;
	return false;
}

void usemove (teams &user, teams &target, weathers &weather, bool hitself) {
	blockexecution (user, target, weather, hitself);
	if (user.active->move->execute) {
		if (PRESSURE == target.active->ability and 2 <= user.active->move->pp)
			user.active->move->pp -= 2;
		else
			--user.active->move->pp;

//		if (ASSIST == user.active->move->name or COPYCAT == user.active->move->name or ME_FIRST == user.active->move->name or METRONOME_MOVE == user.active->move->name or MIRROR_MOVE == user.active->move->name or SLEEP_TALK == user.active->move->name)
//			usemove2 (user, target, move2, weather);		// ???
//		 (NATURE_POWER == user.active->move->name)
//		else
			usemove2 (user, target, weather);
	
		++user.active->move->times_used;
	}
}

void usemove2 (teams &user, teams &target, weathers &weather) {
	speed (user, weather);
	speed (target, weather);
	movepower (*user.active, *target.active, weather);
	int damage;
	
	if (0 != user.active->move->basepower) {
		if (BRICK_BREAK == user.active->move->name) {
			target.reflect = 0;
			target.light_screen = 0;
		}
		else if (FLAME_WHEEL == user.active->move->name or SACRED_FIRE == user.active->move->name) {
			if (user.active->status == FREEZE)
				user.active->status = NO_STATUS;
		}
		defense (*user.active, *target.active, weather);
		attack (*user.active, weather);
		damage = damagecalculator (*user.active, target, weather);
		if (damage != 0) {
			target.active->hp.stat -= damage;
			if (user.active->item == LIFE_ORB)
				heal (*user.active, -10);
		}
	}
	
	if (ABSORB == user.active->move->name or DRAIN_PUNCH == user.active->move->name or GIGA_DRAIN == user.active->move->name or LEECH_LIFE == user.active->move->name or MEGA_DRAIN == user.active->move->name) {
		if (LIQUID_OOZE == target.active->ability) {
			user.active->hp.stat -= damage / 2;
			if (user.active->hp.stat < 0)
				user.active->hp.stat = 0;
		}
		else {
			user.active->hp.stat += damage / 2;
			if (user.active->hp.stat > user.active->hp.max)
				user.active->hp.stat = user.active->hp.max;
		}
	}
	else if (ACID_ARMOR == user.active->move->name or BARRIER == user.active->move->name or IRON_DEFENSE == user.active->move->name)
		statboost (user.active->def, 2);
//	else if (ACCUPRESSURE == user.active->move->name)
	else if (AGILITY == user.active->move->name or ROCK_POLISH == user.active->move->name)
		statboost (user.active->spe, 2);
	else if (AMNESIA == user.active->move->name)
		statboost (user.active->spd, 2);
	else if (AQUA_RING == user.active->move->name)
		user.active->aqua_ring = true;
	else if (AROMATHERAPY == user.active->move->name) {
		for (std::vector<pokemon>::iterator it = user.member.begin(); it != user.member.end(); ++it)
			it->status = NO_STATUS;
	}
	else if (ATTRACT == user.active->move->name and static_cast<int> (user.active->gender) + static_cast<int> (target.active->gender) == 1)		// male + female == 1
			target.active->attract = true;
//	else if (BATON_PASS == user.active->move->name)
	else if (BELLY_DRUM == user.active->move->name) {
		user.active->atk.stage = 6;
		if (user.active->hp.max <= 2)
			--user.active->hp.stat;
		else
			user.active->hp.stat -= user.active->hp.max / 2;
	}
//	else if (BIDE == user.active->move->name)
//	else if (BLAST_BURN == user.active->move->name or FRENZY_PLANT == user.active->move->name or GIGA_IMPACT == user.active->move->name or HYDRO_CANNON == user.active->move->name or HYPER_BEAM == user.active->move->name or ROAR_OF_TIME == user.active->move->name or ROCK_WRECKER == user.active->move->name)
	else if (BLOCK == user.active->move->name or MEAN_LOOK == user.active->move->name or SPIDER_WEB == user.active->move->name)
		target.active->trapped = true;
	else if (BRAVE_BIRD == user.active->move->name or DOUBLE_EDGE == user.active->move->name or WOOD_HAMMER == user.active->move->name)
		recoil (*user.active, damage, 3);
//	else if (BUG_BITE == user.active->move->name or PLUCK == user.active->move->name)
	else if (BULK_UP == user.active->move->name) {
		statboost (user.active->atk, 1);
		statboost (user.active->def, 1);
	}
	else if (CALM_MIND == user.active->move->name) {
		statboost (user.active->spa, 1);
		statboost (user.active->spd, 1);
	}
//	else if (CAMOUFLAGE == user.active->move->name)
	else if (CAPTIVATE == user.active->move->name and static_cast<int> (user.active->gender) + static_cast<int> (target.active->gender) == 1)		// male + female == 1
		statboost (target.active->spd, -2);
	else if (CHARGE == user.active->move->name) {
		user.active->charge = true;
		statboost (user.active->spd, 1);
	}
	else if (CHARM == user.active->move->name or FEATHERDANCE == user.active->move->name)
		statboost (target.active->atk, -2);
	else if (CLOSE_COMBAT == user.active->move->name) {
		statboost (user.active->atk, -1);
		statboost (user.active->def, -1);
	}
	else if (COSMIC_POWER == user.active->move->name or DEFEND_ORDER == user.active->move->name) {
		statboost (user.active->def, 1);
		statboost (user.active->spd, 1);
	}
	else if (COTTON_SPORE == user.active->move->name or SCARY_FACE == user.active->move->name)
		statboost (target.active->spe, -2);
//	else if (COUNTER == user.active->move->name)
	else if (COVET == user.active->move->name or THIEF == user.active->move->name) {
		if (user.active->item == NO_ITEM)
			user.active->item = target.active->item;
	}
	else if (CURSE == user.active->move->name) {
		if (istype (*user.active, GHOST) and user.active->ability != MAGIC_GUARD and target.active->curse == false) {
			if (user.active->hp.max <= 4)
				--user.active->hp.stat;
			else
				user.active->hp.stat -= user.active->hp.max / 2;
			target.active->curse = true;
		}
		else {
			statboost (user.active->atk, 1);
			statboost (user.active->def, 1);
			statboost (user.active->spe, -1);
		}
	}
	else if (DEFENSE_CURL == user.active->move->name) {
		statboost (user.active->def, 1);
		user.active->defense_curl = true;
	}
	else if (DEFOG == user.active->move->name) {
//		statboost (target.active->evasion, -1);
		weather.fog = false;
	}
//	else if (DESTINY_BOND == user.active->move->name)
//	else if (DETECT == user.active->move->name or PROTECT == user.active->move->name)
//	else if (DISABLE == user.active->move->name)
//	else if (DOOM_DESIRE == user.active->move->name or FUTURE_SIGHT == user.active->move->name)
//	else if (DOUBLE_TEAM == user.active->move->name or MINIMIZE == user.active->move->name)
//		statboost (user.active->evasion, 1);
	else if (DRACO_METEOR == user.active->move->name or LEAF_STORM == user.active->move->name or OVERHEAT == user.active->move->name or PSYCHO_BOOST == user.active->move->name)
		statboost (user.active->spa, -2);
	else if (DRAGON_DANCE == user.active->move->name) {
		statboost (user.active->atk, 1);
		statboost (user.active->spe, 1);
	}
	else if (DREAM_EATER == user.active->move->name and target.active->status == SLEEP) {
		user.active->hp.stat += damage / 2;
		if (user.active->hp.stat > user.active->hp.max)
			user.active->hp.stat = user.active->hp.max;
	}
	else if (EMBARGO == user.active->move->name)
		target.active->embargo = 5;
//	else if (ENDURE == user.active->move->name)
	else if (EXPLOSION == user.active->move->name or SELFDESTRUCT == user.active->move->name)
		user.active->hp.stat = 0;
	else if (FAKE_OUT == user.active->move->name)
		target.active->flinch = true;
	else if (FAKE_TEARS == user.active->move->name or METAL_SOUND == user.active->move->name)
		statboost (target.active->spd, -2);
//	else if (FEINT == user.active->move->name)
//	else if (FLASH == user.active->move->name or KINESIS == user.active->move->name or MUD_SLAP == user.active->move->name or SAND_ATTACK == user.active->move->name or SMOKESCREEN == user.active->move->name)
//		statboost (target.active->accuracy, -1);
	else if (FLATTER == user.active->move->name)
		statboost (target.active->spa, 1);
	else if (FLING == user.active->move->name)
		user.active->item = NO_ITEM;
	else if (FOCUS_ENERGY == user.active->move->name)
		user.active->focus_energy = true;
//	else if (FOCUS_PUNCH == user.active->move->name)
//	else if (FOLLOW_ME == user.active->move->name)
	else if (FORESIGHT == user.active->move->name or ODOR_SLEUTH == user.active->move->name)
		target.active->identified = true;
//	else if (GASTRO_ACID == user.active->move->name)
	else if ((GLARE == user.active->move->name or STUN_SPORE == user.active->move->name or THUNDER_WAVE == user.active->move->name or ZAP_CANNON == user.active->move->name) and target.active->status == NO_STATUS)
		target.active->status = PARALYSIS;
//	else if (GRAVITY == user.active->move->name)
	else if (GROWL == user.active->move->name)
		statboost (target.active->atk, -1);
	else if (GROWTH == user.active->move->name)
		statboost (user.active->spa, 1);
//	else if (GRUDGE == user.active->move->name)
	else if (GUARD_SWAP == user.active->move->name) {
		std::swap (user.active->def.stage, target.active->def.stage);
		std::swap (user.active->spd.stage, target.active->spd.stage);
	}
//	else if (HAIL == user.active->move->name)
	else if (HAMMER_ARM == user.active->move->name)
		statboost (user.active->spe, -1);
	else if (HARDEN == user.active->move->name or WITHDRAW == user.active->move->name)
		statboost (user.active->def, 1);
	else if (HAZE == user.active->move->name) {
		user.active->atk.stage = 0;
		user.active->def.stage = 0;
		user.active->spa.stage = 0;
		user.active->spd.stage = 0;
		user.active->spe.stage = 0;
		target.active->atk.stage = 0;
		target.active->def.stage = 0;
		target.active->spa.stage = 0;
		target.active->spd.stage = 0;
		target.active->spe.stage = 0;
	}
	else if (HEAD_SMASH == user.active->move->name)
		recoil (*user.active, damage, 2);
	else if (HEAL_BELL == user.active->move->name) {
		for (std::vector<pokemon>::iterator it = user.member.begin(); it != user.member.end(); ++it) {
			if (it->ability != SOUNDPROOF)
				it->status = NO_STATUS;
		}
	}
//	else if (HEAL_BLOCK == user.active->move->name)
	else if (HEAL_ORDER == user.active->move->name or MILK_DRINK == user.active->move->name or RECOVER == user.active->move->name or SLACK_OFF == user.active->move->name or SOFTBOILED == user.active->move->name)
		heal (*user.active, 2);
//	else if (HEALING_WISH == user.active->move->name)
	else if (HEART_SWAP == user.active->move->name) {
		std::swap (user.active->atk.stage, target.active->atk.stage);
		std::swap (user.active->def.stage, target.active->def.stage);
		std::swap (user.active->spa.stage, target.active->spa.stage);
		std::swap (user.active->spd.stage, target.active->spd.stage);
		std::swap (user.active->spe.stage, target.active->spe.stage);
	}
//	else if (HI_JUMP_KICK == user.active->move->name or JUMP_KICK == user.active->move->name)
	else if (HOWL == user.active->move->name or MEDITATE == user.active->move->name or SHARPEN == user.active->move->name)
		statboost (user.active->atk, 1);
//	else if (ICE_BALL == user.active->move->name or ROLLOUT == user.active->move->name)
	else if (ICY_WIND == user.active->move->name or MUD_SHOT == user.active->move->name or ROCK_TOMB == user.active->move->name or STRING_SHOT == user.active->move->name)
		statboost (target.active->spe, -1);
	else if (IMPRISON == user.active->move->name)
		user.active->imprison = true;
	else if (INGRAIN == user.active->move->name)
		user.active->ingrain = true;
//	else if (KNOCK_OFF == user.active->move->name)
	else if (LEECH_SEED == user.active->move->name)
		target.active->leech_seed = true;
	else if (LEER == user.active->move->name or TAIL_WHIP == user.active->move->name)
		statboost (target.active->def, -1);
	else if (LIGHT_SCREEN == user.active->move->name and user.light_screen == 0) {
		if (user.active->item == LIGHT_CLAY)
			user.light_screen = 8;
		else
			user.light_screen = 5;
	}
//	else if (LOCK_ON == user.active->move->name or MIND-READER == user.active->move->name)
	else if (LUCKY_CHANT == user.active->move->name and user.lucky_chant == 0)
		user.lucky_chant = 5;
//	else if (LUNAR_DANCE == user.active->move->name)
//	else if (MAGIC_COAT == user.active->move->name)
	else if (MAGNET_RISE == user.active->move->name and user.active->magnet_rise == 0)
		user.active->magnet_rise = 5;
//	else if (ME_FIRST == user.active->move->name)
	else if (MEMENTO == user.active->move->name) {
		statboost (target.active->atk, -2);
		statboost (target.active->spa, -2);
		user.active->hp.stat = 0;
	}
//	else if (METAL_BURST == user.active->move->name)
//	else if (MIMIC == user.active->move->name)
//	else if (MIRACLE_EYE == user.active->move->name)
//	else if (MIRROR_COAT == user.active->move->name)
//	else if (MIST == user.active->move->name)
//	else if (MOONLIGHT == user.active->move->name or MORNING_SUN == user.active->move->name or SYNTHESIS == user.active->move->name)
	else if (MUD_SPORT == user.active->move->name)
		user.active->mud_sport = true;
	else if (NASTY_PLOT == user.active->move->name or TAIL_GLOW == user.active->move->name)
		statboost (user.active->spa, 2);
	else if (NIGHTMARE == user.active->move->name)
		target.active->nightmare = true;
//	else if (OUTRAGE == user.active->move->name or PETAL_DANCE == user.active->move->name or THRASH == user.active->move->name)
	else if (PAIN_SPLIT == user.active->move->name) {
		user.active->hp.stat = (user.active->hp.stat + target.active->hp.stat) / 2;
		target.active->hp.stat = user.active->hp.stat;
		if (user.active->hp.stat > user.active->hp.max)
			user.active->hp.stat = user.active->hp.max;
		if (target.active->hp.stat > target.active->hp.max)
			target.active->hp.stat = target.active->hp.max;
	}
	else if (PERISH_SONG == user.active->move->name) {
		if (user.active->perish_song == 0)
			user.active->perish_song = 3;
		if (target.active->perish_song == 0)
			target.active->perish_song = 3;
	}
	else if ((POISON_GAS == user.active->move->name or POISONPOWDER == user.active->move->name) and target.active->status == NO_STATUS)
		target.active->status = POISON_NORMAL;
	else if (POWER_SWAP == user.active->move->name) {
		std::swap (user.active->atk.stage, target.active->atk.stage);
		std::swap (user.active->spa.stage, target.active->spa.stage);
	}
//	else if (POWER_TRICK == user.active->move->name) {
	else if (PSYCH_UP == user.active->move->name) {
		user.active->atk.stage = target.active->atk.stage;
		user.active->def.stage = target.active->def.stage;
		user.active->spa.stage = target.active->spa.stage;
		user.active->spd.stage = target.active->spd.stage;
		user.active->spe.stage = target.active->spe.stage;
	}
	else if (PSYCHO_SHIFT == user.active->move->name and target.active->status == NO_STATUS) {
		target.active->status = user.active->status;
		user.active->status = NO_STATUS;
	}
//	else if (RAGE == user.active->move->name)
//	else if (RAIN_DANCE == user.active->move->name)
	else if (RAPID_SPIN == user.active->move->name) {
		user.spikes = 0;
		user.stealth_rock = false;
		user.toxic_spikes = 0;
		user.active->leech_seed = false;
		user.active->partial_trap = false;
	}
//	else if (RAZOR_WIND == user.active->move->name)
//	else if (RECYCLE == user.active->move->name)
	else if (REFLECT == user.active->move->name and user.reflect == 0) {
		if (user.active->item == LIGHT_CLAY)
			user.reflect = 8;
		else
			user.reflect = 5;
	}
	else if (REFRESH == user.active->move->name)
		user.active->status = NO_STATUS;
	else if (REST == user.active->move->name and user.active->hp.stat != user.active->hp.max) {
		user.active->hp.stat = user.active->hp.max;
		user.active->status = SLEEP;
		user.active->sleep = 3;
	}
//	else if (ROLE_PLAY == user.active->move->name)
	else if (ROOST == user.active->move->name) {
		user.active->roost = true;
		heal (*user.active, 2);
	}
	else if (SAFEGUARD == user.active->move->name and user.safeguard == 0)
		user.safeguard = 5;
//	else if (SANDSTORM == user.active->move->name)
	else if (SCREECH == user.active->move->name)
		statboost (target.active->def, -2);
//	else if (SKETCH == user.active->move->name)
//	else if (SKILL_SWAP == user.active->move->name)
//	else if (SKULL_BASH == user.active->move->name)
	else if (SMELLINGSALT == user.active->move->name and target.active->status == PARALYSIS)
		target.active->status = NO_STATUS;
//	else if (SNATCH == user.active->move->name)
//	else if (SOLARBEAM == user.active->move->name)
	else if (SPIKES == user.active->move->name and target.spikes < 3)
		++target.spikes;
//	else if (SPIT_UP == user.active->move->name)
//	else if (SPITE == user.active->move->name)
	else if (STEALTH_ROCK == user.active->move->name)
		target.stealth_rock = true;
//	else if (STOCKPILE == user.active->move->name)
	else if (STRUGGLE == user.active->move->name)
		user.active->hp.stat -= user.active->hp.max / 4;
	else if (SUBMISSION == user.active->move->name or TAKE_DOWN == user.active->move->name)
		recoil (*user.active, damage, 4);
//	else if (SUBSTITUTE == user.active->move->name)
//	else if (SUNNY_DAY == user.active->move->name)
	else if (SUPERPOWER == user.active->move->name) {
		statboost (user.active->atk, -1);
		statboost (user.active->def, -1);
	}
//	else if (SWAGGER == user.active->move->name)
//	else if (SWALLOW == user.active->move->name)
//	else if (SWEET_SCENT == user.active->move->name)
//		statboost (target.active->evasion, -1);
	else if (SWITCHEROO == user.active->move->name or TRICK == user.active->move->name)
		std::swap (user.active->item, target.active->item);
	else if (SWORDS_DANCE == user.active->move->name)
		statboost (user.active->atk, 2);
	else if (TAILWIND == user.active->move->name and user.tailwind == 0)
		user.tailwind = 3;
	else if (TICKLE == user.active->move->name) {
		statboost (target.active->atk, -1);
		statboost (target.active->def, -1);
	}
	else if (TORMENT == user.active->move->name)
		target.active->torment = true;
	else if (TOXIC == user.active->move->name)
		user.active->status = POISON_TOXIC;
	else if (TOXIC_SPIKES == user.active->move->name and target.toxic_spikes < 2)
		++target.toxic_spikes;
//	else if (TRANSFORM == user.active->move->name)
//	else if (TRICK_ROOM == user.active->move->name)
//	else if (U_TURN == user.active->move->name)
	else if (WAKE_UP_SLAP == user.active->move->name and target.active->status == SLEEP)
		target.active->status = NO_STATUS;
	else if (WATER_SPORT == user.active->move->name)
		user.active->water_sport = true;
	else if (WILL_O_WISP == user.active->move->name and user.active->status == NO_STATUS and istype (*target.active, FIRE) == false and target.active->ability != WATER_VEIL and (weather.sun == 0 or target.active->ability != LEAF_GUARD))
		user.active->status = BURN;
	else if (WISH == user.active->move->name and user.wish == 0)
		user.wish = 2;

//	else if (WORRY_SEED == user.active->move->name)
//	else if (SHADOW_FORCE == user.active->move->name)
//	else if (DIG == user.active->move->name)
//	else if (DIVE == user.active->move->name)
//	else if (FLY == user.active->move->name)
//	else if (BOUNCE == user.active->move->name)
	
	
	
	
	
	
	
/*	else if (ACID == user.active->move->name or BUG_BUZZ == user.active->move->name or EARTH_POWER == user.active->move->name or ENERGY_BALL == user.active->move->name or FLASH_CANNON == user.active->move->name or FOCUS_BLAST == user.active->move->name or PSYCHIC == user.active->move->name)
	else if (ANCIENT_POWER == user.active->move->name or OMINOUS_WIND == user.active->move->name or SILVER_WIND == user.active->move->name)
	else if (BLAZE_KICK == user.active->move->name)
	else if (BIND == user.active->move->name or CLAMP == user.active->move->name or FIRE_SPIN == user.active->move->name or MAGMA_STORM == user.active->move->name or SAND_TOMB == user.active->move->name or WHIRLPOOL == user.active->move->name or WRAP == user.active->move->name)
	else if (CHATTER == user.active->move->name)
	else if (CONFUSE_RAY == user.active->move->name or DYNAMICPUNCH == user.active->move->name or SUPERSONIC == user.active->move->name or SWEET_KISS == user.active->move->name or TEETER_DANCE == user.active->move->name)
	else if (CONVERSION == user.active->move->name)
	else if (CONVERSION2 == user.active->move->name)
	else if (CROSS_POISON == user.active->move->name)
	else if ((DARK_VOID == user.active->move->name or GRASSWHISTLE == user.active->move->name or HYPNOSIS == user.active->move->name or LOVELY_KISS == user.active->move->name or SING == user.active->move->name or SLEEP_POWDER == user.active->move->name or SPORE == user.active->move->name) and target.active->status == NO_STATUS)
		target.active->status = SLEEP;
	else if (EMBER == user.active->move->name or FIRE_BLAST == user.active->move->name or FIRE_PUNCH == user.active->move->name or FLAMETHROWER == user.active->move->name or HEAT_WAVE == user.active->move->name)
	else if (ENCORE == user.active->move->name)
	else if (FLARE_BLITZ == user.active->move->name)
	else if (METAL_CLAW == user.active->move->name)
	else if (POISON_TAIL == user.active->move->name)
	else if (ROAR == user.active->move->name)
	else if (SECRET_POWER == user.active->move->name)
	else if (SNORE == user.active->move->name)
	else if (SKY_ATTACK == user.active->move->name)
	else if (TAUNT == user.active->move->name)
	else if (UPROAR == user.active->move->name)
	else if (WHIRLWIND == user.active->move->name)
	else if (YAWN == user.active->move->name)
*/
}

void switchpokemon (teams &team, const pokemon &memberin, const weathers &weather) {

	// Cure the status of a Natural Cure Pokemon as it switches out
	if (NATURAL_CURE == team.active->ability)
		team.active->status = NO_STATUS;
	
	// Change the active Pokemon to the one switching in. Initialize all variables that switches reset.
	*team.active = memberin;
	team.active->aqua_ring = false;
	team.active->charge = false;
	team.active->curse = false;
	team.active->damaged = false;
	team.active->defense_curl = false;
	team.active->ff = false;
	team.active->flinch = false;
	team.active->heal_block = false;
	team.active->imprison = false;
	team.active->ingrain = false;
	team.active->item_recycle = false;
	team.active->item_unburden = false;
	team.active->leech_seed = false;
	team.active->loaf = false;			// Do I set to true or false? True makes it wrong when a fainted Pokemon is replaced; false makes it wrong otherwise
	team.active->mf = false;
	team.active->minimize = false;
	team.active->moved = true;
	team.active->mud_sport = false;
	team.active->nightmare = false;
	team.active->roost = false;
	team.active->torment = false;
	team.active->trapped = false;
	team.active->water_sport = false;
	team.active->confused = 0;
	team.active->embargo = 0;
	team.active->encore = 0;
	team.active->magnet_rise = 0;
	team.active->partial_trap = 0;
	team.active->perish_song = 0;
	team.active->rampage = 0;
	team.active->atk.stage = 0;
	team.active->def.stage = 0;
	team.active->spa.stage = 0;
	team.active->spd.stage = 0;
	team.active->spe.stage = 0;
	team.active->stockpile = 0;
	team.active->taunt = 0;
	team.active->uproar = 0;
	team.active->vanish = LANDED;
	team.active->yawn = 0;

	if (SLOW_START == memberin.ability)
		team.active->slow_start = 5;
	else
		team.active->slow_start = 0;
	for (std::vector<moves>::iterator it = team.active->moveset.begin(); it != team.active->moveset.end(); ++it) {
		it->disable = 0;
		it->times_used = 0;
	}
	if (grounded (*team.active, weather) and MAGIC_GUARD != team.active->ability) {
		if (0 != team.toxic_spikes) {
			if (istype(*team.active, POISON))
				team.toxic_spikes = 0;
			else if (1 == team.toxic_spikes)
				team.active->status = POISON_NORMAL;
			else
				team.active->status = POISON_TOXIC;
		}
		if (0 != team.spikes)
			heal (*team.active, -16, team.spikes + 1);
	}
	if (team.stealth_rock)
		heal (*team.active, -32, effectiveness (ROCK, team.active->type1) * effectiveness (ROCK, team.active->type2));	// effectiveness () outputs a value between 1 and 4, with higher numbers being more effective. 4 * effective Stealth Rock does 16 / 32 damage.
}

#endif
