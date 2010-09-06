// Pokemon data structure
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_H_
#define POKEMON_H_

#include <map>
#include <vector>
#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "stat.h"
#include "status.h"
#include "type.h"

enum species { ABOMASNOW, ABRA, ABSOL, AERODACTYL, AGGRON, AIPOM, ALAKAZAM, ALTARIA, AMBIPOM, AMPHAROS, ANORITH, ARBOK, ARCANINE, ARCEUS, ARIADOS, ARMALDO, ARON, ARTICUNO, AZELF, AZUMARILL, AZURILL, BAGON, BALTOY, BANETTE, BARBOACH, BASTIODON, BAYLEEF, BEAUTIFLY, BEEDRILL, BELDUM, BELLOSSOM, BELLSPROUT, BIBAREL, BIDOOF, BLASTOISE, BLAZIKEN, BLISSEY, BONSLY, BRELOOM, BRONZONG, BRONZOR, BUDEW, BUIZEL, BULBASAUR, BUNEARY, BURMY, BUTTERFREE, CACNEA, CACTURNE, CAMERUPT, CARNIVINE, CARVANHA, CASCOON, CASTFORM, CATERPIE, CELEBI, CHANSEY, CHARIZARD, CHARMANDER, CHARMELEON, CHATOT, CHERRIM, CHERUBI, CHIKORITA, CHIMCHAR, CHIMECHO, CHINCHOU, CHINGLING, CLAMPERL, CLAYDOL, CLEFABLE, CLEFAIRY, CLEFFA, CLOYSTER, COMBEE, COMBUSKEN, CORPHISH, CORSOLA, CRADILY, CRANIDOS, CRAWDAUNT, CRESSELIA, CROAGUNK, CROBAT, CROCONAW, CUBONE, CYNDAQUIL, DARKRAI, DELCATTY, DELIBIRD, DEOXYS_A, DEOXYS_D, DEOXYS_M, DEOXYS_S, DEWGONG, DIALGA, DIGLETT, DITTO, DODRIO, DODUO, DONPHAN, DRAGONAIR, DRAGONITE, DRAPION, DRATINI, DRIFBLIM, DRIFLOON, DROWZEE, DUGTRIO, DUNSPARCE, DUSCLOPS, DUSKNOIR, DUSKULL, DUSTOX, EEVEE, EKANS, ELECTABUZZ, ELECTIVIRE, ELECTRIKE, ELECTRODE, ELEKID, EMPOLEON, ENTEI, ESPEON, EXEGGCUTE, EXEGGUTOR, EXPLOUD, FARFETCHD, FEAROW, FEEBAS, FERALIGATR, FINNEON, FLAAFFY, FLAREON, FLOATZEL, FLYGON, FORRETRESS, FROSLASS, FURRET, GABITE, GALLADE, GARCHOMP, GARDEVOIR, GASTLY, GASTRODON, GENGAR, GEODUDE, GIBLE, GIRAFARIG, GIRATINA_A, GIRATINA_O, GLACEON, GLALIE, GLAMEOW, GLIGAR, GLISCOR, GLOOM, GOLBAT, GOLDEEN, GOLDUCK, GOLEM, GOREBYSS, GRANBULL, GRAVELER, GRIMER, GROTLE, GROUDON, GROVYLE, GROWLITHE, GRUMPIG, GULPIN, GYARADOS, HAPPINY, HARIYAMA, HAUNTER, HEATRAN, HERACROSS, HIPPOPOTAS, HIPPOWDON, HITMONCHAN, HITMONLEE, HITMONTOP, HO_OH, HONCHKROW, HOOTHOOT, HOPPIP, HORSEA, HOUNDOOM, HOUNDOUR, HUNTAIL, HYPNO, IGGLYBUFF, ILLUMISE, INFERNAPE, IVYSAUR, JIGGLYPUFF, JIRACHI, JOLTEON, JUMPLUFF, JYNX, KABUTO, KABUTOPS, KADABRA, KAKUNA, KANGASKHAN, KECLEON, KINGDRA, KINGLER, KIRLIA, KOFFING, KRABBY, KRICKETOT, KRICKETUNE, KYOGRE, LAIRON, LANTURN, LAPRAS, LARVITAR, LATIAS, LATIOS, LEAFEON, LEDIAN, LEDYBA, LICKILICKY, LICKITUNG, LILEEP, LINOONE, LOMBRE, LOPUNNY, LOTAD, LOUDRED, LUCARIO, LUDICOLO, LUGIA, LUMINEON, LUNATONE, LUVDISC, LUXIO, LUXRAY, MACHAMP, MACHOKE, MACHOP, MAGBY, MAGCARGO, MAGIKARP, MAGMAR, MAGMORTAR, MAGNEMITE, MAGNETON, MAGNEZONE, MAKUHITA, MAMOSWINE, MANAPHY, MANECTRIC, MANKEY, MANTINE, MANTYKE, MAREEP, MARILL, MAROWAK, MARSHTOMP, MASQUERAIN, MAWILE, MEDICHAM, MEDITITE, MEGANIUM, MEOWTH, MESPRIT, METAGROSS, METANG, METAPOD, MEW, MEWTWO, MIGHTYENA, MILOTIC, MILTANK, MIME_JR, MINUN, MISDREAVUS, MISMAGIUS, MOLTRES, MONFERNO, MOTHIM, MR_MIME, MUDKIP, MUK, MUNCHLAX, MURKROW, NATU, NIDOKING, NIDOQUEEN, NIDORAN_F, NIDORAN_M, NIDORINA, NIDORINO, NINCADA, NINETALES, NINJASK, NOCTOWL, NOSEPASS, NUMEL, NUZLEAF, OCTILLERY, ODDISH, OMANYTE, OMASTAR, ONIX, PACHIRISU, PALKIA, PARAS, PARASECT, PELIPPER, PERSIAN, PHANPY, PHIONE, PICHU, PIDGEOT, PIDGEOTTO, PIDGEY, PIKACHU, PILOSWINE, PINECO, PINSIR, PIPLUP, PLUSLE, POLITOED, POLIWAG, POLIWHIRL, POLIWRATH, PONYTA, POOCHYENA, PORYGON, PORYGON_Z, PORYGON2, PRIMEAPE, PRINPLUP, PROBOPASS, PSYDUCK, PUPITAR, PURUGLY, QUAGSIRE, QUILAVA, QWILFISH, RAICHU, RAIKOU, RALTS, RAMPARDOS, RAPIDASH, RATICATE, RATTATA, RAYQUAZA, REGICE, REGIGIGAS, REGIROCK, REGISTEEL, RELICANTH, REMORAID, RHYDON, RHYHORN, RHYPERIOR, RIOLU, ROSELIA, ROSERADE, ROTOM, ROTOM_C, ROTOM_F, ROTOM_H, ROTOM_S, ROTOM_W, SABLEYE, SALAMENCE, SANDSHREW, SANDSLASH, SCEPTILE, SCIZOR, SCYTHER, SEADRA, SEAKING, SEALEO, SEEDOT, SEEL, SENTRET, SEVIPER, SHARPEDO, SHAYMIN_L, SHAYMIN_S, SHEDINJA, SHELGON, SHELLDER, SHELLOS, SHIELDON, SHIFTRY, SHINX, SHROOMISH, SHUCKLE, SHUPPET, SILCOON, SKARMORY, SKIPLOOM, SKITTY, SKORUPI, SKUNTANK, SLAKING, SLAKOTH, SLOWBRO, SLOWKING, SLOWPOKE, SLUGMA, SMEARGLE, SMOOCHUM, SNEASEL, SNORLAX, SNORUNT, SNOVER, SNUBBULL, SOLROCK, SPEAROW, SPHEAL, SPINARAK, SPINDA, SPIRITOMB, SPOINK, SQUIRTLE, STANTLER, STARAPTOR, STARAVIA, STARLY, STARMIE, STARYU, STEELIX, STUNKY, SUDOWOODO, SUICUNE, SUNFLORA, SUNKERN, SURSKIT, SWABLU, SWALOT, SWAMPERT, SWELLOW, SWINUB, TAILLOW, TANGELA, TANGROWTH, TAUROS, TEDDIURSA, TENTACOOL, TENTACRUEL, TOGEKISS, TOGEPI, TOGETIC, TORCHIC, TORKOAL, TORTERRA, TOTODILE, TOXICROAK, TRAPINCH, TREECKO, TROPIUS, TURTWIG, TYPHLOSION, TYRANITAR, TYROGUE, UMBREON, UNOWN, URSARING, UXIE, VAPOREON, VENOMOTH, VENONAT, VENUSAUR, VESPIQUEN, VIBRAVA, VICTREEBEL, VIGOROTH, VILEPLUME, VOLBEAT, VOLTORB, VULPIX, WAILMER, WAILORD, WALREIN, WARTORTLE, WEAVILE, WEEDLE, WEEPINBELL, WEEZING, WHISCASH, WHISMUR, WIGGLYTUFF, WINGULL, WOBBUFFET, WOOPER, WORMADAM_P, WORMADAM_S, WORMADAM_T, WURMPLE, WYNAUT, XATU, YANMA, YANMEGA, ZANGOOSE, ZAPDOS, ZIGZAGOON, ZUBAT };

/*
I decided to go with alphabetical listing over Pokedex listing due to the issue of alternate forms. Alternate forms (like Deoxys-A vs. Deoxys-S) mean that I have two choices: I can have one be in the middle of the list, and the other be at the other end, or I can stick the forms together. Either way, it ruins the idea of referencing Pokemon by #, so to avoid any mistakes, I just went alphabetical.

Hyphens and spaces were replaced by underscores. Dots and apostrophes were removed.

The following are my format for species:

Deoxys-A(ttack), Deoxys-D(efense), Deoxys-M(ediocre), Deoxys-S(peed). All of these are standard except Deoxys-M, which is usually refered as simply Deoxys. On Shoddybattle, A = f, D = l, S = e, and M has no letter.

Giratina-A(nother), Giratina-O(rigin). Both of these are somewhat standard. Giratina-A is sometimes refered to as simply Giratina. On Shoddy, Giratina-A has no letter, and Giratina-O is Giratina-o.

The weaker Rotom form is Rotom. The appliance Rotom forms use their standard one-letter abbreviation. This is the same as Shoddybattle, except Shoddy does not capitalize the form letter.

Shaymin-L(and), Shaymin-S(ky). Shoddy uses Shaymin and Shaymin-s.

Wormadam-P(lant) = Grass-type, Wormadam-S(andy) = Ground-type, Wormadam-T(rash) = Steel-type. On Shoddybattle, P = no letter, S = g, T = s.
*/

struct pokemon {

	species pokemon;
	genders gender;
	statuses status;
	types type1;
	types type2;
	Vanish vanish;
	char confused;
	char embargo;
	char encore;
	char heal_block;
	char level;
	char magnet_rise;
	char mass;					// mass == power of Grass Knot / Low Kick, not the actual mass stat
	char partial_trap;			// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm, Sand Tomb, Whirlpool, and Wrap
	char perish_song;
	char rampage;				// Number of turns remaining on Outrage, Petal Dance, and Thrash
	char sleep;					// 2 through 5
	char slow_start;
	char stockpile;
	char taunt;
	char toxic;					// Number of turns this Pokemon has already taken Toxic damage (or would have if Magic Guard weren't in play)
	char uproar;				// Number of turns remaining on Uproar
	char yawn;
	bool aqua_ring;
	bool attract;
	bool charge;
	bool curse;
	bool damaged;
	bool defense_curl;
	bool ff;						// Flash Fire
	bool flinch;
	bool focus_energy;
	bool identified;
	bool imprison;				// Has this Pokemon used Imprison?
	bool ingrain;
	bool item_recycle;		// Was the item lost in a way that can be restored with Recycle?
	bool item_unburden;		// Was the item lost in a way that activates Unburden?
	bool leech_seed;
	bool loaf;
	bool mf;						// Me First
	bool minimize;
	bool moved;
	bool mud_sport;
	bool nightmare;
	bool roost;
	bool torment;
	bool trapped;				// Block, Mean Look, Spider Web
	bool water_sport;

	// Hidden variables intrinsic to the Pokemon
	unsigned char happiness;
	abilities ability;
	items item;
	natures nature;
	stats hp;
	stats atk;
	stats def;
	stats spa;
	stats spd;
	stats spe;
	std::vector<moves> moveset;
	std::vector<moves>::iterator move;			// The move currently being used
};

