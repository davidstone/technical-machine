// Copyright (C) 2019 David Stone
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

#include <tm/stat/base_stats.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {

using namespace bounded::literal;

BaseStats::BaseStats(Generation const generation, Species const species):
	BaseStats([=]() -> BaseStats {
		switch (species) {
			case Species::Bulbasaur: return {45_bi, 49_bi, 49_bi, 65_bi, 65_bi, 45_bi};
			case Species::Ivysaur: return {60_bi, 62_bi, 63_bi, 80_bi, 80_bi, 60_bi};
			case Species::Venusaur: return {80_bi, 82_bi, 83_bi, 100_bi, 100_bi, 80_bi};
			// case Species::Venusaur_Mega: return {80_bi, 100_bi, 123_bi, 122_bi, 120_bi, 80_bi};
			case Species::Charmander: return {
				39_bi,
				52_bi,
				43_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 60_bi),
				50_bi,
				65_bi
			};
			case Species::Charmeleon: return {
				58_bi,
				64_bi,
				58_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 80_bi),
				65_bi,
				80_bi
			};
			case Species::Charizard: return {
				78_bi,
				84_bi,
				78_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 109_bi),
				85_bi,
				100_bi
			};
			// case Species::Charizard_Mega_X: return {78_bi, 130_bi, 111_bi, 130_bi, 85_bi, 100_bi};
			// case Species::Charizard_Mega_Y: return {78_bi, 104_bi, 78_bi, 159_bi, 115_bi, 100_bi};
			case Species::Squirtle: return {
				44_bi,
				48_bi,
				65_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 64_bi),
				43_bi
			};
			case Species::Wartortle: return {
				59_bi,
				63_bi,
				80_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 80_bi),
				58_bi
			};
			case Species::Blastoise: return {
				79_bi,
				83_bi,
				100_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 105_bi),
				78_bi
			};
			// case Species::Blastoise_Mega: return {79_bi, 103_bi, 120_bi, 135_bi, 115_bi, 78_bi};
			case Species::Caterpie: return {45_bi, 30_bi, 35_bi, 20_bi, 20_bi, 45_bi};
			case Species::Metapod: return {50_bi, 20_bi, 55_bi, 25_bi, 25_bi, 30_bi};
			case Species::Butterfree: return {
				60_bi,
				45_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 80_bi, 90_bi),
				80_bi,
				70_bi
			};
			case Species::Weedle: return {40_bi, 35_bi, 30_bi, 20_bi, 20_bi, 50_bi};
			case Species::Kakuna: return {45_bi, 25_bi, 50_bi, 25_bi, 25_bi, 35_bi};
			case Species::Beedrill: return {
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 80_bi, 90_bi),
				40_bi,
				45_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 45_bi, 80_bi),
				75_bi
			};
			// case Species::Beedrill_Mega: return {65_bi, 150_bi, 40_bi, 15_bi, 80_bi, 145_bi};
			case Species::Pidgey: return {40_bi, 45_bi, 40_bi, 35_bi, 35_bi, 56_bi};
			case Species::Pidgeotto: return {63_bi, 60_bi, 55_bi, 50_bi, 50_bi, 71_bi};
			case Species::Pidgeot: return {
				83_bi,
				80_bi,
				75_bi,
				70_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 91_bi, 101_bi)
			};
			// case Species::Pidgeot_Mega: return {83_bi, 80_bi, 80_bi, 135_bi, 80_bi, 121_bi};
			case Species::Rattata: return {
				30_bi,
				56_bi,
				35_bi,
				25_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 25_bi, 35_bi),
				72_bi
			};
			case Species::Rattata_Alolan: return {30_bi, 56_bi, 35_bi, 25_bi, 35_bi, 72_bi};
			case Species::Raticate: return {
				55_bi,
				81_bi,
				60_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 70_bi),
				97_bi
			};
			case Species::Raticate_Alolan: return {75_bi, 71_bi, 70_bi, 40_bi, 80_bi, 77_bi};
			case Species::Spearow: return {40_bi, 60_bi, 30_bi, 31_bi, 31_bi, 70_bi};
			case Species::Fearow: return {65_bi, 90_bi, 65_bi, 61_bi, 61_bi, 100_bi};
			case Species::Ekans: return {
				35_bi,
				60_bi,
				44_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 54_bi),
				55_bi
			};
			case Species::Arbok: return {
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 85_bi, 95_bi),
				69_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 79_bi),
				80_bi
			};
			case Species::Pikachu: return {
				35_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 30_bi, 40_bi),
				50_bi,
				50_bi,
				90_bi
			};
			case Species::Raichu: return {
				60_bi,
				90_bi,
				55_bi,
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 90_bi, 80_bi),
				BOUNDED_CONDITIONAL(generation <= Generation::five, 100_bi, 110_bi)
			};
			case Species::Raichu_Alolan: return {60_bi, 85_bi, 50_bi, 95_bi, 85_bi, 110_bi};
			case Species::Sandshrew: return {
				50_bi,
				75_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 30_bi, 20_bi),
				30_bi,
				40_bi
			};
			case Species::Sandshrew_Alolan: return {50_bi, 75_bi, 90_bi, 10_bi, 35_bi, 40_bi};
			case Species::Sandslash: return {
				75_bi,
				100_bi,
				110_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 55_bi, 45_bi),
				55_bi,
				65_bi
			};
			case Species::Sandslash_Alolan: return {75_bi, 100_bi, 120_bi, 25_bi, 65_bi, 65_bi};
			case Species::Nidoran_F: return {55_bi, 47_bi, 52_bi, 40_bi, 40_bi, 41_bi};
			case Species::Nidorina: return {70_bi, 62_bi, 67_bi, 55_bi, 55_bi, 56_bi};
			case Species::Nidoqueen: return {
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 82_bi, 92_bi),
				87_bi,
				75_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 75_bi, 85_bi),
				76_bi
			};
			case Species::Nidoran_M: return {46_bi, 57_bi, 40_bi, 40_bi, 40_bi, 50_bi};
			case Species::Nidorino: return {61_bi, 72_bi, 57_bi, 55_bi, 55_bi, 65_bi};
			case Species::Nidoking: return {
				81_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 92_bi, 102_bi),
				77_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 75_bi, 85_bi),
				75_bi,
				85_bi
			};
			case Species::Clefairy: return {
				70_bi,
				45_bi,
				48_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 65_bi),
				35_bi
			};
			case Species::Clefable: return {
				95_bi,
				70_bi,
				73_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi),
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 90_bi),
				60_bi
			};
			case Species::Vulpix: return {
				38_bi,
				41_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 50_bi),
				65_bi,
				65_bi
			};
			case Species::Vulpix_Alolan: return {38_bi, 41_bi, 40_bi, 50_bi, 65_bi, 65_bi};
			case Species::Ninetales: return {
				73_bi,
				76_bi,
				75_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 81_bi),
				100_bi,
				100_bi
			};
			case Species::Ninetales_Alolan: return {73_bi, 67_bi, 75_bi, 81_bi, 100_bi, 109_bi};
			case Species::Jigglypuff: return {
				115_bi,
				45_bi,
				20_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 25_bi, 45_bi),
				25_bi,
				20_bi
			};
			case Species::Wigglytuff: return {
				140_bi,
				70_bi,
				45_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, BOUNDED_CONDITIONAL(generation <= Generation::five, 75_bi, 85_bi)),
				50_bi,
				45_bi
			};
			case Species::Zubat: return {
				40_bi,
				45_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 30_bi),
				40_bi,
				55_bi
			};
			case Species::Golbat: return {
				75_bi,
				80_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 75_bi, 65_bi),
				75_bi,
				90_bi
			};
			case Species::Oddish: return {
				45_bi,
				50_bi,
				55_bi,
				75_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 75_bi, 65_bi),
				30_bi
			};
			case Species::Gloom: return {
				60_bi,
				65_bi,
				70_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 75_bi),
				40_bi
			};
			case Species::Vileplume: return {
				75_bi,
				80_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 100_bi, 110_bi),
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 90_bi),
				50_bi
			};
			case Species::Paras: return {
				35_bi,
				70_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 55_bi, 45_bi),
				55_bi,
				25_bi
			};
			case Species::Parasect: return {
				60_bi,
				95_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 80_bi, 60_bi),
				80_bi,
				30_bi
			};
			case Species::Venonat: return {
				60_bi,
				55_bi,
				50_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 55_bi),
				45_bi
			};
			case Species::Venomoth: return {
				70_bi,
				65_bi,
				60_bi,
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 90_bi, 75_bi),
				90_bi
			};
			case Species::Diglett: return {
				10_bi,
				55_bi,
				25_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 45_bi, 35_bi),
				45_bi,
				95_bi
			};
			case Species::Diglett_Alolan: return {10_bi, 55_bi, 30_bi, 35_bi, 45_bi, 90_bi};
			case Species::Dugtrio: return {
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 80_bi, 100_bi),
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 50_bi),
				70_bi,
				120_bi
			};
			case Species::Dugtrio_Alolan: return {35_bi, 100_bi, 60_bi, 50_bi, 70_bi, 110_bi};
			case Species::Meowth: return {40_bi, 45_bi, 35_bi, 40_bi, 40_bi, 90_bi};
			case Species::Meowth_Alolan: return {40_bi, 35_bi, 35_bi, 50_bi, 40_bi, 90_bi};
			case Species::Persian: return {65_bi, 70_bi, 60_bi, 65_bi, 65_bi, 115_bi};
			case Species::Persian_Alolan: return {65_bi, 60_bi, 60_bi, 75_bi, 65_bi, 115_bi};
			case Species::Psyduck: return {
				50_bi,
				52_bi,
				48_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 65_bi),
				50_bi,
				55_bi
			};
			case Species::Golduck: return {
				80_bi,
				82_bi,
				78_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 80_bi, 95_bi),
				80_bi,
				85_bi
			};
			case Species::Mankey: return {
				40_bi,
				80_bi,
				35_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 35_bi, 45_bi),
				70_bi
			};
			case Species::Primeape: return {
				65_bi,
				105_bi,
				60_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 70_bi),
				95_bi
			};
			case Species::Growlithe: return {
				55_bi,
				70_bi,
				45_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 70_bi),
				50_bi,
				60_bi
			};
			case Species::Arcanine: return {
				90_bi,
				110_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 80_bi, 100_bi),
				80_bi,
				95_bi
			};
			case Species::Poliwag: return {40_bi, 50_bi, 40_bi, 40_bi, 40_bi, 90_bi};
			case Species::Poliwhirl: return {65_bi, 65_bi, 65_bi, 50_bi, 50_bi, 90_bi};
			case Species::Poliwrath: return {
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi),
				95_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 90_bi),
				70_bi
			};
			case Species::Abra: return {
				25_bi,
				20_bi,
				15_bi,
				105_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 105_bi, 55_bi),
				90_bi
			};
			case Species::Kadabra: return {
				40_bi,
				35_bi,
				30_bi,
				120_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 120_bi, 70_bi),
				105_bi
			};
			case Species::Alakazam: return {
				55_bi,
				50_bi,
				45_bi,
				135_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 135_bi, BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi)),
				120_bi
			};
			#if 0
			case Species::Alakazam_Mega: return {
				55_bi,
				50_bi,
				65_bi,
				175_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 95_bi, 105_bi),
				150_bi
			};
			#endif
			case Species::Machop: return {70_bi, 80_bi, 50_bi, 35_bi, 35_bi, 35_bi};
			case Species::Machoke: return {
				80_bi,
				100_bi,
				70_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 60_bi),
				45_bi
			};
			case Species::Machamp: return {
				90_bi,
				130_bi,
				80_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 85_bi),
				55_bi
			};
			case Species::Bellsprout: return {
				50_bi,
				75_bi,
				35_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 30_bi),
				40_bi
			};
			case Species::Weepinbell: return {
				65_bi,
				90_bi,
				50_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 45_bi),
				55_bi
			};
			case Species::Victreebel: return {
				80_bi,
				105_bi,
				65_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, BOUNDED_CONDITIONAL(generation <= Generation::five, 60_bi, 70_bi)),
				70_bi
			};
			case Species::Tentacool: return {
				40_bi,
				40_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 50_bi),
				100_bi,
				70_bi
			};
			case Species::Tentacruel: return {
				80_bi,
				70_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 120_bi, 80_bi),
				120_bi,
				100_bi
			};
			case Species::Geodude: return {40_bi, 80_bi, 100_bi, 30_bi, 30_bi, 20_bi};
			case Species::Geodude_Alolan: return {40_bi, 80_bi, 100_bi, 30_bi, 30_bi, 20_bi};
			case Species::Graveler: return {55_bi, 95_bi, 115_bi, 45_bi, 45_bi, 35_bi};
			case Species::Graveler_Alolan: return {55_bi, 95_bi, 115_bi, 45_bi, 45_bi, 35_bi};
			case Species::Golem: return {
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 110_bi, 120_bi),
				130_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 55_bi, 65_bi),
				45_bi
			};
			case Species::Golem_Alolan: return {80_bi, 120_bi, 130_bi, 55_bi, 65_bi, 45_bi};
			case Species::Ponyta: return {50_bi, 85_bi, 55_bi, 65_bi, 65_bi, 90_bi};
			case Species::Rapidash: return {65_bi, 100_bi, 70_bi, 80_bi, 80_bi, 105_bi};
			case Species::Slowpoke: return {90_bi, 65_bi, 65_bi, 40_bi, 40_bi, 15_bi};
			case Species::Slowbro: return {
				95_bi,
				75_bi,
				110_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 80_bi, 100_bi),
				80_bi,
				30_bi
			};
			// case Species::Slowbro_Mega: return {95_bi, 75_bi, 180_bi, 130_bi, 80_bi, 30_bi};
			case Species::Magnemite: return {
				25_bi,
				35_bi,
				70_bi,
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 95_bi, 55_bi),
				45_bi
			};
			case Species::Magneton: return {
				50_bi,
				60_bi,
				95_bi,
				120_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 120_bi, 70_bi),
				70_bi
			};
			case Species::Farfetchd: return {
				52_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 65_bi, 90_bi),
				55_bi,
				58_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 58_bi, 62_bi),
				60_bi
			};
			case Species::Doduo: return {35_bi, 85_bi, 45_bi, 35_bi, 35_bi, 75_bi};
			case Species::Dodrio: return {
				60_bi,
				110_bi,
				70_bi,
				60_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 100_bi, 110_bi),
			};
			case Species::Seel: return {
				65_bi,
				45_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 45_bi),
				70_bi,
				45_bi
			};
			case Species::Dewgong: return {
				90_bi,
				70_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 95_bi, 70_bi),
				95_bi,
				70_bi
			};
			case Species::Grimer: return {
				80_bi,
				80_bi,
				50_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 50_bi),
				25_bi
			};
			case Species::Grimer_Alolan: return {80_bi, 80_bi, 50_bi, 40_bi, 50_bi, 25_bi};
			case Species::Muk: return {
				105_bi,
				105_bi,
				75_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 100_bi),
				50_bi
			};
			case Species::Muk_Alolan: return {105_bi, 105_bi, 75_bi, 65_bi, 100_bi, 50_bi};
			case Species::Shellder: return {
				30_bi,
				65_bi,
				100_bi,
				45_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 45_bi, 25_bi),
				40_bi
			};
			case Species::Cloyster: return {
				50_bi,
				95_bi,
				180_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 45_bi),
				70_bi
			};
			case Species::Gastly: return {
				30_bi,
				35_bi,
				30_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 35_bi),
				80_bi
			};
			case Species::Haunter: return {
				45_bi,
				50_bi,
				45_bi,
				115_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 115_bi, 55_bi),
				95_bi
			};
			case Species::Gengar: return {
				60_bi,
				65_bi,
				60_bi,
				130_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 130_bi, 75_bi),
				110_bi
			};
			// case Species::Gengar_Mega: return {60_bi, 65_bi, 80_bi, 170_bi, 95_bi, 130_bi};
			case Species::Onix: return {
				35_bi,
				45_bi,
				160_bi,
				30_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 30_bi, 45_bi),
				70_bi
			};
			case Species::Drowzee: return {
				60_bi,
				48_bi,
				45_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 90_bi, 43_bi),
				90_bi,
				42_bi
			};
			case Species::Hypno: return {
				85_bi,
				73_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 115_bi, 73_bi),
				115_bi,
				67_bi
			};
			case Species::Krabby: return {30_bi, 105_bi, 90_bi, 25_bi, 25_bi, 50_bi};
			case Species::Kingler: return {55_bi, 130_bi, 115_bi, 50_bi, 50_bi, 75_bi};
			case Species::Voltorb: return {40_bi, 30_bi, 50_bi, 55_bi, 55_bi, 100_bi};
			case Species::Electrode: return {
				60_bi,
				50_bi,
				70_bi,
				80_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 140_bi, 150_bi)
			};
			case Species::Exeggcute: return {
				60_bi,
				40_bi,
				80_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 45_bi),
				40_bi
			};
			case Species::Exeggutor: return {
				95_bi,
				95_bi,
				85_bi,
				125_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 125_bi, BOUNDED_CONDITIONAL(generation <= Generation::six, 65_bi, 75_bi)),
				55_bi
			};
			case Species::Exeggutor_Alolan: return {95_bi, 105_bi, 85_bi, 125_bi, 75_bi, 45_bi};
			case Species::Cubone: return {
				50_bi,
				50_bi,
				95_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 50_bi),
				35_bi
			};
			case Species::Marowak: return {
				60_bi,
				80_bi,
				110_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 80_bi),
				45_bi
			};
			case Species::Marowak_Alolan: return {60_bi, 80_bi, 110_bi, 50_bi, 80_bi, 45_bi};
			case Species::Hitmonlee: return {
				50_bi,
				120_bi,
				53_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 35_bi, 110_bi),
				87_bi
			};
			case Species::Hitmonchan: return {
				50_bi,
				105_bi,
				79_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 35_bi, 110_bi),
				76_bi
			};
			case Species::Lickitung: return {
				90_bi,
				55_bi,
				75_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 75_bi),
				30_bi
			};
			case Species::Koffing: return {
				40_bi,
				65_bi,
				95_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 45_bi),
				35_bi
			};
			case Species::Weezing: return {
				65_bi,
				90_bi,
				120_bi,
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 70_bi),
				60_bi
			};
			case Species::Rhyhorn: return {80_bi, 85_bi, 95_bi, 30_bi, 30_bi, 25_bi};
			case Species::Rhydon: return {105_bi, 130_bi, 120_bi, 45_bi, 45_bi, 40_bi};
			case Species::Chansey: return {
				250_bi,
				5_bi,
				5_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 105_bi, 35_bi),
				105_bi,
				50_bi
			};
			case Species::Tangela: return {
				65_bi,
				55_bi,
				115_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 40_bi),
				60_bi
			};
			case Species::Kangaskhan: return {
				105_bi,
				95_bi,
				80_bi,
				40_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 40_bi, 80_bi),
				90_bi
			};
			// case Species::Kangaskhan_Mega: return {105_bi, 125_bi, 100_bi, 60_bi, 100_bi, 100_bi};
			case Species::Horsea: return {
				30_bi,
				40_bi,
				70_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 25_bi),
				60_bi
			};
			case Species::Seadra: return {
				55_bi,
				65_bi,
				95_bi,
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 95_bi, 45_bi),
				85_bi
			};
			case Species::Goldeen: return {
				45_bi,
				67_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 50_bi, 35_bi),
				50_bi,
				63_bi
			};
			case Species::Seaking: return {
				80_bi,
				92_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 80_bi, 65_bi),
				80_bi,
				68_bi
			};
			case Species::Staryu: return {
				30_bi,
				45_bi,
				55_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 55_bi),
				85_bi
			};
			case Species::Starmie: return {
				60_bi,
				75_bi,
				85_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 85_bi),
				115_bi
			};
			case Species::Mr_Mime: return {
				40_bi,
				45_bi,
				65_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 120_bi),
				90_bi
			};
			case Species::Scyther: return {
				70_bi,
				110_bi,
				80_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 55_bi, 80_bi),
				105_bi
			};
			case Species::Jynx: return {
				65_bi,
				50_bi,
				35_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 95_bi, 115_bi),
				95_bi,
				95_bi
			};
			case Species::Electabuzz: return {
				65_bi,
				83_bi,
				57_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 95_bi),
				85_bi,
				105_bi
			};
			case Species::Magmar: return {
				65_bi,
				95_bi,
				57_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 85_bi, 100_bi),
				85_bi,
				93_bi
			};
			case Species::Pinsir: return {
				65_bi,
				125_bi,
				100_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 55_bi, 70_bi),
				85_bi
			};
			// case Species::Pinsir_Mega: return {65_bi, 155_bi, 120_bi, 65_bi, 90_bi, 105_bi};
			case Species::Tauros: return {
				75_bi,
				100_bi,
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 40_bi),
				70_bi,
				110_bi
			};
			case Species::Magikarp: return {
				20_bi,
				10_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 20_bi, 15_bi),
				20_bi,
				80_bi
			};
			case Species::Gyarados: return {
				95_bi,
				125_bi,
				79_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 100_bi, 60_bi),
				100_bi,
				81_bi
			};
			// case Species::Gyarados_Mega: return {95_bi, 155_bi, 109_bi, 70_bi, 130_bi, 81_bi};
			case Species::Lapras: return {
				130_bi,
				85_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 95_bi, 85_bi),
				95_bi,
				60_bi
			};
			case Species::Ditto: return {48_bi, 48_bi, 48_bi, 48_bi, 48_bi, 48_bi};
			case Species::Eevee: return {
				55_bi,
				55_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 45_bi),
				65_bi,
				55_bi
			};
			case Species::Vaporeon: return {
				130_bi,
				65_bi,
				60_bi,
				110_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 110_bi, 95_bi),
				65_bi
			};
			case Species::Jolteon: return {
				65_bi,
				65_bi,
				60_bi,
				110_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 110_bi, 95_bi),
				130_bi
			};
			case Species::Flareon: return {
				65_bi,
				130_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 110_bi, 95_bi),
				110_bi,
				65_bi
			};
			case Species::Porygon: return {
				65_bi,
				60_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 75_bi, 85_bi),
				75_bi,
				40_bi
			};
			case Species::Omanyte: return {
				35_bi,
				40_bi,
				100_bi,
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 90_bi, 55_bi),
				35_bi
			};
			case Species::Omastar: return {
				70_bi,
				60_bi,
				125_bi,
				115_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 115_bi, 70_bi),
				55_bi
			};
			case Species::Kabuto: return {
				30_bi,
				80_bi,
				90_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 45_bi, 55_bi),
				45_bi,
				55_bi
			};
			case Species::Kabutops: return {
				60_bi,
				115_bi,
				105_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 70_bi, 65_bi),
				70_bi,
				80_bi
			};
			case Species::Aerodactyl: return {
				80_bi,
				105_bi,
				65_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 60_bi, 75_bi),
				130_bi
			};
			// case Species::Aerodactyl_Mega: return {80_bi, 135_bi, 85_bi, 70_bi, 95_bi, 150_bi};
			case Species::Snorlax: return {
				160_bi,
				110_bi,
				65_bi,
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 65_bi, 110_bi),
				30_bi
			};
			case Species::Articuno: return {
				90_bi,
				85_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 125_bi, 95_bi),
				125_bi,
				85_bi
			};
			case Species::Zapdos: return {
				90_bi,
				90_bi,
				85_bi,
				125_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 125_bi, 90_bi),
				100_bi
			};
			case Species::Moltres: return {
				90_bi,
				100_bi,
				90_bi,
				125_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 125_bi, 85_bi),
				90_bi
			};
			case Species::Dratini: return {41_bi, 64_bi, 45_bi, 50_bi, 50_bi, 50_bi};
			case Species::Dragonair: return {61_bi, 84_bi, 65_bi, 70_bi, 70_bi, 70_bi};
			case Species::Dragonite: return {91_bi, 134_bi, 95_bi, 100_bi, 100_bi, 80_bi};
			case Species::Mewtwo: return {
				106_bi,
				110_bi,
				90_bi,
				154_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::one, 154_bi, 90_bi),
				130_bi
			};
			// case Species::Mewtwo_Mega_X: return {106_bi, 190_bi, 100_bi, 154_bi, 100_bi, 130_bi};
			// case Species::Mewtwo_Mega_Y: return {106_bi, 150_bi, 70_bi, 194_bi, 120_bi, 140_bi};
			case Species::Mew: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Chikorita: return {45_bi, 49_bi, 65_bi, 49_bi, 65_bi, 45_bi};
			case Species::Bayleef: return {60_bi, 62_bi, 80_bi, 63_bi, 80_bi, 60_bi};
			case Species::Meganium: return {80_bi, 82_bi, 100_bi, 83_bi, 100_bi, 80_bi};
			case Species::Cyndaquil: return {39_bi, 52_bi, 43_bi, 60_bi, 50_bi, 65_bi};
			case Species::Quilava: return {58_bi, 64_bi, 58_bi, 80_bi, 65_bi, 80_bi};
			case Species::Typhlosion: return {78_bi, 84_bi, 78_bi, 109_bi, 85_bi, 100_bi};
			case Species::Totodile: return {50_bi, 65_bi, 64_bi, 44_bi, 48_bi, 43_bi};
			case Species::Croconaw: return {65_bi, 80_bi, 80_bi, 59_bi, 63_bi, 58_bi};
			case Species::Feraligatr: return {85_bi, 105_bi, 100_bi, 79_bi, 83_bi, 78_bi};
			case Species::Sentret: return {35_bi, 46_bi, 34_bi, 35_bi, 45_bi, 20_bi};
			case Species::Furret: return {85_bi, 76_bi, 64_bi, 45_bi, 55_bi, 90_bi};
			case Species::Hoothoot: return {60_bi, 30_bi, 30_bi, 36_bi, 56_bi, 50_bi};
			case Species::Noctowl: return {
				100_bi,
				50_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 76_bi, 86_bi),
				96_bi,
				70_bi
			};
			case Species::Ledyba: return {40_bi, 20_bi, 30_bi, 40_bi, 80_bi, 55_bi};
			case Species::Ledian: return {55_bi, 35_bi, 50_bi, 55_bi, 110_bi, 85_bi};
			case Species::Spinarak: return {40_bi, 60_bi, 40_bi, 40_bi, 40_bi, 30_bi};
			case Species::Ariados: return {
				70_bi,
				90_bi,
				70_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 60_bi, 70_bi),
				40_bi
			};
			case Species::Crobat: return {85_bi, 90_bi, 80_bi, 70_bi, 80_bi, 130_bi};
			case Species::Chinchou: return {75_bi, 38_bi, 38_bi, 56_bi, 56_bi, 67_bi};
			case Species::Lanturn: return {125_bi, 58_bi, 58_bi, 76_bi, 76_bi, 67_bi};
			case Species::Pichu: return {20_bi, 40_bi, 15_bi, 35_bi, 35_bi, 60_bi};
			case Species::Cleffa: return {50_bi, 25_bi, 28_bi, 45_bi, 55_bi, 15_bi};
			case Species::Igglybuff: return {90_bi, 30_bi, 15_bi, 40_bi, 20_bi, 15_bi};
			case Species::Togepi: return {35_bi, 20_bi, 65_bi, 40_bi, 65_bi, 20_bi};
			case Species::Togetic: return {55_bi, 40_bi, 85_bi, 80_bi, 105_bi, 40_bi};
			case Species::Natu: return {40_bi, 50_bi, 45_bi, 70_bi, 45_bi, 70_bi};
			case Species::Xatu: return {65_bi, 75_bi, 70_bi, 95_bi, 70_bi, 95_bi};
			case Species::Mareep: return {55_bi, 40_bi, 40_bi, 65_bi, 45_bi, 35_bi};
			case Species::Flaaffy: return {70_bi, 55_bi, 55_bi, 80_bi, 60_bi, 45_bi};
			case Species::Ampharos: return {90_bi, 75_bi, 85_bi, 115_bi, 90_bi, 55_bi};
			// case Species::Ampharos_Mega: return {90_bi, 95_bi, 105_bi, 165_bi, 110_bi, 45_bi};
			case Species::Bellossom: return {
				75_bi,
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi),
				90_bi,
				100_bi,
				50_bi
			};
			case Species::Marill: return {70_bi, 20_bi, 50_bi, 20_bi, 50_bi, 40_bi};
			case Species::Azumarill: return {100_bi, 50_bi, 80_bi, 60_bi, 80_bi, 50_bi};
			case Species::Sudowoodo: return {70_bi, 100_bi, 115_bi, 30_bi, 65_bi, 30_bi};
			case Species::Politoed: return {90_bi, 75_bi, 75_bi, 90_bi, 100_bi, 70_bi};
			case Species::Hoppip: return {35_bi, 35_bi, 40_bi, 35_bi, 55_bi, 50_bi};
			case Species::Skiploom: return {55_bi, 45_bi, 50_bi, 45_bi, 65_bi, 80_bi};
			case Species::Jumpluff: return {
				75_bi,
				55_bi,
				70_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi),
				110_bi
			};
			case Species::Aipom: return {55_bi, 70_bi, 55_bi, 40_bi, 55_bi, 85_bi};
			case Species::Sunkern: return {30_bi, 30_bi, 30_bi, 30_bi, 30_bi, 30_bi};
			case Species::Sunflora: return {75_bi, 75_bi, 55_bi, 105_bi, 85_bi, 30_bi};
			case Species::Yanma: return {65_bi, 65_bi, 45_bi, 75_bi, 45_bi, 95_bi};
			case Species::Wooper: return {55_bi, 45_bi, 45_bi, 25_bi, 25_bi, 15_bi};
			case Species::Quagsire: return {95_bi, 85_bi, 85_bi, 65_bi, 65_bi, 35_bi};
			case Species::Espeon: return {65_bi, 65_bi, 60_bi, 130_bi, 95_bi, 110_bi};
			case Species::Umbreon: return {95_bi, 65_bi, 110_bi, 60_bi, 130_bi, 65_bi};
			case Species::Murkrow: return {60_bi, 85_bi, 42_bi, 85_bi, 42_bi, 91_bi};
			case Species::Slowking: return {95_bi, 75_bi, 80_bi, 100_bi, 110_bi, 30_bi};
			case Species::Misdreavus: return {60_bi, 60_bi, 60_bi, 85_bi, 85_bi, 85_bi};
			case Species::Unown: return {48_bi, 72_bi, 48_bi, 72_bi, 48_bi, 48_bi};
			case Species::Wobbuffet: return {190_bi, 33_bi, 58_bi, 33_bi, 58_bi, 33_bi};
			case Species::Girafarig: return {70_bi, 80_bi, 65_bi, 90_bi, 65_bi, 85_bi};
			case Species::Pineco: return {50_bi, 65_bi, 90_bi, 35_bi, 35_bi, 15_bi};
			case Species::Forretress: return {75_bi, 90_bi, 140_bi, 60_bi, 60_bi, 40_bi};
			case Species::Dunsparce: return {100_bi, 70_bi, 70_bi, 65_bi, 65_bi, 45_bi};
			case Species::Gligar: return {65_bi, 75_bi, 105_bi, 35_bi, 65_bi, 85_bi};
			case Species::Steelix: return {75_bi, 85_bi, 200_bi, 55_bi, 65_bi, 30_bi};
			// case Species::Steelix_Mega: return {75_bi, 125_bi, 230_bi, 55_bi, 95_bi, 30_bi};
			case Species::Snubbull: return {60_bi, 80_bi, 50_bi, 40_bi, 40_bi, 30_bi};
			case Species::Granbull: return {90_bi, 120_bi, 75_bi, 60_bi, 60_bi, 45_bi};
			case Species::Qwilfish: return {
				65_bi,
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 75_bi, 85_bi),
				55_bi,
				55_bi,
				85_bi
			};
			case Species::Scizor: return {70_bi, 130_bi, 100_bi, 55_bi, 80_bi, 65_bi};
			// case Species::Scizor_Mega: return {70_bi, 150_bi, 140_bi, 65_bi, 100_bi, 75_bi};
			case Species::Shuckle: return {20_bi, 10_bi, 230_bi, 10_bi, 230_bi, 5_bi};
			case Species::Heracross: return {80_bi, 125_bi, 75_bi, 40_bi, 95_bi, 85_bi};
			// case Species::Heracross_Mega: return {80_bi, 185_bi, 115_bi, 40_bi, 105_bi, 75_bi};
			case Species::Sneasel: return {55_bi, 95_bi, 55_bi, 35_bi, 75_bi, 115_bi};
			case Species::Teddiursa: return {60_bi, 80_bi, 50_bi, 50_bi, 50_bi, 40_bi};
			case Species::Ursaring: return {90_bi, 130_bi, 75_bi, 75_bi, 75_bi, 55_bi};
			case Species::Slugma: return {40_bi, 40_bi, 40_bi, 70_bi, 40_bi, 20_bi};
			case Species::Magcargo: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 50_bi, 60_bi),
				50_bi,
				120_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 80_bi, 90_bi),
				80_bi,
				30_bi
			};
			case Species::Swinub: return {50_bi, 50_bi, 40_bi, 30_bi, 30_bi, 50_bi};
			case Species::Piloswine: return {100_bi, 100_bi, 80_bi, 60_bi, 60_bi, 50_bi};
			case Species::Corsola: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 55_bi, 65_bi),
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 85_bi, 95_bi),
				65_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 85_bi, 95_bi),
				35_bi
			};
			case Species::Remoraid: return {35_bi, 65_bi, 35_bi, 65_bi, 35_bi, 65_bi};
			case Species::Octillery: return {75_bi, 105_bi, 75_bi, 105_bi, 75_bi, 45_bi};
			case Species::Delibird: return {45_bi, 55_bi, 45_bi, 65_bi, 45_bi, 75_bi};
			case Species::Mantine: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 65_bi, 85_bi),
				40_bi,
				70_bi,
				80_bi,
				140_bi,
				70_bi
			};
			case Species::Skarmory: return {65_bi, 80_bi, 140_bi, 40_bi, 70_bi, 70_bi};
			case Species::Houndour: return {45_bi, 60_bi, 30_bi, 80_bi, 50_bi, 65_bi};
			case Species::Houndoom: return {75_bi, 90_bi, 50_bi, 110_bi, 80_bi, 95_bi};
			// case Species::Houndoom_Mega: return {75_bi, 90_bi, 90_bi, 140_bi, 90_bi, 115_bi};
			case Species::Kingdra: return {75_bi, 95_bi, 95_bi, 95_bi, 95_bi, 85_bi};
			case Species::Phanpy: return {90_bi, 60_bi, 60_bi, 40_bi, 40_bi, 40_bi};
			case Species::Donphan: return {90_bi, 120_bi, 120_bi, 60_bi, 60_bi, 50_bi};
			case Species::Porygon2: return {85_bi, 80_bi, 90_bi, 105_bi, 95_bi, 60_bi};
			case Species::Stantler: return {73_bi, 95_bi, 62_bi, 85_bi, 65_bi, 85_bi};
			case Species::Smeargle: return {55_bi, 20_bi, 35_bi, 20_bi, 45_bi, 75_bi};
			case Species::Tyrogue: return {35_bi, 35_bi, 35_bi, 35_bi, 35_bi, 35_bi};
			case Species::Hitmontop: return {50_bi, 95_bi, 95_bi, 35_bi, 110_bi, 70_bi};
			case Species::Smoochum: return {45_bi, 30_bi, 15_bi, 85_bi, 65_bi, 65_bi};
			case Species::Elekid: return {45_bi, 63_bi, 37_bi, 65_bi, 55_bi, 95_bi};
			case Species::Magby: return {45_bi, 75_bi, 37_bi, 70_bi, 55_bi, 83_bi};
			case Species::Miltank: return {95_bi, 80_bi, 105_bi, 40_bi, 70_bi, 100_bi};
			case Species::Blissey: return {255_bi, 10_bi, 10_bi, 75_bi, 135_bi, 55_bi};
			case Species::Raikou: return {90_bi, 85_bi, 75_bi, 115_bi, 100_bi, 115_bi};
			case Species::Entei: return {115_bi, 115_bi, 85_bi, 90_bi, 75_bi, 100_bi};
			case Species::Suicune: return {100_bi, 75_bi, 115_bi, 90_bi, 115_bi, 85_bi};
			case Species::Larvitar: return {50_bi, 64_bi, 50_bi, 45_bi, 50_bi, 41_bi};
			case Species::Pupitar: return {70_bi, 84_bi, 70_bi, 65_bi, 70_bi, 51_bi};
			case Species::Tyranitar: return {100_bi, 134_bi, 110_bi, 95_bi, 100_bi, 61_bi};
			// case Species::Tyranitar_Mega: return {100_bi, 164_bi, 150_bi, 95_bi, 120_bi, 71_bi};
			case Species::Lugia: return {106_bi, 90_bi, 130_bi, 90_bi, 154_bi, 110_bi};
			case Species::Ho_Oh: return {106_bi, 130_bi, 90_bi, 110_bi, 154_bi, 90_bi};
			case Species::Celebi: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Treecko: return {40_bi, 45_bi, 35_bi, 65_bi, 55_bi, 70_bi};
			case Species::Grovyle: return {50_bi, 65_bi, 45_bi, 85_bi, 65_bi, 95_bi};
			case Species::Sceptile: return {70_bi, 85_bi, 65_bi, 105_bi, 85_bi, 120_bi};
			// case Species::Sceptile_Mega: return {70_bi, 110_bi, 75_bi, 145_bi, 85_bi, 145_bi};
			case Species::Torchic: return {45_bi, 60_bi, 40_bi, 70_bi, 50_bi, 45_bi};
			case Species::Combusken: return {60_bi, 85_bi, 60_bi, 85_bi, 60_bi, 55_bi};
			case Species::Blaziken: return {80_bi, 120_bi, 70_bi, 110_bi, 70_bi, 80_bi};
			// case Species::Blaziken_Mega: return {80_bi, 160_bi, 80_bi, 130_bi, 80_bi, 100_bi};
			case Species::Mudkip: return {50_bi, 70_bi, 50_bi, 50_bi, 50_bi, 40_bi};
			case Species::Marshtomp: return {70_bi, 85_bi, 70_bi, 60_bi, 70_bi, 50_bi};
			case Species::Swampert: return {100_bi, 110_bi, 90_bi, 85_bi, 90_bi, 60_bi};
			// case Species::Swampert_Mega: return {100_bi, 150_bi, 110_bi, 95_bi, 110_bi, 70_bi};
			case Species::Poochyena: return {35_bi, 55_bi, 35_bi, 30_bi, 30_bi, 35_bi};
			case Species::Mightyena: return {70_bi, 90_bi, 70_bi, 60_bi, 60_bi, 70_bi};
			case Species::Zigzagoon: return {38_bi, 30_bi, 41_bi, 30_bi, 41_bi, 60_bi};
			case Species::Linoone: return {78_bi, 70_bi, 61_bi, 50_bi, 61_bi, 100_bi};
			case Species::Wurmple: return {45_bi, 45_bi, 35_bi, 20_bi, 30_bi, 20_bi};
			case Species::Silcoon: return {50_bi, 35_bi, 55_bi, 25_bi, 25_bi, 15_bi};
			case Species::Beautifly: return {
				60_bi,
				70_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 90_bi, 100_bi),
				50_bi,
				65_bi
			};
			case Species::Cascoon: return {50_bi, 35_bi, 55_bi, 25_bi, 25_bi, 15_bi};
			case Species::Dustox: return {60_bi, 50_bi, 70_bi, 50_bi, 90_bi, 65_bi};
			case Species::Lotad: return {40_bi, 30_bi, 30_bi, 40_bi, 50_bi, 30_bi};
			case Species::Lombre: return {60_bi, 50_bi, 50_bi, 60_bi, 70_bi, 50_bi};
			case Species::Ludicolo: return {80_bi, 70_bi, 70_bi, 90_bi, 100_bi, 70_bi};
			case Species::Seedot: return {40_bi, 40_bi, 50_bi, 30_bi, 30_bi, 30_bi};
			case Species::Nuzleaf: return {70_bi, 70_bi, 40_bi, 60_bi, 40_bi, 60_bi};
			case Species::Shiftry: return {90_bi, 100_bi, 60_bi, 90_bi, 60_bi, 80_bi};
			case Species::Taillow: return {40_bi, 55_bi, 30_bi, 30_bi, 30_bi, 85_bi};
			case Species::Swellow: return {
				60_bi,
				85_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 50_bi, 75_bi),
				50_bi,
				125_bi
			};
			case Species::Wingull: return {40_bi, 30_bi, 30_bi, 55_bi, 30_bi, 85_bi};
			case Species::Pelipper: return {
				60_bi,
				50_bi,
				100_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 85_bi, 95_bi),
				70_bi,
				65_bi
			};
			case Species::Ralts: return {28_bi, 25_bi, 25_bi, 45_bi, 35_bi, 40_bi};
			case Species::Kirlia: return {38_bi, 35_bi, 35_bi, 65_bi, 55_bi, 50_bi};
			case Species::Gardevoir: return {68_bi, 65_bi, 65_bi, 125_bi, 115_bi, 80_bi};
			// case Species::Gardevoir_Mega: return {68_bi, 85_bi, 65_bi, 165_bi, 135_bi, 100_bi};
			case Species::Surskit: return {40_bi, 30_bi, 32_bi, 50_bi, 52_bi, 65_bi};
			case Species::Masquerain: return {
				70_bi,
				60_bi,
				62_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 80_bi, 100_bi),
				82_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 60_bi, 80_bi)
			};
			case Species::Shroomish: return {60_bi, 40_bi, 60_bi, 40_bi, 60_bi, 35_bi};
			case Species::Breloom: return {60_bi, 130_bi, 80_bi, 60_bi, 60_bi, 70_bi};
			case Species::Slakoth: return {60_bi, 60_bi, 60_bi, 35_bi, 35_bi, 30_bi};
			case Species::Vigoroth: return {80_bi, 80_bi, 80_bi, 55_bi, 55_bi, 90_bi};
			case Species::Slaking: return {150_bi, 160_bi, 100_bi, 95_bi, 65_bi, 100_bi};
			case Species::Nincada: return {31_bi, 45_bi, 90_bi, 30_bi, 30_bi, 40_bi};
			case Species::Ninjask: return {61_bi, 90_bi, 45_bi, 50_bi, 50_bi, 160_bi};
			case Species::Shedinja: return {1_bi, 90_bi, 45_bi, 30_bi, 30_bi, 40_bi};
			case Species::Whismur: return {64_bi, 51_bi, 23_bi, 51_bi, 23_bi, 28_bi};
			case Species::Loudred: return {84_bi, 71_bi, 43_bi, 71_bi, 43_bi, 48_bi};
			case Species::Exploud: return {
				104_bi,
				91_bi,
				63_bi,
				91_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 63_bi, 73_bi),
				68_bi
			};
			case Species::Makuhita: return {72_bi, 60_bi, 30_bi, 20_bi, 30_bi, 25_bi};
			case Species::Hariyama: return {144_bi, 120_bi, 60_bi, 40_bi, 60_bi, 50_bi};
			case Species::Azurill: return {50_bi, 20_bi, 40_bi, 20_bi, 40_bi, 20_bi};
			case Species::Nosepass: return {30_bi, 45_bi, 135_bi, 45_bi, 90_bi, 30_bi};
			case Species::Skitty: return {50_bi, 45_bi, 45_bi, 35_bi, 35_bi, 50_bi};
			case Species::Delcatty: return {
				70_bi,
				65_bi,
				65_bi,
				55_bi,
				55_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 70_bi, 90_bi),
			};
			case Species::Sableye: return {50_bi, 75_bi, 75_bi, 65_bi, 65_bi, 50_bi};
			// case Species::Sableye_Mega: return {50_bi, 85_bi, 125_bi, 85_bi, 115_bi, 20_bi};
			case Species::Mawile: return {50_bi, 85_bi, 85_bi, 55_bi, 55_bi, 50_bi};
			// case Species::Mawile_Mega: return {50_bi, 105_bi, 125_bi, 55_bi, 95_bi, 50_bi};
			case Species::Aron: return {50_bi, 70_bi, 100_bi, 40_bi, 40_bi, 30_bi};
			case Species::Lairon: return {60_bi, 90_bi, 140_bi, 50_bi, 50_bi, 40_bi};
			case Species::Aggron: return {70_bi, 110_bi, 180_bi, 60_bi, 60_bi, 50_bi};
			// case Species::Aggron_Mega: return {70_bi, 140_bi, 230_bi, 60_bi, 80_bi, 50_bi};
			case Species::Meditite: return {30_bi, 40_bi, 55_bi, 40_bi, 55_bi, 60_bi};
			case Species::Medicham: return {60_bi, 60_bi, 75_bi, 60_bi, 75_bi, 80_bi};
			// case Species::Medicham_Mega: return {60_bi, 100_bi, 85_bi, 80_bi, 85_bi, 100_bi};
			case Species::Electrike: return {40_bi, 45_bi, 40_bi, 65_bi, 40_bi, 65_bi};
			case Species::Manectric: return {70_bi, 75_bi, 60_bi, 105_bi, 60_bi, 105_bi};
			// case Species::Manectric_Mega: return {70_bi, 75_bi, 80_bi, 135_bi, 80_bi, 135_bi};
			case Species::Plusle: return {60_bi, 50_bi, 40_bi, 85_bi, 75_bi, 95_bi};
			case Species::Minun: return {60_bi, 40_bi, 50_bi, 75_bi, 85_bi, 95_bi};
			case Species::Volbeat: return {
				65_bi,
				73_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 55_bi, 75_bi),
				47_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 75_bi, 85_bi),
				85_bi
			};
			case Species::Illumise: return {
				65_bi,
				47_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 55_bi, 75_bi),
				73_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 75_bi, 85_bi),
				85_bi
			};
			case Species::Roselia: return {50_bi, 60_bi, 45_bi, 100_bi, 80_bi, 65_bi};
			case Species::Gulpin: return {70_bi, 43_bi, 53_bi, 43_bi, 53_bi, 40_bi};
			case Species::Swalot: return {100_bi, 73_bi, 83_bi, 73_bi, 83_bi, 55_bi};
			case Species::Carvanha: return {45_bi, 90_bi, 20_bi, 65_bi, 20_bi, 65_bi};
			case Species::Sharpedo: return {70_bi, 120_bi, 40_bi, 95_bi, 40_bi, 95_bi};
			// case Species::Sharpedo_Mega: return {70_bi, 140_bi, 70_bi, 110_bi, 65_bi, 105_bi};
			case Species::Wailmer: return {130_bi, 70_bi, 35_bi, 70_bi, 35_bi, 60_bi};
			case Species::Wailord: return {170_bi, 90_bi, 45_bi, 90_bi, 45_bi, 60_bi};
			case Species::Numel: return {60_bi, 60_bi, 40_bi, 65_bi, 45_bi, 35_bi};
			case Species::Camerupt: return {70_bi, 100_bi, 70_bi, 105_bi, 75_bi, 40_bi};
			// case Species::Camerupt_Mega: return {70_bi, 120_bi, 100_bi, 145_bi, 105_bi, 20_bi};
			case Species::Torkoal: return {70_bi, 85_bi, 140_bi, 85_bi, 70_bi, 20_bi};
			case Species::Spoink: return {60_bi, 25_bi, 35_bi, 70_bi, 80_bi, 60_bi};
			case Species::Grumpig: return {80_bi, 45_bi, 65_bi, 90_bi, 110_bi, 80_bi};
			case Species::Spinda: return {60_bi, 60_bi, 60_bi, 60_bi, 60_bi, 60_bi};
			case Species::Trapinch: return {45_bi, 100_bi, 45_bi, 45_bi, 45_bi, 10_bi};
			case Species::Vibrava: return {50_bi, 70_bi, 50_bi, 50_bi, 50_bi, 70_bi};
			case Species::Flygon: return {80_bi, 100_bi, 80_bi, 80_bi, 80_bi, 100_bi};
			case Species::Cacnea: return {50_bi, 85_bi, 40_bi, 85_bi, 40_bi, 35_bi};
			case Species::Cacturne: return {70_bi, 115_bi, 60_bi, 115_bi, 60_bi, 55_bi};
			case Species::Swablu: return {45_bi, 40_bi, 60_bi, 40_bi, 75_bi, 50_bi};
			case Species::Altaria: return {75_bi, 70_bi, 90_bi, 70_bi, 105_bi, 80_bi};
			// case Species::Altaria_Mega: return {75_bi, 110_bi, 110_bi, 110_bi, 105_bi, 80_bi};
			case Species::Zangoose: return {73_bi, 115_bi, 60_bi, 60_bi, 60_bi, 90_bi};
			case Species::Seviper: return {73_bi, 100_bi, 60_bi, 100_bi, 60_bi, 65_bi};
			case Species::Lunatone: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 70_bi, 90_bi),
				55_bi,
				65_bi,
				95_bi,
				85_bi,
				70_bi
			};
			case Species::Solrock: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 70_bi, 90_bi),
				95_bi,
				85_bi,
				55_bi,
				65_bi,
				70_bi
			};
			case Species::Barboach: return {50_bi, 48_bi, 43_bi, 46_bi, 41_bi, 60_bi};
			case Species::Whiscash: return {110_bi, 78_bi, 73_bi, 76_bi, 71_bi, 60_bi};
			case Species::Corphish: return {43_bi, 80_bi, 65_bi, 50_bi, 35_bi, 35_bi};
			case Species::Crawdaunt: return {63_bi, 120_bi, 85_bi, 90_bi, 55_bi, 55_bi};
			case Species::Baltoy: return {40_bi, 40_bi, 55_bi, 40_bi, 70_bi, 55_bi};
			case Species::Claydol: return {60_bi, 70_bi, 105_bi, 70_bi, 120_bi, 75_bi};
			case Species::Lileep: return {66_bi, 41_bi, 77_bi, 61_bi, 87_bi, 23_bi};
			case Species::Cradily: return {86_bi, 81_bi, 97_bi, 81_bi, 107_bi, 43_bi};
			case Species::Anorith: return {45_bi, 95_bi, 50_bi, 40_bi, 50_bi, 75_bi};
			case Species::Armaldo: return {75_bi, 125_bi, 100_bi, 70_bi, 80_bi, 45_bi};
			case Species::Feebas: return {20_bi, 15_bi, 20_bi, 10_bi, 55_bi, 80_bi};
			case Species::Milotic: return {95_bi, 60_bi, 79_bi, 100_bi, 125_bi, 81_bi};
			case Species::Castform: return {70_bi, 70_bi, 70_bi, 70_bi, 70_bi, 70_bi};
			case Species::Kecleon: return {60_bi, 90_bi, 70_bi, 60_bi, 120_bi, 40_bi};
			case Species::Shuppet: return {44_bi, 75_bi, 35_bi, 63_bi, 33_bi, 45_bi};
			case Species::Banette: return {64_bi, 115_bi, 65_bi, 83_bi, 63_bi, 65_bi};
			// case Species::Banette_Mega: return {64_bi, 165_bi, 75_bi, 93_bi, 83_bi, 75_bi};
			case Species::Duskull: return {20_bi, 40_bi, 90_bi, 30_bi, 90_bi, 25_bi};
			case Species::Dusclops: return {40_bi, 70_bi, 130_bi, 60_bi, 130_bi, 25_bi};
			case Species::Tropius: return {99_bi, 68_bi, 83_bi, 72_bi, 87_bi, 51_bi};
			case Species::Chimecho: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 65_bi, 75_bi),
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 70_bi, 80_bi),
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 80_bi, 90_bi),
				65_bi
			};
			case Species::Absol: return {65_bi, 130_bi, 60_bi, 75_bi, 60_bi, 75_bi};
			// case Species::Absol_Mega: return {65_bi, 150_bi, 60_bi, 115_bi, 60_bi, 115_bi};
			case Species::Wynaut: return {95_bi, 23_bi, 48_bi, 23_bi, 48_bi, 23_bi};
			case Species::Snorunt: return {50_bi, 50_bi, 50_bi, 50_bi, 50_bi, 50_bi};
			case Species::Glalie: return {80_bi, 80_bi, 80_bi, 80_bi, 80_bi, 80_bi};
			// case Species::Glalie_Mega: return {80_bi, 120_bi, 80_bi, 120_bi, 80_bi, 100_bi};
			case Species::Spheal: return {70_bi, 40_bi, 50_bi, 55_bi, 50_bi, 25_bi};
			case Species::Sealeo: return {90_bi, 60_bi, 70_bi, 75_bi, 70_bi, 45_bi};
			case Species::Walrein: return {110_bi, 80_bi, 90_bi, 95_bi, 90_bi, 65_bi};
			case Species::Clamperl: return {35_bi, 64_bi, 85_bi, 74_bi, 55_bi, 32_bi};
			case Species::Huntail: return {55_bi, 104_bi, 105_bi, 94_bi, 75_bi, 52_bi};
			case Species::Gorebyss: return {55_bi, 84_bi, 105_bi, 114_bi, 75_bi, 52_bi};
			case Species::Relicanth: return {100_bi, 90_bi, 130_bi, 45_bi, 65_bi, 55_bi};
			case Species::Luvdisc: return {43_bi, 30_bi, 55_bi, 40_bi, 65_bi, 97_bi};
			case Species::Bagon: return {45_bi, 75_bi, 60_bi, 40_bi, 30_bi, 50_bi};
			case Species::Shelgon: return {65_bi, 95_bi, 100_bi, 60_bi, 50_bi, 50_bi};
			case Species::Salamence: return {95_bi, 135_bi, 80_bi, 110_bi, 80_bi, 100_bi};
			// case Species::Salamence_Mega: return {95_bi, 145_bi, 130_bi, 120_bi, 90_bi, 120_bi};
			case Species::Beldum: return {40_bi, 55_bi, 80_bi, 35_bi, 60_bi, 30_bi};
			case Species::Metang: return {60_bi, 75_bi, 100_bi, 55_bi, 80_bi, 50_bi};
			case Species::Metagross: return {80_bi, 135_bi, 130_bi, 95_bi, 90_bi, 70_bi};
			// case Species::Metagross_Mega: return {80_bi, 145_bi, 150_bi, 105_bi, 110_bi, 110_bi};
			case Species::Regirock: return {80_bi, 100_bi, 200_bi, 50_bi, 100_bi, 50_bi};
			case Species::Regice: return {80_bi, 50_bi, 100_bi, 100_bi, 200_bi, 50_bi};
			case Species::Registeel: return {80_bi, 75_bi, 150_bi, 75_bi, 150_bi, 50_bi};
			case Species::Latias: return {80_bi, 80_bi, 90_bi, 110_bi, 130_bi, 110_bi};
			// case Species::Latias_Mega: return {80_bi, 100_bi, 120_bi, 140_bi, 150_bi, 110_bi};
			case Species::Latios: return {80_bi, 90_bi, 80_bi, 130_bi, 110_bi, 110_bi};
			// case Species::Latios_Mega: return {80_bi, 130_bi, 100_bi, 160_bi, 120_bi, 110_bi};
			case Species::Kyogre: return {100_bi, 100_bi, 90_bi, 150_bi, 140_bi, 90_bi};
			// case Species::Kyogre_Primal: return {100_bi, 150_bi, 90_bi, 180_bi, 160_bi, 90_bi};
			case Species::Groudon: return {100_bi, 150_bi, 140_bi, 100_bi, 90_bi, 90_bi};
			// case Species::Groudon_Primal: return {100_bi, 180_bi, 160_bi, 150_bi, 90_bi, 90_bi};
			case Species::Rayquaza: return {105_bi, 150_bi, 90_bi, 150_bi, 90_bi, 95_bi};
			// case Species::Rayquaza_Mega: return {105_bi, 180_bi, 100_bi, 180_bi, 100_bi, 115_bi};
			case Species::Jirachi: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Deoxys_Normal: return {50_bi, 150_bi, 50_bi, 150_bi, 50_bi, 150_bi};
			case Species::Deoxys_Attack: return {50_bi, 180_bi, 20_bi, 180_bi, 20_bi, 150_bi};
			case Species::Deoxys_Defense: return {50_bi, 70_bi, 160_bi, 70_bi, 160_bi, 90_bi};
			case Species::Deoxys_Speed: return {50_bi, 95_bi, 90_bi, 95_bi, 90_bi, 180_bi};
			case Species::Turtwig: return {55_bi, 68_bi, 64_bi, 45_bi, 55_bi, 31_bi};
			case Species::Grotle: return {75_bi, 89_bi, 85_bi, 55_bi, 65_bi, 36_bi};
			case Species::Torterra: return {95_bi, 109_bi, 105_bi, 75_bi, 85_bi, 56_bi};
			case Species::Chimchar: return {44_bi, 58_bi, 44_bi, 58_bi, 44_bi, 61_bi};
			case Species::Monferno: return {64_bi, 78_bi, 52_bi, 78_bi, 52_bi, 81_bi};
			case Species::Infernape: return {76_bi, 104_bi, 71_bi, 104_bi, 71_bi, 108_bi};
			case Species::Piplup: return {53_bi, 51_bi, 53_bi, 61_bi, 56_bi, 40_bi};
			case Species::Prinplup: return {64_bi, 66_bi, 68_bi, 81_bi, 76_bi, 50_bi};
			case Species::Empoleon: return {84_bi, 86_bi, 88_bi, 111_bi, 101_bi, 60_bi};
			case Species::Starly: return {40_bi, 55_bi, 30_bi, 30_bi, 30_bi, 60_bi};
			case Species::Staravia: return {55_bi, 75_bi, 50_bi, 40_bi, 40_bi, 80_bi};
			case Species::Staraptor: return {
				85_bi,
				120_bi,
				70_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 50_bi, 60_bi),
				100_bi
			};
			case Species::Bidoof: return {59_bi, 45_bi, 40_bi, 35_bi, 40_bi, 31_bi};
			case Species::Bibarel: return {79_bi, 85_bi, 60_bi, 55_bi, 60_bi, 71_bi};
			case Species::Kricketot: return {37_bi, 25_bi, 41_bi, 25_bi, 41_bi, 25_bi};
			case Species::Kricketune: return {77_bi, 85_bi, 51_bi, 55_bi, 51_bi, 65_bi};
			case Species::Shinx: return {45_bi, 65_bi, 34_bi, 40_bi, 34_bi, 45_bi};
			case Species::Luxio: return {60_bi, 85_bi, 49_bi, 60_bi, 49_bi, 60_bi};
			case Species::Luxray: return {80_bi, 120_bi, 79_bi, 95_bi, 79_bi, 70_bi};
			case Species::Budew: return {40_bi, 30_bi, 35_bi, 50_bi, 70_bi, 55_bi};
			case Species::Roserade: return {
				60_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 55_bi, 65_bi),
				125_bi,
				105_bi,
				90_bi
			};
			case Species::Cranidos: return {67_bi, 125_bi, 40_bi, 30_bi, 30_bi, 58_bi};
			case Species::Rampardos: return {97_bi, 165_bi, 60_bi, 65_bi, 50_bi, 58_bi};
			case Species::Shieldon: return {30_bi, 42_bi, 118_bi, 42_bi, 88_bi, 30_bi};
			case Species::Bastiodon: return {60_bi, 52_bi, 168_bi, 47_bi, 138_bi, 30_bi};
			case Species::Burmy: return {40_bi, 29_bi, 45_bi, 29_bi, 45_bi, 36_bi};
			case Species::Wormadam_Plant: return {60_bi, 59_bi, 85_bi, 79_bi, 105_bi, 36_bi};
			case Species::Wormadam_Sandy: return {60_bi, 79_bi, 105_bi, 59_bi, 85_bi, 36_bi};
			case Species::Wormadam_Trash: return {60_bi, 69_bi, 95_bi, 69_bi, 95_bi, 36_bi};
			case Species::Mothim: return {70_bi, 94_bi, 50_bi, 94_bi, 50_bi, 66_bi};
			case Species::Combee: return {30_bi, 30_bi, 42_bi, 30_bi, 42_bi, 70_bi};
			case Species::Vespiquen: return {70_bi, 80_bi, 102_bi, 80_bi, 102_bi, 40_bi};
			case Species::Pachirisu: return {60_bi, 45_bi, 70_bi, 45_bi, 90_bi, 95_bi};
			case Species::Buizel: return {55_bi, 65_bi, 35_bi, 60_bi, 30_bi, 85_bi};
			case Species::Floatzel: return {85_bi, 105_bi, 55_bi, 85_bi, 50_bi, 115_bi};
			case Species::Cherubi: return {45_bi, 35_bi, 45_bi, 62_bi, 53_bi, 35_bi};
			case Species::Cherrim: return {70_bi, 60_bi, 70_bi, 87_bi, 78_bi, 85_bi};
			case Species::Shellos: return {76_bi, 48_bi, 48_bi, 57_bi, 62_bi, 34_bi};
			case Species::Gastrodon: return {111_bi, 83_bi, 68_bi, 92_bi, 82_bi, 39_bi};
			case Species::Ambipom: return {75_bi, 100_bi, 66_bi, 60_bi, 66_bi, 115_bi};
			case Species::Drifloon: return {90_bi, 50_bi, 34_bi, 60_bi, 44_bi, 70_bi};
			case Species::Drifblim: return {150_bi, 80_bi, 44_bi, 90_bi, 54_bi, 80_bi};
			case Species::Buneary: return {55_bi, 66_bi, 44_bi, 44_bi, 56_bi, 85_bi};
			case Species::Lopunny: return {65_bi, 76_bi, 84_bi, 54_bi, 96_bi, 105_bi};
			// case Species::Lopunny_Mega: return {65_bi, 136_bi, 94_bi, 54_bi, 96_bi, 135_bi};
			case Species::Mismagius: return {60_bi, 60_bi, 60_bi, 105_bi, 105_bi, 105_bi};
			case Species::Honchkrow: return {100_bi, 125_bi, 52_bi, 105_bi, 52_bi, 71_bi};
			case Species::Glameow: return {49_bi, 55_bi, 42_bi, 42_bi, 37_bi, 85_bi};
			case Species::Purugly: return {71_bi, 82_bi, 64_bi, 64_bi, 59_bi, 112_bi};
			case Species::Chingling: return {45_bi, 30_bi, 50_bi, 65_bi, 50_bi, 45_bi};
			case Species::Stunky: return {63_bi, 63_bi, 47_bi, 41_bi, 41_bi, 74_bi};
			case Species::Skuntank: return {103_bi, 93_bi, 67_bi, 71_bi, 61_bi, 84_bi};
			case Species::Bronzor: return {57_bi, 24_bi, 86_bi, 24_bi, 86_bi, 23_bi};
			case Species::Bronzong: return {67_bi, 89_bi, 116_bi, 79_bi, 116_bi, 33_bi};
			case Species::Bonsly: return {50_bi, 80_bi, 95_bi, 10_bi, 45_bi, 10_bi};
			case Species::Mime_Jr: return {20_bi, 25_bi, 45_bi, 70_bi, 90_bi, 60_bi};
			case Species::Happiny: return {100_bi, 5_bi, 5_bi, 15_bi, 65_bi, 30_bi};
			case Species::Chatot: return {76_bi, 65_bi, 45_bi, 92_bi, 42_bi, 91_bi};
			case Species::Spiritomb: return {50_bi, 92_bi, 108_bi, 92_bi, 108_bi, 35_bi};
			case Species::Gible: return {58_bi, 70_bi, 45_bi, 40_bi, 45_bi, 42_bi};
			case Species::Gabite: return {68_bi, 90_bi, 65_bi, 50_bi, 55_bi, 82_bi};
			case Species::Garchomp: return {108_bi, 130_bi, 95_bi, 80_bi, 85_bi, 102_bi};
			// case Species::Garchomp_Mega: return {108_bi, 170_bi, 115_bi, 120_bi, 95_bi, 92_bi};
			case Species::Munchlax: return {135_bi, 85_bi, 40_bi, 40_bi, 85_bi, 5_bi};
			case Species::Riolu: return {40_bi, 70_bi, 40_bi, 35_bi, 40_bi, 60_bi};
			case Species::Lucario: return {70_bi, 110_bi, 70_bi, 115_bi, 70_bi, 90_bi};
			// case Species::Lucario_Mega: return {70_bi, 145_bi, 88_bi, 140_bi, 70_bi, 112_bi};
			case Species::Hippopotas: return {68_bi, 72_bi, 78_bi, 38_bi, 42_bi, 32_bi};
			case Species::Hippowdon: return {108_bi, 112_bi, 118_bi, 68_bi, 72_bi, 47_bi};
			case Species::Skorupi: return {40_bi, 50_bi, 90_bi, 30_bi, 55_bi, 65_bi};
			case Species::Drapion: return {70_bi, 90_bi, 110_bi, 60_bi, 75_bi, 95_bi};
			case Species::Croagunk: return {48_bi, 61_bi, 40_bi, 61_bi, 40_bi, 50_bi};
			case Species::Toxicroak: return {83_bi, 106_bi, 65_bi, 86_bi, 65_bi, 85_bi};
			case Species::Carnivine: return {74_bi, 100_bi, 72_bi, 90_bi, 72_bi, 46_bi};
			case Species::Finneon: return {49_bi, 49_bi, 56_bi, 49_bi, 61_bi, 66_bi};
			case Species::Lumineon: return {69_bi, 69_bi, 76_bi, 69_bi, 86_bi, 91_bi};
			case Species::Mantyke: return {45_bi, 20_bi, 50_bi, 60_bi, 120_bi, 50_bi};
			case Species::Snover: return {60_bi, 62_bi, 50_bi, 62_bi, 60_bi, 40_bi};
			case Species::Abomasnow: return {90_bi, 92_bi, 75_bi, 92_bi, 85_bi, 60_bi};
			// case Species::Abomasnow_Mega: return {90_bi, 132_bi, 105_bi, 132_bi, 105_bi, 30_bi};
			case Species::Weavile: return {70_bi, 120_bi, 65_bi, 45_bi, 85_bi, 125_bi};
			case Species::Magnezone: return {70_bi, 70_bi, 115_bi, 130_bi, 90_bi, 60_bi};
			case Species::Lickilicky: return {110_bi, 85_bi, 95_bi, 80_bi, 95_bi, 50_bi};
			case Species::Rhyperior: return {115_bi, 140_bi, 130_bi, 55_bi, 55_bi, 40_bi};
			case Species::Tangrowth: return {100_bi, 100_bi, 125_bi, 110_bi, 50_bi, 50_bi};
			case Species::Electivire: return {75_bi, 123_bi, 67_bi, 95_bi, 85_bi, 95_bi};
			case Species::Magmortar: return {75_bi, 95_bi, 67_bi, 125_bi, 95_bi, 83_bi};
			case Species::Togekiss: return {85_bi, 50_bi, 95_bi, 120_bi, 115_bi, 80_bi};
			case Species::Yanmega: return {86_bi, 76_bi, 86_bi, 116_bi, 56_bi, 95_bi};
			case Species::Leafeon: return {65_bi, 110_bi, 130_bi, 60_bi, 65_bi, 95_bi};
			case Species::Glaceon: return {65_bi, 60_bi, 110_bi, 130_bi, 95_bi, 65_bi};
			case Species::Gliscor: return {75_bi, 95_bi, 125_bi, 45_bi, 75_bi, 95_bi};
			case Species::Mamoswine: return {110_bi, 130_bi, 80_bi, 70_bi, 60_bi, 80_bi};
			case Species::Porygon_Z: return {85_bi, 80_bi, 70_bi, 135_bi, 75_bi, 90_bi};
			case Species::Gallade: return {68_bi, 125_bi, 65_bi, 65_bi, 115_bi, 80_bi};
			// case Species::Gallade_Mega: return {68_bi, 165_bi, 95_bi, 65_bi, 115_bi, 110_bi};
			case Species::Probopass: return {60_bi, 55_bi, 145_bi, 75_bi, 150_bi, 40_bi};
			case Species::Dusknoir: return {45_bi, 100_bi, 135_bi, 65_bi, 135_bi, 45_bi};
			case Species::Froslass: return {70_bi, 80_bi, 70_bi, 80_bi, 70_bi, 110_bi};
			case Species::Rotom: return {50_bi, 50_bi, 77_bi, 95_bi, 77_bi, 91_bi};
			case Species::Rotom_Heat: return {50_bi, 65_bi, 107_bi, 105_bi, 107_bi, 86_bi};
			case Species::Rotom_Wash: return {50_bi, 65_bi, 107_bi, 105_bi, 107_bi, 86_bi};
			case Species::Rotom_Frost: return {50_bi, 65_bi, 107_bi, 105_bi, 107_bi, 86_bi};
			case Species::Rotom_Fan: return {50_bi, 65_bi, 107_bi, 105_bi, 107_bi, 86_bi};
			case Species::Rotom_Mow: return {50_bi, 65_bi, 107_bi, 105_bi, 107_bi, 86_bi};
			case Species::Uxie: return {75_bi, 75_bi, 130_bi, 75_bi, 130_bi, 95_bi};
			case Species::Mesprit: return {80_bi, 105_bi, 105_bi, 105_bi, 105_bi, 80_bi};
			case Species::Azelf: return {75_bi, 125_bi, 70_bi, 125_bi, 70_bi, 115_bi};
			case Species::Dialga: return {100_bi, 120_bi, 120_bi, 150_bi, 100_bi, 90_bi};
			case Species::Palkia: return {90_bi, 120_bi, 100_bi, 150_bi, 120_bi, 100_bi};
			case Species::Heatran: return {91_bi, 90_bi, 106_bi, 130_bi, 106_bi, 77_bi};
			case Species::Regigigas: return {110_bi, 160_bi, 110_bi, 80_bi, 110_bi, 100_bi};
			case Species::Giratina_Altered: return {150_bi, 100_bi, 120_bi, 100_bi, 120_bi, 90_bi};
			case Species::Giratina_Origin: return {150_bi, 120_bi, 100_bi, 120_bi, 100_bi, 90_bi};
			case Species::Cresselia: return {120_bi, 70_bi, 120_bi, 75_bi, 130_bi, 85_bi};
			case Species::Phione: return {80_bi, 80_bi, 80_bi, 80_bi, 80_bi, 80_bi};
			case Species::Manaphy: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Darkrai: return {70_bi, 90_bi, 90_bi, 135_bi, 90_bi, 125_bi};
			case Species::Shaymin_Land: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Shaymin_Sky: return {100_bi, 103_bi, 75_bi, 120_bi, 75_bi, 127_bi};
			case Species::Arceus: return {120_bi, 120_bi, 120_bi, 120_bi, 120_bi, 120_bi};
			case Species::Victini: return {100_bi, 100_bi, 100_bi, 100_bi, 100_bi, 100_bi};
			case Species::Snivy: return {45_bi, 45_bi, 55_bi, 45_bi, 55_bi, 63_bi};
			case Species::Servine: return {60_bi, 60_bi, 75_bi, 60_bi, 75_bi, 83_bi};
			case Species::Serperior: return {75_bi, 75_bi, 95_bi, 75_bi, 95_bi, 113_bi};
			case Species::Tepig: return {65_bi, 63_bi, 45_bi, 45_bi, 45_bi, 45_bi};
			case Species::Pignite: return {90_bi, 93_bi, 55_bi, 70_bi, 55_bi, 55_bi};
			case Species::Emboar: return {110_bi, 123_bi, 65_bi, 100_bi, 65_bi, 65_bi};
			case Species::Oshawott: return {55_bi, 55_bi, 45_bi, 63_bi, 45_bi, 45_bi};
			case Species::Dewott: return {75_bi, 75_bi, 60_bi, 83_bi, 60_bi, 60_bi};
			case Species::Samurott: return {95_bi, 100_bi, 85_bi, 108_bi, 70_bi, 70_bi};
			case Species::Patrat: return {45_bi, 55_bi, 39_bi, 35_bi, 39_bi, 42_bi};
			case Species::Watchog: return {60_bi, 85_bi, 69_bi, 60_bi, 69_bi, 77_bi};
			case Species::Lillipup: return {45_bi, 60_bi, 45_bi, 25_bi, 45_bi, 55_bi};
			case Species::Herdier: return {65_bi, 80_bi, 65_bi, 35_bi, 65_bi, 60_bi};
			case Species::Stoutland: return {
				85_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 100_bi, 110_bi),
				90_bi,
				45_bi,
				90_bi,
				80_bi
			};
			case Species::Purrloin: return {41_bi, 50_bi, 37_bi, 50_bi, 37_bi, 66_bi};
			case Species::Liepard: return {64_bi, 88_bi, 50_bi, 88_bi, 50_bi, 106_bi};
			case Species::Pansage: return {50_bi, 53_bi, 48_bi, 53_bi, 48_bi, 64_bi};
			case Species::Simisage: return {75_bi, 98_bi, 63_bi, 98_bi, 63_bi, 101_bi};
			case Species::Pansear: return {50_bi, 53_bi, 48_bi, 53_bi, 48_bi, 64_bi};
			case Species::Simisear: return {75_bi, 98_bi, 63_bi, 98_bi, 63_bi, 101_bi};
			case Species::Panpour: return {50_bi, 53_bi, 48_bi, 53_bi, 48_bi, 64_bi};
			case Species::Simipour: return {75_bi, 98_bi, 63_bi, 98_bi, 63_bi, 101_bi};
			case Species::Munna: return {76_bi, 25_bi, 45_bi, 67_bi, 55_bi, 24_bi};
			case Species::Musharna: return {116_bi, 55_bi, 85_bi, 107_bi, 95_bi, 29_bi};
			case Species::Pidove: return {50_bi, 55_bi, 50_bi, 36_bi, 30_bi, 43_bi};
			case Species::Tranquill: return {62_bi, 77_bi, 62_bi, 50_bi, 42_bi, 65_bi};
			case Species::Unfezant: return {
				80_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 105_bi, 115_bi),
				80_bi,
				65_bi,
				55_bi,
				93_bi
			};
			case Species::Blitzle: return {45_bi, 60_bi, 32_bi, 50_bi, 32_bi, 76_bi};
			case Species::Zebstrika: return {75_bi, 100_bi, 63_bi, 80_bi, 63_bi, 116_bi};
			case Species::Roggenrola: return {55_bi, 75_bi, 85_bi, 25_bi, 25_bi, 15_bi};
			case Species::Boldore: return {70_bi, 105_bi, 105_bi, 50_bi, 40_bi, 20_bi};
			case Species::Gigalith: return {
				85_bi,
				135_bi,
				130_bi,
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 70_bi, 80_bi),
				25_bi
			};
			case Species::Woobat: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 55_bi, 65_bi),
				45_bi,
				43_bi,
				55_bi,
				43_bi,
				72_bi
			};
			case Species::Swoobat: return {67_bi, 57_bi, 55_bi, 77_bi, 55_bi, 114_bi};
			case Species::Drilbur: return {60_bi, 85_bi, 40_bi, 30_bi, 45_bi, 68_bi};
			case Species::Excadrill: return {110_bi, 135_bi, 60_bi, 50_bi, 65_bi, 88_bi};
			case Species::Audino: return {103_bi, 60_bi, 86_bi, 60_bi, 86_bi, 50_bi};
			// case Species::Audino_Mega: return {103_bi, 60_bi, 126_bi, 80_bi, 126_bi, 50_bi};
			case Species::Timburr: return {75_bi, 80_bi, 55_bi, 25_bi, 35_bi, 35_bi};
			case Species::Gurdurr: return {85_bi, 105_bi, 85_bi, 40_bi, 50_bi, 40_bi};
			case Species::Conkeldurr: return {105_bi, 140_bi, 95_bi, 55_bi, 65_bi, 45_bi};
			case Species::Tympole: return {50_bi, 50_bi, 40_bi, 50_bi, 40_bi, 64_bi};
			case Species::Palpitoad: return {75_bi, 65_bi, 55_bi, 65_bi, 55_bi, 69_bi};
			case Species::Seismitoad: return {
				105_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi),
				75_bi,
				85_bi,
				75_bi,
				74_bi
			};
			case Species::Throh: return {120_bi, 100_bi, 85_bi, 30_bi, 85_bi, 45_bi};
			case Species::Sawk: return {75_bi, 125_bi, 75_bi, 30_bi, 75_bi, 85_bi};
			case Species::Sewaddle: return {45_bi, 53_bi, 70_bi, 40_bi, 60_bi, 42_bi};
			case Species::Swadloon: return {55_bi, 63_bi, 90_bi, 50_bi, 80_bi, 42_bi};
			case Species::Leavanny: return {
				75_bi,
				103_bi,
				80_bi,
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 70_bi, 80_bi),
				92_bi
			};
			case Species::Venipede: return {30_bi, 45_bi, 59_bi, 30_bi, 39_bi, 57_bi};
			case Species::Whirlipede: return {40_bi, 55_bi, 99_bi, 40_bi, 79_bi, 47_bi};
			case Species::Scolipede: return {
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 90_bi, 100_bi),
				89_bi,
				55_bi,
				69_bi,
				112_bi
			};
			case Species::Cottonee: return {40_bi, 27_bi, 60_bi, 37_bi, 50_bi, 66_bi};
			case Species::Whimsicott: return {60_bi, 67_bi, 85_bi, 77_bi, 75_bi, 116_bi};
			case Species::Petilil: return {45_bi, 35_bi, 50_bi, 70_bi, 50_bi, 30_bi};
			case Species::Lilligant: return {70_bi, 60_bi, 75_bi, 110_bi, 75_bi, 90_bi};
			case Species::Basculin: return {70_bi, 92_bi, 65_bi, 80_bi, 55_bi, 98_bi};
			case Species::Sandile: return {50_bi, 72_bi, 35_bi, 35_bi, 35_bi, 65_bi};
			case Species::Krokorok: return {60_bi, 82_bi, 45_bi, 45_bi, 45_bi, 74_bi};
			case Species::Krookodile: return {
				95_bi,
				117_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::five, 70_bi, 80_bi),
				65_bi,
				70_bi,
				92_bi
			};
			case Species::Darumaka: return {70_bi, 90_bi, 45_bi, 15_bi, 45_bi, 50_bi};
			case Species::Darmanitan: return {105_bi, 140_bi, 55_bi, 30_bi, 55_bi, 95_bi};
			// Used when a Darmanitan with the Zen Mode ability is under 50% HP
			// case Species::Darmanitan_Zen: return {105_bi, 30_bi, 105_bi, 140_bi, 105_bi, 55_bi};
			case Species::Maractus: return {75_bi, 86_bi, 67_bi, 106_bi, 67_bi, 60_bi};
			case Species::Dwebble: return {50_bi, 65_bi, 85_bi, 35_bi, 35_bi, 55_bi};
			case Species::Crustle: return {
				70_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 95_bi, 105_bi),
				125_bi,
				65_bi,
				75_bi,
				45_bi
			};
			case Species::Scraggy: return {50_bi, 75_bi, 70_bi, 35_bi, 70_bi, 48_bi};
			case Species::Scrafty: return {65_bi, 90_bi, 115_bi, 45_bi, 115_bi, 58_bi};
			case Species::Sigilyph: return {72_bi, 58_bi, 80_bi, 103_bi, 80_bi, 97_bi};
			case Species::Yamask: return {38_bi, 30_bi, 85_bi, 55_bi, 65_bi, 30_bi};
			case Species::Cofagrigus: return {58_bi, 50_bi, 145_bi, 95_bi, 105_bi, 30_bi};
			case Species::Tirtouga: return {54_bi, 78_bi, 103_bi, 53_bi, 45_bi, 22_bi};
			case Species::Carracosta: return {74_bi, 108_bi, 133_bi, 83_bi, 65_bi, 32_bi};
			case Species::Archen: return {55_bi, 112_bi, 45_bi, 74_bi, 45_bi, 70_bi};
			case Species::Archeops: return {75_bi, 140_bi, 65_bi, 112_bi, 65_bi, 110_bi};
			case Species::Trubbish: return {50_bi, 50_bi, 62_bi, 40_bi, 62_bi, 65_bi};
			case Species::Garbodor: return {80_bi, 95_bi, 82_bi, 60_bi, 82_bi, 75_bi};
			case Species::Zorua: return {40_bi, 65_bi, 40_bi, 80_bi, 40_bi, 65_bi};
			case Species::Zoroark: return {60_bi, 105_bi, 60_bi, 120_bi, 60_bi, 105_bi};
			case Species::Minccino: return {55_bi, 50_bi, 40_bi, 40_bi, 40_bi, 75_bi};
			case Species::Cinccino: return {75_bi, 95_bi, 60_bi, 65_bi, 60_bi, 115_bi};
			case Species::Gothita: return {45_bi, 30_bi, 50_bi, 55_bi, 65_bi, 45_bi};
			case Species::Gothorita: return {60_bi, 45_bi, 70_bi, 75_bi, 85_bi, 55_bi};
			case Species::Gothitelle: return {70_bi, 55_bi, 95_bi, 95_bi, 110_bi, 65_bi};
			case Species::Solosis: return {45_bi, 30_bi, 40_bi, 105_bi, 50_bi, 20_bi};
			case Species::Duosion: return {65_bi, 40_bi, 50_bi, 125_bi, 60_bi, 30_bi};
			case Species::Reuniclus: return {110_bi, 65_bi, 75_bi, 125_bi, 85_bi, 30_bi};
			case Species::Ducklett: return {62_bi, 44_bi, 50_bi, 44_bi, 50_bi, 55_bi};
			case Species::Swanna: return {75_bi, 87_bi, 63_bi, 87_bi, 63_bi, 98_bi};
			case Species::Vanillite: return {36_bi, 50_bi, 50_bi, 65_bi, 60_bi, 44_bi};
			case Species::Vanillish: return {51_bi, 65_bi, 65_bi, 80_bi, 75_bi, 59_bi};
			case Species::Vanilluxe: return {71_bi, 95_bi, 85_bi, 110_bi, 95_bi, 79_bi};
			case Species::Deerling: return {60_bi, 60_bi, 50_bi, 40_bi, 50_bi, 75_bi};
			case Species::Sawsbuck: return {80_bi, 100_bi, 70_bi, 60_bi, 70_bi, 95_bi};
			case Species::Emolga: return {55_bi, 75_bi, 60_bi, 75_bi, 60_bi, 103_bi};
			case Species::Karrablast: return {50_bi, 75_bi, 45_bi, 40_bi, 45_bi, 60_bi};
			case Species::Escavalier: return {70_bi, 135_bi, 105_bi, 60_bi, 105_bi, 20_bi};
			case Species::Foongus: return {69_bi, 55_bi, 45_bi, 55_bi, 55_bi, 15_bi};
			case Species::Amoonguss: return {114_bi, 85_bi, 70_bi, 85_bi, 80_bi, 30_bi};
			case Species::Frillish: return {55_bi, 40_bi, 50_bi, 65_bi, 85_bi, 40_bi};
			case Species::Jellicent: return {100_bi, 60_bi, 70_bi, 85_bi, 105_bi, 60_bi};
			case Species::Alomomola: return {165_bi, 75_bi, 80_bi, 40_bi, 45_bi, 65_bi};
			case Species::Joltik: return {50_bi, 47_bi, 50_bi, 57_bi, 50_bi, 65_bi};
			case Species::Galvantula: return {70_bi, 77_bi, 60_bi, 97_bi, 60_bi, 108_bi};
			case Species::Ferroseed: return {44_bi, 50_bi, 91_bi, 24_bi, 86_bi, 10_bi};
			case Species::Ferrothorn: return {74_bi, 94_bi, 131_bi, 54_bi, 116_bi, 20_bi};
			case Species::Klink: return {40_bi, 55_bi, 70_bi, 45_bi, 60_bi, 30_bi};
			case Species::Klang: return {60_bi, 80_bi, 95_bi, 70_bi, 85_bi, 50_bi};
			case Species::Klinklang: return {60_bi, 100_bi, 115_bi, 70_bi, 85_bi, 90_bi};
			case Species::Tynamo: return {35_bi, 55_bi, 40_bi, 45_bi, 40_bi, 60_bi};
			case Species::Eelektrik: return {65_bi, 85_bi, 70_bi, 75_bi, 70_bi, 40_bi};
			case Species::Eelektross: return {85_bi, 115_bi, 80_bi, 105_bi, 80_bi, 50_bi};
			case Species::Elgyem: return {55_bi, 55_bi, 55_bi, 85_bi, 55_bi, 30_bi};
			case Species::Beheeyem: return {75_bi, 75_bi, 75_bi, 125_bi, 95_bi, 40_bi};
			case Species::Litwick: return {50_bi, 30_bi, 55_bi, 65_bi, 55_bi, 20_bi};
			case Species::Lampent: return {60_bi, 40_bi, 60_bi, 95_bi, 60_bi, 55_bi};
			case Species::Chandelure: return {60_bi, 55_bi, 90_bi, 145_bi, 90_bi, 80_bi};
			case Species::Axew: return {46_bi, 87_bi, 60_bi, 30_bi, 40_bi, 57_bi};
			case Species::Fraxure: return {66_bi, 117_bi, 70_bi, 40_bi, 50_bi, 67_bi};
			case Species::Haxorus: return {76_bi, 147_bi, 90_bi, 60_bi, 70_bi, 97_bi};
			case Species::Cubchoo: return {55_bi, 70_bi, 40_bi, 60_bi, 40_bi, 40_bi};
			case Species::Beartic: return {
				95_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 110_bi, 130_bi),
				80_bi,
				70_bi,
				80_bi,
				50_bi
			};
			case Species::Cryogonal: return {
				BOUNDED_CONDITIONAL(generation <= Generation::six, 70_bi, 80_bi),
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::six, 30_bi, 50_bi),
				95_bi,
				135_bi,
				105_bi
			};
			case Species::Shelmet: return {50_bi, 40_bi, 85_bi, 40_bi, 65_bi, 25_bi};
			case Species::Accelgor: return {80_bi, 70_bi, 40_bi, 100_bi, 60_bi, 145_bi};
			case Species::Stunfisk: return {109_bi, 66_bi, 84_bi, 81_bi, 99_bi, 32_bi};
			case Species::Mienfoo: return {45_bi, 85_bi, 50_bi, 55_bi, 50_bi, 65_bi};
			case Species::Mienshao: return {65_bi, 125_bi, 60_bi, 95_bi, 60_bi, 105_bi};
			case Species::Druddigon: return {77_bi, 120_bi, 90_bi, 60_bi, 90_bi, 48_bi};
			case Species::Golett: return {59_bi, 74_bi, 50_bi, 35_bi, 50_bi, 35_bi};
			case Species::Golurk: return {89_bi, 124_bi, 80_bi, 55_bi, 80_bi, 55_bi};
			case Species::Pawniard: return {45_bi, 85_bi, 70_bi, 40_bi, 40_bi, 60_bi};
			case Species::Bisharp: return {65_bi, 125_bi, 100_bi, 60_bi, 70_bi, 70_bi};
			case Species::Bouffalant: return {95_bi, 110_bi, 95_bi, 40_bi, 95_bi, 55_bi};
			case Species::Rufflet: return {70_bi, 83_bi, 50_bi, 37_bi, 50_bi, 60_bi};
			case Species::Braviary: return {100_bi, 123_bi, 75_bi, 57_bi, 75_bi, 80_bi};
			case Species::Vullaby: return {70_bi, 55_bi, 75_bi, 45_bi, 65_bi, 60_bi};
			case Species::Mandibuzz: return {110_bi, 65_bi, 105_bi, 55_bi, 95_bi, 80_bi};
			case Species::Heatmor: return {85_bi, 97_bi, 66_bi, 105_bi, 66_bi, 65_bi};
			case Species::Durant: return {58_bi, 109_bi, 112_bi, 48_bi, 48_bi, 109_bi};
			case Species::Deino: return {52_bi, 65_bi, 50_bi, 45_bi, 50_bi, 38_bi};
			case Species::Zweilous: return {72_bi, 85_bi, 70_bi, 65_bi, 70_bi, 58_bi};
			case Species::Hydreigon: return {92_bi, 105_bi, 90_bi, 125_bi, 90_bi, 98_bi};
			case Species::Larvesta: return {55_bi, 85_bi, 55_bi, 50_bi, 55_bi, 60_bi};
			case Species::Volcarona: return {85_bi, 60_bi, 65_bi, 135_bi, 105_bi, 100_bi};
			case Species::Cobalion: return {91_bi, 90_bi, 129_bi, 90_bi, 72_bi, 108_bi};
			case Species::Terrakion: return {91_bi, 129_bi, 90_bi, 72_bi, 90_bi, 108_bi};
			case Species::Virizion: return {91_bi, 90_bi, 72_bi, 90_bi, 129_bi, 108_bi};
			case Species::Tornadus_Incarnate: return {79_bi, 115_bi, 70_bi, 125_bi, 80_bi, 111_bi};
			case Species::Tornadus_Therian: return {79_bi, 100_bi, 80_bi, 110_bi, 90_bi, 121_bi};
			case Species::Thundurus_Incarnate: return {79_bi, 115_bi, 70_bi, 125_bi, 80_bi, 111_bi};
			case Species::Thundurus_Therian: return {79_bi, 105_bi, 70_bi, 145_bi, 80_bi, 101_bi};
			case Species::Reshiram: return {100_bi, 120_bi, 100_bi, 150_bi, 120_bi, 90_bi};
			case Species::Zekrom: return {100_bi, 150_bi, 120_bi, 120_bi, 100_bi, 90_bi};
			case Species::Landorus_Incarnate: return {89_bi, 125_bi, 90_bi, 115_bi, 80_bi, 101_bi};
			case Species::Landorus_Therian: return {89_bi, 145_bi, 90_bi, 105_bi, 80_bi, 91_bi};
			case Species::Kyurem: return {125_bi, 130_bi, 90_bi, 130_bi, 90_bi, 95_bi};
			case Species::Kyurem_Black: return {125_bi, 170_bi, 100_bi, 120_bi, 90_bi, 95_bi};
			case Species::Kyurem_White: return {125_bi, 120_bi, 90_bi, 170_bi, 100_bi, 95_bi};
			case Species::Keldeo: return {91_bi, 72_bi, 90_bi, 129_bi, 90_bi, 108_bi};
			case Species::Meloetta: return {100_bi, 77_bi, 77_bi, 128_bi, 128_bi, 90_bi};
			// Changes if Meloetta successfully uses Relic Song
			// case Species::Meloetta_Pirouette: return {100_bi, 128_bi, 90_bi, 77_bi, 77_bi, 128_bi};
			case Species::Genesect: return {71_bi, 120_bi, 95_bi, 120_bi, 95_bi, 99_bi};
			case Species::Chespin: return {56_bi, 61_bi, 65_bi, 48_bi, 45_bi, 38_bi};
			case Species::Quilladin: return {61_bi, 78_bi, 95_bi, 56_bi, 58_bi, 57_bi};
			case Species::Chesnaught: return {88_bi, 107_bi, 122_bi, 74_bi, 75_bi, 64_bi};
			case Species::Fennekin: return {40_bi, 45_bi, 40_bi, 62_bi, 60_bi, 60_bi};
			case Species::Braixen: return {59_bi, 59_bi, 58_bi, 90_bi, 70_bi, 73_bi};
			case Species::Delphox: return {75_bi, 69_bi, 72_bi, 114_bi, 100_bi, 104_bi};
			case Species::Froakie: return {41_bi, 56_bi, 40_bi, 62_bi, 44_bi, 71_bi};
			case Species::Frogadier: return {54_bi, 63_bi, 52_bi, 83_bi, 56_bi, 97_bi};
			case Species::Greninja: return {72_bi, 95_bi, 67_bi, 103_bi, 71_bi, 122_bi};
			// If Greninja has Battle Bond as an ability and it KOs a Pokemon
			// case Species::Greninja_Ash: return {72_bi, 145_bi, 67_bi, 153_bi, 71_bi, 132_bi};
			case Species::Bunnelby: return {38_bi, 36_bi, 38_bi, 32_bi, 36_bi, 57_bi};
			case Species::Diggersby: return {85_bi, 56_bi, 77_bi, 50_bi, 77_bi, 78_bi};
			case Species::Fletchling: return {45_bi, 50_bi, 43_bi, 40_bi, 38_bi, 62_bi};
			case Species::Fletchinder: return {62_bi, 73_bi, 55_bi, 56_bi, 52_bi, 84_bi};
			case Species::Talonflame: return {78_bi, 81_bi, 71_bi, 74_bi, 69_bi, 126_bi};
			case Species::Scatterbug: return {38_bi, 35_bi, 40_bi, 27_bi, 25_bi, 35_bi};
			case Species::Spewpa: return {45_bi, 22_bi, 60_bi, 27_bi, 30_bi, 29_bi};
			case Species::Vivillon: return {80_bi, 52_bi, 50_bi, 90_bi, 50_bi, 89_bi};
			case Species::Litleo: return {62_bi, 50_bi, 58_bi, 73_bi, 54_bi, 72_bi};
			case Species::Pyroar: return {86_bi, 68_bi, 72_bi, 109_bi, 66_bi, 106_bi};
			case Species::Flabebe: return {44_bi, 38_bi, 39_bi, 61_bi, 79_bi, 42_bi};
			case Species::Floette: return {54_bi, 45_bi, 47_bi, 75_bi, 98_bi, 52_bi};
			case Species::Florges: return {78_bi, 65_bi, 68_bi, 112_bi, 154_bi, 75_bi};
			case Species::Skiddo: return {66_bi, 65_bi, 48_bi, 62_bi, 57_bi, 52_bi};
			case Species::Gogoat: return {123_bi, 100_bi, 62_bi, 97_bi, 81_bi, 68_bi};
			case Species::Pancham: return {67_bi, 82_bi, 62_bi, 46_bi, 48_bi, 43_bi};
			case Species::Pangoro: return {95_bi, 124_bi, 78_bi, 69_bi, 71_bi, 58_bi};
			case Species::Furfrou: return {75_bi, 80_bi, 60_bi, 65_bi, 90_bi, 102_bi};
			case Species::Espurr: return {62_bi, 48_bi, 54_bi, 63_bi, 60_bi, 68_bi};
			case Species::Meowstic: return {74_bi, 48_bi, 76_bi, 83_bi, 81_bi, 104_bi};
			case Species::Honedge: return {45_bi, 80_bi, 100_bi, 35_bi, 37_bi, 28_bi};
			case Species::Doublade: return {59_bi, 110_bi, 150_bi, 45_bi, 49_bi, 35_bi};
			// Changes before using defensive move
			case Species::Aegislash: return {
				60_bi,
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::seven, 140_bi, 150_bi),
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::seven, 140_bi, 150_bi),
				60_bi
			};
			// Changes before using offensive move
			#if 0
			case Species::Aegislash_Blade: return {
				60_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::seven, 140_bi, 150_bi),
				50_bi,
				BOUNDED_CONDITIONAL(generation <= Generation::seven, 140_bi, 150_bi),
				50_bi,
				60_bi
			};
			#endif
			case Species::Spritzee: return {78_bi, 52_bi, 60_bi, 63_bi, 65_bi, 23_bi};
			case Species::Aromatisse: return {101_bi, 72_bi, 72_bi, 99_bi, 89_bi, 29_bi};
			case Species::Swirlix: return {62_bi, 48_bi, 66_bi, 59_bi, 57_bi, 49_bi};
			case Species::Slurpuff: return {82_bi, 80_bi, 86_bi, 85_bi, 75_bi, 72_bi};
			case Species::Inkay: return {53_bi, 54_bi, 53_bi, 37_bi, 46_bi, 45_bi};
			case Species::Malamar: return {86_bi, 92_bi, 88_bi, 68_bi, 75_bi, 73_bi};
			case Species::Binacle: return {42_bi, 52_bi, 67_bi, 39_bi, 56_bi, 50_bi};
			case Species::Barbaracle: return {72_bi, 105_bi, 115_bi, 54_bi, 86_bi, 68_bi};
			case Species::Skrelp: return {50_bi, 60_bi, 60_bi, 60_bi, 60_bi, 30_bi};
			case Species::Dragalge: return {65_bi, 75_bi, 90_bi, 97_bi, 123_bi, 44_bi};
			case Species::Clauncher: return {50_bi, 53_bi, 62_bi, 58_bi, 63_bi, 44_bi};
			case Species::Clawitzer: return {71_bi, 73_bi, 88_bi, 120_bi, 89_bi, 59_bi};
			case Species::Helioptile: return {44_bi, 38_bi, 33_bi, 61_bi, 43_bi, 70_bi};
			case Species::Heliolisk: return {62_bi, 55_bi, 52_bi, 109_bi, 94_bi, 109_bi};
			case Species::Tyrunt: return {58_bi, 89_bi, 77_bi, 45_bi, 45_bi, 48_bi};
			case Species::Tyrantrum: return {82_bi, 121_bi, 119_bi, 69_bi, 59_bi, 71_bi};
			case Species::Amaura: return {77_bi, 59_bi, 50_bi, 67_bi, 63_bi, 46_bi};
			case Species::Aurorus: return {123_bi, 77_bi, 72_bi, 99_bi, 92_bi, 58_bi};
			case Species::Sylveon: return {95_bi, 65_bi, 65_bi, 110_bi, 130_bi, 60_bi};
			case Species::Hawlucha: return {78_bi, 92_bi, 75_bi, 74_bi, 63_bi, 118_bi};
			case Species::Dedenne: return {67_bi, 58_bi, 57_bi, 81_bi, 67_bi, 101_bi};
			case Species::Carbink: return {50_bi, 50_bi, 150_bi, 50_bi, 150_bi, 50_bi};
			case Species::Goomy: return {45_bi, 50_bi, 35_bi, 55_bi, 75_bi, 40_bi};
			case Species::Sliggoo: return {68_bi, 75_bi, 53_bi, 83_bi, 113_bi, 60_bi};
			case Species::Goodra: return {90_bi, 100_bi, 70_bi, 110_bi, 150_bi, 80_bi};
			case Species::Klefki: return {57_bi, 80_bi, 91_bi, 80_bi, 87_bi, 75_bi};
			case Species::Phantump: return {43_bi, 70_bi, 48_bi, 50_bi, 60_bi, 38_bi};
			case Species::Trevenant: return {85_bi, 110_bi, 76_bi, 65_bi, 82_bi, 56_bi};
			case Species::Pumpkaboo_Small: return {44_bi, 66_bi, 70_bi, 44_bi, 55_bi, 56_bi};
			case Species::Pumpkaboo_Average: return {49_bi, 66_bi, 70_bi, 44_bi, 55_bi, 51_bi};
			case Species::Pumpkaboo_Large: return {54_bi, 66_bi, 70_bi, 44_bi, 55_bi, 46_bi};
			case Species::Pumpkaboo_Super: return {59_bi, 66_bi, 70_bi, 44_bi, 55_bi, 41_bi};
			case Species::Gourgeist_Small: return {55_bi, 85_bi, 122_bi, 58_bi, 75_bi, 99_bi};
			case Species::Gourgeist_Average: return {65_bi, 90_bi, 122_bi, 58_bi, 75_bi, 84_bi};
			case Species::Gourgeist_Large: return {75_bi, 95_bi, 122_bi, 58_bi, 75_bi, 69_bi};
			case Species::Gourgeist_Super: return {85_bi, 100_bi, 122_bi, 58_bi, 75_bi, 54_bi};
			case Species::Bergmite: return {55_bi, 69_bi, 85_bi, 32_bi, 35_bi, 28_bi};
			case Species::Avalugg: return {95_bi, 117_bi, 184_bi, 44_bi, 46_bi, 28_bi};
			case Species::Noibat: return {40_bi, 30_bi, 35_bi, 45_bi, 40_bi, 55_bi};
			case Species::Noivern: return {85_bi, 70_bi, 80_bi, 97_bi, 80_bi, 123_bi};
			case Species::Xerneas: return {126_bi, 131_bi, 95_bi, 131_bi, 98_bi, 99_bi};
			case Species::Yveltal: return {126_bi, 131_bi, 95_bi, 131_bi, 98_bi, 99_bi};
			case Species::Zygarde_50: return {108_bi, 100_bi, 121_bi, 81_bi, 95_bi, 95_bi};
			case Species::Zygarde_10: return {54_bi, 100_bi, 71_bi, 61_bi, 85_bi, 115_bi};
			case Species::Zygarde_Complete: return {216_bi, 100_bi, 121_bi, 91_bi, 95_bi, 85_bi};
			case Species::Diancie: return {50_bi, 100_bi, 150_bi, 100_bi, 150_bi, 50_bi};
			// case Species::Diancie_Mega: return {50_bi, 160_bi, 110_bi, 160_bi, 110_bi, 110_bi};
			case Species::Hoopa_Confined: return {80_bi, 110_bi, 60_bi, 150_bi, 130_bi, 70_bi};
			case Species::Hoopa_Unbound: return {80_bi, 160_bi, 60_bi, 170_bi, 130_bi, 80_bi};
			case Species::Volcanion: return {80_bi, 110_bi, 120_bi, 130_bi, 90_bi, 70_bi};
			case Species::Rowlet: return {68_bi, 55_bi, 55_bi, 50_bi, 50_bi, 42_bi};
			case Species::Dartrix: return {78_bi, 75_bi, 75_bi, 70_bi, 70_bi, 52_bi};
			case Species::Decidueye: return {78_bi, 107_bi, 75_bi, 100_bi, 100_bi, 70_bi};
			case Species::Litten: return {45_bi, 65_bi, 40_bi, 60_bi, 40_bi, 70_bi};
			case Species::Torracat: return {65_bi, 85_bi, 50_bi, 80_bi, 50_bi, 90_bi};
			case Species::Incineroar: return {95_bi, 115_bi, 90_bi, 80_bi, 90_bi, 60_bi};
			case Species::Popplio: return {50_bi, 54_bi, 54_bi, 66_bi, 56_bi, 40_bi};
			case Species::Brionne: return {60_bi, 69_bi, 69_bi, 91_bi, 81_bi, 50_bi};
			case Species::Primarina: return {80_bi, 74_bi, 74_bi, 126_bi, 116_bi, 60_bi};
			case Species::Pikipek: return {35_bi, 75_bi, 30_bi, 30_bi, 30_bi, 65_bi};
			case Species::Trumbeak: return {55_bi, 85_bi, 50_bi, 40_bi, 50_bi, 75_bi};
			case Species::Toucannon: return {80_bi, 120_bi, 75_bi, 75_bi, 75_bi, 60_bi};
			case Species::Yungoos: return {48_bi, 70_bi, 30_bi, 30_bi, 30_bi, 45_bi};
			case Species::Gumshoos: return {88_bi, 110_bi, 60_bi, 55_bi, 60_bi, 45_bi};
			case Species::Grubbin: return {47_bi, 62_bi, 45_bi, 55_bi, 45_bi, 46_bi};
			case Species::Charjabug: return {57_bi, 82_bi, 95_bi, 55_bi, 75_bi, 36_bi};
			case Species::Vikavolt: return {77_bi, 70_bi, 90_bi, 145_bi, 75_bi, 43_bi};
			case Species::Crabrawler: return {47_bi, 82_bi, 57_bi, 42_bi, 47_bi, 63_bi};
			case Species::Crabominable: return {97_bi, 132_bi, 77_bi, 62_bi, 67_bi, 43_bi};
			case Species::Oricorio: return {75_bi, 70_bi, 70_bi, 98_bi, 70_bi, 93_bi};
			case Species::Oricorio_Pau: return {75_bi, 70_bi, 70_bi, 98_bi, 70_bi, 93_bi};
			case Species::Oricorio_Pom_Pom: return {75_bi, 70_bi, 70_bi, 98_bi, 70_bi, 93_bi};
			case Species::Oricorio_Sensu: return {75_bi, 70_bi, 70_bi, 98_bi, 70_bi, 93_bi};
			case Species::Cutiefly: return {40_bi, 45_bi, 40_bi, 55_bi, 40_bi, 84_bi};
			case Species::Ribombee: return {60_bi, 55_bi, 60_bi, 95_bi, 70_bi, 124_bi};
			case Species::Rockruff: return {45_bi, 65_bi, 40_bi, 30_bi, 40_bi, 60_bi};
			case Species::Lycanroc_Midday: return {75_bi, 115_bi, 65_bi, 55_bi, 65_bi, 112_bi};
			case Species::Lycanroc_Midnight: return {85_bi, 115_bi, 75_bi, 55_bi, 75_bi, 82_bi};
			case Species::Lycanroc_Dusk: return {75_bi, 117_bi, 65_bi, 55_bi, 65_bi, 110_bi};
			case Species::Wishiwashi_Solo: return {45_bi, 20_bi, 20_bi, 25_bi, 25_bi, 40_bi};
			case Species::Wishiwashi_School: return {45_bi, 140_bi, 130_bi, 140_bi, 135_bi, 30_bi};
			case Species::Mareanie: return {50_bi, 53_bi, 62_bi, 43_bi, 52_bi, 45_bi};
			case Species::Toxapex: return {50_bi, 63_bi, 152_bi, 53_bi, 142_bi, 35_bi};
			case Species::Mudbray: return {70_bi, 100_bi, 70_bi, 45_bi, 55_bi, 45_bi};
			case Species::Mudsdale: return {100_bi, 125_bi, 100_bi, 55_bi, 85_bi, 35_bi};
			case Species::Dewpider: return {38_bi, 40_bi, 52_bi, 40_bi, 72_bi, 27_bi};
			case Species::Araquanid: return {68_bi, 70_bi, 92_bi, 50_bi, 132_bi, 42_bi};
			case Species::Fomantis: return {40_bi, 55_bi, 35_bi, 50_bi, 35_bi, 35_bi};
			case Species::Lurantis: return {70_bi, 105_bi, 90_bi, 80_bi, 90_bi, 45_bi};
			case Species::Morelull: return {40_bi, 35_bi, 55_bi, 65_bi, 75_bi, 15_bi};
			case Species::Shiinotic: return {60_bi, 45_bi, 80_bi, 90_bi, 100_bi, 30_bi};
			case Species::Salandit: return {48_bi, 44_bi, 40_bi, 71_bi, 40_bi, 77_bi};
			case Species::Salazzle: return {68_bi, 64_bi, 60_bi, 111_bi, 60_bi, 117_bi};
			case Species::Stufful: return {70_bi, 75_bi, 50_bi, 45_bi, 50_bi, 50_bi};
			case Species::Bewear: return {120_bi, 125_bi, 80_bi, 55_bi, 60_bi, 60_bi};
			case Species::Bounsweet: return {42_bi, 30_bi, 38_bi, 30_bi, 38_bi, 32_bi};
			case Species::Steenee: return {52_bi, 40_bi, 48_bi, 40_bi, 48_bi, 62_bi};
			case Species::Tsareena: return {72_bi, 120_bi, 98_bi, 50_bi, 98_bi, 72_bi};
			case Species::Comfey: return {51_bi, 52_bi, 90_bi, 82_bi, 110_bi, 100_bi};
			case Species::Oranguru: return {90_bi, 60_bi, 80_bi, 90_bi, 110_bi, 60_bi};
			case Species::Passimian: return {100_bi, 120_bi, 90_bi, 40_bi, 60_bi, 80_bi};
			case Species::Wimpod: return {25_bi, 35_bi, 40_bi, 20_bi, 30_bi, 80_bi};
			case Species::Golisopod: return {75_bi, 125_bi, 140_bi, 60_bi, 90_bi, 40_bi};
			case Species::Sandygast: return {55_bi, 55_bi, 80_bi, 70_bi, 45_bi, 15_bi};
			case Species::Palossand: return {85_bi, 75_bi, 110_bi, 100_bi, 75_bi, 35_bi};
			case Species::Pyukumuku: return {55_bi, 60_bi, 130_bi, 30_bi, 130_bi, 5_bi};
			case Species::Type_Null: return {95_bi, 95_bi, 95_bi, 95_bi, 95_bi, 59_bi};
			case Species::Silvally: return {95_bi, 95_bi, 95_bi, 95_bi, 95_bi, 95_bi};
			case Species::Minior_Meteor: return {60_bi, 60_bi, 100_bi, 60_bi, 100_bi, 60_bi};
			case Species::Minior_Core: return {60_bi, 100_bi, 60_bi, 100_bi, 60_bi, 120_bi};
			case Species::Komala: return {65_bi, 115_bi, 65_bi, 75_bi, 95_bi, 65_bi};
			case Species::Turtonator: return {60_bi, 78_bi, 135_bi, 91_bi, 85_bi, 36_bi};
			case Species::Togedemaru: return {65_bi, 98_bi, 63_bi, 40_bi, 73_bi, 96_bi};
			case Species::Mimikyu: return {55_bi, 90_bi, 80_bi, 50_bi, 105_bi, 96_bi};
			case Species::Bruxish: return {68_bi, 105_bi, 70_bi, 70_bi, 70_bi, 92_bi};
			case Species::Drampa: return {78_bi, 60_bi, 85_bi, 135_bi, 91_bi, 36_bi};
			case Species::Dhelmise: return {70_bi, 131_bi, 100_bi, 86_bi, 90_bi, 40_bi};
			case Species::Jangmo_o: return {45_bi, 55_bi, 65_bi, 45_bi, 45_bi, 45_bi};
			case Species::Hakamo_o: return {55_bi, 75_bi, 90_bi, 65_bi, 70_bi, 65_bi};
			case Species::Kommo_o: return {75_bi, 110_bi, 125_bi, 100_bi, 105_bi, 85_bi};
			case Species::Tapu_Koko: return {70_bi, 115_bi, 85_bi, 95_bi, 75_bi, 130_bi};
			case Species::Tapu_Lele: return {70_bi, 85_bi, 75_bi, 130_bi, 115_bi, 95_bi};
			case Species::Tapu_Bulu: return {70_bi, 130_bi, 115_bi, 85_bi, 95_bi, 75_bi};
			case Species::Tapu_Fini: return {70_bi, 75_bi, 115_bi, 95_bi, 130_bi, 85_bi};
			case Species::Cosmog: return {43_bi, 29_bi, 31_bi, 29_bi, 31_bi, 37_bi};
			case Species::Cosmoem: return {43_bi, 29_bi, 131_bi, 29_bi, 131_bi, 37_bi};
			case Species::Solgaleo: return {137_bi, 137_bi, 107_bi, 113_bi, 89_bi, 97_bi};
			case Species::Lunala: return {137_bi, 113_bi, 89_bi, 137_bi, 107_bi, 97_bi};
			case Species::Nihilego: return {109_bi, 53_bi, 47_bi, 127_bi, 131_bi, 103_bi};
			case Species::Buzzwole: return {107_bi, 139_bi, 139_bi, 53_bi, 53_bi, 79_bi};
			case Species::Pheromosa: return {71_bi, 137_bi, 37_bi, 137_bi, 37_bi, 151_bi};
			case Species::Xurkitree: return {83_bi, 89_bi, 71_bi, 173_bi, 71_bi, 83_bi};
			case Species::Celesteela: return {97_bi, 101_bi, 103_bi, 107_bi, 101_bi, 61_bi};
			case Species::Kartana: return {59_bi, 181_bi, 131_bi, 59_bi, 31_bi, 109_bi};
			case Species::Guzzlord: return {223_bi, 101_bi, 53_bi, 97_bi, 53_bi, 43_bi};
			case Species::Necrozma: return {97_bi, 107_bi, 101_bi, 127_bi, 89_bi, 79_bi};
			case Species::Necrozma_Dusk_Mane: return {97_bi, 157_bi, 127_bi, 113_bi, 109_bi, 77_bi};
			case Species::Necrozma_Dawn_Wings: return {97_bi, 113_bi, 109_bi, 157_bi, 127_bi, 77_bi};
			case Species::Necrozma_Ultra: return {97_bi, 167_bi, 97_bi, 167_bi, 97_bi, 129_bi};
			case Species::Magearna: return {80_bi, 95_bi, 115_bi, 130_bi, 115_bi, 65_bi};
			case Species::Marshadow: return {90_bi, 125_bi, 80_bi, 90_bi, 90_bi, 125_bi};
			case Species::Poipole: return {67_bi, 73_bi, 67_bi, 73_bi, 67_bi, 73_bi};
			case Species::Naganadel: return {73_bi, 73_bi, 73_bi, 127_bi, 73_bi, 121_bi};
			case Species::Stakataka: return {61_bi, 131_bi, 211_bi, 53_bi, 101_bi, 13_bi};
			case Species::Blacephalon: return {53_bi, 127_bi, 53_bi, 151_bi, 79_bi, 107_bi};
			case Species::Zeraora: return {88_bi, 112_bi, 75_bi, 102_bi, 80_bi, 143_bi};
		}
	}())
{
}

}	// namespace technicalmachine
