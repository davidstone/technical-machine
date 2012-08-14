// Convert to / from PL's format
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

#include "../../gender.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace pl {

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

Moves id_to_move (unsigned const id) {
	return (id < static_cast<unsigned>(Moves::SWITCH0)) ?
		static_cast<Moves>(id) :
		static_cast<Moves>(id + 6);
}

unsigned move_to_id (Moves move) {
	return (move < Moves::SWITCH0) ? static_cast<unsigned>(move) : static_cast<unsigned>(move) - 6;
}

Species id_to_species (unsigned id) {
	constexpr static Species species_converter [] = {
		Species::BULBASAUR,
		Species::IVYSAUR,
		Species::VENUSAUR,
		Species::CHARMANDER,
		Species::CHARMELEON,
		Species::CHARIZARD,
		Species::SQUIRTLE,
		Species::WARTORTLE,
		Species::BLASTOISE,
		Species::CATERPIE,
		Species::METAPOD,
		Species::BUTTERFREE,
		Species::WEEDLE,
		Species::KAKUNA,
		Species::BEEDRILL,
		Species::PIDGEY,
		Species::PIDGEOTTO,
		Species::PIDGEOT,
		Species::RATTATA,
		Species::RATICATE,
		Species::SPEAROW,
		Species::FEAROW,
		Species::EKANS,
		Species::ARBOK,
		Species::PIKACHU,
		Species::RAICHU,
		Species::SANDSHREW,
		Species::SANDSLASH,
		Species::NIDORAN_F,
		Species::NIDORINA,
		Species::NIDOQUEEN,
		Species::NIDORAN_M,
		Species::NIDORINO,
		Species::NIDOKING,
		Species::CLEFAIRY,
		Species::CLEFABLE,
		Species::VULPIX,
		Species::NINETALES,
		Species::JIGGLYPUFF,
		Species::WIGGLYTUFF,
		Species::ZUBAT,
		Species::GOLBAT,
		Species::ODDISH,
		Species::GLOOM,
		Species::VILEPLUME,
		Species::PARAS,
		Species::PARASECT,
		Species::VENONAT,
		Species::VENOMOTH,
		Species::DIGLETT,
		Species::DUGTRIO,
		Species::MEOWTH,
		Species::PERSIAN,
		Species::PSYDUCK,
		Species::GOLDUCK,
		Species::MANKEY,
		Species::PRIMEAPE,
		Species::GROWLITHE,
		Species::ARCANINE,
		Species::POLIWAG,
		Species::POLIWHIRL,
		Species::POLIWRATH,
		Species::ABRA,
		Species::KADABRA,
		Species::ALAKAZAM,
		Species::MACHOP,
		Species::MACHOKE,
		Species::MACHAMP,
		Species::BELLSPROUT,
		Species::WEEPINBELL,
		Species::VICTREEBEL,
		Species::TENTACOOL,
		Species::TENTACRUEL,
		Species::GEODUDE,
		Species::GRAVELER,
		Species::GOLEM,
		Species::PONYTA,
		Species::RAPIDASH,
		Species::SLOWPOKE,
		Species::SLOWBRO,
		Species::MAGNEMITE,
		Species::MAGNETON,
		Species::FARFETCHD,
		Species::DODUO,
		Species::DODRIO,
		Species::SEEL,
		Species::DEWGONG,
		Species::GRIMER,
		Species::MUK,
		Species::SHELLDER,
		Species::CLOYSTER,
		Species::GASTLY,
		Species::HAUNTER,
		Species::GENGAR,
		Species::ONIX,
		Species::DROWZEE,
		Species::HYPNO,
		Species::KRABBY,
		Species::KINGLER,
		Species::VOLTORB,
		Species::ELECTRODE,
		Species::EXEGGCUTE,
		Species::EXEGGUTOR,
		Species::CUBONE,
		Species::MAROWAK,
		Species::HITMONLEE,
		Species::HITMONCHAN,
		Species::LICKITUNG,
		Species::KOFFING,
		Species::WEEZING,
		Species::RHYHORN,
		Species::RHYDON,
		Species::CHANSEY,
		Species::TANGELA,
		Species::KANGASKHAN,
		Species::HORSEA,
		Species::SEADRA,
		Species::GOLDEEN,
		Species::SEAKING,
		Species::STARYU,
		Species::STARMIE,
		Species::MR_MIME,
		Species::SCYTHER,
		Species::JYNX,
		Species::ELECTABUZZ,
		Species::MAGMAR,
		Species::PINSIR,
		Species::TAUROS,
		Species::MAGIKARP,
		Species::GYARADOS,
		Species::LAPRAS,
		Species::DITTO,
		Species::EEVEE,
		Species::VAPOREON,
		Species::JOLTEON,
		Species::FLAREON,
		Species::PORYGON,
		Species::OMANYTE,
		Species::OMASTAR,
		Species::KABUTO,
		Species::KABUTOPS,
		Species::AERODACTYL,
		Species::SNORLAX,
		Species::ARTICUNO,
		Species::ZAPDOS,
		Species::MOLTRES,
		Species::DRATINI,
		Species::DRAGONAIR,
		Species::DRAGONITE,
		Species::MEWTWO,
		Species::MEW,
		Species::CHIKORITA,
		Species::BAYLEEF,
		Species::MEGANIUM,
		Species::CYNDAQUIL,
		Species::QUILAVA,
		Species::TYPHLOSION,
		Species::TOTODILE,
		Species::CROCONAW,
		Species::FERALIGATR,
		Species::SENTRET,
		Species::FURRET,
		Species::HOOTHOOT,
		Species::NOCTOWL,
		Species::LEDYBA,
		Species::LEDIAN,
		Species::SPINARAK,
		Species::ARIADOS,
		Species::CROBAT,
		Species::CHINCHOU,
		Species::LANTURN,
		Species::PICHU,
		Species::CLEFFA,
		Species::IGGLYBUFF,
		Species::TOGEPI,
		Species::TOGETIC,
		Species::NATU,
		Species::XATU,
		Species::MAREEP,
		Species::FLAAFFY,
		Species::AMPHAROS,
		Species::BELLOSSOM,
		Species::MARILL,
		Species::AZUMARILL,
		Species::SUDOWOODO,
		Species::POLITOED,
		Species::HOPPIP,
		Species::SKIPLOOM,
		Species::JUMPLUFF,
		Species::AIPOM,
		Species::SUNKERN,
		Species::SUNFLORA,
		Species::YANMA,
		Species::WOOPER,
		Species::QUAGSIRE,
		Species::ESPEON,
		Species::UMBREON,
		Species::MURKROW,
		Species::SLOWKING,
		Species::MISDREAVUS,
		Species::UNOWN,
		Species::WOBBUFFET,
		Species::GIRAFARIG,
		Species::PINECO,
		Species::FORRETRESS,
		Species::DUNSPARCE,
		Species::GLIGAR,
		Species::STEELIX,
		Species::SNUBBULL,
		Species::GRANBULL,
		Species::QWILFISH,
		Species::SCIZOR,
		Species::SHUCKLE,
		Species::HERACROSS,
		Species::SNEASEL,
		Species::TEDDIURSA,
		Species::URSARING,
		Species::SLUGMA,
		Species::MAGCARGO,
		Species::SWINUB,
		Species::PILOSWINE,
		Species::CORSOLA,
		Species::REMORAID,
		Species::OCTILLERY,
		Species::DELIBIRD,
		Species::MANTINE,
		Species::SKARMORY,
		Species::HOUNDOUR,
		Species::HOUNDOOM,
		Species::KINGDRA,
		Species::PHANPY,
		Species::DONPHAN,
		Species::PORYGON2,
		Species::STANTLER,
		Species::SMEARGLE,
		Species::TYROGUE,
		Species::HITMONTOP,
		Species::SMOOCHUM,
		Species::ELEKID,
		Species::MAGBY,
		Species::MILTANK,
		Species::BLISSEY,
		Species::RAIKOU,
		Species::ENTEI,
		Species::SUICUNE,
		Species::LARVITAR,
		Species::PUPITAR,
		Species::TYRANITAR,
		Species::LUGIA,
		Species::HO_OH,
		Species::CELEBI,
		Species::TREECKO,
		Species::GROVYLE,
		Species::SCEPTILE,
		Species::TORCHIC,
		Species::COMBUSKEN,
		Species::BLAZIKEN,
		Species::MUDKIP,
		Species::MARSHTOMP,
		Species::SWAMPERT,
		Species::POOCHYENA,
		Species::MIGHTYENA,
		Species::ZIGZAGOON,
		Species::LINOONE,
		Species::WURMPLE,
		Species::SILCOON,
		Species::BEAUTIFLY,
		Species::CASCOON,
		Species::DUSTOX,
		Species::LOTAD,
		Species::LOMBRE,
		Species::LUDICOLO,
		Species::SEEDOT,
		Species::NUZLEAF,
		Species::SHIFTRY,
		Species::TAILLOW,
		Species::SWELLOW,
		Species::WINGULL,
		Species::PELIPPER,
		Species::RALTS,
		Species::KIRLIA,
		Species::GARDEVOIR,
		Species::SURSKIT,
		Species::MASQUERAIN,
		Species::SHROOMISH,
		Species::BRELOOM,
		Species::SLAKOTH,
		Species::VIGOROTH,
		Species::SLAKING,
		Species::NINCADA,
		Species::NINJASK,
		Species::SHEDINJA,
		Species::WHISMUR,
		Species::LOUDRED,
		Species::EXPLOUD,
		Species::MAKUHITA,
		Species::HARIYAMA,
		Species::AZURILL,
		Species::NOSEPASS,
		Species::SKITTY,
		Species::DELCATTY,
		Species::SABLEYE,
		Species::MAWILE,
		Species::ARON,
		Species::LAIRON,
		Species::AGGRON,
		Species::MEDITITE,
		Species::MEDICHAM,
		Species::ELECTRIKE,
		Species::MANECTRIC,
		Species::PLUSLE,
		Species::MINUN,
		Species::VOLBEAT,
		Species::ILLUMISE,
		Species::ROSELIA,
		Species::GULPIN,
		Species::SWALOT,
		Species::CARVANHA,
		Species::SHARPEDO,
		Species::WAILMER,
		Species::WAILORD,
		Species::NUMEL,
		Species::CAMERUPT,
		Species::TORKOAL,
		Species::SPOINK,
		Species::GRUMPIG,
		Species::SPINDA,
		Species::TRAPINCH,
		Species::VIBRAVA,
		Species::FLYGON,
		Species::CACNEA,
		Species::CACTURNE,
		Species::SWABLU,
		Species::ALTARIA,
		Species::ZANGOOSE,
		Species::SEVIPER,
		Species::LUNATONE,
		Species::SOLROCK,
		Species::BARBOACH,
		Species::WHISCASH,
		Species::CORPHISH,
		Species::CRAWDAUNT,
		Species::BALTOY,
		Species::CLAYDOL,
		Species::LILEEP,
		Species::CRADILY,
		Species::ANORITH,
		Species::ARMALDO,
		Species::FEEBAS,
		Species::MILOTIC,
		Species::CASTFORM,
		Species::KECLEON,
		Species::SHUPPET,
		Species::BANETTE,
		Species::DUSKULL,
		Species::DUSCLOPS,
		Species::TROPIUS,
		Species::CHIMECHO,
		Species::ABSOL,
		Species::WYNAUT,
		Species::SNORUNT,
		Species::GLALIE,
		Species::SPHEAL,
		Species::SEALEO,
		Species::WALREIN,
		Species::CLAMPERL,
		Species::HUNTAIL,
		Species::GOREBYSS,
		Species::RELICANTH,
		Species::LUVDISC,
		Species::BAGON,
		Species::SHELGON,
		Species::SALAMENCE,
		Species::BELDUM,
		Species::METANG,
		Species::METAGROSS,
		Species::REGIROCK,
		Species::REGICE,
		Species::REGISTEEL,
		Species::LATIAS,
		Species::LATIOS,
		Species::KYOGRE,
		Species::GROUDON,
		Species::RAYQUAZA,
		Species::JIRACHI,
		Species::DEOXYS_M,
		Species::DEOXYS_A,
		Species::DEOXYS_D,
		Species::DEOXYS_S,
		Species::TURTWIG,
		Species::GROTLE,
		Species::TORTERRA,
		Species::CHIMCHAR,
		Species::MONFERNO,
		Species::INFERNAPE,
		Species::PIPLUP,
		Species::PRINPLUP,
		Species::EMPOLEON,
		Species::STARLY,
		Species::STARAVIA,
		Species::STARAPTOR,
		Species::BIDOOF,
		Species::BIBAREL,
		Species::KRICKETOT,
		Species::KRICKETUNE,
		Species::SHINX,
		Species::LUXIO,
		Species::LUXRAY,
		Species::BUDEW,
		Species::ROSERADE,
		Species::CRANIDOS,
		Species::RAMPARDOS,
		Species::SHIELDON,
		Species::BASTIODON,
		Species::BURMY,
		Species::WORMADAM_P,
		Species::WORMADAM_S,
		Species::WORMADAM_T,
		Species::MOTHIM,
		Species::COMBEE,
		Species::VESPIQUEN,
		Species::PACHIRISU,
		Species::BUIZEL,
		Species::FLOATZEL,
		Species::CHERUBI,
		Species::CHERRIM,
		Species::SHELLOS,
		Species::GASTRODON,
		Species::AMBIPOM,
		Species::DRIFLOON,
		Species::DRIFBLIM,
		Species::BUNEARY,
		Species::LOPUNNY,
		Species::MISMAGIUS,
		Species::HONCHKROW,
		Species::GLAMEOW,
		Species::PURUGLY,
		Species::CHINGLING,
		Species::STUNKY,
		Species::SKUNTANK,
		Species::BRONZOR,
		Species::BRONZONG,
		Species::BONSLY,
		Species::MIME_JR,
		Species::HAPPINY,
		Species::CHATOT,
		Species::SPIRITOMB,
		Species::GIBLE,
		Species::GABITE,
		Species::GARCHOMP,
		Species::MUNCHLAX,
		Species::RIOLU,
		Species::LUCARIO,
		Species::HIPPOPOTAS,
		Species::HIPPOWDON,
		Species::SKORUPI,
		Species::DRAPION,
		Species::CROAGUNK,
		Species::TOXICROAK,
		Species::CARNIVINE,
		Species::FINNEON,
		Species::LUMINEON,
		Species::MANTYKE,
		Species::SNOVER,
		Species::ABOMASNOW,
		Species::WEAVILE,
		Species::MAGNEZONE,
		Species::LICKILICKY,
		Species::RHYPERIOR,
		Species::TANGROWTH,
		Species::ELECTIVIRE,
		Species::MAGMORTAR,
		Species::TOGEKISS,
		Species::YANMEGA,
		Species::LEAFEON,
		Species::GLACEON,
		Species::GLISCOR,
		Species::MAMOSWINE,
		Species::PORYGON_Z,
		Species::GALLADE,
		Species::PROBOPASS,
		Species::DUSKNOIR,
		Species::FROSLASS,
		Species::ROTOM,
		Species::UXIE,
		Species::MESPRIT,
		Species::AZELF,
		Species::DIALGA,
		Species::PALKIA,
		Species::HEATRAN,
		Species::REGIGIGAS,
		Species::GIRATINA_A,
		Species::CRESSELIA,
		Species::PHIONE,
		Species::MANAPHY,
		Species::DARKRAI,
		Species::SHAYMIN_L,
		Species::ARCEUS,
		Species::ROTOM_F,
		Species::ROTOM_C,
		Species::ROTOM_H,
		Species::ROTOM_S,
		Species::ROTOM_W,
		Species::GIRATINA_O,
		Species::SHAYMIN_S,
		Species::END
	};
	return species_converter [id];
}

