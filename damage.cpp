// Damage calculator
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "damage.h"
#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

void movepower (Team &attacker, const Team &defender, const Weather weather) {

	// I account for the doubling of the base power for Pursuit in the switching function by simply multiplying the final base power by 2. Regardless of the combination of modifiers, this does not change the final base power. The exception is if the attacker's ally uses Helping Hand. The target uses U-turn and the attacker uses Pursuit with a slower Pokemon that has Rivalry and a Muscle Band and neither the attacker nor target is genderless. This will cause the base power to be 1 less than it should be.

	// Variable power moves

	if (CRUSH_GRIP == attacker.active->move->name or WRING_OUT == attacker.active->move->name)
		attacker.active->move->basepower = 120 * defender.active->hp.stat / defender.active->hp.max + 1;

	else if (ERUPTION == attacker.active->move->name or WATER_SPOUT == attacker.active->move->name)
		attacker.active->move->basepower = 150 * attacker.active->hp.stat / attacker.active->hp.max;

	else if (FLAIL == attacker.active->move->name or REVERSAL == attacker.active->move->name) {
		if (64 * attacker.active->hp.stat / attacker.active->hp.max <= 1)
			attacker.active->move->basepower = 200;
		else if (64 * attacker.active->hp.stat / attacker.active->hp.max <= 5)
			attacker.active->move->basepower = 150;
		else if (64 * attacker.active->hp.stat / attacker.active->hp.max <= 12)
			attacker.active->move->basepower = 100;
		else if (64 * attacker.active->hp.stat / attacker.active->hp.max <= 21)
			attacker.active->move->basepower = 80;
		else if (64 * attacker.active->hp.stat / attacker.active->hp.max <= 42)
			attacker.active->move->basepower = 40;
		else
			attacker.active->move->basepower = 20;
	}

	else if (FLING == attacker.active->move->name) {
		if (IRON_BALL == attacker.active->item)
			attacker.active->move->basepower = 130;
		else if (HARD_STONE == attacker.active->item or OTHER100 == attacker.active->item)
			attacker.active->move->basepower = 100;
		else if (DEEPSEATOOTH == attacker.active->item or DRACO_PLATE == attacker.active->item or DREAD_PLATE == attacker.active->item or EARTH_PLATE == attacker.active->item or FIST_PLATE == attacker.active->item or FLAME_PLATE == attacker.active->item or GRIP_CLAW == attacker.active->item or ICICLE_PLATE == attacker.active->item or INSECT_PLATE == attacker.active->item or IRON_PLATE == attacker.active->item or MEADOW_PLATE == attacker.active->item or MIND_PLATE == attacker.active->item or SKY_PLATE == attacker.active->item or SPLASH_PLATE == attacker.active->item or SPOOKY_PLATE == attacker.active->item or STONE_PLATE == attacker.active->item or THICK_CLUB == attacker.active->item or TOXIC_PLATE == attacker.active->item or ZAP_PLATE == attacker.active->item)
			attacker.active->move->basepower = 90;
		else if (QUICK_CLAW == attacker.active->item or RAZOR_CLAW == attacker.active->item or STICKY_BARB == attacker.active->item or OTHER80 == attacker.active->item)
			attacker.active->move->basepower = 80;
		else if (DRAGON_FANG == attacker.active->item or POISON_BARB == attacker.active->item or POWER_ITEMS == attacker.active->item)
			attacker.active->move->basepower = 70;
		else if (ADAMANT_ORB == attacker.active->item or DAMP_ROCK == attacker.active->item or HEAT_ROCK == attacker.active->item or LUSTROUS_ORB == attacker.active->item or MACHO_BRACE == attacker.active->item or STICK == attacker.active->item)
			attacker.active->move->basepower = 60;
		else if (SHARP_BEAK == attacker.active->item or OTHER50 == attacker.active->item)
			attacker.active->move->basepower = 50;
		else if (ICY_ROCK == attacker.active->item or LUCKY_PUNCH == attacker.active->item)
			attacker.active->move->basepower = 40;
		else if (BERRY_JUICE == attacker.active->item or BLACK_BELT == attacker.active->item or BLACK_SLUDGE == attacker.active->item or BLACKGLASSES == attacker.active->item or CHARCOAL == attacker.active->item or DEEPSEASCALE == attacker.active->item or FLAME_ORB == attacker.active->item or KINGS_ROCK == attacker.active->item or LIFE_ORB == attacker.active->item or LIGHT_BALL == attacker.active->item or LIGHT_CLAY == attacker.active->item or MAGNET == attacker.active->item or METAL_COAT == attacker.active->item or METRONOME_ITEM == attacker.active->item or MIRACLE_SEED == attacker.active->item or MYSTIC_WATER == attacker.active->item or NEVERMELTICE == attacker.active->item or SCOPE_LENS == attacker.active->item or SHELL_BELL == attacker.active->item or SOUL_DEW == attacker.active->item or SPELL_TAG == attacker.active->item or TOXIC_ORB == attacker.active->item or TWISTEDSPOON == attacker.active->item or OTHER30 == attacker.active->item)
			attacker.active->move->basepower = 30;
		else
			attacker.active->move->basepower = 10;
	}

	else if (FRUSTRATION == attacker.active->move->name or RETURN == attacker.active->move->name) {
		attacker.active->move->basepower = attacker.active->happiness * 2 / 5;
		if (FRUSTRATION == attacker.active->move->name)
			attacker.active->move->basepower = 102 - attacker.active->move->basepower;
	}

	else if (FURY_CUTTER == attacker.active->move->name)
		attacker.active->move->basepower = 10 << attacker.active->move->times_used;					// Equivalent to 10 * 2 ^ attacker.active->move->times_used

	else if (GRASS_KNOT == attacker.active->move->name or LOW_KICK == attacker.active->move->name)
		attacker.active->move->basepower = defender.active->mass;

	else if (attacker.active->move->name == GYRO_BALL) {
		attacker.active->move->basepower = 25 * defender.active->spe.stat / attacker.active->spe.stat + 1;
		if (attacker.active->move->basepower > 150)
			attacker.active->move->basepower = 150;
	}

	else if (ICE_BALL == attacker.active->move->name or ROLLOUT == attacker.active->move->name)
		attacker.active->move->basepower = 30 << attacker.active->move->times_used;					// Equivalent to 30 * 2 ^ attacker.active->move->times_used

	else if (HIDDEN_POWER == attacker.active->move->name) {
		// The second-least significant bit of each stat determines the power of Hidden Power
		int u = (attacker.active->hp.iv >> 1) % 2;
		int v = ((attacker.active->atk.iv >> 1) % 2) * 2;
		int w = ((attacker.active->def.iv >> 1) % 2) * 4;
		int x = ((attacker.active->spe.iv >> 1) % 2) * 8;
		int y = ((attacker.active->spa.iv >> 1) % 2) * 16;
		int z = ((attacker.active->spd.iv >> 1) % 2) * 32;
	
		attacker.active->move->basepower = (u + v + w + x + y + z) * 40 / 63 + 30;
	}

	else if (MAGNITUDE == attacker.active->move->name)
		attacker.active->move->basepower = *attacker.active->move->variable;

	else if (NATURAL_GIFT == attacker.active->move->name) {
		if (AGUAV_BERRY == attacker.active->item or ASPEAR_BERRY == attacker.active->item or BABIRI_BERRY == attacker.active->item or CHARTI_BERRY == attacker.active->item or CHERI_BERRY == attacker.active->item or CHESTO_BERRY == attacker.active->item or CHILAN_BERRY == attacker.active->item or CHOPLE_BERRY == attacker.active->item or COBA_BERRY == attacker.active->item or COLBUR_BERRY == attacker.active->item or FIGY_BERRY == attacker.active->item or HABAN_BERRY == attacker.active->item or IAPAPA_BERRY == attacker.active->item or KASIB_BERRY == attacker.active->item or KEBIA_BERRY == attacker.active->item or LEPPA_BERRY == attacker.active->item or LUM_BERRY == attacker.active->item or MAGO_BERRY == attacker.active->item or OCCA_BERRY == attacker.active->item or ORAN_BERRY == attacker.active->item or PASSHO_BERRY == attacker.active->item or PAYAPA_BERRY == attacker.active->item or PECHA_BERRY == attacker.active->item or PERSIM_BERRY == attacker.active->item or RAWST_BERRY == attacker.active->item or RAZZ_BERRY == attacker.active->item or RINDO_BERRY == attacker.active->item or SHUCA_BERRY == attacker.active->item or SITRUS_BERRY == attacker.active->item or TANGA_BERRY == attacker.active->item or WACAN_BERRY == attacker.active->item or WIKI_BERRY == attacker.active->item or YACHE_BERRY == attacker.active->item)
			attacker.active->move->basepower = 60;
		else if (BLUK_BERRY == attacker.active->item or CORNN_BERRY == attacker.active->item or GREPA_BERRY == attacker.active->item or HONDEW_BERRY == attacker.active->item or KELPSY_BERRY == attacker.active->item or MAGOST_BERRY == attacker.active->item or NANAB_BERRY == attacker.active->item or NOMEL_BERRY == attacker.active->item or PAMTRE_BERRY == attacker.active->item or PINAP_BERRY == attacker.active->item or POMEG_BERRY == attacker.active->item or QUALOT_BERRY == attacker.active->item or RABUTA_BERRY == attacker.active->item or SPELON_BERRY == attacker.active->item or TAMATO_BERRY == attacker.active->item or WEPEAR_BERRY == attacker.active->item)
			attacker.active->move->basepower = 70;
		else if (APICOT_BERRY == attacker.active->item or BELUE_BERRY == attacker.active->item or CUSTAP_BERRY == attacker.active->item or DURIN_BERRY == attacker.active->item or ENIGMA_BERRY == attacker.active->item or GANLON_BERRY == attacker.active->item or JABOCA_BERRY == attacker.active->item or LANSAT_BERRY == attacker.active->item or LIECHI_BERRY == attacker.active->item or MICLE_BERRY == attacker.active->item or PETAYA_BERRY == attacker.active->item or ROWAP_BERRY == attacker.active->item or SALAC_BERRY == attacker.active->item or STARF_BERRY == attacker.active->item or WATMEL_BERRY == attacker.active->item)
			attacker.active->move->basepower = 80;
	}

	else if (PRESENT == attacker.active->move->name)
		attacker.active->move->basepower = *attacker.active->move->variable;

	else if (PUNISHMENT == attacker.active->move->name) {
		attacker.active->move->basepower = 60;
		if (defender.active->atk.stage > 0)
			attacker.active->move->basepower += 20 * defender.active->atk.stage;
		if (defender.active->def.stage > 0)
			attacker.active->move->basepower += 20 * defender.active->def.stage;
		if (defender.active->spa.stage > 0)
			attacker.active->move->basepower += 20 * defender.active->spa.stage;
		if (defender.active->spd.stage > 0)
			attacker.active->move->basepower += 20 * defender.active->spd.stage;
		if (defender.active->spe.stage > 0)
			attacker.active->move->basepower += 20 * defender.active->spe.stage;
		if (attacker.active->move->basepower > 200)
			attacker.active->move->basepower = 200;
	}

	else if (SPIT_UP == attacker.active->move->name)
		attacker.active->move->basepower = attacker.stockpile * 100;

	else if (TRIPLE_KICK == attacker.active->move->name)
		attacker.active->move->basepower = 10 * attacker.active->move->times_used;

	else if (TRUMP_CARD == attacker.active->move->name) {
		if (0 == attacker.active->move->pp)
			attacker.active->move->basepower = 200;
		else if (1 == attacker.active->move->pp)
			attacker.active->move->basepower = 80;
		else if (2 == attacker.active->move->pp)
			attacker.active->move->basepower = 60;
		else if (3 == attacker.active->move->pp)
			attacker.active->move->basepower = 50;
		else
			attacker.active->move->basepower = 40;
	}

	attacker.active->move->power = attacker.active->move->basepower;

	if ((ASSURANCE == attacker.active->move->name and defender.damaged)
	 or ((AVALANCHE == attacker.active->move->name or REVENGE == attacker.active->move->name) and attacker.damaged)
	 or (BRINE == attacker.active->move->name and defender.active->hp.stat <= defender.active->hp.max / 2)
	 or ((EARTHQUAKE == attacker.active->move->name or MAGNITUDE == attacker.active->move->name) and defender.vanish == DUG)
	 or (FACADE == attacker.active->move->name and (BURN == attacker.active->status or PARALYSIS == attacker.active->status or POISON_NORMAL == attacker.active->status or POISON_TOXIC == attacker.active->status))
	 or (GUST == attacker.active->move->name and (defender.vanish == BOUNCED or defender.vanish == FLOWN))
	 or ((ICE_BALL == attacker.active->move->name or ROLLOUT == attacker.active->move->name) and attacker.defense_curl)
	 or (PAYBACK == attacker.active->move->name and defender.moved)
	 or (SMELLINGSALT == attacker.active->move->name and PARALYSIS == defender.active->status)
	 or (SOLARBEAM == attacker.active->move->name and weather.rain == 0)
	 or (STOMP == attacker.active->move->name and defender.minimize)
	 or (SURF == attacker.active->move->name and defender.vanish == DIVED)
	 or (WAKE_UP_SLAP == attacker.active->move->name and SLEEP == defender.active->status)
	 or (WEATHER_BALL == attacker.active->move->name and (0 != weather.hail or 0 != weather.rain or 0 != weather.sand or 0 != weather.sun)))
		attacker.active->move->power *= 2;

	if ((MUSCLE_BAND == attacker.active->item and attacker.active->move->physical) or (WISE_GLASSES == attacker.active->item and false == attacker.active->move->physical))
		attacker.active->move->power = attacker.active->move->power * 11 / 10;
	else if ((BUG == attacker.active->move->type and (INSECT_PLATE == attacker.active->item or SILVERPOWDER == attacker.active->item))
		  or (DARK == attacker.active->move->type and (DREAD_PLATE == attacker.active->item or BLACKGLASSES == attacker.active->item))
		  or (DRAGON == attacker.active->move->type and (DRACO_PLATE == attacker.active->item or DRAGON_FANG == attacker.active->item or (ADAMANT_ORB == attacker.active->item and DIALGA == attacker.active->name) or (GRISEOUS_ORB == attacker.active->item and GIRATINA_O == attacker.active->name) or (LUSTROUS_ORB == attacker.active->item and PALKIA == attacker.active->name)))
		  or (ELECTRIC == attacker.active->move->type and (ZAP_PLATE == attacker.active->item or MAGNET == attacker.active->item))
		  or (FIGHTING == attacker.active->move->type and (FIST_PLATE == attacker.active->item or BLACK_BELT == attacker.active->item))
		  or (FIRE == attacker.active->move->type and (FLAME_PLATE == attacker.active->item or CHARCOAL == attacker.active->item))
		  or (FLYING == attacker.active->move->type and (SKY_PLATE == attacker.active->item or SHARP_BEAK == attacker.active->item))
		  or (GHOST == attacker.active->move->type and ((SPOOKY_PLATE == attacker.active->item or SPELL_TAG == attacker.active->item) or (GRISEOUS_ORB == attacker.active->item and GIRATINA_O == attacker.active->name)))
		  or (GRASS == attacker.active->move->type and (MEADOW_PLATE == attacker.active->item or MIRACLE_SEED == attacker.active->item or ROSE_INCENSE == attacker.active->item))
		  or (GROUND == attacker.active->move->type and (EARTH_PLATE == attacker.active->item or SOFT_SAND == attacker.active->item))
		  or (ICE == attacker.active->move->type and (ICICLE_PLATE == attacker.active->item or NEVERMELTICE == attacker.active->item))
		  or (NORMAL == attacker.active->move->type and SILK_SCARF == attacker.active->item)
		  or (POISON == attacker.active->move->type and (TOXIC_PLATE == attacker.active->item or POISON_BARB == attacker.active->item))
		  or (PSYCHIC_TYPE == attacker.active->move->type and (MIND_PLATE == attacker.active->item or TWISTEDSPOON == attacker.active->item or ODD_INCENSE == attacker.active->item))
		  or (ROCK == attacker.active->move->type and (STONE_PLATE == attacker.active->item or HARD_STONE == attacker.active->item or ROCK_INCENSE == attacker.active->item))
		  or (STEEL == attacker.active->move->type and (IRON_PLATE == attacker.active->item or METAL_COAT == attacker.active->item or (ADAMANT_ORB == attacker.active->item and DIALGA == attacker.active->name)))
		  or (WATER == attacker.active->move->type and (SPLASH_PLATE == attacker.active->item or MYSTIC_WATER == attacker.active->item or WAVE_INCENSE == attacker.active->item))
		  or (LUSTROUS_ORB == attacker.active->item and PALKIA == attacker.active->name))
		attacker.active->move->power = attacker.active->move->power * 6 / 5;

	if (attacker.charge and ELECTRIC == attacker.active->move->type)
		attacker.active->move->power *= 2;

	if ((defender.mud_sport and ELECTRIC == attacker.active->move->type) or (defender.water_sport and FIRE == attacker.active->move->type))
		attacker.active->move->power /= 2;

	if ((TECHNICIAN == attacker.active->ability and attacker.active->move->basepower <= 60)
	 or (BLAZE == attacker.active->ability and FIRE == attacker.active->move->type and attacker.active->hp.stat <= attacker.active->hp.max / 3)
	 or (OVERGROW == attacker.active->ability and GRASS == attacker.active->move->type and attacker.active->hp.stat <= attacker.active->hp.max / 3)
	 or (SWARM == attacker.active->ability and BUG == attacker.active->move->type and attacker.active->hp.stat <= attacker.active->hp.max / 3)
	 or (TORRENT == attacker.active->ability and WATER == attacker.active->move->type and attacker.active->hp.stat <= attacker.active->hp.max / 3))
		attacker.active->move->power = attacker.active->move->power * 3 / 2;
	else if (((IRON_FIST == attacker.active->ability) and (BULLET_PUNCH == attacker.active->move->name or COMET_PUNCH == attacker.active->move->name or DIZZY_PUNCH == attacker.active->move->name or DRAIN_PUNCH == attacker.active->move->name or DYNAMICPUNCH == attacker.active->move->name or FIRE_PUNCH == attacker.active->move->name or FOCUS_PUNCH == attacker.active->move->name or HAMMER_ARM == attacker.active->move->name or ICE_PUNCH == attacker.active->move->name or MACH_PUNCH == attacker.active->move->name or MEGA_PUNCH == attacker.active->move->name or METEOR_MASH == attacker.active->move->name or SHADOW_PUNCH == attacker.active->move->name or SKY_UPPERCUT == attacker.active->move->name or THUNDERPUNCH == attacker.active->move->name))
		 or ((RECKLESS == attacker.active->ability) and (attacker.active->move->name == BRAVE_BIRD or attacker.active->move->name == DOUBLE_EDGE or attacker.active->move->name == FLARE_BLITZ or attacker.active->move->name == HEAD_SMASH or attacker.active->move->name == SUBMISSION or attacker.active->move->name == TAKE_DOWN or attacker.active->move->name == VOLT_TACKLE or attacker.active->move->name == WOOD_HAMMER)))
		attacker.active->move->power = attacker.active->move->power * 6 / 5;
	else if (RIVALRY == attacker.active->ability)
		attacker.active->move->power = attacker.active->move->power * (20 + 5 * attacker.active->gender * defender.active->gender) / 20;		// Same gender == 20 + 5, opposite gender == 20 - 5

	if ((HEATPROOF == defender.active->ability and FIRE == attacker.active->move->type)
	 or (THICK_FAT == defender.active->ability and (FIRE == attacker.active->move->type or ICE == attacker.active->move->type)))
		attacker.active->move->power /= 2;
	else if (DRY_SKIN == defender.active->ability and FIRE == attacker.active->move->type)
		attacker.active->move->power = attacker.active->move->power * 5 / 4;

	if (attacker.active->move->power == 0)
		attacker.active->move->power = 1;
}

