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
#include "id.hpp"

#include "../../gender.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace pl {

template<>
ID<Gender::Genders> simulator_cast<ID<Gender::Genders>, Gender::Genders>(Gender::Genders const & gender) {
	switch (gender) {
		case Gender::MALE:
			return ID<Gender::Genders>(1);
		case Gender::FEMALE:
			return ID<Gender::Genders>(2);
		default:	// Gender::GENDERLESS:
			return ID<Gender::Genders>(0);
	}
}

template<>
Gender::Genders simulator_cast<Gender::Genders, ID<Gender::Genders>>(ID<Gender::Genders> const & id) {
	switch (id.value()) {
		case 0:
			return Gender::GENDERLESS;
		case 1:
			return Gender::MALE;
		default:	// case 2:
			return Gender::FEMALE;
	}
}

template<>
Moves simulator_cast<Moves, ID<Moves>>(ID<Moves> const & id) {
	return static_cast<Moves>(id.value());
}

template<>
ID<Moves> simulator_cast<ID<Moves>, Moves>(Moves const & move) {
	return ID<Moves>(static_cast<unsigned>(move));
}

template<>
Species simulator_cast<Species, ID<Species>>(ID<Species> const & id) {
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
		Species::Shaymin_Land,
		Species::Arceus,
		Species::Rotom_Frost,
		Species::Rotom_Mow,
		Species::Rotom_Heat,
		Species::Rotom_Fan,
		Species::Rotom_Wash,
		Species::Giratina_Origin,
		Species::Shaymin_Sky,
	};
	return (id.value() < sizeof(species_converter)) ? species_converter[id.value()] : Species::END;
}

