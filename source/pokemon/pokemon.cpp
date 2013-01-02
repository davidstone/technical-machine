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

#include <cassert>
#include <cstdint>
#include <string>

#include "species.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../rational.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

#include "../stat/stat.hpp"

#include "../string_conversions/conversion.hpp"

namespace technicalmachine {
namespace {
bool is_deoxys (Species species);
bool is_giratina (Species species);
bool is_rotom (Species species);
bool is_shaymin (Species species);
bool is_wormadam (Species species);
}	// unnamed namespace

Pokemon::Pokemon (unsigned const my_team_size, Species const species, uint8_t set_level, Gender set_gender, std::string const & set_nickname, uint8_t set_happiness) : 
	move(my_team_size),
	current_type(species),
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	nickname(set_nickname);
	#endif
	stats(species),

	m_name(species),
	m_gender(set_gender),

	m_will_be_replaced(false),
	m_level(set_level),

	m_happiness(set_happiness)
	{
	calculate_initial_hp();
}

Pokemon::Pokemon(unsigned const my_team_size, Species const species, uint8_t const set_level, Gender const set_gender, Item const & set_item, Ability const & set_ability, Nature const & set_nature, std::string const & set_nickname, uint8_t set_happiness):
	Pokemon::Pokemon(my_team_size, species, set_level, set_gender, set_nickname, set_happiness)
	{
	m_item = set_item;
	m_ability = set_ability;
	m_nature = set_nature;
}

void Pokemon::switch_in() {
	seen.make_visible();
}

void Pokemon::switch_out() {
	// Cure the status of a Natural Cure Pokemon
	if (ability().clears_status_on_switch())
		status().clear();
}

void Pokemon::calculate_initial_hp () {
	stat(Stat::HP).calculate_initial_hp(level());
}

void Pokemon::remove_switch() {
	move.remove_switch();
}

uint8_t Pokemon::index_of_first_switch () const {
	uint8_t index = 0;
	while (!move(index).is_switch())
		++index;
	return index;
}

void Pokemon::correct_error_in_hp(unsigned const correct_hp_stat) {
	stat(Stat::HP).stat = correct_hp_stat;
}

Rational Pokemon::current_hp() const {
	return Rational(stat(Stat::HP).stat, stat(Stat::HP).max);
}

unsigned Pokemon::apply_damage(unsigned damage) {
	auto & hp = stat(Stat::HP);
	damage = std::min(damage, static_cast<unsigned>(hp.stat));
	hp.stat -= damage;
	return damage;
}

void Pokemon::apply_healing(unsigned const amount) {
	// Should be no risk of overflow. hp.stat has to be at least 16 bits, and no
	// healing will be anywhere close to that number.
	auto & hp = stat(Stat::HP);
	assert(hp.stat + amount >= amount);
	hp.stat += amount;
	hp.stat = std::min(hp.stat, hp.max);
}

bool Pokemon::can_confuse_with_chatter() const {
	return name() == Species::Chatot;
}

bool Pokemon::can_use_substitute() const {
	return stat(Stat::HP).stat > stat(Stat::HP).max / 4;
}

bool is_alternate_form(Species first, Species second) {
	return (is_deoxys (first) and is_deoxys (second)) or
			(is_giratina (first) and is_giratina (second)) or
			(is_rotom (first) and is_rotom (second)) or
			(is_shaymin (first) and is_shaymin (second)) or
			(is_wormadam (first) and is_wormadam (second));
}

namespace {

bool is_deoxys (Species species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Deoxys_Defense:
		case Species::Deoxys_Mediocre:
		case Species::Deoxys_Speed:
			return true;
		default:
			return false;
	}
}

bool is_giratina (Species species) {
	switch (species) {
		case Species::Giratina_Altered:
		case Species::Giratina_Origin:
			return true;
		default:
			return false;
	}
}

bool is_rotom (Species species) {
	switch (species) {
		case Species::Rotom:
		case Species::Rotom_Mow:
		case Species::Rotom_Frost:
		case Species::Rotom_Heat:
		case Species::Rotom_Fan:
		case Species::Rotom_Wash:
			return true;
		default:
			return false;
	}
}

bool is_shaymin (Species species) {
	switch (species) {
		case Species::Shaymin_Land:
		case Species::Shaymin_Sky:
			return true;
		default:
			return false;
	}
}

bool is_wormadam (Species species) {
	switch (species) {
		case Species::Wormadam_Plant:
		case Species::Wormadam_Sandy:
		case Species::Wormadam_Trash:
			return true;
		default:
			return false;
	}
}

}	// unnamed namespace

bool Pokemon::is_boosted_by_adamant_orb() const {
	return name() == Species::Dialga;
}

bool Pokemon::is_boosted_by_deepseascale() const {
	return name() == Species::Clamperl;
}

bool Pokemon::is_boosted_by_deepseatooth() const {
	return name() == Species::Clamperl;
}

bool Pokemon::is_boosted_by_griseous_orb() const {
	return name() == Species::Palkia;
}

bool Pokemon::is_boosted_by_light_ball() const {
	return name() == Species::Pikachu;
}

bool Pokemon::is_boosted_by_lustrous_orb() const {
	return name() == Species::Giratina_Origin;
}

bool Pokemon::is_boosted_by_metal_powder() const {
	return name() == Species::Ditto;
}

bool Pokemon::is_boosted_by_quick_powder() const {
	return name() == Species::Ditto;
}

