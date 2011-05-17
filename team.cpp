// Load teams
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "stat.h"
#include "statfunction.h"
#include "switch.h"
#include "team.h"
#include "teampredictor.h"
#include "type.h"

namespace technicalmachine {

Team::Team (bool isme) :
	vanish (LANDED),
	damage (0),
	bide_damage (0),
	substitute (0),
	bide (0),
	confused (0),
	embargo (0),
	encore (0),
	heal_block (0),
	magnet_rise (0),
	partial_trap (0),
	perish_song (0),
	rampage (0),
	slow_start (0),
	stockpile (0),
	taunt (0),
	toxic (0),
	uproar (0),
	yawn (0),
	aqua_ring (false),
	attract (false),
	awaken (false),
	ch (false),
	charge (false),
	curse (false),
	damaged (false),
	defense_curl (false),
	destiny_bond (false),
	endure (false),
	ff (false),
	flinch (false),
	focus_energy (false),
	fully_paralyzed (false),
	gastro_acid (false),
	hitself (false),
	identified (false),
	imprison (false),
	ingrain (false),
	leech_seed (false),
	loaf (false),
	lock_on (false),
	mf (false),
	minimize (false),
	miss (false),
	moved (false),
	mud_sport (false),
	nightmare (false),
	pass (false),
	power_trick (false),
	recharging (false),
	replacing (false),
	roost (false),
	shed_skin (false),
	torment (false),
	trapped (false),
	water_sport (false),
	accuracy (0),
	evasion (0),
	
	counter (0),

	light_screen (0),
	lucky_chant (0),
	mist (0),
	reflect (0),
	safeguard (0),
	tailwind (0),

	wish (0),

	spikes (0),
	toxic_spikes (0),
	stealth_rock (false),

