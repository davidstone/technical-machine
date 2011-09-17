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

#include <vector>

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

void movepower (Team & attacker, Team const & defender, Weather const weather) {

	// I account for the doubling of the base power for Pursuit in the switching function by simply multiplying the final base power by 2. Regardless of the combination of modifiers, this does not change the final base power. The exception is if the attacker's ally uses Helping Hand. The target uses U-turn and the attacker uses Pursuit with a slower Pokemon that has Rivalry and a Muscle Band and neither the attacker nor target is genderless. This will cause the base power to be 1 less than it should be.

	// Variable power moves
	
	switch (attacker.pokemon->move->name) {
		case Move::CRUSH_GRIP:
		case Move::WRING_OUT:
			attacker.pokemon->move->basepower = 120 * defender.pokemon->hp.stat / defender.pokemon->hp.max + 1;
			break;
		case Move::ERUPTION:
		case Move::WATER_SPOUT:
			attacker.pokemon->move->basepower = 150 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max;
			break;
		case Move::FLAIL:
		case Move::REVERSAL: {
			unsigned k = 64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max;
			if (k <= 1)
				attacker.pokemon->move->basepower = 200;
			else if (k <= 5)
				attacker.pokemon->move->basepower = 150;
			else if (k <= 12)
				attacker.pokemon->move->basepower = 100;
			else if (k <= 21)
				attacker.pokemon->move->basepower = 80;
			else if (k <= 42)
				attacker.pokemon->move->basepower = 40;
			else
				attacker.pokemon->move->basepower = 20;
			break;
		}
		case Move::FLING:
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
		case Move::FRUSTRATION:
			attacker.pokemon->move->basepower = 102 - attacker.pokemon->happiness * 2 / 5;
			break;
		case Move::FURY_CUTTER:
			// 10 * 2 ^ attacker.pokemon->move->times_used
			attacker.pokemon->move->basepower = 10 << attacker.pokemon->move->times_used;
			break;
		case Move::GRASS_KNOT:
		case Move::LOW_KICK:
			attacker.pokemon->move->basepower = defender.pokemon->mass;
			break;
		case Move::GYRO_BALL:
			attacker.pokemon->move->basepower = 25 * defender.pokemon->spe.stat / attacker.pokemon->spe.stat + 1;
			if (attacker.pokemon->move->basepower > 150)
				attacker.pokemon->move->basepower = 150;
			break;
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			// 30 * 2 ^ attacker.pokemon->move->times_used
			attacker.pokemon->move->basepower = 30 << attacker.pokemon->move->times_used;
			break;
		case Move::HIDDEN_POWER: {
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
		case Move::MAGNITUDE:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case Move::NATURAL_GIFT:
			switch (attacker.pokemon->item) {
				case AGUAV_BERRY:
				case ASPEAR_BERRY:
				case BABIRI_BERRY:
				case CHARTI_BERRY:
				case CHERI_BERRY:
				case CHESTO_BERRY:
				case CHILAN_BERRY:
				case CHOPLE_BERRY:
				case COBA_BERRY:
				case COLBUR_BERRY:
				case FIGY_BERRY:
				case HABAN_BERRY:
				case IAPAPA_BERRY:
				case KASIB_BERRY:
				case KEBIA_BERRY:
				case LEPPA_BERRY:
				case LUM_BERRY:
				case MAGO_BERRY:
				case OCCA_BERRY:
				case ORAN_BERRY:
				case PASSHO_BERRY:
				case PAYAPA_BERRY:
				case PECHA_BERRY:
				case PERSIM_BERRY:
				case RAWST_BERRY:
				case RAZZ_BERRY:
				case RINDO_BERRY:
				case SHUCA_BERRY:
				case SITRUS_BERRY:
				case TANGA_BERRY:
				case WACAN_BERRY:
				case WIKI_BERRY:
				case YACHE_BERRY:
					attacker.pokemon->move->basepower = 60;
					break;
				case BLUK_BERRY:
				case CORNN_BERRY:
				case GREPA_BERRY:
				case HONDEW_BERRY:
				case KELPSY_BERRY:
				case MAGOST_BERRY:
				case NANAB_BERRY:
				case NOMEL_BERRY:
				case PAMTRE_BERRY:
				case PINAP_BERRY:
				case POMEG_BERRY:
				case QUALOT_BERRY:
				case RABUTA_BERRY:
				case SPELON_BERRY:
				case TAMATO_BERRY:
				case WEPEAR_BERRY:
					attacker.pokemon->move->basepower = 70;
					break;
				case APICOT_BERRY:
				case BELUE_BERRY:
				case CUSTAP_BERRY:
				case DURIN_BERRY:
				case ENIGMA_BERRY:
				case GANLON_BERRY:
				case JABOCA_BERRY:
				case LANSAT_BERRY:
				case LIECHI_BERRY:
				case MICLE_BERRY:
				case PETAYA_BERRY:
				case ROWAP_BERRY:
				case SALAC_BERRY:
				case STARF_BERRY:
				case WATMEL_BERRY:
					attacker.pokemon->move->basepower = 80;
					break;
				default:
					break;
			}
			break;
		case Move::PRESENT:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case Move::PUNISHMENT:
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
		case Move::RETURN:
			attacker.pokemon->move->basepower = attacker.pokemon->happiness * 2 / 5;
			break;
		case Move::SPIT_UP:
			attacker.pokemon->move->basepower = attacker.stockpile * 100;
			break;
		case Move::TRIPLE_KICK:
			attacker.pokemon->move->basepower = 10 * attacker.pokemon->move->times_used;
			break;
		case Move::TRUMP_CARD:
			switch (attacker.pokemon->move->pp) {
				case 0:
					attacker.pokemon->move->basepower = 200;
					break;
				case 1:
					attacker.pokemon->move->basepower = 80;
					break;
				case 2:
					attacker.pokemon->move->basepower = 60;
					break;
				case 3:
					attacker.pokemon->move->basepower = 50;
					break;
				default:
					attacker.pokemon->move->basepower = 40;
					break;
			}
			break;
		default:
			break;
	}
	attacker.pokemon->move->power = attacker.pokemon->move->basepower;

	bool doubling = false;
	switch (attacker.pokemon->move->name) {
		case Move::ASSURANCE:
			if (defender.damaged)
				doubling = true;
			break;
		case Move::AVALANCHE: 
		case Move::REVENGE:
			if (attacker.damaged)
				doubling = true;
			break;
		case Move::BRINE:
			if (defender.pokemon->hp.stat <= defender.pokemon->hp.max / 2)
				doubling = true;
			break;
		case Move::EARTHQUAKE:
		case Move::MAGNITUDE:
			if (defender.vanish == DUG)
				doubling = true;
			break;
		case Move::FACADE:
			switch (attacker.pokemon->status) {
				case BURN:
				case PARALYSIS:
				case POISON_NORMAL:
				case POISON_TOXIC:
					doubling = true;
					break;
				default:
					break;
			}
			break;
		case Move::GUST:
			if (defender.vanish == BOUNCED or defender.vanish == FLOWN)
				doubling = true;
			break;
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			if (attacker.defense_curl)
				doubling = true;
			break;
		case Move::PAYBACK:
			if (defender.moved)
				doubling = true;
			break;
		case Move::SMELLINGSALT:
			if (defender.pokemon->status == PARALYSIS)
				doubling = true;
			break;
		case Move::SOLARBEAM:
			if (weather.rain == 0)
				doubling = true;
			break;
		case Move::STOMP:
			if (defender.minimize)
				doubling = true;
			break;
		case Move::SURF:
			if (defender.vanish == DIVED)
				doubling = true;
			break;
		case Move::WAKE_UP_SLAP:
			if (defender.pokemon->status == SLEEP)
				doubling = true;
			break;
		case Move::WEATHER_BALL:
			if (weather.hail or weather.rain or weather.sand or weather.sun)
				doubling = true;
			break;
		default:
			break;
	}
	if (doubling)
		attacker.pokemon->move->power *= 2;

	unsigned item_modifier = 10;
	switch (attacker.pokemon->item) {
		case MUSCLE_BAND:
			if (attacker.pokemon->move->physical)
				item_modifier = 11;
			break;
		case WISE_GLASSES:
			if (!attacker.pokemon->move->physical)
				item_modifier = 11;
			break;
		case INSECT_PLATE:
		case SILVERPOWDER:
			if (attacker.pokemon->move->type == BUG)
				item_modifier = 12;
			break;
		case DREAD_PLATE:	
		case BLACKGLASSES:
			if (attacker.pokemon->move->type == DARK)
				item_modifier = 12;
			break;
		case DRACO_PLATE:
		case DRAGON_FANG:
			if (attacker.pokemon->move->type == DRAGON)
				item_modifier = 12;
			break;
		case ZAP_PLATE:
		case MAGNET:
			if (attacker.pokemon->move->type == ELECTRIC)
				item_modifier = 12;
			break;
		case FIST_PLATE:
		case BLACK_BELT:
			if (attacker.pokemon->move->type == FIGHTING)
				item_modifier = 12;
			break;
		case FLAME_PLATE:
		case CHARCOAL:
			if (attacker.pokemon->move->type == FIRE)
				item_modifier = 12;
			break;
		case SKY_PLATE:
		case SHARP_BEAK:
			if (attacker.pokemon->move->type == FLYING)
				item_modifier = 12;
			break;
		case SPOOKY_PLATE:
		case SPELL_TAG:
			if (attacker.pokemon->move->type == GHOST)
 				item_modifier = 12;
			break;
		case MEADOW_PLATE:
		case MIRACLE_SEED:
			if (attacker.pokemon->move->type == GRASS)
				item_modifier = 12;
			break;
		case EARTH_PLATE:
		case SOFT_SAND:
			if (attacker.pokemon->move->type == GROUND)
				item_modifier = 12;
			break;
		case ICICLE_PLATE:
		case NEVERMELTICE:
			if (attacker.pokemon->move->type == ICE)
				item_modifier = 12;
			break;
		case SILK_SCARF:
			if (attacker.pokemon->move->type == NORMAL)
				item_modifier = 12;
			break;
		case TOXIC_PLATE:
		case POISON_BARB:
			if (attacker.pokemon->move->type == POISON)
				item_modifier = 12;
			break;
		case MIND_PLATE:
		case TWISTEDSPOON:
		case ODD_INCENSE:
			if (attacker.pokemon->move->type == PSYCHIC_TYPE)
				item_modifier = 12;
			break;
		case STONE_PLATE:
		case HARD_STONE:
		case ROCK_INCENSE:
			if (attacker.pokemon->move->type == ROCK)
				item_modifier = 12;
			break;
		case IRON_PLATE:
		case METAL_COAT:
			if (attacker.pokemon->move->type == STEEL)
				item_modifier = 12;
			break;
		case SPLASH_PLATE:
		case MYSTIC_WATER:
		case WAVE_INCENSE:
			if (attacker.pokemon->move->type == WATER)
				item_modifier = 12;
			break;
		case ADAMANT_ORB:
			if (attacker.pokemon->name == DIALGA and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == STEEL))
				item_modifier = 12;
			break;
		case GRISEOUS_ORB:
			if (attacker.pokemon->name == GIRATINA_O and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == GHOST))
				item_modifier = 12;
			break;
		case LUSTROUS_ORB:
			if (attacker.pokemon->name == PALKIA and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == WATER))
				item_modifier = 12;
			break;
		default:
			break;
	}
	attacker.pokemon->move->power = attacker.pokemon->move->power * item_modifier / 10;

	if (attacker.charge and attacker.pokemon->move->type == ELECTRIC)
		attacker.pokemon->move->power *= 2;

	if ((defender.mud_sport and attacker.pokemon->move->type == ELECTRIC) or (defender.water_sport and attacker.pokemon->move->type == FIRE))
		attacker.pokemon->move->power /= 2;

	switch (attacker.pokemon->ability) {
		case TECHNICIAN:
			if (attacker.pokemon->move->basepower <= 60)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case BLAZE:
			if (attacker.pokemon->move->type == FIRE and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case OVERGROW:
			if (attacker.pokemon->move->type == GRASS and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case SWARM:
			if (attacker.pokemon->move->type == BUG and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case TORRENT:
			if (attacker.pokemon->move->type == WATER and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case IRON_FIST:
			switch (attacker.pokemon->move->name) {
				case Move::BULLET_PUNCH:
				case Move::COMET_PUNCH:
				case Move::DIZZY_PUNCH:
				case Move::DRAIN_PUNCH:
				case Move::DYNAMICPUNCH:
				case Move::FIRE_PUNCH:
				case Move::FOCUS_PUNCH:
				case Move::HAMMER_ARM:
				case Move::ICE_PUNCH:
				case Move::MACH_PUNCH:
				case Move::MEGA_PUNCH:
				case Move::METEOR_MASH:
				case Move::SHADOW_PUNCH:
				case Move::SKY_UPPERCUT:
				case Move::THUNDERPUNCH:
					attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;
					break;
				default:
					break;
			}
		case RECKLESS:
			switch (attacker.pokemon->move->name) {
				case Move::BRAVE_BIRD:
				case Move::DOUBLE_EDGE:
				case Move::FLARE_BLITZ:
				case Move::HEAD_SMASH:
				case Move::SUBMISSION:
				case Move::TAKE_DOWN:
				case Move::VOLT_TACKLE:
				case Move::WOOD_HAMMER:
					attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;
					break;
				default:
					break;
		}
		case RIVALRY:
			// Same gender == 20 + 5, opposite gender == 20 - 5
			attacker.pokemon->move->power = attacker.pokemon->move->power * (20 + 5 * attacker.pokemon->gender * defender.pokemon->gender) / 20;
			break;
		default:
			break;
	}

	switch (defender.pokemon->ability) {
		case DRY_SKIN:
			if (attacker.pokemon->move->type == FIRE)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 5 / 4;
			break;
		case HEATPROOF:
			if (attacker.pokemon->move->type == FIRE)
				attacker.pokemon->move->power /= 2;
			break;
		case THICK_FAT:
			if (attacker.pokemon->move->type == FIRE or attacker.pokemon->move->type == ICE)
				attacker.pokemon->move->power /= 2;
			break;
		default:
			break;
	}

	if (attacker.pokemon->move->power == 0)
		attacker.pokemon->move->power = 1;
}

// I split my damage calculator up into a function that calculates as much as possible with known data, one that calculates without the random number, and a function that does the rest of the work because in many cases, I have the damage calculator in a deep inner loop, and pre-calculating non-random numbers allows me to move much of that calculator to a shallower part of code, and pre-calculating known information moves even more out. Profiling showed this to be a sound optimization.

unsigned damageknown (Team const & attacker, Team const & defender, Weather const & weather, unsigned & rl, unsigned & weather_mod, unsigned & ff, unsigned & mf) {
	if (((defender.reflect and attacker.pokemon->move->physical)
			or (defender.light_screen and !attacker.pokemon->move->physical))
			and !attacker.ch)
		rl = 2;
	else
		rl = 1;

	if ((weather.rain and attacker.pokemon->move->type == WATER)
			or (weather.sun and attacker.pokemon->move->type == FIRE))
		weather_mod = 3;
	else if ((weather.rain and attacker.pokemon->move->type == FIRE)
			or (weather.sun and attacker.pokemon->move->type == WATER))
		weather_mod = 1;
	else
		weather_mod = 2;

	if (attacker.ff and attacker.pokemon->move->type == FIRE)
		ff = 3;
	else
		ff = 2;

	if (attacker.mf)
		mf = 3;
	else
		mf = 2;

	return attacker.pokemon->level * 2 / 5 + 2;
}

unsigned damagenonrandom (Team const & attacker, Team const & defender, unsigned rl, unsigned weather_mod, unsigned ff, unsigned mf, unsigned & stab, unsigned effectiveness, unsigned & aem, unsigned & eb, unsigned & tl, unsigned & rb, unsigned damage) {

	damage *= attacker.pokemon->move->power;

	if (attacker.pokemon->move->physical) {
		damage = damage * attacker.pokemon->atk.stat / 50 / defender.pokemon->def.stat;
		if (attacker.pokemon->status == BURN and attacker.pokemon->ability != GUTS)
			damage /= 2;
	}
	else
		damage = damage * attacker.pokemon->spa.stat / 50 / defender.pokemon->spd.stat;

	damage = damage / rl * weather_mod / 2 * ff / 2 + 2;

	if (attacker.ch) {
		if (attacker.pokemon->ability == SNIPER)
			damage *= 3;
		else
			damage *= 2;
	}

	if (attacker.pokemon->item == LIFE_ORB)
		damage = damage * 13 / 10;
	else if (attacker.pokemon->item == METRONOME_ITEM) {
		if (attacker.pokemon->move->times_used >= 10)
			damage *= 2;
		else
			damage = damage * (10 + attacker.pokemon->move->times_used) / 10;
	}

	damage = damage * mf / 2;

	if (is_type (attacker, attacker.pokemon->move->type) and attacker.pokemon->move->type != TYPELESS) {
		if (attacker.pokemon->ability == ADAPTABILITY)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if (defender.pokemon->ability == SOLID_ROCK and effectiveness > 2)
		aem = 3;
	else
		aem = 4;

	if (attacker.pokemon->item == EXPERT_BELT and effectiveness > 2)
		eb = 6;
	else
		eb = 5;

	if (attacker.pokemon->ability == TINTED_LENS and effectiveness < 2)
		tl = 2;
	else
		tl = 1;

	rb = 1;
	if (defender.pokemon->item == CHILAN_BERRY and attacker.pokemon->move->type == NORMAL)
		rb = 2;
	else if (effectiveness > 2) {
		switch (attacker.pokemon->item) {
			case BABIRI_BERRY:
				if (attacker.pokemon->move->type == STEEL)
					rb = 2;
				break;
			case CHARTI_BERRY:
				if (attacker.pokemon->move->type == ROCK)
					rb = 2;
				break;
			case CHOPLE_BERRY:
				if (attacker.pokemon->move->type == FIGHTING)
					rb = 2;
				break;
			case COBA_BERRY:
				if (attacker.pokemon->move->type == FLYING)
					rb = 2;
				break;
			case COLBUR_BERRY:
				if (attacker.pokemon->move->type == DARK)
					rb = 2;
				break;
			case HABAN_BERRY:
				if (attacker.pokemon->move->type == DRAGON)
					rb = 2;
				break;
			case KASIB_BERRY:
				if (attacker.pokemon->move->type == GHOST)
					rb = 2;
				break;
			case KEBIA_BERRY:
				if (attacker.pokemon->move->type == POISON)
					rb = 2;
				break;
			case OCCA_BERRY:
				if (attacker.pokemon->move->type == FIRE)
					rb = 2;
				break;
			case PASSHO_BERRY:
				if (attacker.pokemon->move->type == WATER)
					rb = 2;
				break;
			case PAYAPA_BERRY:
				if (attacker.pokemon->move->type == PSYCHIC_TYPE)
					rb = 2;
				break;
			case RINDO_BERRY:
				if (attacker.pokemon->move->type == GRASS)
					rb = 2;
				break;
			case SHUCA_BERRY:
				if (attacker.pokemon->move->type == GROUND)
					rb = 2;
				break;
			case TANGA_BERRY:
				if (attacker.pokemon->move->type == BUG)
					rb = 2;
				break;
			case WACAN_BERRY:
				if (attacker.pokemon->move->type == ELECTRIC)
					rb = 2;
				break;
			case YACHE_BERRY:
				if (attacker.pokemon->move->type == ICE)
					rb = 2;
				break;
			default:
				break;
		}
	}

	return damage;
}

unsigned damagerandom (Pokemon const & attacker, Team const & defender, unsigned stab, std::vector <unsigned> const & effectiveness, unsigned aem, unsigned eb, unsigned tl, unsigned rb, unsigned damage) {
	damage = damage * attacker.move->r / 100 * stab / 2;
	for (std::vector <unsigned>::const_iterator it = effectiveness.begin(); it != effectiveness.end(); ++it)
		damage = damage * *it / 2;
	damage = damage * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage >= defender.pokemon->hp.stat) {
		damage = defender.pokemon->hp.stat;
		if (attacker.move->name == Move::FALSE_SWIPE or defender.endure)
			--damage;
	}
	return damage;
}

unsigned damagecalculator (Team const & attacker, Team const & defender, Weather const & weather) {
	unsigned damage = 0;
	unsigned effectiveness = get_effectiveness (attacker.pokemon->move->type, *defender.pokemon);
	if ((effectiveness > 0) and (attacker.pokemon->move->type != GROUND or grounded (defender, weather))) {
		switch (attacker.pokemon->move->name) {
			case Move::DRAGON_RAGE:
				damage = 40;
				break;
			case Move::ENDEAVOR:
				if (defender.pokemon->hp.stat > attacker.pokemon->hp.stat)
					damage = defender.pokemon->hp.stat - attacker.pokemon->hp.stat;
				else
					damage = 0;
				break;
			case Move::FISSURE:
			case Move::GUILLOTINE:
			case Move::HORN_DRILL:
			case Move::SHEER_COLD:
				damage = defender.pokemon->hp.max;
				break;
			case Move::NIGHT_SHADE:
			case Move::SEISMIC_TOSS:
				damage = attacker.pokemon->level;
				break;
			case Move::PSYWAVE:
				damage = attacker.pokemon->level * attacker.pokemon->move->variable->first / 10;
				break;
			case Move::SONICBOOM:
				damage = 20;
				break;
			case Move::SUPER_FANG:
				damage = defender.pokemon->hp.stat / 2;

			default: {
				unsigned rl;						// Reflect / Light Screen (2)
				unsigned weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
				unsigned ff;						// Flash Fire: 3 / 2
				unsigned mf;						// Me First: 3 / 2
				unsigned stab;					// Same Type Attack Bonus: 3 / 2
				unsigned aem;					// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
				unsigned eb;						// Expert Belt: 6 / 5
				unsigned tl;							// Tinted Lens (2)
				unsigned rb;						// Resistance berries (2)
				damage = damageknown (attacker, defender, weather, rl, weather_mod, ff, mf);
				damage = damagenonrandom (attacker, defender, rl, weather_mod, ff, mf, stab, effectiveness, aem, eb, tl, rb, damage);
				std::vector <unsigned> effectiveness_vector = get_effectiveness_variables (attacker.pokemon->move->type, *defender.pokemon);
				damage = damagerandom (*attacker.pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, damage);
			}
		}
	}
	return damage;
}

void recoil (Pokemon & user, unsigned damage, unsigned denominator) {
	if (user.ability != MAGIC_GUARD and user.ability != ROCK_HEAD) {
		if (damage <= 2 * denominator - 1)
			--user.hp.stat;
		else
			damage_side_effect (user, damage / denominator);
	}
}

void damage_side_effect (Pokemon & user, unsigned damage) {
	if (user.hp.stat > damage)
		user.hp.stat -= damage;
	else
		user.hp.stat = 0;
}

}
