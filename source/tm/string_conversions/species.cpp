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
		case Species::Bulbasaur: return "Bulbasaur"_sv;
		case Species::Ivysaur: return "Ivysaur"_sv;
		case Species::Venusaur: return "Venusaur"_sv;
		case Species::Charmander: return "Charmander"_sv;
		case Species::Charmeleon: return "Charmeleon"_sv;
		case Species::Charizard: return "Charizard"_sv;
		case Species::Squirtle: return "Squirtle"_sv;
		case Species::Wartortle: return "Wartortle"_sv;
		case Species::Blastoise: return "Blastoise"_sv;
		case Species::Caterpie: return "Caterpie"_sv;
		case Species::Metapod: return "Metapod"_sv;
		case Species::Butterfree: return "Butterfree"_sv;
		case Species::Weedle: return "Weedle"_sv;
		case Species::Kakuna: return "Kakuna"_sv;
		case Species::Beedrill: return "Beedrill"_sv;
		case Species::Pidgey: return "Pidgey"_sv;
		case Species::Pidgeotto: return "Pidgeotto"_sv;
		case Species::Pidgeot: return "Pidgeot"_sv;
		case Species::Rattata: return "Rattata"_sv;
		case Species::Raticate: return "Raticate"_sv;
		case Species::Spearow: return "Spearow"_sv;
		case Species::Fearow: return "Fearow"_sv;
		case Species::Ekans: return "Ekans"_sv;
		case Species::Arbok: return "Arbok"_sv;
		case Species::Pikachu: return "Pikachu"_sv;
		case Species::Raichu: return "Raichu"_sv;
		case Species::Sandshrew: return "Sandshrew"_sv;
		case Species::Sandslash: return "Sandslash"_sv;
		case Species::Nidoran_F: return "Nidoran-F"_sv;
		case Species::Nidorina: return "Nidorina"_sv;
		case Species::Nidoqueen: return "Nidoqueen"_sv;
		case Species::Nidoran_M: return "Nidoran-M"_sv;
		case Species::Nidorino: return "Nidorino"_sv;
		case Species::Nidoking: return "Nidoking"_sv;
		case Species::Clefairy: return "Clefairy"_sv;
		case Species::Clefable: return "Clefable"_sv;
		case Species::Vulpix: return "Vulpix"_sv;
		case Species::Ninetales: return "Ninetales"_sv;
		case Species::Jigglypuff: return "Jigglypuff"_sv;
		case Species::Wigglytuff: return "Wigglytuff"_sv;
		case Species::Zubat: return "Zubat"_sv;
		case Species::Golbat: return "Golbat"_sv;
		case Species::Oddish: return "Oddish"_sv;
		case Species::Gloom: return "Gloom"_sv;
		case Species::Vileplume: return "Vileplume"_sv;
		case Species::Paras: return "Paras"_sv;
		case Species::Parasect: return "Parasect"_sv;
		case Species::Venonat: return "Venonat"_sv;
		case Species::Venomoth: return "Venomoth"_sv;
		case Species::Diglett: return "Diglett"_sv;
		case Species::Dugtrio: return "Dugtrio"_sv;
		case Species::Meowth: return "Meowth"_sv;
		case Species::Persian: return "Persian"_sv;
		case Species::Psyduck: return "Psyduck"_sv;
		case Species::Golduck: return "Golduck"_sv;
		case Species::Mankey: return "Mankey"_sv;
		case Species::Primeape: return "Primeape"_sv;
		case Species::Growlithe: return "Growlithe"_sv;
		case Species::Arcanine: return "Arcanine"_sv;
		case Species::Poliwag: return "Poliwag"_sv;
		case Species::Poliwhirl: return "Poliwhirl"_sv;
		case Species::Poliwrath: return "Poliwrath"_sv;
		case Species::Abra: return "Abra"_sv;
		case Species::Kadabra: return "Kadabra"_sv;
		case Species::Alakazam: return "Alakazam"_sv;
		case Species::Machop: return "Machop"_sv;
		case Species::Machoke: return "Machoke"_sv;
		case Species::Machamp: return "Machamp"_sv;
		case Species::Bellsprout: return "Bellsprout"_sv;
		case Species::Weepinbell: return "Weepinbell"_sv;
		case Species::Victreebel: return "Victreebel"_sv;
		case Species::Tentacool: return "Tentacool"_sv;
		case Species::Tentacruel: return "Tentacruel"_sv;
		case Species::Geodude: return "Geodude"_sv;
		case Species::Graveler: return "Graveler"_sv;
		case Species::Golem: return "Golem"_sv;
		case Species::Ponyta: return "Ponyta"_sv;
		case Species::Rapidash: return "Rapidash"_sv;
		case Species::Slowpoke: return "Slowpoke"_sv;
		case Species::Slowbro: return "Slowbro"_sv;
		case Species::Magnemite: return "Magnemite"_sv;
		case Species::Magneton: return "Magneton"_sv;
		case Species::Farfetchd: return "Farfetch'd"_sv;
		case Species::Doduo: return "Doduo"_sv;
		case Species::Dodrio: return "Dodrio"_sv;
		case Species::Seel: return "Seel"_sv;
		case Species::Dewgong: return "Dewgong"_sv;
		case Species::Grimer: return "Grimer"_sv;
		case Species::Muk: return "Muk"_sv;
		case Species::Shellder: return "Shellder"_sv;
		case Species::Cloyster: return "Cloyster"_sv;
		case Species::Gastly: return "Gastly"_sv;
		case Species::Haunter: return "Haunter"_sv;
		case Species::Gengar: return "Gengar"_sv;
		case Species::Onix: return "Onix"_sv;
		case Species::Drowzee: return "Drowzee"_sv;
		case Species::Hypno: return "Hypno"_sv;
		case Species::Krabby: return "Krabby"_sv;
		case Species::Kingler: return "Kingler"_sv;
		case Species::Voltorb: return "Voltorb"_sv;
		case Species::Electrode: return "Electrode"_sv;
		case Species::Exeggcute: return "Exeggcute"_sv;
		case Species::Exeggutor: return "Exeggutor"_sv;
		case Species::Cubone: return "Cubone"_sv;
		case Species::Marowak: return "Marowak"_sv;
		case Species::Hitmonlee: return "Hitmonlee"_sv;
		case Species::Hitmonchan: return "Hitmonchan"_sv;
		case Species::Lickitung: return "Lickitung"_sv;
		case Species::Koffing: return "Koffing"_sv;
		case Species::Weezing: return "Weezing"_sv;
		case Species::Rhyhorn: return "Rhyhorn"_sv;
		case Species::Rhydon: return "Rhydon"_sv;
		case Species::Chansey: return "Chansey"_sv;
		case Species::Tangela: return "Tangela"_sv;
		case Species::Kangaskhan: return "Kangaskhan"_sv;
		case Species::Horsea: return "Horsea"_sv;
		case Species::Seadra: return "Seadra"_sv;
		case Species::Goldeen: return "Goldeen"_sv;
		case Species::Seaking: return "Seaking"_sv;
		case Species::Staryu: return "Staryu"_sv;
		case Species::Starmie: return "Starmie"_sv;
		case Species::Mr_Mime: return "Mr. Mime"_sv;
		case Species::Scyther: return "Scyther"_sv;
		case Species::Jynx: return "Jynx"_sv;
		case Species::Electabuzz: return "Electabuzz"_sv;
		case Species::Magmar: return "Magmar"_sv;
		case Species::Pinsir: return "Pinsir"_sv;
		case Species::Tauros: return "Tauros"_sv;
		case Species::Magikarp: return "Magikarp"_sv;
		case Species::Gyarados: return "Gyarados"_sv;
		case Species::Lapras: return "Lapras"_sv;
		case Species::Ditto: return "Ditto"_sv;
		case Species::Eevee: return "Eevee"_sv;
		case Species::Vaporeon: return "Vaporeon"_sv;
		case Species::Jolteon: return "Jolteon"_sv;
		case Species::Flareon: return "Flareon"_sv;
		case Species::Porygon: return "Porygon"_sv;
		case Species::Omanyte: return "Omanyte"_sv;
		case Species::Omastar: return "Omastar"_sv;
		case Species::Kabuto: return "Kabuto"_sv;
		case Species::Kabutops: return "Kabutops"_sv;
		case Species::Aerodactyl: return "Aerodactyl"_sv;
		case Species::Snorlax: return "Snorlax"_sv;
		case Species::Articuno: return "Articuno"_sv;
		case Species::Zapdos: return "Zapdos"_sv;
		case Species::Moltres: return "Moltres"_sv;
		case Species::Dratini: return "Dratini"_sv;
		case Species::Dragonair: return "Dragonair"_sv;
		case Species::Dragonite: return "Dragonite"_sv;
		case Species::Mewtwo: return "Mewtwo"_sv;
		case Species::Mew: return "Mew"_sv;

		// Generation 2
		case Species::Chikorita: return "Chikorita"_sv;
		case Species::Bayleef: return "Bayleef"_sv;
		case Species::Meganium: return "Meganium"_sv;
		case Species::Cyndaquil: return "Cyndaquil"_sv;
		case Species::Quilava: return "Quilava"_sv;
		case Species::Typhlosion: return "Typhlosion"_sv;
		case Species::Totodile: return "Totodile"_sv;
		case Species::Croconaw: return "Croconaw"_sv;
		case Species::Feraligatr: return "Feraligatr"_sv;
		case Species::Sentret: return "Sentret"_sv;
		case Species::Furret: return "Furret"_sv;
		case Species::Hoothoot: return "Hoothoot"_sv;
		case Species::Noctowl: return "Noctowl"_sv;
		case Species::Ledyba: return "Ledyba"_sv;
		case Species::Ledian: return "Ledian"_sv;
		case Species::Spinarak: return "Spinarak"_sv;
		case Species::Ariados: return "Ariados"_sv;
		case Species::Crobat: return "Crobat"_sv;
		case Species::Chinchou: return "Chinchou"_sv;
		case Species::Lanturn: return "Lanturn"_sv;
		case Species::Pichu: return "Pichu"_sv;
		case Species::Cleffa: return "Cleffa"_sv;
		case Species::Igglybuff: return "Igglybuff"_sv;
		case Species::Togepi: return "Togepi"_sv;
		case Species::Togetic: return "Togetic"_sv;
		case Species::Natu: return "Natu"_sv;
		case Species::Xatu: return "Xatu"_sv;
		case Species::Mareep: return "Mareep"_sv;
		case Species::Flaaffy: return "Flaaffy"_sv;
		case Species::Ampharos: return "Ampharos"_sv;
		case Species::Bellossom: return "Bellossom"_sv;
		case Species::Marill: return "Marill"_sv;
		case Species::Azumarill: return "Azumarill"_sv;
		case Species::Sudowoodo: return "Sudowoodo"_sv;
		case Species::Politoed: return "Politoed"_sv;
		case Species::Hoppip: return "Hoppip"_sv;
		case Species::Skiploom: return "Skiploom"_sv;
		case Species::Jumpluff: return "Jumpluff"_sv;
		case Species::Aipom: return "Aipom"_sv;
		case Species::Sunkern: return "Sunkern"_sv;
		case Species::Sunflora: return "Sunflora"_sv;
		case Species::Yanma: return "Yanma"_sv;
		case Species::Wooper: return "Wooper"_sv;
		case Species::Quagsire: return "Quagsire"_sv;
		case Species::Espeon: return "Espeon"_sv;
		case Species::Umbreon: return "Umbreon"_sv;
		case Species::Murkrow: return "Murkrow"_sv;
		case Species::Slowking: return "Slowking"_sv;
		case Species::Misdreavus: return "Misdreavus"_sv;
		case Species::Unown: return "Unown"_sv;
		case Species::Wobbuffet: return "Wobbuffet"_sv;
		case Species::Girafarig: return "Girafarig"_sv;
		case Species::Pineco: return "Pineco"_sv;
		case Species::Forretress: return "Forretress"_sv;
		case Species::Dunsparce: return "Dunsparce"_sv;
		case Species::Gligar: return "Gligar"_sv;
		case Species::Steelix: return "Steelix"_sv;
		case Species::Snubbull: return "Snubbull"_sv;
		case Species::Granbull: return "Granbull"_sv;
		case Species::Qwilfish: return "Qwilfish"_sv;
		case Species::Scizor: return "Scizor"_sv;
		case Species::Shuckle: return "Shuckle"_sv;
		case Species::Heracross: return "Heracross"_sv;
		case Species::Sneasel: return "Sneasel"_sv;
		case Species::Teddiursa: return "Teddiursa"_sv;
		case Species::Ursaring: return "Ursaring"_sv;
		case Species::Slugma: return "Slugma"_sv;
		case Species::Magcargo: return "Magcargo"_sv;
		case Species::Swinub: return "Swinub"_sv;
		case Species::Piloswine: return "Piloswine"_sv;
		case Species::Corsola: return "Corsola"_sv;
		case Species::Remoraid: return "Remoraid"_sv;
		case Species::Octillery: return "Octillery"_sv;
		case Species::Delibird: return "Delibird"_sv;
		case Species::Mantine: return "Mantine"_sv;
		case Species::Skarmory: return "Skarmory"_sv;
		case Species::Houndour: return "Houndour"_sv;
		case Species::Houndoom: return "Houndoom"_sv;
		case Species::Kingdra: return "Kingdra"_sv;
		case Species::Phanpy: return "Phanpy"_sv;
		case Species::Donphan: return "Donphan"_sv;
		case Species::Porygon2: return "Porygon2"_sv;
		case Species::Stantler: return "Stantler"_sv;
		case Species::Smeargle: return "Smeargle"_sv;
		case Species::Tyrogue: return "Tyrogue"_sv;
		case Species::Hitmontop: return "Hitmontop"_sv;
		case Species::Smoochum: return "Smoochum"_sv;
		case Species::Elekid: return "Elekid"_sv;
		case Species::Magby: return "Magby"_sv;
		case Species::Miltank: return "Miltank"_sv;
		case Species::Blissey: return "Blissey"_sv;
		case Species::Raikou: return "Raikou"_sv;
		case Species::Entei: return "Entei"_sv;
		case Species::Suicune: return "Suicune"_sv;
		case Species::Larvitar: return "Larvitar"_sv;
		case Species::Pupitar: return "Pupitar"_sv;
		case Species::Tyranitar: return "Tyranitar"_sv;
		case Species::Lugia: return "Lugia"_sv;
		case Species::Ho_Oh: return "Ho-Oh"_sv;
		case Species::Celebi: return "Celebi"_sv;

		// Generation 3
		case Species::Treecko: return "Treecko"_sv;
		case Species::Grovyle: return "Grovyle"_sv;
		case Species::Sceptile: return "Sceptile"_sv;
		case Species::Torchic: return "Torchic"_sv;
		case Species::Combusken: return "Combusken"_sv;
		case Species::Blaziken: return "Blaziken"_sv;
		case Species::Mudkip: return "Mudkip"_sv;
		case Species::Marshtomp: return "Marshtomp"_sv;
		case Species::Swampert: return "Swampert"_sv;
		case Species::Poochyena: return "Poochyena"_sv;
		case Species::Mightyena: return "Mightyena"_sv;
		case Species::Zigzagoon: return "Zigzagoon"_sv;
		case Species::Linoone: return "Linoone"_sv;
		case Species::Wurmple: return "Wurmple"_sv;
		case Species::Silcoon: return "Silcoon"_sv;
		case Species::Beautifly: return "Beautifly"_sv;
		case Species::Cascoon: return "Cascoon"_sv;
		case Species::Dustox: return "Dustox"_sv;
		case Species::Lotad: return "Lotad"_sv;
		case Species::Lombre: return "Lombre"_sv;
		case Species::Ludicolo: return "Ludicolo"_sv;
		case Species::Seedot: return "Seedot"_sv;
		case Species::Nuzleaf: return "Nuzleaf"_sv;
		case Species::Shiftry: return "Shiftry"_sv;
		case Species::Taillow: return "Taillow"_sv;
		case Species::Swellow: return "Swellow"_sv;
		case Species::Wingull: return "Wingull"_sv;
		case Species::Pelipper: return "Pelipper"_sv;
		case Species::Ralts: return "Ralts"_sv;
		case Species::Kirlia: return "Kirlia"_sv;
		case Species::Gardevoir: return "Gardevoir"_sv;
		case Species::Surskit: return "Surskit"_sv;
		case Species::Masquerain: return "Masquerain"_sv;
		case Species::Shroomish: return "Shroomish"_sv;
		case Species::Breloom: return "Breloom"_sv;
		case Species::Slakoth: return "Slakoth"_sv;
		case Species::Vigoroth: return "Vigoroth"_sv;
		case Species::Slaking: return "Slaking"_sv;
		case Species::Nincada: return "Nincada"_sv;
		case Species::Ninjask: return "Ninjask"_sv;
		case Species::Shedinja: return "Shedinja"_sv;
		case Species::Whismur: return "Whismur"_sv;
		case Species::Loudred: return "Loudred"_sv;
		case Species::Exploud: return "Exploud"_sv;
		case Species::Makuhita: return "Makuhita"_sv;
		case Species::Hariyama: return "Hariyama"_sv;
		case Species::Azurill: return "Azurill"_sv;
		case Species::Nosepass: return "Nosepass"_sv;
		case Species::Skitty: return "Skitty"_sv;
		case Species::Delcatty: return "Delcatty"_sv;
		case Species::Sableye: return "Sableye"_sv;
		case Species::Mawile: return "Mawile"_sv;
		case Species::Aron: return "Aron"_sv;
		case Species::Lairon: return "Lairon"_sv;
		case Species::Aggron: return "Aggron"_sv;
		case Species::Meditite: return "Meditite"_sv;
		case Species::Medicham: return "Medicham"_sv;
		case Species::Electrike: return "Electrike"_sv;
		case Species::Manectric: return "Manectric"_sv;
		case Species::Plusle: return "Plusle"_sv;
		case Species::Minun: return "Minun"_sv;
		case Species::Volbeat: return "Volbeat"_sv;
		case Species::Illumise: return "Illumise"_sv;
		case Species::Roselia: return "Roselia"_sv;
		case Species::Gulpin: return "Gulpin"_sv;
		case Species::Swalot: return "Swalot"_sv;
		case Species::Carvanha: return "Carvanha"_sv;
		case Species::Sharpedo: return "Sharpedo"_sv;
		case Species::Wailmer: return "Wailmer"_sv;
		case Species::Wailord: return "Wailord"_sv;
		case Species::Numel: return "Numel"_sv;
		case Species::Camerupt: return "Camerupt"_sv;
		case Species::Torkoal: return "Torkoal"_sv;
		case Species::Spoink: return "Spoink"_sv;
		case Species::Grumpig: return "Grumpig"_sv;
		case Species::Spinda: return "Spinda"_sv;
		case Species::Trapinch: return "Trapinch"_sv;
		case Species::Vibrava: return "Vibrava"_sv;
		case Species::Flygon: return "Flygon"_sv;
		case Species::Cacnea: return "Cacnea"_sv;
		case Species::Cacturne: return "Cacturne"_sv;
		case Species::Swablu: return "Swablu"_sv;
		case Species::Altaria: return "Altaria"_sv;
		case Species::Zangoose: return "Zangoose"_sv;
		case Species::Seviper: return "Seviper"_sv;
		case Species::Lunatone: return "Lunatone"_sv;
		case Species::Solrock: return "Solrock"_sv;
		case Species::Barboach: return "Barboach"_sv;
		case Species::Whiscash: return "Whiscash"_sv;
		case Species::Corphish: return "Corphish"_sv;
		case Species::Crawdaunt: return "Crawdaunt"_sv;
		case Species::Baltoy: return "Baltoy"_sv;
		case Species::Claydol: return "Claydol"_sv;
		case Species::Lileep: return "Lileep"_sv;
		case Species::Cradily: return "Cradily"_sv;
		case Species::Anorith: return "Anorith"_sv;
		case Species::Armaldo: return "Armaldo"_sv;
		case Species::Feebas: return "Feebas"_sv;
		case Species::Milotic: return "Milotic"_sv;
		case Species::Castform: return "Castform"_sv;
		case Species::Kecleon: return "Kecleon"_sv;
		case Species::Shuppet: return "Shuppet"_sv;
		case Species::Banette: return "Banette"_sv;
		case Species::Duskull: return "Duskull"_sv;
		case Species::Dusclops: return "Dusclops"_sv;
		case Species::Tropius: return "Tropius"_sv;
		case Species::Chimecho: return "Chimecho"_sv;
		case Species::Absol: return "Absol"_sv;
		case Species::Wynaut: return "Wynaut"_sv;
		case Species::Snorunt: return "Snorunt"_sv;
		case Species::Glalie: return "Glalie"_sv;
		case Species::Spheal: return "Spheal"_sv;
		case Species::Sealeo: return "Sealeo"_sv;
		case Species::Walrein: return "Walrein"_sv;
		case Species::Clamperl: return "Clamperl"_sv;
		case Species::Huntail: return "Huntail"_sv;
		case Species::Gorebyss: return "Gorebyss"_sv;
		case Species::Relicanth: return "Relicanth"_sv;
		case Species::Luvdisc: return "Luvdisc"_sv;
		case Species::Bagon: return "Bagon"_sv;
		case Species::Shelgon: return "Shelgon"_sv;
		case Species::Salamence: return "Salamence"_sv;
		case Species::Beldum: return "Beldum"_sv;
		case Species::Metang: return "Metang"_sv;
		case Species::Metagross: return "Metagross"_sv;
		case Species::Regirock: return "Regirock"_sv;
		case Species::Regice: return "Regice"_sv;
		case Species::Registeel: return "Registeel"_sv;
		case Species::Latias: return "Latias"_sv;
		case Species::Latios: return "Latios"_sv;
		case Species::Kyogre: return "Kyogre"_sv;
		case Species::Groudon: return "Groudon"_sv;
		case Species::Rayquaza: return "Rayquaza"_sv;
		case Species::Jirachi: return "Jirachi"_sv;
		case Species::Deoxys_Normal: return "Deoxys-Normal"_sv;
		case Species::Deoxys_Attack: return "Deoxys-Attack"_sv;
		case Species::Deoxys_Defense: return "Deoxys-Defense"_sv;
		case Species::Deoxys_Speed: return "Deoxys-Speed"_sv;

		// Generation 4
		case Species::Turtwig: return "Turtwig"_sv;
		case Species::Grotle: return "Grotle"_sv;
		case Species::Torterra: return "Torterra"_sv;
		case Species::Chimchar: return "Chimchar"_sv;
		case Species::Monferno: return "Monferno"_sv;
		case Species::Infernape: return "Infernape"_sv;
		case Species::Piplup: return "Piplup"_sv;
		case Species::Prinplup: return "Prinplup"_sv;
		case Species::Empoleon: return "Empoleon"_sv;
		case Species::Starly: return "Starly"_sv;
		case Species::Staravia: return "Staravia"_sv;
		case Species::Staraptor: return "Staraptor"_sv;
		case Species::Bidoof: return "Bidoof"_sv;
		case Species::Bibarel: return "Bibarel"_sv;
		case Species::Kricketot: return "Kricketot"_sv;
		case Species::Kricketune: return "Kricketune"_sv;
		case Species::Shinx: return "Shinx"_sv;
		case Species::Luxio: return "Luxio"_sv;
		case Species::Luxray: return "Luxray"_sv;
		case Species::Budew: return "Budew"_sv;
		case Species::Roserade: return "Roserade"_sv;
		case Species::Cranidos: return "Cranidos"_sv;
		case Species::Rampardos: return "Rampardos"_sv;
		case Species::Shieldon: return "Shieldon"_sv;
		case Species::Bastiodon: return "Bastiodon"_sv;
		case Species::Burmy: return "Burmy"_sv;
		case Species::Wormadam_Plant: return "Wormadam-Plant"_sv;
		case Species::Wormadam_Sandy: return "Wormadam-Sandy"_sv;
		case Species::Wormadam_Trash: return "Wormadam-Trash"_sv;
		case Species::Mothim: return "Mothim"_sv;
		case Species::Combee: return "Combee"_sv;
		case Species::Vespiquen: return "Vespiquen"_sv;
		case Species::Pachirisu: return "Pachirisu"_sv;
		case Species::Buizel: return "Buizel"_sv;
		case Species::Floatzel: return "Floatzel"_sv;
		case Species::Cherubi: return "Cherubi"_sv;
		case Species::Cherrim: return "Cherrim"_sv;
		case Species::Shellos: return "Shellos"_sv;
		case Species::Gastrodon: return "Gastrodon"_sv;
		case Species::Ambipom: return "Ambipom"_sv;
		case Species::Drifloon: return "Drifloon"_sv;
		case Species::Drifblim: return "Drifblim"_sv;
		case Species::Buneary: return "Buneary"_sv;
		case Species::Lopunny: return "Lopunny"_sv;
		case Species::Mismagius: return "Mismagius"_sv;
		case Species::Honchkrow: return "Honchkrow"_sv;
		case Species::Glameow: return "Glameow"_sv;
		case Species::Purugly: return "Purugly"_sv;
		case Species::Chingling: return "Chingling"_sv;
		case Species::Stunky: return "Stunky"_sv;
		case Species::Skuntank: return "Skuntank"_sv;
		case Species::Bronzor: return "Bronzor"_sv;
		case Species::Bronzong: return "Bronzong"_sv;
		case Species::Bonsly: return "Bonsly"_sv;
		case Species::Mime_Jr: return "Mime Jr."_sv;
		case Species::Happiny: return "Happiny"_sv;
		case Species::Chatot: return "Chatot"_sv;
		case Species::Spiritomb: return "Spiritomb"_sv;
		case Species::Gible: return "Gible"_sv;
		case Species::Gabite: return "Gabite"_sv;
		case Species::Garchomp: return "Garchomp"_sv;
		case Species::Munchlax: return "Munchlax"_sv;
		case Species::Riolu: return "Riolu"_sv;
		case Species::Lucario: return "Lucario"_sv;
		case Species::Hippopotas: return "Hippopotas"_sv;
		case Species::Hippowdon: return "Hippowdon"_sv;
		case Species::Skorupi: return "Skorupi"_sv;
		case Species::Drapion: return "Drapion"_sv;
		case Species::Croagunk: return "Croagunk"_sv;
		case Species::Toxicroak: return "Toxicroak"_sv;
		case Species::Carnivine: return "Carnivine"_sv;
		case Species::Finneon: return "Finneon"_sv;
		case Species::Lumineon: return "Lumineon"_sv;
		case Species::Mantyke: return "Mantyke"_sv;
		case Species::Snover: return "Snover"_sv;
		case Species::Abomasnow: return "Abomasnow"_sv;
		case Species::Weavile: return "Weavile"_sv;
		case Species::Magnezone: return "Magnezone"_sv;
		case Species::Lickilicky: return "Lickilicky"_sv;
		case Species::Rhyperior: return "Rhyperior"_sv;
		case Species::Tangrowth: return "Tangrowth"_sv;
		case Species::Electivire: return "Electivire"_sv;
		case Species::Magmortar: return "Magmortar"_sv;
		case Species::Togekiss: return "Togekiss"_sv;
		case Species::Yanmega: return "Yanmega"_sv;
		case Species::Leafeon: return "Leafeon"_sv;
		case Species::Glaceon: return "Glaceon"_sv;
		case Species::Gliscor: return "Gliscor"_sv;
		case Species::Mamoswine: return "Mamoswine"_sv;
		case Species::Porygon_Z: return "Porygon-Z"_sv;
		case Species::Gallade: return "Gallade"_sv;
		case Species::Probopass: return "Probopass"_sv;
		case Species::Dusknoir: return "Dusknoir"_sv;
		case Species::Froslass: return "Froslass"_sv;
		case Species::Rotom: return "Rotom"_sv;
		case Species::Rotom_Heat: return "Rotom-Heat"_sv;
		case Species::Rotom_Wash: return "Rotom-Wash"_sv;
		case Species::Rotom_Frost: return "Rotom-Frost"_sv;
		case Species::Rotom_Fan: return "Rotom-Fan"_sv;
		case Species::Rotom_Mow: return "Rotom-Mow"_sv;
		case Species::Uxie: return "Uxie"_sv;
		case Species::Mesprit: return "Mesprit"_sv;
		case Species::Azelf: return "Azelf"_sv;
		case Species::Dialga: return "Dialga"_sv;
		case Species::Palkia: return "Palkia"_sv;
		case Species::Heatran: return "Heatran"_sv;
		case Species::Regigigas: return "Regigigas"_sv;
		case Species::Giratina_Altered: return "Giratina-Altered"_sv;
		case Species::Giratina_Origin: return "Giratina-Origin"_sv;
		case Species::Cresselia: return "Cresselia"_sv;
		case Species::Phione: return "Phione"_sv;
		case Species::Manaphy: return "Manaphy"_sv;
		case Species::Darkrai: return "Darkrai"_sv;
		case Species::Shaymin_Land: return "Shaymin-Land"_sv;
		case Species::Shaymin_Sky: return "Shaymin-Sky"_sv;
		case Species::Arceus: return "Arceus"_sv;

		// Generation 5
		case Species::Victini: return "Victini"_sv;
		case Species::Snivy: return "Snivy"_sv;
		case Species::Servine: return "Servine"_sv;
		case Species::Serperior: return "Serperior"_sv;
		case Species::Tepig: return "Tepig"_sv;
		case Species::Pignite: return "Pignite"_sv;
		case Species::Emboar: return "Emboar"_sv;
		case Species::Oshawott: return "Oshawott"_sv;
		case Species::Dewott: return "Dewott"_sv;
		case Species::Samurott: return "Samurott"_sv;
		case Species::Patrat: return "Patrat"_sv;
		case Species::Watchog: return "Watchog"_sv;
		case Species::Lillipup: return "Lillipup"_sv;
		case Species::Herdier: return "Herdier"_sv;
		case Species::Stoutland: return "Stoutland"_sv;
		case Species::Purrloin: return "Purrloin"_sv;
		case Species::Liepard: return "Liepard"_sv;
		case Species::Pansage: return "Pansage"_sv;
		case Species::Simisage: return "Simisage"_sv;
		case Species::Pansear: return "Pansear"_sv;
		case Species::Simisear: return "Simisear"_sv;
		case Species::Panpour: return "Panpour"_sv;
		case Species::Simipour: return "Simipour"_sv;
		case Species::Munna: return "Munna"_sv;
		case Species::Musharna: return "Musharna"_sv;
		case Species::Pidove: return "Pidove"_sv;
		case Species::Tranquill: return "Tranquill"_sv;
		case Species::Unfezant: return "Unfezant"_sv;
		case Species::Blitzle: return "Blitzle"_sv;
		case Species::Zebstrika: return "Zebstrika"_sv;
		case Species::Roggenrola: return "Roggenrola"_sv;
		case Species::Boldore: return "Boldore"_sv;
		case Species::Gigalith: return "Gigalith"_sv;
		case Species::Woobat: return "Woobat"_sv;
		case Species::Swoobat: return "Swoobat"_sv;
		case Species::Drilbur: return "Drilbur"_sv;
		case Species::Excadrill: return "Excadrill"_sv;
		case Species::Audino: return "Audino"_sv;
		case Species::Timburr: return "Timburr"_sv;
		case Species::Gurdurr: return "Gurdurr"_sv;
		case Species::Conkeldurr: return "Conkeldurr"_sv;
		case Species::Tympole: return "Tympole"_sv;
		case Species::Palpitoad: return "Palpitoad"_sv;
		case Species::Seismitoad: return "Seismitoad"_sv;
		case Species::Throh: return "Throh"_sv;
		case Species::Sawk: return "Sawk"_sv;
		case Species::Sewaddle: return "Sewaddle"_sv;
		case Species::Swadloon: return "Swadloon"_sv;
		case Species::Leavanny: return "Leavanny"_sv;
		case Species::Venipede: return "Venipede"_sv;
		case Species::Whirlipede: return "Whirlipede"_sv;
		case Species::Scolipede: return "Scolipede"_sv;
		case Species::Cottonee: return "Cottonee"_sv;
		case Species::Whimsicott: return "Whimsicott"_sv;
		case Species::Petilil: return "Petilil"_sv;
		case Species::Lilligant: return "Lilligant"_sv;
		case Species::Basculin: return "Basculin"_sv;
		case Species::Sandile: return "Sandile"_sv;
		case Species::Krokorok: return "Krokorok"_sv;
		case Species::Krookodile: return "Krookodile"_sv;
		case Species::Darumaka: return "Darumaka"_sv;
		case Species::Darmanitan: return "Darmanitan"_sv;
		case Species::Maractus: return "Maractus"_sv;
		case Species::Dwebble: return "Dwebble"_sv;
		case Species::Crustle: return "Crustle"_sv;
		case Species::Scraggy: return "Scraggy"_sv;
		case Species::Scrafty: return "Scrafty"_sv;
		case Species::Sigilyph: return "Sigilyph"_sv;
		case Species::Yamask: return "Yamask"_sv;
		case Species::Cofagrigus: return "Cofagrigus"_sv;
		case Species::Tirtouga: return "Tirtouga"_sv;
		case Species::Carracosta: return "Carracosta"_sv;
		case Species::Archen: return "Archen"_sv;
		case Species::Archeops: return "Archeops"_sv;
		case Species::Trubbish: return "Trubbish"_sv;
		case Species::Garbodor: return "Garbodor"_sv;
		case Species::Zorua: return "Zorua"_sv;
		case Species::Zoroark: return "Zoroark"_sv;
		case Species::Minccino: return "Minccino"_sv;
		case Species::Cinccino: return "Cinccino"_sv;
		case Species::Gothita: return "Gothita"_sv;
		case Species::Gothorita: return "Gothorita"_sv;
		case Species::Gothitelle: return "Gothitelle"_sv;
		case Species::Solosis: return "Solosis"_sv;
		case Species::Duosion: return "Duosion"_sv;
		case Species::Reuniclus: return "Reuniclus"_sv;
		case Species::Ducklett: return "Ducklett"_sv;
		case Species::Swanna: return "Swanna"_sv;
		case Species::Vanillite: return "Vanillite"_sv;
		case Species::Vanillish: return "Vanillish"_sv;
		case Species::Vanilluxe: return "Vanilluxe"_sv;
		case Species::Deerling: return "Deerling"_sv;
		case Species::Sawsbuck: return "Sawsbuck"_sv;
		case Species::Emolga: return "Emolga"_sv;
		case Species::Karrablast: return "Karrablast"_sv;
		case Species::Escavalier: return "Escavalier"_sv;
		case Species::Foongus: return "Foongus"_sv;
		case Species::Amoonguss: return "Amoonguss"_sv;
		case Species::Frillish: return "Frillish"_sv;
		case Species::Jellicent: return "Jellicent"_sv;
		case Species::Alomomola: return "Alomomola"_sv;
		case Species::Joltik: return "Joltik"_sv;
		case Species::Galvantula: return "Galvantula"_sv;
		case Species::Ferroseed: return "Ferroseed"_sv;
		case Species::Ferrothorn: return "Ferrothorn"_sv;
		case Species::Klink: return "Klink"_sv;
		case Species::Klang: return "Klang"_sv;
		case Species::Klinklang: return "Klinklang"_sv;
		case Species::Tynamo: return "Tynamo"_sv;
		case Species::Eelektrik: return "Eelektrik"_sv;
		case Species::Eelektross: return "Eelektross"_sv;
		case Species::Elgyem: return "Elgyem"_sv;
		case Species::Beheeyem: return "Beheeyem"_sv;
		case Species::Litwick: return "Litwick"_sv;
		case Species::Lampent: return "Lampent"_sv;
		case Species::Chandelure: return "Chandelure"_sv;
		case Species::Axew: return "Axew"_sv;
		case Species::Fraxure: return "Fraxure"_sv;
		case Species::Haxorus: return "Haxorus"_sv;
		case Species::Cubchoo: return "Cubchoo"_sv;
		case Species::Beartic: return "Beartic"_sv;
		case Species::Cryogonal: return "Cryogonal"_sv;
		case Species::Shelmet: return "Shelmet"_sv;
		case Species::Accelgor: return "Accelgor"_sv;
		case Species::Stunfisk: return "Stunfisk"_sv;
		case Species::Mienfoo: return "Mienfoo"_sv;
		case Species::Mienshao: return "Mienshao"_sv;
		case Species::Druddigon: return "Druddigon"_sv;
		case Species::Golett: return "Golett"_sv;
		case Species::Golurk: return "Golurk"_sv;
		case Species::Pawniard: return "Pawniard"_sv;
		case Species::Bisharp: return "Bisharp"_sv;
		case Species::Bouffalant: return "Bouffalant"_sv;
		case Species::Rufflet: return "Rufflet"_sv;
		case Species::Braviary: return "Braviary"_sv;
		case Species::Vullaby: return "Vullaby"_sv;
		case Species::Mandibuzz: return "Mandibuzz"_sv;
		case Species::Heatmor: return "Heatmor"_sv;
		case Species::Durant: return "Durant"_sv;
		case Species::Deino: return "Deino"_sv;
		case Species::Zweilous: return "Zweilous"_sv;
		case Species::Hydreigon: return "Hydreigon"_sv;
		case Species::Larvesta: return "Larvesta"_sv;
		case Species::Volcarona: return "Volcarona"_sv;
		case Species::Cobalion: return "Cobalion"_sv;
		case Species::Terrakion: return "Terrakion"_sv;
		case Species::Virizion: return "Virizion"_sv;
		case Species::Tornadus_Incarnate: return "Tornadus-Incarnate"_sv;
		case Species::Tornadus_Therian: return "Tornadus-Therian"_sv;
		case Species::Thundurus_Incarnate: return "Thundurus-Incarnate"_sv;
		case Species::Thundurus_Therian: return "Thundurus-Therian"_sv;
		case Species::Reshiram: return "Reshiram"_sv;
		case Species::Zekrom: return "Zekrom"_sv;
		case Species::Landorus_Incarnate: return "Landorus-Incarnate"_sv;
		case Species::Landorus_Therian: return "Landorus-Therian"_sv;
		case Species::Kyurem: return "Kyurem"_sv;
		case Species::Kyurem_Black: return "Kyurem-Black"_sv;
		case Species::Kyurem_White: return "Kyurem-White"_sv;
		case Species::Keldeo: return "Keldeo"_sv;
		case Species::Keldeo_Resolute: return "Keldeo-Resolute"_sv;
		case Species::Meloetta: return "Meloetta"_sv;
		case Species::Genesect: return "Genesect"_sv;

		// Generation 6
		case Species::Chespin: return "Chespin"_sv;
		case Species::Quilladin: return "Quilladin"_sv;
		case Species::Chesnaught: return "Chesnaught"_sv;
		case Species::Fennekin: return "Fennekin"_sv;
		case Species::Braixen: return "Braixen"_sv;
		case Species::Delphox: return "Delphox"_sv;
		case Species::Froakie: return "Froakie"_sv;
		case Species::Frogadier: return "Frogadier"_sv;
		case Species::Greninja: return "Greninja"_sv;
		case Species::Bunnelby: return "Bunnelby"_sv;
		case Species::Diggersby: return "Diggersby"_sv;
		case Species::Fletchling: return "Fletchling"_sv;
		case Species::Fletchinder: return "Fletchinder"_sv;
		case Species::Talonflame: return "Talonflame"_sv;
		case Species::Scatterbug: return "Scatterbug"_sv;
		case Species::Spewpa: return "Spewpa"_sv;
		case Species::Vivillon: return "Vivillon"_sv;
		case Species::Litleo: return "Litleo"_sv;
		case Species::Pyroar: return "Pyroar"_sv;
		case Species::Flabebe: return "Flabebe"_sv;
		case Species::Floette: return "Floette"_sv;
		case Species::Florges: return "Florges"_sv;
		case Species::Skiddo: return "Skiddo"_sv;
		case Species::Gogoat: return "Gogoat"_sv;
		case Species::Pancham: return "Pancham"_sv;
		case Species::Pangoro: return "Pangoro"_sv;
		case Species::Furfrou: return "Furfrou"_sv;
		case Species::Espurr: return "Espurr"_sv;
		case Species::Meowstic_F: return "Meowstic-F"_sv;
		case Species::Meowstic_M: return "Meowstic-M"_sv;
		case Species::Honedge: return "Honedge"_sv;
		case Species::Doublade: return "Doublade"_sv;
		case Species::Aegislash: return "Aegislash"_sv;
		case Species::Spritzee: return "Spritzee"_sv;
		case Species::Aromatisse: return "Aromatisse"_sv;
		case Species::Swirlix: return "Swirlix"_sv;
		case Species::Slurpuff: return "Slurpuff"_sv;
		case Species::Inkay: return "Inkay"_sv;
		case Species::Malamar: return "Malamar"_sv;
		case Species::Binacle: return "Binacle"_sv;
		case Species::Barbaracle: return "Barbaracle"_sv;
		case Species::Skrelp: return "Skrelp"_sv;
		case Species::Dragalge: return "Dragalge"_sv;
		case Species::Clauncher: return "Clauncher"_sv;
		case Species::Clawitzer: return "Clawitzer"_sv;
		case Species::Helioptile: return "Helioptile"_sv;
		case Species::Heliolisk: return "Heliolisk"_sv;
		case Species::Tyrunt: return "Tyrunt"_sv;
		case Species::Tyrantrum: return "Tyrantrum"_sv;
		case Species::Amaura: return "Amaura"_sv;
		case Species::Aurorus: return "Aurorus"_sv;
		case Species::Sylveon: return "Sylveon"_sv;
		case Species::Hawlucha: return "Hawlucha"_sv;
		case Species::Dedenne: return "Dedenne"_sv;
		case Species::Carbink: return "Carbink"_sv;
		case Species::Goomy: return "Goomy"_sv;
		case Species::Sliggoo: return "Sliggoo"_sv;
		case Species::Goodra: return "Goodra"_sv;
		case Species::Klefki: return "Klefki"_sv;
		case Species::Phantump: return "Phantump"_sv;
		case Species::Trevenant: return "Trevenant"_sv;
		case Species::Pumpkaboo_Small: return "Pumpkaboo-Small"_sv;
		case Species::Pumpkaboo_Average: return "Pumpkaboo-Average"_sv;
		case Species::Pumpkaboo_Large: return "Pumpkaboo-Large"_sv;
		case Species::Pumpkaboo_Super: return "Pumpkaboo-Super"_sv;
		case Species::Gourgeist_Small: return "Gourgeist-Small"_sv;
		case Species::Gourgeist_Average: return "Gourgeist-Average"_sv;
		case Species::Gourgeist_Large: return "Gourgeist-Large"_sv;
		case Species::Gourgeist_Super: return "Gourgeist-Super"_sv;
		case Species::Bergmite: return "Bergmite"_sv;
		case Species::Avalugg: return "Avalugg"_sv;
		case Species::Noibat: return "Noibat"_sv;
		case Species::Noivern: return "Noivern"_sv;
		case Species::Xerneas: return "Xerneas"_sv;
		case Species::Yveltal: return "Yveltal"_sv;
		case Species::Zygarde_50: return "Zygarde-50%"_sv;
		case Species::Zygarde_10: return "Zygarde-10%"_sv;
		case Species::Zygarde_Complete: return "Zygarde-Complete"_sv;
		case Species::Diancie: return "Diancie"_sv;
		case Species::Hoopa_Confined: return "Hoopa-Confined"_sv;
		case Species::Hoopa_Unbound: return "Hoopa-Unbound"_sv;
		case Species::Volcanion: return "Volcanion"_sv;

		// Generation 7
		case Species::Rowlet: return "Rowlet"_sv;
		case Species::Dartrix: return "Dartrix"_sv;
		case Species::Decidueye: return "Decidueye"_sv;
		case Species::Litten: return "Litten"_sv;
		case Species::Torracat: return "Torracat"_sv;
		case Species::Incineroar: return "Incineroar"_sv;
		case Species::Popplio: return "Popplio"_sv;
		case Species::Brionne: return "Brionne"_sv;
		case Species::Primarina: return "Primarina"_sv;
		case Species::Pikipek: return "Pikipek"_sv;
		case Species::Trumbeak: return "Trumbeak"_sv;
		case Species::Toucannon: return "Toucannon"_sv;
		case Species::Yungoos: return "Yungoos"_sv;
		case Species::Gumshoos: return "Gumshoos"_sv;
		case Species::Grubbin: return "Grubbin"_sv;
		case Species::Charjabug: return "Charjabug"_sv;
		case Species::Vikavolt: return "Vikavolt"_sv;
		case Species::Crabrawler: return "Crabrawler"_sv;
		case Species::Crabominable: return "Crabominable"_sv;
		case Species::Oricorio_Baile: return "Oricorio-Baile"_sv;
		case Species::Oricorio_Pau: return "Oricorio-Pa'u"_sv;
		case Species::Oricorio_Pom_Pom: return "Oricorio-Pom-Pom"_sv;
		case Species::Oricorio_Sensu: return "Oricorio-Sensu"_sv;
		case Species::Cutiefly: return "Cutiefly"_sv;
		case Species::Ribombee: return "Ribombee"_sv;
		case Species::Rockruff: return "Rockruff"_sv;
		case Species::Lycanroc_Midday: return "Lycanroc-Midday"_sv;
		case Species::Lycanroc_Midnight: return "Lycanroc-Midnight"_sv;
		case Species::Lycanroc_Dusk: return "Lycanroc-Dusk"_sv;
		case Species::Wishiwashi_Solo: return "Wishiwashi-Solo"_sv;
		case Species::Wishiwashi_School: return "Wishiwashi-School"_sv;
		case Species::Mareanie: return "Mareanie"_sv;
		case Species::Toxapex: return "Toxapex"_sv;
		case Species::Mudbray: return "Mudbray"_sv;
		case Species::Mudsdale: return "Mudsdale"_sv;
		case Species::Dewpider: return "Dewpider"_sv;
		case Species::Araquanid: return "Araquanid"_sv;
		case Species::Fomantis: return "Fomantis"_sv;
		case Species::Lurantis: return "Lurantis"_sv;
		case Species::Morelull: return "Morelull"_sv;
		case Species::Shiinotic: return "Shiinotic"_sv;
		case Species::Salandit: return "Salandit"_sv;
		case Species::Salazzle: return "Salazzle"_sv;
		case Species::Stufful: return "Stufful"_sv;
		case Species::Bewear: return "Bewear"_sv;
		case Species::Bounsweet: return "Bounsweet"_sv;
		case Species::Steenee: return "Steenee"_sv;
		case Species::Tsareena: return "Tsareena"_sv;
		case Species::Comfey: return "Comfey"_sv;
		case Species::Oranguru: return "Oranguru"_sv;
		case Species::Passimian: return "Passimian"_sv;
		case Species::Wimpod: return "Wimpod"_sv;
		case Species::Golisopod: return "Golisopod"_sv;
		case Species::Sandygast: return "Sandygast"_sv;
		case Species::Palossand: return "Palossand"_sv;
		case Species::Pyukumuku: return "Pyukumuku"_sv;
		case Species::Type_Null: return "Type: Null"_sv;
		case Species::Silvally: return "Silvally"_sv;
		case Species::Minior_Meteor: return "Minior-Meteor"_sv;
		case Species::Minior_Core: return "Minior-Core"_sv;
		case Species::Komala: return "Komala"_sv;
		case Species::Turtonator: return "Turtonator"_sv;
		case Species::Togedemaru: return "Togedemaru"_sv;
		case Species::Mimikyu: return "Mimikyu"_sv;
		case Species::Bruxish: return "Bruxish"_sv;
		case Species::Drampa: return "Drampa"_sv;
		case Species::Dhelmise: return "Dhelmise"_sv;
		case Species::Jangmo_o: return "Jangmo-o"_sv;
		case Species::Hakamo_o: return "Hakamo-o"_sv;
		case Species::Kommo_o: return "Kommo-o"_sv;
		case Species::Tapu_Koko: return "Tapu Koko"_sv;
		case Species::Tapu_Lele: return "Tapu Lele"_sv;
		case Species::Tapu_Bulu: return "Tapu Bulu"_sv;
		case Species::Tapu_Fini: return "Tapu Fini"_sv;
		case Species::Cosmog: return "Cosmog"_sv;
		case Species::Cosmoem: return "Cosmoem"_sv;
		case Species::Solgaleo: return "Solgaleo"_sv;
		case Species::Lunala: return "Lunala"_sv;
		case Species::Nihilego: return "Nihilego"_sv;
		case Species::Buzzwole: return "Buzzwole"_sv;
		case Species::Pheromosa: return "Pheromosa"_sv;
		case Species::Xurkitree: return "Xurkitree"_sv;
		case Species::Celesteela: return "Celesteela"_sv;
		case Species::Kartana: return "Kartana"_sv;
		case Species::Guzzlord: return "Guzzlord"_sv;
		case Species::Necrozma: return "Necrozma"_sv;
		case Species::Necrozma_Dusk_Mane: return "Necrozma-Dusk Mane"_sv;
		case Species::Necrozma_Dawn_Wings: return "Necrozma-Dawn Wings"_sv;
		case Species::Necrozma_Ultra: return "Necrozma-Ultra"_sv;
		case Species::Magearna: return "Magearna"_sv;
		case Species::Marshadow: return "Marshadow"_sv;
		case Species::Poipole: return "Poipole"_sv;
		case Species::Naganadel: return "Naganadel"_sv;
		case Species::Stakataka: return "Stakataka"_sv;
		case Species::Blacephalon: return "Blacephalon"_sv;
		case Species::Zeraora: return "Zeraora"_sv;
		case Species::Meltan: return "Meltan"_sv;
		case Species::Melmetal: return "Melmetal"_sv;
		case Species::Rattata_Alola: return "Rattata-Alola"_sv;
		case Species::Raticate_Alola: return "Raticate-Alola"_sv;
		case Species::Raichu_Alola: return "Raichu-Alola"_sv;
		case Species::Sandshrew_Alola: return "Sandshrew-Alola"_sv;
		case Species::Sandslash_Alola: return "Sandslash-Alola"_sv;
		case Species::Vulpix_Alola: return "Vulpix-Alola"_sv;
		case Species::Ninetales_Alola: return "Ninetales-Alola"_sv;
		case Species::Diglett_Alola: return "Diglett-Alola"_sv;
		case Species::Dugtrio_Alola: return "Dugtrio-Alola"_sv;
		case Species::Meowth_Alola: return "Meowth-Alola"_sv;
		case Species::Persian_Alola: return "Persian-Alola"_sv;
		case Species::Geodude_Alola: return "Geodude-Alola"_sv;
		case Species::Graveler_Alola: return "Graveler-Alola"_sv;
		case Species::Golem_Alola: return "Golem-Alola"_sv;
		case Species::Grimer_Alola: return "Grimer-Alola"_sv;
		case Species::Muk_Alola: return "Muk-Alola"_sv;
		case Species::Exeggutor_Alola: return "Exeggutor-Alola"_sv;
		case Species::Marowak_Alola: return "Marowak-Alola"_sv;

		// Generation 8
		case Species::Grookey: return "Grookey"_sv;
		case Species::Thwackey: return "Thwackey"_sv;
		case Species::Rillaboom: return "Rillaboom"_sv;
		case Species::Scorbunny: return "Scorbunny"_sv;
		case Species::Raboot: return "Raboot"_sv;
		case Species::Cinderace: return "Cinderace"_sv;
		case Species::Sobble: return "Sobble"_sv;
		case Species::Drizzile: return "Drizzile"_sv;
		case Species::Inteleon: return "Inteleon"_sv;
		case Species::Skwovet: return "Skwovet"_sv;
		case Species::Greedent: return "Greedent"_sv;
		case Species::Rookidee: return "Rookidee"_sv;
		case Species::Corvisquire: return "Corvisquire"_sv;
		case Species::Corviknight: return "Corviknight"_sv;
		case Species::Blipbug: return "Blipbug"_sv;
		case Species::Dottler: return "Dottler"_sv;
		case Species::Orbeetle: return "Orbeetle"_sv;
		case Species::Nickit: return "Nickit"_sv;
		case Species::Thievul: return "Thievul"_sv;
		case Species::Gossifleur: return "Gossifleur"_sv;
		case Species::Eldegoss: return "Eldegoss"_sv;
		case Species::Wooloo: return "Wooloo"_sv;
		case Species::Dubwool: return "Dubwool"_sv;
		case Species::Chewtle: return "Chewtle"_sv;
		case Species::Drednaw: return "Drednaw"_sv;
		case Species::Yamper: return "Yamper"_sv;
		case Species::Boltund: return "Boltund"_sv;
		case Species::Rolycoly: return "Rolycoly"_sv;
		case Species::Carkol: return "Carkol"_sv;
		case Species::Coalossal: return "Coalossal"_sv;
		case Species::Applin: return "Applin"_sv;
		case Species::Flapple: return "Flapple"_sv;
		case Species::Appletun: return "Appletun"_sv;
		case Species::Silicobra: return "Silicobra"_sv;
		case Species::Sandaconda: return "Sandaconda"_sv;
		case Species::Cramorant: return "Cramorant"_sv;
		case Species::Arrokuda: return "Arrokuda"_sv;
		case Species::Barraskewda: return "Barraskewda"_sv;
		case Species::Toxel: return "Toxel"_sv;
		case Species::Toxtricity_Amped: return "Toxtricity Amped"_sv;
		case Species::Toxtricity_Low_Key: return "Toxtricity Low Key"_sv;
		case Species::Sizzlipede: return "Sizzlipede"_sv;
		case Species::Centiskorch: return "Centiskorch"_sv;
		case Species::Clobbopus: return "Clobbopus"_sv;
		case Species::Grapploct: return "Grapploct"_sv;
		case Species::Sinistea: return "Sinistea"_sv;
		case Species::Polteageist: return "Polteageist"_sv;
		case Species::Hatenna: return "Hatenna"_sv;
		case Species::Hattrem: return "Hattrem"_sv;
		case Species::Hatterene: return "Hatterene"_sv;
		case Species::Impidimp: return "Impidimp"_sv;
		case Species::Morgrem: return "Morgrem"_sv;
		case Species::Grimmsnarl: return "Grimmsnarl"_sv;
		case Species::Obstagoon: return "Obstagoon"_sv;
		case Species::Perrserker: return "Perrserker"_sv;
		case Species::Cursola: return "Cursola"_sv;
		case Species::Sirfetchd: return "Sirfetch'd"_sv;
		case Species::Mr_Rime: return "Mr. Rime"_sv;
		case Species::Runerigus: return "Runerigus"_sv;
		case Species::Milcery: return "Milcery"_sv;
		case Species::Alcremie: return "Alcremie"_sv;
		case Species::Falinks: return "Falinks"_sv;
		case Species::Pincurchin: return "Pincurchin"_sv;
		case Species::Snom: return "Snom"_sv;
		case Species::Frosmoth: return "Frosmoth"_sv;
		case Species::Stonjourner: return "Stonjourner"_sv;
		case Species::Eiscue: return "Eiscue"_sv;
		case Species::Indeedee_F: return "Indeedee-F"_sv;
		case Species::Indeedee_M: return "Indeedee-M"_sv;
		case Species::Morpeko: return "Morpeko"_sv;
		case Species::Cufant: return "Cufant"_sv;
		case Species::Copperajah: return "Copperajah"_sv;
		case Species::Dracozolt: return "Dracozolt"_sv;
		case Species::Arctozolt: return "Arctozolt"_sv;
		case Species::Dracovish: return "Dracovish"_sv;
		case Species::Arctovish: return "Arctovish"_sv;
		case Species::Duraludon: return "Duraludon"_sv;
		case Species::Dreepy: return "Dreepy"_sv;
		case Species::Drakloak: return "Drakloak"_sv;
		case Species::Dragapult: return "Dragapult"_sv;
		case Species::Zacian: return "Zacian"_sv;
		case Species::Zacian_Crowned_Sword: return "Zacian Crowned Sword"_sv;
		case Species::Zamazenta: return "Zamazenta"_sv;
		case Species::Zamazenta_Crowned_Shield: return "Zamazenta Crowned Shield"_sv;
		case Species::Eternatus: return "Eternatus"_sv;
		case Species::Kubfu: return "Kubfu"_sv;
		case Species::Urshifu_Single_Strike: return "Urshifu Single Strike"_sv;
		case Species::Urshifu_Rapid_Strike: return "Urshifu Rapid Strike"_sv;
		case Species::Zarude: return "Zarude"_sv;
		case Species::Regieleki: return "Regieleki"_sv;
		case Species::Regidrago: return "Regidrago"_sv;
		case Species::Glastrier: return "Glastrier"_sv;
		case Species::Spectrier: return "Spectrier"_sv;
		case Species::Calyrex: return "Calyrex"_sv;
		case Species::Calyrex_Ice_Rider: return "Calyrex Ice Rider"_sv;
		case Species::Calyrex_Shadow_Rider: return "Calyrex Shadow Rider"_sv;
		case Species::Meowth_Galar: return "Meowth-Galar"_sv;
		case Species::Ponyta_Galar: return "Ponyta-Galar"_sv;
		case Species::Rapidash_Galar: return "Rapidash-Galar"_sv;
		case Species::Farfetchd_Galar: return "Farfetch'd-Galar"_sv;
		case Species::Weezing_Galar: return "Weezing-Galar"_sv;
		case Species::Mr_Mime_Galar: return "Mr. Mime-Galar"_sv;
		case Species::Corsola_Galar: return "Corsola-Galar"_sv;
		case Species::Zigzagoon_Galar: return "Zigzagoon-Galar"_sv;
		case Species::Linoone_Galar: return "Linoone-Galar"_sv;
		case Species::Darumaka_Galar: return "Darumaka-Galar"_sv;
		case Species::Darmanitan_Galar: return "Darmanitan-Galar"_sv;
		case Species::Yamask_Galar: return "Yamask-Galar"_sv;
		case Species::Stunfisk_Galar: return "Stunfisk-Galar"_sv;
		case Species::Slowpoke_Galar: return "Slowpoke-Galar"_sv;
		case Species::Slowbro_Galar: return "Slowbro-Galar"_sv;
		case Species::Articuno_Galar: return "Articuno-Galar"_sv;
		case Species::Zapdos_Galar: return "Zapdos-Galar"_sv;
		case Species::Moltres_Galar: return "Moltres-Galar"_sv;
		case Species::Slowking_Galar: return "Slowking-Galar"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Species {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Species>>({
			{"abomasnow"_sv, Species::Abomasnow},
			{"abomasnowmega"_sv, Species::Abomasnow},
			{"abra"_sv, Species::Abra},
			{"absol"_sv, Species::Absol},
			{"absolmega"_sv, Species::Absol},
			{"accelgor"_sv, Species::Accelgor},
			{"aegislash"_sv, Species::Aegislash},
			{"aerodactyl"_sv, Species::Aerodactyl},
			{"aerodactylmega"_sv, Species::Aerodactyl},
			{"aggron"_sv, Species::Aggron},
			{"aggronmega"_sv, Species::Aggron},
			{"aipom"_sv, Species::Aipom},
			{"alakazam"_sv, Species::Alakazam},
			{"alakazammega"_sv, Species::Alakazam},
			{"alcremie"_sv, Species::Alcremie},
			{"alcremiecaramel"_sv, Species::Alcremie},
			{"alcremielemon"_sv, Species::Alcremie},
			{"alcremiematcha"_sv, Species::Alcremie},
			{"alcremiemint"_sv, Species::Alcremie},
			{"alcremierainbow"_sv, Species::Alcremie},
			{"alcremierubycream"_sv, Species::Alcremie},
			{"alcremierubyswirl"_sv, Species::Alcremie},
			{"alcremiesalted"_sv, Species::Alcremie},
			{"alcremievanilla"_sv, Species::Alcremie},
			{"alomomola"_sv, Species::Alomomola},
			{"altaria"_sv, Species::Altaria},
			{"altariamega"_sv, Species::Altaria},
			{"amaura"_sv, Species::Amaura},
			{"ambipom"_sv, Species::Ambipom},
			{"amoonguss"_sv, Species::Amoonguss},
			{"ampharos"_sv, Species::Ampharos},
			{"ampharosmega"_sv, Species::Ampharos},
			{"anorith"_sv, Species::Anorith},
			{"appletun"_sv, Species::Appletun},
			{"applin"_sv, Species::Applin},
			{"araquanid"_sv, Species::Araquanid},
			{"araquanidtotem"_sv, Species::Araquanid},
			{"arbok"_sv, Species::Arbok},
			{"arcanine"_sv, Species::Arcanine},
			{"arceus"_sv, Species::Arceus},
			{"arceusbug"_sv, Species::Arceus},
			{"arceusdark"_sv, Species::Arceus},
			{"arceusdragon"_sv, Species::Arceus},
			{"arceuselectric"_sv, Species::Arceus},
			{"arceusfighting"_sv, Species::Arceus},
			{"arceusfire"_sv, Species::Arceus},
			{"arceusflying"_sv, Species::Arceus},
			{"arceusghost"_sv, Species::Arceus},
			{"arceusgrass"_sv, Species::Arceus},
			{"arceusground"_sv, Species::Arceus},
			{"arceusice"_sv, Species::Arceus},
			{"arceusnormal"_sv, Species::Arceus},
			{"arceuspoison"_sv, Species::Arceus},
			{"arceuspsychic"_sv, Species::Arceus},
			{"arceusrock"_sv, Species::Arceus},
			{"arceussteel"_sv, Species::Arceus},
			{"arceuswater"_sv, Species::Arceus},
			{"archen"_sv, Species::Archen},
			{"archeops"_sv, Species::Archeops},
			{"arctovish"_sv, Species::Arctovish},
			{"arctozolt"_sv, Species::Arctozolt},
			{"ariados"_sv, Species::Ariados},
			{"armaldo"_sv, Species::Armaldo},
			{"aromatisse"_sv, Species::Aromatisse},
			{"aron"_sv, Species::Aron},
			{"arrokuda"_sv, Species::Arrokuda},
			{"articuno"_sv, Species::Articuno},
			{"articunogalar"_sv, Species::Articuno_Galar},
			{"audino"_sv, Species::Audino},
			{"audinomega"_sv, Species::Audino},
			{"aurorus"_sv, Species::Aurorus},
			{"avalugg"_sv, Species::Avalugg},
			{"axew"_sv, Species::Axew},
			{"azelf"_sv, Species::Azelf},
			{"azumarill"_sv, Species::Azumarill},
			{"azurill"_sv, Species::Azurill},
			{"bagon"_sv, Species::Bagon},
			{"baltoy"_sv, Species::Baltoy},
			{"banette"_sv, Species::Banette},
			{"banettemega"_sv, Species::Banette},
			{"barbaracle"_sv, Species::Barbaracle},
			{"barboach"_sv, Species::Barboach},
			{"barraskewda"_sv, Species::Barraskewda},
			{"basculin"_sv, Species::Basculin},
			{"basculinbluestriped"_sv, Species::Basculin},
			{"bastiodon"_sv, Species::Bastiodon},
			{"bayleef"_sv, Species::Bayleef},
			{"beartic"_sv, Species::Beartic},
			{"beautifly"_sv, Species::Beautifly},
			{"beedrill"_sv, Species::Beedrill},
			{"beedrillmega"_sv, Species::Beedrill},
			{"beheeyem"_sv, Species::Beheeyem},
			{"beldum"_sv, Species::Beldum},
			{"bellossom"_sv, Species::Bellossom},
			{"bellsprout"_sv, Species::Bellsprout},
			{"bergmite"_sv, Species::Bergmite},
			{"bewear"_sv, Species::Bewear},
			{"bibarel"_sv, Species::Bibarel},
			{"bidoof"_sv, Species::Bidoof},
			{"binacle"_sv, Species::Binacle},
			{"bisharp"_sv, Species::Bisharp},
			{"blacephalon"_sv, Species::Blacephalon},
			{"blastoise"_sv, Species::Blastoise},
			{"blastoisemega"_sv, Species::Blastoise},
			{"blaziken"_sv, Species::Blaziken},
			{"blipbug"_sv, Species::Blipbug},
			{"blissey"_sv, Species::Blissey},
			{"blitzle"_sv, Species::Blitzle},
			{"boldore"_sv, Species::Boldore},
			{"boltund"_sv, Species::Boltund},
			{"bonsly"_sv, Species::Bonsly},
			{"bouffalant"_sv, Species::Bouffalant},
			{"bounsweet"_sv, Species::Bounsweet},
			{"braixen"_sv, Species::Braixen},
			{"braviary"_sv, Species::Braviary},
			{"breloom"_sv, Species::Breloom},
			{"brionne"_sv, Species::Brionne},
			{"bronzong"_sv, Species::Bronzong},
			{"bronzor"_sv, Species::Bronzor},
			{"bruxish"_sv, Species::Bruxish},
			{"budew"_sv, Species::Budew},
			{"buizel"_sv, Species::Buizel},
			{"bulbasaur"_sv, Species::Bulbasaur},
			{"buneary"_sv, Species::Buneary},
			{"bunnelby"_sv, Species::Bunnelby},
			{"burmy"_sv, Species::Burmy},
			{"butterfree"_sv, Species::Butterfree},
			{"buzzwole"_sv, Species::Buzzwole},
			{"cacnea"_sv, Species::Cacnea},
			{"cacturne"_sv, Species::Cacturne},
			{"calyrex"_sv, Species::Calyrex},
			{"calyrexicerider"_sv, Species::Calyrex_Ice_Rider},
			{"calyrexshadowrider"_sv, Species::Calyrex_Shadow_Rider},
			{"camerupt"_sv, Species::Camerupt},
			{"cameruptmega"_sv, Species::Camerupt},
			{"carbink"_sv, Species::Carbink},
			{"carkol"_sv, Species::Carkol},
			{"carnivine"_sv, Species::Carnivine},
			{"carracosta"_sv, Species::Carracosta},
			{"carvanha"_sv, Species::Carvanha},
			{"cascoon"_sv, Species::Cascoon},
			{"castform"_sv, Species::Castform},
			{"caterpie"_sv, Species::Caterpie},
			{"celebi"_sv, Species::Celebi},
			{"celesteela"_sv, Species::Celesteela},
			{"centiskorch"_sv, Species::Centiskorch},
			{"chandelure"_sv, Species::Chandelure},
			{"chansey"_sv, Species::Chansey},
			{"charizard"_sv, Species::Charizard},
			{"charizardmegax"_sv, Species::Charizard},
			{"charizardmegay"_sv, Species::Charizard},
			{"charjabug"_sv, Species::Charjabug},
			{"charmander"_sv, Species::Charmander},
			{"charmeleon"_sv, Species::Charmeleon},
			{"chatot"_sv, Species::Chatot},
			{"cherrim"_sv, Species::Cherrim},
			{"cherubi"_sv, Species::Cherubi},
			{"chesnaught"_sv, Species::Chesnaught},
			{"chespin"_sv, Species::Chespin},
			{"chewtle"_sv, Species::Chewtle},
			{"chikorita"_sv, Species::Chikorita},
			{"chimchar"_sv, Species::Chimchar},
			{"chimecho"_sv, Species::Chimecho},
			{"chinchou"_sv, Species::Chinchou},
			{"chingling"_sv, Species::Chingling},
			{"cinccino"_sv, Species::Cinccino},
			{"cinderace"_sv, Species::Cinderace},
			{"clamperl"_sv, Species::Clamperl},
			{"clauncher"_sv, Species::Clauncher},
			{"clawitzer"_sv, Species::Clawitzer},
			{"claydol"_sv, Species::Claydol},
			{"clefable"_sv, Species::Clefable},
			{"clefairy"_sv, Species::Clefairy},
			{"cleffa"_sv, Species::Cleffa},
			{"clobbopus"_sv, Species::Clobbopus},
			{"cloyster"_sv, Species::Cloyster},
			{"coalossal"_sv, Species::Coalossal},
			{"cobalion"_sv, Species::Cobalion},
			{"cofagrigus"_sv, Species::Cofagrigus},
			{"combee"_sv, Species::Combee},
			{"combusken"_sv, Species::Combusken},
			{"comfey"_sv, Species::Comfey},
			{"conkeldurr"_sv, Species::Conkeldurr},
			{"copperajah"_sv, Species::Copperajah},
			{"corphish"_sv, Species::Corphish},
			{"corsola"_sv, Species::Corsola},
			{"corsolagalar"_sv, Species::Corsola_Galar},
			{"corviknight"_sv, Species::Corviknight},
			{"corvisquire"_sv, Species::Corvisquire},
			{"cosmoem"_sv, Species::Cosmoem},
			{"cosmog"_sv, Species::Cosmog},
			{"cottonee"_sv, Species::Cottonee},
			{"crabominable"_sv, Species::Crabominable},
			{"crabrawler"_sv, Species::Crabrawler},
			{"cradily"_sv, Species::Cradily},
			{"cramorant"_sv, Species::Cramorant},
			{"cranidos"_sv, Species::Cranidos},
			{"crawdaunt"_sv, Species::Crawdaunt},
			{"cresselia"_sv, Species::Cresselia},
			{"croagunk"_sv, Species::Croagunk},
			{"crobat"_sv, Species::Crobat},
			{"croconaw"_sv, Species::Croconaw},
			{"crustle"_sv, Species::Crustle},
			{"cryogonal"_sv, Species::Cryogonal},
			{"cubchoo"_sv, Species::Cubchoo},
			{"cubone"_sv, Species::Cubone},
			{"cufant"_sv, Species::Cufant},
			{"cursola"_sv, Species::Cursola},
			{"cutiefly"_sv, Species::Cutiefly},
			{"cyndaquil"_sv, Species::Cyndaquil},
			{"darkrai"_sv, Species::Darkrai},
			{"darmanitan"_sv, Species::Darmanitan},
			{"darmanitangalar"_sv, Species::Darmanitan_Galar},
			{"dartrix"_sv, Species::Dartrix},
			{"darumaka"_sv, Species::Darumaka},
			{"darumakagalar"_sv, Species::Darumaka_Galar},
			{"decidueye"_sv, Species::Decidueye},
			{"dedenne"_sv, Species::Dedenne},
			{"deerling"_sv, Species::Deerling},
			{"deino"_sv, Species::Deino},
			{"delcatty"_sv, Species::Delcatty},
			{"delibird"_sv, Species::Delibird},
			{"delphox"_sv, Species::Delphox},
			{"deoxys"_sv, Species::Deoxys_Normal},
			{"deoxysattack"_sv, Species::Deoxys_Attack},
			{"deoxysdefense"_sv, Species::Deoxys_Defense},
			{"deoxyse"_sv, Species::Deoxys_Speed},
			{"deoxysf"_sv, Species::Deoxys_Attack},
			{"deoxysl"_sv, Species::Deoxys_Defense},
			{"deoxysnormal"_sv, Species::Deoxys_Normal},
			{"deoxysspeed"_sv, Species::Deoxys_Speed},
			{"dewgong"_sv, Species::Dewgong},
			{"dewott"_sv, Species::Dewott},
			{"dewpider"_sv, Species::Dewpider},
			{"dhelmise"_sv, Species::Dhelmise},
			{"dialga"_sv, Species::Dialga},
			{"diancie"_sv, Species::Diancie},
			{"dianciemega"_sv, Species::Diancie},
			{"diggersby"_sv, Species::Diggersby},
			{"diglett"_sv, Species::Diglett},
			{"diglettalola"_sv, Species::Diglett_Alola},
			{"ditto"_sv, Species::Ditto},
			{"dodrio"_sv, Species::Dodrio},
			{"doduo"_sv, Species::Doduo},
			{"donphan"_sv, Species::Donphan},
			{"dottler"_sv, Species::Dottler},
			{"doublade"_sv, Species::Doublade},
			{"dracovish"_sv, Species::Dracovish},
			{"dracozolt"_sv, Species::Dracozolt},
			{"dragalge"_sv, Species::Dragalge},
			{"dragapult"_sv, Species::Dragapult},
			{"dragonair"_sv, Species::Dragonair},
			{"dragonite"_sv, Species::Dragonite},
			{"drakloak"_sv, Species::Drakloak},
			{"drampa"_sv, Species::Drampa},
			{"drapion"_sv, Species::Drapion},
			{"dratini"_sv, Species::Dratini},
			{"drednaw"_sv, Species::Drednaw},
			{"dreepy"_sv, Species::Dreepy},
			{"drifblim"_sv, Species::Drifblim},
			{"drifloon"_sv, Species::Drifloon},
			{"drilbur"_sv, Species::Drilbur},
			{"drizzile"_sv, Species::Drizzile},
			{"drowzee"_sv, Species::Drowzee},
			{"druddigon"_sv, Species::Druddigon},
			{"dubwool"_sv, Species::Dubwool},
			{"ducklett"_sv, Species::Ducklett},
			{"dugtrio"_sv, Species::Dugtrio},
			{"dugtrioalola"_sv, Species::Dugtrio_Alola},
			{"dunsparce"_sv, Species::Dunsparce},
			{"duosion"_sv, Species::Duosion},
			{"duraludon"_sv, Species::Duraludon},
			{"durant"_sv, Species::Durant},
			{"dusclops"_sv, Species::Dusclops},
			{"dusknoir"_sv, Species::Dusknoir},
			{"duskull"_sv, Species::Duskull},
			{"dustox"_sv, Species::Dustox},
			{"dwebble"_sv, Species::Dwebble},
			{"eelektrik"_sv, Species::Eelektrik},
			{"eelektross"_sv, Species::Eelektross},
			{"eevee"_sv, Species::Eevee},
			{"eiscue"_sv, Species::Eiscue},
			{"ekans"_sv, Species::Ekans},
			{"eldegoss"_sv, Species::Eldegoss},
			{"electabuzz"_sv, Species::Electabuzz},
			{"electivire"_sv, Species::Electivire},
			{"electrike"_sv, Species::Electrike},
			{"electrode"_sv, Species::Electrode},
			{"elekid"_sv, Species::Elekid},
			{"elgyem"_sv, Species::Elgyem},
			{"emboar"_sv, Species::Emboar},
			{"emolga"_sv, Species::Emolga},
			{"empoleon"_sv, Species::Empoleon},
			{"entei"_sv, Species::Entei},
			{"escavalier"_sv, Species::Escavalier},
			{"espeon"_sv, Species::Espeon},
			{"espurr"_sv, Species::Espurr},
			{"eternatus"_sv, Species::Eternatus},
			{"excadrill"_sv, Species::Excadrill},
			{"exeggcute"_sv, Species::Exeggcute},
			{"exeggutor"_sv, Species::Exeggutor},
			{"exeggutoralola"_sv, Species::Exeggutor_Alola},
			{"exploud"_sv, Species::Exploud},
			{"falinks"_sv, Species::Falinks},
			{"farfetchd"_sv, Species::Farfetchd},
			{"farfetchdgalar"_sv, Species::Farfetchd_Galar},
			{"fearow"_sv, Species::Fearow},
			{"feebas"_sv, Species::Feebas},
			{"fennekin"_sv, Species::Fennekin},
			{"feraligatr"_sv, Species::Feraligatr},
			{"ferroseed"_sv, Species::Ferroseed},
			{"ferrothorn"_sv, Species::Ferrothorn},
			{"finneon"_sv, Species::Finneon},
			{"flaaffy"_sv, Species::Flaaffy},
			{"flabebe"_sv, Species::Flabebe},
			{"flapple"_sv, Species::Flapple},
			{"flareon"_sv, Species::Flareon},
			{"fletchinder"_sv, Species::Fletchinder},
			{"fletchling"_sv, Species::Fletchling},
			{"floatzel"_sv, Species::Floatzel},
			{"floette"_sv, Species::Floette},
			{"florges"_sv, Species::Florges},
			{"flygon"_sv, Species::Flygon},
			{"fomantis"_sv, Species::Fomantis},
			{"foongus"_sv, Species::Foongus},
			{"forretress"_sv, Species::Forretress},
			{"fraxure"_sv, Species::Fraxure},
			{"frillish"_sv, Species::Frillish},
			{"froakie"_sv, Species::Froakie},
			{"frogadier"_sv, Species::Frogadier},
			{"froslass"_sv, Species::Froslass},
			{"frosmoth"_sv, Species::Frosmoth},
			{"furfrou"_sv, Species::Furfrou},
			{"furret"_sv, Species::Furret},
			{"gabite"_sv, Species::Gabite},
			{"gallade"_sv, Species::Gallade},
			{"gallademega"_sv, Species::Gallade},
			{"galvantula"_sv, Species::Galvantula},
			{"garbodor"_sv, Species::Garbodor},
			{"garchomp"_sv, Species::Garchomp},
			{"garchompmega"_sv, Species::Garchomp},
			{"gardevoir"_sv, Species::Gardevoir},
			{"gardevoirmega"_sv, Species::Gardevoir},
			{"gastly"_sv, Species::Gastly},
			{"gastrodon"_sv, Species::Gastrodon},
			{"gastrodoneast"_sv, Species::Gastrodon},
			{"gastrodonwest"_sv, Species::Gastrodon},
			{"genesect"_sv, Species::Genesect},
			{"genesectburn"_sv, Species::Genesect},
			{"genesectchill"_sv, Species::Genesect},
			{"genesectdouse"_sv, Species::Genesect},
			{"genesectshock"_sv, Species::Genesect},
			{"gengar"_sv, Species::Gengar},
			{"geodude"_sv, Species::Geodude},
			{"geodudealola"_sv, Species::Geodude_Alola},
			{"gible"_sv, Species::Gible},
			{"gigalith"_sv, Species::Gigalith},
			{"girafarig"_sv, Species::Girafarig},
			{"giratina"_sv, Species::Giratina_Altered},
			{"giratinaaltered"_sv, Species::Giratina_Altered},
			{"giratinaorigin"_sv, Species::Giratina_Origin},
			{"glaceon"_sv, Species::Glaceon},
			{"glalie"_sv, Species::Glalie},
			{"glaliemega"_sv, Species::Glalie},
			{"glameow"_sv, Species::Glameow},
			{"glastrier"_sv, Species::Glastrier},
			{"gligar"_sv, Species::Gligar},
			{"gliscor"_sv, Species::Gliscor},
			{"gloom"_sv, Species::Gloom},
			{"gogoat"_sv, Species::Gogoat},
			{"golbat"_sv, Species::Golbat},
			{"goldeen"_sv, Species::Goldeen},
			{"golduck"_sv, Species::Golduck},
			{"golem"_sv, Species::Golem},
			{"golemalola"_sv, Species::Golem_Alola},
			{"golett"_sv, Species::Golett},
			{"golisopod"_sv, Species::Golisopod},
			{"golurk"_sv, Species::Golurk},
			{"goodra"_sv, Species::Goodra},
			{"goomy"_sv, Species::Goomy},
			{"gorebyss"_sv, Species::Gorebyss},
			{"gossifleur"_sv, Species::Gossifleur},
			{"gothita"_sv, Species::Gothita},
			{"gothitelle"_sv, Species::Gothitelle},
			{"gothorita"_sv, Species::Gothorita},
			{"gourgeist"_sv, Species::Gourgeist_Average},
			{"gourgeistaverage"_sv, Species::Gourgeist_Average},
			{"gourgeistlarge"_sv, Species::Gourgeist_Large},
			{"gourgeistsmall"_sv, Species::Gourgeist_Small},
			{"gourgeistsuper"_sv, Species::Gourgeist_Super},
			{"granbull"_sv, Species::Granbull},
			{"grapploct"_sv, Species::Grapploct},
			{"graveler"_sv, Species::Graveler},
			{"graveleralola"_sv, Species::Graveler_Alola},
			{"greedent"_sv, Species::Greedent},
			{"greninja"_sv, Species::Greninja},
			{"greninjaash"_sv, Species::Greninja},
			{"grimer"_sv, Species::Grimer},
			{"grimeralola"_sv, Species::Grimer_Alola},
			{"grimmsnarl"_sv, Species::Grimmsnarl},
			{"grookey"_sv, Species::Grookey},
			{"grotle"_sv, Species::Grotle},
			{"groudon"_sv, Species::Groudon},
			{"grovyle"_sv, Species::Grovyle},
			{"growlithe"_sv, Species::Growlithe},
			{"grubbin"_sv, Species::Grubbin},
			{"grumpig"_sv, Species::Grumpig},
			{"gulpin"_sv, Species::Gulpin},
			{"gumshoos"_sv, Species::Gumshoos},
			{"gumshoostotem"_sv, Species::Gumshoos}, // TODO: ???
			{"gurdurr"_sv, Species::Gurdurr},
			{"guzzlord"_sv, Species::Guzzlord},
			{"gyarados"_sv, Species::Gyarados},
			{"gyaradosmega"_sv, Species::Gyarados},
			{"hakamoo"_sv, Species::Hakamo_o},
			{"happiny"_sv, Species::Happiny},
			{"hariyama"_sv, Species::Hariyama},
			{"hatenna"_sv, Species::Hatenna},
			{"hatterene"_sv, Species::Hatterene},
			{"hattrem"_sv, Species::Hattrem},
			{"haunter"_sv, Species::Haunter},
			{"hawlucha"_sv, Species::Hawlucha},
			{"haxorus"_sv, Species::Haxorus},
			{"heatmor"_sv, Species::Heatmor},
			{"heatran"_sv, Species::Heatran},
			{"heliolisk"_sv, Species::Heliolisk},
			{"helioptile"_sv, Species::Helioptile},
			{"heracross"_sv, Species::Heracross},
			{"heracrossmega"_sv, Species::Heracross},
			{"herdier"_sv, Species::Herdier},
			{"hippopotas"_sv, Species::Hippopotas},
			{"hippowdon"_sv, Species::Hippowdon},
			{"hitmonchan"_sv, Species::Hitmonchan},
			{"hitmonlee"_sv, Species::Hitmonlee},
			{"hitmontop"_sv, Species::Hitmontop},
			{"honchkrow"_sv, Species::Honchkrow},
			{"honedge"_sv, Species::Honedge},
			{"hooh"_sv, Species::Ho_Oh},
			{"hoopa"_sv, Species::Hoopa_Confined},
			{"hoopaconfined"_sv, Species::Hoopa_Confined},
			{"hoopaunbound"_sv, Species::Hoopa_Unbound},
			{"hoothoot"_sv, Species::Hoothoot},
			{"hoppip"_sv, Species::Hoppip},
			{"horsea"_sv, Species::Horsea},
			{"houndoom"_sv, Species::Houndoom},
			{"houndoommega"_sv, Species::Houndoom},
			{"houndour"_sv, Species::Houndour},
			{"huntail"_sv, Species::Huntail},
			{"hydreigon"_sv, Species::Hydreigon},
			{"hypno"_sv, Species::Hypno},
			{"igglybuff"_sv, Species::Igglybuff},
			{"illumise"_sv, Species::Illumise},
			{"impidimp"_sv, Species::Impidimp},
			{"incineroar"_sv, Species::Incineroar},
			{"indeedee"_sv, Species::Indeedee_M},
			{"indeedeef"_sv, Species::Indeedee_F},
			{"indeedeem"_sv, Species::Indeedee_M},
			{"infernape"_sv, Species::Infernape},
			{"inkay"_sv, Species::Inkay},
			{"inteleon"_sv, Species::Inteleon},
			{"ivysaur"_sv, Species::Ivysaur},
			{"jangmoo"_sv, Species::Jangmo_o},
			{"jellicent"_sv, Species::Jellicent},
			{"jigglypuff"_sv, Species::Jigglypuff},
			{"jirachi"_sv, Species::Jirachi},
			{"jolteon"_sv, Species::Jolteon},
			{"joltik"_sv, Species::Joltik},
			{"jumpluff"_sv, Species::Jumpluff},
			{"jynx"_sv, Species::Jynx},
			{"kabuto"_sv, Species::Kabuto},
			{"kabutops"_sv, Species::Kabutops},
			{"kadabra"_sv, Species::Kadabra},
			{"kakuna"_sv, Species::Kakuna},
			{"kangaskhan"_sv, Species::Kangaskhan},
			{"karrablast"_sv, Species::Karrablast},
			{"kartana"_sv, Species::Kartana},
			{"kecleon"_sv, Species::Kecleon},
			{"keldeo"_sv, Species::Keldeo},
			{"keldeoresolute"_sv, Species::Keldeo_Resolute},
			{"kingdra"_sv, Species::Kingdra},
			{"kingler"_sv, Species::Kingler},
			{"kirlia"_sv, Species::Kirlia},
			{"klang"_sv, Species::Klang},
			{"klefki"_sv, Species::Klefki},
			{"klink"_sv, Species::Klink},
			{"klinklang"_sv, Species::Klinklang},
			{"koffing"_sv, Species::Koffing},
			{"komala"_sv, Species::Komala},
			{"kommoo"_sv, Species::Kommo_o},
			{"kommoototem"_sv, Species::Kommo_o},
			{"krabby"_sv, Species::Krabby},
			{"kricketot"_sv, Species::Kricketot},
			{"kricketune"_sv, Species::Kricketune},
			{"krokorok"_sv, Species::Krokorok},
			{"krookodile"_sv, Species::Krookodile},
			{"kubfu"_sv, Species::Kubfu},
			{"kyogre"_sv, Species::Kyogre},
			{"kyurem"_sv, Species::Kyurem},
			{"kyuremblack"_sv, Species::Kyurem_Black},
			{"kyuremwhite"_sv, Species::Kyurem_White},
			{"lairon"_sv, Species::Lairon},
			{"lampent"_sv, Species::Lampent},
			{"landorus"_sv, Species::Landorus_Incarnate},
			{"landorusincarnate"_sv, Species::Landorus_Incarnate},
			{"landorustherian"_sv, Species::Landorus_Therian},
			{"lanturn"_sv, Species::Lanturn},
			{"lapras"_sv, Species::Lapras},
			{"larvesta"_sv, Species::Larvesta},
			{"larvitar"_sv, Species::Larvitar},
			{"latias"_sv, Species::Latias},
			{"latiasmega"_sv, Species::Latias},
			{"latios"_sv, Species::Latios},
			{"latiosmega"_sv, Species::Latios},
			{"leafeon"_sv, Species::Leafeon},
			{"leavanny"_sv, Species::Leavanny},
			{"ledian"_sv, Species::Ledian},
			{"ledyba"_sv, Species::Ledyba},
			{"lickilicky"_sv, Species::Lickilicky},
			{"lickitung"_sv, Species::Lickitung},
			{"liepard"_sv, Species::Liepard},
			{"lileep"_sv, Species::Lileep},
			{"lilligant"_sv, Species::Lilligant},
			{"lillipup"_sv, Species::Lillipup},
			{"linoone"_sv, Species::Linoone},
			{"linoonegalar"_sv, Species::Linoone_Galar},
			{"litleo"_sv, Species::Litleo},
			{"litten"_sv, Species::Litten},
			{"litwick"_sv, Species::Litwick},
			{"lombre"_sv, Species::Lombre},
			{"lopunny"_sv, Species::Lopunny},
			{"lopunnymega"_sv, Species::Lopunny},
			{"lotad"_sv, Species::Lotad},
			{"loudred"_sv, Species::Loudred},
			{"lucario"_sv, Species::Lucario},
			{"ludicolo"_sv, Species::Ludicolo},
			{"lugia"_sv, Species::Lugia},
			{"lumineon"_sv, Species::Lumineon},
			{"lunala"_sv, Species::Lunala},
			{"lunatone"_sv, Species::Lunatone},
			{"lurantis"_sv, Species::Lurantis},
			{"lurantistotem"_sv, Species::Lurantis},
			{"luvdisc"_sv, Species::Luvdisc},
			{"luxio"_sv, Species::Luxio},
			{"luxray"_sv, Species::Luxray},
			{"lycanroc"_sv, Species::Lycanroc_Midday},
			{"lycanrocdusk"_sv, Species::Lycanroc_Dusk},
			{"lycanrocmidday"_sv, Species::Lycanroc_Midday},
			{"lycanrocmidnight"_sv, Species::Lycanroc_Midnight},
			{"machamp"_sv, Species::Machamp},
			{"machoke"_sv, Species::Machoke},
			{"machop"_sv, Species::Machop},
			{"magby"_sv, Species::Magby},
			{"magcargo"_sv, Species::Magcargo},
			{"magearna"_sv, Species::Magearna},
			{"magikarp"_sv, Species::Magikarp},
			{"magmar"_sv, Species::Magmar},
			{"magmortar"_sv, Species::Magmortar},
			{"magnemite"_sv, Species::Magnemite},
			{"magneton"_sv, Species::Magneton},
			{"magnezone"_sv, Species::Magnezone},
			{"makuhita"_sv, Species::Makuhita},
			{"malamar"_sv, Species::Malamar},
			{"mamoswine"_sv, Species::Mamoswine},
			{"manaphy"_sv, Species::Manaphy},
			{"mandibuzz"_sv, Species::Mandibuzz},
			{"manectric"_sv, Species::Manectric},
			{"manectricmega"_sv, Species::Manectric},
			{"mankey"_sv, Species::Mankey},
			{"mantine"_sv, Species::Mantine},
			{"mantyke"_sv, Species::Mantyke},
			{"maractus"_sv, Species::Maractus},
			{"mareanie"_sv, Species::Mareanie},
			{"mareep"_sv, Species::Mareep},
			{"marill"_sv, Species::Marill},
			{"marowak"_sv, Species::Marowak},
			{"marowakalola"_sv, Species::Marowak_Alola},
			{"marowakalolatotem"_sv, Species::Marowak_Alola},
			{"marshadow"_sv, Species::Marshadow},
			{"marshtomp"_sv, Species::Marshtomp},
			{"masquerain"_sv, Species::Masquerain},
			{"mawile"_sv, Species::Mawile},
			{"mawilemega"_sv, Species::Mawile},
			{"medicham"_sv, Species::Medicham},
			{"medichammega"_sv, Species::Medicham},
			{"meditite"_sv, Species::Meditite},
			{"meganium"_sv, Species::Meganium},
			{"melmetal"_sv, Species::Melmetal},
			{"meloetta"_sv, Species::Meloetta},
			{"meltan"_sv, Species::Meltan},
			{"meowstic"_sv, Species::Meowstic_M},
			{"meowsticf"_sv, Species::Meowstic_F},
			{"meowsticm"_sv, Species::Meowstic_M},
			{"meowth"_sv, Species::Meowth},
			{"meowthalola"_sv, Species::Meowth_Alola},
			{"meowthgalar"_sv, Species::Meowth_Galar},
			{"mesprit"_sv, Species::Mesprit},
			{"metagross"_sv, Species::Metagross},
			{"metagrossmega"_sv, Species::Metagross},
			{"metang"_sv, Species::Metang},
			{"metapod"_sv, Species::Metapod},
			{"mew"_sv, Species::Mew},
			{"mewtwo"_sv, Species::Mewtwo},
			{"mienfoo"_sv, Species::Mienfoo},
			{"mienshao"_sv, Species::Mienshao},
			{"mightyena"_sv, Species::Mightyena},
			{"milcery"_sv, Species::Milcery},
			{"milotic"_sv, Species::Milotic},
			{"miltank"_sv, Species::Miltank},
			{"mimejr"_sv, Species::Mime_Jr},
			{"mimikyu"_sv, Species::Mimikyu},
			{"mimikyutotem"_sv, Species::Mimikyu},
			{"minccino"_sv, Species::Minccino},
			{"minior"_sv, Species::Minior_Core},
			{"miniorcore"_sv, Species::Minior_Core},
			{"miniormeteor"_sv, Species::Minior_Meteor},
			{"minun"_sv, Species::Minun},
			{"misdreavus"_sv, Species::Misdreavus},
			{"mismagius"_sv, Species::Mismagius},
			{"moltres"_sv, Species::Moltres},
			{"moltresgalar"_sv, Species::Moltres_Galar},
			{"monferno"_sv, Species::Monferno},
			{"morelull"_sv, Species::Morelull},
			{"morgrem"_sv, Species::Morgrem},
			{"morpeko"_sv, Species::Morpeko},
			{"mothim"_sv, Species::Mothim},
			{"mrmime"_sv, Species::Mr_Mime},
			{"mrmimegalar"_sv, Species::Mr_Mime_Galar},
			{"mrrime"_sv, Species::Mr_Rime},
			{"mudbray"_sv, Species::Mudbray},
			{"mudkip"_sv, Species::Mudkip},
			{"mudsdale"_sv, Species::Mudsdale},
			{"muk"_sv, Species::Muk},
			{"mukalola"_sv, Species::Muk_Alola},
			{"munchlax"_sv, Species::Munchlax},
			{"munna"_sv, Species::Munna},
			{"murkrow"_sv, Species::Murkrow},
			{"musharna"_sv, Species::Musharna},
			{"naganadel"_sv, Species::Naganadel},
			{"natu"_sv, Species::Natu},
			{"necrozma"_sv, Species::Necrozma},
			{"necrozmadawnwings"_sv, Species::Necrozma_Dawn_Wings},
			{"necrozmaduskmane"_sv, Species::Necrozma_Dusk_Mane},
			{"necrozmaultra"_sv, Species::Necrozma_Ultra},
			{"nickit"_sv, Species::Nickit},
			{"nidoking"_sv, Species::Nidoking},
			{"nidoqueen"_sv, Species::Nidoqueen},
			{"nidoranf"_sv, Species::Nidoran_F},
			{"nidoranm"_sv, Species::Nidoran_M},
			{"nidorina"_sv, Species::Nidorina},
			{"nidorino"_sv, Species::Nidorino},
			{"nihilego"_sv, Species::Nihilego},
			{"nincada"_sv, Species::Nincada},
			{"ninetales"_sv, Species::Ninetales},
			{"ninetalesalola"_sv, Species::Ninetales_Alola},
			{"ninjask"_sv, Species::Ninjask},
			{"noctowl"_sv, Species::Noctowl},
			{"noibat"_sv, Species::Noibat},
			{"noivern"_sv, Species::Noivern},
			{"nosepass"_sv, Species::Nosepass},
			{"numel"_sv, Species::Numel},
			{"nuzleaf"_sv, Species::Nuzleaf},
			{"obstagoon"_sv, Species::Obstagoon},
			{"octillery"_sv, Species::Octillery},
			{"oddish"_sv, Species::Oddish},
			{"omanyte"_sv, Species::Omanyte},
			{"omastar"_sv, Species::Omastar},
			{"onix"_sv, Species::Onix},
			{"oranguru"_sv, Species::Oranguru},
			{"orbeetle"_sv, Species::Orbeetle},
			{"oricorio"_sv, Species::Oricorio_Baile},
			{"oricoriobaile"_sv, Species::Oricorio_Baile},
			{"oricoriopau"_sv, Species::Oricorio_Pau},
			{"oricoriopompom"_sv, Species::Oricorio_Pom_Pom},
			{"oricoriosensu"_sv, Species::Oricorio_Sensu},
			{"oshawott"_sv, Species::Oshawott},
			{"pachirisu"_sv, Species::Pachirisu},
			{"palkia"_sv, Species::Palkia},
			{"palossand"_sv, Species::Palossand},
			{"palpitoad"_sv, Species::Palpitoad},
			{"pancham"_sv, Species::Pancham},
			{"pangoro"_sv, Species::Pangoro},
			{"panpour"_sv, Species::Panpour},
			{"pansage"_sv, Species::Pansage},
			{"pansear"_sv, Species::Pansear},
			{"paras"_sv, Species::Paras},
			{"parasect"_sv, Species::Parasect},
			{"passimian"_sv, Species::Passimian},
			{"patrat"_sv, Species::Patrat},
			{"pawniard"_sv, Species::Pawniard},
			{"pelipper"_sv, Species::Pelipper},
			{"perrserker"_sv, Species::Perrserker},
			{"persian"_sv, Species::Persian},
			{"persianalola"_sv, Species::Persian_Alola},
			{"petilil"_sv, Species::Petilil},
			{"phanpy"_sv, Species::Phanpy},
			{"phantump"_sv, Species::Phantump},
			{"pheromosa"_sv, Species::Pheromosa},
			{"phione"_sv, Species::Phione},
			{"pichu"_sv, Species::Pichu},
			{"pidgeot"_sv, Species::Pidgeot},
			{"pidgeotmega"_sv, Species::Pidgeot},
			{"pidgeotto"_sv, Species::Pidgeotto},
			{"pidgey"_sv, Species::Pidgey},
			{"pidove"_sv, Species::Pidove},
			{"pignite"_sv, Species::Pignite},
			{"pikachu"_sv, Species::Pikachu},
			{"pikachualola"_sv, Species::Pikachu},
			{"pikachuhoenn"_sv, Species::Pikachu},
			{"pikachukalos"_sv, Species::Pikachu},
			{"pikachuoriginal"_sv, Species::Pikachu},
			{"pikachupartner"_sv, Species::Pikachu},
			{"pikachusinnoh"_sv, Species::Pikachu},
			{"pikachuunova"_sv, Species::Pikachu},
			{"pikachuworld"_sv, Species::Pikachu},
			{"pikipek"_sv, Species::Pikipek},
			{"piloswine"_sv, Species::Piloswine},
			{"pincurchin"_sv, Species::Pincurchin},
			{"pineco"_sv, Species::Pineco},
			{"pinsir"_sv, Species::Pinsir},
			{"pinsirmega"_sv, Species::Pinsir},
			{"piplup"_sv, Species::Piplup},
			{"plusle"_sv, Species::Plusle},
			{"poipole"_sv, Species::Poipole},
			{"politoed"_sv, Species::Politoed},
			{"poliwag"_sv, Species::Poliwag},
			{"poliwhirl"_sv, Species::Poliwhirl},
			{"poliwrath"_sv, Species::Poliwrath},
			{"polteageist"_sv, Species::Polteageist},
			{"ponyta"_sv, Species::Ponyta},
			{"ponytagalar"_sv, Species::Ponyta_Galar},
			{"poochyena"_sv, Species::Poochyena},
			{"popplio"_sv, Species::Popplio},
			{"porygon"_sv, Species::Porygon},
			{"porygon2"_sv, Species::Porygon2},
			{"porygonz"_sv, Species::Porygon_Z},
			{"primarina"_sv, Species::Primarina},
			{"primeape"_sv, Species::Primeape},
			{"prinplup"_sv, Species::Prinplup},
			{"probopass"_sv, Species::Probopass},
			{"psyduck"_sv, Species::Psyduck},
			{"pumpkaboo"_sv, Species::Pumpkaboo_Average},
			{"pumpkabooaverage"_sv, Species::Pumpkaboo_Average},
			{"pumpkaboolarge"_sv, Species::Pumpkaboo_Large},
			{"pumpkaboosmall"_sv, Species::Pumpkaboo_Small},
			{"pumpkaboosuper"_sv, Species::Pumpkaboo_Super},
			{"pupitar"_sv, Species::Pupitar},
			{"purrloin"_sv, Species::Purrloin},
			{"purugly"_sv, Species::Purugly},
			{"pyroar"_sv, Species::Pyroar},
			{"pyukumuku"_sv, Species::Pyukumuku},
			{"quagsire"_sv, Species::Quagsire},
			{"quilava"_sv, Species::Quilava},
			{"quilladin"_sv, Species::Quilladin},
			{"qwilfish"_sv, Species::Qwilfish},
			{"raboot"_sv, Species::Raboot},
			{"raichu"_sv, Species::Raichu},
			{"raichualola"_sv, Species::Raichu_Alola},
			{"raikou"_sv, Species::Raikou},
			{"ralts"_sv, Species::Ralts},
			{"rampardos"_sv, Species::Rampardos},
			{"rapidash"_sv, Species::Rapidash},
			{"rapidashgalar"_sv, Species::Rapidash_Galar},
			{"raticate"_sv, Species::Raticate},
			{"raticatealola"_sv, Species::Raticate_Alola},
			{"raticatealolatotem"_sv, Species::Raticate_Alola},
			{"rattata"_sv, Species::Rattata},
			{"rattataalola"_sv, Species::Rattata_Alola},
			{"rayquaza"_sv, Species::Rayquaza},
			{"regice"_sv, Species::Regice},
			{"regidrago"_sv, Species::Regidrago},
			{"regieleki"_sv, Species::Regieleki},
			{"regigigas"_sv, Species::Regigigas},
			{"regirock"_sv, Species::Regirock},
			{"registeel"_sv, Species::Registeel},
			{"relicanth"_sv, Species::Relicanth},
			{"remoraid"_sv, Species::Remoraid},
			{"reshiram"_sv, Species::Reshiram},
			{"reuniclus"_sv, Species::Reuniclus},
			{"rhydon"_sv, Species::Rhydon},
			{"rhyhorn"_sv, Species::Rhyhorn},
			{"rhyperior"_sv, Species::Rhyperior},
			{"ribombee"_sv, Species::Ribombee},
			{"ribombeetotem"_sv, Species::Ribombee},
			{"rillaboom"_sv, Species::Rillaboom},
			{"riolu"_sv, Species::Riolu},
			{"rockruff"_sv, Species::Rockruff},
			{"roggenrola"_sv, Species::Roggenrola},
			{"rolycoly"_sv, Species::Rolycoly},
			{"rookidee"_sv, Species::Rookidee},
			{"roselia"_sv, Species::Roselia},
			{"roserade"_sv, Species::Roserade},
			{"rotom"_sv, Species::Rotom},
			{"rotomc"_sv, Species::Rotom_Mow},
			{"rotomf"_sv, Species::Rotom_Frost},
			{"rotomfan"_sv, Species::Rotom_Fan},
			{"rotomfrost"_sv, Species::Rotom_Frost},
			{"rotomh"_sv, Species::Rotom_Heat},
			{"rotomheat"_sv, Species::Rotom_Heat},
			{"rotommow"_sv, Species::Rotom_Mow},
			{"rotoms"_sv, Species::Rotom_Fan},
			{"rotomw"_sv, Species::Rotom_Wash},
			{"rotomwash"_sv, Species::Rotom_Wash},
			{"rowlet"_sv, Species::Rowlet},
			{"rufflet"_sv, Species::Rufflet},
			{"runerigus"_sv, Species::Runerigus},
			{"sableye"_sv, Species::Sableye},
			{"sableyemega"_sv, Species::Sableye},
			{"salamence"_sv, Species::Salamence},
			{"salandit"_sv, Species::Salandit},
			{"salazzle"_sv, Species::Salazzle},
			{"salazzletotem"_sv, Species::Salazzle},
			{"samurott"_sv, Species::Samurott},
			{"sandaconda"_sv, Species::Sandaconda},
			{"sandile"_sv, Species::Sandile},
			{"sandshrew"_sv, Species::Sandshrew},
			{"sandshrewalola"_sv, Species::Sandshrew_Alola},
			{"sandslash"_sv, Species::Sandslash},
			{"sandslashalola"_sv, Species::Sandslash_Alola},
			{"sandygast"_sv, Species::Sandygast},
			{"sawk"_sv, Species::Sawk},
			{"sawsbuck"_sv, Species::Sawsbuck},
			{"scatterbug"_sv, Species::Scatterbug},
			{"sceptile"_sv, Species::Sceptile},
			{"sceptilemega"_sv, Species::Sceptile},
			{"scizor"_sv, Species::Scizor},
			{"scizormega"_sv, Species::Scizor},
			{"scolipede"_sv, Species::Scolipede},
			{"scorbunny"_sv, Species::Scorbunny},
			{"scrafty"_sv, Species::Scrafty},
			{"scraggy"_sv, Species::Scraggy},
			{"scyther"_sv, Species::Scyther},
			{"seadra"_sv, Species::Seadra},
			{"seaking"_sv, Species::Seaking},
			{"sealeo"_sv, Species::Sealeo},
			{"seedot"_sv, Species::Seedot},
			{"seel"_sv, Species::Seel},
			{"seismitoad"_sv, Species::Seismitoad},
			{"sentret"_sv, Species::Sentret},
			{"serperior"_sv, Species::Serperior},
			{"servine"_sv, Species::Servine},
			{"seviper"_sv, Species::Seviper},
			{"sewaddle"_sv, Species::Sewaddle},
			{"sharpedo"_sv, Species::Sharpedo},
			{"sharpedomega"_sv, Species::Sharpedo},
			{"shaymin"_sv, Species::Shaymin_Land},
			{"shayminland"_sv, Species::Shaymin_Land},
			{"shayminsky"_sv, Species::Shaymin_Sky},
			{"shedinja"_sv, Species::Shedinja},
			{"shelgon"_sv, Species::Shelgon},
			{"shellder"_sv, Species::Shellder},
			{"shellos"_sv, Species::Shellos},
			{"shelloseast"_sv, Species::Shellos},
			{"shelloswest"_sv, Species::Shellos},
			{"shelmet"_sv, Species::Shelmet},
			{"shieldon"_sv, Species::Shieldon},
			{"shiftry"_sv, Species::Shiftry},
			{"shiinotic"_sv, Species::Shiinotic},
			{"shinx"_sv, Species::Shinx},
			{"shroomish"_sv, Species::Shroomish},
			{"shuckle"_sv, Species::Shuckle},
			{"shuppet"_sv, Species::Shuppet},
			{"sigilyph"_sv, Species::Sigilyph},
			{"silcoon"_sv, Species::Silcoon},
			{"silicobra"_sv, Species::Silicobra},
			{"silvally"_sv, Species::Silvally},
			{"silvallybug"_sv, Species::Silvally},
			{"silvallydark"_sv, Species::Silvally},
			{"silvallydragon"_sv, Species::Silvally},
			{"silvallyelectric"_sv, Species::Silvally},
			{"silvallyfairy"_sv, Species::Silvally},
			{"silvallyfighting"_sv, Species::Silvally},
			{"silvallyfire"_sv, Species::Silvally},
			{"silvallyflying"_sv, Species::Silvally},
			{"silvallyghost"_sv, Species::Silvally},
			{"silvallygrass"_sv, Species::Silvally},
			{"silvallyground"_sv, Species::Silvally},
			{"silvallyice"_sv, Species::Silvally},
			{"silvallypoison"_sv, Species::Silvally},
			{"silvallypsychic"_sv, Species::Silvally},
			{"silvallyrock"_sv, Species::Silvally},
			{"silvallysteel"_sv, Species::Silvally},
			{"silvallywater"_sv, Species::Silvally},
			{"simipour"_sv, Species::Simipour},
			{"simisage"_sv, Species::Simisage},
			{"simisear"_sv, Species::Simisear},
			{"sinistea"_sv, Species::Sinistea},
			{"sirfetchd"_sv, Species::Sirfetchd},
			{"sizzlipede"_sv, Species::Sizzlipede},
			{"skarmory"_sv, Species::Skarmory},
			{"skiddo"_sv, Species::Skiddo},
			{"skiploom"_sv, Species::Skiploom},
			{"skitty"_sv, Species::Skitty},
			{"skorupi"_sv, Species::Skorupi},
			{"skrelp"_sv, Species::Skrelp},
			{"skuntank"_sv, Species::Skuntank},
			{"skwovet"_sv, Species::Skwovet},
			{"slaking"_sv, Species::Slaking},
			{"slakoth"_sv, Species::Slakoth},
			{"sliggoo"_sv, Species::Sliggoo},
			{"slowbro"_sv, Species::Slowbro},
			{"slowbrogalar"_sv, Species::Slowbro_Galar},
			{"slowbromega"_sv, Species::Slowbro},
			{"slowking"_sv, Species::Slowking},
			{"slowkinggalar"_sv, Species::Slowking_Galar},
			{"slowpoke"_sv, Species::Slowpoke},
			{"slowpokegalar"_sv, Species::Slowpoke_Galar},
			{"slugma"_sv, Species::Slugma},
			{"slurpuff"_sv, Species::Slurpuff},
			{"smeargle"_sv, Species::Smeargle},
			{"smoochum"_sv, Species::Smoochum},
			{"sneasel"_sv, Species::Sneasel},
			{"snivy"_sv, Species::Snivy},
			{"snom"_sv, Species::Snom},
			{"snorlax"_sv, Species::Snorlax},
			{"snorunt"_sv, Species::Snorunt},
			{"snover"_sv, Species::Snover},
			{"snubbull"_sv, Species::Snubbull},
			{"sobble"_sv, Species::Sobble},
			{"solgaleo"_sv, Species::Solgaleo},
			{"solosis"_sv, Species::Solosis},
			{"solrock"_sv, Species::Solrock},
			{"spearow"_sv, Species::Spearow},
			{"spectrier"_sv, Species::Spectrier},
			{"spewpa"_sv, Species::Spewpa},
			{"spheal"_sv, Species::Spheal},
			{"spinarak"_sv, Species::Spinarak},
			{"spinda"_sv, Species::Spinda},
			{"spiritomb"_sv, Species::Spiritomb},
			{"spoink"_sv, Species::Spoink},
			{"spritzee"_sv, Species::Spritzee},
			{"squirtle"_sv, Species::Squirtle},
			{"stakataka"_sv, Species::Stakataka},
			{"stantler"_sv, Species::Stantler},
			{"staraptor"_sv, Species::Staraptor},
			{"staravia"_sv, Species::Staravia},
			{"starly"_sv, Species::Starly},
			{"starmie"_sv, Species::Starmie},
			{"staryu"_sv, Species::Staryu},
			{"steelix"_sv, Species::Steelix},
			{"steelixmega"_sv, Species::Steelix},
			{"steenee"_sv, Species::Steenee},
			{"stonjourner"_sv, Species::Stonjourner},
			{"stoutland"_sv, Species::Stoutland},
			{"stufful"_sv, Species::Stufful},
			{"stunfisk"_sv, Species::Stunfisk},
			{"stunfiskgalar"_sv, Species::Stunfisk_Galar},
			{"stunky"_sv, Species::Stunky},
			{"sudowoodo"_sv, Species::Sudowoodo},
			{"suicune"_sv, Species::Suicune},
			{"sunflora"_sv, Species::Sunflora},
			{"sunkern"_sv, Species::Sunkern},
			{"surskit"_sv, Species::Surskit},
			{"swablu"_sv, Species::Swablu},
			{"swadloon"_sv, Species::Swadloon},
			{"swalot"_sv, Species::Swalot},
			{"swampert"_sv, Species::Swampert},
			{"swampertmega"_sv, Species::Swampert},
			{"swanna"_sv, Species::Swanna},
			{"swellow"_sv, Species::Swellow},
			{"swinub"_sv, Species::Swinub},
			{"swirlix"_sv, Species::Swirlix},
			{"swoobat"_sv, Species::Swoobat},
			{"sylveon"_sv, Species::Sylveon},
			{"taillow"_sv, Species::Taillow},
			{"talonflame"_sv, Species::Talonflame},
			{"tangela"_sv, Species::Tangela},
			{"tangrowth"_sv, Species::Tangrowth},
			{"tapubulu"_sv, Species::Tapu_Bulu},
			{"tapufini"_sv, Species::Tapu_Fini},
			{"tapukoko"_sv, Species::Tapu_Koko},
			{"tapulele"_sv, Species::Tapu_Lele},
			{"tauros"_sv, Species::Tauros},
			{"teddiursa"_sv, Species::Teddiursa},
			{"tentacool"_sv, Species::Tentacool},
			{"tentacruel"_sv, Species::Tentacruel},
			{"tepig"_sv, Species::Tepig},
			{"terrakion"_sv, Species::Terrakion},
			{"thievul"_sv, Species::Thievul},
			{"throh"_sv, Species::Throh},
			{"thundurus"_sv, Species::Thundurus_Incarnate},
			{"thundurusincarnate"_sv, Species::Thundurus_Incarnate},
			{"thundurustherian"_sv, Species::Thundurus_Therian},
			{"thwackey"_sv, Species::Thwackey},
			{"timburr"_sv, Species::Timburr},
			{"tirtouga"_sv, Species::Tirtouga},
			{"togedemaru"_sv, Species::Togedemaru},
			{"togedemarutotem"_sv, Species::Togedemaru},
			{"togekiss"_sv, Species::Togekiss},
			{"togepi"_sv, Species::Togepi},
			{"togetic"_sv, Species::Togetic},
			{"torchic"_sv, Species::Torchic},
			{"torkoal"_sv, Species::Torkoal},
			{"tornadus"_sv, Species::Tornadus_Incarnate},
			{"tornadusincarnate"_sv, Species::Tornadus_Incarnate},
			{"tornadustherian"_sv, Species::Tornadus_Therian},
			{"torracat"_sv, Species::Torracat},
			{"torterra"_sv, Species::Torterra},
			{"totodile"_sv, Species::Totodile},
			{"toucannon"_sv, Species::Toucannon},
			{"toxapex"_sv, Species::Toxapex},
			{"toxel"_sv, Species::Toxel},
			{"toxicroak"_sv, Species::Toxicroak},
			{"toxtricity"_sv, Species::Toxtricity_Amped},
			{"toxtricityamped"_sv, Species::Toxtricity_Amped},
			{"toxtricitylowkey"_sv, Species::Toxtricity_Low_Key},
			{"tranquill"_sv, Species::Tranquill},
			{"trapinch"_sv, Species::Trapinch},
			{"treecko"_sv, Species::Treecko},
			{"trevenant"_sv, Species::Trevenant},
			{"tropius"_sv, Species::Tropius},
			{"trubbish"_sv, Species::Trubbish},
			{"trumbeak"_sv, Species::Trumbeak},
			{"tsareena"_sv, Species::Tsareena},
			{"turtonator"_sv, Species::Turtonator},
			{"turtwig"_sv, Species::Turtwig},
			{"tympole"_sv, Species::Tympole},
			{"tynamo"_sv, Species::Tynamo},
			{"typenull"_sv, Species::Type_Null},
			{"typhlosion"_sv, Species::Typhlosion},
			{"tyranitar"_sv, Species::Tyranitar},
			{"tyranitarmega"_sv, Species::Tyranitar},
			{"tyrantrum"_sv, Species::Tyrantrum},
			{"tyrogue"_sv, Species::Tyrogue},
			{"tyrunt"_sv, Species::Tyrunt},
			{"umbreon"_sv, Species::Umbreon},
			{"unfezant"_sv, Species::Unfezant},
			{"unown"_sv, Species::Unown},
			{"unowna"_sv, Species::Unown},
			{"unownb"_sv, Species::Unown},
			{"unownc"_sv, Species::Unown},
			{"unownd"_sv, Species::Unown},
			{"unowne"_sv, Species::Unown},
			{"unownexclamation"_sv, Species::Unown},
			{"unownf"_sv, Species::Unown},
			{"unowng"_sv, Species::Unown},
			{"unownh"_sv, Species::Unown},
			{"unowni"_sv, Species::Unown},
			{"unownj"_sv, Species::Unown},
			{"unownk"_sv, Species::Unown},
			{"unownl"_sv, Species::Unown},
			{"unownm"_sv, Species::Unown},
			{"unownn"_sv, Species::Unown},
			{"unowno"_sv, Species::Unown},
			{"unownp"_sv, Species::Unown},
			{"unownq"_sv, Species::Unown},
			{"unownquestion"_sv, Species::Unown},
			{"unownr"_sv, Species::Unown},
			{"unowns"_sv, Species::Unown},
			{"unownt"_sv, Species::Unown},
			{"unownu"_sv, Species::Unown},
			{"unownv"_sv, Species::Unown},
			{"unownw"_sv, Species::Unown},
			{"unownx"_sv, Species::Unown},
			{"unowny"_sv, Species::Unown},
			{"unownz"_sv, Species::Unown},
			{"ursaring"_sv, Species::Ursaring},
			{"urshifurapidstrike"_sv, Species::Urshifu_Rapid_Strike},
			{"urshifusinglestrike"_sv, Species::Urshifu_Single_Strike},
			{"uxie"_sv, Species::Uxie},
			{"vanillish"_sv, Species::Vanillish},
			{"vanillite"_sv, Species::Vanillite},
			{"vanilluxe"_sv, Species::Vanilluxe},
			{"vaporeon"_sv, Species::Vaporeon},
			{"venipede"_sv, Species::Venipede},
			{"venomoth"_sv, Species::Venomoth},
			{"venonat"_sv, Species::Venonat},
			{"venusaur"_sv, Species::Venusaur},
			{"venusaurmega"_sv, Species::Venusaur},
			{"vespiquen"_sv, Species::Vespiquen},
			{"vibrava"_sv, Species::Vibrava},
			{"victini"_sv, Species::Victini},
			{"victreebel"_sv, Species::Victreebel},
			{"vigoroth"_sv, Species::Vigoroth},
			{"vikavolt"_sv, Species::Vikavolt},
			{"vikavolttotem"_sv, Species::Vikavolt},
			{"vileplume"_sv, Species::Vileplume},
			{"virizion"_sv, Species::Virizion},
			{"vivillon"_sv, Species::Vivillon},
			{"volbeat"_sv, Species::Volbeat},
			{"volcanion"_sv, Species::Volcanion},
			{"volcarona"_sv, Species::Volcarona},
			{"voltorb"_sv, Species::Voltorb},
			{"vullaby"_sv, Species::Vullaby},
			{"vulpix"_sv, Species::Vulpix},
			{"vulpixalola"_sv, Species::Vulpix_Alola},
			{"wailmer"_sv, Species::Wailmer},
			{"wailord"_sv, Species::Wailord},
			{"walrein"_sv, Species::Walrein},
			{"wartortle"_sv, Species::Wartortle},
			{"watchog"_sv, Species::Watchog},
			{"weavile"_sv, Species::Weavile},
			{"weedle"_sv, Species::Weedle},
			{"weepinbell"_sv, Species::Weepinbell},
			{"weezing"_sv, Species::Weezing},
			{"weezinggalar"_sv, Species::Weezing_Galar},
			{"whimsicott"_sv, Species::Whimsicott},
			{"whirlipede"_sv, Species::Whirlipede},
			{"whiscash"_sv, Species::Whiscash},
			{"whismur"_sv, Species::Whismur},
			{"wigglytuff"_sv, Species::Wigglytuff},
			{"wimpod"_sv, Species::Wimpod},
			{"wingull"_sv, Species::Wingull},
			{"wishiwashi"_sv, Species::Wishiwashi_Solo},
			{"wishiwashischool"_sv, Species::Wishiwashi_School},
			{"wishiwashisolo"_sv, Species::Wishiwashi_Solo},
			{"wobbuffet"_sv, Species::Wobbuffet},
			{"woobat"_sv, Species::Woobat},
			{"wooloo"_sv, Species::Wooloo},
			{"wooper"_sv, Species::Wooper},
			{"wormadam"_sv, Species::Wormadam_Plant},
			{"wormadamg"_sv, Species::Wormadam_Sandy},
			{"wormadamplant"_sv, Species::Wormadam_Plant},
			{"wormadams"_sv, Species::Wormadam_Trash},
			{"wormadamsandy"_sv, Species::Wormadam_Sandy},
			{"wormadamtrash"_sv, Species::Wormadam_Trash},
			{"wurmple"_sv, Species::Wurmple},
			{"wynaut"_sv, Species::Wynaut},
			{"xatu"_sv, Species::Xatu},
			{"xerneas"_sv, Species::Xerneas},
			{"xurkitree"_sv, Species::Xurkitree},
			{"yamask"_sv, Species::Yamask},
			{"yamaskgalar"_sv, Species::Yamask_Galar},
			{"yamper"_sv, Species::Yamper},
			{"yanma"_sv, Species::Yanma},
			{"yanmega"_sv, Species::Yanmega},
			{"yungoos"_sv, Species::Yungoos},
			{"yveltal"_sv, Species::Yveltal},
			{"zacian"_sv, Species::Zacian},
			{"zaciancrownedsword"_sv, Species::Zacian_Crowned_Sword},
			{"zamazenta"_sv, Species::Zamazenta},
			{"zamazentacrownedshield"_sv, Species::Zamazenta_Crowned_Shield},
			{"zangoose"_sv, Species::Zangoose},
			{"zapdos"_sv, Species::Zapdos},
			{"zapdosgalar"_sv, Species::Zapdos_Galar},
			{"zarude"_sv, Species::Zarude},
			{"zebstrika"_sv, Species::Zebstrika},
			{"zekrom"_sv, Species::Zekrom},
			{"zeraora"_sv, Species::Zeraora},
			{"zigzagoon"_sv, Species::Zigzagoon},
			{"zigzagoongalar"_sv, Species::Zigzagoon_Galar},
			{"zoroark"_sv, Species::Zoroark},
			{"zorua"_sv, Species::Zorua},
			{"zubat"_sv, Species::Zubat},
			{"zweilous"_sv, Species::Zweilous},
			{"zygarde"_sv, Species::Zygarde_50},
			{"zygarde10"_sv, Species::Zygarde_10},
			{"zygarde50"_sv, Species::Zygarde_50},
			{"zygardecomplete"_sv, Species::Zygarde_Complete},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<22_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Species"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