// I split my damage calculator up into a function that calculates as much as possible with known data, one that calculates without the random number, and a function that does the rest of the work because in many cases, I have the damage calculator in a deep inner loop, and pre-calculating non-random numbers allows me to move much of that calculator to a shallower part of code, and pre-calculating known information moves even more out. Profiling showed this to be a sound optimization.

int damageknown (const Team &attacker, const Team &defender, const Weather &weather, int &rl, int &weather_mod, int &ff, int &mf) {
	if (((0 != defender.reflect and attacker.active->move->physical) or (0 != defender.light_screen and false == attacker.active->move->physical)) and false == attacker.active->move->ch)
		rl = 2;
	else
		rl = 1;

	if ((0 != weather.rain and WATER == attacker.active->move->type) or (0 != weather.sun and FIRE == attacker.active->move->type))
		weather_mod = 3;
	else if ((0 != weather.rain and FIRE == attacker.active->move->type) or (0 != weather.sun and WATER == attacker.active->move->type))
		weather_mod = 1;
	else
		weather_mod = 2;

	if (attacker.ff and FIRE == attacker.active->move->type)
		ff = 3;
	else
		ff = 2;

	if (attacker.mf)
		mf = 3;
	else
		mf = 2;

	return attacker.active->level * 2 / 5 + 2;
}