template<>
ID<Species> simulator_cast<ID<Species>, Species>(Species const & species) {
	static constexpr ID<Species> species_converter[] {
		ID<Species>(0),		//  Bulbasaur
		ID<Species>(1),		//  Ivysaur
		ID<Species>(2),		//  Venusaur
		ID<Species>(3),		//  Charmander
		ID<Species>(4),		//  Charmeleon
		ID<Species>(5),		//  Charizard
		ID<Species>(6),		//  Squirtle
		ID<Species>(7),		//  Wartortle
		ID<Species>(8),		//  Blastoise
		ID<Species>(9),		//  Caterpie
		ID<Species>(10),		//  Metapod
		ID<Species>(11),		//  Butterfree
		ID<Species>(12),		//  Weedle
		ID<Species>(13),		//  Kakuna
		ID<Species>(14),		//  Beedrill
		ID<Species>(15),		//  Pidgey
		ID<Species>(16),		//  Pidgeotto
		ID<Species>(17),		//  Pidgeot
		ID<Species>(18),		//  Rattata
		ID<Species>(19),		//  Raticate
		ID<Species>(20),		//  Spearow
		ID<Species>(21),		//  Fearow
		ID<Species>(22),		//  Ekans
		ID<Species>(23),		//  Arbok
		ID<Species>(24),		//  Pikachu
		ID<Species>(25),		//  Raichu
		ID<Species>(26),		//  Sandshrew
		ID<Species>(27),		//  Sandslash
		ID<Species>(28),		//  Nidoran-F
		ID<Species>(29),		//  Nidorina
		ID<Species>(30),		//  Nidoqueen
		ID<Species>(31),		//  Nidoran-M
		ID<Species>(32),		//  Nidorino
		ID<Species>(33),		//  Nidoking
		ID<Species>(34),		//  Clefairy
		ID<Species>(35),		//  Clefable
		ID<Species>(36),		//  Vulpix
		ID<Species>(37),		//  Ninetales
		ID<Species>(38),		//  Jigglypuff
		ID<Species>(39),		//  Wigglytuff
		ID<Species>(40),		//  Zubat
		ID<Species>(41),		//  Golbat
		ID<Species>(42),		//  Oddish
		ID<Species>(43),		//  Gloom
		ID<Species>(44),		//  Vileplume
		ID<Species>(45),		//  Paras
		ID<Species>(46),		//  Parasect
		ID<Species>(47),		//  Venonat
		ID<Species>(48),		//  Venomoth
		ID<Species>(49),		//  Diglett
		ID<Species>(50),		//  Dugtrio
		ID<Species>(51),		//  Meowth
		ID<Species>(52),		//  Persian
		ID<Species>(53),		//  Psyduck
		ID<Species>(54),		//  Golduck
		ID<Species>(55),		//  Mankey
		ID<Species>(56),		//  Primeape
		ID<Species>(57),		//  Growlithe
		ID<Species>(58),		//  Arcanine
		ID<Species>(59),		//  Poliwag
		ID<Species>(60),		//  Poliwhirl
		ID<Species>(61),		//  Poliwrath
		ID<Species>(62),		//  Abra
		ID<Species>(63),		//  Kadabra
		ID<Species>(64),		//  Alakazam
		ID<Species>(65),		//  Machop
		ID<Species>(66),		//  Machoke
		ID<Species>(67),		//  Machamp
		ID<Species>(68),		//  Bellsprout
		ID<Species>(69),		//  Weepinbell
		ID<Species>(70),		//  Victreebel
		ID<Species>(71),		//  Tentacool
		ID<Species>(72),		//  Tentacruel
		ID<Species>(73),		//  Geodude
		ID<Species>(74),		//  Graveler
		ID<Species>(75),		//  Golem
		ID<Species>(76),		//  Ponyta
		ID<Species>(77),		//  Rapidash
		ID<Species>(78),		//  Slowpoke
		ID<Species>(79),		//  Slowbro
		ID<Species>(80),		//  Magnemite
		ID<Species>(81),		//  Magneton
		ID<Species>(82),		//  Farfetchd
		ID<Species>(83),		//  Doduo
		ID<Species>(84),		//  Dodrio
		ID<Species>(85),		//  Seel
		ID<Species>(86),		//  Dewgong
		ID<Species>(87),		//  Grimer
		ID<Species>(88),		//  Muk
		ID<Species>(89),		//  Shellder
		ID<Species>(90),		//  Cloyster
		ID<Species>(91),		//  Gastly
		ID<Species>(92),		//  Haunter
		ID<Species>(93),		//  Gengar
		ID<Species>(94),		//  Onix
		ID<Species>(95),		//  Drowzee
		ID<Species>(96),		//  Hypno
		ID<Species>(97),		//  Krabby
		ID<Species>(98),		//  Kingler
		ID<Species>(99),		//  Voltorb
		ID<Species>(100),		//  Electrode
		ID<Species>(101),		//  Exeggcute
		ID<Species>(102),		//  Exeggutor
		ID<Species>(103),		//  Cubone
		ID<Species>(104),		//  Marowak
		ID<Species>(105),		//  Hitmonlee
		ID<Species>(106),		//  Hitmonchan
		ID<Species>(107),		//  Lickitung
		ID<Species>(108),		//  Koffing
		ID<Species>(109),		//  Weezing
		ID<Species>(110),		//  Rhyhorn
		ID<Species>(111),		//  Rhydon
		ID<Species>(112),		//  Chansey
		ID<Species>(113),		//  Tangela
		ID<Species>(114),		//  Kangaskhan
		ID<Species>(115),		//  Horsea
		ID<Species>(116),		//  Seadra
		ID<Species>(117),		//  Goldeen
		ID<Species>(118),		//  Seaking
		ID<Species>(119),		//  Staryu
		ID<Species>(120),		//  Starmie
		ID<Species>(121),		//  Mr. Mime
		ID<Species>(122),		//  Scyther
		ID<Species>(123),		//  Jynx
		ID<Species>(124),		//  Electabuzz
		ID<Species>(125),		//  Magmar
		ID<Species>(126),		//  Pinsir
		ID<Species>(127),		//  Tauros
		ID<Species>(128),		//  Magikarp
		ID<Species>(129),		//  Gyarados
		ID<Species>(130),		//  Lapras
		ID<Species>(131),		//  Ditto
		ID<Species>(132),		//  Eevee
		ID<Species>(133),		//  Vaporeon
		ID<Species>(134),		//  Jolteon
		ID<Species>(135),		//  Flareon
		ID<Species>(136),		//  Porygon
		ID<Species>(137),		//  Omanyte
		ID<Species>(138),		//  Omastar
		ID<Species>(139),		//  Kabuto
		ID<Species>(140),		//  Kabutops
		ID<Species>(141),		//  Aerodactyl
		ID<Species>(142),		//  Snorlax
		ID<Species>(143),		//  Articuno
		ID<Species>(144),		//  Zapdos
		ID<Species>(145),		//  Moltres
		ID<Species>(146),		//  Dratini
		ID<Species>(147),		//  Dragonair
		ID<Species>(148),		//  Dragonite
		ID<Species>(149),		//  Mewtwo
		ID<Species>(150),		//  Mew
		ID<Species>(151),		//  Chikorita
		ID<Species>(152),		//  Bayleef
		ID<Species>(153),		//  Meganium
		ID<Species>(154),		//  Cyndaquil
		ID<Species>(155),		//  Quilava
		ID<Species>(156),		//  Typhlosion
		ID<Species>(157),		//  Totodile
		ID<Species>(158),		//  Croconaw
		ID<Species>(159),		//  Feraligatr
		ID<Species>(160),		//  Sentret
		ID<Species>(161),		//  Furret
		ID<Species>(162),		//  Hoothoot
		ID<Species>(163),		//  Noctowl
		ID<Species>(164),		//  Ledyba
		ID<Species>(165),		//  Ledian
		ID<Species>(166),		//  Spinarak
		ID<Species>(167),		//  Ariados
		ID<Species>(168),		//  Crobat
		ID<Species>(169),		//  Chinchou
		ID<Species>(170),		//  Lanturn
		ID<Species>(171),		//  Pichu
		ID<Species>(172),		//  Cleffa
		ID<Species>(173),		//  Igglybuff
		ID<Species>(174),		//  Togepi
		ID<Species>(175),		//  Togetic
		ID<Species>(176),		//  Natu
		ID<Species>(177),		//  Xatu
		ID<Species>(178),		//  Mareep
		ID<Species>(179),		//  Flaaffy
		ID<Species>(180),		//  Ampharos
		ID<Species>(181),		//  Bellossom
		ID<Species>(182),		//  Marill
		ID<Species>(183),		//  Azumarill
		ID<Species>(184),		//  Sudowoodo
		ID<Species>(185),		//  Politoed
		ID<Species>(186),		//  Hoppip
		ID<Species>(187),		//  Skiploom
		ID<Species>(188),		//  Jumpluff
		ID<Species>(189),		//  Aipom
		ID<Species>(190),		//  Sunkern
		ID<Species>(191),		//  Sunflora
		ID<Species>(192),		//  Yanma
		ID<Species>(193),		//  Wooper
		ID<Species>(194),		//  Quagsire
		ID<Species>(195),		//  Espeon
		ID<Species>(196),		//  Umbreon
		ID<Species>(197),		//  Murkrow
		ID<Species>(198),		//  Slowking
		ID<Species>(199),		//  Misdreavus
		ID<Species>(200),		//  Unown
		ID<Species>(201),		//  Wobbuffet
		ID<Species>(202),		//  Girafarig
		ID<Species>(203),		//  Pineco
		ID<Species>(204),		//  Forretress
		ID<Species>(205),		//  Dunsparce
		ID<Species>(206),		//  Gligar
		ID<Species>(207),		//  Steelix
		ID<Species>(208),		//  Snubbull
		ID<Species>(209),		//  Granbull
		ID<Species>(210),		//  Qwilfish
		ID<Species>(211),		//  Scizor
		ID<Species>(212),		//  Shuckle
		ID<Species>(213),		//  Heracross
		ID<Species>(214),		//  Sneasel
		ID<Species>(215),		//  Teddiursa
		ID<Species>(216),		//  Ursaring
		ID<Species>(217),		//  Slugma
		ID<Species>(218),		//  Magcargo
		ID<Species>(219),		//  Swinub
		ID<Species>(220),		//  Piloswine
		ID<Species>(221),		//  Corsola
		ID<Species>(222),		//  Remoraid
		ID<Species>(223),		//  Octillery
		ID<Species>(224),		//  Delibird
		ID<Species>(225),		//  Mantine
		ID<Species>(226),		//  Skarmory
		ID<Species>(227),		//  Houndour
		ID<Species>(228),		//  Houndoom
		ID<Species>(229),		//  Kingdra
		ID<Species>(230),		//  Phanpy
		ID<Species>(231),		//  Donphan
		ID<Species>(232),		//  Porygon2
		ID<Species>(233),		//  Stantler
		ID<Species>(234),		//  Smeargle
		ID<Species>(235),		//  Tyrogue
		ID<Species>(236),		//  Hitmontop
		ID<Species>(237),		//  Smoochum
		ID<Species>(238),		//  Elekid
		ID<Species>(239),		//  Magby
		ID<Species>(240),		//  Miltank
		ID<Species>(241),		//  Blissey
		ID<Species>(242),		//  Raikou
		ID<Species>(243),		//  Entei
		ID<Species>(244),		//  Suicune
		ID<Species>(245),		//  Larvitar
		ID<Species>(246),		//  Pupitar
		ID<Species>(247),		//  Tyranitar
		ID<Species>(248),		//  Lugia
		ID<Species>(249),		//  Ho-Oh
		ID<Species>(250),		//  Celebi
		ID<Species>(251),		//  Treecko
		ID<Species>(252),		//  Grovyle
		ID<Species>(253),		//  Sceptile
		ID<Species>(254),		//  Torchic
		ID<Species>(255),		//  Combusken
		ID<Species>(256),		//  Blaziken
		ID<Species>(257),		//  Mudkip
		ID<Species>(258),		//  Marshtomp
		ID<Species>(259),		//  Swampert
		ID<Species>(260),		//  Poochyena
		ID<Species>(261),		//  Mightyena
		ID<Species>(262),		//  Zigzagoon
		ID<Species>(263),		//  Linoone
		ID<Species>(264),		//  Wurmple
		ID<Species>(265),		//  Silcoon
		ID<Species>(266),		//  Beautifly
		ID<Species>(267),		//  Cascoon
		ID<Species>(268),		//  Dustox
		ID<Species>(269),		//  Lotad
		ID<Species>(270),		//  Lombre
		ID<Species>(271),		//  Ludicolo
		ID<Species>(272),		//  Seedot
		ID<Species>(273),		//  Nuzleaf
		ID<Species>(274),		//  Shiftry
		ID<Species>(275),		//  Taillow
		ID<Species>(276),		//  Swellow
		ID<Species>(277),		//  Wingull
		ID<Species>(278),		//  Pelipper
		ID<Species>(279),		//  Ralts
		ID<Species>(280),		//  Kirlia
		ID<Species>(281),		//  Gardevoir
		ID<Species>(282),		//  Surskit
		ID<Species>(283),		//  Masquerain
		ID<Species>(284),		//  Shroomish
		ID<Species>(285),		//  Breloom
		ID<Species>(286),		//  Slakoth
		ID<Species>(287),		//  Vigoroth
		ID<Species>(288),		//  Slaking
		ID<Species>(289),		//  Nincada
		ID<Species>(290),		//  Ninjask
		ID<Species>(291),		//  Shedinja
		ID<Species>(292),		//  Whismur
		ID<Species>(293),		//  Loudred
		ID<Species>(294),		//  Exploud
		ID<Species>(295),		//  Makuhita
		ID<Species>(296),		//  Hariyama
		ID<Species>(297),		//  Azurill
		ID<Species>(298),		//  Nosepass
		ID<Species>(299),		//  Skitty
		ID<Species>(300),		//  Delcatty
		ID<Species>(301),		//  Sableye
		ID<Species>(302),		//  Mawile
		ID<Species>(303),		//  Aron
		ID<Species>(304),		//  Lairon
		ID<Species>(305),		//  Aggron
		ID<Species>(306),		//  Meditite
		ID<Species>(307),		//  Medicham
		ID<Species>(308),		//  Electrike
		ID<Species>(309),		//  Manectric
		ID<Species>(310),		//  Plusle
		ID<Species>(311),		//  Minun
		ID<Species>(312),		//  Volbeat
		ID<Species>(313),		//  Illumise
		ID<Species>(314),		//  Roselia
		ID<Species>(315),		//  Gulpin
		ID<Species>(316),		//  Swalot
		ID<Species>(317),		//  Carvanha
		ID<Species>(318),		//  Sharpedo
		ID<Species>(319),		//  Wailmer
		ID<Species>(320),		//  Wailord
		ID<Species>(321),		//  Numel
		ID<Species>(322),		//  Camerupt
		ID<Species>(323),		//  Torkoal
		ID<Species>(324),		//  Spoink
		ID<Species>(325),		//  Grumpig
		ID<Species>(326),		//  Spinda
		ID<Species>(327),		//  Trapinch
		ID<Species>(328),		//  Vibrava
		ID<Species>(329),		//  Flygon
		ID<Species>(330),		//  Cacnea
		ID<Species>(331),		//  Cacturne
		ID<Species>(332),		//  Swablu
		ID<Species>(333),		//  Altaria
		ID<Species>(334),		//  Zangoose
		ID<Species>(335),		//  Seviper
		ID<Species>(336),		//  Lunatone
		ID<Species>(337),		//  Solrock
		ID<Species>(338),		//  Barboach
		ID<Species>(339),		//  Whiscash
		ID<Species>(340),		//  Corphish
		ID<Species>(341),		//  Crawdaunt
		ID<Species>(342),		//  Baltoy
		ID<Species>(343),		//  Claydol
		ID<Species>(344),		//  Lileep
		ID<Species>(345),		//  Cradily
		ID<Species>(346),		//  Anorith
		ID<Species>(347),		//  Armaldo
		ID<Species>(348),		//  Feebas
		ID<Species>(349),		//  Milotic
		ID<Species>(350),		//  Castform
		ID<Species>(351),		//  Kecleon
		ID<Species>(352),		//  Shuppet
		ID<Species>(353),		//  Banette
		ID<Species>(354),		//  Duskull
		ID<Species>(355),		//  Dusclops
		ID<Species>(356),		//  Tropius
		ID<Species>(357),		//  Chimecho
		ID<Species>(358),		//  Absol
		ID<Species>(359),		//  Wynaut
		ID<Species>(360),		//  Snorunt
		ID<Species>(361),		//  Glalie
		ID<Species>(362),		//  Spheal
		ID<Species>(363),		//  Sealeo
		ID<Species>(364),		//  Walrein
		ID<Species>(365),		//  Clamperl
		ID<Species>(366),		//  Huntail
		ID<Species>(367),		//  Gorebyss
		ID<Species>(368),		//  Relicanth
		ID<Species>(369),		//  Luvdisc
		ID<Species>(370),		//  Bagon
		ID<Species>(371),		//  Shelgon
		ID<Species>(372),		//  Salamence
		ID<Species>(373),		//  Beldum
		ID<Species>(374),		//  Metang
		ID<Species>(375),		//  Metagross
		ID<Species>(376),		//  Regirock
		ID<Species>(377),		//  Regice
		ID<Species>(378),		//  Registeel
		ID<Species>(379),		//  Latias
		ID<Species>(380),		//  Latios
		ID<Species>(381),		//  Kyogre
		ID<Species>(382),		//  Groudon
		ID<Species>(383),		//  Rayquaza
		ID<Species>(384),		//  Jirachi
		ID<Species>(385),		//  Deoxys-Mediocre
		ID<Species>(386),		//  Deoxys-Attack
		ID<Species>(387),		//  Deoxys-Defense
		ID<Species>(388),		//  Deoxys-Speed
		ID<Species>(389),		//  Turtwig
		ID<Species>(390),		//  Grotle
		ID<Species>(391),		//  Torterra
		ID<Species>(392),		//  Chimchar
		ID<Species>(393),		//  Monferno
		ID<Species>(394),		//  Infernape
		ID<Species>(395),		//  Piplup
		ID<Species>(396),		//  Prinplup
		ID<Species>(397),		//  Empoleon
		ID<Species>(398),		//  Starly
		ID<Species>(399),		//  Staravia
		ID<Species>(400),		//  Staraptor
		ID<Species>(401),		//  Bidoof
		ID<Species>(402),		//  Bibarel
		ID<Species>(403),		//  Kricketot
		ID<Species>(404),		//  Kricketune
		ID<Species>(405),		//  Shinx
		ID<Species>(406),		//  Luxio
		ID<Species>(407),		//  Luxray
		ID<Species>(408),		//  Budew
		ID<Species>(409),		//  Roserade
		ID<Species>(410),		//  Cranidos
		ID<Species>(411),		//  Rampardos
		ID<Species>(412),		//  Shieldon
		ID<Species>(413),		//  Bastiodon
		ID<Species>(414),		//  Burmy
		ID<Species>(415),		//  Wormadam-Plant
		ID<Species>(416),		//  Wormadam-Sandy
		ID<Species>(417),		//  Wormadam-Trash
		ID<Species>(418),		//  Mothim
		ID<Species>(419),		//  Combee
		ID<Species>(420),		//  Vespiquen
		ID<Species>(421),		//  Pachirisu
		ID<Species>(422),		//  Buizel
		ID<Species>(423),		//  Floatzel
		ID<Species>(424),		//  Cherubi
		ID<Species>(425),		//  Cherrim
		ID<Species>(426),		//  Shellos
		ID<Species>(427),		//  Gastrodon
		ID<Species>(428),		//  Ambipom
		ID<Species>(429),		//  Drifloon
		ID<Species>(430),		//  Drifblim
		ID<Species>(431),		//  Buneary
		ID<Species>(432),		//  Lopunny
		ID<Species>(433),		//  Mismagius
		ID<Species>(434),		//  Honchkrow
		ID<Species>(435),		//  Glameow
		ID<Species>(436),		//  Purugly
		ID<Species>(437),		//  Chingling
		ID<Species>(438),		//  Stunky
		ID<Species>(439),		//  Skuntank
		ID<Species>(440),		//  Bronzor
		ID<Species>(441),		//  Bronzong
		ID<Species>(442),		//  Bonsly
		ID<Species>(443),		//  Mime Jr.
		ID<Species>(444),		//  Happiny
		ID<Species>(445),		//  Chatot
		ID<Species>(446),		//  Spiritomb
		ID<Species>(447),		//  Gible
		ID<Species>(448),		//  Gabite
		ID<Species>(449),		//  Garchomp
		ID<Species>(450),		//  Munchlax
		ID<Species>(451),		//  Riolu
		ID<Species>(452),		//  Lucario
		ID<Species>(453),		//  Hippopotas
		ID<Species>(454),		//  Hippowdon
		ID<Species>(455),		//  Skorupi
		ID<Species>(456),		//  Drapion
		ID<Species>(457),		//  Croagunk
		ID<Species>(458),		//  Toxicroak
		ID<Species>(459),		//  Carnivine
		ID<Species>(460),		//  Finneon
		ID<Species>(461),		//  Lumineon
		ID<Species>(462),		//  Mantyke
		ID<Species>(463),		//  Snover
		ID<Species>(464),		//  Abomasnow
		ID<Species>(465),		//  Weavile
		ID<Species>(466),		//  Magnezone
		ID<Species>(467),		//  Lickilicky
		ID<Species>(468),		//  Rhyperior
		ID<Species>(469),		//  Tangrowth
		ID<Species>(470),		//  Electivire
		ID<Species>(471),		//  Magmortar
		ID<Species>(472),		//  Togekiss
		ID<Species>(473),		//  Yanmega
		ID<Species>(474),		//  Leafeon
		ID<Species>(475),		//  Glaceon
		ID<Species>(476),		//  Gliscor
		ID<Species>(477),		//  Mamoswine
		ID<Species>(478),		//  Porygon-Z
		ID<Species>(479),		//  Gallade
		ID<Species>(480),		//  Probopass
		ID<Species>(481),		//  Dusknoir
		ID<Species>(482),		//  Froslass
		ID<Species>(483),		//  Rotom
		ID<Species>(500),		//  Rotom-Heat
		ID<Species>(502),		//  Rotom-Wash
		ID<Species>(498),		//  Rotom-Frost
		ID<Species>(501),		//  Rotom-Fan
		ID<Species>(499),		//  Rotom-Mow
		ID<Species>(484),		//  Uxie
		ID<Species>(485),		//  Mesprit
		ID<Species>(486),		//  Azelf
		ID<Species>(487),		//  Dialga
		ID<Species>(488),		//  Palkia
		ID<Species>(489),		//  Heatran
		ID<Species>(490),		//  Regigigas
		ID<Species>(491),		//  Giratina-Altered
		ID<Species>(503),		//  Giratina-Origin
		ID<Species>(492),		//  Cresselia
		ID<Species>(493),		//  Phione
		ID<Species>(494),		//  Manaphy
		ID<Species>(495),		//  Darkrai
		ID<Species>(496),		//  Shaymin-Land
		ID<Species>(504),		//  Shaymin-Sky
		ID<Species>(497)		// Arceus
	};
	return species_converter[static_cast<unsigned>(species)];
}

}	// namespace pl
}	// namespace technicalmachine
