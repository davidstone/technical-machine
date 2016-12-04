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

#include "hp.hpp"

#include "../pokemon/level.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {
namespace {

using base_type = bounded::integer<1, 255>;
auto get_base(Species species) -> base_type;

auto initial_hp(base_type const base, EV const ev, IV const iv, Level const level) {
	auto const value = BOUNDED_CONDITIONAL((base > 1_bi),
		(2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 10_bi + level(),
		1_bi
	);
	static_assert(std::numeric_limits<decltype(value)>::min() == std::numeric_limits<HP::max_type>::min(), "Incorrect HP min.");
	static_assert(std::numeric_limits<decltype(value)>::max() == std::numeric_limits<HP::max_type>::max(), "Incorrect HP max.");
	return value;
}

}	// namespace

HP::HP(Species const species, Level const level, EV const ev_, IV const iv_) :
	m_ev(ev_),
	m_iv(iv_),
	m_max(initial_hp(get_base(species), m_ev, m_iv, level)),
	m_current(m_max)
	{
}

auto HP::current() const -> current_type {
	return m_current;
}

auto HP::max() const -> max_type {
	return m_max;
}

auto HP::ev() const -> EV {
	return m_ev;
}

auto HP::iv() const -> IV {
	return m_iv;
}

auto set_hp_ev(Pokemon & pokemon, EV const ev) -> void {
	set_hp_ev(pokemon, ev, get_hp(pokemon).iv());
}
auto set_hp_ev(Pokemon & pokemon, EV const ev, IV const iv) -> void {
	get_hp(pokemon) = HP(pokemon, get_level(pokemon), ev, iv);
}

namespace {

auto get_base(Species const species) -> base_type {
	switch (species) {
		// Generation 1
		case Species::Bulbasaur: return 45_bi;
		case Species::Ivysaur: return 60_bi;
		case Species::Venusaur: return 80_bi;
		case Species::Charmander: return 39_bi;
		case Species::Charmeleon: return 58_bi;
		case Species::Charizard: return 78_bi;
		case Species::Squirtle: return 44_bi;
		case Species::Wartortle: return 59_bi;
		case Species::Blastoise: return 79_bi;
		case Species::Caterpie: return 45_bi;
		case Species::Metapod: return 50_bi;
		case Species::Butterfree: return 60_bi;
		case Species::Weedle: return 40_bi;
		case Species::Kakuna: return 45_bi;
		case Species::Beedrill: return 65_bi;
		case Species::Pidgey: return 40_bi;
		case Species::Pidgeotto: return 63_bi;
		case Species::Pidgeot: return 83_bi;
		case Species::Rattata: return 30_bi;
		case Species::Raticate: return 55_bi;
		case Species::Spearow: return 40_bi;
		case Species::Fearow: return 65_bi;
		case Species::Ekans: return 35_bi;
		case Species::Arbok: return 60_bi;
		case Species::Pikachu: return 35_bi;
		case Species::Raichu: return 60_bi;
		case Species::Sandshrew: return 50_bi;
		case Species::Sandslash: return 75_bi;
		case Species::Nidoran_F: return 55_bi;
		case Species::Nidorina: return 70_bi;
		case Species::Nidoqueen: return 90_bi;
		case Species::Nidoran_M: return 46_bi;
		case Species::Nidorino: return 61_bi;
		case Species::Nidoking: return 81_bi;
		case Species::Clefairy: return 70_bi;
		case Species::Clefable: return 95_bi;
		case Species::Vulpix: return 38_bi;
		case Species::Ninetales: return 73_bi;
		case Species::Jigglypuff: return 115_bi;
		case Species::Wigglytuff: return 140_bi;
		case Species::Zubat: return 40_bi;
		case Species::Golbat: return 75_bi;
		case Species::Oddish: return 45_bi;
		case Species::Gloom: return 60_bi;
		case Species::Vileplume: return 75_bi;
		case Species::Paras: return 35_bi;
		case Species::Parasect: return 60_bi;
		case Species::Venonat: return 60_bi;
		case Species::Venomoth: return 70_bi;
		case Species::Diglett: return 10_bi;
		case Species::Dugtrio: return 35_bi;
		case Species::Meowth: return 40_bi;
		case Species::Persian: return 65_bi;
		case Species::Psyduck: return 50_bi;
		case Species::Golduck: return 80_bi;
		case Species::Mankey: return 40_bi;
		case Species::Primeape: return 65_bi;
		case Species::Growlithe: return 55_bi;
		case Species::Arcanine: return 90_bi;
		case Species::Poliwag: return 40_bi;
		case Species::Poliwhirl: return 65_bi;
		case Species::Poliwrath: return 90_bi;
		case Species::Abra: return 25_bi;
		case Species::Kadabra: return 40_bi;
		case Species::Alakazam: return 55_bi;
		case Species::Machop: return 70_bi;
		case Species::Machoke: return 80_bi;
		case Species::Machamp: return 90_bi;
		case Species::Bellsprout: return 50_bi;
		case Species::Weepinbell: return 65_bi;
		case Species::Victreebel: return 80_bi;
		case Species::Tentacool: return 40_bi;
		case Species::Tentacruel: return 80_bi;
		case Species::Geodude: return 40_bi;
		case Species::Graveler: return 55_bi;
		case Species::Golem: return 80_bi;
		case Species::Ponyta: return 50_bi;
		case Species::Rapidash: return 65_bi;
		case Species::Slowpoke: return 90_bi;
		case Species::Slowbro: return 95_bi;
		case Species::Magnemite: return 25_bi;
		case Species::Magneton: return 50_bi;
		case Species::Farfetchd: return 52_bi;
		case Species::Doduo: return 35_bi;
		case Species::Dodrio: return 60_bi;
		case Species::Seel: return 65_bi;
		case Species::Dewgong: return 90_bi;
		case Species::Grimer: return 80_bi;
		case Species::Muk: return 105_bi;
		case Species::Shellder: return 30_bi;
		case Species::Cloyster: return 50_bi;
		case Species::Gastly: return 30_bi;
		case Species::Haunter: return 45_bi;
		case Species::Gengar: return 60_bi;
		case Species::Onix: return 35_bi;
		case Species::Drowzee: return 60_bi;
		case Species::Hypno: return 85_bi;
		case Species::Krabby: return 30_bi;
		case Species::Kingler: return 55_bi;
		case Species::Voltorb: return 40_bi;
		case Species::Electrode: return 60_bi;
		case Species::Exeggcute: return 60_bi;
		case Species::Exeggutor: return 95_bi;
		case Species::Cubone: return 50_bi;
		case Species::Marowak: return 60_bi;
		case Species::Hitmonlee: return 50_bi;
		case Species::Hitmonchan: return 50_bi;
		case Species::Lickitung: return 90_bi;
		case Species::Koffing: return 40_bi;
		case Species::Weezing: return 65_bi;
		case Species::Rhyhorn: return 80_bi;
		case Species::Rhydon: return 105_bi;
		case Species::Chansey: return 250_bi;
		case Species::Tangela: return 65_bi;
		case Species::Kangaskhan: return 105_bi;
		case Species::Horsea: return 30_bi;
		case Species::Seadra: return 55_bi;
		case Species::Goldeen: return 45_bi;
		case Species::Seaking: return 80_bi;
		case Species::Staryu: return 30_bi;
		case Species::Starmie: return 60_bi;
		case Species::Mr_Mime: return 40_bi;
		case Species::Scyther: return 70_bi;
		case Species::Jynx: return 65_bi;
		case Species::Electabuzz: return 65_bi;
		case Species::Magmar: return 65_bi;
		case Species::Pinsir: return 65_bi;
		case Species::Tauros: return 75_bi;
		case Species::Magikarp: return 20_bi;
		case Species::Gyarados: return 95_bi;
		case Species::Lapras: return 130_bi;
		case Species::Ditto: return 48_bi;
		case Species::Eevee: return 55_bi;
		case Species::Vaporeon: return 130_bi;
		case Species::Jolteon: return 65_bi;
		case Species::Flareon: return 65_bi;
		case Species::Porygon: return 65_bi;
		case Species::Omanyte: return 35_bi;
		case Species::Omastar: return 70_bi;
		case Species::Kabuto: return 30_bi;
		case Species::Kabutops: return 60_bi;
		case Species::Aerodactyl: return 80_bi;
		case Species::Snorlax: return 160_bi;
		case Species::Articuno: return 90_bi;
		case Species::Zapdos: return 90_bi;
		case Species::Moltres: return 90_bi;
		case Species::Dratini: return 41_bi;
		case Species::Dragonair: return 61_bi;
		case Species::Dragonite: return 91_bi;
		case Species::Mewtwo: return 106_bi;
		case Species::Mew: return 100_bi;

		// Generation 2
		case Species::Chikorita: return 45_bi;
		case Species::Bayleef: return 60_bi;
		case Species::Meganium: return 80_bi;
		case Species::Cyndaquil: return 39_bi;
		case Species::Quilava: return 58_bi;
		case Species::Typhlosion: return 78_bi;
		case Species::Totodile: return 50_bi;
		case Species::Croconaw: return 65_bi;
		case Species::Feraligatr: return 85_bi;
		case Species::Sentret: return 35_bi;
		case Species::Furret: return 85_bi;
		case Species::Hoothoot: return 60_bi;
		case Species::Noctowl: return 100_bi;
		case Species::Ledyba: return 40_bi;
		case Species::Ledian: return 55_bi;
		case Species::Spinarak: return 40_bi;
		case Species::Ariados: return 70_bi;
		case Species::Crobat: return 85_bi;
		case Species::Chinchou: return 75_bi;
		case Species::Lanturn: return 125_bi;
		case Species::Pichu: return 20_bi;
		case Species::Cleffa: return 50_bi;
		case Species::Igglybuff: return 90_bi;
		case Species::Togepi: return 35_bi;
		case Species::Togetic: return 55_bi;
		case Species::Natu: return 40_bi;
		case Species::Xatu: return 65_bi;
		case Species::Mareep: return 55_bi;
		case Species::Flaaffy: return 70_bi;
		case Species::Ampharos: return 90_bi;
		case Species::Bellossom: return 75_bi;
		case Species::Marill: return 70_bi;
		case Species::Azumarill: return 100_bi;
		case Species::Sudowoodo: return 70_bi;
		case Species::Politoed: return 90_bi;
		case Species::Hoppip: return 35_bi;
		case Species::Skiploom: return 55_bi;
		case Species::Jumpluff: return 75_bi;
		case Species::Aipom: return 55_bi;
		case Species::Sunkern: return 30_bi;
		case Species::Sunflora: return 75_bi;
		case Species::Yanma: return 65_bi;
		case Species::Wooper: return 55_bi;
		case Species::Quagsire: return 95_bi;
		case Species::Espeon: return 65_bi;
		case Species::Umbreon: return 95_bi;
		case Species::Murkrow: return 60_bi;
		case Species::Slowking: return 95_bi;
		case Species::Misdreavus: return 60_bi;
		case Species::Unown: return 48_bi;
		case Species::Wobbuffet: return 190_bi;
		case Species::Girafarig: return 70_bi;
		case Species::Pineco: return 50_bi;
		case Species::Forretress: return 75_bi;
		case Species::Dunsparce: return 100_bi;
		case Species::Gligar: return 65_bi;
		case Species::Steelix: return 75_bi;
		case Species::Snubbull: return 60_bi;
		case Species::Granbull: return 90_bi;
		case Species::Qwilfish: return 65_bi;
		case Species::Scizor: return 70_bi;
		case Species::Shuckle: return 20_bi;
		case Species::Heracross: return 80_bi;
		case Species::Sneasel: return 55_bi;
		case Species::Teddiursa: return 60_bi;
		case Species::Ursaring: return 90_bi;
		case Species::Slugma: return 40_bi;
		case Species::Magcargo: return 50_bi;
		case Species::Swinub: return 50_bi;
		case Species::Piloswine: return 100_bi;
		case Species::Corsola: return 55_bi;
		case Species::Remoraid: return 35_bi;
		case Species::Octillery: return 75_bi;
		case Species::Delibird: return 45_bi;
		case Species::Mantine: return 65_bi;
		case Species::Skarmory: return 65_bi;
		case Species::Houndour: return 45_bi;
		case Species::Houndoom: return 75_bi;
		case Species::Kingdra: return 75_bi;
		case Species::Phanpy: return 90_bi;
		case Species::Donphan: return 90_bi;
		case Species::Porygon2: return 85_bi;
		case Species::Stantler: return 73_bi;
		case Species::Smeargle: return 55_bi;
		case Species::Tyrogue: return 35_bi;
		case Species::Hitmontop: return 50_bi;
		case Species::Smoochum: return 45_bi;
		case Species::Elekid: return 45_bi;
		case Species::Magby: return 45_bi;
		case Species::Miltank: return 95_bi;
		case Species::Blissey: return 255_bi;
		case Species::Raikou: return 90_bi;
		case Species::Entei: return 115_bi;
		case Species::Suicune: return 100_bi;
		case Species::Larvitar: return 50_bi;
		case Species::Pupitar: return 70_bi;
		case Species::Tyranitar: return 100_bi;
		case Species::Lugia: return 106_bi;
		case Species::Ho_Oh: return 106_bi;
		case Species::Celebi: return 100_bi;

		// Generation 3
		case Species::Treecko: return 40_bi;
		case Species::Grovyle: return 50_bi;
		case Species::Sceptile: return 70_bi;
		case Species::Torchic: return 45_bi;
		case Species::Combusken: return 60_bi;
		case Species::Blaziken: return 80_bi;
		case Species::Mudkip: return 50_bi;
		case Species::Marshtomp: return 70_bi;
		case Species::Swampert: return 100_bi;
		case Species::Poochyena: return 35_bi;
		case Species::Mightyena: return 70_bi;
		case Species::Zigzagoon: return 38_bi;
		case Species::Linoone: return 78_bi;
		case Species::Wurmple: return 45_bi;
		case Species::Silcoon: return 50_bi;
		case Species::Beautifly: return 60_bi;
		case Species::Cascoon: return 50_bi;
		case Species::Dustox: return 60_bi;
		case Species::Lotad: return 40_bi;
		case Species::Lombre: return 60_bi;
		case Species::Ludicolo: return 80_bi;
		case Species::Seedot: return 40_bi;
		case Species::Nuzleaf: return 70_bi;
		case Species::Shiftry: return 90_bi;
		case Species::Taillow: return 40_bi;
		case Species::Swellow: return 60_bi;
		case Species::Wingull: return 40_bi;
		case Species::Pelipper: return 60_bi;
		case Species::Ralts: return 28_bi;
		case Species::Kirlia: return 38_bi;
		case Species::Gardevoir: return 68_bi;
		case Species::Surskit: return 40_bi;
		case Species::Masquerain: return 70_bi;
		case Species::Shroomish: return 60_bi;
		case Species::Breloom: return 60_bi;
		case Species::Slakoth: return 60_bi;
		case Species::Vigoroth: return 80_bi;
		case Species::Slaking: return 150_bi;
		case Species::Nincada: return 31_bi;
		case Species::Ninjask: return 61_bi;
		case Species::Shedinja: return 1_bi;
		case Species::Whismur: return 64_bi;
		case Species::Loudred: return 84_bi;
		case Species::Exploud: return 104_bi;
		case Species::Makuhita: return 72_bi;
		case Species::Hariyama: return 144_bi;
		case Species::Azurill: return 50_bi;
		case Species::Nosepass: return 30_bi;
		case Species::Skitty: return 50_bi;
		case Species::Delcatty: return 70_bi;
		case Species::Sableye: return 50_bi;
		case Species::Mawile: return 50_bi;
		case Species::Aron: return 50_bi;
		case Species::Lairon: return 60_bi;
		case Species::Aggron: return 70_bi;
		case Species::Meditite: return 30_bi;
		case Species::Medicham: return 60_bi;
		case Species::Electrike: return 40_bi;
		case Species::Manectric: return 70_bi;
		case Species::Plusle: return 60_bi;
		case Species::Minun: return 60_bi;
		case Species::Volbeat: return 65_bi;
		case Species::Illumise: return 65_bi;
		case Species::Roselia: return 50_bi;
		case Species::Gulpin: return 70_bi;
		case Species::Swalot: return 100_bi;
		case Species::Carvanha: return 45_bi;
		case Species::Sharpedo: return 70_bi;
		case Species::Wailmer: return 130_bi;
		case Species::Wailord: return 170_bi;
		case Species::Numel: return 60_bi;
		case Species::Camerupt: return 70_bi;
		case Species::Torkoal: return 70_bi;
		case Species::Spoink: return 60_bi;
		case Species::Grumpig: return 80_bi;
		case Species::Spinda: return 60_bi;
		case Species::Trapinch: return 45_bi;
		case Species::Vibrava: return 50_bi;
		case Species::Flygon: return 80_bi;
		case Species::Cacnea: return 50_bi;
		case Species::Cacturne: return 70_bi;
		case Species::Swablu: return 45_bi;
		case Species::Altaria: return 75_bi;
		case Species::Zangoose: return 73_bi;
		case Species::Seviper: return 73_bi;
		case Species::Lunatone: return 70_bi;
		case Species::Solrock: return 70_bi;
		case Species::Barboach: return 50_bi;
		case Species::Whiscash: return 110_bi;
		case Species::Corphish: return 43_bi;
		case Species::Crawdaunt: return 63_bi;
		case Species::Baltoy: return 40_bi;
		case Species::Claydol: return 60_bi;
		case Species::Lileep: return 66_bi;
		case Species::Cradily: return 86_bi;
		case Species::Anorith: return 45_bi;
		case Species::Armaldo: return 75_bi;
		case Species::Feebas: return 20_bi;
		case Species::Milotic: return 95_bi;
		case Species::Castform: return 70_bi;
		case Species::Kecleon: return 60_bi;
		case Species::Shuppet: return 44_bi;
		case Species::Banette: return 64_bi;
		case Species::Duskull: return 20_bi;
		case Species::Dusclops: return 40_bi;
		case Species::Tropius: return 99_bi;
		case Species::Chimecho: return 65_bi;
		case Species::Absol: return 65_bi;
		case Species::Wynaut: return 95_bi;
		case Species::Snorunt: return 50_bi;
		case Species::Glalie: return 80_bi;
		case Species::Spheal: return 70_bi;
		case Species::Sealeo: return 90_bi;
		case Species::Walrein: return 110_bi;
		case Species::Clamperl: return 35_bi;
		case Species::Huntail: return 55_bi;
		case Species::Gorebyss: return 55_bi;
		case Species::Relicanth: return 100_bi;
		case Species::Luvdisc: return 43_bi;
		case Species::Bagon: return 45_bi;
		case Species::Shelgon: return 65_bi;
		case Species::Salamence: return 95_bi;
		case Species::Beldum: return 40_bi;
		case Species::Metang: return 60_bi;
		case Species::Metagross: return 80_bi;
		case Species::Regirock: return 80_bi;
		case Species::Regice: return 80_bi;
		case Species::Registeel: return 80_bi;
		case Species::Latias: return 80_bi;
		case Species::Latios: return 80_bi;
		case Species::Kyogre: return 100_bi;
		case Species::Groudon: return 100_bi;
		case Species::Rayquaza: return 105_bi;
		case Species::Jirachi: return 100_bi;
		case Species::Deoxys_Mediocre: return 50_bi;
		case Species::Deoxys_Attack: return 50_bi;
		case Species::Deoxys_Defense: return 50_bi;
		case Species::Deoxys_Speed: return 50_bi;

		// Generation 4
		case Species::Turtwig: return 55_bi;
		case Species::Grotle: return 75_bi;
		case Species::Torterra: return 95_bi;
		case Species::Chimchar: return 44_bi;
		case Species::Monferno: return 64_bi;
		case Species::Infernape: return 76_bi;
		case Species::Piplup: return 53_bi;
		case Species::Prinplup: return 64_bi;
		case Species::Empoleon: return 84_bi;
		case Species::Starly: return 40_bi;
		case Species::Staravia: return 55_bi;
		case Species::Staraptor: return 85_bi;
		case Species::Bidoof: return 59_bi;
		case Species::Bibarel: return 79_bi;
		case Species::Kricketot: return 37_bi;
		case Species::Kricketune: return 77_bi;
		case Species::Shinx: return 45_bi;
		case Species::Luxio: return 60_bi;
		case Species::Luxray: return 80_bi;
		case Species::Budew: return 40_bi;
		case Species::Roserade: return 60_bi;
		case Species::Cranidos: return 67_bi;
		case Species::Rampardos: return 97_bi;
		case Species::Shieldon: return 30_bi;
		case Species::Bastiodon: return 60_bi;
		case Species::Burmy: return 40_bi;
		case Species::Wormadam_Plant: return 60_bi;
		case Species::Wormadam_Sandy: return 60_bi;
		case Species::Wormadam_Trash: return 60_bi;
		case Species::Mothim: return 70_bi;
		case Species::Combee: return 30_bi;
		case Species::Vespiquen: return 70_bi;
		case Species::Pachirisu: return 60_bi;
		case Species::Buizel: return 55_bi;
		case Species::Floatzel: return 85_bi;
		case Species::Cherubi: return 45_bi;
		case Species::Cherrim: return 70_bi;
		case Species::Shellos: return 76_bi;
		case Species::Gastrodon: return 111_bi;
		case Species::Ambipom: return 75_bi;
		case Species::Drifloon: return 90_bi;
		case Species::Drifblim: return 150_bi;
		case Species::Buneary: return 55_bi;
		case Species::Lopunny: return 65_bi;
		case Species::Mismagius: return 60_bi;
		case Species::Honchkrow: return 100_bi;
		case Species::Glameow: return 49_bi;
		case Species::Purugly: return 71_bi;
		case Species::Chingling: return 45_bi;
		case Species::Stunky: return 63_bi;
		case Species::Skuntank: return 103_bi;
		case Species::Bronzor: return 57_bi;
		case Species::Bronzong: return 67_bi;
		case Species::Bonsly: return 50_bi;
		case Species::Mime_Jr: return 20_bi;
		case Species::Happiny: return 100_bi;
		case Species::Chatot: return 76_bi;
		case Species::Spiritomb: return 50_bi;
		case Species::Gible: return 58_bi;
		case Species::Gabite: return 68_bi;
		case Species::Garchomp: return 108_bi;
		case Species::Munchlax: return 135_bi;
		case Species::Riolu: return 40_bi;
		case Species::Lucario: return 70_bi;
		case Species::Hippopotas: return 68_bi;
		case Species::Hippowdon: return 108_bi;
		case Species::Skorupi: return 40_bi;
		case Species::Drapion: return 70_bi;
		case Species::Croagunk: return 48_bi;
		case Species::Toxicroak: return 83_bi;
		case Species::Carnivine: return 74_bi;
		case Species::Finneon: return 49_bi;
		case Species::Lumineon: return 69_bi;
		case Species::Mantyke: return 45_bi;
		case Species::Snover: return 60_bi;
		case Species::Abomasnow: return 90_bi;
		case Species::Weavile: return 70_bi;
		case Species::Magnezone: return 70_bi;
		case Species::Lickilicky: return 110_bi;
		case Species::Rhyperior: return 115_bi;
		case Species::Tangrowth: return 100_bi;
		case Species::Electivire: return 75_bi;
		case Species::Magmortar: return 75_bi;
		case Species::Togekiss: return 85_bi;
		case Species::Yanmega: return 86_bi;
		case Species::Leafeon: return 65_bi;
		case Species::Glaceon: return 65_bi;
		case Species::Gliscor: return 75_bi;
		case Species::Mamoswine: return 110_bi;
		case Species::Porygon_Z: return 85_bi;
		case Species::Gallade: return 68_bi;
		case Species::Probopass: return 60_bi;
		case Species::Dusknoir: return 45_bi;
		case Species::Froslass: return 70_bi;
		case Species::Rotom: return 50_bi;
		case Species::Rotom_Heat: return 50_bi;
		case Species::Rotom_Wash: return 50_bi;
		case Species::Rotom_Frost: return 50_bi;
		case Species::Rotom_Fan: return 50_bi;
		case Species::Rotom_Mow: return 50_bi;
		case Species::Uxie: return 75_bi;
		case Species::Mesprit: return 80_bi;
		case Species::Azelf: return 75_bi;
		case Species::Dialga: return 100_bi;
		case Species::Palkia: return 90_bi;
		case Species::Heatran: return 91_bi;
		case Species::Regigigas: return 110_bi;
		case Species::Giratina_Altered: return 150_bi;
		case Species::Giratina_Origin: return 150_bi;
		case Species::Cresselia: return 120_bi;
		case Species::Phione: return 80_bi;
		case Species::Manaphy: return 100_bi;
		case Species::Darkrai: return 70_bi;
		case Species::Shaymin_Land: return 100_bi;
		case Species::Shaymin_Sky: return 100_bi;
		case Species::Arceus: return 120_bi;

		// Generation 5
		case Species::Victini: return 100_bi;
		case Species::Snivy: return 45_bi;
		case Species::Servine: return 60_bi;
		case Species::Serperior: return 75_bi;
		case Species::Tepig: return 65_bi;
		case Species::Pignite: return 90_bi;
		case Species::Emboar: return 110_bi;
		case Species::Oshawott: return 55_bi;
		case Species::Dewott: return 75_bi;
		case Species::Samurott: return 95_bi;
		case Species::Patrat: return 45_bi;
		case Species::Watchog: return 60_bi;
		case Species::Lillipup: return 45_bi;
		case Species::Herdier: return 65_bi;
		case Species::Stoutland: return 85_bi;
		case Species::Purrloin: return 41_bi;
		case Species::Liepard: return 64_bi;
		case Species::Pansage: return 50_bi;
		case Species::Simisage: return 75_bi;
		case Species::Pansear: return 50_bi;
		case Species::Simisear: return 75_bi;
		case Species::Panpour: return 50_bi;
		case Species::Simipour: return 75_bi;
		case Species::Munna: return 76_bi;
		case Species::Musharna: return 116_bi;
		case Species::Pidove: return 50_bi;
		case Species::Tranquill: return 62_bi;
		case Species::Unfezant: return 80_bi;
		case Species::Blitzle: return 45_bi;
		case Species::Zebstrika: return 75_bi;
		case Species::Roggenrola: return 55_bi;
		case Species::Boldore: return 70_bi;
		case Species::Gigalith: return 85_bi;
		case Species::Woobat: return 55_bi;
		case Species::Swoobat: return 67_bi;
		case Species::Drilbur: return 60_bi;
		case Species::Excadrill: return 110_bi;
		case Species::Audino: return 103_bi;
		case Species::Timburr: return 75_bi;
		case Species::Gurdurr: return 85_bi;
		case Species::Conkeldurr: return 105_bi;
		case Species::Tympole: return 50_bi;
		case Species::Palpitoad: return 75_bi;
		case Species::Seismitoad: return 105_bi;
		case Species::Throh: return 120_bi;
		case Species::Sawk: return 75_bi;
		case Species::Sewaddle: return 45_bi;
		case Species::Swadloon: return 55_bi;
		case Species::Leavanny: return 75_bi;
		case Species::Venipede: return 30_bi;
		case Species::Whirlipede: return 40_bi;
		case Species::Scolipede: return 60_bi;
		case Species::Cottonee: return 40_bi;
		case Species::Whimsicott: return 60_bi;
		case Species::Petilil: return 45_bi;
		case Species::Lilligant: return 70_bi;
		case Species::Basculin_Red: return 70_bi;
		case Species::Basculin_Blue: return 70_bi;
		case Species::Sandile: return 50_bi;
		case Species::Krokorok: return 60_bi;
		case Species::Krookodile: return 95_bi;
		case Species::Darumaka: return 70_bi;
		case Species::Darmanitan: return 105_bi;
		// case Species::Darmanitan_Zen: return 105_bi;
		case Species::Maractus: return 75_bi;
		case Species::Dwebble: return 50_bi;
		case Species::Crustle: return 70_bi;
		case Species::Scraggy: return 50_bi;
		case Species::Scrafty: return 65_bi;
		case Species::Sigilyph: return 72_bi;
		case Species::Yamask: return 38_bi;
		case Species::Cofagrigus: return 58_bi;
		case Species::Tirtouga: return 54_bi;
		case Species::Carracosta: return 74_bi;
		case Species::Archen: return 55_bi;
		case Species::Archeops: return 75_bi;
		case Species::Trubbish: return 50_bi;
		case Species::Garbodor: return 80_bi;
		case Species::Zorua: return 40_bi;
		case Species::Zoroark: return 60_bi;
		case Species::Minccino: return 55_bi;
		case Species::Cinccino: return 75_bi;
		case Species::Gothita: return 45_bi;
		case Species::Gothorita: return 60_bi;
		case Species::Gothitelle: return 70_bi;
		case Species::Solosis: return 45_bi;
		case Species::Duosion: return 65_bi;
		case Species::Reuniclus: return 110_bi;
		case Species::Ducklett: return 62_bi;
		case Species::Swanna: return 75_bi;
		case Species::Vanillite: return 36_bi;
		case Species::Vanillish: return 51_bi;
		case Species::Vanilluxe: return 71_bi;
		case Species::Deerling: return 60_bi;
		case Species::Sawsbuck: return 80_bi;
		case Species::Emolga: return 55_bi;
		case Species::Karrablast: return 50_bi;
		case Species::Escavalier: return 70_bi;
		case Species::Foongus: return 69_bi;
		case Species::Amoonguss: return 114_bi;
		case Species::Frillish: return 55_bi;
		case Species::Jellicent: return 100_bi;
		case Species::Alomomola: return 165_bi;
		case Species::Joltik: return 50_bi;
		case Species::Galvantula: return 70_bi;
		case Species::Ferroseed: return 44_bi;
		case Species::Ferrothorn: return 74_bi;
		case Species::Klink: return 40_bi;
		case Species::Klang: return 60_bi;
		case Species::Klinklang: return 60_bi;
		case Species::Tynamo: return 35_bi;
		case Species::Eelektrik: return 65_bi;
		case Species::Eelektross: return 85_bi;
		case Species::Elgyem: return 55_bi;
		case Species::Beheeyem: return 75_bi;
		case Species::Litwick: return 50_bi;
		case Species::Lampent: return 60_bi;
		case Species::Chandelure: return 60_bi;
		case Species::Axew: return 46_bi;
		case Species::Fraxure: return 66_bi;
		case Species::Haxorus: return 76_bi;
		case Species::Cubchoo: return 55_bi;
		case Species::Beartic: return 95_bi;
		case Species::Cryogonal: return 70_bi;
		case Species::Shelmet: return 50_bi;
		case Species::Accelgor: return 80_bi;
		case Species::Stunfisk: return 109_bi;
		case Species::Mienfoo: return 45_bi;
		case Species::Mienshao: return 65_bi;
		case Species::Druddigon: return 77_bi;
		case Species::Golett: return 59_bi;
		case Species::Golurk: return 89_bi;
		case Species::Pawniard: return 45_bi;
		case Species::Bisharp: return 65_bi;
		case Species::Bouffalant: return 95_bi;
		case Species::Rufflet: return 70_bi;
		case Species::Braviary: return 100_bi;
		case Species::Vullaby: return 70_bi;
		case Species::Mandibuzz: return 110_bi;
		case Species::Heatmor: return 85_bi;
		case Species::Durant: return 58_bi;
		case Species::Deino: return 52_bi;
		case Species::Zweilous: return 72_bi;
		case Species::Hydreigon: return 92_bi;
		case Species::Larvesta: return 55_bi;
		case Species::Volcarona: return 85_bi;
		case Species::Cobalion: return 91_bi;
		case Species::Terrakion: return 91_bi;
		case Species::Virizion: return 91_bi;
		case Species::Tornadus_Incarnate: return 79_bi;
		case Species::Tornadus_Therian: return 79_bi;
		case Species::Thundurus_Incarnate: return 79_bi;
		case Species::Thundurus_Therian: return 79_bi;
		case Species::Reshiram: return 100_bi;
		case Species::Zekrom: return 100_bi;
		case Species::Landorus_Incarnate: return 89_bi;
		case Species::Landorus_Therian: return 89_bi;
		case Species::Kyurem: return 125_bi;
		case Species::Kyurem_Black: return 125_bi;
		case Species::Kyurem_White: return 125_bi;
		case Species::Keldeo: return 91_bi;
		case Species::Meloetta: return 100_bi;
		// case Species::Meloetta_Pirouette: return 100_bi;
		case Species::Genesect: return 71_bi;
		default: assert(false);
	}
}

}	// namespace
}	// namespace technicalmachine
