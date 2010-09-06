// Damage calculator
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DAMAGE_CPP_
#define DAMAGE_CPP_

#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "status.h"
#include "weather.h"

void movepower (pokemon &attacker, const pokemon &defender, const weathers weather) {

	// I account for the doubling of the base power for Pursuit in the switching function by simply multiplying the final base power by 2. Regardless of the combination of modifiers, this does not change the final base power. The exception is if the attacker's ally uses Helping Hand. The target uses U-turn and the attacker uses Pursuit with a slower Pokemon that has Rivalry and a Muscle Band and neither the attacker nor target is genderless. This will cause the base power to be 1 less than it should be.
	
	// Variable power moves
	
	int doubling = 1;
		
	if ((ASSURANCE == attacker.move->name and defender.damaged)
	 or ((AVALANCHE == attacker.move->name or REVENGE == attacker.move->name) and attacker.damaged)
	 or (BRINE == attacker.move->name and defender.hp.stat <= defender.hp.max / 2)
	 or (EARTHQUAKE == attacker.move->name and defender.vanish == DUG)
	 or (FACADE == attacker.move->name and (BURN == attacker.status or PARALYSIS == attacker.status or POISON_NORMAL == attacker.status or POISON_TOXIC == attacker.status))
	 or (GUST == attacker.move->name and (defender.vanish == BOUNCED or defender.vanish == FLOWN))
	 or (PAYBACK == attacker.move->name and defender.moved)
	 or (SMELLINGSALT == attacker.move->name and PARALYSIS == defender.status)
	 or (SOLARBEAM == attacker.move->name and weather.rain == 0)
	 or (STOMP == attacker.move->name and defender.minimize)
	 or (SURF == attacker.move->name and defender.vanish == DIVED)
	 or (WAKE_UP_SLAP == attacker.move->name and SLEEP == defender.status)
	 or (WEATHER_BALL == attacker.move->name and (0 != weather.hail or 0 != weather.rain or 0 != weather.sand or 0 != weather.sun)))
		doubling = 2;

	else if (CRUSH_GRIP == attacker.move->name or WRING_OUT == attacker.move->name)
		attacker.move->basepower = 120 * defender.hp.stat / defender.hp.max + 1;
	
	else if (ERUPTION == attacker.move->name or WATER_SPOUT == attacker.move->name)
		attacker.move->basepower = 150 * attacker.hp.stat / attacker.hp.max;

	else if (FLAIL == attacker.move->name or REVERSAL == attacker.move->name) {
		if (64 * attacker.hp.stat / attacker.hp.max <= 1)
			attacker.move->basepower = 200;
		else if (64 * attacker.hp.stat / attacker.hp.max <= 5)
			attacker.move->basepower = 150;
		else if (64 * attacker.hp.stat / attacker.hp.max <= 12)
			attacker.move->basepower = 100;
		else if (64 * attacker.hp.stat / attacker.hp.max <= 21)
			attacker.move->basepower = 80;
		else if (64 * attacker.hp.stat / attacker.hp.max <= 42)
			attacker.move->basepower = 40;
		else
			attacker.move->basepower = 20;
	}

	else if (FLING == attacker.move->name) {
		if (IRON_BALL == attacker.item)
			attacker.move->basepower = 130;
		else if (HARD_STONE == attacker.item or OTHER100 == attacker.item)
			attacker.move->basepower = 100;
		else if (DEEPSEATOOTH == attacker.item or DRACO_PLATE == attacker.item or DREAD_PLATE == attacker.item or EARTH_PLATE == attacker.item or FIST_PLATE == attacker.item or FLAME_PLATE == attacker.item or GRIP_CLAW == attacker.item or ICICLE_PLATE == attacker.item or INSECT_PLATE == attacker.item or IRON_PLATE == attacker.item or MEADOW_PLATE == attacker.item or MIND_PLATE == attacker.item or SKY_PLATE == attacker.item or SPLASH_PLATE == attacker.item or SPOOKY_PLATE == attacker.item or STONE_PLATE == attacker.item or THICK_CLUB == attacker.item or TOXIC_PLATE == attacker.item or ZAP_PLATE == attacker.item)
			attacker.move->basepower = 90;
		else if (QUICK_CLAW == attacker.item or RAZOR_CLAW == attacker.item or STICKY_BARB == attacker.item or OTHER80 == attacker.item)
			attacker.move->basepower = 80;
		else if (DRAGON_FANG == attacker.item or POISON_BARB == attacker.item or POWER_ITEMS == attacker.item)
			attacker.move->basepower = 70;
		else if (ADAMANT_ORB == attacker.item or DAMP_ROCK == attacker.item or HEAT_ROCK == attacker.item or LUSTROUS_ORB == attacker.item or MACHO_BRACE == attacker.item or STICK == attacker.item)
			attacker.move->basepower = 60;
		else if (SHARP_BEAK == attacker.item or OTHER50 == attacker.item)
			attacker.move->basepower = 50;
		else if (ICY_ROCK == attacker.item or LUCKY_PUNCH == attacker.item)
			attacker.move->basepower = 40;
		else if (BERRY_JUICE == attacker.item or BLACK_BELT == attacker.item or BLACK_SLUDGE == attacker.item or BLACKGLASSES == attacker.item or CHARCOAL == attacker.item or DEEPSEASCALE == attacker.item or FLAME_ORB == attacker.item or KINGS_ROCK == attacker.item or LIFE_ORB == attacker.item or LIGHT_BALL == attacker.item or LIGHT_CLAY == attacker.item or MAGNET == attacker.item or METAL_COAT == attacker.item or METRONOME_ITEM == attacker.item or MIRACLE_SEED == attacker.item or MYSTIC_WATER == attacker.item or NEVERMELTICE == attacker.item or RAZOR_FANG == attacker.item or SCOPE_LENS == attacker.item or SHELL_BELL == attacker.item or SOUL_DEW == attacker.item or SPELL_TAG == attacker.item or TOXIC_ORB == attacker.item or TWISTEDSPOON == attacker.item or OTHER30 == attacker.item)
			attacker.move->basepower = 30;
		else
			attacker.move->basepower = 10;
	}

	else if (FRUSTRATION == attacker.move->name or RETURN == attacker.move->name) {
		attacker.move->basepower = attacker.happiness * 2 / 5;
		if (FRUSTRATION == attacker.move->name)
			attacker.move->basepower = 102 - attacker.move->basepower;
	}

	else if (FURY_CUTTER == attacker.move->name)
		attacker.move->basepower = 10 << attacker.move->times_used;					// Equivalent to 10 * 2 ^ attacker.move->times_used

	else if (GRASS_KNOT == attacker.move->name or LOW_KICK == attacker.move->name)
		attacker.move->basepower = defender.mass;
	
	else if (attacker.move->name == GYRO_BALL) {
		attacker.move->basepower = 25 * defender.spe.stat / attacker.spe.stat + 1;
		if (attacker.move->basepower > 150)
			attacker.move->basepower = 150;
	}

	else if (ICE_BALL == attacker.move->name or ROLLOUT == attacker.move->name) {
		attacker.move->basepower = 30 << attacker.move->times_used;					// Equivalent to 30 * 2 ^ attacker.move->times_used
		if (defender.defense_curl == true)
			doubling = 2;
	}

	else if (HIDDEN_POWER == attacker.move->name) {
		// The second-least significant bit of each stat determines the power of Hidden Power
		int u = (attacker.hp.iv >> 1) % 2;
		int v = ((attacker.atk.iv >> 1) % 2) * 2;
		int w = ((attacker.def.iv >> 1) % 2) * 4;
		int x = ((attacker.spe.iv >> 1) % 2) * 8;
		int y = ((attacker.spa.iv >> 1) % 2) * 16;
		int z = ((attacker.spd.iv >> 1) % 2) * 32;
		
		attacker.move->basepower = (u + v + w + x + y + z) * 40 / 63 + 30;
	}

	else if (MAGNITUDE == attacker.move->name) {
		if (4 == attacker.move->variable)
			attacker.move->basepower = 10;
		else if (5 == attacker.move->variable)
			attacker.move->basepower = 30;
		else if (6 == attacker.move->variable)
			attacker.move->basepower = 50;
		else if (7 == attacker.move->variable)
			attacker.move->basepower = 70;
		else if (8 == attacker.move->variable)
			attacker.move->basepower = 90;
		else if (9 == attacker.move->variable)
			attacker.move->basepower = 110;
		else if (10 == attacker.move->variable)
			attacker.move->basepower = 150;
		if (defender.vanish == DUG)
			doubling = 2;
	}

	else if (NATURAL_GIFT == attacker.move->name) {
		if (AGUAV_BERRY == attacker.item or ASPEAR_BERRY == attacker.item or BABIRI_BERRY == attacker.item or CHARTI_BERRY == attacker.item or CHERI_BERRY == attacker.item or CHESTO_BERRY == attacker.item or CHILAN_BERRY == attacker.item or CHOPLE_BERRY == attacker.item or COBA_BERRY == attacker.item or COLBUR_BERRY == attacker.item or FIGY_BERRY == attacker.item or HABAN_BERRY == attacker.item or IAPAPA_BERRY == attacker.item or KASIB_BERRY == attacker.item or KEBIA_BERRY == attacker.item or LEPPA_BERRY == attacker.item or LUM_BERRY == attacker.item or MAGO_BERRY == attacker.item or OCCA_BERRY == attacker.item or ORAN_BERRY == attacker.item or PASSHO_BERRY == attacker.item or PAYAPA_BERRY == attacker.item or PECHA_BERRY == attacker.item or PERSIM_BERRY == attacker.item or RAWST_BERRY == attacker.item or RAZZ_BERRY == attacker.item or RINDO_BERRY == attacker.item or SHUCA_BERRY == attacker.item or SITRUS_BERRY == attacker.item or TANGA_BERRY == attacker.item or WACAN_BERRY == attacker.item or WIKI_BERRY == attacker.item or YACHE_BERRY == attacker.item)
			attacker.move->basepower = 60;
		else if (BLUK_BERRY == attacker.item or CORNN_BERRY == attacker.item or GREPA_BERRY == attacker.item or HONDEW_BERRY == attacker.item or KELPSY_BERRY == attacker.item or MAGOST_BERRY == attacker.item or NANAB_BERRY == attacker.item or NOMEL_BERRY == attacker.item or PAMTRE_BERRY == attacker.item or PINAP_BERRY == attacker.item or POMEG_BERRY == attacker.item or QUALOT_BERRY == attacker.item or RABUTA_BERRY == attacker.item or SPELON_BERRY == attacker.item or TAMATO_BERRY == attacker.item or WEPEAR_BERRY == attacker.item)
			attacker.move->basepower = 70;
		else if (APICOT_BERRY == attacker.item or BELUE_BERRY == attacker.item or CUSTAP_BERRY == attacker.item or DURIN_BERRY == attacker.item or ENIGMA_BERRY == attacker.item or GANLON_BERRY == attacker.item or JABOCA_BERRY == attacker.item or LANSAT_BERRY == attacker.item or LIECHI_BERRY == attacker.item or MICLE_BERRY == attacker.item or PETAYA_BERRY == attacker.item or ROWAP_BERRY == attacker.item or SALAC_BERRY == attacker.item or STARF_BERRY == attacker.item or WATMEL_BERRY == attacker.item)
			attacker.move->basepower = 80;
	}

	else if (PRESENT == attacker.move->name)
		attacker.move->basepower = attacker.move->variable * 40;

	else if (PUNISHMENT == attacker.move->name) {
		attacker.move->basepower = 60;
		if (defender.atk.stage > 0)
			attacker.move->basepower += 20 * defender.atk.stage;
		if (defender.def.stage > 0)
			attacker.move->basepower += 20 * defender.def.stage;
		if (defender.spa.stage > 0)
			attacker.move->basepower += 20 * defender.spa.stage;
		if (defender.spd.stage > 0)
			attacker.move->basepower += 20 * defender.spd.stage;
		if (defender.spe.stage > 0)
			attacker.move->basepower += 20 * defender.spe.stage;
		if (attacker.move->basepower > 200)
			attacker.move->basepower = 200;
	}

	else if (SPIT_UP == attacker.move->name)
		attacker.move->basepower = attacker.stockpile * 100;

	else if (TRIPLE_KICK == attacker.move->name)
		attacker.move->basepower = 10 * attacker.move->times_used;

	else if (TRUMP_CARD == attacker.move->name) {
		if (0 == attacker.move->pp)
			attacker.move->basepower = 200;
		else if (1 == attacker.move->pp)
			attacker.move->basepower = 80;
		else if (2 == attacker.move->pp)
			attacker.move->basepower = 60;
		else if (3 == attacker.move->pp)
			attacker.move->basepower = 50;
		else
			attacker.move->basepower = 40;
	}

	if (attacker.move->basepower == 0)
		attacker.move->power = 0;
	else {
		int bpaim;			// Base Power Attacker Item Modifier: Muscle Band (11), Wise Glasses (11), type-boosting items (12) / 10
		if ((MUSCLE_BAND == attacker.item and attacker.move->physical) or (WISE_GLASSES == attacker.item and false == attacker.move->physical))
			bpaim = 11;
		else if ((BUG == attacker.move->type and (INSECT_PLATE == attacker.item or SILVERPOWDER == attacker.item))
			  or (DARK == attacker.move->type and (DREAD_PLATE == attacker.item or BLACKGLASSES == attacker.item))
			  or (DRAGON == attacker.move->type and (DRACO_PLATE == attacker.item or DRAGON_FANG == attacker.item or (ADAMANT_ORB == attacker.item and DIALGA == attacker.pokemon) or (GRISEOUS_ORB == attacker.item and GIRATINA_O == attacker.pokemon) or (LUSTROUS_ORB == attacker.item and PALKIA == attacker.pokemon)))
			  or (ELECTRIC == attacker.move->type and (ZAP_PLATE == attacker.item or MAGNET == attacker.item))
			  or (FIGHTING == attacker.move->type and (FIST_PLATE == attacker.item or BLACK_BELT == attacker.item))
			  or (FIRE == attacker.move->type and (FLAME_PLATE == attacker.item or CHARCOAL == attacker.item))
			  or (FLYING == attacker.move->type and (SKY_PLATE == attacker.item or SHARP_BEAK == attacker.item))
			  or (GHOST == attacker.move->type and ((SPOOKY_PLATE == attacker.item or SPELL_TAG == attacker.item) or (GRISEOUS_ORB == attacker.item and GIRATINA_O == attacker.pokemon)))
			  or (GRASS == attacker.move->type and (MEADOW_PLATE == attacker.item or MIRACLE_SEED == attacker.item or ROSE_INCENSE == attacker.item))
			  or (GROUND == attacker.move->type and (EARTH_PLATE == attacker.item or SOFT_SAND == attacker.item))
			  or (ICE == attacker.move->type and (ICICLE_PLATE == attacker.item or NEVERMELTICE == attacker.item))
			  or (NORMAL == attacker.move->type and SILK_SCARF == attacker.item)
			  or (POISON == attacker.move->type and (TOXIC_PLATE == attacker.item or POISON_BARB == attacker.item))
			  or (PSYCHIC_TYPE == attacker.move->type and (MIND_PLATE == attacker.item or TWISTEDSPOON == attacker.item or ODD_INCENSE == attacker.item))
			  or (ROCK == attacker.move->type and (STONE_PLATE == attacker.item or HARD_STONE == attacker.item or ROCK_INCENSE == attacker.item))
			  or (STEEL == attacker.move->type and (IRON_PLATE == attacker.item or METAL_COAT == attacker.item or (ADAMANT_ORB == attacker.item and DIALGA == attacker.pokemon)))
			  or (WATER == attacker.move->type and (SPLASH_PLATE == attacker.item or MYSTIC_WATER == attacker.item or WAVE_INCENSE == attacker.item))
			  or (LUSTROUS_ORB == attacker.item and PALKIA == attacker.pokemon))
			bpaim = 12;
		else
			bpaim = 10;
	
		int charge;		// Charge (2)
		if (attacker.charge and ELECTRIC == attacker.move->type)
			charge = 2;
		else
			charge = 1;
	
		int sport;			// Mud Sport / Water Sport (2)
		if ((defender.mud_sport and ELECTRIC == attacker.move->type) or (defender.water_sport and FIRE == attacker.move->type))
			sport = 2;
		else
			sport = 1;
		
		int bpaam;			// Base Power Attacker Ability Modifier: Blaze (30), Iron Fist (24), Overgrow (30), Reckless (24), Rivalry (15 opposite, 25 same), Swarm (30), Technician (30), Torrent (30) / 20
		if ((TECHNICIAN == attacker.ability and attacker.move->basepower <= 60)
		 or (BLAZE == attacker.ability and FIRE == attacker.move->type and attacker.hp.stat <= attacker.hp.max / 3)
		 or (OVERGROW == attacker.ability and GRASS == attacker.move->type and attacker.hp.stat <= attacker.hp.max / 3)
		 or (SWARM == attacker.ability and BUG == attacker.move->type and attacker.hp.stat <= attacker.hp.max / 3)
		 or (TORRENT == attacker.ability and WATER == attacker.move->type and attacker.hp.stat <= attacker.hp.max / 3))
			bpaam = 30;
		else if (((IRON_FIST == attacker.ability) and (BULLET_PUNCH == attacker.move->name or COMET_PUNCH == attacker.move->name or DIZZY_PUNCH == attacker.move->name or DRAIN_PUNCH == attacker.move->name or DYNAMICPUNCH == attacker.move->name or FIRE_PUNCH == attacker.move->name or FOCUS_PUNCH == attacker.move->name or HAMMER_ARM == attacker.move->name or ICE_PUNCH == attacker.move->name or MACH_PUNCH == attacker.move->name or MEGA_PUNCH == attacker.move->name or METEOR_MASH == attacker.move->name or SHADOW_PUNCH == attacker.move->name or SKY_UPPERCUT == attacker.move->name or THUNDERPUNCH == attacker.move->name))
			 or ((RECKLESS == attacker.ability) and (attacker.move->name == BRAVE_BIRD or attacker.move->name == DOUBLE_EDGE or attacker.move->name == FLARE_BLITZ or attacker.move->name == HEAD_SMASH or attacker.move->name == SUBMISSION or attacker.move->name == TAKE_DOWN or attacker.move->name == VOLT_TACKLE or attacker.move->name == WOOD_HAMMER)))
			bpaam = 24;
		else if (RIVALRY == attacker.ability and GENDERLESS != attacker.gender and GENDERLESS != defender.gender) {
			if (attacker.gender == defender.gender)
				bpaam = 25;
			else
				bpaam = 15;
		}
		else
			bpaam = 20;
	
		int bpdam;			// Base Power Defender Ability Modifier: Dry Skin (5), Heatproof (2), Thick Fat (2)
		if (HEATPROOF == defender.ability and FIRE == attacker.move->type)
			bpdam = 2;
		else if (THICK_FAT == defender.ability and (FIRE == attacker.move->type or ICE == attacker.move->type))
			bpdam = 2;
		else if (DRY_SKIN == defender.ability and FIRE == attacker.move->type)
			bpdam = 5;
		else
			bpdam = 4;
		
		attacker.move->power = attacker.move->basepower * doubling * bpaim / 10 * charge / sport * bpaam / 20 * bpdam / 4;
		if (attacker.move->power == 0)
			attacker.move->power = 1;
	}
}

