// Pokemon functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <map>
#include <string>
#include "movefunction.h"
#include "pokemon.h"
#include "statfunction.h"

namespace technicalmachine {

Pokemon::Pokemon (const species &member) : 
	name (member),
	status (NO_STATUS),
	type1 (get_pokemon_type [name][0]),
	type2 (get_pokemon_type [name][1]),
	vanish (LANDED),
	bide_damage (0),
	bide (0),
	confused (0),
	encore (0),
	heal_block (0),
	magnet_rise (0),
	mass (get_mass [name]),
	partial_trap (0),
	perish_song (0),
	rampage (0),
	sleep (0),
	slow_start (0),
	stockpile (0),
	taunt (0),
	toxic (0),
	uproar (0),
	yawn (0),
	aqua_ring (false),
	attract (false),
	charge (false),
	curse (false),
	damaged (false),
	defense_curl (false),
	destiny_bond (false),
	ff (false),
	flinch (false),
	focus_energy (false),
	identified (false),
	imprison (false),
	ingrain (false),
	leech_seed (false),
	loaf (false),
	lock_on (false),
	mf (false),
	minimize (false),
	moved (false),
	mud_sport (false),
	nightmare (false),
	roost (false),
	torment (false),
	trapped (false),
	water_sport (false),
	accuracy (0),
	evasion (0) {
	hp.base = base_stat [name][0];
	atk.base = base_stat [name][1];
	def.base = base_stat [name][2];
	spa.base = base_stat [name][3];
	spd.base = base_stat [name][4];
	spe.base = base_stat [name][5];
	hitpoints (*this);
}


void set_species_map (std::map <std::string, species> &species_map) {
	species_map["Abomasnow"] = ABOMASNOW;
	species_map["Abra"] = ABRA;
	species_map["Absol"] = ABSOL;
	species_map["Aerodactyl"] = AERODACTYL;
	species_map["Aggron"] = AGGRON;
	species_map["Aipom"] = AIPOM;
	species_map["Alakazam"] = ALAKAZAM;
	species_map["Altaria"] = ALTARIA;
	species_map["Ambipom"] = AMBIPOM;
	species_map["Ampharos"] = AMPHAROS;
	species_map["Anorith"] = ANORITH;
	species_map["Arbok"] = ARBOK;
	species_map["Arcanine"] = ARCANINE;
	species_map["Arceus"] = ARCEUS;
	species_map["Ariados"] = ARIADOS;
	species_map["Armaldo"] = ARMALDO;
	species_map["Aron"] = ARON;
	species_map["Articuno"] = ARTICUNO;
	species_map["Azelf"] = AZELF;
	species_map["Azumarill"] = AZUMARILL;
	species_map["Azurill"] = AZURILL;
	species_map["Bagon"] = BAGON;
	species_map["Baltoy"] = BALTOY;
	species_map["Banette"] = BANETTE;
	species_map["Barboach"] = BARBOACH;
	species_map["Bastiodon"] = BASTIODON;
	species_map["Bayleef"] = BAYLEEF;
	species_map["Beautifly"] = BEAUTIFLY;
	species_map["Beedrill"] = BEEDRILL;
	species_map["Beldum"] = BELDUM;
	species_map["Bellossom"] = BELLOSSOM;
	species_map["Bellsprout"] = BELLSPROUT;
	species_map["Bibarel"] = BIBAREL;
	species_map["Bidoof"] = BIDOOF;
	species_map["Blastoise"] = BLASTOISE;
	species_map["Blaziken"] = BLAZIKEN;
	species_map["Blissey"] = BLISSEY;
	species_map["Bonsly"] = BONSLY;
	species_map["Breloom"] = BRELOOM;
	species_map["Bronzong"] = BRONZONG;
	species_map["Bronzor"] = BRONZOR;
	species_map["Budew"] = BUDEW;
	species_map["Buizel"] = BUIZEL;
	species_map["Bulbasaur"] = BULBASAUR;
	species_map["Buneary"] = BUNEARY;
	species_map["Burmy"] = BURMY;
	species_map["Butterfree"] = BUTTERFREE;
	species_map["Cacnea"] = CACNEA;
	species_map["Cacturne"] = CACTURNE;
	species_map["Camerupt"] = CAMERUPT;
	species_map["Carnivine"] = CARNIVINE;
	species_map["Carvanha"] = CARVANHA;
	species_map["Cascoon"] = CASCOON;
	species_map["Castform"] = CASTFORM;
	species_map["Caterpie"] = CATERPIE;
	species_map["Celebi"] = CELEBI;
	species_map["Chansey"] = CHANSEY;
	species_map["Charizard"] = CHARIZARD;
	species_map["Charmander"] = CHARMANDER;
	species_map["Charmeleon"] = CHARMELEON;
	species_map["Chatot"] = CHATOT;
	species_map["Cherrim"] = CHERRIM;
	species_map["Cherubi"] = CHERUBI;
	species_map["Chikorita"] = CHIKORITA;
	species_map["Chimchar"] = CHIMCHAR;
	species_map["Chimecho"] = CHIMECHO;
	species_map["Chinchou"] = CHINCHOU;
	species_map["Chingling"] = CHINGLING;
	species_map["Clamperl"] = CLAMPERL;
	species_map["Claydol"] = CLAYDOL;
	species_map["Clefable"] = CLEFABLE;
	species_map["Clefairy"] = CLEFAIRY;
	species_map["Cleffa"] = CLEFFA;
	species_map["Cloyster"] = CLOYSTER;
	species_map["Combee"] = COMBEE;
	species_map["Combusken"] = COMBUSKEN;
	species_map["Corphish"] = CORPHISH;
	species_map["Corsola"] = CORSOLA;
	species_map["Cradily"] = CRADILY;
	species_map["Cranidos"] = CRANIDOS;
	species_map["Crawdaunt"] = CRAWDAUNT;
	species_map["Cresselia"] = CRESSELIA;
	species_map["Croagunk"] = CROAGUNK;
	species_map["Crobat"] = CROBAT;
	species_map["Croconaw"] = CROCONAW;
	species_map["Cubone"] = CUBONE;
	species_map["Cyndaquil"] = CYNDAQUIL;
	species_map["Darkrai"] = DARKRAI;
	species_map["Delcatty"] = DELCATTY;
	species_map["Delibird"] = DELIBIRD;
	species_map["Deoxys-A"] = DEOXYS_A;
	species_map["Deoxys-a"] = DEOXYS_A;
	species_map["Deoxys-F"] = DEOXYS_A;
	species_map["Deoxys-f"] = DEOXYS_A;
	species_map["Deoxys-D"] = DEOXYS_D;
	species_map["Deoxys-d"] = DEOXYS_D;
	species_map["Deoxys-L"] = DEOXYS_D;
	species_map["Deoxys-l"] = DEOXYS_D;
	species_map["Deoxys"] = DEOXYS_M;
	species_map["Deoxys-M"] = DEOXYS_M;
	species_map["Deoxys-m"] = DEOXYS_M;
	species_map["Deoxys-S"] = DEOXYS_S;
	species_map["Deoxys-s"] = DEOXYS_S;
	species_map["Deoxys-E"] = DEOXYS_S;
	species_map["Deoxys-e"] = DEOXYS_S;
	species_map["Dewgong"] = DEWGONG;
	species_map["Dialga"] = DIALGA;
	species_map["Diglett"] = DIGLETT;
	species_map["Ditto"] = DITTO;
	species_map["Dodrio"] = DODRIO;
	species_map["Doduo"] = DODUO;
	species_map["Donphan"] = DONPHAN;
	species_map["Dragonair"] = DRAGONAIR;
	species_map["Dragonite"] = DRAGONITE;
	species_map["Drapion"] = DRAPION;
	species_map["Dratini"] = DRATINI;
	species_map["Drifblim"] = DRIFBLIM;
	species_map["Drifloon"] = DRIFLOON;
	species_map["Drowzee"] = DROWZEE;
	species_map["Dugtrio"] = DUGTRIO;
	species_map["Dunsparce"] = DUNSPARCE;
	species_map["Dusclops"] = DUSCLOPS;
	species_map["Dusknoir"] = DUSKNOIR;
	species_map["Duskull"] = DUSKULL;
	species_map["Dustox"] = DUSTOX;
	species_map["Eevee"] = EEVEE;
	species_map["Ekans"] = EKANS;
	species_map["Electabuzz"] = ELECTABUZZ;
	species_map["Electivire"] = ELECTIVIRE;
	species_map["Electrike"] = ELECTRIKE;
	species_map["Electrode"] = ELECTRODE;
	species_map["Elekid"] = ELEKID;
	species_map["Empoleon"] = EMPOLEON;
	species_map["Entei"] = ENTEI;
	species_map["Espeon"] = ESPEON;
	species_map["Exeggcute"] = EXEGGCUTE;
	species_map["Exeggutor"] = EXEGGUTOR;
	species_map["Exploud"] = EXPLOUD;
	species_map["Farfetch'd"] = FARFETCHD;
	species_map["Fearow"] = FEAROW;
	species_map["Feebas"] = FEEBAS;
	species_map["Feraligatr"] = FERALIGATR;
	species_map["Finneon"] = FINNEON;
	species_map["Flaaffy"] = FLAAFFY;
	species_map["Flareon"] = FLAREON;
	species_map["Floatzel"] = FLOATZEL;
	species_map["Flygon"] = FLYGON;
	species_map["Forretress"] = FORRETRESS;
	species_map["Froslass"] = FROSLASS;
	species_map["Furret"] = FURRET;
	species_map["Gabite"] = GABITE;
	species_map["Gallade"] = GALLADE;
	species_map["Garchomp"] = GARCHOMP;
	species_map["Gardevoir"] = GARDEVOIR;
	species_map["Gastly"] = GASTLY;
	species_map["Gastrodon"] = GASTRODON;
	species_map["Gengar"] = GENGAR;
	species_map["Geodude"] = GEODUDE;
	species_map["Gible"] = GIBLE;
	species_map["Girafarig"] = GIRAFARIG;
	species_map["Giratina"] = GIRATINA_A;
	species_map["Giratina-A"] = GIRATINA_A;
	species_map["Giratina-a"] = GIRATINA_A;
	species_map["Giratina-O"] = GIRATINA_O;
	species_map["Giratina-o"] = GIRATINA_O;
	species_map["Glaceon"] = GLACEON;
	species_map["Glalie"] = GLALIE;
	species_map["Glameow"] = GLAMEOW;
	species_map["Gligar"] = GLIGAR;
	species_map["Gliscor"] = GLISCOR;
	species_map["Gloom"] = GLOOM;
	species_map["Golbat"] = GOLBAT;
	species_map["Goldeen"] = GOLDEEN;
	species_map["Golduck"] = GOLDUCK;
	species_map["Golem"] = GOLEM;
	species_map["Gorebyss"] = GOREBYSS;
	species_map["Granbull"] = GRANBULL;
	species_map["Graveler"] = GRAVELER;
	species_map["Grimer"] = GRIMER;
	species_map["Grotle"] = GROTLE;
	species_map["Groudon"] = GROUDON;
	species_map["Grovyle"] = GROVYLE;
	species_map["Growlithe"] = GROWLITHE;
	species_map["Grumpig"] = GRUMPIG;
	species_map["Gulpin"] = GULPIN;
	species_map["Gyarados"] = GYARADOS;
	species_map["Happiny"] = HAPPINY;
	species_map["Hariyama"] = HARIYAMA;
	species_map["Haunter"] = HAUNTER;
	species_map["Heatran"] = HEATRAN;
	species_map["Heracross"] = HERACROSS;
	species_map["Hippopotas"] = HIPPOPOTAS;
	species_map["Hippowdon"] = HIPPOWDON;
	species_map["Hitmonchan"] = HITMONCHAN;
	species_map["Hitmonlee"] = HITMONLEE;
	species_map["Hitmontop"] = HITMONTOP;
	species_map["Ho-Oh"] = HO_OH;
	species_map["Ho-oh"] = HO_OH;
	species_map["Honchkrow"] = HONCHKROW;
	species_map["Hoothoot"] = HOOTHOOT;
	species_map["Hoppip"] = HOPPIP;
	species_map["Horsea"] = HORSEA;
	species_map["Houndoom"] = HOUNDOOM;
	species_map["Houndour"] = HOUNDOUR;
	species_map["Huntail"] = HUNTAIL;
	species_map["Hypno"] = HYPNO;
	species_map["Igglybuff"] = IGGLYBUFF;
	species_map["Illumise"] = ILLUMISE;
	species_map["Infernape"] = INFERNAPE;
	species_map["Ivysaur"] = IVYSAUR;
	species_map["Jigglypuff"] = JIGGLYPUFF;
	species_map["Jirachi"] = JIRACHI;
	species_map["Jolteon"] = JOLTEON;
	species_map["Jumpluff"] = JUMPLUFF;
	species_map["Jynx"] = JYNX;
	species_map["Kabuto"] = KABUTO;
	species_map["Kabutops"] = KABUTOPS;
	species_map["Kadabra"] = KADABRA;
	species_map["Kakuna"] = KAKUNA;
	species_map["Kangaskhan"] = KANGASKHAN;
	species_map["Kecleon"] = KECLEON;
	species_map["Kingdra"] = KINGDRA;
	species_map["Kingler"] = KINGLER;
	species_map["Kirlia"] = KIRLIA;
	species_map["Koffing"] = KOFFING;
	species_map["Krabby"] = KRABBY;
	species_map["Kricketot"] = KRICKETOT;
	species_map["Kricketune"] = KRICKETUNE;
	species_map["Kyogre"] = KYOGRE;
	species_map["Lairon"] = LAIRON;
	species_map["Lanturn"] = LANTURN;
	species_map["Lapras"] = LAPRAS;
	species_map["Larvitar"] = LARVITAR;
	species_map["Latias"] = LATIAS;
	species_map["Latios"] = LATIOS;
	species_map["Leafeon"] = LEAFEON;
	species_map["Ledian"] = LEDIAN;
	species_map["Ledyba"] = LEDYBA;
	species_map["Lickilicky"] = LICKILICKY;
	species_map["Lickitung"] = LICKITUNG;
	species_map["Lileep"] = LILEEP;
	species_map["Linoone"] = LINOONE;
	species_map["Lombre"] = LOMBRE;
	species_map["Lopunny"] = LOPUNNY;
	species_map["Lotad"] = LOTAD;
	species_map["Loudred"] = LOUDRED;
	species_map["Lucario"] = LUCARIO;
	species_map["Ludicolo"] = LUDICOLO;
	species_map["Lugia"] = LUGIA;
	species_map["Lumineon"] = LUMINEON;
	species_map["Lunatone"] = LUNATONE;
	species_map["Luvdisc"] = LUVDISC;
	species_map["Luxio"] = LUXIO;
	species_map["Luxray"] = LUXRAY;
	species_map["Machamp"] = MACHAMP;
	species_map["Machoke"] = MACHOKE;
	species_map["Machop"] = MACHOP;
	species_map["Magby"] = MAGBY;
	species_map["Magcargo"] = MAGCARGO;
	species_map["Magikarp"] = MAGIKARP;
	species_map["Magmar"] = MAGMAR;
	species_map["Magmortar"] = MAGMORTAR;
	species_map["Magnemite"] = MAGNEMITE;
	species_map["Magneton"] = MAGNETON;
	species_map["Magnezone"] = MAGNEZONE;
	species_map["Makuhita"] = MAKUHITA;
	species_map["Mamoswine"] = MAMOSWINE;
	species_map["Manaphy"] = MANAPHY;
	species_map["Manectric"] = MANECTRIC;
	species_map["Mankey"] = MANKEY;
	species_map["Mantine"] = MANTINE;
	species_map["Mantyke"] = MANTYKE;
	species_map["Mareep"] = MAREEP;
	species_map["Marill"] = MARILL;
	species_map["Marowak"] = MAROWAK;
	species_map["Marshtomp"] = MARSHTOMP;
	species_map["Masquerain"] = MASQUERAIN;
	species_map["Mawile"] = MAWILE;
	species_map["Medicham"] = MEDICHAM;
	species_map["Meditite"] = MEDITITE;
	species_map["Meganium"] = MEGANIUM;
	species_map["Meowth"] = MEOWTH;
	species_map["Mesprit"] = MESPRIT;
	species_map["Metagross"] = METAGROSS;
	species_map["Metang"] = METANG;
	species_map["Metapod"] = METAPOD;
	species_map["Mew"] = MEW;
	species_map["Mewtwo"] = MEWTWO;
	species_map["Mightyena"] = MIGHTYENA;
	species_map["Milotic"] = MILOTIC;
	species_map["Miltank"] = MILTANK;
	species_map["Mime Jr."] = MIME_JR;
	species_map["Minun"] = MINUN;
	species_map["Misdreavus"] = MISDREAVUS;
	species_map["Mismagius"] = MISMAGIUS;
	species_map["Moltres"] = MOLTRES;
	species_map["Monferno"] = MONFERNO;
	species_map["Mothim"] = MOTHIM;
	species_map["Mr. Mime"] = MR_MIME;
	species_map["Mudkip"] = MUDKIP;
	species_map["Muk"] = MUK;
	species_map["Munchlax"] = MUNCHLAX;
	species_map["Murkrow"] = MURKROW;
	species_map["Natu"] = NATU;
	species_map["Nidoking"] = NIDOKING;
	species_map["Nidoqueen"] = NIDOQUEEN;
	species_map["Nidoran-F"] = NIDORAN_F;
	species_map["Nidoran-f"] = NIDORAN_F;
	species_map["Nidoran-M"] = NIDORAN_M;
	species_map["Nidoran-m"] = NIDORAN_M;
	species_map["Nidorina"] = NIDORINA;
	species_map["Nidorino"] = NIDORINO;
	species_map["Nincada"] = NINCADA;
	species_map["Ninetales"] = NINETALES;
	species_map["Ninjask"] = NINJASK;
	species_map["Noctowl"] = NOCTOWL;
	species_map["Nosepass"] = NOSEPASS;
	species_map["Numel"] = NUMEL;
	species_map["Nuzleaf"] = NUZLEAF;
	species_map["Octillery"] = OCTILLERY;
	species_map["Oddish"] = ODDISH;
	species_map["Omanyte"] = OMANYTE;
	species_map["Omastar"] = OMASTAR;
	species_map["Onix"] = ONIX;
	species_map["Pachirisu"] = PACHIRISU;
	species_map["Palkia"] = PALKIA;
	species_map["Paras"] = PARAS;
	species_map["Parasect"] = PARASECT;
	species_map["Pelipper"] = PELIPPER;
	species_map["Persian"] = PERSIAN;
	species_map["Phanpy"] = PHANPY;
	species_map["Phione"] = PHIONE;
	species_map["Pichu"] = PICHU;
	species_map["Pidgeot"] = PIDGEOT;
	species_map["Pidgeotto"] = PIDGEOTTO;
	species_map["Pidgey"] = PIDGEY;
	species_map["Pikachu"] = PIKACHU;
	species_map["Piloswine"] = PILOSWINE;
	species_map["Pineco"] = PINECO;
	species_map["Pinsir"] = PINSIR;
	species_map["Piplup"] = PIPLUP;
	species_map["Plusle"] = PLUSLE;
	species_map["Politoed"] = POLITOED;
	species_map["Poliwag"] = POLIWAG;
	species_map["Poliwhirl"] = POLIWHIRL;
	species_map["Poliwrath"] = POLIWRATH;
	species_map["Ponyta"] = PONYTA;
	species_map["Poochyena"] = POOCHYENA;
	species_map["Porygon"] = PORYGON;
	species_map["Porygon-Z"] = PORYGON_Z;
	species_map["Porygon-z"] = PORYGON_Z;
	species_map["Porygonz"] = PORYGON_Z;
	species_map["Porygon2"] = PORYGON2;
	species_map["Primeape"] = PRIMEAPE;
	species_map["Prinplup"] = PRINPLUP;
	species_map["Probopass"] = PROBOPASS;
	species_map["Psyduck"] = PSYDUCK;
	species_map["Pupitar"] = PUPITAR;
	species_map["Purugly"] = PURUGLY;
	species_map["Quagsire"] = QUAGSIRE;
	species_map["Quilava"] = QUILAVA;
	species_map["Qwilfish"] = QWILFISH;
	species_map["Raichu"] = RAICHU;
	species_map["Raikou"] = RAIKOU;
	species_map["Ralts"] = RALTS;
	species_map["Rampardos"] = RAMPARDOS;
	species_map["Rapidash"] = RAPIDASH;
	species_map["Raticate"] = RATICATE;
	species_map["Rattata"] = RATTATA;
	species_map["Rayquaza"] = RAYQUAZA;
	species_map["Regice"] = REGICE;
	species_map["Regigigas"] = REGIGIGAS;
	species_map["Regirock"] = REGIROCK;
	species_map["Registeel"] = REGISTEEL;
	species_map["Relicanth"] = RELICANTH;
	species_map["Remoraid"] = REMORAID;
	species_map["Rhydon"] = RHYDON;
	species_map["Rhyhorn"] = RHYHORN;
	species_map["Rhyperior"] = RHYPERIOR;
	species_map["Riolu"] = RIOLU;
	species_map["Roselia"] = ROSELIA;
	species_map["Roserade"] = ROSERADE;
	species_map["Rotom"] = ROTOM;
	species_map["Rotom-C"] = ROTOM_C;
	species_map["Rotom-c"] = ROTOM_C;
	species_map["Rotom-F"] = ROTOM_F;
	species_map["Rotom-f"] = ROTOM_F;
	species_map["Rotom-H"] = ROTOM_H;
	species_map["Rotom-h"] = ROTOM_H;
	species_map["Rotom-S"] = ROTOM_S;
	species_map["Rotom-s"] = ROTOM_S;
	species_map["Rotom-W"] = ROTOM_W;
	species_map["Rotom-w"] = ROTOM_W;
	species_map["Sableye"] = SABLEYE;
	species_map["Salamence"] = SALAMENCE;
	species_map["Sandshrew"] = SANDSHREW;
	species_map["Sandslash"] = SANDSLASH;
	species_map["Sceptile"] = SCEPTILE;
	species_map["Scizor"] = SCIZOR;
	species_map["Scyther"] = SCYTHER;
	species_map["Seadra"] = SEADRA;
	species_map["Seaking"] = SEAKING;
	species_map["Sealeo"] = SEALEO;
	species_map["Seedot"] = SEEDOT;
	species_map["Seel"] = SEEL;
	species_map["Sentret"] = SENTRET;
	species_map["Seviper"] = SEVIPER;
	species_map["Sharpedo"] = SHARPEDO;
	species_map["Shaymin"] = SHAYMIN_L;
	species_map["Shaymin-L"] = SHAYMIN_L;
	species_map["Shaymin-l"] = SHAYMIN_L;
	species_map["Shaymin-S"] = SHAYMIN_L;
	species_map["Shaymin-s"] = SHAYMIN_S;
	species_map["Shedinja"] = SHEDINJA;
	species_map["Shelgon"] = SHELGON;
	species_map["Shellder"] = SHELLDER;
	species_map["Shellos"] = SHELLOS;
	species_map["Shieldon"] = SHIELDON;
	species_map["Shiftry"] = SHIFTRY;
	species_map["Shinx"] = SHINX;
	species_map["Shroomish"] = SHROOMISH;
	species_map["Shuckle"] = SHUCKLE;
	species_map["Shuppet"] = SHUPPET;
	species_map["Silcoon"] = SILCOON;
	species_map["Skarmory"] = SKARMORY;
	species_map["Skiploom"] = SKIPLOOM;
	species_map["Skitty"] = SKITTY;
	species_map["Skorupi"] = SKORUPI;
	species_map["Skuntank"] = SKUNTANK;
	species_map["Slaking"] = SLAKING;
	species_map["Slakoth"] = SLAKOTH;
	species_map["Slowbro"] = SLOWBRO;
	species_map["Slowking"] = SLOWKING;
	species_map["Slowpoke"] = SLOWPOKE;
	species_map["Slugma"] = SLUGMA;
	species_map["Smeargle"] = SMEARGLE;
	species_map["Smoochum"] = SMOOCHUM;
	species_map["Sneasel"] = SNEASEL;
	species_map["Snorlax"] = SNORLAX;
	species_map["Snorunt"] = SNORUNT;
	species_map["Snover"] = SNOVER;
	species_map["Snubbull"] = SNUBBULL;
	species_map["Solrock"] = SOLROCK;
	species_map["Spearow"] = SPEAROW;
	species_map["Spheal"] = SPHEAL;
	species_map["Spinarak"] = SPINARAK;
	species_map["Spinda"] = SPINDA;
	species_map["Spiritomb"] = SPIRITOMB;
	species_map["Spoink"] = SPOINK;
	species_map["Squirtle"] = SQUIRTLE;
	species_map["Stantler"] = STANTLER;
	species_map["Staraptor"] = STARAPTOR;
	species_map["Staravia"] = STARAVIA;
	species_map["Starly"] = STARLY;
	species_map["Starmie"] = STARMIE;
	species_map["Staryu"] = STARYU;
	species_map["Steelix"] = STEELIX;
	species_map["Stunky"] = STUNKY;
	species_map["Sudowoodo"] = SUDOWOODO;
	species_map["Suicune"] = SUICUNE;
	species_map["Sunflora"] = SUNFLORA;
	species_map["Sunkern"] = SUNKERN;
	species_map["Surskit"] = SURSKIT;
	species_map["Swablu"] = SWABLU;
	species_map["Swalot"] = SWALOT;
	species_map["Swampert"] = SWAMPERT;
	species_map["Swellow"] = SWELLOW;
	species_map["Swinub"] = SWINUB;
	species_map["Taillow"] = TAILLOW;
	species_map["Tangela"] = TANGELA;
	species_map["Tangrowth"] = TANGROWTH;
	species_map["Tauros"] = TAUROS;
	species_map["Teddiursa"] = TEDDIURSA;
	species_map["Tentacool"] = TENTACOOL;
	species_map["Tentacruel"] = TENTACRUEL;
	species_map["Togekiss"] = TOGEKISS;
	species_map["Togepi"] = TOGEPI;
	species_map["Togetic"] = TOGETIC;
	species_map["Torchic"] = TORCHIC;
	species_map["Torkoal"] = TORKOAL;
	species_map["Torterra"] = TORTERRA;
	species_map["Totodile"] = TOTODILE;
	species_map["Toxicroak"] = TOXICROAK;
	species_map["Trapinch"] = TRAPINCH;
	species_map["Treecko"] = TREECKO;
	species_map["Tropius"] = TROPIUS;
	species_map["Turtwig"] = TURTWIG;
	species_map["Typhlosion"] = TYPHLOSION;
	species_map["Tyranitar"] = TYRANITAR;
	species_map["Tyrogue"] = TYROGUE;
	species_map["Umbreon"] = UMBREON;
	species_map["Unown"] = UNOWN;
	species_map["Ursaring"] = URSARING;
	species_map["Uxie"] = UXIE;
	species_map["Vaporeon"] = VAPOREON;
	species_map["Venomoth"] = VENOMOTH;
	species_map["Venonat"] = VENONAT;
	species_map["Venusaur"] = VENUSAUR;
	species_map["Vespiquen"] = VESPIQUEN;
	species_map["Vibrava"] = VIBRAVA;
	species_map["Victreebel"] = VICTREEBEL;
	species_map["Vigoroth"] = VIGOROTH;
	species_map["Vileplume"] = VILEPLUME;
	species_map["Volbeat"] = VOLBEAT;
	species_map["Voltorb"] = VOLTORB;
	species_map["Vulpix"] = VULPIX;
	species_map["Wailmer"] = WAILMER;
	species_map["Wailord"] = WAILORD;
	species_map["Walrein"] = WALREIN;
	species_map["Wartortle"] = WARTORTLE;
	species_map["Weavile"] = WEAVILE;
	species_map["Weedle"] = WEEDLE;
	species_map["Weepinbell"] = WEEPINBELL;
	species_map["Weezing"] = WEEZING;
	species_map["Whiscash"] = WHISCASH;
	species_map["Whismur"] = WHISMUR;
	species_map["Wigglytuff"] = WIGGLYTUFF;
	species_map["Wingull"] = WINGULL;
	species_map["Wobbuffet"] = WOBBUFFET;
	species_map["Wooper"] = WOOPER;
	species_map["Wormadam"] = WORMADAM_P;
	species_map["Wormadam-P"] = WORMADAM_P;
	species_map["Wormadam-g"] = WORMADAM_S;
	species_map["Wormadam-S"] = WORMADAM_S;
	species_map["Wormadam-s"] = WORMADAM_T;
	species_map["Wormadam-T"] = WORMADAM_T;
	species_map["Wurmple"] = WURMPLE;
	species_map["Wynaut"] = WYNAUT;
	species_map["Xatu"] = XATU;
	species_map["Yanma"] = YANMA;
	species_map["Yanmega"] = YANMEGA;
	species_map["Zangoose"] = ZANGOOSE;
	species_map["Zapdos"] = ZAPDOS;
	species_map["Zigzagoon"] = ZIGZAGOON;
	species_map["Zubat"] = ZUBAT;
}

Map::Map() {
	set_species_map (specie);
	set_ability_map (ability);
	set_item_map (item);
	set_nature_map (nature);
	set_move_map (move);
}

}