unsigned species_to_id (Species species) {
	constexpr static unsigned species_converter [] = {
		464,		// Abomasnow
		62,		// Abra
		358,		// Absol
		141,		// Aerodactyl
		305,		// Aggron
		189,		// Aipom
		64,		// Alakazam
		333,		// Altaria
		428,		// Ambipom
		180,		// Ampharos
		346,		// Anorith
		23,		// Arbok
		58,		// Arcanine
		497,		// Arceus
		167,		// Ariados
		347,		// Armaldo
		303,		// Aron
		143,		// Articuno
		486,		// Azelf
		183,		// Azumarill
		297,		// Azurill
		370,		// Bagon
		342,		// Baltoy
		353,		// Banette
		338,		// Barboach
		413,		// Bastiodon
		152,		// Bayleef
		266,		// Beautifly
		14,		// Beedrill
		373,		// Beldum
		181,		// Bellossom
		68,		// Bellsprout
		402,		// Bibarel
		401,		// Bidoof
		8,		// Blastoise
		256,		// Blaziken
		241,		// Blissey
		442,		// Bonsly
		285,		// Breloom
		441,		// Bronzong
		440,		// Bronzor
		408,		// Budew
		422,		// Buizel
		0,		// Bulbasaur
		431,		// Buneary
		414,		// Burmy
		11,		// Butterfree
		330,		// Cacnea
		331,		// Cacturne
		322,		// Camerupt
		459,		// Carnivine
		317,		// Carvanha
		267,		// Cascoon
		350,		// Castform
		9,		// Caterpie
		250,		// Celebi
		112,		// Chansey
		5,		// Charizard
		3,		// Charmander
		4,		// Charmeleon
		445,		// Chatot
		425,		// Cherrim
		424,		// Cherubi
		151,		// Chikorita
		392,		// Chimchar
		357,		// Chimecho
		169,		// Chinchou
		437,		// Chingling
		365,		// Clamperl
		343,		// Claydol
		35,		// Clefable
		34,		// Clefairy
		172,		// Cleffa
		90,		// Cloyster
		419,		// Combee
		255,		// Combusken
		340,		// Corphish
		221,		// Corsola
		345,		// Cradily
		410,		// Cranidos
		341,		// Crawdaunt
		492,		// Cresselia
		457,		// Croagunk
		168,		// Crobat
		158,		// Croconaw
		103,		// Cubone
		154,		// Cyndaquil
		495,		// Darkrai
		300,		// Delcatty
		224,		// Delibird
		386,		// Deoxys-A
		387,		// Deoxys-D
		385,		// Deoxys-M
		388,		// Deoxys-S
		86,		// Dewgong
		487,		// Dialga
		49,		// Diglett
		131,		// Ditto
		84,		// Dodrio
		83,		// Doduo
		231,		// Donphan
		147,		// Dragonair
		148,		// Dragonite
		456,		// Drapion
		146,		// Dratini
		430,		// Drifblim
		429,		// Drifloon
		95,		// Drowzee
		50,		// Dugtrio
		205,		// Dunsparce
		355,		// Dusclops
		481,		// Dusknoir
		354,		// Duskull
		268,		// Dustox
		132,		// Eevee
		22,		// Ekans
		124,		// Electabuzz
		470,		// Electivire
		308,		// Electrike
		100,		// Electrode
		238,		// Elekid
		397,		// Empoleon
		243,		// Entei
		195,		// Espeon
		101,		// Exeggcute
		102,		// Exeggutor
		294,		// Exploud
		82,		// Farfetch'd
		21,		// Fearow
		348,		// Feebas
		159,		// Feraligatr
		460,		// Finneon
		179,		// Flaaffy
		135,		// Flareon
		423,		// Floatzel
		329,		// Flygon
		204,		// Forretress
		482,		// Froslass
		161,		// Furret
		448,		// Gabite
		479,		// Gallade
		449,		// Garchomp
		281,		// Gardevoir
		91,		// Gastly
		427,		// Gastrodon
		93,		// Gengar
		73,		// Geodude
		447,		// Gible
		202,		// Girafarig
		491,		// Giratina-A
		503,		// Giratina-O
		475,		// Glaceon
		361,		// Glalie
		435,		// Glameow
		206,		// Gligar
		476,		// Gliscor
		43,		// Gloom
		41,		// Golbat
		117,		// Goldeen
		54,		// Golduck
		75,		// Golem
		367,		// Gorebyss
		209,		// Granbull
		74,		// Graveler
		87,		// Grimer
		390,		// Grotle
		382,		// Groudon
		252,		// Grovyle
		57,		// Growlithe
		325,		// Grumpig
		315,		// Gulpin
		129,		// Gyarados
		444,		// Happiny
		296,		// Hariyama
		92,		// Haunter
		489,		// Heatran
		213,		// Heracross
		453,		// Hippopotas
		454,		// Hippowdon
		106,		// Hitmonchan
		105,		// Hitmonlee
		236,		// Hitmontop
		249,		// Ho-Oh
		434,		// Honchkrow
		162,		// Hoothoot
		186,		// Hoppip
		115,		// Horsea
		228,		// Houndoom
		227,		// Houndour
		366,		// Huntail
		96,		// Hypno
		173,		// Igglybuff
		313,		// Illumise
		394,		// Infernape
		1,		// Ivysaur
		38,		// Jigglypuff
		384,		// Jirachi
		134,		// Jolteon
		188,		// Jumpluff
		123,		// Jynx
		139,		// Kabuto
		140,		// Kabutops
		63,		// Kadabra
		13,		// Kakuna
		114,		// Kangaskhan
		351,		// Kecleon
		229,		// Kingdra
		98,		// Kingler
		280,		// Kirlia
		108,		// Koffing
		97,		// Krabby
		403,		// Kricketot
		404,		// Kricketune
		381,		// Kyogre
		304,		// Lairon
		170,		// Lanturn
		130,		// Lapras
		245,		// Larvitar
		379,		// Latias
		380,		// Latios
		474,		// Leafeon
		165,		// Ledian
		164,		// Ledyba
		467,		// Lickilicky
		107,		// Lickitung
		344,		// Lileep
		263,		// Linoone
		270,		// Lombre
		432,		// Lopunny
		269,		// Lotad
		293,		// Loudred
		452,		// Lucario
		271,		// Ludicolo
		248,		// Lugia
		461,		// Lumineon
		336,		// Lunatone
		369,		// Luvdisc
		406,		// Luxio
		407,		// Luxray
		67,		// Machamp
		66,		// Machoke
		65,		// Machop
		239,		// Magby
		218,		// Magcargo
		128,		// Magikarp
		125,		// Magmar
		471,		// Magmortar
		80,		// Magnemite
		81,		// Magneton
		466,		// Magnezone
		295,		// Makuhita
		477,		// Mamoswine
		494,		// Manaphy
		309,		// Manectric
		55,		// Mankey
		225,		// Mantine
		462,		// Mantyke
		178,		// Mareep
		182,		// Marill
		104,		// Marowak
		258,		// Marshtomp
		283,		// Masquerain
		302,		// Mawile
		307,		// Medicham
		306,		// Meditite
		153,		// Meganium
		51,		// Meowth
		485,		// Mesprit
		375,		// Metagross
		374,		// Metang
		10,		// Metapod
		150,		// Mew
		149,		// Mewtwo
		261,		// Mightyena
		349,		// Milotic
		240,		// Miltank
		443,		// Mime Jr.
		311,		// Minun
		199,		// Misdreavus
		433,		// Mismagius
		145,		// Moltres
		393,		// Monferno
		418,		// Mothim
		121,		// Mr. Mime
		257,		// Mudkip
		88,		// Muk
		450,		// Munchlax
		197,		// Murkrow
		176,		// Natu
		33,		// Nidoking
		30,		// Nidoqueen
		28,		// Nidoran-F
		31,		// Nidoran-M
		29,		// Nidorina
		32,		// Nidorino
		289,		// Nincada
		37,		// Ninetales
		290,		// Ninjask
		163,		// Noctowl
		298,		// Nosepass
		321,		// Numel
		273,		// Nuzleaf
		223,		// Octillery
		42,		// Oddish
		137,		// Omanyte
		138,		// Omastar
		94,		// Onix
		421,		// Pachirisu
		488,		// Palkia
		45,		// Paras
		46,		// Parasect
		278,		// Pelipper
		52,		// Persian
		230,		// Phanpy
		493,		// Phione
		171,		// Pichu
		17,		// Pidgeot
		16,		// Pidgeotto
		15,		// Pidgey
		24,		// Pikachu
		220,		// Piloswine
		203,		// Pineco
		126,		// Pinsir
		395,		// Piplup
		310,		// Plusle
		185,		// Politoed
		59,		// Poliwag
		60,		// Poliwhirl
		61,		// Poliwrath
		76,		// Ponyta
		260,		// Poochyena
		136,		// Porygon
		478,		// Porygon-Z
		232,		// Porygon2
		56,		// Primeape
		396,		// Prinplup
		480,		// Probopass
		53,		// Psyduck
		246,		// Pupitar
		436,		// Purugly
		194,		// Quagsire
		155,		// Quilava
		210,		// Qwilfish
		25,		// Raichu
		242,		// Raikou
		279,		// Ralts
		411,		// Rampardos
		77,		// Rapidash
		19,		// Raticate
		18,		// Rattata
		383,		// Rayquaza
		377,		// Regice
		490,		// Regigigas
		376,		// Regirock
		378,		// Registeel
		368,		// Relicanth
		222,		// Remoraid
		111,		// Rhydon
		110,		// Rhyhorn
		468,		// Rhyperior
		451,		// Riolu
		314,		// Roselia
		409,		// Roserade
		483,		// Rotom
		499,		// Rotom-C
		498,		// Rotom-F
		500,		// Rotom-H
		501,		// Rotom-S
		502,		// Rotom-W
		301,		// Sableye
		372,		// Salamence
		26,		// Sandshrew
		27,		// Sandslash
		253,		// Sceptile
		211,		// Scizor
		122,		// Scyther
		116,		// Seadra
		118,		// Seaking
		363,		// Sealeo
		272,		// Seedot
		85,		// Seel
		160,		// Sentret
		335,		// Seviper
		318,		// Sharpedo
		496,		// Shaymin-L
		504,		// Shaymin-S
		291,		// Shedinja
		371,		// Shelgon
		89,		// Shellder
		426,		// Shellos
		412,		// Shieldon
		274,		// Shiftry
		405,		// Shinx
		284,		// Shroomish
		212,		// Shuckle
		352,		// Shuppet
		265,		// Silcoon
		226,		// Skarmory
		187,		// Skiploom
		299,		// Skitty
		455,		// Skorupi
		439,		// Skuntank
		288,		// Slaking
		286,		// Slakoth
		79,		// Slowbro
		198,		// Slowking
		78,		// Slowpoke
		217,		// Slugma
		234,		// Smeargle
		237,		// Smoochum
		214,		// Sneasel
		142,		// Snorlax
		360,		// Snorunt
		463,		// Snover
		208,		// Snubbull
		337,		// Solrock
		20,		// Spearow
		362,		// Spheal
		166,		// Spinarak
		326,		// Spinda
		446,		// Spiritomb
		324,		// Spoink
		6,		// Squirtle
		233,		// Stantler
		400,		// Staraptor
		399,		// Staravia
		398,		// Starly
		120,		// Starmie
		119,		// Staryu
		207,		// Steelix
		438,		// Stunky
		184,		// Sudowoodo
		244,		// Suicune
		191,		// Sunflora
		190,		// Sunkern
		282,		// Surskit
		332,		// Swablu
		316,		// Swalot
		259,		// Swampert
		276,		// Swellow
		219,		// Swinub
		275,		// Taillow
		113,		// Tangela
		469,		// Tangrowth
		127,		// Tauros
		215,		// Teddiursa
		71,		// Tentacool
		72,		// Tentacruel
		472,		// Togekiss
		174,		// Togepi
		175,		// Togetic
		254,		// Torchic
		323,		// Torkoal
		391,		// Torterra
		157,		// Totodile
		458,		// Toxicroak
		327,		// Trapinch
		251,		// Treecko
		356,		// Tropius
		389,		// Turtwig
		156,		// Typhlosion
		247,		// Tyranitar
		235,		// Tyrogue
		196,		// Umbreon
		200,		// Unown
		216,		// Ursaring
		484,		// Uxie
		133,		// Vaporeon
		48,		// Venomoth
		47,		// Venonat
		2,		// Venusaur
		420,		// Vespiquen
		328,		// Vibrava
		70,		// Victreebel
		287,		// Vigoroth
		44,		// Vileplume
		312,		// Volbeat
		99,		// Voltorb
		36,		// Vulpix
		319,		// Wailmer
		320,		// Wailord
		364,		// Walrein
		7,		// Wartortle
		465,		// Weavile
		12,		// Weedle
		69,		// Weepinbell
		109,		// Weezing
		339,		// Whiscash
		292,		// Whismur
		39,		// Wigglytuff
		277,		// Wingull
		201,		// Wobbuffet
		193,		// Wooper
		415,		// Wormadam-P
		416,		// Wormadam-S
		417,		// Wormadam-T
		264,		// Wurmple
		359,		// Wynaut
		177,		// Xatu
		192,		// Yanma
		473,		// Yanmega
		334,		// Zangoose
		144,		// Zapdos
		262,		// Zigzagoon
		40		// Zubat
	};
	return species_converter[static_cast<unsigned>(species)];
}

}	// namespace pl
}	// namespace technicalmachine
