// Convert to / from PO's format
// Copyright (C) 2014 David Stone
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

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include "../../ability.hpp"
#include "../../gender.hpp"
#include "../../item.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

#include "../../stat/stat.hpp"

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

Species rotom_forme_to_species(unsigned const forme) {
	switch (forme) {
		case 0:
			return Species::Rotom;
		case 1:
			return Species::Rotom_Mow;
		case 2:
			return Species::Rotom_Heat;
		case 3:
			return Species::Rotom_Frost;
		case 4:
			return Species::Rotom_Wash;
		case 5:
			return Species::Rotom_Fan;
		default:
			throw (InvalidFormeId("Rotom"));
	}
}

unsigned get_forme_offset (unsigned const id, unsigned const forme) {
	constexpr unsigned deoxys = 386;
	constexpr unsigned giratina = 487;
	constexpr unsigned rotom = 479;
	constexpr unsigned shaymin = 492;
	constexpr unsigned wormadam = 413;
	auto const convert = [forme](unsigned const range, std::string const & species){
		if (forme <= range)
			return forme;
		else
			throw (InvalidFormeId(species));
	};
	switch (id) {
		case deoxys:
			return convert(3, "Deoxys");
		case giratina:
			return convert(1, "Giratina");
		case rotom: {
			auto const species_distance = [](Species const form) {
				return static_cast<unsigned>(form) - static_cast<unsigned>(Species::Rotom);
			};
			return species_distance(rotom_forme_to_species(forme));
		}
		case shaymin:
			return convert(1, "Shaymin");
		case wormadam:
			return convert(2, "Wormadam");
		default:
			return 0;
	}
}
	
unsigned get_forme (Species const species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Giratina_Origin:
		case Species::Rotom_Mow:
		case Species::Shaymin_Sky:
		case Species::Wormadam_Sandy:
			return 1;
		case Species::Deoxys_Defense:
		case Species::Rotom_Heat:
		case Species::Wormadam_Trash:
			return 2;
		case Species::Deoxys_Speed:
		case Species::Rotom_Frost:
			return 3;
		case Species::Rotom_Wash:
			return 4;
		case Species::Rotom_Fan:
			return 5;
		default:
			return 0;
	}
}

}	// unnamed namespace

Species id_to_species (unsigned const id, unsigned const forme) {
	constexpr static Species species_converter [] = {
		Species::END,	// "Missingno."
		
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
		Species::Shaymin_Land,
		Species::Arceus
	};
	Species const base_species = (id < sizeof(species_converter)) ? species_converter [id] : Species::END;
	unsigned const forme_offset = get_forme_offset (id, forme);
	return static_cast<Species>(static_cast<unsigned>(base_species) + forme_offset);
}

