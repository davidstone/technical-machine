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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>

#include "ability.hpp"
#include "gender.hpp"
#include "species.hpp"
#include "stat.hpp"

#include "move/move.hpp"
#include "move/moves.hpp"
#include "move/shared.hpp"

namespace technicalmachine {

Pokemon::Pokemon (Species const species, SharedMoves & shared, uint8_t set_happiness) : 
	move (shared),
	type (species),
	hp (species, Stat::HP),
	atk (species, Stat::ATK),
	def (species, Stat::DEF),
	spa (species, Stat::SPA),
	spd (species, Stat::SPD),
	spe (species, Stat::SPE),

	new_hp (48),

	name (species),

	fainted (false),
	hidden (true),
	level (100),

	m_happiness(set_happiness)
	{
	calculate_initial_hp();
}

void Pokemon::calculate_initial_hp () {
	hp.calculate_initial_hp (level);
}

uint64_t Pokemon::hash () const {
	uint64_t current_hash = 0;
	move.for_each_regular_move([& current_hash](Move const & next_move) {
		current_hash *= next_move.max_hash();
		current_hash += next_move.hash();
	});
	// current_hash is in the innermost nested parentheses, so all of the arguments
	// are promoted to uint64_t
	return name + Species::END *
			(item.name + Item::END *
			(status.hash() + Status::max_hash() *
			((hp.stat - 1u) + hp.max *	// - 1 because you can't have 0 HP
			current_hash)));
}

uint8_t Pokemon::index_of_first_switch () const {
	uint8_t index = 0;
	while (!move(index).is_switch())
		++index;
	return index;
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

bool operator== (Pokemon const & lhs, Pokemon const & rhs) {
	return lhs.move == rhs.move and
			lhs.name == rhs.name and
			lhs.status == rhs.status and
			lhs.hp.stat == rhs.hp.stat and
			lhs.item == rhs.item;
}

bool operator!= (Pokemon const & lhs, Pokemon const & rhs) {
	return !(lhs == rhs);
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
	#else
		static_cast<void>(nick);
	#endif
}

void Pokemon::set_hidden_power_type() {
	Move * const move_ptr = move.find(Moves::HIDDEN_POWER);
	if (move_ptr != nullptr)
		move_ptr->set_type(calculate_hidden_power_type());
}

unsigned Pokemon::happiness() const {
	return m_happiness;
}

namespace {

constexpr unsigned lowest_bit(unsigned const iv) {
	return iv % 2;
}

constexpr unsigned hidden_power_type_helper(unsigned const iv, unsigned const n) {
	return lowest_bit(iv) << n;
}

}	// unnamed namespace

Type::Types Pokemon::calculate_hidden_power_type() const {
	unsigned const a = hidden_power_type_helper(hp.iv, 0);
	unsigned const b = hidden_power_type_helper(atk.iv, 1);
	unsigned const c = hidden_power_type_helper(def.iv, 2);
	unsigned const d = hidden_power_type_helper(spe.iv, 3);
	unsigned const e = hidden_power_type_helper(spa.iv, 4);
	unsigned const f = hidden_power_type_helper(spd.iv, 5);
	unsigned const index = (a + b + c + d + e + f) * 15 / 63;
	constexpr static Type::Types lookup [] = {
		Type::FIGHTING,
		Type::FLYING,
		Type::POISON,
		Type::GROUND,
		Type::ROCK,
		Type::BUG,
		Type::GHOST,
		Type::STEEL,
		Type::FIRE,
		Type::WATER,
		Type::GRASS,
		Type::ELECTRIC,
		Type::PSYCHIC,
		Type::ICE,
		Type::DRAGON,
		Type::DARK
	};
	return lookup [index];
}

uint8_t Pokemon::power_of_mass_based_moves() const {
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
