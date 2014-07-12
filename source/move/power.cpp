// Move power calculator
// Copyright (C) 2014 David Stone
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

#include <algorithm>
#include <cassert>
#include <numeric>

#include <bounded_integer/array.hpp>

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
			return attacker.fury_cutter_power();
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
			return attacker.momentum_move_power();
		case Moves::Hidden_Power: {
			using stat_and_position_type = std::pair<StatNames, bounded::integer<0, 5>>;
			static constexpr bounded::array<stat_and_position_type, 5> stat_and_position {{
				{ StatNames::ATK, 1_bi },
				{ StatNames::DEF, 2_bi },
				{ StatNames::SPE, 3_bi },
				{ StatNames::SPA, 4_bi },
				{ StatNames::SPD, 5_bi }
			}};
			using intermediate_type = bounded::checked_integer<0, (1 << 6) - 1>;
			auto const sum = [&](intermediate_type value, stat_and_position_type const & stat) {
				return value + (((get_stat(attacker, stat.first).iv.value() / 2_bi) % 2_bi) << stat.second);
			};
			intermediate_type const initial = (get_hp(attacker).iv.value() / 2_bi) % 2_bi;
			auto const result = std::accumulate(std::begin(stat_and_position), std::end(stat_and_position), initial, sum) * 40_bi / 63_bi + 30_bi;
			static_assert(std::numeric_limits<decltype(result)>::min() == 30_bi, "Incorrect Hidden Power minimum.");
			static_assert(std::numeric_limits<decltype(result)>::max() == 70_bi, "Incorrect Hidden Power maximum.");
			return result;
		}
		case Moves::Magnitude:
			return variable.value();
		case Moves::Natural_Gift:
			return berry_power(get_item(attacker));
		case Moves::Present:
			assert (!variable.present_heals());
			return variable.value();
		case Moves::Punishment: {
			auto const uncapped_power = 60_bi + 20_bi * positive_boosts(defender.stage());
			return bounded::min(uncapped_power, 200_bi);
		}
		case Moves::Return:
			return return_power(get_happiness(attacker));
		case Moves::Spit_Up:
			return attacker.spit_up_power();
		case Moves::Triple_Kick:
			return attacker.triple_kick_power();
		case Moves::Trump_Card:
			return current_move(attacker).pp().trump_card_power();
		default:
			return *base_power(current_move(attacker));
	}
}

auto doubling(ActivePokemon const & attacker, ActivePokemon const & defender, Weather weather) -> bool;

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
	return static_cast<bounded::equivalent_type<MovePower, bounded::throw_policy>>(bounded::max(1_bi,
		base_power *
		BOUNDED_CONDITIONAL(doubling(attacker, defender, weather), 2_bi, 1_bi) *
		item_modifier(attacker) *
		BOUNDED_CONDITIONAL(attacker.charge_boosted(), 2_bi, 1_bi) /
		BOUNDED_CONDITIONAL(defender.sport_is_active(current_move(attacker)), 2_bi, 1_bi) *
		attacker_ability_power_modifier(attacker, defender, base_power) *
		defender_ability_modifier(attacker, get_ability(defender))
	));
}