std::pair <uint16_t, uint8_t> species_to_id (Species species) {
	constexpr static unsigned species_converter [] = {
		1,		//	Bulbasaur
		2,		//	Ivysaur
		3,		//	Venusaur
		4,		//	Charmander
		5,		//	Charmeleon
		6,		//	Charizard
		7,		//	Squirtle
		8,		//	Wartortle
		9,		//	Blastoise
		10,		//	Caterpie
		11,		//	Metapod
		12,		//	Butterfree
		13,		//	Weedle
		14,		//	Kakuna
		15,		//	Beedrill
		16,		//	Pidgey
		17,		//	Pidgeotto
		18,		//	Pidgeot
		19,		//	Rattata
		20,		//	Raticate
		21,		//	Spearow
		22,		//	Fearow
		23,		//	Ekans
		24,		//	Arbok
		25,		//	Pikachu
		26,		//	Raichu
		27,		//	Sandshrew
		28,		//	Sandslash
		29,		//	Nidoran-F
		30,		//	Nidorina
		31,		//	Nidoqueen
		32,		//	Nidoran-M
		33,		//	Nidorino
		34,		//	Nidoking
		35,		//	Clefairy
		36,		//	Clefable
		37,		//	Vulpix
		38,		//	Ninetales
		39,		//	Jigglypuff
		40,		//	Wigglytuff
		41,		//	Zubat
		42,		//	Golbat
		43,		//	Oddish
		44,		//	Gloom
		45,		//	Vileplume
		46,		//	Paras
		47,		//	Parasect
		48,		//	Venonat
		49,		//	Venomoth
		50,		//	Diglett
		51,		//	Dugtrio
		52,		//	Meowth
		53,		//	Persian
		54,		//	Psyduck
		55,		//	Golduck
		56,		//	Mankey
		57,		//	Primeape
		58,		//	Growlithe
		59,		//	Arcanine
		60,		//	Poliwag
		61,		//	Poliwhirl
		62,		//	Poliwrath
		63,		//	Abra
		64,		//	Kadabra
		65,		//	Alakazam
		66,		//	Machop
		67,		//	Machoke
		68,		//	Machamp
		69,		//	Bellsprout
		70,		//	Weepinbell
		71,		//	Victreebel
		72,		//	Tentacool
		73,		//	Tentacruel
		74,		//	Geodude
		75,		//	Graveler
		76,		//	Golem
		77,		//	Ponyta
		78,		//	Rapidash
		79,		//	Slowpoke
		80,		//	Slowbro
		81,		//	Magnemite
		82,		//	Magneton
		83,		//	Farfetch'd
		84,		//	Doduo
		85,		//	Dodrio
		86,		//	Seel
		87,		//	Dewgong
		88,		//	Grimer
		89,		//	Muk
		90,		//	Shellder
		91,		//	Cloyster
		92,		//	Gastly
		93,		//	Haunter
		94,		//	Gengar
		95,		//	Onix
		96,		//	Drowzee
		97,		//	Hypno
		98,		//	Krabby
		99,		//	Kingler
		100,		//	Voltorb
		101,		//	Electrode
		102,		//	Exeggcute
		103,		//	Exeggutor
		104,		//	Cubone
		105,		//	Marowak
		106,		//	Hitmonlee
		107,		//	Hitmonchan
		108,		//	Lickitung
		109,		//	Koffing
		110,		//	Weezing
		111,		//	Rhyhorn
		112,		//	Rhydon
		113,		//	Chansey
		114,		//	Tangela
		115,		//	Kangaskhan
		116,		//	Horsea
		117,		//	Seadra
		118,		//	Goldeen
		119,		//	Seaking
		120,		//	Staryu
		121,		//	Starmie
		122,		//	Mr. Mime
		123,		//	Scyther
		124,		//	Jynx
		125,		//	Electabuzz
		126,		//	Magmar
		127,		//	Pinsir
		128,		//	Tauros
		129,		//	Magikarp
		130,		//	Gyarados
		131,		//	Lapras
		132,		//	Ditto
		133,		//	Eevee
		134,		//	Vaporeon
		135,		//	Jolteon
		136,		//	Flareon
		137,		//	Porygon
		138,		//	Omanyte
		139,		//	Omastar
		140,		//	Kabuto
		141,		//	Kabutops
		142,		//	Aerodactyl
		143,		//	Snorlax
		144,		//	Articuno
		145,		//	Zapdos
		146,		//	Moltres
		147,		//	Dratini
		148,		//	Dragonair
		149,		//	Dragonite
		150,		//	Mewtwo
		151,		//	Mew
		152,		//	Chikorita
		153,		//	Bayleef
		154,		//	Meganium
		155,		//	Cyndaquil
		156,		//	Quilava
		157,		//	Typhlosion
		158,		//	Totodile
		159,		//	Croconaw
		160,		//	Feraligatr
		161,		//	Sentret
		162,		//	Furret
		163,		//	Hoothoot
		164,		//	Noctowl
		165,		//	Ledyba
		166,		//	Ledian
		167,		//	Spinarak
		168,		//	Ariados
		169,		//	Crobat
		170,		//	Chinchou
		171,		//	Lanturn
		172,		//	Pichu
		173,		//	Cleffa
		174,		//	Igglybuff
		175,		//	Togepi
		176,		//	Togetic
		177,		//	Natu
		178,		//	Xatu
		179,		//	Mareep
		180,		//	Flaaffy
		181,		//	Ampharos
		182,		//	Bellossom
		183,		//	Marill
		184,		//	Azumarill
		185,		//	Sudowoodo
		186,		//	Politoed
		187,		//	Hoppip
		188,		//	Skiploom
		189,		//	Jumpluff
		190,		//	Aipom
		191,		//	Sunkern
		192,		//	Sunflora
		193,		//	Yanma
		194,		//	Wooper
		195,		//	Quagsire
		196,		//	Espeon
		197,		//	Umbreon
		198,		//	Murkrow
		199,		//	Slowking
		200,		//	Misdreavus
		201,		//	Unown
		202,		//	Wobbuffet
		203,		//	Girafarig
		204,		//	Pineco
		205,		//	Forretress
		206,		//	Dunsparce
		207,		//	Gligar
		208,		//	Steelix
		209,		//	Snubbull
		210,		//	Granbull
		211,		//	Qwilfish
		212,		//	Scizor
		213,		//	Shuckle
		214,		//	Heracross
		215,		//	Sneasel
		216,		//	Teddiursa
		217,		//	Ursaring
		218,		//	Slugma
		219,		//	Magcargo
		220,		//	Swinub
		221,		//	Piloswine
		222,		//	Corsola
		223,		//	Remoraid
		224,		//	Octillery
		225,		//	Delibird
		226,		//	Mantine
		227,		//	Skarmory
		228,		//	Houndour
		229,		//	Houndoom
		230,		//	Kingdra
		231,		//	Phanpy
		232,		//	Donphan
		233,		//	Porygon2
		234,		//	Stantler
		235,		//	Smeargle
		236,		//	Tyrogue
		237,		//	Hitmontop
		238,		//	Smoochum
		239,		//	Elekid
		240,		//	Magby
		241,		//	Miltank
		242,		//	Blissey
		243,		//	Raikou
		244,		//	Entei
		245,		//	Suicune
		246,		//	Larvitar
		247,		//	Pupitar
		248,		//	Tyranitar
		249,		//	Lugia
		250,		//	Ho-Oh
		251,		//	Celebi
		252,		//	Treecko
		253,		//	Grovyle
		254,		//	Sceptile
		255,		//	Torchic
		256,		//	Combusken
		257,		//	Blaziken
		258,		//	Mudkip
		259,		//	Marshtomp
		260,		//	Swampert
		261,		//	Poochyena
		262,		//	Mightyena
		263,		//	Zigzagoon
		264,		//	Linoone
		265,		//	Wurmple
		266,		//	Silcoon
		267,		//	Beautifly
		268,		//	Cascoon
		269,		//	Dustox
		270,		//	Lotad
		271,		//	Lombre
		272,		//	Ludicolo
		273,		//	Seedot
		274,		//	Nuzleaf
		275,		//	Shiftry
		276,		//	Taillow
		277,		//	Swellow
		278,		//	Wingull
		279,		//	Pelipper
		280,		//	Ralts
		281,		//	Kirlia
		282,		//	Gardevoir
		283,		//	Surskit
		284,		//	Masquerain
		285,		//	Shroomish
		286,		//	Breloom
		287,		//	Slakoth
		288,		//	Vigoroth
		289,		//	Slaking
		290,		//	Nincada
		291,		//	Ninjask
		292,		//	Shedinja
		293,		//	Whismur
		294,		//	Loudred
		295,		//	Exploud
		296,		//	Makuhita
		297,		//	Hariyama
		298,		//	Azurill
		299,		//	Nosepass
		300,		//	Skitty
		301,		//	Delcatty
		302,		//	Sableye
		303,		//	Mawile
		304,		//	Aron
		305,		//	Lairon
		306,		//	Aggron
		307,		//	Meditite
		308,		//	Medicham
		309,		//	Electrike
		310,		//	Manectric
		311,		//	Plusle
		312,		//	Minun
		313,		//	Volbeat
		314,		//	Illumise
		315,		//	Roselia
		316,		//	Gulpin
		317,		//	Swalot
		318,		//	Carvanha
		319,		//	Sharpedo
		320,		//	Wailmer
		321,		//	Wailord
		322,		//	Numel
		323,		//	Camerupt
		324,		//	Torkoal
		325,		//	Spoink
		326,		//	Grumpig
		327,		//	Spinda
		328,		//	Trapinch
		329,		//	Vibrava
		330,		//	Flygon
		331,		//	Cacnea
		332,		//	Cacturne
		333,		//	Swablu
		334,		//	Altaria
		335,		//	Zangoose
		336,		//	Seviper
		337,		//	Lunatone
		338,		//	Solrock
		339,		//	Barboach
		340,		//	Whiscash
		341,		//	Corphish
		342,		//	Crawdaunt
		343,		//	Baltoy
		344,		//	Claydol
		345,		//	Lileep
		346,		//	Cradily
		347,		//	Anorith
		348,		//	Armaldo
		349,		//	Feebas
		350,		//	Milotic
		351,		//	Castform
		352,		//	Kecleon
		353,		//	Shuppet
		354,		//	Banette
		355,		//	Duskull
		356,		//	Dusclops
		357,		//	Tropius
		358,		//	Chimecho
		359,		//	Absol
		360,		//	Wynaut
		361,		//	Snorunt
		362,		//	Glalie
		363,		//	Spheal
		364,		//	Sealeo
		365,		//	Walrein
		366,		//	Clamperl
		367,		//	Huntail
		368,		//	Gorebyss
		369,		//	Relicanth
		370,		//	Luvdisc
		371,		//	Bagon
		372,		//	Shelgon
		373,		//	Salamence
		374,		//	Beldum
		375,		//	Metang
		376,		//	Metagross
		377,		//	Regirock
		378,		//	Regice
		379,		//	Registeel
		380,		//	Latias
		381,		//	Latios
		382,		//	Kyogre
		383,		//	Groudon
		384,		//	Rayquaza
		385,		//	Jirachi
		386,		//	Deoxys-Mediocre
		386,		//	Deoxys-Attack
		386,		//	Deoxys-Defense
		386,		//	Deoxys-Speed
		387,		//	Turtwig
		388,		//	Grotle
		389,		//	Torterra
		390,		//	Chimchar
		391,		//	Monferno
		392,		//	Infernape
		393,		//	Piplup
		394,		//	Prinplup
		395,		//	Empoleon
		396,		//	Starly
		397,		//	Staravia
		398,		//	Staraptor
		399,		//	Bidoof
		400,		//	Bibarel
		401,		//	Kricketot
		402,		//	Kricketune
		403,		//	Shinx
		404,		//	Luxio
		405,		//	Luxray
		406,		//	Budew
		407,		//	Roserade
		408,		//	Cranidos
		409,		//	Rampardos
		410,		//	Shieldon
		411,		//	Bastiodon
		412,		//	Burmy
		413,		//	Wormadam-Plant
		413,		//	Wormadam-Sandy
		413,		//	Wormadam-Trash
		414,		//	Mothim
		415,		//	Combee
		416,		//	Vespiquen
		417,		//	Pachirisu
		418,		//	Buizel
		419,		//	Floatzel
		420,		//	Cherubi
		421,		//	Cherrim
		422,		//	Shellos
		423,		//	Gastrodon
		424,		//	Ambipom
		425,		//	Drifloon
		426,		//	Drifblim
		427,		//	Buneary
		428,		//	Lopunny
		429,		//	Mismagius
		430,		//	Honchkrow
		431,		//	Glameow
		432,		//	Purugly
		433,		//	Chingling
		434,		//	Stunky
		435,		//	Skuntank
		436,		//	Bronzor
		437,		//	Bronzong
		438,		//	Bonsly
		439,		//	Mime Jr.
		440,		//	Happiny
		441,		//	Chatot
		442,		//	Spiritomb
		443,		//	Gible
		444,		//	Gabite
		445,		//	Garchomp
		446,		//	Munchlax
		447,		//	Riolu
		448,		//	Lucario
		449,		//	Hippopotas
		450,		//	Hippowdon
		451,		//	Skorupi
		452,		//	Drapion
		453,		//	Croagunk
		454,		//	Toxicroak
		455,		//	Carnivine
		456,		//	Finneon
		457,		//	Lumineon
		458,		//	Mantyke
		459,		//	Snover
		460,		//	Abomasnow
		461,		//	Weavile
		462,		//	Magnezone
		463,		//	Lickilicky
		464,		//	Rhyperior
		465,		//	Tangrowth
		466,		//	Electivire
		467,		//	Magmortar
		468,		//	Togekiss
		469,		//	Yanmega
		470,		//	Leafeon
		471,		//	Glaceon
		472,		//	Gliscor
		473,		//	Mamoswine
		474,		//	Porygon-Z
		475,		//	Gallade
		476,		//	Probopass
		477,		//	Dusknoir
		478,		//	Froslass
		479,		//	Rotom
		479,		//	Rotom-Heat
		479,		//	Rotom-Wash
		479,		//	Rotom-Frost
		479,		//	Rotom-Fan
		479,		//	Rotom-Mow
		480,		//	Uxie
		481,		//	Mesprit
		482,		//	Azelf
		483,		//	Dialga
		484,		//	Palkia
		485,		//	Heatran
		486,		//	Regigigas
		487,		//	Giratina-Altered
		487,		//	Giratina-Origin
		488,		//	Cresselia
		489,		//	Phione
		490,		//	Manaphy
		491,		//	Darkrai
		492,		//	Shaymin-Land
		492,		//	Shaymin-Sky
		493,		//	Arceus
		0			//	End
	};
	unsigned const species_id = species_converter[static_cast<unsigned>(species)];
	unsigned const forme_id = get_forme (species);
	return std::pair <unsigned, unsigned> (species_id, forme_id);
}

