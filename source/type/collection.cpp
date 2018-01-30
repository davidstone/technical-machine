// Type function definitions
// Copyright (C) 2016 David Stone
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

#include "collection.hpp"

#include "type.hpp"

#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {
using namespace detail_type_collection;

namespace {
auto get_type(Species const name) -> TypeArray;
}	// namespace

TypeCollection::TypeCollection(Species const name):
	types(get_type(name)) {
}

auto is_immune_to_sandstorm(TypeCollection const collection) -> bool {
	for (Type const type : collection.types) {
		if (is_immune_to_sandstorm(type)) {
			return true;
		}
	}
	return false;
}

auto is_immune_to_hail(TypeCollection const collection) -> bool {
	for (Type const type : collection.types) {
		if (is_immune_to_hail(type)) {
			return true;
		}
	}
	return false;
}

auto TypeCollection::change_type(Type const type) -> void {
	types = TypeArray(type);
}

auto is_type(Pokemon const & pokemon, Type const type, bool const roosting) -> bool {
	if (type != Type::Flying or !roosting) {
		for (Type const check : get_type(pokemon).types) {
			if (check == type) {
				return true;
			}
		}
	}
	return false;
}

namespace {

// If it is not an ActivePokemon, these flags cannot apply
template<typename T>
constexpr auto ingrained(T const &) {
	static_assert(!std::is_same<std::decay_t<T>, ActivePokemon>::value, "Not valid with ActivePokemon.");
	return false;
}
template<typename T>
constexpr auto is_roosting(T const &) {
	static_assert(!std::is_same<std::decay_t<T>, ActivePokemon>::value, "Not valid with ActivePokemon.");
	return false;
}
auto is_magnet_rising(ActivePokemon const pokemon) {
	return magnet_rise(pokemon).is_active();
}
constexpr auto is_magnet_rising(Pokemon const &) {
	return false;
}

template<typename PossiblyActivePokemon>
auto forced_grounded(PossiblyActivePokemon const & pokemon, Weather const weather) -> bool {
	return weather.gravity() or grounds(get_item(pokemon)) or ingrained(pokemon);
}

template<typename PossiblyActivePokemon>
auto is_immune_to_ground(PossiblyActivePokemon const & pokemon) -> bool {
	return
		is_type(pokemon, Type::Flying, is_roosting(pokemon)) or
		get_ability(pokemon).is_immune_to_ground() or
		is_magnet_rising(pokemon)
	;
}

}	// namespace

auto grounded(MutableActivePokemon pokemon, Weather const weather) -> bool {
	return grounded(static_cast<ActivePokemon>(pokemon), weather);
}
auto grounded(ActivePokemon const pokemon, Weather const weather) -> bool {
	return !is_immune_to_ground(pokemon) or forced_grounded(pokemon, weather);
}
auto grounded(Pokemon const & pokemon, Weather const weather) -> bool {
	return !is_immune_to_ground(pokemon) or forced_grounded(pokemon, weather);
}


