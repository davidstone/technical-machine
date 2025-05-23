// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.species;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.pokemon.species;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

export constexpr auto to_string(Species const species) -> std::string_view {
	switch (species) {
		// Generation 1
		case Species::Bulbasaur: return "Bulbasaur";
		case Species::Ivysaur: return "Ivysaur";
		case Species::Venusaur: return "Venusaur";
		case Species::Charmander: return "Charmander";
		case Species::Charmeleon: return "Charmeleon";
		case Species::Charizard: return "Charizard";
		case Species::Squirtle: return "Squirtle";
		case Species::Wartortle: return "Wartortle";
		case Species::Blastoise: return "Blastoise";
		case Species::Caterpie: return "Caterpie";
		case Species::Metapod: return "Metapod";
		case Species::Butterfree: return "Butterfree";
		case Species::Weedle: return "Weedle";
		case Species::Kakuna: return "Kakuna";
		case Species::Beedrill: return "Beedrill";
		case Species::Pidgey: return "Pidgey";
		case Species::Pidgeotto: return "Pidgeotto";
		case Species::Pidgeot: return "Pidgeot";
		case Species::Rattata: return "Rattata";
		case Species::Raticate: return "Raticate";
		case Species::Spearow: return "Spearow";
		case Species::Fearow: return "Fearow";
		case Species::Ekans: return "Ekans";
		case Species::Arbok: return "Arbok";
		case Species::Pikachu: return "Pikachu";
		case Species::Raichu: return "Raichu";
		case Species::Sandshrew: return "Sandshrew";
		case Species::Sandslash: return "Sandslash";
		case Species::Nidoran_F: return "Nidoran-F";
		case Species::Nidorina: return "Nidorina";
		case Species::Nidoqueen: return "Nidoqueen";
		case Species::Nidoran_M: return "Nidoran-M";
		case Species::Nidorino: return "Nidorino";
		case Species::Nidoking: return "Nidoking";
		case Species::Clefairy: return "Clefairy";
		case Species::Clefable: return "Clefable";
		case Species::Vulpix: return "Vulpix";
		case Species::Ninetales: return "Ninetales";
		case Species::Jigglypuff: return "Jigglypuff";
		case Species::Wigglytuff: return "Wigglytuff";
		case Species::Zubat: return "Zubat";
		case Species::Golbat: return "Golbat";
		case Species::Oddish: return "Oddish";
		case Species::Gloom: return "Gloom";
		case Species::Vileplume: return "Vileplume";
		case Species::Paras: return "Paras";
		case Species::Parasect: return "Parasect";
		case Species::Venonat: return "Venonat";
		case Species::Venomoth: return "Venomoth";
		case Species::Diglett: return "Diglett";
		case Species::Dugtrio: return "Dugtrio";
		case Species::Meowth: return "Meowth";
		case Species::Persian: return "Persian";
		case Species::Psyduck: return "Psyduck";
		case Species::Golduck: return "Golduck";
		case Species::Mankey: return "Mankey";
		case Species::Primeape: return "Primeape";
		case Species::Growlithe: return "Growlithe";
		case Species::Arcanine: return "Arcanine";
		case Species::Poliwag: return "Poliwag";
		case Species::Poliwhirl: return "Poliwhirl";
		case Species::Poliwrath: return "Poliwrath";
		case Species::Abra: return "Abra";
		case Species::Kadabra: return "Kadabra";
		case Species::Alakazam: return "Alakazam";
		case Species::Machop: return "Machop";
		case Species::Machoke: return "Machoke";
		case Species::Machamp: return "Machamp";
		case Species::Bellsprout: return "Bellsprout";
		case Species::Weepinbell: return "Weepinbell";
		case Species::Victreebel: return "Victreebel";
		case Species::Tentacool: return "Tentacool";
		case Species::Tentacruel: return "Tentacruel";
		case Species::Geodude: return "Geodude";
		case Species::Graveler: return "Graveler";
		case Species::Golem: return "Golem";
		case Species::Ponyta: return "Ponyta";
		case Species::Rapidash: return "Rapidash";
		case Species::Slowpoke: return "Slowpoke";
		case Species::Slowbro: return "Slowbro";
		case Species::Magnemite: return "Magnemite";
		case Species::Magneton: return "Magneton";
		case Species::Farfetchd: return "Farfetch'd";
		case Species::Doduo: return "Doduo";
		case Species::Dodrio: return "Dodrio";
		case Species::Seel: return "Seel";
		case Species::Dewgong: return "Dewgong";
		case Species::Grimer: return "Grimer";
		case Species::Muk: return "Muk";
		case Species::Shellder: return "Shellder";
		case Species::Cloyster: return "Cloyster";
		case Species::Gastly: return "Gastly";
		case Species::Haunter: return "Haunter";
		case Species::Gengar: return "Gengar";
		case Species::Onix: return "Onix";
		case Species::Drowzee: return "Drowzee";
		case Species::Hypno: return "Hypno";
		case Species::Krabby: return "Krabby";
		case Species::Kingler: return "Kingler";
		case Species::Voltorb: return "Voltorb";
		case Species::Electrode: return "Electrode";
		case Species::Exeggcute: return "Exeggcute";
		case Species::Exeggutor: return "Exeggutor";
		case Species::Cubone: return "Cubone";
		case Species::Marowak: return "Marowak";
		case Species::Hitmonlee: return "Hitmonlee";
		case Species::Hitmonchan: return "Hitmonchan";
		case Species::Lickitung: return "Lickitung";
		case Species::Koffing: return "Koffing";
		case Species::Weezing: return "Weezing";
		case Species::Rhyhorn: return "Rhyhorn";
		case Species::Rhydon: return "Rhydon";
		case Species::Chansey: return "Chansey";
		case Species::Tangela: return "Tangela";
		case Species::Kangaskhan: return "Kangaskhan";
		case Species::Horsea: return "Horsea";
		case Species::Seadra: return "Seadra";
		case Species::Goldeen: return "Goldeen";
		case Species::Seaking: return "Seaking";
		case Species::Staryu: return "Staryu";
		case Species::Starmie: return "Starmie";
		case Species::Mr_Mime: return "Mr. Mime";
		case Species::Scyther: return "Scyther";
		case Species::Jynx: return "Jynx";
		case Species::Electabuzz: return "Electabuzz";
		case Species::Magmar: return "Magmar";
		case Species::Pinsir: return "Pinsir";
		case Species::Tauros: return "Tauros";
		case Species::Magikarp: return "Magikarp";
		case Species::Gyarados: return "Gyarados";
		case Species::Lapras: return "Lapras";
		case Species::Ditto: return "Ditto";
		case Species::Eevee: return "Eevee";
		case Species::Vaporeon: return "Vaporeon";
		case Species::Jolteon: return "Jolteon";
		case Species::Flareon: return "Flareon";
		case Species::Porygon: return "Porygon";
		case Species::Omanyte: return "Omanyte";
		case Species::Omastar: return "Omastar";
		case Species::Kabuto: return "Kabuto";
		case Species::Kabutops: return "Kabutops";
		case Species::Aerodactyl: return "Aerodactyl";
		case Species::Snorlax: return "Snorlax";
		case Species::Articuno: return "Articuno";
		case Species::Zapdos: return "Zapdos";
		case Species::Moltres: return "Moltres";
		case Species::Dratini: return "Dratini";
		case Species::Dragonair: return "Dragonair";
		case Species::Dragonite: return "Dragonite";
		case Species::Mewtwo: return "Mewtwo";
		case Species::Mew: return "Mew";

		// Generation 2
		case Species::Chikorita: return "Chikorita";
		case Species::Bayleef: return "Bayleef";
		case Species::Meganium: return "Meganium";
		case Species::Cyndaquil: return "Cyndaquil";
		case Species::Quilava: return "Quilava";
		case Species::Typhlosion: return "Typhlosion";
		case Species::Totodile: return "Totodile";
		case Species::Croconaw: return "Croconaw";
		case Species::Feraligatr: return "Feraligatr";
		case Species::Sentret: return "Sentret";
		case Species::Furret: return "Furret";
		case Species::Hoothoot: return "Hoothoot";
		case Species::Noctowl: return "Noctowl";
		case Species::Ledyba: return "Ledyba";
		case Species::Ledian: return "Ledian";
		case Species::Spinarak: return "Spinarak";
		case Species::Ariados: return "Ariados";
		case Species::Crobat: return "Crobat";
		case Species::Chinchou: return "Chinchou";
		case Species::Lanturn: return "Lanturn";
		case Species::Pichu: return "Pichu";
		case Species::Cleffa: return "Cleffa";
		case Species::Igglybuff: return "Igglybuff";
		case Species::Togepi: return "Togepi";
		case Species::Togetic: return "Togetic";
		case Species::Natu: return "Natu";
		case Species::Xatu: return "Xatu";
		case Species::Mareep: return "Mareep";
		case Species::Flaaffy: return "Flaaffy";
		case Species::Ampharos: return "Ampharos";
		case Species::Bellossom: return "Bellossom";
		case Species::Marill: return "Marill";
		case Species::Azumarill: return "Azumarill";
		case Species::Sudowoodo: return "Sudowoodo";
		case Species::Politoed: return "Politoed";
		case Species::Hoppip: return "Hoppip";
		case Species::Skiploom: return "Skiploom";
		case Species::Jumpluff: return "Jumpluff";
		case Species::Aipom: return "Aipom";
		case Species::Sunkern: return "Sunkern";
		case Species::Sunflora: return "Sunflora";
		case Species::Yanma: return "Yanma";
		case Species::Wooper: return "Wooper";
		case Species::Quagsire: return "Quagsire";
		case Species::Espeon: return "Espeon";
		case Species::Umbreon: return "Umbreon";
		case Species::Murkrow: return "Murkrow";
		case Species::Slowking: return "Slowking";
		case Species::Misdreavus: return "Misdreavus";
		case Species::Unown: return "Unown";
		case Species::Wobbuffet: return "Wobbuffet";
		case Species::Girafarig: return "Girafarig";
		case Species::Pineco: return "Pineco";
		case Species::Forretress: return "Forretress";
		case Species::Dunsparce: return "Dunsparce";
		case Species::Gligar: return "Gligar";
		case Species::Steelix: return "Steelix";
		case Species::Snubbull: return "Snubbull";
		case Species::Granbull: return "Granbull";
		case Species::Qwilfish: return "Qwilfish";
		case Species::Scizor: return "Scizor";
		case Species::Shuckle: return "Shuckle";
		case Species::Heracross: return "Heracross";
		case Species::Sneasel: return "Sneasel";
		case Species::Teddiursa: return "Teddiursa";
		case Species::Ursaring: return "Ursaring";
		case Species::Slugma: return "Slugma";
		case Species::Magcargo: return "Magcargo";
		case Species::Swinub: return "Swinub";
		case Species::Piloswine: return "Piloswine";
		case Species::Corsola: return "Corsola";
		case Species::Remoraid: return "Remoraid";
		case Species::Octillery: return "Octillery";
		case Species::Delibird: return "Delibird";
		case Species::Mantine: return "Mantine";
		case Species::Skarmory: return "Skarmory";
		case Species::Houndour: return "Houndour";
		case Species::Houndoom: return "Houndoom";
		case Species::Kingdra: return "Kingdra";
		case Species::Phanpy: return "Phanpy";
		case Species::Donphan: return "Donphan";
		case Species::Porygon2: return "Porygon2";
		case Species::Stantler: return "Stantler";
		case Species::Smeargle: return "Smeargle";
		case Species::Tyrogue: return "Tyrogue";
		case Species::Hitmontop: return "Hitmontop";
		case Species::Smoochum: return "Smoochum";
		case Species::Elekid: return "Elekid";
		case Species::Magby: return "Magby";
		case Species::Miltank: return "Miltank";
		case Species::Blissey: return "Blissey";
		case Species::Raikou: return "Raikou";
		case Species::Entei: return "Entei";
		case Species::Suicune: return "Suicune";
		case Species::Larvitar: return "Larvitar";
		case Species::Pupitar: return "Pupitar";
		case Species::Tyranitar: return "Tyranitar";
		case Species::Lugia: return "Lugia";
		case Species::Ho_Oh: return "Ho-Oh";
		case Species::Celebi: return "Celebi";

		// Generation 3
		case Species::Treecko: return "Treecko";
		case Species::Grovyle: return "Grovyle";
		case Species::Sceptile: return "Sceptile";
		case Species::Torchic: return "Torchic";
		case Species::Combusken: return "Combusken";
		case Species::Blaziken: return "Blaziken";
		case Species::Mudkip: return "Mudkip";
		case Species::Marshtomp: return "Marshtomp";
		case Species::Swampert: return "Swampert";
		case Species::Poochyena: return "Poochyena";
		case Species::Mightyena: return "Mightyena";
		case Species::Zigzagoon: return "Zigzagoon";
		case Species::Linoone: return "Linoone";
		case Species::Wurmple: return "Wurmple";
		case Species::Silcoon: return "Silcoon";
		case Species::Beautifly: return "Beautifly";
		case Species::Cascoon: return "Cascoon";
		case Species::Dustox: return "Dustox";
		case Species::Lotad: return "Lotad";
		case Species::Lombre: return "Lombre";
		case Species::Ludicolo: return "Ludicolo";
		case Species::Seedot: return "Seedot";
		case Species::Nuzleaf: return "Nuzleaf";
		case Species::Shiftry: return "Shiftry";
		case Species::Taillow: return "Taillow";
		case Species::Swellow: return "Swellow";
		case Species::Wingull: return "Wingull";
		case Species::Pelipper: return "Pelipper";
		case Species::Ralts: return "Ralts";
		case Species::Kirlia: return "Kirlia";
		case Species::Gardevoir: return "Gardevoir";
		case Species::Surskit: return "Surskit";
		case Species::Masquerain: return "Masquerain";
		case Species::Shroomish: return "Shroomish";
		case Species::Breloom: return "Breloom";
		case Species::Slakoth: return "Slakoth";
		case Species::Vigoroth: return "Vigoroth";
		case Species::Slaking: return "Slaking";
		case Species::Nincada: return "Nincada";
		case Species::Ninjask: return "Ninjask";
		case Species::Shedinja: return "Shedinja";
		case Species::Whismur: return "Whismur";
		case Species::Loudred: return "Loudred";
		case Species::Exploud: return "Exploud";
		case Species::Makuhita: return "Makuhita";
		case Species::Hariyama: return "Hariyama";
		case Species::Azurill: return "Azurill";
		case Species::Nosepass: return "Nosepass";
		case Species::Skitty: return "Skitty";
		case Species::Delcatty: return "Delcatty";
		case Species::Sableye: return "Sableye";
		case Species::Mawile: return "Mawile";
		case Species::Aron: return "Aron";
		case Species::Lairon: return "Lairon";
		case Species::Aggron: return "Aggron";
		case Species::Meditite: return "Meditite";
		case Species::Medicham: return "Medicham";
		case Species::Electrike: return "Electrike";
		case Species::Manectric: return "Manectric";
		case Species::Plusle: return "Plusle";
		case Species::Minun: return "Minun";
		case Species::Volbeat: return "Volbeat";
		case Species::Illumise: return "Illumise";
		case Species::Roselia: return "Roselia";
		case Species::Gulpin: return "Gulpin";
		case Species::Swalot: return "Swalot";
		case Species::Carvanha: return "Carvanha";
		case Species::Sharpedo: return "Sharpedo";
		case Species::Wailmer: return "Wailmer";
		case Species::Wailord: return "Wailord";
		case Species::Numel: return "Numel";
		case Species::Camerupt: return "Camerupt";
		case Species::Torkoal: return "Torkoal";
		case Species::Spoink: return "Spoink";
		case Species::Grumpig: return "Grumpig";
		case Species::Spinda: return "Spinda";
		case Species::Trapinch: return "Trapinch";
		case Species::Vibrava: return "Vibrava";
		case Species::Flygon: return "Flygon";
		case Species::Cacnea: return "Cacnea";
		case Species::Cacturne: return "Cacturne";
		case Species::Swablu: return "Swablu";
		case Species::Altaria: return "Altaria";
		case Species::Zangoose: return "Zangoose";
		case Species::Seviper: return "Seviper";
		case Species::Lunatone: return "Lunatone";
		case Species::Solrock: return "Solrock";
		case Species::Barboach: return "Barboach";
		case Species::Whiscash: return "Whiscash";
		case Species::Corphish: return "Corphish";
		case Species::Crawdaunt: return "Crawdaunt";
		case Species::Baltoy: return "Baltoy";
		case Species::Claydol: return "Claydol";
		case Species::Lileep: return "Lileep";
		case Species::Cradily: return "Cradily";
		case Species::Anorith: return "Anorith";
		case Species::Armaldo: return "Armaldo";
		case Species::Feebas: return "Feebas";
		case Species::Milotic: return "Milotic";
		case Species::Castform: return "Castform";
		case Species::Kecleon: return "Kecleon";
		case Species::Shuppet: return "Shuppet";
		case Species::Banette: return "Banette";
		case Species::Duskull: return "Duskull";
		case Species::Dusclops: return "Dusclops";
		case Species::Tropius: return "Tropius";
		case Species::Chimecho: return "Chimecho";
		case Species::Absol: return "Absol";
		case Species::Wynaut: return "Wynaut";
		case Species::Snorunt: return "Snorunt";
		case Species::Glalie: return "Glalie";
		case Species::Spheal: return "Spheal";
		case Species::Sealeo: return "Sealeo";
		case Species::Walrein: return "Walrein";
		case Species::Clamperl: return "Clamperl";
		case Species::Huntail: return "Huntail";
		case Species::Gorebyss: return "Gorebyss";
		case Species::Relicanth: return "Relicanth";
		case Species::Luvdisc: return "Luvdisc";
		case Species::Bagon: return "Bagon";
		case Species::Shelgon: return "Shelgon";
		case Species::Salamence: return "Salamence";
		case Species::Beldum: return "Beldum";
		case Species::Metang: return "Metang";
		case Species::Metagross: return "Metagross";
		case Species::Regirock: return "Regirock";
		case Species::Regice: return "Regice";
		case Species::Registeel: return "Registeel";
		case Species::Latias: return "Latias";
		case Species::Latios: return "Latios";
		case Species::Kyogre: return "Kyogre";
		case Species::Groudon: return "Groudon";
		case Species::Rayquaza: return "Rayquaza";
		case Species::Jirachi: return "Jirachi";
		case Species::Deoxys_Normal: return "Deoxys-Normal";
		case Species::Deoxys_Attack: return "Deoxys-Attack";
		case Species::Deoxys_Defense: return "Deoxys-Defense";
		case Species::Deoxys_Speed: return "Deoxys-Speed";

		// Generation 4
		case Species::Turtwig: return "Turtwig";
		case Species::Grotle: return "Grotle";
		case Species::Torterra: return "Torterra";
		case Species::Chimchar: return "Chimchar";
		case Species::Monferno: return "Monferno";
		case Species::Infernape: return "Infernape";
		case Species::Piplup: return "Piplup";
		case Species::Prinplup: return "Prinplup";
		case Species::Empoleon: return "Empoleon";
		case Species::Starly: return "Starly";
		case Species::Staravia: return "Staravia";
		case Species::Staraptor: return "Staraptor";
		case Species::Bidoof: return "Bidoof";
		case Species::Bibarel: return "Bibarel";
		case Species::Kricketot: return "Kricketot";
		case Species::Kricketune: return "Kricketune";
		case Species::Shinx: return "Shinx";
		case Species::Luxio: return "Luxio";
		case Species::Luxray: return "Luxray";
		case Species::Budew: return "Budew";
		case Species::Roserade: return "Roserade";
		case Species::Cranidos: return "Cranidos";
		case Species::Rampardos: return "Rampardos";
		case Species::Shieldon: return "Shieldon";
		case Species::Bastiodon: return "Bastiodon";
		case Species::Burmy: return "Burmy";
		case Species::Wormadam_Plant: return "Wormadam-Plant";
		case Species::Wormadam_Sandy: return "Wormadam-Sandy";
		case Species::Wormadam_Trash: return "Wormadam-Trash";
		case Species::Mothim: return "Mothim";
		case Species::Combee: return "Combee";
		case Species::Vespiquen: return "Vespiquen";
		case Species::Pachirisu: return "Pachirisu";
		case Species::Buizel: return "Buizel";
		case Species::Floatzel: return "Floatzel";
		case Species::Cherubi: return "Cherubi";
		case Species::Cherrim: return "Cherrim";
		case Species::Shellos: return "Shellos";
		case Species::Gastrodon: return "Gastrodon";
		case Species::Ambipom: return "Ambipom";
		case Species::Drifloon: return "Drifloon";
		case Species::Drifblim: return "Drifblim";
		case Species::Buneary: return "Buneary";
		case Species::Lopunny: return "Lopunny";
		case Species::Mismagius: return "Mismagius";
		case Species::Honchkrow: return "Honchkrow";
		case Species::Glameow: return "Glameow";
		case Species::Purugly: return "Purugly";
		case Species::Chingling: return "Chingling";
		case Species::Stunky: return "Stunky";
		case Species::Skuntank: return "Skuntank";
		case Species::Bronzor: return "Bronzor";
		case Species::Bronzong: return "Bronzong";
		case Species::Bonsly: return "Bonsly";
		case Species::Mime_Jr: return "Mime Jr.";
		case Species::Happiny: return "Happiny";
		case Species::Chatot: return "Chatot";
		case Species::Spiritomb: return "Spiritomb";
		case Species::Gible: return "Gible";
		case Species::Gabite: return "Gabite";
		case Species::Garchomp: return "Garchomp";
		case Species::Munchlax: return "Munchlax";
		case Species::Riolu: return "Riolu";
		case Species::Lucario: return "Lucario";
		case Species::Hippopotas: return "Hippopotas";
		case Species::Hippowdon: return "Hippowdon";
		case Species::Skorupi: return "Skorupi";
		case Species::Drapion: return "Drapion";
		case Species::Croagunk: return "Croagunk";
		case Species::Toxicroak: return "Toxicroak";
		case Species::Carnivine: return "Carnivine";
		case Species::Finneon: return "Finneon";
		case Species::Lumineon: return "Lumineon";
		case Species::Mantyke: return "Mantyke";
		case Species::Snover: return "Snover";
		case Species::Abomasnow: return "Abomasnow";
		case Species::Weavile: return "Weavile";
		case Species::Magnezone: return "Magnezone";
		case Species::Lickilicky: return "Lickilicky";
		case Species::Rhyperior: return "Rhyperior";
		case Species::Tangrowth: return "Tangrowth";
		case Species::Electivire: return "Electivire";
		case Species::Magmortar: return "Magmortar";
		case Species::Togekiss: return "Togekiss";
		case Species::Yanmega: return "Yanmega";
		case Species::Leafeon: return "Leafeon";
		case Species::Glaceon: return "Glaceon";
		case Species::Gliscor: return "Gliscor";
		case Species::Mamoswine: return "Mamoswine";
		case Species::Porygon_Z: return "Porygon-Z";
		case Species::Gallade: return "Gallade";
		case Species::Probopass: return "Probopass";
		case Species::Dusknoir: return "Dusknoir";
		case Species::Froslass: return "Froslass";
		case Species::Rotom: return "Rotom";
		case Species::Rotom_Heat: return "Rotom-Heat";
		case Species::Rotom_Wash: return "Rotom-Wash";
		case Species::Rotom_Frost: return "Rotom-Frost";
		case Species::Rotom_Fan: return "Rotom-Fan";
		case Species::Rotom_Mow: return "Rotom-Mow";
		case Species::Uxie: return "Uxie";
		case Species::Mesprit: return "Mesprit";
		case Species::Azelf: return "Azelf";
		case Species::Dialga: return "Dialga";
		case Species::Palkia: return "Palkia";
		case Species::Heatran: return "Heatran";
		case Species::Regigigas: return "Regigigas";
		case Species::Giratina_Altered: return "Giratina-Altered";
		case Species::Giratina_Origin: return "Giratina-Origin";
		case Species::Cresselia: return "Cresselia";
		case Species::Phione: return "Phione";
		case Species::Manaphy: return "Manaphy";
		case Species::Darkrai: return "Darkrai";
		case Species::Shaymin_Land: return "Shaymin-Land";
		case Species::Shaymin_Sky: return "Shaymin-Sky";
		case Species::Arceus: return "Arceus";

		// Generation 5
		case Species::Victini: return "Victini";
		case Species::Snivy: return "Snivy";
		case Species::Servine: return "Servine";
		case Species::Serperior: return "Serperior";
		case Species::Tepig: return "Tepig";
		case Species::Pignite: return "Pignite";
		case Species::Emboar: return "Emboar";
		case Species::Oshawott: return "Oshawott";
		case Species::Dewott: return "Dewott";
		case Species::Samurott: return "Samurott";
		case Species::Patrat: return "Patrat";
		case Species::Watchog: return "Watchog";
		case Species::Lillipup: return "Lillipup";
		case Species::Herdier: return "Herdier";
		case Species::Stoutland: return "Stoutland";
		case Species::Purrloin: return "Purrloin";
		case Species::Liepard: return "Liepard";
		case Species::Pansage: return "Pansage";
		case Species::Simisage: return "Simisage";
		case Species::Pansear: return "Pansear";
		case Species::Simisear: return "Simisear";
		case Species::Panpour: return "Panpour";
		case Species::Simipour: return "Simipour";
		case Species::Munna: return "Munna";
		case Species::Musharna: return "Musharna";
		case Species::Pidove: return "Pidove";
		case Species::Tranquill: return "Tranquill";
		case Species::Unfezant: return "Unfezant";
		case Species::Blitzle: return "Blitzle";
		case Species::Zebstrika: return "Zebstrika";
		case Species::Roggenrola: return "Roggenrola";
		case Species::Boldore: return "Boldore";
		case Species::Gigalith: return "Gigalith";
		case Species::Woobat: return "Woobat";
		case Species::Swoobat: return "Swoobat";
		case Species::Drilbur: return "Drilbur";
		case Species::Excadrill: return "Excadrill";
		case Species::Audino: return "Audino";
		case Species::Timburr: return "Timburr";
		case Species::Gurdurr: return "Gurdurr";
		case Species::Conkeldurr: return "Conkeldurr";
		case Species::Tympole: return "Tympole";
		case Species::Palpitoad: return "Palpitoad";
		case Species::Seismitoad: return "Seismitoad";
		case Species::Throh: return "Throh";
		case Species::Sawk: return "Sawk";
		case Species::Sewaddle: return "Sewaddle";
		case Species::Swadloon: return "Swadloon";
		case Species::Leavanny: return "Leavanny";
		case Species::Venipede: return "Venipede";
		case Species::Whirlipede: return "Whirlipede";
		case Species::Scolipede: return "Scolipede";
		case Species::Cottonee: return "Cottonee";
		case Species::Whimsicott: return "Whimsicott";
		case Species::Petilil: return "Petilil";
		case Species::Lilligant: return "Lilligant";
		case Species::Basculin: return "Basculin";
		case Species::Sandile: return "Sandile";
		case Species::Krokorok: return "Krokorok";
		case Species::Krookodile: return "Krookodile";
		case Species::Darumaka: return "Darumaka";
		case Species::Darmanitan: return "Darmanitan";
		case Species::Maractus: return "Maractus";
		case Species::Dwebble: return "Dwebble";
		case Species::Crustle: return "Crustle";
		case Species::Scraggy: return "Scraggy";
		case Species::Scrafty: return "Scrafty";
		case Species::Sigilyph: return "Sigilyph";
		case Species::Yamask: return "Yamask";
		case Species::Cofagrigus: return "Cofagrigus";
		case Species::Tirtouga: return "Tirtouga";
		case Species::Carracosta: return "Carracosta";
		case Species::Archen: return "Archen";
		case Species::Archeops: return "Archeops";
		case Species::Trubbish: return "Trubbish";
		case Species::Garbodor: return "Garbodor";
		case Species::Zorua: return "Zorua";
		case Species::Zoroark: return "Zoroark";
		case Species::Minccino: return "Minccino";
		case Species::Cinccino: return "Cinccino";
		case Species::Gothita: return "Gothita";
		case Species::Gothorita: return "Gothorita";
		case Species::Gothitelle: return "Gothitelle";
		case Species::Solosis: return "Solosis";
		case Species::Duosion: return "Duosion";
		case Species::Reuniclus: return "Reuniclus";
		case Species::Ducklett: return "Ducklett";
		case Species::Swanna: return "Swanna";
		case Species::Vanillite: return "Vanillite";
		case Species::Vanillish: return "Vanillish";
		case Species::Vanilluxe: return "Vanilluxe";
		case Species::Deerling: return "Deerling";
		case Species::Sawsbuck: return "Sawsbuck";
		case Species::Emolga: return "Emolga";
		case Species::Karrablast: return "Karrablast";
		case Species::Escavalier: return "Escavalier";
		case Species::Foongus: return "Foongus";
		case Species::Amoonguss: return "Amoonguss";
		case Species::Frillish: return "Frillish";
		case Species::Jellicent: return "Jellicent";
		case Species::Alomomola: return "Alomomola";
		case Species::Joltik: return "Joltik";
		case Species::Galvantula: return "Galvantula";
		case Species::Ferroseed: return "Ferroseed";
		case Species::Ferrothorn: return "Ferrothorn";
		case Species::Klink: return "Klink";
		case Species::Klang: return "Klang";
		case Species::Klinklang: return "Klinklang";
		case Species::Tynamo: return "Tynamo";
		case Species::Eelektrik: return "Eelektrik";
		case Species::Eelektross: return "Eelektross";
		case Species::Elgyem: return "Elgyem";
		case Species::Beheeyem: return "Beheeyem";
		case Species::Litwick: return "Litwick";
		case Species::Lampent: return "Lampent";
		case Species::Chandelure: return "Chandelure";
		case Species::Axew: return "Axew";
		case Species::Fraxure: return "Fraxure";
		case Species::Haxorus: return "Haxorus";
		case Species::Cubchoo: return "Cubchoo";
		case Species::Beartic: return "Beartic";
		case Species::Cryogonal: return "Cryogonal";
		case Species::Shelmet: return "Shelmet";
		case Species::Accelgor: return "Accelgor";
		case Species::Stunfisk: return "Stunfisk";
		case Species::Mienfoo: return "Mienfoo";
		case Species::Mienshao: return "Mienshao";
		case Species::Druddigon: return "Druddigon";
		case Species::Golett: return "Golett";
		case Species::Golurk: return "Golurk";
		case Species::Pawniard: return "Pawniard";
		case Species::Bisharp: return "Bisharp";
		case Species::Bouffalant: return "Bouffalant";
		case Species::Rufflet: return "Rufflet";
		case Species::Braviary: return "Braviary";
		case Species::Vullaby: return "Vullaby";
		case Species::Mandibuzz: return "Mandibuzz";
		case Species::Heatmor: return "Heatmor";
		case Species::Durant: return "Durant";
		case Species::Deino: return "Deino";
		case Species::Zweilous: return "Zweilous";
		case Species::Hydreigon: return "Hydreigon";
		case Species::Larvesta: return "Larvesta";
		case Species::Volcarona: return "Volcarona";
		case Species::Cobalion: return "Cobalion";
		case Species::Terrakion: return "Terrakion";
		case Species::Virizion: return "Virizion";
		case Species::Tornadus_Incarnate: return "Tornadus-Incarnate";
		case Species::Tornadus_Therian: return "Tornadus-Therian";
		case Species::Thundurus_Incarnate: return "Thundurus-Incarnate";
		case Species::Thundurus_Therian: return "Thundurus-Therian";
		case Species::Reshiram: return "Reshiram";
		case Species::Zekrom: return "Zekrom";
		case Species::Landorus_Incarnate: return "Landorus-Incarnate";
		case Species::Landorus_Therian: return "Landorus-Therian";
		case Species::Kyurem: return "Kyurem";
		case Species::Kyurem_Black: return "Kyurem-Black";
		case Species::Kyurem_White: return "Kyurem-White";
		case Species::Keldeo: return "Keldeo";
		case Species::Keldeo_Resolute: return "Keldeo-Resolute";
		case Species::Meloetta: return "Meloetta";
		case Species::Genesect: return "Genesect";

		// Generation 6
		case Species::Chespin: return "Chespin";
		case Species::Quilladin: return "Quilladin";
		case Species::Chesnaught: return "Chesnaught";
		case Species::Fennekin: return "Fennekin";
		case Species::Braixen: return "Braixen";
		case Species::Delphox: return "Delphox";
		case Species::Froakie: return "Froakie";
		case Species::Frogadier: return "Frogadier";
		case Species::Greninja: return "Greninja";
		case Species::Bunnelby: return "Bunnelby";
		case Species::Diggersby: return "Diggersby";
		case Species::Fletchling: return "Fletchling";
		case Species::Fletchinder: return "Fletchinder";
		case Species::Talonflame: return "Talonflame";
		case Species::Scatterbug: return "Scatterbug";
		case Species::Spewpa: return "Spewpa";
		case Species::Vivillon: return "Vivillon";
		case Species::Litleo: return "Litleo";
		case Species::Pyroar: return "Pyroar";
		case Species::Flabebe: return "Flabebe";
		case Species::Floette: return "Floette";
		case Species::Florges: return "Florges";
		case Species::Skiddo: return "Skiddo";
		case Species::Gogoat: return "Gogoat";
		case Species::Pancham: return "Pancham";
		case Species::Pangoro: return "Pangoro";
		case Species::Furfrou: return "Furfrou";
		case Species::Espurr: return "Espurr";
		case Species::Meowstic_F: return "Meowstic-F";
		case Species::Meowstic_M: return "Meowstic-M";
		case Species::Honedge: return "Honedge";
		case Species::Doublade: return "Doublade";
		case Species::Aegislash: return "Aegislash";
		case Species::Spritzee: return "Spritzee";
		case Species::Aromatisse: return "Aromatisse";
		case Species::Swirlix: return "Swirlix";
		case Species::Slurpuff: return "Slurpuff";
		case Species::Inkay: return "Inkay";
		case Species::Malamar: return "Malamar";
		case Species::Binacle: return "Binacle";
		case Species::Barbaracle: return "Barbaracle";
		case Species::Skrelp: return "Skrelp";
		case Species::Dragalge: return "Dragalge";
		case Species::Clauncher: return "Clauncher";
		case Species::Clawitzer: return "Clawitzer";
		case Species::Helioptile: return "Helioptile";
		case Species::Heliolisk: return "Heliolisk";
		case Species::Tyrunt: return "Tyrunt";
		case Species::Tyrantrum: return "Tyrantrum";
		case Species::Amaura: return "Amaura";
		case Species::Aurorus: return "Aurorus";
		case Species::Sylveon: return "Sylveon";
		case Species::Hawlucha: return "Hawlucha";
		case Species::Dedenne: return "Dedenne";
		case Species::Carbink: return "Carbink";
		case Species::Goomy: return "Goomy";
		case Species::Sliggoo: return "Sliggoo";
		case Species::Goodra: return "Goodra";
		case Species::Klefki: return "Klefki";
		case Species::Phantump: return "Phantump";
		case Species::Trevenant: return "Trevenant";
		case Species::Pumpkaboo_Small: return "Pumpkaboo-Small";
		case Species::Pumpkaboo_Average: return "Pumpkaboo-Average";
		case Species::Pumpkaboo_Large: return "Pumpkaboo-Large";
		case Species::Pumpkaboo_Super: return "Pumpkaboo-Super";
		case Species::Gourgeist_Small: return "Gourgeist-Small";
		case Species::Gourgeist_Average: return "Gourgeist-Average";
		case Species::Gourgeist_Large: return "Gourgeist-Large";
		case Species::Gourgeist_Super: return "Gourgeist-Super";
		case Species::Bergmite: return "Bergmite";
		case Species::Avalugg: return "Avalugg";
		case Species::Noibat: return "Noibat";
		case Species::Noivern: return "Noivern";
		case Species::Xerneas: return "Xerneas";
		case Species::Yveltal: return "Yveltal";
		case Species::Zygarde_50: return "Zygarde-50%";
		case Species::Zygarde_10: return "Zygarde-10%";
		case Species::Zygarde_Complete: return "Zygarde-Complete";
		case Species::Diancie: return "Diancie";
		case Species::Hoopa_Confined: return "Hoopa-Confined";
		case Species::Hoopa_Unbound: return "Hoopa-Unbound";
		case Species::Volcanion: return "Volcanion";

		// Generation 7
		case Species::Rowlet: return "Rowlet";
		case Species::Dartrix: return "Dartrix";
		case Species::Decidueye: return "Decidueye";
		case Species::Litten: return "Litten";
		case Species::Torracat: return "Torracat";
		case Species::Incineroar: return "Incineroar";
		case Species::Popplio: return "Popplio";
		case Species::Brionne: return "Brionne";
		case Species::Primarina: return "Primarina";
		case Species::Pikipek: return "Pikipek";
		case Species::Trumbeak: return "Trumbeak";
		case Species::Toucannon: return "Toucannon";
		case Species::Yungoos: return "Yungoos";
		case Species::Gumshoos: return "Gumshoos";
		case Species::Grubbin: return "Grubbin";
		case Species::Charjabug: return "Charjabug";
		case Species::Vikavolt: return "Vikavolt";
		case Species::Crabrawler: return "Crabrawler";
		case Species::Crabominable: return "Crabominable";
		case Species::Oricorio_Baile: return "Oricorio-Baile";
		case Species::Oricorio_Pau: return "Oricorio-Pa'u";
		case Species::Oricorio_Pom_Pom: return "Oricorio-Pom-Pom";
		case Species::Oricorio_Sensu: return "Oricorio-Sensu";
		case Species::Cutiefly: return "Cutiefly";
		case Species::Ribombee: return "Ribombee";
		case Species::Rockruff: return "Rockruff";
		case Species::Lycanroc_Midday: return "Lycanroc-Midday";
		case Species::Lycanroc_Midnight: return "Lycanroc-Midnight";
		case Species::Lycanroc_Dusk: return "Lycanroc-Dusk";
		case Species::Wishiwashi_Solo: return "Wishiwashi-Solo";
		case Species::Wishiwashi_School: return "Wishiwashi-School";
		case Species::Mareanie: return "Mareanie";
		case Species::Toxapex: return "Toxapex";
		case Species::Mudbray: return "Mudbray";
		case Species::Mudsdale: return "Mudsdale";
		case Species::Dewpider: return "Dewpider";
		case Species::Araquanid: return "Araquanid";
		case Species::Fomantis: return "Fomantis";
		case Species::Lurantis: return "Lurantis";
		case Species::Morelull: return "Morelull";
		case Species::Shiinotic: return "Shiinotic";
		case Species::Salandit: return "Salandit";
		case Species::Salazzle: return "Salazzle";
		case Species::Stufful: return "Stufful";
		case Species::Bewear: return "Bewear";
		case Species::Bounsweet: return "Bounsweet";
		case Species::Steenee: return "Steenee";
		case Species::Tsareena: return "Tsareena";
		case Species::Comfey: return "Comfey";
		case Species::Oranguru: return "Oranguru";
		case Species::Passimian: return "Passimian";
		case Species::Wimpod: return "Wimpod";
		case Species::Golisopod: return "Golisopod";
		case Species::Sandygast: return "Sandygast";
		case Species::Palossand: return "Palossand";
		case Species::Pyukumuku: return "Pyukumuku";
		case Species::Type_Null: return "Type: Null";
		case Species::Silvally: return "Silvally";
		case Species::Minior_Meteor: return "Minior-Meteor";
		case Species::Minior_Core: return "Minior-Core";
		case Species::Komala: return "Komala";
		case Species::Turtonator: return "Turtonator";
		case Species::Togedemaru: return "Togedemaru";
		case Species::Mimikyu: return "Mimikyu";
		case Species::Bruxish: return "Bruxish";
		case Species::Drampa: return "Drampa";
		case Species::Dhelmise: return "Dhelmise";
		case Species::Jangmo_o: return "Jangmo-o";
		case Species::Hakamo_o: return "Hakamo-o";
		case Species::Kommo_o: return "Kommo-o";
		case Species::Tapu_Koko: return "Tapu Koko";
		case Species::Tapu_Lele: return "Tapu Lele";
		case Species::Tapu_Bulu: return "Tapu Bulu";
		case Species::Tapu_Fini: return "Tapu Fini";
		case Species::Cosmog: return "Cosmog";
		case Species::Cosmoem: return "Cosmoem";
		case Species::Solgaleo: return "Solgaleo";
		case Species::Lunala: return "Lunala";
		case Species::Nihilego: return "Nihilego";
		case Species::Buzzwole: return "Buzzwole";
		case Species::Pheromosa: return "Pheromosa";
		case Species::Xurkitree: return "Xurkitree";
		case Species::Celesteela: return "Celesteela";
		case Species::Kartana: return "Kartana";
		case Species::Guzzlord: return "Guzzlord";
		case Species::Necrozma: return "Necrozma";
		case Species::Necrozma_Dusk_Mane: return "Necrozma-Dusk Mane";
		case Species::Necrozma_Dawn_Wings: return "Necrozma-Dawn Wings";
		case Species::Necrozma_Ultra: return "Necrozma-Ultra";
		case Species::Magearna: return "Magearna";
		case Species::Marshadow: return "Marshadow";
		case Species::Poipole: return "Poipole";
		case Species::Naganadel: return "Naganadel";
		case Species::Stakataka: return "Stakataka";
		case Species::Blacephalon: return "Blacephalon";
		case Species::Zeraora: return "Zeraora";
		case Species::Meltan: return "Meltan";
		case Species::Melmetal: return "Melmetal";
		case Species::Rattata_Alola: return "Rattata-Alola";
		case Species::Raticate_Alola: return "Raticate-Alola";
		case Species::Raichu_Alola: return "Raichu-Alola";
		case Species::Sandshrew_Alola: return "Sandshrew-Alola";
		case Species::Sandslash_Alola: return "Sandslash-Alola";
		case Species::Vulpix_Alola: return "Vulpix-Alola";
		case Species::Ninetales_Alola: return "Ninetales-Alola";
		case Species::Diglett_Alola: return "Diglett-Alola";
		case Species::Dugtrio_Alola: return "Dugtrio-Alola";
		case Species::Meowth_Alola: return "Meowth-Alola";
		case Species::Persian_Alola: return "Persian-Alola";
		case Species::Geodude_Alola: return "Geodude-Alola";
		case Species::Graveler_Alola: return "Graveler-Alola";
		case Species::Golem_Alola: return "Golem-Alola";
		case Species::Grimer_Alola: return "Grimer-Alola";
		case Species::Muk_Alola: return "Muk-Alola";
		case Species::Exeggutor_Alola: return "Exeggutor-Alola";
		case Species::Marowak_Alola: return "Marowak-Alola";

		// Generation 8
		case Species::Grookey: return "Grookey";
		case Species::Thwackey: return "Thwackey";
		case Species::Rillaboom: return "Rillaboom";
		case Species::Scorbunny: return "Scorbunny";
		case Species::Raboot: return "Raboot";
		case Species::Cinderace: return "Cinderace";
		case Species::Sobble: return "Sobble";
		case Species::Drizzile: return "Drizzile";
		case Species::Inteleon: return "Inteleon";
		case Species::Skwovet: return "Skwovet";
		case Species::Greedent: return "Greedent";
		case Species::Rookidee: return "Rookidee";
		case Species::Corvisquire: return "Corvisquire";
		case Species::Corviknight: return "Corviknight";
		case Species::Blipbug: return "Blipbug";
		case Species::Dottler: return "Dottler";
		case Species::Orbeetle: return "Orbeetle";
		case Species::Nickit: return "Nickit";
		case Species::Thievul: return "Thievul";
		case Species::Gossifleur: return "Gossifleur";
		case Species::Eldegoss: return "Eldegoss";
		case Species::Wooloo: return "Wooloo";
		case Species::Dubwool: return "Dubwool";
		case Species::Chewtle: return "Chewtle";
		case Species::Drednaw: return "Drednaw";
		case Species::Yamper: return "Yamper";
		case Species::Boltund: return "Boltund";
		case Species::Rolycoly: return "Rolycoly";
		case Species::Carkol: return "Carkol";
		case Species::Coalossal: return "Coalossal";
		case Species::Applin: return "Applin";
		case Species::Flapple: return "Flapple";
		case Species::Appletun: return "Appletun";
		case Species::Silicobra: return "Silicobra";
		case Species::Sandaconda: return "Sandaconda";
		case Species::Cramorant: return "Cramorant";
		case Species::Arrokuda: return "Arrokuda";
		case Species::Barraskewda: return "Barraskewda";
		case Species::Toxel: return "Toxel";
		case Species::Toxtricity_Amped: return "Toxtricity Amped";
		case Species::Toxtricity_Low_Key: return "Toxtricity Low Key";
		case Species::Sizzlipede: return "Sizzlipede";
		case Species::Centiskorch: return "Centiskorch";
		case Species::Clobbopus: return "Clobbopus";
		case Species::Grapploct: return "Grapploct";
		case Species::Sinistea: return "Sinistea";
		case Species::Polteageist: return "Polteageist";
		case Species::Hatenna: return "Hatenna";
		case Species::Hattrem: return "Hattrem";
		case Species::Hatterene: return "Hatterene";
		case Species::Impidimp: return "Impidimp";
		case Species::Morgrem: return "Morgrem";
		case Species::Grimmsnarl: return "Grimmsnarl";
		case Species::Obstagoon: return "Obstagoon";
		case Species::Perrserker: return "Perrserker";
		case Species::Cursola: return "Cursola";
		case Species::Sirfetchd: return "Sirfetch'd";
		case Species::Mr_Rime: return "Mr. Rime";
		case Species::Runerigus: return "Runerigus";
		case Species::Milcery: return "Milcery";
		case Species::Alcremie: return "Alcremie";
		case Species::Falinks: return "Falinks";
		case Species::Pincurchin: return "Pincurchin";
		case Species::Snom: return "Snom";
		case Species::Frosmoth: return "Frosmoth";
		case Species::Stonjourner: return "Stonjourner";
		case Species::Eiscue: return "Eiscue";
		case Species::Indeedee_F: return "Indeedee-F";
		case Species::Indeedee_M: return "Indeedee-M";
		case Species::Morpeko: return "Morpeko";
		case Species::Cufant: return "Cufant";
		case Species::Copperajah: return "Copperajah";
		case Species::Dracozolt: return "Dracozolt";
		case Species::Arctozolt: return "Arctozolt";
		case Species::Dracovish: return "Dracovish";
		case Species::Arctovish: return "Arctovish";
		case Species::Duraludon: return "Duraludon";
		case Species::Dreepy: return "Dreepy";
		case Species::Drakloak: return "Drakloak";
		case Species::Dragapult: return "Dragapult";
		case Species::Zacian: return "Zacian";
		case Species::Zacian_Crowned_Sword: return "Zacian Crowned Sword";
		case Species::Zamazenta: return "Zamazenta";
		case Species::Zamazenta_Crowned_Shield: return "Zamazenta Crowned Shield";
		case Species::Eternatus: return "Eternatus";
		case Species::Kubfu: return "Kubfu";
		case Species::Urshifu_Single_Strike: return "Urshifu Single Strike";
		case Species::Urshifu_Rapid_Strike: return "Urshifu Rapid Strike";
		case Species::Zarude: return "Zarude";
		case Species::Regieleki: return "Regieleki";
		case Species::Regidrago: return "Regidrago";
		case Species::Glastrier: return "Glastrier";
		case Species::Spectrier: return "Spectrier";
		case Species::Calyrex: return "Calyrex";
		case Species::Calyrex_Ice_Rider: return "Calyrex Ice Rider";
		case Species::Calyrex_Shadow_Rider: return "Calyrex Shadow Rider";
		case Species::Meowth_Galar: return "Meowth-Galar";
		case Species::Ponyta_Galar: return "Ponyta-Galar";
		case Species::Rapidash_Galar: return "Rapidash-Galar";
		case Species::Farfetchd_Galar: return "Farfetch'd-Galar";
		case Species::Weezing_Galar: return "Weezing-Galar";
		case Species::Mr_Mime_Galar: return "Mr. Mime-Galar";
		case Species::Corsola_Galar: return "Corsola-Galar";
		case Species::Zigzagoon_Galar: return "Zigzagoon-Galar";
		case Species::Linoone_Galar: return "Linoone-Galar";
		case Species::Darumaka_Galar: return "Darumaka-Galar";
		case Species::Darmanitan_Galar: return "Darmanitan-Galar";
		case Species::Yamask_Galar: return "Yamask-Galar";
		case Species::Stunfisk_Galar: return "Stunfisk-Galar";
		case Species::Slowpoke_Galar: return "Slowpoke-Galar";
		case Species::Slowbro_Galar: return "Slowbro-Galar";
		case Species::Articuno_Galar: return "Articuno-Galar";
		case Species::Zapdos_Galar: return "Zapdos-Galar";
		case Species::Moltres_Galar: return "Moltres-Galar";
		case Species::Slowking_Galar: return "Slowking-Galar";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Species {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Species>>({
			{"abomasnow", Species::Abomasnow},
			{"abomasnowmega", Species::Abomasnow},
			{"abra", Species::Abra},
			{"absol", Species::Absol},
			{"absolmega", Species::Absol},
			{"accelgor", Species::Accelgor},
			{"aegislash", Species::Aegislash},
			{"aerodactyl", Species::Aerodactyl},
			{"aerodactylmega", Species::Aerodactyl},
			{"aggron", Species::Aggron},
			{"aggronmega", Species::Aggron},
			{"aipom", Species::Aipom},
			{"alakazam", Species::Alakazam},
			{"alakazammega", Species::Alakazam},
			{"alcremie", Species::Alcremie},
			{"alcremiecaramel", Species::Alcremie},
			{"alcremielemon", Species::Alcremie},
			{"alcremiematcha", Species::Alcremie},
			{"alcremiemint", Species::Alcremie},
			{"alcremierainbow", Species::Alcremie},
			{"alcremierubycream", Species::Alcremie},
			{"alcremierubyswirl", Species::Alcremie},
			{"alcremiesalted", Species::Alcremie},
			{"alcremievanilla", Species::Alcremie},
			{"alomomola", Species::Alomomola},
			{"altaria", Species::Altaria},
			{"altariamega", Species::Altaria},
			{"amaura", Species::Amaura},
			{"ambipom", Species::Ambipom},
			{"amoonguss", Species::Amoonguss},
			{"ampharos", Species::Ampharos},
			{"ampharosmega", Species::Ampharos},
			{"anorith", Species::Anorith},
			{"appletun", Species::Appletun},
			{"applin", Species::Applin},
			{"araquanid", Species::Araquanid},
			{"araquanidtotem", Species::Araquanid},
			{"arbok", Species::Arbok},
			{"arcanine", Species::Arcanine},
			{"arceus", Species::Arceus},
			{"arceusbug", Species::Arceus},
			{"arceusdark", Species::Arceus},
			{"arceusdragon", Species::Arceus},
			{"arceuselectric", Species::Arceus},
			{"arceusfighting", Species::Arceus},
			{"arceusfire", Species::Arceus},
			{"arceusflying", Species::Arceus},
			{"arceusghost", Species::Arceus},
			{"arceusgrass", Species::Arceus},
			{"arceusground", Species::Arceus},
			{"arceusice", Species::Arceus},
			{"arceusnormal", Species::Arceus},
			{"arceuspoison", Species::Arceus},
			{"arceuspsychic", Species::Arceus},
			{"arceusrock", Species::Arceus},
			{"arceussteel", Species::Arceus},
			{"arceuswater", Species::Arceus},
			{"archen", Species::Archen},
			{"archeops", Species::Archeops},
			{"arctovish", Species::Arctovish},
			{"arctozolt", Species::Arctozolt},
			{"ariados", Species::Ariados},
			{"armaldo", Species::Armaldo},
			{"aromatisse", Species::Aromatisse},
			{"aron", Species::Aron},
			{"arrokuda", Species::Arrokuda},
			{"articuno", Species::Articuno},
			{"articunogalar", Species::Articuno_Galar},
			{"audino", Species::Audino},
			{"audinomega", Species::Audino},
			{"aurorus", Species::Aurorus},
			{"avalugg", Species::Avalugg},
			{"axew", Species::Axew},
			{"azelf", Species::Azelf},
			{"azumarill", Species::Azumarill},
			{"azurill", Species::Azurill},
			{"bagon", Species::Bagon},
			{"baltoy", Species::Baltoy},
			{"banette", Species::Banette},
			{"banettemega", Species::Banette},
			{"barbaracle", Species::Barbaracle},
			{"barboach", Species::Barboach},
			{"barraskewda", Species::Barraskewda},
			{"basculin", Species::Basculin},
			{"basculinbluestriped", Species::Basculin},
			{"bastiodon", Species::Bastiodon},
			{"bayleef", Species::Bayleef},
			{"beartic", Species::Beartic},
			{"beautifly", Species::Beautifly},
			{"beedrill", Species::Beedrill},
			{"beedrillmega", Species::Beedrill},
			{"beheeyem", Species::Beheeyem},
			{"beldum", Species::Beldum},
			{"bellossom", Species::Bellossom},
			{"bellsprout", Species::Bellsprout},
			{"bergmite", Species::Bergmite},
			{"bewear", Species::Bewear},
			{"bibarel", Species::Bibarel},
			{"bidoof", Species::Bidoof},
			{"binacle", Species::Binacle},
			{"bisharp", Species::Bisharp},
			{"blacephalon", Species::Blacephalon},
			{"blastoise", Species::Blastoise},
			{"blastoisemega", Species::Blastoise},
			{"blaziken", Species::Blaziken},
			{"blipbug", Species::Blipbug},
			{"blissey", Species::Blissey},
			{"blitzle", Species::Blitzle},
			{"boldore", Species::Boldore},
			{"boltund", Species::Boltund},
			{"bonsly", Species::Bonsly},
			{"bouffalant", Species::Bouffalant},
			{"bounsweet", Species::Bounsweet},
			{"braixen", Species::Braixen},
			{"braviary", Species::Braviary},
			{"breloom", Species::Breloom},
			{"brionne", Species::Brionne},
			{"bronzong", Species::Bronzong},
			{"bronzor", Species::Bronzor},
			{"bruxish", Species::Bruxish},
			{"budew", Species::Budew},
			{"buizel", Species::Buizel},
			{"bulbasaur", Species::Bulbasaur},
			{"buneary", Species::Buneary},
			{"bunnelby", Species::Bunnelby},
			{"burmy", Species::Burmy},
			{"butterfree", Species::Butterfree},
			{"buzzwole", Species::Buzzwole},
			{"cacnea", Species::Cacnea},
			{"cacturne", Species::Cacturne},
			{"calyrex", Species::Calyrex},
			{"calyrexicerider", Species::Calyrex_Ice_Rider},
			{"calyrexshadowrider", Species::Calyrex_Shadow_Rider},
			{"camerupt", Species::Camerupt},
			{"cameruptmega", Species::Camerupt},
			{"carbink", Species::Carbink},
			{"carkol", Species::Carkol},
			{"carnivine", Species::Carnivine},
			{"carracosta", Species::Carracosta},
			{"carvanha", Species::Carvanha},
			{"cascoon", Species::Cascoon},
			{"castform", Species::Castform},
			{"caterpie", Species::Caterpie},
			{"celebi", Species::Celebi},
			{"celesteela", Species::Celesteela},
			{"centiskorch", Species::Centiskorch},
			{"chandelure", Species::Chandelure},
			{"chansey", Species::Chansey},
			{"charizard", Species::Charizard},
			{"charizardmegax", Species::Charizard},
			{"charizardmegay", Species::Charizard},
			{"charjabug", Species::Charjabug},
			{"charmander", Species::Charmander},
			{"charmeleon", Species::Charmeleon},
			{"chatot", Species::Chatot},
			{"cherrim", Species::Cherrim},
			{"cherubi", Species::Cherubi},
			{"chesnaught", Species::Chesnaught},
			{"chespin", Species::Chespin},
			{"chewtle", Species::Chewtle},
			{"chikorita", Species::Chikorita},
			{"chimchar", Species::Chimchar},
			{"chimecho", Species::Chimecho},
			{"chinchou", Species::Chinchou},
			{"chingling", Species::Chingling},
			{"cinccino", Species::Cinccino},
			{"cinderace", Species::Cinderace},
			{"clamperl", Species::Clamperl},
			{"clauncher", Species::Clauncher},
			{"clawitzer", Species::Clawitzer},
			{"claydol", Species::Claydol},
			{"clefable", Species::Clefable},
			{"clefairy", Species::Clefairy},
			{"cleffa", Species::Cleffa},
			{"clobbopus", Species::Clobbopus},
			{"cloyster", Species::Cloyster},
			{"coalossal", Species::Coalossal},
			{"cobalion", Species::Cobalion},
			{"cofagrigus", Species::Cofagrigus},
			{"combee", Species::Combee},
			{"combusken", Species::Combusken},
			{"comfey", Species::Comfey},
			{"conkeldurr", Species::Conkeldurr},
			{"copperajah", Species::Copperajah},
			{"corphish", Species::Corphish},
			{"corsola", Species::Corsola},
			{"corsolagalar", Species::Corsola_Galar},
			{"corviknight", Species::Corviknight},
			{"corvisquire", Species::Corvisquire},
			{"cosmoem", Species::Cosmoem},
			{"cosmog", Species::Cosmog},
			{"cottonee", Species::Cottonee},
			{"crabominable", Species::Crabominable},
			{"crabrawler", Species::Crabrawler},
			{"cradily", Species::Cradily},
			{"cramorant", Species::Cramorant},
			{"cranidos", Species::Cranidos},
			{"crawdaunt", Species::Crawdaunt},
			{"cresselia", Species::Cresselia},
			{"croagunk", Species::Croagunk},
			{"crobat", Species::Crobat},
			{"croconaw", Species::Croconaw},
			{"crustle", Species::Crustle},
			{"cryogonal", Species::Cryogonal},
			{"cubchoo", Species::Cubchoo},
			{"cubone", Species::Cubone},
			{"cufant", Species::Cufant},
			{"cursola", Species::Cursola},
			{"cutiefly", Species::Cutiefly},
			{"cyndaquil", Species::Cyndaquil},
			{"darkrai", Species::Darkrai},
			{"darmanitan", Species::Darmanitan},
			{"darmanitangalar", Species::Darmanitan_Galar},
			{"dartrix", Species::Dartrix},
			{"darumaka", Species::Darumaka},
			{"darumakagalar", Species::Darumaka_Galar},
			{"decidueye", Species::Decidueye},
			{"dedenne", Species::Dedenne},
			{"deerling", Species::Deerling},
			{"deino", Species::Deino},
			{"delcatty", Species::Delcatty},
			{"delibird", Species::Delibird},
			{"delphox", Species::Delphox},
			{"deoxys", Species::Deoxys_Normal},
			{"deoxysattack", Species::Deoxys_Attack},
			{"deoxysdefense", Species::Deoxys_Defense},
			{"deoxyse", Species::Deoxys_Speed},
			{"deoxysf", Species::Deoxys_Attack},
			{"deoxysl", Species::Deoxys_Defense},
			{"deoxysnormal", Species::Deoxys_Normal},
			{"deoxysspeed", Species::Deoxys_Speed},
			{"dewgong", Species::Dewgong},
			{"dewott", Species::Dewott},
			{"dewpider", Species::Dewpider},
			{"dhelmise", Species::Dhelmise},
			{"dialga", Species::Dialga},
			{"diancie", Species::Diancie},
			{"dianciemega", Species::Diancie},
			{"diggersby", Species::Diggersby},
			{"diglett", Species::Diglett},
			{"diglettalola", Species::Diglett_Alola},
			{"ditto", Species::Ditto},
			{"dodrio", Species::Dodrio},
			{"doduo", Species::Doduo},
			{"donphan", Species::Donphan},
			{"dottler", Species::Dottler},
			{"doublade", Species::Doublade},
			{"dracovish", Species::Dracovish},
			{"dracozolt", Species::Dracozolt},
			{"dragalge", Species::Dragalge},
			{"dragapult", Species::Dragapult},
			{"dragonair", Species::Dragonair},
			{"dragonite", Species::Dragonite},
			{"drakloak", Species::Drakloak},
			{"drampa", Species::Drampa},
			{"drapion", Species::Drapion},
			{"dratini", Species::Dratini},
			{"drednaw", Species::Drednaw},
			{"dreepy", Species::Dreepy},
			{"drifblim", Species::Drifblim},
			{"drifloon", Species::Drifloon},
			{"drilbur", Species::Drilbur},
			{"drizzile", Species::Drizzile},
			{"drowzee", Species::Drowzee},
			{"druddigon", Species::Druddigon},
			{"dubwool", Species::Dubwool},
			{"ducklett", Species::Ducklett},
			{"dugtrio", Species::Dugtrio},
			{"dugtrioalola", Species::Dugtrio_Alola},
			{"dunsparce", Species::Dunsparce},
			{"duosion", Species::Duosion},
			{"duraludon", Species::Duraludon},
			{"durant", Species::Durant},
			{"dusclops", Species::Dusclops},
			{"dusknoir", Species::Dusknoir},
			{"duskull", Species::Duskull},
			{"dustox", Species::Dustox},
			{"dwebble", Species::Dwebble},
			{"eelektrik", Species::Eelektrik},
			{"eelektross", Species::Eelektross},
			{"eevee", Species::Eevee},
			{"eiscue", Species::Eiscue},
			{"ekans", Species::Ekans},
			{"eldegoss", Species::Eldegoss},
			{"electabuzz", Species::Electabuzz},
			{"electivire", Species::Electivire},
			{"electrike", Species::Electrike},
			{"electrode", Species::Electrode},
			{"elekid", Species::Elekid},
			{"elgyem", Species::Elgyem},
			{"emboar", Species::Emboar},
			{"emolga", Species::Emolga},
			{"empoleon", Species::Empoleon},
			{"entei", Species::Entei},
			{"escavalier", Species::Escavalier},
			{"espeon", Species::Espeon},
			{"espurr", Species::Espurr},
			{"eternatus", Species::Eternatus},
			{"excadrill", Species::Excadrill},
			{"exeggcute", Species::Exeggcute},
			{"exeggutor", Species::Exeggutor},
			{"exeggutoralola", Species::Exeggutor_Alola},
			{"exploud", Species::Exploud},
			{"falinks", Species::Falinks},
			{"farfetchd", Species::Farfetchd},
			{"farfetchdgalar", Species::Farfetchd_Galar},
			{"fearow", Species::Fearow},
			{"feebas", Species::Feebas},
			{"fennekin", Species::Fennekin},
			{"feraligatr", Species::Feraligatr},
			{"ferroseed", Species::Ferroseed},
			{"ferrothorn", Species::Ferrothorn},
			{"finneon", Species::Finneon},
			{"flaaffy", Species::Flaaffy},
			{"flabebe", Species::Flabebe},
			{"flapple", Species::Flapple},
			{"flareon", Species::Flareon},
			{"fletchinder", Species::Fletchinder},
			{"fletchling", Species::Fletchling},
			{"floatzel", Species::Floatzel},
			{"floette", Species::Floette},
			{"florges", Species::Florges},
			{"flygon", Species::Flygon},
			{"fomantis", Species::Fomantis},
			{"foongus", Species::Foongus},
			{"forretress", Species::Forretress},
			{"fraxure", Species::Fraxure},
			{"frillish", Species::Frillish},
			{"froakie", Species::Froakie},
			{"frogadier", Species::Frogadier},
			{"froslass", Species::Froslass},
			{"frosmoth", Species::Frosmoth},
			{"furfrou", Species::Furfrou},
			{"furret", Species::Furret},
			{"gabite", Species::Gabite},
			{"gallade", Species::Gallade},
			{"gallademega", Species::Gallade},
			{"galvantula", Species::Galvantula},
			{"garbodor", Species::Garbodor},
			{"garchomp", Species::Garchomp},
			{"garchompmega", Species::Garchomp},
			{"gardevoir", Species::Gardevoir},
			{"gardevoirmega", Species::Gardevoir},
			{"gastly", Species::Gastly},
			{"gastrodon", Species::Gastrodon},
			{"gastrodoneast", Species::Gastrodon},
			{"gastrodonwest", Species::Gastrodon},
			{"genesect", Species::Genesect},
			{"genesectburn", Species::Genesect},
			{"genesectchill", Species::Genesect},
			{"genesectdouse", Species::Genesect},
			{"genesectshock", Species::Genesect},
			{"gengar", Species::Gengar},
			{"geodude", Species::Geodude},
			{"geodudealola", Species::Geodude_Alola},
			{"gible", Species::Gible},
			{"gigalith", Species::Gigalith},
			{"girafarig", Species::Girafarig},
			{"giratina", Species::Giratina_Altered},
			{"giratinaaltered", Species::Giratina_Altered},
			{"giratinaorigin", Species::Giratina_Origin},
			{"glaceon", Species::Glaceon},
			{"glalie", Species::Glalie},
			{"glaliemega", Species::Glalie},
			{"glameow", Species::Glameow},
			{"glastrier", Species::Glastrier},
			{"gligar", Species::Gligar},
			{"gliscor", Species::Gliscor},
			{"gloom", Species::Gloom},
			{"gogoat", Species::Gogoat},
			{"golbat", Species::Golbat},
			{"goldeen", Species::Goldeen},
			{"golduck", Species::Golduck},
			{"golem", Species::Golem},
			{"golemalola", Species::Golem_Alola},
			{"golett", Species::Golett},
			{"golisopod", Species::Golisopod},
			{"golurk", Species::Golurk},
			{"goodra", Species::Goodra},
			{"goomy", Species::Goomy},
			{"gorebyss", Species::Gorebyss},
			{"gossifleur", Species::Gossifleur},
			{"gothita", Species::Gothita},
			{"gothitelle", Species::Gothitelle},
			{"gothorita", Species::Gothorita},
			{"gourgeist", Species::Gourgeist_Average},
			{"gourgeistaverage", Species::Gourgeist_Average},
			{"gourgeistlarge", Species::Gourgeist_Large},
			{"gourgeistsmall", Species::Gourgeist_Small},
			{"gourgeistsuper", Species::Gourgeist_Super},
			{"granbull", Species::Granbull},
			{"grapploct", Species::Grapploct},
			{"graveler", Species::Graveler},
			{"graveleralola", Species::Graveler_Alola},
			{"greedent", Species::Greedent},
			{"greninja", Species::Greninja},
			{"greninjaash", Species::Greninja},
			{"grimer", Species::Grimer},
			{"grimeralola", Species::Grimer_Alola},
			{"grimmsnarl", Species::Grimmsnarl},
			{"grookey", Species::Grookey},
			{"grotle", Species::Grotle},
			{"groudon", Species::Groudon},
			{"grovyle", Species::Grovyle},
			{"growlithe", Species::Growlithe},
			{"grubbin", Species::Grubbin},
			{"grumpig", Species::Grumpig},
			{"gulpin", Species::Gulpin},
			{"gumshoos", Species::Gumshoos},
			{"gumshoostotem", Species::Gumshoos}, // TODO: ???
			{"gurdurr", Species::Gurdurr},
			{"guzzlord", Species::Guzzlord},
			{"gyarados", Species::Gyarados},
			{"gyaradosmega", Species::Gyarados},
			{"hakamoo", Species::Hakamo_o},
			{"happiny", Species::Happiny},
			{"hariyama", Species::Hariyama},
			{"hatenna", Species::Hatenna},
			{"hatterene", Species::Hatterene},
			{"hattrem", Species::Hattrem},
			{"haunter", Species::Haunter},
			{"hawlucha", Species::Hawlucha},
			{"haxorus", Species::Haxorus},
			{"heatmor", Species::Heatmor},
			{"heatran", Species::Heatran},
			{"heliolisk", Species::Heliolisk},
			{"helioptile", Species::Helioptile},
			{"heracross", Species::Heracross},
			{"heracrossmega", Species::Heracross},
			{"herdier", Species::Herdier},
			{"hippopotas", Species::Hippopotas},
			{"hippowdon", Species::Hippowdon},
			{"hitmonchan", Species::Hitmonchan},
			{"hitmonlee", Species::Hitmonlee},
			{"hitmontop", Species::Hitmontop},
			{"honchkrow", Species::Honchkrow},
			{"honedge", Species::Honedge},
			{"hooh", Species::Ho_Oh},
			{"hoopa", Species::Hoopa_Confined},
			{"hoopaconfined", Species::Hoopa_Confined},
			{"hoopaunbound", Species::Hoopa_Unbound},
			{"hoothoot", Species::Hoothoot},
			{"hoppip", Species::Hoppip},
			{"horsea", Species::Horsea},
			{"houndoom", Species::Houndoom},
			{"houndoommega", Species::Houndoom},
			{"houndour", Species::Houndour},
			{"huntail", Species::Huntail},
			{"hydreigon", Species::Hydreigon},
			{"hypno", Species::Hypno},
			{"igglybuff", Species::Igglybuff},
			{"illumise", Species::Illumise},
			{"impidimp", Species::Impidimp},
			{"incineroar", Species::Incineroar},
			{"indeedee", Species::Indeedee_M},
			{"indeedeef", Species::Indeedee_F},
			{"indeedeem", Species::Indeedee_M},
			{"infernape", Species::Infernape},
			{"inkay", Species::Inkay},
			{"inteleon", Species::Inteleon},
			{"ivysaur", Species::Ivysaur},
			{"jangmoo", Species::Jangmo_o},
			{"jellicent", Species::Jellicent},
			{"jigglypuff", Species::Jigglypuff},
			{"jirachi", Species::Jirachi},
			{"jolteon", Species::Jolteon},
			{"joltik", Species::Joltik},
			{"jumpluff", Species::Jumpluff},
			{"jynx", Species::Jynx},
			{"kabuto", Species::Kabuto},
			{"kabutops", Species::Kabutops},
			{"kadabra", Species::Kadabra},
			{"kakuna", Species::Kakuna},
			{"kangaskhan", Species::Kangaskhan},
			{"karrablast", Species::Karrablast},
			{"kartana", Species::Kartana},
			{"kecleon", Species::Kecleon},
			{"keldeo", Species::Keldeo},
			{"keldeoresolute", Species::Keldeo_Resolute},
			{"kingdra", Species::Kingdra},
			{"kingler", Species::Kingler},
			{"kirlia", Species::Kirlia},
			{"klang", Species::Klang},
			{"klefki", Species::Klefki},
			{"klink", Species::Klink},
			{"klinklang", Species::Klinklang},
			{"koffing", Species::Koffing},
			{"komala", Species::Komala},
			{"kommoo", Species::Kommo_o},
			{"kommoototem", Species::Kommo_o},
			{"krabby", Species::Krabby},
			{"kricketot", Species::Kricketot},
			{"kricketune", Species::Kricketune},
			{"krokorok", Species::Krokorok},
			{"krookodile", Species::Krookodile},
			{"kubfu", Species::Kubfu},
			{"kyogre", Species::Kyogre},
			{"kyurem", Species::Kyurem},
			{"kyuremblack", Species::Kyurem_Black},
			{"kyuremwhite", Species::Kyurem_White},
			{"lairon", Species::Lairon},
			{"lampent", Species::Lampent},
			{"landorus", Species::Landorus_Incarnate},
			{"landorusincarnate", Species::Landorus_Incarnate},
			{"landorustherian", Species::Landorus_Therian},
			{"lanturn", Species::Lanturn},
			{"lapras", Species::Lapras},
			{"larvesta", Species::Larvesta},
			{"larvitar", Species::Larvitar},
			{"latias", Species::Latias},
			{"latiasmega", Species::Latias},
			{"latios", Species::Latios},
			{"latiosmega", Species::Latios},
			{"leafeon", Species::Leafeon},
			{"leavanny", Species::Leavanny},
			{"ledian", Species::Ledian},
			{"ledyba", Species::Ledyba},
			{"lickilicky", Species::Lickilicky},
			{"lickitung", Species::Lickitung},
			{"liepard", Species::Liepard},
			{"lileep", Species::Lileep},
			{"lilligant", Species::Lilligant},
			{"lillipup", Species::Lillipup},
			{"linoone", Species::Linoone},
			{"linoonegalar", Species::Linoone_Galar},
			{"litleo", Species::Litleo},
			{"litten", Species::Litten},
			{"litwick", Species::Litwick},
			{"lombre", Species::Lombre},
			{"lopunny", Species::Lopunny},
			{"lopunnymega", Species::Lopunny},
			{"lotad", Species::Lotad},
			{"loudred", Species::Loudred},
			{"lucario", Species::Lucario},
			{"ludicolo", Species::Ludicolo},
			{"lugia", Species::Lugia},
			{"lumineon", Species::Lumineon},
			{"lunala", Species::Lunala},
			{"lunatone", Species::Lunatone},
			{"lurantis", Species::Lurantis},
			{"lurantistotem", Species::Lurantis},
			{"luvdisc", Species::Luvdisc},
			{"luxio", Species::Luxio},
			{"luxray", Species::Luxray},
			{"lycanroc", Species::Lycanroc_Midday},
			{"lycanrocdusk", Species::Lycanroc_Dusk},
			{"lycanrocmidday", Species::Lycanroc_Midday},
			{"lycanrocmidnight", Species::Lycanroc_Midnight},
			{"machamp", Species::Machamp},
			{"machoke", Species::Machoke},
			{"machop", Species::Machop},
			{"magby", Species::Magby},
			{"magcargo", Species::Magcargo},
			{"magearna", Species::Magearna},
			{"magikarp", Species::Magikarp},
			{"magmar", Species::Magmar},
			{"magmortar", Species::Magmortar},
			{"magnemite", Species::Magnemite},
			{"magneton", Species::Magneton},
			{"magnezone", Species::Magnezone},
			{"makuhita", Species::Makuhita},
			{"malamar", Species::Malamar},
			{"mamoswine", Species::Mamoswine},
			{"manaphy", Species::Manaphy},
			{"mandibuzz", Species::Mandibuzz},
			{"manectric", Species::Manectric},
			{"manectricmega", Species::Manectric},
			{"mankey", Species::Mankey},
			{"mantine", Species::Mantine},
			{"mantyke", Species::Mantyke},
			{"maractus", Species::Maractus},
			{"mareanie", Species::Mareanie},
			{"mareep", Species::Mareep},
			{"marill", Species::Marill},
			{"marowak", Species::Marowak},
			{"marowakalola", Species::Marowak_Alola},
			{"marowakalolatotem", Species::Marowak_Alola},
			{"marshadow", Species::Marshadow},
			{"marshtomp", Species::Marshtomp},
			{"masquerain", Species::Masquerain},
			{"mawile", Species::Mawile},
			{"mawilemega", Species::Mawile},
			{"medicham", Species::Medicham},
			{"medichammega", Species::Medicham},
			{"meditite", Species::Meditite},
			{"meganium", Species::Meganium},
			{"melmetal", Species::Melmetal},
			{"meloetta", Species::Meloetta},
			{"meltan", Species::Meltan},
			{"meowstic", Species::Meowstic_M},
			{"meowsticf", Species::Meowstic_F},
			{"meowsticm", Species::Meowstic_M},
			{"meowth", Species::Meowth},
			{"meowthalola", Species::Meowth_Alola},
			{"meowthgalar", Species::Meowth_Galar},
			{"mesprit", Species::Mesprit},
			{"metagross", Species::Metagross},
			{"metagrossmega", Species::Metagross},
			{"metang", Species::Metang},
			{"metapod", Species::Metapod},
			{"mew", Species::Mew},
			{"mewtwo", Species::Mewtwo},
			{"mienfoo", Species::Mienfoo},
			{"mienshao", Species::Mienshao},
			{"mightyena", Species::Mightyena},
			{"milcery", Species::Milcery},
			{"milotic", Species::Milotic},
			{"miltank", Species::Miltank},
			{"mimejr", Species::Mime_Jr},
			{"mimikyu", Species::Mimikyu},
			{"mimikyutotem", Species::Mimikyu},
			{"minccino", Species::Minccino},
			{"minior", Species::Minior_Core},
			{"miniorcore", Species::Minior_Core},
			{"miniormeteor", Species::Minior_Meteor},
			{"minun", Species::Minun},
			{"misdreavus", Species::Misdreavus},
			{"mismagius", Species::Mismagius},
			{"moltres", Species::Moltres},
			{"moltresgalar", Species::Moltres_Galar},
			{"monferno", Species::Monferno},
			{"morelull", Species::Morelull},
			{"morgrem", Species::Morgrem},
			{"morpeko", Species::Morpeko},
			{"mothim", Species::Mothim},
			{"mrmime", Species::Mr_Mime},
			{"mrmimegalar", Species::Mr_Mime_Galar},
			{"mrrime", Species::Mr_Rime},
			{"mudbray", Species::Mudbray},
			{"mudkip", Species::Mudkip},
			{"mudsdale", Species::Mudsdale},
			{"muk", Species::Muk},
			{"mukalola", Species::Muk_Alola},
			{"munchlax", Species::Munchlax},
			{"munna", Species::Munna},
			{"murkrow", Species::Murkrow},
			{"musharna", Species::Musharna},
			{"naganadel", Species::Naganadel},
			{"natu", Species::Natu},
			{"necrozma", Species::Necrozma},
			{"necrozmadawnwings", Species::Necrozma_Dawn_Wings},
			{"necrozmaduskmane", Species::Necrozma_Dusk_Mane},
			{"necrozmaultra", Species::Necrozma_Ultra},
			{"nickit", Species::Nickit},
			{"nidoking", Species::Nidoking},
			{"nidoqueen", Species::Nidoqueen},
			{"nidoranf", Species::Nidoran_F},
			{"nidoranm", Species::Nidoran_M},
			{"nidorina", Species::Nidorina},
			{"nidorino", Species::Nidorino},
			{"nihilego", Species::Nihilego},
			{"nincada", Species::Nincada},
			{"ninetales", Species::Ninetales},
			{"ninetalesalola", Species::Ninetales_Alola},
			{"ninjask", Species::Ninjask},
			{"noctowl", Species::Noctowl},
			{"noibat", Species::Noibat},
			{"noivern", Species::Noivern},
			{"nosepass", Species::Nosepass},
			{"numel", Species::Numel},
			{"nuzleaf", Species::Nuzleaf},
			{"obstagoon", Species::Obstagoon},
			{"octillery", Species::Octillery},
			{"oddish", Species::Oddish},
			{"omanyte", Species::Omanyte},
			{"omastar", Species::Omastar},
			{"onix", Species::Onix},
			{"oranguru", Species::Oranguru},
			{"orbeetle", Species::Orbeetle},
			{"oricorio", Species::Oricorio_Baile},
			{"oricoriobaile", Species::Oricorio_Baile},
			{"oricoriopau", Species::Oricorio_Pau},
			{"oricoriopompom", Species::Oricorio_Pom_Pom},
			{"oricoriosensu", Species::Oricorio_Sensu},
			{"oshawott", Species::Oshawott},
			{"pachirisu", Species::Pachirisu},
			{"palkia", Species::Palkia},
			{"palossand", Species::Palossand},
			{"palpitoad", Species::Palpitoad},
			{"pancham", Species::Pancham},
			{"pangoro", Species::Pangoro},
			{"panpour", Species::Panpour},
			{"pansage", Species::Pansage},
			{"pansear", Species::Pansear},
			{"paras", Species::Paras},
			{"parasect", Species::Parasect},
			{"passimian", Species::Passimian},
			{"patrat", Species::Patrat},
			{"pawniard", Species::Pawniard},
			{"pelipper", Species::Pelipper},
			{"perrserker", Species::Perrserker},
			{"persian", Species::Persian},
			{"persianalola", Species::Persian_Alola},
			{"petilil", Species::Petilil},
			{"phanpy", Species::Phanpy},
			{"phantump", Species::Phantump},
			{"pheromosa", Species::Pheromosa},
			{"phione", Species::Phione},
			{"pichu", Species::Pichu},
			{"pidgeot", Species::Pidgeot},
			{"pidgeotmega", Species::Pidgeot},
			{"pidgeotto", Species::Pidgeotto},
			{"pidgey", Species::Pidgey},
			{"pidove", Species::Pidove},
			{"pignite", Species::Pignite},
			{"pikachu", Species::Pikachu},
			{"pikachualola", Species::Pikachu},
			{"pikachuhoenn", Species::Pikachu},
			{"pikachukalos", Species::Pikachu},
			{"pikachuoriginal", Species::Pikachu},
			{"pikachupartner", Species::Pikachu},
			{"pikachusinnoh", Species::Pikachu},
			{"pikachuunova", Species::Pikachu},
			{"pikachuworld", Species::Pikachu},
			{"pikipek", Species::Pikipek},
			{"piloswine", Species::Piloswine},
			{"pincurchin", Species::Pincurchin},
			{"pineco", Species::Pineco},
			{"pinsir", Species::Pinsir},
			{"pinsirmega", Species::Pinsir},
			{"piplup", Species::Piplup},
			{"plusle", Species::Plusle},
			{"poipole", Species::Poipole},
			{"politoed", Species::Politoed},
			{"poliwag", Species::Poliwag},
			{"poliwhirl", Species::Poliwhirl},
			{"poliwrath", Species::Poliwrath},
			{"polteageist", Species::Polteageist},
			{"ponyta", Species::Ponyta},
			{"ponytagalar", Species::Ponyta_Galar},
			{"poochyena", Species::Poochyena},
			{"popplio", Species::Popplio},
			{"porygon", Species::Porygon},
			{"porygon2", Species::Porygon2},
			{"porygonz", Species::Porygon_Z},
			{"primarina", Species::Primarina},
			{"primeape", Species::Primeape},
			{"prinplup", Species::Prinplup},
			{"probopass", Species::Probopass},
			{"psyduck", Species::Psyduck},
			{"pumpkaboo", Species::Pumpkaboo_Average},
			{"pumpkabooaverage", Species::Pumpkaboo_Average},
			{"pumpkaboolarge", Species::Pumpkaboo_Large},
			{"pumpkaboosmall", Species::Pumpkaboo_Small},
			{"pumpkaboosuper", Species::Pumpkaboo_Super},
			{"pupitar", Species::Pupitar},
			{"purrloin", Species::Purrloin},
			{"purugly", Species::Purugly},
			{"pyroar", Species::Pyroar},
			{"pyukumuku", Species::Pyukumuku},
			{"quagsire", Species::Quagsire},
			{"quilava", Species::Quilava},
			{"quilladin", Species::Quilladin},
			{"qwilfish", Species::Qwilfish},
			{"raboot", Species::Raboot},
			{"raichu", Species::Raichu},
			{"raichualola", Species::Raichu_Alola},
			{"raikou", Species::Raikou},
			{"ralts", Species::Ralts},
			{"rampardos", Species::Rampardos},
			{"rapidash", Species::Rapidash},
			{"rapidashgalar", Species::Rapidash_Galar},
			{"raticate", Species::Raticate},
			{"raticatealola", Species::Raticate_Alola},
			{"raticatealolatotem", Species::Raticate_Alola},
			{"rattata", Species::Rattata},
			{"rattataalola", Species::Rattata_Alola},
			{"rayquaza", Species::Rayquaza},
			{"regice", Species::Regice},
			{"regidrago", Species::Regidrago},
			{"regieleki", Species::Regieleki},
			{"regigigas", Species::Regigigas},
			{"regirock", Species::Regirock},
			{"registeel", Species::Registeel},
			{"relicanth", Species::Relicanth},
			{"remoraid", Species::Remoraid},
			{"reshiram", Species::Reshiram},
			{"reuniclus", Species::Reuniclus},
			{"rhydon", Species::Rhydon},
			{"rhyhorn", Species::Rhyhorn},
			{"rhyperior", Species::Rhyperior},
			{"ribombee", Species::Ribombee},
			{"ribombeetotem", Species::Ribombee},
			{"rillaboom", Species::Rillaboom},
			{"riolu", Species::Riolu},
			{"rockruff", Species::Rockruff},
			{"roggenrola", Species::Roggenrola},
			{"rolycoly", Species::Rolycoly},
			{"rookidee", Species::Rookidee},
			{"roselia", Species::Roselia},
			{"roserade", Species::Roserade},
			{"rotom", Species::Rotom},
			{"rotomc", Species::Rotom_Mow},
			{"rotomf", Species::Rotom_Frost},
			{"rotomfan", Species::Rotom_Fan},
			{"rotomfrost", Species::Rotom_Frost},
			{"rotomh", Species::Rotom_Heat},
			{"rotomheat", Species::Rotom_Heat},
			{"rotommow", Species::Rotom_Mow},
			{"rotoms", Species::Rotom_Fan},
			{"rotomw", Species::Rotom_Wash},
			{"rotomwash", Species::Rotom_Wash},
			{"rowlet", Species::Rowlet},
			{"rufflet", Species::Rufflet},
			{"runerigus", Species::Runerigus},
			{"sableye", Species::Sableye},
			{"sableyemega", Species::Sableye},
			{"salamence", Species::Salamence},
			{"salandit", Species::Salandit},
			{"salazzle", Species::Salazzle},
			{"salazzletotem", Species::Salazzle},
			{"samurott", Species::Samurott},
			{"sandaconda", Species::Sandaconda},
			{"sandile", Species::Sandile},
			{"sandshrew", Species::Sandshrew},
			{"sandshrewalola", Species::Sandshrew_Alola},
			{"sandslash", Species::Sandslash},
			{"sandslashalola", Species::Sandslash_Alola},
			{"sandygast", Species::Sandygast},
			{"sawk", Species::Sawk},
			{"sawsbuck", Species::Sawsbuck},
			{"scatterbug", Species::Scatterbug},
			{"sceptile", Species::Sceptile},
			{"sceptilemega", Species::Sceptile},
			{"scizor", Species::Scizor},
			{"scizormega", Species::Scizor},
			{"scolipede", Species::Scolipede},
			{"scorbunny", Species::Scorbunny},
			{"scrafty", Species::Scrafty},
			{"scraggy", Species::Scraggy},
			{"scyther", Species::Scyther},
			{"seadra", Species::Seadra},
			{"seaking", Species::Seaking},
			{"sealeo", Species::Sealeo},
			{"seedot", Species::Seedot},
			{"seel", Species::Seel},
			{"seismitoad", Species::Seismitoad},
			{"sentret", Species::Sentret},
			{"serperior", Species::Serperior},
			{"servine", Species::Servine},
			{"seviper", Species::Seviper},
			{"sewaddle", Species::Sewaddle},
			{"sharpedo", Species::Sharpedo},
			{"sharpedomega", Species::Sharpedo},
			{"shaymin", Species::Shaymin_Land},
			{"shayminland", Species::Shaymin_Land},
			{"shayminsky", Species::Shaymin_Sky},
			{"shedinja", Species::Shedinja},
			{"shelgon", Species::Shelgon},
			{"shellder", Species::Shellder},
			{"shellos", Species::Shellos},
			{"shelloseast", Species::Shellos},
			{"shelloswest", Species::Shellos},
			{"shelmet", Species::Shelmet},
			{"shieldon", Species::Shieldon},
			{"shiftry", Species::Shiftry},
			{"shiinotic", Species::Shiinotic},
			{"shinx", Species::Shinx},
			{"shroomish", Species::Shroomish},
			{"shuckle", Species::Shuckle},
			{"shuppet", Species::Shuppet},
			{"sigilyph", Species::Sigilyph},
			{"silcoon", Species::Silcoon},
			{"silicobra", Species::Silicobra},
			{"silvally", Species::Silvally},
			{"silvallybug", Species::Silvally},
			{"silvallydark", Species::Silvally},
			{"silvallydragon", Species::Silvally},
			{"silvallyelectric", Species::Silvally},
			{"silvallyfairy", Species::Silvally},
			{"silvallyfighting", Species::Silvally},
			{"silvallyfire", Species::Silvally},
			{"silvallyflying", Species::Silvally},
			{"silvallyghost", Species::Silvally},
			{"silvallygrass", Species::Silvally},
			{"silvallyground", Species::Silvally},
			{"silvallyice", Species::Silvally},
			{"silvallypoison", Species::Silvally},
			{"silvallypsychic", Species::Silvally},
			{"silvallyrock", Species::Silvally},
			{"silvallysteel", Species::Silvally},
			{"silvallywater", Species::Silvally},
			{"simipour", Species::Simipour},
			{"simisage", Species::Simisage},
			{"simisear", Species::Simisear},
			{"sinistea", Species::Sinistea},
			{"sirfetchd", Species::Sirfetchd},
			{"sizzlipede", Species::Sizzlipede},
			{"skarmory", Species::Skarmory},
			{"skiddo", Species::Skiddo},
			{"skiploom", Species::Skiploom},
			{"skitty", Species::Skitty},
			{"skorupi", Species::Skorupi},
			{"skrelp", Species::Skrelp},
			{"skuntank", Species::Skuntank},
			{"skwovet", Species::Skwovet},
			{"slaking", Species::Slaking},
			{"slakoth", Species::Slakoth},
			{"sliggoo", Species::Sliggoo},
			{"slowbro", Species::Slowbro},
			{"slowbrogalar", Species::Slowbro_Galar},
			{"slowbromega", Species::Slowbro},
			{"slowking", Species::Slowking},
			{"slowkinggalar", Species::Slowking_Galar},
			{"slowpoke", Species::Slowpoke},
			{"slowpokegalar", Species::Slowpoke_Galar},
			{"slugma", Species::Slugma},
			{"slurpuff", Species::Slurpuff},
			{"smeargle", Species::Smeargle},
			{"smoochum", Species::Smoochum},
			{"sneasel", Species::Sneasel},
			{"snivy", Species::Snivy},
			{"snom", Species::Snom},
			{"snorlax", Species::Snorlax},
			{"snorunt", Species::Snorunt},
			{"snover", Species::Snover},
			{"snubbull", Species::Snubbull},
			{"sobble", Species::Sobble},
			{"solgaleo", Species::Solgaleo},
			{"solosis", Species::Solosis},
			{"solrock", Species::Solrock},
			{"spearow", Species::Spearow},
			{"spectrier", Species::Spectrier},
			{"spewpa", Species::Spewpa},
			{"spheal", Species::Spheal},
			{"spinarak", Species::Spinarak},
			{"spinda", Species::Spinda},
			{"spiritomb", Species::Spiritomb},
			{"spoink", Species::Spoink},
			{"spritzee", Species::Spritzee},
			{"squirtle", Species::Squirtle},
			{"stakataka", Species::Stakataka},
			{"stantler", Species::Stantler},
			{"staraptor", Species::Staraptor},
			{"staravia", Species::Staravia},
			{"starly", Species::Starly},
			{"starmie", Species::Starmie},
			{"staryu", Species::Staryu},
			{"steelix", Species::Steelix},
			{"steelixmega", Species::Steelix},
			{"steenee", Species::Steenee},
			{"stonjourner", Species::Stonjourner},
			{"stoutland", Species::Stoutland},
			{"stufful", Species::Stufful},
			{"stunfisk", Species::Stunfisk},
			{"stunfiskgalar", Species::Stunfisk_Galar},
			{"stunky", Species::Stunky},
			{"sudowoodo", Species::Sudowoodo},
			{"suicune", Species::Suicune},
			{"sunflora", Species::Sunflora},
			{"sunkern", Species::Sunkern},
			{"surskit", Species::Surskit},
			{"swablu", Species::Swablu},
			{"swadloon", Species::Swadloon},
			{"swalot", Species::Swalot},
			{"swampert", Species::Swampert},
			{"swampertmega", Species::Swampert},
			{"swanna", Species::Swanna},
			{"swellow", Species::Swellow},
			{"swinub", Species::Swinub},
			{"swirlix", Species::Swirlix},
			{"swoobat", Species::Swoobat},
			{"sylveon", Species::Sylveon},
			{"taillow", Species::Taillow},
			{"talonflame", Species::Talonflame},
			{"tangela", Species::Tangela},
			{"tangrowth", Species::Tangrowth},
			{"tapubulu", Species::Tapu_Bulu},
			{"tapufini", Species::Tapu_Fini},
			{"tapukoko", Species::Tapu_Koko},
			{"tapulele", Species::Tapu_Lele},
			{"tauros", Species::Tauros},
			{"teddiursa", Species::Teddiursa},
			{"tentacool", Species::Tentacool},
			{"tentacruel", Species::Tentacruel},
			{"tepig", Species::Tepig},
			{"terrakion", Species::Terrakion},
			{"thievul", Species::Thievul},
			{"throh", Species::Throh},
			{"thundurus", Species::Thundurus_Incarnate},
			{"thundurusincarnate", Species::Thundurus_Incarnate},
			{"thundurustherian", Species::Thundurus_Therian},
			{"thwackey", Species::Thwackey},
			{"timburr", Species::Timburr},
			{"tirtouga", Species::Tirtouga},
			{"togedemaru", Species::Togedemaru},
			{"togedemarutotem", Species::Togedemaru},
			{"togekiss", Species::Togekiss},
			{"togepi", Species::Togepi},
			{"togetic", Species::Togetic},
			{"torchic", Species::Torchic},
			{"torkoal", Species::Torkoal},
			{"tornadus", Species::Tornadus_Incarnate},
			{"tornadusincarnate", Species::Tornadus_Incarnate},
			{"tornadustherian", Species::Tornadus_Therian},
			{"torracat", Species::Torracat},
			{"torterra", Species::Torterra},
			{"totodile", Species::Totodile},
			{"toucannon", Species::Toucannon},
			{"toxapex", Species::Toxapex},
			{"toxel", Species::Toxel},
			{"toxicroak", Species::Toxicroak},
			{"toxtricity", Species::Toxtricity_Amped},
			{"toxtricityamped", Species::Toxtricity_Amped},
			{"toxtricitylowkey", Species::Toxtricity_Low_Key},
			{"tranquill", Species::Tranquill},
			{"trapinch", Species::Trapinch},
			{"treecko", Species::Treecko},
			{"trevenant", Species::Trevenant},
			{"tropius", Species::Tropius},
			{"trubbish", Species::Trubbish},
			{"trumbeak", Species::Trumbeak},
			{"tsareena", Species::Tsareena},
			{"turtonator", Species::Turtonator},
			{"turtwig", Species::Turtwig},
			{"tympole", Species::Tympole},
			{"tynamo", Species::Tynamo},
			{"typenull", Species::Type_Null},
			{"typhlosion", Species::Typhlosion},
			{"tyranitar", Species::Tyranitar},
			{"tyranitarmega", Species::Tyranitar},
			{"tyrantrum", Species::Tyrantrum},
			{"tyrogue", Species::Tyrogue},
			{"tyrunt", Species::Tyrunt},
			{"umbreon", Species::Umbreon},
			{"unfezant", Species::Unfezant},
			{"unown", Species::Unown},
			{"unowna", Species::Unown},
			{"unownb", Species::Unown},
			{"unownc", Species::Unown},
			{"unownd", Species::Unown},
			{"unowne", Species::Unown},
			{"unownexclamation", Species::Unown},
			{"unownf", Species::Unown},
			{"unowng", Species::Unown},
			{"unownh", Species::Unown},
			{"unowni", Species::Unown},
			{"unownj", Species::Unown},
			{"unownk", Species::Unown},
			{"unownl", Species::Unown},
			{"unownm", Species::Unown},
			{"unownn", Species::Unown},
			{"unowno", Species::Unown},
			{"unownp", Species::Unown},
			{"unownq", Species::Unown},
			{"unownquestion", Species::Unown},
			{"unownr", Species::Unown},
			{"unowns", Species::Unown},
			{"unownt", Species::Unown},
			{"unownu", Species::Unown},
			{"unownv", Species::Unown},
			{"unownw", Species::Unown},
			{"unownx", Species::Unown},
			{"unowny", Species::Unown},
			{"unownz", Species::Unown},
			{"ursaring", Species::Ursaring},
			{"urshifurapidstrike", Species::Urshifu_Rapid_Strike},
			{"urshifusinglestrike", Species::Urshifu_Single_Strike},
			{"uxie", Species::Uxie},
			{"vanillish", Species::Vanillish},
			{"vanillite", Species::Vanillite},
			{"vanilluxe", Species::Vanilluxe},
			{"vaporeon", Species::Vaporeon},
			{"venipede", Species::Venipede},
			{"venomoth", Species::Venomoth},
			{"venonat", Species::Venonat},
			{"venusaur", Species::Venusaur},
			{"venusaurmega", Species::Venusaur},
			{"vespiquen", Species::Vespiquen},
			{"vibrava", Species::Vibrava},
			{"victini", Species::Victini},
			{"victreebel", Species::Victreebel},
			{"vigoroth", Species::Vigoroth},
			{"vikavolt", Species::Vikavolt},
			{"vikavolttotem", Species::Vikavolt},
			{"vileplume", Species::Vileplume},
			{"virizion", Species::Virizion},
			{"vivillon", Species::Vivillon},
			{"volbeat", Species::Volbeat},
			{"volcanion", Species::Volcanion},
			{"volcarona", Species::Volcarona},
			{"voltorb", Species::Voltorb},
			{"vullaby", Species::Vullaby},
			{"vulpix", Species::Vulpix},
			{"vulpixalola", Species::Vulpix_Alola},
			{"wailmer", Species::Wailmer},
			{"wailord", Species::Wailord},
			{"walrein", Species::Walrein},
			{"wartortle", Species::Wartortle},
			{"watchog", Species::Watchog},
			{"weavile", Species::Weavile},
			{"weedle", Species::Weedle},
			{"weepinbell", Species::Weepinbell},
			{"weezing", Species::Weezing},
			{"weezinggalar", Species::Weezing_Galar},
			{"whimsicott", Species::Whimsicott},
			{"whirlipede", Species::Whirlipede},
			{"whiscash", Species::Whiscash},
			{"whismur", Species::Whismur},
			{"wigglytuff", Species::Wigglytuff},
			{"wimpod", Species::Wimpod},
			{"wingull", Species::Wingull},
			{"wishiwashi", Species::Wishiwashi_Solo},
			{"wishiwashischool", Species::Wishiwashi_School},
			{"wishiwashisolo", Species::Wishiwashi_Solo},
			{"wobbuffet", Species::Wobbuffet},
			{"woobat", Species::Woobat},
			{"wooloo", Species::Wooloo},
			{"wooper", Species::Wooper},
			{"wormadam", Species::Wormadam_Plant},
			{"wormadamg", Species::Wormadam_Sandy},
			{"wormadamplant", Species::Wormadam_Plant},
			{"wormadams", Species::Wormadam_Trash},
			{"wormadamsandy", Species::Wormadam_Sandy},
			{"wormadamtrash", Species::Wormadam_Trash},
			{"wurmple", Species::Wurmple},
			{"wynaut", Species::Wynaut},
			{"xatu", Species::Xatu},
			{"xerneas", Species::Xerneas},
			{"xurkitree", Species::Xurkitree},
			{"yamask", Species::Yamask},
			{"yamaskgalar", Species::Yamask_Galar},
			{"yamper", Species::Yamper},
			{"yanma", Species::Yanma},
			{"yanmega", Species::Yanmega},
			{"yungoos", Species::Yungoos},
			{"yveltal", Species::Yveltal},
			{"zacian", Species::Zacian},
			{"zaciancrownedsword", Species::Zacian_Crowned_Sword},
			{"zamazenta", Species::Zamazenta},
			{"zamazentacrownedshield", Species::Zamazenta_Crowned_Shield},
			{"zangoose", Species::Zangoose},
			{"zapdos", Species::Zapdos},
			{"zapdosgalar", Species::Zapdos_Galar},
			{"zarude", Species::Zarude},
			{"zebstrika", Species::Zebstrika},
			{"zekrom", Species::Zekrom},
			{"zeraora", Species::Zeraora},
			{"zigzagoon", Species::Zigzagoon},
			{"zigzagoongalar", Species::Zigzagoon_Galar},
			{"zoroark", Species::Zoroark},
			{"zorua", Species::Zorua},
			{"zubat", Species::Zubat},
			{"zweilous", Species::Zweilous},
			{"zygarde", Species::Zygarde_50},
			{"zygarde10", Species::Zygarde_10},
			{"zygarde50", Species::Zygarde_50},
			{"zygardecomplete", Species::Zygarde_Complete},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<22_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Species", str);
	}
	return *result;
}

} // namespace technicalmachine