Ability::Abilities id_to_ability (unsigned id) {
	constexpr static Ability::Abilities ability_converter [] = {
		Ability::END, Ability::Stench, Ability::Drizzle,
		Ability::Speed_Boost, Ability::Battle_Armor, Ability::Sturdy,
		Ability::Damp, Ability::Limber, Ability::Sand_Veil,
		Ability::Static, Ability::Volt_Absorb, Ability::Water_Absorb,
		Ability::Oblivious, Ability::Cloud_Nine, Ability::Compoundeyes,
		Ability::Insomnia, Ability::Color_Change, Ability::Immunity,
		Ability::Flash_Fire, Ability::Shield_Dust, Ability::Own_Tempo,
		Ability::Suction_Cups, Ability::Intimidate, Ability::Shadow_Tag,
		Ability::Rough_Skin, Ability::Wonder_Guard, Ability::Levitate,
		Ability::Effect_Spore, Ability::Synchronize, Ability::Clear_Body,
		Ability::Natural_Cure, Ability::Lightningrod, Ability::Serene_Grace,
		Ability::Swift_Swim, Ability::Chlorophyll, Ability::Illuminate,
		Ability::Trace, Ability::Huge_Power, Ability::Poison_Point,
		Ability::Inner_Focus, Ability::Magma_Armor, Ability::Water_Veil,
		Ability::Magnet_Pull, Ability::Soundproof, Ability::Rain_Dish,
		Ability::Sand_Stream, Ability::Pressure, Ability::Thick_Fat,
		Ability::Early_Bird, Ability::Flame_Body, Ability::Run_Away,
		Ability::Keen_Eye, Ability::Hyper_Cutter, Ability::Pickup,
		Ability::Truant, Ability::Hustle, Ability::Cute_Charm,
		Ability::Plus, Ability::Minus, Ability::Forecast,
		Ability::Sticky_Hold, Ability::Shed_Skin, Ability::Guts,
		Ability::Marvel_Scale, Ability::Liquid_Ooze, Ability::Overgrow,
		Ability::Blaze, Ability::Torrent, Ability::Swarm,
		Ability::Rock_Head, Ability::Drought, Ability::Arena_Trap,
		Ability::Vital_Spirit, Ability::White_Smoke, Ability::Pure_Power,
		Ability::Shell_Armor, Ability::Air_Lock, Ability::Tangled_Feet,
		Ability::Motor_Drive, Ability::Rivalry, Ability::Steadfast,
		Ability::Snow_Cloak, Ability::Gluttony, Ability::Anger_Point,
		Ability::Unburden, Ability::Heatproof, Ability::Simple,
		Ability::Dry_Skin, Ability::Download, Ability::Iron_Fist,
		Ability::Poison_Heal, Ability::Adaptability, Ability::Skill_Link,
		Ability::Hydration, Ability::Solar_Power, Ability::Quick_Feet,
		Ability::Normalize, Ability::Sniper, Ability::Magic_Guard,
		Ability::No_Guard, Ability::Stall, Ability::Technician,
		Ability::Leaf_Guard, Ability::Klutz, Ability::Mold_Breaker,
		Ability::Super_Luck, Ability::Aftermath, Ability::Anticipation,
		Ability::Forewarn, Ability::Unaware, Ability::Tinted_Lens,
		Ability::Filter, Ability::Slow_Start, Ability::Scrappy,
		Ability::Storm_Drain, Ability::Ice_Body, Ability::Solid_Rock,
		Ability::Snow_Warning, Ability::Honey_Gather, Ability::Frisk,
		Ability::Reckless, Ability::Multitype, Ability::Flower_Gift,
		Ability::Bad_Dreams
	};
	return ability_converter [id];
}

