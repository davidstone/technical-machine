// Move power calculator
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

#include "power.hpp"

#include "base_power.hpp"
#include "move.hpp"
#include "moves.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../stat/calculate.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/happiness.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include <containers/algorithms/accumulate.hpp>
#include <containers/array/array.hpp>

#include <algorithm>
#include <cassert>
#include <numeric>
#include <stdexcept>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto power_of_mass_based_moves(Species species) -> bounded::integer<20, 120>;

auto variable_adjusted_base_power(Team const & attacker_team, Team const & defender_team, Weather const weather, Variable const & variable) -> VariableAdjustedBasePower {
	auto const & attacker = attacker_team.pokemon();
	auto const & defender = defender_team.pokemon();
	switch (static_cast<Moves>(current_move(attacker))) {
		case Moves::Crush_Grip:
		case Moves::Wring_Out:
			return bounded::integer<1, 121>(120_bi * hp_ratio(defender) + 1_bi, bounded::non_check);
		case Moves::Eruption:
		case Moves::Water_Spout:
			return bounded::integer<1, 150>(bounded::max(150_bi * hp_ratio(attacker), 1_bi), bounded::non_check);
		case Moves::Flail:
		case Moves::Reversal: {
			auto const k = 64_bi * hp_ratio(attacker);
			if (k <= 1_bi)
				return 200_bi;
			else if (k <= 5_bi)
				return 150_bi;
			else if (k <= 12_bi)
				return 100_bi;
			else if (k <= 21_bi)
				return 80_bi;
			else if (k <= 42_bi)
				return 40_bi;
			else
				return 20_bi;
		}
		case Moves::Fling:
			return fling_power(get_item(attacker));
		case Moves::Frustration:
			return frustration_power(get_happiness(attacker));
		case Moves::Fury_Cutter:
			return last_used_move(attacker).fury_cutter_power();
		case Moves::Grass_Knot:
		case Moves::Low_Kick:
			return power_of_mass_based_moves(defender);
		case Moves::Gyro_Ball: {
			auto const defender_speed = calculate_speed(defender_team, weather);
			auto const attacker_speed = calculate_speed(attacker_team, weather);
			auto const uncapped_power = 25_bi * defender_speed / attacker_speed + 1_bi;
			return bounded::min(uncapped_power, 150_bi);
		}
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return last_used_move(attacker).momentum_move_power();
		case Moves::Hidden_Power: {
			using stat_and_position_type = std::pair<StatNames, bounded::integer<0, 5>>;
			static constexpr containers::array<stat_and_position_type, 5> stat_and_position {{
				{ StatNames::ATK, 1_bi },
				{ StatNames::DEF, 2_bi },
				{ StatNames::SPE, 3_bi },
				{ StatNames::SPA, 4_bi },
				{ StatNames::SPD, 5_bi }
			}};
			using intermediate_type = bounded::checked_integer<0, (1 << 6) - 1>;
			auto const sum = [&](intermediate_type value, stat_and_position_type const & stat) {
				return value + (((get_stat(attacker, stat.first).iv().value() / 2_bi) % 2_bi) << stat.second);
			};
			intermediate_type const initial = (get_hp(attacker).iv().value() / 2_bi) % 2_bi;
			auto const result = std::accumulate(std::begin(stat_and_position), std::end(stat_and_position), initial, sum) * 40_bi / 63_bi + 30_bi;
			static_assert(std::numeric_limits<decltype(result)>::min() == 30_bi, "Incorrect Hidden Power minimum.");
			static_assert(std::numeric_limits<decltype(result)>::max() == 70_bi, "Incorrect Hidden Power maximum.");
			return result;
		}
		case Moves::Magnitude:
			return variable.value;
		case Moves::Natural_Gift:
			return berry_power(get_item(attacker));
		case Moves::Present:
			assert(!present_heals(variable));
			return variable.value;
		case Moves::Punishment: {
			auto const & boosts = stage(defender);
			auto const filtered = containers::filter_iterator(begin(boosts), end(boosts), [](auto const value) { return value > 0_bi; });
			auto const uncapped_power = 60_bi + 20_bi * bounded::increase_min<0>(containers::accumulate(filtered, end(boosts)));
			return bounded::min(uncapped_power, 200_bi);
		}
		case Moves::Return:
			return return_power(get_happiness(attacker));
		case Moves::Spit_Up:
			return spit_up_power(attacker);
		case Moves::Triple_Kick:
			return last_used_move(attacker).triple_kick_power();
		case Moves::Trump_Card:
			return current_move(attacker).pp().trump_card_power();
		default:
			return *base_power(current_move(attacker));
	}
}

auto doubling(ActivePokemon const attacker, ActivePokemon const defender, Weather weather) -> bool;

auto item_modifier_numerator(Pokemon const & attacker) -> bounded::integer<10, 12>;
auto item_modifier(Pokemon const & attacker) {
	return make_rational(item_modifier_numerator(attacker), 10_bi);
}

auto is_boosted_by_adamant_orb(Species species) -> bool;
auto is_boosted_by_griseous_orb(Species species) -> bool;
auto is_boosted_by_lustrous_orb(Species species) -> bool;

