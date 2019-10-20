// Type function definitions
// Copyright (C) 2018 David Stone
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

#include <tm/type/collection.hpp>

#include <tm/type/type.hpp>

#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

namespace {

constexpr auto make_types(Type const type1) {
	return containers::array{type1, Type::Typeless};
}
constexpr auto make_types(Type const type1, Type const type2) {
	return containers::array{type1, type2};
}

constexpr auto get_type(Species const species) {
	switch (species) {
		// Generation 1
		case Species::Bulbasaur: return make_types(Type::Grass, Type::Poison);
		case Species::Ivysaur: return make_types(Type::Grass, Type::Poison);
		case Species::Venusaur: return make_types(Type::Grass, Type::Poison);
		case Species::Charmander: return make_types(Type::Fire);
		case Species::Charmeleon: return make_types(Type::Fire);
		case Species::Charizard: return make_types(Type::Fire, Type::Flying);
		case Species::Squirtle: return make_types(Type::Water);
		case Species::Wartortle: return make_types(Type::Water);
		case Species::Blastoise: return make_types(Type::Water);
		case Species::Caterpie: return make_types(Type::Bug);
		case Species::Metapod: return make_types(Type::Bug);
		case Species::Butterfree: return make_types(Type::Bug, Type::Flying);
		case Species::Weedle: return make_types(Type::Bug, Type::Poison);
		case Species::Kakuna: return make_types(Type::Bug, Type::Poison);
		case Species::Beedrill: return make_types(Type::Bug, Type::Poison);
		case Species::Pidgey: return make_types(Type::Normal, Type::Flying);
		case Species::Pidgeotto: return make_types(Type::Normal, Type::Flying);
		case Species::Pidgeot: return make_types(Type::Normal, Type::Flying);
		case Species::Rattata: return make_types(Type::Normal);
		case Species::Raticate: return make_types(Type::Normal);
		case Species::Spearow: return make_types(Type::Normal, Type::Flying);
		case Species::Fearow: return make_types(Type::Normal, Type::Flying);
		case Species::Ekans: return make_types(Type::Poison);
		case Species::Arbok: return make_types(Type::Poison);
		case Species::Pikachu: return make_types(Type::Electric);
		case Species::Raichu: return make_types(Type::Electric);
		case Species::Sandshrew: return make_types(Type::Ground);
		case Species::Sandslash: return make_types(Type::Ground);
		case Species::Nidoran_F: return make_types(Type::Poison);
		case Species::Nidorina: return make_types(Type::Poison);
		case Species::Nidoqueen: return make_types(Type::Poison, Type::Ground);
		case Species::Nidoran_M: return make_types(Type::Poison);
		case Species::Nidorino: return make_types(Type::Poison);
		case Species::Nidoking: return make_types(Type::Poison, Type::Ground);
		case Species::Clefairy: return make_types(Type::Normal);
		case Species::Clefable: return make_types(Type::Normal);
		case Species::Vulpix: return make_types(Type::Fire);
		case Species::Ninetales: return make_types(Type::Fire);
		case Species::Jigglypuff: return make_types(Type::Normal);
		case Species::Wigglytuff: return make_types(Type::Normal);
		case Species::Zubat: return make_types(Type::Poison, Type::Flying);
		case Species::Golbat: return make_types(Type::Poison, Type::Flying);
		case Species::Oddish: return make_types(Type::Grass, Type::Poison);
		case Species::Gloom: return make_types(Type::Grass, Type::Poison);
		case Species::Vileplume: return make_types(Type::Grass, Type::Poison);
		case Species::Paras: return make_types(Type::Bug, Type::Grass);
		case Species::Parasect: return make_types(Type::Bug, Type::Grass);
		case Species::Venonat: return make_types(Type::Bug, Type::Poison);
		case Species::Venomoth: return make_types(Type::Bug, Type::Poison);
		case Species::Diglett: return make_types(Type::Ground);
		case Species::Dugtrio: return make_types(Type::Ground);
		case Species::Meowth: return make_types(Type::Normal);
		case Species::Persian: return make_types(Type::Normal);
		case Species::Psyduck: return make_types(Type::Water);
		case Species::Golduck: return make_types(Type::Water);
		case Species::Mankey: return make_types(Type::Fighting);
		case Species::Primeape: return make_types(Type::Fighting);
		case Species::Growlithe: return make_types(Type::Fire);
		case Species::Arcanine: return make_types(Type::Fire);
		case Species::Poliwag: return make_types(Type::Water);
		case Species::Poliwhirl: return make_types(Type::Water);
		case Species::Poliwrath: return make_types(Type::Water, Type::Fighting);
		case Species::Abra: return make_types(Type::Psychic);
		case Species::Kadabra: return make_types(Type::Psychic);
		case Species::Alakazam: return make_types(Type::Psychic);
		case Species::Machop: return make_types(Type::Fighting);
		case Species::Machoke: return make_types(Type::Fighting);
		case Species::Machamp: return make_types(Type::Fighting);
		case Species::Bellsprout: return make_types(Type::Grass, Type::Poison);
		case Species::Weepinbell: return make_types(Type::Grass, Type::Poison);
		case Species::Victreebel: return make_types(Type::Grass, Type::Poison);
		case Species::Tentacool: return make_types(Type::Water, Type::Poison);
		case Species::Tentacruel: return make_types(Type::Water, Type::Poison);
		case Species::Geodude: return make_types(Type::Rock, Type::Ground);
		case Species::Graveler: return make_types(Type::Rock, Type::Ground);
		case Species::Golem: return make_types(Type::Rock, Type::Ground);
		case Species::Ponyta: return make_types(Type::Fire);
		case Species::Rapidash: return make_types(Type::Fire);
		case Species::Slowpoke: return make_types(Type::Water, Type::Psychic);
		case Species::Slowbro: return make_types(Type::Water, Type::Psychic);
		case Species::Magnemite: return make_types(Type::Electric, Type::Steel);
		case Species::Magneton: return make_types(Type::Electric, Type::Steel);
		case Species::Farfetchd: return make_types(Type::Normal, Type::Flying);
		case Species::Doduo: return make_types(Type::Normal, Type::Flying);
		case Species::Dodrio: return make_types(Type::Normal, Type::Flying);
		case Species::Seel: return make_types(Type::Water);
		case Species::Dewgong: return make_types(Type::Water, Type::Ice);
		case Species::Grimer: return make_types(Type::Poison);
		case Species::Muk: return make_types(Type::Poison);
		case Species::Shellder: return make_types(Type::Water);
		case Species::Cloyster: return make_types(Type::Water, Type::Ice);
		case Species::Gastly: return make_types(Type::Ghost, Type::Poison);
		case Species::Haunter: return make_types(Type::Ghost, Type::Poison);
		case Species::Gengar: return make_types(Type::Ghost, Type::Poison);
		case Species::Onix: return make_types(Type::Rock, Type::Ground);
		case Species::Drowzee: return make_types(Type::Psychic);
		case Species::Hypno: return make_types(Type::Psychic);
		case Species::Krabby: return make_types(Type::Water);
		case Species::Kingler: return make_types(Type::Water);
		case Species::Voltorb: return make_types(Type::Electric);
		case Species::Electrode: return make_types(Type::Electric);
		case Species::Exeggcute: return make_types(Type::Grass, Type::Psychic);
		case Species::Exeggutor: return make_types(Type::Grass, Type::Psychic);
		case Species::Cubone: return make_types(Type::Ground);
		case Species::Marowak: return make_types(Type::Ground);
		case Species::Hitmonlee: return make_types(Type::Fighting);
		case Species::Hitmonchan: return make_types(Type::Fighting);
		case Species::Lickitung: return make_types(Type::Normal);
		case Species::Koffing: return make_types(Type::Poison);
		case Species::Weezing: return make_types(Type::Poison);
		case Species::Rhyhorn: return make_types(Type::Ground, Type::Rock);
		case Species::Rhydon: return make_types(Type::Ground, Type::Rock);
		case Species::Chansey: return make_types(Type::Normal);
		case Species::Tangela: return make_types(Type::Grass);
		case Species::Kangaskhan: return make_types(Type::Normal);
		case Species::Horsea: return make_types(Type::Water);
		case Species::Seadra: return make_types(Type::Water);
		case Species::Goldeen: return make_types(Type::Water);
		case Species::Seaking: return make_types(Type::Water);
		case Species::Staryu: return make_types(Type::Water);
		case Species::Starmie: return make_types(Type::Water, Type::Psychic);
		case Species::Mr_Mime: return make_types(Type::Psychic);
		case Species::Scyther: return make_types(Type::Bug, Type::Flying);
		case Species::Jynx: return make_types(Type::Ice, Type::Psychic);
		case Species::Electabuzz: return make_types(Type::Electric);
		case Species::Magmar: return make_types(Type::Fire);
		case Species::Pinsir: return make_types(Type::Bug);
		case Species::Tauros: return make_types(Type::Normal);
		case Species::Magikarp: return make_types(Type::Water);
		case Species::Gyarados: return make_types(Type::Water, Type::Flying);
		case Species::Lapras: return make_types(Type::Water, Type::Ice);
		case Species::Ditto: return make_types(Type::Normal);
		case Species::Eevee: return make_types(Type::Normal);
		case Species::Vaporeon: return make_types(Type::Water);
		case Species::Jolteon: return make_types(Type::Electric);
		case Species::Flareon: return make_types(Type::Fire);
		case Species::Porygon: return make_types(Type::Normal);
		case Species::Omanyte: return make_types(Type::Rock, Type::Water);
		case Species::Omastar: return make_types(Type::Rock, Type::Water);
		case Species::Kabuto: return make_types(Type::Rock, Type::Water);
		case Species::Kabutops: return make_types(Type::Rock, Type::Water);
		case Species::Aerodactyl: return make_types(Type::Rock, Type::Flying);
		case Species::Snorlax: return make_types(Type::Normal);
		case Species::Articuno: return make_types(Type::Ice, Type::Flying);
		case Species::Zapdos: return make_types(Type::Electric, Type::Flying);
		case Species::Moltres: return make_types(Type::Fire, Type::Flying);
		case Species::Dratini: return make_types(Type::Dragon);
		case Species::Dragonair: return make_types(Type::Dragon);
		case Species::Dragonite: return make_types(Type::Dragon, Type::Flying);
		case Species::Mewtwo: return make_types(Type::Psychic);
		case Species::Mew: return make_types(Type::Psychic);

		// Generation 2
		case Species::Chikorita: return make_types(Type::Grass);
		case Species::Bayleef: return make_types(Type::Grass);
		case Species::Meganium: return make_types(Type::Grass);
		case Species::Cyndaquil: return make_types(Type::Fire);
		case Species::Quilava: return make_types(Type::Fire);
		case Species::Typhlosion: return make_types(Type::Fire);
		case Species::Totodile: return make_types(Type::Water);
		case Species::Croconaw: return make_types(Type::Water);
		case Species::Feraligatr: return make_types(Type::Water);
		case Species::Sentret: return make_types(Type::Normal);
		case Species::Furret: return make_types(Type::Normal);
		case Species::Hoothoot: return make_types(Type::Normal, Type::Flying);
		case Species::Noctowl: return make_types(Type::Normal, Type::Flying);
		case Species::Ledyba: return make_types(Type::Bug, Type::Flying);
		case Species::Ledian: return make_types(Type::Bug, Type::Flying);
		case Species::Spinarak: return make_types(Type::Bug, Type::Poison);
		case Species::Ariados: return make_types(Type::Bug, Type::Poison);
		case Species::Crobat: return make_types(Type::Poison, Type::Flying);
		case Species::Chinchou: return make_types(Type::Water, Type::Electric);
		case Species::Lanturn: return make_types(Type::Water, Type::Electric);
		case Species::Pichu: return make_types(Type::Electric);
		case Species::Cleffa: return make_types(Type::Normal);
		case Species::Igglybuff: return make_types(Type::Normal);
		case Species::Togepi: return make_types(Type::Normal);
		case Species::Togetic: return make_types(Type::Normal, Type::Flying);
		case Species::Natu: return make_types(Type::Psychic, Type::Flying);
		case Species::Xatu: return make_types(Type::Psychic, Type::Flying);
		case Species::Mareep: return make_types(Type::Electric);
		case Species::Flaaffy: return make_types(Type::Electric);
		case Species::Ampharos: return make_types(Type::Electric);
		case Species::Bellossom: return make_types(Type::Grass);
		case Species::Marill: return make_types(Type::Water);
		case Species::Azumarill: return make_types(Type::Water);
		case Species::Sudowoodo: return make_types(Type::Rock);
		case Species::Politoed: return make_types(Type::Water);
		case Species::Hoppip: return make_types(Type::Grass, Type::Flying);
		case Species::Skiploom: return make_types(Type::Grass, Type::Flying);
		case Species::Jumpluff: return make_types(Type::Grass, Type::Flying);
		case Species::Aipom: return make_types(Type::Normal);
		case Species::Sunkern: return make_types(Type::Grass);
		case Species::Sunflora: return make_types(Type::Grass);
		case Species::Yanma: return make_types(Type::Bug, Type::Flying);
		case Species::Wooper: return make_types(Type::Water, Type::Ground);
		case Species::Quagsire: return make_types(Type::Water, Type::Ground);
		case Species::Espeon: return make_types(Type::Psychic);
		case Species::Umbreon: return make_types(Type::Dark);
		case Species::Murkrow: return make_types(Type::Dark, Type::Flying);
		case Species::Slowking: return make_types(Type::Water, Type::Psychic);
		case Species::Misdreavus: return make_types(Type::Ghost);
		case Species::Unown: return make_types(Type::Psychic);
		case Species::Wobbuffet: return make_types(Type::Psychic);
		case Species::Girafarig: return make_types(Type::Normal, Type::Psychic);
		case Species::Pineco: return make_types(Type::Bug);
		case Species::Forretress: return make_types(Type::Bug, Type::Steel);
		case Species::Dunsparce: return make_types(Type::Normal);
		case Species::Gligar: return make_types(Type::Ground, Type::Flying);
		case Species::Steelix: return make_types(Type::Steel, Type::Ground);
		case Species::Snubbull: return make_types(Type::Normal);
		case Species::Granbull: return make_types(Type::Normal);
		case Species::Qwilfish: return make_types(Type::Water, Type::Poison);
		case Species::Scizor: return make_types(Type::Bug, Type::Steel);
		case Species::Shuckle: return make_types(Type::Bug, Type::Rock);
		case Species::Heracross: return make_types(Type::Bug, Type::Fighting);
		case Species::Sneasel: return make_types(Type::Dark, Type::Ice);
		case Species::Teddiursa: return make_types(Type::Normal);
		case Species::Ursaring: return make_types(Type::Normal);
		case Species::Slugma: return make_types(Type::Fire);
		case Species::Magcargo: return make_types(Type::Fire, Type::Rock);
		case Species::Swinub: return make_types(Type::Ice, Type::Ground);
		case Species::Piloswine: return make_types(Type::Ice, Type::Ground);
		case Species::Corsola: return make_types(Type::Water, Type::Rock);
		case Species::Remoraid: return make_types(Type::Water);
		case Species::Octillery: return make_types(Type::Water);
		case Species::Delibird: return make_types(Type::Ice, Type::Flying);
		case Species::Mantine: return make_types(Type::Water, Type::Flying);
		case Species::Skarmory: return make_types(Type::Steel, Type::Flying);
		case Species::Houndour: return make_types(Type::Dark, Type::Fire);
		case Species::Houndoom: return make_types(Type::Dark, Type::Fire);
		case Species::Kingdra: return make_types(Type::Water, Type::Dragon);
		case Species::Phanpy: return make_types(Type::Ground);
		case Species::Donphan: return make_types(Type::Ground);
		case Species::Porygon2: return make_types(Type::Normal);
		case Species::Stantler: return make_types(Type::Normal);
		case Species::Smeargle: return make_types(Type::Normal);
		case Species::Tyrogue: return make_types(Type::Fighting);
		case Species::Hitmontop: return make_types(Type::Fighting);
		case Species::Smoochum: return make_types(Type::Ice, Type::Psychic);
		case Species::Elekid: return make_types(Type::Electric);
		case Species::Magby: return make_types(Type::Fire);
		case Species::Miltank: return make_types(Type::Normal);
		case Species::Blissey: return make_types(Type::Normal);
		case Species::Raikou: return make_types(Type::Electric);
		case Species::Entei: return make_types(Type::Fire);
		case Species::Suicune: return make_types(Type::Water);
		case Species::Larvitar: return make_types(Type::Rock, Type::Ground);
		case Species::Pupitar: return make_types(Type::Rock, Type::Ground);
		case Species::Tyranitar: return make_types(Type::Rock, Type::Dark);
		case Species::Lugia: return make_types(Type::Psychic, Type::Flying);
		case Species::Ho_Oh: return make_types(Type::Fire, Type::Flying);
		case Species::Celebi: return make_types(Type::Psychic, Type::Grass);

		// Generation 3
		case Species::Treecko: return make_types(Type::Grass);
		case Species::Grovyle: return make_types(Type::Grass);
		case Species::Sceptile: return make_types(Type::Grass);
		case Species::Torchic: return make_types(Type::Fire);
		case Species::Combusken: return make_types(Type::Fire, Type::Fighting);
		case Species::Blaziken: return make_types(Type::Fire, Type::Fighting);
		case Species::Mudkip: return make_types(Type::Water);
		case Species::Marshtomp: return make_types(Type::Water, Type::Ground);
		case Species::Swampert: return make_types(Type::Water, Type::Ground);
		case Species::Poochyena: return make_types(Type::Dark);
		case Species::Mightyena: return make_types(Type::Dark);
		case Species::Zigzagoon: return make_types(Type::Normal);
		case Species::Linoone: return make_types(Type::Normal);
		case Species::Wurmple: return make_types(Type::Bug);
		case Species::Silcoon: return make_types(Type::Bug);
		case Species::Beautifly: return make_types(Type::Bug, Type::Flying);
		case Species::Cascoon: return make_types(Type::Bug);
		case Species::Dustox: return make_types(Type::Bug, Type::Poison);
		case Species::Lotad: return make_types(Type::Water, Type::Grass);
		case Species::Lombre: return make_types(Type::Water, Type::Grass);
		case Species::Ludicolo: return make_types(Type::Water, Type::Grass);
		case Species::Seedot: return make_types(Type::Grass);
		case Species::Nuzleaf: return make_types(Type::Grass, Type::Dark);
		case Species::Shiftry: return make_types(Type::Grass, Type::Dark);
		case Species::Taillow: return make_types(Type::Normal, Type::Flying);
		case Species::Swellow: return make_types(Type::Normal, Type::Flying);
		case Species::Wingull: return make_types(Type::Water, Type::Flying);
		case Species::Pelipper: return make_types(Type::Water, Type::Flying);
		case Species::Ralts: return make_types(Type::Psychic);
		case Species::Kirlia: return make_types(Type::Psychic);
		case Species::Gardevoir: return make_types(Type::Psychic);
		case Species::Surskit: return make_types(Type::Bug, Type::Water);
		case Species::Masquerain: return make_types(Type::Bug, Type::Flying);
		case Species::Shroomish: return make_types(Type::Grass);
		case Species::Breloom: return make_types(Type::Grass, Type::Fighting);
		case Species::Slakoth: return make_types(Type::Normal);
		case Species::Vigoroth: return make_types(Type::Normal);
		case Species::Slaking: return make_types(Type::Normal);
		case Species::Nincada: return make_types(Type::Bug, Type::Ground);
		case Species::Ninjask: return make_types(Type::Bug, Type::Flying);
		case Species::Shedinja: return make_types(Type::Bug, Type::Ghost);
		case Species::Whismur: return make_types(Type::Normal);
		case Species::Loudred: return make_types(Type::Normal);
		case Species::Exploud: return make_types(Type::Normal);
		case Species::Makuhita: return make_types(Type::Fighting);
		case Species::Hariyama: return make_types(Type::Fighting);
		case Species::Azurill: return make_types(Type::Normal);
		case Species::Nosepass: return make_types(Type::Rock);
		case Species::Skitty: return make_types(Type::Normal);
		case Species::Delcatty: return make_types(Type::Normal);
		case Species::Sableye: return make_types(Type::Dark, Type::Ghost);
		case Species::Mawile: return make_types(Type::Steel);
		case Species::Aron: return make_types(Type::Steel, Type::Rock);
		case Species::Lairon: return make_types(Type::Steel, Type::Rock);
		case Species::Aggron: return make_types(Type::Steel, Type::Rock);
		case Species::Meditite: return make_types(Type::Fighting, Type::Psychic);
		case Species::Medicham: return make_types(Type::Fighting, Type::Psychic);
		case Species::Electrike: return make_types(Type::Electric);
		case Species::Manectric: return make_types(Type::Electric);
		case Species::Plusle: return make_types(Type::Electric);
		case Species::Minun: return make_types(Type::Electric);
		case Species::Volbeat: return make_types(Type::Bug);
		case Species::Illumise: return make_types(Type::Bug);
		case Species::Roselia: return make_types(Type::Grass, Type::Poison);
		case Species::Gulpin: return make_types(Type::Poison);
		case Species::Swalot: return make_types(Type::Poison);
		case Species::Carvanha: return make_types(Type::Water, Type::Dark);
		case Species::Sharpedo: return make_types(Type::Water, Type::Dark);
		case Species::Wailmer: return make_types(Type::Water);
		case Species::Wailord: return make_types(Type::Water);
		case Species::Numel: return make_types(Type::Fire, Type::Ground);
		case Species::Camerupt: return make_types(Type::Fire, Type::Ground);
		case Species::Torkoal: return make_types(Type::Fire);
		case Species::Spoink: return make_types(Type::Psychic);
		case Species::Grumpig: return make_types(Type::Psychic);
		case Species::Spinda: return make_types(Type::Normal);
		case Species::Trapinch: return make_types(Type::Ground);
		case Species::Vibrava: return make_types(Type::Ground, Type::Dragon);
		case Species::Flygon: return make_types(Type::Ground, Type::Dragon);
		case Species::Cacnea: return make_types(Type::Grass);
		case Species::Cacturne: return make_types(Type::Grass, Type::Dark);
		case Species::Swablu: return make_types(Type::Normal, Type::Flying);
		case Species::Altaria: return make_types(Type::Dragon, Type::Flying);
		case Species::Zangoose: return make_types(Type::Normal);
		case Species::Seviper: return make_types(Type::Poison);
		case Species::Lunatone: return make_types(Type::Rock, Type::Psychic);
		case Species::Solrock: return make_types(Type::Rock, Type::Psychic);
		case Species::Barboach: return make_types(Type::Water, Type::Ground);
		case Species::Whiscash: return make_types(Type::Water, Type::Ground);
		case Species::Corphish: return make_types(Type::Water);
		case Species::Crawdaunt: return make_types(Type::Water, Type::Dark);
		case Species::Baltoy: return make_types(Type::Ground, Type::Psychic);
		case Species::Claydol: return make_types(Type::Ground, Type::Psychic);
		case Species::Lileep: return make_types(Type::Rock, Type::Grass);
		case Species::Cradily: return make_types(Type::Rock, Type::Grass);
		case Species::Anorith: return make_types(Type::Rock, Type::Bug);
		case Species::Armaldo: return make_types(Type::Rock, Type::Bug);
		case Species::Feebas: return make_types(Type::Water);
		case Species::Milotic: return make_types(Type::Water);
		case Species::Castform: return make_types(Type::Normal);
			// return make_types(Type::Fire);
			// return make_types(Type::Water);
			// return make_types(Type::Ice);
		case Species::Kecleon: return make_types(Type::Normal);
		case Species::Shuppet: return make_types(Type::Ghost);
		case Species::Banette: return make_types(Type::Ghost);
		case Species::Duskull: return make_types(Type::Ghost);
		case Species::Dusclops: return make_types(Type::Ghost);
		case Species::Tropius: return make_types(Type::Grass, Type::Flying);
		case Species::Chimecho: return make_types(Type::Psychic);
		case Species::Absol: return make_types(Type::Dark);
		case Species::Wynaut: return make_types(Type::Psychic);
		case Species::Snorunt: return make_types(Type::Ice);
		case Species::Glalie: return make_types(Type::Ice);
		case Species::Spheal: return make_types(Type::Ice, Type::Water);
		case Species::Sealeo: return make_types(Type::Ice, Type::Water);
		case Species::Walrein: return make_types(Type::Ice, Type::Water);
		case Species::Clamperl: return make_types(Type::Water);
		case Species::Huntail: return make_types(Type::Water);
		case Species::Gorebyss: return make_types(Type::Water);
		case Species::Relicanth: return make_types(Type::Water, Type::Rock);
		case Species::Luvdisc: return make_types(Type::Water);
		case Species::Bagon: return make_types(Type::Dragon);
		case Species::Shelgon: return make_types(Type::Dragon);
		case Species::Salamence: return make_types(Type::Dragon, Type::Flying);
		case Species::Beldum: return make_types(Type::Steel, Type::Psychic);
		case Species::Metang: return make_types(Type::Steel, Type::Psychic);
		case Species::Metagross: return make_types(Type::Steel, Type::Psychic);
		case Species::Regirock: return make_types(Type::Rock);
		case Species::Regice: return make_types(Type::Ice);
		case Species::Registeel: return make_types(Type::Steel);
		case Species::Latias: return make_types(Type::Dragon, Type::Psychic);
		case Species::Latios: return make_types(Type::Dragon, Type::Psychic);
		case Species::Kyogre: return make_types(Type::Water);
		case Species::Groudon: return make_types(Type::Ground);
		case Species::Rayquaza: return make_types(Type::Dragon, Type::Flying);
		case Species::Jirachi: return make_types(Type::Steel, Type::Psychic);
		case Species::Deoxys_Mediocre: return make_types(Type::Psychic);
		case Species::Deoxys_Attack: return make_types(Type::Psychic);
		case Species::Deoxys_Defense: return make_types(Type::Psychic);
		case Species::Deoxys_Speed: return make_types(Type::Psychic);

		// Generation 4
		case Species::Turtwig: return make_types(Type::Grass);
		case Species::Grotle: return make_types(Type::Grass);
		case Species::Torterra: return make_types(Type::Grass, Type::Ground);
		case Species::Chimchar: return make_types(Type::Fire);
		case Species::Monferno: return make_types(Type::Fire, Type::Fighting);
		case Species::Infernape: return make_types(Type::Fire, Type::Fighting);
		case Species::Piplup: return make_types(Type::Water);
		case Species::Prinplup: return make_types(Type::Water);
		case Species::Empoleon: return make_types(Type::Water, Type::Steel);
		case Species::Starly: return make_types(Type::Normal, Type::Flying);
		case Species::Staravia: return make_types(Type::Normal, Type::Flying);
		case Species::Staraptor: return make_types(Type::Normal, Type::Flying);
		case Species::Bidoof: return make_types(Type::Normal);
		case Species::Bibarel: return make_types(Type::Normal, Type::Water);
		case Species::Kricketot: return make_types(Type::Bug);
		case Species::Kricketune: return make_types(Type::Bug);
		case Species::Shinx: return make_types(Type::Electric);
		case Species::Luxio: return make_types(Type::Electric);
		case Species::Luxray: return make_types(Type::Electric);
		case Species::Budew: return make_types(Type::Grass, Type::Poison);
		case Species::Roserade: return make_types(Type::Grass, Type::Poison);
		case Species::Cranidos: return make_types(Type::Rock);
		case Species::Rampardos: return make_types(Type::Rock);
		case Species::Shieldon: return make_types(Type::Rock, Type::Steel);
		case Species::Bastiodon: return make_types(Type::Rock, Type::Steel);
		case Species::Burmy: return make_types(Type::Bug);
		case Species::Wormadam_Plant: return make_types(Type::Bug, Type::Grass);
		case Species::Wormadam_Sandy: return make_types(Type::Bug, Type::Ground);
		case Species::Wormadam_Trash: return make_types(Type::Bug, Type::Steel);
		case Species::Mothim: return make_types(Type::Bug, Type::Flying);
		case Species::Combee: return make_types(Type::Bug, Type::Flying);
		case Species::Vespiquen: return make_types(Type::Bug, Type::Flying);
		case Species::Pachirisu: return make_types(Type::Electric);
		case Species::Buizel: return make_types(Type::Water);
		case Species::Floatzel: return make_types(Type::Water);
		case Species::Cherubi: return make_types(Type::Grass);
		case Species::Cherrim: return make_types(Type::Grass);
		case Species::Shellos: return make_types(Type::Water);
		case Species::Gastrodon: return make_types(Type::Water, Type::Ground);
		case Species::Ambipom: return make_types(Type::Normal);
		case Species::Drifloon: return make_types(Type::Ghost, Type::Flying);
		case Species::Drifblim: return make_types(Type::Ghost, Type::Flying);
		case Species::Buneary: return make_types(Type::Normal);
		case Species::Lopunny: return make_types(Type::Normal);
		case Species::Mismagius: return make_types(Type::Ghost);
		case Species::Honchkrow: return make_types(Type::Dark, Type::Flying);
		case Species::Glameow: return make_types(Type::Normal);
		case Species::Purugly: return make_types(Type::Normal);
		case Species::Chingling: return make_types(Type::Psychic);
		case Species::Stunky: return make_types(Type::Poison, Type::Dark);
		case Species::Skuntank: return make_types(Type::Poison, Type::Dark);
		case Species::Bronzor: return make_types(Type::Steel, Type::Psychic);
		case Species::Bronzong: return make_types(Type::Steel, Type::Psychic);
		case Species::Bonsly: return make_types(Type::Rock);
		case Species::Mime_Jr: return make_types(Type::Psychic);
		case Species::Happiny: return make_types(Type::Normal);
		case Species::Chatot: return make_types(Type::Normal, Type::Flying);
		case Species::Spiritomb: return make_types(Type::Ghost, Type::Dark);
		case Species::Gible: return make_types(Type::Dragon, Type::Ground);
		case Species::Gabite: return make_types(Type::Dragon, Type::Ground);
		case Species::Garchomp: return make_types(Type::Dragon, Type::Ground);
		case Species::Munchlax: return make_types(Type::Normal);
		case Species::Riolu: return make_types(Type::Fighting);
		case Species::Lucario: return make_types(Type::Fighting, Type::Steel);
		case Species::Hippopotas: return make_types(Type::Ground);
		case Species::Hippowdon: return make_types(Type::Ground);
		case Species::Skorupi: return make_types(Type::Poison, Type::Bug);
		case Species::Drapion: return make_types(Type::Poison, Type::Dark);
		case Species::Croagunk: return make_types(Type::Poison, Type::Fighting);
		case Species::Toxicroak: return make_types(Type::Poison, Type::Fighting);
		case Species::Carnivine: return make_types(Type::Grass);
		case Species::Finneon: return make_types(Type::Water);
		case Species::Lumineon: return make_types(Type::Water);
		case Species::Mantyke: return make_types(Type::Water, Type::Flying);
		case Species::Snover: return make_types(Type::Grass, Type::Ice);
		case Species::Abomasnow: return make_types(Type::Grass, Type::Ice);
		case Species::Weavile: return make_types(Type::Dark, Type::Ice);
		case Species::Magnezone: return make_types(Type::Electric, Type::Steel);
		case Species::Lickilicky: return make_types(Type::Normal);
		case Species::Rhyperior: return make_types(Type::Ground, Type::Rock);
		case Species::Tangrowth: return make_types(Type::Grass);
		case Species::Electivire: return make_types(Type::Electric);
		case Species::Magmortar: return make_types(Type::Fire);
		case Species::Togekiss: return make_types(Type::Normal, Type::Flying);
		case Species::Yanmega: return make_types(Type::Bug, Type::Flying);
		case Species::Leafeon: return make_types(Type::Grass);
		case Species::Glaceon: return make_types(Type::Ice);
		case Species::Gliscor: return make_types(Type::Ground, Type::Flying);
		case Species::Mamoswine: return make_types(Type::Ice, Type::Ground);
		case Species::Porygon_Z: return make_types(Type::Normal);
		case Species::Gallade: return make_types(Type::Psychic, Type::Fighting);
		case Species::Probopass: return make_types(Type::Rock, Type::Steel);
		case Species::Dusknoir: return make_types(Type::Ghost);
		case Species::Froslass: return make_types(Type::Ice, Type::Ghost);
		case Species::Rotom: return make_types(Type::Electric, Type::Ghost);
			// return make_types(Type::Electric, Type::Fire);
		case Species::Rotom_Heat: return make_types(Type::Electric, Type::Ghost);
			// return make_types(Type::Electric, Type::Water);
		case Species::Rotom_Wash: return make_types(Type::Electric, Type::Ghost);
			// return make_types(Type::Electric, Type::Ice);
		case Species::Rotom_Frost: return make_types(Type::Electric, Type::Ghost);
			// return make_types(Type::Electric, Type::Flying);
		case Species::Rotom_Fan: return make_types(Type::Electric, Type::Ghost);
			// return make_types(Type::Electric, Type::Grass);
		case Species::Rotom_Mow: return make_types(Type::Electric, Type::Ghost);
		case Species::Uxie: return make_types(Type::Psychic);
		case Species::Mesprit: return make_types(Type::Psychic);
		case Species::Azelf: return make_types(Type::Psychic);
		case Species::Dialga: return make_types(Type::Steel, Type::Dragon);
		case Species::Palkia: return make_types(Type::Water, Type::Dragon);
		case Species::Heatran: return make_types(Type::Fire, Type::Steel);
		case Species::Regigigas: return make_types(Type::Normal);
		case Species::Giratina_Altered: return make_types(Type::Ghost, Type::Dragon);
		case Species::Giratina_Origin: return make_types(Type::Ghost, Type::Dragon);
		case Species::Cresselia: return make_types(Type::Psychic);
		case Species::Phione: return make_types(Type::Water);
		case Species::Manaphy: return make_types(Type::Water);
		case Species::Darkrai: return make_types(Type::Dark);
		case Species::Shaymin_Land: return make_types(Type::Grass);
		case Species::Shaymin_Sky: return make_types(Type::Grass, Type::Flying);
		case Species::Arceus: return make_types(Type::Normal);

		// Generation 5
		case Species::Victini: return make_types(Type::Psychic, Type::Fire);
		case Species::Snivy: return make_types(Type::Grass);
		case Species::Servine: return make_types(Type::Grass);
		case Species::Serperior: return make_types(Type::Grass);
		case Species::Tepig: return make_types(Type::Fire);
		case Species::Pignite: return make_types(Type::Fire, Type::Fighting);
		case Species::Emboar: return make_types(Type::Fire, Type::Fighting);
		case Species::Oshawott: return make_types(Type::Water);
		case Species::Dewott: return make_types(Type::Water);
		case Species::Samurott: return make_types(Type::Water);
		case Species::Patrat: return make_types(Type::Normal);
		case Species::Watchog: return make_types(Type::Normal);
		case Species::Lillipup: return make_types(Type::Normal);
		case Species::Herdier: return make_types(Type::Normal);
		case Species::Stoutland: return make_types(Type::Normal);
		case Species::Purrloin: return make_types(Type::Dark);
		case Species::Liepard: return make_types(Type::Dark);
		case Species::Pansage: return make_types(Type::Grass);
		case Species::Simisage: return make_types(Type::Grass);
		case Species::Pansear: return make_types(Type::Fire);
		case Species::Simisear: return make_types(Type::Fire);
		case Species::Panpour: return make_types(Type::Water);
		case Species::Simipour: return make_types(Type::Water);
		case Species::Munna: return make_types(Type::Psychic);
		case Species::Musharna: return make_types(Type::Psychic);
		case Species::Pidove: return make_types(Type::Normal, Type::Flying);
		case Species::Tranquill: return make_types(Type::Normal, Type::Flying);
		case Species::Unfezant: return make_types(Type::Normal, Type::Flying);
		case Species::Blitzle: return make_types(Type::Electric);
		case Species::Zebstrika: return make_types(Type::Electric);
		case Species::Roggenrola: return make_types(Type::Rock);
		case Species::Boldore: return make_types(Type::Rock);
		case Species::Gigalith: return make_types(Type::Rock);
		case Species::Woobat: return make_types(Type::Psychic, Type::Flying);
		case Species::Swoobat: return make_types(Type::Psychic, Type::Flying);
		case Species::Drilbur: return make_types(Type::Ground);
		case Species::Excadrill: return make_types(Type::Ground, Type::Steel);
		case Species::Audino: return make_types(Type::Normal);
		case Species::Timburr: return make_types(Type::Fighting);
		case Species::Gurdurr: return make_types(Type::Fighting);
		case Species::Conkeldurr: return make_types(Type::Fighting);
		case Species::Tympole: return make_types(Type::Water);
		case Species::Palpitoad: return make_types(Type::Water, Type::Ground);
		case Species::Seismitoad: return make_types(Type::Water, Type::Ground);
		case Species::Throh: return make_types(Type::Fighting);
		case Species::Sawk: return make_types(Type::Fighting);
		case Species::Sewaddle: return make_types(Type::Bug, Type::Grass);
		case Species::Swadloon: return make_types(Type::Bug, Type::Grass);
		case Species::Leavanny: return make_types(Type::Bug, Type::Grass);
		case Species::Venipede: return make_types(Type::Bug, Type::Poison);
		case Species::Whirlipede: return make_types(Type::Bug, Type::Poison);
		case Species::Scolipede: return make_types(Type::Bug, Type::Poison);
		case Species::Cottonee: return make_types(Type::Grass);
		case Species::Whimsicott: return make_types(Type::Grass);
		case Species::Petilil: return make_types(Type::Grass);
		case Species::Lilligant: return make_types(Type::Grass);
		case Species::Basculin_Red: return make_types(Type::Water);
		case Species::Basculin_Blue: return make_types(Type::Water);
		case Species::Sandile: return make_types(Type::Ground, Type::Dark);
		case Species::Krokorok: return make_types(Type::Ground, Type::Dark);
		case Species::Krookodile: return make_types(Type::Ground, Type::Dark);
		case Species::Darumaka: return make_types(Type::Fire);
		case Species::Darmanitan: return make_types(Type::Fire);
		// case Species::Darmanitan_Zen: return make_types(Type::Fire, Type::Psychic);
		case Species::Maractus: return make_types(Type::Grass);
		case Species::Dwebble: return make_types(Type::Bug, Type::Rock);
		case Species::Crustle: return make_types(Type::Bug, Type::Rock);
		case Species::Scraggy: return make_types(Type::Dark, Type::Fighting);
		case Species::Scrafty: return make_types(Type::Dark, Type::Fighting);
		case Species::Sigilyph: return make_types(Type::Psychic, Type::Flying);
		case Species::Yamask: return make_types(Type::Ghost);
		case Species::Cofagrigus: return make_types(Type::Ghost);
		case Species::Tirtouga: return make_types(Type::Water, Type::Rock);
		case Species::Carracosta: return make_types(Type::Water, Type::Rock);
		case Species::Archen: return make_types(Type::Rock, Type::Flying);
		case Species::Archeops: return make_types(Type::Rock, Type::Flying);
		case Species::Trubbish: return make_types(Type::Poison);
		case Species::Garbodor: return make_types(Type::Poison);
		case Species::Zorua: return make_types(Type::Dark);
		case Species::Zoroark: return make_types(Type::Dark);
		case Species::Minccino: return make_types(Type::Normal);
		case Species::Cinccino: return make_types(Type::Normal);
		case Species::Gothita: return make_types(Type::Psychic);
		case Species::Gothorita: return make_types(Type::Psychic);
		case Species::Gothitelle: return make_types(Type::Psychic);
		case Species::Solosis: return make_types(Type::Psychic);
		case Species::Duosion: return make_types(Type::Psychic);
		case Species::Reuniclus: return make_types(Type::Psychic);
		case Species::Ducklett: return make_types(Type::Water, Type::Flying);
		case Species::Swanna: return make_types(Type::Water, Type::Flying);
		case Species::Vanillite: return make_types(Type::Ice);
		case Species::Vanillish: return make_types(Type::Ice);
		case Species::Vanilluxe: return make_types(Type::Ice);
		case Species::Deerling: return make_types(Type::Normal, Type::Grass);
		case Species::Sawsbuck: return make_types(Type::Normal, Type::Grass);
		case Species::Emolga: return make_types(Type::Electric, Type::Flying);
		case Species::Karrablast: return make_types(Type::Bug);
		case Species::Escavalier: return make_types(Type::Bug, Type::Steel);
		case Species::Foongus: return make_types(Type::Grass, Type::Poison);
		case Species::Amoonguss: return make_types(Type::Grass, Type::Poison);
		case Species::Frillish: return make_types(Type::Water, Type::Ghost);
		case Species::Jellicent: return make_types(Type::Water, Type::Ghost);
		case Species::Alomomola: return make_types(Type::Water);
		case Species::Joltik: return make_types(Type::Bug, Type::Electric);
		case Species::Galvantula: return make_types(Type::Bug, Type::Electric);
		case Species::Ferroseed: return make_types(Type::Grass, Type::Steel);
		case Species::Ferrothorn: return make_types(Type::Grass, Type::Steel);
		case Species::Klink: return make_types(Type::Steel);
		case Species::Klang: return make_types(Type::Steel);
		case Species::Klinklang: return make_types(Type::Steel);
		case Species::Tynamo: return make_types(Type::Electric);
		case Species::Eelektrik: return make_types(Type::Electric);
		case Species::Eelektross: return make_types(Type::Electric);
		case Species::Elgyem: return make_types(Type::Psychic);
		case Species::Beheeyem: return make_types(Type::Psychic);
		case Species::Litwick: return make_types(Type::Ghost, Type::Fire);
		case Species::Lampent: return make_types(Type::Ghost, Type::Fire);
		case Species::Chandelure: return make_types(Type::Ghost, Type::Fire);
		case Species::Axew: return make_types(Type::Dragon);
		case Species::Fraxure: return make_types(Type::Dragon);
		case Species::Haxorus: return make_types(Type::Dragon);
		case Species::Cubchoo: return make_types(Type::Ice);
		case Species::Beartic: return make_types(Type::Ice);
		case Species::Cryogonal: return make_types(Type::Ice);
		case Species::Shelmet: return make_types(Type::Bug);
		case Species::Accelgor: return make_types(Type::Bug);
		case Species::Stunfisk: return make_types(Type::Ground, Type::Electric);
		case Species::Mienfoo: return make_types(Type::Fighting);
		case Species::Mienshao: return make_types(Type::Fighting);
		case Species::Druddigon: return make_types(Type::Dragon);
		case Species::Golett: return make_types(Type::Ground, Type::Ghost);
		case Species::Golurk: return make_types(Type::Ground, Type::Ghost);
		case Species::Pawniard: return make_types(Type::Dark, Type::Steel);
		case Species::Bisharp: return make_types(Type::Dark, Type::Steel);
		case Species::Bouffalant: return make_types(Type::Normal);
		case Species::Rufflet: return make_types(Type::Normal, Type::Flying);
		case Species::Braviary: return make_types(Type::Normal, Type::Flying);
		case Species::Vullaby: return make_types(Type::Dark, Type::Flying);
		case Species::Mandibuzz: return make_types(Type::Dark, Type::Flying);
		case Species::Heatmor: return make_types(Type::Fire);
		case Species::Durant: return make_types(Type::Bug, Type::Steel);
		case Species::Deino: return make_types(Type::Dark, Type::Dragon);
		case Species::Zweilous: return make_types(Type::Dark, Type::Dragon);
		case Species::Hydreigon: return make_types(Type::Dark, Type::Dragon);
		case Species::Larvesta: return make_types(Type::Bug, Type::Fire);
		case Species::Volcarona: return make_types(Type::Bug, Type::Fire);
		case Species::Cobalion: return make_types(Type::Steel, Type::Fighting);
		case Species::Terrakion: return make_types(Type::Rock, Type::Fighting);
		case Species::Virizion: return make_types(Type::Grass, Type::Fighting);
		case Species::Tornadus_Incarnate: return make_types(Type::Flying);
		case Species::Tornadus_Therian: return make_types(Type::Flying);
		case Species::Thundurus_Incarnate: return make_types(Type::Electric, Type::Flying);
		case Species::Thundurus_Therian: return make_types(Type::Electric, Type::Flying);
		case Species::Reshiram: return make_types(Type::Dragon, Type::Fire);
		case Species::Zekrom: return make_types(Type::Dragon, Type::Electric);
		case Species::Landorus_Incarnate: return make_types(Type::Ground, Type::Flying);
		case Species::Landorus_Therian: return make_types(Type::Ground, Type::Flying);
		case Species::Kyurem: return make_types(Type::Dragon, Type::Ice);
		case Species::Kyurem_Black: return make_types(Type::Dragon, Type::Ice);
		case Species::Kyurem_White: return make_types(Type::Dragon, Type::Ice);
		case Species::Keldeo: return make_types(Type::Water, Type::Fighting);
		case Species::Meloetta: return make_types(Type::Normal, Type::Psychic);
		// case Species::Meloetta_Pirouette: return make_types(Type::Normal, Type::Fighting);
		case Species::Genesect: return make_types(Type::Bug, Type::Steel);
	}
}

}	// namespace

