// Move power calculator
// Copyright (C) 2013 David Stone
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

#include "base_power.hpp"
#include "move.hpp"
#include "moves.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {
namespace {

unsigned calculate_base_power(ActivePokemon const & attacker, ActivePokemon const & defender, Variable const & variable);
unsigned second_lowest_bit (Stat const & stat);
bool doubling (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather);
unsigned item_modifier (Pokemon const & attacker);
Rational defender_ability_modifier(Pokemon const & attacker, Ability const ability);

bool is_boosted_by_adamant_orb(Species species);
bool is_boosted_by_griseous_orb(Species species);
bool is_boosted_by_lustrous_orb(Species species);

unsigned power_of_mass_based_moves(Species species);
}	// namespace

unsigned move_power (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather, Variable const & variable) {
	unsigned const base_power = calculate_base_power(attacker, defender, variable);
	unsigned power = base_power;

	if (doubling (attacker, defender, weather))
		power *= 2;

	power *= Rational(item_modifier(attacker), 10);

	if (attacker.charge_boosted())
		power *= 2;

	if (defender.sport_is_active(attacker.move()))
		power /= 2;

	power *= Ability::attacker_modifier(attacker, defender, base_power);
	
	power *= defender_ability_modifier(attacker, get_ability(defender));
	
	return std::max(power, 1u);
}

