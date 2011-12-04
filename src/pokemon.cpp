// Pokemon functions
// Copyright (C) 2011 David Stone
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

#include "pokemon.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "ability.h"
#include "active.h"
#include "gender.h"
#include "move.h"
#include "species.h"
#include "stat.h"

namespace technicalmachine {

Pokemon::Pokemon (Species const & member, unsigned size) : 
	name (member),
	hp (name, Stat::HP),
	atk (name, Stat::ATK),
	def (name, Stat::DEF),
	spa (name, Stat::SPA),
	spd (name, Stat::SPD),
	spe (name, Stat::SPE),

	new_hp (48),
	fainted (false),
	level (100),
	mass (get_mass ()),
	sleep (0),

	happiness (255)
	{
		set_type ();
		Move struggle (Move::STRUGGLE, 0, 0);
		move.set.push_back (struggle);
		// A Pokemon has a new "Switch" move for each Pokemon in the party.
		if (size > 1) {
			for (size_t index = Move::SWITCH0; index - Move::SWITCH0 < size; ++index) {
				Move switchn (static_cast<Move::Moves> (index), 0, 0);
				move.set.push_back (switchn);
			}
		}
}

uint64_t Pokemon::hash () const {
	uint64_t hash = 0;
	for (Move const & next_move : move.set)
		hash ^= next_move.hash();
	return name + END_SPECIES * (item.name + Item::END_ITEM * (status.name + Status::END_STATUS * (hp.stat + hp.max * (sleep + 5 * hash))));
}

bool Pokemon::find_move (Move::Moves name) {
	for (move.index = 0; move->name != Move::STRUGGLE; ++move.index) {
		if (name == move->name)
			return true;
	}
	return false;
}

void Pokemon::normalize_hp () {
	// Correct rounding issues caused by not seeing the foe's exact HP.
	if (fainted)
		hp.stat = 0;
	else if (hp.stat == 0)
		hp.stat = 1;
}

bool Pokemon::operator== (Pokemon const & other) const {
	if (move.set.size() != other.move.set.size())
		return false;
	for (size_t n = 0; n != move.set.size(); ++n) {
		if (move.set [n] == other.move.set [n])
			return false;
	}
	return name == other.name and status.name == other.status.name and sleep == other.sleep and hp.stat == other.hp.stat and item.name == other.item.name;
}

bool Pokemon::operator!= (Pokemon const & other) const {
	return !(*this == other);
}

void Pokemon::load () {
	hp.max = hitpoints (*this);
	hp.stat = hp.max;
}

void Pokemon::set_type () {
	constexpr static Type get_type [][2] = {
		{ GRASS, ICE },				// Abomasnow
		{ PSYCHIC_TYPE, TYPELESS },		// Abra
		{ DARK, TYPELESS },			// Absol
		{ ROCK, FLYING },			// Aerodactyl
		{ STEEL, ROCK },			// Aggron
		{ NORMAL, TYPELESS },		// Aipom
		{ PSYCHIC_TYPE, TYPELESS },		// Alakazam
		{ DRAGON, FLYING },			// Altaria
		{ NORMAL, TYPELESS },		// Ambipom
		{ ELECTRIC, TYPELESS },		// Ampharos
		{ ROCK, BUG },				// Anorith
		{ POISON, TYPELESS },		// Arbok
		{ FIRE, TYPELESS },			// Arcanine
		{ NORMAL, TYPELESS },		// Arceus
		{ BUG, POISON },			// Ariados
		{ ROCK, BUG },				// Armaldo
		{ STEEL, ROCK },			// Aron
		{ ICE, FLYING },			// Articuno
		{ PSYCHIC_TYPE, TYPELESS },		// Azelf
		{ WATER, TYPELESS },		// Azumarill
		{ NORMAL, TYPELESS },		// Azurill
		{ DRAGON, TYPELESS },		// Bagon
		{ GROUND, PSYCHIC_TYPE },		// Baltoy
		{ GHOST, TYPELESS },		// Banette
		{ WATER, GROUND },			// Barboach
		{ ROCK, STEEL },			// Bastiodon
		{ GRASS, TYPELESS },		// Bayleef
		{ BUG, FLYING },			// Beautifly
		{ BUG, POISON },			// Beedrill
		{ STEEL, PSYCHIC_TYPE },			// Beldum
		{ GRASS, TYPELESS },		// Bellossom
		{ GRASS, POISON },			// Bellsprout
		{ NORMAL, WATER },			// Bibarel
		{ NORMAL, TYPELESS },		// Bidoof
		{ WATER, TYPELESS },		// Blastoise
		{ FIRE, FIGHTING },			// Blaziken
		{ NORMAL, TYPELESS },		// Blissey
		{ ROCK, TYPELESS },			// Bonsly
		{ GRASS, FIGHTING },		// Breloom
		{ STEEL, PSYCHIC_TYPE },			// Bronzong
		{ STEEL, PSYCHIC_TYPE },			// Bronzor
		{ GRASS, POISON },			// Budew
		{ WATER, TYPELESS },		// Buizel
		{ GRASS, POISON },			// Bulbasaur
		{ NORMAL, TYPELESS },		// Buneary
		{ BUG, TYPELESS },			// Burmy
		{ BUG, FLYING },			// Butterfree
		{ GRASS, TYPELESS },		// Cacnea
		{ GRASS, DARK },			// Cacturne
		{ FIRE, GROUND },			// Camerupt
		{ GRASS, TYPELESS },		// Carnivine
		{ WATER, DARK },			// Carvanha
		{ BUG, TYPELESS },			// Cascoon
		{ NORMAL, TYPELESS },		// Castform
		{ BUG, TYPELESS },			// Caterpie
		{ PSYCHIC_TYPE, GRASS },			// Celebi
		{ NORMAL, TYPELESS },		// Chansey
		{ FIRE, FLYING },			// Charizard
		{ FIRE, TYPELESS },			// Charmander
		{ FIRE, TYPELESS },			// Charmeleon
		{ NORMAL, FLYING },			// Chatot
		{ GRASS, TYPELESS },		// Cherrim
		{ GRASS, TYPELESS },		// Cherubi
		{ GRASS, TYPELESS },		// Chikorita
		{ FIRE, TYPELESS },			// Chimchar
		{ PSYCHIC_TYPE, TYPELESS },		// Chimecho
		{ WATER, ELECTRIC },		// Chinchou
		{ PSYCHIC_TYPE, TYPELESS },		// Chingling
		{ WATER, TYPELESS },		// Clamperl
		{ GROUND, PSYCHIC_TYPE },		// Claydol
		{ NORMAL, TYPELESS },		// Clefable
		{ NORMAL, TYPELESS },		// Clefairy
		{ NORMAL, TYPELESS },		// Cleffa
		{ WATER, ICE },				// Cloyster
		{ BUG, FLYING },			// Combee
		{ FIRE, FIGHTING },			// Combusken
		{ WATER, TYPELESS },		// Corphish
		{ WATER, ROCK },			// Corsola
		{ ROCK, GRASS },			// Cradily
		{ ROCK, TYPELESS },			// Cranidos
		{ WATER, DARK },			// Crawdaunt
		{ PSYCHIC_TYPE, TYPELESS },		// Cresselia
		{ POISON, FIGHTING },		// Croagunk
		{ POISON, FLYING },			// Crobat
		{ WATER, TYPELESS },		// Croconaw
		{ GROUND, TYPELESS },		// Cubone
		{ FIRE, TYPELESS },			// Cyndaquil
		{ DARK, TYPELESS },			// Darkrai
		{ NORMAL, TYPELESS },		// Delcatty
		{ ICE, FLYING },			// Delibird
		{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-A
		{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-D
		{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-M
		{ PSYCHIC_TYPE, TYPELESS },		// Deoxys-S
		{ WATER, ICE },				// Dewgong
		{ STEEL, DRAGON },			// Dialga
		{ GROUND, TYPELESS },		// Diglett
		{ NORMAL, TYPELESS },		// Ditto
		{ NORMAL, FLYING },			// Dodrio
		{ NORMAL, FLYING },			// Doduo
		{ GROUND, TYPELESS },		// Donphan
		{ DRAGON, TYPELESS },		// Dragonair
		{ DRAGON, FLYING },			// Dragonite
		{ POISON, DARK },			// Drapion
		{ DRAGON, TYPELESS },		// Dratini
		{ GHOST, FLYING },			// Drifblim
		{ GHOST, FLYING },			// Drifloon
		{ PSYCHIC_TYPE, TYPELESS },		// Drowzee
		{ GROUND, TYPELESS },		// Dugtrio
		{ NORMAL, TYPELESS },		// Dunsparce
		{ GHOST, TYPELESS },		// Dusclops
		{ GHOST, TYPELESS },		// Dusknoir
		{ GHOST, TYPELESS },		// Duskull
		{ BUG, POISON },			// Dustox
		{ NORMAL, TYPELESS },		// Eevee
		{ POISON, TYPELESS },		// Ekans
		{ ELECTRIC, TYPELESS },		// Electabuzz
		{ ELECTRIC, TYPELESS },		// Electivire
		{ ELECTRIC, TYPELESS },		// Electrike
		{ ELECTRIC, TYPELESS },		// Electrode
		{ ELECTRIC, TYPELESS },		// Elekid
		{ WATER, STEEL },			// Empoleon
		{ FIRE, TYPELESS },			// Entei
		{ PSYCHIC_TYPE, TYPELESS },		// Espeon
		{ GRASS, PSYCHIC_TYPE },			// Exeggcute
		{ GRASS, PSYCHIC_TYPE },			// Exeggutor
		{ NORMAL, TYPELESS },		// Exploud
		{ NORMAL, FLYING },			// Farfetch'd
		{ NORMAL, FLYING },			// Fearow
		{ WATER, TYPELESS },		// Feebas
		{ WATER, TYPELESS },		// Feraligatr
		{ WATER, TYPELESS },		// Finneon
		{ ELECTRIC, TYPELESS },		// Flaaffy
		{ FIRE, TYPELESS },			// Flareon
		{ WATER, TYPELESS },		// Floatzel
		{ GROUND, DRAGON },			// Flygon
		{ BUG, STEEL },				// Forretress
		{ ICE, GHOST },				// Froslass
		{ NORMAL, TYPELESS },		// Furret
		{ DRAGON, GROUND },			// Gabite
		{ PSYCHIC_TYPE, FIGHTING },		// Gallade
		{ DRAGON, GROUND },			// Garchomp
		{ PSYCHIC_TYPE, TYPELESS },		// Gardevoir
		{ GHOST, POISON },			// Gastly
		{ WATER, GROUND },			// Gastrodon
		{ GHOST, POISON },			// Gengar
		{ ROCK, GROUND },			// Geodude
		{ DRAGON, GROUND },			// Gible
		{ NORMAL, PSYCHIC_TYPE },		// Girafarig
		{ GHOST, DRAGON },			// Giratina-A
		{ GHOST, DRAGON },			// Giratina-O
		{ ICE, TYPELESS },			// Glaceon
		{ ICE, TYPELESS },			// Glalie
		{ NORMAL, TYPELESS },		// Glameow
		{ GROUND, FLYING },			// Gligar
		{ GROUND, FLYING },			// Gliscor
		{ GRASS, POISON },			// Gloom
		{ POISON, FLYING },			// Golbat
		{ WATER, TYPELESS },		// Goldeen
		{ WATER, TYPELESS },		// Golduck
		{ ROCK, GROUND },			// Golem
		{ WATER, TYPELESS },		// Gorebyss
		{ NORMAL, TYPELESS },		// Granbull
		{ ROCK, GROUND },			// Graveler
		{ POISON, TYPELESS },		// Grimer
		{ GRASS, TYPELESS },		// Grotle
		{ GROUND, TYPELESS },		// Groudon
		{ GRASS, TYPELESS },		// Grovyle
		{ FIRE, TYPELESS },			// Growlithe
		{ PSYCHIC_TYPE, TYPELESS },		// Grumpig
		{ POISON, TYPELESS },		// Gulpin
		{ WATER, FLYING },			// Gyarados
		{ NORMAL, TYPELESS },		// Happiny
		{ FIGHTING, TYPELESS },		// Hariyama
		{ GHOST, POISON },			// Haunter
		{ FIRE, STEEL },			// Heatran
		{ BUG, FIGHTING },			// Heracross
		{ GROUND, TYPELESS },		// Hippopotas
		{ GROUND, TYPELESS },		// Hippowdon
		{ FIGHTING, TYPELESS },		// Hitmonchan
		{ FIGHTING, TYPELESS },		// Hitmonlee
		{ FIGHTING, TYPELESS },		// Hitmontop
		{ FIRE, FLYING },			// Ho-Oh
		{ DARK, FLYING },			// Honchkrow
		{ NORMAL, FLYING },			// Hoothoot
		{ GRASS, FLYING },			// Hoppip
		{ WATER, TYPELESS },		// Horsea
		{ DARK, FIRE },				// Houndoom
		{ DARK, FIRE },				// Houndour
		{ WATER, TYPELESS },		// Huntail
		{ PSYCHIC_TYPE, TYPELESS },		// Hypno
		{ NORMAL, TYPELESS },		// Igglybuff
		{ BUG, TYPELESS },			// Illumise
		{ FIRE, FIGHTING },			// Infernape
		{ GRASS, POISON },			// Ivysaur
		{ NORMAL, TYPELESS },		// Jigglypuff
		{ STEEL, PSYCHIC_TYPE },			// Jirachi
		{ ELECTRIC, TYPELESS },		// Jolteon
		{ GRASS, FLYING },			// Jumpluff
		{ ICE, PSYCHIC_TYPE },			// Jynx
		{ ROCK, WATER },			// Kabuto
		{ ROCK, WATER },			// Kabutops
		{ PSYCHIC_TYPE, TYPELESS },		// Kadabra
		{ BUG, POISON },			// Kakuna
		{ NORMAL, TYPELESS },		// Kangaskhan
		{ NORMAL, TYPELESS },		// Kecleon
		{ WATER, DRAGON },			// Kingdra
		{ WATER, TYPELESS },		// Kingler
		{ PSYCHIC_TYPE, TYPELESS },		// Kirlia
		{ POISON, TYPELESS },		// Koffing
		{ WATER, TYPELESS },		// Krabby
		{ BUG, TYPELESS },			// Kricketot
		{ BUG, TYPELESS },			// Kricketune
		{ WATER, TYPELESS },		// Kyogre
		{ STEEL, ROCK },			// Lairon
		{ WATER, ELECTRIC },		// Lanturn
		{ WATER, ICE },				// Lapras
		{ ROCK, GROUND },			// Larvitar
		{ DRAGON, PSYCHIC_TYPE },		// Latias
		{ DRAGON, PSYCHIC_TYPE },		// Latios
		{ GRASS, TYPELESS },		// Leafeon
		{ BUG, FLYING },			// Ledian
		{ BUG, FLYING },			// Ledyba
		{ NORMAL, TYPELESS },		// Lickilicky
		{ NORMAL, TYPELESS },		// Lickitung
		{ ROCK, GRASS },			// Lileep
		{ NORMAL, TYPELESS },		// Linoone
		{ WATER, GRASS },			// Lombre
		{ NORMAL, TYPELESS },		// Lopunny
		{ WATER, GRASS },			// Lotad
		{ NORMAL, TYPELESS },		// Loudred
		{ FIGHTING, STEEL },		// Lucario
		{ WATER, GRASS },			// Ludicolo
		{ PSYCHIC_TYPE, FLYING },		// Lugia
		{ WATER, TYPELESS },		// Lumineon
		{ ROCK, PSYCHIC_TYPE },			// Lunatone
		{ WATER, TYPELESS },		// Luvdisc
		{ ELECTRIC, TYPELESS },		// Luxio
		{ ELECTRIC, TYPELESS },		// Luxray
		{ FIGHTING, TYPELESS },		// Machamp
		{ FIGHTING, TYPELESS },		// Machoke
		{ FIGHTING, TYPELESS },		// Machop
		{ FIRE, TYPELESS },			// Magby
		{ FIRE, ROCK },				// Magcargo
		{ WATER, TYPELESS },		// Magikarp
		{ FIRE, TYPELESS },			// Magmar
		{ FIRE, TYPELESS },			// Magmortar
		{ ELECTRIC, STEEL },		// Magnemite
		{ ELECTRIC, STEEL },		// Magneton
		{ ELECTRIC, STEEL },		// Magnezone
		{ FIGHTING, TYPELESS },		// Makuhita
		{ ICE, GROUND },			// Mamoswine
		{ WATER, TYPELESS },		// Manaphy
		{ ELECTRIC, TYPELESS },		// Manectric
		{ FIGHTING, TYPELESS },		// Mankey
		{ WATER, FLYING },			// Mantine
		{ WATER, FLYING },			// Mantyke
		{ ELECTRIC, TYPELESS },		// Mareep
		{ WATER, TYPELESS },		// Marill
		{ GROUND, TYPELESS },		// Marowak
		{ WATER, GROUND },			// Marshtomp
		{ BUG, FLYING },			// Masquerain
		{ STEEL, TYPELESS },		// Mawile
		{ FIGHTING, PSYCHIC_TYPE },		// Medicham
		{ FIGHTING, PSYCHIC_TYPE },		// Meditite
		{ GRASS, TYPELESS },		// Meganium
		{ NORMAL, TYPELESS },		// Meowth
		{ PSYCHIC_TYPE, TYPELESS },		// Mesprit
		{ STEEL, PSYCHIC_TYPE },			// Metagross
		{ STEEL, PSYCHIC_TYPE },			// Metang
		{ BUG, TYPELESS },			// Metapod
		{ PSYCHIC_TYPE, TYPELESS },		// Mew
		{ PSYCHIC_TYPE, TYPELESS },		// Mewtwo
		{ DARK, TYPELESS },			// Mightyena
		{ WATER, TYPELESS },		// Milotic
		{ NORMAL, TYPELESS },		// Miltank
		{ PSYCHIC_TYPE, TYPELESS },		// Mime Jr.
		{ ELECTRIC, TYPELESS },		// Minun
		{ GHOST, TYPELESS },		// Misdreavus
		{ GHOST, TYPELESS },		// Mismagius
		{ FIRE, FLYING },			// Moltres
		{ FIRE, FIGHTING },			// Monferno
		{ BUG, FLYING },			// Mothim
		{ PSYCHIC_TYPE, TYPELESS },		// Mr. Mime
		{ WATER, TYPELESS },		// Mudkip
		{ POISON, TYPELESS },		// Muk
		{ NORMAL, TYPELESS },		// Munchlax
		{ DARK, FLYING },			// Murkrow
		{ PSYCHIC_TYPE, FLYING },		// Natu
		{ POISON, GROUND },			// Nidoking
		{ POISON, GROUND },			// Nidoqueen
		{ POISON, TYPELESS },		// Nidoran-F
		{ POISON, TYPELESS },		// Nidoran-M
		{ POISON, TYPELESS },		// Nidorina
		{ POISON, TYPELESS },		// Nidorino
		{ BUG, GROUND },			// Nincada
		{ FIRE, TYPELESS },			// Ninetales
		{ BUG, FLYING },			// Ninjask
		{ NORMAL, FLYING },			// Noctowl
		{ ROCK, TYPELESS },			// Nosepass
		{ FIRE, GROUND },			// Numel
		{ GRASS, DARK },			// Nuzleaf
		{ WATER, TYPELESS },		// Octillery
		{ GRASS, POISON },			// Oddish
		{ ROCK, WATER },			// Omanyte
		{ ROCK, WATER },			// Omastar
		{ ROCK, GROUND },			// Onix
		{ ELECTRIC, TYPELESS },		// Pachirisu
		{ WATER, DRAGON },			// Palkia
		{ BUG, GRASS },				// Paras
		{ BUG, GRASS },				// Parasect
		{ WATER, FLYING },			// Pelipper
		{ NORMAL, TYPELESS },		// Persian
		{ GROUND, TYPELESS },		// Phanpy
		{ WATER, TYPELESS },		// Phione
		{ ELECTRIC, TYPELESS },		// Pichu
		{ NORMAL, FLYING },			// Pidgeot
		{ NORMAL, FLYING },			// Pidgeotto
		{ NORMAL, FLYING },			// Pidgey
		{ ELECTRIC, TYPELESS },		// Pikachu
		{ ICE, GROUND },			// Piloswine
		{ BUG, TYPELESS },			// Pineco
		{ BUG, TYPELESS },			// Pinsir
		{ WATER, TYPELESS },		// Piplup
		{ ELECTRIC, TYPELESS },		// Plusle
		{ WATER, TYPELESS },		// Politoed
		{ WATER, TYPELESS },		// Poliwag
		{ WATER, TYPELESS },		// Poliwhirl
		{ WATER, FIGHTING },		// Poliwrath
		{ FIRE, TYPELESS },			// Ponyta
		{ DARK, TYPELESS },			// Poochyena
		{ NORMAL, TYPELESS },		// Porygon
		{ NORMAL, TYPELESS },		// Porygon-Z
		{ NORMAL, TYPELESS },		// Porygon2
		{ FIGHTING, TYPELESS },		// Primeape
		{ WATER, TYPELESS },		// Prinplup
		{ ROCK, STEEL },			// Probopass
		{ WATER, TYPELESS },		// Psyduck
		{ ROCK, GROUND },			// Pupitar
		{ NORMAL, TYPELESS },		// Purugly
		{ WATER, GROUND },			// Quagsire
		{ FIRE, TYPELESS },			// Quilava
		{ WATER, POISON },			// Qwilfish
		{ ELECTRIC, TYPELESS },		// Raichu
		{ ELECTRIC, TYPELESS },		// Raikou
		{ PSYCHIC_TYPE, TYPELESS },		// Ralts
		{ ROCK, TYPELESS },			// Rampardos
		{ FIRE, TYPELESS },			// Rapidash
		{ NORMAL, TYPELESS },		// Raticate
		{ NORMAL, TYPELESS },		// Rattata
		{ DRAGON, FLYING },			// Rayquaza
		{ ICE, TYPELESS },			// Regice
		{ NORMAL, TYPELESS },		// Regigigas
		{ ROCK, TYPELESS },			// Regirock
		{ STEEL, TYPELESS },		// Registeel
		{ WATER, ROCK },			// Relicanth
		{ WATER, TYPELESS },		// Remoraid
		{ GROUND, ROCK },			// Rhydon
		{ GROUND, ROCK },			// Rhyhorn
		{ GROUND, ROCK },			// Rhyperior
		{ FIGHTING, TYPELESS },		// Riolu
		{ GRASS, POISON },			// Roselia
		{ GRASS, POISON },			// Roserade
		{ ELECTRIC, GHOST },		// Rotom
		{ ELECTRIC, GHOST },		// Rotom-C
		{ ELECTRIC, GHOST },		// Rotom-F
		{ ELECTRIC, GHOST },		// Rotom-H
		{ ELECTRIC, GHOST },		// Rotom-S
		{ ELECTRIC, GHOST },		// Rotom-W
		{ DARK, GHOST },			// Sableye
		{ DRAGON, FLYING },			// Salamence
		{ GROUND, TYPELESS },		// Sandshrew
		{ GROUND, TYPELESS },		// Sandslash
		{ GRASS, TYPELESS },		// Sceptile
		{ BUG, STEEL },				// Scizor
		{ BUG, FLYING },			// Scyther
		{ WATER, TYPELESS },		// Seadra
		{ WATER, TYPELESS },		// Seaking
		{ ICE, WATER },				// Sealeo
		{ GRASS, TYPELESS },		// Seedot
		{ WATER, TYPELESS },		// Seel
		{ NORMAL, TYPELESS },		// Sentret
		{ POISON, TYPELESS },		// Seviper
		{ WATER, DARK },			// Sharpedo
		{ GRASS, TYPELESS },		// Shaymin-L
		{ GRASS, FLYING },			// Shaymin-S
		{ BUG, GHOST },				// Shedinja
		{ DRAGON, TYPELESS },		// Shelgon
		{ WATER, TYPELESS },		// Shellder
		{ WATER, TYPELESS },		// Shellos
		{ ROCK, STEEL },			// Shieldon
		{ GRASS, DARK },			// Shiftry
		{ ELECTRIC, TYPELESS },		// Shinx
		{ GRASS, TYPELESS },		// Shroomish
		{ BUG, ROCK },				// Shuckle
		{ GHOST, TYPELESS },		// Shuppet
		{ BUG, TYPELESS },			// Silcoon
		{ STEEL, FLYING },			// Skarmory
		{ GRASS, FLYING },			// Skiploom
		{ NORMAL, TYPELESS },		// Skitty
		{ POISON, BUG },			// Skorupi
		{ POISON, DARK },			// Skuntank
		{ NORMAL, TYPELESS },		// Slaking
		{ NORMAL, TYPELESS },		// Slakoth
		{ WATER, PSYCHIC_TYPE },			// Slowbro
		{ WATER, PSYCHIC_TYPE },			// Slowking
		{ WATER, PSYCHIC_TYPE },			// Slowpoke
		{ FIRE, TYPELESS },			// Slugma
		{ NORMAL, TYPELESS },		// Smeargle
		{ ICE, PSYCHIC_TYPE },			// Smoochum
		{ DARK, ICE },				// Sneasel
		{ NORMAL, TYPELESS },		// Snorlax
		{ ICE, TYPELESS },			// Snorunt
		{ GRASS, ICE },				// Snover
		{ NORMAL, TYPELESS },		// Snubbull
		{ ROCK, PSYCHIC_TYPE },			// Solrock
		{ NORMAL, FLYING },			// Spearow
		{ ICE, WATER },				// Spheal
		{ BUG, POISON },			// Spinarak
		{ NORMAL, TYPELESS },		// Spinda
		{ GHOST, DARK },			// Spiritomb
		{ PSYCHIC_TYPE, TYPELESS },		// Spoink
		{ WATER, TYPELESS },		// Squirtle
		{ NORMAL, TYPELESS },		// Stantler
		{ NORMAL, FLYING },			// Staraptor
		{ NORMAL, FLYING },			// Staravia
		{ NORMAL, FLYING },			// Starly
		{ WATER, PSYCHIC_TYPE },			// Starmie
		{ WATER, TYPELESS },		// Staryu
		{ STEEL, GROUND },			// Steelix
		{ POISON, DARK },			// Stunky
		{ ROCK, TYPELESS },			// Sudowoodo
		{ WATER, TYPELESS },		// Suicune
		{ GRASS, TYPELESS },		// Sunflora
		{ GRASS, TYPELESS },		// Sunkern
		{ BUG, WATER },				// Surskit
		{ NORMAL, FLYING },			// Swablu
		{ POISON, TYPELESS },		// Swalot
		{ WATER, GROUND },			// Swampert
		{ NORMAL, FLYING },			// Swellow
		{ ICE, GROUND },			// Swinub
		{ NORMAL, FLYING },			// Taillow
		{ GRASS, TYPELESS },		// Tangela
		{ GRASS, TYPELESS },		// Tangrowth
		{ NORMAL, TYPELESS },		// Tauros
		{ NORMAL, TYPELESS },		// Teddiursa
		{ WATER, POISON },			// Tentacool
		{ WATER, POISON },			// Tentacruel
		{ NORMAL, FLYING },			// Togekiss
		{ NORMAL, TYPELESS },		// Togepi
		{ NORMAL, FLYING },			// Togetic
		{ FIRE, TYPELESS },			// Torchic
		{ FIRE, TYPELESS },			// Torkoal
		{ GRASS, GROUND },			// Torterra
		{ WATER, TYPELESS },		// Totodile
		{ POISON, FIGHTING },		// Toxicroak
		{ GROUND, TYPELESS },		// Trapinch
		{ GRASS, TYPELESS },		// Treecko
		{ GRASS, FLYING },			// Tropius
		{ GRASS, TYPELESS },		// Turtwig
		{ FIRE, TYPELESS },			// Typhlosion
		{ ROCK, DARK },				// Tyranitar
		{ FIGHTING, TYPELESS },		// Tyrogue
		{ DARK, TYPELESS },			// Umbreon
		{ PSYCHIC_TYPE, TYPELESS },		// Unown
		{ NORMAL, TYPELESS },		// Ursaring
		{ PSYCHIC_TYPE, TYPELESS },		// Uxie
		{ WATER, TYPELESS },		// Vaporeon
		{ BUG, POISON },			// Venomoth
		{ BUG, POISON },			// Venonat
		{ GRASS, POISON },			// Venusaur
		{ BUG, FLYING },			// Vespiquen
		{ GROUND, DRAGON },			// Vibrava
		{ GRASS, POISON },			// Victreebel
		{ NORMAL, TYPELESS },		// Vigoroth
		{ GRASS, POISON },			// Vileplume
		{ BUG, TYPELESS },			// Volbeat
		{ ELECTRIC, TYPELESS },		// Voltorb
		{ FIRE, TYPELESS },			// Vulpix
		{ WATER, TYPELESS },		// Wailmer
		{ WATER, TYPELESS },		// Wailord
		{ ICE, WATER },				// Walrein
		{ WATER, TYPELESS },		// Wartortle
		{ DARK, ICE },				// Weavile
		{ BUG, POISON },			// Weedle
		{ GRASS, POISON },			// Weepinbell
		{ POISON, TYPELESS },		// Weezing
		{ WATER, GROUND },			// Whiscash
		{ NORMAL, TYPELESS },		// Whismur
		{ NORMAL, TYPELESS },		// Wigglytuff
		{ WATER, FLYING },			// Wingull
		{ PSYCHIC_TYPE, TYPELESS },		// Wobbuffet
		{ WATER, GROUND },			// Wooper
		{ BUG, GRASS },				// Wormadam-P
		{ BUG, GROUND },			// Wormadam-S
		{ BUG, STEEL },				// Wormadam-T
		{ BUG, TYPELESS },			// Wurmple
		{ PSYCHIC_TYPE, TYPELESS },		// Wynaut
		{ PSYCHIC_TYPE, FLYING },		// Xatu
		{ BUG, FLYING },			// Yanma
		{ BUG, FLYING },			// Yanmega
		{ NORMAL, TYPELESS },		// Zangoose
		{ ELECTRIC, FLYING },		// Zapdos
		{ NORMAL, TYPELESS },		// Zigzagoon
		{ POISON, FLYING }			// Zubat
	};

	for (unsigned n = 0; n != 2; ++n) {
		Type t = get_type [name] [n];
		if (t != TYPELESS)
			type.push_back (t);
	}
}

int8_t Pokemon::get_mass () const {
	constexpr static int8_t get_mass [] = {
		100,	// Abomasnow
		40,	// Abra
		60,	// Absol
		80,	// Aerodactyl
		120,	// Aggron
		40,	// Aipom
		60,	// Alakazam
		40,	// Altaria
		40,	// Ambipom
		80,	// Ampharos
		40,	// Anorith
		80,	// Arbok
		100,	// Arcanine
		120,	// Arceus
		60,	// Ariados
		80,	// Armaldo
		80,	// Aron
		80,	// Articuno
		20,	// Azelf
		60,	// Azumarill
		20,	// Azurill
		60,	// Bagon
		40,	// Baltoy
		40,	// Banette
		20,	// Barboach
		100,	// Bastiodon
		40,	// Bayleef
		60,	// Beautifly
		60,	// Beedrill
		80,	// Beldum
		20,	// Bellossom
		20,	// Bellsprout
		60,	// Bibarel
		40,	// Bidoof
		80,	// Blastoise
		80,	// Blaziken
		60,	// Blissey
		40,	// Bonsly
		60,	// Breloom
		100,	// Bronzong
		80,	// Bronzor
		20,	// Budew
		60,	// Buizel
		20,	// Bulbasaur
		20,	// Buneary
		20,	// Burmy
		60,	// Butterfree
		80,	// Cacnea
		80,	// Cacturne
		120,	// Camerupt
		60,	// Carnivine
		40,	// Carvanha
		40,	// Cascoon
		20,	// Castform
		20,	// Caterpie
		20,	// Celebi
		60,	// Chansey
		80,	// Charizard
		20,	// Charmander
		40,	// Charmeleon
		20,	// Chatot
		20,	// Cherrim
		20,	// Cherubi
		20,	// Chikorita
		20,	// Chimchar
		20,	// Chimecho
		40,	// Chinchou
		20,	// Chingling
		80,	// Clamperl
		100,	// Claydol
		60,	// Clefable
		20,	// Clefairy
		20,	// Cleffa
		100,	// Cloyster
		20,	// Combee
		40,	// Combusken
		40,	// Corphish
		20,	// Corsola
		80,	// Cradily
		60,	// Cranidos
		60,	// Crawdaunt
		80,	// Cresselia
		40,	// Croagunk
		80,	// Crobat
		40,	// Croconaw
		20,	// Cubone
		20,	// Cyndaquil
		80,	// Darkrai
		60,	// Delcatty
		40,	// Delibird
		80,	// Deoxys-A
		80,	// Deoxys-D
		80,	// Deoxys-M
		80,	// Deoxys-S
		100,	// Dewgong
		120,	// Dialga
		20,	// Diglett
		20,	// Ditto
		80,	// Dodrio
		60,	// Doduo
		100,	// Donphan
		40,	// Dragonair
		120,	// Dragonite
		80,	// Drapion
		20,	// Dratini
		40,	// Drifblim
		20,	// Drifloon
		60,	// Drowzee
		60,	// Dugtrio
		40,	// Dunsparce
		60,	// Dusclops
		100,	// Dusknoir
		40,	// Duskull
		60,	// Dustox
		20,	// Eevee
		20,	// Ekans
		60,	// Electabuzz
		100,	// Electivire
		40,	// Electrike
		80,	// Electrode
		40,	// Elekid
		80,	// Empoleon
		100,	// Entei
		60,	// Espeon
		20,	// Exeggcute
		100,	// Exeggutor
		80,	// Exploud
		40,	// Farfetch'd
		60,	// Fearow
		20,	// Feebas
		80,	// Feraligatr
		20,	// Finneon
		40,	// Flaaffy
		40,	// Flareon
		60,	// Floatzel
		80,	// Flygon
		100,	// Forretress
		60,	// Froslass
		60,	// Furret
		80,	// Gabite
		80,	// Gallade
		80,	// Garchomp
		60,	// Gardevoir
		20,	// Gastly
		60,	// Gastrodon
		60,	// Gengar
		40,	// Geodude
		40,	// Gible
		60,	// Girafarig
		120,	// Giratina-A
		120,	// Giratina-O
		60,	// Glaceon
		120,	// Glalie
		20,	// Glameow
		80,	// Gligar
		60,	// Gliscor
		20,	// Gloom
		80,	// Golbat
		40,	// Goldeen
		80,	// Golduck
		120,	// Golem
		40,	// Gorebyss
		60,	// Granbull
		100,	// Graveler
		60,	// Grimer
		80,	// Grotle
		120,	// Groudon
		40,	// Grovyle
		40,	// Growlithe
		80,	// Grumpig
		40,	// Gulpin
		120,	// Gyarados
		40,	// Happiny
		120,	// Hariyama
		20,	// Haunter
		120,	// Heatran
		80,	// Heracross
		60,	// Hippopotas
		120,	// Hippowdon
		80,	// Hitmonchan
		60,	// Hitmonlee
		60,	// Hitmontop
		100,	// Ho-Oh
		60,	// Honchkrow
		40,	// Hoothoot
		20,	// Hoppip
		20,	// Horsea
		60,	// Houndoom
		40,	// Houndour
		60,	// Huntail
		80,	// Hypno
		20,	// Igglybuff
		40,	// Illumise
		80,	// Infernape
		40,	// Ivysaur
		20,	// Jigglypuff
		20,	// Jirachi
		40,	// Jolteon
		20,	// Jumpluff
		60,	// Jynx
		40,	// Kabuto
		60,	// Kabutops
		80,	// Kadabra
		20,	// Kakuna
		80,	// Kangaskhan
		40,	// Kecleon
		100,	// Kingdra
		80,	// Kingler
		40,	// Kirlia
		20,	// Koffing
		20,	// Krabby
		20,	// Kricketot
		60,	// Kricketune
		120,	// Kyogre
		100,	// Lairon
		40,	// Lanturn
		120,	// Lapras
		80,	// Larvitar
		60,	// Latias
		80,	// Latios
		60,	// Leafeon
		60,	// Ledian
		40,	// Ledyba
		100,	// Lickilicky
		80,	// Lickitung
		40,	// Lileep
		60,	// Linoone
		60,	// Lombre
		60,	// Lopunny
		20,	// Lotad
		60,	// Loudred
		80,	// Lucario
		80,	// Ludicolo
		120,	// Lugia
		40,	// Lumineon
		100,	// Lunatone
		20,	// Luvdisc
		60,	// Luxio
		60,	// Luxray
		100,	// Machamp
		80,	// Machoke
		40,	// Machop
		40,	// Magby
		80,	// Magcargo
		20,	// Magikarp
		60,	// Magmar
		80,	// Magmortar
		20,	// Magnemite
		80,	// Magneton
		100,	// Magnezone
		80,	// Makuhita
		120,	// Mamoswine
		20,	// Manaphy
		60,	// Manectric
		60,	// Mankey
		120,	// Mantine
		80,	// Mantyke
		20,	// Mareep
		20,	// Marill
		60,	// Marowak
		60,	// Marshtomp
		20,	// Masquerain
		40,	// Mawile
		60,	// Medicham
		40,	// Meditite
		100,	// Meganium
		20,	// Meowth
		20,	// Mesprit
		120,	// Metagross
		120,	// Metang
		20,	// Metapod
		20,	// Mew
		100,	// Mewtwo
		60,	// Mightyena
		100,	// Milotic
		80,	// Miltank
		40,	// Mime Jr.
		20,	// Minun
		20,	// Misdreavus
		20,	// Mismagius
		80,	// Moltres
		40,	// Monferno
		40,	// Mothim
		80,	// Mr. Mime
		20,	// Mudkip
		60,	// Muk
		100,	// Munchlax
		20,	// Murkrow
		20,	// Natu
		80,	// Nidoking
		80,	// Nidoqueen
		20,	// Nidoran♀
		20,	// Nidoran♂
		40,	// Nidorina
		40,	// Nidorino
		20,	// Nincada
		40,	// Ninetales
		40,	// Ninjask
		60,	// Noctowl
		80,	// Nosepass
		40,	// Numel
		60,	// Nuzleaf
		60,	// Octillery
		20,	// Oddish
		20,	// Omanyte
		60,	// Omastar
		120,	// Onix
		20,	// Pachirisu
		120,	// Palkia
		20,	// Paras
		60,	// Parasect
		60,	// Pelipper
		60,	// Persian
		60,	// Phanpy
		20,	// Phione
		20,	// Pichu
		60,	// Pidgeot
		60,	// Pidgeotto
		20,	// Pidgey
		20,	// Pikachu
		80,	// Piloswine
		20,	// Pineco
		80,	// Pinsir
		20,	// Piplup
		20,	// Plusle
		60,	// Politoed
		40,	// Poliwag
		40,	// Poliwhirl
		80,	// Poliwrath
		60,	// Ponyta
		40,	// Poochyena
		60,	// Porygon
		60,	// Porygon-Z
		60,	// Porygon2
		60,	// Primeape
		40,	// Prinplup
		120,	// Probopass
		40,	// Psyduck
		100,	// Pupitar
		60,	// Purugly
		80,	// Quagsire
		40,	// Quilava
		20,	// Qwilfish
		60,	// Raichu
		100,	// Raikou
		20,	// Ralts
		100,	// Rampardos
		80,	// Rapidash
		40,	// Raticate
		20,	// Rattata
		120,	// Rayquaza
		100,	// Regice
		120,	// Regigigas
		120,	// Regirock
		120,	// Registeel
		40,	// Relicanth
		40,	// Remoraid
		100,	// Rhydon
		100,	// Rhyhorn
		120,	// Rhyperior
		40,	// Riolu
		20,	// Roselia
		40,	// Roserade
		20,	// Rotom
		20,	// Rotom-C
		20,	// Rotom-F
		20,	// Rotom-H
		20,	// Rotom-S
		20,	// Rotom-W
		40,	// Sableye
		100,	// Salamence
		40,	// Sandshrew
		60,	// Sandslash
		80,	// Sceptile
		100,	// Scizor
		80,	// Scyther
		40,	// Seadra
		60,	// Seaking
		80,	// Sealeo
		20,	// Seedot
		80,	// Seel
		20,	// Sentret
		80,	// Seviper
		80,	// Sharpedo
		20,	// Shaymin-L
		20,	// Shaymin-S
		20,	// Shedinja
		100,	// Shelgon
		20,	// Shellder
		20,	// Shellos
		80,	// Shieldon
		80,	// Shiftry
		20,	// Shinx
		20,	// Shroomish
		40,	// Shuckle
		20,	// Shuppet
		20,	// Silcoon
		80,	// Skarmory
		20,	// Skiploom
		40,	// Skitty
		40,	// Skorupi
		60,	// Skuntank
		100,	// Slaking
		40,	// Slakoth
		80,	// Slowbro
		80,	// Slowking
		60,	// Slowpoke
		60,	// Slugma
		80,	// Smeargle
		20,	// Smoochum
		60,	// Sneasel
		120,	// Snorlax
		40,	// Snorunt
		80,	// Snover
		20,	// Snubbull
		100,	// Solrock
		20,	// Spearow
		60,	// Spheal
		20,	// Spinarak
		20,	// Spinda
		100,	// Spiritomb
		60,	// Spoink
		20,	// Squirtle
		80,	// Stantler
		40,	// Staraptor
		40,	// Staravia
		20,	// Starly
		80,	// Starmie
		60,	// Staryu
		120,	// Steelix
		40,	// Stunky
		60,	// Sudowoodo
		100,	// Suicune
		20,	// Sunflora
		20,	// Sunkern
		20,	// Surskit
		20,	// Swablu
		80,	// Swalot
		80,	// Swampert
		40,	// Swellow
		20,	// Swinub
		20,	// Taillow
		60,	// Tangela
		100,	// Tangrowth
		80,	// Tauros
		20,	// Teddiursa
		60,	// Tentacool
		80,	// Tentacruel
		60,	// Togekiss
		20,	// Togepi
		20,	// Togetic
		20,	// Torchic
		80,	// Torkoal
		120,	// Torterra
		20,	// Totodile
		60,	// Toxicroak
		40,	// Trapinch
		20,	// Treecko
		80,	// Tropius
		40,	// Turtwig
		80,	// Typhlosion
		120,	// Tyranitar
		40,	// Tyrogue
		60,	// Umbreon
		20,	// Unown
		100,	// Ursaring
		20,	// Uxie
		60,	// Vaporeon
		40,	// Venomoth
		60,	// Venonat
		80,	// Venusaur
		60,	// Vespiquen
		40,	// Vibrava
		40,	// Victreebel
		60,	// Vigoroth
		40,	// Vileplume
		40,	// Volbeat
		40,	// Voltorb
		20,	// Vulpix
		100,	// Wailmer
		120,	// Wailord
		100,	// Walrein
		40,	// Wartortle
		60,	// Weavile
		20,	// Weedle
		20,	// Weepinbell
		20,	// Weezing
		40,	// Whiscash
		40,	// Whismur
		40,	// Wigglytuff
		20,	// Wingull
		60,	// Wobbuffet
		20,	// Wooper
		20,	// Wormadam-P
		20,	// Wormadam-S
		20,	// Wormadam-T
		20,	// Wurmple
		40,	// Wynaut
		40,	// Xatu
		60,	// Yanma
		80,	// Yanmega
		60,	// Zangoose
		80,	// Zapdos
		40,	// Zigzagoon
		20	// Zubat
	};
	return get_mass [name];
}

std::string Pokemon::get_name() const {
	static std::string const name_to_string [] = {
		"Abomasnow",
		"Abra",
		"Absol",
		"Aerodactyl",
		"Aggron",
		"Aipom",
		"Alakazam",
		"Altaria",
		"Ambipom",
		"Ampharos",
		"Anorith",
		"Arbok",
		"Arcanine",
		"Arceus",
		"Ariados",
		"Armaldo",
		"Aron",
		"Articuno",
		"Azelf",
		"Azumarill",
		"Azurill",
		"Bagon",
		"Baltoy",
		"Banette",
		"Barboach",
		"Bastiodon",
		"Bayleef",
		"Beautifly",
		"Beedrill",
		"Beldum",
		"Bellossom",
		"Bellsprout",
		"Bibarel",
		"Bidoof",
		"Blastoise",
		"Blaziken",
		"Blissey",
		"Bonsly",
		"Breloom",
		"Bronzong",
		"Bronzor",
		"Budew",
		"Buizel",
		"Bulbasaur",
		"Buneary",
		"Burmy",
		"Butterfree",
		"Cacnea",
		"Cacturne",
		"Camerupt",
		"Carnivine",
		"Carvanha",
		"Cascoon",
		"Castform",
		"Caterpie",
		"Celebi",
		"Chansey",
		"Charizard",
		"Charmander",
		"Charmeleon",
		"Chatot",
		"Cherrim",
		"Cherubi",
		"Chikorita",
		"Chimchar",
		"Chimecho",
		"Chinchou",
		"Chingling",
		"Clamperl",
		"Claydol",
		"Clefable",
		"Clefairy",
		"Cleffa",
		"Cloyster",
		"Combee",
		"Combusken",
		"Corphish",
		"Corsola",
		"Cradily",
		"Cranidos",
		"Crawdaunt",
		"Cresselia",
		"Croagunk",
		"Crobat",
		"Croconaw",
		"Cubone",
		"Cyndaquil",
		"Darkrai",
		"Delcatty",
		"Delibird",
		"Deoxys-A",
		"Deoxys-D",
		"Deoxys-M",
		"Deoxys-S",
		"Dewgong",
		"Dialga",
		"Diglett",
		"Ditto",
		"Dodrio",
		"Doduo",
		"Donphan",
		"Dragonair",
		"Dragonite",
		"Drapion",
		"Dratini",
		"Drifblim",
		"Drifloon",
		"Drowzee",
		"Dugtrio",
		"Dunsparce",
		"Dusclops",
		"Dusknoir",
		"Duskull",
		"Dustox",
		"Eevee",
		"Ekans",
		"Electabuzz",
		"Electivire",
		"Electrike",
		"Electrode",
		"Elekid",
		"Empoleon",
		"Entei",
		"Espeon",
		"Exeggcute",
		"Exeggutor",
		"Exploud",
		"Farfetch'd",
		"Fearow",
		"Feebas",
		"Feraligatr",
		"Finneon",
		"Flaaffy",
		"Flareon",
		"Floatzel",
		"Flygon",
		"Forretress",
		"Froslass",
		"Furret",
		"Gabite",
		"Gallade",
		"Garchomp",
		"Gardevoir",
		"Gastly",
		"Gastrodon",
		"Gengar",
		"Geodude",
		"Gible",
		"Girafarig",
		"Giratina-A",
		"Giratina-O",
		"Glaceon",
		"Glalie",
		"Glameow",
		"Gligar",
		"Gliscor",
		"Gloom",
		"Golbat",
		"Goldeen",
		"Golduck",
		"Golem",
		"Gorebyss",
		"Granbull",
		"Graveler",
		"Grimer",
		"Grotle",
		"Groudon",
		"Grovyle",
		"Growlithe",
		"Grumpig",
		"Gulpin",
		"Gyarados",
		"Happiny",
		"Hariyama",
		"Haunter",
		"Heatran",
		"Heracross",
		"Hippopotas",
		"Hippowdon",
		"Hitmonchan",
		"Hitmonlee",
		"Hitmontop",
		"Ho-Oh",
		"Honchkrow",
		"Hoothoot",
		"Hoppip",
		"Horsea",
		"Houndoom",
		"Houndour",
		"Huntail",
		"Hypno",
		"Igglybuff",
		"Illumise",
		"Infernape",
		"Ivysaur",
		"Jigglypuff",
		"Jirachi",
		"Jolteon",
		"Jumpluff",
		"Jynx",
		"Kabuto",
		"Kabutops",
		"Kadabra",
		"Kakuna",
		"Kangaskhan",
		"Kecleon",
		"Kingdra",
		"Kingler",
		"Kirlia",
		"Koffing",
		"Krabby",
		"Kricketot",
		"Kricketune",
		"Kyogre",
		"Lairon",
		"Lanturn",
		"Lapras",
		"Larvitar",
		"Latias",
		"Latios",
		"Leafeon",
		"Ledian",
		"Ledyba",
		"Lickilicky",
		"Lickitung",
		"Lileep",
		"Linoone",
		"Lombre",
		"Lopunny",
		"Lotad",
		"Loudred",
		"Lucario",
		"Ludicolo",
		"Lugia",
		"Lumineon",
		"Lunatone",
		"Luvdisc",
		"Luxio",
		"Luxray",
		"Machamp",
		"Machoke",
		"Machop",
		"Magby",
		"Magcargo",
		"Magikarp",
		"Magmar",
		"Magmortar",
		"Magnemite",
		"Magneton",
		"Magnezone",
		"Makuhita",
		"Mamoswine",
		"Manaphy",
		"Manectric",
		"Mankey",
		"Mantine",
		"Mantyke",
		"Mareep",
		"Marill",
		"Marowak",
		"Marshtomp",
		"Masquerain",
		"Mawile",
		"Medicham",
		"Meditite",
		"Meganium",
		"Meowth",
		"Mesprit",
		"Metagross",
		"Metang",
		"Metapod",
		"Mew",
		"Mewtwo",
		"Mightyena",
		"Milotic",
		"Miltank",
		"Mime Jr.",
		"Minun",
		"Misdreavus",
		"Mismagius",
		"Moltres",
		"Monferno",
		"Mothim",
		"Mr. Mime",
		"Mudkip",
		"Muk",
		"Munchlax",
		"Murkrow",
		"Natu",
		"Nidoking",
		"Nidoqueen",
		"Nidoran-F",
		"Nidoran-M",
		"Nidorina",
		"Nidorino",
		"Nincada",
		"Ninetales",
		"Ninjask",
		"Noctowl",
		"Nosepass",
		"Numel",
		"Nuzleaf",
		"Octillery",
		"Oddish",
		"Omanyte",
		"Omastar",
		"Onix",
		"Pachirisu",
		"Palkia",
		"Paras",
		"Parasect",
		"Pelipper",
		"Persian",
		"Phanpy",
		"Phione",
		"Pichu",
		"Pidgeot",
		"Pidgeotto",
		"Pidgey",
		"Pikachu",
		"Piloswine",
		"Pineco",
		"Pinsir",
		"Piplup",
		"Plusle",
		"Politoed",
		"Poliwag",
		"Poliwhirl",
		"Poliwrath",
		"Ponyta",
		"Poochyena",
		"Porygon",
		"Porygon-Z",
		"Porygon2",
		"Primeape",
		"Prinplup",
		"Probopass",
		"Psyduck",
		"Pupitar",
		"Purugly",
		"Quagsire",
		"Quilava",
		"Qwilfish",
		"Raichu",
		"Raikou",
		"Ralts",
		"Rampardos",
		"Rapidash",
		"Raticate",
		"Rattata",
		"Rayquaza",
		"Regice",
		"Regigigas",
		"Regirock",
		"Registeel",
		"Relicanth",
		"Remoraid",
		"Rhydon",
		"Rhyhorn",
		"Rhyperior",
		"Riolu",
		"Roselia",
		"Roserade",
		"Rotom",
		"Rotom-C",
		"Rotom-F",
		"Rotom-H",
		"Rotom-S",
		"Rotom-W",
		"Sableye",
		"Salamence",
		"Sandshrew",
		"Sandslash",
		"Sceptile",
		"Scizor",
		"Scyther",
		"Seadra",
		"Seaking",
		"Sealeo",
		"Seedot",
		"Seel",
		"Sentret",
		"Seviper",
		"Sharpedo",
		"Shaymin-L",
		"Shaymin-S",
		"Shedinja",
		"Shelgon",
		"Shellder",
		"Shellos",
		"Shieldon",
		"Shiftry",
		"Shinx",
		"Shroomish",
		"Shuckle",
		"Shuppet",
		"Silcoon",
		"Skarmory",
		"Skiploom",
		"Skitty",
		"Skorupi",
		"Skuntank",
		"Slaking",
		"Slakoth",
		"Slowbro",
		"Slowking",
		"Slowpoke",
		"Slugma",
		"Smeargle",
		"Smoochum",
		"Sneasel",
		"Snorlax",
		"Snorunt",
		"Snover",
		"Snubbull",
		"Solrock",
		"Spearow",
		"Spheal",
		"Spinarak",
		"Spinda",
		"Spiritomb",
		"Spoink",
		"Squirtle",
		"Stantler",
		"Staraptor",
		"Staravia",
		"Starly",
		"Starmie",
		"Staryu",
		"Steelix",
		"Stunky",
		"Sudowoodo",
		"Suicune",
		"Sunflora",
		"Sunkern",
		"Surskit",
		"Swablu",
		"Swalot",
		"Swampert",
		"Swellow",
		"Swinub",
		"Taillow",
		"Tangela",
		"Tangrowth",
		"Tauros",
		"Teddiursa",
		"Tentacool",
		"Tentacruel",
		"Togekiss",
		"Togepi",
		"Togetic",
		"Torchic",
		"Torkoal",
		"Torterra",
		"Totodile",
		"Toxicroak",
		"Trapinch",
		"Treecko",
		"Tropius",
		"Turtwig",
		"Typhlosion",
		"Tyranitar",
		"Tyrogue",
		"Umbreon",
		"Unown",
		"Ursaring",
		"Uxie",
		"Vaporeon",
		"Venomoth",
		"Venonat",
		"Venusaur",
		"Vespiquen",
		"Vibrava",
		"Victreebel",
		"Vigoroth",
		"Vileplume",
		"Volbeat",
		"Voltorb",
		"Vulpix",
		"Wailmer",
		"Wailord",
		"Walrein",
		"Wartortle",
		"Weavile",
		"Weedle",
		"Weepinbell",
		"Weezing",
		"Whiscash",
		"Whismur",
		"Wigglytuff",
		"Wingull",
		"Wobbuffet",
		"Wooper",
		"Wormadam-P",
		"Wormadam-S",
		"Wormadam-T",
		"Wurmple",
		"Wynaut",
		"Xatu",
		"Yanma",
		"Yanmega",
		"Zangoose",
		"Zapdos",
		"Zigzagoon",
		"Zubat"
	};
	return name_to_string [name];
}

Species Pokemon::name_from_string (std::string const & str) {
	static std::map <std::string, Species> const converter {
		{ "Abomasnow", ABOMASNOW },
		{ "Abra", ABRA },
		{ "Absol", ABSOL },
		{ "Aerodactyl", AERODACTYL },
		{ "Aggron", AGGRON },
		{ "Aipom", AIPOM },
		{ "Alakazam", ALAKAZAM },
		{ "Altaria", ALTARIA },
		{ "Ambipom", AMBIPOM },
		{ "Ampharos", AMPHAROS },
		{ "Anorith", ANORITH },
		{ "Arbok", ARBOK },
		{ "Arcanine", ARCANINE },
		{ "Arceus", ARCEUS },
		{ "Ariados", ARIADOS },
		{ "Armaldo", ARMALDO },
		{ "Aron", ARON },
		{ "Articuno", ARTICUNO },
		{ "Azelf", AZELF },
		{ "Azumarill", AZUMARILL },
		{ "Azurill", AZURILL },
		{ "Bagon", BAGON },
		{ "Baltoy", BALTOY },
		{ "Banette", BANETTE },
		{ "Barboach", BARBOACH },
		{ "Bastiodon", BASTIODON },
		{ "Bayleef", BAYLEEF },
		{ "Beautifly", BEAUTIFLY },
		{ "Beedrill", BEEDRILL },
		{ "Beldum", BELDUM },
		{ "Bellossom", BELLOSSOM },
		{ "Bellsprout", BELLSPROUT },
		{ "Bibarel", BIBAREL },
		{ "Bidoof", BIDOOF },
		{ "Blastoise", BLASTOISE },
		{ "Blaziken", BLAZIKEN },
		{ "Blissey", BLISSEY },
		{ "Bonsly", BONSLY },
		{ "Breloom", BRELOOM },
		{ "Bronzong", BRONZONG },
		{ "Bronzor", BRONZOR },
		{ "Budew", BUDEW },
		{ "Buizel", BUIZEL },
		{ "Bulbasaur", BULBASAUR },
		{ "Buneary", BUNEARY },
		{ "Burmy", BURMY },
		{ "Butterfree", BUTTERFREE },
		{ "Cacnea", CACNEA },
		{ "Cacturne", CACTURNE },
		{ "Camerupt", CAMERUPT },
		{ "Carnivine", CARNIVINE },
		{ "Carvanha", CARVANHA },
		{ "Cascoon", CASCOON },
		{ "Castform", CASTFORM },
		{ "Caterpie", CATERPIE },
		{ "Celebi", CELEBI },
		{ "Chansey", CHANSEY },
		{ "Charizard", CHARIZARD },
		{ "Charmander", CHARMANDER },
		{ "Charmeleon", CHARMELEON },
		{ "Chatot", CHATOT },
		{ "Cherrim", CHERRIM },
		{ "Cherubi", CHERUBI },
		{ "Chikorita", CHIKORITA },
		{ "Chimchar", CHIMCHAR },
		{ "Chimecho", CHIMECHO },
		{ "Chinchou", CHINCHOU },
		{ "Chingling", CHINGLING },
		{ "Clamperl", CLAMPERL },
		{ "Claydol", CLAYDOL },
		{ "Clefable", CLEFABLE },
		{ "Clefairy", CLEFAIRY },
		{ "Cleffa", CLEFFA },
		{ "Cloyster", CLOYSTER },
		{ "Combee", COMBEE },
		{ "Combusken", COMBUSKEN },
		{ "Corphish", CORPHISH },
		{ "Corsola", CORSOLA },
		{ "Cradily", CRADILY },
		{ "Cranidos", CRANIDOS },
		{ "Crawdaunt", CRAWDAUNT },
		{ "Cresselia", CRESSELIA },
		{ "Croagunk", CROAGUNK },
		{ "Crobat", CROBAT },
		{ "Croconaw", CROCONAW },
		{ "Cubone", CUBONE },
		{ "Cyndaquil", CYNDAQUIL },
		{ "Darkrai", DARKRAI },
		{ "Delcatty", DELCATTY },
		{ "Delibird", DELIBIRD },
		{ "Deoxys-A", DEOXYS_A },
		{ "Deoxys-a", DEOXYS_A },
		{ "Deoxys-F", DEOXYS_A },
		{ "Deoxys-f", DEOXYS_A },
		{ "Deoxys-D", DEOXYS_D },
		{ "Deoxys-d", DEOXYS_D },
		{ "Deoxys-L", DEOXYS_D },
		{ "Deoxys-l", DEOXYS_D },
		{ "Deoxys", DEOXYS_M },
		{ "Deoxys-M", DEOXYS_M },
		{ "Deoxys-m", DEOXYS_M },
		{ "Deoxys-S", DEOXYS_S },
		{ "Deoxys-s", DEOXYS_S },
		{ "Deoxys-E", DEOXYS_S },
		{ "Deoxys-e", DEOXYS_S },
		{ "Dewgong", DEWGONG },
		{ "Dialga", DIALGA },
		{ "Diglett", DIGLETT },
		{ "Ditto", DITTO },
		{ "Dodrio", DODRIO },
		{ "Doduo", DODUO },
		{ "Donphan", DONPHAN },
		{ "Dragonair", DRAGONAIR },
		{ "Dragonite", DRAGONITE },
		{ "Drapion", DRAPION },
		{ "Dratini", DRATINI },
		{ "Drifblim", DRIFBLIM },
		{ "Drifloon", DRIFLOON },
		{ "Drowzee", DROWZEE },
		{ "Dugtrio", DUGTRIO },
		{ "Dunsparce", DUNSPARCE },
		{ "Dusclops", DUSCLOPS },
		{ "Dusknoir", DUSKNOIR },
		{ "Duskull", DUSKULL },
		{ "Dustox", DUSTOX },
		{ "Eevee", EEVEE },
		{ "Ekans", EKANS },
		{ "Electabuzz", ELECTABUZZ },
		{ "Electivire", ELECTIVIRE },
		{ "Electrike", ELECTRIKE },
		{ "Electrode", ELECTRODE },
		{ "Elekid", ELEKID },
		{ "Empoleon", EMPOLEON },
		{ "Entei", ENTEI },
		{ "Espeon", ESPEON },
		{ "Exeggcute", EXEGGCUTE },
		{ "Exeggutor", EXEGGUTOR },
		{ "Exploud", EXPLOUD },
		{ "Farfetch'd", FARFETCHD },
		{ "Fearow", FEAROW },
		{ "Feebas", FEEBAS },
		{ "Feraligatr", FERALIGATR },
		{ "Finneon", FINNEON },
		{ "Flaaffy", FLAAFFY },
		{ "Flareon", FLAREON },
		{ "Floatzel", FLOATZEL },
		{ "Flygon", FLYGON },
		{ "Forretress", FORRETRESS },
		{ "Froslass", FROSLASS },
		{ "Furret", FURRET },
		{ "Gabite", GABITE },
		{ "Gallade", GALLADE },
		{ "Garchomp", GARCHOMP },
		{ "Gardevoir", GARDEVOIR },
		{ "Gastly", GASTLY },
		{ "Gastrodon", GASTRODON },
		{ "Gengar", GENGAR },
		{ "Geodude", GEODUDE },
		{ "Gible", GIBLE },
		{ "Girafarig", GIRAFARIG },
		{ "Giratina", GIRATINA_A },
		{ "Giratina-A", GIRATINA_A },
		{ "Giratina-a", GIRATINA_A },
		{ "Giratina-O", GIRATINA_O },
		{ "Giratina-o", GIRATINA_O },
		{ "Glaceon", GLACEON },
		{ "Glalie", GLALIE },
		{ "Glameow", GLAMEOW },
		{ "Gligar", GLIGAR },
		{ "Gliscor", GLISCOR },
		{ "Gloom", GLOOM },
		{ "Golbat", GOLBAT },
		{ "Goldeen", GOLDEEN },
		{ "Golduck", GOLDUCK },
		{ "Golem", GOLEM },
		{ "Gorebyss", GOREBYSS },
		{ "Granbull", GRANBULL },
		{ "Graveler", GRAVELER },
		{ "Grimer", GRIMER },
		{ "Grotle", GROTLE },
		{ "Groudon", GROUDON },
		{ "Grovyle", GROVYLE },
		{ "Growlithe", GROWLITHE },
		{ "Grumpig", GRUMPIG },
		{ "Gulpin", GULPIN },
		{ "Gyarados", GYARADOS },
		{ "Happiny", HAPPINY },
		{ "Hariyama", HARIYAMA },
		{ "Haunter", HAUNTER },
		{ "Heatran", HEATRAN },
		{ "Heracross", HERACROSS },
		{ "Hippopotas", HIPPOPOTAS },
		{ "Hippowdon", HIPPOWDON },
		{ "Hitmonchan", HITMONCHAN },
		{ "Hitmonlee", HITMONLEE },
		{ "Hitmontop", HITMONTOP },
		{ "Ho-Oh", HO_OH },
		{ "Ho-oh", HO_OH },
		{ "Honchkrow", HONCHKROW },
		{ "Hoothoot", HOOTHOOT },
		{ "Hoppip", HOPPIP },
		{ "Horsea", HORSEA },
		{ "Houndoom", HOUNDOOM },
		{ "Houndour", HOUNDOUR },
		{ "Huntail", HUNTAIL },
		{ "Hypno", HYPNO },
		{ "Igglybuff", IGGLYBUFF },
		{ "Illumise", ILLUMISE },
		{ "Infernape", INFERNAPE },
		{ "Ivysaur", IVYSAUR },
		{ "Jigglypuff", JIGGLYPUFF },
		{ "Jirachi", JIRACHI },
		{ "Jolteon", JOLTEON },
		{ "Jumpluff", JUMPLUFF },
		{ "Jynx", JYNX },
		{ "Kabuto", KABUTO },
		{ "Kabutops", KABUTOPS },
		{ "Kadabra", KADABRA },
		{ "Kakuna", KAKUNA },
		{ "Kangaskhan", KANGASKHAN },
		{ "Kecleon", KECLEON },
		{ "Kingdra", KINGDRA },
		{ "Kingler", KINGLER },
		{ "Kirlia", KIRLIA },
		{ "Koffing", KOFFING },
		{ "Krabby", KRABBY },
		{ "Kricketot", KRICKETOT },
		{ "Kricketune", KRICKETUNE },
		{ "Kyogre", KYOGRE },
		{ "Lairon", LAIRON },
		{ "Lanturn", LANTURN },
		{ "Lapras", LAPRAS },
		{ "Larvitar", LARVITAR },
		{ "Latias", LATIAS },
		{ "Latios", LATIOS },
		{ "Leafeon", LEAFEON },
		{ "Ledian", LEDIAN },
		{ "Ledyba", LEDYBA },
		{ "Lickilicky", LICKILICKY },
		{ "Lickitung", LICKITUNG },
		{ "Lileep", LILEEP },
		{ "Linoone", LINOONE },
		{ "Lombre", LOMBRE },
		{ "Lopunny", LOPUNNY },
		{ "Lotad", LOTAD },
		{ "Loudred", LOUDRED },
		{ "Lucario", LUCARIO },
		{ "Ludicolo", LUDICOLO },
		{ "Lugia", LUGIA },
		{ "Lumineon", LUMINEON },
		{ "Lunatone", LUNATONE },
		{ "Luvdisc", LUVDISC },
		{ "Luxio", LUXIO },
		{ "Luxray", LUXRAY },
		{ "Machamp", MACHAMP },
		{ "Machoke", MACHOKE },
		{ "Machop", MACHOP },
		{ "Magby", MAGBY },
		{ "Magcargo", MAGCARGO },
		{ "Magikarp", MAGIKARP },
		{ "Magmar", MAGMAR },
		{ "Magmortar", MAGMORTAR },
		{ "Magnemite", MAGNEMITE },
		{ "Magneton", MAGNETON },
		{ "Magnezone", MAGNEZONE },
		{ "Makuhita", MAKUHITA },
		{ "Mamoswine", MAMOSWINE },
		{ "Manaphy", MANAPHY },
		{ "Manectric", MANECTRIC },
		{ "Mankey", MANKEY },
		{ "Mantine", MANTINE },
		{ "Mantyke", MANTYKE },
		{ "Mareep", MAREEP },
		{ "Marill", MARILL },
		{ "Marowak", MAROWAK },
		{ "Marshtomp", MARSHTOMP },
		{ "Masquerain", MASQUERAIN },
		{ "Mawile", MAWILE },
		{ "Medicham", MEDICHAM },
		{ "Meditite", MEDITITE },
		{ "Meganium", MEGANIUM },
		{ "Meowth", MEOWTH },
		{ "Mesprit", MESPRIT },
		{ "Metagross", METAGROSS },
		{ "Metang", METANG },
		{ "Metapod", METAPOD },
		{ "Mew", MEW },
		{ "Mewtwo", MEWTWO },
		{ "Mightyena", MIGHTYENA },
		{ "Milotic", MILOTIC },
		{ "Miltank", MILTANK },
		{ "Mime Jr.", MIME_JR },
		{ "Minun", MINUN },
		{ "Misdreavus", MISDREAVUS },
		{ "Mismagius", MISMAGIUS },
		{ "Moltres", MOLTRES },
		{ "Monferno", MONFERNO },
		{ "Mothim", MOTHIM },
		{ "Mr. Mime", MR_MIME },
		{ "Mudkip", MUDKIP },
		{ "Muk", MUK },
		{ "Munchlax", MUNCHLAX },
		{ "Murkrow", MURKROW },
		{ "Natu", NATU },
		{ "Nidoking", NIDOKING },
		{ "Nidoqueen", NIDOQUEEN },
		{ "Nidoran-F", NIDORAN_F },
		{ "Nidoran-f", NIDORAN_F },
		{ "Nidoran-M", NIDORAN_M },
		{ "Nidoran-m", NIDORAN_M },
		{ "Nidorina", NIDORINA },
		{ "Nidorino", NIDORINO },
		{ "Nincada", NINCADA },
		{ "Ninetales", NINETALES },
		{ "Ninjask", NINJASK },
		{ "Noctowl", NOCTOWL },
		{ "Nosepass", NOSEPASS },
		{ "Numel", NUMEL },
		{ "Nuzleaf", NUZLEAF },
		{ "Octillery", OCTILLERY },
		{ "Oddish", ODDISH },
		{ "Omanyte", OMANYTE },
		{ "Omastar", OMASTAR },
		{ "Onix", ONIX },
		{ "Pachirisu", PACHIRISU },
		{ "Palkia", PALKIA },
		{ "Paras", PARAS },
		{ "Parasect", PARASECT },
		{ "Pelipper", PELIPPER },
		{ "Persian", PERSIAN },
		{ "Phanpy", PHANPY },
		{ "Phione", PHIONE },
		{ "Pichu", PICHU },
		{ "Pidgeot", PIDGEOT },
		{ "Pidgeotto", PIDGEOTTO },
		{ "Pidgey", PIDGEY },
		{ "Pikachu", PIKACHU },
		{ "Piloswine", PILOSWINE },
		{ "Pineco", PINECO },
		{ "Pinsir", PINSIR },
		{ "Piplup", PIPLUP },
		{ "Plusle", PLUSLE },
		{ "Politoed", POLITOED },
		{ "Poliwag", POLIWAG },
		{ "Poliwhirl", POLIWHIRL },
		{ "Poliwrath", POLIWRATH },
		{ "Ponyta", PONYTA },
		{ "Poochyena", POOCHYENA },
		{ "Porygon", PORYGON },
		{ "Porygon-Z", PORYGON_Z },
		{ "Porygon-z", PORYGON_Z },
		{ "Porygonz", PORYGON_Z },
		{ "Porygon2", PORYGON2 },
		{ "Primeape", PRIMEAPE },
		{ "Prinplup", PRINPLUP },
		{ "Probopass", PROBOPASS },
		{ "Psyduck", PSYDUCK },
		{ "Pupitar", PUPITAR },
		{ "Purugly", PURUGLY },
		{ "Quagsire", QUAGSIRE },
		{ "Quilava", QUILAVA },
		{ "Qwilfish", QWILFISH },
		{ "Raichu", RAICHU },
		{ "Raikou", RAIKOU },
		{ "Ralts", RALTS },
		{ "Rampardos", RAMPARDOS },
		{ "Rapidash", RAPIDASH },
		{ "Raticate", RATICATE },
		{ "Rattata", RATTATA },
		{ "Rayquaza", RAYQUAZA },
		{ "Regice", REGICE },
		{ "Regigigas", REGIGIGAS },
		{ "Regirock", REGIROCK },
		{ "Registeel", REGISTEEL },
		{ "Relicanth", RELICANTH },
		{ "Remoraid", REMORAID },
		{ "Rhydon", RHYDON },
		{ "Rhyhorn", RHYHORN },
		{ "Rhyperior", RHYPERIOR },
		{ "Riolu", RIOLU },
		{ "Roselia", ROSELIA },
		{ "Roserade", ROSERADE },
		{ "Rotom", ROTOM },
		{ "Rotom-C", ROTOM_C },
		{ "Rotom-c", ROTOM_C },
		{ "Rotom-F", ROTOM_F },
		{ "Rotom-f", ROTOM_F },
		{ "Rotom-H", ROTOM_H },
		{ "Rotom-h", ROTOM_H },
		{ "Rotom-S", ROTOM_S },
		{ "Rotom-s", ROTOM_S },
		{ "Rotom-W", ROTOM_W },
		{ "Rotom-w", ROTOM_W },
		{ "Sableye", SABLEYE },
		{ "Salamence", SALAMENCE },
		{ "Sandshrew", SANDSHREW },
		{ "Sandslash", SANDSLASH },
		{ "Sceptile", SCEPTILE },
		{ "Scizor", SCIZOR },
		{ "Scyther", SCYTHER },
		{ "Seadra", SEADRA },
		{ "Seaking", SEAKING },
		{ "Sealeo", SEALEO },
		{ "Seedot", SEEDOT },
		{ "Seel", SEEL },
		{ "Sentret", SENTRET },
		{ "Seviper", SEVIPER },
		{ "Sharpedo", SHARPEDO },
		{ "Shaymin", SHAYMIN_L },
		{ "Shaymin-L", SHAYMIN_L },
		{ "Shaymin-l", SHAYMIN_L },
		{ "Shaymin-S", SHAYMIN_L },
		{ "Shaymin-s", SHAYMIN_S },
		{ "Shedinja", SHEDINJA },
		{ "Shelgon", SHELGON },
		{ "Shellder", SHELLDER },
		{ "Shellos", SHELLOS },
		{ "Shieldon", SHIELDON },
		{ "Shiftry", SHIFTRY },
		{ "Shinx", SHINX },
		{ "Shroomish", SHROOMISH },
		{ "Shuckle", SHUCKLE },
		{ "Shuppet", SHUPPET },
		{ "Silcoon", SILCOON },
		{ "Skarmory", SKARMORY },
		{ "Skiploom", SKIPLOOM },
		{ "Skitty", SKITTY },
		{ "Skorupi", SKORUPI },
		{ "Skuntank", SKUNTANK },
		{ "Slaking", SLAKING },
		{ "Slakoth", SLAKOTH },
		{ "Slowbro", SLOWBRO },
		{ "Slowking", SLOWKING },
		{ "Slowpoke", SLOWPOKE },
		{ "Slugma", SLUGMA },
		{ "Smeargle", SMEARGLE },
		{ "Smoochum", SMOOCHUM },
		{ "Sneasel", SNEASEL },
		{ "Snorlax", SNORLAX },
		{ "Snorunt", SNORUNT },
		{ "Snover", SNOVER },
		{ "Snubbull", SNUBBULL },
		{ "Solrock", SOLROCK },
		{ "Spearow", SPEAROW },
		{ "Spheal", SPHEAL },
		{ "Spinarak", SPINARAK },
		{ "Spinda", SPINDA },
		{ "Spiritomb", SPIRITOMB },
		{ "Spoink", SPOINK },
		{ "Squirtle", SQUIRTLE },
		{ "Stantler", STANTLER },
		{ "Staraptor", STARAPTOR },
		{ "Staravia", STARAVIA },
		{ "Starly", STARLY },
		{ "Starmie", STARMIE },
		{ "Staryu", STARYU },
		{ "Steelix", STEELIX },
		{ "Stunky", STUNKY },
		{ "Sudowoodo", SUDOWOODO },
		{ "Suicune", SUICUNE },
		{ "Sunflora", SUNFLORA },
		{ "Sunkern", SUNKERN },
		{ "Surskit", SURSKIT },
		{ "Swablu", SWABLU },
		{ "Swalot", SWALOT },
		{ "Swampert", SWAMPERT },
		{ "Swellow", SWELLOW },
		{ "Swinub", SWINUB },
		{ "Taillow", TAILLOW },
		{ "Tangela", TANGELA },
		{ "Tangrowth", TANGROWTH },
		{ "Tauros", TAUROS },
		{ "Teddiursa", TEDDIURSA },
		{ "Tentacool", TENTACOOL },
		{ "Tentacruel", TENTACRUEL },
		{ "Togekiss", TOGEKISS },
		{ "Togepi", TOGEPI },
		{ "Togetic", TOGETIC },
		{ "Torchic", TORCHIC },
		{ "Torkoal", TORKOAL },
		{ "Torterra", TORTERRA },
		{ "Totodile", TOTODILE },
		{ "Toxicroak", TOXICROAK },
		{ "Trapinch", TRAPINCH },
		{ "Treecko", TREECKO },
		{ "Tropius", TROPIUS },
		{ "Turtwig", TURTWIG },
		{ "Typhlosion", TYPHLOSION },
		{ "Tyranitar", TYRANITAR },
		{ "Tyrogue", TYROGUE },
		{ "Umbreon", UMBREON },
		{ "Unown", UNOWN },
		{ "Ursaring", URSARING },
		{ "Uxie", UXIE },
		{ "Vaporeon", VAPOREON },
		{ "Venomoth", VENOMOTH },
		{ "Venonat", VENONAT },
		{ "Venusaur", VENUSAUR },
		{ "Vespiquen", VESPIQUEN },
		{ "Vibrava", VIBRAVA },
		{ "Victreebel", VICTREEBEL },
		{ "Vigoroth", VIGOROTH },
		{ "Vileplume", VILEPLUME },
		{ "Volbeat", VOLBEAT },
		{ "Voltorb", VOLTORB },
		{ "Vulpix", VULPIX },
		{ "Wailmer", WAILMER },
		{ "Wailord", WAILORD },
		{ "Walrein", WALREIN },
		{ "Wartortle", WARTORTLE },
		{ "Weavile", WEAVILE },
		{ "Weedle", WEEDLE },
		{ "Weepinbell", WEEPINBELL },
		{ "Weezing", WEEZING },
		{ "Whiscash", WHISCASH },
		{ "Whismur", WHISMUR },
		{ "Wigglytuff", WIGGLYTUFF },
		{ "Wingull", WINGULL },
		{ "Wobbuffet", WOBBUFFET },
		{ "Wooper", WOOPER },
		{ "Wormadam", WORMADAM_P },
		{ "Wormadam-P", WORMADAM_P },
		{ "Wormadam-g", WORMADAM_S },
		{ "Wormadam-S", WORMADAM_S },
		{ "Wormadam-s", WORMADAM_T },
		{ "Wormadam-T", WORMADAM_T },
		{ "Wurmple", WURMPLE },
		{ "Wynaut", WYNAUT },
		{ "Xatu", XATU },
		{ "Yanma", YANMA },
		{ "Yanmega", YANMEGA },
		{ "Zangoose", ZANGOOSE },
		{ "Zapdos", ZAPDOS },
		{ "Zigzagoon", ZIGZAGOON },
		{ "Zubat", ZUBAT }
	};
	auto const it = converter.find (str);
	return (it != converter.end ()) ? it->second : END_SPECIES;
}

void Pokemon::set_name_from_string (std::string const & str) {
	name = name_from_string (str);
}
}