unsigned ability_to_id (Ability::Abilities ability) {
	constexpr static unsigned ability_converter [] = {
		91,		// Adaptability
		106,		// Aftermath
		76,		// Air_Lock
		83,		// Anger_Point
		107,		// Anticipation
		71,		// Arena_Trap
		123,		// Bad_Dreams
		4,		// Battle_Armor
		66,		// Blaze
		34,		// Chlorophyll
		29,		// Clear_Body
		13,		// Cloud_Nine
		16,		// Color_Change
		14,		// Compoundeyes
		56,		// Cute_Charm
		6,		// Damp
		88,		// Download
		2,		// Drizzle
		70,		// Drought
		87,		// Dry_Skin
		48,		// Early_Bird
		27,		// Effect_Spore
		111,		// Filter
		49,		// Flame_Body
		18,		// Flash_Fire
		122,		// Flower_Gift
		59,		// Forecast
		108,		// Forewarn
		119,		// Frisk
		82,		// Gluttony
		62,		// Guts
		85,		// Heatproof
		118,		// Honey_Gather
		37,		// Huge_Power
		55,		// Hustle
		93,		// Hydration
		52,		// Hyper_Cutter
		115,		// Ice_Body
		35,		// Illuminate
		17,		// Immunity
		39,		// Inner_Focus
		15,		// Insomnia
		22,		// Intimidate
		89,		// Iron_Fist
		51,		// Keen_Eye
		103,		// Klutz
		102,		// Leaf_Guard
		26,		// Levitate
		31,		// Lightningrod
		7,		// Limber
		64,		// Liquid_Ooze
		98,		// Magic_Guard
		40,		// Magma_Armor
		42,		// Magnet_Pull
		63,		// Marvel_Scale
		58,		// Minus
		104,		// Mold_Breaker
		78,		// Motor_Drive
		121,		// Multitype
		30,		// Natural_Cure
		99,		// No_Guard
		96,		// Normalize
		12,		// Oblivious
		65,		// Overgrow
		20,		// Own_Tempo
		53,		// Pickup
		57,		// Plus
		90,		// Poison_Heal
		38,		// Poison_Point
		46,		// Pressure
		74,		// Pure_Power
		95,		// Quick_Feet
		44,		// Rain_Dish
		120,		// Reckless
		79,		// Rivalry
		69,		// Rock_Head
		24,		// Rough_Skin
		50,		// Run_Away
		45,		// Sand_Stream
		8,		// Sand_Veil
		113,		// Scrappy
		32,		// Serene_Grace
		23,		// Shadow_Tag
		61,		// Shed_Skin
		75,		// Shell_Armor
		19,		// Shield_Dust
		86,		// Simple
		92,		// Skill_Link
		112,		// Slow_Start
		97,		// Sniper
		81,		// Snow_Cloak
		117,		// Snow_Warning
		94,		// Solar_Power
		116,		// Solid_Rock
		43,		// Soundproof
		3,		// Speed_Boost
		100,		// Stall
		9,		// Static
		80,		// Steadfast
		1,		// Stench
		60,		// Sticky_Hold
		114,		// Storm_Drain
		5,		// Sturdy
		21,		// Suction_Cups
		105,		// Super_Luck
		68,		// Swarm
		33,		// Swift_Swim
		28,		// Synchronize
		77,		// Tangled_Feet
		101,		// Technician
		47,		// Thick_Fat
		110,		// Tinted_Lens
		67,		// Torrent
		36,		// Trace
		54,		// Truant
		109,		// Unaware
		84,		// Unburden
		72,		// Vital_Spirit
		10,		// Volt_Absorb
		11,		// Water_Absorb
		41,		// Water_Veil
		73,		// White_Smoke
		25,		// Wonder_Guard
		0		// END
	};
	return ability_converter [ability];
}

