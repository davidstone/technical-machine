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
	return static_cast<Moves>(id);
}

unsigned move_to_id (Moves move) {
	return static_cast<unsigned>(move);
}

Species id_to_species (unsigned const id) {
	constexpr static Species species_converter [] = {
		// Generation 1
		Species::Bulbasaur,
		Species::Ivysaur,
		Species::Venusaur,
		Species::Charmander,
		Species::Charmeleon,
		Species::Charizard,
		Species::Squirtle,
		Species::Wartortle,
		Species::Blastoise,
		Species::Caterpie,
		Species::Metapod,
		Species::Butterfree,
		Species::Weedle,
		Species::Kakuna,
		Species::Beedrill,
		Species::Pidgey,
		Species::Pidgeotto,
		Species::Pidgeot,
		Species::Rattata,
		Species::Raticate,
		Species::Spearow,
		Species::Fearow,
		Species::Ekans,
		Species::Arbok,
		Species::Pikachu,
		Species::Raichu,
		Species::Sandshrew,
		Species::Sandslash,
		Species::Nidoran_F,
		Species::Nidorina,
		Species::Nidoqueen,
		Species::Nidoran_M,
		Species::Nidorino,
		Species::Nidoking,
		Species::Clefairy,
		Species::Clefable,
		Species::Vulpix,
		Species::Ninetales,
		Species::Jigglypuff,
		Species::Wigglytuff,
		Species::Zubat,
		Species::Golbat,
		Species::Oddish,
		Species::Gloom,
		Species::Vileplume,
		Species::Paras,
		Species::Parasect,
		Species::Venonat,
		Species::Venomoth,
		Species::Diglett,
		Species::Dugtrio,
		Species::Meowth,
		Species::Persian,
		Species::Psyduck,
		Species::Golduck,
		Species::Mankey,
		Species::Primeape,
		Species::Growlithe,
		Species::Arcanine,
		Species::Poliwag,
		Species::Poliwhirl,
		Species::Poliwrath,
		Species::Abra,
		Species::Kadabra,
		Species::Alakazam,
		Species::Machop,
		Species::Machoke,
		Species::Machamp,
		Species::Bellsprout,
		Species::Weepinbell,
		Species::Victreebel,
		Species::Tentacool,
		Species::Tentacruel,
		Species::Geodude,
		Species::Graveler,
		Species::Golem,
		Species::Ponyta,
		Species::Rapidash,
		Species::Slowpoke,
		Species::Slowbro,
		Species::Magnemite,
		Species::Magneton,
		Species::Farfetchd,
		Species::Doduo,
		Species::Dodrio,
		Species::Seel,
		Species::Dewgong,
		Species::Grimer,
		Species::Muk,
		Species::Shellder,
		Species::Cloyster,
		Species::Gastly,
		Species::Haunter,
		Species::Gengar,
		Species::Onix,
		Species::Drowzee,
		Species::Hypno,
		Species::Krabby,
		Species::Kingler,
		Species::Voltorb,
		Species::Electrode,
		Species::Exeggcute,
		Species::Exeggutor,
		Species::Cubone,
		Species::Marowak,
		Species::Hitmonlee,
		Species::Hitmonchan,
		Species::Lickitung,
		Species::Koffing,
		Species::Weezing,
		Species::Rhyhorn,
		Species::Rhydon,
		Species::Chansey,
		Species::Tangela,
		Species::Kangaskhan,
		Species::Horsea,
		Species::Seadra,
		Species::Goldeen,
		Species::Seaking,
		Species::Staryu,
		Species::Starmie,
		Species::Mr_Mime,
		Species::Scyther,
		Species::Jynx,
		Species::Electabuzz,
		Species::Magmar,
		Species::Pinsir,
		Species::Tauros,
		Species::Magikarp,
		Species::Gyarados,
		Species::Lapras,
		Species::Ditto,
		Species::Eevee,
		Species::Vaporeon,
		Species::Jolteon,
		Species::Flareon,
		Species::Porygon,
		Species::Omanyte,
		Species::Omastar,
		Species::Kabuto,
		Species::Kabutops,
		Species::Aerodactyl,
		Species::Snorlax,
		Species::Articuno,
		Species::Zapdos,
		Species::Moltres,
		Species::Dratini,
		Species::Dragonair,
		Species::Dragonite,
		Species::Mewtwo,
		Species::Mew,
		
		// Generation 2
		Species::Chikorita,
		Species::Bayleef,
		Species::Meganium,
		Species::Cyndaquil,
		Species::Quilava,
		Species::Typhlosion,
		Species::Totodile,
		Species::Croconaw,
		Species::Feraligatr,
		Species::Sentret,
		Species::Furret,
		Species::Hoothoot,
		Species::Noctowl,
		Species::Ledyba,
		Species::Ledian,
		Species::Spinarak,
		Species::Ariados,
		Species::Crobat,
		Species::Chinchou,
		Species::Lanturn,
		Species::Pichu,
		Species::Cleffa,
		Species::Igglybuff,
		Species::Togepi,
		Species::Togetic,
		Species::Natu,
		Species::Xatu,
		Species::Mareep,
		Species::Flaaffy,
		Species::Ampharos,
		Species::Bellossom,
		Species::Marill,
		Species::Azumarill,
		Species::Sudowoodo,
		Species::Politoed,
		Species::Hoppip,
		Species::Skiploom,
		Species::Jumpluff,
		Species::Aipom,
		Species::Sunkern,
		Species::Sunflora,
		Species::Yanma,
		Species::Wooper,
		Species::Quagsire,
		Species::Espeon,
		Species::Umbreon,
		Species::Murkrow,
		Species::Slowking,
		Species::Misdreavus,
		Species::Unown,
		Species::Wobbuffet,
		Species::Girafarig,
		Species::Pineco,
		Species::Forretress,
		Species::Dunsparce,
		Species::Gligar,
		Species::Steelix,
		Species::Snubbull,
		Species::Granbull,
		Species::Qwilfish,
		Species::Scizor,
		Species::Shuckle,
		Species::Heracross,
		Species::Sneasel,
		Species::Teddiursa,
		Species::Ursaring,
		Species::Slugma,
		Species::Magcargo,
		Species::Swinub,
		Species::Piloswine,
		Species::Corsola,
		Species::Remoraid,
		Species::Octillery,
		Species::Delibird,
		Species::Mantine,
		Species::Skarmory,
		Species::Houndour,
		Species::Houndoom,
		Species::Kingdra,
		Species::Phanpy,
		Species::Donphan,
		Species::Porygon2,
		Species::Stantler,
		Species::Smeargle,
		Species::Tyrogue,
		Species::Hitmontop,
		Species::Smoochum,
		Species::Elekid,
		Species::Magby,
		Species::Miltank,
		Species::Blissey,
		Species::Raikou,
		Species::Entei,
		Species::Suicune,
		Species::Larvitar,
		Species::Pupitar,
		Species::Tyranitar,
		Species::Lugia,
		Species::Ho_Oh,
		Species::Celebi,
		
		// Generation 3
		Species::Treecko,
		Species::Grovyle,
		Species::Sceptile,
		Species::Torchic,
		Species::Combusken,
		Species::Blaziken,
		Species::Mudkip,
		Species::Marshtomp,
		Species::Swampert,
		Species::Poochyena,
		Species::Mightyena,
		Species::Zigzagoon,
		Species::Linoone,
		Species::Wurmple,
		Species::Silcoon,
		Species::Beautifly,
		Species::Cascoon,
		Species::Dustox,
		Species::Lotad,
		Species::Lombre,
		Species::Ludicolo,
		Species::Seedot,
		Species::Nuzleaf,
		Species::Shiftry,
		Species::Taillow,
		Species::Swellow,
		Species::Wingull,
		Species::Pelipper,
		Species::Ralts,
		Species::Kirlia,
		Species::Gardevoir,
		Species::Surskit,
		Species::Masquerain,
		Species::Shroomish,
		Species::Breloom,
		Species::Slakoth,
		Species::Vigoroth,
		Species::Slaking,
		Species::Nincada,
		Species::Ninjask,
		Species::Shedinja,
		Species::Whismur,
		Species::Loudred,
		Species::Exploud,
		Species::Makuhita,
		Species::Hariyama,
		Species::Azurill,
		Species::Nosepass,
		Species::Skitty,
		Species::Delcatty,
		Species::Sableye,
		Species::Mawile,
		Species::Aron,
		Species::Lairon,
		Species::Aggron,
		Species::Meditite,
		Species::Medicham,
		Species::Electrike,
		Species::Manectric,
		Species::Plusle,
		Species::Minun,
		Species::Volbeat,
		Species::Illumise,
		Species::Roselia,
		Species::Gulpin,
		Species::Swalot,
		Species::Carvanha,
		Species::Sharpedo,
		Species::Wailmer,
		Species::Wailord,
		Species::Numel,
		Species::Camerupt,
		Species::Torkoal,
		Species::Spoink,
		Species::Grumpig,
		Species::Spinda,
		Species::Trapinch,
		Species::Vibrava,
		Species::Flygon,
		Species::Cacnea,
		Species::Cacturne,
		Species::Swablu,
		Species::Altaria,
		Species::Zangoose,
		Species::Seviper,
		Species::Lunatone,
		Species::Solrock,
		Species::Barboach,
		Species::Whiscash,
		Species::Corphish,
		Species::Crawdaunt,
		Species::Baltoy,
		Species::Claydol,
		Species::Lileep,
		Species::Cradily,
		Species::Anorith,
		Species::Armaldo,
		Species::Feebas,
		Species::Milotic,
		Species::Castform,
		Species::Kecleon,
		Species::Shuppet,
		Species::Banette,
		Species::Duskull,
		Species::Dusclops,
		Species::Tropius,
		Species::Chimecho,
		Species::Absol,
		Species::Wynaut,
		Species::Snorunt,
		Species::Glalie,
		Species::Spheal,
		Species::Sealeo,
		Species::Walrein,
		Species::Clamperl,
		Species::Huntail,
		Species::Gorebyss,
		Species::Relicanth,
		Species::Luvdisc,
		Species::Bagon,
		Species::Shelgon,
		Species::Salamence,
		Species::Beldum,
		Species::Metang,
		Species::Metagross,
		Species::Regirock,
		Species::Regice,
		Species::Registeel,
		Species::Latias,
		Species::Latios,
		Species::Kyogre,
		Species::Groudon,
		Species::Rayquaza,
		Species::Jirachi,
		Species::Deoxys_Mediocre,
		Species::Deoxys_Attack,
		Species::Deoxys_Defense,
		Species::Deoxys_Speed,
		
		// Generation 4
		Species::Turtwig,
		Species::Grotle,
		Species::Torterra,
		Species::Chimchar,
		Species::Monferno,
		Species::Infernape,
		Species::Piplup,
		Species::Prinplup,
		Species::Empoleon,
		Species::Starly,
		Species::Staravia,
		Species::Staraptor,
		Species::Bidoof,
		Species::Bibarel,
		Species::Kricketot,
		Species::Kricketune,
		Species::Shinx,
		Species::Luxio,
		Species::Luxray,
		Species::Budew,
		Species::Roserade,
		Species::Cranidos,
		Species::Rampardos,
		Species::Shieldon,
		Species::Bastiodon,
		Species::Burmy,
		Species::Wormadam_Plant,
		Species::Wormadam_Sandy,
		Species::Wormadam_Trash,
		Species::Mothim,
		Species::Combee,
		Species::Vespiquen,
		Species::Pachirisu,
		Species::Buizel,
		Species::Floatzel,
		Species::Cherubi,
		Species::Cherrim,
		Species::Shellos,
		Species::Gastrodon,
		Species::Ambipom,
		Species::Drifloon,
		Species::Drifblim,
		Species::Buneary,
		Species::Lopunny,
		Species::Mismagius,
		Species::Honchkrow,
		Species::Glameow,
		Species::Purugly,
		Species::Chingling,
		Species::Stunky,
		Species::Skuntank,
		Species::Bronzor,
		Species::Bronzong,
		Species::Bonsly,
		Species::Mime_Jr,
		Species::Happiny,
		Species::Chatot,
		Species::Spiritomb,
		Species::Gible,
		Species::Gabite,
		Species::Garchomp,
		Species::Munchlax,
		Species::Riolu,
		Species::Lucario,
		Species::Hippopotas,
		Species::Hippowdon,
		Species::Skorupi,
		Species::Drapion,
		Species::Croagunk,
		Species::Toxicroak,
		Species::Carnivine,
		Species::Finneon,
		Species::Lumineon,
		Species::Mantyke,
		Species::Snover,
		Species::Abomasnow,
		Species::Weavile,
		Species::Magnezone,
		Species::Lickilicky,
		Species::Rhyperior,
		Species::Tangrowth,
		Species::Electivire,
		Species::Magmortar,
		Species::Togekiss,
		Species::Yanmega,
		Species::Leafeon,
		Species::Glaceon,
		Species::Gliscor,
		Species::Mamoswine,
		Species::Porygon_Z,
		Species::Gallade,
		Species::Probopass,
		Species::Dusknoir,
		Species::Froslass,
		Species::Rotom,
		Species::Uxie,
		Species::Mesprit,
		Species::Azelf,
		Species::Dialga,
		Species::Palkia,
		Species::Heatran,
		Species::Regigigas,
		Species::Giratina_Altered,
		Species::Cresselia,
		Species::Phione,
		Species::Manaphy,
		Species::Darkrai,
		Species::Shaymin_L,
		Species::Arceus,
		Species::Rotom_F,
		Species::Rotom_C,
		Species::Rotom_H,
		Species::Rotom_S,
		Species::Rotom_W,
		Species::Giratina_Origin,
		Species::Shaymin_S,
	};
	return (id < sizeof(species_converter)) ? species_converter [id] : Species::END;
}

