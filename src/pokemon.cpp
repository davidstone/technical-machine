// Pokemon functions
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

#include "pokemon.hpp"

#include <cstdint>
#include <string>

#include "ability.hpp"
#include "active.hpp"
#include "gender.hpp"
#include "move.hpp"
#include "species.hpp"
#include "stat.hpp"

namespace technicalmachine {

Pokemon::Pokemon (Species member, unsigned size) : 
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
	sleep (0),

	happiness (255)
	{
	hp.calculate_initial_hp (level);
	set_type ();
	Move struggle (Move::STRUGGLE, 0, 0);
	move.set.push_back (struggle);
	// A Pokemon has a new "Switch" move for each Pokemon in the party.
	if (size > 1) {
		for (size_t index = 0; index != size; ++index) {
			Move switchn (Move::from_replacement (index), 0, 0);
			move.set.push_back (switchn);
		}
	}
}

uint64_t Pokemon::hash () const {
	uint64_t current_hash = 0;
	// Should probably think of a better way to combine Move hashes than xor
	for (Move const & next_move : move.set)
		current_hash ^= next_move.hash();
	// current_hash is in the innermost nested parentheses, so all of the arguments
	// are promoted to uint64_t
	return name + END *
			(item.name + Item::END *
			(status.name + Status::END *
			((hp.stat - 1u) + hp.max *	// - 1 because you can't have 0 HP
			(sleep + (Stat::max_sleep_turns + 1) *
			current_hash))));
}

bool Pokemon::find_move (Move::Moves move_name) {
	for (move.index = 0; move().name != Move::STRUGGLE; ++move.index) {
		if (move_name == move().name)
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

bool Pokemon::is_alternate_form (Species first, Species second) {
	return (Pokemon::is_deoxys (first) and Pokemon::is_deoxys (second)) or
			(Pokemon::is_giratina (first) and Pokemon::is_giratina (second)) or
			(Pokemon::is_rotom (first) and Pokemon::is_rotom (second)) or
			(Pokemon::is_shaymin (first) and Pokemon::is_shaymin (second)) or
			(Pokemon::is_wormadam (first) and Pokemon::is_wormadam (second));
}

bool Pokemon::is_deoxys (Species species) {
	switch (species) {
		case DEOXYS_A:
		case DEOXYS_D:
		case DEOXYS_M:
		case DEOXYS_S:
			return true;
		default:
			return false;
	}
}

bool Pokemon::is_giratina (Species species) {
	switch (species) {
		case GIRATINA_A:
		case GIRATINA_O:
			return true;
		default:
			return false;
	}
}

bool Pokemon::is_rotom (Species species) {
	switch (species) {
		case ROTOM:
		case ROTOM_C:
		case ROTOM_F:
		case ROTOM_H:
		case ROTOM_S:
		case ROTOM_W:
			return true;
		default:
			return false;
	}
}

bool Pokemon::is_shaymin (Species species) {
	switch (species) {
		case SHAYMIN_L:
		case SHAYMIN_S:
			return true;
		default:
			return false;
	}
}

bool Pokemon::is_wormadam (Species species) {
	switch (species) {
		case WORMADAM_P:
		case WORMADAM_S:
		case WORMADAM_T:
			return true;
		default:
			return false;
	}
}

bool Pokemon::operator== (Pokemon const & other) const {
	if (move.set.size() != other.move.set.size())
		return false;
	for (size_t n = 0; n != move.set.size(); ++n) {
		if (move.set [n] == other.move.set [n])
			return false;
	}
	return name == other.name and
			status.name == other.status.name and
			sleep == other.sleep and
			hp.stat == other.hp.stat and
			item == other.item;
}

bool Pokemon::operator!= (Pokemon const & other) const {
	return !(*this == other);
}

std::string Pokemon::get_nickname () const {
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		return nickname;
	#else
		return to_string();
	#endif
}

void Pokemon::set_nickname (std::string const & nick) {
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		nickname = nick;
	#endif
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

uint8_t Pokemon::mass () const {
	constexpr static uint8_t mass_array [] = {
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
	return mass_array [name];
}

}	// namespace technicalmachine