namespace {

Rational defender_ability_modifier(Pokemon const & attacker, Ability const ability) {
	Move const & move = attacker.move();
	switch (ability.name()) {
		case Ability::Dry_Skin:
			return (Type(move, attacker) == Type::Fire) ? Rational(5, 4) : Rational(1);
		case Ability::Heatproof:
			return (Type(move, attacker) == Type::Fire) ? Rational(1, 2) : Rational(1);
		case Ability::Thick_Fat: {
			Type const type(move, attacker);
			return (type == Type::Fire or type == Type::Ice) ?
				Rational(1, 2) :
				Rational(1);
		}
		default:
			return Rational(1);
	}
}

unsigned return_power(Pokemon const & pokemon) {
	return pokemon.happiness() * 2u / 5;
}

unsigned calculate_base_power(ActivePokemon const & attacker, ActivePokemon const & defender, Variable const & variable) {
	switch (static_cast<Moves>(attacker.move())) {
		case Moves::Crush_Grip:
		case Moves::Wring_Out:
			return 120u * hp_ratio(defender) + 1;
		case Moves::Eruption:
		case Moves::Water_Spout:
			return 150u * hp_ratio(attacker);
		case Moves::Flail:
		case Moves::Reversal: {
			unsigned const k = 64u * hp_ratio(attacker);
			if (k <= 1)
				return 200;
			else if (k <= 5)
				return 150;
			else if (k <= 12)
				return 100;
			else if (k <= 21)
				return 80;
			else if (k <= 42)
				return 40;
			else
				return 20;
		}
		case Moves::Fling:
			return get_item(attacker).get_fling_power();
		case Moves::Frustration:
			return 102 - return_power(attacker);
		case Moves::Fury_Cutter:
			return attacker.fury_cutter_power();
		case Moves::Grass_Knot:
		case Moves::Low_Kick:
			return power_of_mass_based_moves(defender);
		case Moves::Gyro_Ball: {
			unsigned const uncapped_power = 25u * get_stat(defender, Stat::SPE).stat / get_stat(attacker, Stat::SPE).stat + 1;
			return std::min(uncapped_power, 150u);
		}
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.momentum_move_power();
		case Moves::Hidden_Power: {
			static constexpr std::array<std::pair<Stat::Stats, unsigned>, 6> stat_and_position {{
				{ Stat::HP, 0 },
				{ Stat::ATK, 1 },
				{ Stat::DEF, 2 },
				{ Stat::SPE, 3 },
				{ Stat::SPA, 4 },
				{ Stat::SPD, 5 }
			}};
			auto const sum = [&](unsigned value, std::pair<Stat::Stats, unsigned> const & stat) {
				return value + second_lowest_bit(get_stat(attacker, stat.first)) * (1u << stat.second);
			};
			return std::accumulate(std::begin(stat_and_position), std::end(stat_and_position), 0u, sum) * 40 / 63 + 30;
		}
		case Moves::Magnitude:
			return variable.value();
		case Moves::Natural_Gift:
			return get_item(attacker).get_berry_power ();
		case Moves::Present:
			assert (!variable.present_heals());
			return variable.value();
		case Moves::Punishment: {
			unsigned const uncapped_power = 60 + 20 * defender.positive_stat_boosts();
			return std::min(uncapped_power, 200u);
		}
		case Moves::Return:
			return return_power(attacker);
		case Moves::Spit_Up:
			return attacker.spit_up_power();
		case Moves::Triple_Kick:
			return attacker.triple_kick_power();
		case Moves::Trump_Card:
			return attacker.move().pp.trump_card_power();
		default:
			return BasePower(attacker.move())();
	}
}

unsigned second_lowest_bit (Stat const & stat) {
	return static_cast<unsigned>(stat.iv >> 1) % 2;
}

bool doubling (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather) {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	Moves const move = attacker.move();
	if (defender.vanish_doubles_power(move))
		return true;
	switch (move) {
		case Moves::Assurance:
			return defender.damaged();
		case Moves::Avalanche: 
		case Moves::Revenge:
			return attacker.damaged();
		case Moves::Brine:
			return get_stat(defender, Stat::HP).stat <= get_stat(defender, Stat::HP).max / 2;
		case Moves::Facade:
			return get_status(attacker).boosts_facade();
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.defense_curled();
		case Moves::Payback:
			return defender.moved();
		case Moves::SmellingSalt:
			return get_status(defender).boosts_smellingsalt();
		case Moves::SolarBeam:
			return !weather.rain();
		case Moves::Stomp:
			return defender.minimized();
		case Moves::Wake_Up_Slap:
			return get_status(defender).is_sleeping();
		case Moves::Weather_Ball:
			return weather.hail() or weather.rain() or weather.sand() or weather.sun();
		default:
			return false;
	}
}

unsigned item_modifier (Pokemon const & attacker) {
	Type const type(attacker.move(), attacker);
	switch (get_item(attacker).name) {
		case Item::MUSCLE_BAND:
			if (is_physical(attacker.move()))
				return 11;
			break;
		case Item::WISE_GLASSES:
			if (is_special(attacker.move()))
				return 11;
			break;
		case Item::INSECT_PLATE:
		case Item::SILVERPOWDER:
			if (type == Type::Bug)
				return 12;
			break;
		case Item::DREAD_PLATE:	
		case Item::BLACKGLASSES:
			if (type == Type::Dark)
				return 12;
			break;
		case Item::DRACO_PLATE:
		case Item::DRAGON_FANG:
			if (type == Type::Dragon)
				return 12;
			break;
		case Item::ZAP_PLATE:
		case Item::MAGNET:
			if (type == Type::Electric)
				return 12;
			break;
		case Item::FIST_PLATE:
		case Item::BLACK_BELT:
			if (type == Type::Fighting)
				return 12;
			break;
		case Item::FLAME_PLATE:
		case Item::CHARCOAL:
			if (type == Type::Fire)
				return 12;
			break;
		case Item::SKY_PLATE:
		case Item::SHARP_BEAK:
			if (type == Type::Flying)
				return 12;
			break;
		case Item::SPOOKY_PLATE:
		case Item::SPELL_TAG:
			if (type == Type::Ghost)
 				return 12;
			break;
		case Item::MEADOW_PLATE:
		case Item::MIRACLE_SEED:
			if (type == Type::Grass)
				return 12;
			break;
		case Item::EARTH_PLATE:
		case Item::SOFT_SAND:
			if (type == Type::Ground)
				return 12;
			break;
		case Item::ICICLE_PLATE:
		case Item::NEVERMELTICE:
			if (type == Type::Ice)
				return 12;
			break;
		case Item::SILK_SCARF:
			if (type == Type::Normal)
				return 12;
			break;
		case Item::TOXIC_PLATE:
		case Item::POISON_BARB:
			if (type == Type::Poison)
				return 12;
			break;
		case Item::MIND_PLATE:
		case Item::TWISTEDSPOON:
		case Item::ODD_INCENSE:
			if (type == Type::Psychic)
				return 12;
			break;
		case Item::STONE_PLATE:
		case Item::HARD_STONE:
		case Item::ROCK_INCENSE:
			if (type == Type::Rock)
				return 12;
			break;
		case Item::IRON_PLATE:
		case Item::METAL_COAT:
			if (type == Type::Steel)
				return 12;
			break;
		case Item::SPLASH_PLATE:
		case Item::MYSTIC_WATER:
		case Item::SEA_INCENSE:
		case Item::WAVE_INCENSE:
			if (type == Type::Water)
				return 12;
			break;
		case Item::ADAMANT_ORB:
			if (is_boosted_by_adamant_orb(attacker) and (type == Type::Dragon or type == Type::Steel))
				return 12;
			break;
		case Item::GRISEOUS_ORB:
			if (is_boosted_by_griseous_orb(attacker) and (type == Type::Dragon or type == Type::Ghost))
				return 12;
			break;
		case Item::LUSTROUS_ORB:
			if (is_boosted_by_lustrous_orb(attacker) and (type == Type::Dragon or type == Type::Water))
				return 12;
			break;
		default:
			break;
	}
	return 10;
}

bool is_boosted_by_adamant_orb(Species const species) {
	return species == Species::Dialga;
}

bool is_boosted_by_griseous_orb(Species const species) {
	return species == Species::Palkia;
}

bool is_boosted_by_lustrous_orb(Species const species) {
	return species == Species::Giratina_Origin;
}

unsigned power_of_mass_based_moves(Species const species) {
	constexpr static uint8_t mass_array [] = {
		// Generation 1
		20,		// Bulbasaur
		40,		// Ivysaur
		80,		// Venusaur
		20,		// Charmander
		40,		// Charmeleon
		80,		// Charizard
		20,		// Squirtle
		40,		// Wartortle
		80,		// Blastoise
		20,		// Caterpie
		20,		// Metapod
		60,		// Butterfree
		20,		// Weedle
		20,		// Kakuna
		60,		// Beedrill
		20,		// Pidgey
		60,		// Pidgeotto
		60,		// Pidgeot
		20,		// Rattata
		40,		// Raticate
		20,		// Spearow
		60,		// Fearow
		20,		// Ekans
		80,		// Arbok
		20,		// Pikachu
		60,		// Raichu
		40,		// Sandshrew
		60,		// Sandslash
		20,		// Nidoran-F
		40,		// Nidorina
		80,		// Nidoqueen
		20,		// Nidoran-M
		40,		// Nidorino
		80,		// Nidoking
		20,		// Clefairy
		60,		// Clefable
		20,		// Vulpix
		40,		// Ninetales
		20,		// Jigglypuff
		40,		// Wigglytuff
		20,		// Zubat
		80,		// Golbat
		20,		// Oddish
		20,		// Gloom
		40,		// Vileplume
		20,		// Paras
		60,		// Parasect
		60,		// Venonat
		40,		// Venomoth
		20,		// Diglett
		60,		// Dugtrio
		20,		// Meowth
		60,		// Persian
		40,		// Psyduck
		80,		// Golduck
		60,		// Mankey
		60,		// Primeape
		40,		// Growlithe
		100,		// Arcanine
		40,		// Poliwag
		40,		// Poliwhirl
		80,		// Poliwrath
		40,		// Abra
		80,		// Kadabra
		60,		// Alakazam
		40,		// Machop
		80,		// Machoke
		100,		// Machamp
		20,		// Bellsprout
		20,		// Weepinbell
		40,		// Victreebel
		60,		// Tentacool
		80,		// Tentacruel
		40,		// Geodude
		100,		// Graveler
		120,		// Golem
		60,		// Ponyta
		80,		// Rapidash
		60,		// Slowpoke
		80,		// Slowbro
		20,		// Magnemite
		80,		// Magneton
		40,		// Farfetch'd
		60,		// Doduo
		80,		// Dodrio
		80,		// Seel
		100,		// Dewgong
		60,		// Grimer
		60,		// Muk
		20,		// Shellder
		100,		// Cloyster
		20,		// Gastly
		20,		// Haunter
		60,		// Gengar
		120,		// Onix
		60,		// Drowzee
		80,		// Hypno
		20,		// Krabby
		80,		// Kingler
		40,		// Voltorb
		80,		// Electrode
		20,		// Exeggcute
		100,		// Exeggutor
		20,		// Cubone
		60,		// Marowak
		60,		// Hitmonlee
		80,		// Hitmonchan
		80,		// Lickitung
		20,		// Koffing
		20,		// Weezing
		100,		// Rhyhorn
		100,		// Rhydon
		60,		// Chansey
		60,		// Tangela
		80,		// Kangaskhan
		20,		// Horsea
		40,		// Seadra
		40,		// Goldeen
		60,		// Seaking
		60,		// Staryu
		80,		// Starmie
		80,		// Mr. Mime
		80,		// Scyther
		60,		// Jynx
		60,		// Electabuzz
		60,		// Magmar
		80,		// Pinsir
		80,		// Tauros
		20,		// Magikarp
		120,		// Gyarados
		120,		// Lapras
		20,		// Ditto
		20,		// Eevee
		60,		// Vaporeon
		40,		// Jolteon
		40,		// Flareon
		60,		// Porygon
		20,		// Omanyte
		60,		// Omastar
		40,		// Kabuto
		60,		// Kabutops
		80,		// Aerodactyl
		120,		// Snorlax
		80,		// Articuno
		80,		// Zapdos
		80,		// Moltres
		20,		// Dratini
		40,		// Dragonair
		120,		// Dragonite
		100,		// Mewtwo
		20,		// Mew

		// Generation 2
		20,		// Chikorita
		40,		// Bayleef
		100,		// Meganium
		20,		// Cyndaquil
		40,		// Quilava
		80,		// Typhlosion
		20,		// Totodile
		40,		// Croconaw
		80,		// Feraligatr
		20,		// Sentret
		60,		// Furret
		40,		// Hoothoot
		60,		// Noctowl
		40,		// Ledyba
		60,		// Ledian
		20,		// Spinarak
		60,		// Ariados
		80,		// Crobat
		40,		// Chinchou
		40,		// Lanturn
		20,		// Pichu
		20,		// Cleffa
		20,		// Igglybuff
		20,		// Togepi
		20,		// Togetic
		20,		// Natu
		40,		// Xatu
		20,		// Mareep
		40,		// Flaaffy
		80,		// Ampharos
		20,		// Bellossom
		20,		// Marill
		60,		// Azumarill
		60,		// Sudowoodo
		60,		// Politoed
		20,		// Hoppip
		20,		// Skiploom
		20,		// Jumpluff
		40,		// Aipom
		20,		// Sunkern
		20,		// Sunflora
		60,		// Yanma
		20,		// Wooper
		80,		// Quagsire
		60,		// Espeon
		60,		// Umbreon
		20,		// Murkrow
		80,		// Slowking
		20,		// Misdreavus
		20,		// Unown
		60,		// Wobbuffet
		60,		// Girafarig
		20,		// Pineco
		100,		// Forretress
		40,		// Dunsparce
		80,		// Gligar
		120,		// Steelix
		20,		// Snubbull
		60,		// Granbull
		20,		// Qwilfish
		100,		// Scizor
		40,		// Shuckle
		80,		// Heracross
		60,		// Sneasel
		20,		// Teddiursa
		100,		// Ursaring
		60,		// Slugma
		80,		// Magcargo
		20,		// Swinub
		80,		// Piloswine
		20,		// Corsola
		40,		// Remoraid
		60,		// Octillery
		40,		// Delibird
		120,		// Mantine
		80,		// Skarmory
		40,		// Houndour
		60,		// Houndoom
		100,		// Kingdra
		60,		// Phanpy
		100,		// Donphan
		60,		// Porygon2
		80,		// Stantler
		80,		// Smeargle
		40,		// Tyrogue
		60,		// Hitmontop
		20,		// Smoochum
		40,		// Elekid
		40,		// Magby
		80,		// Miltank
		60,		// Blissey
		100,		// Raikou
		100,		// Entei
		100,		// Suicune
		80,		// Larvitar
		100,		// Pupitar
		120,		// Tyranitar
		120,		// Lugia
		100,		// Ho-Oh
		20,		// Celebi

		// Generation 3
		20,		// Treecko
		40,		// Grovyle
		80,		// Sceptile
		20,		// Torchic
		40,		// Combusken
		80,		// Blaziken
		20,		// Mudkip
		60,		// Marshtomp
		80,		// Swampert
		40,		// Poochyena
		60,		// Mightyena
		40,		// Zigzagoon
		60,		// Linoone
		20,		// Wurmple
		20,		// Silcoon
		60,		// Beautifly
		40,		// Cascoon
		60,		// Dustox
		20,		// Lotad
		60,		// Lombre
		80,		// Ludicolo
		20,		// Seedot
		60,		// Nuzleaf
		80,		// Shiftry
		20,		// Taillow
		40,		// Swellow
		20,		// Wingull
		60,		// Pelipper
		20,		// Ralts
		40,		// Kirlia
		60,		// Gardevoir
		20,		// Surskit
		20,		// Masquerain
		20,		// Shroomish
		60,		// Breloom
		40,		// Slakoth
		60,		// Vigoroth
		100,		// Slaking
		20,		// Nincada
		40,		// Ninjask
		20,		// Shedinja
		40,		// Whismur
		60,		// Loudred
		80,		// Exploud
		80,		// Makuhita
		120,		// Hariyama
		20,		// Azurill
		80,		// Nosepass
		40,		// Skitty
		60,		// Delcatty
		40,		// Sableye
		40,		// Mawile
		80,		// Aron
		100,		// Lairon
		120,		// Aggron
		40,		// Meditite
		60,		// Medicham
		40,		// Electrike
		60,		// Manectric
		20,		// Plusle
		20,		// Minun
		40,		// Volbeat
		40,		// Illumise
		20,		// Roselia
		40,		// Gulpin
		80,		// Swalot
		40,		// Carvanha
		80,		// Sharpedo
		100,		// Wailmer
		120,		// Wailord
		40,		// Numel
		120,		// Camerupt
		80,		// Torkoal
		60,		// Spoink
		80,		// Grumpig
		20,		// Spinda
		40,		// Trapinch
		40,		// Vibrava
		80,		// Flygon
		80,		// Cacnea
		80,		// Cacturne
		20,		// Swablu
		40,		// Altaria
		60,		// Zangoose
		80,		// Seviper
		100,		// Lunatone
		100,		// Solrock
		20,		// Barboach
		40,		// Whiscash
		40,		// Corphish
		60,		// Crawdaunt
		40,		// Baltoy
		100,		// Claydol
		40,		// Lileep
		80,		// Cradily
		40,		// Anorith
		80,		// Armaldo
		20,		// Feebas
		100,		// Milotic
		20,		// Castform
		40,		// Kecleon
		20,		// Shuppet
		40,		// Banette
		40,		// Duskull
		60,		// Dusclops
		80,		// Tropius
		20,		// Chimecho
		60,		// Absol
		40,		// Wynaut
		40,		// Snorunt
		120,		// Glalie
		60,		// Spheal
		80,		// Sealeo
		100,		// Walrein
		80,		// Clamperl
		60,		// Huntail
		40,		// Gorebyss
		40,		// Relicanth
		20,		// Luvdisc
		60,		// Bagon
		100,		// Shelgon
		100,		// Salamence
		80,		// Beldum
		120,		// Metang
		120,		// Metagross
		120,		// Regirock
		100,		// Regice
		120,		// Registeel
		60,		// Latias
		80,		// Latios
		120,		// Kyogre
		120,		// Groudon
		120,		// Rayquaza
		20,		// Jirachi
		80,		// Deoxys-Mediocre
		80,		// Deoxys-Attack
		80,		// Deoxys-Defense
		80,		// Deoxys-Speed

		// Generation 4
		40,		// Turtwig
		80,		// Grotle
		120,		// Torterra
		20,		// Chimchar
		40,		// Monferno
		80,		// Infernape
		20,		// Piplup
		40,		// Prinplup
		80,		// Empoleon
		20,		// Starly
		40,		// Staravia
		40,		// Staraptor
		40,		// Bidoof
		60,		// Bibarel
		20,		// Kricketot
		60,		// Kricketune
		20,		// Shinx
		60,		// Luxio
		60,		// Luxray
		20,		// Budew
		40,		// Roserade
		60,		// Cranidos
		100,		// Rampardos
		80,		// Shieldon
		100,		// Bastiodon
		20,		// Burmy
		20,		// Wormadam-Plant
		20,		// Wormadam-Sandy
		20,		// Wormadam-Trash
		40,		// Mothim
		20,		// Combee
		60,		// Vespiquen
		20,		// Pachirisu
		60,		// Buizel
		60,		// Floatzel
		20,		// Cherubi
		20,		// Cherrim
		20,		// Shellos
		60,		// Gastrodon
		40,		// Ambipom
		20,		// Drifloon
		40,		// Drifblim
		20,		// Buneary
		60,		// Lopunny
		20,		// Mismagius
		60,		// Honchkrow
		20,		// Glameow
		60,		// Purugly
		20,		// Chingling
		40,		// Stunky
		60,		// Skuntank
		80,		// Bronzor
		100,		// Bronzong
		40,		// Bonsly
		40,		// Mime Jr.
		40,		// Happiny
		20,		// Chatot
		100,		// Spiritomb
		40,		// Gible
		80,		// Gabite
		80,		// Garchomp
		100,		// Munchlax
		40,		// Riolu
		80,		// Lucario
		60,		// Hippopotas
		120,		// Hippowdon
		40,		// Skorupi
		80,		// Drapion
		40,		// Croagunk
		60,		// Toxicroak
		60,		// Carnivine
		20,		// Finneon
		40,		// Lumineon
		80,		// Mantyke
		80,		// Snover
		100,		// Abomasnow
		60,		// Weavile
		100,		// Magnezone
		100,		// Lickilicky
		120,		// Rhyperior
		100,		// Tangrowth
		100,		// Electivire
		80,		// Magmortar
		60,		// Togekiss
		80,		// Yanmega
		60,		// Leafeon
		60,		// Glaceon
		60,		// Gliscor
		120,		// Mamoswine
		60,		// Porygon-Z
		80,		// Gallade
		120,		// Probopass
		100,		// Dusknoir
		60,		// Froslass
		20,		// Rotom
		20,		// Rotom-Heat
		20,		// Rotom-Wash
		20,		// Rotom-Frost
		20,		// Rotom-Fan
		20,		// Rotom-Mow
		20,		// Uxie
		20,		// Mesprit
		20,		// Azelf
		120,		// Dialga
		120,		// Palkia
		120,		// Heatran
		120,		// Regigigas
		120,		// Giratina-Altered
		120,		// Giratina-Origin
		80,		// Cresselia
		20,		// Phione
		20,		// Manaphy
		80,		// Darkrai
		20,		// Shaymin-Land
		20,		// Shaymin-Sky
		120,		// Arceus

		// Generation 5
		20,		// Victini
		20,		// Snivy
		40,		// Servine
		80,		// Serperior
		20,		// Tepig
		80,		// Pignite
		100,		// Emboar
		20,		// Oshawott
		40,		// Dewott
		80,		// Samurott
		40,		// Patrat
		60,		// Watchog
		20,		// Lillipup
		40,		// Herdier
		80,		// Stoutland
		40,		// Purrloin
		60,		// Liepard
		40,		// Pansage
		60,		// Simisage
		40,		// Pansear
		60,		// Simisear
		40,		// Panpour
		60,		// Simipour
		40,		// Munna
		80,		// Musharna
		20,		// Pidove
		40,		// Tranquill
		60,		// Unfezant
		60,		// Blitzle
		80,		// Zebstrika
		40,		// Roggenrola
		100,		// Boldore
		120,		// Gigalith
		20,		// Woobat
		40,		// Swoobat
		20,		// Drilbur
		60,		// Excadrill
		60,		// Audino
		40,		// Timburr
		60,		// Gurdurr
		80,		// Conkeldurr
		20,		// Tympole
		40,		// Palpitoad
		80,		// Seismitoad
		80,		// Throh
		80,		// Sawk
		20,		// Sewaddle
		20,		// Swadloon
		40,		// Leavanny
		20,		// Venipede
		80,		// Whirlipede
		120,		// Scolipede
		20,		// Cottonee
		20,		// Whimsicott
		20,		// Petilil
		40,		// Lilligant
		40,		// Basculin-R
		40,		// Basculin-B
		40,		// Sandile
		60,		// Krokorok
		80,		// Krookodile
		60,		// Darumaka
		80,		// Darmanitan
		60,		// Maractus
		40,		// Dwebble
		100,		// Crustle
		40,		// Scraggy
		60,		// Scrafty
		40,		// Sigilyph
		20,		// Yamask
		80,		// Cofagrigus
		40,		// Tirtouga
		80,		// Carracosta
		20,		// Archen
		60,		// Archeops
		60,		// Trubbish
		100,		// Garbodor
		40,		// Zorua
		80,		// Zoroark
		20,		// Minccino
		20,		// Cinccino
		20,		// Gothita
		40,		// Gothorita
		60,		// Gothitelle
		20,		// Solosis
		20,		// Duosion
		40,		// Reuniclus
		20,		// Ducklett
		40,		// Swanna
		20,		// Vanillite
		60,		// Vanillish
		80,		// Vanilluxe
		40,		// Deerling
		80,		// Sawsbuck
		20,		// Emolga
		20,		// Karrablast
		60,		// Escavalier
		20,		// Foongus
		40,		// Amoonguss
		60,		// Frillish
		100,		// Jellicent
		60,		// Alomomola
		20,		// Joltik
		40,		// Galvantula
		40,		// Ferroseed
		100,		// Ferrothorn
		40,		// Klink
		80,		// Klang
		80,		// Klinklang
		20,		// Tynamo
		40,		// Eelektrik
		80,		// Eelektross
		20,		// Elgyem
		60,		// Beheeyem
		20,		// Litwick
		40,		// Lampent
		60,		// Chandelure
		40,		// Axew
		60,		// Fraxure
		100,		// Haxorus
		20,		// Cubchoo
		120,		// Beartic
		100,		// Cryogonal
		20,		// Shelmet
		60,		// Accelgor
		40,		// Stunfisk
		40,		// Mienfoo
		60,		// Mienshao
		100,		// Druddigon
		80,		// Golett
		120,		// Golurk
		40,		// Pawniard
		80,		// Bisharp
		80,		// Bouffalant
		40,		// Rufflet
		60,		// Braviary
		20,		// Vullaby
		60,		// Mandibuzz
		80,		// Heatmor
		60,		// Durant
		40,		// Deino
		60,		// Zweilous
		100,		// Hydreigon
		60,		// Larvesta
		60,		// Volcarona
		120,		// Cobalion
		120,		// Terrakion
		100,		// Virizion
		80,		// Tornadus-Incarnate
		80,		// Tornadus-Therian
		80,		// Thundurus-Incarnate
		80,		// Thundurus-Therian
		120,		// Reshiram
		120,		// Zekrom
		80,		// Landorus-Incarnate
		80,		// Landorus-Therian
		120,		// Kyurem
		120,		// Kyurem-Black
		120,		// Kyurem-White
		60,		// Keldeo
		20,		// Meloetta
		80,		// Genesect 
	};
	return mass_array[static_cast<unsigned>(species)];
}

}	// unnamed namespace
}	// namespace technicalmachine