bool Pokemon::is_boosted_by_soul_dew() const {
	return name() == Species::Latias or name() == Species::Latios;
}

bool Pokemon::is_boosted_by_thick_club() const {
	return name() == Species::Cubone or name() == Species::Marowak;
}

std::string Pokemon::get_nickname () const {
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		return nickname;
	#else
		return to_string();
	#endif
}

void Pokemon::set_hidden_power_type() {
	Move * const move_ptr = move.find(Moves::Hidden_Power);
	if (move_ptr != nullptr)
		move_ptr->set_type(calculate_hidden_power_type());
}

Species Pokemon::name() const {
	return m_name;
}

Ability const & Pokemon::ability() const {
	return m_ability;
}
Ability & Pokemon::ability() {
	return m_ability;
}

Gender const & Pokemon::gender() const {
	return m_gender;
}
Gender & Pokemon::gender() {
	return m_gender;
}

Item const & Pokemon::item() const {
	return m_item;
}
Item & Pokemon::item() {
	return m_item;
}

Nature const & Pokemon::nature() const {
	return m_nature;
}
Nature & Pokemon::nature() {
	return m_nature;
}

Stat const & Pokemon::stat(Stat::Stats const index_stat) const {
	return stats[index_stat];
}
Stat & Pokemon::stat(Stat::Stats const index_stat) {
	return stats[index_stat];
}

Status const & Pokemon::status() const {
	return m_status;
}
Status & Pokemon::status() {
	return m_status;
}

TypeCollection const & Pokemon::type() const {
	return current_type;
}

void Pokemon::change_type(Type::Types const new_type) {
	current_type.change_type(new_type);
}

unsigned Pokemon::level() const {
	return m_level();
}

unsigned Pokemon::happiness() const {
	return m_happiness;
}

bool Pokemon::is_fainted() const {
	return m_will_be_replaced;
}
bool Pokemon::will_be_replaced() const {
	return m_will_be_replaced;
}
void Pokemon::faint() {
	stat(Stat::HP).stat = 0;
	m_will_be_replaced = true;
}
void Pokemon::reset_replacing() {
	m_will_be_replaced = false;
}

Pokemon::hash_type Pokemon::hash() const {
	return static_cast<hash_type>(m_name) + number_of_species *
			(m_item.name + Item::END *
			(m_status.hash() + Status::max_hash() *
			((stat(Stat::HP).stat - 1u) + stat(Stat::HP).max *	// - 1 because you can't have 0 HP
			(seen.hash() + seen.max_hash() *
			move.hash()))));
}

Pokemon::hash_type Pokemon::max_hash() const {
	return number_of_species * Item::END * Status::max_hash() * stat(Stat::HP).max * seen.max_hash() * move.max_hash();
}

bool operator== (Pokemon const & lhs, Pokemon const & rhs) {
	// Species clause is assumed, and Pokemon will only be compared for equality
	// on the same team, so the same species implies many other things are the
	// same
	assert(illegal_inequality_check(lhs, rhs));
	return lhs.move == rhs.move and
			lhs.m_name == rhs.m_name and
			lhs.m_status == rhs.m_status and
			lhs.stat(Stat::HP).stat == rhs.stat(Stat::HP).stat and
			lhs.m_item == rhs.m_item and
			lhs.seen == rhs.seen;
}

bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs) {
	if (lhs.name() != rhs.name())
		return true;
	return lhs.m_ability == rhs.m_ability and
			lhs.m_gender == rhs.m_gender and
			lhs.m_nature == rhs.m_nature and
			lhs.will_be_replaced() == rhs.will_be_replaced() and
			lhs.level() == rhs.level() and
			lhs.happiness() == rhs.happiness();
}

bool operator!= (Pokemon const & lhs, Pokemon const & rhs) {
	return !(lhs == rhs);
}

namespace {

constexpr unsigned lowest_bit(unsigned const iv) {
	return iv % 2;
}

}	// unnamed namespace

Type::Types Pokemon::calculate_hidden_power_type() const {
	static constexpr std::pair<Stat::Stats, unsigned> modifiers[] = {
		{ Stat::HP, 0 },
		{ Stat::ATK, 1 },
		{ Stat::DEF, 2 },
		{ Stat::SPE, 3 },
		{ Stat::SPA, 4 },
		{ Stat::SPD, 5 }
	};
	auto const sum = [&](unsigned value, std::pair<Stat::Stats, unsigned> const & pair) {
		return value + (lowest_bit(stat(pair.first).iv) << pair.second);
	};
	auto const index = std::accumulate(std::begin(modifiers), std::end(modifiers), 0u, sum) * 15 / 63;
	constexpr static Type::Types lookup [] = {
		Type::Fighting,
		Type::Flying,
		Type::Poison,
		Type::Ground,
		Type::Rock,
		Type::Bug,
		Type::Ghost,
		Type::Steel,
		Type::Fire,
		Type::Water,
		Type::Grass,
		Type::Electric,
		Type::Psychic,
		Type::Ice,
		Type::Dragon,
		Type::Dark
	};
	return lookup[index];
}

unsigned Pokemon::power_of_mass_based_moves() const {
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
	return mass_array[static_cast<unsigned>(name())];
}

std::string Pokemon::to_string() const {
	return ::technicalmachine::to_string(name());
}

}	// namespace technicalmachine