const unsigned char base_stat [][6] = {
	{ 90, 92, 75, 92, 85, 60 },			// Abomasnow
	{ 25, 20, 15, 105, 55, 90 },		// Abra	
	{ 65, 130, 60, 75, 60, 75 },		// Absol	
	{ 80, 105, 65, 60, 75, 130 },		// Aerodactyl	
	{ 70, 110, 180, 60, 60, 50 },		// Aggron	
	{ 55, 70, 55, 40, 55, 85 },			// Aipom
	{ 55, 50, 45, 135, 85, 120 },		// Alakazam	
	{ 75, 70, 90, 70, 105, 80 },		// Altaria	
	{ 75, 100, 66, 60, 66, 115 },		// Ambipom	
	{ 90, 75, 75, 115, 90, 55 },		// Ampharos	
	{ 45, 95, 50, 40, 50, 75 },			// Anorith
	{ 60, 85, 69, 65, 79, 80 },			// Arbok
	{ 90, 110, 80, 100, 80, 95 },		// Arcanine	
	{ 120, 120, 120, 120, 120, 120 },	// Arceus		
	{ 70, 90, 70, 60, 60, 40 },			// Ariados
	{ 75, 125, 100, 70, 80, 45 },		// Armaldo	
	{ 50, 70, 100, 40, 40, 30 },		// Aron	
	{ 90, 85, 100, 95, 125, 85 },		// Articuno	
	{ 75, 125, 70, 125, 70, 115 },		// Azelf	
	{ 100, 50, 80, 50, 80, 50 },		// Azumarill	
	{ 50, 20, 40, 20, 40, 20 },			// Azurill
	{ 45, 75, 60, 40, 30, 50 },			// Bagon
	{ 40, 40, 55, 40, 70, 55 },			// Baltoy
	{ 64, 115, 65, 83, 63, 65 },		// Banette	
	{ 50, 48, 43, 46, 41, 60 },			// Barboach
	{ 60, 52, 168, 47, 138, 30 },		// Bastiodon	
	{ 60, 62, 80, 63, 80, 60 },			// Bayleef
	{ 60, 70, 50, 90, 50, 65 },			// Beautifly
	{ 65, 80, 40, 45, 80, 75 },			// Beedrill
	{ 40, 55, 80, 35, 60, 30 },			// Beldum
	{ 75, 80, 85, 90, 100, 50 },		// Bellossom	
	{ 50, 75, 35, 70, 30, 40 },			// Bellsprout
	{ 79, 85, 60, 55, 60, 71 },			// Bibarel
	{ 59, 45, 40, 35, 40, 31 },			// Bidoof
	{ 79, 83, 100, 85, 105, 78 },		// Blastoise	
	{ 80, 120, 70, 110, 70, 80 },		// Blaziken	
	{ 255, 10, 10, 75, 135, 55 },		// Blissey	
	{ 50, 80, 95, 10, 45, 10 },			// Bonsly
	{ 60, 130, 80, 60, 60, 70 },		// Breloom	
	{ 67, 89, 116, 79, 116, 33 },		// Bronzong	
	{ 57, 24, 86, 24, 86, 23 },			// Bronzor
	{ 40, 30, 35, 50, 70, 55 },			// Budew
	{ 55, 65, 35, 60, 30, 85 },			// Buizel
	{ 45, 49, 49, 65, 65, 45 },			// Bulbasaur
	{ 55, 66, 44, 44, 56, 85 },			// Buneary
	{ 40, 29, 45, 29, 45, 36 },			// Burmy
	{ 60, 45, 50, 80, 80, 70 },			// Butterfree
	{ 50, 85, 40, 85, 40, 35 },			// Cacnea
	{ 70, 115, 60, 115, 60, 55 },		// Cacturne	
	{ 70, 100, 70, 105, 75, 40 },		// Camerupt	
	{ 74, 100, 72, 90, 72, 46 },		// Carnivine	
	{ 45, 90, 20, 65, 20, 65 },			// Carvanha
	{ 50, 35, 55, 25, 25, 15 },			// Cascoon
	{ 70, 70, 70, 70, 70, 70 },			// Castform
	{ 45, 30, 35, 20, 20, 45 },			// Caterpie
	{ 100, 100, 100, 100, 100, 100 },	// Celebi		
	{ 250, 5, 5, 35, 105, 50 },			// Chansey
	{ 78, 84, 78, 109, 85, 100 },		// Charizard	
	{ 39, 52, 43, 60, 50, 65 },			// Charmander
	{ 58, 64, 58, 80, 65, 80 },			// Charmeleon
	{ 76, 65, 45, 92, 42, 91 },			// Chatot
	{ 70, 60, 70, 87, 78, 85 },			// Cherrim
	{ 45, 35, 45, 62, 53, 35 },			// Cherubi
	{ 45, 49, 65, 49, 65, 45 },			// Chikorita
	{ 44, 58, 44, 58, 44, 61 },			// Chimchar
	{ 65, 50, 70, 95, 80, 65 },			// Chimecho
	{ 75, 38, 38, 56, 56, 67 },			// Chinchou
	{ 45, 30, 50, 65, 50, 45 },			// Chingling
	{ 35, 64, 85, 74, 55, 32 },			// Clamperl
	{ 60, 70, 105, 70, 120, 75 },		// Claydol	
	{ 95, 70, 73, 85, 90, 60 },			// Clefable
	{ 70, 45, 48, 60, 65, 35 },			// Clefairy
	{ 50, 25, 28, 45, 55, 15 },			// Cleffa
	{ 50, 95, 180, 85, 45, 70 },		// Cloyster	
	{ 30, 30, 42, 30, 42, 70 },			// Combee
	{ 60, 85, 60, 85, 60, 55 },			// Combusken
	{ 43, 80, 65, 50, 35, 35 },			// Corphish
	{ 55, 55, 85, 65, 85, 35 },			// Corsola
	{ 86, 81, 97, 81, 107, 43 },		// Cradily	
	{ 67, 125, 40, 30, 30, 58 },		// Cranidos	
	{ 63, 120, 85, 90, 55, 55 },		// Crawdaunt	
	{ 120, 70, 120, 75, 130, 85 },		// Cresselia	
	{ 48, 61, 40, 61, 40, 50 },			// Croagunk
	{ 85, 90, 80, 70, 80, 130 },		// Crobat	
	{ 65, 80, 80, 59, 63, 58 },			// Croconaw
	{ 50, 50, 95, 40, 50, 35 },			// Cubone
	{ 39, 52, 43, 60, 50, 65 },			// Cyndaquil
	{ 70, 90, 90, 135, 90, 125 },		// Darkrai	
	{ 70, 65, 65, 55, 55, 70 },			// Delcatty
	{ 45, 55, 45, 65, 45, 75 },			// Delibird
	{ 50, 180, 20, 180, 20, 150 },		// Deoxys-A	
	{ 50, 70, 160, 70, 160, 90 },		// Deoxys-D	
	{ 50, 150, 50, 150, 50, 150 },		// Deoxys-M	
	{ 50, 95, 90, 95, 90, 180 },		// Deoxys-S	
	{ 90, 70, 80, 70, 95, 70 },			// Dewgong
	{ 100, 120, 120, 150, 100, 90 },	// Dialga		
	{ 10, 55, 25, 35, 45, 95 },			// Diglett
	{ 48, 48, 48, 48, 48, 48 },			// Ditto
	{ 60, 110, 70, 60, 60, 100 },		// Dodrio	
	{ 35, 85, 45, 35, 35, 75 },			// Doduo
	{ 90, 120, 120, 60, 60, 50 },		// Donphan	
	{ 61, 84, 65, 70, 70, 70 },			// Dragonair
	{ 91, 134, 95, 100, 100, 80 },		// Dragonite	
	{ 70, 90, 110, 60, 75, 95 },		// Drapion	
	{ 41, 64, 45, 50, 50, 50 },			// Dratini
	{ 150, 80, 44, 90, 54, 80 },		// Drifblim	
	{ 90, 50, 34, 60, 44, 70 },			// Drifloon
	{ 60, 48, 45, 43, 90, 42 },			// Drowzee
	{ 35, 80, 50, 50, 70, 120 },		// Dugtrio	
	{ 100, 70, 70, 65, 65, 45 },		// Dunsparce	
	{ 40, 70, 130, 60, 130, 25 },		// Dusclops	
	{ 45, 100, 135, 65, 135, 45 },		// Dusknoir	
	{ 20, 40, 90, 30, 90, 25 },			// Duskull
	{ 60, 50, 70, 50, 90, 65 },			// Dustox
	{ 55, 55, 50, 45, 65, 55 },			// Eevee
	{ 35, 60, 44, 40, 54, 55 },			// Ekans
	{ 65, 83, 57, 95, 85, 105 },		// Electabuzz	
	{ 75, 123, 67, 95, 85, 95 },		// Electivire	
	{ 40, 45, 40, 65, 40, 65 },			// Electrike
	{ 60, 50, 70, 80, 80, 140 },		// Electrode	
	{ 45, 63, 37, 65, 55, 95 },			// Elekid
	{ 84, 86, 88, 111, 101, 60 },		// Empoleon	
	{ 115, 115, 85, 90, 75, 100 },		// Entei	
	{ 65, 65, 60, 130, 95, 110 },		// Espeon	
	{ 60, 40, 80, 60, 45, 40 },			// Exeggcute
	{ 95, 95, 85, 125, 65, 55 },		// Exeggutor	
	{ 104, 91, 63, 91, 63, 68 },		// Exploud	
	{ 52, 65, 55, 58, 62, 60 },			// Farfetch'd
	{ 65, 90, 65, 61, 61, 100 },		// Fearow	
	{ 20, 15, 20, 10, 55, 80 },			// Feebas
	{ 85, 105, 100, 79, 83, 78 },		// Feraligatr	
	{ 49, 49, 56, 49, 61, 66 },			// Finneon
	{ 70, 55, 55, 80, 60, 45 },			// Flaaffy
	{ 65, 130, 60, 95, 110, 65 },		// Flareon	
	{ 85, 105, 55, 85, 50, 115 },		// Floatzel	
	{ 80, 100, 80, 80, 80, 100 },		// Flygon	
	{ 75, 90, 140, 60, 60, 40 },		// Forretress	
	{ 70, 80, 70, 80, 70, 110 },		// Froslass	
	{ 85, 76, 64, 45, 55, 90 },			// Furret
	{ 68, 90, 65, 50, 55, 82 },			// Gabite
	{ 68, 125, 65, 65, 115, 80 },		// Gallade	
	{ 108, 130, 95, 80, 85, 102 },		// Garchomp	
	{ 68, 65, 65, 125, 115, 80 },		// Gardevoir	
	{ 30, 35, 30, 100, 35, 80 },		// Gastly	
	{ 111, 83, 68, 92, 82, 39 },		// Gastrodon	
	{ 60, 65, 60, 130, 75, 110 },		// Gengar	
	{ 40, 80, 100, 30, 30, 20 },		// Geodude	
	{ 58, 70, 45, 40, 45, 42 },			// Gible
	{ 70, 80, 65, 90, 65, 85 },			// Girafarig
	{ 150, 100, 120, 100, 120, 90 },	// Giratina-A		
	{ 150, 120, 100, 120, 100, 90 },	// Giratina-O		
	{ 65, 60, 110, 130, 95, 65 },		// Glaceon	
	{ 80, 80, 80, 80, 80, 80 },			// Glalie
	{ 49, 55, 42, 42, 37, 85 },			// Glameow
	{ 65, 75, 105, 35, 65, 85 },		// Gligar	
	{ 75, 95, 125, 45, 75, 95 },		// Gliscor	
	{ 60, 65, 70, 85, 75, 40 },			// Gloom
	{ 75, 80, 70, 65, 75, 90 },			// Golbat
	{ 45, 67, 60, 35, 50, 63 },			// Goldeen
	{ 80, 82, 78, 95, 80, 85 },			// Golduck
	{ 80, 110, 130, 55, 65, 45 },		// Golem	
	{ 55, 84, 105, 114, 75, 52 },		// Gorebyss	
	{ 90, 120, 75, 60, 60, 45 },		// Granbull	
	{ 55, 95, 115, 45, 45, 35 },		// Graveler	
	{ 80, 80, 50, 40, 50, 25 },			// Grimer
	{ 75, 89, 85, 55, 65, 36 },			// Grotle
	{ 100, 150, 140, 100, 90, 90 },		// Groudon	
	{ 50, 65, 45, 85, 65, 95 },			// Grovyle
	{ 55, 70, 45, 70, 50, 60 },			// Growlithe
	{ 80, 45, 65, 90, 110, 80 },		// Grumpig	
	{ 70, 43, 53, 43, 53, 40 },			// Gulpin
	{ 95, 125, 79, 60, 100, 81 },		// Gyarados	
	{ 100, 5, 5, 15, 65, 30 },			// Happiny
	{ 144, 120, 60, 40, 60, 50 },		// Hariyama	
	{ 45, 50, 45, 115, 55, 95 },		// Haunter	
	{ 91, 90, 106, 130, 106, 77 },		// Heatran	
	{ 80, 125, 75, 40, 95, 85 },		// Heracross	
	{ 68, 72, 78, 38, 42, 32 },			// Hippopotas
	{ 108, 112, 118, 68, 72, 47 },		// Hippowdon	
	{ 50, 105, 79, 35, 110, 76 },		// Hitmonchan	
	{ 50, 120, 53, 35, 110, 87 },		// Hitmonlee	
	{ 50, 95, 95, 35, 110, 70 },		// Hitmontop	
	{ 106, 130, 90, 110, 154, 90 },		// Ho-Oh	
	{ 100, 125, 52, 105, 52, 71 },		// Honchkrow	
	{ 60, 30, 30, 36, 56, 50 },			// Hoothoot
	{ 35, 35, 40, 35, 55, 50 },			// Hoppip
	{ 30, 40, 70, 70, 25, 60 },			// Horsea
	{ 75, 90, 50, 110, 80, 95 },		// Houndoom	
	{ 45, 60, 30, 80, 50, 65 },			// Houndour
	{ 55, 104, 105, 94, 75, 52 },		// Huntail	
	{ 85, 73, 70, 73, 115, 67 },		// Hypno	
	{ 90, 30, 15, 40, 20, 15 },			// Igglybuff
	{ 65, 47, 55, 73, 75, 85 },			// Illumise
	{ 76, 104, 71, 104, 71, 108 },		// Infernape	
	{ 60, 62, 63, 80, 80, 60 },			// Ivysaur
	{ 115, 45, 20, 45, 25, 20 },		// Jigglypuff	
	{ 100, 100, 100, 100, 100, 100 },	// Jirachi		
	{ 65, 65, 60, 110, 95, 130 },		// Jolteon	
	{ 75, 55, 70, 55, 85, 110 },		// Jumpluff	
	{ 65, 50, 35, 115, 95, 95 },		// Jynx	
	{ 30, 80, 90, 55, 45, 55 },			// Kabuto
	{ 60, 115, 105, 65, 70, 80 },		// Kabutops	
	{ 40, 35, 30, 120, 70, 105 },		// Kadabra	
	{ 45, 25, 50, 25, 25, 35 },			// Kakuna
	{ 105, 95, 80, 40, 80, 90 },		// Kangaskhan	
	{ 60, 90, 70, 60, 120, 40 },		// Kecleon	
	{ 75, 95, 95, 95, 95, 85 },			// Kingdra
	{ 55, 130, 115, 50, 50, 75 },		// Kingler	
	{ 38, 35, 35, 65, 55, 50 },			// Kirlia
	{ 40, 65, 95, 60, 45, 35 },			// Koffing
	{ 30, 105, 90, 25, 25, 50 },		// Krabby	
	{ 37, 25, 41, 25, 41, 25 },			// Kricketot
	{ 77, 85, 51, 55, 51, 65 },			// Kricketune
	{ 100, 100, 90, 150, 140, 90 },		// Kyogre	
	{ 60, 90, 140, 50, 50, 40 },		// Lairon	
	{ 125, 58, 58, 76, 76, 67 },		// Lanturn	
	{ 130, 85, 80, 85, 95, 60 },		// Lapras	
	{ 50, 64, 50, 45, 50, 41 },			// Larvitar
	{ 80, 80, 90, 110, 130, 110 },		// Latias	
	{ 80, 90, 80, 130, 110, 110 },		// Latios	
	{ 65, 110, 130, 60, 65, 95 },		// Leafeon	
	{ 55, 35, 50, 55, 110, 85 },		// Ledian	
	{ 40, 20, 30, 40, 80, 55 },			// Ledyba
	{ 110, 85, 95, 80, 95, 50 },		// Lickilicky	
	{ 90, 55, 75, 60, 75, 30 },			// Lickitung
	{ 66, 41, 77, 61, 87, 23 },			// Lileep
	{ 78, 70, 61, 50, 61, 100 },		// Linoone	
	{ 60, 50, 50, 60, 70, 50 },			// Lombre
	{ 65, 76, 84, 54, 96, 105 },		// Lopunny	
	{ 40, 30, 30, 40, 50, 30 },			// Lotad
	{ 84, 71, 43, 71, 43, 48 },			// Loudred
	{ 70, 110, 70, 115, 70, 90 },		// Lucario	
	{ 80, 70, 70, 90, 100, 70 },		// Ludicolo	
	{ 106, 90, 130, 90, 154, 110 },		// Lugia	
	{ 69, 69, 76, 69, 86, 91 },			// Lumineon
	{ 70, 55, 65, 95, 85, 70 },			// Lunatone
	{ 43, 30, 55, 40, 65, 97 },			// Luvdisc
	{ 60, 85, 49, 60, 49, 60 },			// Luxio
	{ 80, 120, 79, 95, 79, 70 },		// Luxray	
	{ 90, 130, 80, 65, 85, 55 },		// Machamp	
	{ 80, 100, 70, 50, 60, 45 },		// Machoke	
	{ 70, 80, 50, 35, 35, 35 },			// Machop
	{ 45, 75, 37, 70, 55, 83 },			// Magby
	{ 50, 50, 120, 80, 80, 30 },		// Magcargo	
	{ 20, 10, 55, 15, 20, 80 },			// Magikarp
	{ 65, 95, 57, 100, 85, 93 },		// Magmar	
	{ 75, 95, 67, 125, 95, 83 },		// Magmortar	
	{ 25, 35, 70, 95, 55, 45 },			// Magnemite
	{ 50, 60, 95, 120, 70, 70 },		// Magneton	
	{ 70, 70, 115, 130, 90, 60 },		// Magnezone	
	{ 72, 60, 30, 20, 30, 25 },			// Makuhita
	{ 110, 130, 80, 70, 60, 80 },		// Mamoswine	
	{ 100, 100, 100, 100, 100, 100 },	// Manaphy		
	{ 70, 75, 60, 105, 60, 105 },		// Manectric	
	{ 40, 80, 35, 35, 45, 70 },			// Mankey
	{ 65, 40, 70, 80, 140, 70 },		// Mantine	
	{ 45, 20, 50, 60, 120, 50 },		// Mantyke	
	{ 55, 40, 40, 65, 45, 35 },			// Mareep
	{ 70, 20, 50, 20, 50, 40 },			// Marill
	{ 60, 80, 110, 50, 80, 45 },		// Marowak	
	{ 70, 85, 70, 60, 70, 50 },			// Marshtomp
	{ 70, 60, 62, 80, 82, 60 },			// Masquerain
	{ 50, 85, 85, 55, 55, 50 },			// Mawile
	{ 60, 60, 75, 60, 75, 80 },			// Medicham
	{ 30, 40, 55, 40, 55, 60 },			// Meditite
	{ 80, 82, 100, 83, 100, 80 },		// Meganium	
	{ 40, 45, 35, 40, 40, 90 },			// Meowth
	{ 80, 105, 105, 105, 105, 80 },		// Mesprit	
	{ 80, 135, 130, 95, 90, 70 },		// Metagross	
	{ 60, 75, 100, 55, 80, 50 },		// Metang	
	{ 50, 20, 55, 25, 25, 30 },			// Metapod
	{ 100, 100, 100, 100, 100, 100 },	// Mew		
	{ 106, 110, 90, 154, 90, 130 },		// Mewtwo	
	{ 70, 90, 70, 60, 60, 70 },			// Mightyena
	{ 95, 60, 79, 100, 125, 81 },		// Milotic	
	{ 95, 80, 105, 40, 70, 100 },		// Miltank	
	{ 20, 25, 45, 70, 90, 60 },			// Mime Jr.
	{ 60, 40, 50, 75, 85, 95 },			// Minun
	{ 60, 60, 60, 85, 85, 85 },			// Misdreavus
	{ 60, 60, 60, 105, 105, 105 },		// Mismagius	
	{ 90, 100, 90, 125, 85, 90 },		// Moltres	
	{ 64, 78, 52, 78, 52, 81 },			// Monferno
	{ 70, 94, 50, 94, 50, 66 },			// Mothim
	{ 40, 45, 65, 100, 120, 90 },		// Mr. Mime	
	{ 50, 70, 50, 50, 50, 40 },			// Mudkip
	{ 105, 105, 75, 65, 100, 50 },		// Muk	
	{ 135, 85, 40, 40, 85, 5 },			// Munchlax
	{ 60, 85, 42, 85, 42, 91 },			// Murkrow
	{ 40, 50, 45, 70, 45, 70 },			// Natu
	{ 81, 92, 77, 85, 75, 85 },			// Nidoking
	{ 90, 82, 87, 75, 85, 76 },			// Nidoqueen
	{ 55, 47, 52, 40, 40, 41 },			// Nidoran-F
	{ 46, 57, 40, 40, 40, 50 },			// Nidoran-M
	{ 70, 62, 67, 55, 55, 56 },			// Nidorina
	{ 61, 72, 57, 55, 55, 65 },			// Nidorino
	{ 31, 45, 90, 30, 30, 40 },			// Nincada
	{ 73, 76, 75, 81, 100, 100 },		// Ninetales	
	{ 61, 90, 45, 50, 50, 160 },		// Ninjask	
	{ 100, 50, 50, 76, 96, 70 },		// Noctowl	
	{ 30, 45, 135, 45, 90, 30 },		// Nosepass	
	{ 60, 60, 40, 65, 45, 35 },			// Numel
	{ 70, 70, 40, 60, 40, 60 },			// Nuzleaf
	{ 75, 105, 75, 105, 75, 45 },		// Octillery	
	{ 45, 50, 55, 75, 65, 30 },			// Oddish
	{ 35, 40, 100, 90, 55, 35 },		// Omanyte	
	{ 70, 60, 125, 115, 70, 55 },		// Omastar	
	{ 35, 45, 160, 30, 45, 70 },		// Onix	
	{ 60, 45, 70, 45, 90, 95 },			// Pachirisu
	{ 90, 120, 100, 150, 120, 100 },	// Palkia		
	{ 35, 70, 55, 45, 55, 25 },			// Paras
	{ 60, 95, 80, 60, 80, 30 },			// Parasect
	{ 60, 50, 100, 85, 70, 65 },		// Pelipper	
	{ 65, 70, 60, 65, 65, 115 },		// Persian	
	{ 90, 60, 60, 40, 40, 40 },			// Phanpy
	{ 80, 80, 80, 80, 80, 80 },			// Phione
	{ 20, 40, 15, 35, 35, 60 },			// Pichu
	{ 83, 80, 75, 70, 70, 91 },			// Pidgeot
	{ 63, 60, 55, 50, 50, 71 },			// Pidgeotto
	{ 40, 45, 40, 35, 35, 56 },			// Pidgey
	{ 35, 55, 30, 50, 40, 90 },			// Pikachu
	{ 100, 100, 80, 60, 60, 50 },		// Piloswine	
	{ 50, 65, 90, 35, 35, 15 },			// Pineco
	{ 65, 125, 100, 55, 70, 85 },		// Pinsir	
	{ 53, 51, 53, 61, 56, 40 },			// Piplup
	{ 60, 50, 40, 85, 75, 95 },			// Plusle
	{ 90, 75, 75, 90, 100, 70 },		// Politoed	
	{ 40, 50, 40, 40, 40, 90 },			// Poliwag
	{ 65, 65, 65, 50, 50, 90 },			// Poliwhirl
	{ 90, 85, 95, 70, 90, 70 },			// Poliwrath
	{ 50, 85, 55, 65, 65, 90 },			// Ponyta
	{ 35, 55, 35, 30, 30, 35 },			// Poochyena
	{ 65, 60, 70, 85, 75, 40 },			// Porygon
	{ 85, 80, 70, 135, 75, 90 },		// Porygon-Z	
	{ 85, 80, 90, 105, 95, 60 },		// Porygon2	
	{ 65, 105, 60, 60, 70, 95 },		// Primeape	
	{ 64, 66, 68, 81, 76, 50 },			// Prinplup
	{ 60, 55, 145, 75, 150, 40 },		// Probopass	
	{ 50, 52, 48, 65, 50, 55 },			// Psyduck
	{ 70, 84, 70, 65, 70, 51 },			// Pupitar
	{ 71, 82, 64, 64, 59, 112 },		// Purugly	
	{ 95, 85, 85, 65, 65, 35 },			// Quagsire
	{ 58, 64, 58, 80, 65, 80 },			// Quilava
	{ 65, 95, 75, 55, 55, 85 },			// Qwilfish
	{ 60, 90, 55, 90, 80, 100 },		// Raichu	
	{ 90, 85, 75, 115, 100, 115 },		// Raikou	
	{ 28, 25, 25, 45, 35, 40 },			// Ralts
	{ 97, 165, 60, 65, 50, 58 },		// Rampardos	
	{ 65, 100, 70, 80, 80, 105 },		// Rapidash	
	{ 55, 81, 60, 50, 70, 97 },			// Raticate
	{ 30, 56, 35, 25, 35, 72 },			// Rattata
	{ 105, 150, 90, 150, 90, 95 },		// Rayquaza	
	{ 80, 50, 100, 100, 200, 50 },		// Regice	
	{ 110, 160, 110, 80, 110, 100 },	// Regigigas		
	{ 80, 100, 200, 50, 100, 50 },		// Regirock	
	{ 80, 75, 150, 75, 150, 50 },		// Registeel	
	{ 100, 90, 130, 45, 65, 55 },		// Relicanth	
	{ 35, 65, 35, 65, 35, 65 },			// Remoraid
	{ 105, 130, 120, 45, 45, 40 },		// Rhydon	
	{ 80, 85, 95, 30, 30, 25 },			// Rhyhorn
	{ 115, 140, 130, 55, 55, 40 },		// Rhyperior	
	{ 40, 70, 40, 35, 40, 60 },			// Riolu
	{ 50, 60, 45, 100, 80, 65 },		// Roselia	
	{ 60, 70, 55, 125, 105, 90 },		// Roserade	
	{ 50, 50, 77, 95, 77, 91 },			// Rotom
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-C	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-F	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-H	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-S	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-W	
	{ 50, 75, 75, 65, 65, 50 },			// Sableye
	{ 95, 135, 80, 110, 80, 100 },		// Salamence	
	{ 50, 75, 85, 20, 30, 40 },			// Sandshrew
	{ 75, 100, 110, 45, 55, 65 },		// Sandslash	
	{ 70, 85, 65, 105, 85, 120 },		// Sceptile	
	{ 70, 130, 100, 55, 80, 65 },		// Scizor	
	{ 70, 110, 80, 55, 80, 105 },		// Scyther	
	{ 55, 65, 95, 95, 45, 85 },			// Seadra
	{ 80, 92, 65, 65, 80, 68 },			// Seaking
	{ 90, 60, 70, 75, 70, 45 },			// Sealeo
	{ 40, 40, 50, 30, 30, 30 },			// Seedot
	{ 65, 45, 55, 45, 70, 45 },			// Seel
	{ 35, 46, 34, 35, 45, 20 },			// Sentret
	{ 73, 100, 60, 100, 60, 65 },		// Seviper	
	{ 70, 120, 40, 95, 40, 95 },		// Sharpedo	
	{ 100, 100, 100, 100, 100, 100 },	// Shaymin-L		
	{ 100, 103, 75, 120, 75, 127 },		// Shaymin-S	
	{ 1, 90, 45, 30, 30, 40 },			// Shedinja
	{ 65, 95, 100, 60, 50, 50 },		// Shelgon	
	{ 30, 65, 100, 45, 25, 40 },		// Shellder	
	{ 76, 48, 48, 57, 62, 34 },			// Shellos
	{ 30, 42, 118, 42, 88, 30 },		// Shieldon	
	{ 90, 100, 60, 90, 60, 80 },		// Shiftry	
	{ 45, 65, 34, 40, 34, 45 },			// Shinx
	{ 60, 40, 60, 40, 60, 35 },			// Shroomish
	{ 20, 10, 230, 10, 230, 5 },		// Shuckle	
	{ 44, 75, 35, 63, 33, 45 },			// Shuppet
	{ 50, 35, 55, 25, 25, 15 },			// Silcoon
	{ 65, 80, 140, 40, 70, 70 },		// Skarmory	
	{ 55, 45, 50, 45, 65, 80 },			// Skiploom
	{ 50, 45, 45, 35, 35, 50 },			// Skitty
	{ 40, 50, 90, 30, 55, 65 },			// Skorupi
	{ 103, 93, 67, 71, 61, 84 },		// Skuntank	
	{ 150, 160, 100, 95, 65, 100 },		// Slaking	
	{ 60, 60, 60, 35, 35, 30 },			// Slakoth
	{ 95, 75, 110, 100, 80, 30 },		// Slowbro	
	{ 95, 75, 80, 100, 110, 30 },		// Slowking	
	{ 90, 65, 65, 40, 40, 15 },			// Slowpoke
	{ 40, 40, 40, 70, 40, 20 },			// Slugma
	{ 55, 20, 35, 20, 45, 75 },			// Smeargle
	{ 45, 30, 15, 85, 65, 65 },			// Smoochum
	{ 55, 95, 55, 35, 75, 115 },		// Sneasel	
	{ 160, 110, 65, 65, 110, 30 },		// Snorlax	
	{ 50, 50, 50, 50, 50, 50 },			// Snorunt
	{ 60, 62, 50, 62, 60, 40 },			// Snover
	{ 60, 80, 50, 40, 40, 30 },			// Snubbull
	{ 70, 95, 85, 55, 65, 70 },			// Solrock
	{ 40, 60, 30, 31, 31, 70 },			// Spearow
	{ 70, 40, 50, 55, 50, 25 },			// Spheal
	{ 40, 60, 40, 40, 40, 30 },			// Spinarak
	{ 60, 60, 60, 60, 60, 60 },			// Spinda
	{ 50, 92, 108, 92, 108, 35 },		// Spiritomb	
	{ 60, 25, 35, 70, 80, 60 },			// Spoink
	{ 44, 48, 65, 50, 64, 43 },			// Squirtle
	{ 73, 95, 62, 85, 65, 85 },			// Stantler
	{ 85, 120, 70, 50, 50, 100 },		// Staraptor	
	{ 55, 75, 50, 40, 40, 80 },			// Staravia
	{ 40, 55, 30, 30, 30, 60 },			// Starly
	{ 60, 75, 85, 100, 85, 115 },		// Starmie	
	{ 30, 45, 55, 70, 55, 85 },			// Staryu
	{ 75, 85, 200, 55, 65, 30 },		// Steelix	
	{ 63, 63, 47, 41, 41, 74 },			// Stunky
	{ 70, 100, 115, 30, 65, 30 },		// Sudowoodo	
	{ 100, 75, 115, 90, 115, 85 },		// Suicune	
	{ 75, 75, 55, 105, 85, 30 },		// Sunflora	
	{ 30, 30, 30, 30, 30, 30 },			// Sunkern
	{ 40, 30, 32, 50, 52, 65 },			// Surskit
	{ 45, 40, 60, 40, 75, 50 },			// Swablu
	{ 100, 73, 83, 73, 83, 55 },		// Swalot	
	{ 100, 110, 90, 85, 90, 60 },		// Swampert	
	{ 60, 85, 60, 50, 50, 125 },		// Swellow	
	{ 50, 50, 40, 30, 30, 50 },			// Swinub
	{ 40, 55, 30, 30, 30, 85 },			// Taillow
	{ 65, 55, 115, 100, 40, 60 },		// Tangela	
	{ 100, 100, 125, 110, 50, 50 },		// Tangrowth	
	{ 75, 100, 95, 40, 70, 110 },		// Tauros	
	{ 60, 80, 50, 50, 50, 40 },			// Teddiursa
	{ 40, 40, 35, 50, 100, 70 },		// Tentacool	
	{ 80, 70, 65, 80, 120, 100 },		// Tentacruel	
	{ 85, 50, 95, 120, 115, 80 },		// Togekiss	
	{ 35, 20, 65, 40, 65, 20 },			// Togepi
	{ 55, 40, 85, 80, 105, 40 },		// Togetic	
	{ 45, 60, 40, 70, 50, 45 },			// Torchic
	{ 70, 85, 140, 85, 70, 20 },		// Torkoal	
	{ 95, 109, 105, 75, 85, 56 },		// Torterra	
	{ 50, 65, 64, 44, 48, 43 },			// Totodile
	{ 83, 106, 65, 86, 65, 85 },		// Toxicroak	
	{ 45, 100, 45, 45, 45, 10 },		// Trapinch	
	{ 40, 45, 35, 65, 55, 70 },			// Treecko
	{ 99, 68, 83, 72, 87, 51 },			// Tropius
	{ 55, 68, 64, 45, 55, 31 },			// Turtwig
	{ 78, 84, 78, 109, 85, 100 },		// Typhlosion	
	{ 100, 134, 110, 95, 100, 61 },		// Tyranitar	
	{ 35, 35, 35, 35, 35, 35 },			// Tyrogue
	{ 95, 65, 110, 60, 130, 65 },		// Umbreon	
	{ 48, 72, 48, 72, 48, 48 },			// Unown
	{ 90, 130, 75, 75, 75, 55 },		// Ursaring	
	{ 75, 75, 130, 75, 130, 95 },		// Uxie	
	{ 130, 65, 60, 110, 95, 65 },		// Vaporeon	
	{ 70, 65, 60, 90, 75, 90 },			// Venomoth
	{ 60, 55, 50, 40, 55, 45 },			// Venonat
	{ 80, 82, 83, 100, 100, 80 },		// Venusaur	
	{ 70, 80, 102, 80, 102, 40 },		// Vespiquen	
	{ 50, 70, 50, 50, 50, 70 },			// Vibrava
	{ 80, 105, 65, 100, 60, 70 },		// Victreebel	
	{ 80, 80, 80, 55, 55, 90 },			// Vigoroth
	{ 75, 80, 85, 100, 90, 50 },		// Vileplume	
	{ 65, 73, 55, 47, 75, 85 },			// Volbeat
	{ 40, 30, 50, 55, 55, 100 },		// Voltorb	
	{ 38, 41, 40, 50, 65, 65 },			// Vulpix
	{ 130, 70, 35, 70, 35, 60 },		// Wailmer	
	{ 170, 90, 45, 90, 45, 60 },		// Wailord	
	{ 110, 80, 90, 95, 90, 65 },		// Walrein	
	{ 59, 63, 80, 65, 80, 58 },			// Wartortle
	{ 70, 120, 65, 45, 85, 125 },		// Weavile	
	{ 40, 35, 30, 20, 20, 50 },			// Weedle
	{ 65, 90, 50, 85, 45, 55 },			// Weepinbell
	{ 65, 90, 120, 85, 70, 60 },		// Weezing	
	{ 110, 78, 73, 76, 71, 60 },		// Whiscash	
	{ 64, 51, 23, 51, 23, 28 },			// Whismur
	{ 140, 70, 45, 75, 50, 45 },		// Wigglytuff	
	{ 40, 30, 30, 55, 30, 85 },			// Wingull
	{ 190, 33, 58, 33, 58, 33 },		// Wobbuffet	
	{ 55, 45, 45, 25, 25, 15 },			// Wooper
	{ 60, 59, 85, 79, 105, 36 },		// Wormadam-P	
	{ 60, 79, 105, 59, 85, 36 },		// Wormadam-S	
	{ 60, 69, 95, 69, 95, 36 },			// Wormadam-T
	{ 45, 45, 35, 20, 30, 20 },			// Wurmple
	{ 95, 23, 48, 23, 48, 23 },			// Wynaut
	{ 65, 75, 70, 95, 70, 95 },			// Xatu
	{ 65, 65, 45, 75, 45, 95 },			// Yanma
	{ 86, 76, 86, 116, 56, 95 },		// Yanmega	
	{ 73, 115, 60, 60, 60, 90 },		// Zangoose	
	{ 90, 90, 85, 125, 90, 100 },		// Zapdos	
	{ 38, 30, 41, 30, 41, 60 },			// Zigzagoon
	{ 40, 45, 35, 30, 40, 55	 }			// Zubat
};

