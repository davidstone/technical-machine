// Type function definitions
// Copyright (C) 2012 David Stone
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

namespace technicalmachine {
namespace {
detail::TypeArray get_type(Species const name);
}	// unnamed namespace

TypeCollection::TypeCollection(Species const name):
	types(get_type(name)) {
}

bool TypeCollection::is_immune_to_sandstorm () const {
	for (Type const type : types) {
		if (type.is_immune_to_sandstorm ())
			return true;
	}
	return false;
}

bool TypeCollection::is_immune_to_hail () const {
	for (Type const type : types) {
		if (type.is_immune_to_hail())
			return true;
	}
	return false;
}

void TypeCollection::change_type(Type const type) {
	types = detail::TypeArray(type);
}

bool is_type(Pokemon const & pokemon, Type const type, bool const roosting) {
	if (type != Type::Flying or !roosting) {
		for (Type const check : get_type(pokemon).types) {
			if (check == type)
				return true;
		}
	}
	return false;
}
bool is_type (ActivePokemon const & pokemon, Type const type) {
	return is_type(pokemon, type, pokemon.is_roosting());
}

namespace {

bool forced_grounded(Pokemon const & pokemon, Weather const & weather) {
	return weather.gravity() or get_item(pokemon).grounds();
}
bool forced_grounded(ActivePokemon const & pokemon, Weather const & weather) {
	return forced_grounded(static_cast<Pokemon const &>(pokemon), weather) or pokemon.ingrained();
}

bool is_immune_to_ground(Pokemon const & pokemon, Weather const & weather, bool const roosting = false) {
	return is_type(pokemon, Type::Flying, roosting) or get_ability(pokemon).is_immune_to_ground();
}
bool is_immune_to_ground(ActivePokemon const & active, Weather const & weather) {
	auto const & pokemon = static_cast<Pokemon const &>(active);
	return is_immune_to_ground(pokemon, weather, active.is_roosting()) or active.magnet_rise_is_active();
}

template<typename PossiblyActivePokemon>
bool is_grounded(PossiblyActivePokemon const & pokemon, Weather const & weather) {
	return !is_immune_to_ground(pokemon, weather) or forced_grounded(pokemon, weather);
}

}	// unnamed namespace

bool grounded(Pokemon const & pokemon, Weather const & weather) {
	return is_grounded(pokemon, weather);
}
bool grounded(ActivePokemon const & pokemon, Weather const & weather) {
	return is_grounded(pokemon, weather);
}

namespace detail {
TypeArray::TypeArray(Type const type1):
	m_types({{type1, Type::Typeless}})
	{
}
TypeArray::TypeArray(Type const type1, Type const type2):
	m_types({{type1, type2}})
	{
}
TypeArray::size_type TypeArray::size() const {
	return (m_types[1] == Type::Typeless) ? 1 : 2;
}
TypeArray::const_iterator TypeArray::begin() const {
	return std::begin(m_types);
}
TypeArray::const_iterator TypeArray::end() const {
	return begin() + size();
}

}

namespace {

detail::TypeArray get_type(Species const name) {
	static detail::TypeArray const type_array [] = {
		// Generation 1
		detail::TypeArray{ Type::Grass, Type::Poison },			// Bulbasaur
		detail::TypeArray{ Type::Grass, Type::Poison },			// Ivysaur
		detail::TypeArray{ Type::Grass, Type::Poison },			// Venusaur
		detail::TypeArray{ Type::Fire },			// Charmander
		detail::TypeArray{ Type::Fire },			// Charmeleon
		detail::TypeArray{ Type::Fire, Type::Flying },			// Charizard
		detail::TypeArray{ Type::Water },			// Squirtle
		detail::TypeArray{ Type::Water },			// Wartortle
		detail::TypeArray{ Type::Water },			// Blastoise
		detail::TypeArray{ Type::Bug },			// Caterpie
		detail::TypeArray{ Type::Bug },			// Metapod
		detail::TypeArray{ Type::Bug, Type::Flying },			// Butterfree
		detail::TypeArray{ Type::Bug, Type::Poison },			// Weedle
		detail::TypeArray{ Type::Bug, Type::Poison },			// Kakuna
		detail::TypeArray{ Type::Bug, Type::Poison },			// Beedrill
		detail::TypeArray{ Type::Normal, Type::Flying },			// Pidgey
		detail::TypeArray{ Type::Normal, Type::Flying },			// Pidgeotto
		detail::TypeArray{ Type::Normal, Type::Flying },			// Pidgeot
		detail::TypeArray{ Type::Normal },			// Rattata
		detail::TypeArray{ Type::Normal },			// Raticate
		detail::TypeArray{ Type::Normal, Type::Flying },			// Spearow
		detail::TypeArray{ Type::Normal, Type::Flying },			// Fearow
		detail::TypeArray{ Type::Poison },			// Ekans
		detail::TypeArray{ Type::Poison },			// Arbok
		detail::TypeArray{ Type::Electric },			// Pikachu
		detail::TypeArray{ Type::Electric },			// Raichu
		detail::TypeArray{ Type::Ground },			// Sandshrew
		detail::TypeArray{ Type::Ground },			// Sandslash
		detail::TypeArray{ Type::Poison },			// Nidoran-F
		detail::TypeArray{ Type::Poison },			// Nidorina
		detail::TypeArray{ Type::Poison, Type::Ground },			// Nidoqueen
		detail::TypeArray{ Type::Poison },			// Nidoran-M
		detail::TypeArray{ Type::Poison },			// Nidorino
		detail::TypeArray{ Type::Poison, Type::Ground },			// Nidoking
		detail::TypeArray{ Type::Normal },			// Clefairy
		detail::TypeArray{ Type::Normal },			// Clefable
		detail::TypeArray{ Type::Fire },			// Vulpix
		detail::TypeArray{ Type::Fire },			// Ninetales
		detail::TypeArray{ Type::Normal },			// Jigglypuff
		detail::TypeArray{ Type::Normal },			// Wigglytuff
		detail::TypeArray{ Type::Poison, Type::Flying },			// Zubat
		detail::TypeArray{ Type::Poison, Type::Flying },			// Golbat
		detail::TypeArray{ Type::Grass, Type::Poison },			// Oddish
		detail::TypeArray{ Type::Grass, Type::Poison },			// Gloom
		detail::TypeArray{ Type::Grass, Type::Poison },			// Vileplume
		detail::TypeArray{ Type::Bug, Type::Grass },			// Paras
		detail::TypeArray{ Type::Bug, Type::Grass },			// Parasect
		detail::TypeArray{ Type::Bug, Type::Poison },			// Venonat
		detail::TypeArray{ Type::Bug, Type::Poison },			// Venomoth
		detail::TypeArray{ Type::Ground },			// Diglett
		detail::TypeArray{ Type::Ground },			// Dugtrio
		detail::TypeArray{ Type::Normal },			// Meowth
		detail::TypeArray{ Type::Normal },			// Persian
		detail::TypeArray{ Type::Water },			// Psyduck
		detail::TypeArray{ Type::Water },			// Golduck
		detail::TypeArray{ Type::Fighting },			// Mankey
		detail::TypeArray{ Type::Fighting },			// Primeape
		detail::TypeArray{ Type::Fire },			// Growlithe
		detail::TypeArray{ Type::Fire },			// Arcanine
		detail::TypeArray{ Type::Water },			// Poliwag
		detail::TypeArray{ Type::Water },			// Poliwhirl
		detail::TypeArray{ Type::Water, Type::Fighting },			// Poliwrath
		detail::TypeArray{ Type::Psychic },			// Abra
		detail::TypeArray{ Type::Psychic },			// Kadabra
		detail::TypeArray{ Type::Psychic },			// Alakazam
		detail::TypeArray{ Type::Fighting },			// Machop
		detail::TypeArray{ Type::Fighting },			// Machoke
		detail::TypeArray{ Type::Fighting },			// Machamp
		detail::TypeArray{ Type::Grass, Type::Poison },			// Bellsprout
		detail::TypeArray{ Type::Grass, Type::Poison },			// Weepinbell
		detail::TypeArray{ Type::Grass, Type::Poison },			// Victreebel
		detail::TypeArray{ Type::Water, Type::Poison },			// Tentacool
		detail::TypeArray{ Type::Water, Type::Poison },			// Tentacruel
		detail::TypeArray{ Type::Rock, Type::Ground },			// Geodude
		detail::TypeArray{ Type::Rock, Type::Ground },			// Graveler
		detail::TypeArray{ Type::Rock, Type::Ground },			// Golem
		detail::TypeArray{ Type::Fire },			// Ponyta
		detail::TypeArray{ Type::Fire },			// Rapidash
		detail::TypeArray{ Type::Water, Type::Psychic },			// Slowpoke
		detail::TypeArray{ Type::Water, Type::Psychic },			// Slowbro
		detail::TypeArray{ Type::Electric, Type::Steel },			// Magnemite
		detail::TypeArray{ Type::Electric, Type::Steel },			// Magneton
		detail::TypeArray{ Type::Normal, Type::Flying },			// Farfetch'd
		detail::TypeArray{ Type::Normal, Type::Flying },			// Doduo
		detail::TypeArray{ Type::Normal, Type::Flying },			// Dodrio
		detail::TypeArray{ Type::Water },			// Seel
		detail::TypeArray{ Type::Water, Type::Ice },			// Dewgong
		detail::TypeArray{ Type::Poison },			// Grimer
		detail::TypeArray{ Type::Poison },			// Muk
		detail::TypeArray{ Type::Water },			// Shellder
		detail::TypeArray{ Type::Water, Type::Ice },			// Cloyster
		detail::TypeArray{ Type::Ghost, Type::Poison },			// Gastly
		detail::TypeArray{ Type::Ghost, Type::Poison },			// Haunter
		detail::TypeArray{ Type::Ghost, Type::Poison },			// Gengar
		detail::TypeArray{ Type::Rock, Type::Ground },			// Onix
		detail::TypeArray{ Type::Psychic },			// Drowzee
		detail::TypeArray{ Type::Psychic },			// Hypno
		detail::TypeArray{ Type::Water },			// Krabby
		detail::TypeArray{ Type::Water },			// Kingler
		detail::TypeArray{ Type::Electric },			// Voltorb
		detail::TypeArray{ Type::Electric },			// Electrode
		detail::TypeArray{ Type::Grass, Type::Psychic },			// Exeggcute
		detail::TypeArray{ Type::Grass, Type::Psychic },			// Exeggutor
		detail::TypeArray{ Type::Ground },			// Cubone
		detail::TypeArray{ Type::Ground },			// Marowak
		detail::TypeArray{ Type::Fighting },			// Hitmonlee
		detail::TypeArray{ Type::Fighting },			// Hitmonchan
		detail::TypeArray{ Type::Normal },			// Lickitung
		detail::TypeArray{ Type::Poison },			// Koffing
		detail::TypeArray{ Type::Poison },			// Weezing
		detail::TypeArray{ Type::Ground, Type::Rock },			// Rhyhorn
		detail::TypeArray{ Type::Ground, Type::Rock },			// Rhydon
		detail::TypeArray{ Type::Normal },			// Chansey
		detail::TypeArray{ Type::Grass },			// Tangela
		detail::TypeArray{ Type::Normal },			// Kangaskhan
		detail::TypeArray{ Type::Water },			// Horsea
		detail::TypeArray{ Type::Water },			// Seadra
		detail::TypeArray{ Type::Water },			// Goldeen
		detail::TypeArray{ Type::Water },			// Seaking
		detail::TypeArray{ Type::Water },			// Staryu
		detail::TypeArray{ Type::Water, Type::Psychic },			// Starmie
		detail::TypeArray{ Type::Psychic },			// Mr. Mime
		detail::TypeArray{ Type::Bug, Type::Flying },			// Scyther
		detail::TypeArray{ Type::Ice, Type::Psychic },			// Jynx
		detail::TypeArray{ Type::Electric },			// Electabuzz
		detail::TypeArray{ Type::Fire },			// Magmar
		detail::TypeArray{ Type::Bug },			// Pinsir
		detail::TypeArray{ Type::Normal },			// Tauros
		detail::TypeArray{ Type::Water },			// Magikarp
		detail::TypeArray{ Type::Water, Type::Flying },			// Gyarados
		detail::TypeArray{ Type::Water, Type::Ice },			// Lapras
		detail::TypeArray{ Type::Normal },			// Ditto
		detail::TypeArray{ Type::Normal },			// Eevee
		detail::TypeArray{ Type::Water },			// Vaporeon
		detail::TypeArray{ Type::Electric },			// Jolteon
		detail::TypeArray{ Type::Fire },			// Flareon
		detail::TypeArray{ Type::Normal },			// Porygon
		detail::TypeArray{ Type::Rock, Type::Water },			// Omanyte
		detail::TypeArray{ Type::Rock, Type::Water },			// Omastar
		detail::TypeArray{ Type::Rock, Type::Water },			// Kabuto
		detail::TypeArray{ Type::Rock, Type::Water },			// Kabutops
		detail::TypeArray{ Type::Rock, Type::Flying },			// Aerodactyl
		detail::TypeArray{ Type::Normal },			// Snorlax
		detail::TypeArray{ Type::Ice, Type::Flying },			// Articuno
		detail::TypeArray{ Type::Electric, Type::Flying },			// Zapdos
		detail::TypeArray{ Type::Fire, Type::Flying },			// Moltres
		detail::TypeArray{ Type::Dragon },			// Dratini
		detail::TypeArray{ Type::Dragon },			// Dragonair
		detail::TypeArray{ Type::Dragon, Type::Flying },			// Dragonite
		detail::TypeArray{ Type::Psychic },			// Mewtwo
		detail::TypeArray{ Type::Psychic },			// Mew
		
		// Generation 2
		detail::TypeArray{ Type::Grass },			// Chikorita
		detail::TypeArray{ Type::Grass },			// Bayleef
		detail::TypeArray{ Type::Grass },			// Meganium
		detail::TypeArray{ Type::Fire },			// Cyndaquil
		detail::TypeArray{ Type::Fire },			// Quilava
		detail::TypeArray{ Type::Fire },			// Typhlosion
		detail::TypeArray{ Type::Water },			// Totodile
		detail::TypeArray{ Type::Water },			// Croconaw
		detail::TypeArray{ Type::Water },			// Feraligatr
		detail::TypeArray{ Type::Normal },			// Sentret
		detail::TypeArray{ Type::Normal },			// Furret
		detail::TypeArray{ Type::Normal, Type::Flying },			// Hoothoot
		detail::TypeArray{ Type::Normal, Type::Flying },			// Noctowl
		detail::TypeArray{ Type::Bug, Type::Flying },			// Ledyba
		detail::TypeArray{ Type::Bug, Type::Flying },			// Ledian
		detail::TypeArray{ Type::Bug, Type::Poison },			// Spinarak
		detail::TypeArray{ Type::Bug, Type::Poison },			// Ariados
		detail::TypeArray{ Type::Poison, Type::Flying },			// Crobat
		detail::TypeArray{ Type::Water, Type::Electric },			// Chinchou
		detail::TypeArray{ Type::Water, Type::Electric },			// Lanturn
		detail::TypeArray{ Type::Electric },			// Pichu
		detail::TypeArray{ Type::Normal },			// Cleffa
		detail::TypeArray{ Type::Normal },			// Igglybuff
		detail::TypeArray{ Type::Normal },			// Togepi
		detail::TypeArray{ Type::Normal, Type::Flying },			// Togetic
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Natu
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Xatu
		detail::TypeArray{ Type::Electric },			// Mareep
		detail::TypeArray{ Type::Electric },			// Flaaffy
		detail::TypeArray{ Type::Electric },			// Ampharos
		detail::TypeArray{ Type::Grass },			// Bellossom
		detail::TypeArray{ Type::Water },			// Marill
		detail::TypeArray{ Type::Water },			// Azumarill
		detail::TypeArray{ Type::Rock },			// Sudowoodo
		detail::TypeArray{ Type::Water },			// Politoed
		detail::TypeArray{ Type::Grass, Type::Flying },			// Hoppip
		detail::TypeArray{ Type::Grass, Type::Flying },			// Skiploom
		detail::TypeArray{ Type::Grass, Type::Flying },			// Jumpluff
		detail::TypeArray{ Type::Normal },			// Aipom
		detail::TypeArray{ Type::Grass },			// Sunkern
		detail::TypeArray{ Type::Grass },			// Sunflora
		detail::TypeArray{ Type::Bug, Type::Flying },			// Yanma
		detail::TypeArray{ Type::Water, Type::Ground },			// Wooper
		detail::TypeArray{ Type::Water, Type::Ground },			// Quagsire
		detail::TypeArray{ Type::Psychic },			// Espeon
		detail::TypeArray{ Type::Dark },			// Umbreon
		detail::TypeArray{ Type::Dark, Type::Flying },			// Murkrow
		detail::TypeArray{ Type::Water, Type::Psychic },			// Slowking
		detail::TypeArray{ Type::Ghost },			// Misdreavus
		detail::TypeArray{ Type::Psychic },			// Unown
		detail::TypeArray{ Type::Psychic },			// Wobbuffet
		detail::TypeArray{ Type::Normal, Type::Psychic },			// Girafarig
		detail::TypeArray{ Type::Bug },			// Pineco
		detail::TypeArray{ Type::Bug, Type::Steel },			// Forretress
		detail::TypeArray{ Type::Normal },			// Dunsparce
		detail::TypeArray{ Type::Ground, Type::Flying },			// Gligar
		detail::TypeArray{ Type::Steel, Type::Ground },			// Steelix
		detail::TypeArray{ Type::Normal },			// Snubbull
		detail::TypeArray{ Type::Normal },			// Granbull
		detail::TypeArray{ Type::Water, Type::Poison },			// Qwilfish
		detail::TypeArray{ Type::Bug, Type::Steel },			// Scizor
		detail::TypeArray{ Type::Bug, Type::Rock },			// Shuckle
		detail::TypeArray{ Type::Bug, Type::Fighting },			// Heracross
		detail::TypeArray{ Type::Dark, Type::Ice },			// Sneasel
		detail::TypeArray{ Type::Normal },			// Teddiursa
		detail::TypeArray{ Type::Normal },			// Ursaring
		detail::TypeArray{ Type::Fire },			// Slugma
		detail::TypeArray{ Type::Fire, Type::Rock },			// Magcargo
		detail::TypeArray{ Type::Ice, Type::Ground },			// Swinub
		detail::TypeArray{ Type::Ice, Type::Ground },			// Piloswine
		detail::TypeArray{ Type::Water, Type::Rock },			// Corsola
		detail::TypeArray{ Type::Water },			// Remoraid
		detail::TypeArray{ Type::Water },			// Octillery
		detail::TypeArray{ Type::Ice, Type::Flying },			// Delibird
		detail::TypeArray{ Type::Water, Type::Flying },			// Mantine
		detail::TypeArray{ Type::Steel, Type::Flying },			// Skarmory
		detail::TypeArray{ Type::Dark, Type::Fire },			// Houndour
		detail::TypeArray{ Type::Dark, Type::Fire },			// Houndoom
		detail::TypeArray{ Type::Water, Type::Dragon },			// Kingdra
		detail::TypeArray{ Type::Ground },			// Phanpy
		detail::TypeArray{ Type::Ground },			// Donphan
		detail::TypeArray{ Type::Normal },			// Porygon2
		detail::TypeArray{ Type::Normal },			// Stantler
		detail::TypeArray{ Type::Normal },			// Smeargle
		detail::TypeArray{ Type::Fighting },			// Tyrogue
		detail::TypeArray{ Type::Fighting },			// Hitmontop
		detail::TypeArray{ Type::Ice, Type::Psychic },			// Smoochum
		detail::TypeArray{ Type::Electric },			// Elekid
		detail::TypeArray{ Type::Fire },			// Magby
		detail::TypeArray{ Type::Normal },			// Miltank
		detail::TypeArray{ Type::Normal },			// Blissey
		detail::TypeArray{ Type::Electric },			// Raikou
		detail::TypeArray{ Type::Fire },			// Entei
		detail::TypeArray{ Type::Water },			// Suicune
		detail::TypeArray{ Type::Rock, Type::Ground },			// Larvitar
		detail::TypeArray{ Type::Rock, Type::Ground },			// Pupitar
		detail::TypeArray{ Type::Rock, Type::Dark },			// Tyranitar
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Lugia
		detail::TypeArray{ Type::Fire, Type::Flying },			// Ho-Oh
		detail::TypeArray{ Type::Psychic, Type::Grass },			// Celebi
		
		// Generation 3
		detail::TypeArray{ Type::Grass },			// Treecko
		detail::TypeArray{ Type::Grass },			// Grovyle
		detail::TypeArray{ Type::Grass },			// Sceptile
		detail::TypeArray{ Type::Fire },			// Torchic
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Combusken
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Blaziken
		detail::TypeArray{ Type::Water },			// Mudkip
		detail::TypeArray{ Type::Water, Type::Ground },			// Marshtomp
		detail::TypeArray{ Type::Water, Type::Ground },			// Swampert
		detail::TypeArray{ Type::Dark },			// Poochyena
		detail::TypeArray{ Type::Dark },			// Mightyena
		detail::TypeArray{ Type::Normal },			// Zigzagoon
		detail::TypeArray{ Type::Normal },			// Linoone
		detail::TypeArray{ Type::Bug },			// Wurmple
		detail::TypeArray{ Type::Bug },			// Silcoon
		detail::TypeArray{ Type::Bug, Type::Flying },			// Beautifly
		detail::TypeArray{ Type::Bug },			// Cascoon
		detail::TypeArray{ Type::Bug, Type::Poison },			// Dustox
		detail::TypeArray{ Type::Water, Type::Grass },			// Lotad
		detail::TypeArray{ Type::Water, Type::Grass },			// Lombre
		detail::TypeArray{ Type::Water, Type::Grass },			// Ludicolo
		detail::TypeArray{ Type::Grass },			// Seedot
		detail::TypeArray{ Type::Grass, Type::Dark },			// Nuzleaf
		detail::TypeArray{ Type::Grass, Type::Dark },			// Shiftry
		detail::TypeArray{ Type::Normal, Type::Flying },			// Taillow
		detail::TypeArray{ Type::Normal, Type::Flying },			// Swellow
		detail::TypeArray{ Type::Water, Type::Flying },			// Wingull
		detail::TypeArray{ Type::Water, Type::Flying },			// Pelipper
		detail::TypeArray{ Type::Psychic },			// Ralts
		detail::TypeArray{ Type::Psychic },			// Kirlia
		detail::TypeArray{ Type::Psychic },			// Gardevoir
		detail::TypeArray{ Type::Bug, Type::Water },			// Surskit
		detail::TypeArray{ Type::Bug, Type::Flying },			// Masquerain
		detail::TypeArray{ Type::Grass },			// Shroomish
		detail::TypeArray{ Type::Grass, Type::Fighting },			// Breloom
		detail::TypeArray{ Type::Normal },			// Slakoth
		detail::TypeArray{ Type::Normal },			// Vigoroth
		detail::TypeArray{ Type::Normal },			// Slaking
		detail::TypeArray{ Type::Bug, Type::Ground },			// Nincada
		detail::TypeArray{ Type::Bug, Type::Flying },			// Ninjask
		detail::TypeArray{ Type::Bug, Type::Ghost },			// Shedinja
		detail::TypeArray{ Type::Normal },			// Whismur
		detail::TypeArray{ Type::Normal },			// Loudred
		detail::TypeArray{ Type::Normal },			// Exploud
		detail::TypeArray{ Type::Fighting },			// Makuhita
		detail::TypeArray{ Type::Fighting },			// Hariyama
		detail::TypeArray{ Type::Normal },			// Azurill
		detail::TypeArray{ Type::Rock },			// Nosepass
		detail::TypeArray{ Type::Normal },			// Skitty
		detail::TypeArray{ Type::Normal },			// Delcatty
		detail::TypeArray{ Type::Dark, Type::Ghost },			// Sableye
		detail::TypeArray{ Type::Steel },			// Mawile
		detail::TypeArray{ Type::Steel, Type::Rock },			// Aron
		detail::TypeArray{ Type::Steel, Type::Rock },			// Lairon
		detail::TypeArray{ Type::Steel, Type::Rock },			// Aggron
		detail::TypeArray{ Type::Fighting, Type::Psychic },			// Meditite
		detail::TypeArray{ Type::Fighting, Type::Psychic },			// Medicham
		detail::TypeArray{ Type::Electric },			// Electrike
		detail::TypeArray{ Type::Electric },			// Manectric
		detail::TypeArray{ Type::Electric },			// Plusle
		detail::TypeArray{ Type::Electric },			// Minun
		detail::TypeArray{ Type::Bug },			// Volbeat
		detail::TypeArray{ Type::Bug },			// Illumise
		detail::TypeArray{ Type::Grass, Type::Poison },			// Roselia
		detail::TypeArray{ Type::Poison },			// Gulpin
		detail::TypeArray{ Type::Poison },			// Swalot
		detail::TypeArray{ Type::Water, Type::Dark },			// Carvanha
		detail::TypeArray{ Type::Water, Type::Dark },			// Sharpedo
		detail::TypeArray{ Type::Water },			// Wailmer
		detail::TypeArray{ Type::Water },			// Wailord
		detail::TypeArray{ Type::Fire, Type::Ground },			// Numel
		detail::TypeArray{ Type::Fire, Type::Ground },			// Camerupt
		detail::TypeArray{ Type::Fire },			// Torkoal
		detail::TypeArray{ Type::Psychic },			// Spoink
		detail::TypeArray{ Type::Psychic },			// Grumpig
		detail::TypeArray{ Type::Normal },			// Spinda
		detail::TypeArray{ Type::Ground },			// Trapinch
		detail::TypeArray{ Type::Ground, Type::Dragon },			// Vibrava
		detail::TypeArray{ Type::Ground, Type::Dragon },			// Flygon
		detail::TypeArray{ Type::Grass },			// Cacnea
		detail::TypeArray{ Type::Grass, Type::Dark },			// Cacturne
		detail::TypeArray{ Type::Normal, Type::Flying },			// Swablu
		detail::TypeArray{ Type::Dragon, Type::Flying },			// Altaria
		detail::TypeArray{ Type::Normal },			// Zangoose
		detail::TypeArray{ Type::Poison },			// Seviper
		detail::TypeArray{ Type::Rock, Type::Psychic },			// Lunatone
		detail::TypeArray{ Type::Rock, Type::Psychic },			// Solrock
		detail::TypeArray{ Type::Water, Type::Ground },			// Barboach
		detail::TypeArray{ Type::Water, Type::Ground },			// Whiscash
		detail::TypeArray{ Type::Water },			// Corphish
		detail::TypeArray{ Type::Water, Type::Dark },			// Crawdaunt
		detail::TypeArray{ Type::Ground, Type::Psychic },			// Baltoy
		detail::TypeArray{ Type::Ground, Type::Psychic },			// Claydol
		detail::TypeArray{ Type::Rock, Type::Grass },			// Lileep
		detail::TypeArray{ Type::Rock, Type::Grass },			// Cradily
		detail::TypeArray{ Type::Rock, Type::Bug },			// Anorith
		detail::TypeArray{ Type::Rock, Type::Bug },			// Armaldo
		detail::TypeArray{ Type::Water },			// Feebas
		detail::TypeArray{ Type::Water },			// Milotic
		detail::TypeArray{ Type::Normal },			// Castform
	//	TypeArray{ Type::Fire },			// --------------------
	//	TypeArray{ Type::Water },			// --------------------
	//	TypeArray{ Type::Ice },			// --------------------
		detail::TypeArray{ Type::Normal },			// Kecleon
		detail::TypeArray{ Type::Ghost },			// Shuppet
		detail::TypeArray{ Type::Ghost },			// Banette
		detail::TypeArray{ Type::Ghost },			// Duskull
		detail::TypeArray{ Type::Ghost },			// Dusclops
		detail::TypeArray{ Type::Grass, Type::Flying },			// Tropius
		detail::TypeArray{ Type::Psychic },			// Chimecho
		detail::TypeArray{ Type::Dark },			// Absol
		detail::TypeArray{ Type::Psychic },			// Wynaut
		detail::TypeArray{ Type::Ice },			// Snorunt
		detail::TypeArray{ Type::Ice },			// Glalie
		detail::TypeArray{ Type::Ice, Type::Water },			// Spheal
		detail::TypeArray{ Type::Ice, Type::Water },			// Sealeo
		detail::TypeArray{ Type::Ice, Type::Water },			// Walrein
		detail::TypeArray{ Type::Water },			// Clamperl
		detail::TypeArray{ Type::Water },			// Huntail
		detail::TypeArray{ Type::Water },			// Gorebyss
		detail::TypeArray{ Type::Water, Type::Rock },			// Relicanth
		detail::TypeArray{ Type::Water },			// Luvdisc
		detail::TypeArray{ Type::Dragon },			// Bagon
		detail::TypeArray{ Type::Dragon },			// Shelgon
		detail::TypeArray{ Type::Dragon, Type::Flying },			// Salamence
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Beldum
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Metang
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Metagross
		detail::TypeArray{ Type::Rock },			// Regirock
		detail::TypeArray{ Type::Ice },			// Regice
		detail::TypeArray{ Type::Steel },			// Registeel
		detail::TypeArray{ Type::Dragon, Type::Psychic },			// Latias
		detail::TypeArray{ Type::Dragon, Type::Psychic },			// Latios
		detail::TypeArray{ Type::Water },			// Kyogre
		detail::TypeArray{ Type::Ground },			// Groudon
		detail::TypeArray{ Type::Dragon, Type::Flying },			// Rayquaza
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Jirachi
		detail::TypeArray{ Type::Psychic },			// Deoxys-Mediocre
		detail::TypeArray{ Type::Psychic },			// Deoxys-Attack
		detail::TypeArray{ Type::Psychic },			// Deoxys-Defense
		detail::TypeArray{ Type::Psychic },			// Deoxys-Speed
		
		// Generation 4
		detail::TypeArray{ Type::Grass },			// Turtwig
		detail::TypeArray{ Type::Grass },			// Grotle
		detail::TypeArray{ Type::Grass, Type::Ground },			// Torterra
		detail::TypeArray{ Type::Fire },			// Chimchar
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Monferno
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Infernape
		detail::TypeArray{ Type::Water },			// Piplup
		detail::TypeArray{ Type::Water },			// Prinplup
		detail::TypeArray{ Type::Water, Type::Steel },			// Empoleon
		detail::TypeArray{ Type::Normal, Type::Flying },			// Starly
		detail::TypeArray{ Type::Normal, Type::Flying },			// Staravia
		detail::TypeArray{ Type::Normal, Type::Flying },			// Staraptor
		detail::TypeArray{ Type::Normal },			// Bidoof
		detail::TypeArray{ Type::Normal, Type::Water },			// Bibarel
		detail::TypeArray{ Type::Bug },			// Kricketot
		detail::TypeArray{ Type::Bug },			// Kricketune
		detail::TypeArray{ Type::Electric },			// Shinx
		detail::TypeArray{ Type::Electric },			// Luxio
		detail::TypeArray{ Type::Electric },			// Luxray
		detail::TypeArray{ Type::Grass, Type::Poison },			// Budew
		detail::TypeArray{ Type::Grass, Type::Poison },			// Roserade
		detail::TypeArray{ Type::Rock },			// Cranidos
		detail::TypeArray{ Type::Rock },			// Rampardos
		detail::TypeArray{ Type::Rock, Type::Steel },			// Shieldon
		detail::TypeArray{ Type::Rock, Type::Steel },			// Bastiodon
		detail::TypeArray{ Type::Bug },			// Burmy
		detail::TypeArray{ Type::Bug, Type::Grass },			// Wormadam-Plant
		detail::TypeArray{ Type::Bug, Type::Ground },			// Wormadam-Sandy
		detail::TypeArray{ Type::Bug, Type::Steel },			// Wormadam-Trash
		detail::TypeArray{ Type::Bug, Type::Flying },			// Mothim
		detail::TypeArray{ Type::Bug, Type::Flying },			// Combee
		detail::TypeArray{ Type::Bug, Type::Flying },			// Vespiquen
		detail::TypeArray{ Type::Electric },			// Pachirisu
		detail::TypeArray{ Type::Water },			// Buizel
		detail::TypeArray{ Type::Water },			// Floatzel
		detail::TypeArray{ Type::Grass },			// Cherubi
		detail::TypeArray{ Type::Grass },			// Cherrim
		detail::TypeArray{ Type::Water },			// Shellos
		detail::TypeArray{ Type::Water, Type::Ground },			// Gastrodon
		detail::TypeArray{ Type::Normal },			// Ambipom
		detail::TypeArray{ Type::Ghost, Type::Flying },			// Drifloon
		detail::TypeArray{ Type::Ghost, Type::Flying },			// Drifblim
		detail::TypeArray{ Type::Normal },			// Buneary
		detail::TypeArray{ Type::Normal },			// Lopunny
		detail::TypeArray{ Type::Ghost },			// Mismagius
		detail::TypeArray{ Type::Dark, Type::Flying },			// Honchkrow
		detail::TypeArray{ Type::Normal },			// Glameow
		detail::TypeArray{ Type::Normal },			// Purugly
		detail::TypeArray{ Type::Psychic },			// Chingling
		detail::TypeArray{ Type::Poison, Type::Dark },			// Stunky
		detail::TypeArray{ Type::Poison, Type::Dark },			// Skuntank
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Bronzor
		detail::TypeArray{ Type::Steel, Type::Psychic },			// Bronzong
		detail::TypeArray{ Type::Rock },			// Bonsly
		detail::TypeArray{ Type::Psychic },			// Mime Jr.
		detail::TypeArray{ Type::Normal },			// Happiny
		detail::TypeArray{ Type::Normal, Type::Flying },			// Chatot
		detail::TypeArray{ Type::Ghost, Type::Dark },			// Spiritomb
		detail::TypeArray{ Type::Dragon, Type::Ground },			// Gible
		detail::TypeArray{ Type::Dragon, Type::Ground },			// Gabite
		detail::TypeArray{ Type::Dragon, Type::Ground },			// Garchomp
		detail::TypeArray{ Type::Normal },			// Munchlax
		detail::TypeArray{ Type::Fighting },			// Riolu
		detail::TypeArray{ Type::Fighting, Type::Steel },			// Lucario
		detail::TypeArray{ Type::Ground },			// Hippopotas
		detail::TypeArray{ Type::Ground },			// Hippowdon
		detail::TypeArray{ Type::Poison, Type::Bug },			// Skorupi
		detail::TypeArray{ Type::Poison, Type::Dark },			// Drapion
		detail::TypeArray{ Type::Poison, Type::Fighting },			// Croagunk
		detail::TypeArray{ Type::Poison, Type::Fighting },			// Toxicroak
		detail::TypeArray{ Type::Grass },			// Carnivine
		detail::TypeArray{ Type::Water },			// Finneon
		detail::TypeArray{ Type::Water },			// Lumineon
		detail::TypeArray{ Type::Water, Type::Flying },			// Mantyke
		detail::TypeArray{ Type::Grass, Type::Ice },			// Snover
		detail::TypeArray{ Type::Grass, Type::Ice },			// Abomasnow
		detail::TypeArray{ Type::Dark, Type::Ice },			// Weavile
		detail::TypeArray{ Type::Electric, Type::Steel },			// Magnezone
		detail::TypeArray{ Type::Normal },			// Lickilicky
		detail::TypeArray{ Type::Ground, Type::Rock },			// Rhyperior
		detail::TypeArray{ Type::Grass },			// Tangrowth
		detail::TypeArray{ Type::Electric },			// Electivire
		detail::TypeArray{ Type::Fire },			// Magmortar
		detail::TypeArray{ Type::Normal, Type::Flying },			// Togekiss
		detail::TypeArray{ Type::Bug, Type::Flying },			// Yanmega
		detail::TypeArray{ Type::Grass },			// Leafeon
		detail::TypeArray{ Type::Ice },			// Glaceon
		detail::TypeArray{ Type::Ground, Type::Flying },			// Gliscor
		detail::TypeArray{ Type::Ice, Type::Ground },			// Mamoswine
		detail::TypeArray{ Type::Normal },			// Porygon-Z
		detail::TypeArray{ Type::Psychic, Type::Fighting },			// Gallade
		detail::TypeArray{ Type::Rock, Type::Steel },			// Probopass
		detail::TypeArray{ Type::Ghost },			// Dusknoir
		detail::TypeArray{ Type::Ice, Type::Ghost },			// Froslass
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom
		// Electric + Fire
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom-H
		// Electric + Water
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom-W
		// Electric + Ice
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom-F
		// Electric + Flying
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom-S
		// Electric + Grass
		detail::TypeArray{ Type::Electric, Type::Ghost },			// Rotom-C
		detail::TypeArray{ Type::Psychic },			// Uxie
		detail::TypeArray{ Type::Psychic },			// Mesprit
		detail::TypeArray{ Type::Psychic },			// Azelf
		detail::TypeArray{ Type::Steel, Type::Dragon },			// Dialga
		detail::TypeArray{ Type::Water, Type::Dragon },			// Palkia
		detail::TypeArray{ Type::Fire, Type::Steel },			// Heatran
		detail::TypeArray{ Type::Normal },			// Regigigas
		detail::TypeArray{ Type::Ghost, Type::Dragon },			// Giratina-Altered
		detail::TypeArray{ Type::Ghost, Type::Dragon },			// Giratina-Origin
		detail::TypeArray{ Type::Psychic },			// Cresselia
		detail::TypeArray{ Type::Water },			// Phione
		detail::TypeArray{ Type::Water },			// Manaphy
		detail::TypeArray{ Type::Dark },			// Darkrai
		detail::TypeArray{ Type::Grass },			// Shaymin-Land
		detail::TypeArray{ Type::Grass, Type::Flying },			// Shaymin-Sky
		detail::TypeArray{ Type::Normal },			// Arceus
		
		// Generation 5
		detail::TypeArray{ Type::Psychic, Type::Fire },			// Victini
		detail::TypeArray{ Type::Grass },			// Snivy
		detail::TypeArray{ Type::Grass },			// Servine
		detail::TypeArray{ Type::Grass },			// Serperior
		detail::TypeArray{ Type::Fire },			// Tepig
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Pignite
		detail::TypeArray{ Type::Fire, Type::Fighting },			// Emboar
		detail::TypeArray{ Type::Water },			// Oshawott
		detail::TypeArray{ Type::Water },			// Dewott
		detail::TypeArray{ Type::Water },			// Samurott
		detail::TypeArray{ Type::Normal },			// Patrat
		detail::TypeArray{ Type::Normal },			// Watchog
		detail::TypeArray{ Type::Normal },			// Lillipup
		detail::TypeArray{ Type::Normal },			// Herdier
		detail::TypeArray{ Type::Normal },			// Stoutland
		detail::TypeArray{ Type::Dark },			// Purrloin
		detail::TypeArray{ Type::Dark },			// Liepard
		detail::TypeArray{ Type::Grass },			// Pansage
		detail::TypeArray{ Type::Grass },			// Simisage
		detail::TypeArray{ Type::Fire },			// Pansear
		detail::TypeArray{ Type::Fire },			// Simisear
		detail::TypeArray{ Type::Water },			// Panpour
		detail::TypeArray{ Type::Water },			// Simipour
		detail::TypeArray{ Type::Psychic },			// Munna
		detail::TypeArray{ Type::Psychic },			// Musharna
		detail::TypeArray{ Type::Normal, Type::Flying },			// Pidove
		detail::TypeArray{ Type::Normal, Type::Flying },			// Tranquill
		detail::TypeArray{ Type::Normal, Type::Flying },			// Unfezant
		detail::TypeArray{ Type::Electric },			// Blitzle
		detail::TypeArray{ Type::Electric },			// Zebstrika
		detail::TypeArray{ Type::Rock },			// Roggenrola
		detail::TypeArray{ Type::Rock },			// Boldore
		detail::TypeArray{ Type::Rock },			// Gigalith
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Woobat
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Swoobat
		detail::TypeArray{ Type::Ground },			// Drilbur
		detail::TypeArray{ Type::Ground, Type::Steel },			// Excadrill
		detail::TypeArray{ Type::Normal },			// Audino
		detail::TypeArray{ Type::Fighting },			// Timburr
		detail::TypeArray{ Type::Fighting },			// Gurdurr
		detail::TypeArray{ Type::Fighting },			// Conkeldurr
		detail::TypeArray{ Type::Water },			// Tympole
		detail::TypeArray{ Type::Water, Type::Ground },			// Palpitoad
		detail::TypeArray{ Type::Water, Type::Ground },			// Seismitoad
		detail::TypeArray{ Type::Fighting },			// Throh
		detail::TypeArray{ Type::Fighting },			// Sawk
		detail::TypeArray{ Type::Bug, Type::Grass },			// Sewaddle
		detail::TypeArray{ Type::Bug, Type::Grass },			// Swadloon
		detail::TypeArray{ Type::Bug, Type::Grass },			// Leavanny
		detail::TypeArray{ Type::Bug, Type::Poison },			// Venipede
		detail::TypeArray{ Type::Bug, Type::Poison },			// Whirlipede
		detail::TypeArray{ Type::Bug, Type::Poison },			// Scolipede
		detail::TypeArray{ Type::Grass },			// Cottonee
		detail::TypeArray{ Type::Grass },			// Whimsicott
		detail::TypeArray{ Type::Grass },			// Petilil
		detail::TypeArray{ Type::Grass },			// Lilligant
		detail::TypeArray{ Type::Water },			// Basculin-Red
		detail::TypeArray{ Type::Water },			// Basculin-Blue
		detail::TypeArray{ Type::Ground, Type::Dark },			// Sandile
		detail::TypeArray{ Type::Ground, Type::Dark },			// Krokorok
		detail::TypeArray{ Type::Ground, Type::Dark },			// Krookodile
		detail::TypeArray{ Type::Fire },			// Darumaka
		detail::TypeArray{ Type::Fire },			// Darmanitan
	//	TypeArray{ Type::Fire, Type::Psychic },			// Darmanitan (Zen Mode)
		detail::TypeArray{ Type::Grass },			// Maractus
		detail::TypeArray{ Type::Bug, Type::Rock },			// Dwebble
		detail::TypeArray{ Type::Bug, Type::Rock },			// Crustle
		detail::TypeArray{ Type::Dark, Type::Fighting },			// Scraggy
		detail::TypeArray{ Type::Dark, Type::Fighting },			// Scrafty
		detail::TypeArray{ Type::Psychic, Type::Flying },			// Sigilyph
		detail::TypeArray{ Type::Ghost },			// Yamask
		detail::TypeArray{ Type::Ghost },			// Cofagrigus
		detail::TypeArray{ Type::Water, Type::Rock },			// Tirtouga
		detail::TypeArray{ Type::Water, Type::Rock },			// Carracosta
		detail::TypeArray{ Type::Rock, Type::Flying },			// Archen
		detail::TypeArray{ Type::Rock, Type::Flying },			// Archeops
		detail::TypeArray{ Type::Poison },			// Trubbish
		detail::TypeArray{ Type::Poison },			// Garbodor
		detail::TypeArray{ Type::Dark },			// Zorua
		detail::TypeArray{ Type::Dark },			// Zoroark
		detail::TypeArray{ Type::Normal },			// Minccino
		detail::TypeArray{ Type::Normal },			// Cinccino
		detail::TypeArray{ Type::Psychic },			// Gothita
		detail::TypeArray{ Type::Psychic },			// Gothorita
		detail::TypeArray{ Type::Psychic },			// Gothitelle
		detail::TypeArray{ Type::Psychic },			// Solosis
		detail::TypeArray{ Type::Psychic },			// Duosion
		detail::TypeArray{ Type::Psychic },			// Reuniclus
		detail::TypeArray{ Type::Water, Type::Flying },			// Ducklett
		detail::TypeArray{ Type::Water, Type::Flying },			// Swanna
		detail::TypeArray{ Type::Ice },			// Vanillite
		detail::TypeArray{ Type::Ice },			// Vanillish
		detail::TypeArray{ Type::Ice },			// Vanilluxe
		detail::TypeArray{ Type::Normal, Type::Grass },			// Deerling
		detail::TypeArray{ Type::Normal, Type::Grass },			// Sawsbuck
		detail::TypeArray{ Type::Electric, Type::Flying },			// Emolga
		detail::TypeArray{ Type::Bug },			// Karrablast
		detail::TypeArray{ Type::Bug, Type::Steel },			// Escavalier
		detail::TypeArray{ Type::Grass, Type::Poison },			// Foongus
		detail::TypeArray{ Type::Grass, Type::Poison },			// Amoonguss
		detail::TypeArray{ Type::Water, Type::Ghost },			// Frillish
		detail::TypeArray{ Type::Water, Type::Ghost },			// Jellicent
		detail::TypeArray{ Type::Water },			// Alomomola
		detail::TypeArray{ Type::Bug, Type::Electric },			// Joltik
		detail::TypeArray{ Type::Bug, Type::Electric },			// Galvantula
		detail::TypeArray{ Type::Grass, Type::Steel },			// Ferroseed
		detail::TypeArray{ Type::Grass, Type::Steel },			// Ferrothorn
		detail::TypeArray{ Type::Steel },			// Klink
		detail::TypeArray{ Type::Steel },			// Klang
		detail::TypeArray{ Type::Steel },			// Klinklang
		detail::TypeArray{ Type::Electric },			// Tynamo
		detail::TypeArray{ Type::Electric },			// Eelektrik
		detail::TypeArray{ Type::Electric },			// Eelektross
		detail::TypeArray{ Type::Psychic },			// Elgyem
		detail::TypeArray{ Type::Psychic },			// Beheeyem
		detail::TypeArray{ Type::Ghost, Type::Fire },			// Litwick
		detail::TypeArray{ Type::Ghost, Type::Fire },			// Lampent
		detail::TypeArray{ Type::Ghost, Type::Fire },			// Chandelure
		detail::TypeArray{ Type::Dragon },			// Axew
		detail::TypeArray{ Type::Dragon },			// Fraxure
		detail::TypeArray{ Type::Dragon },			// Haxorus
		detail::TypeArray{ Type::Ice },			// Cubchoo
		detail::TypeArray{ Type::Ice },			// Beartic
		detail::TypeArray{ Type::Ice },			// Cryogonal
		detail::TypeArray{ Type::Bug },			// Shelmet
		detail::TypeArray{ Type::Bug },			// Accelgor
		detail::TypeArray{ Type::Ground, Type::Electric },			// Stunfisk
		detail::TypeArray{ Type::Fighting },			// Mienfoo
		detail::TypeArray{ Type::Fighting },			// Mienshao
		detail::TypeArray{ Type::Dragon },			// Druddigon
		detail::TypeArray{ Type::Ground, Type::Ghost },			// Golett
		detail::TypeArray{ Type::Ground, Type::Ghost },			// Golurk
		detail::TypeArray{ Type::Dark, Type::Steel },			// Pawniard
		detail::TypeArray{ Type::Dark, Type::Steel },			// Bisharp
		detail::TypeArray{ Type::Normal },			// Bouffalant
		detail::TypeArray{ Type::Normal, Type::Flying },			// Rufflet
		detail::TypeArray{ Type::Normal, Type::Flying },			// Braviary
		detail::TypeArray{ Type::Dark, Type::Flying },			// Vullaby
		detail::TypeArray{ Type::Dark, Type::Flying },			// Mandibuzz
		detail::TypeArray{ Type::Fire },			// Heatmor
		detail::TypeArray{ Type::Bug, Type::Steel },			// Durant
		detail::TypeArray{ Type::Dark, Type::Dragon },			// Deino
		detail::TypeArray{ Type::Dark, Type::Dragon },			// Zweilous
		detail::TypeArray{ Type::Dark, Type::Dragon },			// Hydreigon
		detail::TypeArray{ Type::Bug, Type::Fire },			// Larvesta
		detail::TypeArray{ Type::Bug, Type::Fire },			// Volcarona
		detail::TypeArray{ Type::Steel, Type::Fighting },			// Cobalion
		detail::TypeArray{ Type::Rock, Type::Fighting },			// Terrakion
		detail::TypeArray{ Type::Grass, Type::Fighting },			// Virizion
		detail::TypeArray{ Type::Flying },			// Tornadus-Incarnate
		detail::TypeArray{ Type::Flying },			// Tornadus-Therian
		detail::TypeArray{ Type::Electric, Type::Flying },			// Thundurus-Incarnate
		detail::TypeArray{ Type::Electric, Type::Flying },			// Thundurus-Therian
		detail::TypeArray{ Type::Dragon, Type::Fire },			// Reshiram
		detail::TypeArray{ Type::Dragon, Type::Electric },			// Zekrom
		detail::TypeArray{ Type::Ground, Type::Flying },			// Landorus-Incarnate
		detail::TypeArray{ Type::Ground, Type::Flying },			// Landorus-Therian
		detail::TypeArray{ Type::Dragon, Type::Ice },			// Kyurem
		detail::TypeArray{ Type::Dragon, Type::Ice },			// Kyurem-Black
		detail::TypeArray{ Type::Dragon, Type::Ice },			// Kyurem-White
		detail::TypeArray{ Type::Water, Type::Fighting },			// Keldeo
		detail::TypeArray{ Type::Normal, Type::Psychic },			// Meloetta
	//	TypeArray{ Type::Normal, Type::Fighting },			// Meloetta (Pirouette form)
		detail::TypeArray{ Type::Bug, Type::Steel }				// Genesect 
	};
	return type_array [static_cast<size_t>(name)];
}

}	// unnamed namespace
}	// namespace technicalmachine
