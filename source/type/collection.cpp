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

#include <vector>

#include "type.hpp"

#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
std::vector<Type> get_type(Species const name);
}	// unnamed namespace

TypeCollection::TypeCollection(Species const name):
	types (get_type(name)) {
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
	types = std::vector<Type>({ type });
}

bool is_type(Pokemon const & pokemon, Type const type, bool const roosting) {
	if (type != Type::Flying or !roosting) {
		for (Type const check : pokemon.type().types) {
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
	return weather.gravity() or pokemon.item().grounds();
}
bool forced_grounded(ActivePokemon const & pokemon, Weather const & weather) {
	return forced_grounded(static_cast<Pokemon const &>(pokemon), weather) or pokemon.ingrained();
}

bool is_immune_to_ground(Pokemon const & pokemon, Weather const & weather, bool const roosting = false) {
	return is_type(pokemon, Type::Flying, roosting) or pokemon.ability().is_immune_to_ground();
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

namespace {

std::vector<Type> get_type (Species const name) {
	static std::vector<Type> const type_array [] = {
		// Generation 1
		{ Type::Grass, Type::Poison },			// Bulbasaur
		{ Type::Grass, Type::Poison },			// Ivysaur
		{ Type::Grass, Type::Poison },			// Venusaur
		{ Type::Fire },			// Charmander
		{ Type::Fire },			// Charmeleon
		{ Type::Fire, Type::Flying },			// Charizard
		{ Type::Water },			// Squirtle
		{ Type::Water },			// Wartortle
		{ Type::Water },			// Blastoise
		{ Type::Bug },			// Caterpie
		{ Type::Bug },			// Metapod
		{ Type::Bug, Type::Flying },			// Butterfree
		{ Type::Bug, Type::Poison },			// Weedle
		{ Type::Bug, Type::Poison },			// Kakuna
		{ Type::Bug, Type::Poison },			// Beedrill
		{ Type::Normal, Type::Flying },			// Pidgey
		{ Type::Normal, Type::Flying },			// Pidgeotto
		{ Type::Normal, Type::Flying },			// Pidgeot
		{ Type::Normal },			// Rattata
		{ Type::Normal },			// Raticate
		{ Type::Normal, Type::Flying },			// Spearow
		{ Type::Normal, Type::Flying },			// Fearow
		{ Type::Poison },			// Ekans
		{ Type::Poison },			// Arbok
		{ Type::Electric },			// Pikachu
		{ Type::Electric },			// Raichu
		{ Type::Ground },			// Sandshrew
		{ Type::Ground },			// Sandslash
		{ Type::Poison },			// Nidoran-F
		{ Type::Poison },			// Nidorina
		{ Type::Poison, Type::Ground },			// Nidoqueen
		{ Type::Poison },			// Nidoran-M
		{ Type::Poison },			// Nidorino
		{ Type::Poison, Type::Ground },			// Nidoking
		{ Type::Normal },			// Clefairy
		{ Type::Normal },			// Clefable
		{ Type::Fire },			// Vulpix
		{ Type::Fire },			// Ninetales
		{ Type::Normal },			// Jigglypuff
		{ Type::Normal },			// Wigglytuff
		{ Type::Poison, Type::Flying },			// Zubat
		{ Type::Poison, Type::Flying },			// Golbat
		{ Type::Grass, Type::Poison },			// Oddish
		{ Type::Grass, Type::Poison },			// Gloom
		{ Type::Grass, Type::Poison },			// Vileplume
		{ Type::Bug, Type::Grass },			// Paras
		{ Type::Bug, Type::Grass },			// Parasect
		{ Type::Bug, Type::Poison },			// Venonat
		{ Type::Bug, Type::Poison },			// Venomoth
		{ Type::Ground },			// Diglett
		{ Type::Ground },			// Dugtrio
		{ Type::Normal },			// Meowth
		{ Type::Normal },			// Persian
		{ Type::Water },			// Psyduck
		{ Type::Water },			// Golduck
		{ Type::Fighting },			// Mankey
		{ Type::Fighting },			// Primeape
		{ Type::Fire },			// Growlithe
		{ Type::Fire },			// Arcanine
		{ Type::Water },			// Poliwag
		{ Type::Water },			// Poliwhirl
		{ Type::Water, Type::Fighting },			// Poliwrath
		{ Type::Psychic },			// Abra
		{ Type::Psychic },			// Kadabra
		{ Type::Psychic },			// Alakazam
		{ Type::Fighting },			// Machop
		{ Type::Fighting },			// Machoke
		{ Type::Fighting },			// Machamp
		{ Type::Grass, Type::Poison },			// Bellsprout
		{ Type::Grass, Type::Poison },			// Weepinbell
		{ Type::Grass, Type::Poison },			// Victreebel
		{ Type::Water, Type::Poison },			// Tentacool
		{ Type::Water, Type::Poison },			// Tentacruel
		{ Type::Rock, Type::Ground },			// Geodude
		{ Type::Rock, Type::Ground },			// Graveler
		{ Type::Rock, Type::Ground },			// Golem
		{ Type::Fire },			// Ponyta
		{ Type::Fire },			// Rapidash
		{ Type::Water, Type::Psychic },			// Slowpoke
		{ Type::Water, Type::Psychic },			// Slowbro
		{ Type::Electric, Type::Steel },			// Magnemite
		{ Type::Electric, Type::Steel },			// Magneton
		{ Type::Normal, Type::Flying },			// Farfetch'd
		{ Type::Normal, Type::Flying },			// Doduo
		{ Type::Normal, Type::Flying },			// Dodrio
		{ Type::Water },			// Seel
		{ Type::Water, Type::Ice },			// Dewgong
		{ Type::Poison },			// Grimer
		{ Type::Poison },			// Muk
		{ Type::Water },			// Shellder
		{ Type::Water, Type::Ice },			// Cloyster
		{ Type::Ghost, Type::Poison },			// Gastly
		{ Type::Ghost, Type::Poison },			// Haunter
		{ Type::Ghost, Type::Poison },			// Gengar
		{ Type::Rock, Type::Ground },			// Onix
		{ Type::Psychic },			// Drowzee
		{ Type::Psychic },			// Hypno
		{ Type::Water },			// Krabby
		{ Type::Water },			// Kingler
		{ Type::Electric },			// Voltorb
		{ Type::Electric },			// Electrode
		{ Type::Grass, Type::Psychic },			// Exeggcute
		{ Type::Grass, Type::Psychic },			// Exeggutor
		{ Type::Ground },			// Cubone
		{ Type::Ground },			// Marowak
		{ Type::Fighting },			// Hitmonlee
		{ Type::Fighting },			// Hitmonchan
		{ Type::Normal },			// Lickitung
		{ Type::Poison },			// Koffing
		{ Type::Poison },			// Weezing
		{ Type::Ground, Type::Rock },			// Rhyhorn
		{ Type::Ground, Type::Rock },			// Rhydon
		{ Type::Normal },			// Chansey
		{ Type::Grass },			// Tangela
		{ Type::Normal },			// Kangaskhan
		{ Type::Water },			// Horsea
		{ Type::Water },			// Seadra
		{ Type::Water },			// Goldeen
		{ Type::Water },			// Seaking
		{ Type::Water },			// Staryu
		{ Type::Water, Type::Psychic },			// Starmie
		{ Type::Psychic },			// Mr. Mime
		{ Type::Bug, Type::Flying },			// Scyther
		{ Type::Ice, Type::Psychic },			// Jynx
		{ Type::Electric },			// Electabuzz
		{ Type::Fire },			// Magmar
		{ Type::Bug },			// Pinsir
		{ Type::Normal },			// Tauros
		{ Type::Water },			// Magikarp
		{ Type::Water, Type::Flying },			// Gyarados
		{ Type::Water, Type::Ice },			// Lapras
		{ Type::Normal },			// Ditto
		{ Type::Normal },			// Eevee
		{ Type::Water },			// Vaporeon
		{ Type::Electric },			// Jolteon
		{ Type::Fire },			// Flareon
		{ Type::Normal },			// Porygon
		{ Type::Rock, Type::Water },			// Omanyte
		{ Type::Rock, Type::Water },			// Omastar
		{ Type::Rock, Type::Water },			// Kabuto
		{ Type::Rock, Type::Water },			// Kabutops
		{ Type::Rock, Type::Flying },			// Aerodactyl
		{ Type::Normal },			// Snorlax
		{ Type::Ice, Type::Flying },			// Articuno
		{ Type::Electric, Type::Flying },			// Zapdos
		{ Type::Fire, Type::Flying },			// Moltres
		{ Type::Dragon },			// Dratini
		{ Type::Dragon },			// Dragonair
		{ Type::Dragon, Type::Flying },			// Dragonite
		{ Type::Psychic },			// Mewtwo
		{ Type::Psychic },			// Mew
		
		// Generation 2
		{ Type::Grass },			// Chikorita
		{ Type::Grass },			// Bayleef
		{ Type::Grass },			// Meganium
		{ Type::Fire },			// Cyndaquil
		{ Type::Fire },			// Quilava
		{ Type::Fire },			// Typhlosion
		{ Type::Water },			// Totodile
		{ Type::Water },			// Croconaw
		{ Type::Water },			// Feraligatr
		{ Type::Normal },			// Sentret
		{ Type::Normal },			// Furret
		{ Type::Normal, Type::Flying },			// Hoothoot
		{ Type::Normal, Type::Flying },			// Noctowl
		{ Type::Bug, Type::Flying },			// Ledyba
		{ Type::Bug, Type::Flying },			// Ledian
		{ Type::Bug, Type::Poison },			// Spinarak
		{ Type::Bug, Type::Poison },			// Ariados
		{ Type::Poison, Type::Flying },			// Crobat
		{ Type::Water, Type::Electric },			// Chinchou
		{ Type::Water, Type::Electric },			// Lanturn
		{ Type::Electric },			// Pichu
		{ Type::Normal },			// Cleffa
		{ Type::Normal },			// Igglybuff
		{ Type::Normal },			// Togepi
		{ Type::Normal, Type::Flying },			// Togetic
		{ Type::Psychic, Type::Flying },			// Natu
		{ Type::Psychic, Type::Flying },			// Xatu
		{ Type::Electric },			// Mareep
		{ Type::Electric },			// Flaaffy
		{ Type::Electric },			// Ampharos
		{ Type::Grass },			// Bellossom
		{ Type::Water },			// Marill
		{ Type::Water },			// Azumarill
		{ Type::Rock },			// Sudowoodo
		{ Type::Water },			// Politoed
		{ Type::Grass, Type::Flying },			// Hoppip
		{ Type::Grass, Type::Flying },			// Skiploom
		{ Type::Grass, Type::Flying },			// Jumpluff
		{ Type::Normal },			// Aipom
		{ Type::Grass },			// Sunkern
		{ Type::Grass },			// Sunflora
		{ Type::Bug, Type::Flying },			// Yanma
		{ Type::Water, Type::Ground },			// Wooper
		{ Type::Water, Type::Ground },			// Quagsire
		{ Type::Psychic },			// Espeon
		{ Type::Dark },			// Umbreon
		{ Type::Dark, Type::Flying },			// Murkrow
		{ Type::Water, Type::Psychic },			// Slowking
		{ Type::Ghost },			// Misdreavus
		{ Type::Psychic },			// Unown
		{ Type::Psychic },			// Wobbuffet
		{ Type::Normal, Type::Psychic },			// Girafarig
		{ Type::Bug },			// Pineco
		{ Type::Bug, Type::Steel },			// Forretress
		{ Type::Normal },			// Dunsparce
		{ Type::Ground, Type::Flying },			// Gligar
		{ Type::Steel, Type::Ground },			// Steelix
		{ Type::Normal },			// Snubbull
		{ Type::Normal },			// Granbull
		{ Type::Water, Type::Poison },			// Qwilfish
		{ Type::Bug, Type::Steel },			// Scizor
		{ Type::Bug, Type::Rock },			// Shuckle
		{ Type::Bug, Type::Fighting },			// Heracross
		{ Type::Dark, Type::Ice },			// Sneasel
		{ Type::Normal },			// Teddiursa
		{ Type::Normal },			// Ursaring
		{ Type::Fire },			// Slugma
		{ Type::Fire, Type::Rock },			// Magcargo
		{ Type::Ice, Type::Ground },			// Swinub
		{ Type::Ice, Type::Ground },			// Piloswine
		{ Type::Water, Type::Rock },			// Corsola
		{ Type::Water },			// Remoraid
		{ Type::Water },			// Octillery
		{ Type::Ice, Type::Flying },			// Delibird
		{ Type::Water, Type::Flying },			// Mantine
		{ Type::Steel, Type::Flying },			// Skarmory
		{ Type::Dark, Type::Fire },			// Houndour
		{ Type::Dark, Type::Fire },			// Houndoom
		{ Type::Water, Type::Dragon },			// Kingdra
		{ Type::Ground },			// Phanpy
		{ Type::Ground },			// Donphan
		{ Type::Normal },			// Porygon2
		{ Type::Normal },			// Stantler
		{ Type::Normal },			// Smeargle
		{ Type::Fighting },			// Tyrogue
		{ Type::Fighting },			// Hitmontop
		{ Type::Ice, Type::Psychic },			// Smoochum
		{ Type::Electric },			// Elekid
		{ Type::Fire },			// Magby
		{ Type::Normal },			// Miltank
		{ Type::Normal },			// Blissey
		{ Type::Electric },			// Raikou
		{ Type::Fire },			// Entei
		{ Type::Water },			// Suicune
		{ Type::Rock, Type::Ground },			// Larvitar
		{ Type::Rock, Type::Ground },			// Pupitar
		{ Type::Rock, Type::Dark },			// Tyranitar
		{ Type::Psychic, Type::Flying },			// Lugia
		{ Type::Fire, Type::Flying },			// Ho-Oh
		{ Type::Psychic, Type::Grass },			// Celebi
		
		// Generation 3
		{ Type::Grass },			// Treecko
		{ Type::Grass },			// Grovyle
		{ Type::Grass },			// Sceptile
		{ Type::Fire },			// Torchic
		{ Type::Fire, Type::Fighting },			// Combusken
		{ Type::Fire, Type::Fighting },			// Blaziken
		{ Type::Water },			// Mudkip
		{ Type::Water, Type::Ground },			// Marshtomp
		{ Type::Water, Type::Ground },			// Swampert
		{ Type::Dark },			// Poochyena
		{ Type::Dark },			// Mightyena
		{ Type::Normal },			// Zigzagoon
		{ Type::Normal },			// Linoone
		{ Type::Bug },			// Wurmple
		{ Type::Bug },			// Silcoon
		{ Type::Bug, Type::Flying },			// Beautifly
		{ Type::Bug },			// Cascoon
		{ Type::Bug, Type::Poison },			// Dustox
		{ Type::Water, Type::Grass },			// Lotad
		{ Type::Water, Type::Grass },			// Lombre
		{ Type::Water, Type::Grass },			// Ludicolo
		{ Type::Grass },			// Seedot
		{ Type::Grass, Type::Dark },			// Nuzleaf
		{ Type::Grass, Type::Dark },			// Shiftry
		{ Type::Normal, Type::Flying },			// Taillow
		{ Type::Normal, Type::Flying },			// Swellow
		{ Type::Water, Type::Flying },			// Wingull
		{ Type::Water, Type::Flying },			// Pelipper
		{ Type::Psychic },			// Ralts
		{ Type::Psychic },			// Kirlia
		{ Type::Psychic },			// Gardevoir
		{ Type::Bug, Type::Water },			// Surskit
		{ Type::Bug, Type::Flying },			// Masquerain
		{ Type::Grass },			// Shroomish
		{ Type::Grass, Type::Fighting },			// Breloom
		{ Type::Normal },			// Slakoth
		{ Type::Normal },			// Vigoroth
		{ Type::Normal },			// Slaking
		{ Type::Bug, Type::Ground },			// Nincada
		{ Type::Bug, Type::Flying },			// Ninjask
		{ Type::Bug, Type::Ghost },			// Shedinja
		{ Type::Normal },			// Whismur
		{ Type::Normal },			// Loudred
		{ Type::Normal },			// Exploud
		{ Type::Fighting },			// Makuhita
		{ Type::Fighting },			// Hariyama
		{ Type::Normal },			// Azurill
		{ Type::Rock },			// Nosepass
		{ Type::Normal },			// Skitty
		{ Type::Normal },			// Delcatty
		{ Type::Dark, Type::Ghost },			// Sableye
		{ Type::Steel },			// Mawile
		{ Type::Steel, Type::Rock },			// Aron
		{ Type::Steel, Type::Rock },			// Lairon
		{ Type::Steel, Type::Rock },			// Aggron
		{ Type::Fighting, Type::Psychic },			// Meditite
		{ Type::Fighting, Type::Psychic },			// Medicham
		{ Type::Electric },			// Electrike
		{ Type::Electric },			// Manectric
		{ Type::Electric },			// Plusle
		{ Type::Electric },			// Minun
		{ Type::Bug },			// Volbeat
		{ Type::Bug },			// Illumise
		{ Type::Grass, Type::Poison },			// Roselia
		{ Type::Poison },			// Gulpin
		{ Type::Poison },			// Swalot
		{ Type::Water, Type::Dark },			// Carvanha
		{ Type::Water, Type::Dark },			// Sharpedo
		{ Type::Water },			// Wailmer
		{ Type::Water },			// Wailord
		{ Type::Fire, Type::Ground },			// Numel
		{ Type::Fire, Type::Ground },			// Camerupt
		{ Type::Fire },			// Torkoal
		{ Type::Psychic },			// Spoink
		{ Type::Psychic },			// Grumpig
		{ Type::Normal },			// Spinda
		{ Type::Ground },			// Trapinch
		{ Type::Ground, Type::Dragon },			// Vibrava
		{ Type::Ground, Type::Dragon },			// Flygon
		{ Type::Grass },			// Cacnea
		{ Type::Grass, Type::Dark },			// Cacturne
		{ Type::Normal, Type::Flying },			// Swablu
		{ Type::Dragon, Type::Flying },			// Altaria
		{ Type::Normal },			// Zangoose
		{ Type::Poison },			// Seviper
		{ Type::Rock, Type::Psychic },			// Lunatone
		{ Type::Rock, Type::Psychic },			// Solrock
		{ Type::Water, Type::Ground },			// Barboach
		{ Type::Water, Type::Ground },			// Whiscash
		{ Type::Water },			// Corphish
		{ Type::Water, Type::Dark },			// Crawdaunt
		{ Type::Ground, Type::Psychic },			// Baltoy
		{ Type::Ground, Type::Psychic },			// Claydol
		{ Type::Rock, Type::Grass },			// Lileep
		{ Type::Rock, Type::Grass },			// Cradily
		{ Type::Rock, Type::Bug },			// Anorith
		{ Type::Rock, Type::Bug },			// Armaldo
		{ Type::Water },			// Feebas
		{ Type::Water },			// Milotic
		{ Type::Normal },			// Castform
	//	{ Type::Fire },			// --------------------
	//	{ Type::Water },			// --------------------
	//	{ Type::Ice },			// --------------------
		{ Type::Normal },			// Kecleon
		{ Type::Ghost },			// Shuppet
		{ Type::Ghost },			// Banette
		{ Type::Ghost },			// Duskull
		{ Type::Ghost },			// Dusclops
		{ Type::Grass, Type::Flying },			// Tropius
		{ Type::Psychic },			// Chimecho
		{ Type::Dark },			// Absol
		{ Type::Psychic },			// Wynaut
		{ Type::Ice },			// Snorunt
		{ Type::Ice },			// Glalie
		{ Type::Ice, Type::Water },			// Spheal
		{ Type::Ice, Type::Water },			// Sealeo
		{ Type::Ice, Type::Water },			// Walrein
		{ Type::Water },			// Clamperl
		{ Type::Water },			// Huntail
		{ Type::Water },			// Gorebyss
		{ Type::Water, Type::Rock },			// Relicanth
		{ Type::Water },			// Luvdisc
		{ Type::Dragon },			// Bagon
		{ Type::Dragon },			// Shelgon
		{ Type::Dragon, Type::Flying },			// Salamence
		{ Type::Steel, Type::Psychic },			// Beldum
		{ Type::Steel, Type::Psychic },			// Metang
		{ Type::Steel, Type::Psychic },			// Metagross
		{ Type::Rock },			// Regirock
		{ Type::Ice },			// Regice
		{ Type::Steel },			// Registeel
		{ Type::Dragon, Type::Psychic },			// Latias
		{ Type::Dragon, Type::Psychic },			// Latios
		{ Type::Water },			// Kyogre
		{ Type::Ground },			// Groudon
		{ Type::Dragon, Type::Flying },			// Rayquaza
		{ Type::Steel, Type::Psychic },			// Jirachi
		{ Type::Psychic },			// Deoxys-Mediocre
		{ Type::Psychic },			// Deoxys-Attack
		{ Type::Psychic },			// Deoxys-Defense
		{ Type::Psychic },			// Deoxys-Speed
		
		// Generation 4
		{ Type::Grass },			// Turtwig
		{ Type::Grass },			// Grotle
		{ Type::Grass, Type::Ground },			// Torterra
		{ Type::Fire },			// Chimchar
		{ Type::Fire, Type::Fighting },			// Monferno
		{ Type::Fire, Type::Fighting },			// Infernape
		{ Type::Water },			// Piplup
		{ Type::Water },			// Prinplup
		{ Type::Water, Type::Steel },			// Empoleon
		{ Type::Normal, Type::Flying },			// Starly
		{ Type::Normal, Type::Flying },			// Staravia
		{ Type::Normal, Type::Flying },			// Staraptor
		{ Type::Normal },			// Bidoof
		{ Type::Normal, Type::Water },			// Bibarel
		{ Type::Bug },			// Kricketot
		{ Type::Bug },			// Kricketune
		{ Type::Electric },			// Shinx
		{ Type::Electric },			// Luxio
		{ Type::Electric },			// Luxray
		{ Type::Grass, Type::Poison },			// Budew
		{ Type::Grass, Type::Poison },			// Roserade
		{ Type::Rock },			// Cranidos
		{ Type::Rock },			// Rampardos
		{ Type::Rock, Type::Steel },			// Shieldon
		{ Type::Rock, Type::Steel },			// Bastiodon
		{ Type::Bug },			// Burmy
		{ Type::Bug, Type::Grass },			// Wormadam-Plant
		{ Type::Bug, Type::Ground },			// Wormadam-Sandy
		{ Type::Bug, Type::Steel },			// Wormadam-Trash
		{ Type::Bug, Type::Flying },			// Mothim
		{ Type::Bug, Type::Flying },			// Combee
		{ Type::Bug, Type::Flying },			// Vespiquen
		{ Type::Electric },			// Pachirisu
		{ Type::Water },			// Buizel
		{ Type::Water },			// Floatzel
		{ Type::Grass },			// Cherubi
		{ Type::Grass },			// Cherrim
		{ Type::Water },			// Shellos
		{ Type::Water, Type::Ground },			// Gastrodon
		{ Type::Normal },			// Ambipom
		{ Type::Ghost, Type::Flying },			// Drifloon
		{ Type::Ghost, Type::Flying },			// Drifblim
		{ Type::Normal },			// Buneary
		{ Type::Normal },			// Lopunny
		{ Type::Ghost },			// Mismagius
		{ Type::Dark, Type::Flying },			// Honchkrow
		{ Type::Normal },			// Glameow
		{ Type::Normal },			// Purugly
		{ Type::Psychic },			// Chingling
		{ Type::Poison, Type::Dark },			// Stunky
		{ Type::Poison, Type::Dark },			// Skuntank
		{ Type::Steel, Type::Psychic },			// Bronzor
		{ Type::Steel, Type::Psychic },			// Bronzong
		{ Type::Rock },			// Bonsly
		{ Type::Psychic },			// Mime Jr.
		{ Type::Normal },			// Happiny
		{ Type::Normal, Type::Flying },			// Chatot
		{ Type::Ghost, Type::Dark },			// Spiritomb
		{ Type::Dragon, Type::Ground },			// Gible
		{ Type::Dragon, Type::Ground },			// Gabite
		{ Type::Dragon, Type::Ground },			// Garchomp
		{ Type::Normal },			// Munchlax
		{ Type::Fighting },			// Riolu
		{ Type::Fighting, Type::Steel },			// Lucario
		{ Type::Ground },			// Hippopotas
		{ Type::Ground },			// Hippowdon
		{ Type::Poison, Type::Bug },			// Skorupi
		{ Type::Poison, Type::Dark },			// Drapion
		{ Type::Poison, Type::Fighting },			// Croagunk
		{ Type::Poison, Type::Fighting },			// Toxicroak
		{ Type::Grass },			// Carnivine
		{ Type::Water },			// Finneon
		{ Type::Water },			// Lumineon
		{ Type::Water, Type::Flying },			// Mantyke
		{ Type::Grass, Type::Ice },			// Snover
		{ Type::Grass, Type::Ice },			// Abomasnow
		{ Type::Dark, Type::Ice },			// Weavile
		{ Type::Electric, Type::Steel },			// Magnezone
		{ Type::Normal },			// Lickilicky
		{ Type::Ground, Type::Rock },			// Rhyperior
		{ Type::Grass },			// Tangrowth
		{ Type::Electric },			// Electivire
		{ Type::Fire },			// Magmortar
		{ Type::Normal, Type::Flying },			// Togekiss
		{ Type::Bug, Type::Flying },			// Yanmega
		{ Type::Grass },			// Leafeon
		{ Type::Ice },			// Glaceon
		{ Type::Ground, Type::Flying },			// Gliscor
		{ Type::Ice, Type::Ground },			// Mamoswine
		{ Type::Normal },			// Porygon-Z
		{ Type::Psychic, Type::Fighting },			// Gallade
		{ Type::Rock, Type::Steel },			// Probopass
		{ Type::Ghost },			// Dusknoir
		{ Type::Ice, Type::Ghost },			// Froslass
		{ Type::Electric, Type::Ghost },			// Rotom
		// Electric + Fire
		{ Type::Electric, Type::Ghost },			// Rotom-H
		// Electric + Water
		{ Type::Electric, Type::Ghost },			// Rotom-W
		// Electric + Ice
		{ Type::Electric, Type::Ghost },			// Rotom-F
		// Electric + Flying
		{ Type::Electric, Type::Ghost },			// Rotom-S
		// Electric + Grass
		{ Type::Electric, Type::Ghost },			// Rotom-C
		{ Type::Psychic },			// Uxie
		{ Type::Psychic },			// Mesprit
		{ Type::Psychic },			// Azelf
		{ Type::Steel, Type::Dragon },			// Dialga
		{ Type::Water, Type::Dragon },			// Palkia
		{ Type::Fire, Type::Steel },			// Heatran
		{ Type::Normal },			// Regigigas
		{ Type::Ghost, Type::Dragon },			// Giratina-Altered
		{ Type::Ghost, Type::Dragon },			// Giratina-Origin
		{ Type::Psychic },			// Cresselia
		{ Type::Water },			// Phione
		{ Type::Water },			// Manaphy
		{ Type::Dark },			// Darkrai
		{ Type::Grass },			// Shaymin-Land
		{ Type::Grass, Type::Flying },			// Shaymin-Sky
		{ Type::Normal },			// Arceus
		
		// Generation 5
		{ Type::Psychic, Type::Fire },			// Victini
		{ Type::Grass },			// Snivy
		{ Type::Grass },			// Servine
		{ Type::Grass },			// Serperior
		{ Type::Fire },			// Tepig
		{ Type::Fire, Type::Fighting },			// Pignite
		{ Type::Fire, Type::Fighting },			// Emboar
		{ Type::Water },			// Oshawott
		{ Type::Water },			// Dewott
		{ Type::Water },			// Samurott
		{ Type::Normal },			// Patrat
		{ Type::Normal },			// Watchog
		{ Type::Normal },			// Lillipup
		{ Type::Normal },			// Herdier
		{ Type::Normal },			// Stoutland
		{ Type::Dark },			// Purrloin
		{ Type::Dark },			// Liepard
		{ Type::Grass },			// Pansage
		{ Type::Grass },			// Simisage
		{ Type::Fire },			// Pansear
		{ Type::Fire },			// Simisear
		{ Type::Water },			// Panpour
		{ Type::Water },			// Simipour
		{ Type::Psychic },			// Munna
		{ Type::Psychic },			// Musharna
		{ Type::Normal, Type::Flying },			// Pidove
		{ Type::Normal, Type::Flying },			// Tranquill
		{ Type::Normal, Type::Flying },			// Unfezant
		{ Type::Electric },			// Blitzle
		{ Type::Electric },			// Zebstrika
		{ Type::Rock },			// Roggenrola
		{ Type::Rock },			// Boldore
		{ Type::Rock },			// Gigalith
		{ Type::Psychic, Type::Flying },			// Woobat
		{ Type::Psychic, Type::Flying },			// Swoobat
		{ Type::Ground },			// Drilbur
		{ Type::Ground, Type::Steel },			// Excadrill
		{ Type::Normal },			// Audino
		{ Type::Fighting },			// Timburr
		{ Type::Fighting },			// Gurdurr
		{ Type::Fighting },			// Conkeldurr
		{ Type::Water },			// Tympole
		{ Type::Water, Type::Ground },			// Palpitoad
		{ Type::Water, Type::Ground },			// Seismitoad
		{ Type::Fighting },			// Throh
		{ Type::Fighting },			// Sawk
		{ Type::Bug, Type::Grass },			// Sewaddle
		{ Type::Bug, Type::Grass },			// Swadloon
		{ Type::Bug, Type::Grass },			// Leavanny
		{ Type::Bug, Type::Poison },			// Venipede
		{ Type::Bug, Type::Poison },			// Whirlipede
		{ Type::Bug, Type::Poison },			// Scolipede
		{ Type::Grass },			// Cottonee
		{ Type::Grass },			// Whimsicott
		{ Type::Grass },			// Petilil
		{ Type::Grass },			// Lilligant
		{ Type::Water },			// Basculin-Red
		{ Type::Water },			// Basculin-Blue
		{ Type::Ground, Type::Dark },			// Sandile
		{ Type::Ground, Type::Dark },			// Krokorok
		{ Type::Ground, Type::Dark },			// Krookodile
		{ Type::Fire },			// Darumaka
		{ Type::Fire },			// Darmanitan
	//	{ Type::Fire, Type::Psychic },			// Darmanitan (Zen Mode)
		{ Type::Grass },			// Maractus
		{ Type::Bug, Type::Rock },			// Dwebble
		{ Type::Bug, Type::Rock },			// Crustle
		{ Type::Dark, Type::Fighting },			// Scraggy
		{ Type::Dark, Type::Fighting },			// Scrafty
		{ Type::Psychic, Type::Flying },			// Sigilyph
		{ Type::Ghost },			// Yamask
		{ Type::Ghost },			// Cofagrigus
		{ Type::Water, Type::Rock },			// Tirtouga
		{ Type::Water, Type::Rock },			// Carracosta
		{ Type::Rock, Type::Flying },			// Archen
		{ Type::Rock, Type::Flying },			// Archeops
		{ Type::Poison },			// Trubbish
		{ Type::Poison },			// Garbodor
		{ Type::Dark },			// Zorua
		{ Type::Dark },			// Zoroark
		{ Type::Normal },			// Minccino
		{ Type::Normal },			// Cinccino
		{ Type::Psychic },			// Gothita
		{ Type::Psychic },			// Gothorita
		{ Type::Psychic },			// Gothitelle
		{ Type::Psychic },			// Solosis
		{ Type::Psychic },			// Duosion
		{ Type::Psychic },			// Reuniclus
		{ Type::Water, Type::Flying },			// Ducklett
		{ Type::Water, Type::Flying },			// Swanna
		{ Type::Ice },			// Vanillite
		{ Type::Ice },			// Vanillish
		{ Type::Ice },			// Vanilluxe
		{ Type::Normal, Type::Grass },			// Deerling
		{ Type::Normal, Type::Grass },			// Sawsbuck
		{ Type::Electric, Type::Flying },			// Emolga
		{ Type::Bug },			// Karrablast
		{ Type::Bug, Type::Steel },			// Escavalier
		{ Type::Grass, Type::Poison },			// Foongus
		{ Type::Grass, Type::Poison },			// Amoonguss
		{ Type::Water, Type::Ghost },			// Frillish
		{ Type::Water, Type::Ghost },			// Jellicent
		{ Type::Water },			// Alomomola
		{ Type::Bug, Type::Electric },			// Joltik
		{ Type::Bug, Type::Electric },			// Galvantula
		{ Type::Grass, Type::Steel },			// Ferroseed
		{ Type::Grass, Type::Steel },			// Ferrothorn
		{ Type::Steel },			// Klink
		{ Type::Steel },			// Klang
		{ Type::Steel },			// Klinklang
		{ Type::Electric },			// Tynamo
		{ Type::Electric },			// Eelektrik
		{ Type::Electric },			// Eelektross
		{ Type::Psychic },			// Elgyem
		{ Type::Psychic },			// Beheeyem
		{ Type::Ghost, Type::Fire },			// Litwick
		{ Type::Ghost, Type::Fire },			// Lampent
		{ Type::Ghost, Type::Fire },			// Chandelure
		{ Type::Dragon },			// Axew
		{ Type::Dragon },			// Fraxure
		{ Type::Dragon },			// Haxorus
		{ Type::Ice },			// Cubchoo
		{ Type::Ice },			// Beartic
		{ Type::Ice },			// Cryogonal
		{ Type::Bug },			// Shelmet
		{ Type::Bug },			// Accelgor
		{ Type::Ground, Type::Electric },			// Stunfisk
		{ Type::Fighting },			// Mienfoo
		{ Type::Fighting },			// Mienshao
		{ Type::Dragon },			// Druddigon
		{ Type::Ground, Type::Ghost },			// Golett
		{ Type::Ground, Type::Ghost },			// Golurk
		{ Type::Dark, Type::Steel },			// Pawniard
		{ Type::Dark, Type::Steel },			// Bisharp
		{ Type::Normal },			// Bouffalant
		{ Type::Normal, Type::Flying },			// Rufflet
		{ Type::Normal, Type::Flying },			// Braviary
		{ Type::Dark, Type::Flying },			// Vullaby
		{ Type::Dark, Type::Flying },			// Mandibuzz
		{ Type::Fire },			// Heatmor
		{ Type::Bug, Type::Steel },			// Durant
		{ Type::Dark, Type::Dragon },			// Deino
		{ Type::Dark, Type::Dragon },			// Zweilous
		{ Type::Dark, Type::Dragon },			// Hydreigon
		{ Type::Bug, Type::Fire },			// Larvesta
		{ Type::Bug, Type::Fire },			// Volcarona
		{ Type::Steel, Type::Fighting },			// Cobalion
		{ Type::Rock, Type::Fighting },			// Terrakion
		{ Type::Grass, Type::Fighting },			// Virizion
		{ Type::Flying },			// Tornadus-Incarnate
		{ Type::Flying },			// Tornadus-Therian
		{ Type::Electric, Type::Flying },			// Thundurus-Incarnate
		{ Type::Electric, Type::Flying },			// Thundurus-Therian
		{ Type::Dragon, Type::Fire },			// Reshiram
		{ Type::Dragon, Type::Electric },			// Zekrom
		{ Type::Ground, Type::Flying },			// Landorus-Incarnate
		{ Type::Ground, Type::Flying },			// Landorus-Therian
		{ Type::Dragon, Type::Ice },			// Kyurem
		{ Type::Dragon, Type::Ice },			// Kyurem-Black
		{ Type::Dragon, Type::Ice },			// Kyurem-White
		{ Type::Water, Type::Fighting },			// Keldeo
		{ Type::Normal, Type::Psychic },			// Meloetta
	//	{ Type::Normal, Type::Fighting },			// Meloetta (Pirouette form)
		{ Type::Bug, Type::Steel }				// Genesect 
	};
	return type_array [static_cast<size_t>(name)];
}

}	// unnamed namespace
}	// namespace technicalmachine