const types get_pokemon_type [][2] = {
{ GRASS, ICE },				// Abomasnow
{ PSYCHIC_TYPE, TYPELESS },		// Abra
{ DARK, TYPELESS },			// Absol
{ ROCK, FLYING },			// Aerodactyl
{ STEEL, ROCK },			// Aggron
{ NORMAL, TYPELESS },		// Aipom
{ PSYCHIC_TYPE, TYPELESS },		// Alakazam
{ DRAGON, FLYING },			// Altaria
{ NORMAL, TYPELESS },		// Ambipom
{ ELECTRIC, TYPELESS },		// Ampharos
{ ROCK, BUG },				// Anorith
{ POISON, TYPELESS },		// Arbok
{ FIRE, TYPELESS },			// Arcanine
{ NORMAL, TYPELESS },		// Arceus
{ BUG, POISON },			// Ariados
{ ROCK, BUG },				// Armaldo
{ STEEL, ROCK },			// Aron
{ ICE, FLYING },			// Articuno
{ PSYCHIC_TYPE, TYPELESS },		// Azelf
{ WATER, TYPELESS },		// Azumarill
{ NORMAL, TYPELESS },		// Azurill
{ DRAGON, TYPELESS },		// Bagon
{ GROUND, PSYCHIC_TYPE },		// Baltoy
{ GHOST, TYPELESS },		// Banette
{ WATER, GROUND },			// Barboach
{ ROCK, STEEL },			// Bastiodon
{ GRASS, TYPELESS },		// Bayleef
{ BUG, FLYING },			// Beautifly
{ BUG, POISON },			// Beedrill
{ STEEL, PSYCHIC_TYPE },			// Beldum
{ GRASS, TYPELESS },		// Bellossom
{ GRASS, POISON },			// Bellsprout
{ NORMAL, WATER },			// Bibarel
{ NORMAL, TYPELESS },		// Bidoof
{ WATER, TYPELESS },		// Blastoise
{ FIRE, FIGHTING },			// Blaziken
{ NORMAL, TYPELESS },		// Blissey
{ ROCK, TYPELESS },			// Bonsly
{ GRASS, FIGHTING },		// Breloom
{ STEEL, PSYCHIC_TYPE },			// Bronzong
{ STEEL, PSYCHIC_TYPE },			// Bronzor
{ GRASS, POISON },			// Budew
{ WATER, TYPELESS },		// Buizel
{ GRASS, POISON },			// Bulbasaur
{ NORMAL, TYPELESS },		// Buneary
{ BUG, TYPELESS },			// Burmy
{ BUG, FLYING },			// Butterfree
{ GRASS, TYPELESS },		// Cacnea
{ GRASS, DARK },			// Cacturne
{ FIRE, GROUND },			// Camerupt
{ GRASS, TYPELESS },		// Carnivine
{ WATER, DARK },			// Carvanha
{ BUG, TYPELESS },			// Cascoon
{ NORMAL, TYPELESS },		// Castform
{ BUG, TYPELESS },			// Caterpie
{ PSYCHIC_TYPE, GRASS },			// Celebi
{ NORMAL, TYPELESS },		// Chansey
{ FIRE, FLYING },			// Charizard
{ FIRE, TYPELESS },			// Charmander
{ FIRE, TYPELESS },			// Charmeleon
{ NORMAL, FLYING },			// Chatot
{ GRASS, TYPELESS },		// Cherrim
{ GRASS, TYPELESS },		// Cherubi
{ GRASS, TYPELESS },		// Chikorita
{ FIRE, TYPELESS },			// Chimchar
{ PSYCHIC_TYPE, TYPELESS },		// Chimecho
{ WATER, ELECTRIC },		// Chinchou
{ PSYCHIC_TYPE, TYPELESS },		// Chingling
{ WATER, TYPELESS },		// Clamperl
{ GROUND, PSYCHIC_TYPE },		// Claydol
{ NORMAL, TYPELESS },		// Clefable
{ NORMAL, TYPELESS },		// Clefairy
{ NORMAL, TYPELESS },		// Cleffa
{ WATER, ICE },				// Cloyster
{ BUG, FLYING },			// Combee
{ FIRE, FIGHTING },			// Combusken
{ WATER, TYPELESS },		// Corphish
{ WATER, ROCK },			// Corsola
{ ROCK, GRASS },			// Cradily
{ ROCK, TYPELESS },			// Cranidos
{ WATER, DARK },			// Crawdaunt
{ PSYCHIC_TYPE, TYPELESS },		// Cresselia
{ POISON, FIGHTING },		// Croagunk
{ POISON, FLYING },			// Crobat
{ WATER, TYPELESS },		// Croconaw
{ GROUND, TYPELESS },		// Cubone
{ FIRE, TYPELESS },			// Cyndaquil
{ DARK, TYPELESS },			// Darkrai
{ NORMAL, TYPELESS },		// Delcatty
{ ICE, FLYING },			// Delibird
{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-A
{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-D
{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-M
{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-S
{ WATER, ICE },				// Dewgong
{ STEEL, DRAGON },			// Dialga
{ GROUND, TYPELESS },		// Diglett
{ NORMAL, TYPELESS },		// Ditto
{ NORMAL, FLYING },			// Dodrio
{ NORMAL, FLYING },			// Doduo
{ GROUND, TYPELESS },		// Donphan
{ DRAGON, TYPELESS },		// Dragonair
{ DRAGON, FLYING },			// Dragonite
{ POISON, DARK },			// Drapion
{ DRAGON, TYPELESS },		// Dratini
{ GHOST, FLYING },			// Drifblim
{ GHOST, FLYING },			// Drifloon
{ PSYCHIC_TYPE, TYPELESS },		// Drowzee
{ GROUND, TYPELESS },		// Dugtrio
{ NORMAL, TYPELESS },		// Dunsparce
{ GHOST, TYPELESS },		// Dusclops
{ GHOST, TYPELESS },		// Dusknoir
{ GHOST, TYPELESS },		// Duskull
{ BUG, POISON },			// Dustox
{ NORMAL, TYPELESS },		// Eevee
{ POISON, TYPELESS },		// Ekans
{ ELECTRIC, TYPELESS },		// Electabuzz
{ ELECTRIC, TYPELESS },		// Electivire
{ ELECTRIC, TYPELESS },		// Electrike
{ ELECTRIC, TYPELESS },		// Electrode
{ ELECTRIC, TYPELESS },		// Elekid
{ WATER, STEEL },			// Empoleon
{ FIRE, TYPELESS },			// Entei
{ PSYCHIC_TYPE, TYPELESS },		// Espeon
{ GRASS, PSYCHIC_TYPE },			// Exeggcute
{ GRASS, PSYCHIC_TYPE },			// Exeggutor
{ NORMAL, TYPELESS },		// Exploud
{ NORMAL, FLYING },			// Farfetch'd
{ NORMAL, FLYING },			// Fearow
{ WATER, TYPELESS },		// Feebas
{ WATER, TYPELESS },		// Feraligatr
{ WATER, TYPELESS },		// Finneon
{ ELECTRIC, TYPELESS },		// Flaaffy
{ FIRE, TYPELESS },			// Flareon
{ WATER, TYPELESS },		// Floatzel
{ GROUND, DRAGON },			// Flygon
{ BUG, STEEL },				// Forretress
{ ICE, GHOST },				// Froslass
{ NORMAL, TYPELESS },		// Furret
{ DRAGON, GROUND },			// Gabite
{ PSYCHIC_TYPE, FIGHTING },		// Gallade
{ DRAGON, GROUND },			// Garchomp
{ PSYCHIC_TYPE, TYPELESS },		// Gardevoir
{ GHOST, POISON },			// Gastly
{ WATER, GROUND },			// Gastrodon
{ GHOST, POISON },			// Gengar
{ ROCK, GROUND },			// Geodude
{ DRAGON, GROUND },			// Gible
{ NORMAL, PSYCHIC_TYPE },		// Girafarig
{ GHOST, DRAGON },			// Giratina-A
{ GHOST, DRAGON },			// Giratina-O
{ ICE, TYPELESS },			// Glaceon
{ ICE, TYPELESS },			// Glalie
{ NORMAL, TYPELESS },		// Glameow
{ GROUND, FLYING },			// Gligar
{ GROUND, FLYING },			// Gliscor
{ GRASS, POISON },			// Gloom
{ POISON, FLYING },			// Golbat
{ WATER, TYPELESS },		// Goldeen
{ WATER, TYPELESS },		// Golduck
{ ROCK, GROUND },			// Golem
{ WATER, TYPELESS },		// Gorebyss
{ NORMAL, TYPELESS },		// Granbull
{ ROCK, GROUND },			// Graveler
{ POISON, TYPELESS },		// Grimer
{ GRASS, TYPELESS },		// Grotle
{ GROUND, TYPELESS },		// Groudon
{ GRASS, TYPELESS },		// Grovyle
{ FIRE, TYPELESS },			// Growlithe
{ PSYCHIC_TYPE, TYPELESS },		// Grumpig
{ POISON, TYPELESS },		// Gulpin
{ WATER, FLYING },			// Gyarados
{ NORMAL, TYPELESS },		// Happiny
{ FIGHTING, TYPELESS },		// Hariyama
{ GHOST, POISON },			// Haunter
{ FIRE, STEEL },			// Heatran
{ BUG, FIGHTING },			// Heracross
{ GROUND, TYPELESS },		// Hippopotas
{ GROUND, TYPELESS },		// Hippowdon
{ FIGHTING, TYPELESS },		// Hitmonchan
{ FIGHTING, TYPELESS },		// Hitmonlee
{ FIGHTING, TYPELESS },		// Hitmontop
{ FIRE, FLYING },			// Ho-Oh
{ DARK, FLYING },			// Honchkrow
{ NORMAL, FLYING },			// Hoothoot
{ GRASS, FLYING },			// Hoppip
{ WATER, TYPELESS },		// Horsea
{ DARK, FIRE },				// Houndoom
{ DARK, FIRE },				// Houndour
{ WATER, TYPELESS },		// Huntail
{ PSYCHIC_TYPE, TYPELESS },		// Hypno
{ NORMAL, TYPELESS },		// Igglybuff
{ BUG, TYPELESS },			// Illumise
{ FIRE, FIGHTING },			// Infernape
{ GRASS, POISON },			// Ivysaur
{ NORMAL, TYPELESS },		// Jigglypuff
{ STEEL, PSYCHIC_TYPE },			// Jirachi
{ ELECTRIC, TYPELESS },		// Jolteon
{ GRASS, FLYING },			// Jumpluff
{ ICE, PSYCHIC_TYPE },			// Jynx
{ ROCK, WATER },			// Kabuto
{ ROCK, WATER },			// Kabutops
{ PSYCHIC_TYPE, TYPELESS },		// Kadabra
{ BUG, POISON },			// Kakuna
{ NORMAL, TYPELESS },		// Kangaskhan
{ NORMAL, TYPELESS },		// Kecleon
{ WATER, DRAGON },			// Kingdra
{ WATER, TYPELESS },		// Kingler
{ PSYCHIC_TYPE, TYPELESS },		// Kirlia
{ POISON, TYPELESS },		// Koffing
{ WATER, TYPELESS },		// Krabby
{ BUG, TYPELESS },			// Kricketot
{ BUG, TYPELESS },			// Kricketune
{ WATER, TYPELESS },		// Kyogre
{ STEEL, ROCK },			// Lairon
{ WATER, ELECTRIC },		// Lanturn
{ WATER, ICE },				// Lapras
{ ROCK, GROUND },			// Larvitar
{ DRAGON, PSYCHIC_TYPE },		// Latias
{ DRAGON, PSYCHIC_TYPE },		// Latios
{ GRASS, TYPELESS },		// Leafeon
{ BUG, FLYING },			// Ledian
{ BUG, FLYING },			// Ledyba
{ NORMAL, TYPELESS },		// Lickilicky
{ NORMAL, TYPELESS },		// Lickitung
{ ROCK, GRASS },			// Lileep
{ NORMAL, TYPELESS },		// Linoone
{ WATER, GRASS },			// Lombre
{ NORMAL, TYPELESS },		// Lopunny
{ WATER, GRASS },			// Lotad
{ NORMAL, TYPELESS },		// Loudred
{ FIGHTING, STEEL },		// Lucario
{ WATER, GRASS },			// Ludicolo
{ PSYCHIC_TYPE, FLYING },		// Lugia
{ WATER, TYPELESS },		// Lumineon
{ ROCK, PSYCHIC_TYPE },			// Lunatone
{ WATER, TYPELESS },		// Luvdisc
{ ELECTRIC, TYPELESS },		// Luxio
{ ELECTRIC, TYPELESS },		// Luxray
{ FIGHTING, TYPELESS },		// Machamp
{ FIGHTING, TYPELESS },		// Machoke
{ FIGHTING, TYPELESS },		// Machop
{ FIRE, TYPELESS },			// Magby
{ FIRE, ROCK },				// Magcargo
{ WATER, TYPELESS },		// Magikarp
{ FIRE, TYPELESS },			// Magmar
{ FIRE, TYPELESS },			// Magmortar
{ ELECTRIC, STEEL },		// Magnemite
{ ELECTRIC, STEEL },		// Magneton
{ ELECTRIC, STEEL },		// Magnezone
{ FIGHTING, TYPELESS },		// Makuhita
{ ICE, GROUND },			// Mamoswine
{ WATER, TYPELESS },		// Manaphy
{ ELECTRIC, TYPELESS },		// Manectric
{ FIGHTING, TYPELESS },		// Mankey
{ WATER, FLYING },			// Mantine
{ WATER, FLYING },			// Mantyke
{ ELECTRIC, TYPELESS },		// Mareep
{ WATER, TYPELESS },		// Marill
{ GROUND, TYPELESS },		// Marowak
{ WATER, GROUND },			// Marshtomp
{ BUG, FLYING },			// Masquerain
{ STEEL, TYPELESS },		// Mawile
{ FIGHTING, PSYCHIC_TYPE },		// Medicham
{ FIGHTING, PSYCHIC_TYPE },		// Meditite
{ GRASS, TYPELESS },		// Meganium
{ NORMAL, TYPELESS },		// Meowth
{ PSYCHIC_TYPE, TYPELESS },		// Mesprit
{ STEEL, PSYCHIC_TYPE },			// Metagross
{ STEEL, PSYCHIC_TYPE },			// Metang
{ BUG, TYPELESS },			// Metapod
{ PSYCHIC_TYPE, TYPELESS },		// Mew
{ PSYCHIC_TYPE, TYPELESS },		// Mewtwo
{ DARK, TYPELESS },			// Mightyena
{ WATER, TYPELESS },		// Milotic
{ NORMAL, TYPELESS },		// Miltank
{ PSYCHIC_TYPE, TYPELESS },		// Mime Jr.
{ ELECTRIC, TYPELESS },		// Minun
{ GHOST, TYPELESS },		// Misdreavus
{ GHOST, TYPELESS },		// Mismagius
{ FIRE, FLYING },			// Moltres
{ FIRE, FIGHTING },			// Monferno
{ BUG, FLYING },			// Mothim
{ PSYCHIC_TYPE, TYPELESS },		// Mr. Mime
{ WATER, TYPELESS },		// Mudkip
{ POISON, TYPELESS },		// Muk
{ NORMAL, TYPELESS },		// Munchlax
{ DARK, FLYING },			// Murkrow
{ PSYCHIC_TYPE, FLYING },		// Natu
{ POISON, GROUND },			// Nidoking
{ POISON, GROUND },			// Nidoqueen
{ POISON, TYPELESS },		// Nidoran-F
{ POISON, TYPELESS },		// Nidoran-M
{ POISON, TYPELESS },		// Nidorina
{ POISON, TYPELESS },		// Nidorino
{ BUG, GROUND },			// Nincada
{ FIRE, TYPELESS },			// Ninetales
{ BUG, FLYING },			// Ninjask
{ NORMAL, FLYING },			// Noctowl
{ ROCK, TYPELESS },			// Nosepass
{ FIRE, GROUND },			// Numel
{ GRASS, DARK },			// Nuzleaf
{ WATER, TYPELESS },		// Octillery
{ GRASS, POISON },			// Oddish
{ ROCK, WATER },			// Omanyte
{ ROCK, WATER },			// Omastar
{ ROCK, GROUND },			// Onix
{ ELECTRIC, TYPELESS },		// Pachirisu
{ WATER, DRAGON },			// Palkia
{ BUG, GRASS },				// Paras
{ BUG, GRASS },				// Parasect
{ WATER, FLYING },			// Pelipper
{ NORMAL, TYPELESS },		// Persian
{ GROUND, TYPELESS },		// Phanpy
{ WATER, TYPELESS },		// Phione
{ ELECTRIC, TYPELESS },		// Pichu
{ NORMAL, FLYING },			// Pidgeot
{ NORMAL, FLYING },			// Pidgeotto
{ NORMAL, FLYING },			// Pidgey
{ ELECTRIC, TYPELESS },		// Pikachu
{ ICE, GROUND },			// Piloswine
{ BUG, TYPELESS },			// Pineco
{ BUG, TYPELESS },			// Pinsir
{ WATER, TYPELESS },		// Piplup
{ ELECTRIC, TYPELESS },		// Plusle
{ WATER, TYPELESS },		// Politoed
{ WATER, TYPELESS },		// Poliwag
{ WATER, TYPELESS },		// Poliwhirl
{ WATER, FIGHTING },		// Poliwrath
{ FIRE, TYPELESS },			// Ponyta
{ DARK, TYPELESS },			// Poochyena
{ NORMAL, TYPELESS },		// Porygon
{ NORMAL, TYPELESS },		// Porygon-Z
{ NORMAL, TYPELESS },		// Porygon2
{ FIGHTING, TYPELESS },		// Primeape
{ WATER, TYPELESS },		// Prinplup
{ ROCK, STEEL },			// Probopass
{ WATER, TYPELESS },		// Psyduck
{ ROCK, GROUND },			// Pupitar
{ NORMAL, TYPELESS },		// Purugly
{ WATER, GROUND },			// Quagsire
{ FIRE, TYPELESS },			// Quilava
{ WATER, POISON },			// Qwilfish
{ ELECTRIC, TYPELESS },		// Raichu
{ ELECTRIC, TYPELESS },		// Raikou
{ PSYCHIC_TYPE, TYPELESS },		// Ralts
{ ROCK, TYPELESS },			// Rampardos
{ FIRE, TYPELESS },			// Rapidash
{ NORMAL, TYPELESS },		// Raticate
{ NORMAL, TYPELESS },		// Rattata
{ DRAGON, FLYING },			// Rayquaza
{ ICE, TYPELESS },			// Regice
{ NORMAL, TYPELESS },		// Regigigas
{ ROCK, TYPELESS },			// Regirock
{ STEEL, TYPELESS },		// Registeel
{ WATER, ROCK },			// Relicanth
{ WATER, TYPELESS },		// Remoraid
{ GROUND, ROCK },			// Rhydon
{ GROUND, ROCK },			// Rhyhorn
{ GROUND, ROCK },			// Rhyperior
{ FIGHTING, TYPELESS },		// Riolu
{ GRASS, POISON },			// Roselia
{ GRASS, POISON },			// Roserade
{ ELECTRIC, GHOST },		// Rotom
{ ELECTRIC, GHOST },		// Rotom-C
{ ELECTRIC, GHOST },		// Rotom-F
{ ELECTRIC, GHOST },		// Rotom-H
{ ELECTRIC, GHOST },		// Rotom-S
{ ELECTRIC, GHOST },		// Rotom-W
{ DARK, GHOST },			// Sableye
{ DRAGON, FLYING },			// Salamence
{ GROUND, TYPELESS },		// Sandshrew
{ GROUND, TYPELESS },		// Sandslash
{ GRASS, TYPELESS },		// Sceptile
{ BUG, STEEL },				// Scizor
{ BUG, FLYING },			// Scyther
{ WATER, TYPELESS },		// Seadra
{ WATER, TYPELESS },		// Seaking
{ ICE, WATER },				// Sealeo
{ GRASS, TYPELESS },		// Seedot
{ WATER, TYPELESS },		// Seel
{ NORMAL, TYPELESS },		// Sentret
{ POISON, TYPELESS },		// Seviper
{ WATER, DARK },			// Sharpedo
{ GRASS, TYPELESS },		// Shaymin-L
{ GRASS, FLYING },			// Shaymin-S
{ BUG, GHOST },				// Shedinja
{ DRAGON, TYPELESS },		// Shelgon
{ WATER, TYPELESS },		// Shellder
{ WATER, TYPELESS },		// Shellos
{ ROCK, STEEL },			// Shieldon
{ GRASS, DARK },			// Shiftry
{ ELECTRIC, TYPELESS },		// Shinx
{ GRASS, TYPELESS },		// Shroomish
{ BUG, ROCK },				// Shuckle
{ GHOST, TYPELESS },		// Shuppet
{ BUG, TYPELESS },			// Silcoon
{ STEEL, FLYING },			// Skarmory
{ GRASS, FLYING },			// Skiploom
{ NORMAL, TYPELESS },		// Skitty
{ POISON, BUG },			// Skorupi
{ POISON, DARK },			// Skuntank
{ NORMAL, TYPELESS },		// Slaking
{ NORMAL, TYPELESS },		// Slakoth
{ WATER, PSYCHIC_TYPE },			// Slowbro
{ WATER, PSYCHIC_TYPE },			// Slowking
{ WATER, PSYCHIC_TYPE },			// Slowpoke
{ FIRE, TYPELESS },			// Slugma
{ NORMAL, TYPELESS },		// Smeargle
{ ICE, PSYCHIC_TYPE },			// Smoochum
{ DARK, ICE },				// Sneasel
{ NORMAL, TYPELESS },		// Snorlax
{ ICE, TYPELESS },			// Snorunt
{ GRASS, ICE },				// Snover
{ NORMAL, TYPELESS },		// Snubbull
{ ROCK, PSYCHIC_TYPE },			// Solrock
{ NORMAL, FLYING },			// Spearow
{ ICE, WATER },				// Spheal
{ BUG, POISON },			// Spinarak
{ NORMAL, TYPELESS },		// Spinda
{ GHOST, DARK },			// Spiritomb
{ PSYCHIC_TYPE, TYPELESS },		// Spoink
{ WATER, TYPELESS },		// Squirtle
{ NORMAL, TYPELESS },		// Stantler
{ NORMAL, FLYING },			// Staraptor
{ NORMAL, FLYING },			// Staravia
{ NORMAL, FLYING },			// Starly
{ WATER, PSYCHIC_TYPE },			// Starmie
{ WATER, TYPELESS },		// Staryu
{ STEEL, GROUND },			// Steelix
{ POISON, DARK },			// Stunky
{ ROCK, TYPELESS },			// Sudowoodo
{ WATER, TYPELESS },		// Suicune
{ GRASS, TYPELESS },		// Sunflora
{ GRASS, TYPELESS },		// Sunkern
{ BUG, WATER },				// Surskit
{ NORMAL, FLYING },			// Swablu
{ POISON, TYPELESS },		// Swalot
{ WATER, GROUND },			// Swampert
{ NORMAL, FLYING },			// Swellow
{ ICE, GROUND },			// Swinub
{ NORMAL, FLYING },			// Taillow
{ GRASS, TYPELESS },		// Tangela
{ GRASS, TYPELESS },		// Tangrowth
{ NORMAL, TYPELESS },		// Tauros
{ NORMAL, TYPELESS },		// Teddiursa
{ WATER, POISON },			// Tentacool
{ WATER, POISON },			// Tentacruel
{ NORMAL, FLYING },			// Togekiss
{ NORMAL, TYPELESS },		// Togepi
{ NORMAL, FLYING },			// Togetic
{ FIRE, TYPELESS },			// Torchic
{ FIRE, TYPELESS },			// Torkoal
{ GRASS, GROUND },			// Torterra
{ WATER, TYPELESS },		// Totodile
{ POISON, FIGHTING },		// Toxicroak
{ GROUND, TYPELESS },		// Trapinch
{ GRASS, TYPELESS },		// Treecko
{ GRASS, FLYING },			// Tropius
{ GRASS, TYPELESS },		// Turtwig
{ FIRE, TYPELESS },			// Typhlosion
{ ROCK, DARK },				// Tyranitar
{ FIGHTING, TYPELESS },		// Tyrogue
{ DARK, TYPELESS },			// Umbreon
{ PSYCHIC_TYPE, TYPELESS },		// Unown
{ NORMAL, TYPELESS },		// Ursaring
{ PSYCHIC_TYPE, TYPELESS },		// Uxie
{ WATER, TYPELESS },		// Vaporeon
{ BUG, POISON },			// Venomoth
{ BUG, POISON },			// Venonat
{ GRASS, POISON },			// Venusaur
{ BUG, FLYING },			// Vespiquen
{ GROUND, DRAGON },			// Vibrava
{ GRASS, POISON },			// Victreebel
{ NORMAL, TYPELESS },		// Vigoroth
{ GRASS, POISON },			// Vileplume
{ BUG, TYPELESS },			// Volbeat
{ ELECTRIC, TYPELESS },		// Voltorb
{ FIRE, TYPELESS },			// Vulpix
{ WATER, TYPELESS },		// Wailmer
{ WATER, TYPELESS },		// Wailord
{ ICE, WATER },				// Walrein
{ WATER, TYPELESS },		// Wartortle
{ DARK, ICE },				// Weavile
{ BUG, POISON },			// Weedle
{ GRASS, POISON },			// Weepinbell
{ POISON, TYPELESS },		// Weezing
{ WATER, GROUND },			// Whiscash
{ NORMAL, TYPELESS },		// Whismur
{ NORMAL, TYPELESS },		// Wigglytuff
{ WATER, FLYING },			// Wingull
{ PSYCHIC_TYPE, TYPELESS },		// Wobbuffet
{ WATER, GROUND },			// Wooper
{ BUG, GRASS },				// Wormadam-P
{ BUG, GROUND },			// Wormadam-S
{ BUG, STEEL },				// Wormadam-T
{ BUG, TYPELESS },			// Wurmple
{ PSYCHIC_TYPE, TYPELESS },		// Wynaut
{ PSYCHIC_TYPE, FLYING },		// Xatu
{ BUG, FLYING },			// Yanma
{ BUG, FLYING },			// Yanmega
{ NORMAL, TYPELESS },		// Zangoose
{ ELECTRIC, FLYING },		// Zapdos
{ NORMAL, TYPELESS },		// Zigzagoon
{ POISON, FLYING }			// Zubat
};

void set_species_map (std::map <std::string, species> &species_map) {
	species_map["Abomasnow"] = ABOMASNOW;
	species_map["Abra"] = ABRA;
	species_map["Absol"] = ABSOL;
	species_map["Aerodactyl"] = AERODACTYL;
	species_map["Aggron"] = AGGRON;
	species_map["Aipom"] = AIPOM;
	species_map["Alakazam"] = ALAKAZAM;
	species_map["Altaria"] = ALTARIA;
	species_map["Ambipom"] = AMBIPOM;
	species_map["Ampharos"] = AMPHAROS;
	species_map["Anorith"] = ANORITH;
	species_map["Arbok"] = ARBOK;
	species_map["Arcanine"] = ARCANINE;
	species_map["Arceus"] = ARCEUS;
	species_map["Ariados"] = ARIADOS;
	species_map["Armaldo"] = ARMALDO;
	species_map["Aron"] = ARON;
	species_map["Articuno"] = ARTICUNO;
	species_map["Azelf"] = AZELF;
	species_map["Azumarill"] = AZUMARILL;
	species_map["Azurill"] = AZURILL;
	species_map["Bagon"] = BAGON;
	species_map["Baltoy"] = BALTOY;
	species_map["Banette"] = BANETTE;
	species_map["Barboach"] = BARBOACH;
	species_map["Bastiodon"] = BASTIODON;
	species_map["Bayleef"] = BAYLEEF;
	species_map["Beautifly"] = BEAUTIFLY;
	species_map["Beedrill"] = BEEDRILL;
	species_map["Beldum"] = BELDUM;
	species_map["Bellossom"] = BELLOSSOM;
	species_map["Bellsprout"] = BELLSPROUT;
	species_map["Bibarel"] = BIBAREL;
	species_map["Bidoof"] = BIDOOF;
	species_map["Blastoise"] = BLASTOISE;
	species_map["Blaziken"] = BLAZIKEN;
	species_map["Blissey"] = BLISSEY;
	species_map["Bonsly"] = BONSLY;
	species_map["Breloom"] = BRELOOM;
	species_map["Bronzong"] = BRONZONG;
	species_map["Bronzor"] = BRONZOR;
	species_map["Budew"] = BUDEW;
	species_map["Buizel"] = BUIZEL;
	species_map["Bulbasaur"] = BULBASAUR;
	species_map["Buneary"] = BUNEARY;
	species_map["Burmy"] = BURMY;
	species_map["Butterfree"] = BUTTERFREE;
	species_map["Cacnea"] = CACNEA;
	species_map["Cacturne"] = CACTURNE;
	species_map["Camerupt"] = CAMERUPT;
	species_map["Carnivine"] = CARNIVINE;
	species_map["Carvanha"] = CARVANHA;
	species_map["Cascoon"] = CASCOON;
	species_map["Castform"] = CASTFORM;
	species_map["Caterpie"] = CATERPIE;
	species_map["Celebi"] = CELEBI;
	species_map["Chansey"] = CHANSEY;
	species_map["Charizard"] = CHARIZARD;
	species_map["Charmander"] = CHARMANDER;
	species_map["Charmeleon"] = CHARMELEON;
	species_map["Chatot"] = CHATOT;
	species_map["Cherrim"] = CHERRIM;
	species_map["Cherubi"] = CHERUBI;
	species_map["Chikorita"] = CHIKORITA;
	species_map["Chimchar"] = CHIMCHAR;
	species_map["Chimecho"] = CHIMECHO;
	species_map["Chinchou"] = CHINCHOU;
	species_map["Chingling"] = CHINGLING;
	species_map["Clamperl"] = CLAMPERL;
	species_map["Claydol"] = CLAYDOL;
	species_map["Clefable"] = CLEFABLE;
	species_map["Clefairy"] = CLEFAIRY;
	species_map["Cleffa"] = CLEFFA;
	species_map["Cloyster"] = CLOYSTER;
	species_map["Combee"] = COMBEE;
	species_map["Combusken"] = COMBUSKEN;
	species_map["Corphish"] = CORPHISH;
	species_map["Corsola"] = CORSOLA;
	species_map["Cradily"] = CRADILY;
	species_map["Cranidos"] = CRANIDOS;
	species_map["Crawdaunt"] = CRAWDAUNT;
	species_map["Cresselia"] = CRESSELIA;
	species_map["Croagunk"] = CROAGUNK;
	species_map["Crobat"] = CROBAT;
	species_map["Croconaw"] = CROCONAW;
	species_map["Cubone"] = CUBONE;
	species_map["Cyndaquil"] = CYNDAQUIL;
	species_map["Darkrai"] = DARKRAI;
	species_map["Delcatty"] = DELCATTY;
	species_map["Delibird"] = DELIBIRD;
	species_map["Deoxys-a"] = DEOXYS_A;
	species_map["Deoxys-d"] = DEOXYS_D;
	species_map["Deoxys"] = DEOXYS_M;
	species_map["Deoxys-s"] = DEOXYS_S;
	species_map["Dewgong"] = DEWGONG;
	species_map["Dialga"] = DIALGA;
	species_map["Diglett"] = DIGLETT;
	species_map["Ditto"] = DITTO;
	species_map["Dodrio"] = DODRIO;
	species_map["Doduo"] = DODUO;
	species_map["Donphan"] = DONPHAN;
	species_map["Dragonair"] = DRAGONAIR;
	species_map["Dragonite"] = DRAGONITE;
	species_map["Drapion"] = DRAPION;
	species_map["Dratini"] = DRATINI;
	species_map["Drifblim"] = DRIFBLIM;
	species_map["Drifloon"] = DRIFLOON;
	species_map["Drowzee"] = DROWZEE;
	species_map["Dugtrio"] = DUGTRIO;
	species_map["Dunsparce"] = DUNSPARCE;
	species_map["Dusclops"] = DUSCLOPS;
	species_map["Dusknoir"] = DUSKNOIR;
	species_map["Duskull"] = DUSKULL;
	species_map["Dustox"] = DUSTOX;
	species_map["Eevee"] = EEVEE;
	species_map["Ekans"] = EKANS;
	species_map["Electabuzz"] = ELECTABUZZ;
	species_map["Electivire"] = ELECTIVIRE;
	species_map["Electrike"] = ELECTRIKE;
	species_map["Electrode"] = ELECTRODE;
	species_map["Elekid"] = ELEKID;
	species_map["Empoleon"] = EMPOLEON;
	species_map["Entei"] = ENTEI;
	species_map["Espeon"] = ESPEON;
	species_map["Exeggcute"] = EXEGGCUTE;
	species_map["Exeggutor"] = EXEGGUTOR;
	species_map["Exploud"] = EXPLOUD;
	species_map["Farfetch'd"] = FARFETCHD;
	species_map["Fearow"] = FEAROW;
	species_map["Feebas"] = FEEBAS;
	species_map["Feraligatr"] = FERALIGATR;
	species_map["Finneon"] = FINNEON;
	species_map["Flaaffy"] = FLAAFFY;
	species_map["Flareon"] = FLAREON;
	species_map["Floatzel"] = FLOATZEL;
	species_map["Flygon"] = FLYGON;
	species_map["Forretress"] = FORRETRESS;
	species_map["Froslass"] = FROSLASS;
	species_map["Furret"] = FURRET;
	species_map["Gabite"] = GABITE;
	species_map["Gallade"] = GALLADE;
	species_map["Garchomp"] = GARCHOMP;
	species_map["Gardevoir"] = GARDEVOIR;
	species_map["Gastly"] = GASTLY;
	species_map["Gastrodon"] = GASTRODON;
	species_map["Gengar"] = GENGAR;
	species_map["Geodude"] = GEODUDE;
	species_map["Gible"] = GIBLE;
	species_map["Girafarig"] = GIRAFARIG;
	species_map["Giratina"] = GIRATINA_A;
	species_map["Giratina-o"] = GIRATINA_O;
	species_map["Glaceon"] = GLACEON;
	species_map["Glalie"] = GLALIE;
	species_map["Glameow"] = GLAMEOW;
	species_map["Gligar"] = GLIGAR;
	species_map["Gliscor"] = GLISCOR;
	species_map["Gloom"] = GLOOM;
	species_map["Golbat"] = GOLBAT;
	species_map["Goldeen"] = GOLDEEN;
	species_map["Golduck"] = GOLDUCK;
	species_map["Golem"] = GOLEM;
	species_map["Gorebyss"] = GOREBYSS;
	species_map["Granbull"] = GRANBULL;
	species_map["Graveler"] = GRAVELER;
	species_map["Grimer"] = GRIMER;
	species_map["Grotle"] = GROTLE;
	species_map["Groudon"] = GROUDON;
	species_map["Grovyle"] = GROVYLE;
	species_map["Growlithe"] = GROWLITHE;
	species_map["Grumpig"] = GRUMPIG;
	species_map["Gulpin"] = GULPIN;
	species_map["Gyarados"] = GYARADOS;
	species_map["Happiny"] = HAPPINY;
	species_map["Hariyama"] = HARIYAMA;
	species_map["Haunter"] = HAUNTER;
	species_map["Heatran"] = HEATRAN;
	species_map["Heracross"] = HERACROSS;
	species_map["Hippopotas"] = HIPPOPOTAS;
	species_map["Hippowdon"] = HIPPOWDON;
	species_map["Hitmonchan"] = HITMONCHAN;
	species_map["Hitmonlee"] = HITMONLEE;
	species_map["Hitmontop"] = HITMONTOP;
	species_map["Ho-Oh"] = HO_OH;
	species_map["Honchkrow"] = HONCHKROW;
	species_map["Hoothoot"] = HOOTHOOT;
	species_map["Hoppip"] = HOPPIP;
	species_map["Horsea"] = HORSEA;
	species_map["Houndoom"] = HOUNDOOM;
	species_map["Houndour"] = HOUNDOUR;
	species_map["Huntail"] = HUNTAIL;
	species_map["Hypno"] = HYPNO;
	species_map["Igglybuff"] = IGGLYBUFF;
	species_map["Illumise"] = ILLUMISE;
	species_map["Infernape"] = INFERNAPE;
	species_map["Ivysaur"] = IVYSAUR;
	species_map["Jigglypuff"] = JIGGLYPUFF;
	species_map["Jirachi"] = JIRACHI;
	species_map["Jolteon"] = JOLTEON;
	species_map["Jumpluff"] = JUMPLUFF;
	species_map["Jynx"] = JYNX;
	species_map["Kabuto"] = KABUTO;
	species_map["Kabutops"] = KABUTOPS;
	species_map["Kadabra"] = KADABRA;
	species_map["Kakuna"] = KAKUNA;
	species_map["Kangaskhan"] = KANGASKHAN;
	species_map["Kecleon"] = KECLEON;
	species_map["Kingdra"] = KINGDRA;
	species_map["Kingler"] = KINGLER;
	species_map["Kirlia"] = KIRLIA;
	species_map["Koffing"] = KOFFING;
	species_map["Krabby"] = KRABBY;
	species_map["Kricketot"] = KRICKETOT;
	species_map["Kricketune"] = KRICKETUNE;
	species_map["Kyogre"] = KYOGRE;
	species_map["Lairon"] = LAIRON;
	species_map["Lanturn"] = LANTURN;
	species_map["Lapras"] = LAPRAS;
	species_map["Larvitar"] = LARVITAR;
	species_map["Latias"] = LATIAS;
	species_map["Latios"] = LATIOS;
	species_map["Leafeon"] = LEAFEON;
	species_map["Ledian"] = LEDIAN;
	species_map["Ledyba"] = LEDYBA;
	species_map["Lickilicky"] = LICKILICKY;
	species_map["Lickitung"] = LICKITUNG;
	species_map["Lileep"] = LILEEP;
	species_map["Linoone"] = LINOONE;
	species_map["Lombre"] = LOMBRE;
	species_map["Lopunny"] = LOPUNNY;
	species_map["Lotad"] = LOTAD;
	species_map["Loudred"] = LOUDRED;
	species_map["Lucario"] = LUCARIO;
	species_map["Ludicolo"] = LUDICOLO;
	species_map["Lugia"] = LUGIA;
	species_map["Lumineon"] = LUMINEON;
	species_map["Lunatone"] = LUNATONE;
	species_map["Luvdisc"] = LUVDISC;
	species_map["Luxio"] = LUXIO;
	species_map["Luxray"] = LUXRAY;
	species_map["Machamp"] = MACHAMP;
	species_map["Machoke"] = MACHOKE;
	species_map["Machop"] = MACHOP;
	species_map["Magby"] = MAGBY;
	species_map["Magcargo"] = MAGCARGO;
	species_map["Magikarp"] = MAGIKARP;
	species_map["Magmar"] = MAGMAR;
	species_map["Magmortar"] = MAGMORTAR;
	species_map["Magnemite"] = MAGNEMITE;
	species_map["Magneton"] = MAGNETON;
	species_map["Magnezone"] = MAGNEZONE;
	species_map["Makuhita"] = MAKUHITA;
	species_map["Mamoswine"] = MAMOSWINE;
	species_map["Manaphy"] = MANAPHY;
	species_map["Manectric"] = MANECTRIC;
	species_map["Mankey"] = MANKEY;
	species_map["Mantine"] = MANTINE;
	species_map["Mantyke"] = MANTYKE;
	species_map["Mareep"] = MAREEP;
	species_map["Marill"] = MARILL;
	species_map["Marowak"] = MAROWAK;
	species_map["Marshtomp"] = MARSHTOMP;
	species_map["Masquerain"] = MASQUERAIN;
	species_map["Mawile"] = MAWILE;
	species_map["Medicham"] = MEDICHAM;
	species_map["Meditite"] = MEDITITE;
	species_map["Meganium"] = MEGANIUM;
	species_map["Meowth"] = MEOWTH;
	species_map["Mesprit"] = MESPRIT;
	species_map["Metagross"] = METAGROSS;
	species_map["Metang"] = METANG;
	species_map["Metapod"] = METAPOD;
	species_map["Mew"] = MEW;
	species_map["Mewtwo"] = MEWTWO;
	species_map["Mightyena"] = MIGHTYENA;
	species_map["Milotic"] = MILOTIC;
	species_map["Miltank"] = MILTANK;
	species_map["Mime Jr."] = MIME_JR;
	species_map["Minun"] = MINUN;
	species_map["Misdreavus"] = MISDREAVUS;
	species_map["Mismagius"] = MISMAGIUS;
	species_map["Moltres"] = MOLTRES;
	species_map["Monferno"] = MONFERNO;
	species_map["Mothim"] = MOTHIM;
	species_map["Mr. Mime"] = MR_MIME;
	species_map["Mudkip"] = MUDKIP;
	species_map["Muk"] = MUK;
	species_map["Munchlax"] = MUNCHLAX;
	species_map["Murkrow"] = MURKROW;
	species_map["Natu"] = NATU;
	species_map["Nidoking"] = NIDOKING;
	species_map["Nidoqueen"] = NIDOQUEEN;
	species_map["Nidoran-F"] = NIDORAN_F;
	species_map["Nidoran-M"] = NIDORAN_M;
	species_map["Nidorina"] = NIDORINA;
	species_map["Nidorino"] = NIDORINO;
	species_map["Nincada"] = NINCADA;
	species_map["Ninetales"] = NINETALES;
	species_map["Ninjask"] = NINJASK;
	species_map["Noctowl"] = NOCTOWL;
	species_map["Nosepass"] = NOSEPASS;
	species_map["Numel"] = NUMEL;
	species_map["Nuzleaf"] = NUZLEAF;
	species_map["Octillery"] = OCTILLERY;
	species_map["Oddish"] = ODDISH;
	species_map["Omanyte"] = OMANYTE;
	species_map["Omastar"] = OMASTAR;
	species_map["Onix"] = ONIX;
	species_map["Pachirisu"] = PACHIRISU;
	species_map["Palkia"] = PALKIA;
	species_map["Paras"] = PARAS;
	species_map["Parasect"] = PARASECT;
	species_map["Pelipper"] = PELIPPER;
	species_map["Persian"] = PERSIAN;
	species_map["Phanpy"] = PHANPY;
	species_map["Phione"] = PHIONE;
	species_map["Pichu"] = PICHU;
	species_map["Pidgeot"] = PIDGEOT;
	species_map["Pidgeotto"] = PIDGEOTTO;
	species_map["Pidgey"] = PIDGEY;
	species_map["Pikachu"] = PIKACHU;
	species_map["Piloswine"] = PILOSWINE;
	species_map["Pineco"] = PINECO;
	species_map["Pinsir"] = PINSIR;
	species_map["Piplup"] = PIPLUP;
	species_map["Plusle"] = PLUSLE;
	species_map["Politoed"] = POLITOED;
	species_map["Poliwag"] = POLIWAG;
	species_map["Poliwhirl"] = POLIWHIRL;
	species_map["Poliwrath"] = POLIWRATH;
	species_map["Ponyta"] = PONYTA;
	species_map["Poochyena"] = POOCHYENA;
	species_map["Porygon"] = PORYGON;
	species_map["Porygon-Z"] = PORYGON_Z;
	species_map["Porygon2"] = PORYGON2;
	species_map["Primeape"] = PRIMEAPE;
	species_map["Prinplup"] = PRINPLUP;
	species_map["Probopass"] = PROBOPASS;
	species_map["Psyduck"] = PSYDUCK;
	species_map["Pupitar"] = PUPITAR;
	species_map["Purugly"] = PURUGLY;
	species_map["Quagsire"] = QUAGSIRE;
	species_map["Quilava"] = QUILAVA;
	species_map["Qwilfish"] = QWILFISH;
	species_map["Raichu"] = RAICHU;
	species_map["Raikou"] = RAIKOU;
	species_map["Ralts"] = RALTS;
	species_map["Rampardos"] = RAMPARDOS;
	species_map["Rapidash"] = RAPIDASH;
	species_map["Raticate"] = RATICATE;
	species_map["Rattata"] = RATTATA;
	species_map["Rayquaza"] = RAYQUAZA;
	species_map["Regice"] = REGICE;
	species_map["Regigigas"] = REGIGIGAS;
	species_map["Regirock"] = REGIROCK;
	species_map["Registeel"] = REGISTEEL;
	species_map["Relicanth"] = RELICANTH;
	species_map["Remoraid"] = REMORAID;
	species_map["Rhydon"] = RHYDON;
	species_map["Rhyhorn"] = RHYHORN;
	species_map["Rhyperior"] = RHYPERIOR;
	species_map["Riolu"] = RIOLU;
	species_map["Roselia"] = ROSELIA;
	species_map["Roserade"] = ROSERADE;
	species_map["Rotom"] = ROTOM;
	species_map["Rotom-c"] = ROTOM_C;
	species_map["Rotom-f"] = ROTOM_F;
	species_map["Rotom-h"] = ROTOM_H;
	species_map["Rotom-s"] = ROTOM_S;
	species_map["Rotom-w"] = ROTOM_W;
	species_map["Sableye"] = SABLEYE;
	species_map["Salamence"] = SALAMENCE;
	species_map["Sandshrew"] = SANDSHREW;
	species_map["Sandslash"] = SANDSLASH;
	species_map["Sceptile"] = SCEPTILE;
	species_map["Scizor"] = SCIZOR;
	species_map["Scyther"] = SCYTHER;
	species_map["Seadra"] = SEADRA;
	species_map["Seaking"] = SEAKING;
	species_map["Sealeo"] = SEALEO;
	species_map["Seedot"] = SEEDOT;
	species_map["Seel"] = SEEL;
	species_map["Sentret"] = SENTRET;
	species_map["Seviper"] = SEVIPER;
	species_map["Sharpedo"] = SHARPEDO;
	species_map["Shaymin"] = SHAYMIN_L;
	species_map["Shaymin-s"] = SHAYMIN_S;
	species_map["Shedinja"] = SHEDINJA;
	species_map["Shelgon"] = SHELGON;
	species_map["Shellder"] = SHELLDER;
	species_map["Shellos"] = SHELLOS;
	species_map["Shieldon"] = SHIELDON;
	species_map["Shiftry"] = SHIFTRY;
	species_map["Shinx"] = SHINX;
	species_map["Shroomish"] = SHROOMISH;
	species_map["Shuckle"] = SHUCKLE;
	species_map["Shuppet"] = SHUPPET;
	species_map["Silcoon"] = SILCOON;
	species_map["Skarmory"] = SKARMORY;
	species_map["Skiploom"] = SKIPLOOM;
	species_map["Skitty"] = SKITTY;
	species_map["Skorupi"] = SKORUPI;
	species_map["Skuntank"] = SKUNTANK;
	species_map["Slaking"] = SLAKING;
	species_map["Slakoth"] = SLAKOTH;
	species_map["Slowbro"] = SLOWBRO;
	species_map["Slowking"] = SLOWKING;
	species_map["Slowpoke"] = SLOWPOKE;
	species_map["Slugma"] = SLUGMA;
	species_map["Smeargle"] = SMEARGLE;
	species_map["Smoochum"] = SMOOCHUM;
	species_map["Sneasel"] = SNEASEL;
	species_map["Snorlax"] = SNORLAX;
	species_map["Snorunt"] = SNORUNT;
	species_map["Snover"] = SNOVER;
	species_map["Snubbull"] = SNUBBULL;
	species_map["Solrock"] = SOLROCK;
	species_map["Spearow"] = SPEAROW;
	species_map["Spheal"] = SPHEAL;
	species_map["Spinarak"] = SPINARAK;
	species_map["Spinda"] = SPINDA;
	species_map["Spiritomb"] = SPIRITOMB;
	species_map["Spoink"] = SPOINK;
	species_map["Squirtle"] = SQUIRTLE;
	species_map["Stantler"] = STANTLER;
	species_map["Staraptor"] = STARAPTOR;
	species_map["Staravia"] = STARAVIA;
	species_map["Starly"] = STARLY;
	species_map["Starmie"] = STARMIE;
	species_map["Staryu"] = STARYU;
	species_map["Steelix"] = STEELIX;
	species_map["Stunky"] = STUNKY;
	species_map["Sudowoodo"] = SUDOWOODO;
	species_map["Suicune"] = SUICUNE;
	species_map["Sunflora"] = SUNFLORA;
	species_map["Sunkern"] = SUNKERN;
	species_map["Surskit"] = SURSKIT;
	species_map["Swablu"] = SWABLU;
	species_map["Swalot"] = SWALOT;
	species_map["Swampert"] = SWAMPERT;
	species_map["Swellow"] = SWELLOW;
	species_map["Swinub"] = SWINUB;
	species_map["Taillow"] = TAILLOW;
	species_map["Tangela"] = TANGELA;
	species_map["Tangrowth"] = TANGROWTH;
	species_map["Tauros"] = TAUROS;
	species_map["Teddiursa"] = TEDDIURSA;
	species_map["Tentacool"] = TENTACOOL;
	species_map["Tentacruel"] = TENTACRUEL;
	species_map["Togekiss"] = TOGEKISS;
	species_map["Togepi"] = TOGEPI;
	species_map["Togetic"] = TOGETIC;
	species_map["Torchic"] = TORCHIC;
	species_map["Torkoal"] = TORKOAL;
	species_map["Torterra"] = TORTERRA;
	species_map["Totodile"] = TOTODILE;
	species_map["Toxicroak"] = TOXICROAK;
	species_map["Trapinch"] = TRAPINCH;
	species_map["Treecko"] = TREECKO;
	species_map["Tropius"] = TROPIUS;
	species_map["Turtwig"] = TURTWIG;
	species_map["Typhlosion"] = TYPHLOSION;
	species_map["Tyranitar"] = TYRANITAR;
	species_map["Tyrogue"] = TYROGUE;
	species_map["Umbreon"] = UMBREON;
	species_map["Unown"] = UNOWN;
	species_map["Ursaring"] = URSARING;
	species_map["Uxie"] = UXIE;
	species_map["Vaporeon"] = VAPOREON;
	species_map["Venomoth"] = VENOMOTH;
	species_map["Venonat"] = VENONAT;
	species_map["Venusaur"] = VENUSAUR;
	species_map["Vespiquen"] = VESPIQUEN;
	species_map["Vibrava"] = VIBRAVA;
	species_map["Victreebel"] = VICTREEBEL;
	species_map["Vigoroth"] = VIGOROTH;
	species_map["Vileplume"] = VILEPLUME;
	species_map["Volbeat"] = VOLBEAT;
	species_map["Voltorb"] = VOLTORB;
	species_map["Vulpix"] = VULPIX;
	species_map["Wailmer"] = WAILMER;
	species_map["Wailord"] = WAILORD;
	species_map["Walrein"] = WALREIN;
	species_map["Wartortle"] = WARTORTLE;
	species_map["Weavile"] = WEAVILE;
	species_map["Weedle"] = WEEDLE;
	species_map["Weepinbell"] = WEEPINBELL;
	species_map["Weezing"] = WEEZING;
	species_map["Whiscash"] = WHISCASH;
	species_map["Whismur"] = WHISMUR;
	species_map["Wigglytuff"] = WIGGLYTUFF;
	species_map["Wingull"] = WINGULL;
	species_map["Wobbuffet"] = WOBBUFFET;
	species_map["Wooper"] = WOOPER;
	species_map["Wormadam"] = WORMADAM_P;
	species_map["Wormadam-g"] = WORMADAM_S;
	species_map["Wormadam-s"] = WORMADAM_T;
	species_map["Wurmple"] = WURMPLE;
	species_map["Wynaut"] = WYNAUT;
	species_map["Xatu"] = XATU;
	species_map["Yanma"] = YANMA;
	species_map["Yanmega"] = YANMEGA;
	species_map["Zangoose"] = ZANGOOSE;
	species_map["Zapdos"] = ZAPDOS;
	species_map["Zigzagoon"] = ZIGZAGOON;
	species_map["Zubat"] = ZUBAT;
}

#endif
