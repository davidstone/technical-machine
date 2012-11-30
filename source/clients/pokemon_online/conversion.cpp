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
			return Species::Rotom_C;
		case 2:
			return Species::Rotom_H;
		case 3:
			return Species::Rotom_F;
		case 4:
			return Species::Rotom_W;
		case 5:
			return Species::Rotom_S;
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
		case Species::Giratina_O:
		case Species::Rotom_C:
		case Species::Shaymin_S:
		case Species::Wormadam_Sandy:
			return 1;
		case Species::Deoxys_Defense:
		case Species::Rotom_H:
		case Species::Wormadam_Trash:
			return 2;
		case Species::Deoxys_Speed:
		case Species::Rotom_F:
			return 3;
		case Species::Rotom_W:
			return 4;
		case Species::Rotom_S:
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
		Species::Giratina_A,
		Species::Cresselia,
		Species::Phione,
		Species::Manaphy,
		Species::Darkrai,
		Species::Shaymin_L,
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
		122,		//	Mr_Mime
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
		479,		//	Rotom-H
		479,		//	Rotom-W
		479,		//	Rotom-F
		479,		//	Rotom-S
		479,		//	Rotom-C
		480,		//	Uxie
		481,		//	Mesprit
		482,		//	Azelf
		483,		//	Dialga
		484,		//	Palkia
		485,		//	Heatran
		486,		//	Regigigas
		487,		//	Giratina-A
		487,		//	Giratina-O
		488,		//	Cresselia
		489,		//	Phione
		490,		//	Manaphy
		491,		//	Darkrai
		492,		//	Shaymin_L
		492,		//	Shaymin-S
		493,		//	Arceus
		0			//	End
	};
	unsigned const species_id = species_converter[static_cast<unsigned>(species)];
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

Moves id_to_move (unsigned id) {
	return (id == 0) ? Moves::END : static_cast<Moves>(id + static_cast<unsigned>(Moves::Regular_Begin) - 1);
}

unsigned move_to_id (Moves move) {
	auto const move_id = static_cast<unsigned>(move) - static_cast<unsigned>(Moves::Regular_Begin) + 1u;
	assert(move_id > 0);
	return (move == Moves::END) ? 0 : move_id;
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
