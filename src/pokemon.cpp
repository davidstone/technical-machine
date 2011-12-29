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

#include "pokemon.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "ability.hpp"
#include "active.hpp"
#include "gender.hpp"
#include "move.hpp"
#include "species.hpp"
#include "stat.hpp"

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
	constexpr static Type::Types get_type [][2] = {
		{ Type::GRASS, Type::ICE },				// Abomasnow
		{ Type::PSYCHIC, Type::TYPELESS },		// Abra
		{ Type::DARK, Type::TYPELESS },			// Absol
		{ Type::ROCK, Type::FLYING },			// Aerodactyl
		{ Type::STEEL, Type::ROCK },			// Aggron
		{ Type::NORMAL, Type::TYPELESS },		// Aipom
		{ Type::PSYCHIC, Type::TYPELESS },		// Alakazam
		{ Type::DRAGON, Type::FLYING },			// Altaria
		{ Type::NORMAL, Type::TYPELESS },		// Ambipom
		{ Type::ELECTRIC, Type::TYPELESS },		// Ampharos
		{ Type::ROCK, Type::BUG },				// Anorith
		{ Type::POISON, Type::TYPELESS },		// Arbok
		{ Type::FIRE, Type::TYPELESS },			// Arcanine
		{ Type::NORMAL, Type::TYPELESS },		// Arceus
		{ Type::BUG, Type::POISON },			// Ariados
		{ Type::ROCK, Type::BUG },				// Armaldo
		{ Type::STEEL, Type::ROCK },			// Aron
		{ Type::ICE, Type::FLYING },			// Articuno
		{ Type::PSYCHIC, Type::TYPELESS },		// Azelf
		{ Type::WATER, Type::TYPELESS },		// Azumarill
		{ Type::NORMAL, Type::TYPELESS },		// Azurill
		{ Type::DRAGON, Type::TYPELESS },		// Bagon
		{ Type::GROUND, Type::PSYCHIC },		// Baltoy
		{ Type::GHOST, Type::TYPELESS },		// Banette
		{ Type::WATER, Type::GROUND },			// Barboach
		{ Type::ROCK, Type::STEEL },			// Bastiodon
		{ Type::GRASS, Type::TYPELESS },		// Bayleef
		{ Type::BUG, Type::FLYING },			// Beautifly
		{ Type::BUG, Type::POISON },			// Beedrill
		{ Type::STEEL, Type::PSYCHIC },			// Beldum
		{ Type::GRASS, Type::TYPELESS },		// Bellossom
		{ Type::GRASS, Type::POISON },			// Bellsprout
		{ Type::NORMAL, Type::WATER },			// Bibarel
		{ Type::NORMAL, Type::TYPELESS },		// Bidoof
		{ Type::WATER, Type::TYPELESS },		// Blastoise
		{ Type::FIRE, Type::FIGHTING },			// Blaziken
		{ Type::NORMAL, Type::TYPELESS },		// Blissey
		{ Type::ROCK, Type::TYPELESS },			// Bonsly
		{ Type::GRASS, Type::FIGHTING },		// Breloom
		{ Type::STEEL, Type::PSYCHIC },			// Bronzong
		{ Type::STEEL, Type::PSYCHIC },			// Bronzor
		{ Type::GRASS, Type::POISON },			// Budew
		{ Type::WATER, Type::TYPELESS },		// Buizel
		{ Type::GRASS, Type::POISON },			// Bulbasaur
		{ Type::NORMAL, Type::TYPELESS },		// Buneary
		{ Type::BUG, Type::TYPELESS },			// Burmy
		{ Type::BUG, Type::FLYING },			// Butterfree
		{ Type::GRASS, Type::TYPELESS },		// Cacnea
		{ Type::GRASS, Type::DARK },			// Cacturne
		{ Type::FIRE, Type::GROUND },			// Camerupt
		{ Type::GRASS, Type::TYPELESS },		// Carnivine
		{ Type::WATER, Type::DARK },			// Carvanha
		{ Type::BUG, Type::TYPELESS },			// Cascoon
		{ Type::NORMAL, Type::TYPELESS },		// Castform
		{ Type::BUG, Type::TYPELESS },			// Caterpie
		{ Type::PSYCHIC, Type::GRASS },			// Celebi
		{ Type::NORMAL, Type::TYPELESS },		// Chansey
		{ Type::FIRE, Type::FLYING },			// Charizard
		{ Type::FIRE, Type::TYPELESS },			// Charmander
		{ Type::FIRE, Type::TYPELESS },			// Charmeleon
		{ Type::NORMAL, Type::FLYING },			// Chatot
		{ Type::GRASS, Type::TYPELESS },		// Cherrim
		{ Type::GRASS, Type::TYPELESS },		// Cherubi
		{ Type::GRASS, Type::TYPELESS },		// Chikorita
		{ Type::FIRE, Type::TYPELESS },			// Chimchar
		{ Type::PSYCHIC, Type::TYPELESS },		// Chimecho
		{ Type::WATER, Type::ELECTRIC },		// Chinchou
		{ Type::PSYCHIC, Type::TYPELESS },		// Chingling
		{ Type::WATER, Type::TYPELESS },		// Clamperl
		{ Type::GROUND, Type::PSYCHIC },		// Claydol
		{ Type::NORMAL, Type::TYPELESS },		// Clefable
		{ Type::NORMAL, Type::TYPELESS },		// Clefairy
		{ Type::NORMAL, Type::TYPELESS },		// Cleffa
		{ Type::WATER, Type::ICE },				// Cloyster
		{ Type::BUG, Type::FLYING },			// Combee
		{ Type::FIRE, Type::FIGHTING },			// Combusken
		{ Type::WATER, Type::TYPELESS },		// Corphish
		{ Type::WATER, Type::ROCK },			// Corsola
		{ Type::ROCK, Type::GRASS },			// Cradily
		{ Type::ROCK, Type::TYPELESS },			// Cranidos
		{ Type::WATER, Type::DARK },			// Crawdaunt
		{ Type::PSYCHIC, Type::TYPELESS },		// Cresselia
		{ Type::POISON, Type::FIGHTING },		// Croagunk
		{ Type::POISON, Type::FLYING },			// Crobat
		{ Type::WATER, Type::TYPELESS },		// Croconaw
		{ Type::GROUND, Type::TYPELESS },		// Cubone
		{ Type::FIRE, Type::TYPELESS },			// Cyndaquil
		{ Type::DARK, Type::TYPELESS },			// Darkrai
		{ Type::NORMAL, Type::TYPELESS },		// Delcatty
		{ Type::ICE, Type::FLYING },			// Delibird
		{ Type::PSYCHIC, Type::TYPELESS },		// Deoxys-A
		{ Type::PSYCHIC, Type::TYPELESS },		// Deoxys-D
		{ Type::PSYCHIC, Type::TYPELESS },		// Deoxys-M
		{ Type::PSYCHIC, Type::TYPELESS },		// Deoxys-S
		{ Type::WATER, Type::ICE },				// Dewgong
		{ Type::STEEL, Type::DRAGON },			// Dialga
		{ Type::GROUND, Type::TYPELESS },		// Diglett
		{ Type::NORMAL, Type::TYPELESS },		// Ditto
		{ Type::NORMAL, Type::FLYING },			// Dodrio
		{ Type::NORMAL, Type::FLYING },			// Doduo
		{ Type::GROUND, Type::TYPELESS },		// Donphan
		{ Type::DRAGON, Type::TYPELESS },		// Dragonair
		{ Type::DRAGON, Type::FLYING },			// Dragonite
		{ Type::POISON, Type::DARK },			// Drapion
		{ Type::DRAGON, Type::TYPELESS },		// Dratini
		{ Type::GHOST, Type::FLYING },			// Drifblim
		{ Type::GHOST, Type::FLYING },			// Drifloon
		{ Type::PSYCHIC, Type::TYPELESS },		// Drowzee
		{ Type::GROUND, Type::TYPELESS },		// Dugtrio
		{ Type::NORMAL, Type::TYPELESS },		// Dunsparce
		{ Type::GHOST, Type::TYPELESS },		// Dusclops
		{ Type::GHOST, Type::TYPELESS },		// Dusknoir
		{ Type::GHOST, Type::TYPELESS },		// Duskull
		{ Type::BUG, Type::POISON },			// Dustox
		{ Type::NORMAL, Type::TYPELESS },		// Eevee
		{ Type::POISON, Type::TYPELESS },		// Ekans
		{ Type::ELECTRIC, Type::TYPELESS },		// Electabuzz
		{ Type::ELECTRIC, Type::TYPELESS },		// Electivire
		{ Type::ELECTRIC, Type::TYPELESS },		// Electrike
		{ Type::ELECTRIC, Type::TYPELESS },		// Electrode
		{ Type::ELECTRIC, Type::TYPELESS },		// Elekid
		{ Type::WATER, Type::STEEL },			// Empoleon
		{ Type::FIRE, Type::TYPELESS },			// Entei
		{ Type::PSYCHIC, Type::TYPELESS },		// Espeon
		{ Type::GRASS, Type::PSYCHIC },			// Exeggcute
		{ Type::GRASS, Type::PSYCHIC },			// Exeggutor
		{ Type::NORMAL, Type::TYPELESS },		// Exploud
		{ Type::NORMAL, Type::FLYING },			// Farfetch'd
		{ Type::NORMAL, Type::FLYING },			// Fearow
		{ Type::WATER, Type::TYPELESS },		// Feebas
		{ Type::WATER, Type::TYPELESS },		// Feraligatr
		{ Type::WATER, Type::TYPELESS },		// Finneon
		{ Type::ELECTRIC, Type::TYPELESS },		// Flaaffy
		{ Type::FIRE, Type::TYPELESS },			// Flareon
		{ Type::WATER, Type::TYPELESS },		// Floatzel
		{ Type::GROUND, Type::DRAGON },			// Flygon
		{ Type::BUG, Type::STEEL },				// Forretress
		{ Type::ICE, Type::GHOST },				// Froslass
		{ Type::NORMAL, Type::TYPELESS },		// Furret
		{ Type::DRAGON, Type::GROUND },			// Gabite
		{ Type::PSYCHIC, Type::FIGHTING },		// Gallade
		{ Type::DRAGON, Type::GROUND },			// Garchomp
		{ Type::PSYCHIC, Type::TYPELESS },		// Gardevoir
		{ Type::GHOST, Type::POISON },			// Gastly
		{ Type::WATER, Type::GROUND },			// Gastrodon
		{ Type::GHOST, Type::POISON },			// Gengar
		{ Type::ROCK, Type::GROUND },			// Geodude
		{ Type::DRAGON, Type::GROUND },			// Gible
		{ Type::NORMAL, Type::PSYCHIC },		// Girafarig
		{ Type::GHOST, Type::DRAGON },			// Giratina-A
		{ Type::GHOST, Type::DRAGON },			// Giratina-O
		{ Type::ICE, Type::TYPELESS },			// Glaceon
		{ Type::ICE, Type::TYPELESS },			// Glalie
		{ Type::NORMAL, Type::TYPELESS },		// Glameow
		{ Type::GROUND, Type::FLYING },			// Gligar
		{ Type::GROUND, Type::FLYING },			// Gliscor
		{ Type::GRASS, Type::POISON },			// Gloom
		{ Type::POISON, Type::FLYING },			// Golbat
		{ Type::WATER, Type::TYPELESS },		// Goldeen
		{ Type::WATER, Type::TYPELESS },		// Golduck
		{ Type::ROCK, Type::GROUND },			// Golem
		{ Type::WATER, Type::TYPELESS },		// Gorebyss
		{ Type::NORMAL, Type::TYPELESS },		// Granbull
		{ Type::ROCK, Type::GROUND },			// Graveler
		{ Type::POISON, Type::TYPELESS },		// Grimer
		{ Type::GRASS, Type::TYPELESS },		// Grotle
		{ Type::GROUND, Type::TYPELESS },		// Groudon
		{ Type::GRASS, Type::TYPELESS },		// Grovyle
		{ Type::FIRE, Type::TYPELESS },			// Growlithe
		{ Type::PSYCHIC, Type::TYPELESS },		// Grumpig
		{ Type::POISON, Type::TYPELESS },		// Gulpin
		{ Type::WATER, Type::FLYING },			// Gyarados
		{ Type::NORMAL, Type::TYPELESS },		// Happiny
		{ Type::FIGHTING, Type::TYPELESS },		// Hariyama
		{ Type::GHOST, Type::POISON },			// Haunter
		{ Type::FIRE, Type::STEEL },			// Heatran
		{ Type::BUG, Type::FIGHTING },			// Heracross
		{ Type::GROUND, Type::TYPELESS },		// Hippopotas
		{ Type::GROUND, Type::TYPELESS },		// Hippowdon
		{ Type::FIGHTING, Type::TYPELESS },		// Hitmonchan
		{ Type::FIGHTING, Type::TYPELESS },		// Hitmonlee
		{ Type::FIGHTING, Type::TYPELESS },		// Hitmontop
		{ Type::FIRE, Type::FLYING },			// Ho-Oh
		{ Type::DARK, Type::FLYING },			// Honchkrow
		{ Type::NORMAL, Type::FLYING },			// Hoothoot
		{ Type::GRASS, Type::FLYING },			// Hoppip
		{ Type::WATER, Type::TYPELESS },		// Horsea
		{ Type::DARK, Type::FIRE },				// Houndoom
		{ Type::DARK, Type::FIRE },				// Houndour
		{ Type::WATER, Type::TYPELESS },		// Huntail
		{ Type::PSYCHIC, Type::TYPELESS },		// Hypno
		{ Type::NORMAL, Type::TYPELESS },		// Igglybuff
		{ Type::BUG, Type::TYPELESS },			// Illumise
		{ Type::FIRE, Type::FIGHTING },			// Infernape
		{ Type::GRASS, Type::POISON },			// Ivysaur
		{ Type::NORMAL, Type::TYPELESS },		// Jigglypuff
		{ Type::STEEL, Type::PSYCHIC },			// Jirachi
		{ Type::ELECTRIC, Type::TYPELESS },		// Jolteon
		{ Type::GRASS, Type::FLYING },			// Jumpluff
		{ Type::ICE, Type::PSYCHIC },			// Jynx
		{ Type::ROCK, Type::WATER },			// Kabuto
		{ Type::ROCK, Type::WATER },			// Kabutops
		{ Type::PSYCHIC, Type::TYPELESS },		// Kadabra
		{ Type::BUG, Type::POISON },			// Kakuna
		{ Type::NORMAL, Type::TYPELESS },		// Kangaskhan
		{ Type::NORMAL, Type::TYPELESS },		// Kecleon
		{ Type::WATER, Type::DRAGON },			// Kingdra
		{ Type::WATER, Type::TYPELESS },		// Kingler
		{ Type::PSYCHIC, Type::TYPELESS },		// Kirlia
		{ Type::POISON, Type::TYPELESS },		// Koffing
		{ Type::WATER, Type::TYPELESS },		// Krabby
		{ Type::BUG, Type::TYPELESS },			// Kricketot
		{ Type::BUG, Type::TYPELESS },			// Kricketune
		{ Type::WATER, Type::TYPELESS },		// Kyogre
		{ Type::STEEL, Type::ROCK },			// Lairon
		{ Type::WATER, Type::ELECTRIC },		// Lanturn
		{ Type::WATER, Type::ICE },				// Lapras
		{ Type::ROCK, Type::GROUND },			// Larvitar
		{ Type::DRAGON, Type::PSYCHIC },		// Latias
		{ Type::DRAGON, Type::PSYCHIC },		// Latios
		{ Type::GRASS, Type::TYPELESS },		// Leafeon
		{ Type::BUG, Type::FLYING },			// Ledian
		{ Type::BUG, Type::FLYING },			// Ledyba
		{ Type::NORMAL, Type::TYPELESS },		// Lickilicky
		{ Type::NORMAL, Type::TYPELESS },		// Lickitung
		{ Type::ROCK, Type::GRASS },			// Lileep
		{ Type::NORMAL, Type::TYPELESS },		// Linoone
		{ Type::WATER, Type::GRASS },			// Lombre
		{ Type::NORMAL, Type::TYPELESS },		// Lopunny
		{ Type::WATER, Type::GRASS },			// Lotad
		{ Type::NORMAL, Type::TYPELESS },		// Loudred
		{ Type::FIGHTING, Type::STEEL },		// Lucario
		{ Type::WATER, Type::GRASS },			// Ludicolo
		{ Type::PSYCHIC, Type::FLYING },		// Lugia
		{ Type::WATER, Type::TYPELESS },		// Lumineon
		{ Type::ROCK, Type::PSYCHIC },			// Lunatone
		{ Type::WATER, Type::TYPELESS },		// Luvdisc
		{ Type::ELECTRIC, Type::TYPELESS },		// Luxio
		{ Type::ELECTRIC, Type::TYPELESS },		// Luxray
		{ Type::FIGHTING, Type::TYPELESS },		// Machamp
		{ Type::FIGHTING, Type::TYPELESS },		// Machoke
		{ Type::FIGHTING, Type::TYPELESS },		// Machop
		{ Type::FIRE, Type::TYPELESS },			// Magby
		{ Type::FIRE, Type::ROCK },				// Magcargo
		{ Type::WATER, Type::TYPELESS },		// Magikarp
		{ Type::FIRE, Type::TYPELESS },			// Magmar
		{ Type::FIRE, Type::TYPELESS },			// Magmortar
		{ Type::ELECTRIC, Type::STEEL },		// Magnemite
		{ Type::ELECTRIC, Type::STEEL },		// Magneton
		{ Type::ELECTRIC, Type::STEEL },		// Magnezone
		{ Type::FIGHTING, Type::TYPELESS },		// Makuhita
		{ Type::ICE, Type::GROUND },			// Mamoswine
		{ Type::WATER, Type::TYPELESS },		// Manaphy
		{ Type::ELECTRIC, Type::TYPELESS },		// Manectric
		{ Type::FIGHTING, Type::TYPELESS },		// Mankey
		{ Type::WATER, Type::FLYING },			// Mantine
		{ Type::WATER, Type::FLYING },			// Mantyke
		{ Type::ELECTRIC, Type::TYPELESS },		// Mareep
		{ Type::WATER, Type::TYPELESS },		// Marill
		{ Type::GROUND, Type::TYPELESS },		// Marowak
		{ Type::WATER, Type::GROUND },			// Marshtomp
		{ Type::BUG, Type::FLYING },			// Masquerain
		{ Type::STEEL, Type::TYPELESS },		// Mawile
		{ Type::FIGHTING, Type::PSYCHIC },		// Medicham
		{ Type::FIGHTING, Type::PSYCHIC },		// Meditite
		{ Type::GRASS, Type::TYPELESS },		// Meganium
		{ Type::NORMAL, Type::TYPELESS },		// Meowth
		{ Type::PSYCHIC, Type::TYPELESS },		// Mesprit
		{ Type::STEEL, Type::PSYCHIC },			// Metagross
		{ Type::STEEL, Type::PSYCHIC },			// Metang
		{ Type::BUG, Type::TYPELESS },			// Metapod
		{ Type::PSYCHIC, Type::TYPELESS },		// Mew
		{ Type::PSYCHIC, Type::TYPELESS },		// Mewtwo
		{ Type::DARK, Type::TYPELESS },			// Mightyena
		{ Type::WATER, Type::TYPELESS },		// Milotic
		{ Type::NORMAL, Type::TYPELESS },		// Miltank
		{ Type::PSYCHIC, Type::TYPELESS },		// Mime Jr.
		{ Type::ELECTRIC, Type::TYPELESS },		// Minun
		{ Type::GHOST, Type::TYPELESS },		// Misdreavus
		{ Type::GHOST, Type::TYPELESS },		// Mismagius
		{ Type::FIRE, Type::FLYING },			// Moltres
		{ Type::FIRE, Type::FIGHTING },			// Monferno
		{ Type::BUG, Type::FLYING },			// Mothim
		{ Type::PSYCHIC, Type::TYPELESS },		// Mr. Mime
		{ Type::WATER, Type::TYPELESS },		// Mudkip
		{ Type::POISON, Type::TYPELESS },		// Muk
		{ Type::NORMAL, Type::TYPELESS },		// Munchlax
		{ Type::DARK, Type::FLYING },			// Murkrow
		{ Type::PSYCHIC, Type::FLYING },		// Natu
		{ Type::POISON, Type::GROUND },			// Nidoking
		{ Type::POISON, Type::GROUND },			// Nidoqueen
		{ Type::POISON, Type::TYPELESS },		// Nidoran-F
		{ Type::POISON, Type::TYPELESS },		// Nidoran-M
		{ Type::POISON, Type::TYPELESS },		// Nidorina
		{ Type::POISON, Type::TYPELESS },		// Nidorino
		{ Type::BUG, Type::GROUND },			// Nincada
		{ Type::FIRE, Type::TYPELESS },			// Ninetales
		{ Type::BUG, Type::FLYING },			// Ninjask
		{ Type::NORMAL, Type::FLYING },			// Noctowl
		{ Type::ROCK, Type::TYPELESS },			// Nosepass
		{ Type::FIRE, Type::GROUND },			// Numel
		{ Type::GRASS, Type::DARK },			// Nuzleaf
		{ Type::WATER, Type::TYPELESS },		// Octillery
		{ Type::GRASS, Type::POISON },			// Oddish
		{ Type::ROCK, Type::WATER },			// Omanyte
		{ Type::ROCK, Type::WATER },			// Omastar
		{ Type::ROCK, Type::GROUND },			// Onix
		{ Type::ELECTRIC, Type::TYPELESS },		// Pachirisu
		{ Type::WATER, Type::DRAGON },			// Palkia
		{ Type::BUG, Type::GRASS },				// Paras
		{ Type::BUG, Type::GRASS },				// Parasect
		{ Type::WATER, Type::FLYING },			// Pelipper
		{ Type::NORMAL, Type::TYPELESS },		// Persian
		{ Type::GROUND, Type::TYPELESS },		// Phanpy
		{ Type::WATER, Type::TYPELESS },		// Phione
		{ Type::ELECTRIC, Type::TYPELESS },		// Pichu
		{ Type::NORMAL, Type::FLYING },			// Pidgeot
		{ Type::NORMAL, Type::FLYING },			// Pidgeotto
		{ Type::NORMAL, Type::FLYING },			// Pidgey
		{ Type::ELECTRIC, Type::TYPELESS },		// Pikachu
		{ Type::ICE, Type::GROUND },			// Piloswine
		{ Type::BUG, Type::TYPELESS },			// Pineco
		{ Type::BUG, Type::TYPELESS },			// Pinsir
		{ Type::WATER, Type::TYPELESS },		// Piplup
		{ Type::ELECTRIC, Type::TYPELESS },		// Plusle
		{ Type::WATER, Type::TYPELESS },		// Politoed
		{ Type::WATER, Type::TYPELESS },		// Poliwag
		{ Type::WATER, Type::TYPELESS },		// Poliwhirl
		{ Type::WATER, Type::FIGHTING },		// Poliwrath
		{ Type::FIRE, Type::TYPELESS },			// Ponyta
		{ Type::DARK, Type::TYPELESS },			// Poochyena
		{ Type::NORMAL, Type::TYPELESS },		// Porygon
		{ Type::NORMAL, Type::TYPELESS },		// Porygon-Z
		{ Type::NORMAL, Type::TYPELESS },		// Porygon2
		{ Type::FIGHTING, Type::TYPELESS },		// Primeape
		{ Type::WATER, Type::TYPELESS },		// Prinplup
		{ Type::ROCK, Type::STEEL },			// Probopass
		{ Type::WATER, Type::TYPELESS },		// Psyduck
		{ Type::ROCK, Type::GROUND },			// Pupitar
		{ Type::NORMAL, Type::TYPELESS },		// Purugly
		{ Type::WATER, Type::GROUND },			// Quagsire
		{ Type::FIRE, Type::TYPELESS },			// Quilava
		{ Type::WATER, Type::POISON },			// Qwilfish
		{ Type::ELECTRIC, Type::TYPELESS },		// Raichu
		{ Type::ELECTRIC, Type::TYPELESS },		// Raikou
		{ Type::PSYCHIC, Type::TYPELESS },		// Ralts
		{ Type::ROCK, Type::TYPELESS },			// Rampardos
		{ Type::FIRE, Type::TYPELESS },			// Rapidash
		{ Type::NORMAL, Type::TYPELESS },		// Raticate
		{ Type::NORMAL, Type::TYPELESS },		// Rattata
		{ Type::DRAGON, Type::FLYING },			// Rayquaza
		{ Type::ICE, Type::TYPELESS },			// Regice
		{ Type::NORMAL, Type::TYPELESS },		// Regigigas
		{ Type::ROCK, Type::TYPELESS },			// Regirock
		{ Type::STEEL, Type::TYPELESS },		// Registeel
		{ Type::WATER, Type::ROCK },			// Relicanth
		{ Type::WATER, Type::TYPELESS },		// Remoraid
		{ Type::GROUND, Type::ROCK },			// Rhydon
		{ Type::GROUND, Type::ROCK },			// Rhyhorn
		{ Type::GROUND, Type::ROCK },			// Rhyperior
		{ Type::FIGHTING, Type::TYPELESS },		// Riolu
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
		{ Type::GROUND, Type::TYPELESS },		// Sandshrew
		{ Type::GROUND, Type::TYPELESS },		// Sandslash
		{ Type::GRASS, Type::TYPELESS },		// Sceptile
		{ Type::BUG, Type::STEEL },				// Scizor
		{ Type::BUG, Type::FLYING },			// Scyther
		{ Type::WATER, Type::TYPELESS },		// Seadra
		{ Type::WATER, Type::TYPELESS },		// Seaking
		{ Type::ICE, Type::WATER },				// Sealeo
		{ Type::GRASS, Type::TYPELESS },		// Seedot
		{ Type::WATER, Type::TYPELESS },		// Seel
		{ Type::NORMAL, Type::TYPELESS },		// Sentret
		{ Type::POISON, Type::TYPELESS },		// Seviper
		{ Type::WATER, Type::DARK },			// Sharpedo
		{ Type::GRASS, Type::TYPELESS },		// Shaymin-L
		{ Type::GRASS, Type::FLYING },			// Shaymin-S
		{ Type::BUG, Type::GHOST },				// Shedinja
		{ Type::DRAGON, Type::TYPELESS },		// Shelgon
		{ Type::WATER, Type::TYPELESS },		// Shellder
		{ Type::WATER, Type::TYPELESS },		// Shellos
		{ Type::ROCK, Type::STEEL },			// Shieldon
		{ Type::GRASS, Type::DARK },			// Shiftry
		{ Type::ELECTRIC, Type::TYPELESS },		// Shinx
		{ Type::GRASS, Type::TYPELESS },		// Shroomish
		{ Type::BUG, Type::ROCK },				// Shuckle
		{ Type::GHOST, Type::TYPELESS },		// Shuppet
		{ Type::BUG, Type::TYPELESS },			// Silcoon
		{ Type::STEEL, Type::FLYING },			// Skarmory
		{ Type::GRASS, Type::FLYING },			// Skiploom
		{ Type::NORMAL, Type::TYPELESS },		// Skitty
		{ Type::POISON, Type::BUG },			// Skorupi
		{ Type::POISON, Type::DARK },			// Skuntank
		{ Type::NORMAL, Type::TYPELESS },		// Slaking
		{ Type::NORMAL, Type::TYPELESS },		// Slakoth
		{ Type::WATER, Type::PSYCHIC },			// Slowbro
		{ Type::WATER, Type::PSYCHIC },			// Slowking
		{ Type::WATER, Type::PSYCHIC },			// Slowpoke
		{ Type::FIRE, Type::TYPELESS },			// Slugma
		{ Type::NORMAL, Type::TYPELESS },		// Smeargle
		{ Type::ICE, Type::PSYCHIC },			// Smoochum
		{ Type::DARK, Type::ICE },				// Sneasel
		{ Type::NORMAL, Type::TYPELESS },		// Snorlax
		{ Type::ICE, Type::TYPELESS },			// Snorunt
		{ Type::GRASS, Type::ICE },				// Snover
		{ Type::NORMAL, Type::TYPELESS },		// Snubbull
		{ Type::ROCK, Type::PSYCHIC },			// Solrock
		{ Type::NORMAL, Type::FLYING },			// Spearow
		{ Type::ICE, Type::WATER },				// Spheal
		{ Type::BUG, Type::POISON },			// Spinarak
		{ Type::NORMAL, Type::TYPELESS },		// Spinda
		{ Type::GHOST, Type::DARK },			// Spiritomb
		{ Type::PSYCHIC, Type::TYPELESS },		// Spoink
		{ Type::WATER, Type::TYPELESS },		// Squirtle
		{ Type::NORMAL, Type::TYPELESS },		// Stantler
		{ Type::NORMAL, Type::FLYING },			// Staraptor
		{ Type::NORMAL, Type::FLYING },			// Staravia
		{ Type::NORMAL, Type::FLYING },			// Starly
		{ Type::WATER, Type::PSYCHIC },			// Starmie
		{ Type::WATER, Type::TYPELESS },		// Staryu
		{ Type::STEEL, Type::GROUND },			// Steelix
		{ Type::POISON, Type::DARK },			// Stunky
		{ Type::ROCK, Type::TYPELESS },			// Sudowoodo
		{ Type::WATER, Type::TYPELESS },		// Suicune
		{ Type::GRASS, Type::TYPELESS },		// Sunflora
		{ Type::GRASS, Type::TYPELESS },		// Sunkern
		{ Type::BUG, Type::WATER },				// Surskit
		{ Type::NORMAL, Type::FLYING },			// Swablu
		{ Type::POISON, Type::TYPELESS },		// Swalot
		{ Type::WATER, Type::GROUND },			// Swampert
		{ Type::NORMAL, Type::FLYING },			// Swellow
		{ Type::ICE, Type::GROUND },			// Swinub
		{ Type::NORMAL, Type::FLYING },			// Taillow
		{ Type::GRASS, Type::TYPELESS },		// Tangela
		{ Type::GRASS, Type::TYPELESS },		// Tangrowth
		{ Type::NORMAL, Type::TYPELESS },		// Tauros
		{ Type::NORMAL, Type::TYPELESS },		// Teddiursa
		{ Type::WATER, Type::POISON },			// Tentacool
		{ Type::WATER, Type::POISON },			// Tentacruel
		{ Type::NORMAL, Type::FLYING },			// Togekiss
		{ Type::NORMAL, Type::TYPELESS },		// Togepi
		{ Type::NORMAL, Type::FLYING },			// Togetic
		{ Type::FIRE, Type::TYPELESS },			// Torchic
		{ Type::FIRE, Type::TYPELESS },			// Torkoal
		{ Type::GRASS, Type::GROUND },			// Torterra
		{ Type::WATER, Type::TYPELESS },		// Totodile
		{ Type::POISON, Type::FIGHTING },		// Toxicroak
		{ Type::GROUND, Type::TYPELESS },		// Trapinch
		{ Type::GRASS, Type::TYPELESS },		// Treecko
		{ Type::GRASS, Type::FLYING },			// Tropius
		{ Type::GRASS, Type::TYPELESS },		// Turtwig
		{ Type::FIRE, Type::TYPELESS },			// Typhlosion
		{ Type::ROCK, Type::DARK },				// Tyranitar
		{ Type::FIGHTING, Type::TYPELESS },		// Tyrogue
		{ Type::DARK, Type::TYPELESS },			// Umbreon
		{ Type::PSYCHIC, Type::TYPELESS },		// Unown
		{ Type::NORMAL, Type::TYPELESS },		// Ursaring
		{ Type::PSYCHIC, Type::TYPELESS },		// Uxie
		{ Type::WATER, Type::TYPELESS },		// Vaporeon
		{ Type::BUG, Type::POISON },			// Venomoth
		{ Type::BUG, Type::POISON },			// Venonat
		{ Type::GRASS, Type::POISON },			// Venusaur
		{ Type::BUG, Type::FLYING },			// Vespiquen
		{ Type::GROUND, Type::DRAGON },			// Vibrava
		{ Type::GRASS, Type::POISON },			// Victreebel
		{ Type::NORMAL, Type::TYPELESS },		// Vigoroth
		{ Type::GRASS, Type::POISON },			// Vileplume
		{ Type::BUG, Type::TYPELESS },			// Volbeat
		{ Type::ELECTRIC, Type::TYPELESS },		// Voltorb
		{ Type::FIRE, Type::TYPELESS },			// Vulpix
		{ Type::WATER, Type::TYPELESS },		// Wailmer
		{ Type::WATER, Type::TYPELESS },		// Wailord
		{ Type::ICE, Type::WATER },				// Walrein
		{ Type::WATER, Type::TYPELESS },		// Wartortle
		{ Type::DARK, Type::ICE },				// Weavile
		{ Type::BUG, Type::POISON },			// Weedle
		{ Type::GRASS, Type::POISON },			// Weepinbell
		{ Type::POISON, Type::TYPELESS },		// Weezing
		{ Type::WATER, Type::GROUND },			// Whiscash
		{ Type::NORMAL, Type::TYPELESS },		// Whismur
		{ Type::NORMAL, Type::TYPELESS },		// Wigglytuff
		{ Type::WATER, Type::FLYING },			// Wingull
		{ Type::PSYCHIC, Type::TYPELESS },		// Wobbuffet
		{ Type::WATER, Type::GROUND },			// Wooper
		{ Type::BUG, Type::GRASS },				// Wormadam-P
		{ Type::BUG, Type::GROUND },			// Wormadam-S
		{ Type::BUG, Type::STEEL },				// Wormadam-T
		{ Type::BUG, Type::TYPELESS },			// Wurmple
		{ Type::PSYCHIC, Type::TYPELESS },		// Wynaut
		{ Type::PSYCHIC, Type::FLYING },		// Xatu
		{ Type::BUG, Type::FLYING },			// Yanma
		{ Type::BUG, Type::FLYING },			// Yanmega
		{ Type::NORMAL, Type::TYPELESS },		// Zangoose
		{ Type::ELECTRIC, Type::FLYING },		// Zapdos
		{ Type::NORMAL, Type::TYPELESS },		// Zigzagoon
		{ Type::POISON, Type::FLYING }			// Zubat
	};

	for (unsigned n = 0; n != 2; ++n) {
		Type t = get_type [name] [n];
		if (t != Type::TYPELESS)
			type.types.push_back (t);
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

std::string Pokemon::to_string (Species name) {
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

std::string Pokemon::to_string() const {
	return to_string (name);
}

Species Pokemon::from_string (std::string const & str) {
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
	name = from_string (str);
}
}
