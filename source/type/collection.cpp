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

#include "../team.hpp"
#include "../weather.hpp"

#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
enum class Species : uint16_t;
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

bool is_type (Team const & team, Type const type) {
	if (type != Type::FLYING or !team.is_roosting()) {
		for (Type const check : team.pokemon().type.types) {
			if (check == type)
				return true;
		}
	}
	return false;
}

bool grounded (Team const & team, Pokemon const & pokemon, Weather const & weather) {
	return !(is_type(team, Type::FLYING) or pokemon.ability.is_immune_to_ground() or team.magnet_rise_is_active()) or weather.gravity() or pokemon.item.grounds() or team.ingrained();
}

namespace {

std::vector<Type> get_type (Species const name) {
	static std::vector<Type> const type_array [] = {
		{ Type::GRASS, Type::ICE },				// Abomasnow
		{ Type::PSYCHIC },		// Abra
		{ Type::DARK },			// Absol
		{ Type::ROCK, Type::FLYING },			// Aerodactyl
		{ Type::STEEL, Type::ROCK },			// Aggron
		{ Type::NORMAL },		// Aipom
		{ Type::PSYCHIC },		// Alakazam
		{ Type::DRAGON, Type::FLYING },			// Altaria
		{ Type::NORMAL },		// Ambipom
		{ Type::ELECTRIC },		// Ampharos
		{ Type::ROCK, Type::BUG },				// Anorith
		{ Type::POISON },		// Arbok
		{ Type::FIRE },			// Arcanine
		{ Type::NORMAL },		// Arceus
		{ Type::BUG, Type::POISON },			// Ariados
		{ Type::ROCK, Type::BUG },				// Armaldo
		{ Type::STEEL, Type::ROCK },			// Aron
		{ Type::ICE, Type::FLYING },			// Articuno
		{ Type::PSYCHIC },		// Azelf
		{ Type::WATER },		// Azumarill
		{ Type::NORMAL },		// Azurill
		{ Type::DRAGON },		// Bagon
		{ Type::GROUND, Type::PSYCHIC },		// Baltoy
		{ Type::GHOST },		// Banette
		{ Type::WATER, Type::GROUND },			// Barboach
		{ Type::ROCK, Type::STEEL },			// Bastiodon
		{ Type::GRASS },		// Bayleef
		{ Type::BUG, Type::FLYING },			// Beautifly
		{ Type::BUG, Type::POISON },			// Beedrill
		{ Type::STEEL, Type::PSYCHIC },			// Beldum
		{ Type::GRASS },		// Bellossom
		{ Type::GRASS, Type::POISON },			// Bellsprout
		{ Type::NORMAL, Type::WATER },			// Bibarel
		{ Type::NORMAL },		// Bidoof
		{ Type::WATER },		// Blastoise
		{ Type::FIRE, Type::FIGHTING },			// Blaziken
		{ Type::NORMAL },		// Blissey
		{ Type::ROCK },			// Bonsly
		{ Type::GRASS, Type::FIGHTING },		// Breloom
		{ Type::STEEL, Type::PSYCHIC },			// Bronzong
		{ Type::STEEL, Type::PSYCHIC },			// Bronzor
		{ Type::GRASS, Type::POISON },			// Budew
		{ Type::WATER },		// Buizel
		{ Type::GRASS, Type::POISON },			// Bulbasaur
		{ Type::NORMAL },		// Buneary
		{ Type::BUG },			// Burmy
		{ Type::BUG, Type::FLYING },			// Butterfree
		{ Type::GRASS },		// Cacnea
		{ Type::GRASS, Type::DARK },			// Cacturne
		{ Type::FIRE, Type::GROUND },			// Camerupt
		{ Type::GRASS },		// Carnivine
		{ Type::WATER, Type::DARK },			// Carvanha
		{ Type::BUG },			// Cascoon
		{ Type::NORMAL },		// Castform
		{ Type::BUG },			// Caterpie
		{ Type::PSYCHIC, Type::GRASS },			// Celebi
		{ Type::NORMAL },		// Chansey
		{ Type::FIRE, Type::FLYING },			// Charizard
		{ Type::FIRE },			// Charmander
		{ Type::FIRE },			// Charmeleon
		{ Type::NORMAL, Type::FLYING },			// Chatot
		{ Type::GRASS },		// Cherrim
		{ Type::GRASS },		// Cherubi
		{ Type::GRASS },		// Chikorita
		{ Type::FIRE },			// Chimchar
		{ Type::PSYCHIC },		// Chimecho
		{ Type::WATER, Type::ELECTRIC },		// Chinchou
		{ Type::PSYCHIC },		// Chingling
		{ Type::WATER },		// Clamperl
		{ Type::GROUND, Type::PSYCHIC },		// Claydol
		{ Type::NORMAL },		// Clefable
		{ Type::NORMAL },		// Clefairy
		{ Type::NORMAL },		// Cleffa
		{ Type::WATER, Type::ICE },				// Cloyster
		{ Type::BUG, Type::FLYING },			// Combee
		{ Type::FIRE, Type::FIGHTING },			// Combusken
		{ Type::WATER },		// Corphish
		{ Type::WATER, Type::ROCK },			// Corsola
		{ Type::ROCK, Type::GRASS },			// Cradily
		{ Type::ROCK },			// Cranidos
		{ Type::WATER, Type::DARK },			// Crawdaunt
		{ Type::PSYCHIC },		// Cresselia
		{ Type::POISON, Type::FIGHTING },		// Croagunk
		{ Type::POISON, Type::FLYING },			// Crobat
		{ Type::WATER },		// Croconaw
		{ Type::GROUND },		// Cubone
		{ Type::FIRE },			// Cyndaquil
		{ Type::DARK },			// Darkrai
		{ Type::NORMAL },		// Delcatty
		{ Type::ICE, Type::FLYING },			// Delibird
		{ Type::PSYCHIC },		// Deoxys-A
		{ Type::PSYCHIC },		// Deoxys-D
		{ Type::PSYCHIC },		// Deoxys-M
		{ Type::PSYCHIC },		// Deoxys-S
		{ Type::WATER, Type::ICE },				// Dewgong
		{ Type::STEEL, Type::DRAGON },			// Dialga
		{ Type::GROUND },		// Diglett
		{ Type::NORMAL },		// Ditto
		{ Type::NORMAL, Type::FLYING },			// Dodrio
		{ Type::NORMAL, Type::FLYING },			// Doduo
		{ Type::GROUND },		// Donphan
		{ Type::DRAGON },		// Dragonair
		{ Type::DRAGON, Type::FLYING },			// Dragonite
		{ Type::POISON, Type::DARK },			// Drapion
		{ Type::DRAGON },		// Dratini
		{ Type::GHOST, Type::FLYING },			// Drifblim
		{ Type::GHOST, Type::FLYING },			// Drifloon
		{ Type::PSYCHIC },		// Drowzee
		{ Type::GROUND },		// Dugtrio
		{ Type::NORMAL },		// Dunsparce
		{ Type::GHOST },		// Dusclops
		{ Type::GHOST },		// Dusknoir
		{ Type::GHOST },		// Duskull
		{ Type::BUG, Type::POISON },			// Dustox
		{ Type::NORMAL },		// Eevee
		{ Type::POISON },		// Ekans
		{ Type::ELECTRIC },		// Electabuzz
		{ Type::ELECTRIC },		// Electivire
		{ Type::ELECTRIC },		// Electrike
		{ Type::ELECTRIC },		// Electrode
		{ Type::ELECTRIC },		// Elekid
		{ Type::WATER, Type::STEEL },			// Empoleon
		{ Type::FIRE },			// Entei
		{ Type::PSYCHIC },		// Espeon
		{ Type::GRASS, Type::PSYCHIC },			// Exeggcute
		{ Type::GRASS, Type::PSYCHIC },			// Exeggutor
		{ Type::NORMAL },		// Exploud
		{ Type::NORMAL, Type::FLYING },			// Farfetch'd
		{ Type::NORMAL, Type::FLYING },			// Fearow
		{ Type::WATER },		// Feebas
		{ Type::WATER },		// Feraligatr
		{ Type::WATER },		// Finneon
		{ Type::ELECTRIC },		// Flaaffy
		{ Type::FIRE },			// Flareon
		{ Type::WATER },		// Floatzel
		{ Type::GROUND, Type::DRAGON },			// Flygon
		{ Type::BUG, Type::STEEL },				// Forretress
		{ Type::ICE, Type::GHOST },				// Froslass
		{ Type::NORMAL },		// Furret
		{ Type::DRAGON, Type::GROUND },			// Gabite
		{ Type::PSYCHIC, Type::FIGHTING },		// Gallade
		{ Type::DRAGON, Type::GROUND },			// Garchomp
		{ Type::PSYCHIC },		// Gardevoir
		{ Type::GHOST, Type::POISON },			// Gastly
		{ Type::WATER, Type::GROUND },			// Gastrodon
		{ Type::GHOST, Type::POISON },			// Gengar
		{ Type::ROCK, Type::GROUND },			// Geodude
		{ Type::DRAGON, Type::GROUND },			// Gible
		{ Type::NORMAL, Type::PSYCHIC },		// Girafarig
		{ Type::GHOST, Type::DRAGON },			// Giratina-A
		{ Type::GHOST, Type::DRAGON },			// Giratina-O
		{ Type::ICE },			// Glaceon
		{ Type::ICE },			// Glalie
		{ Type::NORMAL },		// Glameow
		{ Type::GROUND, Type::FLYING },			// Gligar
		{ Type::GROUND, Type::FLYING },			// Gliscor
		{ Type::GRASS, Type::POISON },			// Gloom
		{ Type::POISON, Type::FLYING },			// Golbat
		{ Type::WATER },		// Goldeen
		{ Type::WATER },		// Golduck
		{ Type::ROCK, Type::GROUND },			// Golem
		{ Type::WATER },		// Gorebyss
		{ Type::NORMAL },		// Granbull
		{ Type::ROCK, Type::GROUND },			// Graveler
		{ Type::POISON },		// Grimer
		{ Type::GRASS },		// Grotle
		{ Type::GROUND },		// Groudon
		{ Type::GRASS },		// Grovyle
		{ Type::FIRE },			// Growlithe
		{ Type::PSYCHIC },		// Grumpig
		{ Type::POISON },		// Gulpin
		{ Type::WATER, Type::FLYING },			// Gyarados
		{ Type::NORMAL },		// Happiny
		{ Type::FIGHTING },		// Hariyama
		{ Type::GHOST, Type::POISON },			// Haunter
		{ Type::FIRE, Type::STEEL },			// Heatran
		{ Type::BUG, Type::FIGHTING },			// Heracross
		{ Type::GROUND },		// Hippopotas
		{ Type::GROUND },		// Hippowdon
		{ Type::FIGHTING },		// Hitmonchan
		{ Type::FIGHTING },		// Hitmonlee
		{ Type::FIGHTING },		// Hitmontop
		{ Type::FIRE, Type::FLYING },			// Ho-Oh
		{ Type::DARK, Type::FLYING },			// Honchkrow
		{ Type::NORMAL, Type::FLYING },			// Hoothoot
		{ Type::GRASS, Type::FLYING },			// Hoppip
		{ Type::WATER },		// Horsea
		{ Type::DARK, Type::FIRE },				// Houndoom
		{ Type::DARK, Type::FIRE },				// Houndour
		{ Type::WATER },		// Huntail
		{ Type::PSYCHIC },		// Hypno
		{ Type::NORMAL },		// Igglybuff
		{ Type::BUG },			// Illumise
		{ Type::FIRE, Type::FIGHTING },			// Infernape
		{ Type::GRASS, Type::POISON },			// Ivysaur
		{ Type::NORMAL },		// Jigglypuff
		{ Type::STEEL, Type::PSYCHIC },			// Jirachi
		{ Type::ELECTRIC },		// Jolteon
		{ Type::GRASS, Type::FLYING },			// Jumpluff
		{ Type::ICE, Type::PSYCHIC },			// Jynx
		{ Type::ROCK, Type::WATER },			// Kabuto
		{ Type::ROCK, Type::WATER },			// Kabutops
		{ Type::PSYCHIC },		// Kadabra
		{ Type::BUG, Type::POISON },			// Kakuna
		{ Type::NORMAL },		// Kangaskhan
		{ Type::NORMAL },		// Kecleon
		{ Type::WATER, Type::DRAGON },			// Kingdra
		{ Type::WATER },		// Kingler
		{ Type::PSYCHIC },		// Kirlia
		{ Type::POISON },		// Koffing
		{ Type::WATER },		// Krabby
		{ Type::BUG },			// Kricketot
		{ Type::BUG },			// Kricketune
		{ Type::WATER },		// Kyogre
		{ Type::STEEL, Type::ROCK },			// Lairon
		{ Type::WATER, Type::ELECTRIC },		// Lanturn
		{ Type::WATER, Type::ICE },				// Lapras
		{ Type::ROCK, Type::GROUND },			// Larvitar
		{ Type::DRAGON, Type::PSYCHIC },		// Latias
		{ Type::DRAGON, Type::PSYCHIC },		// Latios
		{ Type::GRASS },		// Leafeon
		{ Type::BUG, Type::FLYING },			// Ledian
		{ Type::BUG, Type::FLYING },			// Ledyba
		{ Type::NORMAL },		// Lickilicky
		{ Type::NORMAL },		// Lickitung
		{ Type::ROCK, Type::GRASS },			// Lileep
		{ Type::NORMAL },		// Linoone
		{ Type::WATER, Type::GRASS },			// Lombre
		{ Type::NORMAL },		// Lopunny
		{ Type::WATER, Type::GRASS },			// Lotad
		{ Type::NORMAL },		// Loudred
		{ Type::FIGHTING, Type::STEEL },		// Lucario
		{ Type::WATER, Type::GRASS },			// Ludicolo
		{ Type::PSYCHIC, Type::FLYING },		// Lugia
		{ Type::WATER },		// Lumineon
		{ Type::ROCK, Type::PSYCHIC },			// Lunatone
		{ Type::WATER },		// Luvdisc
		{ Type::ELECTRIC },		// Luxio
		{ Type::ELECTRIC },		// Luxray
		{ Type::FIGHTING },		// Machamp
		{ Type::FIGHTING },		// Machoke
		{ Type::FIGHTING },		// Machop
		{ Type::FIRE },			// Magby
		{ Type::FIRE, Type::ROCK },				// Magcargo
		{ Type::WATER },		// Magikarp
		{ Type::FIRE },			// Magmar
		{ Type::FIRE },			// Magmortar
		{ Type::ELECTRIC, Type::STEEL },		// Magnemite
		{ Type::ELECTRIC, Type::STEEL },		// Magneton
		{ Type::ELECTRIC, Type::STEEL },		// Magnezone
		{ Type::FIGHTING },		// Makuhita
		{ Type::ICE, Type::GROUND },			// Mamoswine
		{ Type::WATER },		// Manaphy
		{ Type::ELECTRIC },		// Manectric
		{ Type::FIGHTING },		// Mankey
		{ Type::WATER, Type::FLYING },			// Mantine
		{ Type::WATER, Type::FLYING },			// Mantyke
		{ Type::ELECTRIC },		// Mareep
		{ Type::WATER },		// Marill
		{ Type::GROUND },		// Marowak
		{ Type::WATER, Type::GROUND },			// Marshtomp
		{ Type::BUG, Type::FLYING },			// Masquerain
		{ Type::STEEL },		// Mawile
		{ Type::FIGHTING, Type::PSYCHIC },		// Medicham
		{ Type::FIGHTING, Type::PSYCHIC },		// Meditite
		{ Type::GRASS },		// Meganium
		{ Type::NORMAL },		// Meowth
		{ Type::PSYCHIC },		// Mesprit
		{ Type::STEEL, Type::PSYCHIC },			// Metagross
		{ Type::STEEL, Type::PSYCHIC },			// Metang
		{ Type::BUG },			// Metapod
		{ Type::PSYCHIC },		// Mew
		{ Type::PSYCHIC },		// Mewtwo
		{ Type::DARK },			// Mightyena
		{ Type::WATER },		// Milotic
		{ Type::NORMAL },		// Miltank
		{ Type::PSYCHIC },		// Mime Jr.
		{ Type::ELECTRIC },		// Minun
		{ Type::GHOST },		// Misdreavus
		{ Type::GHOST },		// Mismagius
		{ Type::FIRE, Type::FLYING },			// Moltres
		{ Type::FIRE, Type::FIGHTING },			// Monferno
		{ Type::BUG, Type::FLYING },			// Mothim
		{ Type::PSYCHIC },		// Mr. Mime
		{ Type::WATER },		// Mudkip
		{ Type::POISON },		// Muk
		{ Type::NORMAL },		// Munchlax
		{ Type::DARK, Type::FLYING },			// Murkrow
		{ Type::PSYCHIC, Type::FLYING },		// Natu
		{ Type::POISON, Type::GROUND },			// Nidoking
		{ Type::POISON, Type::GROUND },			// Nidoqueen
		{ Type::POISON },		// Nidoran-F
		{ Type::POISON },		// Nidoran-M
		{ Type::POISON },		// Nidorina
		{ Type::POISON },		// Nidorino
		{ Type::BUG, Type::GROUND },			// Nincada
		{ Type::FIRE },			// Ninetales
		{ Type::BUG, Type::FLYING },			// Ninjask
		{ Type::NORMAL, Type::FLYING },			// Noctowl
		{ Type::ROCK },			// Nosepass
		{ Type::FIRE, Type::GROUND },			// Numel
		{ Type::GRASS, Type::DARK },			// Nuzleaf
		{ Type::WATER },		// Octillery
		{ Type::GRASS, Type::POISON },			// Oddish
		{ Type::ROCK, Type::WATER },			// Omanyte
		{ Type::ROCK, Type::WATER },			// Omastar
		{ Type::ROCK, Type::GROUND },			// Onix
		{ Type::ELECTRIC },		// Pachirisu
		{ Type::WATER, Type::DRAGON },			// Palkia
		{ Type::BUG, Type::GRASS },				// Paras
		{ Type::BUG, Type::GRASS },				// Parasect
		{ Type::WATER, Type::FLYING },			// Pelipper
		{ Type::NORMAL },		// Persian
		{ Type::GROUND },		// Phanpy
		{ Type::WATER },		// Phione
		{ Type::ELECTRIC },		// Pichu
		{ Type::NORMAL, Type::FLYING },			// Pidgeot
		{ Type::NORMAL, Type::FLYING },			// Pidgeotto
		{ Type::NORMAL, Type::FLYING },			// Pidgey
		{ Type::ELECTRIC },		// Pikachu
		{ Type::ICE, Type::GROUND },			// Piloswine
		{ Type::BUG },			// Pineco
		{ Type::BUG },			// Pinsir
		{ Type::WATER },		// Piplup
		{ Type::ELECTRIC },		// Plusle
		{ Type::WATER },		// Politoed
		{ Type::WATER },		// Poliwag
		{ Type::WATER },		// Poliwhirl
		{ Type::WATER, Type::FIGHTING },		// Poliwrath
		{ Type::FIRE },			// Ponyta
		{ Type::DARK },			// Poochyena
		{ Type::NORMAL },		// Porygon
		{ Type::NORMAL },		// Porygon-Z
		{ Type::NORMAL },		// Porygon2
		{ Type::FIGHTING },		// Primeape
		{ Type::WATER },		// Prinplup
		{ Type::ROCK, Type::STEEL },			// Probopass
		{ Type::WATER },		// Psyduck
		{ Type::ROCK, Type::GROUND },			// Pupitar
		{ Type::NORMAL },		// Purugly
		{ Type::WATER, Type::GROUND },			// Quagsire
		{ Type::FIRE },			// Quilava
		{ Type::WATER, Type::POISON },			// Qwilfish
		{ Type::ELECTRIC },		// Raichu
		{ Type::ELECTRIC },		// Raikou
		{ Type::PSYCHIC },		// Ralts
		{ Type::ROCK },			// Rampardos
		{ Type::FIRE },			// Rapidash
		{ Type::NORMAL },		// Raticate
		{ Type::NORMAL },		// Rattata
		{ Type::DRAGON, Type::FLYING },			// Rayquaza
		{ Type::ICE },			// Regice
		{ Type::NORMAL },		// Regigigas
		{ Type::ROCK },			// Regirock
		{ Type::STEEL },		// Registeel
		{ Type::WATER, Type::ROCK },			// Relicanth
		{ Type::WATER },		// Remoraid
		{ Type::GROUND, Type::ROCK },			// Rhydon
		{ Type::GROUND, Type::ROCK },			// Rhyhorn
		{ Type::GROUND, Type::ROCK },			// Rhyperior
		{ Type::FIGHTING },		// Riolu
		{ Type::GRASS, Type::POISON },			// Roselia
		{ Type::GRASS, Type::POISON },			// Roserade
		{ Type::ELECTRIC, Type::GHOST },		// Rotom
		{ Type::ELECTRIC, Type::GHOST },		// Rotom-C
		{ Type::ELECTRIC, Type::GHOST },		// Rotom-F
		{ Type::ELECTRIC, Type::GHOST },		// Rotom-H
		{ Type::ELECTRIC, Type::GHOST },		// Rotom-S
		{ Type::ELECTRIC, Type::GHOST },		// Rotom-W
		{ Type::DARK, Type::GHOST },			// Sableye
		{ Type::DRAGON, Type::FLYING },			// Salamence
		{ Type::GROUND },		// Sandshrew
		{ Type::GROUND },		// Sandslash
		{ Type::GRASS },		// Sceptile
		{ Type::BUG, Type::STEEL },				// Scizor
		{ Type::BUG, Type::FLYING },			// Scyther
		{ Type::WATER },		// Seadra
		{ Type::WATER },		// Seaking
		{ Type::ICE, Type::WATER },				// Sealeo
		{ Type::GRASS },		// Seedot
		{ Type::WATER },		// Seel
		{ Type::NORMAL },		// Sentret
		{ Type::POISON },		// Seviper
		{ Type::WATER, Type::DARK },			// Sharpedo
		{ Type::GRASS },		// Shaymin-L
		{ Type::GRASS, Type::FLYING },			// Shaymin-S
		{ Type::BUG, Type::GHOST },				// Shedinja
		{ Type::DRAGON },		// Shelgon
		{ Type::WATER },		// Shellder
		{ Type::WATER },		// Shellos
		{ Type::ROCK, Type::STEEL },			// Shieldon
		{ Type::GRASS, Type::DARK },			// Shiftry
		{ Type::ELECTRIC },		// Shinx
		{ Type::GRASS },		// Shroomish
		{ Type::BUG, Type::ROCK },				// Shuckle
		{ Type::GHOST },		// Shuppet
		{ Type::BUG },			// Silcoon
		{ Type::STEEL, Type::FLYING },			// Skarmory
		{ Type::GRASS, Type::FLYING },			// Skiploom
		{ Type::NORMAL },		// Skitty
		{ Type::POISON, Type::BUG },			// Skorupi
		{ Type::POISON, Type::DARK },			// Skuntank
		{ Type::NORMAL },		// Slaking
		{ Type::NORMAL },		// Slakoth
		{ Type::WATER, Type::PSYCHIC },			// Slowbro
		{ Type::WATER, Type::PSYCHIC },			// Slowking
		{ Type::WATER, Type::PSYCHIC },			// Slowpoke
		{ Type::FIRE },			// Slugma
		{ Type::NORMAL },		// Smeargle
		{ Type::ICE, Type::PSYCHIC },			// Smoochum
		{ Type::DARK, Type::ICE },				// Sneasel
		{ Type::NORMAL },		// Snorlax
		{ Type::ICE },			// Snorunt
		{ Type::GRASS, Type::ICE },				// Snover
		{ Type::NORMAL },		// Snubbull
		{ Type::ROCK, Type::PSYCHIC },			// Solrock
		{ Type::NORMAL, Type::FLYING },			// Spearow
		{ Type::ICE, Type::WATER },				// Spheal
		{ Type::BUG, Type::POISON },			// Spinarak
		{ Type::NORMAL },		// Spinda
		{ Type::GHOST, Type::DARK },			// Spiritomb
		{ Type::PSYCHIC },		// Spoink
		{ Type::WATER },		// Squirtle
		{ Type::NORMAL },		// Stantler
		{ Type::NORMAL, Type::FLYING },			// Staraptor
		{ Type::NORMAL, Type::FLYING },			// Staravia
		{ Type::NORMAL, Type::FLYING },			// Starly
		{ Type::WATER, Type::PSYCHIC },			// Starmie
		{ Type::WATER },		// Staryu
		{ Type::STEEL, Type::GROUND },			// Steelix
		{ Type::POISON, Type::DARK },			// Stunky
		{ Type::ROCK },			// Sudowoodo
		{ Type::WATER },		// Suicune
		{ Type::GRASS },		// Sunflora
		{ Type::GRASS },		// Sunkern
		{ Type::BUG, Type::WATER },				// Surskit
		{ Type::NORMAL, Type::FLYING },			// Swablu
		{ Type::POISON },		// Swalot
		{ Type::WATER, Type::GROUND },			// Swampert
		{ Type::NORMAL, Type::FLYING },			// Swellow
		{ Type::ICE, Type::GROUND },			// Swinub
		{ Type::NORMAL, Type::FLYING },			// Taillow
		{ Type::GRASS },		// Tangela
		{ Type::GRASS },		// Tangrowth
		{ Type::NORMAL },		// Tauros
		{ Type::NORMAL },		// Teddiursa
		{ Type::WATER, Type::POISON },			// Tentacool
		{ Type::WATER, Type::POISON },			// Tentacruel
		{ Type::NORMAL, Type::FLYING },			// Togekiss
		{ Type::NORMAL },		// Togepi
		{ Type::NORMAL, Type::FLYING },			// Togetic
		{ Type::FIRE },			// Torchic
		{ Type::FIRE },			// Torkoal
		{ Type::GRASS, Type::GROUND },			// Torterra
		{ Type::WATER },		// Totodile
		{ Type::POISON, Type::FIGHTING },		// Toxicroak
		{ Type::GROUND },		// Trapinch
		{ Type::GRASS },		// Treecko
		{ Type::GRASS, Type::FLYING },			// Tropius
		{ Type::GRASS },		// Turtwig
		{ Type::FIRE },			// Typhlosion
		{ Type::ROCK, Type::DARK },				// Tyranitar
		{ Type::FIGHTING },		// Tyrogue
		{ Type::DARK },			// Umbreon
		{ Type::PSYCHIC },		// Unown
		{ Type::NORMAL },		// Ursaring
		{ Type::PSYCHIC },		// Uxie
		{ Type::WATER },		// Vaporeon
		{ Type::BUG, Type::POISON },			// Venomoth
		{ Type::BUG, Type::POISON },			// Venonat
		{ Type::GRASS, Type::POISON },			// Venusaur
		{ Type::BUG, Type::FLYING },			// Vespiquen
		{ Type::GROUND, Type::DRAGON },			// Vibrava
		{ Type::GRASS, Type::POISON },			// Victreebel
		{ Type::NORMAL },		// Vigoroth
		{ Type::GRASS, Type::POISON },			// Vileplume
		{ Type::BUG },			// Volbeat
		{ Type::ELECTRIC },		// Voltorb
		{ Type::FIRE },			// Vulpix
		{ Type::WATER },		// Wailmer
		{ Type::WATER },		// Wailord
		{ Type::ICE, Type::WATER },				// Walrein
		{ Type::WATER },		// Wartortle
		{ Type::DARK, Type::ICE },				// Weavile
		{ Type::BUG, Type::POISON },			// Weedle
		{ Type::GRASS, Type::POISON },			// Weepinbell
		{ Type::POISON },		// Weezing
		{ Type::WATER, Type::GROUND },			// Whiscash
		{ Type::NORMAL },		// Whismur
		{ Type::NORMAL },		// Wigglytuff
		{ Type::WATER, Type::FLYING },			// Wingull
		{ Type::PSYCHIC },		// Wobbuffet
		{ Type::WATER, Type::GROUND },			// Wooper
		{ Type::BUG, Type::GRASS },				// Wormadam-P
		{ Type::BUG, Type::GROUND },			// Wormadam-S
		{ Type::BUG, Type::STEEL },				// Wormadam-T
		{ Type::BUG },			// Wurmple
		{ Type::PSYCHIC },		// Wynaut
		{ Type::PSYCHIC, Type::FLYING },		// Xatu
		{ Type::BUG, Type::FLYING },			// Yanma
		{ Type::BUG, Type::FLYING },			// Yanmega
		{ Type::NORMAL },		// Zangoose
		{ Type::ELECTRIC, Type::FLYING },		// Zapdos
		{ Type::NORMAL },		// Zigzagoon
		{ Type::POISON, Type::FLYING }			// Zubat
	};
	return type_array [static_cast<unsigned>(name)];
}

}	// unnamed namespace
}	// namespace technicalmachine