// I split my damage calculator up into a function that calculates as much as possible without the random number and a function that does the rest of the work because in many cases, I have the damage calculator in a deep inner loop, and pre-calculating non-random numbers allows me to move much of that calculator to a shallower part of code. Profiling showed this to be a sound optimization.

int damagenonrandom (const pokemon &attacker, const teams &defender, const weathers &weather, int &stab, int &type1, int &type2, int &aem, int &eb, int &tl, int &rb) {
	int damage;
	int rl;					// Reflect / Light Screen (2)
	if (((0 != defender.reflect and attacker.move->physical) or (0 != defender.light_screen and false == attacker.move->physical)) and false == attacker.move->ch)
		rl = 2;
	else
		rl = 1;

	int weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
	if ((0 != weather.rain and WATER == attacker.move->type) or (0 != weather.sun and FIRE == attacker.move->type))
		weather_mod = 3;
	else if ((0 != weather.rain and FIRE == attacker.move->type) or (0 != weather.sun and WATER == attacker.move->type))
		weather_mod = 1;
	else
		weather_mod = 2;

	int ff_mod;				// Flash Fire: 3 / 2
	if (attacker.ff and FIRE == attacker.move->type)
		ff_mod = 3;
	else
		ff_mod = 2;

	int ch_mod;				// Critical Hit (2 normally, 3 with Sniper)
	if (attacker.move->ch) {
		if (SNIPER == attacker.ability)
			ch_mod = 3;
		else
			ch_mod = 2;
	}
	else
		ch_mod = 1;

	int itm;					// Life Orb (13), Metronome (10 through 20) / 10
	if (LIFE_ORB == attacker.item)
		itm = 13;
	else if (METRONOME_ITEM == attacker.item) {
		if (attacker.move->times_used >= 10)
			itm = 20;
		else
			itm = 10 + attacker.move->times_used;
	}
	else
		itm = 10;

	int mf_mod;				// Me First: 3 / 2
	if (attacker.mf)
		mf_mod = 3;
	else
		mf_mod = 2;
	
	damage = (attacker.level * 2 / 5 + 2) * attacker.move->power;
	if (attacker.move->physical) {
		int burn;				// Burn (2)
		if (BURN == attacker.status and GUTS != attacker.ability)
			burn = 2;
		else
			burn = 1;

		damage = damage * attacker.atk.stat / 50 / defender.active->def.stat / burn;
	}
	else
		damage = damage * attacker.spa.stat / 50 / defender.active->spd.stat;
	damage = (damage / rl * weather_mod / 2 * ff_mod / 2 + 2) * ch_mod * itm / 10 * mf_mod / 2;

	if (istype (attacker, attacker.move->type) and TYPELESS != attacker.move->type) {
		if (ADAPTABILITY == attacker.ability)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if ((FILTER == defender.active->ability or SOLID_ROCK == defender.active->ability) and type1 * type2 > 2)
		aem = 3;
	else
		aem = 4;

	if (EXPERT_BELT == attacker.item and type1 * type2 > 2)
		eb = 6;
	else
		eb = 5;

	if (TINTED_LENS == attacker.ability and type1 * type2 < 2)
		tl = 2;
	else
		tl = 1;

	if ((defender.active->item == CHILAN_BERRY and attacker.move->type == NORMAL) or (type1 * type2 > 2 and ((defender.active->item == BABIRI_BERRY and attacker.move->type == STEEL) or (defender.active->item == CHARTI_BERRY and attacker.move->type == ROCK) or (defender.active->item == CHOPLE_BERRY and attacker.move->type == FIGHTING) or (defender.active->item == COBA_BERRY and attacker.move->type == FLYING) or (defender.active->item == COLBUR_BERRY and attacker.move->type == DARK) or (defender.active->item == HABAN_BERRY and attacker.move->type == DRAGON) or (defender.active->item == KASIB_BERRY and attacker.move->type == GHOST) or (defender.active->item == KEBIA_BERRY and attacker.move->type == POISON) or (defender.active->item == OCCA_BERRY and attacker.move->type == FIRE) or (defender.active->item == PASSHO_BERRY and attacker.move->type == WATER) or (defender.active->item == PAYAPA_BERRY and attacker.move->type == PSYCHIC_TYPE) or (defender.active->item == RINDO_BERRY and attacker.move->type == GRASS) or (defender.active->item == SHUCA_BERRY and attacker.move->type == GROUND) or (defender.active->item == TANGA_BERRY and attacker.move->type == BUG) or (defender.active->item == WACAN_BERRY and attacker.move->type == ELECTRIC) or (defender.active->item == YACHE_BERRY and attacker.move->type == ICE))))
		rb = 2;
	else
		rb = 1;
	return damage;
}

int damagerandom (const pokemon &attacker, const teams &defender, int &stab, int &type1, int &type2, int &aem, int &eb, int &tl, int &rb, int damage) {
	damage = damage * attacker.move->r / 100 * stab / 2 * type1 / 2 * type2 / 2 * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage > defender.active->hp.stat)
		damage = defender.active->hp.stat;
	if (FALSE_SWIPE == attacker.move->name and damage == defender.active->hp.stat)
		--damage;
	return damage;
}

