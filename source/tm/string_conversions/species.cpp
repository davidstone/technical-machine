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
using namespace containers::string_literals;

export constexpr auto to_string(Species const species) -> containers::string_view {
	switch (species) {
		// Generation 1
		case Species::Bulbasaur: return "Bulbasaur"_s;
		case Species::Ivysaur: return "Ivysaur"_s;
		case Species::Venusaur: return "Venusaur"_s;
		case Species::Charmander: return "Charmander"_s;
		case Species::Charmeleon: return "Charmeleon"_s;
		case Species::Charizard: return "Charizard"_s;
		case Species::Squirtle: return "Squirtle"_s;
		case Species::Wartortle: return "Wartortle"_s;
		case Species::Blastoise: return "Blastoise"_s;
		case Species::Caterpie: return "Caterpie"_s;
		case Species::Metapod: return "Metapod"_s;
		case Species::Butterfree: return "Butterfree"_s;
		case Species::Weedle: return "Weedle"_s;
		case Species::Kakuna: return "Kakuna"_s;
		case Species::Beedrill: return "Beedrill"_s;
		case Species::Pidgey: return "Pidgey"_s;
		case Species::Pidgeotto: return "Pidgeotto"_s;
		case Species::Pidgeot: return "Pidgeot"_s;
		case Species::Rattata: return "Rattata"_s;
		case Species::Raticate: return "Raticate"_s;
		case Species::Spearow: return "Spearow"_s;
		case Species::Fearow: return "Fearow"_s;
		case Species::Ekans: return "Ekans"_s;
		case Species::Arbok: return "Arbok"_s;
		case Species::Pikachu: return "Pikachu"_s;
		case Species::Raichu: return "Raichu"_s;
		case Species::Sandshrew: return "Sandshrew"_s;
		case Species::Sandslash: return "Sandslash"_s;
		case Species::Nidoran_F: return "Nidoran-F"_s;
		case Species::Nidorina: return "Nidorina"_s;
		case Species::Nidoqueen: return "Nidoqueen"_s;
		case Species::Nidoran_M: return "Nidoran-M"_s;
		case Species::Nidorino: return "Nidorino"_s;
		case Species::Nidoking: return "Nidoking"_s;
		case Species::Clefairy: return "Clefairy"_s;
		case Species::Clefable: return "Clefable"_s;
		case Species::Vulpix: return "Vulpix"_s;
		case Species::Ninetales: return "Ninetales"_s;
		case Species::Jigglypuff: return "Jigglypuff"_s;
		case Species::Wigglytuff: return "Wigglytuff"_s;
		case Species::Zubat: return "Zubat"_s;
		case Species::Golbat: return "Golbat"_s;
		case Species::Oddish: return "Oddish"_s;
		case Species::Gloom: return "Gloom"_s;
		case Species::Vileplume: return "Vileplume"_s;
		case Species::Paras: return "Paras"_s;
		case Species::Parasect: return "Parasect"_s;
		case Species::Venonat: return "Venonat"_s;
		case Species::Venomoth: return "Venomoth"_s;
		case Species::Diglett: return "Diglett"_s;
		case Species::Dugtrio: return "Dugtrio"_s;
		case Species::Meowth: return "Meowth"_s;
		case Species::Persian: return "Persian"_s;
		case Species::Psyduck: return "Psyduck"_s;
		case Species::Golduck: return "Golduck"_s;
		case Species::Mankey: return "Mankey"_s;
		case Species::Primeape: return "Primeape"_s;
		case Species::Growlithe: return "Growlithe"_s;
		case Species::Arcanine: return "Arcanine"_s;
		case Species::Poliwag: return "Poliwag"_s;
		case Species::Poliwhirl: return "Poliwhirl"_s;
		case Species::Poliwrath: return "Poliwrath"_s;
		case Species::Abra: return "Abra"_s;
		case Species::Kadabra: return "Kadabra"_s;
		case Species::Alakazam: return "Alakazam"_s;
		case Species::Machop: return "Machop"_s;
		case Species::Machoke: return "Machoke"_s;
		case Species::Machamp: return "Machamp"_s;
		case Species::Bellsprout: return "Bellsprout"_s;
		case Species::Weepinbell: return "Weepinbell"_s;
		case Species::Victreebel: return "Victreebel"_s;
		case Species::Tentacool: return "Tentacool"_s;
		case Species::Tentacruel: return "Tentacruel"_s;
		case Species::Geodude: return "Geodude"_s;
		case Species::Graveler: return "Graveler"_s;
		case Species::Golem: return "Golem"_s;
		case Species::Ponyta: return "Ponyta"_s;
		case Species::Rapidash: return "Rapidash"_s;
		case Species::Slowpoke: return "Slowpoke"_s;
		case Species::Slowbro: return "Slowbro"_s;
		case Species::Magnemite: return "Magnemite"_s;
		case Species::Magneton: return "Magneton"_s;
		case Species::Farfetchd: return "Farfetch'd"_s;
		case Species::Doduo: return "Doduo"_s;
		case Species::Dodrio: return "Dodrio"_s;
		case Species::Seel: return "Seel"_s;
		case Species::Dewgong: return "Dewgong"_s;
		case Species::Grimer: return "Grimer"_s;
		case Species::Muk: return "Muk"_s;
		case Species::Shellder: return "Shellder"_s;
		case Species::Cloyster: return "Cloyster"_s;
		case Species::Gastly: return "Gastly"_s;
		case Species::Haunter: return "Haunter"_s;
		case Species::Gengar: return "Gengar"_s;
		case Species::Onix: return "Onix"_s;
		case Species::Drowzee: return "Drowzee"_s;
		case Species::Hypno: return "Hypno"_s;
		case Species::Krabby: return "Krabby"_s;
		case Species::Kingler: return "Kingler"_s;
		case Species::Voltorb: return "Voltorb"_s;
		case Species::Electrode: return "Electrode"_s;
		case Species::Exeggcute: return "Exeggcute"_s;
		case Species::Exeggutor: return "Exeggutor"_s;
		case Species::Cubone: return "Cubone"_s;
		case Species::Marowak: return "Marowak"_s;
		case Species::Hitmonlee: return "Hitmonlee"_s;
		case Species::Hitmonchan: return "Hitmonchan"_s;
		case Species::Lickitung: return "Lickitung"_s;
		case Species::Koffing: return "Koffing"_s;
		case Species::Weezing: return "Weezing"_s;
		case Species::Rhyhorn: return "Rhyhorn"_s;
		case Species::Rhydon: return "Rhydon"_s;
		case Species::Chansey: return "Chansey"_s;
		case Species::Tangela: return "Tangela"_s;
		case Species::Kangaskhan: return "Kangaskhan"_s;
		case Species::Horsea: return "Horsea"_s;
		case Species::Seadra: return "Seadra"_s;
		case Species::Goldeen: return "Goldeen"_s;
		case Species::Seaking: return "Seaking"_s;
		case Species::Staryu: return "Staryu"_s;
		case Species::Starmie: return "Starmie"_s;
		case Species::Mr_Mime: return "Mr. Mime"_s;
		case Species::Scyther: return "Scyther"_s;
		case Species::Jynx: return "Jynx"_s;
		case Species::Electabuzz: return "Electabuzz"_s;
		case Species::Magmar: return "Magmar"_s;
		case Species::Pinsir: return "Pinsir"_s;
		case Species::Tauros: return "Tauros"_s;
		case Species::Magikarp: return "Magikarp"_s;
		case Species::Gyarados: return "Gyarados"_s;
		case Species::Lapras: return "Lapras"_s;
		case Species::Ditto: return "Ditto"_s;
		case Species::Eevee: return "Eevee"_s;
		case Species::Vaporeon: return "Vaporeon"_s;
		case Species::Jolteon: return "Jolteon"_s;
		case Species::Flareon: return "Flareon"_s;
		case Species::Porygon: return "Porygon"_s;
		case Species::Omanyte: return "Omanyte"_s;
		case Species::Omastar: return "Omastar"_s;
		case Species::Kabuto: return "Kabuto"_s;
		case Species::Kabutops: return "Kabutops"_s;
		case Species::Aerodactyl: return "Aerodactyl"_s;
		case Species::Snorlax: return "Snorlax"_s;
		case Species::Articuno: return "Articuno"_s;
		case Species::Zapdos: return "Zapdos"_s;
		case Species::Moltres: return "Moltres"_s;
		case Species::Dratini: return "Dratini"_s;
		case Species::Dragonair: return "Dragonair"_s;
		case Species::Dragonite: return "Dragonite"_s;
		case Species::Mewtwo: return "Mewtwo"_s;
		case Species::Mew: return "Mew"_s;

		// Generation 2
		case Species::Chikorita: return "Chikorita"_s;
		case Species::Bayleef: return "Bayleef"_s;
		case Species::Meganium: return "Meganium"_s;
		case Species::Cyndaquil: return "Cyndaquil"_s;
		case Species::Quilava: return "Quilava"_s;
		case Species::Typhlosion: return "Typhlosion"_s;
		case Species::Totodile: return "Totodile"_s;
		case Species::Croconaw: return "Croconaw"_s;
		case Species::Feraligatr: return "Feraligatr"_s;
		case Species::Sentret: return "Sentret"_s;
		case Species::Furret: return "Furret"_s;
		case Species::Hoothoot: return "Hoothoot"_s;
		case Species::Noctowl: return "Noctowl"_s;
		case Species::Ledyba: return "Ledyba"_s;
		case Species::Ledian: return "Ledian"_s;
		case Species::Spinarak: return "Spinarak"_s;
		case Species::Ariados: return "Ariados"_s;
		case Species::Crobat: return "Crobat"_s;
		case Species::Chinchou: return "Chinchou"_s;
		case Species::Lanturn: return "Lanturn"_s;
		case Species::Pichu: return "Pichu"_s;
		case Species::Cleffa: return "Cleffa"_s;
		case Species::Igglybuff: return "Igglybuff"_s;
		case Species::Togepi: return "Togepi"_s;
		case Species::Togetic: return "Togetic"_s;
		case Species::Natu: return "Natu"_s;
		case Species::Xatu: return "Xatu"_s;
		case Species::Mareep: return "Mareep"_s;
		case Species::Flaaffy: return "Flaaffy"_s;
		case Species::Ampharos: return "Ampharos"_s;
		case Species::Bellossom: return "Bellossom"_s;
		case Species::Marill: return "Marill"_s;
		case Species::Azumarill: return "Azumarill"_s;
		case Species::Sudowoodo: return "Sudowoodo"_s;
		case Species::Politoed: return "Politoed"_s;
		case Species::Hoppip: return "Hoppip"_s;
		case Species::Skiploom: return "Skiploom"_s;
		case Species::Jumpluff: return "Jumpluff"_s;
		case Species::Aipom: return "Aipom"_s;
		case Species::Sunkern: return "Sunkern"_s;
		case Species::Sunflora: return "Sunflora"_s;
		case Species::Yanma: return "Yanma"_s;
		case Species::Wooper: return "Wooper"_s;
		case Species::Quagsire: return "Quagsire"_s;
		case Species::Espeon: return "Espeon"_s;
		case Species::Umbreon: return "Umbreon"_s;
		case Species::Murkrow: return "Murkrow"_s;
		case Species::Slowking: return "Slowking"_s;
		case Species::Misdreavus: return "Misdreavus"_s;
		case Species::Unown: return "Unown"_s;
		case Species::Wobbuffet: return "Wobbuffet"_s;
		case Species::Girafarig: return "Girafarig"_s;
		case Species::Pineco: return "Pineco"_s;
		case Species::Forretress: return "Forretress"_s;
		case Species::Dunsparce: return "Dunsparce"_s;
		case Species::Gligar: return "Gligar"_s;
		case Species::Steelix: return "Steelix"_s;
		case Species::Snubbull: return "Snubbull"_s;
		case Species::Granbull: return "Granbull"_s;
		case Species::Qwilfish: return "Qwilfish"_s;
		case Species::Scizor: return "Scizor"_s;
		case Species::Shuckle: return "Shuckle"_s;
		case Species::Heracross: return "Heracross"_s;
		case Species::Sneasel: return "Sneasel"_s;
		case Species::Teddiursa: return "Teddiursa"_s;
		case Species::Ursaring: return "Ursaring"_s;
		case Species::Slugma: return "Slugma"_s;
		case Species::Magcargo: return "Magcargo"_s;
		case Species::Swinub: return "Swinub"_s;
		case Species::Piloswine: return "Piloswine"_s;
		case Species::Corsola: return "Corsola"_s;
		case Species::Remoraid: return "Remoraid"_s;
		case Species::Octillery: return "Octillery"_s;
		case Species::Delibird: return "Delibird"_s;
		case Species::Mantine: return "Mantine"_s;
		case Species::Skarmory: return "Skarmory"_s;
		case Species::Houndour: return "Houndour"_s;
		case Species::Houndoom: return "Houndoom"_s;
		case Species::Kingdra: return "Kingdra"_s;
		case Species::Phanpy: return "Phanpy"_s;
		case Species::Donphan: return "Donphan"_s;
		case Species::Porygon2: return "Porygon2"_s;
		case Species::Stantler: return "Stantler"_s;
		case Species::Smeargle: return "Smeargle"_s;
		case Species::Tyrogue: return "Tyrogue"_s;
		case Species::Hitmontop: return "Hitmontop"_s;
		case Species::Smoochum: return "Smoochum"_s;
		case Species::Elekid: return "Elekid"_s;
		case Species::Magby: return "Magby"_s;
		case Species::Miltank: return "Miltank"_s;
		case Species::Blissey: return "Blissey"_s;
		case Species::Raikou: return "Raikou"_s;
		case Species::Entei: return "Entei"_s;
		case Species::Suicune: return "Suicune"_s;
		case Species::Larvitar: return "Larvitar"_s;
		case Species::Pupitar: return "Pupitar"_s;
		case Species::Tyranitar: return "Tyranitar"_s;
		case Species::Lugia: return "Lugia"_s;
		case Species::Ho_Oh: return "Ho-Oh"_s;
		case Species::Celebi: return "Celebi"_s;

		// Generation 3
		case Species::Treecko: return "Treecko"_s;
		case Species::Grovyle: return "Grovyle"_s;
		case Species::Sceptile: return "Sceptile"_s;
		case Species::Torchic: return "Torchic"_s;
		case Species::Combusken: return "Combusken"_s;
		case Species::Blaziken: return "Blaziken"_s;
		case Species::Mudkip: return "Mudkip"_s;
		case Species::Marshtomp: return "Marshtomp"_s;
		case Species::Swampert: return "Swampert"_s;
		case Species::Poochyena: return "Poochyena"_s;
		case Species::Mightyena: return "Mightyena"_s;
		case Species::Zigzagoon: return "Zigzagoon"_s;
		case Species::Linoone: return "Linoone"_s;
		case Species::Wurmple: return "Wurmple"_s;
		case Species::Silcoon: return "Silcoon"_s;
		case Species::Beautifly: return "Beautifly"_s;
		case Species::Cascoon: return "Cascoon"_s;
		case Species::Dustox: return "Dustox"_s;
		case Species::Lotad: return "Lotad"_s;
		case Species::Lombre: return "Lombre"_s;
		case Species::Ludicolo: return "Ludicolo"_s;
		case Species::Seedot: return "Seedot"_s;
		case Species::Nuzleaf: return "Nuzleaf"_s;
		case Species::Shiftry: return "Shiftry"_s;
		case Species::Taillow: return "Taillow"_s;
		case Species::Swellow: return "Swellow"_s;
		case Species::Wingull: return "Wingull"_s;
		case Species::Pelipper: return "Pelipper"_s;
		case Species::Ralts: return "Ralts"_s;
		case Species::Kirlia: return "Kirlia"_s;
		case Species::Gardevoir: return "Gardevoir"_s;
		case Species::Surskit: return "Surskit"_s;
		case Species::Masquerain: return "Masquerain"_s;
		case Species::Shroomish: return "Shroomish"_s;
		case Species::Breloom: return "Breloom"_s;
		case Species::Slakoth: return "Slakoth"_s;
		case Species::Vigoroth: return "Vigoroth"_s;
		case Species::Slaking: return "Slaking"_s;
		case Species::Nincada: return "Nincada"_s;
		case Species::Ninjask: return "Ninjask"_s;
		case Species::Shedinja: return "Shedinja"_s;
		case Species::Whismur: return "Whismur"_s;
		case Species::Loudred: return "Loudred"_s;
		case Species::Exploud: return "Exploud"_s;
		case Species::Makuhita: return "Makuhita"_s;
		case Species::Hariyama: return "Hariyama"_s;
		case Species::Azurill: return "Azurill"_s;
		case Species::Nosepass: return "Nosepass"_s;
		case Species::Skitty: return "Skitty"_s;
		case Species::Delcatty: return "Delcatty"_s;
		case Species::Sableye: return "Sableye"_s;
		case Species::Mawile: return "Mawile"_s;
		case Species::Aron: return "Aron"_s;
		case Species::Lairon: return "Lairon"_s;
		case Species::Aggron: return "Aggron"_s;
		case Species::Meditite: return "Meditite"_s;
		case Species::Medicham: return "Medicham"_s;
		case Species::Electrike: return "Electrike"_s;
		case Species::Manectric: return "Manectric"_s;
		case Species::Plusle: return "Plusle"_s;
		case Species::Minun: return "Minun"_s;
		case Species::Volbeat: return "Volbeat"_s;
		case Species::Illumise: return "Illumise"_s;
		case Species::Roselia: return "Roselia"_s;
		case Species::Gulpin: return "Gulpin"_s;
		case Species::Swalot: return "Swalot"_s;
		case Species::Carvanha: return "Carvanha"_s;
		case Species::Sharpedo: return "Sharpedo"_s;
		case Species::Wailmer: return "Wailmer"_s;
		case Species::Wailord: return "Wailord"_s;
		case Species::Numel: return "Numel"_s;
		case Species::Camerupt: return "Camerupt"_s;
		case Species::Torkoal: return "Torkoal"_s;
		case Species::Spoink: return "Spoink"_s;
		case Species::Grumpig: return "Grumpig"_s;
		case Species::Spinda: return "Spinda"_s;
		case Species::Trapinch: return "Trapinch"_s;
		case Species::Vibrava: return "Vibrava"_s;
		case Species::Flygon: return "Flygon"_s;
		case Species::Cacnea: return "Cacnea"_s;
		case Species::Cacturne: return "Cacturne"_s;
		case Species::Swablu: return "Swablu"_s;
		case Species::Altaria: return "Altaria"_s;
		case Species::Zangoose: return "Zangoose"_s;
		case Species::Seviper: return "Seviper"_s;
		case Species::Lunatone: return "Lunatone"_s;
		case Species::Solrock: return "Solrock"_s;
		case Species::Barboach: return "Barboach"_s;
		case Species::Whiscash: return "Whiscash"_s;
		case Species::Corphish: return "Corphish"_s;
		case Species::Crawdaunt: return "Crawdaunt"_s;
		case Species::Baltoy: return "Baltoy"_s;
		case Species::Claydol: return "Claydol"_s;
		case Species::Lileep: return "Lileep"_s;
		case Species::Cradily: return "Cradily"_s;
		case Species::Anorith: return "Anorith"_s;
		case Species::Armaldo: return "Armaldo"_s;
		case Species::Feebas: return "Feebas"_s;
		case Species::Milotic: return "Milotic"_s;
		case Species::Castform: return "Castform"_s;
		case Species::Kecleon: return "Kecleon"_s;
		case Species::Shuppet: return "Shuppet"_s;
		case Species::Banette: return "Banette"_s;
		case Species::Duskull: return "Duskull"_s;
		case Species::Dusclops: return "Dusclops"_s;
		case Species::Tropius: return "Tropius"_s;
		case Species::Chimecho: return "Chimecho"_s;
		case Species::Absol: return "Absol"_s;
		case Species::Wynaut: return "Wynaut"_s;
		case Species::Snorunt: return "Snorunt"_s;
		case Species::Glalie: return "Glalie"_s;
		case Species::Spheal: return "Spheal"_s;
		case Species::Sealeo: return "Sealeo"_s;
		case Species::Walrein: return "Walrein"_s;
		case Species::Clamperl: return "Clamperl"_s;
		case Species::Huntail: return "Huntail"_s;
		case Species::Gorebyss: return "Gorebyss"_s;
		case Species::Relicanth: return "Relicanth"_s;
		case Species::Luvdisc: return "Luvdisc"_s;
		case Species::Bagon: return "Bagon"_s;
		case Species::Shelgon: return "Shelgon"_s;
		case Species::Salamence: return "Salamence"_s;
		case Species::Beldum: return "Beldum"_s;
		case Species::Metang: return "Metang"_s;
		case Species::Metagross: return "Metagross"_s;
		case Species::Regirock: return "Regirock"_s;
		case Species::Regice: return "Regice"_s;
		case Species::Registeel: return "Registeel"_s;
		case Species::Latias: return "Latias"_s;
		case Species::Latios: return "Latios"_s;
		case Species::Kyogre: return "Kyogre"_s;
		case Species::Groudon: return "Groudon"_s;
		case Species::Rayquaza: return "Rayquaza"_s;
		case Species::Jirachi: return "Jirachi"_s;
		case Species::Deoxys_Normal: return "Deoxys-Normal"_s;
		case Species::Deoxys_Attack: return "Deoxys-Attack"_s;
		case Species::Deoxys_Defense: return "Deoxys-Defense"_s;
		case Species::Deoxys_Speed: return "Deoxys-Speed"_s;

		// Generation 4
		case Species::Turtwig: return "Turtwig"_s;
		case Species::Grotle: return "Grotle"_s;
		case Species::Torterra: return "Torterra"_s;
		case Species::Chimchar: return "Chimchar"_s;
		case Species::Monferno: return "Monferno"_s;
		case Species::Infernape: return "Infernape"_s;
		case Species::Piplup: return "Piplup"_s;
		case Species::Prinplup: return "Prinplup"_s;
		case Species::Empoleon: return "Empoleon"_s;
		case Species::Starly: return "Starly"_s;
		case Species::Staravia: return "Staravia"_s;
		case Species::Staraptor: return "Staraptor"_s;
		case Species::Bidoof: return "Bidoof"_s;
		case Species::Bibarel: return "Bibarel"_s;
		case Species::Kricketot: return "Kricketot"_s;
		case Species::Kricketune: return "Kricketune"_s;
		case Species::Shinx: return "Shinx"_s;
		case Species::Luxio: return "Luxio"_s;
		case Species::Luxray: return "Luxray"_s;
		case Species::Budew: return "Budew"_s;
		case Species::Roserade: return "Roserade"_s;
		case Species::Cranidos: return "Cranidos"_s;
		case Species::Rampardos: return "Rampardos"_s;
		case Species::Shieldon: return "Shieldon"_s;
		case Species::Bastiodon: return "Bastiodon"_s;
		case Species::Burmy: return "Burmy"_s;
		case Species::Wormadam_Plant: return "Wormadam-Plant"_s;
		case Species::Wormadam_Sandy: return "Wormadam-Sandy"_s;
		case Species::Wormadam_Trash: return "Wormadam-Trash"_s;
		case Species::Mothim: return "Mothim"_s;
		case Species::Combee: return "Combee"_s;
		case Species::Vespiquen: return "Vespiquen"_s;
		case Species::Pachirisu: return "Pachirisu"_s;
		case Species::Buizel: return "Buizel"_s;
		case Species::Floatzel: return "Floatzel"_s;
		case Species::Cherubi: return "Cherubi"_s;
		case Species::Cherrim: return "Cherrim"_s;
		case Species::Shellos: return "Shellos"_s;
		case Species::Gastrodon: return "Gastrodon"_s;
		case Species::Ambipom: return "Ambipom"_s;
		case Species::Drifloon: return "Drifloon"_s;
		case Species::Drifblim: return "Drifblim"_s;
		case Species::Buneary: return "Buneary"_s;
		case Species::Lopunny: return "Lopunny"_s;
		case Species::Mismagius: return "Mismagius"_s;
		case Species::Honchkrow: return "Honchkrow"_s;
		case Species::Glameow: return "Glameow"_s;
		case Species::Purugly: return "Purugly"_s;
		case Species::Chingling: return "Chingling"_s;
		case Species::Stunky: return "Stunky"_s;
		case Species::Skuntank: return "Skuntank"_s;
		case Species::Bronzor: return "Bronzor"_s;
		case Species::Bronzong: return "Bronzong"_s;
		case Species::Bonsly: return "Bonsly"_s;
		case Species::Mime_Jr: return "Mime Jr."_s;
		case Species::Happiny: return "Happiny"_s;
		case Species::Chatot: return "Chatot"_s;
		case Species::Spiritomb: return "Spiritomb"_s;
		case Species::Gible: return "Gible"_s;
		case Species::Gabite: return "Gabite"_s;
		case Species::Garchomp: return "Garchomp"_s;
		case Species::Munchlax: return "Munchlax"_s;
		case Species::Riolu: return "Riolu"_s;
		case Species::Lucario: return "Lucario"_s;
		case Species::Hippopotas: return "Hippopotas"_s;
		case Species::Hippowdon: return "Hippowdon"_s;
		case Species::Skorupi: return "Skorupi"_s;
		case Species::Drapion: return "Drapion"_s;
		case Species::Croagunk: return "Croagunk"_s;
		case Species::Toxicroak: return "Toxicroak"_s;
		case Species::Carnivine: return "Carnivine"_s;
		case Species::Finneon: return "Finneon"_s;
		case Species::Lumineon: return "Lumineon"_s;
		case Species::Mantyke: return "Mantyke"_s;
		case Species::Snover: return "Snover"_s;
		case Species::Abomasnow: return "Abomasnow"_s;
		case Species::Weavile: return "Weavile"_s;
		case Species::Magnezone: return "Magnezone"_s;
		case Species::Lickilicky: return "Lickilicky"_s;
		case Species::Rhyperior: return "Rhyperior"_s;
		case Species::Tangrowth: return "Tangrowth"_s;
		case Species::Electivire: return "Electivire"_s;
		case Species::Magmortar: return "Magmortar"_s;
		case Species::Togekiss: return "Togekiss"_s;
		case Species::Yanmega: return "Yanmega"_s;
		case Species::Leafeon: return "Leafeon"_s;
		case Species::Glaceon: return "Glaceon"_s;
		case Species::Gliscor: return "Gliscor"_s;
		case Species::Mamoswine: return "Mamoswine"_s;
		case Species::Porygon_Z: return "Porygon-Z"_s;
		case Species::Gallade: return "Gallade"_s;
		case Species::Probopass: return "Probopass"_s;
		case Species::Dusknoir: return "Dusknoir"_s;
		case Species::Froslass: return "Froslass"_s;
		case Species::Rotom: return "Rotom"_s;
		case Species::Rotom_Heat: return "Rotom-Heat"_s;
		case Species::Rotom_Wash: return "Rotom-Wash"_s;
		case Species::Rotom_Frost: return "Rotom-Frost"_s;
		case Species::Rotom_Fan: return "Rotom-Fan"_s;
		case Species::Rotom_Mow: return "Rotom-Mow"_s;
		case Species::Uxie: return "Uxie"_s;
		case Species::Mesprit: return "Mesprit"_s;
		case Species::Azelf: return "Azelf"_s;
		case Species::Dialga: return "Dialga"_s;
		case Species::Palkia: return "Palkia"_s;
		case Species::Heatran: return "Heatran"_s;
		case Species::Regigigas: return "Regigigas"_s;
		case Species::Giratina_Altered: return "Giratina-Altered"_s;
		case Species::Giratina_Origin: return "Giratina-Origin"_s;
		case Species::Cresselia: return "Cresselia"_s;
		case Species::Phione: return "Phione"_s;
		case Species::Manaphy: return "Manaphy"_s;
		case Species::Darkrai: return "Darkrai"_s;
		case Species::Shaymin_Land: return "Shaymin-Land"_s;
		case Species::Shaymin_Sky: return "Shaymin-Sky"_s;
		case Species::Arceus: return "Arceus"_s;

		// Generation 5
		case Species::Victini: return "Victini"_s;
		case Species::Snivy: return "Snivy"_s;
		case Species::Servine: return "Servine"_s;
		case Species::Serperior: return "Serperior"_s;
		case Species::Tepig: return "Tepig"_s;
		case Species::Pignite: return "Pignite"_s;
		case Species::Emboar: return "Emboar"_s;
		case Species::Oshawott: return "Oshawott"_s;
		case Species::Dewott: return "Dewott"_s;
		case Species::Samurott: return "Samurott"_s;
		case Species::Patrat: return "Patrat"_s;
		case Species::Watchog: return "Watchog"_s;
		case Species::Lillipup: return "Lillipup"_s;
		case Species::Herdier: return "Herdier"_s;
		case Species::Stoutland: return "Stoutland"_s;
		case Species::Purrloin: return "Purrloin"_s;
		case Species::Liepard: return "Liepard"_s;
		case Species::Pansage: return "Pansage"_s;
		case Species::Simisage: return "Simisage"_s;
		case Species::Pansear: return "Pansear"_s;
		case Species::Simisear: return "Simisear"_s;
		case Species::Panpour: return "Panpour"_s;
		case Species::Simipour: return "Simipour"_s;
		case Species::Munna: return "Munna"_s;
		case Species::Musharna: return "Musharna"_s;
		case Species::Pidove: return "Pidove"_s;
		case Species::Tranquill: return "Tranquill"_s;
		case Species::Unfezant: return "Unfezant"_s;
		case Species::Blitzle: return "Blitzle"_s;
		case Species::Zebstrika: return "Zebstrika"_s;
		case Species::Roggenrola: return "Roggenrola"_s;
		case Species::Boldore: return "Boldore"_s;
		case Species::Gigalith: return "Gigalith"_s;
		case Species::Woobat: return "Woobat"_s;
		case Species::Swoobat: return "Swoobat"_s;
		case Species::Drilbur: return "Drilbur"_s;
		case Species::Excadrill: return "Excadrill"_s;
		case Species::Audino: return "Audino"_s;
		case Species::Timburr: return "Timburr"_s;
		case Species::Gurdurr: return "Gurdurr"_s;
		case Species::Conkeldurr: return "Conkeldurr"_s;
		case Species::Tympole: return "Tympole"_s;
		case Species::Palpitoad: return "Palpitoad"_s;
		case Species::Seismitoad: return "Seismitoad"_s;
		case Species::Throh: return "Throh"_s;
		case Species::Sawk: return "Sawk"_s;
		case Species::Sewaddle: return "Sewaddle"_s;
		case Species::Swadloon: return "Swadloon"_s;
		case Species::Leavanny: return "Leavanny"_s;
		case Species::Venipede: return "Venipede"_s;
		case Species::Whirlipede: return "Whirlipede"_s;
		case Species::Scolipede: return "Scolipede"_s;
		case Species::Cottonee: return "Cottonee"_s;
		case Species::Whimsicott: return "Whimsicott"_s;
		case Species::Petilil: return "Petilil"_s;
		case Species::Lilligant: return "Lilligant"_s;
		case Species::Basculin: return "Basculin"_s;
		case Species::Sandile: return "Sandile"_s;
		case Species::Krokorok: return "Krokorok"_s;
		case Species::Krookodile: return "Krookodile"_s;
		case Species::Darumaka: return "Darumaka"_s;
		case Species::Darmanitan: return "Darmanitan"_s;
		case Species::Maractus: return "Maractus"_s;
		case Species::Dwebble: return "Dwebble"_s;
		case Species::Crustle: return "Crustle"_s;
		case Species::Scraggy: return "Scraggy"_s;
		case Species::Scrafty: return "Scrafty"_s;
		case Species::Sigilyph: return "Sigilyph"_s;
		case Species::Yamask: return "Yamask"_s;
		case Species::Cofagrigus: return "Cofagrigus"_s;
		case Species::Tirtouga: return "Tirtouga"_s;
		case Species::Carracosta: return "Carracosta"_s;
		case Species::Archen: return "Archen"_s;
		case Species::Archeops: return "Archeops"_s;
		case Species::Trubbish: return "Trubbish"_s;
		case Species::Garbodor: return "Garbodor"_s;
		case Species::Zorua: return "Zorua"_s;
		case Species::Zoroark: return "Zoroark"_s;
		case Species::Minccino: return "Minccino"_s;
		case Species::Cinccino: return "Cinccino"_s;
		case Species::Gothita: return "Gothita"_s;
		case Species::Gothorita: return "Gothorita"_s;
		case Species::Gothitelle: return "Gothitelle"_s;
		case Species::Solosis: return "Solosis"_s;
		case Species::Duosion: return "Duosion"_s;
		case Species::Reuniclus: return "Reuniclus"_s;
		case Species::Ducklett: return "Ducklett"_s;
		case Species::Swanna: return "Swanna"_s;
		case Species::Vanillite: return "Vanillite"_s;
		case Species::Vanillish: return "Vanillish"_s;
		case Species::Vanilluxe: return "Vanilluxe"_s;
		case Species::Deerling: return "Deerling"_s;
		case Species::Sawsbuck: return "Sawsbuck"_s;
		case Species::Emolga: return "Emolga"_s;
		case Species::Karrablast: return "Karrablast"_s;
		case Species::Escavalier: return "Escavalier"_s;
		case Species::Foongus: return "Foongus"_s;
		case Species::Amoonguss: return "Amoonguss"_s;
		case Species::Frillish: return "Frillish"_s;
		case Species::Jellicent: return "Jellicent"_s;
		case Species::Alomomola: return "Alomomola"_s;
		case Species::Joltik: return "Joltik"_s;
		case Species::Galvantula: return "Galvantula"_s;
		case Species::Ferroseed: return "Ferroseed"_s;
		case Species::Ferrothorn: return "Ferrothorn"_s;
		case Species::Klink: return "Klink"_s;
		case Species::Klang: return "Klang"_s;
		case Species::Klinklang: return "Klinklang"_s;
		case Species::Tynamo: return "Tynamo"_s;
		case Species::Eelektrik: return "Eelektrik"_s;
		case Species::Eelektross: return "Eelektross"_s;
		case Species::Elgyem: return "Elgyem"_s;
		case Species::Beheeyem: return "Beheeyem"_s;
		case Species::Litwick: return "Litwick"_s;
		case Species::Lampent: return "Lampent"_s;
		case Species::Chandelure: return "Chandelure"_s;
		case Species::Axew: return "Axew"_s;
		case Species::Fraxure: return "Fraxure"_s;
		case Species::Haxorus: return "Haxorus"_s;
		case Species::Cubchoo: return "Cubchoo"_s;
		case Species::Beartic: return "Beartic"_s;
		case Species::Cryogonal: return "Cryogonal"_s;
		case Species::Shelmet: return "Shelmet"_s;
		case Species::Accelgor: return "Accelgor"_s;
		case Species::Stunfisk: return "Stunfisk"_s;
		case Species::Mienfoo: return "Mienfoo"_s;
		case Species::Mienshao: return "Mienshao"_s;
		case Species::Druddigon: return "Druddigon"_s;
		case Species::Golett: return "Golett"_s;
		case Species::Golurk: return "Golurk"_s;
		case Species::Pawniard: return "Pawniard"_s;
		case Species::Bisharp: return "Bisharp"_s;
		case Species::Bouffalant: return "Bouffalant"_s;
		case Species::Rufflet: return "Rufflet"_s;
		case Species::Braviary: return "Braviary"_s;
		case Species::Vullaby: return "Vullaby"_s;
		case Species::Mandibuzz: return "Mandibuzz"_s;
		case Species::Heatmor: return "Heatmor"_s;
		case Species::Durant: return "Durant"_s;
		case Species::Deino: return "Deino"_s;
		case Species::Zweilous: return "Zweilous"_s;
		case Species::Hydreigon: return "Hydreigon"_s;
		case Species::Larvesta: return "Larvesta"_s;
		case Species::Volcarona: return "Volcarona"_s;
		case Species::Cobalion: return "Cobalion"_s;
		case Species::Terrakion: return "Terrakion"_s;
		case Species::Virizion: return "Virizion"_s;
		case Species::Tornadus_Incarnate: return "Tornadus-Incarnate"_s;
		case Species::Tornadus_Therian: return "Tornadus-Therian"_s;
		case Species::Thundurus_Incarnate: return "Thundurus-Incarnate"_s;
		case Species::Thundurus_Therian: return "Thundurus-Therian"_s;
		case Species::Reshiram: return "Reshiram"_s;
		case Species::Zekrom: return "Zekrom"_s;
		case Species::Landorus_Incarnate: return "Landorus-Incarnate"_s;
		case Species::Landorus_Therian: return "Landorus-Therian"_s;
		case Species::Kyurem: return "Kyurem"_s;
		case Species::Kyurem_Black: return "Kyurem-Black"_s;
		case Species::Kyurem_White: return "Kyurem-White"_s;
		case Species::Keldeo: return "Keldeo"_s;
		case Species::Keldeo_Resolute: return "Keldeo-Resolute"_s;
		case Species::Meloetta: return "Meloetta"_s;
		case Species::Genesect: return "Genesect"_s;

		// Generation 6
		case Species::Chespin: return "Chespin"_s;
		case Species::Quilladin: return "Quilladin"_s;
		case Species::Chesnaught: return "Chesnaught"_s;
		case Species::Fennekin: return "Fennekin"_s;
		case Species::Braixen: return "Braixen"_s;
		case Species::Delphox: return "Delphox"_s;
		case Species::Froakie: return "Froakie"_s;
		case Species::Frogadier: return "Frogadier"_s;
		case Species::Greninja: return "Greninja"_s;
		case Species::Bunnelby: return "Bunnelby"_s;
		case Species::Diggersby: return "Diggersby"_s;
		case Species::Fletchling: return "Fletchling"_s;
		case Species::Fletchinder: return "Fletchinder"_s;
		case Species::Talonflame: return "Talonflame"_s;
		case Species::Scatterbug: return "Scatterbug"_s;
		case Species::Spewpa: return "Spewpa"_s;
		case Species::Vivillon: return "Vivillon"_s;
		case Species::Litleo: return "Litleo"_s;
		case Species::Pyroar: return "Pyroar"_s;
		case Species::Flabebe: return "Flabebe"_s;
		case Species::Floette: return "Floette"_s;
		case Species::Florges: return "Florges"_s;
		case Species::Skiddo: return "Skiddo"_s;
		case Species::Gogoat: return "Gogoat"_s;
		case Species::Pancham: return "Pancham"_s;
		case Species::Pangoro: return "Pangoro"_s;
		case Species::Furfrou: return "Furfrou"_s;
		case Species::Espurr: return "Espurr"_s;
		case Species::Meowstic_F: return "Meowstic-F"_s;
		case Species::Meowstic_M: return "Meowstic-M"_s;
		case Species::Honedge: return "Honedge"_s;
		case Species::Doublade: return "Doublade"_s;
		case Species::Aegislash: return "Aegislash"_s;
		case Species::Spritzee: return "Spritzee"_s;
		case Species::Aromatisse: return "Aromatisse"_s;
		case Species::Swirlix: return "Swirlix"_s;
		case Species::Slurpuff: return "Slurpuff"_s;
		case Species::Inkay: return "Inkay"_s;
		case Species::Malamar: return "Malamar"_s;
		case Species::Binacle: return "Binacle"_s;
		case Species::Barbaracle: return "Barbaracle"_s;
		case Species::Skrelp: return "Skrelp"_s;
		case Species::Dragalge: return "Dragalge"_s;
		case Species::Clauncher: return "Clauncher"_s;
		case Species::Clawitzer: return "Clawitzer"_s;
		case Species::Helioptile: return "Helioptile"_s;
		case Species::Heliolisk: return "Heliolisk"_s;
		case Species::Tyrunt: return "Tyrunt"_s;
		case Species::Tyrantrum: return "Tyrantrum"_s;
		case Species::Amaura: return "Amaura"_s;
		case Species::Aurorus: return "Aurorus"_s;
		case Species::Sylveon: return "Sylveon"_s;
		case Species::Hawlucha: return "Hawlucha"_s;
		case Species::Dedenne: return "Dedenne"_s;
		case Species::Carbink: return "Carbink"_s;
		case Species::Goomy: return "Goomy"_s;
		case Species::Sliggoo: return "Sliggoo"_s;
		case Species::Goodra: return "Goodra"_s;
		case Species::Klefki: return "Klefki"_s;
		case Species::Phantump: return "Phantump"_s;
		case Species::Trevenant: return "Trevenant"_s;
		case Species::Pumpkaboo_Small: return "Pumpkaboo-Small"_s;
		case Species::Pumpkaboo_Average: return "Pumpkaboo-Average"_s;
		case Species::Pumpkaboo_Large: return "Pumpkaboo-Large"_s;
		case Species::Pumpkaboo_Super: return "Pumpkaboo-Super"_s;
		case Species::Gourgeist_Small: return "Gourgeist-Small"_s;
		case Species::Gourgeist_Average: return "Gourgeist-Average"_s;
		case Species::Gourgeist_Large: return "Gourgeist-Large"_s;
		case Species::Gourgeist_Super: return "Gourgeist-Super"_s;
		case Species::Bergmite: return "Bergmite"_s;
		case Species::Avalugg: return "Avalugg"_s;
		case Species::Noibat: return "Noibat"_s;
		case Species::Noivern: return "Noivern"_s;
		case Species::Xerneas: return "Xerneas"_s;
		case Species::Yveltal: return "Yveltal"_s;
		case Species::Zygarde_50: return "Zygarde-50%"_s;
		case Species::Zygarde_10: return "Zygarde-10%"_s;
		case Species::Zygarde_Complete: return "Zygarde-Complete"_s;
		case Species::Diancie: return "Diancie"_s;
		case Species::Hoopa_Confined: return "Hoopa-Confined"_s;
		case Species::Hoopa_Unbound: return "Hoopa-Unbound"_s;
		case Species::Volcanion: return "Volcanion"_s;

		// Generation 7
		case Species::Rowlet: return "Rowlet"_s;
		case Species::Dartrix: return "Dartrix"_s;
		case Species::Decidueye: return "Decidueye"_s;
		case Species::Litten: return "Litten"_s;
		case Species::Torracat: return "Torracat"_s;
		case Species::Incineroar: return "Incineroar"_s;
		case Species::Popplio: return "Popplio"_s;
		case Species::Brionne: return "Brionne"_s;
		case Species::Primarina: return "Primarina"_s;
		case Species::Pikipek: return "Pikipek"_s;
		case Species::Trumbeak: return "Trumbeak"_s;
		case Species::Toucannon: return "Toucannon"_s;
		case Species::Yungoos: return "Yungoos"_s;
		case Species::Gumshoos: return "Gumshoos"_s;
		case Species::Grubbin: return "Grubbin"_s;
		case Species::Charjabug: return "Charjabug"_s;
		case Species::Vikavolt: return "Vikavolt"_s;
		case Species::Crabrawler: return "Crabrawler"_s;
		case Species::Crabominable: return "Crabominable"_s;
		case Species::Oricorio_Baile: return "Oricorio-Baile"_s;
		case Species::Oricorio_Pau: return "Oricorio-Pa'u"_s;
		case Species::Oricorio_Pom_Pom: return "Oricorio-Pom-Pom"_s;
		case Species::Oricorio_Sensu: return "Oricorio-Sensu"_s;
		case Species::Cutiefly: return "Cutiefly"_s;
		case Species::Ribombee: return "Ribombee"_s;
		case Species::Rockruff: return "Rockruff"_s;
		case Species::Lycanroc_Midday: return "Lycanroc-Midday"_s;
		case Species::Lycanroc_Midnight: return "Lycanroc-Midnight"_s;
		case Species::Lycanroc_Dusk: return "Lycanroc-Dusk"_s;
		case Species::Wishiwashi_Solo: return "Wishiwashi-Solo"_s;
		case Species::Wishiwashi_School: return "Wishiwashi-School"_s;
		case Species::Mareanie: return "Mareanie"_s;
		case Species::Toxapex: return "Toxapex"_s;
		case Species::Mudbray: return "Mudbray"_s;
		case Species::Mudsdale: return "Mudsdale"_s;
		case Species::Dewpider: return "Dewpider"_s;
		case Species::Araquanid: return "Araquanid"_s;
		case Species::Fomantis: return "Fomantis"_s;
		case Species::Lurantis: return "Lurantis"_s;
		case Species::Morelull: return "Morelull"_s;
		case Species::Shiinotic: return "Shiinotic"_s;
		case Species::Salandit: return "Salandit"_s;
		case Species::Salazzle: return "Salazzle"_s;
		case Species::Stufful: return "Stufful"_s;
		case Species::Bewear: return "Bewear"_s;
		case Species::Bounsweet: return "Bounsweet"_s;
		case Species::Steenee: return "Steenee"_s;
		case Species::Tsareena: return "Tsareena"_s;
		case Species::Comfey: return "Comfey"_s;
		case Species::Oranguru: return "Oranguru"_s;
		case Species::Passimian: return "Passimian"_s;
		case Species::Wimpod: return "Wimpod"_s;
		case Species::Golisopod: return "Golisopod"_s;
		case Species::Sandygast: return "Sandygast"_s;
		case Species::Palossand: return "Palossand"_s;
		case Species::Pyukumuku: return "Pyukumuku"_s;
		case Species::Type_Null: return "Type: Null"_s;
		case Species::Silvally: return "Silvally"_s;
		case Species::Minior_Meteor: return "Minior-Meteor"_s;
		case Species::Minior_Core: return "Minior-Core"_s;
		case Species::Komala: return "Komala"_s;
		case Species::Turtonator: return "Turtonator"_s;
		case Species::Togedemaru: return "Togedemaru"_s;
		case Species::Mimikyu: return "Mimikyu"_s;
		case Species::Bruxish: return "Bruxish"_s;
		case Species::Drampa: return "Drampa"_s;
		case Species::Dhelmise: return "Dhelmise"_s;
		case Species::Jangmo_o: return "Jangmo-o"_s;
		case Species::Hakamo_o: return "Hakamo-o"_s;
		case Species::Kommo_o: return "Kommo-o"_s;
		case Species::Tapu_Koko: return "Tapu Koko"_s;
		case Species::Tapu_Lele: return "Tapu Lele"_s;
		case Species::Tapu_Bulu: return "Tapu Bulu"_s;
		case Species::Tapu_Fini: return "Tapu Fini"_s;
		case Species::Cosmog: return "Cosmog"_s;
		case Species::Cosmoem: return "Cosmoem"_s;
		case Species::Solgaleo: return "Solgaleo"_s;
		case Species::Lunala: return "Lunala"_s;
		case Species::Nihilego: return "Nihilego"_s;
		case Species::Buzzwole: return "Buzzwole"_s;
		case Species::Pheromosa: return "Pheromosa"_s;
		case Species::Xurkitree: return "Xurkitree"_s;
		case Species::Celesteela: return "Celesteela"_s;
		case Species::Kartana: return "Kartana"_s;
		case Species::Guzzlord: return "Guzzlord"_s;
		case Species::Necrozma: return "Necrozma"_s;
		case Species::Necrozma_Dusk_Mane: return "Necrozma-Dusk Mane"_s;
		case Species::Necrozma_Dawn_Wings: return "Necrozma-Dawn Wings"_s;
		case Species::Necrozma_Ultra: return "Necrozma-Ultra"_s;
		case Species::Magearna: return "Magearna"_s;
		case Species::Marshadow: return "Marshadow"_s;
		case Species::Poipole: return "Poipole"_s;
		case Species::Naganadel: return "Naganadel"_s;
		case Species::Stakataka: return "Stakataka"_s;
		case Species::Blacephalon: return "Blacephalon"_s;
		case Species::Zeraora: return "Zeraora"_s;
		case Species::Meltan: return "Meltan"_s;
		case Species::Melmetal: return "Melmetal"_s;
		case Species::Rattata_Alola: return "Rattata-Alola"_s;
		case Species::Raticate_Alola: return "Raticate-Alola"_s;
		case Species::Raichu_Alola: return "Raichu-Alola"_s;
		case Species::Sandshrew_Alola: return "Sandshrew-Alola"_s;
		case Species::Sandslash_Alola: return "Sandslash-Alola"_s;
		case Species::Vulpix_Alola: return "Vulpix-Alola"_s;
		case Species::Ninetales_Alola: return "Ninetales-Alola"_s;
		case Species::Diglett_Alola: return "Diglett-Alola"_s;
		case Species::Dugtrio_Alola: return "Dugtrio-Alola"_s;
		case Species::Meowth_Alola: return "Meowth-Alola"_s;
		case Species::Persian_Alola: return "Persian-Alola"_s;
		case Species::Geodude_Alola: return "Geodude-Alola"_s;
		case Species::Graveler_Alola: return "Graveler-Alola"_s;
		case Species::Golem_Alola: return "Golem-Alola"_s;
		case Species::Grimer_Alola: return "Grimer-Alola"_s;
		case Species::Muk_Alola: return "Muk-Alola"_s;
		case Species::Exeggutor_Alola: return "Exeggutor-Alola"_s;
		case Species::Marowak_Alola: return "Marowak-Alola"_s;

		// Generation 8
		case Species::Grookey: return "Grookey"_s;
		case Species::Thwackey: return "Thwackey"_s;
		case Species::Rillaboom: return "Rillaboom"_s;
		case Species::Scorbunny: return "Scorbunny"_s;
		case Species::Raboot: return "Raboot"_s;
		case Species::Cinderace: return "Cinderace"_s;
		case Species::Sobble: return "Sobble"_s;
		case Species::Drizzile: return "Drizzile"_s;
		case Species::Inteleon: return "Inteleon"_s;
		case Species::Skwovet: return "Skwovet"_s;
		case Species::Greedent: return "Greedent"_s;
		case Species::Rookidee: return "Rookidee"_s;
		case Species::Corvisquire: return "Corvisquire"_s;
		case Species::Corviknight: return "Corviknight"_s;
		case Species::Blipbug: return "Blipbug"_s;
		case Species::Dottler: return "Dottler"_s;
		case Species::Orbeetle: return "Orbeetle"_s;
		case Species::Nickit: return "Nickit"_s;
		case Species::Thievul: return "Thievul"_s;
		case Species::Gossifleur: return "Gossifleur"_s;
		case Species::Eldegoss: return "Eldegoss"_s;
		case Species::Wooloo: return "Wooloo"_s;
		case Species::Dubwool: return "Dubwool"_s;
		case Species::Chewtle: return "Chewtle"_s;
		case Species::Drednaw: return "Drednaw"_s;
		case Species::Yamper: return "Yamper"_s;
		case Species::Boltund: return "Boltund"_s;
		case Species::Rolycoly: return "Rolycoly"_s;
		case Species::Carkol: return "Carkol"_s;
		case Species::Coalossal: return "Coalossal"_s;
		case Species::Applin: return "Applin"_s;
		case Species::Flapple: return "Flapple"_s;
		case Species::Appletun: return "Appletun"_s;
		case Species::Silicobra: return "Silicobra"_s;
		case Species::Sandaconda: return "Sandaconda"_s;
		case Species::Cramorant: return "Cramorant"_s;
		case Species::Arrokuda: return "Arrokuda"_s;
		case Species::Barraskewda: return "Barraskewda"_s;
		case Species::Toxel: return "Toxel"_s;
		case Species::Toxtricity_Amped: return "Toxtricity Amped"_s;
		case Species::Toxtricity_Low_Key: return "Toxtricity Low Key"_s;
		case Species::Sizzlipede: return "Sizzlipede"_s;
		case Species::Centiskorch: return "Centiskorch"_s;
		case Species::Clobbopus: return "Clobbopus"_s;
		case Species::Grapploct: return "Grapploct"_s;
		case Species::Sinistea: return "Sinistea"_s;
		case Species::Polteageist: return "Polteageist"_s;
		case Species::Hatenna: return "Hatenna"_s;
		case Species::Hattrem: return "Hattrem"_s;
		case Species::Hatterene: return "Hatterene"_s;
		case Species::Impidimp: return "Impidimp"_s;
		case Species::Morgrem: return "Morgrem"_s;
		case Species::Grimmsnarl: return "Grimmsnarl"_s;
		case Species::Obstagoon: return "Obstagoon"_s;
		case Species::Perrserker: return "Perrserker"_s;
		case Species::Cursola: return "Cursola"_s;
		case Species::Sirfetchd: return "Sirfetch'd"_s;
		case Species::Mr_Rime: return "Mr. Rime"_s;
		case Species::Runerigus: return "Runerigus"_s;
		case Species::Milcery: return "Milcery"_s;
		case Species::Alcremie: return "Alcremie"_s;
		case Species::Falinks: return "Falinks"_s;
		case Species::Pincurchin: return "Pincurchin"_s;
		case Species::Snom: return "Snom"_s;
		case Species::Frosmoth: return "Frosmoth"_s;
		case Species::Stonjourner: return "Stonjourner"_s;
		case Species::Eiscue: return "Eiscue"_s;
		case Species::Indeedee_F: return "Indeedee-F"_s;
		case Species::Indeedee_M: return "Indeedee-M"_s;
		case Species::Morpeko: return "Morpeko"_s;
		case Species::Cufant: return "Cufant"_s;
		case Species::Copperajah: return "Copperajah"_s;
		case Species::Dracozolt: return "Dracozolt"_s;
		case Species::Arctozolt: return "Arctozolt"_s;
		case Species::Dracovish: return "Dracovish"_s;
		case Species::Arctovish: return "Arctovish"_s;
		case Species::Duraludon: return "Duraludon"_s;
		case Species::Dreepy: return "Dreepy"_s;
		case Species::Drakloak: return "Drakloak"_s;
		case Species::Dragapult: return "Dragapult"_s;
		case Species::Zacian: return "Zacian"_s;
		case Species::Zacian_Crowned_Sword: return "Zacian Crowned Sword"_s;
		case Species::Zamazenta: return "Zamazenta"_s;
		case Species::Zamazenta_Crowned_Shield: return "Zamazenta Crowned Shield"_s;
		case Species::Eternatus: return "Eternatus"_s;
		case Species::Kubfu: return "Kubfu"_s;
		case Species::Urshifu_Single_Strike: return "Urshifu Single Strike"_s;
		case Species::Urshifu_Rapid_Strike: return "Urshifu Rapid Strike"_s;
		case Species::Zarude: return "Zarude"_s;
		case Species::Regieleki: return "Regieleki"_s;
		case Species::Regidrago: return "Regidrago"_s;
		case Species::Glastrier: return "Glastrier"_s;
		case Species::Spectrier: return "Spectrier"_s;
		case Species::Calyrex: return "Calyrex"_s;
		case Species::Calyrex_Ice_Rider: return "Calyrex Ice Rider"_s;
		case Species::Calyrex_Shadow_Rider: return "Calyrex Shadow Rider"_s;
		case Species::Meowth_Galar: return "Meowth-Galar"_s;
		case Species::Ponyta_Galar: return "Ponyta-Galar"_s;
		case Species::Rapidash_Galar: return "Rapidash-Galar"_s;
		case Species::Farfetchd_Galar: return "Farfetch'd-Galar"_s;
		case Species::Weezing_Galar: return "Weezing-Galar"_s;
		case Species::Mr_Mime_Galar: return "Mr. Mime-Galar"_s;
		case Species::Corsola_Galar: return "Corsola-Galar"_s;
		case Species::Zigzagoon_Galar: return "Zigzagoon-Galar"_s;
		case Species::Linoone_Galar: return "Linoone-Galar"_s;
		case Species::Darumaka_Galar: return "Darumaka-Galar"_s;
		case Species::Darmanitan_Galar: return "Darmanitan-Galar"_s;
		case Species::Yamask_Galar: return "Yamask-Galar"_s;
		case Species::Stunfisk_Galar: return "Stunfisk-Galar"_s;
		case Species::Slowpoke_Galar: return "Slowpoke-Galar"_s;
		case Species::Slowbro_Galar: return "Slowbro-Galar"_s;
		case Species::Articuno_Galar: return "Articuno-Galar"_s;
		case Species::Zapdos_Galar: return "Zapdos-Galar"_s;
		case Species::Moltres_Galar: return "Moltres-Galar"_s;
		case Species::Slowking_Galar: return "Slowking-Galar"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Species {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Species>>({
			{"abomasnow"_s, Species::Abomasnow},
			{"abomasnowmega"_s, Species::Abomasnow},
			{"abra"_s, Species::Abra},
			{"absol"_s, Species::Absol},
			{"absolmega"_s, Species::Absol},
			{"accelgor"_s, Species::Accelgor},
			{"aegislash"_s, Species::Aegislash},
			{"aerodactyl"_s, Species::Aerodactyl},
			{"aerodactylmega"_s, Species::Aerodactyl},
			{"aggron"_s, Species::Aggron},
			{"aggronmega"_s, Species::Aggron},
			{"aipom"_s, Species::Aipom},
			{"alakazam"_s, Species::Alakazam},
			{"alakazammega"_s, Species::Alakazam},
			{"alcremie"_s, Species::Alcremie},
			{"alcremiecaramel"_s, Species::Alcremie},
			{"alcremielemon"_s, Species::Alcremie},
			{"alcremiematcha"_s, Species::Alcremie},
			{"alcremiemint"_s, Species::Alcremie},
			{"alcremierainbow"_s, Species::Alcremie},
			{"alcremierubycream"_s, Species::Alcremie},
			{"alcremierubyswirl"_s, Species::Alcremie},
			{"alcremiesalted"_s, Species::Alcremie},
			{"alcremievanilla"_s, Species::Alcremie},
			{"alomomola"_s, Species::Alomomola},
			{"altaria"_s, Species::Altaria},
			{"altariamega"_s, Species::Altaria},
			{"amaura"_s, Species::Amaura},
			{"ambipom"_s, Species::Ambipom},
			{"amoonguss"_s, Species::Amoonguss},
			{"ampharos"_s, Species::Ampharos},
			{"ampharosmega"_s, Species::Ampharos},
			{"anorith"_s, Species::Anorith},
			{"appletun"_s, Species::Appletun},
			{"applin"_s, Species::Applin},
			{"araquanid"_s, Species::Araquanid},
			{"araquanidtotem"_s, Species::Araquanid},
			{"arbok"_s, Species::Arbok},
			{"arcanine"_s, Species::Arcanine},
			{"arceus"_s, Species::Arceus},
			{"arceusbug"_s, Species::Arceus},
			{"arceusdark"_s, Species::Arceus},
			{"arceusdragon"_s, Species::Arceus},
			{"arceuselectric"_s, Species::Arceus},
			{"arceusfighting"_s, Species::Arceus},
			{"arceusfire"_s, Species::Arceus},
			{"arceusflying"_s, Species::Arceus},
			{"arceusghost"_s, Species::Arceus},
			{"arceusgrass"_s, Species::Arceus},
			{"arceusground"_s, Species::Arceus},
			{"arceusice"_s, Species::Arceus},
			{"arceusnormal"_s, Species::Arceus},
			{"arceuspoison"_s, Species::Arceus},
			{"arceuspsychic"_s, Species::Arceus},
			{"arceusrock"_s, Species::Arceus},
			{"arceussteel"_s, Species::Arceus},
			{"arceuswater"_s, Species::Arceus},
			{"archen"_s, Species::Archen},
			{"archeops"_s, Species::Archeops},
			{"arctovish"_s, Species::Arctovish},
			{"arctozolt"_s, Species::Arctozolt},
			{"ariados"_s, Species::Ariados},
			{"armaldo"_s, Species::Armaldo},
			{"aromatisse"_s, Species::Aromatisse},
			{"aron"_s, Species::Aron},
			{"arrokuda"_s, Species::Arrokuda},
			{"articuno"_s, Species::Articuno},
			{"articunogalar"_s, Species::Articuno_Galar},
			{"audino"_s, Species::Audino},
			{"audinomega"_s, Species::Audino},
			{"aurorus"_s, Species::Aurorus},
			{"avalugg"_s, Species::Avalugg},
			{"axew"_s, Species::Axew},
			{"azelf"_s, Species::Azelf},
			{"azumarill"_s, Species::Azumarill},
			{"azurill"_s, Species::Azurill},
			{"bagon"_s, Species::Bagon},
			{"baltoy"_s, Species::Baltoy},
			{"banette"_s, Species::Banette},
			{"banettemega"_s, Species::Banette},
			{"barbaracle"_s, Species::Barbaracle},
			{"barboach"_s, Species::Barboach},
			{"barraskewda"_s, Species::Barraskewda},
			{"basculin"_s, Species::Basculin},
			{"basculinbluestriped"_s, Species::Basculin},
			{"bastiodon"_s, Species::Bastiodon},
			{"bayleef"_s, Species::Bayleef},
			{"beartic"_s, Species::Beartic},
			{"beautifly"_s, Species::Beautifly},
			{"beedrill"_s, Species::Beedrill},
			{"beedrillmega"_s, Species::Beedrill},
			{"beheeyem"_s, Species::Beheeyem},
			{"beldum"_s, Species::Beldum},
			{"bellossom"_s, Species::Bellossom},
			{"bellsprout"_s, Species::Bellsprout},
			{"bergmite"_s, Species::Bergmite},
			{"bewear"_s, Species::Bewear},
			{"bibarel"_s, Species::Bibarel},
			{"bidoof"_s, Species::Bidoof},
			{"binacle"_s, Species::Binacle},
			{"bisharp"_s, Species::Bisharp},
			{"blacephalon"_s, Species::Blacephalon},
			{"blastoise"_s, Species::Blastoise},
			{"blastoisemega"_s, Species::Blastoise},
			{"blaziken"_s, Species::Blaziken},
			{"blipbug"_s, Species::Blipbug},
			{"blissey"_s, Species::Blissey},
			{"blitzle"_s, Species::Blitzle},
			{"boldore"_s, Species::Boldore},
			{"boltund"_s, Species::Boltund},
			{"bonsly"_s, Species::Bonsly},
			{"bouffalant"_s, Species::Bouffalant},
			{"bounsweet"_s, Species::Bounsweet},
			{"braixen"_s, Species::Braixen},
			{"braviary"_s, Species::Braviary},
			{"breloom"_s, Species::Breloom},
			{"brionne"_s, Species::Brionne},
			{"bronzong"_s, Species::Bronzong},
			{"bronzor"_s, Species::Bronzor},
			{"bruxish"_s, Species::Bruxish},
			{"budew"_s, Species::Budew},
			{"buizel"_s, Species::Buizel},
			{"bulbasaur"_s, Species::Bulbasaur},
			{"buneary"_s, Species::Buneary},
			{"bunnelby"_s, Species::Bunnelby},
			{"burmy"_s, Species::Burmy},
			{"butterfree"_s, Species::Butterfree},
			{"buzzwole"_s, Species::Buzzwole},
			{"cacnea"_s, Species::Cacnea},
			{"cacturne"_s, Species::Cacturne},
			{"calyrex"_s, Species::Calyrex},
			{"calyrexicerider"_s, Species::Calyrex_Ice_Rider},
			{"calyrexshadowrider"_s, Species::Calyrex_Shadow_Rider},
			{"camerupt"_s, Species::Camerupt},
			{"cameruptmega"_s, Species::Camerupt},
			{"carbink"_s, Species::Carbink},
			{"carkol"_s, Species::Carkol},
			{"carnivine"_s, Species::Carnivine},
			{"carracosta"_s, Species::Carracosta},
			{"carvanha"_s, Species::Carvanha},
			{"cascoon"_s, Species::Cascoon},
			{"castform"_s, Species::Castform},
			{"caterpie"_s, Species::Caterpie},
			{"celebi"_s, Species::Celebi},
			{"celesteela"_s, Species::Celesteela},
			{"centiskorch"_s, Species::Centiskorch},
			{"chandelure"_s, Species::Chandelure},
			{"chansey"_s, Species::Chansey},
			{"charizard"_s, Species::Charizard},
			{"charizardmegax"_s, Species::Charizard},
			{"charizardmegay"_s, Species::Charizard},
			{"charjabug"_s, Species::Charjabug},
			{"charmander"_s, Species::Charmander},
			{"charmeleon"_s, Species::Charmeleon},
			{"chatot"_s, Species::Chatot},
			{"cherrim"_s, Species::Cherrim},
			{"cherubi"_s, Species::Cherubi},
			{"chesnaught"_s, Species::Chesnaught},
			{"chespin"_s, Species::Chespin},
			{"chewtle"_s, Species::Chewtle},
			{"chikorita"_s, Species::Chikorita},
			{"chimchar"_s, Species::Chimchar},
			{"chimecho"_s, Species::Chimecho},
			{"chinchou"_s, Species::Chinchou},
			{"chingling"_s, Species::Chingling},
			{"cinccino"_s, Species::Cinccino},
			{"cinderace"_s, Species::Cinderace},
			{"clamperl"_s, Species::Clamperl},
			{"clauncher"_s, Species::Clauncher},
			{"clawitzer"_s, Species::Clawitzer},
			{"claydol"_s, Species::Claydol},
			{"clefable"_s, Species::Clefable},
			{"clefairy"_s, Species::Clefairy},
			{"cleffa"_s, Species::Cleffa},
			{"clobbopus"_s, Species::Clobbopus},
			{"cloyster"_s, Species::Cloyster},
			{"coalossal"_s, Species::Coalossal},
			{"cobalion"_s, Species::Cobalion},
			{"cofagrigus"_s, Species::Cofagrigus},
			{"combee"_s, Species::Combee},
			{"combusken"_s, Species::Combusken},
			{"comfey"_s, Species::Comfey},
			{"conkeldurr"_s, Species::Conkeldurr},
			{"copperajah"_s, Species::Copperajah},
			{"corphish"_s, Species::Corphish},
			{"corsola"_s, Species::Corsola},
			{"corsolagalar"_s, Species::Corsola_Galar},
			{"corviknight"_s, Species::Corviknight},
			{"corvisquire"_s, Species::Corvisquire},
			{"cosmoem"_s, Species::Cosmoem},
			{"cosmog"_s, Species::Cosmog},
			{"cottonee"_s, Species::Cottonee},
			{"crabominable"_s, Species::Crabominable},
			{"crabrawler"_s, Species::Crabrawler},
			{"cradily"_s, Species::Cradily},
			{"cramorant"_s, Species::Cramorant},
			{"cranidos"_s, Species::Cranidos},
			{"crawdaunt"_s, Species::Crawdaunt},
			{"cresselia"_s, Species::Cresselia},
			{"croagunk"_s, Species::Croagunk},
			{"crobat"_s, Species::Crobat},
			{"croconaw"_s, Species::Croconaw},
			{"crustle"_s, Species::Crustle},
			{"cryogonal"_s, Species::Cryogonal},
			{"cubchoo"_s, Species::Cubchoo},
			{"cubone"_s, Species::Cubone},
			{"cufant"_s, Species::Cufant},
			{"cursola"_s, Species::Cursola},
			{"cutiefly"_s, Species::Cutiefly},
			{"cyndaquil"_s, Species::Cyndaquil},
			{"darkrai"_s, Species::Darkrai},
			{"darmanitan"_s, Species::Darmanitan},
			{"darmanitangalar"_s, Species::Darmanitan_Galar},
			{"dartrix"_s, Species::Dartrix},
			{"darumaka"_s, Species::Darumaka},
			{"darumakagalar"_s, Species::Darumaka_Galar},
			{"decidueye"_s, Species::Decidueye},
			{"dedenne"_s, Species::Dedenne},
			{"deerling"_s, Species::Deerling},
			{"deino"_s, Species::Deino},
			{"delcatty"_s, Species::Delcatty},
			{"delibird"_s, Species::Delibird},
			{"delphox"_s, Species::Delphox},
			{"deoxys"_s, Species::Deoxys_Normal},
			{"deoxysattack"_s, Species::Deoxys_Attack},
			{"deoxysdefense"_s, Species::Deoxys_Defense},
			{"deoxyse"_s, Species::Deoxys_Speed},
			{"deoxysf"_s, Species::Deoxys_Attack},
			{"deoxysl"_s, Species::Deoxys_Defense},
			{"deoxysnormal"_s, Species::Deoxys_Normal},
			{"deoxysspeed"_s, Species::Deoxys_Speed},
			{"dewgong"_s, Species::Dewgong},
			{"dewott"_s, Species::Dewott},
			{"dewpider"_s, Species::Dewpider},
			{"dhelmise"_s, Species::Dhelmise},
			{"dialga"_s, Species::Dialga},
			{"diancie"_s, Species::Diancie},
			{"dianciemega"_s, Species::Diancie},
			{"diggersby"_s, Species::Diggersby},
			{"diglett"_s, Species::Diglett},
			{"diglettalola"_s, Species::Diglett_Alola},
			{"ditto"_s, Species::Ditto},
			{"dodrio"_s, Species::Dodrio},
			{"doduo"_s, Species::Doduo},
			{"donphan"_s, Species::Donphan},
			{"dottler"_s, Species::Dottler},
			{"doublade"_s, Species::Doublade},
			{"dracovish"_s, Species::Dracovish},
			{"dracozolt"_s, Species::Dracozolt},
			{"dragalge"_s, Species::Dragalge},
			{"dragapult"_s, Species::Dragapult},
			{"dragonair"_s, Species::Dragonair},
			{"dragonite"_s, Species::Dragonite},
			{"drakloak"_s, Species::Drakloak},
			{"drampa"_s, Species::Drampa},
			{"drapion"_s, Species::Drapion},
			{"dratini"_s, Species::Dratini},
			{"drednaw"_s, Species::Drednaw},
			{"dreepy"_s, Species::Dreepy},
			{"drifblim"_s, Species::Drifblim},
			{"drifloon"_s, Species::Drifloon},
			{"drilbur"_s, Species::Drilbur},
			{"drizzile"_s, Species::Drizzile},
			{"drowzee"_s, Species::Drowzee},
			{"druddigon"_s, Species::Druddigon},
			{"dubwool"_s, Species::Dubwool},
			{"ducklett"_s, Species::Ducklett},
			{"dugtrio"_s, Species::Dugtrio},
			{"dugtrioalola"_s, Species::Dugtrio_Alola},
			{"dunsparce"_s, Species::Dunsparce},
			{"duosion"_s, Species::Duosion},
			{"duraludon"_s, Species::Duraludon},
			{"durant"_s, Species::Durant},
			{"dusclops"_s, Species::Dusclops},
			{"dusknoir"_s, Species::Dusknoir},
			{"duskull"_s, Species::Duskull},
			{"dustox"_s, Species::Dustox},
			{"dwebble"_s, Species::Dwebble},
			{"eelektrik"_s, Species::Eelektrik},
			{"eelektross"_s, Species::Eelektross},
			{"eevee"_s, Species::Eevee},
			{"eiscue"_s, Species::Eiscue},
			{"ekans"_s, Species::Ekans},
			{"eldegoss"_s, Species::Eldegoss},
			{"electabuzz"_s, Species::Electabuzz},
			{"electivire"_s, Species::Electivire},
			{"electrike"_s, Species::Electrike},
			{"electrode"_s, Species::Electrode},
			{"elekid"_s, Species::Elekid},
			{"elgyem"_s, Species::Elgyem},
			{"emboar"_s, Species::Emboar},
			{"emolga"_s, Species::Emolga},
			{"empoleon"_s, Species::Empoleon},
			{"entei"_s, Species::Entei},
			{"escavalier"_s, Species::Escavalier},
			{"espeon"_s, Species::Espeon},
			{"espurr"_s, Species::Espurr},
			{"eternatus"_s, Species::Eternatus},
			{"excadrill"_s, Species::Excadrill},
			{"exeggcute"_s, Species::Exeggcute},
			{"exeggutor"_s, Species::Exeggutor},
			{"exeggutoralola"_s, Species::Exeggutor_Alola},
			{"exploud"_s, Species::Exploud},
			{"falinks"_s, Species::Falinks},
			{"farfetchd"_s, Species::Farfetchd},
			{"farfetchdgalar"_s, Species::Farfetchd_Galar},
			{"fearow"_s, Species::Fearow},
			{"feebas"_s, Species::Feebas},
			{"fennekin"_s, Species::Fennekin},
			{"feraligatr"_s, Species::Feraligatr},
			{"ferroseed"_s, Species::Ferroseed},
			{"ferrothorn"_s, Species::Ferrothorn},
			{"finneon"_s, Species::Finneon},
			{"flaaffy"_s, Species::Flaaffy},
			{"flabebe"_s, Species::Flabebe},
			{"flapple"_s, Species::Flapple},
			{"flareon"_s, Species::Flareon},
			{"fletchinder"_s, Species::Fletchinder},
			{"fletchling"_s, Species::Fletchling},
			{"floatzel"_s, Species::Floatzel},
			{"floette"_s, Species::Floette},
			{"florges"_s, Species::Florges},
			{"flygon"_s, Species::Flygon},
			{"fomantis"_s, Species::Fomantis},
			{"foongus"_s, Species::Foongus},
			{"forretress"_s, Species::Forretress},
			{"fraxure"_s, Species::Fraxure},
			{"frillish"_s, Species::Frillish},
			{"froakie"_s, Species::Froakie},
			{"frogadier"_s, Species::Frogadier},
			{"froslass"_s, Species::Froslass},
			{"frosmoth"_s, Species::Frosmoth},
			{"furfrou"_s, Species::Furfrou},
			{"furret"_s, Species::Furret},
			{"gabite"_s, Species::Gabite},
			{"gallade"_s, Species::Gallade},
			{"gallademega"_s, Species::Gallade},
			{"galvantula"_s, Species::Galvantula},
			{"garbodor"_s, Species::Garbodor},
			{"garchomp"_s, Species::Garchomp},
			{"garchompmega"_s, Species::Garchomp},
			{"gardevoir"_s, Species::Gardevoir},
			{"gardevoirmega"_s, Species::Gardevoir},
			{"gastly"_s, Species::Gastly},
			{"gastrodon"_s, Species::Gastrodon},
			{"gastrodoneast"_s, Species::Gastrodon},
			{"gastrodonwest"_s, Species::Gastrodon},
			{"genesect"_s, Species::Genesect},
			{"genesectburn"_s, Species::Genesect},
			{"genesectchill"_s, Species::Genesect},
			{"genesectdouse"_s, Species::Genesect},
			{"genesectshock"_s, Species::Genesect},
			{"gengar"_s, Species::Gengar},
			{"geodude"_s, Species::Geodude},
			{"geodudealola"_s, Species::Geodude_Alola},
			{"gible"_s, Species::Gible},
			{"gigalith"_s, Species::Gigalith},
			{"girafarig"_s, Species::Girafarig},
			{"giratina"_s, Species::Giratina_Altered},
			{"giratinaaltered"_s, Species::Giratina_Altered},
			{"giratinaorigin"_s, Species::Giratina_Origin},
			{"glaceon"_s, Species::Glaceon},
			{"glalie"_s, Species::Glalie},
			{"glaliemega"_s, Species::Glalie},
			{"glameow"_s, Species::Glameow},
			{"glastrier"_s, Species::Glastrier},
			{"gligar"_s, Species::Gligar},
			{"gliscor"_s, Species::Gliscor},
			{"gloom"_s, Species::Gloom},
			{"gogoat"_s, Species::Gogoat},
			{"golbat"_s, Species::Golbat},
			{"goldeen"_s, Species::Goldeen},
			{"golduck"_s, Species::Golduck},
			{"golem"_s, Species::Golem},
			{"golemalola"_s, Species::Golem_Alola},
			{"golett"_s, Species::Golett},
			{"golisopod"_s, Species::Golisopod},
			{"golurk"_s, Species::Golurk},
			{"goodra"_s, Species::Goodra},
			{"goomy"_s, Species::Goomy},
			{"gorebyss"_s, Species::Gorebyss},
			{"gossifleur"_s, Species::Gossifleur},
			{"gothita"_s, Species::Gothita},
			{"gothitelle"_s, Species::Gothitelle},
			{"gothorita"_s, Species::Gothorita},
			{"gourgeist"_s, Species::Gourgeist_Average},
			{"gourgeistaverage"_s, Species::Gourgeist_Average},
			{"gourgeistlarge"_s, Species::Gourgeist_Large},
			{"gourgeistsmall"_s, Species::Gourgeist_Small},
			{"gourgeistsuper"_s, Species::Gourgeist_Super},
			{"granbull"_s, Species::Granbull},
			{"grapploct"_s, Species::Grapploct},
			{"graveler"_s, Species::Graveler},
			{"graveleralola"_s, Species::Graveler_Alola},
			{"greedent"_s, Species::Greedent},
			{"greninja"_s, Species::Greninja},
			{"greninjaash"_s, Species::Greninja},
			{"grimer"_s, Species::Grimer},
			{"grimeralola"_s, Species::Grimer_Alola},
			{"grimmsnarl"_s, Species::Grimmsnarl},
			{"grookey"_s, Species::Grookey},
			{"grotle"_s, Species::Grotle},
			{"groudon"_s, Species::Groudon},
			{"grovyle"_s, Species::Grovyle},
			{"growlithe"_s, Species::Growlithe},
			{"grubbin"_s, Species::Grubbin},
			{"grumpig"_s, Species::Grumpig},
			{"gulpin"_s, Species::Gulpin},
			{"gumshoos"_s, Species::Gumshoos},
			{"gumshoostotem"_s, Species::Gumshoos}, // TODO: ???
			{"gurdurr"_s, Species::Gurdurr},
			{"guzzlord"_s, Species::Guzzlord},
			{"gyarados"_s, Species::Gyarados},
			{"gyaradosmega"_s, Species::Gyarados},
			{"hakamoo"_s, Species::Hakamo_o},
			{"happiny"_s, Species::Happiny},
			{"hariyama"_s, Species::Hariyama},
			{"hatenna"_s, Species::Hatenna},
			{"hatterene"_s, Species::Hatterene},
			{"hattrem"_s, Species::Hattrem},
			{"haunter"_s, Species::Haunter},
			{"hawlucha"_s, Species::Hawlucha},
			{"haxorus"_s, Species::Haxorus},
			{"heatmor"_s, Species::Heatmor},
			{"heatran"_s, Species::Heatran},
			{"heliolisk"_s, Species::Heliolisk},
			{"helioptile"_s, Species::Helioptile},
			{"heracross"_s, Species::Heracross},
			{"heracrossmega"_s, Species::Heracross},
			{"herdier"_s, Species::Herdier},
			{"hippopotas"_s, Species::Hippopotas},
			{"hippowdon"_s, Species::Hippowdon},
			{"hitmonchan"_s, Species::Hitmonchan},
			{"hitmonlee"_s, Species::Hitmonlee},
			{"hitmontop"_s, Species::Hitmontop},
			{"honchkrow"_s, Species::Honchkrow},
			{"honedge"_s, Species::Honedge},
			{"hooh"_s, Species::Ho_Oh},
			{"hoopa"_s, Species::Hoopa_Confined},
			{"hoopaconfined"_s, Species::Hoopa_Confined},
			{"hoopaunbound"_s, Species::Hoopa_Unbound},
			{"hoothoot"_s, Species::Hoothoot},
			{"hoppip"_s, Species::Hoppip},
			{"horsea"_s, Species::Horsea},
			{"houndoom"_s, Species::Houndoom},
			{"houndoommega"_s, Species::Houndoom},
			{"houndour"_s, Species::Houndour},
			{"huntail"_s, Species::Huntail},
			{"hydreigon"_s, Species::Hydreigon},
			{"hypno"_s, Species::Hypno},
			{"igglybuff"_s, Species::Igglybuff},
			{"illumise"_s, Species::Illumise},
			{"impidimp"_s, Species::Impidimp},
			{"incineroar"_s, Species::Incineroar},
			{"indeedee"_s, Species::Indeedee_M},
			{"indeedeef"_s, Species::Indeedee_F},
			{"indeedeem"_s, Species::Indeedee_M},
			{"infernape"_s, Species::Infernape},
			{"inkay"_s, Species::Inkay},
			{"inteleon"_s, Species::Inteleon},
			{"ivysaur"_s, Species::Ivysaur},
			{"jangmoo"_s, Species::Jangmo_o},
			{"jellicent"_s, Species::Jellicent},
			{"jigglypuff"_s, Species::Jigglypuff},
			{"jirachi"_s, Species::Jirachi},
			{"jolteon"_s, Species::Jolteon},
			{"joltik"_s, Species::Joltik},
			{"jumpluff"_s, Species::Jumpluff},
			{"jynx"_s, Species::Jynx},
			{"kabuto"_s, Species::Kabuto},
			{"kabutops"_s, Species::Kabutops},
			{"kadabra"_s, Species::Kadabra},
			{"kakuna"_s, Species::Kakuna},
			{"kangaskhan"_s, Species::Kangaskhan},
			{"karrablast"_s, Species::Karrablast},
			{"kartana"_s, Species::Kartana},
			{"kecleon"_s, Species::Kecleon},
			{"keldeo"_s, Species::Keldeo},
			{"keldeoresolute"_s, Species::Keldeo_Resolute},
			{"kingdra"_s, Species::Kingdra},
			{"kingler"_s, Species::Kingler},
			{"kirlia"_s, Species::Kirlia},
			{"klang"_s, Species::Klang},
			{"klefki"_s, Species::Klefki},
			{"klink"_s, Species::Klink},
			{"klinklang"_s, Species::Klinklang},
			{"koffing"_s, Species::Koffing},
			{"komala"_s, Species::Komala},
			{"kommoo"_s, Species::Kommo_o},
			{"kommoototem"_s, Species::Kommo_o},
			{"krabby"_s, Species::Krabby},
			{"kricketot"_s, Species::Kricketot},
			{"kricketune"_s, Species::Kricketune},
			{"krokorok"_s, Species::Krokorok},
			{"krookodile"_s, Species::Krookodile},
			{"kubfu"_s, Species::Kubfu},
			{"kyogre"_s, Species::Kyogre},
			{"kyurem"_s, Species::Kyurem},
			{"kyuremblack"_s, Species::Kyurem_Black},
			{"kyuremwhite"_s, Species::Kyurem_White},
			{"lairon"_s, Species::Lairon},
			{"lampent"_s, Species::Lampent},
			{"landorus"_s, Species::Landorus_Incarnate},
			{"landorusincarnate"_s, Species::Landorus_Incarnate},
			{"landorustherian"_s, Species::Landorus_Therian},
			{"lanturn"_s, Species::Lanturn},
			{"lapras"_s, Species::Lapras},
			{"larvesta"_s, Species::Larvesta},
			{"larvitar"_s, Species::Larvitar},
			{"latias"_s, Species::Latias},
			{"latiasmega"_s, Species::Latias},
			{"latios"_s, Species::Latios},
			{"latiosmega"_s, Species::Latios},
			{"leafeon"_s, Species::Leafeon},
			{"leavanny"_s, Species::Leavanny},
			{"ledian"_s, Species::Ledian},
			{"ledyba"_s, Species::Ledyba},
			{"lickilicky"_s, Species::Lickilicky},
			{"lickitung"_s, Species::Lickitung},
			{"liepard"_s, Species::Liepard},
			{"lileep"_s, Species::Lileep},
			{"lilligant"_s, Species::Lilligant},
			{"lillipup"_s, Species::Lillipup},
			{"linoone"_s, Species::Linoone},
			{"linoonegalar"_s, Species::Linoone_Galar},
			{"litleo"_s, Species::Litleo},
			{"litten"_s, Species::Litten},
			{"litwick"_s, Species::Litwick},
			{"lombre"_s, Species::Lombre},
			{"lopunny"_s, Species::Lopunny},
			{"lopunnymega"_s, Species::Lopunny},
			{"lotad"_s, Species::Lotad},
			{"loudred"_s, Species::Loudred},
			{"lucario"_s, Species::Lucario},
			{"ludicolo"_s, Species::Ludicolo},
			{"lugia"_s, Species::Lugia},
			{"lumineon"_s, Species::Lumineon},
			{"lunala"_s, Species::Lunala},
			{"lunatone"_s, Species::Lunatone},
			{"lurantis"_s, Species::Lurantis},
			{"lurantistotem"_s, Species::Lurantis},
			{"luvdisc"_s, Species::Luvdisc},
			{"luxio"_s, Species::Luxio},
			{"luxray"_s, Species::Luxray},
			{"lycanroc"_s, Species::Lycanroc_Midday},
			{"lycanrocdusk"_s, Species::Lycanroc_Dusk},
			{"lycanrocmidday"_s, Species::Lycanroc_Midday},
			{"lycanrocmidnight"_s, Species::Lycanroc_Midnight},
			{"machamp"_s, Species::Machamp},
			{"machoke"_s, Species::Machoke},
			{"machop"_s, Species::Machop},
			{"magby"_s, Species::Magby},
			{"magcargo"_s, Species::Magcargo},
			{"magearna"_s, Species::Magearna},
			{"magikarp"_s, Species::Magikarp},
			{"magmar"_s, Species::Magmar},
			{"magmortar"_s, Species::Magmortar},
			{"magnemite"_s, Species::Magnemite},
			{"magneton"_s, Species::Magneton},
			{"magnezone"_s, Species::Magnezone},
			{"makuhita"_s, Species::Makuhita},
			{"malamar"_s, Species::Malamar},
			{"mamoswine"_s, Species::Mamoswine},
			{"manaphy"_s, Species::Manaphy},
			{"mandibuzz"_s, Species::Mandibuzz},
			{"manectric"_s, Species::Manectric},
			{"manectricmega"_s, Species::Manectric},
			{"mankey"_s, Species::Mankey},
			{"mantine"_s, Species::Mantine},
			{"mantyke"_s, Species::Mantyke},
			{"maractus"_s, Species::Maractus},
			{"mareanie"_s, Species::Mareanie},
			{"mareep"_s, Species::Mareep},
			{"marill"_s, Species::Marill},
			{"marowak"_s, Species::Marowak},
			{"marowakalola"_s, Species::Marowak_Alola},
			{"marowakalolatotem"_s, Species::Marowak_Alola},
			{"marshadow"_s, Species::Marshadow},
			{"marshtomp"_s, Species::Marshtomp},
			{"masquerain"_s, Species::Masquerain},
			{"mawile"_s, Species::Mawile},
			{"mawilemega"_s, Species::Mawile},
			{"medicham"_s, Species::Medicham},
			{"medichammega"_s, Species::Medicham},
			{"meditite"_s, Species::Meditite},
			{"meganium"_s, Species::Meganium},
			{"melmetal"_s, Species::Melmetal},
			{"meloetta"_s, Species::Meloetta},
			{"meltan"_s, Species::Meltan},
			{"meowstic"_s, Species::Meowstic_M},
			{"meowsticf"_s, Species::Meowstic_F},
			{"meowsticm"_s, Species::Meowstic_M},
			{"meowth"_s, Species::Meowth},
			{"meowthalola"_s, Species::Meowth_Alola},
			{"meowthgalar"_s, Species::Meowth_Galar},
			{"mesprit"_s, Species::Mesprit},
			{"metagross"_s, Species::Metagross},
			{"metagrossmega"_s, Species::Metagross},
			{"metang"_s, Species::Metang},
			{"metapod"_s, Species::Metapod},
			{"mew"_s, Species::Mew},
			{"mewtwo"_s, Species::Mewtwo},
			{"mienfoo"_s, Species::Mienfoo},
			{"mienshao"_s, Species::Mienshao},
			{"mightyena"_s, Species::Mightyena},
			{"milcery"_s, Species::Milcery},
			{"milotic"_s, Species::Milotic},
			{"miltank"_s, Species::Miltank},
			{"mimejr"_s, Species::Mime_Jr},
			{"mimikyu"_s, Species::Mimikyu},
			{"mimikyutotem"_s, Species::Mimikyu},
			{"minccino"_s, Species::Minccino},
			{"minior"_s, Species::Minior_Core},
			{"miniorcore"_s, Species::Minior_Core},
			{"miniormeteor"_s, Species::Minior_Meteor},
			{"minun"_s, Species::Minun},
			{"misdreavus"_s, Species::Misdreavus},
			{"mismagius"_s, Species::Mismagius},
			{"moltres"_s, Species::Moltres},
			{"moltresgalar"_s, Species::Moltres_Galar},
			{"monferno"_s, Species::Monferno},
			{"morelull"_s, Species::Morelull},
			{"morgrem"_s, Species::Morgrem},
			{"morpeko"_s, Species::Morpeko},
			{"mothim"_s, Species::Mothim},
			{"mrmime"_s, Species::Mr_Mime},
			{"mrmimegalar"_s, Species::Mr_Mime_Galar},
			{"mrrime"_s, Species::Mr_Rime},
			{"mudbray"_s, Species::Mudbray},
			{"mudkip"_s, Species::Mudkip},
			{"mudsdale"_s, Species::Mudsdale},
			{"muk"_s, Species::Muk},
			{"mukalola"_s, Species::Muk_Alola},
			{"munchlax"_s, Species::Munchlax},
			{"munna"_s, Species::Munna},
			{"murkrow"_s, Species::Murkrow},
			{"musharna"_s, Species::Musharna},
			{"naganadel"_s, Species::Naganadel},
			{"natu"_s, Species::Natu},
			{"necrozma"_s, Species::Necrozma},
			{"necrozmadawnwings"_s, Species::Necrozma_Dawn_Wings},
			{"necrozmaduskmane"_s, Species::Necrozma_Dusk_Mane},
			{"necrozmaultra"_s, Species::Necrozma_Ultra},
			{"nickit"_s, Species::Nickit},
			{"nidoking"_s, Species::Nidoking},
			{"nidoqueen"_s, Species::Nidoqueen},
			{"nidoranf"_s, Species::Nidoran_F},
			{"nidoranm"_s, Species::Nidoran_M},
			{"nidorina"_s, Species::Nidorina},
			{"nidorino"_s, Species::Nidorino},
			{"nihilego"_s, Species::Nihilego},
			{"nincada"_s, Species::Nincada},
			{"ninetales"_s, Species::Ninetales},
			{"ninetalesalola"_s, Species::Ninetales_Alola},
			{"ninjask"_s, Species::Ninjask},
			{"noctowl"_s, Species::Noctowl},
			{"noibat"_s, Species::Noibat},
			{"noivern"_s, Species::Noivern},
			{"nosepass"_s, Species::Nosepass},
			{"numel"_s, Species::Numel},
			{"nuzleaf"_s, Species::Nuzleaf},
			{"obstagoon"_s, Species::Obstagoon},
			{"octillery"_s, Species::Octillery},
			{"oddish"_s, Species::Oddish},
			{"omanyte"_s, Species::Omanyte},
			{"omastar"_s, Species::Omastar},
			{"onix"_s, Species::Onix},
			{"oranguru"_s, Species::Oranguru},
			{"orbeetle"_s, Species::Orbeetle},
			{"oricorio"_s, Species::Oricorio_Baile},
			{"oricoriobaile"_s, Species::Oricorio_Baile},
			{"oricoriopau"_s, Species::Oricorio_Pau},
			{"oricoriopompom"_s, Species::Oricorio_Pom_Pom},
			{"oricoriosensu"_s, Species::Oricorio_Sensu},
			{"oshawott"_s, Species::Oshawott},
			{"pachirisu"_s, Species::Pachirisu},
			{"palkia"_s, Species::Palkia},
			{"palossand"_s, Species::Palossand},
			{"palpitoad"_s, Species::Palpitoad},
			{"pancham"_s, Species::Pancham},
			{"pangoro"_s, Species::Pangoro},
			{"panpour"_s, Species::Panpour},
			{"pansage"_s, Species::Pansage},
			{"pansear"_s, Species::Pansear},
			{"paras"_s, Species::Paras},
			{"parasect"_s, Species::Parasect},
			{"passimian"_s, Species::Passimian},
			{"patrat"_s, Species::Patrat},
			{"pawniard"_s, Species::Pawniard},
			{"pelipper"_s, Species::Pelipper},
			{"perrserker"_s, Species::Perrserker},
			{"persian"_s, Species::Persian},
			{"persianalola"_s, Species::Persian_Alola},
			{"petilil"_s, Species::Petilil},
			{"phanpy"_s, Species::Phanpy},
			{"phantump"_s, Species::Phantump},
			{"pheromosa"_s, Species::Pheromosa},
			{"phione"_s, Species::Phione},
			{"pichu"_s, Species::Pichu},
			{"pidgeot"_s, Species::Pidgeot},
			{"pidgeotmega"_s, Species::Pidgeot},
			{"pidgeotto"_s, Species::Pidgeotto},
			{"pidgey"_s, Species::Pidgey},
			{"pidove"_s, Species::Pidove},
			{"pignite"_s, Species::Pignite},
			{"pikachu"_s, Species::Pikachu},
			{"pikachualola"_s, Species::Pikachu},
			{"pikachuhoenn"_s, Species::Pikachu},
			{"pikachukalos"_s, Species::Pikachu},
			{"pikachuoriginal"_s, Species::Pikachu},
			{"pikachupartner"_s, Species::Pikachu},
			{"pikachusinnoh"_s, Species::Pikachu},
			{"pikachuunova"_s, Species::Pikachu},
			{"pikachuworld"_s, Species::Pikachu},
			{"pikipek"_s, Species::Pikipek},
			{"piloswine"_s, Species::Piloswine},
			{"pincurchin"_s, Species::Pincurchin},
			{"pineco"_s, Species::Pineco},
			{"pinsir"_s, Species::Pinsir},
			{"pinsirmega"_s, Species::Pinsir},
			{"piplup"_s, Species::Piplup},
			{"plusle"_s, Species::Plusle},
			{"poipole"_s, Species::Poipole},
			{"politoed"_s, Species::Politoed},
			{"poliwag"_s, Species::Poliwag},
			{"poliwhirl"_s, Species::Poliwhirl},
			{"poliwrath"_s, Species::Poliwrath},
			{"polteageist"_s, Species::Polteageist},
			{"ponyta"_s, Species::Ponyta},
			{"ponytagalar"_s, Species::Ponyta_Galar},
			{"poochyena"_s, Species::Poochyena},
			{"popplio"_s, Species::Popplio},
			{"porygon"_s, Species::Porygon},
			{"porygon2"_s, Species::Porygon2},
			{"porygonz"_s, Species::Porygon_Z},
			{"primarina"_s, Species::Primarina},
			{"primeape"_s, Species::Primeape},
			{"prinplup"_s, Species::Prinplup},
			{"probopass"_s, Species::Probopass},
			{"psyduck"_s, Species::Psyduck},
			{"pumpkaboo"_s, Species::Pumpkaboo_Average},
			{"pumpkabooaverage"_s, Species::Pumpkaboo_Average},
			{"pumpkaboolarge"_s, Species::Pumpkaboo_Large},
			{"pumpkaboosmall"_s, Species::Pumpkaboo_Small},
			{"pumpkaboosuper"_s, Species::Pumpkaboo_Super},
			{"pupitar"_s, Species::Pupitar},
			{"purrloin"_s, Species::Purrloin},
			{"purugly"_s, Species::Purugly},
			{"pyroar"_s, Species::Pyroar},
			{"pyukumuku"_s, Species::Pyukumuku},
			{"quagsire"_s, Species::Quagsire},
			{"quilava"_s, Species::Quilava},
			{"quilladin"_s, Species::Quilladin},
			{"qwilfish"_s, Species::Qwilfish},
			{"raboot"_s, Species::Raboot},
			{"raichu"_s, Species::Raichu},
			{"raichualola"_s, Species::Raichu_Alola},
			{"raikou"_s, Species::Raikou},
			{"ralts"_s, Species::Ralts},
			{"rampardos"_s, Species::Rampardos},
			{"rapidash"_s, Species::Rapidash},
			{"rapidashgalar"_s, Species::Rapidash_Galar},
			{"raticate"_s, Species::Raticate},
			{"raticatealola"_s, Species::Raticate_Alola},
			{"raticatealolatotem"_s, Species::Raticate_Alola},
			{"rattata"_s, Species::Rattata},
			{"rattataalola"_s, Species::Rattata_Alola},
			{"rayquaza"_s, Species::Rayquaza},
			{"regice"_s, Species::Regice},
			{"regidrago"_s, Species::Regidrago},
			{"regieleki"_s, Species::Regieleki},
			{"regigigas"_s, Species::Regigigas},
			{"regirock"_s, Species::Regirock},
			{"registeel"_s, Species::Registeel},
			{"relicanth"_s, Species::Relicanth},
			{"remoraid"_s, Species::Remoraid},
			{"reshiram"_s, Species::Reshiram},
			{"reuniclus"_s, Species::Reuniclus},
			{"rhydon"_s, Species::Rhydon},
			{"rhyhorn"_s, Species::Rhyhorn},
			{"rhyperior"_s, Species::Rhyperior},
			{"ribombee"_s, Species::Ribombee},
			{"ribombeetotem"_s, Species::Ribombee},
			{"rillaboom"_s, Species::Rillaboom},
			{"riolu"_s, Species::Riolu},
			{"rockruff"_s, Species::Rockruff},
			{"roggenrola"_s, Species::Roggenrola},
			{"rolycoly"_s, Species::Rolycoly},
			{"rookidee"_s, Species::Rookidee},
			{"roselia"_s, Species::Roselia},
			{"roserade"_s, Species::Roserade},
			{"rotom"_s, Species::Rotom},
			{"rotomc"_s, Species::Rotom_Mow},
			{"rotomf"_s, Species::Rotom_Frost},
			{"rotomfan"_s, Species::Rotom_Fan},
			{"rotomfrost"_s, Species::Rotom_Frost},
			{"rotomh"_s, Species::Rotom_Heat},
			{"rotomheat"_s, Species::Rotom_Heat},
			{"rotommow"_s, Species::Rotom_Mow},
			{"rotoms"_s, Species::Rotom_Fan},
			{"rotomw"_s, Species::Rotom_Wash},
			{"rotomwash"_s, Species::Rotom_Wash},
			{"rowlet"_s, Species::Rowlet},
			{"rufflet"_s, Species::Rufflet},
			{"runerigus"_s, Species::Runerigus},
			{"sableye"_s, Species::Sableye},
			{"sableyemega"_s, Species::Sableye},
			{"salamence"_s, Species::Salamence},
			{"salandit"_s, Species::Salandit},
			{"salazzle"_s, Species::Salazzle},
			{"salazzletotem"_s, Species::Salazzle},
			{"samurott"_s, Species::Samurott},
			{"sandaconda"_s, Species::Sandaconda},
			{"sandile"_s, Species::Sandile},
			{"sandshrew"_s, Species::Sandshrew},
			{"sandshrewalola"_s, Species::Sandshrew_Alola},
			{"sandslash"_s, Species::Sandslash},
			{"sandslashalola"_s, Species::Sandslash_Alola},
			{"sandygast"_s, Species::Sandygast},
			{"sawk"_s, Species::Sawk},
			{"sawsbuck"_s, Species::Sawsbuck},
			{"scatterbug"_s, Species::Scatterbug},
			{"sceptile"_s, Species::Sceptile},
			{"sceptilemega"_s, Species::Sceptile},
			{"scizor"_s, Species::Scizor},
			{"scizormega"_s, Species::Scizor},
			{"scolipede"_s, Species::Scolipede},
			{"scorbunny"_s, Species::Scorbunny},
			{"scrafty"_s, Species::Scrafty},
			{"scraggy"_s, Species::Scraggy},
			{"scyther"_s, Species::Scyther},
			{"seadra"_s, Species::Seadra},
			{"seaking"_s, Species::Seaking},
			{"sealeo"_s, Species::Sealeo},
			{"seedot"_s, Species::Seedot},
			{"seel"_s, Species::Seel},
			{"seismitoad"_s, Species::Seismitoad},
			{"sentret"_s, Species::Sentret},
			{"serperior"_s, Species::Serperior},
			{"servine"_s, Species::Servine},
			{"seviper"_s, Species::Seviper},
			{"sewaddle"_s, Species::Sewaddle},
			{"sharpedo"_s, Species::Sharpedo},
			{"sharpedomega"_s, Species::Sharpedo},
			{"shaymin"_s, Species::Shaymin_Land},
			{"shayminland"_s, Species::Shaymin_Land},
			{"shayminsky"_s, Species::Shaymin_Sky},
			{"shedinja"_s, Species::Shedinja},
			{"shelgon"_s, Species::Shelgon},
			{"shellder"_s, Species::Shellder},
			{"shellos"_s, Species::Shellos},
			{"shelloseast"_s, Species::Shellos},
			{"shelloswest"_s, Species::Shellos},
			{"shelmet"_s, Species::Shelmet},
			{"shieldon"_s, Species::Shieldon},
			{"shiftry"_s, Species::Shiftry},
			{"shiinotic"_s, Species::Shiinotic},
			{"shinx"_s, Species::Shinx},
			{"shroomish"_s, Species::Shroomish},
			{"shuckle"_s, Species::Shuckle},
			{"shuppet"_s, Species::Shuppet},
			{"sigilyph"_s, Species::Sigilyph},
			{"silcoon"_s, Species::Silcoon},
			{"silicobra"_s, Species::Silicobra},
			{"silvally"_s, Species::Silvally},
			{"silvallybug"_s, Species::Silvally},
			{"silvallydark"_s, Species::Silvally},
			{"silvallydragon"_s, Species::Silvally},
			{"silvallyelectric"_s, Species::Silvally},
			{"silvallyfairy"_s, Species::Silvally},
			{"silvallyfighting"_s, Species::Silvally},
			{"silvallyfire"_s, Species::Silvally},
			{"silvallyflying"_s, Species::Silvally},
			{"silvallyghost"_s, Species::Silvally},
			{"silvallygrass"_s, Species::Silvally},
			{"silvallyground"_s, Species::Silvally},
			{"silvallyice"_s, Species::Silvally},
			{"silvallypoison"_s, Species::Silvally},
			{"silvallypsychic"_s, Species::Silvally},
			{"silvallyrock"_s, Species::Silvally},
			{"silvallysteel"_s, Species::Silvally},
			{"silvallywater"_s, Species::Silvally},
			{"simipour"_s, Species::Simipour},
			{"simisage"_s, Species::Simisage},
			{"simisear"_s, Species::Simisear},
			{"sinistea"_s, Species::Sinistea},
			{"sirfetchd"_s, Species::Sirfetchd},
			{"sizzlipede"_s, Species::Sizzlipede},
			{"skarmory"_s, Species::Skarmory},
			{"skiddo"_s, Species::Skiddo},
			{"skiploom"_s, Species::Skiploom},
			{"skitty"_s, Species::Skitty},
			{"skorupi"_s, Species::Skorupi},
			{"skrelp"_s, Species::Skrelp},
			{"skuntank"_s, Species::Skuntank},
			{"skwovet"_s, Species::Skwovet},
			{"slaking"_s, Species::Slaking},
			{"slakoth"_s, Species::Slakoth},
			{"sliggoo"_s, Species::Sliggoo},
			{"slowbro"_s, Species::Slowbro},
			{"slowbrogalar"_s, Species::Slowbro_Galar},
			{"slowbromega"_s, Species::Slowbro},
			{"slowking"_s, Species::Slowking},
			{"slowkinggalar"_s, Species::Slowking_Galar},
			{"slowpoke"_s, Species::Slowpoke},
			{"slowpokegalar"_s, Species::Slowpoke_Galar},
			{"slugma"_s, Species::Slugma},
			{"slurpuff"_s, Species::Slurpuff},
			{"smeargle"_s, Species::Smeargle},
			{"smoochum"_s, Species::Smoochum},
			{"sneasel"_s, Species::Sneasel},
			{"snivy"_s, Species::Snivy},
			{"snom"_s, Species::Snom},
			{"snorlax"_s, Species::Snorlax},
			{"snorunt"_s, Species::Snorunt},
			{"snover"_s, Species::Snover},
			{"snubbull"_s, Species::Snubbull},
			{"sobble"_s, Species::Sobble},
			{"solgaleo"_s, Species::Solgaleo},
			{"solosis"_s, Species::Solosis},
			{"solrock"_s, Species::Solrock},
			{"spearow"_s, Species::Spearow},
			{"spectrier"_s, Species::Spectrier},
			{"spewpa"_s, Species::Spewpa},
			{"spheal"_s, Species::Spheal},
			{"spinarak"_s, Species::Spinarak},
			{"spinda"_s, Species::Spinda},
			{"spiritomb"_s, Species::Spiritomb},
			{"spoink"_s, Species::Spoink},
			{"spritzee"_s, Species::Spritzee},
			{"squirtle"_s, Species::Squirtle},
			{"stakataka"_s, Species::Stakataka},
			{"stantler"_s, Species::Stantler},
			{"staraptor"_s, Species::Staraptor},
			{"staravia"_s, Species::Staravia},
			{"starly"_s, Species::Starly},
			{"starmie"_s, Species::Starmie},
			{"staryu"_s, Species::Staryu},
			{"steelix"_s, Species::Steelix},
			{"steelixmega"_s, Species::Steelix},
			{"steenee"_s, Species::Steenee},
			{"stonjourner"_s, Species::Stonjourner},
			{"stoutland"_s, Species::Stoutland},
			{"stufful"_s, Species::Stufful},
			{"stunfisk"_s, Species::Stunfisk},
			{"stunfiskgalar"_s, Species::Stunfisk_Galar},
			{"stunky"_s, Species::Stunky},
			{"sudowoodo"_s, Species::Sudowoodo},
			{"suicune"_s, Species::Suicune},
			{"sunflora"_s, Species::Sunflora},
			{"sunkern"_s, Species::Sunkern},
			{"surskit"_s, Species::Surskit},
			{"swablu"_s, Species::Swablu},
			{"swadloon"_s, Species::Swadloon},
			{"swalot"_s, Species::Swalot},
			{"swampert"_s, Species::Swampert},
			{"swampertmega"_s, Species::Swampert},
			{"swanna"_s, Species::Swanna},
			{"swellow"_s, Species::Swellow},
			{"swinub"_s, Species::Swinub},
			{"swirlix"_s, Species::Swirlix},
			{"swoobat"_s, Species::Swoobat},
			{"sylveon"_s, Species::Sylveon},
			{"taillow"_s, Species::Taillow},
			{"talonflame"_s, Species::Talonflame},
			{"tangela"_s, Species::Tangela},
			{"tangrowth"_s, Species::Tangrowth},
			{"tapubulu"_s, Species::Tapu_Bulu},
			{"tapufini"_s, Species::Tapu_Fini},
			{"tapukoko"_s, Species::Tapu_Koko},
			{"tapulele"_s, Species::Tapu_Lele},
			{"tauros"_s, Species::Tauros},
			{"teddiursa"_s, Species::Teddiursa},
			{"tentacool"_s, Species::Tentacool},
			{"tentacruel"_s, Species::Tentacruel},
			{"tepig"_s, Species::Tepig},
			{"terrakion"_s, Species::Terrakion},
			{"thievul"_s, Species::Thievul},
			{"throh"_s, Species::Throh},
			{"thundurus"_s, Species::Thundurus_Incarnate},
			{"thundurusincarnate"_s, Species::Thundurus_Incarnate},
			{"thundurustherian"_s, Species::Thundurus_Therian},
			{"thwackey"_s, Species::Thwackey},
			{"timburr"_s, Species::Timburr},
			{"tirtouga"_s, Species::Tirtouga},
			{"togedemaru"_s, Species::Togedemaru},
			{"togedemarutotem"_s, Species::Togedemaru},
			{"togekiss"_s, Species::Togekiss},
			{"togepi"_s, Species::Togepi},
			{"togetic"_s, Species::Togetic},
			{"torchic"_s, Species::Torchic},
			{"torkoal"_s, Species::Torkoal},
			{"tornadus"_s, Species::Tornadus_Incarnate},
			{"tornadusincarnate"_s, Species::Tornadus_Incarnate},
			{"tornadustherian"_s, Species::Tornadus_Therian},
			{"torracat"_s, Species::Torracat},
			{"torterra"_s, Species::Torterra},
			{"totodile"_s, Species::Totodile},
			{"toucannon"_s, Species::Toucannon},
			{"toxapex"_s, Species::Toxapex},
			{"toxel"_s, Species::Toxel},
			{"toxicroak"_s, Species::Toxicroak},
			{"toxtricity"_s, Species::Toxtricity_Amped},
			{"toxtricityamped"_s, Species::Toxtricity_Amped},
			{"toxtricitylowkey"_s, Species::Toxtricity_Low_Key},
			{"tranquill"_s, Species::Tranquill},
			{"trapinch"_s, Species::Trapinch},
			{"treecko"_s, Species::Treecko},
			{"trevenant"_s, Species::Trevenant},
			{"tropius"_s, Species::Tropius},
			{"trubbish"_s, Species::Trubbish},
			{"trumbeak"_s, Species::Trumbeak},
			{"tsareena"_s, Species::Tsareena},
			{"turtonator"_s, Species::Turtonator},
			{"turtwig"_s, Species::Turtwig},
			{"tympole"_s, Species::Tympole},
			{"tynamo"_s, Species::Tynamo},
			{"typenull"_s, Species::Type_Null},
			{"typhlosion"_s, Species::Typhlosion},
			{"tyranitar"_s, Species::Tyranitar},
			{"tyranitarmega"_s, Species::Tyranitar},
			{"tyrantrum"_s, Species::Tyrantrum},
			{"tyrogue"_s, Species::Tyrogue},
			{"tyrunt"_s, Species::Tyrunt},
			{"umbreon"_s, Species::Umbreon},
			{"unfezant"_s, Species::Unfezant},
			{"unown"_s, Species::Unown},
			{"unowna"_s, Species::Unown},
			{"unownb"_s, Species::Unown},
			{"unownc"_s, Species::Unown},
			{"unownd"_s, Species::Unown},
			{"unowne"_s, Species::Unown},
			{"unownexclamation"_s, Species::Unown},
			{"unownf"_s, Species::Unown},
			{"unowng"_s, Species::Unown},
			{"unownh"_s, Species::Unown},
			{"unowni"_s, Species::Unown},
			{"unownj"_s, Species::Unown},
			{"unownk"_s, Species::Unown},
			{"unownl"_s, Species::Unown},
			{"unownm"_s, Species::Unown},
			{"unownn"_s, Species::Unown},
			{"unowno"_s, Species::Unown},
			{"unownp"_s, Species::Unown},
			{"unownq"_s, Species::Unown},
			{"unownquestion"_s, Species::Unown},
			{"unownr"_s, Species::Unown},
			{"unowns"_s, Species::Unown},
			{"unownt"_s, Species::Unown},
			{"unownu"_s, Species::Unown},
			{"unownv"_s, Species::Unown},
			{"unownw"_s, Species::Unown},
			{"unownx"_s, Species::Unown},
			{"unowny"_s, Species::Unown},
			{"unownz"_s, Species::Unown},
			{"ursaring"_s, Species::Ursaring},
			{"urshifurapidstrike"_s, Species::Urshifu_Rapid_Strike},
			{"urshifusinglestrike"_s, Species::Urshifu_Single_Strike},
			{"uxie"_s, Species::Uxie},
			{"vanillish"_s, Species::Vanillish},
			{"vanillite"_s, Species::Vanillite},
			{"vanilluxe"_s, Species::Vanilluxe},
			{"vaporeon"_s, Species::Vaporeon},
			{"venipede"_s, Species::Venipede},
			{"venomoth"_s, Species::Venomoth},
			{"venonat"_s, Species::Venonat},
			{"venusaur"_s, Species::Venusaur},
			{"venusaurmega"_s, Species::Venusaur},
			{"vespiquen"_s, Species::Vespiquen},
			{"vibrava"_s, Species::Vibrava},
			{"victini"_s, Species::Victini},
			{"victreebel"_s, Species::Victreebel},
			{"vigoroth"_s, Species::Vigoroth},
			{"vikavolt"_s, Species::Vikavolt},
			{"vikavolttotem"_s, Species::Vikavolt},
			{"vileplume"_s, Species::Vileplume},
			{"virizion"_s, Species::Virizion},
			{"vivillon"_s, Species::Vivillon},
			{"volbeat"_s, Species::Volbeat},
			{"volcanion"_s, Species::Volcanion},
			{"volcarona"_s, Species::Volcarona},
			{"voltorb"_s, Species::Voltorb},
			{"vullaby"_s, Species::Vullaby},
			{"vulpix"_s, Species::Vulpix},
			{"vulpixalola"_s, Species::Vulpix_Alola},
			{"wailmer"_s, Species::Wailmer},
			{"wailord"_s, Species::Wailord},
			{"walrein"_s, Species::Walrein},
			{"wartortle"_s, Species::Wartortle},
			{"watchog"_s, Species::Watchog},
			{"weavile"_s, Species::Weavile},
			{"weedle"_s, Species::Weedle},
			{"weepinbell"_s, Species::Weepinbell},
			{"weezing"_s, Species::Weezing},
			{"weezinggalar"_s, Species::Weezing_Galar},
			{"whimsicott"_s, Species::Whimsicott},
			{"whirlipede"_s, Species::Whirlipede},
			{"whiscash"_s, Species::Whiscash},
			{"whismur"_s, Species::Whismur},
			{"wigglytuff"_s, Species::Wigglytuff},
			{"wimpod"_s, Species::Wimpod},
			{"wingull"_s, Species::Wingull},
			{"wishiwashi"_s, Species::Wishiwashi_Solo},
			{"wishiwashischool"_s, Species::Wishiwashi_School},
			{"wishiwashisolo"_s, Species::Wishiwashi_Solo},
			{"wobbuffet"_s, Species::Wobbuffet},
			{"woobat"_s, Species::Woobat},
			{"wooloo"_s, Species::Wooloo},
			{"wooper"_s, Species::Wooper},
			{"wormadam"_s, Species::Wormadam_Plant},
			{"wormadamg"_s, Species::Wormadam_Sandy},
			{"wormadamplant"_s, Species::Wormadam_Plant},
			{"wormadams"_s, Species::Wormadam_Trash},
			{"wormadamsandy"_s, Species::Wormadam_Sandy},
			{"wormadamtrash"_s, Species::Wormadam_Trash},
			{"wurmple"_s, Species::Wurmple},
			{"wynaut"_s, Species::Wynaut},
			{"xatu"_s, Species::Xatu},
			{"xerneas"_s, Species::Xerneas},
			{"xurkitree"_s, Species::Xurkitree},
			{"yamask"_s, Species::Yamask},
			{"yamaskgalar"_s, Species::Yamask_Galar},
			{"yamper"_s, Species::Yamper},
			{"yanma"_s, Species::Yanma},
			{"yanmega"_s, Species::Yanmega},
			{"yungoos"_s, Species::Yungoos},
			{"yveltal"_s, Species::Yveltal},
			{"zacian"_s, Species::Zacian},
			{"zaciancrownedsword"_s, Species::Zacian_Crowned_Sword},
			{"zamazenta"_s, Species::Zamazenta},
			{"zamazentacrownedshield"_s, Species::Zamazenta_Crowned_Shield},
			{"zangoose"_s, Species::Zangoose},
			{"zapdos"_s, Species::Zapdos},
			{"zapdosgalar"_s, Species::Zapdos_Galar},
			{"zarude"_s, Species::Zarude},
			{"zebstrika"_s, Species::Zebstrika},
			{"zekrom"_s, Species::Zekrom},
			{"zeraora"_s, Species::Zeraora},
			{"zigzagoon"_s, Species::Zigzagoon},
			{"zigzagoongalar"_s, Species::Zigzagoon_Galar},
			{"zoroark"_s, Species::Zoroark},
			{"zorua"_s, Species::Zorua},
			{"zubat"_s, Species::Zubat},
			{"zweilous"_s, Species::Zweilous},
			{"zygarde"_s, Species::Zygarde_50},
			{"zygarde10"_s, Species::Zygarde_10},
			{"zygarde50"_s, Species::Zygarde_50},
			{"zygardecomplete"_s, Species::Zygarde_Complete},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<22_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Species"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