class InvalidPart : public std::runtime_error {
public:
	InvalidPart (uint16_t id, uint8_t part):
		std::runtime_error ("Invalid conversion to ability ID: " + std::to_string (static_cast<unsigned> (id)) + " with part ID: " + std::to_string (static_cast <unsigned> (part)) + ".\n") {
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
			return Ability::Aftermath;
		case 3:
			return Ability::Anger_Point;
		case 4:
			return Ability::Anticipation;
		case 6:
			return Ability::Bad_Dreams;
		case 9:
			return Ability::Color_Change;
		case 11:
			return Ability::Cute_Charm;
		case 12:
			return Ability::Inner_Focus;
		case 13:
			return Ability::Download;
		case 14:
			switch (part) {
				case 0:
					return Ability::Snow_Warning;
				case 1:
					return Ability::Drizzle;
				case 2:
					return Ability::Sand_Stream;
				case 3:
					return Ability::Drought;
				default:
					throw InvalidPart(id, part);
			}
		case 15:
			return Ability::Dry_Skin;
		case 16:
			return Ability::Effect_Spore;
		case 19:
			return Ability::Flash_Fire;
		case 22:
			return Ability::Forewarn;
		case 23:
			return Ability::Frisk;
		case 24:
			return Ability::Shield_Dust;
		case 29:
			return Ability::Hydration;
		case 34:
			return Ability::Intimidate;
		case 37:
			return Ability::Leaf_Guard;
		case 41:
			return Ability::Motor_Drive;
		case 44:
			return Ability::Own_Tempo;
		case 45:
			return Ability::Poison_Heal;
		case 46:
			return Ability::Pressure;
		case 54:
			return Ability::Shed_Skin;
		case 55:
			return Ability::Slow_Start;
		case 56:
			return Ability::Solar_Power;
		case 57:
			return Ability::Soundproof;
		case 58:
			return Ability::Speed_Boost;
		case 60:
			return Ability::Steadfast;
		case 61:
			return Ability::Synchronize;
		case 66:
			return Ability::Trace;
		case 67:
			return Ability::Truant;
		case 71:
			return Ability::Wonder_Guard;
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

Item id_to_item(unsigned id) {
	switch (id) {
		case 0:
			return Item::No_Item;
		case 1:
			return Item::Big_Root;
		case 2:
			return Item::Blue_Scarf;
		case 3:
			return Item::BrightPowder;
		case 4:
			return Item::Choice_Band;
		case 5:
			return Item::Choice_Scarf;
		case 6:
			return Item::Choice_Specs;
		case 7:
			return Item::Destiny_Knot;
		case 8:
			return Item::Expert_Belt;
		case 9:
			return Item::Focus_Band;
		case 10:
			return Item::Focus_Sash;
		case 11:
			return Item::Full_Incense;
		case 12:
			return Item::Green_Scarf;
		case 13:
			return Item::Lagging_Tail;
		case 14:
			return Item::Lax_Incense;
		case 15:
			return Item::Leftovers;
		case 16:
			return Item::Luck_Incense;
		case 17:
			return Item::Mental_Herb;
		case 18:
			return Item::Metal_Powder;
		case 19:
			return Item::Muscle_Band;
		case 20:
			return Item::Odd_Incense;
		case 21:
			return Item::Pink_Scarf;
		case 22:
			return Item::Power_Herb;
		case 23:
			return Item::Pure_Incense;
		case 24:
			return Item::Quick_Powder;
		case 25:
			return Item::Reaper_Cloth;
		case 26:
			return Item::Red_Scarf;
		case 27:
			return Item::Rock_Incense;
		case 28:
			return Item::Rose_Incense;
		case 29:
			return Item::Sea_Incense;
		case 30:
			return Item::Shed_Shell;
		case 31:
			return Item::Silk_Scarf;
		case 32:
			return Item::SilverPowder;
		case 33:
			return Item::Smooth_Rock;
		case 34:
			return Item::Soft_Sand;
		case 35:
			return Item::Soothe_Bell;
		case 36:
			return Item::Wave_Incense;
		case 37:
			return Item::White_Herb;
		case 38:
			return Item::Wide_Lens;
		case 39:
			return Item::Wise_Glasses;
		case 40:
			return Item::Yellow_Scarf;
		case 41:
			return Item::Zoom_Lens;
		case 42:
			return Item::Amulet_Coin;
		case 43:
			return Item::Antidote;
		case 44:
			return Item::Awakening;
		case 45:
			return Item::Berry_Juice;
		case 46:
			return Item::Big_Pearl;
		case 47:
			return Item::Big_Mushroom;
		case 48:
			return Item::Black_Belt;
		case 49:
			return Item::Black_Flute;
		case 50:
			return Item::Black_Sludge;
		case 51:
			return Item::BlackGlasses;
		case 52:
			return Item::Blue_Flute;
		case 53:
			return Item::Blue_Shard;
		case 54:
			return Item::Burn_Heal;
		case 55:
			return Item::Calcium;
		case 56:
			return Item::Carbos;
		case 57:
			return Item::Charcoal;
		case 58:
			return Item::Cleanse_Tag;
		case 59:
			return Item::Damp_Mulch;
		case 60:
			return Item::DeepSeaScale;
		case 61:
			return Item::Dire_Hit;
		case 62:
			return Item::Dragon_Scale;
		case 63:
			return Item::Elixir;
		case 64:
			return Item::Energy_Root;
		case 65:
			return Item::EnergyPowder;
		case 66:
			return Item::Escape_Rope;
		case 67:
			return Item::Ether;
		case 68:
			return Item::Everstone;
		case 69:
			return Item::Exp_Share;
		case 70:
			return Item::Fire_Stone;
		case 71:
			return Item::Flame_Orb;
		case 72:
			return Item::Fluffy_Tail;
		case 73:
			return Item::Fresh_Water;
		case 74:
			return Item::Full_Heal;
		case 75:
			return Item::Full_Sleep_Restore;
		case 76:
			return Item::Gooey_Mulch;
		case 77:
			return Item::Green_Shard;
		case 78:
			return Item::Growth_Mulch;
		case 79:
			return Item::Guard_Spec;
		case 80:
			return Item::Heal_Powder;
		case 81:
			return Item::Heart_Scale;
		case 82:
			return Item::Honey;
		case 83:
			return Item::HP_Up;
		case 84:
			return Item::Hyper_Potion;
		case 85:
			return Item::Ice_Heal;
		case 86:
			return Item::Iron;
		case 87:
			return Item::Kings_Rock;
		case 88:
			return Item::Lava_Cookie;
		case 89:
			return Item::Leaf_Stone;
		case 90:
			return Item::Lemonade;
		case 91:
			return Item::Life_Orb;
		case 92:
			return Item::Light_Ball;
		case 93:
			return Item::Light_Clay;
		case 94:
			return Item::Lucky_Egg;
		case 95:
			return Item::Magnet;
		case 96:
			return Item::Max_Elixir;
		case 97:
			return Item::Max_Ether;
		case 98:
			return Item::Max_Potion;
		case 99:
			return Item::Max_Repel;
		case 100:
			return Item::Max_Revive;
		case 101:
			return Item::Metal_Coat;
		case 102:
			return Item::Metronome;
		case 103:
			return Item::Miracle_Seed;
		case 104:
			return Item::MooMoo_Milk;
		case 105:
			return Item::Moon_Stone;
		case 106:
			return Item::Mystic_Water;
		case 107:
			return Item::NeverMeltIce;
		case 108:
			return Item::Nugget;
		case 109:
			return Item::Old_Gateau;
		case 110:
			return Item::Parlyz_Heal;
		case 111:
			return Item::Pearl;
		case 112:
			return Item::Poke_Doll;
		case 113:
			return Item::Potion;
		case 114:
			return Item::PP_Max;
		case 115:
			return Item::PP_Up;
		case 116:
			return Item::Protein;
		case 117:
			return Item::Rare_Candy;
		case 118:
			return Item::Razor_Fang;
		case 119:
			return Item::Red_Flute;
		case 120:
			return Item::Red_Shard;
		case 121:
			return Item::Repel;
		case 122:
			return Item::Revival_Herb;
		case 123:
			return Item::Revive;
		case 124:
			return Item::Sacred_Ash;
		case 125:
			return Item::Scope_Lens;
		case 126:
			return Item::Shell_Bell;
		case 127:
			return Item::Shoal_Salt;
		case 128:
			return Item::Shoal_Shell;
		case 129:
			return Item::Smoke_Ball;
		case 130:
			return Item::Soda_Pop;
		case 131:
			return Item::Soul_Dew;
		case 132:
			return Item::Spell_Tag;
		case 133:
			return Item::Stable_Mulch;
		case 134:
			return Item::Star_Piece;
		case 135:
			return Item::Stardust;
		case 136:
			return Item::Sun_Stone;
		case 137:
			return Item::Super_Potion;
		case 138:
			return Item::Super_Repel;
		case 139:
			return Item::Thunderstone;
		case 140:
			return Item::TinyMushroom;
		case 141:
			return Item::Toxic_Orb;
		case 142:
			return Item::TwistedSpoon;
		case 143:
			return Item::Up_Grade;
		case 144:
			return Item::Water_Stone;
		case 145:
			return Item::White_Flute;
		case 146:
			return Item::X_Accuracy;
		case 147:
			return Item::X_Attack;
		case 148:
			return Item::X_Defend;
		case 149:
			return Item::X_Special;
		case 150:
			return Item::X_Sp_Def;
		case 151:
			return Item::X_Speed;
		case 152:
			return Item::Yellow_Flute;
		case 153:
			return Item::Yellow_Shard;
		case 154:
			return Item::Zinc;
		case 155:
			return Item::Icy_Rock;
		case 156:
			return Item::Lucky_Punch;
		case 157:
			return Item::Dubious_Disc;
		case 158:
			return Item::Sharp_Beak;
		case 159:
			return Item::Adamant_Orb;
		case 160:
			return Item::Damp_Rock;
		case 161:
			return Item::Heat_Rock;
		case 162:
			return Item::Lustrous_Orb;
		case 163:
			return Item::Macho_Brace;
		case 164:
			return Item::Stick;
		case 165:
			return Item::Dragon_Fang;
		case 166:
			return Item::Poison_Barb;
		case 167:
			return Item::Power_Anklet;
		case 168:
			return Item::Power_Band;
		case 169:
			return Item::Power_Belt;
		case 170:
			return Item::Power_Bracer;
		case 171:
			return Item::Power_Lens;
		case 172:
			return Item::Power_Weight;
		case 173:
			return Item::Dawn_Stone;
		case 174:
			return Item::Dusk_Stone;
		case 175:
			return Item::Electirizer;
		case 176:
			return Item::Magmarizer;
		case 177:
			return Item::Odd_Keystone;
		case 178:
			return Item::Oval_Stone;
		case 179:
			return Item::Protector;
		case 180:
			return Item::Quick_Claw;
		case 181:
			return Item::Razor_Claw;
		case 182:
			return Item::Shiny_Stone;
		case 183:
			return Item::Sticky_Barb;
		case 184:
			return Item::DeepSeaTooth;
		case 185:
			return Item::Draco_Plate;
		case 186:
			return Item::Dread_Plate;
		case 187:
			return Item::Earth_Plate;
		case 188:
			return Item::Fist_Plate;
		case 189:
			return Item::Flame_Plate;
		case 190:
			return Item::Grip_Claw;
		case 191:
			return Item::Icicle_Plate;
		case 192:
			return Item::Insect_Plate;
		case 193:
			return Item::Iron_Plate;
		case 194:
			return Item::Meadow_Plate;
		case 195:
			return Item::Mind_Plate;
		case 196:
			return Item::Sky_Plate;
		case 197:
			return Item::Splash_Plate;
		case 198:
			return Item::Spooky_Plate;
		case 199:
			return Item::Stone_Plate;
		case 200:
			return Item::Thick_Club;
		case 201:
			return Item::Toxic_Plate;
		case 202:
			return Item::Zap_Plate;
		case 203:
			return Item::Armor_Fossil;
		case 204:
			return Item::Claw_Fossil;
		case 205:
			return Item::Dome_Fossil;
		case 206:
			return Item::Hard_Stone;
		case 207:
			return Item::Helix_Fossil;
		case 208:
			return Item::Old_Amber;
		case 209:
			return Item::Rare_Bone;
		case 210:
			return Item::Root_Fossil;
		case 211:
			return Item::Skull_Fossil;
		case 212:
			return Item::Iron_Ball;
		case 213:
			return Item::Griseous_Orb;
		case 214:
			return Item::Air_Mail;
		case 215:
			return Item::Bloom_Mail;
		case 216:
			return Item::Brick_Mail;
		case 217:
			return Item::Bubble_Mail;
		case 218:
			return Item::Flame_Mail;
		case 219:
			return Item::Grass_Mail;
		// case 220 Intentionally Missing
		case 221:
			return Item::Heart_Mail;
		case 222:
			return Item::Mosaic_Mail;
		case 223:
			return Item::Snow_Mail;
		case 224:
			return Item::Space_Mail;
		case 225:
			return Item::Steel_Mail;
		case 226:
			return Item::Tunnel_Mail;
		default:
			return Item::END;
	}
}

unsigned item_to_id(Item item) {
	switch (item) {
		case Item::No_Item:
			return 0;
		case Item::Big_Root:
			return 1;
		case Item::Blue_Scarf:
			return 2;
		case Item::BrightPowder:
			return 3;
		case Item::Choice_Band:
			return 4;
		case Item::Choice_Scarf:
			return 5;
		case Item::Choice_Specs:
			return 6;
		case Item::Destiny_Knot:
			return 7;
		case Item::Expert_Belt:
			return 8;
		case Item::Focus_Band:
			return 9;
		case Item::Focus_Sash:
			return 10;
		case Item::Full_Incense:
			return 11;
		case Item::Green_Scarf:
			return 12;
		case Item::Lagging_Tail:
			return 13;
		case Item::Lax_Incense:
			return 14;
		case Item::Leftovers:
			return 15;
		case Item::Luck_Incense:
			return 16;
		case Item::Mental_Herb:
			return 17;
		case Item::Metal_Powder:
			return 18;
		case Item::Muscle_Band:
			return 19;
		case Item::Odd_Incense:
			return 20;
		case Item::Pink_Scarf:
			return 21;
		case Item::Power_Herb:
			return 22;
		case Item::Pure_Incense:
			return 23;
		case Item::Quick_Powder:
			return 24;
		case Item::Reaper_Cloth:
			return 25;
		case Item::Red_Scarf:
			return 26;
		case Item::Rock_Incense:
			return 27;
		case Item::Rose_Incense:
			return 28;
		case Item::Sea_Incense:
			return 29;
		case Item::Shed_Shell:
			return 30;
		case Item::Silk_Scarf:
			return 31;
		case Item::SilverPowder:
			return 32;
		case Item::Smooth_Rock:
			return 33;
		case Item::Soft_Sand:
			return 34;
		case Item::Soothe_Bell:
			return 35;
		case Item::Wave_Incense:
			return 36;
		case Item::White_Herb:
			return 37;
		case Item::Wide_Lens:
			return 38;
		case Item::Wise_Glasses:
			return 39;
		case Item::Yellow_Scarf:
			return 40;
		case Item::Zoom_Lens:
			return 41;
		case Item::Amulet_Coin:
			return 42;
		case Item::Antidote:
			return 43;
		case Item::Awakening:
			return 44;
		case Item::Berry_Juice:
			return 45;
		case Item::Big_Pearl:
			return 46;
		case Item::Big_Mushroom:
			return 47;
		case Item::Black_Belt:
			return 48;
		case Item::Black_Flute:
			return 49;
		case Item::Black_Sludge:
			return 50;
		case Item::BlackGlasses:
			return 51;
		case Item::Blue_Flute:
			return 52;
		case Item::Blue_Shard:
			return 53;
		case Item::Burn_Heal:
			return 54;
		case Item::Calcium:
			return 55;
		case Item::Carbos:
			return 56;
		case Item::Charcoal:
			return 57;
		case Item::Cleanse_Tag:
			return 58;
		case Item::Damp_Mulch:
			return 59;
		case Item::DeepSeaScale:
			return 60;
		case Item::Dire_Hit:
			return 61;
		case Item::Dragon_Scale:
			return 62;
		case Item::Elixir:
			return 63;
		case Item::Energy_Root:
			return 64;
		case Item::EnergyPowder:
			return 65;
		case Item::Escape_Rope:
			return 66;
		case Item::Ether:
			return 67;
		case Item::Everstone:
			return 68;
		case Item::Exp_Share:
			return 69;
		case Item::Fire_Stone:
			return 70;
		case Item::Flame_Orb:
			return 71;
		case Item::Fluffy_Tail:
			return 72;
		case Item::Fresh_Water:
			return 73;
		case Item::Full_Heal:
			return 74;
		case Item::Full_Sleep_Restore:
			return 75;
		case Item::Gooey_Mulch:
			return 76;
		case Item::Green_Shard:
			return 77;
		case Item::Growth_Mulch:
			return 78;
		case Item::Guard_Spec:
			return 79;
		case Item::Heal_Powder:
			return 80;
		case Item::Heart_Scale:
			return 81;
		case Item::Honey:
			return 82;
		case Item::HP_Up:
			return 83;
		case Item::Hyper_Potion:
			return 84;
		case Item::Ice_Heal:
			return 85;
		case Item::Iron:
			return 86;
		case Item::Kings_Rock:
			return 87;
		case Item::Lava_Cookie:
			return 88;
		case Item::Leaf_Stone:
			return 89;
		case Item::Lemonade:
			return 90;
		case Item::Life_Orb:
			return 91;
		case Item::Light_Ball:
			return 92;
		case Item::Light_Clay:
			return 93;
		case Item::Lucky_Egg:
			return 94;
		case Item::Magnet:
			return 95;
		case Item::Max_Elixir:
			return 96;
		case Item::Max_Ether:
			return 97;
		case Item::Max_Potion:
			return 98;
		case Item::Max_Repel:
			return 99;
		case Item::Max_Revive:
			return 100;
		case Item::Metal_Coat:
			return 101;
		case Item::Metronome:
			return 102;
		case Item::Miracle_Seed:
			return 103;
		case Item::MooMoo_Milk:
			return 104;
		case Item::Moon_Stone:
			return 105;
		case Item::Mystic_Water:
			return 106;
		case Item::NeverMeltIce:
			return 107;
		case Item::Nugget:
			return 108;
		case Item::Old_Gateau:
			return 109;
		case Item::Parlyz_Heal:
			return 110;
		case Item::Pearl:
			return 111;
		case Item::Poke_Doll:
			return 112;
		case Item::Potion:
			return 113;
		case Item::PP_Max:
			return 114;
		case Item::PP_Up:
			return 115;
		case Item::Protein:
			return 116;
		case Item::Rare_Candy:
			return 117;
		case Item::Razor_Fang:
			return 118;
		case Item::Red_Flute:
			return 119;
		case Item::Red_Shard:
			return 120;
		case Item::Repel:
			return 121;
		case Item::Revival_Herb:
			return 122;
		case Item::Revive:
			return 123;
		case Item::Sacred_Ash:
			return 124;
		case Item::Scope_Lens:
			return 125;
		case Item::Shell_Bell:
			return 126;
		case Item::Shoal_Salt:
			return 127;
		case Item::Shoal_Shell:
			return 128;
		case Item::Smoke_Ball:
			return 129;
		case Item::Soda_Pop:
			return 130;
		case Item::Soul_Dew:
			return 131;
		case Item::Spell_Tag:
			return 132;
		case Item::Stable_Mulch:
			return 133;
		case Item::Star_Piece:
			return 134;
		case Item::Stardust:
			return 135;
		case Item::Sun_Stone:
			return 136;
		case Item::Super_Potion:
			return 137;
		case Item::Super_Repel:
			return 138;
		case Item::Thunderstone:
			return 139;
		case Item::TinyMushroom:
			return 140;
		case Item::Toxic_Orb:
			return 141;
		case Item::TwistedSpoon:
			return 142;
		case Item::Up_Grade:
			return 143;
		case Item::Water_Stone:
			return 144;
		case Item::White_Flute:
			return 145;
		case Item::X_Accuracy:
			return 146;
		case Item::X_Attack:
			return 147;
		case Item::X_Defend:
			return 148;
		case Item::X_Special:
			return 149;
		case Item::X_Sp_Def:
			return 150;
		case Item::X_Speed:
			return 151;
		case Item::Yellow_Flute:
			return 152;
		case Item::Yellow_Shard:
			return 153;
		case Item::Zinc:
			return 154;
		case Item::Icy_Rock:
			return 155;
		case Item::Lucky_Punch:
			return 156;
		case Item::Dubious_Disc:
			return 157;
		case Item::Sharp_Beak:
			return 158;
		case Item::Adamant_Orb:
			return 159;
		case Item::Damp_Rock:
			return 160;
		case Item::Heat_Rock:
			return 161;
		case Item::Lustrous_Orb:
			return 162;
		case Item::Macho_Brace:
			return 163;
		case Item::Stick:
			return 164;
		case Item::Dragon_Fang:
			return 165;
		case Item::Poison_Barb:
			return 166;
		case Item::Power_Anklet:
			return 167;
		case Item::Power_Band:
			return 168;
		case Item::Power_Belt:
			return 169;
		case Item::Power_Bracer:
			return 170;
		case Item::Power_Lens:
			return 171;
		case Item::Power_Weight:
			return 172;
		case Item::Dawn_Stone:
			return 173;
		case Item::Dusk_Stone:
			return 174;
		case Item::Electirizer:
			return 175;
		case Item::Magmarizer:
			return 176;
		case Item::Odd_Keystone:
			return 177;
		case Item::Oval_Stone:
			return 178;
		case Item::Protector:
			return 179;
		case Item::Quick_Claw:
			return 180;
		case Item::Razor_Claw:
			return 181;
		case Item::Shiny_Stone:
			return 182;
		case Item::Sticky_Barb:
			return 183;
		case Item::DeepSeaTooth:
			return 184;
		case Item::Draco_Plate:
			return 185;
		case Item::Dread_Plate:
			return 186;
		case Item::Earth_Plate:
			return 187;
		case Item::Fist_Plate:
			return 188;
		case Item::Flame_Plate:
			return 189;
		case Item::Grip_Claw:
			return 190;
		case Item::Icicle_Plate:
			return 191;
		case Item::Insect_Plate:
			return 192;
		case Item::Iron_Plate:
			return 193;
		case Item::Meadow_Plate:
			return 194;
		case Item::Mind_Plate:
			return 195;
		case Item::Sky_Plate:
			return 196;
		case Item::Splash_Plate:
			return 197;
		case Item::Spooky_Plate:
			return 198;
		case Item::Stone_Plate:
			return 199;
		case Item::Thick_Club:
			return 200;
		case Item::Toxic_Plate:
			return 201;
		case Item::Zap_Plate:
			return 202;
		case Item::Armor_Fossil:
			return 203;
		case Item::Claw_Fossil:
			return 204;
		case Item::Dome_Fossil:
			return 205;
		case Item::Hard_Stone:
			return 206;
		case Item::Helix_Fossil:
			return 207;
		case Item::Old_Amber:
			return 208;
		case Item::Rare_Bone:
			return 209;
		case Item::Root_Fossil:
			return 210;
		case Item::Skull_Fossil:
			return 211;
		case Item::Iron_Ball:
			return 212;
		case Item::Griseous_Orb:
			return 213;
		case Item::Air_Mail:
			return 214;
		case Item::Bloom_Mail:
			return 215;
		case Item::Brick_Mail:
			return 216;
		case Item::Bubble_Mail:
			return 217;
		case Item::Flame_Mail:
			return 218;
		case Item::Grass_Mail:
			return 219;
		// Intentionally Missing 220
		case Item::Heart_Mail:
			return 221;
		case Item::Mosaic_Mail:
			return 222;
		case Item::Snow_Mail:
			return 223;
		case Item::Space_Mail:
			return 224;
		case Item::Steel_Mail:
			return 225;
		case Item::Tunnel_Mail:
			return 226;
		default:
			return 0;
			/*
			Aguav_Berry
			Apicot_Berry
			Aspear_Berry
			Babiri_Berry
			Belue_Berry
			Bicycle
			Bluk_Berry
			Charti_Berry
			Cheri_Berry
			Cherish_Ball
			Chesto_Berry
			Chilan_Berry
			Chople_Berry
			Coba_Berry
			Coin_case
			Colbur_Berry
			Cornn_Berry
			Coupon_1
			Coupon_2
			Coupon_3
			Custap_Berry
			Dive_Ball
			Durin_Berry
			Dusk_Ball
			Enigma_Berry
			Explorer_Kit
			Fashion_case
			Figy_Berry
			Galactic_Key
			Ganlon_Berry
			Good_Rod
			Great_Ball
			Grepa_Berry
			Haban_Berry
			Heal_Ball
			HM01
			HM02
			HM03
			HM04
			HM05
			HM06
			HM07
			HM08
			Hondew_Berry
			Iapapa_Berry
			Jaboca_Berry
			Journal
			Kasib_Berry
			Kebia_Berry
			Kelpsy_Berry
			Lansat_Berry
			Leppa_Berry
			Liechi_Berry
			Loot_Sack
			Lum_Berry
			Lunar_Wing
			Luxury_Ball
			Mago_Berry
			Magost_Berry
			Master_Ball
			Member_Card
			Micle_Berry
			Nanab_Berry
			Nest_Ball
			Net_Ball
			Nomel_Berry
			Oaks_Letter
			Occa_Berry
			Old_Charm
			Old_Rod
			Oran_Berry
			Pal_Pad
			Pamtre_Berry
			Parcel
			Passho_Berry
			Payapa_Berry
			Pecha_Berry
			Persim_Berry
			Petaya_Berry
			Pinap_Berry
			Poffin_case
			Point_Card
			Poke_Ball
			Poke_Radar
			Pomeg_Berry
			Premier_Ball
			Qualot_Berry
			Quick_Ball
			Rabuta_Berry
			Rawst_Berry
			Razz_Berry
			Repeat_Ball
			Rindo_Berry
			Rowap_Berry
			Safari_Ball
			Salac_Berry
			Seal_Bag
			Seal_case
			Secretpotion
			Shuca_Berry
			Sitrus_Berry
			Spelon_Berry
			Sprayduck
			Starf_Berry
			Storage_Key
			Suite_Key
			Super_Rod
			Tamato_Berry
			Tanga_Berry
			Timer_Ball
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
			Town_Map
			Ultra_Ball
			Vs_Seeker
			Wacan_Berry
			Watmel_Berry
			Wepear_Berry
			Wiki_Berry
			Works_Key
			Yache_Berry
			*/
	}
}

Item battle_id_to_item(uint16_t id, uint8_t part) {
	switch (id) {
		case 3:
			return Item::White_Herb;
		case 4:
			return Item::Focus_Band;
		case 5:
			return Item::Focus_Sash;
		case 7:
			return Item::Mental_Herb;
		case 11:
			return Item::Power_Herb;
		case 12:
			return Item::Leftovers;
		case 16:
			return Item::Black_Sludge;
		case 17:
			return Item::Quick_Claw;
		case 18:
			return Item::Berry_Juice;
		case 19:
			return (part == 0) ? Item::Flame_Orb : Item::Toxic_Orb;
		case 21:
			return Item::Life_Orb;
		case 24:
			return Item::Shell_Bell;
		case 29:
			return Item::Sticky_Barb;
		default:
			return Item::END;
	}
}

Moves id_to_move (unsigned id) {
	return (id == 0) ? Moves::END : static_cast<Moves>(id + static_cast<unsigned>(Moves::Regular_Begin) - 1);
}

unsigned move_to_id (Moves move) {
	auto const move_id = static_cast<unsigned>(move) - static_cast<unsigned>(Moves::Regular_Begin) + 1u;
	assert(move_id > 0);
	return (move == Moves::END) ? 0 : move_id;
}

Nature id_to_nature (unsigned id) {
	constexpr static Nature nature_converter [] = {
		Nature::Hardy,
		Nature::Lonely,
		Nature::Brave,
		Nature::Adamant,
		Nature::Naughty,
		Nature::Bold,
		Nature::Docile,
		Nature::Relaxed,
		Nature::Impish,
		Nature::Lax,
		Nature::Timid,
		Nature::Hasty,
		Nature::Serious,
		Nature::Jolly,
		Nature::Naive,
		Nature::Modest,
		Nature::Mild,
		Nature::Quiet,
		Nature::Bashful,
		Nature::Rash,
		Nature::Calm,
		Nature::Gentle,
		Nature::Sassy,
		Nature::Careful,
		Nature::Quirky
	};
	return nature_converter [id];
}

unsigned nature_to_id (Nature nature) {
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
	return nature_converter[static_cast<std::size_t>(nature)];
}
} // namespace po
} // namespace technicalmachine