int damagecalculator (const pokemon &attacker, const teams &defender, const weathers &weather) {
	int damage = 0;
	int type1 = effectiveness [attacker.move->type] [defender.active->type1];		// Effectiveness on the defender's first type (1 if NVE, 4 if SE) / 2
	int type2 = effectiveness [attacker.move->type] [defender.active->type2];		// Effectiveness on the defender's second type (1 if NVE, 4 if SE) / 2
	if ((type1 == 0 or type2 == 0) or (GROUND == attacker.move->type and !grounded (*defender.active, weather)) == false) {
		if (DRAGON_RAGE == attacker.move->name)
			damage = 40;
		else if (ENDEAVOR == attacker.move->name)
			damage = defender.active->hp.stat - attacker.hp.stat;
		else if (FISSURE == attacker.move->name or GUILLOTINE == attacker.move->name or HORN_DRILL == attacker.move->name or SHEER_COLD == attacker.move->name)
			damage = defender.active->hp.max;
		else if (NIGHT_SHADE == attacker.move->name or SEISMIC_TOSS == attacker.move->name)
			damage = attacker.level;
		else if (PSYWAVE == attacker.move->name)
			damage = attacker.move->variable * attacker.level / 10;
		else if (SONICBOOM == attacker.move->name)
			damage = 20;
		else if (SUPER_FANG == attacker.move->name)
			damage = defender.active->hp.stat / 2;

		else {
			int stab;		// Same Type Attack Bonus: 3 / 2
			int aem;		// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
			int eb;		// Expert Belt: 6 / 5
			int tl;			// Tinted Lens (2)
			int rb;		// Resistance berries (2)
			damage = damagenonrandom (attacker, defender, weather, stab, type1, type2, aem, eb, tl, rb);
			damage = damagerandom (attacker, defender, stab, type1, type2, aem, eb, tl, rb, damage);
		}
	}
	return damage;
}

#endif