unsigned species_to_id (Species species) {
	constexpr static unsigned species_converter [] = {
		0,			//  Bulbasaur
		1,			//  Ivysaur
		2,			//  Venusaur
		3,			//  Charmander
		4,			//  Charmeleon
		5,			//  Charizard
		6,			//  Squirtle
		7,			//  Wartortle
		8,			//  Blastoise
		9,			//  Caterpie
		10,		//  Metapod
		11,		//  Butterfree
		12,		//  Weedle
		13,		//  Kakuna
		14,		//  Beedrill
		15,		//  Pidgey
		16,		//  Pidgeotto
		17,		//  Pidgeot
		18,		//  Rattata
		19,		//  Raticate
		20,		//  Spearow
		21,		//  Fearow
		22,		//  Ekans
		23,		//  Arbok
		24,		//  Pikachu
		25,		//  Raichu
		26,		//  Sandshrew
		27,		//  Sandslash
		28,		//  Nidoran-F
		29,		//  Nidorina
		30,		//  Nidoqueen
		31,		//  Nidoran-M
		32,		//  Nidorino
		33,		//  Nidoking
		34,		//  Clefairy
		35,		//  Clefable
		36,		//  Vulpix
		37,		//  Ninetales
		38,		//  Jigglypuff
		39,		//  Wigglytuff
		40,		//  Zubat
		41,		//  Golbat
		42,		//  Oddish
		43,		//  Gloom
		44,		//  Vileplume
		45,		//  Paras
		46,		//  Parasect
		47,		//  Venonat
		48,		//  Venomoth
		49,		//  Diglett
		50,		//  Dugtrio
		51,		//  Meowth
		52,		//  Persian
		53,		//  Psyduck
		54,		//  Golduck
		55,		//  Mankey
		56,		//  Primeape
		57,		//  Growlithe
		58,		//  Arcanine
		59,		//  Poliwag
		60,		//  Poliwhirl
		61,		//  Poliwrath
		62,		//  Abra
		63,		//  Kadabra
		64,		//  Alakazam
		65,		//  Machop
		66,		//  Machoke
		67,		//  Machamp
		68,		//  Bellsprout
		69,		//  Weepinbell
		70,		//  Victreebel
		71,		//  Tentacool
		72,		//  Tentacruel
		73,		//  Geodude
		74,		//  Graveler
		75,		//  Golem
		76,		//  Ponyta
		77,		//  Rapidash
		78,		//  Slowpoke
		79,		//  Slowbro
		80,		//  Magnemite
		81,		//  Magneton
		82,		//  Farfetchd
		83,		//  Doduo
		84,		//  Dodrio
		85,		//  Seel
		86,		//  Dewgong
		87,		//  Grimer
		88,		//  Muk
		89,		//  Shellder
		90,		//  Cloyster
		91,		//  Gastly
		92,		//  Haunter
		93,		//  Gengar
		94,		//  Onix
		95,		//  Drowzee
		96,		//  Hypno
		97,		//  Krabby
		98,		//  Kingler
		99,		//  Voltorb
		100,		//  Electrode
		101,		//  Exeggcute
		102,		//  Exeggutor
		103,		//  Cubone
		104,		//  Marowak
		105,		//  Hitmonlee
		106,		//  Hitmonchan
		107,		//  Lickitung
		108,		//  Koffing
		109,		//  Weezing
		110,		//  Rhyhorn
		111,		//  Rhydon
		112,		//  Chansey
		113,		//  Tangela
		114,		//  Kangaskhan
		115,		//  Horsea
		116,		//  Seadra
		117,		//  Goldeen
		118,		//  Seaking
		119,		//  Staryu
		120,		//  Starmie
		121,		//  Mr. Mime
		122,		//  Scyther
		123,		//  Jynx
		124,		//  Electabuzz
		125,		//  Magmar
		126,		//  Pinsir
		127,		//  Tauros
		128,		//  Magikarp
		129,		//  Gyarados
		130,		//  Lapras
		131,		//  Ditto
		132,		//  Eevee
		133,		//  Vaporeon
		134,		//  Jolteon
		135,		//  Flareon
		136,		//  Porygon
		137,		//  Omanyte
		138,		//  Omastar
		139,		//  Kabuto
		140,		//  Kabutops
		141,		//  Aerodactyl
		142,		//  Snorlax
		143,		//  Articuno
		144,		//  Zapdos
		145,		//  Moltres
		146,		//  Dratini
		147,		//  Dragonair
		148,		//  Dragonite
		149,		//  Mewtwo
		150,		//  Mew
		151,		//  Chikorita
		152,		//  Bayleef
		153,		//  Meganium
		154,		//  Cyndaquil
		155,		//  Quilava
		156,		//  Typhlosion
		157,		//  Totodile
		158,		//  Croconaw
		159,		//  Feraligatr
		160,		//  Sentret
		161,		//  Furret
		162,		//  Hoothoot
		163,		//  Noctowl
		164,		//  Ledyba
		165,		//  Ledian
		166,		//  Spinarak
		167,		//  Ariados
		168,		//  Crobat
		169,		//  Chinchou
		170,		//  Lanturn
		171,		//  Pichu
		172,		//  Cleffa
		173,		//  Igglybuff
		174,		//  Togepi
		175,		//  Togetic
		176,		//  Natu
		177,		//  Xatu
		178,		//  Mareep
		179,		//  Flaaffy
		180,		//  Ampharos
		181,		//  Bellossom
		182,		//  Marill
		183,		//  Azumarill
		184,		//  Sudowoodo
		185,		//  Politoed
		186,		//  Hoppip
		187,		//  Skiploom
		188,		//  Jumpluff
		189,		//  Aipom
		190,		//  Sunkern
		191,		//  Sunflora
		192,		//  Yanma
		193,		//  Wooper
		194,		//  Quagsire
		195,		//  Espeon
		196,		//  Umbreon
		197,		//  Murkrow
		198,		//  Slowking
		199,		//  Misdreavus
		200,		//  Unown
		201,		//  Wobbuffet
		202,		//  Girafarig
		203,		//  Pineco
		204,		//  Forretress
		205,		//  Dunsparce
		206,		//  Gligar
		207,		//  Steelix
		208,		//  Snubbull
		209,		//  Granbull
		210,		//  Qwilfish
		211,		//  Scizor
		212,		//  Shuckle
		213,		//  Heracross
		214,		//  Sneasel
		215,		//  Teddiursa
		216,		//  Ursaring
		217,		//  Slugma
		218,		//  Magcargo
		219,		//  Swinub
		220,		//  Piloswine
		221,		//  Corsola
		222,		//  Remoraid
		223,		//  Octillery
		224,		//  Delibird
		225,		//  Mantine
		226,		//  Skarmory
		227,		//  Houndour
		228,		//  Houndoom
		229,		//  Kingdra
		230,		//  Phanpy
		231,		//  Donphan
		232,		//  Porygon2
		233,		//  Stantler
		234,		//  Smeargle
		235,		//  Tyrogue
		236,		//  Hitmontop
		237,		//  Smoochum
		238,		//  Elekid
		239,		//  Magby
		240,		//  Miltank
		241,		//  Blissey
		242,		//  Raikou
		243,		//  Entei
		244,		//  Suicune
		245,		//  Larvitar
		246,		//  Pupitar
		247,		//  Tyranitar
		248,		//  Lugia
		249,		//  Ho-Oh
		250,		//  Celebi
		251,		//  Treecko
		252,		//  Grovyle
		253,		//  Sceptile
		254,		//  Torchic
		255,		//  Combusken
		256,		//  Blaziken
		257,		//  Mudkip
		258,		//  Marshtomp
		259,		//  Swampert
		260,		//  Poochyena
		261,		//  Mightyena
		262,		//  Zigzagoon
		263,		//  Linoone
		264,		//  Wurmple
		265,		//  Silcoon
		266,		//  Beautifly
		267,		//  Cascoon
		268,		//  Dustox
		269,		//  Lotad
		270,		//  Lombre
		271,		//  Ludicolo
		272,		//  Seedot
		273,		//  Nuzleaf
		274,		//  Shiftry
		275,		//  Taillow
		276,		//  Swellow
		277,		//  Wingull
		278,		//  Pelipper
		279,		//  Ralts
		280,		//  Kirlia
		281,		//  Gardevoir
		282,		//  Surskit
		283,		//  Masquerain
		284,		//  Shroomish
		285,		//  Breloom
		286,		//  Slakoth
		287,		//  Vigoroth
		288,		//  Slaking
		289,		//  Nincada
		290,		//  Ninjask
		291,		//  Shedinja
		292,		//  Whismur
		293,		//  Loudred
		294,		//  Exploud
		295,		//  Makuhita
		296,		//  Hariyama
		297,		//  Azurill
		298,		//  Nosepass
		299,		//  Skitty
		300,		//  Delcatty
		301,		//  Sableye
		302,		//  Mawile
		303,		//  Aron
		304,		//  Lairon
		305,		//  Aggron
		306,		//  Meditite
		307,		//  Medicham
		308,		//  Electrike
		309,		//  Manectric
		310,		//  Plusle
		311,		//  Minun
		312,		//  Volbeat
		313,		//  Illumise
		314,		//  Roselia
		315,		//  Gulpin
		316,		//  Swalot
		317,		//  Carvanha
		318,		//  Sharpedo
		319,		//  Wailmer
		320,		//  Wailord
		321,		//  Numel
		322,		//  Camerupt
		323,		//  Torkoal
		324,		//  Spoink
		325,		//  Grumpig
		326,		//  Spinda
		327,		//  Trapinch
		328,		//  Vibrava
		329,		//  Flygon
		330,		//  Cacnea
		331,		//  Cacturne
		332,		//  Swablu
		333,		//  Altaria
		334,		//  Zangoose
		335,		//  Seviper
		336,		//  Lunatone
		337,		//  Solrock
		338,		//  Barboach
		339,		//  Whiscash
		340,		//  Corphish
		341,		//  Crawdaunt
		342,		//  Baltoy
		343,		//  Claydol
		344,		//  Lileep
		345,		//  Cradily
		346,		//  Anorith
		347,		//  Armaldo
		348,		//  Feebas
		349,		//  Milotic
		350,		//  Castform
		351,		//  Kecleon
		352,		//  Shuppet
		353,		//  Banette
		354,		//  Duskull
		355,		//  Dusclops
		356,		//  Tropius
		357,		//  Chimecho
		358,		//  Absol
		359,		//  Wynaut
		360,		//  Snorunt
		361,		//  Glalie
		362,		//  Spheal
		363,		//  Sealeo
		364,		//  Walrein
		365,		//  Clamperl
		366,		//  Huntail
		367,		//  Gorebyss
		368,		//  Relicanth
		369,		//  Luvdisc
		370,		//  Bagon
		371,		//  Shelgon
		372,		//  Salamence
		373,		//  Beldum
		374,		//  Metang
		375,		//  Metagross
		376,		//  Regirock
		377,		//  Regice
		378,		//  Registeel
		379,		//  Latias
		380,		//  Latios
		381,		//  Kyogre
		382,		//  Groudon
		383,		//  Rayquaza
		384,		//  Jirachi
		385,		//  Deoxys-Mediocre
		386,		//  Deoxys-Attack
		387,		//  Deoxys-Defense
		388,		//  Deoxys-Speed
		389,		//  Turtwig
		390,		//  Grotle
		391,		//  Torterra
		392,		//  Chimchar
		393,		//  Monferno
		394,		//  Infernape
		395,		//  Piplup
		396,		//  Prinplup
		397,		//  Empoleon
		398,		//  Starly
		399,		//  Staravia
		400,		//  Staraptor
		401,		//  Bidoof
		402,		//  Bibarel
		403,		//  Kricketot
		404,		//  Kricketune
		405,		//  Shinx
		406,		//  Luxio
		407,		//  Luxray
		408,		//  Budew
		409,		//  Roserade
		410,		//  Cranidos
		411,		//  Rampardos
		412,		//  Shieldon
		413,		//  Bastiodon
		414,		//  Burmy
		415,		//  Wormadam-Plant
		416,		//  Wormadam-Sandy
		417,		//  Wormadam-Trash
		418,		//  Mothim
		419,		//  Combee
		420,		//  Vespiquen
		421,		//  Pachirisu
		422,		//  Buizel
		423,		//  Floatzel
		424,		//  Cherubi
		425,		//  Cherrim
		426,		//  Shellos
		427,		//  Gastrodon
		428,		//  Ambipom
		429,		//  Drifloon
		430,		//  Drifblim
		431,		//  Buneary
		432,		//  Lopunny
		433,		//  Mismagius
		434,		//  Honchkrow
		435,		//  Glameow
		436,		//  Purugly
		437,		//  Chingling
		438,		//  Stunky
		439,		//  Skuntank
		440,		//  Bronzor
		441,		//  Bronzong
		442,		//  Bonsly
		443,		//  Mime Jr.
		444,		//  Happiny
		445,		//  Chatot
		446,		//  Spiritomb
		447,		//  Gible
		448,		//  Gabite
		449,		//  Garchomp
		450,		//  Munchlax
		451,		//  Riolu
		452,		//  Lucario
		453,		//  Hippopotas
		454,		//  Hippowdon
		455,		//  Skorupi
		456,		//  Drapion
		457,		//  Croagunk
		458,		//  Toxicroak
		459,		//  Carnivine
		460,		//  Finneon
		461,		//  Lumineon
		462,		//  Mantyke
		463,		//  Snover
		464,		//  Abomasnow
		465,		//  Weavile
		466,		//  Magnezone
		467,		//  Lickilicky
		468,		//  Rhyperior
		469,		//  Tangrowth
		470,		//  Electivire
		471,		//  Magmortar
		472,		//  Togekiss
		473,		//  Yanmega
		474,		//  Leafeon
		475,		//  Glaceon
		476,		//  Gliscor
		477,		//  Mamoswine
		478,		//  Porygon-Z
		479,		//  Gallade
		480,		//  Probopass
		481,		//  Dusknoir
		482,		//  Froslass
		483,		//  Rotom
		500,		//  Rotom-H
		502,		//  Rotom-W
		498,		//  Rotom-F
		501,		//  Rotom-S
		499,		//  Rotom-C
		484,		//  Uxie
		485,		//  Mesprit
		486,		//  Azelf
		487,		//  Dialga
		488,		//  Palkia
		489,		//  Heatran
		490,		//  Regigigas
		491,		//  Giratina-Altered
		503,		//  Giratina-Origin
		492,		//  Cresselia
		493,		//  Phione
		494,		//  Manaphy
		495,		//  Darkrai
		496,		//  Shaymin-L
		504,		//  Shaymin-S
		497		// Arceus
	};
	return species_converter[static_cast<unsigned>(species)];
}

}	// namespace pl
}	// namespace technicalmachine