namespace {

auto get_type(Species const name) -> TypeArray {
	switch (name) {
		// Generation 1
		case Species::Bulbasaur: return {Type::Grass, Type::Poison};
		case Species::Ivysaur: return {Type::Grass, Type::Poison};
		case Species::Venusaur: return {Type::Grass, Type::Poison};
		case Species::Charmander: return {Type::Fire};
		case Species::Charmeleon: return {Type::Fire};
		case Species::Charizard: return {Type::Fire, Type::Flying};
		case Species::Squirtle: return {Type::Water};
		case Species::Wartortle: return {Type::Water};
		case Species::Blastoise: return {Type::Water};
		case Species::Caterpie: return {Type::Bug};
		case Species::Metapod: return {Type::Bug};
		case Species::Butterfree: return {Type::Bug, Type::Flying};
		case Species::Weedle: return {Type::Bug, Type::Poison};
		case Species::Kakuna: return {Type::Bug, Type::Poison};
		case Species::Beedrill: return {Type::Bug, Type::Poison};
		case Species::Pidgey: return {Type::Normal, Type::Flying};
		case Species::Pidgeotto: return {Type::Normal, Type::Flying};
		case Species::Pidgeot: return {Type::Normal, Type::Flying};
		case Species::Rattata: return {Type::Normal};
		case Species::Raticate: return {Type::Normal};
		case Species::Spearow: return {Type::Normal, Type::Flying};
		case Species::Fearow: return {Type::Normal, Type::Flying};
		case Species::Ekans: return {Type::Poison};
		case Species::Arbok: return {Type::Poison};
		case Species::Pikachu: return {Type::Electric};
		case Species::Raichu: return {Type::Electric};
		case Species::Sandshrew: return {Type::Ground};
		case Species::Sandslash: return {Type::Ground};
		case Species::Nidoran_F: return {Type::Poison};
		case Species::Nidorina: return {Type::Poison};
		case Species::Nidoqueen: return {Type::Poison, Type::Ground};
		case Species::Nidoran_M: return {Type::Poison};
		case Species::Nidorino: return {Type::Poison};
		case Species::Nidoking: return {Type::Poison, Type::Ground};
		case Species::Clefairy: return {Type::Normal};
		case Species::Clefable: return {Type::Normal};
		case Species::Vulpix: return {Type::Fire};
		case Species::Ninetales: return {Type::Fire};
		case Species::Jigglypuff: return {Type::Normal};
		case Species::Wigglytuff: return {Type::Normal};
		case Species::Zubat: return {Type::Poison, Type::Flying};
		case Species::Golbat: return {Type::Poison, Type::Flying};
		case Species::Oddish: return {Type::Grass, Type::Poison};
		case Species::Gloom: return {Type::Grass, Type::Poison};
		case Species::Vileplume: return {Type::Grass, Type::Poison};
		case Species::Paras: return {Type::Bug, Type::Grass};
		case Species::Parasect: return {Type::Bug, Type::Grass};
		case Species::Venonat: return {Type::Bug, Type::Poison};
		case Species::Venomoth: return {Type::Bug, Type::Poison};
		case Species::Diglett: return {Type::Ground};
		case Species::Dugtrio: return {Type::Ground};
		case Species::Meowth: return {Type::Normal};
		case Species::Persian: return {Type::Normal};
		case Species::Psyduck: return {Type::Water};
		case Species::Golduck: return {Type::Water};
		case Species::Mankey: return {Type::Fighting};
		case Species::Primeape: return {Type::Fighting};
		case Species::Growlithe: return {Type::Fire};
		case Species::Arcanine: return {Type::Fire};
		case Species::Poliwag: return {Type::Water};
		case Species::Poliwhirl: return {Type::Water};
		case Species::Poliwrath: return {Type::Water, Type::Fighting};
		case Species::Abra: return {Type::Psychic};
		case Species::Kadabra: return {Type::Psychic};
		case Species::Alakazam: return {Type::Psychic};
		case Species::Machop: return {Type::Fighting};
		case Species::Machoke: return {Type::Fighting};
		case Species::Machamp: return {Type::Fighting};
		case Species::Bellsprout: return {Type::Grass, Type::Poison};
		case Species::Weepinbell: return {Type::Grass, Type::Poison};
		case Species::Victreebel: return {Type::Grass, Type::Poison};
		case Species::Tentacool: return {Type::Water, Type::Poison};
		case Species::Tentacruel: return {Type::Water, Type::Poison};
		case Species::Geodude: return {Type::Rock, Type::Ground};
		case Species::Graveler: return {Type::Rock, Type::Ground};
		case Species::Golem: return {Type::Rock, Type::Ground};
		case Species::Ponyta: return {Type::Fire};
		case Species::Rapidash: return {Type::Fire};
		case Species::Slowpoke: return {Type::Water, Type::Psychic};
		case Species::Slowbro: return {Type::Water, Type::Psychic};
		case Species::Magnemite: return {Type::Electric, Type::Steel};
		case Species::Magneton: return {Type::Electric, Type::Steel};
		case Species::Farfetchd: return {Type::Normal, Type::Flying};
		case Species::Doduo: return {Type::Normal, Type::Flying};
		case Species::Dodrio: return {Type::Normal, Type::Flying};
		case Species::Seel: return {Type::Water};
		case Species::Dewgong: return {Type::Water, Type::Ice};
		case Species::Grimer: return {Type::Poison};
		case Species::Muk: return {Type::Poison};
		case Species::Shellder: return {Type::Water};
		case Species::Cloyster: return {Type::Water, Type::Ice};
		case Species::Gastly: return {Type::Ghost, Type::Poison};
		case Species::Haunter: return {Type::Ghost, Type::Poison};
		case Species::Gengar: return {Type::Ghost, Type::Poison};
		case Species::Onix: return {Type::Rock, Type::Ground};
		case Species::Drowzee: return {Type::Psychic};
		case Species::Hypno: return {Type::Psychic};
		case Species::Krabby: return {Type::Water};
		case Species::Kingler: return {Type::Water};
		case Species::Voltorb: return {Type::Electric};
		case Species::Electrode: return {Type::Electric};
		case Species::Exeggcute: return {Type::Grass, Type::Psychic};
		case Species::Exeggutor: return {Type::Grass, Type::Psychic};
		case Species::Cubone: return {Type::Ground};
		case Species::Marowak: return {Type::Ground};
		case Species::Hitmonlee: return {Type::Fighting};
		case Species::Hitmonchan: return {Type::Fighting};
		case Species::Lickitung: return {Type::Normal};
		case Species::Koffing: return {Type::Poison};
		case Species::Weezing: return {Type::Poison};
		case Species::Rhyhorn: return {Type::Ground, Type::Rock};
		case Species::Rhydon: return {Type::Ground, Type::Rock};
		case Species::Chansey: return {Type::Normal};
		case Species::Tangela: return {Type::Grass};
		case Species::Kangaskhan: return {Type::Normal};
		case Species::Horsea: return {Type::Water};
		case Species::Seadra: return {Type::Water};
		case Species::Goldeen: return {Type::Water};
		case Species::Seaking: return {Type::Water};
		case Species::Staryu: return {Type::Water};
		case Species::Starmie: return {Type::Water, Type::Psychic};
		case Species::Mr_Mime: return {Type::Psychic};
		case Species::Scyther: return {Type::Bug, Type::Flying};
		case Species::Jynx: return {Type::Ice, Type::Psychic};
		case Species::Electabuzz: return {Type::Electric};
		case Species::Magmar: return {Type::Fire};
		case Species::Pinsir: return {Type::Bug};
		case Species::Tauros: return {Type::Normal};
		case Species::Magikarp: return {Type::Water};
		case Species::Gyarados: return {Type::Water, Type::Flying};
		case Species::Lapras: return {Type::Water, Type::Ice};
		case Species::Ditto: return {Type::Normal};
		case Species::Eevee: return {Type::Normal};
		case Species::Vaporeon: return {Type::Water};
		case Species::Jolteon: return {Type::Electric};
		case Species::Flareon: return {Type::Fire};
		case Species::Porygon: return {Type::Normal};
		case Species::Omanyte: return {Type::Rock, Type::Water};
		case Species::Omastar: return {Type::Rock, Type::Water};
		case Species::Kabuto: return {Type::Rock, Type::Water};
		case Species::Kabutops: return {Type::Rock, Type::Water};
		case Species::Aerodactyl: return {Type::Rock, Type::Flying};
		case Species::Snorlax: return {Type::Normal};
		case Species::Articuno: return {Type::Ice, Type::Flying};
		case Species::Zapdos: return {Type::Electric, Type::Flying};
		case Species::Moltres: return {Type::Fire, Type::Flying};
		case Species::Dratini: return {Type::Dragon};
		case Species::Dragonair: return {Type::Dragon};
		case Species::Dragonite: return {Type::Dragon, Type::Flying};
		case Species::Mewtwo: return {Type::Psychic};
		case Species::Mew: return {Type::Psychic};

		// Generation 2
		case Species::Chikorita: return {Type::Grass};
		case Species::Bayleef: return {Type::Grass};
		case Species::Meganium: return {Type::Grass};
		case Species::Cyndaquil: return {Type::Fire};
		case Species::Quilava: return {Type::Fire};
		case Species::Typhlosion: return {Type::Fire};
		case Species::Totodile: return {Type::Water};
		case Species::Croconaw: return {Type::Water};
		case Species::Feraligatr: return {Type::Water};
		case Species::Sentret: return {Type::Normal};
		case Species::Furret: return {Type::Normal};
		case Species::Hoothoot: return {Type::Normal, Type::Flying};
		case Species::Noctowl: return {Type::Normal, Type::Flying};
		case Species::Ledyba: return {Type::Bug, Type::Flying};
		case Species::Ledian: return {Type::Bug, Type::Flying};
		case Species::Spinarak: return {Type::Bug, Type::Poison};
		case Species::Ariados: return {Type::Bug, Type::Poison};
		case Species::Crobat: return {Type::Poison, Type::Flying};
		case Species::Chinchou: return {Type::Water, Type::Electric};
		case Species::Lanturn: return {Type::Water, Type::Electric};
		case Species::Pichu: return {Type::Electric};
		case Species::Cleffa: return {Type::Normal};
		case Species::Igglybuff: return {Type::Normal};
		case Species::Togepi: return {Type::Normal};
		case Species::Togetic: return {Type::Normal, Type::Flying};
		case Species::Natu: return {Type::Psychic, Type::Flying};
		case Species::Xatu: return {Type::Psychic, Type::Flying};
		case Species::Mareep: return {Type::Electric};
		case Species::Flaaffy: return {Type::Electric};
		case Species::Ampharos: return {Type::Electric};
		case Species::Bellossom: return {Type::Grass};
		case Species::Marill: return {Type::Water};
		case Species::Azumarill: return {Type::Water};
		case Species::Sudowoodo: return {Type::Rock};
		case Species::Politoed: return {Type::Water};
		case Species::Hoppip: return {Type::Grass, Type::Flying};
		case Species::Skiploom: return {Type::Grass, Type::Flying};
		case Species::Jumpluff: return {Type::Grass, Type::Flying};
		case Species::Aipom: return {Type::Normal};
		case Species::Sunkern: return {Type::Grass};
		case Species::Sunflora: return {Type::Grass};
		case Species::Yanma: return {Type::Bug, Type::Flying};
		case Species::Wooper: return {Type::Water, Type::Ground};
		case Species::Quagsire: return {Type::Water, Type::Ground};
		case Species::Espeon: return {Type::Psychic};
		case Species::Umbreon: return {Type::Dark};
		case Species::Murkrow: return {Type::Dark, Type::Flying};
		case Species::Slowking: return {Type::Water, Type::Psychic};
		case Species::Misdreavus: return {Type::Ghost};
		case Species::Unown: return {Type::Psychic};
		case Species::Wobbuffet: return {Type::Psychic};
		case Species::Girafarig: return {Type::Normal, Type::Psychic};
		case Species::Pineco: return {Type::Bug};
		case Species::Forretress: return {Type::Bug, Type::Steel};
		case Species::Dunsparce: return {Type::Normal};
		case Species::Gligar: return {Type::Ground, Type::Flying};
		case Species::Steelix: return {Type::Steel, Type::Ground};
		case Species::Snubbull: return {Type::Normal};
		case Species::Granbull: return {Type::Normal};
		case Species::Qwilfish: return {Type::Water, Type::Poison};
		case Species::Scizor: return {Type::Bug, Type::Steel};
		case Species::Shuckle: return {Type::Bug, Type::Rock};
		case Species::Heracross: return {Type::Bug, Type::Fighting};
		case Species::Sneasel: return {Type::Dark, Type::Ice};
		case Species::Teddiursa: return {Type::Normal};
		case Species::Ursaring: return {Type::Normal};
		case Species::Slugma: return {Type::Fire};
		case Species::Magcargo: return {Type::Fire, Type::Rock};
		case Species::Swinub: return {Type::Ice, Type::Ground};
		case Species::Piloswine: return {Type::Ice, Type::Ground};
		case Species::Corsola: return {Type::Water, Type::Rock};
		case Species::Remoraid: return {Type::Water};
		case Species::Octillery: return {Type::Water};
		case Species::Delibird: return {Type::Ice, Type::Flying};
		case Species::Mantine: return {Type::Water, Type::Flying};
		case Species::Skarmory: return {Type::Steel, Type::Flying};
		case Species::Houndour: return {Type::Dark, Type::Fire};
		case Species::Houndoom: return {Type::Dark, Type::Fire};
		case Species::Kingdra: return {Type::Water, Type::Dragon};
		case Species::Phanpy: return {Type::Ground};
		case Species::Donphan: return {Type::Ground};
		case Species::Porygon2: return {Type::Normal};
		case Species::Stantler: return {Type::Normal};
		case Species::Smeargle: return {Type::Normal};
		case Species::Tyrogue: return {Type::Fighting};
		case Species::Hitmontop: return {Type::Fighting};
		case Species::Smoochum: return {Type::Ice, Type::Psychic};
		case Species::Elekid: return {Type::Electric};
		case Species::Magby: return {Type::Fire};
		case Species::Miltank: return {Type::Normal};
		case Species::Blissey: return {Type::Normal};
		case Species::Raikou: return {Type::Electric};
		case Species::Entei: return {Type::Fire};
		case Species::Suicune: return {Type::Water};
		case Species::Larvitar: return {Type::Rock, Type::Ground};
		case Species::Pupitar: return {Type::Rock, Type::Ground};
		case Species::Tyranitar: return {Type::Rock, Type::Dark};
		case Species::Lugia: return {Type::Psychic, Type::Flying};
		case Species::Ho_Oh: return {Type::Fire, Type::Flying};
		case Species::Celebi: return {Type::Psychic, Type::Grass};

		// Generation 3
		case Species::Treecko: return {Type::Grass};
		case Species::Grovyle: return {Type::Grass};
		case Species::Sceptile: return {Type::Grass};
		case Species::Torchic: return {Type::Fire};
		case Species::Combusken: return {Type::Fire, Type::Fighting};
		case Species::Blaziken: return {Type::Fire, Type::Fighting};
		case Species::Mudkip: return {Type::Water};
		case Species::Marshtomp: return {Type::Water, Type::Ground};
		case Species::Swampert: return {Type::Water, Type::Ground};
		case Species::Poochyena: return {Type::Dark};
		case Species::Mightyena: return {Type::Dark};
		case Species::Zigzagoon: return {Type::Normal};
		case Species::Linoone: return {Type::Normal};
		case Species::Wurmple: return {Type::Bug};
		case Species::Silcoon: return {Type::Bug};
		case Species::Beautifly: return {Type::Bug, Type::Flying};
		case Species::Cascoon: return {Type::Bug};
		case Species::Dustox: return {Type::Bug, Type::Poison};
		case Species::Lotad: return {Type::Water, Type::Grass};
		case Species::Lombre: return {Type::Water, Type::Grass};
		case Species::Ludicolo: return {Type::Water, Type::Grass};
		case Species::Seedot: return {Type::Grass};
		case Species::Nuzleaf: return {Type::Grass, Type::Dark};
		case Species::Shiftry: return {Type::Grass, Type::Dark};
		case Species::Taillow: return {Type::Normal, Type::Flying};
		case Species::Swellow: return {Type::Normal, Type::Flying};
		case Species::Wingull: return {Type::Water, Type::Flying};
		case Species::Pelipper: return {Type::Water, Type::Flying};
		case Species::Ralts: return {Type::Psychic};
		case Species::Kirlia: return {Type::Psychic};
		case Species::Gardevoir: return {Type::Psychic};
		case Species::Surskit: return {Type::Bug, Type::Water};
		case Species::Masquerain: return {Type::Bug, Type::Flying};
		case Species::Shroomish: return {Type::Grass};
		case Species::Breloom: return {Type::Grass, Type::Fighting};
		case Species::Slakoth: return {Type::Normal};
		case Species::Vigoroth: return {Type::Normal};
		case Species::Slaking: return {Type::Normal};
		case Species::Nincada: return {Type::Bug, Type::Ground};
		case Species::Ninjask: return {Type::Bug, Type::Flying};
		case Species::Shedinja: return {Type::Bug, Type::Ghost};
		case Species::Whismur: return {Type::Normal};
		case Species::Loudred: return {Type::Normal};
		case Species::Exploud: return {Type::Normal};
		case Species::Makuhita: return {Type::Fighting};
		case Species::Hariyama: return {Type::Fighting};
		case Species::Azurill: return {Type::Normal};
		case Species::Nosepass: return {Type::Rock};
		case Species::Skitty: return {Type::Normal};
		case Species::Delcatty: return {Type::Normal};
		case Species::Sableye: return {Type::Dark, Type::Ghost};
		case Species::Mawile: return {Type::Steel};
		case Species::Aron: return {Type::Steel, Type::Rock};
		case Species::Lairon: return {Type::Steel, Type::Rock};
		case Species::Aggron: return {Type::Steel, Type::Rock};
		case Species::Meditite: return {Type::Fighting, Type::Psychic};
		case Species::Medicham: return {Type::Fighting, Type::Psychic};
		case Species::Electrike: return {Type::Electric};
		case Species::Manectric: return {Type::Electric};
		case Species::Plusle: return {Type::Electric};
		case Species::Minun: return {Type::Electric};
		case Species::Volbeat: return {Type::Bug};
		case Species::Illumise: return {Type::Bug};
		case Species::Roselia: return {Type::Grass, Type::Poison};
		case Species::Gulpin: return {Type::Poison};
		case Species::Swalot: return {Type::Poison};
		case Species::Carvanha: return {Type::Water, Type::Dark};
		case Species::Sharpedo: return {Type::Water, Type::Dark};
		case Species::Wailmer: return {Type::Water};
		case Species::Wailord: return {Type::Water};
		case Species::Numel: return {Type::Fire, Type::Ground};
		case Species::Camerupt: return {Type::Fire, Type::Ground};
		case Species::Torkoal: return {Type::Fire};
		case Species::Spoink: return {Type::Psychic};
		case Species::Grumpig: return {Type::Psychic};
		case Species::Spinda: return {Type::Normal};
		case Species::Trapinch: return {Type::Ground};
		case Species::Vibrava: return {Type::Ground, Type::Dragon};
		case Species::Flygon: return {Type::Ground, Type::Dragon};
		case Species::Cacnea: return {Type::Grass};
		case Species::Cacturne: return {Type::Grass, Type::Dark};
		case Species::Swablu: return {Type::Normal, Type::Flying};
		case Species::Altaria: return {Type::Dragon, Type::Flying};
		case Species::Zangoose: return {Type::Normal};
		case Species::Seviper: return {Type::Poison};
		case Species::Lunatone: return {Type::Rock, Type::Psychic};
		case Species::Solrock: return {Type::Rock, Type::Psychic};
		case Species::Barboach: return {Type::Water, Type::Ground};
		case Species::Whiscash: return {Type::Water, Type::Ground};
		case Species::Corphish: return {Type::Water};
		case Species::Crawdaunt: return {Type::Water, Type::Dark};
		case Species::Baltoy: return {Type::Ground, Type::Psychic};
		case Species::Claydol: return {Type::Ground, Type::Psychic};
		case Species::Lileep: return {Type::Rock, Type::Grass};
		case Species::Cradily: return {Type::Rock, Type::Grass};
		case Species::Anorith: return {Type::Rock, Type::Bug};
		case Species::Armaldo: return {Type::Rock, Type::Bug};
		case Species::Feebas: return {Type::Water};
		case Species::Milotic: return {Type::Water};
		case Species::Castform: return {Type::Normal};
			// return {Type::Fire};
			// return {Type::Water};
			// return {Type::Ice};
		case Species::Kecleon: return {Type::Normal};
		case Species::Shuppet: return {Type::Ghost};
		case Species::Banette: return {Type::Ghost};
		case Species::Duskull: return {Type::Ghost};
		case Species::Dusclops: return {Type::Ghost};
		case Species::Tropius: return {Type::Grass, Type::Flying};
		case Species::Chimecho: return {Type::Psychic};
		case Species::Absol: return {Type::Dark};
		case Species::Wynaut: return {Type::Psychic};
		case Species::Snorunt: return {Type::Ice};
		case Species::Glalie: return {Type::Ice};
		case Species::Spheal: return {Type::Ice, Type::Water};
		case Species::Sealeo: return {Type::Ice, Type::Water};
		case Species::Walrein: return {Type::Ice, Type::Water};
		case Species::Clamperl: return {Type::Water};
		case Species::Huntail: return {Type::Water};
		case Species::Gorebyss: return {Type::Water};
		case Species::Relicanth: return {Type::Water, Type::Rock};
		case Species::Luvdisc: return {Type::Water};
		case Species::Bagon: return {Type::Dragon};
		case Species::Shelgon: return {Type::Dragon};
		case Species::Salamence: return {Type::Dragon, Type::Flying};
		case Species::Beldum: return {Type::Steel, Type::Psychic};
		case Species::Metang: return {Type::Steel, Type::Psychic};
		case Species::Metagross: return {Type::Steel, Type::Psychic};
		case Species::Regirock: return {Type::Rock};
		case Species::Regice: return {Type::Ice};
		case Species::Registeel: return {Type::Steel};
		case Species::Latias: return {Type::Dragon, Type::Psychic};
		case Species::Latios: return {Type::Dragon, Type::Psychic};
		case Species::Kyogre: return {Type::Water};
		case Species::Groudon: return {Type::Ground};
		case Species::Rayquaza: return {Type::Dragon, Type::Flying};
		case Species::Jirachi: return {Type::Steel, Type::Psychic};
		case Species::Deoxys_Mediocre: return {Type::Psychic};
		case Species::Deoxys_Attack: return {Type::Psychic};
		case Species::Deoxys_Defense: return {Type::Psychic};
		case Species::Deoxys_Speed: return {Type::Psychic};

		// Generation 4
		case Species::Turtwig: return {Type::Grass};
		case Species::Grotle: return {Type::Grass};
		case Species::Torterra: return {Type::Grass, Type::Ground};
		case Species::Chimchar: return {Type::Fire};
		case Species::Monferno: return {Type::Fire, Type::Fighting};
		case Species::Infernape: return {Type::Fire, Type::Fighting};
		case Species::Piplup: return {Type::Water};
		case Species::Prinplup: return {Type::Water};
		case Species::Empoleon: return {Type::Water, Type::Steel};
		case Species::Starly: return {Type::Normal, Type::Flying};
		case Species::Staravia: return {Type::Normal, Type::Flying};
		case Species::Staraptor: return {Type::Normal, Type::Flying};
		case Species::Bidoof: return {Type::Normal};
		case Species::Bibarel: return {Type::Normal, Type::Water};
		case Species::Kricketot: return {Type::Bug};
		case Species::Kricketune: return {Type::Bug};
		case Species::Shinx: return {Type::Electric};
		case Species::Luxio: return {Type::Electric};
		case Species::Luxray: return {Type::Electric};
		case Species::Budew: return {Type::Grass, Type::Poison};
		case Species::Roserade: return {Type::Grass, Type::Poison};
		case Species::Cranidos: return {Type::Rock};
		case Species::Rampardos: return {Type::Rock};
		case Species::Shieldon: return {Type::Rock, Type::Steel};
		case Species::Bastiodon: return {Type::Rock, Type::Steel};
		case Species::Burmy: return {Type::Bug};
		case Species::Wormadam_Plant: return {Type::Bug, Type::Grass};
		case Species::Wormadam_Sandy: return {Type::Bug, Type::Ground};
		case Species::Wormadam_Trash: return {Type::Bug, Type::Steel};
		case Species::Mothim: return {Type::Bug, Type::Flying};
		case Species::Combee: return {Type::Bug, Type::Flying};
		case Species::Vespiquen: return {Type::Bug, Type::Flying};
		case Species::Pachirisu: return {Type::Electric};
		case Species::Buizel: return {Type::Water};
		case Species::Floatzel: return {Type::Water};
		case Species::Cherubi: return {Type::Grass};
		case Species::Cherrim: return {Type::Grass};
		case Species::Shellos: return {Type::Water};
		case Species::Gastrodon: return {Type::Water, Type::Ground};
		case Species::Ambipom: return {Type::Normal};
		case Species::Drifloon: return {Type::Ghost, Type::Flying};
		case Species::Drifblim: return {Type::Ghost, Type::Flying};
		case Species::Buneary: return {Type::Normal};
		case Species::Lopunny: return {Type::Normal};
		case Species::Mismagius: return {Type::Ghost};
		case Species::Honchkrow: return {Type::Dark, Type::Flying};
		case Species::Glameow: return {Type::Normal};
		case Species::Purugly: return {Type::Normal};
		case Species::Chingling: return {Type::Psychic};
		case Species::Stunky: return {Type::Poison, Type::Dark};
		case Species::Skuntank: return {Type::Poison, Type::Dark};
		case Species::Bronzor: return {Type::Steel, Type::Psychic};
		case Species::Bronzong: return {Type::Steel, Type::Psychic};
		case Species::Bonsly: return {Type::Rock};
		case Species::Mime_Jr: return {Type::Psychic};
		case Species::Happiny: return {Type::Normal};
		case Species::Chatot: return {Type::Normal, Type::Flying};
		case Species::Spiritomb: return {Type::Ghost, Type::Dark};
		case Species::Gible: return {Type::Dragon, Type::Ground};
		case Species::Gabite: return {Type::Dragon, Type::Ground};
		case Species::Garchomp: return {Type::Dragon, Type::Ground};
		case Species::Munchlax: return {Type::Normal};
		case Species::Riolu: return {Type::Fighting};
		case Species::Lucario: return {Type::Fighting, Type::Steel};
		case Species::Hippopotas: return {Type::Ground};
		case Species::Hippowdon: return {Type::Ground};
		case Species::Skorupi: return {Type::Poison, Type::Bug};
		case Species::Drapion: return {Type::Poison, Type::Dark};
		case Species::Croagunk: return {Type::Poison, Type::Fighting};
		case Species::Toxicroak: return {Type::Poison, Type::Fighting};
		case Species::Carnivine: return {Type::Grass};
		case Species::Finneon: return {Type::Water};
		case Species::Lumineon: return {Type::Water};
		case Species::Mantyke: return {Type::Water, Type::Flying};
		case Species::Snover: return {Type::Grass, Type::Ice};
		case Species::Abomasnow: return {Type::Grass, Type::Ice};
		case Species::Weavile: return {Type::Dark, Type::Ice};
		case Species::Magnezone: return {Type::Electric, Type::Steel};
		case Species::Lickilicky: return {Type::Normal};
		case Species::Rhyperior: return {Type::Ground, Type::Rock};
		case Species::Tangrowth: return {Type::Grass};
		case Species::Electivire: return {Type::Electric};
		case Species::Magmortar: return {Type::Fire};
		case Species::Togekiss: return {Type::Normal, Type::Flying};
		case Species::Yanmega: return {Type::Bug, Type::Flying};
		case Species::Leafeon: return {Type::Grass};
		case Species::Glaceon: return {Type::Ice};
		case Species::Gliscor: return {Type::Ground, Type::Flying};
		case Species::Mamoswine: return {Type::Ice, Type::Ground};
		case Species::Porygon_Z: return {Type::Normal};
		case Species::Gallade: return {Type::Psychic, Type::Fighting};
		case Species::Probopass: return {Type::Rock, Type::Steel};
		case Species::Dusknoir: return {Type::Ghost};
		case Species::Froslass: return {Type::Ice, Type::Ghost};
		case Species::Rotom: return {Type::Electric, Type::Ghost};
			// return {Type::Electric, Type::Fire};
		case Species::Rotom_Heat: return {Type::Electric, Type::Ghost};
			// return {Type::Electric, Type::Water};
		case Species::Rotom_Wash: return {Type::Electric, Type::Ghost};
			// return {Type::Electric, Type::Ice};
		case Species::Rotom_Frost: return {Type::Electric, Type::Ghost};
			// return {Type::Electric, Type::Flying};
		case Species::Rotom_Fan: return {Type::Electric, Type::Ghost};
			// return {Type::Electric, Type::Grass};
		case Species::Rotom_Mow: return {Type::Electric, Type::Ghost};
		case Species::Uxie: return {Type::Psychic};
		case Species::Mesprit: return {Type::Psychic};
		case Species::Azelf: return {Type::Psychic};
		case Species::Dialga: return {Type::Steel, Type::Dragon};
		case Species::Palkia: return {Type::Water, Type::Dragon};
		case Species::Heatran: return {Type::Fire, Type::Steel};
		case Species::Regigigas: return {Type::Normal};
		case Species::Giratina_Altered: return {Type::Ghost, Type::Dragon};
		case Species::Giratina_Origin: return {Type::Ghost, Type::Dragon};
		case Species::Cresselia: return {Type::Psychic};
		case Species::Phione: return {Type::Water};
		case Species::Manaphy: return {Type::Water};
		case Species::Darkrai: return {Type::Dark};
		case Species::Shaymin_Land: return {Type::Grass};
		case Species::Shaymin_Sky: return {Type::Grass, Type::Flying};
		case Species::Arceus: return {Type::Normal};

		// Generation 5
		case Species::Victini: return {Type::Psychic, Type::Fire};
		case Species::Snivy: return {Type::Grass};
		case Species::Servine: return {Type::Grass};
		case Species::Serperior: return {Type::Grass};
		case Species::Tepig: return {Type::Fire};
		case Species::Pignite: return {Type::Fire, Type::Fighting};
		case Species::Emboar: return {Type::Fire, Type::Fighting};
		case Species::Oshawott: return {Type::Water};
		case Species::Dewott: return {Type::Water};
		case Species::Samurott: return {Type::Water};
		case Species::Patrat: return {Type::Normal};
		case Species::Watchog: return {Type::Normal};
		case Species::Lillipup: return {Type::Normal};
		case Species::Herdier: return {Type::Normal};
		case Species::Stoutland: return {Type::Normal};
		case Species::Purrloin: return {Type::Dark};
		case Species::Liepard: return {Type::Dark};
		case Species::Pansage: return {Type::Grass};
		case Species::Simisage: return {Type::Grass};
		case Species::Pansear: return {Type::Fire};
		case Species::Simisear: return {Type::Fire};
		case Species::Panpour: return {Type::Water};
		case Species::Simipour: return {Type::Water};
		case Species::Munna: return {Type::Psychic};
		case Species::Musharna: return {Type::Psychic};
		case Species::Pidove: return {Type::Normal, Type::Flying};
		case Species::Tranquill: return {Type::Normal, Type::Flying};
		case Species::Unfezant: return {Type::Normal, Type::Flying};
		case Species::Blitzle: return {Type::Electric};
		case Species::Zebstrika: return {Type::Electric};
		case Species::Roggenrola: return {Type::Rock};
		case Species::Boldore: return {Type::Rock};
		case Species::Gigalith: return {Type::Rock};
		case Species::Woobat: return {Type::Psychic, Type::Flying};
		case Species::Swoobat: return {Type::Psychic, Type::Flying};
		case Species::Drilbur: return {Type::Ground};
		case Species::Excadrill: return {Type::Ground, Type::Steel};
		case Species::Audino: return {Type::Normal};
		case Species::Timburr: return {Type::Fighting};
		case Species::Gurdurr: return {Type::Fighting};
		case Species::Conkeldurr: return {Type::Fighting};
		case Species::Tympole: return {Type::Water};
		case Species::Palpitoad: return {Type::Water, Type::Ground};
		case Species::Seismitoad: return {Type::Water, Type::Ground};
		case Species::Throh: return {Type::Fighting};
		case Species::Sawk: return {Type::Fighting};
		case Species::Sewaddle: return {Type::Bug, Type::Grass};
		case Species::Swadloon: return {Type::Bug, Type::Grass};
		case Species::Leavanny: return {Type::Bug, Type::Grass};
		case Species::Venipede: return {Type::Bug, Type::Poison};
		case Species::Whirlipede: return {Type::Bug, Type::Poison};
		case Species::Scolipede: return {Type::Bug, Type::Poison};
		case Species::Cottonee: return {Type::Grass};
		case Species::Whimsicott: return {Type::Grass};
		case Species::Petilil: return {Type::Grass};
		case Species::Lilligant: return {Type::Grass};
		case Species::Basculin_Red: return {Type::Water};
		case Species::Basculin_Blue: return {Type::Water};
		case Species::Sandile: return {Type::Ground, Type::Dark};
		case Species::Krokorok: return {Type::Ground, Type::Dark};
		case Species::Krookodile: return {Type::Ground, Type::Dark};
		case Species::Darumaka: return {Type::Fire};
		case Species::Darmanitan: return {Type::Fire};
		// case Species::Darmanitan_Zen: return {Type::Fire, Type::Psychic};
		case Species::Maractus: return {Type::Grass};
		case Species::Dwebble: return {Type::Bug, Type::Rock};
		case Species::Crustle: return {Type::Bug, Type::Rock};
		case Species::Scraggy: return {Type::Dark, Type::Fighting};
		case Species::Scrafty: return {Type::Dark, Type::Fighting};
		case Species::Sigilyph: return {Type::Psychic, Type::Flying};
		case Species::Yamask: return {Type::Ghost};
		case Species::Cofagrigus: return {Type::Ghost};
		case Species::Tirtouga: return {Type::Water, Type::Rock};
		case Species::Carracosta: return {Type::Water, Type::Rock};
		case Species::Archen: return {Type::Rock, Type::Flying};
		case Species::Archeops: return {Type::Rock, Type::Flying};
		case Species::Trubbish: return {Type::Poison};
		case Species::Garbodor: return {Type::Poison};
		case Species::Zorua: return {Type::Dark};
		case Species::Zoroark: return {Type::Dark};
		case Species::Minccino: return {Type::Normal};
		case Species::Cinccino: return {Type::Normal};
		case Species::Gothita: return {Type::Psychic};
		case Species::Gothorita: return {Type::Psychic};
		case Species::Gothitelle: return {Type::Psychic};
		case Species::Solosis: return {Type::Psychic};
		case Species::Duosion: return {Type::Psychic};
		case Species::Reuniclus: return {Type::Psychic};
		case Species::Ducklett: return {Type::Water, Type::Flying};
		case Species::Swanna: return {Type::Water, Type::Flying};
		case Species::Vanillite: return {Type::Ice};
		case Species::Vanillish: return {Type::Ice};
		case Species::Vanilluxe: return {Type::Ice};
		case Species::Deerling: return {Type::Normal, Type::Grass};
		case Species::Sawsbuck: return {Type::Normal, Type::Grass};
		case Species::Emolga: return {Type::Electric, Type::Flying};
		case Species::Karrablast: return {Type::Bug};
		case Species::Escavalier: return {Type::Bug, Type::Steel};
		case Species::Foongus: return {Type::Grass, Type::Poison};
		case Species::Amoonguss: return {Type::Grass, Type::Poison};
		case Species::Frillish: return {Type::Water, Type::Ghost};
		case Species::Jellicent: return {Type::Water, Type::Ghost};
		case Species::Alomomola: return {Type::Water};
		case Species::Joltik: return {Type::Bug, Type::Electric};
		case Species::Galvantula: return {Type::Bug, Type::Electric};
		case Species::Ferroseed: return {Type::Grass, Type::Steel};
		case Species::Ferrothorn: return {Type::Grass, Type::Steel};
		case Species::Klink: return {Type::Steel};
		case Species::Klang: return {Type::Steel};
		case Species::Klinklang: return {Type::Steel};
		case Species::Tynamo: return {Type::Electric};
		case Species::Eelektrik: return {Type::Electric};
		case Species::Eelektross: return {Type::Electric};
		case Species::Elgyem: return {Type::Psychic};
		case Species::Beheeyem: return {Type::Psychic};
		case Species::Litwick: return {Type::Ghost, Type::Fire};
		case Species::Lampent: return {Type::Ghost, Type::Fire};
		case Species::Chandelure: return {Type::Ghost, Type::Fire};
		case Species::Axew: return {Type::Dragon};
		case Species::Fraxure: return {Type::Dragon};
		case Species::Haxorus: return {Type::Dragon};
		case Species::Cubchoo: return {Type::Ice};
		case Species::Beartic: return {Type::Ice};
		case Species::Cryogonal: return {Type::Ice};
		case Species::Shelmet: return {Type::Bug};
		case Species::Accelgor: return {Type::Bug};
		case Species::Stunfisk: return {Type::Ground, Type::Electric};
		case Species::Mienfoo: return {Type::Fighting};
		case Species::Mienshao: return {Type::Fighting};
		case Species::Druddigon: return {Type::Dragon};
		case Species::Golett: return {Type::Ground, Type::Ghost};
		case Species::Golurk: return {Type::Ground, Type::Ghost};
		case Species::Pawniard: return {Type::Dark, Type::Steel};
		case Species::Bisharp: return {Type::Dark, Type::Steel};
		case Species::Bouffalant: return {Type::Normal};
		case Species::Rufflet: return {Type::Normal, Type::Flying};
		case Species::Braviary: return {Type::Normal, Type::Flying};
		case Species::Vullaby: return {Type::Dark, Type::Flying};
		case Species::Mandibuzz: return {Type::Dark, Type::Flying};
		case Species::Heatmor: return {Type::Fire};
		case Species::Durant: return {Type::Bug, Type::Steel};
		case Species::Deino: return {Type::Dark, Type::Dragon};
		case Species::Zweilous: return {Type::Dark, Type::Dragon};
		case Species::Hydreigon: return {Type::Dark, Type::Dragon};
		case Species::Larvesta: return {Type::Bug, Type::Fire};
		case Species::Volcarona: return {Type::Bug, Type::Fire};
		case Species::Cobalion: return {Type::Steel, Type::Fighting};
		case Species::Terrakion: return {Type::Rock, Type::Fighting};
		case Species::Virizion: return {Type::Grass, Type::Fighting};
		case Species::Tornadus_Incarnate: return {Type::Flying};
		case Species::Tornadus_Therian: return {Type::Flying};
		case Species::Thundurus_Incarnate: return {Type::Electric, Type::Flying};
		case Species::Thundurus_Therian: return {Type::Electric, Type::Flying};
		case Species::Reshiram: return {Type::Dragon, Type::Fire};
		case Species::Zekrom: return {Type::Dragon, Type::Electric};
		case Species::Landorus_Incarnate: return {Type::Ground, Type::Flying};
		case Species::Landorus_Therian: return {Type::Ground, Type::Flying};
		case Species::Kyurem: return {Type::Dragon, Type::Ice};
		case Species::Kyurem_Black: return {Type::Dragon, Type::Ice};
		case Species::Kyurem_White: return {Type::Dragon, Type::Ice};
		case Species::Keldeo: return {Type::Water, Type::Fighting};
		case Species::Meloetta: return {Type::Normal, Type::Psychic};
		// case Species::Meloetta_Pirouette: return {Type::Normal, Type::Fighting};
		case Species::Genesect: return {Type::Bug, Type::Steel};
	}
}

}	// namespace
}	// namespace technicalmachine
