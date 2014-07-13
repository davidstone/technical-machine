// Type function definitions
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

#include "collection.hpp"

#include "type.hpp"

#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {

namespace detail_type_collection {

auto TypeArray::begin() const -> const_iterator {
	return std::begin(m_types);
}
auto TypeArray::end() const -> const_iterator {
	auto const size = BOUNDED_CONDITIONAL(m_types[1_bi] == Type::Typeless, 1_bi, 2_bi);
	return begin() + size;
}

}	// namespace detail_type_collection

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
	static constexpr auto type_array = bounded::make_array<TypeArray>(
		// Generation 1
		TypeArray{ Type::Grass, Type::Poison },			// Bulbasaur
		TypeArray{ Type::Grass, Type::Poison },			// Ivysaur
		TypeArray{ Type::Grass, Type::Poison },			// Venusaur
		TypeArray{ Type::Fire },			// Charmander
		TypeArray{ Type::Fire },			// Charmeleon
		TypeArray{ Type::Fire, Type::Flying },			// Charizard
		TypeArray{ Type::Water },			// Squirtle
		TypeArray{ Type::Water },			// Wartortle
		TypeArray{ Type::Water },			// Blastoise
		TypeArray{ Type::Bug },			// Caterpie
		TypeArray{ Type::Bug },			// Metapod
		TypeArray{ Type::Bug, Type::Flying },			// Butterfree
		TypeArray{ Type::Bug, Type::Poison },			// Weedle
		TypeArray{ Type::Bug, Type::Poison },			// Kakuna
		TypeArray{ Type::Bug, Type::Poison },			// Beedrill
		TypeArray{ Type::Normal, Type::Flying },			// Pidgey
		TypeArray{ Type::Normal, Type::Flying },			// Pidgeotto
		TypeArray{ Type::Normal, Type::Flying },			// Pidgeot
		TypeArray{ Type::Normal },			// Rattata
		TypeArray{ Type::Normal },			// Raticate
		TypeArray{ Type::Normal, Type::Flying },			// Spearow
		TypeArray{ Type::Normal, Type::Flying },			// Fearow
		TypeArray{ Type::Poison },			// Ekans
		TypeArray{ Type::Poison },			// Arbok
		TypeArray{ Type::Electric },			// Pikachu
		TypeArray{ Type::Electric },			// Raichu
		TypeArray{ Type::Ground },			// Sandshrew
		TypeArray{ Type::Ground },			// Sandslash
		TypeArray{ Type::Poison },			// Nidoran-F
		TypeArray{ Type::Poison },			// Nidorina
		TypeArray{ Type::Poison, Type::Ground },			// Nidoqueen
		TypeArray{ Type::Poison },			// Nidoran-M
		TypeArray{ Type::Poison },			// Nidorino
		TypeArray{ Type::Poison, Type::Ground },			// Nidoking
		TypeArray{ Type::Normal },			// Clefairy
		TypeArray{ Type::Normal },			// Clefable
		TypeArray{ Type::Fire },			// Vulpix
		TypeArray{ Type::Fire },			// Ninetales
		TypeArray{ Type::Normal },			// Jigglypuff
		TypeArray{ Type::Normal },			// Wigglytuff
		TypeArray{ Type::Poison, Type::Flying },			// Zubat
		TypeArray{ Type::Poison, Type::Flying },			// Golbat
		TypeArray{ Type::Grass, Type::Poison },			// Oddish
		TypeArray{ Type::Grass, Type::Poison },			// Gloom
		TypeArray{ Type::Grass, Type::Poison },			// Vileplume
		TypeArray{ Type::Bug, Type::Grass },			// Paras
		TypeArray{ Type::Bug, Type::Grass },			// Parasect
		TypeArray{ Type::Bug, Type::Poison },			// Venonat
		TypeArray{ Type::Bug, Type::Poison },			// Venomoth
		TypeArray{ Type::Ground },			// Diglett
		TypeArray{ Type::Ground },			// Dugtrio
		TypeArray{ Type::Normal },			// Meowth
		TypeArray{ Type::Normal },			// Persian
		TypeArray{ Type::Water },			// Psyduck
		TypeArray{ Type::Water },			// Golduck
		TypeArray{ Type::Fighting },			// Mankey
		TypeArray{ Type::Fighting },			// Primeape
		TypeArray{ Type::Fire },			// Growlithe
		TypeArray{ Type::Fire },			// Arcanine
		TypeArray{ Type::Water },			// Poliwag
		TypeArray{ Type::Water },			// Poliwhirl
		TypeArray{ Type::Water, Type::Fighting },			// Poliwrath
		TypeArray{ Type::Psychic },			// Abra
		TypeArray{ Type::Psychic },			// Kadabra
		TypeArray{ Type::Psychic },			// Alakazam
		TypeArray{ Type::Fighting },			// Machop
		TypeArray{ Type::Fighting },			// Machoke
		TypeArray{ Type::Fighting },			// Machamp
		TypeArray{ Type::Grass, Type::Poison },			// Bellsprout
		TypeArray{ Type::Grass, Type::Poison },			// Weepinbell
		TypeArray{ Type::Grass, Type::Poison },			// Victreebel
		TypeArray{ Type::Water, Type::Poison },			// Tentacool
		TypeArray{ Type::Water, Type::Poison },			// Tentacruel
		TypeArray{ Type::Rock, Type::Ground },			// Geodude
		TypeArray{ Type::Rock, Type::Ground },			// Graveler
		TypeArray{ Type::Rock, Type::Ground },			// Golem
		TypeArray{ Type::Fire },			// Ponyta
		TypeArray{ Type::Fire },			// Rapidash
		TypeArray{ Type::Water, Type::Psychic },			// Slowpoke
		TypeArray{ Type::Water, Type::Psychic },			// Slowbro
		TypeArray{ Type::Electric, Type::Steel },			// Magnemite
		TypeArray{ Type::Electric, Type::Steel },			// Magneton
		TypeArray{ Type::Normal, Type::Flying },			// Farfetch'd
		TypeArray{ Type::Normal, Type::Flying },			// Doduo
		TypeArray{ Type::Normal, Type::Flying },			// Dodrio
		TypeArray{ Type::Water },			// Seel
		TypeArray{ Type::Water, Type::Ice },			// Dewgong
		TypeArray{ Type::Poison },			// Grimer
		TypeArray{ Type::Poison },			// Muk
		TypeArray{ Type::Water },			// Shellder
		TypeArray{ Type::Water, Type::Ice },			// Cloyster
		TypeArray{ Type::Ghost, Type::Poison },			// Gastly
		TypeArray{ Type::Ghost, Type::Poison },			// Haunter
		TypeArray{ Type::Ghost, Type::Poison },			// Gengar
		TypeArray{ Type::Rock, Type::Ground },			// Onix
		TypeArray{ Type::Psychic },			// Drowzee
		TypeArray{ Type::Psychic },			// Hypno
		TypeArray{ Type::Water },			// Krabby
		TypeArray{ Type::Water },			// Kingler
		TypeArray{ Type::Electric },			// Voltorb
		TypeArray{ Type::Electric },			// Electrode
		TypeArray{ Type::Grass, Type::Psychic },			// Exeggcute
		TypeArray{ Type::Grass, Type::Psychic },			// Exeggutor
		TypeArray{ Type::Ground },			// Cubone
		TypeArray{ Type::Ground },			// Marowak
		TypeArray{ Type::Fighting },			// Hitmonlee
		TypeArray{ Type::Fighting },			// Hitmonchan
		TypeArray{ Type::Normal },			// Lickitung
		TypeArray{ Type::Poison },			// Koffing
		TypeArray{ Type::Poison },			// Weezing
		TypeArray{ Type::Ground, Type::Rock },			// Rhyhorn
		TypeArray{ Type::Ground, Type::Rock },			// Rhydon
		TypeArray{ Type::Normal },			// Chansey
		TypeArray{ Type::Grass },			// Tangela
		TypeArray{ Type::Normal },			// Kangaskhan
		TypeArray{ Type::Water },			// Horsea
		TypeArray{ Type::Water },			// Seadra
		TypeArray{ Type::Water },			// Goldeen
		TypeArray{ Type::Water },			// Seaking
		TypeArray{ Type::Water },			// Staryu
		TypeArray{ Type::Water, Type::Psychic },			// Starmie
		TypeArray{ Type::Psychic },			// Mr. Mime
		TypeArray{ Type::Bug, Type::Flying },			// Scyther
		TypeArray{ Type::Ice, Type::Psychic },			// Jynx
		TypeArray{ Type::Electric },			// Electabuzz
		TypeArray{ Type::Fire },			// Magmar
		TypeArray{ Type::Bug },			// Pinsir
		TypeArray{ Type::Normal },			// Tauros
		TypeArray{ Type::Water },			// Magikarp
		TypeArray{ Type::Water, Type::Flying },			// Gyarados
		TypeArray{ Type::Water, Type::Ice },			// Lapras
		TypeArray{ Type::Normal },			// Ditto
		TypeArray{ Type::Normal },			// Eevee
		TypeArray{ Type::Water },			// Vaporeon
		TypeArray{ Type::Electric },			// Jolteon
		TypeArray{ Type::Fire },			// Flareon
		TypeArray{ Type::Normal },			// Porygon
		TypeArray{ Type::Rock, Type::Water },			// Omanyte
		TypeArray{ Type::Rock, Type::Water },			// Omastar
		TypeArray{ Type::Rock, Type::Water },			// Kabuto
		TypeArray{ Type::Rock, Type::Water },			// Kabutops
		TypeArray{ Type::Rock, Type::Flying },			// Aerodactyl
		TypeArray{ Type::Normal },			// Snorlax
		TypeArray{ Type::Ice, Type::Flying },			// Articuno
		TypeArray{ Type::Electric, Type::Flying },			// Zapdos
		TypeArray{ Type::Fire, Type::Flying },			// Moltres
		TypeArray{ Type::Dragon },			// Dratini
		TypeArray{ Type::Dragon },			// Dragonair
		TypeArray{ Type::Dragon, Type::Flying },			// Dragonite
		TypeArray{ Type::Psychic },			// Mewtwo
		TypeArray{ Type::Psychic },			// Mew
		
		// Generation 2
		TypeArray{ Type::Grass },			// Chikorita
		TypeArray{ Type::Grass },			// Bayleef
		TypeArray{ Type::Grass },			// Meganium
		TypeArray{ Type::Fire },			// Cyndaquil
		TypeArray{ Type::Fire },			// Quilava
		TypeArray{ Type::Fire },			// Typhlosion
		TypeArray{ Type::Water },			// Totodile
		TypeArray{ Type::Water },			// Croconaw
		TypeArray{ Type::Water },			// Feraligatr
		TypeArray{ Type::Normal },			// Sentret
		TypeArray{ Type::Normal },			// Furret
		TypeArray{ Type::Normal, Type::Flying },			// Hoothoot
		TypeArray{ Type::Normal, Type::Flying },			// Noctowl
		TypeArray{ Type::Bug, Type::Flying },			// Ledyba
		TypeArray{ Type::Bug, Type::Flying },			// Ledian
		TypeArray{ Type::Bug, Type::Poison },			// Spinarak
		TypeArray{ Type::Bug, Type::Poison },			// Ariados
		TypeArray{ Type::Poison, Type::Flying },			// Crobat
		TypeArray{ Type::Water, Type::Electric },			// Chinchou
		TypeArray{ Type::Water, Type::Electric },			// Lanturn
		TypeArray{ Type::Electric },			// Pichu
		TypeArray{ Type::Normal },			// Cleffa
		TypeArray{ Type::Normal },			// Igglybuff
		TypeArray{ Type::Normal },			// Togepi
		TypeArray{ Type::Normal, Type::Flying },			// Togetic
		TypeArray{ Type::Psychic, Type::Flying },			// Natu
		TypeArray{ Type::Psychic, Type::Flying },			// Xatu
		TypeArray{ Type::Electric },			// Mareep
		TypeArray{ Type::Electric },			// Flaaffy
		TypeArray{ Type::Electric },			// Ampharos
		TypeArray{ Type::Grass },			// Bellossom
		TypeArray{ Type::Water },			// Marill
		TypeArray{ Type::Water },			// Azumarill
		TypeArray{ Type::Rock },			// Sudowoodo
		TypeArray{ Type::Water },			// Politoed
		TypeArray{ Type::Grass, Type::Flying },			// Hoppip
		TypeArray{ Type::Grass, Type::Flying },			// Skiploom
		TypeArray{ Type::Grass, Type::Flying },			// Jumpluff
		TypeArray{ Type::Normal },			// Aipom
		TypeArray{ Type::Grass },			// Sunkern
		TypeArray{ Type::Grass },			// Sunflora
		TypeArray{ Type::Bug, Type::Flying },			// Yanma
		TypeArray{ Type::Water, Type::Ground },			// Wooper
		TypeArray{ Type::Water, Type::Ground },			// Quagsire
		TypeArray{ Type::Psychic },			// Espeon
		TypeArray{ Type::Dark },			// Umbreon
		TypeArray{ Type::Dark, Type::Flying },			// Murkrow
		TypeArray{ Type::Water, Type::Psychic },			// Slowking
		TypeArray{ Type::Ghost },			// Misdreavus
		TypeArray{ Type::Psychic },			// Unown
		TypeArray{ Type::Psychic },			// Wobbuffet
		TypeArray{ Type::Normal, Type::Psychic },			// Girafarig
		TypeArray{ Type::Bug },			// Pineco
		TypeArray{ Type::Bug, Type::Steel },			// Forretress
		TypeArray{ Type::Normal },			// Dunsparce
		TypeArray{ Type::Ground, Type::Flying },			// Gligar
		TypeArray{ Type::Steel, Type::Ground },			// Steelix
		TypeArray{ Type::Normal },			// Snubbull
		TypeArray{ Type::Normal },			// Granbull
		TypeArray{ Type::Water, Type::Poison },			// Qwilfish
		TypeArray{ Type::Bug, Type::Steel },			// Scizor
		TypeArray{ Type::Bug, Type::Rock },			// Shuckle
		TypeArray{ Type::Bug, Type::Fighting },			// Heracross
		TypeArray{ Type::Dark, Type::Ice },			// Sneasel
		TypeArray{ Type::Normal },			// Teddiursa
		TypeArray{ Type::Normal },			// Ursaring
		TypeArray{ Type::Fire },			// Slugma
		TypeArray{ Type::Fire, Type::Rock },			// Magcargo
		TypeArray{ Type::Ice, Type::Ground },			// Swinub
		TypeArray{ Type::Ice, Type::Ground },			// Piloswine
		TypeArray{ Type::Water, Type::Rock },			// Corsola
		TypeArray{ Type::Water },			// Remoraid
		TypeArray{ Type::Water },			// Octillery
		TypeArray{ Type::Ice, Type::Flying },			// Delibird
		TypeArray{ Type::Water, Type::Flying },			// Mantine
		TypeArray{ Type::Steel, Type::Flying },			// Skarmory
		TypeArray{ Type::Dark, Type::Fire },			// Houndour
		TypeArray{ Type::Dark, Type::Fire },			// Houndoom
		TypeArray{ Type::Water, Type::Dragon },			// Kingdra
		TypeArray{ Type::Ground },			// Phanpy
		TypeArray{ Type::Ground },			// Donphan
		TypeArray{ Type::Normal },			// Porygon2
		TypeArray{ Type::Normal },			// Stantler
		TypeArray{ Type::Normal },			// Smeargle
		TypeArray{ Type::Fighting },			// Tyrogue
		TypeArray{ Type::Fighting },			// Hitmontop
		TypeArray{ Type::Ice, Type::Psychic },			// Smoochum
		TypeArray{ Type::Electric },			// Elekid
		TypeArray{ Type::Fire },			// Magby
		TypeArray{ Type::Normal },			// Miltank
		TypeArray{ Type::Normal },			// Blissey
		TypeArray{ Type::Electric },			// Raikou
		TypeArray{ Type::Fire },			// Entei
		TypeArray{ Type::Water },			// Suicune
		TypeArray{ Type::Rock, Type::Ground },			// Larvitar
		TypeArray{ Type::Rock, Type::Ground },			// Pupitar
		TypeArray{ Type::Rock, Type::Dark },			// Tyranitar
		TypeArray{ Type::Psychic, Type::Flying },			// Lugia
		TypeArray{ Type::Fire, Type::Flying },			// Ho-Oh
		TypeArray{ Type::Psychic, Type::Grass },			// Celebi
		
		// Generation 3
		TypeArray{ Type::Grass },			// Treecko
		TypeArray{ Type::Grass },			// Grovyle
		TypeArray{ Type::Grass },			// Sceptile
		TypeArray{ Type::Fire },			// Torchic
		TypeArray{ Type::Fire, Type::Fighting },			// Combusken
		TypeArray{ Type::Fire, Type::Fighting },			// Blaziken
		TypeArray{ Type::Water },			// Mudkip
		TypeArray{ Type::Water, Type::Ground },			// Marshtomp
		TypeArray{ Type::Water, Type::Ground },			// Swampert
		TypeArray{ Type::Dark },			// Poochyena
		TypeArray{ Type::Dark },			// Mightyena
		TypeArray{ Type::Normal },			// Zigzagoon
		TypeArray{ Type::Normal },			// Linoone
		TypeArray{ Type::Bug },			// Wurmple
		TypeArray{ Type::Bug },			// Silcoon
		TypeArray{ Type::Bug, Type::Flying },			// Beautifly
		TypeArray{ Type::Bug },			// Cascoon
		TypeArray{ Type::Bug, Type::Poison },			// Dustox
		TypeArray{ Type::Water, Type::Grass },			// Lotad
		TypeArray{ Type::Water, Type::Grass },			// Lombre
		TypeArray{ Type::Water, Type::Grass },			// Ludicolo
		TypeArray{ Type::Grass },			// Seedot
		TypeArray{ Type::Grass, Type::Dark },			// Nuzleaf
		TypeArray{ Type::Grass, Type::Dark },			// Shiftry
		TypeArray{ Type::Normal, Type::Flying },			// Taillow
		TypeArray{ Type::Normal, Type::Flying },			// Swellow
		TypeArray{ Type::Water, Type::Flying },			// Wingull
		TypeArray{ Type::Water, Type::Flying },			// Pelipper
		TypeArray{ Type::Psychic },			// Ralts
		TypeArray{ Type::Psychic },			// Kirlia
		TypeArray{ Type::Psychic },			// Gardevoir
		TypeArray{ Type::Bug, Type::Water },			// Surskit
		TypeArray{ Type::Bug, Type::Flying },			// Masquerain
		TypeArray{ Type::Grass },			// Shroomish
		TypeArray{ Type::Grass, Type::Fighting },			// Breloom
		TypeArray{ Type::Normal },			// Slakoth
		TypeArray{ Type::Normal },			// Vigoroth
		TypeArray{ Type::Normal },			// Slaking
		TypeArray{ Type::Bug, Type::Ground },			// Nincada
		TypeArray{ Type::Bug, Type::Flying },			// Ninjask
		TypeArray{ Type::Bug, Type::Ghost },			// Shedinja
		TypeArray{ Type::Normal },			// Whismur
		TypeArray{ Type::Normal },			// Loudred
		TypeArray{ Type::Normal },			// Exploud
		TypeArray{ Type::Fighting },			// Makuhita
		TypeArray{ Type::Fighting },			// Hariyama
		TypeArray{ Type::Normal },			// Azurill
		TypeArray{ Type::Rock },			// Nosepass
		TypeArray{ Type::Normal },			// Skitty
		TypeArray{ Type::Normal },			// Delcatty
		TypeArray{ Type::Dark, Type::Ghost },			// Sableye
		TypeArray{ Type::Steel },			// Mawile
		TypeArray{ Type::Steel, Type::Rock },			// Aron
		TypeArray{ Type::Steel, Type::Rock },			// Lairon
		TypeArray{ Type::Steel, Type::Rock },			// Aggron
		TypeArray{ Type::Fighting, Type::Psychic },			// Meditite
		TypeArray{ Type::Fighting, Type::Psychic },			// Medicham
		TypeArray{ Type::Electric },			// Electrike
		TypeArray{ Type::Electric },			// Manectric
		TypeArray{ Type::Electric },			// Plusle
		TypeArray{ Type::Electric },			// Minun
		TypeArray{ Type::Bug },			// Volbeat
		TypeArray{ Type::Bug },			// Illumise
		TypeArray{ Type::Grass, Type::Poison },			// Roselia
		TypeArray{ Type::Poison },			// Gulpin
		TypeArray{ Type::Poison },			// Swalot
		TypeArray{ Type::Water, Type::Dark },			// Carvanha
		TypeArray{ Type::Water, Type::Dark },			// Sharpedo
		TypeArray{ Type::Water },			// Wailmer
		TypeArray{ Type::Water },			// Wailord
		TypeArray{ Type::Fire, Type::Ground },			// Numel
		TypeArray{ Type::Fire, Type::Ground },			// Camerupt
		TypeArray{ Type::Fire },			// Torkoal
		TypeArray{ Type::Psychic },			// Spoink
		TypeArray{ Type::Psychic },			// Grumpig
		TypeArray{ Type::Normal },			// Spinda
		TypeArray{ Type::Ground },			// Trapinch
		TypeArray{ Type::Ground, Type::Dragon },			// Vibrava
		TypeArray{ Type::Ground, Type::Dragon },			// Flygon
		TypeArray{ Type::Grass },			// Cacnea
		TypeArray{ Type::Grass, Type::Dark },			// Cacturne
		TypeArray{ Type::Normal, Type::Flying },			// Swablu
		TypeArray{ Type::Dragon, Type::Flying },			// Altaria
		TypeArray{ Type::Normal },			// Zangoose
		TypeArray{ Type::Poison },			// Seviper
		TypeArray{ Type::Rock, Type::Psychic },			// Lunatone
		TypeArray{ Type::Rock, Type::Psychic },			// Solrock
		TypeArray{ Type::Water, Type::Ground },			// Barboach
		TypeArray{ Type::Water, Type::Ground },			// Whiscash
		TypeArray{ Type::Water },			// Corphish
		TypeArray{ Type::Water, Type::Dark },			// Crawdaunt
		TypeArray{ Type::Ground, Type::Psychic },			// Baltoy
		TypeArray{ Type::Ground, Type::Psychic },			// Claydol
		TypeArray{ Type::Rock, Type::Grass },			// Lileep
		TypeArray{ Type::Rock, Type::Grass },			// Cradily
		TypeArray{ Type::Rock, Type::Bug },			// Anorith
		TypeArray{ Type::Rock, Type::Bug },			// Armaldo
		TypeArray{ Type::Water },			// Feebas
		TypeArray{ Type::Water },			// Milotic
		TypeArray{ Type::Normal },			// Castform
	//	TypeArray{ Type::Fire },			// --------------------
	//	TypeArray{ Type::Water },			// --------------------
	//	TypeArray{ Type::Ice },			// --------------------
		TypeArray{ Type::Normal },			// Kecleon
		TypeArray{ Type::Ghost },			// Shuppet
		TypeArray{ Type::Ghost },			// Banette
		TypeArray{ Type::Ghost },			// Duskull
		TypeArray{ Type::Ghost },			// Dusclops
		TypeArray{ Type::Grass, Type::Flying },			// Tropius
		TypeArray{ Type::Psychic },			// Chimecho
		TypeArray{ Type::Dark },			// Absol
		TypeArray{ Type::Psychic },			// Wynaut
		TypeArray{ Type::Ice },			// Snorunt
		TypeArray{ Type::Ice },			// Glalie
		TypeArray{ Type::Ice, Type::Water },			// Spheal
		TypeArray{ Type::Ice, Type::Water },			// Sealeo
		TypeArray{ Type::Ice, Type::Water },			// Walrein
		TypeArray{ Type::Water },			// Clamperl
		TypeArray{ Type::Water },			// Huntail
		TypeArray{ Type::Water },			// Gorebyss
		TypeArray{ Type::Water, Type::Rock },			// Relicanth
		TypeArray{ Type::Water },			// Luvdisc
		TypeArray{ Type::Dragon },			// Bagon
		TypeArray{ Type::Dragon },			// Shelgon
		TypeArray{ Type::Dragon, Type::Flying },			// Salamence
		TypeArray{ Type::Steel, Type::Psychic },			// Beldum
		TypeArray{ Type::Steel, Type::Psychic },			// Metang
		TypeArray{ Type::Steel, Type::Psychic },			// Metagross
		TypeArray{ Type::Rock },			// Regirock
		TypeArray{ Type::Ice },			// Regice
		TypeArray{ Type::Steel },			// Registeel
		TypeArray{ Type::Dragon, Type::Psychic },			// Latias
		TypeArray{ Type::Dragon, Type::Psychic },			// Latios
		TypeArray{ Type::Water },			// Kyogre
		TypeArray{ Type::Ground },			// Groudon
		TypeArray{ Type::Dragon, Type::Flying },			// Rayquaza
		TypeArray{ Type::Steel, Type::Psychic },			// Jirachi
		TypeArray{ Type::Psychic },			// Deoxys-Mediocre
		TypeArray{ Type::Psychic },			// Deoxys-Attack
		TypeArray{ Type::Psychic },			// Deoxys-Defense
		TypeArray{ Type::Psychic },			// Deoxys-Speed
		
		// Generation 4
		TypeArray{ Type::Grass },			// Turtwig
		TypeArray{ Type::Grass },			// Grotle
		TypeArray{ Type::Grass, Type::Ground },			// Torterra
		TypeArray{ Type::Fire },			// Chimchar
		TypeArray{ Type::Fire, Type::Fighting },			// Monferno
		TypeArray{ Type::Fire, Type::Fighting },			// Infernape
		TypeArray{ Type::Water },			// Piplup
		TypeArray{ Type::Water },			// Prinplup
		TypeArray{ Type::Water, Type::Steel },			// Empoleon
		TypeArray{ Type::Normal, Type::Flying },			// Starly
		TypeArray{ Type::Normal, Type::Flying },			// Staravia
		TypeArray{ Type::Normal, Type::Flying },			// Staraptor
		TypeArray{ Type::Normal },			// Bidoof
		TypeArray{ Type::Normal, Type::Water },			// Bibarel
		TypeArray{ Type::Bug },			// Kricketot
		TypeArray{ Type::Bug },			// Kricketune
		TypeArray{ Type::Electric },			// Shinx
		TypeArray{ Type::Electric },			// Luxio
		TypeArray{ Type::Electric },			// Luxray
		TypeArray{ Type::Grass, Type::Poison },			// Budew
		TypeArray{ Type::Grass, Type::Poison },			// Roserade
		TypeArray{ Type::Rock },			// Cranidos
		TypeArray{ Type::Rock },			// Rampardos
		TypeArray{ Type::Rock, Type::Steel },			// Shieldon
		TypeArray{ Type::Rock, Type::Steel },			// Bastiodon
		TypeArray{ Type::Bug },			// Burmy
		TypeArray{ Type::Bug, Type::Grass },			// Wormadam-Plant
		TypeArray{ Type::Bug, Type::Ground },			// Wormadam-Sandy
		TypeArray{ Type::Bug, Type::Steel },			// Wormadam-Trash
		TypeArray{ Type::Bug, Type::Flying },			// Mothim
		TypeArray{ Type::Bug, Type::Flying },			// Combee
		TypeArray{ Type::Bug, Type::Flying },			// Vespiquen
		TypeArray{ Type::Electric },			// Pachirisu
		TypeArray{ Type::Water },			// Buizel
		TypeArray{ Type::Water },			// Floatzel
		TypeArray{ Type::Grass },			// Cherubi
		TypeArray{ Type::Grass },			// Cherrim
		TypeArray{ Type::Water },			// Shellos
		TypeArray{ Type::Water, Type::Ground },			// Gastrodon
		TypeArray{ Type::Normal },			// Ambipom
		TypeArray{ Type::Ghost, Type::Flying },			// Drifloon
		TypeArray{ Type::Ghost, Type::Flying },			// Drifblim
		TypeArray{ Type::Normal },			// Buneary
		TypeArray{ Type::Normal },			// Lopunny
		TypeArray{ Type::Ghost },			// Mismagius
		TypeArray{ Type::Dark, Type::Flying },			// Honchkrow
		TypeArray{ Type::Normal },			// Glameow
		TypeArray{ Type::Normal },			// Purugly
		TypeArray{ Type::Psychic },			// Chingling
		TypeArray{ Type::Poison, Type::Dark },			// Stunky
		TypeArray{ Type::Poison, Type::Dark },			// Skuntank
		TypeArray{ Type::Steel, Type::Psychic },			// Bronzor
		TypeArray{ Type::Steel, Type::Psychic },			// Bronzong
		TypeArray{ Type::Rock },			// Bonsly
		TypeArray{ Type::Psychic },			// Mime Jr.
		TypeArray{ Type::Normal },			// Happiny
		TypeArray{ Type::Normal, Type::Flying },			// Chatot
		TypeArray{ Type::Ghost, Type::Dark },			// Spiritomb
		TypeArray{ Type::Dragon, Type::Ground },			// Gible
		TypeArray{ Type::Dragon, Type::Ground },			// Gabite
		TypeArray{ Type::Dragon, Type::Ground },			// Garchomp
		TypeArray{ Type::Normal },			// Munchlax
		TypeArray{ Type::Fighting },			// Riolu
		TypeArray{ Type::Fighting, Type::Steel },			// Lucario
		TypeArray{ Type::Ground },			// Hippopotas
		TypeArray{ Type::Ground },			// Hippowdon
		TypeArray{ Type::Poison, Type::Bug },			// Skorupi
		TypeArray{ Type::Poison, Type::Dark },			// Drapion
		TypeArray{ Type::Poison, Type::Fighting },			// Croagunk
		TypeArray{ Type::Poison, Type::Fighting },			// Toxicroak
		TypeArray{ Type::Grass },			// Carnivine
		TypeArray{ Type::Water },			// Finneon
		TypeArray{ Type::Water },			// Lumineon
		TypeArray{ Type::Water, Type::Flying },			// Mantyke
		TypeArray{ Type::Grass, Type::Ice },			// Snover
		TypeArray{ Type::Grass, Type::Ice },			// Abomasnow
		TypeArray{ Type::Dark, Type::Ice },			// Weavile
		TypeArray{ Type::Electric, Type::Steel },			// Magnezone
		TypeArray{ Type::Normal },			// Lickilicky
		TypeArray{ Type::Ground, Type::Rock },			// Rhyperior
		TypeArray{ Type::Grass },			// Tangrowth
		TypeArray{ Type::Electric },			// Electivire
		TypeArray{ Type::Fire },			// Magmortar
		TypeArray{ Type::Normal, Type::Flying },			// Togekiss
		TypeArray{ Type::Bug, Type::Flying },			// Yanmega
		TypeArray{ Type::Grass },			// Leafeon
		TypeArray{ Type::Ice },			// Glaceon
		TypeArray{ Type::Ground, Type::Flying },			// Gliscor
		TypeArray{ Type::Ice, Type::Ground },			// Mamoswine
		TypeArray{ Type::Normal },			// Porygon-Z
		TypeArray{ Type::Psychic, Type::Fighting },			// Gallade
		TypeArray{ Type::Rock, Type::Steel },			// Probopass
		TypeArray{ Type::Ghost },			// Dusknoir
		TypeArray{ Type::Ice, Type::Ghost },			// Froslass
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom
		// Electric + Fire
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom-H
		// Electric + Water
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom-W
		// Electric + Ice
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom-F
		// Electric + Flying
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom-S
		// Electric + Grass
		TypeArray{ Type::Electric, Type::Ghost },			// Rotom-C
		TypeArray{ Type::Psychic },			// Uxie
		TypeArray{ Type::Psychic },			// Mesprit
		TypeArray{ Type::Psychic },			// Azelf
		TypeArray{ Type::Steel, Type::Dragon },			// Dialga
		TypeArray{ Type::Water, Type::Dragon },			// Palkia
		TypeArray{ Type::Fire, Type::Steel },			// Heatran
		TypeArray{ Type::Normal },			// Regigigas
		TypeArray{ Type::Ghost, Type::Dragon },			// Giratina-Altered
		TypeArray{ Type::Ghost, Type::Dragon },			// Giratina-Origin
		TypeArray{ Type::Psychic },			// Cresselia
		TypeArray{ Type::Water },			// Phione
		TypeArray{ Type::Water },			// Manaphy
		TypeArray{ Type::Dark },			// Darkrai
		TypeArray{ Type::Grass },			// Shaymin-Land
		TypeArray{ Type::Grass, Type::Flying },			// Shaymin-Sky
		TypeArray{ Type::Normal },			// Arceus
		
		// Generation 5
		TypeArray{ Type::Psychic, Type::Fire },			// Victini
		TypeArray{ Type::Grass },			// Snivy
		TypeArray{ Type::Grass },			// Servine
		TypeArray{ Type::Grass },			// Serperior
		TypeArray{ Type::Fire },			// Tepig
		TypeArray{ Type::Fire, Type::Fighting },			// Pignite
		TypeArray{ Type::Fire, Type::Fighting },			// Emboar
		TypeArray{ Type::Water },			// Oshawott
		TypeArray{ Type::Water },			// Dewott
		TypeArray{ Type::Water },			// Samurott
		TypeArray{ Type::Normal },			// Patrat
		TypeArray{ Type::Normal },			// Watchog
		TypeArray{ Type::Normal },			// Lillipup
		TypeArray{ Type::Normal },			// Herdier
		TypeArray{ Type::Normal },			// Stoutland
		TypeArray{ Type::Dark },			// Purrloin
		TypeArray{ Type::Dark },			// Liepard
		TypeArray{ Type::Grass },			// Pansage
		TypeArray{ Type::Grass },			// Simisage
		TypeArray{ Type::Fire },			// Pansear
		TypeArray{ Type::Fire },			// Simisear
		TypeArray{ Type::Water },			// Panpour
		TypeArray{ Type::Water },			// Simipour
		TypeArray{ Type::Psychic },			// Munna
		TypeArray{ Type::Psychic },			// Musharna
		TypeArray{ Type::Normal, Type::Flying },			// Pidove
		TypeArray{ Type::Normal, Type::Flying },			// Tranquill
		TypeArray{ Type::Normal, Type::Flying },			// Unfezant
		TypeArray{ Type::Electric },			// Blitzle
		TypeArray{ Type::Electric },			// Zebstrika
		TypeArray{ Type::Rock },			// Roggenrola
		TypeArray{ Type::Rock },			// Boldore
		TypeArray{ Type::Rock },			// Gigalith
		TypeArray{ Type::Psychic, Type::Flying },			// Woobat
		TypeArray{ Type::Psychic, Type::Flying },			// Swoobat
		TypeArray{ Type::Ground },			// Drilbur
		TypeArray{ Type::Ground, Type::Steel },			// Excadrill
		TypeArray{ Type::Normal },			// Audino
		TypeArray{ Type::Fighting },			// Timburr
		TypeArray{ Type::Fighting },			// Gurdurr
		TypeArray{ Type::Fighting },			// Conkeldurr
		TypeArray{ Type::Water },			// Tympole
		TypeArray{ Type::Water, Type::Ground },			// Palpitoad
		TypeArray{ Type::Water, Type::Ground },			// Seismitoad
		TypeArray{ Type::Fighting },			// Throh
		TypeArray{ Type::Fighting },			// Sawk
		TypeArray{ Type::Bug, Type::Grass },			// Sewaddle
		TypeArray{ Type::Bug, Type::Grass },			// Swadloon
		TypeArray{ Type::Bug, Type::Grass },			// Leavanny
		TypeArray{ Type::Bug, Type::Poison },			// Venipede
		TypeArray{ Type::Bug, Type::Poison },			// Whirlipede
		TypeArray{ Type::Bug, Type::Poison },			// Scolipede
		TypeArray{ Type::Grass },			// Cottonee
		TypeArray{ Type::Grass },			// Whimsicott
		TypeArray{ Type::Grass },			// Petilil
		TypeArray{ Type::Grass },			// Lilligant
		TypeArray{ Type::Water },			// Basculin-Red
		TypeArray{ Type::Water },			// Basculin-Blue
		TypeArray{ Type::Ground, Type::Dark },			// Sandile
		TypeArray{ Type::Ground, Type::Dark },			// Krokorok
		TypeArray{ Type::Ground, Type::Dark },			// Krookodile
		TypeArray{ Type::Fire },			// Darumaka
		TypeArray{ Type::Fire },			// Darmanitan
	//	TypeArray{ Type::Fire, Type::Psychic },			// Darmanitan (Zen Mode)
		TypeArray{ Type::Grass },			// Maractus
		TypeArray{ Type::Bug, Type::Rock },			// Dwebble
		TypeArray{ Type::Bug, Type::Rock },			// Crustle
		TypeArray{ Type::Dark, Type::Fighting },			// Scraggy
		TypeArray{ Type::Dark, Type::Fighting },			// Scrafty
		TypeArray{ Type::Psychic, Type::Flying },			// Sigilyph
		TypeArray{ Type::Ghost },			// Yamask
		TypeArray{ Type::Ghost },			// Cofagrigus
		TypeArray{ Type::Water, Type::Rock },			// Tirtouga
		TypeArray{ Type::Water, Type::Rock },			// Carracosta
		TypeArray{ Type::Rock, Type::Flying },			// Archen
		TypeArray{ Type::Rock, Type::Flying },			// Archeops
		TypeArray{ Type::Poison },			// Trubbish
		TypeArray{ Type::Poison },			// Garbodor
		TypeArray{ Type::Dark },			// Zorua
		TypeArray{ Type::Dark },			// Zoroark
		TypeArray{ Type::Normal },			// Minccino
		TypeArray{ Type::Normal },			// Cinccino
		TypeArray{ Type::Psychic },			// Gothita
		TypeArray{ Type::Psychic },			// Gothorita
		TypeArray{ Type::Psychic },			// Gothitelle
		TypeArray{ Type::Psychic },			// Solosis
		TypeArray{ Type::Psychic },			// Duosion
		TypeArray{ Type::Psychic },			// Reuniclus
		TypeArray{ Type::Water, Type::Flying },			// Ducklett
		TypeArray{ Type::Water, Type::Flying },			// Swanna
		TypeArray{ Type::Ice },			// Vanillite
		TypeArray{ Type::Ice },			// Vanillish
		TypeArray{ Type::Ice },			// Vanilluxe
		TypeArray{ Type::Normal, Type::Grass },			// Deerling
		TypeArray{ Type::Normal, Type::Grass },			// Sawsbuck
		TypeArray{ Type::Electric, Type::Flying },			// Emolga
		TypeArray{ Type::Bug },			// Karrablast
		TypeArray{ Type::Bug, Type::Steel },			// Escavalier
		TypeArray{ Type::Grass, Type::Poison },			// Foongus
		TypeArray{ Type::Grass, Type::Poison },			// Amoonguss
		TypeArray{ Type::Water, Type::Ghost },			// Frillish
		TypeArray{ Type::Water, Type::Ghost },			// Jellicent
		TypeArray{ Type::Water },			// Alomomola
		TypeArray{ Type::Bug, Type::Electric },			// Joltik
		TypeArray{ Type::Bug, Type::Electric },			// Galvantula
		TypeArray{ Type::Grass, Type::Steel },			// Ferroseed
		TypeArray{ Type::Grass, Type::Steel },			// Ferrothorn
		TypeArray{ Type::Steel },			// Klink
		TypeArray{ Type::Steel },			// Klang
		TypeArray{ Type::Steel },			// Klinklang
		TypeArray{ Type::Electric },			// Tynamo
		TypeArray{ Type::Electric },			// Eelektrik
		TypeArray{ Type::Electric },			// Eelektross
		TypeArray{ Type::Psychic },			// Elgyem
		TypeArray{ Type::Psychic },			// Beheeyem
		TypeArray{ Type::Ghost, Type::Fire },			// Litwick
		TypeArray{ Type::Ghost, Type::Fire },			// Lampent
		TypeArray{ Type::Ghost, Type::Fire },			// Chandelure
		TypeArray{ Type::Dragon },			// Axew
		TypeArray{ Type::Dragon },			// Fraxure
		TypeArray{ Type::Dragon },			// Haxorus
		TypeArray{ Type::Ice },			// Cubchoo
		TypeArray{ Type::Ice },			// Beartic
		TypeArray{ Type::Ice },			// Cryogonal
		TypeArray{ Type::Bug },			// Shelmet
		TypeArray{ Type::Bug },			// Accelgor
		TypeArray{ Type::Ground, Type::Electric },			// Stunfisk
		TypeArray{ Type::Fighting },			// Mienfoo
		TypeArray{ Type::Fighting },			// Mienshao
		TypeArray{ Type::Dragon },			// Druddigon
		TypeArray{ Type::Ground, Type::Ghost },			// Golett
		TypeArray{ Type::Ground, Type::Ghost },			// Golurk
		TypeArray{ Type::Dark, Type::Steel },			// Pawniard
		TypeArray{ Type::Dark, Type::Steel },			// Bisharp
		TypeArray{ Type::Normal },			// Bouffalant
		TypeArray{ Type::Normal, Type::Flying },			// Rufflet
		TypeArray{ Type::Normal, Type::Flying },			// Braviary
		TypeArray{ Type::Dark, Type::Flying },			// Vullaby
		TypeArray{ Type::Dark, Type::Flying },			// Mandibuzz
		TypeArray{ Type::Fire },			// Heatmor
		TypeArray{ Type::Bug, Type::Steel },			// Durant
		TypeArray{ Type::Dark, Type::Dragon },			// Deino
		TypeArray{ Type::Dark, Type::Dragon },			// Zweilous
		TypeArray{ Type::Dark, Type::Dragon },			// Hydreigon
		TypeArray{ Type::Bug, Type::Fire },			// Larvesta
		TypeArray{ Type::Bug, Type::Fire },			// Volcarona
		TypeArray{ Type::Steel, Type::Fighting },			// Cobalion
		TypeArray{ Type::Rock, Type::Fighting },			// Terrakion
		TypeArray{ Type::Grass, Type::Fighting },			// Virizion
		TypeArray{ Type::Flying },			// Tornadus-Incarnate
		TypeArray{ Type::Flying },			// Tornadus-Therian
		TypeArray{ Type::Electric, Type::Flying },			// Thundurus-Incarnate
		TypeArray{ Type::Electric, Type::Flying },			// Thundurus-Therian
		TypeArray{ Type::Dragon, Type::Fire },			// Reshiram
		TypeArray{ Type::Dragon, Type::Electric },			// Zekrom
		TypeArray{ Type::Ground, Type::Flying },			// Landorus-Incarnate
		TypeArray{ Type::Ground, Type::Flying },			// Landorus-Therian
		TypeArray{ Type::Dragon, Type::Ice },			// Kyurem
		TypeArray{ Type::Dragon, Type::Ice },			// Kyurem-Black
		TypeArray{ Type::Dragon, Type::Ice },			// Kyurem-White
		TypeArray{ Type::Water, Type::Fighting },			// Keldeo
		TypeArray{ Type::Normal, Type::Psychic },			// Meloetta
	//	TypeArray{ Type::Normal, Type::Fighting },			// Meloetta (Pirouette form)
		TypeArray{ Type::Bug, Type::Steel }				// Genesect 
	);
	static_assert(type_array.size() == number_of_species, "Incorrect type array size.");
	return type_array.at(name);
}

}	// unnamed namespace
}	// namespace technicalmachine
