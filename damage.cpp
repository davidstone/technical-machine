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
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void movepower (Team &attacker, Team const &defender, Weather const weather) {

	// I account for the doubling of the base power for Pursuit in the switching function by simply multiplying the final base power by 2. Regardless of the combination of modifiers, this does not change the final base power. The exception is if the attacker's ally uses Helping Hand. The target uses U-turn and the attacker uses Pursuit with a slower Pokemon that has Rivalry and a Muscle Band and neither the attacker nor target is genderless. This will cause the base power to be 1 less than it should be.

	// Variable power moves
	
	switch (attacker.pokemon->move->name) {
		case CRUSH_GRIP:
		case WRING_OUT:
			attacker.pokemon->move->basepower = 120 * defender.pokemon->hp.stat / defender.pokemon->hp.max + 1;
			break;
		case ERUPTION:
		case WATER_SPOUT:
			attacker.pokemon->move->basepower = 150 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max;
			break;
		case FLAIL:
		case REVERSAL:
			if (64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max <= 1)
				attacker.pokemon->move->basepower = 200;
			else if (64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max <= 5)
				attacker.pokemon->move->basepower = 150;
			else if (64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max <= 12)
				attacker.pokemon->move->basepower = 100;
			else if (64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max <= 21)
				attacker.pokemon->move->basepower = 80;
			else if (64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max <= 42)
				attacker.pokemon->move->basepower = 40;
			else
				attacker.pokemon->move->basepower = 20;
			break;
		case FLING:
			switch (attacker.pokemon->item) {
				case IRON_BALL:
					attacker.pokemon->move->basepower = 130;
					break;
				case HARD_STONE:
				case OTHER100:
					attacker.pokemon->move->basepower = 100;
					break;
				case DEEPSEATOOTH:
				case DRACO_PLATE:
				case DREAD_PLATE:
				case EARTH_PLATE:
				case FIST_PLATE:
				case FLAME_PLATE:
				case GRIP_CLAW:
				case ICICLE_PLATE:
				case INSECT_PLATE:
				case IRON_PLATE:
				case MEADOW_PLATE:
				case MIND_PLATE:
				case SKY_PLATE:
				case SPLASH_PLATE:
				case SPOOKY_PLATE:
				case STONE_PLATE:
				case THICK_CLUB:
				case TOXIC_PLATE:
				case ZAP_PLATE:
					attacker.pokemon->move->basepower = 90;
					break;
				case QUICK_CLAW:
				case RAZOR_CLAW:
				case STICKY_BARB:
				case OTHER80:
					attacker.pokemon->move->basepower = 80;
					break;
				case DRAGON_FANG:
				case POISON_BARB:
				case POWER_ITEMS:
					attacker.pokemon->move->basepower = 70;
					break;
				case ADAMANT_ORB:
				case DAMP_ROCK:
				case HEAT_ROCK:
				case LUSTROUS_ORB:
				case MACHO_BRACE:
				case STICK:
					attacker.pokemon->move->basepower = 60;
					break;
				case SHARP_BEAK:
				case OTHER50:
					attacker.pokemon->move->basepower = 50;
					break;
				case ICY_ROCK:
				case LUCKY_PUNCH:
					attacker.pokemon->move->basepower = 40;
					break;
				case BERRY_JUICE:
				case BLACK_BELT:
				case BLACK_SLUDGE:
				case BLACKGLASSES:
				case CHARCOAL:
				case DEEPSEASCALE:
				case FLAME_ORB:
				case KINGS_ROCK:
				case LIFE_ORB:
				case LIGHT_BALL:
				case LIGHT_CLAY:
				case MAGNET:
				case METAL_COAT:
				case METRONOME_ITEM:
				case MIRACLE_SEED:
				case MYSTIC_WATER:
				case NEVERMELTICE:
				case SCOPE_LENS:
				case SHELL_BELL:
				case SOUL_DEW:
				case SPELL_TAG:
				case TOXIC_ORB:
				case TWISTEDSPOON:
				case OTHER30:
					attacker.pokemon->move->basepower = 30;
					break;
				default:
					attacker.pokemon->move->basepower = 10;
					break;
			}
			break;
		case FRUSTRATION:
		case RETURN:
			attacker.pokemon->move->basepower = attacker.pokemon->happiness * 2 / 5;
			if (FRUSTRATION == attacker.pokemon->move->name)
				attacker.pokemon->move->basepower = 102 - attacker.pokemon->move->basepower;
			break;
		case FURY_CUTTER:
			attacker.pokemon->move->basepower = 10 << attacker.pokemon->move->times_used;					// Equivalent to 10 * 2 ^ attacker.pokemon->move->times_used
			break;
		case GRASS_KNOT:
		case LOW_KICK:
			attacker.pokemon->move->basepower = defender.pokemon->mass;
			break;
		case GYRO_BALL:
			attacker.pokemon->move->basepower = 25 * defender.pokemon->spe.stat / attacker.pokemon->spe.stat + 1;
			if (attacker.pokemon->move->basepower > 150)
				attacker.pokemon->move->basepower = 150;
			break;
		case ICE_BALL:
		case ROLLOUT:
			attacker.pokemon->move->basepower = 30 << attacker.pokemon->move->times_used;			// Equivalent to 30 * 2 ^ attacker.pokemon->move->times_used
			break;
		case HIDDEN_POWER: {
			// The second-least significant bit of each stat determines the power of Hidden Power
			unsigned const u = (attacker.pokemon->hp.iv >> 1) % 2;
			unsigned const v = ((attacker.pokemon->atk.iv >> 1) % 2) * 2;
			unsigned const w = ((attacker.pokemon->def.iv >> 1) % 2) * 4;
			unsigned const x = ((attacker.pokemon->spe.iv >> 1) % 2) * 8;
			unsigned const y = ((attacker.pokemon->spa.iv >> 1) % 2) * 16;
			unsigned const z = ((attacker.pokemon->spd.iv >> 1) % 2) * 32;
	
			attacker.pokemon->move->basepower = (u + v + w + x + y + z) * 40 / 63 + 30;
			break;
		}
		case MAGNITUDE:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case NATURAL_GIFT:
			if (AGUAV_BERRY == attacker.pokemon->item or ASPEAR_BERRY == attacker.pokemon->item or BABIRI_BERRY == attacker.pokemon->item or CHARTI_BERRY == attacker.pokemon->item or CHERI_BERRY == attacker.pokemon->item or CHESTO_BERRY == attacker.pokemon->item or CHILAN_BERRY == attacker.pokemon->item or CHOPLE_BERRY == attacker.pokemon->item or COBA_BERRY == attacker.pokemon->item or COLBUR_BERRY == attacker.pokemon->item or FIGY_BERRY == attacker.pokemon->item or HABAN_BERRY == attacker.pokemon->item or IAPAPA_BERRY == attacker.pokemon->item or KASIB_BERRY == attacker.pokemon->item or KEBIA_BERRY == attacker.pokemon->item or LEPPA_BERRY == attacker.pokemon->item or LUM_BERRY == attacker.pokemon->item or MAGO_BERRY == attacker.pokemon->item or OCCA_BERRY == attacker.pokemon->item or ORAN_BERRY == attacker.pokemon->item or PASSHO_BERRY == attacker.pokemon->item or PAYAPA_BERRY == attacker.pokemon->item or PECHA_BERRY == attacker.pokemon->item or PERSIM_BERRY == attacker.pokemon->item or RAWST_BERRY == attacker.pokemon->item or RAZZ_BERRY == attacker.pokemon->item or RINDO_BERRY == attacker.pokemon->item or SHUCA_BERRY == attacker.pokemon->item or SITRUS_BERRY == attacker.pokemon->item or TANGA_BERRY == attacker.pokemon->item or WACAN_BERRY == attacker.pokemon->item or WIKI_BERRY == attacker.pokemon->item or YACHE_BERRY == attacker.pokemon->item)
				attacker.pokemon->move->basepower = 60;
			else if (BLUK_BERRY == attacker.pokemon->item or CORNN_BERRY == attacker.pokemon->item or GREPA_BERRY == attacker.pokemon->item or HONDEW_BERRY == attacker.pokemon->item or KELPSY_BERRY == attacker.pokemon->item or MAGOST_BERRY == attacker.pokemon->item or NANAB_BERRY == attacker.pokemon->item or NOMEL_BERRY == attacker.pokemon->item or PAMTRE_BERRY == attacker.pokemon->item or PINAP_BERRY == attacker.pokemon->item or POMEG_BERRY == attacker.pokemon->item or QUALOT_BERRY == attacker.pokemon->item or RABUTA_BERRY == attacker.pokemon->item or SPELON_BERRY == attacker.pokemon->item or TAMATO_BERRY == attacker.pokemon->item or WEPEAR_BERRY == attacker.pokemon->item)
				attacker.pokemon->move->basepower = 70;
			else if (APICOT_BERRY == attacker.pokemon->item or BELUE_BERRY == attacker.pokemon->item or CUSTAP_BERRY == attacker.pokemon->item or DURIN_BERRY == attacker.pokemon->item or ENIGMA_BERRY == attacker.pokemon->item or GANLON_BERRY == attacker.pokemon->item or JABOCA_BERRY == attacker.pokemon->item or LANSAT_BERRY == attacker.pokemon->item or LIECHI_BERRY == attacker.pokemon->item or MICLE_BERRY == attacker.pokemon->item or PETAYA_BERRY == attacker.pokemon->item or ROWAP_BERRY == attacker.pokemon->item or SALAC_BERRY == attacker.pokemon->item or STARF_BERRY == attacker.pokemon->item or WATMEL_BERRY == attacker.pokemon->item)
				attacker.pokemon->move->basepower = 80;
			break;
		case PRESENT:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case PUNISHMENT:
			attacker.pokemon->move->basepower = 60;
			if (defender.pokemon->atk.stage > 0)
				attacker.pokemon->move->basepower += 20 * defender.pokemon->atk.stage;
			if (defender.pokemon->def.stage > 0)
				attacker.pokemon->move->basepower += 20 * defender.pokemon->def.stage;
			if (defender.pokemon->spa.stage > 0)
				attacker.pokemon->move->basepower += 20 * defender.pokemon->spa.stage;
			if (defender.pokemon->spd.stage > 0)
				attacker.pokemon->move->basepower += 20 * defender.pokemon->spd.stage;
			if (defender.pokemon->spe.stage > 0)
				attacker.pokemon->move->basepower += 20 * defender.pokemon->spe.stage;
			if (attacker.pokemon->move->basepower > 200)
				attacker.pokemon->move->basepower = 200;
			break;
		case SPIT_UP:
			attacker.pokemon->move->basepower = attacker.stockpile * 100;
			break;
		case TRIPLE_KICK:
			attacker.pokemon->move->basepower = 10 * attacker.pokemon->move->times_used;
			break;
		case TRUMP_CARD:
			if (0 == attacker.pokemon->move->pp)
				attacker.pokemon->move->basepower = 200;
			else if (1 == attacker.pokemon->move->pp)
				attacker.pokemon->move->basepower = 80;
			else if (2 == attacker.pokemon->move->pp)
				attacker.pokemon->move->basepower = 60;
			else if (3 == attacker.pokemon->move->pp)
				attacker.pokemon->move->basepower = 50;
			else
				attacker.pokemon->move->basepower = 40;
			break;
		default:
			break;
	}
	attacker.pokemon->move->power = attacker.pokemon->move->basepower;

	if ((ASSURANCE == attacker.pokemon->move->name and defender.damaged)
			or ((AVALANCHE == attacker.pokemon->move->name or REVENGE == attacker.pokemon->move->name) and attacker.damaged)
			or (BRINE == attacker.pokemon->move->name and defender.pokemon->hp.stat <= defender.pokemon->hp.max / 2)
			or ((EARTHQUAKE == attacker.pokemon->move->name or MAGNITUDE == attacker.pokemon->move->name) and defender.vanish == DUG)
			or (FACADE == attacker.pokemon->move->name and (BURN == attacker.pokemon->status or PARALYSIS == attacker.pokemon->status or POISON_NORMAL == attacker.pokemon->status or POISON_TOXIC == attacker.pokemon->status))
			or (GUST == attacker.pokemon->move->name and (defender.vanish == BOUNCED or defender.vanish == FLOWN))
			or ((ICE_BALL == attacker.pokemon->move->name or ROLLOUT == attacker.pokemon->move->name) and attacker.defense_curl)
			or (PAYBACK == attacker.pokemon->move->name and defender.moved)
			or (SMELLINGSALT == attacker.pokemon->move->name and PARALYSIS == defender.pokemon->status)
			or (SOLARBEAM == attacker.pokemon->move->name and weather.rain == 0)
			or (STOMP == attacker.pokemon->move->name and defender.minimize)
			or (SURF == attacker.pokemon->move->name and defender.vanish == DIVED)
			or (WAKE_UP_SLAP == attacker.pokemon->move->name and SLEEP == defender.pokemon->status)
			or (WEATHER_BALL == attacker.pokemon->move->name and (0 != weather.hail or 0 != weather.rain or 0 != weather.sand or 0 != weather.sun)))
		attacker.pokemon->move->power *= 2;

	if ((MUSCLE_BAND == attacker.pokemon->item and attacker.pokemon->move->physical) or (WISE_GLASSES == attacker.pokemon->item and false == attacker.pokemon->move->physical))
		attacker.pokemon->move->power = attacker.pokemon->move->power * 11 / 10;
	else if ((BUG == attacker.pokemon->move->type and (INSECT_PLATE == attacker.pokemon->item or SILVERPOWDER == attacker.pokemon->item))
				or (DARK == attacker.pokemon->move->type and (DREAD_PLATE == attacker.pokemon->item or BLACKGLASSES == attacker.pokemon->item))
				or (DRAGON == attacker.pokemon->move->type and (DRACO_PLATE == attacker.pokemon->item or DRAGON_FANG == attacker.pokemon->item or (ADAMANT_ORB == attacker.pokemon->item and DIALGA == attacker.pokemon->name) or (GRISEOUS_ORB == attacker.pokemon->item and GIRATINA_O == attacker.pokemon->name) or (LUSTROUS_ORB == attacker.pokemon->item and PALKIA == attacker.pokemon->name)))
				or (ELECTRIC == attacker.pokemon->move->type and (ZAP_PLATE == attacker.pokemon->item or MAGNET == attacker.pokemon->item))
				or (FIGHTING == attacker.pokemon->move->type and (FIST_PLATE == attacker.pokemon->item or BLACK_BELT == attacker.pokemon->item))
				or (FIRE == attacker.pokemon->move->type and (FLAME_PLATE == attacker.pokemon->item or CHARCOAL == attacker.pokemon->item))
				or (FLYING == attacker.pokemon->move->type and (SKY_PLATE == attacker.pokemon->item or SHARP_BEAK == attacker.pokemon->item))
				or (GHOST == attacker.pokemon->move->type and ((SPOOKY_PLATE == attacker.pokemon->item or SPELL_TAG == attacker.pokemon->item) or (GRISEOUS_ORB == attacker.pokemon->item and GIRATINA_O == attacker.pokemon->name)))
				or (GRASS == attacker.pokemon->move->type and (MEADOW_PLATE == attacker.pokemon->item or MIRACLE_SEED == attacker.pokemon->item or ROSE_INCENSE == attacker.pokemon->item))
				or (GROUND == attacker.pokemon->move->type and (EARTH_PLATE == attacker.pokemon->item or SOFT_SAND == attacker.pokemon->item))
				or (ICE == attacker.pokemon->move->type and (ICICLE_PLATE == attacker.pokemon->item or NEVERMELTICE == attacker.pokemon->item))
				or (NORMAL == attacker.pokemon->move->type and SILK_SCARF == attacker.pokemon->item)
				or (POISON == attacker.pokemon->move->type and (TOXIC_PLATE == attacker.pokemon->item or POISON_BARB == attacker.pokemon->item))
				or (PSYCHIC_TYPE == attacker.pokemon->move->type and (MIND_PLATE == attacker.pokemon->item or TWISTEDSPOON == attacker.pokemon->item or ODD_INCENSE == attacker.pokemon->item))
				or (ROCK == attacker.pokemon->move->type and (STONE_PLATE == attacker.pokemon->item or HARD_STONE == attacker.pokemon->item or ROCK_INCENSE == attacker.pokemon->item))
				or (STEEL == attacker.pokemon->move->type and (IRON_PLATE == attacker.pokemon->item or METAL_COAT == attacker.pokemon->item or (ADAMANT_ORB == attacker.pokemon->item and DIALGA == attacker.pokemon->name)))
				or (WATER == attacker.pokemon->move->type and (SPLASH_PLATE == attacker.pokemon->item or MYSTIC_WATER == attacker.pokemon->item or WAVE_INCENSE == attacker.pokemon->item or (LUSTROUS_ORB == attacker.pokemon->item and PALKIA == attacker.pokemon->name))))
		attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;

	if (attacker.charge and ELECTRIC == attacker.pokemon->move->type)
		attacker.pokemon->move->power *= 2;

	if ((defender.mud_sport and ELECTRIC == attacker.pokemon->move->type) or (defender.water_sport and FIRE == attacker.pokemon->move->type))
		attacker.pokemon->move->power /= 2;

	if ((TECHNICIAN == attacker.pokemon->ability and attacker.pokemon->move->basepower <= 60)
			or (BLAZE == attacker.pokemon->ability and FIRE == attacker.pokemon->move->type and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
			or (OVERGROW == attacker.pokemon->ability and GRASS == attacker.pokemon->move->type and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
			or (SWARM == attacker.pokemon->ability and BUG == attacker.pokemon->move->type and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
			or (TORRENT == attacker.pokemon->ability and WATER == attacker.pokemon->move->type and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3))
		attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
	else if (((IRON_FIST == attacker.pokemon->ability) and (BULLET_PUNCH == attacker.pokemon->move->name or COMET_PUNCH == attacker.pokemon->move->name or DIZZY_PUNCH == attacker.pokemon->move->name or DRAIN_PUNCH == attacker.pokemon->move->name or DYNAMICPUNCH == attacker.pokemon->move->name or FIRE_PUNCH == attacker.pokemon->move->name or FOCUS_PUNCH == attacker.pokemon->move->name or HAMMER_ARM == attacker.pokemon->move->name or ICE_PUNCH == attacker.pokemon->move->name or MACH_PUNCH == attacker.pokemon->move->name or MEGA_PUNCH == attacker.pokemon->move->name or METEOR_MASH == attacker.pokemon->move->name or SHADOW_PUNCH == attacker.pokemon->move->name or SKY_UPPERCUT == attacker.pokemon->move->name or THUNDERPUNCH == attacker.pokemon->move->name))
				or ((RECKLESS == attacker.pokemon->ability) and (attacker.pokemon->move->name == BRAVE_BIRD or attacker.pokemon->move->name == DOUBLE_EDGE or attacker.pokemon->move->name == FLARE_BLITZ or attacker.pokemon->move->name == HEAD_SMASH or attacker.pokemon->move->name == SUBMISSION or attacker.pokemon->move->name == TAKE_DOWN or attacker.pokemon->move->name == VOLT_TACKLE or attacker.pokemon->move->name == WOOD_HAMMER)))
		attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;
	else if (RIVALRY == attacker.pokemon->ability)
		attacker.pokemon->move->power = attacker.pokemon->move->power * (20 + 5 * attacker.pokemon->gender * defender.pokemon->gender) / 20;		// Same gender == 20 + 5, opposite gender == 20 - 5

	if ((HEATPROOF == defender.pokemon->ability and FIRE == attacker.pokemon->move->type)
			or (THICK_FAT == defender.pokemon->ability and (FIRE == attacker.pokemon->move->type or ICE == attacker.pokemon->move->type)))
		attacker.pokemon->move->power /= 2;
	else if (DRY_SKIN == defender.pokemon->ability and FIRE == attacker.pokemon->move->type)
		attacker.pokemon->move->power = attacker.pokemon->move->power * 5 / 4;

	if (attacker.pokemon->move->power == 0)
		attacker.pokemon->move->power = 1;
}

// I split my damage calculator up into a function that calculates as much as possible with known data, one that calculates without the random number, and a function that does the rest of the work because in many cases, I have the damage calculator in a deep inner loop, and pre-calculating non-random numbers allows me to move much of that calculator to a shallower part of code, and pre-calculating known information moves even more out. Profiling showed this to be a sound optimization.

int damageknown (Team const &attacker, Team const &defender, Weather const &weather, int &rl, int &weather_mod, int &ff, int &mf) {
	if (((0 != defender.reflect and attacker.pokemon->move->physical) or (0 != defender.light_screen and false == attacker.pokemon->move->physical)) and false == attacker.ch)
		rl = 2;
	else
		rl = 1;

	if ((0 != weather.rain and WATER == attacker.pokemon->move->type) or (0 != weather.sun and FIRE == attacker.pokemon->move->type))
		weather_mod = 3;
	else if ((0 != weather.rain and FIRE == attacker.pokemon->move->type) or (0 != weather.sun and WATER == attacker.pokemon->move->type))
		weather_mod = 1;
	else
		weather_mod = 2;

	if (attacker.ff and FIRE == attacker.pokemon->move->type)
		ff = 3;
	else
		ff = 2;

	if (attacker.mf)
		mf = 3;
	else
		mf = 2;

	return attacker.pokemon->level * 2 / 5 + 2;
}



int damagenonrandom (Team const &attacker, Team const &defender, int const &rl, int const &weather_mod, int const &ff, int const &mf, int &stab, int const &type1, int const &type2, int &aem, int &eb, int &tl, int &rb, int damage) {

	damage *= attacker.pokemon->move->power;

	if (attacker.pokemon->move->physical) {
		damage = damage * attacker.pokemon->atk.stat / 50 / defender.pokemon->def.stat;
		if (BURN == attacker.pokemon->status and GUTS != attacker.pokemon->ability)
			damage /= 2;
	}
	else
		damage = damage * attacker.pokemon->spa.stat / 50 / defender.pokemon->spd.stat;

	damage = damage / rl * weather_mod / 2 * ff / 2 + 2;

	if (attacker.ch) {
		if (SNIPER == attacker.pokemon->ability)
			damage *= 3;
		else
			damage *= 2;
	}

	if (LIFE_ORB == attacker.pokemon->item)
		damage = damage * 13 / 10;
	else if (METRONOME_ITEM == attacker.pokemon->item) {
		if (attacker.pokemon->move->times_used >= 10)
			damage *= 2;
		else
			damage = damage * (10 + attacker.pokemon->move->times_used) / 10;
	}

	damage = damage * mf / 2;

	if (istype (attacker, attacker.pokemon->move->type) and TYPELESS != attacker.pokemon->move->type) {
		if (ADAPTABILITY == attacker.pokemon->ability)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if (SOLID_ROCK == defender.pokemon->ability and type1 * type2 > 2)
		aem = 3;
	else
		aem = 4;

	if (EXPERT_BELT == attacker.pokemon->item and type1 * type2 > 2)
		eb = 6;
	else
		eb = 5;

	if (TINTED_LENS == attacker.pokemon->ability and type1 * type2 < 2)
		tl = 2;
	else
		tl = 1;

	if ((defender.pokemon->item == CHILAN_BERRY and attacker.pokemon->move->type == NORMAL) or (type1 * type2 > 2 and defender.pokemon->item - TANGA_BERRY == attacker.pokemon->move->type))
		rb = 2;
	else
		rb = 1;
	return damage;
}

int damagerandom (Pokemon const &attacker, Team const &defender, int const &stab, int const &type1, int const &type2, int const &aem, int const &eb, int const &tl, int const &rb, int damage) {
	damage = damage * attacker.move->r / 100 * stab / 2 * type1 / 2 * type2 / 2 * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage >= defender.pokemon->hp.stat) {
		damage = defender.pokemon->hp.stat;
		if (FALSE_SWIPE == attacker.move->name or defender.endure)
			--damage;
	}
	return damage;
}

int damagecalculator (Team const &attacker, Team const &defender, Weather const &weather) {
	int damage = 0;
	int const type1 = effectiveness [attacker.pokemon->move->type] [defender.pokemon->type1];		// Effectiveness on the defender's first type (1 if NVE, 4 if SE) / 2
	int const type2 = effectiveness [attacker.pokemon->move->type] [defender.pokemon->type2];		// Effectiveness on the defender's second type (1 if NVE, 4 if SE) / 2
	if ((type1 != 0 and type2 != 0) or (GROUND == attacker.pokemon->move->type and grounded (defender, weather))) {
		if (DRAGON_RAGE == attacker.pokemon->move->name)
			damage = 40;
		else if (ENDEAVOR == attacker.pokemon->move->name)
			damage = defender.pokemon->hp.stat - attacker.pokemon->hp.stat;
		else if (FISSURE == attacker.pokemon->move->name or GUILLOTINE == attacker.pokemon->move->name or HORN_DRILL == attacker.pokemon->move->name or SHEER_COLD == attacker.pokemon->move->name)
			damage = defender.pokemon->hp.max;
		else if (NIGHT_SHADE == attacker.pokemon->move->name or SEISMIC_TOSS == attacker.pokemon->move->name)
			damage = attacker.pokemon->level;
		else if (PSYWAVE == attacker.pokemon->move->name)
			damage = attacker.pokemon->level * attacker.pokemon->move->variable->first / 10;
		else if (SONICBOOM == attacker.pokemon->move->name)
			damage = 20;
		else if (SUPER_FANG == attacker.pokemon->move->name)
			damage = defender.pokemon->hp.stat / 2;

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
			damage = damagerandom (*attacker.pokemon, defender, stab, type1, type2, aem, eb, tl, rb, damage);
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
