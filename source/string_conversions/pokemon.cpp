// Pokemon string functions
// Copyright (C) 2015 David Stone
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

#include "pokemon.hpp"

#include <algorithm>
#include <map>

#include <boost/algorithm/string/case_conv.hpp>

#include "invalid_string_conversion.hpp"

#include "../pokemon/species.hpp"

namespace technicalmachine {

std::string const & to_string(Species const name) {
	static auto const name_to_string = bounded::make_array<std::string>(
		// Generation 1
		"Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon",
		"Charizard", "Squirtle", "Wartortle", "Blastoise", "Caterpie",
		"Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill",
		"Pidgey", "Pidgeotto", "Pidgeot", "Rattata", "Raticate",
		"Spearow", "Fearow", "Ekans", "Arbok", "Pikachu",
		"Raichu", "Sandshrew", "Sandslash", "Nidoran-F", "Nidorina",
		"Nidoqueen", "Nidoran-M", "Nidorino", "Nidoking", "Clefairy",
		"Clefable", "Vulpix", "Ninetales", "Jigglypuff", "Wigglytuff",
		"Zubat", "Golbat", "Oddish", "Gloom", "Vileplume",
		"Paras", "Parasect", "Venonat", "Venomoth", "Diglett",
		"Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck",
		"Mankey", "Primeape", "Growlithe", "Arcanine", "Poliwag",
		"Poliwhirl", "Poliwrath", "Abra", "Kadabra", "Alakazam",
		"Machop", "Machoke", "Machamp", "Bellsprout", "Weepinbell",
		"Victreebel", "Tentacool", "Tentacruel", "Geodude", "Graveler",
		"Golem", "Ponyta", "Rapidash", "Slowpoke", "Slowbro",
		"Magnemite", "Magneton", "Farfetch'd", "Doduo", "Dodrio",
		"Seel", "Dewgong", "Grimer", "Muk", "Shellder",
		"Cloyster", "Gastly", "Haunter", "Gengar", "Onix",
		"Drowzee", "Hypno", "Krabby", "Kingler", "Voltorb",
		"Electrode", "Exeggcute", "Exeggutor", "Cubone", "Marowak",
		"Hitmonlee", "Hitmonchan", "Lickitung", "Koffing", "Weezing",
		"Rhyhorn", "Rhydon", "Chansey", "Tangela", "Kangaskhan",
		"Horsea", "Seadra", "Goldeen", "Seaking", "Staryu",
		"Starmie", "Mr. Mime", "Scyther", "Jynx", "Electabuzz",
		"Magmar", "Pinsir", "Tauros", "Magikarp", "Gyarados",
		"Lapras", "Ditto", "Eevee", "Vaporeon", "Jolteon",
		"Flareon", "Porygon", "Omanyte", "Omastar", "Kabuto",
		"Kabutops", "Aerodactyl", "Snorlax", "Articuno", "Zapdos",
		"Moltres", "Dratini", "Dragonair", "Dragonite", "Mewtwo",
		"Mew",

		// Generation 2
		"Chikorita", "Bayleef", "Meganium", "Cyndaquil", "Quilava",
		"Typhlosion", "Totodile", "Croconaw", "Feraligatr", "Sentret",
		"Furret", "Hoothoot", "Noctowl", "Ledyba", "Ledian",
		"Spinarak", "Ariados", "Crobat", "Chinchou", "Lanturn",
		"Pichu", "Cleffa", "Igglybuff", "Togepi", "Togetic",
		"Natu", "Xatu", "Mareep", "Flaaffy", "Ampharos",
		"Bellossom", "Marill", "Azumarill", "Sudowoodo", "Politoed",
		"Hoppip", "Skiploom", "Jumpluff", "Aipom", "Sunkern",
		"Sunflora", "Yanma", "Wooper", "Quagsire", "Espeon",
		"Umbreon", "Murkrow", "Slowking", "Misdreavus", "Unown",
		"Wobbuffet", "Girafarig", "Pineco", "Forretress", "Dunsparce",
		"Gligar", "Steelix", "Snubbull", "Granbull", "Qwilfish",
		"Scizor", "Shuckle", "Heracross", "Sneasel", "Teddiursa",
		"Ursaring", "Slugma", "Magcargo", "Swinub", "Piloswine",
		"Corsola", "Remoraid", "Octillery", "Delibird", "Mantine",
		"Skarmory", "Houndour", "Houndoom", "Kingdra", "Phanpy",
		"Donphan", "Porygon2", "Stantler", "Smeargle", "Tyrogue",
		"Hitmontop", "Smoochum", "Elekid", "Magby", "Miltank",
		"Blissey", "Raikou", "Entei", "Suicune", "Larvitar",
		"Pupitar", "Tyranitar", "Lugia", "Ho-Oh", "Celebi",

		// Generation 3
		"Treecko", "Grovyle", "Sceptile", "Torchic",
		"Combusken", "Blaziken", "Mudkip", "Marshtomp", "Swampert",
		"Poochyena", "Mightyena", "Zigzagoon", "Linoone", "Wurmple",
		"Silcoon", "Beautifly", "Cascoon", "Dustox", "Lotad",
		"Lombre", "Ludicolo", "Seedot", "Nuzleaf", "Shiftry",
		"Taillow", "Swellow", "Wingull", "Pelipper", "Ralts",
		"Kirlia", "Gardevoir", "Surskit", "Masquerain", "Shroomish",
		"Breloom", "Slakoth", "Vigoroth", "Slaking", "Nincada",
		"Ninjask", "Shedinja", "Whismur", "Loudred", "Exploud",
		"Makuhita", "Hariyama", "Azurill", "Nosepass", "Skitty",
		"Delcatty", "Sableye", "Mawile", "Aron", "Lairon",
		"Aggron", "Meditite", "Medicham", "Electrike", "Manectric",
		"Plusle", "Minun", "Volbeat", "Illumise", "Roselia",
		"Gulpin", "Swalot", "Carvanha", "Sharpedo", "Wailmer",
		"Wailord", "Numel", "Camerupt", "Torkoal", "Spoink",
		"Grumpig", "Spinda", "Trapinch", "Vibrava", "Flygon",
		"Cacnea", "Cacturne", "Swablu", "Altaria", "Zangoose",
		"Seviper", "Lunatone", "Solrock", "Barboach", "Whiscash",
		"Corphish", "Crawdaunt", "Baltoy", "Claydol", "Lileep",
		"Cradily", "Anorith", "Armaldo", "Feebas", "Milotic",
		"Castform", "Kecleon", "Shuppet", "Banette", "Duskull",
		"Dusclops", "Tropius", "Chimecho", "Absol", "Wynaut",
		"Snorunt", "Glalie", "Spheal", "Sealeo", "Walrein",
		"Clamperl", "Huntail", "Gorebyss", "Relicanth", "Luvdisc",
		"Bagon", "Shelgon", "Salamence", "Beldum", "Metang",
		"Metagross", "Regirock", "Regice", "Registeel", "Latias",
		"Latios", "Kyogre", "Groudon", "Rayquaza", "Jirachi",
		"Deoxys-Mediocre", "Deoxys-Attack", "Deoxys-Defense", "Deoxys-Speed",

		// Generation 4
		"Turtwig", "Grotle", "Torterra", "Chimchar", "Monferno",
		"Infernape", "Piplup", "Prinplup", "Empoleon", "Starly",
		"Staravia", "Staraptor", "Bidoof", "Bibarel", "Kricketot",
		"Kricketune", "Shinx", "Luxio", "Luxray", "Budew",
		"Roserade", "Cranidos", "Rampardos", "Shieldon", "Bastiodon",
		"Burmy", "Wormadam-Plant", "Wormadam-Sandy", "Wormadam-Trash", "Mothim",
		"Combee", "Vespiquen", "Pachirisu", "Buizel", "Floatzel",
		"Cherubi", "Cherrim", "Shellos", "Gastrodon", "Ambipom",
		"Drifloon", "Drifblim", "Buneary", "Lopunny", "Mismagius",
		"Honchkrow", "Glameow", "Purugly", "Chingling", "Stunky",
		"Skuntank", "Bronzor", "Bronzong", "Bonsly", "Mime Jr.",
		"Happiny", "Chatot", "Spiritomb", "Gible", "Gabite",
		"Garchomp", "Munchlax", "Riolu", "Lucario", "Hippopotas",
		"Hippowdon", "Skorupi", "Drapion", "Croagunk", "Toxicroak",
		"Carnivine", "Finneon", "Lumineon", "Mantyke", "Snover",
		"Abomasnow", "Weavile", "Magnezone", "Lickilicky", "Rhyperior",
		"Tangrowth", "Electivire", "Magmortar", "Togekiss", "Yanmega",
		"Leafeon", "Glaceon", "Gliscor", "Mamoswine", "Porygon-Z",
		"Gallade", "Probopass", "Dusknoir", "Froslass", "Rotom",
		"Rotom-Heat", "Rotom-Wash", "Rotom-Frost", "Rotom-Fan", "Rotom-Mow",
		"Uxie", "Mesprit", "Azelf", "Dialga", "Palkia",
		"Heatran", "Regigigas", "Giratina-Altered", "Giratina-Origin", "Cresselia",
		"Phione", "Manaphy", "Darkrai", "Shaymin-Land", "Shaymin-Sky",
		"Arceus",

		// Generation 5
		"Victini", "Snivy", "Servine", "Serperior", "Tepig",
		"Pignite", "Emboar", "Oshawott", "Dewott", "Samurott",
		"Patrat", "Watchog", "Lillipup", "Herdier", "Stoutland",
		"Purrloin", "Liepard", "Pansage", "Simisage", "Pansear",
		"Simisear", "Panpour", "Simipour", "Munna", "Musharna",
		"Pidove", "Tranquill", "Unfezant", "Blitzle", "Zebstrika",
		"Roggenrola", "Boldore", "Gigalith", "Woobat", "Swoobat",
		"Drilbur", "Excadrill", "Audino", "Timburr", "Gurdurr",
		"Conkeldurr", "Tympole", "Palpitoad", "Seismitoad", "Throh",
		"Sawk", "Sewaddle", "Swadloon", "Leavanny", "Venipede",
		"Whirlipede", "Scolipede", "Cottonee", "Whimsicott", "Petilil",
		"Lilligant", "Basculin-Red", "Basculin-Blue", "Sandile", "Krokorok",
		"Krookodile", "Darumaka", "Darmanitan", "Maractus", "Dwebble",
		"Crustle", "Scraggy", "Scrafty", "Sigilyph", "Yamask",
		"Cofagrigus", "Tirtouga", "Carracosta", "Archen", "Archeops",
		"Trubbish", "Garbodor", "Zorua", "Zoroark", "Minccino",
		"Cinccino", "Gothita", "Gothorita", "Gothitelle", "Solosis",
		"Duosion", "Reuniclus", "Ducklett", "Swanna", "Vanillite",
		"Vanillish", "Vanilluxe", "Deerling", "Sawsbuck", "Emolga",
		"Karrablast", "Escavalier", "Foongus", "Amoonguss", "Frillish",
		"Jellicent", "Alomomola", "Joltik", "Galvantula", "Ferroseed",
		"Ferrothorn", "Klink", "Klang", "Klinklang", "Tynamo",
		"Eelektrik", "Eelektross", "Elgyem", "Beheeyem", "Litwick",
		"Lampent", "Chandelure", "Axew", "Fraxure", "Haxorus",
		"Cubchoo", "Beartic", "Cryogonal", "Shelmet", "Accelgor",
		"Stunfisk", "Mienfoo", "Mienshao", "Druddigon", "Golett",
		"Golurk", "Pawniard", "Bisharp", "Bouffalant", "Rufflet",
		"Braviary", "Vullaby", "Mandibuzz", "Heatmor", "Durant",
		"Deino", "Zweilous", "Hydreigon", "Larvesta", "Volcarona",
		"Cobalion", "Terrakion", "Virizion", "Tornadus-Incarnate", "Tornadus-Therian",
		"Thundurus-Incarnate", "Thundurus-Therian", "Reshiram", "Zekrom", "Landorus-Incarnate",
		"Landorus-Therian", "Kyurem", "Kyurem-Black", "Kyurem-White", "Keldeo",
		"Meloetta", "Genesect",
		
		"END"
	);
	return name_to_string.at(name);
}

template<>
Species from_string(boost::string_ref const str) {
	static std::map<boost::string_ref, Species> const converter {
		// Generation 1
		{ "bulbasaur", Species::Bulbasaur },
		{ "ivysaur", Species::Ivysaur },
		{ "venusaur", Species::Venusaur },
		{ "charmander", Species::Charmander },
		{ "charmeleon", Species::Charmeleon },
		{ "charizard", Species::Charizard },
		{ "squirtle", Species::Squirtle },
		{ "wartortle", Species::Wartortle },
		{ "blastoise", Species::Blastoise },
		{ "caterpie", Species::Caterpie },
		{ "metapod", Species::Metapod },
		{ "butterfree", Species::Butterfree },
		{ "weedle", Species::Weedle },
		{ "kakuna", Species::Kakuna },
		{ "beedrill", Species::Beedrill },
		{ "pidgey", Species::Pidgey },
		{ "pidgeotto", Species::Pidgeotto },
		{ "pidgeot", Species::Pidgeot },
		{ "rattata", Species::Rattata },
		{ "raticate", Species::Raticate },
		{ "spearow", Species::Spearow },
		{ "fearow", Species::Fearow },
		{ "ekans", Species::Ekans },
		{ "arbok", Species::Arbok },
		{ "pikachu", Species::Pikachu },
		{ "raichu", Species::Raichu },
		{ "sandshrew", Species::Sandshrew },
		{ "sandslash", Species::Sandslash },
		{ "nidoran-f", Species::Nidoran_F },
		{ "nidorina", Species::Nidorina },
		{ "nidoqueen", Species::Nidoqueen },
		{ "nidoran-m", Species::Nidoran_M },
		{ "nidorino", Species::Nidorino },
		{ "nidoking", Species::Nidoking },
		{ "clefairy", Species::Clefairy },
		{ "clefable", Species::Clefable },
		{ "vulpix", Species::Vulpix },
		{ "ninetales", Species::Ninetales },
		{ "jigglypuff", Species::Jigglypuff },
		{ "wigglytuff", Species::Wigglytuff },
		{ "zubat", Species::Zubat },
		{ "golbat", Species::Golbat },
		{ "oddish", Species::Oddish },
		{ "gloom", Species::Gloom },
		{ "vileplume", Species::Vileplume },
		{ "paras", Species::Paras },
		{ "parasect", Species::Parasect },
		{ "venonat", Species::Venonat },
		{ "venomoth", Species::Venomoth },
		{ "diglett", Species::Diglett },
		{ "dugtrio", Species::Dugtrio },
		{ "meowth", Species::Meowth },
		{ "persian", Species::Persian },
		{ "psyduck", Species::Psyduck },
		{ "golduck", Species::Golduck },
		{ "mankey", Species::Mankey },
		{ "primeape", Species::Primeape },
		{ "growlithe", Species::Growlithe },
		{ "arcanine", Species::Arcanine },
		{ "poliwag", Species::Poliwag },
		{ "poliwhirl", Species::Poliwhirl },
		{ "poliwrath", Species::Poliwrath },
		{ "abra", Species::Abra },
		{ "kadabra", Species::Kadabra },
		{ "alakazam", Species::Alakazam },
		{ "machop", Species::Machop },
		{ "machoke", Species::Machoke },
		{ "machamp", Species::Machamp },
		{ "bellsprout", Species::Bellsprout },
		{ "weepinbell", Species::Weepinbell },
		{ "victreebel", Species::Victreebel },
		{ "tentacool", Species::Tentacool },
		{ "tentacruel", Species::Tentacruel },
		{ "geodude", Species::Geodude },
		{ "graveler", Species::Graveler },
		{ "golem", Species::Golem },
		{ "ponyta", Species::Ponyta },
		{ "rapidash", Species::Rapidash },
		{ "slowpoke", Species::Slowpoke },
		{ "slowbro", Species::Slowbro },
		{ "magnemite", Species::Magnemite },
		{ "magneton", Species::Magneton },
		{ "farfetch'd", Species::Farfetchd },
		{ "farfetchd", Species::Farfetchd },
		{ "doduo", Species::Doduo },
		{ "dodrio", Species::Dodrio },
		{ "seel", Species::Seel },
		{ "dewgong", Species::Dewgong },
		{ "grimer", Species::Grimer },
		{ "muk", Species::Muk },
		{ "shellder", Species::Shellder },
		{ "cloyster", Species::Cloyster },
		{ "gastly", Species::Gastly },
		{ "haunter", Species::Haunter },
		{ "gengar", Species::Gengar },
		{ "onix", Species::Onix },
		{ "drowzee", Species::Drowzee },
		{ "hypno", Species::Hypno },
		{ "krabby", Species::Krabby },
		{ "kingler", Species::Kingler },
		{ "voltorb", Species::Voltorb },
		{ "electrode", Species::Electrode },
		{ "exeggcute", Species::Exeggcute },
		{ "exeggutor", Species::Exeggutor },
		{ "cubone", Species::Cubone },
		{ "marowak", Species::Marowak },
		{ "hitmonlee", Species::Hitmonlee },
		{ "hitmonchan", Species::Hitmonchan },
		{ "lickitung", Species::Lickitung },
		{ "koffing", Species::Koffing },
		{ "weezing", Species::Weezing },
		{ "rhyhorn", Species::Rhyhorn },
		{ "rhydon", Species::Rhydon },
		{ "chansey", Species::Chansey },
		{ "tangela", Species::Tangela },
		{ "kangaskhan", Species::Kangaskhan },
		{ "horsea", Species::Horsea },
		{ "seadra", Species::Seadra },
		{ "goldeen", Species::Goldeen },
		{ "seaking", Species::Seaking },
		{ "staryu", Species::Staryu },
		{ "starmie", Species::Starmie },
		{ "mr. mime", Species::Mr_Mime },
		{ "mr mime", Species::Mr_Mime },
		{ "scyther", Species::Scyther },
		{ "jynx", Species::Jynx },
		{ "electabuzz", Species::Electabuzz },
		{ "magmar", Species::Magmar },
		{ "pinsir", Species::Pinsir },
		{ "tauros", Species::Tauros },
		{ "magikarp", Species::Magikarp },
		{ "gyarados", Species::Gyarados },
		{ "lapras", Species::Lapras },
		{ "ditto", Species::Ditto },
		{ "eevee", Species::Eevee },
		{ "vaporeon", Species::Vaporeon },
		{ "jolteon", Species::Jolteon },
		{ "flareon", Species::Flareon },
		{ "porygon", Species::Porygon },
		{ "omanyte", Species::Omanyte },
		{ "omastar", Species::Omastar },
		{ "kabuto", Species::Kabuto },
		{ "kabutops", Species::Kabutops },
		{ "aerodactyl", Species::Aerodactyl },
		{ "snorlax", Species::Snorlax },
		{ "articuno", Species::Articuno },
		{ "zapdos", Species::Zapdos },
		{ "moltres", Species::Moltres },
		{ "dratini", Species::Dratini },
		{ "dragonair", Species::Dragonair },
		{ "dragonite", Species::Dragonite },
		{ "mewtwo", Species::Mewtwo },
		{ "mew", Species::Mew },

		// Generation 2	
		{ "chikorita", Species::Chikorita },
		{ "bayleef", Species::Bayleef },
		{ "meganium", Species::Meganium },
		{ "cyndaquil", Species::Cyndaquil },
		{ "quilava", Species::Quilava },
		{ "typhlosion", Species::Typhlosion },
		{ "totodile", Species::Totodile },
		{ "croconaw", Species::Croconaw },
		{ "feraligatr", Species::Feraligatr },
		{ "sentret", Species::Sentret },
		{ "furret", Species::Furret },
		{ "hoothoot", Species::Hoothoot },
		{ "noctowl", Species::Noctowl },
		{ "ledyba", Species::Ledyba },
		{ "ledian", Species::Ledian },
		{ "spinarak", Species::Spinarak },
		{ "ariados", Species::Ariados },
		{ "crobat", Species::Crobat },
		{ "chinchou", Species::Chinchou },
		{ "lanturn", Species::Lanturn },
		{ "pichu", Species::Pichu },
		{ "cleffa", Species::Cleffa },
		{ "igglybuff", Species::Igglybuff },
		{ "togepi", Species::Togepi },
		{ "togetic", Species::Togetic },
		{ "natu", Species::Natu },
		{ "xatu", Species::Xatu },
		{ "mareep", Species::Mareep },
		{ "flaaffy", Species::Flaaffy },
		{ "ampharos", Species::Ampharos },
		{ "bellossom", Species::Bellossom },
		{ "marill", Species::Marill },
		{ "azumarill", Species::Azumarill },
		{ "sudowoodo", Species::Sudowoodo },
		{ "politoed", Species::Politoed },
		{ "hoppip", Species::Hoppip },
		{ "skiploom", Species::Skiploom },
		{ "jumpluff", Species::Jumpluff },
		{ "aipom", Species::Aipom },
		{ "sunkern", Species::Sunkern },
		{ "sunflora", Species::Sunflora },
		{ "yanma", Species::Yanma },
		{ "wooper", Species::Wooper },
		{ "quagsire", Species::Quagsire },
		{ "espeon", Species::Espeon },
		{ "umbreon", Species::Umbreon },
		{ "murkrow", Species::Murkrow },
		{ "slowking", Species::Slowking },
		{ "misdreavus", Species::Misdreavus },
		{ "unown", Species::Unown },
		{ "wobbuffet", Species::Wobbuffet },
		{ "girafarig", Species::Girafarig },
		{ "pineco", Species::Pineco },
		{ "forretress", Species::Forretress },
		{ "dunsparce", Species::Dunsparce },
		{ "gligar", Species::Gligar },
		{ "steelix", Species::Steelix },
		{ "snubbull", Species::Snubbull },
		{ "granbull", Species::Granbull },
		{ "qwilfish", Species::Qwilfish },
		{ "scizor", Species::Scizor },
		{ "shuckle", Species::Shuckle },
		{ "heracross", Species::Heracross },
		{ "sneasel", Species::Sneasel },
		{ "teddiursa", Species::Teddiursa },
		{ "ursaring", Species::Ursaring },
		{ "slugma", Species::Slugma },
		{ "magcargo", Species::Magcargo },
		{ "swinub", Species::Swinub },
		{ "piloswine", Species::Piloswine },
		{ "corsola", Species::Corsola },
		{ "remoraid", Species::Remoraid },
		{ "octillery", Species::Octillery },
		{ "delibird", Species::Delibird },
		{ "mantine", Species::Mantine },
		{ "skarmory", Species::Skarmory },
		{ "houndour", Species::Houndour },
		{ "houndoom", Species::Houndoom },
		{ "kingdra", Species::Kingdra },
		{ "phanpy", Species::Phanpy },
		{ "donphan", Species::Donphan },
		{ "porygon2", Species::Porygon2 },
		{ "stantler", Species::Stantler },
		{ "smeargle", Species::Smeargle },
		{ "tyrogue", Species::Tyrogue },
		{ "hitmontop", Species::Hitmontop },
		{ "smoochum", Species::Smoochum },
		{ "elekid", Species::Elekid },
		{ "magby", Species::Magby },
		{ "miltank", Species::Miltank },
		{ "blissey", Species::Blissey },
		{ "raikou", Species::Raikou },
		{ "entei", Species::Entei },
		{ "suicune", Species::Suicune },
		{ "larvitar", Species::Larvitar },
		{ "pupitar", Species::Pupitar },
		{ "tyranitar", Species::Tyranitar },
		{ "lugia", Species::Lugia },
		{ "ho-oh", Species::Ho_Oh },
		{ "celebi", Species::Celebi },

		// Generation 3	
		{ "treecko", Species::Treecko },
		{ "grovyle", Species::Grovyle },
		{ "sceptile", Species::Sceptile },
		{ "torchic", Species::Torchic },
		{ "combusken", Species::Combusken },
		{ "blaziken", Species::Blaziken },
		{ "mudkip", Species::Mudkip },
		{ "marshtomp", Species::Marshtomp },
		{ "swampert", Species::Swampert },
		{ "poochyena", Species::Poochyena },
		{ "mightyena", Species::Mightyena },
		{ "zigzagoon", Species::Zigzagoon },
		{ "linoone", Species::Linoone },
		{ "wurmple", Species::Wurmple },
		{ "silcoon", Species::Silcoon },
		{ "beautifly", Species::Beautifly },
		{ "cascoon", Species::Cascoon },
		{ "dustox", Species::Dustox },
		{ "lotad", Species::Lotad },
		{ "lombre", Species::Lombre },
		{ "ludicolo", Species::Ludicolo },
		{ "seedot", Species::Seedot },
		{ "nuzleaf", Species::Nuzleaf },
		{ "shiftry", Species::Shiftry },
		{ "taillow", Species::Taillow },
		{ "swellow", Species::Swellow },
		{ "wingull", Species::Wingull },
		{ "pelipper", Species::Pelipper },
		{ "ralts", Species::Ralts },
		{ "kirlia", Species::Kirlia },
		{ "gardevoir", Species::Gardevoir },
		{ "surskit", Species::Surskit },
		{ "masquerain", Species::Masquerain },
		{ "shroomish", Species::Shroomish },
		{ "breloom", Species::Breloom },
		{ "slakoth", Species::Slakoth },
		{ "vigoroth", Species::Vigoroth },
		{ "slaking", Species::Slaking },
		{ "nincada", Species::Nincada },
		{ "ninjask", Species::Ninjask },
		{ "shedinja", Species::Shedinja },
		{ "whismur", Species::Whismur },
		{ "loudred", Species::Loudred },
		{ "exploud", Species::Exploud },
		{ "makuhita", Species::Makuhita },
		{ "hariyama", Species::Hariyama },
		{ "azurill", Species::Azurill },
		{ "nosepass", Species::Nosepass },
		{ "skitty", Species::Skitty },
		{ "delcatty", Species::Delcatty },
		{ "sableye", Species::Sableye },
		{ "mawile", Species::Mawile },
		{ "aron", Species::Aron },
		{ "lairon", Species::Lairon },
		{ "aggron", Species::Aggron },
		{ "meditite", Species::Meditite },
		{ "medicham", Species::Medicham },
		{ "electrike", Species::Electrike },
		{ "manectric", Species::Manectric },
		{ "plusle", Species::Plusle },
		{ "minun", Species::Minun },
		{ "volbeat", Species::Volbeat },
		{ "illumise", Species::Illumise },
		{ "roselia", Species::Roselia },
		{ "gulpin", Species::Gulpin },
		{ "swalot", Species::Swalot },
		{ "carvanha", Species::Carvanha },
		{ "sharpedo", Species::Sharpedo },
		{ "wailmer", Species::Wailmer },
		{ "wailord", Species::Wailord },
		{ "numel", Species::Numel },
		{ "camerupt", Species::Camerupt },
		{ "torkoal", Species::Torkoal },
		{ "spoink", Species::Spoink },
		{ "grumpig", Species::Grumpig },
		{ "spinda", Species::Spinda },
		{ "trapinch", Species::Trapinch },
		{ "vibrava", Species::Vibrava },
		{ "flygon", Species::Flygon },
		{ "cacnea", Species::Cacnea },
		{ "cacturne", Species::Cacturne },
		{ "swablu", Species::Swablu },
		{ "altaria", Species::Altaria },
		{ "zangoose", Species::Zangoose },
		{ "seviper", Species::Seviper },
		{ "lunatone", Species::Lunatone },
		{ "solrock", Species::Solrock },
		{ "barboach", Species::Barboach },
		{ "whiscash", Species::Whiscash },
		{ "corphish", Species::Corphish },
		{ "crawdaunt", Species::Crawdaunt },
		{ "baltoy", Species::Baltoy },
		{ "claydol", Species::Claydol },
		{ "lileep", Species::Lileep },
		{ "cradily", Species::Cradily },
		{ "anorith", Species::Anorith },
		{ "armaldo", Species::Armaldo },
		{ "feebas", Species::Feebas },
		{ "milotic", Species::Milotic },
		{ "castform", Species::Castform },
		{ "kecleon", Species::Kecleon },
		{ "shuppet", Species::Shuppet },
		{ "banette", Species::Banette },
		{ "duskull", Species::Duskull },
		{ "dusclops", Species::Dusclops },
		{ "tropius", Species::Tropius },
		{ "chimecho", Species::Chimecho },
		{ "absol", Species::Absol },
		{ "wynaut", Species::Wynaut },
		{ "snorunt", Species::Snorunt },
		{ "glalie", Species::Glalie },
		{ "spheal", Species::Spheal },
		{ "sealeo", Species::Sealeo },
		{ "walrein", Species::Walrein },
		{ "clamperl", Species::Clamperl },
		{ "huntail", Species::Huntail },
		{ "gorebyss", Species::Gorebyss },
		{ "relicanth", Species::Relicanth },
		{ "luvdisc", Species::Luvdisc },
		{ "bagon", Species::Bagon },
		{ "shelgon", Species::Shelgon },
		{ "salamence", Species::Salamence },
		{ "beldum", Species::Beldum },
		{ "metang", Species::Metang },
		{ "metagross", Species::Metagross },
		{ "regirock", Species::Regirock },
		{ "regice", Species::Regice },
		{ "registeel", Species::Registeel },
		{ "latias", Species::Latias },
		{ "latios", Species::Latios },
		{ "kyogre", Species::Kyogre },
		{ "groudon", Species::Groudon },
		{ "rayquaza", Species::Rayquaza },
		{ "jirachi", Species::Jirachi },
		{ "deoxys-mediocre", Species::Deoxys_Mediocre },
		{ "deoxys-attack", Species::Deoxys_Attack },
		{ "deoxys-defense", Species::Deoxys_Defense },
		{ "deoxys-speed", Species::Deoxys_Speed },

		// Generation 4	
		{ "turtwig", Species::Turtwig },
		{ "grotle", Species::Grotle },
		{ "torterra", Species::Torterra },
		{ "chimchar", Species::Chimchar },
		{ "monferno", Species::Monferno },
		{ "infernape", Species::Infernape },
		{ "piplup", Species::Piplup },
		{ "prinplup", Species::Prinplup },
		{ "empoleon", Species::Empoleon },
		{ "starly", Species::Starly },
		{ "staravia", Species::Staravia },
		{ "staraptor", Species::Staraptor },
		{ "bidoof", Species::Bidoof },
		{ "bibarel", Species::Bibarel },
		{ "kricketot", Species::Kricketot },
		{ "kricketune", Species::Kricketune },
		{ "shinx", Species::Shinx },
		{ "luxio", Species::Luxio },
		{ "luxray", Species::Luxray },
		{ "budew", Species::Budew },
		{ "roserade", Species::Roserade },
		{ "cranidos", Species::Cranidos },
		{ "rampardos", Species::Rampardos },
		{ "shieldon", Species::Shieldon },
		{ "bastiodon", Species::Bastiodon },
		{ "burmy", Species::Burmy },
		{ "wormadam-plant", Species::Wormadam_Plant },
		{ "wormadam-sandy", Species::Wormadam_Sandy },
		{ "wormadam-trash", Species::Wormadam_Trash },
		{ "mothim", Species::Mothim },
		{ "combee", Species::Combee },
		{ "vespiquen", Species::Vespiquen },
		{ "pachirisu", Species::Pachirisu },
		{ "buizel", Species::Buizel },
		{ "floatzel", Species::Floatzel },
		{ "cherubi", Species::Cherubi },
		{ "cherrim", Species::Cherrim },
		{ "shellos", Species::Shellos },
		{ "gastrodon", Species::Gastrodon },
		{ "ambipom", Species::Ambipom },
		{ "drifloon", Species::Drifloon },
		{ "drifblim", Species::Drifblim },
		{ "buneary", Species::Buneary },
		{ "lopunny", Species::Lopunny },
		{ "mismagius", Species::Mismagius },
		{ "honchkrow", Species::Honchkrow },
		{ "glameow", Species::Glameow },
		{ "purugly", Species::Purugly },
		{ "chingling", Species::Chingling },
		{ "stunky", Species::Stunky },
		{ "skuntank", Species::Skuntank },
		{ "bronzor", Species::Bronzor },
		{ "bronzong", Species::Bronzong },
		{ "bonsly", Species::Bonsly },
		{ "mime jr.", Species::Mime_Jr },
		{ "mime jr", Species::Mime_Jr },
		{ "happiny", Species::Happiny },
		{ "chatot", Species::Chatot },
		{ "spiritomb", Species::Spiritomb },
		{ "gible", Species::Gible },
		{ "gabite", Species::Gabite },
		{ "garchomp", Species::Garchomp },
		{ "munchlax", Species::Munchlax },
		{ "riolu", Species::Riolu },
		{ "lucario", Species::Lucario },
		{ "hippopotas", Species::Hippopotas },
		{ "hippowdon", Species::Hippowdon },
		{ "skorupi", Species::Skorupi },
		{ "drapion", Species::Drapion },
		{ "croagunk", Species::Croagunk },
		{ "toxicroak", Species::Toxicroak },
		{ "carnivine", Species::Carnivine },
		{ "finneon", Species::Finneon },
		{ "lumineon", Species::Lumineon },
		{ "mantyke", Species::Mantyke },
		{ "snover", Species::Snover },
		{ "abomasnow", Species::Abomasnow },
		{ "weavile", Species::Weavile },
		{ "magnezone", Species::Magnezone },
		{ "lickilicky", Species::Lickilicky },
		{ "rhyperior", Species::Rhyperior },
		{ "tangrowth", Species::Tangrowth },
		{ "electivire", Species::Electivire },
		{ "magmortar", Species::Magmortar },
		{ "togekiss", Species::Togekiss },
		{ "yanmega", Species::Yanmega },
		{ "leafeon", Species::Leafeon },
		{ "glaceon", Species::Glaceon },
		{ "gliscor", Species::Gliscor },
		{ "mamoswine", Species::Mamoswine },
		{ "porygon-z", Species::Porygon_Z },
		{ "gallade", Species::Gallade },
		{ "probopass", Species::Probopass },
		{ "dusknoir", Species::Dusknoir },
		{ "froslass", Species::Froslass },
		{ "rotom", Species::Rotom },
		{ "rotom-heat", Species::Rotom_Heat },
		{ "rotom-wash", Species::Rotom_Wash },
		{ "rotom-frost", Species::Rotom_Frost },
		{ "rotom-fan", Species::Rotom_Fan },
		{ "rotom-mow", Species::Rotom_Mow },
		{ "uxie", Species::Uxie },
		{ "mesprit", Species::Mesprit },
		{ "azelf", Species::Azelf },
		{ "dialga", Species::Dialga },
		{ "palkia", Species::Palkia },
		{ "heatran", Species::Heatran },
		{ "regigigas", Species::Regigigas },
		{ "giratina-altered", Species::Giratina_Altered },
		{ "giratina-origin", Species::Giratina_Origin },
		{ "cresselia", Species::Cresselia },
		{ "phione", Species::Phione },
		{ "manaphy", Species::Manaphy },
		{ "darkrai", Species::Darkrai },
		{ "shaymin-land", Species::Shaymin_Land },
		{ "shaymin-sky", Species::Shaymin_Sky },
		{ "arceus", Species::Arceus },
		{ "arceus-bug", Species::Arceus },
		{ "arceus-dark", Species::Arceus },
		{ "arceus-dragon", Species::Arceus },
		{ "arceus-electric", Species::Arceus },
		{ "arceus-fighting", Species::Arceus },
		{ "arceus-fire", Species::Arceus },
		{ "arceus-flying", Species::Arceus },
		{ "arceus-ghost", Species::Arceus },
		{ "arceus-grass", Species::Arceus },
		{ "arceus-ground", Species::Arceus },
		{ "arceus-ice", Species::Arceus },
		{ "arceus-normal", Species::Arceus },
		{ "arceus-poison", Species::Arceus },
		{ "arceus-psychic", Species::Arceus },
		{ "arceus-rock", Species::Arceus },
		{ "arceus-steel", Species::Arceus },
		{ "arceus-water", Species::Arceus },

		// Generation 5	
		{ "victini", Species::Victini },
		{ "snivy", Species::Snivy },
		{ "servine", Species::Servine },
		{ "serperior", Species::Serperior },
		{ "tepig", Species::Tepig },
		{ "pignite", Species::Pignite },
		{ "emboar", Species::Emboar },
		{ "oshawott", Species::Oshawott },
		{ "dewott", Species::Dewott },
		{ "samurott", Species::Samurott },
		{ "patrat", Species::Patrat },
		{ "watchog", Species::Watchog },
		{ "lillipup", Species::Lillipup },
		{ "herdier", Species::Herdier },
		{ "stoutland", Species::Stoutland },
		{ "purrloin", Species::Purrloin },
		{ "liepard", Species::Liepard },
		{ "pansage", Species::Pansage },
		{ "simisage", Species::Simisage },
		{ "pansear", Species::Pansear },
		{ "simisear", Species::Simisear },
		{ "panpour", Species::Panpour },
		{ "simipour", Species::Simipour },
		{ "munna", Species::Munna },
		{ "musharna", Species::Musharna },
		{ "pidove", Species::Pidove },
		{ "tranquill", Species::Tranquill },
		{ "unfezant", Species::Unfezant },
		{ "blitzle", Species::Blitzle },
		{ "zebstrika", Species::Zebstrika },
		{ "roggenrola", Species::Roggenrola },
		{ "boldore", Species::Boldore },
		{ "gigalith", Species::Gigalith },
		{ "woobat", Species::Woobat },
		{ "swoobat", Species::Swoobat },
		{ "drilbur", Species::Drilbur },
		{ "excadrill", Species::Excadrill },
		{ "audino", Species::Audino },
		{ "timburr", Species::Timburr },
		{ "gurdurr", Species::Gurdurr },
		{ "conkeldurr", Species::Conkeldurr },
		{ "tympole", Species::Tympole },
		{ "palpitoad", Species::Palpitoad },
		{ "seismitoad", Species::Seismitoad },
		{ "throh", Species::Throh },
		{ "sawk", Species::Sawk },
		{ "sewaddle", Species::Sewaddle },
		{ "swadloon", Species::Swadloon },
		{ "leavanny", Species::Leavanny },
		{ "venipede", Species::Venipede },
		{ "whirlipede", Species::Whirlipede },
		{ "scolipede", Species::Scolipede },
		{ "cottonee", Species::Cottonee },
		{ "whimsicott", Species::Whimsicott },
		{ "petilil", Species::Petilil },
		{ "lilligant", Species::Lilligant },
		{ "basculin-red", Species::Basculin_Red },
		{ "basculin-blue", Species::Basculin_Blue },
		{ "sandile", Species::Sandile },
		{ "krokorok", Species::Krokorok },
		{ "krookodile", Species::Krookodile },
		{ "darumaka", Species::Darumaka },
		{ "darmanitan", Species::Darmanitan },
		{ "maractus", Species::Maractus },
		{ "dwebble", Species::Dwebble },
		{ "crustle", Species::Crustle },
		{ "scraggy", Species::Scraggy },
		{ "scrafty", Species::Scrafty },
		{ "sigilyph", Species::Sigilyph },
		{ "yamask", Species::Yamask },
		{ "cofagrigus", Species::Cofagrigus },
		{ "tirtouga", Species::Tirtouga },
		{ "carracosta", Species::Carracosta },
		{ "archen", Species::Archen },
		{ "archeops", Species::Archeops },
		{ "trubbish", Species::Trubbish },
		{ "garbodor", Species::Garbodor },
		{ "zorua", Species::Zorua },
		{ "zoroark", Species::Zoroark },
		{ "minccino", Species::Minccino },
		{ "cinccino", Species::Cinccino },
		{ "gothita", Species::Gothita },
		{ "gothorita", Species::Gothorita },
		{ "gothitelle", Species::Gothitelle },
		{ "solosis", Species::Solosis },
		{ "duosion", Species::Duosion },
		{ "reuniclus", Species::Reuniclus },
		{ "ducklett", Species::Ducklett },
		{ "swanna", Species::Swanna },
		{ "vanillite", Species::Vanillite },
		{ "vanillish", Species::Vanillish },
		{ "vanilluxe", Species::Vanilluxe },
		{ "deerling", Species::Deerling },
		{ "sawsbuck", Species::Sawsbuck },
		{ "emolga", Species::Emolga },
		{ "karrablast", Species::Karrablast },
		{ "escavalier", Species::Escavalier },
		{ "foongus", Species::Foongus },
		{ "amoonguss", Species::Amoonguss },
		{ "frillish", Species::Frillish },
		{ "jellicent", Species::Jellicent },
		{ "alomomola", Species::Alomomola },
		{ "joltik", Species::Joltik },
		{ "galvantula", Species::Galvantula },
		{ "ferroseed", Species::Ferroseed },
		{ "ferrothorn", Species::Ferrothorn },
		{ "klink", Species::Klink },
		{ "klang", Species::Klang },
		{ "klinklang", Species::Klinklang },
		{ "tynamo", Species::Tynamo },
		{ "eelektrik", Species::Eelektrik },
		{ "eelektross", Species::Eelektross },
		{ "elgyem", Species::Elgyem },
		{ "beheeyem", Species::Beheeyem },
		{ "litwick", Species::Litwick },
		{ "lampent", Species::Lampent },
		{ "chandelure", Species::Chandelure },
		{ "axew", Species::Axew },
		{ "fraxure", Species::Fraxure },
		{ "haxorus", Species::Haxorus },
		{ "cubchoo", Species::Cubchoo },
		{ "beartic", Species::Beartic },
		{ "cryogonal", Species::Cryogonal },
		{ "shelmet", Species::Shelmet },
		{ "accelgor", Species::Accelgor },
		{ "stunfisk", Species::Stunfisk },
		{ "mienfoo", Species::Mienfoo },
		{ "mienshao", Species::Mienshao },
		{ "druddigon", Species::Druddigon },
		{ "golett", Species::Golett },
		{ "golurk", Species::Golurk },
		{ "pawniard", Species::Pawniard },
		{ "bisharp", Species::Bisharp },
		{ "bouffalant", Species::Bouffalant },
		{ "rufflet", Species::Rufflet },
		{ "braviary", Species::Braviary },
		{ "vullaby", Species::Vullaby },
		{ "mandibuzz", Species::Mandibuzz },
		{ "heatmor", Species::Heatmor },
		{ "durant", Species::Durant },
		{ "deino", Species::Deino },
		{ "zweilous", Species::Zweilous },
		{ "hydreigon", Species::Hydreigon },
		{ "larvesta", Species::Larvesta },
		{ "volcarona", Species::Volcarona },
		{ "cobalion", Species::Cobalion },
		{ "terrakion", Species::Terrakion },
		{ "virizion", Species::Virizion },
		{ "tornadus-incarnate", Species::Tornadus_Incarnate },
		{ "tornadus-therian", Species::Tornadus_Therian },
		{ "thundurus-incarnate", Species::Thundurus_Incarnate },
		{ "thundurus-therian", Species::Thundurus_Therian },
		{ "reshiram", Species::Reshiram },
		{ "zekrom", Species::Zekrom },
		{ "landorus-incarnate", Species::Landorus_Incarnate },
		{ "landorus-therian", Species::Landorus_Therian },
		{ "kyurem", Species::Kyurem },
		{ "kyurem-black", Species::Kyurem_Black },
		{ "kyurem-white", Species::Kyurem_White },
		{ "keldeo", Species::Keldeo },
		{ "meloetta", Species::Meloetta },
		{ "genesect", Species::Genesect }
	};
	std::string normalized = str.to_string();
	boost::algorithm::to_lower(normalized);
	std::replace(normalized.begin(), normalized.end(), '_', '-');
	auto const it = converter.find(normalized);
	if (it != converter.end()) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Species", str);
	}
}

}	// namespace technicalmachine