auto defender_ability_modifier(Pokemon const & attacker, Ability const ability) -> bounded_rational<bounded::integer<1, 5>, bounded::integer<1, 4>> {
	Moves const move = current_move(attacker);
	switch (ability.name()) {
		case Ability::Dry_Skin:
			return make_rational(BOUNDED_CONDITIONAL(get_type(move, attacker) == Type::Fire, 5_bi, 4_bi), 4_bi);
		case Ability::Heatproof:
			return make_rational(1_bi, BOUNDED_CONDITIONAL(get_type(move, attacker) == Type::Fire, 2_bi, 1_bi));
		case Ability::Thick_Fat: {
			auto const type = get_type(move, attacker);
			return make_rational(1_bi, BOUNDED_CONDITIONAL(type == Type::Fire or type == Type::Ice, 2_bi, 1_bi));
		}
		default:
			return make_rational(1_bi, 1_bi);
	}
}

}	// namespace

auto move_power(Team const & attacker_team, Team const & defender_team, Weather const weather, Variable const & variable) -> MovePower {
	auto const & attacker = attacker_team.pokemon();
	auto const & defender = defender_team.pokemon();
	auto const base_power = variable_adjusted_base_power(attacker_team, defender_team, weather, variable);
	return static_cast<bounded::equivalent_type<MovePower, bounded::throw_policy<>>>(bounded::max(1_bi,
		base_power *
		BOUNDED_CONDITIONAL(doubling(attacker, defender, weather), 2_bi, 1_bi) *
		item_modifier(attacker) *
		BOUNDED_CONDITIONAL(charge_boosted(attacker), 2_bi, 1_bi) /
		BOUNDED_CONDITIONAL(sport_is_active(defender, current_move(attacker)), 2_bi, 1_bi) *
		attacker_ability_power_modifier(attacker, defender, base_power) *
		defender_ability_modifier(attacker, get_ability(defender))
	));
}

