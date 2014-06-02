// HP class
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

#include "hp.hpp"

#include <bounded_integer/array.hpp>

#include "../pokemon/level.hpp"

namespace technicalmachine {
namespace {

using base_type = bounded::integer<1, 255>;
auto get_base(Species species) -> base_type;

using hp_type = bounded::dynamic_max_integer<0, HP::max_value, bounded::clamp_policy>;
auto initial_hp(base_type const base, EV const ev, IV const iv, Level const level) -> hp_type {
	auto const value = BOUNDED_CONDITIONAL((base > 1_bi),
		(2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 10_bi + level(),
		1_bi
	);
	static_assert(std::numeric_limits<decltype(value)>::min() == std::numeric_limits<HP::max_type>::min(), "Incorrect HP min.");
	static_assert(std::numeric_limits<hp_type>::max() == std::numeric_limits<decltype(value)>::max(), "Incorrect HP max.");
	// Current HP starts out as max HP
	hp_type::overflow_policy_type max_hp(value);
	return hp_type(value, std::move(max_hp));
}

}	// namespace

HP::HP(Species const species, Level const level, EV const set_ev) :
	ev(set_ev),
	iv(31_bi),
	m_value(initial_hp(get_base(species), ev, iv, level))
	{
	static_assert(std::is_same<decltype(m_value), hp_type>::value, "Incorrect HP type.");
}

auto HP::current() const -> current_type {
	return m_value;
}

auto HP::max() const -> max_type {
	return max_type(m_value.overflow_policy().max(), bounded::non_check);
}


namespace {

auto get_base(Species const name) -> base_type {
	static constexpr auto base = bounded::make_array(
		// Generation 1		
		45_bi,		// Bulbasaur
		60_bi,		// Ivysaur
		80_bi,		// Venusaur
		39_bi,		// Charmander
		58_bi,		// Charmeleon
		78_bi,		// Charizard
		44_bi,		// Squirtle
		59_bi,		// Wartortle
		79_bi,		// Blastoise
		45_bi,		// Caterpie
		50_bi,		// Metapod
		60_bi,		// Butterfree
		40_bi,		// Weedle
		45_bi,		// Kakuna
		65_bi,		// Beedrill
		40_bi,		// Pidgey
		63_bi,		// Pidgeotto
		83_bi,		// Pidgeot
		30_bi,		// Rattata
		55_bi,		// Raticate
		40_bi,		// Spearow
		65_bi,		// Fearow
		35_bi,		// Ekans
		60_bi,		// Arbok
		35_bi,		// Pikachu
		60_bi,		// Raichu
		50_bi,		// Sandshrew
		75_bi,		// Sandslash
		55_bi,		// Nidoran-F
		70_bi,		// Nidorina
		90_bi,		// Nidoqueen
		46_bi,		// Nidoran-M
		61_bi,		// Nidorino
		81_bi,		// Nidoking
		70_bi,		// Clefairy
		95_bi,		// Clefable
		38_bi,		// Vulpix
		73_bi,		// Ninetales
		115_bi,		// Jigglypuff
		140_bi,		// Wigglytuff
		40_bi,		// Zubat
		75_bi,		// Golbat
		45_bi,		// Oddish
		60_bi,		// Gloom
		75_bi,		// Vileplume
		35_bi,		// Paras
		60_bi,		// Parasect
		60_bi,		// Venonat
		70_bi,		// Venomoth
		10_bi,		// Diglett
		35_bi,		// Dugtrio
		40_bi,		// Meowth
		65_bi,		// Persian
		50_bi,		// Psyduck
		80_bi,		// Golduck
		40_bi,		// Mankey
		65_bi,		// Primeape
		55_bi,		// Growlithe
		90_bi,		// Arcanine
		40_bi,		// Poliwag
		65_bi,		// Poliwhirl
		90_bi,		// Poliwrath
		25_bi,		// Abra
		40_bi,		// Kadabra
		55_bi,		// Alakazam
		70_bi,		// Machop
		80_bi,		// Machoke
		90_bi,		// Machamp
		50_bi,		// Bellsprout
		65_bi,		// Weepinbell
		80_bi,		// Victreebel
		40_bi,		// Tentacool
		80_bi,		// Tentacruel
		40_bi,		// Geodude
		55_bi,		// Graveler
		80_bi,		// Golem
		50_bi,		// Ponyta
		65_bi,		// Rapidash
		90_bi,		// Slowpoke
		95_bi,		// Slowbro
		25_bi,		// Magnemite
		50_bi,		// Magneton
		52_bi,		// Farfetch'd
		35_bi,		// Doduo
		60_bi,		// Dodrio
		65_bi,		// Seel
		90_bi,		// Dewgong
		80_bi,		// Grimer
		105_bi,		// Muk
		30_bi,		// Shellder
		50_bi,		// Cloyster
		30_bi,		// Gastly
		45_bi,		// Haunter
		60_bi,		// Gengar
		35_bi,		// Onix
		60_bi,		// Drowzee
		85_bi,		// Hypno
		30_bi,		// Krabby
		55_bi,		// Kingler
		40_bi,		// Voltorb
		60_bi,		// Electrode
		60_bi,		// Exeggcute
		95_bi,		// Exeggutor
		50_bi,		// Cubone
		60_bi,		// Marowak
		50_bi,		// Hitmonlee
		50_bi,		// Hitmonchan
		90_bi,		// Lickitung
		40_bi,		// Koffing
		65_bi,		// Weezing
		80_bi,		// Rhyhorn
		105_bi,		// Rhydon
		250_bi,		// Chansey
		65_bi,		// Tangela
		105_bi,		// Kangaskhan
		30_bi,		// Horsea
		55_bi,		// Seadra
		45_bi,		// Goldeen
		80_bi,		// Seaking
		30_bi,		// Staryu
		60_bi,		// Starmie
		40_bi,		// Mr. Mime
		70_bi,		// Scyther
		65_bi,		// Jynx
		65_bi,		// Electabuzz
		65_bi,		// Magmar
		65_bi,		// Pinsir
		75_bi,		// Tauros
		20_bi,		// Magikarp
		95_bi,		// Gyarados
		130_bi,		// Lapras
		48_bi,		// Ditto
		55_bi,		// Eevee
		130_bi,		// Vaporeon
		65_bi,		// Jolteon
		65_bi,		// Flareon
		65_bi,		// Porygon
		35_bi,		// Omanyte
		70_bi,		// Omastar
		30_bi,		// Kabuto
		60_bi,		// Kabutops
		80_bi,		// Aerodactyl
		160_bi,		// Snorlax
		90_bi,		// Articuno
		90_bi,		// Zapdos
		90_bi,		// Moltres
		41_bi,		// Dratini
		61_bi,		// Dragonair
		91_bi,		// Dragonite
		106_bi,		// Mewtwo
		100_bi,		// Mew
		
		// Generation 2		
		45_bi,		// Chikorita
		60_bi,		// Bayleef
		80_bi,		// Meganium
		39_bi,		// Cyndaquil
		58_bi,		// Quilava
		78_bi,		// Typhlosion
		50_bi,		// Totodile
		65_bi,		// Croconaw
		85_bi,		// Feraligatr
		35_bi,		// Sentret
		85_bi,		// Furret
		60_bi,		// Hoothoot
		100_bi,		// Noctowl
		40_bi,		// Ledyba
		55_bi,		// Ledian
		40_bi,		// Spinarak
		70_bi,		// Ariados
		85_bi,		// Crobat
		75_bi,		// Chinchou
		125_bi,		// Lanturn
		20_bi,		// Pichu
		50_bi,		// Cleffa
		90_bi,		// Igglybuff
		35_bi,		// Togepi
		55_bi,		// Togetic
		40_bi,		// Natu
		65_bi,		// Xatu
		55_bi,		// Mareep
		70_bi,		// Flaaffy
		90_bi,		// Ampharos
		75_bi,		// Bellossom
		70_bi,		// Marill
		100_bi,		// Azumarill
		70_bi,		// Sudowoodo
		90_bi,		// Politoed
		35_bi,		// Hoppip
		55_bi,		// Skiploom
		75_bi,		// Jumpluff
		55_bi,		// Aipom
		30_bi,		// Sunkern
		75_bi,		// Sunflora
		65_bi,		// Yanma
		55_bi,		// Wooper
		95_bi,		// Quagsire
		65_bi,		// Espeon
		95_bi,		// Umbreon
		60_bi,		// Murkrow
		95_bi,		// Slowking
		60_bi,		// Misdreavus
		48_bi,		// Unown
		190_bi,		// Wobbuffet
		70_bi,		// Girafarig
		50_bi,		// Pineco
		75_bi,		// Forretress
		100_bi,		// Dunsparce
		65_bi,		// Gligar
		75_bi,		// Steelix
		60_bi,		// Snubbull
		90_bi,		// Granbull
		65_bi,		// Qwilfish
		70_bi,		// Scizor
		20_bi,		// Shuckle
		80_bi,		// Heracross
		55_bi,		// Sneasel
		60_bi,		// Teddiursa
		90_bi,		// Ursaring
		40_bi,		// Slugma
		50_bi,		// Magcargo
		50_bi,		// Swinub
		100_bi,		// Piloswine
		55_bi,		// Corsola
		35_bi,		// Remoraid
		75_bi,		// Octillery
		45_bi,		// Delibird
		65_bi,		// Mantine
		65_bi,		// Skarmory
		45_bi,		// Houndour
		75_bi,		// Houndoom
		75_bi,		// Kingdra
		90_bi,		// Phanpy
		90_bi,		// Donphan
		85_bi,		// Porygon2
		73_bi,		// Stantler
		55_bi,		// Smeargle
		35_bi,		// Tyrogue
		50_bi,		// Hitmontop
		45_bi,		// Smoochum
		45_bi,		// Elekid
		45_bi,		// Magby
		95_bi,		// Miltank
		255_bi,		// Blissey
		90_bi,		// Raikou
		115_bi,		// Entei
		100_bi,		// Suicune
		50_bi,		// Larvitar
		70_bi,		// Pupitar
		100_bi,		// Tyranitar
		106_bi,		// Lugia
		106_bi,		// Ho-Oh
		100_bi,		// Celebi
		
		// Generation 3		
		40_bi,		// Treecko
		50_bi,		// Grovyle
		70_bi,		// Sceptile
		45_bi,		// Torchic
		60_bi,		// Combusken
		80_bi,		// Blaziken
		50_bi,		// Mudkip
		70_bi,		// Marshtomp
		100_bi,		// Swampert
		35_bi,		// Poochyena
		70_bi,		// Mightyena
		38_bi,		// Zigzagoon
		78_bi,		// Linoone
		45_bi,		// Wurmple
		50_bi,		// Silcoon
		60_bi,		// Beautifly
		50_bi,		// Cascoon
		60_bi,		// Dustox
		40_bi,		// Lotad
		60_bi,		// Lombre
		80_bi,		// Ludicolo
		40_bi,		// Seedot
		70_bi,		// Nuzleaf
		90_bi,		// Shiftry
		40_bi,		// Taillow
		60_bi,		// Swellow
		40_bi,		// Wingull
		60_bi,		// Pelipper
		28_bi,		// Ralts
		38_bi,		// Kirlia
		68_bi,		// Gardevoir
		40_bi,		// Surskit
		70_bi,		// Masquerain
		60_bi,		// Shroomish
		60_bi,		// Breloom
		60_bi,		// Slakoth
		80_bi,		// Vigoroth
		150_bi,		// Slaking
		31_bi,		// Nincada
		61_bi,		// Ninjask
		1_bi,		// Shedinja
		64_bi,		// Whismur
		84_bi,		// Loudred
		104_bi,		// Exploud
		72_bi,		// Makuhita
		144_bi,		// Hariyama
		50_bi,		// Azurill
		30_bi,		// Nosepass
		50_bi,		// Skitty
		70_bi,		// Delcatty
		50_bi,		// Sableye
		50_bi,		// Mawile
		50_bi,		// Aron
		60_bi,		// Lairon
		70_bi,		// Aggron
		30_bi,		// Meditite
		60_bi,		// Medicham
		40_bi,		// Electrike
		70_bi,		// Manectric
		60_bi,		// Plusle
		60_bi,		// Minun
		65_bi,		// Volbeat
		65_bi,		// Illumise
		50_bi,		// Roselia
		70_bi,		// Gulpin
		100_bi,		// Swalot
		45_bi,		// Carvanha
		70_bi,		// Sharpedo
		130_bi,		// Wailmer
		170_bi,		// Wailord
		60_bi,		// Numel
		70_bi,		// Camerupt
		70_bi,		// Torkoal
		60_bi,		// Spoink
		80_bi,		// Grumpig
		60_bi,		// Spinda
		45_bi,		// Trapinch
		50_bi,		// Vibrava
		80_bi,		// Flygon
		50_bi,		// Cacnea
		70_bi,		// Cacturne
		45_bi,		// Swablu
		75_bi,		// Altaria
		73_bi,		// Zangoose
		73_bi,		// Seviper
		70_bi,		// Lunatone
		70_bi,		// Solrock
		50_bi,		// Barboach
		110_bi,		// Whiscash
		43_bi,		// Corphish
		63_bi,		// Crawdaunt
		40_bi,		// Baltoy
		60_bi,		// Claydol
		66_bi,		// Lileep
		86_bi,		// Cradily
		45_bi,		// Anorith
		75_bi,		// Armaldo
		20_bi,		// Feebas
		95_bi,		// Milotic
		70_bi,		// Castform
		60_bi,		// Kecleon
		44_bi,		// Shuppet
		64_bi,		// Banette
		20_bi,		// Duskull
		40_bi,		// Dusclops
		99_bi,		// Tropius
		65_bi,		// Chimecho
		65_bi,		// Absol
		95_bi,		// Wynaut
		50_bi,		// Snorunt
		80_bi,		// Glalie
		70_bi,		// Spheal
		90_bi,		// Sealeo
		110_bi,		// Walrein
		35_bi,		// Clamperl
		55_bi,		// Huntail
		55_bi,		// Gorebyss
		100_bi,		// Relicanth
		43_bi,		// Luvdisc
		45_bi,		// Bagon
		65_bi,		// Shelgon
		95_bi,		// Salamence
		40_bi,		// Beldum
		60_bi,		// Metang
		80_bi,		// Metagross
		80_bi,		// Regirock
		80_bi,		// Regice
		80_bi,		// Registeel
		80_bi,		// Latias
		80_bi,		// Latios
		100_bi,		// Kyogre
		100_bi,		// Groudon
		105_bi,		// Rayquaza
		100_bi,		// Jirachi
		50_bi,		// Deoxys-Mediocre
		50_bi,		// Deoxys-Attack
		50_bi,		// Deoxys-Defense
		50_bi,		// Deoxys-Speed
		
		// Generation 4		
		55_bi,		// Turtwig
		75_bi,		// Grotle
		95_bi,		// Torterra
		44_bi,		// Chimchar
		64_bi,		// Monferno
		76_bi,		// Infernape
		53_bi,		// Piplup
		64_bi,		// Prinplup
		84_bi,		// Empoleon
		40_bi,		// Starly
		55_bi,		// Staravia
		85_bi,		// Staraptor
		59_bi,		// Bidoof
		79_bi,		// Bibarel
		37_bi,		// Kricketot
		77_bi,		// Kricketune
		45_bi,		// Shinx
		60_bi,		// Luxio
		80_bi,		// Luxray
		40_bi,		// Budew
		60_bi,		// Roserade
		67_bi,		// Cranidos
		97_bi,		// Rampardos
		30_bi,		// Shieldon
		60_bi,		// Bastiodon
		40_bi,		// Burmy
		60_bi,		// Wormadam-Plant
		60_bi,		// Wormadam-Sandy
		60_bi,		// Wormadam-Trash
		70_bi,		// Mothim
		30_bi,		// Combee
		70_bi,		// Vespiquen
		60_bi,		// Pachirisu
		55_bi,		// Buizel
		85_bi,		// Floatzel
		45_bi,		// Cherubi
		70_bi,		// Cherrim
		76_bi,		// Shellos
		111_bi,		// Gastrodon
		75_bi,		// Ambipom
		90_bi,		// Drifloon
		150_bi,		// Drifblim
		55_bi,		// Buneary
		65_bi,		// Lopunny
		60_bi,		// Mismagius
		100_bi,		// Honchkrow
		49_bi,		// Glameow
		71_bi,		// Purugly
		45_bi,		// Chingling
		63_bi,		// Stunky
		103_bi,		// Skuntank
		57_bi,		// Bronzor
		67_bi,		// Bronzong
		50_bi,		// Bonsly
		20_bi,		// Mime Jr.
		100_bi,		// Happiny
		76_bi,		// Chatot
		50_bi,		// Spiritomb
		58_bi,		// Gible
		68_bi,		// Gabite
		108_bi,		// Garchomp
		135_bi,		// Munchlax
		40_bi,		// Riolu
		70_bi,		// Lucario
		68_bi,		// Hippopotas
		108_bi,		// Hippowdon
		40_bi,		// Skorupi
		70_bi,		// Drapion
		48_bi,		// Croagunk
		83_bi,		// Toxicroak
		74_bi,		// Carnivine
		49_bi,		// Finneon
		69_bi,		// Lumineon
		45_bi,		// Mantyke
		60_bi,		// Snover
		90_bi,		// Abomasnow
		70_bi,		// Weavile
		70_bi,		// Magnezone
		110_bi,		// Lickilicky
		115_bi,		// Rhyperior
		100_bi,		// Tangrowth
		75_bi,		// Electivire
		75_bi,		// Magmortar
		85_bi,		// Togekiss
		86_bi,		// Yanmega
		65_bi,		// Leafeon
		65_bi,		// Glaceon
		75_bi,		// Gliscor
		110_bi,		// Mamoswine
		85_bi,		// Porygon-Z
		68_bi,		// Gallade
		60_bi,		// Probopass
		45_bi,		// Dusknoir
		70_bi,		// Froslass
		50_bi,		// Rotom
		50_bi,		// Rotom-Heat
		50_bi,		// Rotom-Wash
		50_bi,		// Rotom-Frost
		50_bi,		// Rotom-Fan
		50_bi,		// Rotom-Mow
		75_bi,		// Uxie
		80_bi,		// Mesprit
		75_bi,		// Azelf
		100_bi,		// Dialga
		90_bi,		// Palkia
		91_bi,		// Heatran
		110_bi,		// Regigigas
		150_bi,		// Giratina-Altered
		150_bi,		// Giratina-Origin
		120_bi,		// Cresselia
		80_bi,		// Phione
		100_bi,		// Manaphy
		70_bi,		// Darkrai
		100_bi,		// Shaymin-Land
		100_bi,		// Shaymin-Sky
		120_bi,		// Arceus
		
		// Generation 5		
		100_bi,		// Victini
		45_bi,		// Snivy
		60_bi,		// Servine
		75_bi,		// Serperior
		65_bi,		// Tepig
		90_bi,		// Pignite
		110_bi,		// Emboar
		55_bi,		// Oshawott
		75_bi,		// Dewott
		95_bi,		// Samurott
		45_bi,		// Patrat
		60_bi,		// Watchog
		45_bi,		// Lillipup
		65_bi,		// Herdier
		85_bi,		// Stoutland
		41_bi,		// Purrloin
		64_bi,		// Liepard
		50_bi,		// Pansage
		75_bi,		// Simisage
		50_bi,		// Pansear
		75_bi,		// Simisear
		50_bi,		// Panpour
		75_bi,		// Simipour
		76_bi,		// Munna
		116_bi,		// Musharna
		50_bi,		// Pidove
		62_bi,		// Tranquill
		80_bi,		// Unfezant
		45_bi,		// Blitzle
		75_bi,		// Zebstrika
		55_bi,		// Roggenrola
		70_bi,		// Boldore
		85_bi,		// Gigalith
		55_bi,		// Woobat
		67_bi,		// Swoobat
		60_bi,		// Drilbur
		110_bi,		// Excadrill
		103_bi,		// Audino
		75_bi,		// Timburr
		85_bi,		// Gurdurr
		105_bi,		// Conkeldurr
		50_bi,		// Tympole
		75_bi,		// Palpitoad
		105_bi,		// Seismitoad
		120_bi,		// Throh
		75_bi,		// Sawk
		45_bi,		// Sewaddle
		55_bi,		// Swadloon
		75_bi,		// Leavanny
		30_bi,		// Venipede
		40_bi,		// Whirlipede
		60_bi,		// Scolipede
		40_bi,		// Cottonee
		60_bi,		// Whimsicott
		45_bi,		// Petilil
		70_bi,		// Lilligant
		70_bi,		// Basculin-Red
		70_bi,		// Basculin-Blue
		50_bi,		// Sandile
		60_bi,		// Krokorok
		95_bi,		// Krookodile
		70_bi,		// Darumaka
		105_bi,		// Darmanitan
		//105_bi,		// Darmanitan (Zen Mode)
		75_bi,		// Maractus
		50_bi,		// Dwebble
		70_bi,		// Crustle
		50_bi,		// Scraggy
		65_bi,		// Scrafty
		72_bi,		// Sigilyph
		38_bi,		// Yamask
		58_bi,		// Cofagrigus
		54_bi,		// Tirtouga
		74_bi,		// Carracosta
		55_bi,		// Archen
		75_bi,		// Archeops
		50_bi,		// Trubbish
		80_bi,		// Garbodor
		40_bi,		// Zorua
		60_bi,		// Zoroark
		55_bi,		// Minccino
		75_bi,		// Cinccino
		45_bi,		// Gothita
		60_bi,		// Gothorita
		70_bi,		// Gothitelle
		45_bi,		// Solosis
		65_bi,		// Duosion
		110_bi,		// Reuniclus
		62_bi,		// Ducklett
		75_bi,		// Swanna
		36_bi,		// Vanillite
		51_bi,		// Vanillish
		71_bi,		// Vanilluxe
		60_bi,		// Deerling
		80_bi,		// Sawsbuck
		55_bi,		// Emolga
		50_bi,		// Karrablast
		70_bi,		// Escavalier
		69_bi,		// Foongus
		114_bi,		// Amoonguss
		55_bi,		// Frillish
		100_bi,		// Jellicent
		165_bi,		// Alomomola
		50_bi,		// Joltik
		70_bi,		// Galvantula
		44_bi,		// Ferroseed
		74_bi,		// Ferrothorn
		40_bi,		// Klink
		60_bi,		// Klang
		60_bi,		// Klinklang
		35_bi,		// Tynamo
		65_bi,		// Eelektrik
		85_bi,		// Eelektross
		55_bi,		// Elgyem
		75_bi,		// Beheeyem
		50_bi,		// Litwick
		60_bi,		// Lampent
		60_bi,		// Chandelure
		46_bi,		// Axew
		66_bi,		// Fraxure
		76_bi,		// Haxorus
		55_bi,		// Cubchoo
		95_bi,		// Beartic
		70_bi,		// Cryogonal
		50_bi,		// Shelmet
		80_bi,		// Accelgor
		109_bi,		// Stunfisk
		45_bi,		// Mienfoo
		65_bi,		// Mienshao
		77_bi,		// Druddigon
		59_bi,		// Golett
		89_bi,		// Golurk
		45_bi,		// Pawniard
		65_bi,		// Bisharp
		95_bi,		// Bouffalant
		70_bi,		// Rufflet
		100_bi,		// Braviary
		70_bi,		// Vullaby
		110_bi,		// Mandibuzz
		85_bi,		// Heatmor
		58_bi,		// Durant
		52_bi,		// Deino
		72_bi,		// Zweilous
		92_bi,		// Hydreigon
		55_bi,		// Larvesta
		85_bi,		// Volcarona
		91_bi,		// Cobalion
		91_bi,		// Terrakion
		91_bi,		// Virizion
		79_bi,		// Tornadus-Incarnate
		79_bi,		// Tornadus-Therian
		79_bi,		// Thundurus-Incarnate
		79_bi,		// Thundurus-Therian
		100_bi,		// Reshiram
		100_bi,		// Zekrom
		89_bi,		// Landorus-Incarnate
		89_bi,		// Landorus-Therian
		125_bi,		// Kyurem
		125_bi,		// Kyurem-Black
		125_bi,		// Kyurem-White
		91_bi,		// Keldeo
		100_bi,		// Meloetta
		// 100_bi,		// Meloetta (Pirouette form)
		71_bi		// Genesect 
	);
	static_assert(std::is_same<std::decay_t<decltype(base.at(name))>, base_type>::value, "Incorrect base stat HP type.");
	return base.at(name);
}

}	// unnamed namespace
}	// namespace technicalmachine