namespace {

auto doubling(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const weather) -> bool {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	Moves const move = current_move(attacker);
	if (defender.vanish_doubles_power(move))
		return true;
	switch (move) {
		case Moves::Assurance:
			return static_cast<bool>(defender.damaged());
		case Moves::Avalanche: 
		case Moves::Revenge:
			return static_cast<bool>(attacker.damaged());
		case Moves::Brine:
			return get_hp(defender).current() <= get_hp(defender).max() / 2_bi;
		case Moves::Facade:
			return boosts_facade(get_status(attacker));
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.defense_curled();
		case Moves::Payback:
			return defender.moved();
		case Moves::SmellingSalt:
			return boosts_smellingsalt(get_status(defender));
		case Moves::SolarBeam:
			return !weather.rain();
		case Moves::Stomp:
			return defender.minimized();
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

auto power_of_mass_based_moves(Species const species) -> bounded::integer<20, 120> {
	static constexpr auto mass_array = bounded::make_array(
		// Generation 1
		20_bi,		// Bulbasaur
		40_bi,		// Ivysaur
		80_bi,		// Venusaur
		20_bi,		// Charmander
		40_bi,		// Charmeleon
		80_bi,		// Charizard
		20_bi,		// Squirtle
		40_bi,		// Wartortle
		80_bi,		// Blastoise
		20_bi,		// Caterpie
		20_bi,		// Metapod
		60_bi,		// Butterfree
		20_bi,		// Weedle
		20_bi,		// Kakuna
		60_bi,		// Beedrill
		20_bi,		// Pidgey
		60_bi,		// Pidgeotto
		60_bi,		// Pidgeot
		20_bi,		// Rattata
		40_bi,		// Raticate
		20_bi,		// Spearow
		60_bi,		// Fearow
		20_bi,		// Ekans
		80_bi,		// Arbok
		20_bi,		// Pikachu
		60_bi,		// Raichu
		40_bi,		// Sandshrew
		60_bi,		// Sandslash
		20_bi,		// Nidoran-F
		40_bi,		// Nidorina
		80_bi,		// Nidoqueen
		20_bi,		// Nidoran-M
		40_bi,		// Nidorino
		80_bi,		// Nidoking
		20_bi,		// Clefairy
		60_bi,		// Clefable
		20_bi,		// Vulpix
		40_bi,		// Ninetales
		20_bi,		// Jigglypuff
		40_bi,		// Wigglytuff
		20_bi,		// Zubat
		80_bi,		// Golbat
		20_bi,		// Oddish
		20_bi,		// Gloom
		40_bi,		// Vileplume
		20_bi,		// Paras
		60_bi,		// Parasect
		60_bi,		// Venonat
		40_bi,		// Venomoth
		20_bi,		// Diglett
		60_bi,		// Dugtrio
		20_bi,		// Meowth
		60_bi,		// Persian
		40_bi,		// Psyduck
		80_bi,		// Golduck
		60_bi,		// Mankey
		60_bi,		// Primeape
		40_bi,		// Growlithe
		100_bi,		// Arcanine
		40_bi,		// Poliwag
		40_bi,		// Poliwhirl
		80_bi,		// Poliwrath
		40_bi,		// Abra
		80_bi,		// Kadabra
		60_bi,		// Alakazam
		40_bi,		// Machop
		80_bi,		// Machoke
		100_bi,		// Machamp
		20_bi,		// Bellsprout
		20_bi,		// Weepinbell
		40_bi,		// Victreebel
		60_bi,		// Tentacool
		80_bi,		// Tentacruel
		40_bi,		// Geodude
		100_bi,		// Graveler
		120_bi,		// Golem
		60_bi,		// Ponyta
		80_bi,		// Rapidash
		60_bi,		// Slowpoke
		80_bi,		// Slowbro
		20_bi,		// Magnemite
		80_bi,		// Magneton
		40_bi,		// Farfetch'd
		60_bi,		// Doduo
		80_bi,		// Dodrio
		80_bi,		// Seel
		100_bi,		// Dewgong
		60_bi,		// Grimer
		60_bi,		// Muk
		20_bi,		// Shellder
		100_bi,		// Cloyster
		20_bi,		// Gastly
		20_bi,		// Haunter
		60_bi,		// Gengar
		120_bi,		// Onix
		60_bi,		// Drowzee
		80_bi,		// Hypno
		20_bi,		// Krabby
		80_bi,		// Kingler
		40_bi,		// Voltorb
		80_bi,		// Electrode
		20_bi,		// Exeggcute
		100_bi,		// Exeggutor
		20_bi,		// Cubone
		60_bi,		// Marowak
		60_bi,		// Hitmonlee
		80_bi,		// Hitmonchan
		80_bi,		// Lickitung
		20_bi,		// Koffing
		20_bi,		// Weezing
		100_bi,		// Rhyhorn
		100_bi,		// Rhydon
		60_bi,		// Chansey
		60_bi,		// Tangela
		80_bi,		// Kangaskhan
		20_bi,		// Horsea
		40_bi,		// Seadra
		40_bi,		// Goldeen
		60_bi,		// Seaking
		60_bi,		// Staryu
		80_bi,		// Starmie
		80_bi,		// Mr. Mime
		80_bi,		// Scyther
		60_bi,		// Jynx
		60_bi,		// Electabuzz
		60_bi,		// Magmar
		80_bi,		// Pinsir
		80_bi,		// Tauros
		20_bi,		// Magikarp
		120_bi,		// Gyarados
		120_bi,		// Lapras
		20_bi,		// Ditto
		20_bi,		// Eevee
		60_bi,		// Vaporeon
		40_bi,		// Jolteon
		40_bi,		// Flareon
		60_bi,		// Porygon
		20_bi,		// Omanyte
		60_bi,		// Omastar
		40_bi,		// Kabuto
		60_bi,		// Kabutops
		80_bi,		// Aerodactyl
		120_bi,		// Snorlax
		80_bi,		// Articuno
		80_bi,		// Zapdos
		80_bi,		// Moltres
		20_bi,		// Dratini
		40_bi,		// Dragonair
		120_bi,		// Dragonite
		100_bi,		// Mewtwo
		20_bi,		// Mew

		// Generation 2
		20_bi,		// Chikorita
		40_bi,		// Bayleef
		100_bi,		// Meganium
		20_bi,		// Cyndaquil
		40_bi,		// Quilava
		80_bi,		// Typhlosion
		20_bi,		// Totodile
		40_bi,		// Croconaw
		80_bi,		// Feraligatr
		20_bi,		// Sentret
		60_bi,		// Furret
		40_bi,		// Hoothoot
		60_bi,		// Noctowl
		40_bi,		// Ledyba
		60_bi,		// Ledian
		20_bi,		// Spinarak
		60_bi,		// Ariados
		80_bi,		// Crobat
		40_bi,		// Chinchou
		40_bi,		// Lanturn
		20_bi,		// Pichu
		20_bi,		// Cleffa
		20_bi,		// Igglybuff
		20_bi,		// Togepi
		20_bi,		// Togetic
		20_bi,		// Natu
		40_bi,		// Xatu
		20_bi,		// Mareep
		40_bi,		// Flaaffy
		80_bi,		// Ampharos
		20_bi,		// Bellossom
		20_bi,		// Marill
		60_bi,		// Azumarill
		60_bi,		// Sudowoodo
		60_bi,		// Politoed
		20_bi,		// Hoppip
		20_bi,		// Skiploom
		20_bi,		// Jumpluff
		40_bi,		// Aipom
		20_bi,		// Sunkern
		20_bi,		// Sunflora
		60_bi,		// Yanma
		20_bi,		// Wooper
		80_bi,		// Quagsire
		60_bi,		// Espeon
		60_bi,		// Umbreon
		20_bi,		// Murkrow
		80_bi,		// Slowking
		20_bi,		// Misdreavus
		20_bi,		// Unown
		60_bi,		// Wobbuffet
		60_bi,		// Girafarig
		20_bi,		// Pineco
		100_bi,		// Forretress
		40_bi,		// Dunsparce
		80_bi,		// Gligar
		120_bi,		// Steelix
		20_bi,		// Snubbull
		60_bi,		// Granbull
		20_bi,		// Qwilfish
		100_bi,		// Scizor
		40_bi,		// Shuckle
		80_bi,		// Heracross
		60_bi,		// Sneasel
		20_bi,		// Teddiursa
		100_bi,		// Ursaring
		60_bi,		// Slugma
		80_bi,		// Magcargo
		20_bi,		// Swinub
		80_bi,		// Piloswine
		20_bi,		// Corsola
		40_bi,		// Remoraid
		60_bi,		// Octillery
		40_bi,		// Delibird
		120_bi,		// Mantine
		80_bi,		// Skarmory
		40_bi,		// Houndour
		60_bi,		// Houndoom
		100_bi,		// Kingdra
		60_bi,		// Phanpy
		100_bi,		// Donphan
		60_bi,		// Porygon2
		80_bi,		// Stantler
		80_bi,		// Smeargle
		40_bi,		// Tyrogue
		60_bi,		// Hitmontop
		20_bi,		// Smoochum
		40_bi,		// Elekid
		40_bi,		// Magby
		80_bi,		// Miltank
		60_bi,		// Blissey
		100_bi,		// Raikou
		100_bi,		// Entei
		100_bi,		// Suicune
		80_bi,		// Larvitar
		100_bi,		// Pupitar
		120_bi,		// Tyranitar
		120_bi,		// Lugia
		100_bi,		// Ho-Oh
		20_bi,		// Celebi

		// Generation 3
		20_bi,		// Treecko
		40_bi,		// Grovyle
		80_bi,		// Sceptile
		20_bi,		// Torchic
		40_bi,		// Combusken
		80_bi,		// Blaziken
		20_bi,		// Mudkip
		60_bi,		// Marshtomp
		80_bi,		// Swampert
		40_bi,		// Poochyena
		60_bi,		// Mightyena
		40_bi,		// Zigzagoon
		60_bi,		// Linoone
		20_bi,		// Wurmple
		20_bi,		// Silcoon
		60_bi,		// Beautifly
		40_bi,		// Cascoon
		60_bi,		// Dustox
		20_bi,		// Lotad
		60_bi,		// Lombre
		80_bi,		// Ludicolo
		20_bi,		// Seedot
		60_bi,		// Nuzleaf
		80_bi,		// Shiftry
		20_bi,		// Taillow
		40_bi,		// Swellow
		20_bi,		// Wingull
		60_bi,		// Pelipper
		20_bi,		// Ralts
		40_bi,		// Kirlia
		60_bi,		// Gardevoir
		20_bi,		// Surskit
		20_bi,		// Masquerain
		20_bi,		// Shroomish
		60_bi,		// Breloom
		40_bi,		// Slakoth
		60_bi,		// Vigoroth
		100_bi,		// Slaking
		20_bi,		// Nincada
		40_bi,		// Ninjask
		20_bi,		// Shedinja
		40_bi,		// Whismur
		60_bi,		// Loudred
		80_bi,		// Exploud
		80_bi,		// Makuhita
		120_bi,		// Hariyama
		20_bi,		// Azurill
		80_bi,		// Nosepass
		40_bi,		// Skitty
		60_bi,		// Delcatty
		40_bi,		// Sableye
		40_bi,		// Mawile
		80_bi,		// Aron
		100_bi,		// Lairon
		120_bi,		// Aggron
		40_bi,		// Meditite
		60_bi,		// Medicham
		40_bi,		// Electrike
		60_bi,		// Manectric
		20_bi,		// Plusle
		20_bi,		// Minun
		40_bi,		// Volbeat
		40_bi,		// Illumise
		20_bi,		// Roselia
		40_bi,		// Gulpin
		80_bi,		// Swalot
		40_bi,		// Carvanha
		80_bi,		// Sharpedo
		100_bi,		// Wailmer
		120_bi,		// Wailord
		40_bi,		// Numel
		120_bi,		// Camerupt
		80_bi,		// Torkoal
		60_bi,		// Spoink
		80_bi,		// Grumpig
		20_bi,		// Spinda
		40_bi,		// Trapinch
		40_bi,		// Vibrava
		80_bi,		// Flygon
		80_bi,		// Cacnea
		80_bi,		// Cacturne
		20_bi,		// Swablu
		40_bi,		// Altaria
		60_bi,		// Zangoose
		80_bi,		// Seviper
		100_bi,		// Lunatone
		100_bi,		// Solrock
		20_bi,		// Barboach
		40_bi,		// Whiscash
		40_bi,		// Corphish
		60_bi,		// Crawdaunt
		40_bi,		// Baltoy
		100_bi,		// Claydol
		40_bi,		// Lileep
		80_bi,		// Cradily
		40_bi,		// Anorith
		80_bi,		// Armaldo
		20_bi,		// Feebas
		100_bi,		// Milotic
		20_bi,		// Castform
		40_bi,		// Kecleon
		20_bi,		// Shuppet
		40_bi,		// Banette
		40_bi,		// Duskull
		60_bi,		// Dusclops
		80_bi,		// Tropius
		20_bi,		// Chimecho
		60_bi,		// Absol
		40_bi,		// Wynaut
		40_bi,		// Snorunt
		120_bi,		// Glalie
		60_bi,		// Spheal
		80_bi,		// Sealeo
		100_bi,		// Walrein
		80_bi,		// Clamperl
		60_bi,		// Huntail
		40_bi,		// Gorebyss
		40_bi,		// Relicanth
		20_bi,		// Luvdisc
		60_bi,		// Bagon
		100_bi,		// Shelgon
		100_bi,		// Salamence
		80_bi,		// Beldum
		120_bi,		// Metang
		120_bi,		// Metagross
		120_bi,		// Regirock
		100_bi,		// Regice
		120_bi,		// Registeel
		60_bi,		// Latias
		80_bi,		// Latios
		120_bi,		// Kyogre
		120_bi,		// Groudon
		120_bi,		// Rayquaza
		20_bi,		// Jirachi
		80_bi,		// Deoxys-Mediocre
		80_bi,		// Deoxys-Attack
		80_bi,		// Deoxys-Defense
		80_bi,		// Deoxys-Speed

		// Generation 4
		40_bi,		// Turtwig
		80_bi,		// Grotle
		120_bi,		// Torterra
		20_bi,		// Chimchar
		40_bi,		// Monferno
		80_bi,		// Infernape
		20_bi,		// Piplup
		40_bi,		// Prinplup
		80_bi,		// Empoleon
		20_bi,		// Starly
		40_bi,		// Staravia
		40_bi,		// Staraptor
		40_bi,		// Bidoof
		60_bi,		// Bibarel
		20_bi,		// Kricketot
		60_bi,		// Kricketune
		20_bi,		// Shinx
		60_bi,		// Luxio
		60_bi,		// Luxray
		20_bi,		// Budew
		40_bi,		// Roserade
		60_bi,		// Cranidos
		100_bi,		// Rampardos
		80_bi,		// Shieldon
		100_bi,		// Bastiodon
		20_bi,		// Burmy
		20_bi,		// Wormadam-Plant
		20_bi,		// Wormadam-Sandy
		20_bi,		// Wormadam-Trash
		40_bi,		// Mothim
		20_bi,		// Combee
		60_bi,		// Vespiquen
		20_bi,		// Pachirisu
		60_bi,		// Buizel
		60_bi,		// Floatzel
		20_bi,		// Cherubi
		20_bi,		// Cherrim
		20_bi,		// Shellos
		60_bi,		// Gastrodon
		40_bi,		// Ambipom
		20_bi,		// Drifloon
		40_bi,		// Drifblim
		20_bi,		// Buneary
		60_bi,		// Lopunny
		20_bi,		// Mismagius
		60_bi,		// Honchkrow
		20_bi,		// Glameow
		60_bi,		// Purugly
		20_bi,		// Chingling
		40_bi,		// Stunky
		60_bi,		// Skuntank
		80_bi,		// Bronzor
		100_bi,		// Bronzong
		40_bi,		// Bonsly
		40_bi,		// Mime Jr.
		40_bi,		// Happiny
		20_bi,		// Chatot
		100_bi,		// Spiritomb
		40_bi,		// Gible
		80_bi,		// Gabite
		80_bi,		// Garchomp
		100_bi,		// Munchlax
		40_bi,		// Riolu
		80_bi,		// Lucario
		60_bi,		// Hippopotas
		120_bi,		// Hippowdon
		40_bi,		// Skorupi
		80_bi,		// Drapion
		40_bi,		// Croagunk
		60_bi,		// Toxicroak
		60_bi,		// Carnivine
		20_bi,		// Finneon
		40_bi,		// Lumineon
		80_bi,		// Mantyke
		80_bi,		// Snover
		100_bi,		// Abomasnow
		60_bi,		// Weavile
		100_bi,		// Magnezone
		100_bi,		// Lickilicky
		120_bi,		// Rhyperior
		100_bi,		// Tangrowth
		100_bi,		// Electivire
		80_bi,		// Magmortar
		60_bi,		// Togekiss
		80_bi,		// Yanmega
		60_bi,		// Leafeon
		60_bi,		// Glaceon
		60_bi,		// Gliscor
		120_bi,		// Mamoswine
		60_bi,		// Porygon-Z
		80_bi,		// Gallade
		120_bi,		// Probopass
		100_bi,		// Dusknoir
		60_bi,		// Froslass
		20_bi,		// Rotom
		20_bi,		// Rotom-Heat
		20_bi,		// Rotom-Wash
		20_bi,		// Rotom-Frost
		20_bi,		// Rotom-Fan
		20_bi,		// Rotom-Mow
		20_bi,		// Uxie
		20_bi,		// Mesprit
		20_bi,		// Azelf
		120_bi,		// Dialga
		120_bi,		// Palkia
		120_bi,		// Heatran
		120_bi,		// Regigigas
		120_bi,		// Giratina-Altered
		120_bi,		// Giratina-Origin
		80_bi,		// Cresselia
		20_bi,		// Phione
		20_bi,		// Manaphy
		80_bi,		// Darkrai
		20_bi,		// Shaymin-Land
		20_bi,		// Shaymin-Sky
		120_bi,		// Arceus

		// Generation 5
		20_bi,		// Victini
		20_bi,		// Snivy
		40_bi,		// Servine
		80_bi,		// Serperior
		20_bi,		// Tepig
		80_bi,		// Pignite
		100_bi,		// Emboar
		20_bi,		// Oshawott
		40_bi,		// Dewott
		80_bi,		// Samurott
		40_bi,		// Patrat
		60_bi,		// Watchog
		20_bi,		// Lillipup
		40_bi,		// Herdier
		80_bi,		// Stoutland
		40_bi,		// Purrloin
		60_bi,		// Liepard
		40_bi,		// Pansage
		60_bi,		// Simisage
		40_bi,		// Pansear
		60_bi,		// Simisear
		40_bi,		// Panpour
		60_bi,		// Simipour
		40_bi,		// Munna
		80_bi,		// Musharna
		20_bi,		// Pidove
		40_bi,		// Tranquill
		60_bi,		// Unfezant
		60_bi,		// Blitzle
		80_bi,		// Zebstrika
		40_bi,		// Roggenrola
		100_bi,		// Boldore
		120_bi,		// Gigalith
		20_bi,		// Woobat
		40_bi,		// Swoobat
		20_bi,		// Drilbur
		60_bi,		// Excadrill
		60_bi,		// Audino
		40_bi,		// Timburr
		60_bi,		// Gurdurr
		80_bi,		// Conkeldurr
		20_bi,		// Tympole
		40_bi,		// Palpitoad
		80_bi,		// Seismitoad
		80_bi,		// Throh
		80_bi,		// Sawk
		20_bi,		// Sewaddle
		20_bi,		// Swadloon
		40_bi,		// Leavanny
		20_bi,		// Venipede
		80_bi,		// Whirlipede
		120_bi,		// Scolipede
		20_bi,		// Cottonee
		20_bi,		// Whimsicott
		20_bi,		// Petilil
		40_bi,		// Lilligant
		40_bi,		// Basculin-R
		40_bi,		// Basculin-B
		40_bi,		// Sandile
		60_bi,		// Krokorok
		80_bi,		// Krookodile
		60_bi,		// Darumaka
		80_bi,		// Darmanitan
		60_bi,		// Maractus
		40_bi,		// Dwebble
		100_bi,		// Crustle
		40_bi,		// Scraggy
		60_bi,		// Scrafty
		40_bi,		// Sigilyph
		20_bi,		// Yamask
		80_bi,		// Cofagrigus
		40_bi,		// Tirtouga
		80_bi,		// Carracosta
		20_bi,		// Archen
		60_bi,		// Archeops
		60_bi,		// Trubbish
		100_bi,		// Garbodor
		40_bi,		// Zorua
		80_bi,		// Zoroark
		20_bi,		// Minccino
		20_bi,		// Cinccino
		20_bi,		// Gothita
		40_bi,		// Gothorita
		60_bi,		// Gothitelle
		20_bi,		// Solosis
		20_bi,		// Duosion
		40_bi,		// Reuniclus
		20_bi,		// Ducklett
		40_bi,		// Swanna
		20_bi,		// Vanillite
		60_bi,		// Vanillish
		80_bi,		// Vanilluxe
		40_bi,		// Deerling
		80_bi,		// Sawsbuck
		20_bi,		// Emolga
		20_bi,		// Karrablast
		60_bi,		// Escavalier
		20_bi,		// Foongus
		40_bi,		// Amoonguss
		60_bi,		// Frillish
		100_bi,		// Jellicent
		60_bi,		// Alomomola
		20_bi,		// Joltik
		40_bi,		// Galvantula
		40_bi,		// Ferroseed
		100_bi,		// Ferrothorn
		40_bi,		// Klink
		80_bi,		// Klang
		80_bi,		// Klinklang
		20_bi,		// Tynamo
		40_bi,		// Eelektrik
		80_bi,		// Eelektross
		20_bi,		// Elgyem
		60_bi,		// Beheeyem
		20_bi,		// Litwick
		40_bi,		// Lampent
		60_bi,		// Chandelure
		40_bi,		// Axew
		60_bi,		// Fraxure
		100_bi,		// Haxorus
		20_bi,		// Cubchoo
		120_bi,		// Beartic
		100_bi,		// Cryogonal
		20_bi,		// Shelmet
		60_bi,		// Accelgor
		40_bi,		// Stunfisk
		40_bi,		// Mienfoo
		60_bi,		// Mienshao
		100_bi,		// Druddigon
		80_bi,		// Golett
		120_bi,		// Golurk
		40_bi,		// Pawniard
		80_bi,		// Bisharp
		80_bi,		// Bouffalant
		40_bi,		// Rufflet
		60_bi,		// Braviary
		20_bi,		// Vullaby
		60_bi,		// Mandibuzz
		80_bi,		// Heatmor
		60_bi,		// Durant
		40_bi,		// Deino
		60_bi,		// Zweilous
		100_bi,		// Hydreigon
		60_bi,		// Larvesta
		60_bi,		// Volcarona
		120_bi,		// Cobalion
		120_bi,		// Terrakion
		100_bi,		// Virizion
		80_bi,		// Tornadus-Incarnate
		80_bi,		// Tornadus-Therian
		80_bi,		// Thundurus-Incarnate
		80_bi,		// Thundurus-Therian
		120_bi,		// Reshiram
		120_bi,		// Zekrom
		80_bi,		// Landorus-Incarnate
		80_bi,		// Landorus-Therian
		120_bi,		// Kyurem
		120_bi,		// Kyurem-Black
		120_bi,		// Kyurem-White
		60_bi,		// Keldeo
		20_bi,		// Meloetta
		80_bi		// Genesect 
	);
	return mass_array.at(species);
}

}	// namespace
}	// namespace technicalmachine
