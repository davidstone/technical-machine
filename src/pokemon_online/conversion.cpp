// Convert to / from PO's format
// Copyright (C) 2012 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "conversion.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../move.hpp"
#include "../species.hpp"
#include "../stat.hpp"

namespace technicalmachine {
namespace po {

namespace {

class InvalidFormeId : public std::runtime_error {
	public:
		explicit InvalidFormeId (std::string const & species):
			std::runtime_error ("Invalid forme ID for " + species + ".\n")
			{
		}
};

unsigned get_forme_offset (unsigned id, unsigned forme) {
	constexpr unsigned deoxys = 386;
	constexpr unsigned giratina = 487;
	constexpr unsigned rotom = 479;
	constexpr unsigned shaymin = 492;
	constexpr unsigned wormadam = 413;
	switch (id) {
		case deoxys:
			switch (forme) {
				case 0:
					return 2;
				case 1:
					return 0;
				case 2:
					return 1;
				case 3:
					return 3;
				default:
					throw (InvalidFormeId ("Deoxys"));
			}
		case giratina:
			switch (forme) {
				case 0:
				case 1:
					return forme;
				default:
					throw (InvalidFormeId ("Giratina"));
			}
		case rotom:
			switch (forme) {
				case 0:
					return 0;
				case 1:
					return 1;
				case 2:
					return 3;
				case 3:
					return 2;
				case 4:
					return 5;
				case 5:
					return 4;
				default:
					throw (InvalidFormeId ("Rotom"));
			}
		case shaymin:
			switch (forme) {
				case 0:
				case 1:
					return forme;
				default:
					throw (InvalidFormeId ("Shaymin"));
			}
		case wormadam:
			switch (forme) {
				case 0:
				case 1:
				case 2:
					return forme;
				default:
					throw (InvalidFormeId ("Wormadam"));
			}
		default:
			return 0;
	}
}
	
unsigned get_forme (Species species) {
	switch (species) {
		case DEOXYS_A:
		case GIRATINA_O:
		case ROTOM_C:
		case SHAYMIN_S:
		case WORMADAM_S:
			return 1;
		case DEOXYS_D:
		case ROTOM_H:
		case WORMADAM_T:
			return 2;
		case DEOXYS_S:
		case ROTOM_F:
			return 3;
		case ROTOM_W:
			return 4;
		case ROTOM_S:
			return 5;
		default:
			return 0;
	}
}

}	// anonymous namespace

Species id_to_species (unsigned id, unsigned forme) {
	constexpr static Species species_converter [] = {
		END, BULBASAUR, IVYSAUR, VENUSAUR, CHARMANDER,
		CHARMELEON, CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE,
		CATERPIE, METAPOD, BUTTERFREE, WEEDLE, KAKUNA,
		BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, RATTATA,
		RATICATE, SPEAROW, FEAROW, EKANS, ARBOK,
		PIKACHU, RAICHU, SANDSHREW, SANDSLASH, NIDORAN_F,
		NIDORINA, NIDOQUEEN, NIDORAN_M, NIDORINO, NIDOKING,
		CLEFAIRY, CLEFABLE, VULPIX, NINETALES, JIGGLYPUFF,
		WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM,
		VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH,
		DIGLETT, DUGTRIO, MEOWTH, PERSIAN, PSYDUCK,
		GOLDUCK, MANKEY, PRIMEAPE, GROWLITHE, ARCANINE,
		POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA,
		ALAKAZAM, MACHOP, MACHOKE, MACHAMP, BELLSPROUT,
		WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE,
		GRAVELER, GOLEM, PONYTA, RAPIDASH, SLOWPOKE,
		SLOWBRO, MAGNEMITE, MAGNETON, FARFETCHD, DODUO,
		DODRIO, SEEL, DEWGONG, GRIMER, MUK,
		SHELGON, CLOYSTER, GASTLY, HAUNTER, GENGAR,
		ONIX, DROWZEE, HYPNO, KRABBY, KINGLER,
		VOLTORB, ELECTRODE, EXEGGCUTE, EXEGGUTOR, CUBONE,
		MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING,
		WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA,
		KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING,
		STARYU, STARMIE, MR_MIME, SCYTHER, JYNX,
		ELECTABUZZ, MAGMAR, PINSIR, TAUROS, MAGIKARP,
		GYARADOS, LAPRAS, DITTO, EEVEE, VAPOREON,
		JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR,
		KABUTO, KABUTOPS, AERODACTYL, SNORLAX, ARTICUNO,
		ZAPDOS, MOLTRES, DRATINI, DRAGONAIR, DRAGONITE,
		MEWTWO, MEW, CHIKORITA, BAYLEEF, MEGANIUM,
		CYNDAQUIL, QUILAVA, TYPHLOSION, TOTODILE, CROCONAW,
		FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL,
		LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT,
		CHINCHOU, LANTURN, PICHU, CLEFFA, IGGLYBUFF,
		TOGEPI, TOGETIC, NATU, XATU, MAREEP,
		FLAAFFY, AMPHAROS, BELLOSSOM, MARILL, AZUMARILL,
		SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF,
		AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER,
		QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING,
		MISDREAVUS, UNOWN, WOBBUFFET, GIRAFARIG, PINECO,
		FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, SNUBBULL,
		GRANBULL, QWILFISH, SCIZOR, SHUCKLE, HERACROSS,
		SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO,
		SWINUB, PILOSWINE, CORSOLA, REMORAID, OCTILLERY,
		DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM,
		KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER,
		SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID,
		MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI,
		SUICUNE, LARVITAR, PUPITAR, TYRANITAR, LUGIA,
		HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE,
		TORCHIC, COMBUSKEN, BLAZIKEN, MUDKIP, MARSHTOMP,
		SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, LINOONE,
		WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX,
		LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF,
		SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER,
		RALTS, KIRLIA, GARDEVOIR, SURSKIT, MASQUERAIN,
		SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING,
		NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED,
		EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS,
		SKITTY, DELCATTY, SABLEYE, MAWILE, ARON,
		LAIRON, AGGRON, MEDITITE, MEDICHAM, ELECTRIKE,
		MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE,
		ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO,
		WAILMER, WAILORD, NUMEL, CAMERUPT, TORKOAL,
		SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA,
		FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA,
		ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH,
		WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL,
		LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS,
		MILOTIC, CASTFORM, KECLEON, SHUPPET, BANETTE,
		DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL,
		WYNAUT, SNORUNT, GLALIE, SPHEAL, SEALEO,
		WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH,
		LUVDISC, BAGON, SHELLDER, SALAMENCE, BELDUM,
		METANG, METAGROSS, REGIROCK, REGICE, REGISTEEL,
		LATIAS, LATIOS, KYOGRE, GROUDON, RAYQUAZA,
		JIRACHI, DEOXYS_A, TURTWIG, GROTLE, TORTERRA,
		CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP,
		EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF,
		BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO,
		LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS,
		SHIELDON, BASTIODON, BURMY, WORMADAM_P,
		MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL,
		FLOATZEL, CHERUBI, CHERRIM, SHELLOS, GASTRODON,
		AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY,
		MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING,
		STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY,
		MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE,
		GABITE, GARCHOMP, MUNCHLAX, RIOLU, LUCARIO,
		HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK,
		TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE,
		SNOVER, ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY,
		RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS,
		YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE,
		PORYGON_Z, GALLADE, PROBOPASS, DUSKNOIR, FROSLASS,
		ROTOM, UXIE, MESPRIT, AZELF, DIALGA,
		PALKIA, HEATRAN, REGIGIGAS, GIRATINA_A, CRESSELIA,
		PHIONE, MANAPHY, DARKRAI, SHAYMIN_L,
		ARCEUS
	};
	Species const base_species = species_converter [id];
	unsigned const forme_offset = get_forme_offset (id, forme);
	return static_cast <Species> (base_species + forme_offset);
}

std::pair <uint16_t, uint8_t> species_to_id (Species species) {
	constexpr static unsigned species_converter [] = {
		460,		//	ABOMASNOW
		63,		//	ABRA
		359,		//	ABSOL
		142,		//	AERODACTYL
		306,		//	AGGRON
		190,		//	AIPOM
		65,		//	ALAKAZAM
		334,		//	ALTARIA
		424,		//	AMBIPOM
		181,		//	AMPHAROS
		347,		//	ANORITH
		24,		//	ARBOK
		59,		//	ARCANINE
		493,		//	ARCEUS
		168,		//	ARIADOS
		348,		//	ARMALDO
		304,		//	ARON
		144,		//	ARTICUNO
		482,		//	AZELF
		184,		//	AZUMARILL
		298,		//	AZURILL
		371,		//	BAGON
		343,		//	BALTOY
		354,		//	BANETTE
		339,		//	BARBOACH
		411,		//	BASTIODON
		153,		//	BAYLEEF
		267,		//	BEAUTIFLY
		15,		//	BEEDRILL
		374,		//	BELDUM
		182,		//	BELLOSSOM
		69,		//	BELLSPROUT
		400,		//	BIBAREL
		399,		//	BIDOOF
		9,		//	BLASTOISE
		257,		//	BLAZIKEN
		242,		//	BLISSEY
		438,		//	BONSLY
		286,		//	BRELOOM
		437,		//	BRONZONG
		436,		//	BRONZOR
		406,		//	BUDEW
		418,		//	BUIZEL
		1,		//	BULBASAUR
		427,		//	BUNEARY
		412,		//	BURMY
		12,		//	BUTTERFREE
		331,		//	CACNEA
		332,		//	CACTURNE
		323,		//	CAMERUPT
		455,		//	CARNIVINE
		318,		//	CARVANHA
		268,		//	CASCOON
		351,		//	CASTFORM
		10,		//	CATERPIE
		251,		//	CELEBI
		113,		//	CHANSEY
		6,		//	CHARIZARD
		4,		//	CHARMANDER
		5,		//	CHARMELEON
		441,		//	CHATOT
		421,		//	CHERRIM
		420,		//	CHERUBI
		152,		//	CHIKORITA
		390,		//	CHIMCHAR
		358,		//	CHIMECHO
		170,		//	CHINCHOU
		433,		//	CHINGLING
		366,		//	CLAMPERL
		344,		//	CLAYDOL
		36,		//	CLEFABLE
		35,		//	CLEFAIRY
		173,		//	CLEFFA
		91,		//	CLOYSTER
		415,		//	COMBEE
		256,		//	COMBUSKEN
		341,		//	CORPHISH
		222,		//	CORSOLA
		346,		//	CRADILY
		408,		//	CRANIDOS
		342,		//	CRAWDAUNT
		488,		//	CRESSELIA
		453,		//	CROAGUNK
		169,		//	CROBAT
		159,		//	CROCONAW
		104,		//	CUBONE
		155,		//	CYNDAQUIL
		491,		//	DARKRAI
		301,		//	DELCATTY
		225,		//	DELIBIRD
		386,		//	DEOXYS_A
		386,		//	DEOXYS_D
		386,		//	DEOXYS_M
		386,		//	DEOXYS_S
		87,		//	DEWGONG
		483,		//	DIALGA
		50,		//	DIGLETT
		132,		//	DITTO
		85,		//	DODRIO
		84,		//	DODUO
		232,		//	DONPHAN
		148,		//	DRAGONAIR
		149,		//	DRAGONITE
		452,		//	DRAPION
		147,		//	DRATINI
		426,		//	DRIFBLIM
		425,		//	DRIFLOON
		96,		//	DROWZEE
		51,		//	DUGTRIO
		206,		//	DUNSPARCE
		356,		//	DUSCLOPS
		477,		//	DUSKNOIR
		355,		//	DUSKULL
		269,		//	DUSTOX
		133,		//	EEVEE
		23,		//	EKANS
		125,		//	ELECTABUZZ
		466,		//	ELECTIVIRE
		309,		//	ELECTRIKE
		101,		//	ELECTRODE
		239,		//	ELEKID
		395,		//	EMPOLEON
		244,		//	ENTEI
		196,		//	ESPEON
		102,		//	EXEGGCUTE
		103,		//	EXEGGUTOR
		295,		//	EXPLOUD
		83,		//	FARFETCHD
		22,		//	FEAROW
		349,		//	FEEBAS
		160,		//	FERALIGATR
		456,		//	FINNEON
		180,		//	FLAAFFY
		136,		//	FLAREON
		419,		//	FLOATZEL
		330,		//	FLYGON
		205,		//	FORRETRESS
		478,		//	FROSLASS
		162,		//	FURRET
		444,		//	GABITE
		475,		//	GALLADE
		445,		//	GARCHOMP
		282,		//	GARDEVOIR
		92,		//	GASTLY
		423,		//	GASTRODON
		94,		//	GENGAR
		74,		//	GEODUDE
		443,		//	GIBLE
		203,		//	GIRAFARIG
		487,		//	GIRATINA_A
		487,		//	GIRATINA_O
		471,		//	GLACEON
		362,		//	GLALIE
		431,		//	GLAMEOW
		207,		//	GLIGAR
		472,		//	GLISCOR
		44,		//	GLOOM
		42,		//	GOLBAT
		118,		//	GOLDEEN
		55,		//	GOLDUCK
		76,		//	GOLEM
		368,		//	GOREBYSS
		210,		//	GRANBULL
		75,		//	GRAVELER
		88,		//	GRIMER
		388,		//	GROTLE
		383,		//	GROUDON
		253,		//	GROVYLE
		58,		//	GROWLITHE
		326,		//	GRUMPIG
		316,		//	GULPIN
		130,		//	GYARADOS
		440,		//	HAPPINY
		297,		//	HARIYAMA
		93,		//	HAUNTER
		485,		//	HEATRAN
		214,		//	HERACROSS
		449,		//	HIPPOPOTAS
		450,		//	HIPPOWDON
		107,		//	HITMONCHAN
		106,		//	HITMONLEE
		237,		//	HITMONTOP
		250,		//	HO_OH
		430,		//	HONCHKROW
		163,		//	HOOTHOOT
		187,		//	HOPPIP
		116,		//	HORSEA
		229,		//	HOUNDOOM
		228,		//	HOUNDOUR
		367,		//	HUNTAIL
		97,		//	HYPNO
		174,		//	IGGLYBUFF
		314,		//	ILLUMISE
		392,		//	INFERNAPE
		2,		//	IVYSAUR
		39,		//	JIGGLYPUFF
		385,		//	JIRACHI
		135,		//	JOLTEON
		189,		//	JUMPLUFF
		124,		//	JYNX
		140,		//	KABUTO
		141,		//	KABUTOPS
		64,		//	KADABRA
		14,		//	KAKUNA
		115,		//	KANGASKHAN
		352,		//	KECLEON
		230,		//	KINGDRA
		99,		//	KINGLER
		281,		//	KIRLIA
		109,		//	KOFFING
		98,		//	KRABBY
		401,		//	KRICKETOT
		402,		//	KRICKETUNE
		382,		//	KYOGRE
		305,		//	LAIRON
		171,		//	LANTURN
		131,		//	LAPRAS
		246,		//	LARVITAR
		380,		//	LATIAS
		381,		//	LATIOS
		470,		//	LEAFEON
		166,		//	LEDIAN
		165,		//	LEDYBA
		463,		//	LICKILICKY
		108,		//	LICKITUNG
		345,		//	LILEEP
		264,		//	LINOONE
		271,		//	LOMBRE
		428,		//	LOPUNNY
		270,		//	LOTAD
		294,		//	LOUDRED
		448,		//	LUCARIO
		272,		//	LUDICOLO
		249,		//	LUGIA
		457,		//	LUMINEON
		337,		//	LUNATONE
		370,		//	LUVDISC
		404,		//	LUXIO
		405,		//	LUXRAY
		68,		//	MACHAMP
		67,		//	MACHOKE
		66,		//	MACHOP
		240,		//	MAGBY
		219,		//	MAGCARGO
		129,		//	MAGIKARP
		126,		//	MAGMAR
		467,		//	MAGMORTAR
		81,		//	MAGNEMITE
		82,		//	MAGNETON
		462,		//	MAGNEZONE
		296,		//	MAKUHITA
		473,		//	MAMOSWINE
		490,		//	MANAPHY
		310,		//	MANECTRIC
		56,		//	MANKEY
		226,		//	MANTINE
		458,		//	MANTYKE
		179,		//	MAREEP
		183,		//	MARILL
		105,		//	MAROWAK
		259,		//	MARSHTOMP
		284,		//	MASQUERAIN
		303,		//	MAWILE
		308,		//	MEDICHAM
		307,		//	MEDITITE
		154,		//	MEGANIUM
		52,		//	MEOWTH
		481,		//	MESPRIT
		376,		//	METAGROSS
		375,		//	METANG
		11,		//	METAPOD
		151,		//	MEW
		150,		//	MEWTWO
		262,		//	MIGHTYENA
		350,		//	MILOTIC
		241,		//	MILTANK
		439,		//	MIME_JR
		312,		//	MINUN
		200,		//	MISDREAVUS
		429,		//	MISMAGIUS
		146,		//	MOLTRES
		391,		//	MONFERNO
		414,		//	MOTHIM
		122,		//	MR_MIME
		258,		//	MUDKIP
		89,		//	MUK
		446,		//	MUNCHLAX
		198,		//	MURKROW
		177,		//	NATU
		34,		//	NIDOKING
		31,		//	NIDOQUEEN
		29,		//	NIDORAN_F
		32,		//	NIDORAN_M
		30,		//	NIDORINA
		33,		//	NIDORINO
		290,		//	NINCADA
		38,		//	NINETALES
		291,		//	NINJASK
		164,		//	NOCTOWL
		299,		//	NOSEPASS
		322,		//	NUMEL
		274,		//	NUZLEAF
		224,		//	OCTILLERY
		43,		//	ODDISH
		138,		//	OMANYTE
		139,		//	OMASTAR
		95,		//	ONIX
		417,		//	PACHIRISU
		484,		//	PALKIA
		46,		//	PARAS
		47,		//	PARASECT
		279,		//	PELIPPER
		53,		//	PERSIAN
		231,		//	PHANPY
		489,		//	PHIONE
		172,		//	PICHU
		18,		//	PIDGEOT
		17,		//	PIDGEOTTO
		16,		//	PIDGEY
		25,		//	PIKACHU
		221,		//	PILOSWINE
		204,		//	PINECO
		127,		//	PINSIR
		393,		//	PIPLUP
		311,		//	PLUSLE
		186,		//	POLITOED
		60,		//	POLIWAG
		61,		//	POLIWHIRL
		62,		//	POLIWRATH
		77,		//	PONYTA
		261,		//	POOCHYENA
		137,		//	PORYGON
		474,		//	PORYGON_Z
		233,		//	PORYGON2
		57,		//	PRIMEAPE
		394,		//	PRINPLUP
		476,		//	PROBOPASS
		54,		//	PSYDUCK
		247,		//	PUPITAR
		432,		//	PURUGLY
		195,		//	QUAGSIRE
		156,		//	QUILAVA
		211,		//	QWILFISH
		26,		//	RAICHU
		243,		//	RAIKOU
		280,		//	RALTS
		409,		//	RAMPARDOS
		78,		//	RAPIDASH
		20,		//	RATICATE
		19,		//	RATTATA
		384,		//	RAYQUAZA
		378,		//	REGICE
		486,		//	REGIGIGAS
		377,		//	REGIROCK
		379,		//	REGISTEEL
		369,		//	RELICANTH
		223,		//	REMORAID
		112,		//	RHYDON
		111,		//	RHYHORN
		464,		//	RHYPERIOR
		447,		//	RIOLU
		315,		//	ROSELIA
		407,		//	ROSERADE
		479,		//	ROTOM
		479,		//	ROTOM_C
		479,		//	ROTOM_H
		479,		//	ROTOM_F
		479,		//	ROTOM_W
		479,		//	ROTOM_S
		302,		//	SABLEYE
		373,		//	SALAMENCE
		27,		//	SANDSHREW
		28,		//	SANDSLASH
		254,		//	SCEPTILE
		212,		//	SCIZOR
		123,		//	SCYTHER
		117,		//	SEADRA
		119,		//	SEAKING
		364,		//	SEALEO
		273,		//	SEEDOT
		86,		//	SEEL
		161,		//	SENTRET
		336,		//	SEVIPER
		319,		//	SHARPEDO
		492,		//	SHAYMIN_L
		492,		//	SHAYMIN_S
		292,		//	SHEDINJA
		90,		//	SHELGON
		372,		//	SHELLDER
		422,		//	SHELLOS
		410,		//	SHIELDON
		275,		//	SHIFTRY
		403,		//	SHINX
		285,		//	SHROOMISH
		213,		//	SHUCKLE
		353,		//	SHUPPET
		266,		//	SILCOON
		227,		//	SKARMORY
		188,		//	SKIPLOOM
		300,		//	SKITTY
		451,		//	SKORUPI
		435,		//	SKUNTANK
		289,		//	SLAKING
		287,		//	SLAKOTH
		80,		//	SLOWBRO
		199,		//	SLOWKING
		79,		//	SLOWPOKE
		218,		//	SLUGMA
		235,		//	SMEARGLE
		238,		//	SMOOCHUM
		215,		//	SNEASEL
		143,		//	SNORLAX
		361,		//	SNORUNT
		459,		//	SNOVER
		209,		//	SNUBBULL
		338,		//	SOLROCK
		21,		//	SPEAROW
		363,		//	SPHEAL
		167,		//	SPINARAK
		327,		//	SPINDA
		442,		//	SPIRITOMB
		325,		//	SPOINK
		7,		//	SQUIRTLE
		234,		//	STANTLER
		398,		//	STARAPTOR
		397,		//	STARAVIA
		396,		//	STARLY
		121,		//	STARMIE
		120,		//	STARYU
		208,		//	STEELIX
		434,		//	STUNKY
		185,		//	SUDOWOODO
		245,		//	SUICUNE
		192,		//	SUNFLORA
		191,		//	SUNKERN
		283,		//	SURSKIT
		333,		//	SWABLU
		317,		//	SWALOT
		260,		//	SWAMPERT
		277,		//	SWELLOW
		220,		//	SWINUB
		276,		//	TAILLOW
		114,		//	TANGELA
		465,		//	TANGROWTH
		128,		//	TAUROS
		216,		//	TEDDIURSA
		72,		//	TENTACOOL
		73,		//	TENTACRUEL
		468,		//	TOGEKISS
		175,		//	TOGEPI
		176,		//	TOGETIC
		255,		//	TORCHIC
		324,		//	TORKOAL
		389,		//	TORTERRA
		158,		//	TOTODILE
		454,		//	TOXICROAK
		328,		//	TRAPINCH
		252,		//	TREECKO
		357,		//	TROPIUS
		387,		//	TURTWIG
		157,		//	TYPHLOSION
		248,		//	TYRANITAR
		236,		//	TYROGUE
		197,		//	UMBREON
		201,		//	UNOWN
		217,		//	URSARING
		480,		//	UXIE
		134,		//	VAPOREON
		49,		//	VENOMOTH
		48,		//	VENONAT
		3,		//	VENUSAUR
		416,		//	VESPIQUEN
		329,		//	VIBRAVA
		71,		//	VICTREEBEL
		288,		//	VIGOROTH
		45,		//	VILEPLUME
		313,		//	VOLBEAT
		100,		//	VOLTORB
		37,		//	VULPIX
		320,		//	WAILMER
		321,		//	WAILORD
		365,		//	WALREIN
		8,		//	WARTORTLE
		461,		//	WEAVILE
		13,		//	WEEDLE
		70,		//	WEEPINBELL
		110,		//	WEEZING
		340,		//	WHISCASH
		293,		//	WHISMUR
		40,		//	WIGGLYTUFF
		278,		//	WINGULL
		202,		//	WOBBUFFET
		194,		//	WOOPER
		413,		//	WORMADAM_P
		413,		//	WORMADAM_S
		413,		//	WORMADAM_T
		265,		//	WURMPLE
		360,		//	WYNAUT
		178,		//	XATU
		193,		//	YANMA
		469,		//	YANMEGA
		335,		//	ZANGOOSE
		145,		//	ZAPDOS
		263,		//	ZIGZAGOON
		41,		//	ZUBAT
		0		//	END
	};
	unsigned const species_id = species_converter [species];
	unsigned const forme_id = get_forme (species);
	return std::pair <unsigned, unsigned> (species_id, forme_id);
}

Ability::Abilities id_to_ability (unsigned id) {
	constexpr static Ability::Abilities ability_converter [] = {
		Ability::END, Ability::STENCH, Ability::DRIZZLE,
		Ability::SPEED_BOOST, Ability::BATTLE_ARMOR, Ability::STURDY,
		Ability::DAMP, Ability::LIMBER, Ability::SAND_VEIL,
		Ability::STATIC, Ability::VOLT_ABSORB, Ability::WATER_ABSORB,
		Ability::OBLIVIOUS, Ability::CLOUD_NINE, Ability::COMPOUNDEYES,
		Ability::INSOMNIA, Ability::COLOR_CHANGE, Ability::IMMUNITY,
		Ability::FLASH_FIRE, Ability::SHIELD_DUST, Ability::OWN_TEMPO,
		Ability::SUCTION_CUPS, Ability::INTIMIDATE, Ability::SHADOW_TAG,
		Ability::ROUGH_SKIN, Ability::WONDER_GUARD, Ability::LEVITATE,
		Ability::EFFECT_SPORE, Ability::SYNCHRONIZE, Ability::CLEAR_BODY,
		Ability::NATURAL_CURE, Ability::LIGHTNINGROD, Ability::SERENE_GRACE,
		Ability::SWIFT_SWIM, Ability::CHLOROPHYLL, Ability::ILLUMINATE,
		Ability::TRACE, Ability::HUGE_POWER, Ability::POISON_POINT,
		Ability::INNER_FOCUS, Ability::MAGMA_ARMOR, Ability::WATER_VEIL,
		Ability::MAGNET_PULL, Ability::SOUNDPROOF, Ability::RAIN_DISH,
		Ability::SAND_STREAM, Ability::PRESSURE, Ability::THICK_FAT,
		Ability::EARLY_BIRD, Ability::FLAME_BODY, Ability::RUN_AWAY,
		Ability::KEEN_EYE, Ability::HYPER_CUTTER, Ability::PICKUP,
		Ability::TRUANT, Ability::HUSTLE, Ability::CUTE_CHARM,
		Ability::PLUS, Ability::MINUS, Ability::FORECAST,
		Ability::STICKY_HOLD, Ability::SHED_SKIN, Ability::GUTS,
		Ability::MARVEL_SCALE, Ability::LIQUID_OOZE, Ability::OVERGROW,
		Ability::BLAZE, Ability::TORRENT, Ability::SWARM,
		Ability::ROCK_HEAD, Ability::DROUGHT, Ability::ARENA_TRAP,
		Ability::VITAL_SPIRIT, Ability::WHITE_SMOKE, Ability::PURE_POWER,
		Ability::SHELL_ARMOR, Ability::AIR_LOCK, Ability::TANGLED_FEET,
		Ability::MOTOR_DRIVE, Ability::RIVALRY, Ability::STEADFAST,
		Ability::SNOW_CLOAK, Ability::GLUTTONY, Ability::ANGER_POINT,
		Ability::UNBURDEN, Ability::HEATPROOF, Ability::SIMPLE,
		Ability::DRY_SKIN, Ability::DOWNLOAD, Ability::IRON_FIST,
		Ability::POISON_HEAL, Ability::ADAPTABILITY, Ability::SKILL_LINK,
		Ability::HYDRATION, Ability::SOLAR_POWER, Ability::QUICK_FEET,
		Ability::NORMALIZE, Ability::SNIPER, Ability::MAGIC_GUARD,
		Ability::NO_GUARD, Ability::STALL, Ability::TECHNICIAN,
		Ability::LEAF_GUARD, Ability::KLUTZ, Ability::MOLD_BREAKER,
		Ability::SUPER_LUCK, Ability::AFTERMATH, Ability::ANTICIPATION,
		Ability::FOREWARN, Ability::UNAWARE, Ability::TINTED_LENS,
		Ability::FILTER, Ability::SLOW_START, Ability::SCRAPPY,
		Ability::STORM_DRAIN, Ability::ICE_BODY, Ability::SOLID_ROCK,
		Ability::SNOW_WARNING, Ability::HONEY_GATHER, Ability::FRISK,
		Ability::RECKLESS, Ability::MULTITYPE, Ability::FLOWER_GIFT,
		Ability::BAD_DREAMS
	};
	return ability_converter [id];
}

unsigned ability_to_id (Ability::Abilities ability) {
	constexpr static unsigned ability_converter [] = {
		91,		// ADAPTABILITY
		106,		// AFTERMATH
		76,		// AIR_LOCK
		83,		// ANGER_POINT
		107,		// ANTICIPATION
		71,		// ARENA_TRAP
		123,		// BAD_DREAMS
		4,		// BATTLE_ARMOR
		66,		// BLAZE
		34,		// CHLOROPHYLL
		29,		// CLEAR_BODY
		13,		// CLOUD_NINE
		16,		// COLOR_CHANGE
		14,		// COMPOUNDEYES
		56,		// CUTE_CHARM
		6,		// DAMP
		88,		// DOWNLOAD
		2,		// DRIZZLE
		70,		// DROUGHT
		87,		// DRY_SKIN
		48,		// EARLY_BIRD
		27,		// EFFECT_SPORE
		111,		// FILTER
		49,		// FLAME_BODY
		18,		// FLASH_FIRE
		122,		// FLOWER_GIFT
		59,		// FORECAST
		108,		// FOREWARN
		119,		// FRISK
		82,		// GLUTTONY
		62,		// GUTS
		85,		// HEATPROOF
		118,		// HONEY_GATHER
		37,		// HUGE_POWER
		55,		// HUSTLE
		93,		// HYDRATION
		52,		// HYPER_CUTTER
		115,		// ICE_BODY
		35,		// ILLUMINATE
		17,		// IMMUNITY
		39,		// INNER_FOCUS
		15,		// INSOMNIA
		22,		// INTIMIDATE
		89,		// IRON_FIST
		51,		// KEEN_EYE
		103,		// KLUTZ
		102,		// LEAF_GUARD
		26,		// LEVITATE
		31,		// LIGHTNINGROD
		7,		// LIMBER
		64,		// LIQUID_OOZE
		98,		// MAGIC_GUARD
		40,		// MAGMA_ARMOR
		42,		// MAGNET_PULL
		63,		// MARVEL_SCALE
		58,		// MINUS
		104,		// MOLD_BREAKER
		78,		// MOTOR_DRIVE
		121,		// MULTITYPE
		30,		// NATURAL_CURE
		99,		// NO_GUARD
		96,		// NORMALIZE
		12,		// OBLIVIOUS
		65,		// OVERGROW
		20,		// OWN_TEMPO
		53,		// PICKUP
		57,		// PLUS
		90,		// POISON_HEAL
		38,		// POISON_POINT
		46,		// PRESSURE
		74,		// PURE_POWER
		95,		// QUICK_FEET
		44,		// RAIN_DISH
		120,		// RECKLESS
		79,		// RIVALRY
		69,		// ROCK_HEAD
		24,		// ROUGH_SKIN
		50,		// RUN_AWAY
		45,		// SAND_STREAM
		8,		// SAND_VEIL
		113,		// SCRAPPY
		32,		// SERENE_GRACE
		23,		// SHADOW_TAG
		61,		// SHED_SKIN
		75,		// SHELL_ARMOR
		19,		// SHIELD_DUST
		86,		// SIMPLE
		92,		// SKILL_LINK
		112,		// SLOW_START
		97,		// SNIPER
		81,		// SNOW_CLOAK
		117,		// SNOW_WARNING
		94,		// SOLAR_POWER
		116,		// SOLID_ROCK
		43,		// SOUNDPROOF
		3,		// SPEED_BOOST
		100,		// STALL
		9,		// STATIC
		80,		// STEADFAST
		1,		// STENCH
		60,		// STICKY_HOLD
		114,		// STORM_DRAIN
		5,		// STURDY
		21,		// SUCTION_CUPS
		105,		// SUPER_LUCK
		68,		// SWARM
		33,		// SWIFT_SWIM
		28,		// SYNCHRONIZE
		77,		// TANGLED_FEET
		101,		// TECHNICIAN
		47,		// THICK_FAT
		110,		// TINTED_LENS
		67,		// TORRENT
		36,		// TRACE
		54,		// TRUANT
		109,		// UNAWARE
		84,		// UNBURDEN
		72,		// VITAL_SPIRIT
		10,		// VOLT_ABSORB
		11,		// WATER_ABSORB
		41,		// WATER_VEIL
		73,		// WHITE_SMOKE
		25,		// WONDER_GUARD
		0		// END
	};
	return ability_converter [ability];
}

class InvalidPart : public std::runtime_error {
	public:
		InvalidPart (uint16_t id, uint8_t part):
			std::runtime_error ("Invalid conversion to ability ID: " + std::to_string (id) + " with part ID: " + std::to_string (static_cast <unsigned> (part)) + ".\n") {
		}
};

Ability::Abilities battle_id_to_ability (uint16_t id, uint8_t part) {
	// 6: BAD_DREAMS
	// 18: %s's %a activates!
	// 21: %s changed its type to %t!
	// 30: %s's %a prevents its stat from being lowered!
	// 31: %s's %a prevented its stats from being lowered!
	// 32: %s's %a heals it!
	// 33: %s's %a cures it!|%s didn't get paralyzed because of its %a!|%s stayed awake because of its %a!|%s didn't get frozen because of its %a!|%s didn't get burnt because of its %a!|%s didn't get poisoned because of its %a!
	// 38: %s's %a took the attack!|%s's %a raised its special attack!|%s's %a made the attack useless!
	// 40: %s has %a!
	// 50: %s's %a hurts %f
	// 68: %s's %a raised its attack!|%s's %a made the attack useless!
	// 70: %s's %a absorbs the attack!|%s's %a made the attack useless!
	switch (id) {
		case 2:
			return Ability::AFTERMATH;
		case 3:
			return Ability::ANGER_POINT;
		case 4:
			return Ability::ANTICIPATION;
		case 6:
			return Ability::BAD_DREAMS;
		case 9:
			return Ability::COLOR_CHANGE;
		case 11:
			return Ability::CUTE_CHARM;
		case 12:
			return Ability::INNER_FOCUS;
		case 13:
			return Ability::DOWNLOAD;
		case 14:
			switch (part) {
				case 0:
					return Ability::SNOW_WARNING;
				case 1:
					return Ability::DRIZZLE;
				case 2:
					return Ability::SAND_STREAM;
				case 3:
					return Ability::DROUGHT;
				default:
					throw InvalidPart (id, part);
			}
		case 15:
			return Ability::DRY_SKIN;
		case 16:
			return Ability::EFFECT_SPORE;
		case 19:
			return Ability::FLASH_FIRE;
		case 22:
			return Ability::FOREWARN;
		case 23:
			return Ability::FRISK;
		case 24:
			return Ability::SHIELD_DUST;
		case 29:
			return Ability::HYDRATION;
		case 34:
			return Ability::INTIMIDATE;
		case 37:
			return Ability::LEAF_GUARD;
		case 41:
			return Ability::MOTOR_DRIVE;
		case 44:
			return Ability::OWN_TEMPO;
		case 45:
			return Ability::POISON_HEAL;
		case 46:
			return Ability::PRESSURE;
		case 54:
			return Ability::SHED_SKIN;
		case 55:
			return Ability::SLOW_START;
		case 56:
			return Ability::SOLAR_POWER;
		case 57:
			return Ability::SOUNDPROOF;
		case 58:
			return Ability::SPEED_BOOST;
		case 60:
			return Ability::STEADFAST;
		case 61:
			return Ability::SYNCHRONIZE;
		case 66:
			return Ability::TRACE;
		case 67:
			return Ability::TRUANT;
		case 71:
			return Ability::WONDER_GUARD;
		default:
			return Ability::END;
	}
}

Gender::Genders id_to_gender (unsigned id) {
	switch (id) {
		case 0:
			return Gender::GENDERLESS;
		case 1:
			return Gender::MALE;
		default:	// case 2:
			return Gender::FEMALE;
	}
}

unsigned gender_to_id (Gender::Genders gender) {
	switch (gender) {
		case Gender::MALE:
			return 1;
		case Gender::FEMALE:
			return 2;
		default:	// Gender::GENDERLESS:
			return 0;
	}
}

Item::Items id_to_item (unsigned id) {
	switch (id) {
		case 0:
			return Item::NO_ITEM;
		case 1:
			return Item::BIG_ROOT;
		case 2:
			return Item::BLUE_SCARF;
		case 3:
			return Item::BRIGHTPOWDER;
		case 4:
			return Item::CHOICE_BAND;
		case 5:
			return Item::CHOICE_SCARF;
		case 6:
			return Item::CHOICE_SPECS;
		case 7:
			return Item::DESTINY_KNOT;
		case 8:
			return Item::EXPERT_BELT;
		case 9:
			return Item::FOCUS_BAND;
		case 10:
			return Item::FOCUS_SASH;
		case 11:
			return Item::FULL_INCENSE;
		case 12:
			return Item::GREEN_SCARF;
		case 13:
			return Item::LAGGING_TAIL;
		case 14:
			return Item::LAX_INCENSE;
		case 15:
			return Item::LEFTOVERS;
		case 16:
			return Item::LUCK_INCENSE;
		case 17:
			return Item::MENTAL_HERB;
		case 18:
			return Item::METAL_POWDER;
		case 19:
			return Item::MUSCLE_BAND;
		case 20:
			return Item::ODD_INCENSE;
		case 21:
			return Item::PINK_SCARF;
		case 22:
			return Item::POWER_HERB;
		case 23:
			return Item::PURE_INCENSE;
		case 24:
			return Item::QUICK_POWDER;
		case 25:
			return Item::REAPER_CLOTH;
		case 26:
			return Item::RED_SCARF;
		case 27:
			return Item::ROCK_INCENSE;
		case 28:
			return Item::ROSE_INCENSE;
		case 29:
			return Item::SEA_INCENSE;
		case 30:
			return Item::SHED_SHELL;
		case 31:
			return Item::SILK_SCARF;
		case 32:
			return Item::SILVERPOWDER;
		case 33:
			return Item::SMOOTH_ROCK;
		case 34:
			return Item::SOFT_SAND;
		case 35:
			return Item::SOOTHE_BELL;
		case 36:
			return Item::WAVE_INCENSE;
		case 37:
			return Item::WHITE_HERB;
		case 38:
			return Item::WIDE_LENS;
		case 39:
			return Item::WISE_GLASSES;
		case 40:
			return Item::YELLOW_SCARF;
		case 41:
			return Item::ZOOM_LENS;
		case 42:
			return Item::AMULET_COIN;
		case 43:
			return Item::ANTIDOTE;
		case 44:
			return Item::AWAKENING;
		case 45:
			return Item::BERRY_JUICE;
		case 46:
			return Item::BIG_PEARL;
		case 47:
			return Item::BIG_MUSHROOM;
		case 48:
			return Item::BLACK_BELT;
		case 49:
			return Item::BLACK_FLUTE;
		case 50:
			return Item::BLACK_SLUDGE;
		case 51:
			return Item::BLACKGLASSES;
		case 52:
			return Item::BLUE_FLUTE;
		case 53:
			return Item::BLUE_SHARD;
		case 54:
			return Item::BURN_HEAL;
		case 55:
			return Item::CALCIUM;
		case 56:
			return Item::CARBOS;
		case 57:
			return Item::CHARCOAL;
		case 58:
			return Item::CLEANSE_TAG;
		case 59:
			return Item::DAMP_MULCH;
		case 60:
			return Item::DEEPSEASCALE;
		case 61:
			return Item::DIRE_HIT;
		case 62:
			return Item::DRAGON_SCALE;
		case 63:
			return Item::ELIXIR;
		case 64:
			return Item::ENERGY_ROOT;
		case 65:
			return Item::ENERGYPOWDER;
		case 66:
			return Item::ESCAPE_ROPE;
		case 67:
			return Item::ETHER;
		case 68:
			return Item::EVERSTONE;
		case 69:
			return Item::EXP_SHARE;
		case 70:
			return Item::FIRE_STONE;
		case 71:
			return Item::FLAME_ORB;
		case 72:
			return Item::FLUFFY_TAIL;
		case 73:
			return Item::FRESH_WATER;
		case 74:
			return Item::FULL_HEAL;
		case 75:
			return Item::FULL_RESTORE;
		case 76:
			return Item::GOOEY_MULCH;
		case 77:
			return Item::GREEN_SHARD;
		case 78:
			return Item::GROWTH_MULCH;
		case 79:
			return Item::GUARD_SPEC;
		case 80:
			return Item::HEAL_POWDER;
		case 81:
			return Item::HEART_SCALE;
		case 82:
			return Item::HONEY;
		case 83:
			return Item::HP_UP;
		case 84:
			return Item::HYPER_POTION;
		case 85:
			return Item::ICE_HEAL;
		case 86:
			return Item::IRON;
		case 87:
			return Item::KINGS_ROCK;
		case 88:
			return Item::LAVA_COOKIE;
		case 89:
			return Item::LEAF_STONE;
		case 90:
			return Item::LEMONADE;
		case 91:
			return Item::LIFE_ORB;
		case 92:
			return Item::LIGHT_BALL;
		case 93:
			return Item::LIGHT_CLAY;
		case 94:
			return Item::LUCKY_EGG;
		case 95:
			return Item::MAGNET;
		case 96:
			return Item::MAX_ELIXIR;
		case 97:
			return Item::MAX_ETHER;
		case 98:
			return Item::MAX_POTION;
		case 99:
			return Item::MAX_REPEL;
		case 100:
			return Item::MAX_REVIVE;
		case 101:
			return Item::METAL_COAT;
		case 102:
			return Item::METRONOME;
		case 103:
			return Item::MIRACLE_SEED;
		case 104:
			return Item::MOOMOO_MILK;
		case 105:
			return Item::MOON_STONE;
		case 106:
			return Item::MYSTIC_WATER;
		case 107:
			return Item::NEVERMELTICE;
		case 108:
			return Item::NUGGET;
		case 109:
			return Item::OLD_GATEAU;
		case 110:
			return Item::PARLYZ_HEAL;
		case 111:
			return Item::PEARL;
		case 112:
			return Item::POKE_DOLL;
		case 113:
			return Item::POTION;
		case 114:
			return Item::PP_MAX;
		case 115:
			return Item::PP_UP;
		case 116:
			return Item::PROTEIN;
		case 117:
			return Item::RARE_CANDY;
		case 118:
			return Item::RAZOR_FANG;
		case 119:
			return Item::RED_FLUTE;
		case 120:
			return Item::RED_SHARD;
		case 121:
			return Item::REPEL;
		case 122:
			return Item::REVIVAL_HERB;
		case 123:
			return Item::REVIVE;
		case 124:
			return Item::SACRED_ASH;
		case 125:
			return Item::SCOPE_LENS;
		case 126:
			return Item::SHELL_BELL;
		case 127:
			return Item::SHOAL_SALT;
		case 128:
			return Item::SHOAL_SHELL;
		case 129:
			return Item::SMOKE_BALL;
		case 130:
			return Item::SODA_POP;
		case 131:
			return Item::SOUL_DEW;
		case 132:
			return Item::SPELL_TAG;
		case 133:
			return Item::STABLE_MULCH;
		case 134:
			return Item::STAR_PIECE;
		case 135:
			return Item::STARDUST;
		case 136:
			return Item::SUN_STONE;
		case 137:
			return Item::SUPER_POTION;
		case 138:
			return Item::SUPER_REPEL;
		case 139:
			return Item::THUNDERSTONE;
		case 140:
			return Item::TINYMUSHROOM;
		case 141:
			return Item::TOXIC_ORB;
		case 142:
			return Item::TWISTEDSPOON;
		case 143:
			return Item::UP_GRADE;
		case 144:
			return Item::WATER_STONE;
		case 145:
			return Item::WHITE_FLUTE;
		case 146:
			return Item::X_ACCURACY;
		case 147:
			return Item::X_ATTACK;
		case 148:
			return Item::X_DEFEND;
		case 149:
			return Item::X_SPECIAL;
		case 150:
			return Item::X_SP_DEF;
		case 151:
			return Item::X_SPEED;
		case 152:
			return Item::YELLOW_FLUTE;
		case 153:
			return Item::YELLOW_SHARD;
		case 154:
			return Item::ZINC;
		case 155:
			return Item::ICY_ROCK;
		case 156:
			return Item::LUCKY_PUNCH;
		case 157:
			return Item::DUBIOUS_DISC;
		case 158:
			return Item::SHARP_BEAK;
		case 159:
			return Item::ADAMANT_ORB;
		case 160:
			return Item::DAMP_ROCK;
		case 161:
			return Item::HEAT_ROCK;
		case 162:
			return Item::LUSTROUS_ORB;
		case 163:
			return Item::MACHO_BRACE;
		case 164:
			return Item::STICK;
		case 165:
			return Item::DRAGON_FANG;
		case 166:
			return Item::POISON_BARB;
		case 167:
			return Item::POWER_ANKLET;
		case 168:
			return Item::POWER_BAND;
		case 169:
			return Item::POWER_BELT;
		case 170:
			return Item::POWER_BRACER;
		case 171:
			return Item::POWER_LENS;
		case 172:
			return Item::POWER_WEIGHT;
		case 173:
			return Item::DAWN_STONE;
		case 174:
			return Item::DUSK_STONE;
		case 175:
			return Item::ELECTIRIZER;
		case 176:
			return Item::MAGMARIZER;
		case 177:
			return Item::ODD_KEYSTONE;
		case 178:
			return Item::OVAL_STONE;
		case 179:
			return Item::PROTECTOR;
		case 180:
			return Item::QUICK_CLAW;
		case 181:
			return Item::RAZOR_CLAW;
		case 182:
			return Item::SHINY_STONE;
		case 183:
			return Item::STICKY_BARB;
		case 184:
			return Item::DEEPSEATOOTH;
		case 185:
			return Item::DRACO_PLATE;
		case 186:
			return Item::DREAD_PLATE;
		case 187:
			return Item::EARTH_PLATE;
		case 188:
			return Item::FIST_PLATE;
		case 189:
			return Item::FLAME_PLATE;
		case 190:
			return Item::GRIP_CLAW;
		case 191:
			return Item::ICICLE_PLATE;
		case 192:
			return Item::INSECT_PLATE;
		case 193:
			return Item::IRON_PLATE;
		case 194:
			return Item::MEADOW_PLATE;
		case 195:
			return Item::MIND_PLATE;
		case 196:
			return Item::SKY_PLATE;
		case 197:
			return Item::SPLASH_PLATE;
		case 198:
			return Item::SPOOKY_PLATE;
		case 199:
			return Item::STONE_PLATE;
		case 200:
			return Item::THICK_CLUB;
		case 201:
			return Item::TOXIC_PLATE;
		case 202:
			return Item::ZAP_PLATE;
		case 203:
			return Item::ARMOR_FOSSIL;
		case 204:
			return Item::CLAW_FOSSIL;
		case 205:
			return Item::DOME_FOSSIL;
		case 206:
			return Item::HARD_STONE;
		case 207:
			return Item::HELIX_FOSSIL;
		case 208:
			return Item::OLD_AMBER;
		case 209:
			return Item::RARE_BONE;
		case 210:
			return Item::ROOT_FOSSIL;
		case 211:
			return Item::SKULL_FOSSIL;
		case 212:
			return Item::IRON_BALL;
		case 213:
			return Item::GRISEOUS_ORB;
		case 214:
			return Item::AIR_MAIL;
		case 215:
			return Item::BLOOM_MAIL;
		case 216:
			return Item::BRICK_MAIL;
		case 217:
			return Item::BUBBLE_MAIL;
		case 218:
			return Item::FLAME_MAIL;
		case 219:
			return Item::GRASS_MAIL;
		// case 220 intentionally missing
		case 221:
			return Item::HEART_MAIL;
		case 222:
			return Item::MOSAIC_MAIL;
		case 223:
			return Item::SNOW_MAIL;
		case 224:
			return Item::SPACE_MAIL;
		case 225:
			return Item::STEEL_MAIL;
		case 226:
			return Item::TUNNEL_MAIL;
		default:
			return Item::END;
	}
}

unsigned item_to_id (Item::Items item) {
	switch (item) {
		case Item::NO_ITEM:
			return 0;
		case Item::BIG_ROOT:
			return 1;
		case Item::BLUE_SCARF:
			return 2;
		case Item::BRIGHTPOWDER:
			return 3;
		case Item::CHOICE_BAND:
			return 4;
		case Item::CHOICE_SCARF:
			return 5;
		case Item::CHOICE_SPECS:
			return 6;
		case Item::DESTINY_KNOT:
			return 7;
		case Item::EXPERT_BELT:
			return 8;
		case Item::FOCUS_BAND:
			return 9;
		case Item::FOCUS_SASH:
			return 10;
		case Item::FULL_INCENSE:
			return 11;
		case Item::GREEN_SCARF:
			return 12;
		case Item::LAGGING_TAIL:
			return 13;
		case Item::LAX_INCENSE:
			return 14;
		case Item::LEFTOVERS:
			return 15;
		case Item::LUCK_INCENSE:
			return 16;
		case Item::MENTAL_HERB:
			return 17;
		case Item::METAL_POWDER:
			return 18;
		case Item::MUSCLE_BAND:
			return 19;
		case Item::ODD_INCENSE:
			return 20;
		case Item::PINK_SCARF:
			return 21;
		case Item::POWER_HERB:
			return 22;
		case Item::PURE_INCENSE:
			return 23;
		case Item::QUICK_POWDER:
			return 24;
		case Item::REAPER_CLOTH:
			return 25;
		case Item::RED_SCARF:
			return 26;
		case Item::ROCK_INCENSE:
			return 27;
		case Item::ROSE_INCENSE:
			return 28;
		case Item::SEA_INCENSE:
			return 29;
		case Item::SHED_SHELL:
			return 30;
		case Item::SILK_SCARF:
			return 31;
		case Item::SILVERPOWDER:
			return 32;
		case Item::SMOOTH_ROCK:
			return 33;
		case Item::SOFT_SAND:
			return 34;
		case Item::SOOTHE_BELL:
			return 35;
		case Item::WAVE_INCENSE:
			return 36;
		case Item::WHITE_HERB:
			return 37;
		case Item::WIDE_LENS:
			return 38;
		case Item::WISE_GLASSES:
			return 39;
		case Item::YELLOW_SCARF:
			return 40;
		case Item::ZOOM_LENS:
			return 41;
		case Item::AMULET_COIN:
			return 42;
		case Item::ANTIDOTE:
			return 43;
		case Item::AWAKENING:
			return 44;
		case Item::BERRY_JUICE:
			return 45;
		case Item::BIG_PEARL:
			return 46;
		case Item::BIG_MUSHROOM:
			return 47;
		case Item::BLACK_BELT:
			return 48;
		case Item::BLACK_FLUTE:
			return 49;
		case Item::BLACK_SLUDGE:
			return 50;
		case Item::BLACKGLASSES:
			return 51;
		case Item::BLUE_FLUTE:
			return 52;
		case Item::BLUE_SHARD:
			return 53;
		case Item::BURN_HEAL:
			return 54;
		case Item::CALCIUM:
			return 55;
		case Item::CARBOS:
			return 56;
		case Item::CHARCOAL:
			return 57;
		case Item::CLEANSE_TAG:
			return 58;
		case Item::DAMP_MULCH:
			return 59;
		case Item::DEEPSEASCALE:
			return 60;
		case Item::DIRE_HIT:
			return 61;
		case Item::DRAGON_SCALE:
			return 62;
		case Item::ELIXIR:
			return 63;
		case Item::ENERGY_ROOT:
			return 64;
		case Item::ENERGYPOWDER:
			return 65;
		case Item::ESCAPE_ROPE:
			return 66;
		case Item::ETHER:
			return 67;
		case Item::EVERSTONE:
			return 68;
		case Item::EXP_SHARE:
			return 69;
		case Item::FIRE_STONE:
			return 70;
		case Item::FLAME_ORB:
			return 71;
		case Item::FLUFFY_TAIL:
			return 72;
		case Item::FRESH_WATER:
			return 73;
		case Item::FULL_HEAL:
			return 74;
		case Item::FULL_RESTORE:
			return 75;
		case Item::GOOEY_MULCH:
			return 76;
		case Item::GREEN_SHARD:
			return 77;
		case Item::GROWTH_MULCH:
			return 78;
		case Item::GUARD_SPEC:
			return 79;
		case Item::HEAL_POWDER:
			return 80;
		case Item::HEART_SCALE:
			return 81;
		case Item::HONEY:
			return 82;
		case Item::HP_UP:
			return 83;
		case Item::HYPER_POTION:
			return 84;
		case Item::ICE_HEAL:
			return 85;
		case Item::IRON:
			return 86;
		case Item::KINGS_ROCK:
			return 87;
		case Item::LAVA_COOKIE:
			return 88;
		case Item::LEAF_STONE:
			return 89;
		case Item::LEMONADE:
			return 90;
		case Item::LIFE_ORB:
			return 91;
		case Item::LIGHT_BALL:
			return 92;
		case Item::LIGHT_CLAY:
			return 93;
		case Item::LUCKY_EGG:
			return 94;
		case Item::MAGNET:
			return 95;
		case Item::MAX_ELIXIR:
			return 96;
		case Item::MAX_ETHER:
			return 97;
		case Item::MAX_POTION:
			return 98;
		case Item::MAX_REPEL:
			return 99;
		case Item::MAX_REVIVE:
			return 100;
		case Item::METAL_COAT:
			return 101;
		case Item::METRONOME:
			return 102;
		case Item::MIRACLE_SEED:
			return 103;
		case Item::MOOMOO_MILK:
			return 104;
		case Item::MOON_STONE:
			return 105;
		case Item::MYSTIC_WATER:
			return 106;
		case Item::NEVERMELTICE:
			return 107;
		case Item::NUGGET:
			return 108;
		case Item::OLD_GATEAU:
			return 109;
		case Item::PARLYZ_HEAL:
			return 110;
		case Item::PEARL:
			return 111;
		case Item::POKE_DOLL:
			return 112;
		case Item::POTION:
			return 113;
		case Item::PP_MAX:
			return 114;
		case Item::PP_UP:
			return 115;
		case Item::PROTEIN:
			return 116;
		case Item::RARE_CANDY:
			return 117;
		case Item::RAZOR_FANG:
			return 118;
		case Item::RED_FLUTE:
			return 119;
		case Item::RED_SHARD:
			return 120;
		case Item::REPEL:
			return 121;
		case Item::REVIVAL_HERB:
			return 122;
		case Item::REVIVE:
			return 123;
		case Item::SACRED_ASH:
			return 124;
		case Item::SCOPE_LENS:
			return 125;
		case Item::SHELL_BELL:
			return 126;
		case Item::SHOAL_SALT:
			return 127;
		case Item::SHOAL_SHELL:
			return 128;
		case Item::SMOKE_BALL:
			return 129;
		case Item::SODA_POP:
			return 130;
		case Item::SOUL_DEW:
			return 131;
		case Item::SPELL_TAG:
			return 132;
		case Item::STABLE_MULCH:
			return 133;
		case Item::STAR_PIECE:
			return 134;
		case Item::STARDUST:
			return 135;
		case Item::SUN_STONE:
			return 136;
		case Item::SUPER_POTION:
			return 137;
		case Item::SUPER_REPEL:
			return 138;
		case Item::THUNDERSTONE:
			return 139;
		case Item::TINYMUSHROOM:
			return 140;
		case Item::TOXIC_ORB:
			return 141;
		case Item::TWISTEDSPOON:
			return 142;
		case Item::UP_GRADE:
			return 143;
		case Item::WATER_STONE:
			return 144;
		case Item::WHITE_FLUTE:
			return 145;
		case Item::X_ACCURACY:
			return 146;
		case Item::X_ATTACK:
			return 147;
		case Item::X_DEFEND:
			return 148;
		case Item::X_SPECIAL:
			return 149;
		case Item::X_SP_DEF:
			return 150;
		case Item::X_SPEED:
			return 151;
		case Item::YELLOW_FLUTE:
			return 152;
		case Item::YELLOW_SHARD:
			return 153;
		case Item::ZINC:
			return 154;
		case Item::ICY_ROCK:
			return 155;
		case Item::LUCKY_PUNCH:
			return 156;
		case Item::DUBIOUS_DISC:
			return 157;
		case Item::SHARP_BEAK:
			return 158;
		case Item::ADAMANT_ORB:
			return 159;
		case Item::DAMP_ROCK:
			return 160;
		case Item::HEAT_ROCK:
			return 161;
		case Item::LUSTROUS_ORB:
			return 162;
		case Item::MACHO_BRACE:
			return 163;
		case Item::STICK:
			return 164;
		case Item::DRAGON_FANG:
			return 165;
		case Item::POISON_BARB:
			return 166;
		case Item::POWER_ANKLET:
			return 167;
		case Item::POWER_BAND:
			return 168;
		case Item::POWER_BELT:
			return 169;
		case Item::POWER_BRACER:
			return 170;
		case Item::POWER_LENS:
			return 171;
		case Item::POWER_WEIGHT:
			return 172;
		case Item::DAWN_STONE:
			return 173;
		case Item::DUSK_STONE:
			return 174;
		case Item::ELECTIRIZER:
			return 175;
		case Item::MAGMARIZER:
			return 176;
		case Item::ODD_KEYSTONE:
			return 177;
		case Item::OVAL_STONE:
			return 178;
		case Item::PROTECTOR:
			return 179;
		case Item::QUICK_CLAW:
			return 180;
		case Item::RAZOR_CLAW:
			return 181;
		case Item::SHINY_STONE:
			return 182;
		case Item::STICKY_BARB:
			return 183;
		case Item::DEEPSEATOOTH:
			return 184;
		case Item::DRACO_PLATE:
			return 185;
		case Item::DREAD_PLATE:
			return 186;
		case Item::EARTH_PLATE:
			return 187;
		case Item::FIST_PLATE:
			return 188;
		case Item::FLAME_PLATE:
			return 189;
		case Item::GRIP_CLAW:
			return 190;
		case Item::ICICLE_PLATE:
			return 191;
		case Item::INSECT_PLATE:
			return 192;
		case Item::IRON_PLATE:
			return 193;
		case Item::MEADOW_PLATE:
			return 194;
		case Item::MIND_PLATE:
			return 195;
		case Item::SKY_PLATE:
			return 196;
		case Item::SPLASH_PLATE:
			return 197;
		case Item::SPOOKY_PLATE:
			return 198;
		case Item::STONE_PLATE:
			return 199;
		case Item::THICK_CLUB:
			return 200;
		case Item::TOXIC_PLATE:
			return 201;
		case Item::ZAP_PLATE:
			return 202;
		case Item::ARMOR_FOSSIL:
			return 203;
		case Item::CLAW_FOSSIL:
			return 204;
		case Item::DOME_FOSSIL:
			return 205;
		case Item::HARD_STONE:
			return 206;
		case Item::HELIX_FOSSIL:
			return 207;
		case Item::OLD_AMBER:
			return 208;
		case Item::RARE_BONE:
			return 209;
		case Item::ROOT_FOSSIL:
			return 210;
		case Item::SKULL_FOSSIL:
			return 211;
		case Item::IRON_BALL:
			return 212;
		case Item::GRISEOUS_ORB:
			return 213;
		case Item::AIR_MAIL:
			return 214;
		case Item::BLOOM_MAIL:
			return 215;
		case Item::BRICK_MAIL:
			return 216;
		case Item::BUBBLE_MAIL:
			return 217;
		case Item::FLAME_MAIL:
			return 218;
		case Item::GRASS_MAIL:
			return 219;
		// Intentionally missing 220
		case Item::HEART_MAIL:
			return 221;
		case Item::MOSAIC_MAIL:
			return 222;
		case Item::SNOW_MAIL:
			return 223;
		case Item::SPACE_MAIL:
			return 224;
		case Item::STEEL_MAIL:
			return 225;
		case Item::TUNNEL_MAIL:
			return 226;
		default:
			return 0;
			/*
			AGUAV_BERRY
			APICOT_BERRY
			ASPEAR_BERRY
			BABIRI_BERRY
			BELUE_BERRY
			BICYCLE
			BLUK_BERRY
			CHARTI_BERRY
			CHERI_BERRY
			CHERISH_BALL
			CHESTO_BERRY
			CHILAN_BERRY
			CHOPLE_BERRY
			COBA_BERRY
			COIN_CASE
			COLBUR_BERRY
			CORNN_BERRY
			COUPON_1
			COUPON_2
			COUPON_3
			CUSTAP_BERRY
			DIVE_BALL
			DURIN_BERRY
			DUSK_BALL
			ENIGMA_BERRY
			EXPLORER_KIT
			FASHION_CASE
			FIGY_BERRY
			GALACTIC_KEY
			GANLON_BERRY
			GOOD_ROD
			GREAT_BALL
			GREPA_BERRY
			HABAN_BERRY
			HEAL_BALL
			HM01
			HM02
			HM03
			HM04
			HM05
			HM06
			HM07
			HM08
			HONDEW_BERRY
			IAPAPA_BERRY
			JABOCA_BERRY
			JOURNAL
			KASIB_BERRY
			KEBIA_BERRY
			KELPSY_BERRY
			LANSAT_BERRY
			LEPPA_BERRY
			LIECHI_BERRY
			LOOT_SACK
			LUM_BERRY
			LUNAR_WING
			LUXURY_BALL
			MAGO_BERRY
			MAGOST_BERRY
			MASTER_BALL
			MEMBER_CARD
			MICLE_BERRY
			NANAB_BERRY
			NEST_BALL
			NET_BALL
			NOMEL_BERRY
			OAKS_LETTER
			OCCA_BERRY
			OLD_CHARM
			OLD_ROD
			ORAN_BERRY
			PAL_PAD
			PAMTRE_BERRY
			PARCEL
			PASSHO_BERRY
			PAYAPA_BERRY
			PECHA_BERRY
			PERSIM_BERRY
			PETAYA_BERRY
			PINAP_BERRY
			POFFIN_CASE
			POINT_CARD
			POKE_BALL
			POKE_RADAR
			POMEG_BERRY
			PREMIER_BALL
			QUALOT_BERRY
			QUICK_BALL
			RABUTA_BERRY
			RAWST_BERRY
			RAZZ_BERRY
			REPEAT_BALL
			RINDO_BERRY
			ROWAP_BERRY
			SAFARI_BALL
			SALAC_BERRY
			SEAL_BAG
			SEAL_CASE
			SECRETPOTION
			SHUCA_BERRY
			SITRUS_BERRY
			SPELON_BERRY
			SPRAYDUCK
			STARF_BERRY
			STORAGE_KEY
			SUITE_KEY
			SUPER_ROD
			TAMATO_BERRY
			TANGA_BERRY
			TIMER_BALL
			TM01
			TM02
			TM03
			TM04
			TM05
			TM06
			TM07
			TM08
			TM09
			TM10
			TM11
			TM12
			TM13
			TM14
			TM15
			TM16
			TM17
			TM18
			TM19
			TM20
			TM21
			TM22
			TM23
			TM24
			TM25
			TM26
			TM27
			TM28
			TM29
			TM30
			TM31
			TM32
			TM33
			TM34
			TM35
			TM36
			TM37
			TM38
			TM39
			TM40
			TM41
			TM42
			TM43
			TM44
			TM45
			TM46
			TM47
			TM48
			TM49
			TM50
			TM51
			TM52
			TM53
			TM54
			TM55
			TM56
			TM57
			TM58
			TM59
			TM60
			TM61
			TM62
			TM63
			TM64
			TM65
			TM66
			TM67
			TM68
			TM69
			TM70
			TM71
			TM72
			TM73
			TM74
			TM75
			TM76
			TM77
			TM78
			TM79
			TM80
			TM81
			TM82
			TM83
			TM84
			TM85
			TM86
			TM87
			TM88
			TM89
			TM90
			TM91
			TM92
			TOWN_MAP
			ULTRA_BALL
			VS_SEEKER
			WACAN_BERRY
			WATMEL_BERRY
			WEPEAR_BERRY
			WIKI_BERRY
			WORKS_KEY
			YACHE_BERRY
			*/
	}
}

Item::Items battle_id_to_item (uint16_t id, uint8_t part) {
	switch (id) {
		case 3:
			return Item::WHITE_HERB;
		case 4:
			return Item::FOCUS_BAND;
		case 5:
			return Item::FOCUS_SASH;
		case 7:
			return Item::MENTAL_HERB;
		case 11:
			return Item::POWER_HERB;
		case 12:
			return Item::LEFTOVERS;
		case 16:
			return Item::BLACK_SLUDGE;
		case 17:
			return Item::QUICK_CLAW;
		case 18:
			return Item::BERRY_JUICE;
		case 19:
			return (part == 0) ? Item::FLAME_ORB : Item::TOXIC_ORB;
		case 21:
			return Item::LIFE_ORB;
		case 24:
			return Item::SHELL_BELL;
		case 29:
			return Item::STICKY_BARB;
		default:
			return Item::END;
	}
}

Move::Moves id_to_move (unsigned id) {
	constexpr static Move::Moves move_converter [] = {
		Move::END, Move::POUND, Move::KARATE_CHOP,
		Move::DOUBLESLAP, Move::COMET_PUNCH, Move::MEGA_PUNCH,
		Move::PAY_DAY, Move::FIRE_PUNCH, Move::ICE_PUNCH,
		Move::THUNDERPUNCH, Move::SCRATCH, Move::VICEGRIP,
		Move::GUILLOTINE, Move::RAZOR_WIND, Move::SWORDS_DANCE,
		Move::CUT, Move::GUST, Move::WING_ATTACK,
		Move::WHIRLWIND, Move::FLY, Move::BIND,
		Move::SLAM, Move::VINE_WHIP, Move::STOMP,
		Move::DOUBLE_KICK, Move::MEGA_KICK, Move::JUMP_KICK,
		Move::ROLLING_KICK, Move::SAND_ATTACK, Move::HEADBUTT,
		Move::HORN_ATTACK, Move::FURY_ATTACK, Move::HORN_DRILL,
		Move::TACKLE, Move::BODY_SLAM, Move::WRAP,
		Move::TAKE_DOWN, Move::THRASH, Move::DOUBLE_EDGE,
		Move::TAIL_WHIP, Move::POISON_STING, Move::TWINEEDLE,
		Move::PIN_MISSILE, Move::LEER, Move::BITE,
		Move::GROWL, Move::ROAR, Move::SING,
		Move::SUPERSONIC, Move::SONICBOOM, Move::DISABLE,
		Move::ACID, Move::EMBER, Move::FLAMETHROWER,
		Move::MIST, Move::WATER_GUN, Move::HYDRO_PUMP,
		Move::SURF, Move::ICE_BEAM, Move::BLIZZARD,
		Move::PSYBEAM, Move::BUBBLEBEAM, Move::AURORA_BEAM,
		Move::HYPER_BEAM, Move::PECK, Move::DRILL_PECK,
		Move::SUBMISSION, Move::LOW_KICK, Move::COUNTER,
		Move::SEISMIC_TOSS, Move::STRENGTH, Move::ABSORB,
		Move::MEGA_DRAIN, Move::LEECH_SEED, Move::GROWTH,
		Move::RAZOR_LEAF, Move::SOLARBEAM, Move::POISONPOWDER,
		Move::STUN_SPORE, Move::SLEEP_POWDER, Move::PETAL_DANCE,
		Move::STRING_SHOT, Move::DRAGON_RAGE, Move::FIRE_SPIN,
		Move::THUNDERSHOCK, Move::THUNDERBOLT, Move::THUNDER_WAVE,
		Move::THUNDER, Move::ROCK_THROW, Move::EARTHQUAKE,
		Move::FISSURE, Move::DIG, Move::TOXIC,
		Move::CONFUSION, Move::PSYCHIC, Move::HYPNOSIS,
		Move::MEDITATE, Move::AGILITY, Move::QUICK_ATTACK,
		Move::RAGE, Move::TELEPORT, Move::NIGHT_SHADE,
		Move::MIMIC, Move::SCREECH, Move::DOUBLE_TEAM,
		Move::RECOVER, Move::HARDEN, Move::MINIMIZE,
		Move::SMOKESCREEN, Move::CONFUSE_RAY, Move::WITHDRAW,
		Move::DEFENSE_CURL, Move::BARRIER, Move::LIGHT_SCREEN,
		Move::HAZE, Move::REFLECT, Move::FOCUS_ENERGY,
		Move::BIDE, Move::METRONOME, Move::MIRROR_MOVE,
		Move::SELFDESTRUCT, Move::EGG_BOMB, Move::LICK,
		Move::SMOG, Move::SLUDGE, Move::BONE_CLUB,
		Move::FIRE_BLAST, Move::WATERFALL, Move::CLAMP,
		Move::SWIFT, Move::SKULL_BASH, Move::SPIKE_CANNON,
		Move::CONSTRICT, Move::AMNESIA, Move::KINESIS,
		Move::SOFTBOILED, Move::HI_JUMP_KICK, Move::GLARE,
		Move::DREAM_EATER, Move::POISON_GAS, Move::BARRAGE,
		Move::LEECH_LIFE, Move::LOVELY_KISS, Move::SKY_ATTACK,
		Move::TRANSFORM, Move::BUBBLE, Move::DIZZY_PUNCH,
		Move::SPORE, Move::FLASH, Move::PSYWAVE,
		Move::SPLASH, Move::ACID_ARMOR, Move::CRABHAMMER,
		Move::EXPLOSION, Move::FURY_SWIPES, Move::BONEMERANG,
		Move::REST, Move::ROCK_SLIDE, Move::HYPER_FANG,
		Move::SHARPEN, Move::CONVERSION, Move::TRI_ATTACK,
		Move::SUPER_FANG, Move::SLASH, Move::SUBSTITUTE,
		Move::STRUGGLE, Move::SKETCH, Move::TRIPLE_KICK,
		Move::THIEF, Move::SPIDER_WEB, Move::MIND_READER,
		Move::NIGHTMARE, Move::FLAME_WHEEL, Move::SNORE,
		Move::CURSE, Move::FLAIL, Move::CONVERSION2,
		Move::AEROBLAST, Move::COTTON_SPORE, Move::REVERSAL,
		Move::SPITE, Move::POWDER_SNOW, Move::PROTECT,
		Move::MACH_PUNCH, Move::SCARY_FACE, Move::FAINT_ATTACK,
		Move::SWEET_KISS, Move::BELLY_DRUM, Move::SLUDGE_BOMB,
		Move::MUD_SLAP, Move::OCTAZOOKA, Move::SPIKES,
		Move::ZAP_CANNON, Move::FORESIGHT, Move::DESTINY_BOND,
		Move::PERISH_SONG, Move::ICY_WIND, Move::DETECT,
		Move::BONE_RUSH, Move::LOCK_ON, Move::OUTRAGE,
		Move::SANDSTORM, Move::GIGA_DRAIN, Move::ENDURE,
		Move::CHARM, Move::ROLLOUT, Move::FALSE_SWIPE,
		Move::SWAGGER, Move::MILK_DRINK, Move::SPARK,
		Move::FURY_CUTTER, Move::STEEL_WING, Move::MEAN_LOOK,
		Move::ATTRACT, Move::SLEEP_TALK, Move::HEAL_BELL,
		Move::RETURN, Move::PRESENT, Move::FRUSTRATION,
		Move::SAFEGUARD, Move::PAIN_SPLIT, Move::SACRED_FIRE,
		Move::MAGNITUDE, Move::DYNAMICPUNCH, Move::MEGAHORN,
		Move::DRAGONBREATH, Move::BATON_PASS, Move::ENCORE,
		Move::PURSUIT, Move::RAPID_SPIN, Move::SWEET_SCENT,
		Move::IRON_TAIL, Move::METAL_CLAW, Move::VITAL_THROW,
		Move::MORNING_SUN, Move::SYNTHESIS, Move::MOONLIGHT,
		Move::HIDDEN_POWER, Move::CROSS_CHOP, Move::TWISTER,
		Move::RAIN_DANCE, Move::SUNNY_DAY, Move::CRUNCH,
		Move::MIRROR_COAT, Move::PSYCH_UP, Move::EXTREMESPEED,
		Move::ANCIENTPOWER, Move::SHADOW_BALL, Move::FUTURE_SIGHT,
		Move::ROCK_SMASH, Move::WHIRLPOOL, Move::BEAT_UP,
		Move::FAKE_OUT, Move::UPROAR, Move::STOCKPILE,
		Move::SPIT_UP, Move::SWALLOW, Move::HEAT_WAVE,
		Move::HAIL, Move::TORMENT, Move::FLATTER,
		Move::WILL_O_WISP, Move::MEMENTO, Move::FACADE,
		Move::FOCUS_PUNCH, Move::SMELLINGSALT, Move::FOLLOW_ME,
		Move::NATURE_POWER, Move::CHARGE, Move::TAUNT,
		Move::HELPING_HAND, Move::TRICK, Move::ROLE_PLAY,
		Move::WISH, Move::ASSIST, Move::INGRAIN,
		Move::SUPERPOWER, Move::MAGIC_COAT, Move::RECYCLE,
		Move::REVENGE, Move::BRICK_BREAK, Move::YAWN,
		Move::KNOCK_OFF, Move::ENDEAVOR, Move::ERUPTION,
		Move::SKILL_SWAP, Move::IMPRISON, Move::REFRESH,
		Move::GRUDGE, Move::SNATCH, Move::SECRET_POWER,
		Move::DIVE, Move::ARM_THRUST, Move::CAMOUFLAGE,
		Move::TAIL_GLOW, Move::LUSTER_PURGE, Move::MIST_BALL,
		Move::FEATHERDANCE, Move::TEETER_DANCE, Move::BLAZE_KICK,
		Move::MUD_SPORT, Move::ICE_BALL, Move::NEEDLE_ARM,
		Move::SLACK_OFF, Move::HYPER_VOICE, Move::POISON_FANG,
		Move::CRUSH_CLAW, Move::BLAST_BURN, Move::HYDRO_CANNON,
		Move::METEOR_MASH, Move::ASTONISH, Move::WEATHER_BALL,
		Move::AROMATHERAPY, Move::FAKE_TEARS, Move::AIR_CUTTER,
		Move::OVERHEAT, Move::ODOR_SLEUTH, Move::ROCK_TOMB,
		Move::SILVER_WIND, Move::METAL_SOUND, Move::GRASSWHISTLE,
		Move::TICKLE, Move::COSMIC_POWER, Move::WATER_SPOUT,
		Move::SIGNAL_BEAM, Move::SHADOW_PUNCH, Move::EXTRASENSORY,
		Move::SKY_UPPERCUT, Move::SAND_TOMB, Move::SHEER_COLD,
		Move::MUDDY_WATER, Move::BULLET_SEED, Move::AERIAL_ACE,
		Move::ICICLE_SPEAR, Move::IRON_DEFENSE, Move::BLOCK,
		Move::HOWL, Move::DRAGON_CLAW, Move::FRENZY_PLANT,
		Move::BULK_UP, Move::BOUNCE, Move::MUD_SHOT,
		Move::POISON_TAIL, Move::COVET, Move::VOLT_TACKLE,
		Move::MAGICAL_LEAF, Move::WATER_SPORT, Move::CALM_MIND,
		Move::LEAF_BLADE, Move::DRAGON_DANCE, Move::ROCK_BLAST,
		Move::SHOCK_WAVE, Move::WATER_PULSE, Move::DOOM_DESIRE,
		Move::PSYCHO_BOOST, Move::ROOST, Move::GRAVITY,
		Move::MIRACLE_EYE, Move::WAKE_UP_SLAP, Move::HAMMER_ARM,
		Move::GYRO_BALL, Move::HEALING_WISH, Move::BRINE,
		Move::NATURAL_GIFT, Move::FEINT, Move::PLUCK,
		Move::TAILWIND, Move::ACUPRESSURE, Move::METAL_BURST,
		Move::U_TURN, Move::CLOSE_COMBAT, Move::PAYBACK,
		Move::ASSURANCE, Move::EMBARGO, Move::FLING,
		Move::PSYCHO_SHIFT, Move::TRUMP_CARD, Move::HEAL_BLOCK,
		Move::WRING_OUT, Move::POWER_TRICK, Move::GASTRO_ACID,
		Move::LUCKY_CHANT, Move::ME_FIRST, Move::COPYCAT,
		Move::POWER_SWAP, Move::GUARD_SWAP, Move::PUNISHMENT,
		Move::LAST_RESORT, Move::WORRY_SEED, Move::SUCKER_PUNCH,
		Move::TOXIC_SPIKES, Move::HEART_SWAP, Move::AQUA_RING,
		Move::MAGNET_RISE, Move::FLARE_BLITZ, Move::FORCE_PALM,
		Move::AURA_SPHERE, Move::ROCK_POLISH, Move::POISON_JAB,
		Move::DARK_PULSE, Move::NIGHT_SLASH, Move::AQUA_TAIL,
		Move::SEED_BOMB, Move::AIR_SLASH, Move::X_SCISSOR,
		Move::BUG_BUZZ, Move::DRAGON_PULSE, Move::DRAGON_RUSH,
		Move::POWER_GEM, Move::DRAIN_PUNCH, Move::VACUUM_WAVE,
		Move::FOCUS_BLAST, Move::ENERGY_BALL, Move::BRAVE_BIRD,
		Move::EARTH_POWER, Move::SWITCHEROO, Move::GIGA_IMPACT,
		Move::NASTY_PLOT, Move::BULLET_PUNCH, Move::AVALANCHE,
		Move::ICE_SHARD, Move::SHADOW_CLAW, Move::THUNDER_FANG,
		Move::ICE_FANG, Move::FIRE_FANG, Move::SHADOW_SNEAK,
		Move::MUD_BOMB, Move::PSYCHO_CUT, Move::ZEN_HEADBUTT,
		Move::MIRROR_SHOT, Move::FLASH_CANNON, Move::ROCK_CLIMB,
		Move::DEFOG, Move::TRICK_ROOM, Move::DRACO_METEOR,
		Move::DISCHARGE, Move::LAVA_PLUME, Move::LEAF_STORM,
		Move::POWER_WHIP, Move::ROCK_WRECKER, Move::CROSS_POISON,
		Move::GUNK_SHOT, Move::IRON_HEAD, Move::MAGNET_BOMB,
		Move::STONE_EDGE, Move::CAPTIVATE, Move::STEALTH_ROCK,
		Move::GRASS_KNOT, Move::CHATTER, Move::JUDGMENT,
		Move::BUG_BITE, Move::CHARGE_BEAM, Move::WOOD_HAMMER,
		Move::AQUA_JET, Move::ATTACK_ORDER, Move::DEFEND_ORDER,
		Move::HEAL_ORDER, Move::HEAD_SMASH, Move::DOUBLE_HIT,
		Move::ROAR_OF_TIME, Move::SPACIAL_REND, Move::LUNAR_DANCE,
		Move::CRUSH_GRIP, Move::MAGMA_STORM, Move::DARK_VOID,
		Move::SEED_FLARE, Move::OMINOUS_WIND, Move::SHADOW_FORCE
	};
	return move_converter [id];
}

unsigned move_to_id (Move::Moves move) {
	constexpr static unsigned move_converter [] = {
		71,		// ABSORB
		51,		// ACID
		151,		// ACID_ARMOR
		367,		// ACUPRESSURE
		332,		// AERIAL_ACE
		177,		// AEROBLAST
		97,		// AGILITY
		314,		// AIR_CUTTER
		403,		// AIR_SLASH
		133,		// AMNESIA
		246,		// ANCIENTPOWER
		453,		// AQUA_JET
		392,		// AQUA_RING
		401,		// AQUA_TAIL
		292,		// ARM_THRUST
		312,		// AROMATHERAPY
		274,		// ASSIST
		372,		// ASSURANCE
		310,		// ASTONISH
		454,		// ATTACK_ORDER
		213,		// ATTRACT
		396,		// AURA_SPHERE
		62,		// AURORA_BEAM
		419,		// AVALANCHE
		140,		// BARRAGE
		112,		// BARRIER
		226,		// BATON_PASS
		251,		// BEAT_UP
		187,		// BELLY_DRUM
		117,		// BIDE
		20,		// BIND
		44,		// BITE
		307,		// BLAST_BURN
		299,		// BLAZE_KICK
		59,		// BLIZZARD
		335,		// BLOCK
		34,		// BODY_SLAM
		125,		// BONE_CLUB
		198,		// BONE_RUSH
		155,		// BONEMERANG
		340,		// BOUNCE
		413,		// BRAVE_BIRD
		280,		// BRICK_BREAK
		362,		// BRINE
		145,		// BUBBLE
		61,		// BUBBLEBEAM
		450,		// BUG_BITE
		405,		// BUG_BUZZ
		339,		// BULK_UP
		418,		// BULLET_PUNCH
		331,		// BULLET_SEED
		347,		// CALM_MIND
		293,		// CAMOUFLAGE
		445,		// CAPTIVATE
		268,		// CHARGE
		451,		// CHARGE_BEAM
		204,		// CHARM
		448,		// CHATTER
		128,		// CLAMP
		370,		// CLOSE_COMBAT
		4,		// COMET_PUNCH
		109,		// CONFUSE_RAY
		93,		// CONFUSION
		132,		// CONSTRICT
		160,		// CONVERSION
		176,		// CONVERSION2
		383,		// COPYCAT
		322,		// COSMIC_POWER
		178,		// COTTON_SPORE
		68,		// COUNTER
		343,		// COVET
		152,		// CRABHAMMER
		238,		// CROSS_CHOP
		440,		// CROSS_POISON
		242,		// CRUNCH
		306,		// CRUSH_CLAW
		462,		// CRUSH_GRIP
		174,		// CURSE
		15,		// CUT
		399,		// DARK_PULSE
		464,		// DARK_VOID
		455,		// DEFEND_ORDER
		111,		// DEFENSE_CURL
		432,		// DEFOG
		194,		// DESTINY_BOND
		197,		// DETECT
		91,		// DIG
		50,		// DISABLE
		435,		// DISCHARGE
		291,		// DIVE
		146,		// DIZZY_PUNCH
		353,		// DOOM_DESIRE
		458,		// DOUBLE_HIT
		24,		// DOUBLE_KICK
		104,		// DOUBLE_TEAM
		38,		// DOUBLE_EDGE
		3,		// DOUBLESLAP
		434,		// DRACO_METEOR
		337,		// DRAGON_CLAW
		349,		// DRAGON_DANCE
		406,		// DRAGON_PULSE
		82,		// DRAGON_RAGE
		407,		// DRAGON_RUSH
		225,		// DRAGONBREATH
		409,		// DRAIN_PUNCH
		138,		// DREAM_EATER
		65,		// DRILL_PECK
		223,		// DYNAMICPUNCH
		414,		// EARTH_POWER
		89,		// EARTHQUAKE
		121,		// EGG_BOMB
		373,		// EMBARGO
		52,		// EMBER
		227,		// ENCORE
		283,		// ENDEAVOR
		203,		// ENDURE
		412,		// ENERGY_BALL
		284,		// ERUPTION
		153,		// EXPLOSION
		326,		// EXTRASENSORY
		245,		// EXTREMESPEED
		263,		// FACADE
		185,		// FAINT_ATTACK
		252,		// FAKE_OUT
		313,		// FAKE_TEARS
		206,		// FALSE_SWIPE
		297,		// FEATHERDANCE
		364,		// FEINT
		126,		// FIRE_BLAST
		424,		// FIRE_FANG
		7,		// FIRE_PUNCH
		83,		// FIRE_SPIN
		90,		// FISSURE
		175,		// FLAIL
		172,		// FLAME_WHEEL
		53,		// FLAMETHROWER
		394,		// FLARE_BLITZ
		148,		// FLASH
		430,		// FLASH_CANNON
		260,		// FLATTER
		374,		// FLING
		19,		// FLY
		411,		// FOCUS_BLAST
		116,		// FOCUS_ENERGY
		264,		// FOCUS_PUNCH
		266,		// FOLLOW_ME
		395,		// FORCE_PALM
		193,		// FORESIGHT
		338,		// FRENZY_PLANT
		218,		// FRUSTRATION
		31,		// FURY_ATTACK
		210,		// FURY_CUTTER
		154,		// FURY_SWIPES
		248,		// FUTURE_SIGHT
		380,		// GASTRO_ACID
		202,		// GIGA_DRAIN
		416,		// GIGA_IMPACT
		137,		// GLARE
		447,		// GRASS_KNOT
		320,		// GRASSWHISTLE
		356,		// GRAVITY
		45,		// GROWL
		74,		// GROWTH
		288,		// GRUDGE
		385,		// GUARD_SWAP
		12,		// GUILLOTINE
		441,		// GUNK_SHOT
		16,		// GUST
		360,		// GYRO_BALL
		258,		// HAIL
		359,		// HAMMER_ARM
		106,		// HARDEN
		114,		// HAZE
		457,		// HEAD_SMASH
		29,		// HEADBUTT
		215,		// HEAL_BELL
		377,		// HEAL_BLOCK
		456,		// HEAL_ORDER
		361,		// HEALING_WISH
		391,		// HEART_SWAP
		257,		// HEAT_WAVE
		270,		// HELPING_HAND
		136,		// HI_JUMP_KICK
		237,		// HIDDEN_POWER
		30,		// HORN_ATTACK
		32,		// HORN_DRILL
		336,		// HOWL
		308,		// HYDRO_CANNON
		56,		// HYDRO_PUMP
		63,		// HYPER_BEAM
		158,		// HYPER_FANG
		304,		// HYPER_VOICE
		95,		// HYPNOSIS
		301,		// ICE_BALL
		58,		// ICE_BEAM
		423,		// ICE_FANG
		8,		// ICE_PUNCH
		420,		// ICE_SHARD
		333,		// ICICLE_SPEAR
		196,		// ICY_WIND
		286,		// IMPRISON
		275,		// INGRAIN
		334,		// IRON_DEFENSE
		442,		// IRON_HEAD
		231,		// IRON_TAIL
		449,		// JUDGMENT
		26,		// JUMP_KICK
		2,		// KARATE_CHOP
		134,		// KINESIS
		282,		// KNOCK_OFF
		387,		// LAST_RESORT
		436,		// LAVA_PLUME
		348,		// LEAF_BLADE
		437,		// LEAF_STORM
		141,		// LEECH_LIFE
		73,		// LEECH_SEED
		43,		// LEER
		122,		// LICK
		113,		// LIGHT_SCREEN
		199,		// LOCK_ON
		142,		// LOVELY_KISS
		67,		// LOW_KICK
		381,		// LUCKY_CHANT
		461,		// LUNAR_DANCE
		295,		// LUSTER_PURGE
		183,		// MACH_PUNCH
		277,		// MAGIC_COAT
		345,		// MAGICAL_LEAF
		463,		// MAGMA_STORM
		443,		// MAGNET_BOMB
		393,		// MAGNET_RISE
		222,		// MAGNITUDE
		382,		// ME_FIRST
		212,		// MEAN_LOOK
		96,		// MEDITATE
		72,		// MEGA_DRAIN
		25,		// MEGA_KICK
		5,		// MEGA_PUNCH
		224,		// MEGAHORN
		262,		// MEMENTO
		368,		// METAL_BURST
		232,		// METAL_CLAW
		319,		// METAL_SOUND
		309,		// METEOR_MASH
		118,		// METRONOME
		208,		// MILK_DRINK
		102,		// MIMIC
		170,		// MIND_READER
		107,		// MINIMIZE
		357,		// MIRACLE_EYE
		243,		// MIRROR_COAT
		119,		// MIRROR_MOVE
		429,		// MIRROR_SHOT
		54,		// MIST
		296,		// MIST_BALL
		236,		// MOONLIGHT
		234,		// MORNING_SUN
		426,		// MUD_BOMB
		341,		// MUD_SHOT
		300,		// MUD_SPORT
		189,		// MUD_SLAP
		330,		// MUDDY_WATER
		417,		// NASTY_PLOT
		363,		// NATURAL_GIFT
		267,		// NATURE_POWER
		302,		// NEEDLE_ARM
		101,		// NIGHT_SHADE
		400,		// NIGHT_SLASH
		171,		// NIGHTMARE
		190,		// OCTAZOOKA
		316,		// ODOR_SLEUTH
		466,		// OMINOUS_WIND
		200,		// OUTRAGE
		315,		// OVERHEAT
		220,		// PAIN_SPLIT
		6,		// PAY_DAY
		371,		// PAYBACK
		64,		// PECK
		195,		// PERISH_SONG
		80,		// PETAL_DANCE
		42,		// PIN_MISSILE
		365,		// PLUCK
		305,		// POISON_FANG
		139,		// POISON_GAS
		398,		// POISON_JAB
		40,		// POISON_STING
		342,		// POISON_TAIL
		77,		// POISONPOWDER
		1,		// POUND
		181,		// POWDER_SNOW
		408,		// POWER_GEM
		384,		// POWER_SWAP
		379,		// POWER_TRICK
		438,		// POWER_WHIP
		217,		// PRESENT
		182,		// PROTECT
		60,		// PSYBEAM
		244,		// PSYCH_UP
		94,		// PSYCHIC
		354,		// PSYCHO_BOOST
		427,		// PSYCHO_CUT
		375,		// PSYCHO_SHIFT
		149,		// PSYWAVE
		386,		// PUNISHMENT
		228,		// PURSUIT
		98,		// QUICK_ATTACK
		99,		// RAGE
		240,		// RAIN_DANCE
		229,		// RAPID_SPIN
		75,		// RAZOR_LEAF
		13,		// RAZOR_WIND
		105,		// RECOVER
		278,		// RECYCLE
		115,		// REFLECT
		287,		// REFRESH
		156,		// REST
		216,		// RETURN
		279,		// REVENGE
		179,		// REVERSAL
		46,		// ROAR
		459,		// ROAR_OF_TIME
		350,		// ROCK_BLAST
		431,		// ROCK_CLIMB
		397,		// ROCK_POLISH
		157,		// ROCK_SLIDE
		249,		// ROCK_SMASH
		88,		// ROCK_THROW
		317,		// ROCK_TOMB
		439,		// ROCK_WRECKER
		272,		// ROLE_PLAY
		27,		// ROLLING_KICK
		205,		// ROLLOUT
		355,		// ROOST
		221,		// SACRED_FIRE
		219,		// SAFEGUARD
		328,		// SAND_TOMB
		28,		// SAND_ATTACK
		201,		// SANDSTORM
		184,		// SCARY_FACE
		10,		// SCRATCH
		103,		// SCREECH
		290,		// SECRET_POWER
		402,		// SEED_BOMB
		465,		// SEED_FLARE
		69,		// SEISMIC_TOSS
		120,		// SELFDESTRUCT
		247,		// SHADOW_BALL
		421,		// SHADOW_CLAW
		467,		// SHADOW_FORCE
		325,		// SHADOW_PUNCH
		425,		// SHADOW_SNEAK
		159,		// SHARPEN
		329,		// SHEER_COLD
		351,		// SHOCK_WAVE
		324,		// SIGNAL_BEAM
		318,		// SILVER_WIND
		47,		// SING
		166,		// SKETCH
		285,		// SKILL_SWAP
		130,		// SKULL_BASH
		143,		// SKY_ATTACK
		327,		// SKY_UPPERCUT
		303,		// SLACK_OFF
		21,		// SLAM
		163,		// SLASH
		79,		// SLEEP_POWDER
		214,		// SLEEP_TALK
		124,		// SLUDGE
		188,		// SLUDGE_BOMB
		265,		// SMELLINGSALT
		123,		// SMOG
		108,		// SMOKESCREEN
		289,		// SNATCH
		173,		// SNORE
		135,		// SOFTBOILED
		76,		// SOLARBEAM
		49,		// SONICBOOM
		460,		// SPACIAL_REND
		209,		// SPARK
		169,		// SPIDER_WEB
		131,		// SPIKE_CANNON
		191,		// SPIKES
		255,		// SPIT_UP
		180,		// SPITE
		150,		// SPLASH
		147,		// SPORE
		446,		// STEALTH_ROCK
		211,		// STEEL_WING
		254,		// STOCKPILE
		23,		// STOMP
		444,		// STONE_EDGE
		70,		// STRENGTH
		81,		// STRING_SHOT
		165,		// STRUGGLE
		78,		// STUN_SPORE
		66,		// SUBMISSION
		164,		// SUBSTITUTE
		389,		// SUCKER_PUNCH
		241,		// SUNNY_DAY
		162,		// SUPER_FANG
		276,		// SUPERPOWER
		48,		// SUPERSONIC
		57,		// SURF
		207,		// SWAGGER
		256,		// SWALLOW
		186,		// SWEET_KISS
		230,		// SWEET_SCENT
		129,		// SWIFT
		415,		// SWITCHEROO
		14,		// SWORDS_DANCE
		235,		// SYNTHESIS
		33,		// TACKLE
		294,		// TAIL_GLOW
		39,		// TAIL_WHIP
		366,		// TAILWIND
		36,		// TAKE_DOWN
		269,		// TAUNT
		298,		// TEETER_DANCE
		100,		// TELEPORT
		168,		// THIEF
		37,		// THRASH
		87,		// THUNDER
		422,		// THUNDER_FANG
		86,		// THUNDER_WAVE
		85,		// THUNDERBOLT
		9,		// THUNDERPUNCH
		84,		// THUNDERSHOCK
		321,		// TICKLE
		259,		// TORMENT
		92,		// TOXIC
		390,		// TOXIC_SPIKES
		144,		// TRANSFORM
		161,		// TRI_ATTACK
		271,		// TRICK
		433,		// TRICK_ROOM
		167,		// TRIPLE_KICK
		376,		// TRUMP_CARD
		41,		// TWINEEDLE
		239,		// TWISTER
		369,		// U_TURN
		253,		// UPROAR
		410,		// VACUUM_WAVE
		11,		// VICEGRIP
		22,		// VINE_WHIP
		233,		// VITAL_THROW
		344,		// VOLT_TACKLE
		358,		// WAKE_UP_SLAP
		55,		// WATER_GUN
		352,		// WATER_PULSE
		346,		// WATER_SPORT
		323,		// WATER_SPOUT
		127,		// WATERFALL
		311,		// WEATHER_BALL
		250,		// WHIRLPOOL
		18,		// WHIRLWIND
		261,		// WILL_O_WISP
		17,		// WING_ATTACK
		273,		// WISH
		110,		// WITHDRAW
		452,		// WOOD_HAMMER
		388,		// WORRY_SEED
		35,		// WRAP
		378,		// WRING_OUT
		404,		// X_SCISSOR
		281,		// YAWN
		192,		// ZAP_CANNON
		428,		// ZEN_HEADBUTT
		0		// END
	};
	return move_converter [(move < Move::SWITCH0) ? move : move - 6];
}

Nature::Natures id_to_nature (unsigned id) {
	constexpr static Nature::Natures nature_converter [] = {
		Nature::HARDY,
		Nature::LONELY,
		Nature::BRAVE,
		Nature::ADAMANT,
		Nature::NAUGHTY,
		Nature::BOLD,
		Nature::DOCILE,
		Nature::RELAXED,
		Nature::IMPISH,
		Nature::LAX,
		Nature::TIMID,
		Nature::HASTY,
		Nature::SERIOUS,
		Nature::JOLLY,
		Nature::NAIVE,
		Nature::MODEST,
		Nature::MILD,
		Nature::QUIET,
		Nature::BASHFUL,
		Nature::RASH,
		Nature::CALM,
		Nature::GENTLE,
		Nature::SASSY,
		Nature::CAREFUL,
		Nature::QUIRKY
	};
	return nature_converter [id];
}

unsigned nature_to_id (Nature::Natures nature) {
	constexpr static unsigned nature_converter [] = {
		3,		// Adamant
		18,		// Bashful
		5,		// Bold
		2,		// Brave
		20,		// Calm
		23,		// Careful
		6,		// Docile
		21,		// Gentle
		0,		// Hardy
		11,		// Hasty
		8,		// Impish
		13,		// Jolly
		9,		// Lax
		1,		// Lonely
		16,		// Mild
		15,		// Modest
		14,		// Naive
		4,		// Naughty
		17,		// Quiet
		24,		// Quirky
		19,		// Rash
		7,		// Relaxed
		22,		// Sassy
		12,		// Serious
		10		// Timid
	};
	return nature_converter [nature];
}
} // namespace po
} // namespace technicalmachine