int damagenonrandom (const Team &attacker, const Team &defender, const int &rl, const int &weather_mod, const int &ff, const int &mf, int &stab, const int &type1, const int &type2, int &aem, int &eb, int &tl, int &rb, int damage) {

	damage *= attacker.active->move->power;

	if (attacker.active->move->physical) {
		damage = damage * attacker.active->atk.stat / 50 / defender.active->def.stat;
		if (BURN == attacker.active->status and GUTS != attacker.active->ability)
			damage /= 2;
	}
	else
		damage = damage * attacker.active->spa.stat / 50 / defender.active->spd.stat;

	damage = damage / rl * weather_mod / 2 * ff / 2 + 2;

	if (attacker.active->move->ch) {
		if (SNIPER == attacker.active->ability)
			damage *= 3;
		else
			damage *= 2;
	}

	if (LIFE_ORB == attacker.active->item)
		damage = damage * 13 / 10;
	else if (METRONOME_ITEM == attacker.active->item) {
		if (attacker.active->move->times_used >= 10)
			damage *= 2;
		else
			damage = damage * (10 + attacker.active->move->times_used) / 10;
	}

	damage = damage * mf / 2;

	if (istype (attacker, attacker.active->move->type) and TYPELESS != attacker.active->move->type) {
		if (ADAPTABILITY == attacker.active->ability)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if (SOLID_ROCK == defender.active->ability and type1 * type2 > 2)
		aem = 3;
	else
		aem = 4;

	if (EXPERT_BELT == attacker.active->item and type1 * type2 > 2)
		eb = 6;
	else
		eb = 5;

	if (TINTED_LENS == attacker.active->ability and type1 * type2 < 2)
		tl = 2;
	else
		tl = 1;

	if ((defender.active->item == CHILAN_BERRY and attacker.active->move->type == NORMAL) or (type1 * type2 > 2 and defender.active->item - TANGA_BERRY == attacker.active->move->type))
		rb = 2;
	else
		rb = 1;
	return damage;
}

int damagerandom (const Pokemon &attacker, const Team &defender, const int &stab, const int &type1, const int &type2, const int &aem, const int &eb, const int &tl, const int &rb, int damage) {
	damage = damage * attacker.move->r / 100 * stab / 2 * type1 / 2 * type2 / 2 * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage >= defender.active->hp.stat) {
		damage = defender.active->hp.stat;
		if (FALSE_SWIPE == attacker.move->name or defender.endure)
			--damage;
	}
	return damage;
}