TypeCollection::TypeCollection(Species const species):
	types(get_type(species)) {
}

auto is_immune_to_sandstorm(TypeCollection const collection) -> bool {
	return containers::any(collection.types, [](auto const type) { return is_immune_to_sandstorm(type); });
}

auto is_immune_to_hail(TypeCollection const collection) -> bool {
	return containers::any(collection.types, [](auto const type) { return is_immune_to_hail(type); });
}

auto TypeCollection::change_type(Type const type) -> void {
	types = make_types(type);
}

auto is_type(Pokemon const & pokemon, Type const type, bool const roosting) -> bool {
	return (type != Type::Flying or !roosting) and containers::any_equal(get_type(pokemon).types, type);
}

namespace {

auto forced_grounded(Pokemon const & pokemon, Weather const weather) -> bool {
	return weather.gravity() or grounds(get_item(pokemon));
}

auto is_permanently_immune_to_ground(auto const & pokemon, bool const roosting) -> bool {
	return
		is_type(pokemon, Type::Flying, roosting) or
		is_immune_to_ground(get_ability(pokemon));
}

}	// namespace

auto grounded(MutableActivePokemon const pokemon, Weather const weather) -> bool {
	return grounded(static_cast<ActivePokemon>(pokemon), weather);
}
auto grounded(ActivePokemon const pokemon, Weather const weather) -> bool {
	return
		!(is_permanently_immune_to_ground(pokemon, pokemon.is_roosting()) or pokemon.magnet_rise().is_active()) or
		forced_grounded(pokemon, weather) or
		pokemon.ingrained();
}
auto grounded(Pokemon const & pokemon, Weather const weather) -> bool {
	return
		!is_permanently_immune_to_ground(pokemon, false) or
		forced_grounded(pokemon, weather);
}


}	// namespace technicalmachine