namespace {

auto doubling(ActivePokemon const attacker, ActivePokemon const defender, Weather const weather) -> bool {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	Moves const move = current_move(attacker);
	if (vanish_doubles_power(defender, move))
		return true;
	switch (move) {
		case Moves::Assurance:
			return static_cast<bool>(damaged(defender));
		case Moves::Avalanche: 
		case Moves::Revenge:
			return static_cast<bool>(damaged(attacker));
		case Moves::Brine:
			return get_hp(defender).current() <= get_hp(defender).max() / 2_bi;
		case Moves::Facade:
			return boosts_facade(get_status(attacker));
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return defense_curled(attacker);
		case Moves::Payback:
			return moved(defender);
		case Moves::SmellingSalt:
			return boosts_smellingsalt(get_status(defender));
		case Moves::SolarBeam:
			return !weather.rain();
		case Moves::Stomp:
			return minimized(defender);
		case Moves::Wake_Up_Slap:
			return is_sleeping(get_status(defender));
		case Moves::Weather_Ball:
			return weather.hail() or weather.rain() or weather.sand() or weather.sun();
		default:
			return false;
	}
}

auto item_modifier_numerator(Pokemon const & attacker) -> bounded::integer<10, 12> {
	static constexpr auto base = 10_bi;
	auto const type = get_type(current_move(attacker), attacker);
	switch (get_item(attacker)) {
		case Item::Muscle_Band:
			return BOUNDED_CONDITIONAL(is_physical(current_move(attacker)), 11_bi, base);
		case Item::Wise_Glasses:
			return BOUNDED_CONDITIONAL(is_special(current_move(attacker)), 11_bi, base);
		case Item::Insect_Plate:
		case Item::SilverPowder:
			return BOUNDED_CONDITIONAL(type == Type::Bug, 12_bi, base);
		case Item::Dread_Plate:	
		case Item::BlackGlasses:
			return BOUNDED_CONDITIONAL(type == Type::Dark, 12_bi, base);
		case Item::Draco_Plate:
		case Item::Dragon_Fang:
			return BOUNDED_CONDITIONAL(type == Type::Dragon, 12_bi, base);
		case Item::Zap_Plate:
		case Item::Magnet:
			return BOUNDED_CONDITIONAL(type == Type::Electric, 12_bi, base);
		case Item::Fist_Plate:
		case Item::Black_Belt:
			return BOUNDED_CONDITIONAL(type == Type::Fighting, 12_bi, base);
		case Item::Flame_Plate:
		case Item::Charcoal:
			return BOUNDED_CONDITIONAL(type == Type::Fire, 12_bi, base);
		case Item::Sky_Plate:
		case Item::Sharp_Beak:
			return BOUNDED_CONDITIONAL(type == Type::Flying, 12_bi, base);
		case Item::Spooky_Plate:
		case Item::Spell_Tag:
			return BOUNDED_CONDITIONAL(type == Type::Ghost, 12_bi, base);
		case Item::Meadow_Plate:
		case Item::Miracle_Seed:
			return BOUNDED_CONDITIONAL(type == Type::Grass, 12_bi, base);
		case Item::Earth_Plate:
		case Item::Soft_Sand:
			return BOUNDED_CONDITIONAL(type == Type::Ground, 12_bi, base);
		case Item::Icicle_Plate:
		case Item::NeverMeltIce:
			return BOUNDED_CONDITIONAL(type == Type::Ice, 12_bi, base);
		case Item::Silk_Scarf:
			return BOUNDED_CONDITIONAL(type == Type::Normal, 12_bi, base);
		case Item::Toxic_Plate:
		case Item::Poison_Barb:
			return BOUNDED_CONDITIONAL(type == Type::Poison, 12_bi, base);
		case Item::Mind_Plate:
		case Item::TwistedSpoon:
		case Item::Odd_Incense:
			return BOUNDED_CONDITIONAL(type == Type::Psychic, 12_bi, base);
		case Item::Stone_Plate:
		case Item::Hard_Stone:
		case Item::Rock_Incense:
			return BOUNDED_CONDITIONAL(type == Type::Rock, 12_bi, base);
		case Item::Iron_Plate:
		case Item::Metal_Coat:
			return BOUNDED_CONDITIONAL(type == Type::Steel, 12_bi, base);
		case Item::Splash_Plate:
		case Item::Mystic_Water:
		case Item::Sea_Incense:
		case Item::Wave_Incense:
			return BOUNDED_CONDITIONAL(type == Type::Water, 12_bi, base);
		case Item::Adamant_Orb:
			return BOUNDED_CONDITIONAL(is_boosted_by_adamant_orb(attacker) and (type == Type::Dragon or type == Type::Steel), 12_bi, base);
		case Item::Griseous_Orb:
			return BOUNDED_CONDITIONAL(is_boosted_by_griseous_orb(attacker) and (type == Type::Dragon or type == Type::Ghost), 12_bi, base);
		case Item::Lustrous_Orb:
			return BOUNDED_CONDITIONAL(is_boosted_by_lustrous_orb(attacker) and (type == Type::Dragon or type == Type::Water), 12_bi, base);
		default:
			return base;
	}
}

auto is_boosted_by_adamant_orb(Species const species) -> bool {
	return species == Species::Dialga;
}

auto is_boosted_by_griseous_orb(Species const species) -> bool {
	return species == Species::Palkia;
}

auto is_boosted_by_lustrous_orb(Species const species) -> bool {
	return species == Species::Giratina_Origin;
}

namespace {
struct InvalidSpecies : std::exception {};
}	// namespace

auto power_of_mass_based_moves(Species const species) -> bounded::integer<20, 120> {
	switch (species) {
		// Generation 1
		case Species::Bulbasaur: return 20_bi;
		case Species::Ivysaur: return 40_bi;
		case Species::Venusaur: return 80_bi;
		case Species::Charmander: return 20_bi;
		case Species::Charmeleon: return 40_bi;
		case Species::Charizard: return 80_bi;
		case Species::Squirtle: return 20_bi;
		case Species::Wartortle: return 40_bi;
		case Species::Blastoise: return 80_bi;
		case Species::Caterpie: return 20_bi;
		case Species::Metapod: return 20_bi;
		case Species::Butterfree: return 60_bi;
		case Species::Weedle: return 20_bi;
		case Species::Kakuna: return 20_bi;
		case Species::Beedrill: return 60_bi;
		case Species::Pidgey: return 20_bi;
		case Species::Pidgeotto: return 60_bi;
		case Species::Pidgeot: return 60_bi;
		case Species::Rattata: return 20_bi;
		case Species::Raticate: return 40_bi;
		case Species::Spearow: return 20_bi;
		case Species::Fearow: return 60_bi;
		case Species::Ekans: return 20_bi;
		case Species::Arbok: return 80_bi;
		case Species::Pikachu: return 20_bi;
		case Species::Raichu: return 60_bi;
		case Species::Sandshrew: return 40_bi;
		case Species::Sandslash: return 60_bi;
		case Species::Nidoran_F: return 20_bi;
		case Species::Nidorina: return 40_bi;
		case Species::Nidoqueen: return 80_bi;
		case Species::Nidoran_M: return 20_bi;
		case Species::Nidorino: return 40_bi;
		case Species::Nidoking: return 80_bi;
		case Species::Clefairy: return 20_bi;
		case Species::Clefable: return 60_bi;
		case Species::Vulpix: return 20_bi;
		case Species::Ninetales: return 40_bi;
		case Species::Jigglypuff: return 20_bi;
		case Species::Wigglytuff: return 40_bi;
		case Species::Zubat: return 20_bi;
		case Species::Golbat: return 80_bi;
		case Species::Oddish: return 20_bi;
		case Species::Gloom: return 20_bi;
		case Species::Vileplume: return 40_bi;
		case Species::Paras: return 20_bi;
		case Species::Parasect: return 60_bi;
		case Species::Venonat: return 60_bi;
		case Species::Venomoth: return 40_bi;
		case Species::Diglett: return 20_bi;
		case Species::Dugtrio: return 60_bi;
		case Species::Meowth: return 20_bi;
		case Species::Persian: return 60_bi;
		case Species::Psyduck: return 40_bi;
		case Species::Golduck: return 80_bi;
		case Species::Mankey: return 60_bi;
		case Species::Primeape: return 60_bi;
		case Species::Growlithe: return 40_bi;
		case Species::Arcanine: return 100_bi;
		case Species::Poliwag: return 40_bi;
		case Species::Poliwhirl: return 40_bi;
		case Species::Poliwrath: return 80_bi;
		case Species::Abra: return 40_bi;
		case Species::Kadabra: return 80_bi;
		case Species::Alakazam: return 60_bi;
		case Species::Machop: return 40_bi;
		case Species::Machoke: return 80_bi;
		case Species::Machamp: return 100_bi;
		case Species::Bellsprout: return 20_bi;
		case Species::Weepinbell: return 20_bi;
		case Species::Victreebel: return 40_bi;
		case Species::Tentacool: return 60_bi;
		case Species::Tentacruel: return 80_bi;
		case Species::Geodude: return 40_bi;
		case Species::Graveler: return 100_bi;
		case Species::Golem: return 120_bi;
		case Species::Ponyta: return 60_bi;
		case Species::Rapidash: return 80_bi;
		case Species::Slowpoke: return 60_bi;
		case Species::Slowbro: return 80_bi;
		case Species::Magnemite: return 20_bi;
		case Species::Magneton: return 80_bi;
		case Species::Farfetchd: return 40_bi;
		case Species::Doduo: return 60_bi;
		case Species::Dodrio: return 80_bi;
		case Species::Seel: return 80_bi;
		case Species::Dewgong: return 100_bi;
		case Species::Grimer: return 60_bi;
		case Species::Muk: return 60_bi;
		case Species::Shellder: return 20_bi;
		case Species::Cloyster: return 100_bi;
		case Species::Gastly: return 20_bi;
		case Species::Haunter: return 20_bi;
		case Species::Gengar: return 60_bi;
		case Species::Onix: return 120_bi;
		case Species::Drowzee: return 60_bi;
		case Species::Hypno: return 80_bi;
		case Species::Krabby: return 20_bi;
		case Species::Kingler: return 80_bi;
		case Species::Voltorb: return 40_bi;
		case Species::Electrode: return 80_bi;
		case Species::Exeggcute: return 20_bi;
		case Species::Exeggutor: return 100_bi;
		case Species::Cubone: return 20_bi;
		case Species::Marowak: return 60_bi;
		case Species::Hitmonlee: return 60_bi;
		case Species::Hitmonchan: return 80_bi;
		case Species::Lickitung: return 80_bi;
		case Species::Koffing: return 20_bi;
		case Species::Weezing: return 20_bi;
		case Species::Rhyhorn: return 100_bi;
		case Species::Rhydon: return 100_bi;
		case Species::Chansey: return 60_bi;
		case Species::Tangela: return 60_bi;
		case Species::Kangaskhan: return 80_bi;
		case Species::Horsea: return 20_bi;
		case Species::Seadra: return 40_bi;
		case Species::Goldeen: return 40_bi;
		case Species::Seaking: return 60_bi;
		case Species::Staryu: return 60_bi;
		case Species::Starmie: return 80_bi;
		case Species::Mr_Mime: return 80_bi;
		case Species::Scyther: return 80_bi;
		case Species::Jynx: return 60_bi;
		case Species::Electabuzz: return 60_bi;
		case Species::Magmar: return 60_bi;
		case Species::Pinsir: return 80_bi;
		case Species::Tauros: return 80_bi;
		case Species::Magikarp: return 20_bi;
		case Species::Gyarados: return 120_bi;
		case Species::Lapras: return 120_bi;
		case Species::Ditto: return 20_bi;
		case Species::Eevee: return 20_bi;
		case Species::Vaporeon: return 60_bi;
		case Species::Jolteon: return 40_bi;
		case Species::Flareon: return 40_bi;
		case Species::Porygon: return 60_bi;
		case Species::Omanyte: return 20_bi;
		case Species::Omastar: return 60_bi;
		case Species::Kabuto: return 40_bi;
		case Species::Kabutops: return 60_bi;
		case Species::Aerodactyl: return 80_bi;
		case Species::Snorlax: return 120_bi;
		case Species::Articuno: return 80_bi;
		case Species::Zapdos: return 80_bi;
		case Species::Moltres: return 80_bi;
		case Species::Dratini: return 20_bi;
		case Species::Dragonair: return 40_bi;
		case Species::Dragonite: return 120_bi;
		case Species::Mewtwo: return 100_bi;
		case Species::Mew: return 20_bi;

		// Generation 2
		case Species::Chikorita: return 20_bi;
		case Species::Bayleef: return 40_bi;
		case Species::Meganium: return 100_bi;
		case Species::Cyndaquil: return 20_bi;
		case Species::Quilava: return 40_bi;
		case Species::Typhlosion: return 80_bi;
		case Species::Totodile: return 20_bi;
		case Species::Croconaw: return 40_bi;
		case Species::Feraligatr: return 80_bi;
		case Species::Sentret: return 20_bi;
		case Species::Furret: return 60_bi;
		case Species::Hoothoot: return 40_bi;
		case Species::Noctowl: return 60_bi;
		case Species::Ledyba: return 40_bi;
		case Species::Ledian: return 60_bi;
		case Species::Spinarak: return 20_bi;
		case Species::Ariados: return 60_bi;
		case Species::Crobat: return 80_bi;
		case Species::Chinchou: return 40_bi;
		case Species::Lanturn: return 40_bi;
		case Species::Pichu: return 20_bi;
		case Species::Cleffa: return 20_bi;
		case Species::Igglybuff: return 20_bi;
		case Species::Togepi: return 20_bi;
		case Species::Togetic: return 20_bi;
		case Species::Natu: return 20_bi;
		case Species::Xatu: return 40_bi;
		case Species::Mareep: return 20_bi;
		case Species::Flaaffy: return 40_bi;
		case Species::Ampharos: return 80_bi;
		case Species::Bellossom: return 20_bi;
		case Species::Marill: return 20_bi;
		case Species::Azumarill: return 60_bi;
		case Species::Sudowoodo: return 60_bi;
		case Species::Politoed: return 60_bi;
		case Species::Hoppip: return 20_bi;
		case Species::Skiploom: return 20_bi;
		case Species::Jumpluff: return 20_bi;
		case Species::Aipom: return 40_bi;
		case Species::Sunkern: return 20_bi;
		case Species::Sunflora: return 20_bi;
		case Species::Yanma: return 60_bi;
		case Species::Wooper: return 20_bi;
		case Species::Quagsire: return 80_bi;
		case Species::Espeon: return 60_bi;
		case Species::Umbreon: return 60_bi;
		case Species::Murkrow: return 20_bi;
		case Species::Slowking: return 80_bi;
		case Species::Misdreavus: return 20_bi;
		case Species::Unown: return 20_bi;
		case Species::Wobbuffet: return 60_bi;
		case Species::Girafarig: return 60_bi;
		case Species::Pineco: return 20_bi;
		case Species::Forretress: return 100_bi;
		case Species::Dunsparce: return 40_bi;
		case Species::Gligar: return 80_bi;
		case Species::Steelix: return 120_bi;
		case Species::Snubbull: return 20_bi;
		case Species::Granbull: return 60_bi;
		case Species::Qwilfish: return 20_bi;
		case Species::Scizor: return 100_bi;
		case Species::Shuckle: return 40_bi;
		case Species::Heracross: return 80_bi;
		case Species::Sneasel: return 60_bi;
		case Species::Teddiursa: return 20_bi;
		case Species::Ursaring: return 100_bi;
		case Species::Slugma: return 60_bi;
		case Species::Magcargo: return 80_bi;
		case Species::Swinub: return 20_bi;
		case Species::Piloswine: return 80_bi;
		case Species::Corsola: return 20_bi;
		case Species::Remoraid: return 40_bi;
		case Species::Octillery: return 60_bi;
		case Species::Delibird: return 40_bi;
		case Species::Mantine: return 120_bi;
		case Species::Skarmory: return 80_bi;
		case Species::Houndour: return 40_bi;
		case Species::Houndoom: return 60_bi;
		case Species::Kingdra: return 100_bi;
		case Species::Phanpy: return 60_bi;
		case Species::Donphan: return 100_bi;
		case Species::Porygon2: return 60_bi;
		case Species::Stantler: return 80_bi;
		case Species::Smeargle: return 80_bi;
		case Species::Tyrogue: return 40_bi;
		case Species::Hitmontop: return 60_bi;
		case Species::Smoochum: return 20_bi;
		case Species::Elekid: return 40_bi;
		case Species::Magby: return 40_bi;
		case Species::Miltank: return 80_bi;
		case Species::Blissey: return 60_bi;
		case Species::Raikou: return 100_bi;
		case Species::Entei: return 100_bi;
		case Species::Suicune: return 100_bi;
		case Species::Larvitar: return 80_bi;
		case Species::Pupitar: return 100_bi;
		case Species::Tyranitar: return 120_bi;
		case Species::Lugia: return 120_bi;
		case Species::Ho_Oh: return 100_bi;
		case Species::Celebi: return 20_bi;

		// Generation 3
		case Species::Treecko: return 20_bi;
		case Species::Grovyle: return 40_bi;
		case Species::Sceptile: return 80_bi;
		case Species::Torchic: return 20_bi;
		case Species::Combusken: return 40_bi;
		case Species::Blaziken: return 80_bi;
		case Species::Mudkip: return 20_bi;
		case Species::Marshtomp: return 60_bi;
		case Species::Swampert: return 80_bi;
		case Species::Poochyena: return 40_bi;
		case Species::Mightyena: return 60_bi;
		case Species::Zigzagoon: return 40_bi;
		case Species::Linoone: return 60_bi;
		case Species::Wurmple: return 20_bi;
		case Species::Silcoon: return 20_bi;
		case Species::Beautifly: return 60_bi;
		case Species::Cascoon: return 40_bi;
		case Species::Dustox: return 60_bi;
		case Species::Lotad: return 20_bi;
		case Species::Lombre: return 60_bi;
		case Species::Ludicolo: return 80_bi;
		case Species::Seedot: return 20_bi;
		case Species::Nuzleaf: return 60_bi;
		case Species::Shiftry: return 80_bi;
		case Species::Taillow: return 20_bi;
		case Species::Swellow: return 40_bi;
		case Species::Wingull: return 20_bi;
		case Species::Pelipper: return 60_bi;
		case Species::Ralts: return 20_bi;
		case Species::Kirlia: return 40_bi;
		case Species::Gardevoir: return 60_bi;
		case Species::Surskit: return 20_bi;
		case Species::Masquerain: return 20_bi;
		case Species::Shroomish: return 20_bi;
		case Species::Breloom: return 60_bi;
		case Species::Slakoth: return 40_bi;
		case Species::Vigoroth: return 60_bi;
		case Species::Slaking: return 100_bi;
		case Species::Nincada: return 20_bi;
		case Species::Ninjask: return 40_bi;
		case Species::Shedinja: return 20_bi;
		case Species::Whismur: return 40_bi;
		case Species::Loudred: return 60_bi;
		case Species::Exploud: return 80_bi;
		case Species::Makuhita: return 80_bi;
		case Species::Hariyama: return 120_bi;
		case Species::Azurill: return 20_bi;
		case Species::Nosepass: return 80_bi;
		case Species::Skitty: return 40_bi;
		case Species::Delcatty: return 60_bi;
		case Species::Sableye: return 40_bi;
		case Species::Mawile: return 40_bi;
		case Species::Aron: return 80_bi;
		case Species::Lairon: return 100_bi;
		case Species::Aggron: return 120_bi;
		case Species::Meditite: return 40_bi;
		case Species::Medicham: return 60_bi;
		case Species::Electrike: return 40_bi;
		case Species::Manectric: return 60_bi;
		case Species::Plusle: return 20_bi;
		case Species::Minun: return 20_bi;
		case Species::Volbeat: return 40_bi;
		case Species::Illumise: return 40_bi;
		case Species::Roselia: return 20_bi;
		case Species::Gulpin: return 40_bi;
		case Species::Swalot: return 80_bi;
		case Species::Carvanha: return 40_bi;
		case Species::Sharpedo: return 80_bi;
		case Species::Wailmer: return 100_bi;
		case Species::Wailord: return 120_bi;
		case Species::Numel: return 40_bi;
		case Species::Camerupt: return 120_bi;
		case Species::Torkoal: return 80_bi;
		case Species::Spoink: return 60_bi;
		case Species::Grumpig: return 80_bi;
		case Species::Spinda: return 20_bi;
		case Species::Trapinch: return 40_bi;
		case Species::Vibrava: return 40_bi;
		case Species::Flygon: return 80_bi;
		case Species::Cacnea: return 80_bi;
		case Species::Cacturne: return 80_bi;
		case Species::Swablu: return 20_bi;
		case Species::Altaria: return 40_bi;
		case Species::Zangoose: return 60_bi;
		case Species::Seviper: return 80_bi;
		case Species::Lunatone: return 100_bi;
		case Species::Solrock: return 100_bi;
		case Species::Barboach: return 20_bi;
		case Species::Whiscash: return 40_bi;
		case Species::Corphish: return 40_bi;
		case Species::Crawdaunt: return 60_bi;
		case Species::Baltoy: return 40_bi;
		case Species::Claydol: return 100_bi;
		case Species::Lileep: return 40_bi;
		case Species::Cradily: return 80_bi;
		case Species::Anorith: return 40_bi;
		case Species::Armaldo: return 80_bi;
		case Species::Feebas: return 20_bi;
		case Species::Milotic: return 100_bi;
		case Species::Castform: return 20_bi;
		case Species::Kecleon: return 40_bi;
		case Species::Shuppet: return 20_bi;
		case Species::Banette: return 40_bi;
		case Species::Duskull: return 40_bi;
		case Species::Dusclops: return 60_bi;
		case Species::Tropius: return 80_bi;
		case Species::Chimecho: return 20_bi;
		case Species::Absol: return 60_bi;
		case Species::Wynaut: return 40_bi;
		case Species::Snorunt: return 40_bi;
		case Species::Glalie: return 120_bi;
		case Species::Spheal: return 60_bi;
		case Species::Sealeo: return 80_bi;
		case Species::Walrein: return 100_bi;
		case Species::Clamperl: return 80_bi;
		case Species::Huntail: return 60_bi;
		case Species::Gorebyss: return 40_bi;
		case Species::Relicanth: return 40_bi;
		case Species::Luvdisc: return 20_bi;
		case Species::Bagon: return 60_bi;
		case Species::Shelgon: return 100_bi;
		case Species::Salamence: return 100_bi;
		case Species::Beldum: return 80_bi;
		case Species::Metang: return 120_bi;
		case Species::Metagross: return 120_bi;
		case Species::Regirock: return 120_bi;
		case Species::Regice: return 100_bi;
		case Species::Registeel: return 120_bi;
		case Species::Latias: return 60_bi;
		case Species::Latios: return 80_bi;
		case Species::Kyogre: return 120_bi;
		case Species::Groudon: return 120_bi;
		case Species::Rayquaza: return 120_bi;
		case Species::Jirachi: return 20_bi;
		case Species::Deoxys_Mediocre: return 80_bi;
		case Species::Deoxys_Attack: return 80_bi;
		case Species::Deoxys_Defense: return 80_bi;
		case Species::Deoxys_Speed: return 80_bi;

		// Generation 4
		case Species::Turtwig: return 40_bi;
		case Species::Grotle: return 80_bi;
		case Species::Torterra: return 120_bi;
		case Species::Chimchar: return 20_bi;
		case Species::Monferno: return 40_bi;
		case Species::Infernape: return 80_bi;
		case Species::Piplup: return 20_bi;
		case Species::Prinplup: return 40_bi;
		case Species::Empoleon: return 80_bi;
		case Species::Starly: return 20_bi;
		case Species::Staravia: return 40_bi;
		case Species::Staraptor: return 40_bi;
		case Species::Bidoof: return 40_bi;
		case Species::Bibarel: return 60_bi;
		case Species::Kricketot: return 20_bi;
		case Species::Kricketune: return 60_bi;
		case Species::Shinx: return 20_bi;
		case Species::Luxio: return 60_bi;
		case Species::Luxray: return 60_bi;
		case Species::Budew: return 20_bi;
		case Species::Roserade: return 40_bi;
		case Species::Cranidos: return 60_bi;
		case Species::Rampardos: return 100_bi;
		case Species::Shieldon: return 80_bi;
		case Species::Bastiodon: return 100_bi;
		case Species::Burmy: return 20_bi;
		case Species::Wormadam_Plant: return 20_bi;
		case Species::Wormadam_Sandy: return 20_bi;
		case Species::Wormadam_Trash: return 20_bi;
		case Species::Mothim: return 40_bi;
		case Species::Combee: return 20_bi;
		case Species::Vespiquen: return 60_bi;
		case Species::Pachirisu: return 20_bi;
		case Species::Buizel: return 60_bi;
		case Species::Floatzel: return 60_bi;
		case Species::Cherubi: return 20_bi;
		case Species::Cherrim: return 20_bi;
		case Species::Shellos: return 20_bi;
		case Species::Gastrodon: return 60_bi;
		case Species::Ambipom: return 40_bi;
		case Species::Drifloon: return 20_bi;
		case Species::Drifblim: return 40_bi;
		case Species::Buneary: return 20_bi;
		case Species::Lopunny: return 60_bi;
		case Species::Mismagius: return 20_bi;
		case Species::Honchkrow: return 60_bi;
		case Species::Glameow: return 20_bi;
		case Species::Purugly: return 60_bi;
		case Species::Chingling: return 20_bi;
		case Species::Stunky: return 40_bi;
		case Species::Skuntank: return 60_bi;
		case Species::Bronzor: return 80_bi;
		case Species::Bronzong: return 100_bi;
		case Species::Bonsly: return 40_bi;
		case Species::Mime_Jr: return 40_bi;
		case Species::Happiny: return 40_bi;
		case Species::Chatot: return 20_bi;
		case Species::Spiritomb: return 100_bi;
		case Species::Gible: return 40_bi;
		case Species::Gabite: return 80_bi;
		case Species::Garchomp: return 80_bi;
		case Species::Munchlax: return 100_bi;
		case Species::Riolu: return 40_bi;
		case Species::Lucario: return 80_bi;
		case Species::Hippopotas: return 60_bi;
		case Species::Hippowdon: return 120_bi;
		case Species::Skorupi: return 40_bi;
		case Species::Drapion: return 80_bi;
		case Species::Croagunk: return 40_bi;
		case Species::Toxicroak: return 60_bi;
		case Species::Carnivine: return 60_bi;
		case Species::Finneon: return 20_bi;
		case Species::Lumineon: return 40_bi;
		case Species::Mantyke: return 80_bi;
		case Species::Snover: return 80_bi;
		case Species::Abomasnow: return 100_bi;
		case Species::Weavile: return 60_bi;
		case Species::Magnezone: return 100_bi;
		case Species::Lickilicky: return 100_bi;
		case Species::Rhyperior: return 120_bi;
		case Species::Tangrowth: return 100_bi;
		case Species::Electivire: return 100_bi;
		case Species::Magmortar: return 80_bi;
		case Species::Togekiss: return 60_bi;
		case Species::Yanmega: return 80_bi;
		case Species::Leafeon: return 60_bi;
		case Species::Glaceon: return 60_bi;
		case Species::Gliscor: return 60_bi;
		case Species::Mamoswine: return 120_bi;
		case Species::Porygon_Z: return 60_bi;
		case Species::Gallade: return 80_bi;
		case Species::Probopass: return 120_bi;
		case Species::Dusknoir: return 100_bi;
		case Species::Froslass: return 60_bi;
		case Species::Rotom: return 20_bi;
		case Species::Rotom_Heat: return 20_bi;
		case Species::Rotom_Wash: return 20_bi;
		case Species::Rotom_Frost: return 20_bi;
		case Species::Rotom_Fan: return 20_bi;
		case Species::Rotom_Mow: return 20_bi;
		case Species::Uxie: return 20_bi;
		case Species::Mesprit: return 20_bi;
		case Species::Azelf: return 20_bi;
		case Species::Dialga: return 120_bi;
		case Species::Palkia: return 120_bi;
		case Species::Heatran: return 120_bi;
		case Species::Regigigas: return 120_bi;
		case Species::Giratina_Altered: return 120_bi;
		case Species::Giratina_Origin: return 120_bi;
		case Species::Cresselia: return 80_bi;
		case Species::Phione: return 20_bi;
		case Species::Manaphy: return 20_bi;
		case Species::Darkrai: return 80_bi;
		case Species::Shaymin_Land: return 20_bi;
		case Species::Shaymin_Sky: return 20_bi;
		case Species::Arceus: return 120_bi;

		// Generation 5
		case Species::Victini: return 20_bi;
		case Species::Snivy: return 20_bi;
		case Species::Servine: return 40_bi;
		case Species::Serperior: return 80_bi;
		case Species::Tepig: return 20_bi;
		case Species::Pignite: return 80_bi;
		case Species::Emboar: return 100_bi;
		case Species::Oshawott: return 20_bi;
		case Species::Dewott: return 40_bi;
		case Species::Samurott: return 80_bi;
		case Species::Patrat: return 40_bi;
		case Species::Watchog: return 60_bi;
		case Species::Lillipup: return 20_bi;
		case Species::Herdier: return 40_bi;
		case Species::Stoutland: return 80_bi;
		case Species::Purrloin: return 40_bi;
		case Species::Liepard: return 60_bi;
		case Species::Pansage: return 40_bi;
		case Species::Simisage: return 60_bi;
		case Species::Pansear: return 40_bi;
		case Species::Simisear: return 60_bi;
		case Species::Panpour: return 40_bi;
		case Species::Simipour: return 60_bi;
		case Species::Munna: return 40_bi;
		case Species::Musharna: return 80_bi;
		case Species::Pidove: return 20_bi;
		case Species::Tranquill: return 40_bi;
		case Species::Unfezant: return 60_bi;
		case Species::Blitzle: return 60_bi;
		case Species::Zebstrika: return 80_bi;
		case Species::Roggenrola: return 40_bi;
		case Species::Boldore: return 100_bi;
		case Species::Gigalith: return 120_bi;
		case Species::Woobat: return 20_bi;
		case Species::Swoobat: return 40_bi;
		case Species::Drilbur: return 20_bi;
		case Species::Excadrill: return 60_bi;
		case Species::Audino: return 60_bi;
		case Species::Timburr: return 40_bi;
		case Species::Gurdurr: return 60_bi;
		case Species::Conkeldurr: return 80_bi;
		case Species::Tympole: return 20_bi;
		case Species::Palpitoad: return 40_bi;
		case Species::Seismitoad: return 80_bi;
		case Species::Throh: return 80_bi;
		case Species::Sawk: return 80_bi;
		case Species::Sewaddle: return 20_bi;
		case Species::Swadloon: return 20_bi;
		case Species::Leavanny: return 40_bi;
		case Species::Venipede: return 20_bi;
		case Species::Whirlipede: return 80_bi;
		case Species::Scolipede: return 120_bi;
		case Species::Cottonee: return 20_bi;
		case Species::Whimsicott: return 20_bi;
		case Species::Petilil: return 20_bi;
		case Species::Lilligant: return 40_bi;
		case Species::Basculin_Red: return 40_bi;
		case Species::Basculin_Blue: return 40_bi;
		case Species::Sandile: return 40_bi;
		case Species::Krokorok: return 60_bi;
		case Species::Krookodile: return 80_bi;
		case Species::Darumaka: return 60_bi;
		case Species::Darmanitan: return 80_bi;
		case Species::Maractus: return 60_bi;
		case Species::Dwebble: return 40_bi;
		case Species::Crustle: return 100_bi;
		case Species::Scraggy: return 40_bi;
		case Species::Scrafty: return 60_bi;
		case Species::Sigilyph: return 40_bi;
		case Species::Yamask: return 20_bi;
		case Species::Cofagrigus: return 80_bi;
		case Species::Tirtouga: return 40_bi;
		case Species::Carracosta: return 80_bi;
		case Species::Archen: return 20_bi;
		case Species::Archeops: return 60_bi;
		case Species::Trubbish: return 60_bi;
		case Species::Garbodor: return 100_bi;
		case Species::Zorua: return 40_bi;
		case Species::Zoroark: return 80_bi;
		case Species::Minccino: return 20_bi;
		case Species::Cinccino: return 20_bi;
		case Species::Gothita: return 20_bi;
		case Species::Gothorita: return 40_bi;
		case Species::Gothitelle: return 60_bi;
		case Species::Solosis: return 20_bi;
		case Species::Duosion: return 20_bi;
		case Species::Reuniclus: return 40_bi;
		case Species::Ducklett: return 20_bi;
		case Species::Swanna: return 40_bi;
		case Species::Vanillite: return 20_bi;
		case Species::Vanillish: return 60_bi;
		case Species::Vanilluxe: return 80_bi;
		case Species::Deerling: return 40_bi;
		case Species::Sawsbuck: return 80_bi;
		case Species::Emolga: return 20_bi;
		case Species::Karrablast: return 20_bi;
		case Species::Escavalier: return 60_bi;
		case Species::Foongus: return 20_bi;
		case Species::Amoonguss: return 40_bi;
		case Species::Frillish: return 60_bi;
		case Species::Jellicent: return 100_bi;
		case Species::Alomomola: return 60_bi;
		case Species::Joltik: return 20_bi;
		case Species::Galvantula: return 40_bi;
		case Species::Ferroseed: return 40_bi;
		case Species::Ferrothorn: return 100_bi;
		case Species::Klink: return 40_bi;
		case Species::Klang: return 80_bi;
		case Species::Klinklang: return 80_bi;
		case Species::Tynamo: return 20_bi;
		case Species::Eelektrik: return 40_bi;
		case Species::Eelektross: return 80_bi;
		case Species::Elgyem: return 20_bi;
		case Species::Beheeyem: return 60_bi;
		case Species::Litwick: return 20_bi;
		case Species::Lampent: return 40_bi;
		case Species::Chandelure: return 60_bi;
		case Species::Axew: return 40_bi;
		case Species::Fraxure: return 60_bi;
		case Species::Haxorus: return 100_bi;
		case Species::Cubchoo: return 20_bi;
		case Species::Beartic: return 120_bi;
		case Species::Cryogonal: return 100_bi;
		case Species::Shelmet: return 20_bi;
		case Species::Accelgor: return 60_bi;
		case Species::Stunfisk: return 40_bi;
		case Species::Mienfoo: return 40_bi;
		case Species::Mienshao: return 60_bi;
		case Species::Druddigon: return 100_bi;
		case Species::Golett: return 80_bi;
		case Species::Golurk: return 120_bi;
		case Species::Pawniard: return 40_bi;
		case Species::Bisharp: return 80_bi;
		case Species::Bouffalant: return 80_bi;
		case Species::Rufflet: return 40_bi;
		case Species::Braviary: return 60_bi;
		case Species::Vullaby: return 20_bi;
		case Species::Mandibuzz: return 60_bi;
		case Species::Heatmor: return 80_bi;
		case Species::Durant: return 60_bi;
		case Species::Deino: return 40_bi;
		case Species::Zweilous: return 60_bi;
		case Species::Hydreigon: return 100_bi;
		case Species::Larvesta: return 60_bi;
		case Species::Volcarona: return 60_bi;
		case Species::Cobalion: return 120_bi;
		case Species::Terrakion: return 120_bi;
		case Species::Virizion: return 100_bi;
		case Species::Tornadus_Incarnate: return 80_bi;
		case Species::Tornadus_Therian: return 80_bi;
		case Species::Thundurus_Incarnate: return 80_bi;
		case Species::Thundurus_Therian: return 80_bi;
		case Species::Reshiram: return 120_bi;
		case Species::Zekrom: return 120_bi;
		case Species::Landorus_Incarnate: return 80_bi;
		case Species::Landorus_Therian: return 80_bi;
		case Species::Kyurem: return 120_bi;
		case Species::Kyurem_Black: return 120_bi;
		case Species::Kyurem_White: return 120_bi;
		case Species::Keldeo: return 60_bi;
		case Species::Meloetta: return 20_bi;
		case Species::Genesect: return 80_bi;
		default: throw InvalidSpecies{};
	}
}

}	// namespace
}	// namespace technicalmachine