int damagecalculator (const Team &attacker, const Team &defender, const Weather &weather) {
	int damage = 0;
	const int type1 = effectiveness [attacker.active->move->type] [defender.active->type1];		// Effectiveness on the defender's first type (1 if NVE, 4 if SE) / 2
	const int type2 = effectiveness [attacker.active->move->type] [defender.active->type2];		// Effectiveness on the defender's second type (1 if NVE, 4 if SE) / 2
	if ((type1 != 0 and type2 != 0) or (GROUND == attacker.active->move->type and grounded (defender, weather))) {
		if (DRAGON_RAGE == attacker.active->move->name)
			damage = 40;
		else if (ENDEAVOR == attacker.active->move->name)
			damage = defender.active->hp.stat - attacker.active->hp.stat;
		else if (FISSURE == attacker.active->move->name or GUILLOTINE == attacker.active->move->name or HORN_DRILL == attacker.active->move->name or SHEER_COLD == attacker.active->move->name)
			damage = defender.active->hp.max;
		else if (NIGHT_SHADE == attacker.active->move->name or SEISMIC_TOSS == attacker.active->move->name)
			damage = attacker.active->level;
		else if (PSYWAVE == attacker.active->move->name)
			damage = attacker.active->level * *attacker.active->move->variable / 10;
		else if (SONICBOOM == attacker.active->move->name)
			damage = 20;
		else if (SUPER_FANG == attacker.active->move->name)
			damage = defender.active->hp.stat / 2;

		else {
			int rl;						// Reflect / Light Screen (2)
			int weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
			int ff;					// Flash Fire: 3 / 2
			int mf;					// Me First: 3 / 2
			int stab;					// Same Type Attack Bonus: 3 / 2
			int aem;					// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
			int eb;					// Expert Belt: 6 / 5
			int tl;						// Tinted Lens (2)
			int rb;					// Resistance berries (2)
			damage = damageknown (attacker, defender, weather, rl, weather_mod, ff, mf);
			damage = damagenonrandom (attacker, defender, rl, weather_mod, ff, mf, stab, type1, type2, aem, eb, tl, rb, damage);
			damage = damagerandom (*attacker.active, defender, stab, type1, type2, aem, eb, tl, rb, damage);
		}
	}
	return damage;
}

void recoil (Pokemon &user, int damage, int denominator) {
	if (user.ability != MAGIC_GUARD and user.ability != ROCK_HEAD) {
		if (damage <= denominator)
			--user.hp.stat;
		else
			user.hp.stat -= damage / denominator;
		if (user.hp.stat < 0)
			user.hp.stat = 0;
	}
}

}