	replacement (0),
	size (6),
	me (isme) {
}

// Warning: Almost everything you see here is a hack.

// I do no error checking because I assume Pokelab's teams will always be in the proper format. This must be changed if I ever allow arbitary teams to be used.

void loadteam (Team &team, const std::string &name, const Map &map, int detailed [][7]) {
	if (name != "") {
		if (name.substr (name.length() - 3) == ".tp")
			poteam (team, name);
		else			// if (name.substr (name.length() - 4) == ".sbt")
			pokelabteam (team, name, map);
		for (std::vector<Pokemon>::iterator it = team.pokemon.set.begin(); it != team.pokemon.set.end(); ++it)
			loadpokemon (team, *it);
	}
}

void loadpokemon (Team &team, Pokemon &member) {
	if (member.hp.max == -1) {
		member.hp.max = hitpoints (member);
		member.hp.stat = member.hp.max;
	}
	bool struggle = false;
	moves_list switchn = static_cast<moves_list> (SWITCH0 - 1);
	for (std::vector<Move>::const_iterator it = member.move.set.begin(); it != member.move.set.end(); ++it) {
		if (it->name == STRUGGLE)
			struggle = true;
		if (SWITCH0 <= it->name and it->name <= SWITCH5)
			switchn = it->name;
	}
	if (!struggle) {
		Move move (STRUGGLE, 0);
		member.move.set.push_back (move);
	}

	// A Pokemon has a new "Switch" move for each Pokemon in the party.
	if (team.pokemon.set.size() > 1) {
		for (size_t index = switchn + 1; index - SWITCH0 < team.pokemon.set.size(); ++index) {
			Move move (static_cast<moves_list> (index), 0);
			member.move.set.push_back (move);
		}
	}
}

unsigned team_size (const std::string &name) {
	std::ifstream file (name.c_str());
	std::string line;
	unsigned size = 0;
	getline (file, line);
	while (!file.eof()) {
		if (line.find ("<pokemon species") != std::string::npos) {
			++size;
		}
		getline (file, line);
	}
	file.close();
	return size;
}

void pokelabteam (Team &team, const std::string &name, const Map &map) {
	unsigned size = team_size (name);
	std::ifstream file (name.c_str());
	for (unsigned n = 0; n != size; ++n)
		pokelabpokemon(team, file, map);
	file.close ();
}

void pokelabpokemon (Team& team, std::ifstream &file, const Map &map) {	// Replace this with a real XML parser. Couldn't figure out TinyXML, should try Xerces.
	std::string output2;	// Some lines have more than one data point.
	std::string output1 = search (file, output2, "species=\"");
	Pokemon member (map.specie.find (output1)->second);
	member.nickname = search (file, output2, "<nickname>");
	if (member.nickname == "")
		member.nickname = output1;
	member.level = boost::lexical_cast <int> (search (file, output2, "<level>"));
	member.happiness = boost::lexical_cast <int> (search (file, output2, "<happiness>"));
	member.gender = map.gender.find (search (file, output2, "<gender>"))->second;
	member.nature = map.nature.find (search (file, output2, "<nature>"))->second;
	member.item = map.item.find (search (file, output2, "<item>"))->second;
	member.ability = map.ability.find (search (file, output2, "<ability>"))->second;
	
	
	while (true) {
		output1 = search (file, output2, "\">");
		if ("No" == output1)
			break;
		moves_list name = map.move.find (output1)->second;
		int pp_ups = boost::lexical_cast <int> (output2);
		Move move (name, pp_ups);
		member.move.set.push_back (move);
	}
	
	member.hp.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.hp.ev = boost::lexical_cast <int> (output2) / 4;
	member.atk.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.atk.ev = boost::lexical_cast <int> (output2) / 4;
	member.def.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.def.ev = boost::lexical_cast <int> (output2) / 4;
	member.spe.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spe.ev = boost::lexical_cast <int> (output2) / 4;
	member.spa.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spa.ev = boost::lexical_cast <int> (output2) / 4;
	member.spd.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spd.ev = boost::lexical_cast <int> (output2) / 4;
	
	team.pokemon.set.push_back (member);
}

std::string search (std::ifstream &file, std::string &output2, const std::string &data) {
	std::string output1 = "";
	while (!file.eof() and "" == output1) {
		std::string line;
		getline (file, line);
		size_t x = data.length();
		size_t a = line.find (data);
		size_t b = line.find ("</", a + x);
		if (b == std::string::npos)
			b = line.find ("\" ev");
		if (b == std::string::npos)
			b = line.find ("\">", a + x);
		if (a != std::string::npos and b != std::string::npos) {
			output1 = line.substr (a + x, b - a - x);			// finds the data between the tags
			if (output1 == "")
				break;
			if ("iv=\"" == data) {
				a = line.find ("ev=\"");
				b = line.find ("\" /");
				output2 = line.substr (a + x, b - a - x);
			}
			else if ("\">" == data) {
				x = 7;
				a = line.find ("pp-up=\"");
				b = line.find (data);
				output2 = line.substr (a + x, b - a - x);
			}
		}
		if (line.find ("/moveset") != std::string::npos and "\">" == data)
			output1 = "No";
	}
	return output1;
}

void poteam (Team &team, const std::string &name) {
	const species pokemon_converter [] = { END_SPECIES, BULBASAUR, IVYSAUR, VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, RATTATA, RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, RAICHU, SANDSHREW, SANDSLASH, NIDORAN_F, NIDORINA, NIDOQUEEN, NIDORAN_M, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, DUGTRIO, MEOWTH, PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, GROWLITHE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MACHOP, MACHOKE, MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, GRAVELER, GOLEM, PONYTA, RAPIDASH, SLOWPOKE, SLOWBRO, MAGNEMITE, MAGNETON, FARFETCHD, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, MUK, SHELGON, CLOYSTER, GASTLY, HAUNTER, GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, VOLTORB, ELECTRODE, EXEGGCUTE, EXEGGUTOR, CUBONE, MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, TAUROS, MAGIKARP, GYARADOS, LAPRAS, DITTO, EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, SNORLAX, ARTICUNO, ZAPDOS, MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, MISDREAVUS, UNOWN, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, SNUBBULL, GRANBULL, QWILFISH, SCIZOR, SHUCKLE, HERACROSS, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MAWILE, ARON, LAIRON, AGGRON, MEDITITE, MEDICHAM, ELECTRIKE, MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, KECLEON, SHUPPET, BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, WYNAUT, SNORUNT, GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELLDER, SALAMENCE, BELDUM, METANG, METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, LATIOS, KYOGRE, GROUDON, RAYQUAZA, JIRACHI, DEOXYS_A, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, WORMADAM_P, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, SHELLOS, GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, UXIE, MESPRIT, AZELF, DIALGA, PALKIA, HEATRAN, REGIGIGAS, GIRATINA_A, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN_L, ARCEUS };
	const abilities ability_converter [] = { STENCH, DRIZZLE, SPEED_BOOST, BATTLE_ARMOR, STURDY, DAMP, LIMBER, SAND_VEIL, STATIC, VOLT_ABSORB, WATER_ABSORB, OBLIVIOUS, AIR_LOCK, COMPOUNDEYES, INSOMNIA, COLOR_CHANGE, IMMUNITY, FLASH_FIRE, SHIELD_DUST, OWN_TEMPO, SUCTION_CUPS, INTIMIDATE, SHADOW_TAG, ROUGH_SKIN, WONDER_GUARD, LEVITATE, EFFECT_SPORE, SYNCHRONIZE, CLEAR_BODY, NATURAL_CURE, LIGHTNINGROD, SERENE_GRACE, SWIFT_SWIM, CHLOROPHYLL, STENCH, TRACE, PURE_POWER, POISON_POINT, INNER_FOCUS, MAGMA_ARMOR, WATER_VEIL, MAGNET_PULL, SOUNDPROOF, RAIN_DISH, SAND_STREAM, PRESSURE, THICK_FAT, EARLY_BIRD, FLAME_BODY, STENCH, KEEN_EYE, HYPER_CUTTER, STENCH, TRUANT, HUSTLE, CUTE_CHARM, PLUS, MINUS, FORECAST, STICKY_HOLD, SHED_SKIN, GUTS, MARVEL_SCALE, LIQUID_OOZE, OVERGROW, BLAZE, TORRENT, SWARM, ROCK_HEAD, DROUGHT, ARENA_TRAP, INSOMNIA, CLEAR_BODY, PURE_POWER, BATTLE_ARMOR, AIR_LOCK, TANGLED_FEET, MOTOR_DRIVE, RIVALRY, STEADFAST, SNOW_CLOAK, GLUTTONY, ANGER_POINT, UNBURDEN, HEATPROOF, SIMPLE, DRY_SKIN, DOWNLOAD, IRON_FIST, POISON_HEAL, ADAPTABILITY, SKILL_LINK, HYDRATION, SOLAR_POWER, QUICK_FEET, NORMALIZE, SNIPER, MAGIC_GUARD, NO_GUARD, STALL, TECHNICIAN, LEAF_GUARD, KLUTZ, MOLD_BREAKER, SUPER_LUCK, AFTERMATH, ANTICIPATION, FOREWARN, UNAWARE, TINTED_LENS, SOLID_ROCK, SLOW_START, SCRAPPY, STORM_DRAIN, ICE_BODY, SOLID_ROCK, SNOW_WARNING, STENCH, FRISK, RECKLESS, MULTITYPE, FLOWER_GIFT, BAD_DREAMS };
	const items item_converter [] = { NO_ITEM, BIG_ROOT, OTHER10, BRIGHTPOWDER, CHOICE_BAND, CHOICE_SCARF, CHOICE_SPECS, DESTINY_KNOT, EXPERT_BELT, FOCUS_BAND, FOCUS_SASH, LAGGING_TAIL, OTHER10, LAGGING_TAIL, LAX_INCENSE, LEFTOVERS, OTHER10, MENTAL_HERB, METAL_POWDER, MUSCLE_BAND, ODD_INCENSE, OTHER10, POWER_HERB, OTHER10, QUICK_POWDER, OTHER10, OTHER10, ROCK_INCENSE, ROSE_INCENSE, WAVE_INCENSE, SHED_SHELL, SILK_SCARF, SILVERPOWDER, SMOOTH_ROCK, SOFT_SAND, OTHER10, WAVE_INCENSE, WHITE_HERB, WIDE_LENS, WISE_GLASSES, OTHER10, ZOOM_LENS, OTHER30, OTHER30, OTHER30, BERRY_JUICE, OTHER30, OTHER30, BLACK_BELT, OTHER30, BLACK_SLUDGE, BLACKGLASSES, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, CHARCOAL, OTHER30, OTHER30, DEEPSEASCALE, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, FLAME_ORB, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, KINGS_ROCK, OTHER30, OTHER30, OTHER30, LIFE_ORB, LIGHT_BALL, LIGHT_CLAY, OTHER30, MAGNET, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, METAL_COAT, METRONOME_ITEM, MIRACLE_SEED, OTHER30, OTHER30, MYSTIC_WATER, NEVERMELTICE, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, KINGS_ROCK, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, SCOPE_LENS, SHELL_BELL, OTHER30, OTHER30, OTHER30, OTHER30, SOUL_DEW, SPELL_TAG, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, TOXIC_ORB, TWISTEDSPOON, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, OTHER30, ICY_ROCK, LUCKY_PUNCH, OTHER50, SHARP_BEAK, ADAMANT_ORB, DAMP_ROCK, HEAT_ROCK, LUSTROUS_ORB, MACHO_BRACE, STICK, DRAGON_FANG, POISON_BARB, POWER_ITEMS, POWER_ITEMS, POWER_ITEMS, POWER_ITEMS, POWER_ITEMS, POWER_ITEMS, OTHER80, OTHER80, OTHER80, OTHER80, OTHER80, OTHER80, OTHER80,QUICK_CLAW, RAZOR_CLAW, OTHER80, STICKY_BARB, DEEPSEATOOTH, DRACO_PLATE, DREAD_PLATE, EARTH_PLATE, FIST_PLATE, FLAME_PLATE, GRIP_CLAW, ICICLE_PLATE, INSECT_PLATE, IRON_PLATE, MEADOW_PLATE, MIND_PLATE, SKY_PLATE, SPLASH_PLATE, SPOOKY_PLATE, STONE_PLATE, THICK_CLUB, TOXIC_PLATE, ZAP_PLATE, OTHER100, OTHER100, OTHER100, HARD_STONE, OTHER100, OTHER100, OTHER100, OTHER100, OTHER100, IRON_BALL, GRISEOUS_ORB, MAIL, MAIL, MAIL, MAIL, MAIL, MAIL, CHERI_BERRY, CHESTO_BERRY, PECHA_BERRY, RAWST_BERRY, ASPEAR_BERRY, LEPPA_BERRY, ORAN_BERRY, PERSIM_BERRY, LUM_BERRY, SITRUS_BERRY, FIGY_BERRY, WIKI_BERRY, MAGO_BERRY, AGUAV_BERRY, IAPAPA_BERRY, RAZZ_BERRY, BLUK_BERRY, NANAB_BERRY, WEPEAR_BERRY, PINAP_BERRY, POMEG_BERRY, KELPSY_BERRY, QUALOT_BERRY, HONDEW_BERRY, GREPA_BERRY, TAMATO_BERRY, CORNN_BERRY, MAGOST_BERRY, RABUTA_BERRY, NOMEL_BERRY, SPELON_BERRY, PAMTRE_BERRY, WATMEL_BERRY, DURIN_BERRY, BELUE_BERRY, OCCA_BERRY, PASSHO_BERRY, WACAN_BERRY, RINDO_BERRY, YACHE_BERRY, CHOPLE_BERRY, KEBIA_BERRY, SHUCA_BERRY, COBA_BERRY, PAYAPA_BERRY, TANGA_BERRY, CHARTI_BERRY, KASIB_BERRY, HABAN_BERRY, COLBUR_BERRY, BABIRI_BERRY, CHILAN_BERRY, LIECHI_BERRY, GANLON_BERRY, SALAC_BERRY, PETAYA_BERRY, APICOT_BERRY, LANSAT_BERRY, STARF_BERRY, ENIGMA_BERRY, MICLE_BERRY, CUSTAP_BERRY, JABOCA_BERRY, ROWAP_BERRY };
	const natures nature_converter [] = { HARDY, LONELY, BRAVE, ADAMANT, NAUGHTY, BOLD, DOCILE, RELAXED, IMPISH, LAX, TIMID, HASTY, SERIOUS, JOLLY, NAIVE, MODEST, MILD, QUIET, BASHFUL, RASH, CALM, GENTLE, SASSY, CAREFUL, QUIRKY };
	const moves_list move_converter [] = { END_MOVE, POUND, KARATE_CHOP, DOUBLESLAP, COMET_PUNCH, MEGA_PUNCH, PAY_DAY, FIRE_PUNCH, ICE_PUNCH, THUNDERPUNCH, SCRATCH, VICEGRIP, GUILLOTINE, RAZOR_WIND, SWORDS_DANCE, CUT, GUST, WING_ATTACK, WHIRLWIND, FLY, BIND, SLAM, VINE_WHIP, STOMP, DOUBLE_KICK, MEGA_KICK, JUMP_KICK, ROLLING_KICK, SAND_ATTACK, HEADBUTT, HORN_ATTACK, FURY_ATTACK, HORN_DRILL, TACKLE, BODY_SLAM, WRAP, TAKE_DOWN, THRASH, DOUBLE_EDGE, TAIL_WHIP, POISON_STING, TWINEEDLE, PIN_MISSILE, LEER, BITE, GROWL, ROAR, SING, SUPERSONIC, SONICBOOM, DISABLE, ACID, EMBER, FLAMETHROWER, MIST, WATER_GUN, HYDRO_PUMP, SURF, ICE_BEAM, BLIZZARD, PSYBEAM, BUBBLEBEAM, AURORA_BEAM, HYPER_BEAM, PECK, DRILL_PECK, SUBMISSION, LOW_KICK, COUNTER, SEISMIC_TOSS, STRENGTH, ABSORB, MEGA_DRAIN, LEECH_SEED, GROWTH, RAZOR_LEAF, SOLARBEAM, POISONPOWDER, STUN_SPORE, SLEEP_POWDER, PETAL_DANCE, STRING_SHOT, DRAGON_RAGE, FIRE_SPIN, THUNDERSHOCK, THUNDERBOLT, THUNDER_WAVE, THUNDER, ROCK_THROW, EARTHQUAKE, FISSURE, DIG, TOXIC, CONFUSION, PSYCHIC_MOVE, HYPNOSIS, MEDITATE, AGILITY, QUICK_ATTACK, RAGE, TELEPORT, NIGHT_SHADE, MIMIC, SCREECH, DOUBLE_TEAM, RECOVER, HARDEN, MINIMIZE, SMOKESCREEN, CONFUSE_RAY, WITHDRAW, DEFENSE_CURL, BARRIER, LIGHT_SCREEN, HAZE, REFLECT, FOCUS_ENERGY, BIDE, METRONOME_MOVE, MIRROR_MOVE, SELFDESTRUCT, EGG_BOMB, LICK, SMOG, SLUDGE, BONE_CLUB, FIRE_BLAST, WATERFALL, CLAMP, SWIFT, SKULL_BASH, SPIKE_CANNON, CONSTRICT, AMNESIA, KINESIS, SOFTBOILED, HI_JUMP_KICK, GLARE, DREAM_EATER, POISON_GAS, BARRAGE, LEECH_LIFE, LOVELY_KISS, SKY_ATTACK, TRANSFORM, BUBBLE, DIZZY_PUNCH, SPORE, FLASH, PSYWAVE, SPLASH, ACID_ARMOR, CRABHAMMER, EXPLOSION, FURY_SWIPES, BONEMERANG, REST, ROCK_SLIDE, HYPER_FANG, SHARPEN, CONVERSION, TRI_ATTACK, SUPER_FANG, SLASH, SUBSTITUTE, STRUGGLE, SKETCH, TRIPLE_KICK, THIEF, SPIDER_WEB, MIND_READER, NIGHTMARE, FLAME_WHEEL, SNORE, CURSE, FLAIL, CONVERSION2, AEROBLAST, COTTON_SPORE, REVERSAL, SPITE, POWDER_SNOW, PROTECT, MACH_PUNCH, SCARY_FACE, FAINT_ATTACK, SWEET_KISS, BELLY_DRUM, SLUDGE_BOMB, MUD_SLAP, OCTAZOOKA, SPIKES, ZAP_CANNON, FORESIGHT, DESTINY_BOND, PERISH_SONG, ICY_WIND, DETECT, BONE_RUSH, LOCK_ON, OUTRAGE, SANDSTORM, GIGA_DRAIN, ENDURE, CHARM, ROLLOUT, FALSE_SWIPE, SWAGGER, MILK_DRINK, SPARK, FURY_CUTTER, STEEL_WING, MEAN_LOOK, ATTRACT, SLEEP_TALK, HEAL_BELL, RETURN, PRESENT, FRUSTRATION, SAFEGUARD, PAIN_SPLIT, SACRED_FIRE, MAGNITUDE, DYNAMICPUNCH, MEGAHORN, DRAGONBREATH, BATON_PASS, ENCORE, PURSUIT, RAPID_SPIN, SWEET_SCENT, IRON_TAIL, METAL_CLAW, VITAL_THROW, MORNING_SUN, SYNTHESIS, MOONLIGHT, HIDDEN_POWER, CROSS_CHOP, TWISTER, RAIN_DANCE, SUNNY_DAY, CRUNCH, MIRROR_COAT, PSYCH_UP, EXTREMESPEED, ANCIENTPOWER, SHADOW_BALL, FUTURE_SIGHT, ROCK_SMASH, WHIRLPOOL, BEAT_UP, FAKE_OUT, UPROAR, STOCKPILE, SPIT_UP, SWALLOW, HEAT_WAVE, HAIL, TORMENT, FLATTER, WILL_O_WISP, MEMENTO, FACADE, FOCUS_PUNCH, SMELLINGSALT, FOLLOW_ME, NATURE_POWER, CHARGE, TAUNT, HELPING_HAND, TRICK, ROLE_PLAY, WISH, ASSIST, INGRAIN, SUPERPOWER, MAGIC_COAT, RECYCLE, REVENGE, BRICK_BREAK, YAWN, KNOCK_OFF, ENDEAVOR, ERUPTION, SKILL_SWAP, IMPRISON, REFRESH, GRUDGE, SNATCH, SECRET_POWER, DIVE, ARM_THRUST, CAMOUFLAGE, TAIL_GLOW, LUSTER_PURGE, MIST_BALL, FEATHERDANCE, TEETER_DANCE, BLAZE_KICK, MUD_SPORT, ICE_BALL, NEEDLE_ARM, SLACK_OFF, HYPER_VOICE, POISON_FANG, CRUSH_CLAW, BLAST_BURN, HYDRO_CANNON, METEOR_MASH, ASTONISH, WEATHER_BALL, AROMATHERAPY, FAKE_TEARS, AIR_CUTTER, OVERHEAT, ODOR_SLEUTH, ROCK_TOMB, SILVER_WIND, METAL_SOUND, GRASSWHISTLE, TICKLE, COSMIC_POWER, WATER_SPOUT, SIGNAL_BEAM, SHADOW_PUNCH, EXTRASENSORY, SKY_UPPERCUT, SAND_TOMB, SHEER_COLD, MUDDY_WATER, BULLET_SEED, AERIAL_ACE, ICICLE_SPEAR, IRON_DEFENSE, BLOCK, HOWL, DRAGON_CLAW, FRENZY_PLANT, BULK_UP, BOUNCE, MUD_SHOT, POISON_TAIL, COVET, VOLT_TACKLE, MAGICAL_LEAF, WATER_SPORT, CALM_MIND, LEAF_BLADE, DRAGON_DANCE, ROCK_BLAST, SHOCK_WAVE, WATER_PULSE, DOOM_DESIRE, PSYCHO_BOOST, ROOST, GRAVITY, MIRACLE_EYE, WAKE_UP_SLAP, HAMMER_ARM, GYRO_BALL, HEALING_WISH, BRINE, NATURAL_GIFT, FEINT, PLUCK, TAILWIND, ACUPRESSURE, METAL_BURST, U_TURN, CLOSE_COMBAT, PAYBACK, ASSURANCE, EMBARGO, FLING, PSYCHO_SHIFT, TRUMP_CARD, HEAL_BLOCK, WRING_OUT, POWER_TRICK, GASTRO_ACID, LUCKY_CHANT, ME_FIRST, COPYCAT, POWER_SWAP, GUARD_SWAP, PUNISHMENT, LAST_RESORT, WORRY_SEED, SUCKER_PUNCH, TOXIC_SPIKES, HEART_SWAP, AQUA_RING, MAGNET_RISE, FLARE_BLITZ, FORCE_PALM, AURA_SPHERE, ROCK_POLISH, POISON_JAB, DARK_PULSE, NIGHT_SLASH, AQUA_TAIL, SEED_BOMB, AIR_SLASH, X_SCISSOR, BUG_BUZZ, DRAGON_PULSE, DRAGON_RUSH, POWER_GEM, DRAIN_PUNCH, VACUUM_WAVE, FOCUS_BLAST, ENERGY_BALL, BRAVE_BIRD, EARTH_POWER, SWITCHEROO, GIGA_IMPACT, NASTY_PLOT, BULLET_PUNCH, AVALANCHE, ICE_SHARD, SHADOW_CLAW, THUNDER_FANG, ICE_FANG, FIRE_FANG, SHADOW_SNEAK, MUD_BOMB, PSYCHO_CUT, ZEN_HEADBUTT, MIRROR_SHOT, FLASH_CANNON, ROCK_CLIMB, DEFOG, TRICK_ROOM, DRACO_METEOR, DISCHARGE, LAVA_PLUME, LEAF_STORM, POWER_WHIP, ROCK_WRECKER, CROSS_POISON, GUNK_SHOT, IRON_HEAD, MAGNET_BOMB, STONE_EDGE, CAPTIVATE, STEALTH_ROCK, GRASS_KNOT, CHATTER, JUDGMENT, BUG_BITE, CHARGE_BEAM, WOOD_HAMMER, AQUA_JET, ATTACK_ORDER, DEFEND_ORDER, HEAL_ORDER, HEAD_SMASH, DOUBLE_HIT, ROAR_OF_TIME, SPACIAL_REND, LUNAR_DANCE, CRUSH_GRIP, MAGMA_STORM, DARK_VOID, SEED_FLARE, OMINOUS_WIND, SHADOW_FORCE };
	std::ifstream file (name.c_str());
	std::string line;
	getline (file, line);
	for (unsigned n = 0; n != 6; ++n)
		popokemon(team, file, pokemon_converter, ability_converter, item_converter, nature_converter, move_converter);
	file.close ();
}

void popokemon (Team &team, std::ifstream &file, const species pokemon_converter [], const abilities ability_converter [], const items item_converter [], const natures nature_converter [], const moves_list move_converter []) {
	std::string line;
	getline (file, line);
	getline (file, line);
	Pokemon member (pokemon_converter [poconverter ("Num=\"", "\"", line)]);
	int forme = poconverter ("Forme=\"", "\"", line);
	if (member.name == DEOXYS_A) {
		if (forme == 0)
			member.name = DEOXYS_M;
		else if (forme == 2)
			member.name = DEOXYS_D;
		else if (forme == 3)
			member.name = DEOXYS_S;
	}
	else if (member.name == GIRATINA_A)
		member.name = static_cast<species> (member.name + forme);		// Giratina-O has a forme value of 1
	else if (member.name == ROTOM) {
		if (forme == 1)
			member.name = ROTOM_C;
		else if (forme == 2)
			member.name = ROTOM_H;
		else if (forme == 3)
			member.name = ROTOM_F;
		else if (forme == 4)
			member.name = ROTOM_W;
		else if (forme == 5)
			member.name = ROTOM_S;
	}
	else if (member.name == SHAYMIN_L)
		member.name = static_cast<species> (member.name + forme);		// Shaymin-S has a forme value of 1
	else if (member.name == WORMADAM_P)
		member.name = static_cast<species> (member.name + forme);		// Wormadam-P is 0, Wormadam-S is 1, Wormadam-T is 2

	member.ability = ability_converter [poconverter ("Ability=\"", "\"", line)];

	member.item = item_converter [poconverter ("Item=\"", "\"", line)];
	
	member.nature = nature_converter [poconverter ("Nature=\"", "\"", line)];
	
	member.level = poconverter ("Lvl=\"", "\"", line);
	
	const std::string data = "Nickname=\"";
	const size_t x = data.length();
	const size_t a = line.find (data);
	const size_t b = line.find ("\"", a + x);
	member.nickname = line.substr (a + x, b - a - x);
	
	member.happiness = poconverter ("Happiness=\"", "\"", line);
	
	int gender = poconverter ("Gender=\"", "\"", line);
	if (gender == 2)
		member.gender = FEMALE;
	else
		member.gender = static_cast<genders> (gender);

	for (unsigned n = 0; n != 4; ++n) {
		getline (file, line);
		Move move (move_converter [poconverter ("<Move>", "</Move>", line)], 3);
		member.move.set.push_back (move);
	}
	getline (file, line);
	member.hp.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.atk.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.def.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spa.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spd.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spe.iv = poconverter ("<DV>", "</DV>", line);
	getline (file, line);
	member.hp.ev = poconverter ("<EV>", "</EV>", line);
	getline (file, line);
	member.atk.ev = poconverter ("<EV>", "</EV>", line);
	getline (file, line);
	member.def.ev = poconverter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spa.ev = poconverter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spd.ev = poconverter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spe.ev = poconverter ("<EV>", "</EV>", line);
	if (member.name != END_SPECIES and member.move.set.size() != 0)
		team.pokemon.set.push_back (member);
}

unsigned poconverter (const std::string &data, const std::string &end, const std::string &line) {
	const size_t x = data.length();
	const size_t a = line.find (data);
	const size_t b = line.find (end, a + x);
	return boost::lexical_cast<unsigned> (line.substr (a + x, b - a - x));
}

void output (Team const &team, std::string &output) {
	output = "======================\n";
	if (team.me)
		output += "AI";
	else
		output += "Foe";
	output += " team:\n";
	output += team.player + ":\n";
	for (std::vector<Pokemon>::const_iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		output += pokemon_name [pokemon->name];
		output += " (" + boost::lexical_cast<std::string> (100.0 * static_cast<double> (pokemon->hp.stat) / static_cast<double> (pokemon->hp.max)) + "% HP)";
		output += " @ " + item_name [pokemon->item];
		output += " ** " + pokemon->nickname + '\n';
		if (pokemon->ability != END_ABILITY)
			output += "\tAbility: " + ability_name [pokemon->ability] + '\n';
		for (std::vector<Move>::const_iterator move = pokemon->move.set.begin(); move->name != STRUGGLE; ++move)
			output += "\t- " + move_name [move->name] + "\n";
	}
}

}
