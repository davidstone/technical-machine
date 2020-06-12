// Convert to / from PO's format
// Copyright (C) 2015 David Stone
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

#include <tm/clients/pokemon_online/conversion.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/species.hpp>

#include <bounded/assert.hpp>
#include <bounded/unreachable.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace technicalmachine {
namespace po {

namespace {

struct InvalidFormeID : std::runtime_error {
	InvalidFormeID(std::string const & species, SpeciesIDs::Forme const forme):
		std::runtime_error("Invalid forme ID " + to_string(forme) + " for " + species)
	{
	}
};

struct UnsupportedSpecies : std::runtime_error {
	explicit UnsupportedSpecies(Species const species):
		std::runtime_error(std::string(to_string(species)) + " not supported in Pokemon Online.")
	{
	}
};

}	// namespace

Species id_to_species(SpeciesIDs const species) {
	switch (species.id.value()) {
		// Generation 1
		case 1: return Species::Bulbasaur;
		case 2: return Species::Ivysaur;
		case 3: return Species::Venusaur;
		case 4: return Species::Charmander;
		case 5: return Species::Charmeleon;
		case 6: return Species::Charizard;
		case 7: return Species::Squirtle;
		case 8: return Species::Wartortle;
		case 9: return Species::Blastoise;
		case 10: return Species::Caterpie;
		case 11: return Species::Metapod;
		case 12: return Species::Butterfree;
		case 13: return Species::Weedle;
		case 14: return Species::Kakuna;
		case 15: return Species::Beedrill;
		case 16: return Species::Pidgey;
		case 17: return Species::Pidgeotto;
		case 18: return Species::Pidgeot;
		case 19: return Species::Rattata;
		case 20: return Species::Raticate;
		case 21: return Species::Spearow;
		case 22: return Species::Fearow;
		case 23: return Species::Ekans;
		case 24: return Species::Arbok;
		case 25: return Species::Pikachu;
		case 26: return Species::Raichu;
		case 27: return Species::Sandshrew;
		case 28: return Species::Sandslash;
		case 29: return Species::Nidoran_F;
		case 30: return Species::Nidorina;
		case 31: return Species::Nidoqueen;
		case 32: return Species::Nidoran_M;
		case 33: return Species::Nidorino;
		case 34: return Species::Nidoking;
		case 35: return Species::Clefairy;
		case 36: return Species::Clefable;
		case 37: return Species::Vulpix;
		case 38: return Species::Ninetales;
		case 39: return Species::Jigglypuff;
		case 40: return Species::Wigglytuff;
		case 41: return Species::Zubat;
		case 42: return Species::Golbat;
		case 43: return Species::Oddish;
		case 44: return Species::Gloom;
		case 45: return Species::Vileplume;
		case 46: return Species::Paras;
		case 47: return Species::Parasect;
		case 48: return Species::Venonat;
		case 49: return Species::Venomoth;
		case 50: return Species::Diglett;
		case 51: return Species::Dugtrio;
		case 52: return Species::Meowth;
		case 53: return Species::Persian;
		case 54: return Species::Psyduck;
		case 55: return Species::Golduck;
		case 56: return Species::Mankey;
		case 57: return Species::Primeape;
		case 58: return Species::Growlithe;
		case 59: return Species::Arcanine;
		case 60: return Species::Poliwag;
		case 61: return Species::Poliwhirl;
		case 62: return Species::Poliwrath;
		case 63: return Species::Abra;
		case 64: return Species::Kadabra;
		case 65: return Species::Alakazam;
		case 66: return Species::Machop;
		case 67: return Species::Machoke;
		case 68: return Species::Machamp;
		case 69: return Species::Bellsprout;
		case 70: return Species::Weepinbell;
		case 71: return Species::Victreebel;
		case 72: return Species::Tentacool;
		case 73: return Species::Tentacruel;
		case 74: return Species::Geodude;
		case 75: return Species::Graveler;
		case 76: return Species::Golem;
		case 77: return Species::Ponyta;
		case 78: return Species::Rapidash;
		case 79: return Species::Slowpoke;
		case 80: return Species::Slowbro;
		case 81: return Species::Magnemite;
		case 82: return Species::Magneton;
		case 83: return Species::Farfetchd;
		case 84: return Species::Doduo;
		case 85: return Species::Dodrio;
		case 86: return Species::Seel;
		case 87: return Species::Dewgong;
		case 88: return Species::Grimer;
		case 89: return Species::Muk;
		case 90: return Species::Shellder;
		case 91: return Species::Cloyster;
		case 92: return Species::Gastly;
		case 93: return Species::Haunter;
		case 94: return Species::Gengar;
		case 95: return Species::Onix;
		case 96: return Species::Drowzee;
		case 97: return Species::Hypno;
		case 98: return Species::Krabby;
		case 99: return Species::Kingler;
		case 100: return Species::Voltorb;
		case 101: return Species::Electrode;
		case 102: return Species::Exeggcute;
		case 103: return Species::Exeggutor;
		case 104: return Species::Cubone;
		case 105: return Species::Marowak;
		case 106: return Species::Hitmonlee;
		case 107: return Species::Hitmonchan;
		case 108: return Species::Lickitung;
		case 109: return Species::Koffing;
		case 110: return Species::Weezing;
		case 111: return Species::Rhyhorn;
		case 112: return Species::Rhydon;
		case 113: return Species::Chansey;
		case 114: return Species::Tangela;
		case 115: return Species::Kangaskhan;
		case 116: return Species::Horsea;
		case 117: return Species::Seadra;
		case 118: return Species::Goldeen;
		case 119: return Species::Seaking;
		case 120: return Species::Staryu;
		case 121: return Species::Starmie;
		case 122: return Species::Mr_Mime;
		case 123: return Species::Scyther;
		case 124: return Species::Jynx;
		case 125: return Species::Electabuzz;
		case 126: return Species::Magmar;
		case 127: return Species::Pinsir;
		case 128: return Species::Tauros;
		case 129: return Species::Magikarp;
		case 130: return Species::Gyarados;
		case 131: return Species::Lapras;
		case 132: return Species::Ditto;
		case 133: return Species::Eevee;
		case 134: return Species::Vaporeon;
		case 135: return Species::Jolteon;
		case 136: return Species::Flareon;
		case 137: return Species::Porygon;
		case 138: return Species::Omanyte;
		case 139: return Species::Omastar;
		case 140: return Species::Kabuto;
		case 141: return Species::Kabutops;
		case 142: return Species::Aerodactyl;
		case 143: return Species::Snorlax;
		case 144: return Species::Articuno;
		case 145: return Species::Zapdos;
		case 146: return Species::Moltres;
		case 147: return Species::Dratini;
		case 148: return Species::Dragonair;
		case 149: return Species::Dragonite;
		case 150: return Species::Mewtwo;
		case 151: return Species::Mew;

		// Generation 2
		case 152: return Species::Chikorita;
		case 153: return Species::Bayleef;
		case 154: return Species::Meganium;
		case 155: return Species::Cyndaquil;
		case 156: return Species::Quilava;
		case 157: return Species::Typhlosion;
		case 158: return Species::Totodile;
		case 159: return Species::Croconaw;
		case 160: return Species::Feraligatr;
		case 161: return Species::Sentret;
		case 162: return Species::Furret;
		case 163: return Species::Hoothoot;
		case 164: return Species::Noctowl;
		case 165: return Species::Ledyba;
		case 166: return Species::Ledian;
		case 167: return Species::Spinarak;
		case 168: return Species::Ariados;
		case 169: return Species::Crobat;
		case 170: return Species::Chinchou;
		case 171: return Species::Lanturn;
		case 172: return Species::Pichu;
		case 173: return Species::Cleffa;
		case 174: return Species::Igglybuff;
		case 175: return Species::Togepi;
		case 176: return Species::Togetic;
		case 177: return Species::Natu;
		case 178: return Species::Xatu;
		case 179: return Species::Mareep;
		case 180: return Species::Flaaffy;
		case 181: return Species::Ampharos;
		case 182: return Species::Bellossom;
		case 183: return Species::Marill;
		case 184: return Species::Azumarill;
		case 185: return Species::Sudowoodo;
		case 186: return Species::Politoed;
		case 187: return Species::Hoppip;
		case 188: return Species::Skiploom;
		case 189: return Species::Jumpluff;
		case 190: return Species::Aipom;
		case 191: return Species::Sunkern;
		case 192: return Species::Sunflora;
		case 193: return Species::Yanma;
		case 194: return Species::Wooper;
		case 195: return Species::Quagsire;
		case 196: return Species::Espeon;
		case 197: return Species::Umbreon;
		case 198: return Species::Murkrow;
		case 199: return Species::Slowking;
		case 200: return Species::Misdreavus;
		case 201: return Species::Unown;
		case 202: return Species::Wobbuffet;
		case 203: return Species::Girafarig;
		case 204: return Species::Pineco;
		case 205: return Species::Forretress;
		case 206: return Species::Dunsparce;
		case 207: return Species::Gligar;
		case 208: return Species::Steelix;
		case 209: return Species::Snubbull;
		case 210: return Species::Granbull;
		case 211: return Species::Qwilfish;
		case 212: return Species::Scizor;
		case 213: return Species::Shuckle;
		case 214: return Species::Heracross;
		case 215: return Species::Sneasel;
		case 216: return Species::Teddiursa;
		case 217: return Species::Ursaring;
		case 218: return Species::Slugma;
		case 219: return Species::Magcargo;
		case 220: return Species::Swinub;
		case 221: return Species::Piloswine;
		case 222: return Species::Corsola;
		case 223: return Species::Remoraid;
		case 224: return Species::Octillery;
		case 225: return Species::Delibird;
		case 226: return Species::Mantine;
		case 227: return Species::Skarmory;
		case 228: return Species::Houndour;
		case 229: return Species::Houndoom;
		case 230: return Species::Kingdra;
		case 231: return Species::Phanpy;
		case 232: return Species::Donphan;
		case 233: return Species::Porygon2;
		case 234: return Species::Stantler;
		case 235: return Species::Smeargle;
		case 236: return Species::Tyrogue;
		case 237: return Species::Hitmontop;
		case 238: return Species::Smoochum;
		case 239: return Species::Elekid;
		case 240: return Species::Magby;
		case 241: return Species::Miltank;
		case 242: return Species::Blissey;
		case 243: return Species::Raikou;
		case 244: return Species::Entei;
		case 245: return Species::Suicune;
		case 246: return Species::Larvitar;
		case 247: return Species::Pupitar;
		case 248: return Species::Tyranitar;
		case 249: return Species::Lugia;
		case 250: return Species::Ho_Oh;
		case 251: return Species::Celebi;

		// Generation 3
		case 252: return Species::Treecko;
		case 253: return Species::Grovyle;
		case 254: return Species::Sceptile;
		case 255: return Species::Torchic;
		case 256: return Species::Combusken;
		case 257: return Species::Blaziken;
		case 258: return Species::Mudkip;
		case 259: return Species::Marshtomp;
		case 260: return Species::Swampert;
		case 261: return Species::Poochyena;
		case 262: return Species::Mightyena;
		case 263: return Species::Zigzagoon;
		case 264: return Species::Linoone;
		case 265: return Species::Wurmple;
		case 266: return Species::Silcoon;
		case 267: return Species::Beautifly;
		case 268: return Species::Cascoon;
		case 269: return Species::Dustox;
		case 270: return Species::Lotad;
		case 271: return Species::Lombre;
		case 272: return Species::Ludicolo;
		case 273: return Species::Seedot;
		case 274: return Species::Nuzleaf;
		case 275: return Species::Shiftry;
		case 276: return Species::Taillow;
		case 277: return Species::Swellow;
		case 278: return Species::Wingull;
		case 279: return Species::Pelipper;
		case 280: return Species::Ralts;
		case 281: return Species::Kirlia;
		case 282: return Species::Gardevoir;
		case 283: return Species::Surskit;
		case 284: return Species::Masquerain;
		case 285: return Species::Shroomish;
		case 286: return Species::Breloom;
		case 287: return Species::Slakoth;
		case 288: return Species::Vigoroth;
		case 289: return Species::Slaking;
		case 290: return Species::Nincada;
		case 291: return Species::Ninjask;
		case 292: return Species::Shedinja;
		case 293: return Species::Whismur;
		case 294: return Species::Loudred;
		case 295: return Species::Exploud;
		case 296: return Species::Makuhita;
		case 297: return Species::Hariyama;
		case 298: return Species::Azurill;
		case 299: return Species::Nosepass;
		case 300: return Species::Skitty;
		case 301: return Species::Delcatty;
		case 302: return Species::Sableye;
		case 303: return Species::Mawile;
		case 304: return Species::Aron;
		case 305: return Species::Lairon;
		case 306: return Species::Aggron;
		case 307: return Species::Meditite;
		case 308: return Species::Medicham;
		case 309: return Species::Electrike;
		case 310: return Species::Manectric;
		case 311: return Species::Plusle;
		case 312: return Species::Minun;
		case 313: return Species::Volbeat;
		case 314: return Species::Illumise;
		case 315: return Species::Roselia;
		case 316: return Species::Gulpin;
		case 317: return Species::Swalot;
		case 318: return Species::Carvanha;
		case 319: return Species::Sharpedo;
		case 320: return Species::Wailmer;
		case 321: return Species::Wailord;
		case 322: return Species::Numel;
		case 323: return Species::Camerupt;
		case 324: return Species::Torkoal;
		case 325: return Species::Spoink;
		case 326: return Species::Grumpig;
		case 327: return Species::Spinda;
		case 328: return Species::Trapinch;
		case 329: return Species::Vibrava;
		case 330: return Species::Flygon;
		case 331: return Species::Cacnea;
		case 332: return Species::Cacturne;
		case 333: return Species::Swablu;
		case 334: return Species::Altaria;
		case 335: return Species::Zangoose;
		case 336: return Species::Seviper;
		case 337: return Species::Lunatone;
		case 338: return Species::Solrock;
		case 339: return Species::Barboach;
		case 340: return Species::Whiscash;
		case 341: return Species::Corphish;
		case 342: return Species::Crawdaunt;
		case 343: return Species::Baltoy;
		case 344: return Species::Claydol;
		case 345: return Species::Lileep;
		case 346: return Species::Cradily;
		case 347: return Species::Anorith;
		case 348: return Species::Armaldo;
		case 349: return Species::Feebas;
		case 350: return Species::Milotic;
		case 351: return Species::Castform;
		case 352: return Species::Kecleon;
		case 353: return Species::Shuppet;
		case 354: return Species::Banette;
		case 355: return Species::Duskull;
		case 356: return Species::Dusclops;
		case 357: return Species::Tropius;
		case 358: return Species::Chimecho;
		case 359: return Species::Absol;
		case 360: return Species::Wynaut;
		case 361: return Species::Snorunt;
		case 362: return Species::Glalie;
		case 363: return Species::Spheal;
		case 364: return Species::Sealeo;
		case 365: return Species::Walrein;
		case 366: return Species::Clamperl;
		case 367: return Species::Huntail;
		case 368: return Species::Gorebyss;
		case 369: return Species::Relicanth;
		case 370: return Species::Luvdisc;
		case 371: return Species::Bagon;
		case 372: return Species::Shelgon;
		case 373: return Species::Salamence;
		case 374: return Species::Beldum;
		case 375: return Species::Metang;
		case 376: return Species::Metagross;
		case 377: return Species::Regirock;
		case 378: return Species::Regice;
		case 379: return Species::Registeel;
		case 380: return Species::Latias;
		case 381: return Species::Latios;
		case 382: return Species::Kyogre;
		case 383: return Species::Groudon;
		case 384: return Species::Rayquaza;
		case 385: return Species::Jirachi;
		case 386:
			switch (species.forme.value()) {
				case 0: return Species::Deoxys_Normal;
				case 1: return Species::Deoxys_Attack;
				case 2: return Species::Deoxys_Defense;
				case 3: return Species::Deoxys_Speed;
				default: throw InvalidFormeID("Deoxys", species.forme);
			}

		// Generation 4
		case 387: return Species::Turtwig;
		case 388: return Species::Grotle;
		case 389: return Species::Torterra;
		case 390: return Species::Chimchar;
		case 391: return Species::Monferno;
		case 392: return Species::Infernape;
		case 393: return Species::Piplup;
		case 394: return Species::Prinplup;
		case 395: return Species::Empoleon;
		case 396: return Species::Starly;
		case 397: return Species::Staravia;
		case 398: return Species::Staraptor;
		case 399: return Species::Bidoof;
		case 400: return Species::Bibarel;
		case 401: return Species::Kricketot;
		case 402: return Species::Kricketune;
		case 403: return Species::Shinx;
		case 404: return Species::Luxio;
		case 405: return Species::Luxray;
		case 406: return Species::Budew;
		case 407: return Species::Roserade;
		case 408: return Species::Cranidos;
		case 409: return Species::Rampardos;
		case 410: return Species::Shieldon;
		case 411: return Species::Bastiodon;
		case 412: return Species::Burmy;
		case 413:
			switch (species.forme.value()) {
				case 0: return Species::Wormadam_Plant;
				case 1: return Species::Wormadam_Sandy;
				case 2: return Species::Wormadam_Trash;
				default: throw InvalidFormeID("Wormadam", species.forme);
			}
		case 414: return Species::Mothim;
		case 415: return Species::Combee;
		case 416: return Species::Vespiquen;
		case 417: return Species::Pachirisu;
		case 418: return Species::Buizel;
		case 419: return Species::Floatzel;
		case 420: return Species::Cherubi;
		case 421: return Species::Cherrim;
		case 422: return Species::Shellos;
		case 423: return Species::Gastrodon;
		case 424: return Species::Ambipom;
		case 425: return Species::Drifloon;
		case 426: return Species::Drifblim;
		case 427: return Species::Buneary;
		case 428: return Species::Lopunny;
		case 429: return Species::Mismagius;
		case 430: return Species::Honchkrow;
		case 431: return Species::Glameow;
		case 432: return Species::Purugly;
		case 433: return Species::Chingling;
		case 434: return Species::Stunky;
		case 435: return Species::Skuntank;
		case 436: return Species::Bronzor;
		case 437: return Species::Bronzong;
		case 438: return Species::Bonsly;
		case 439: return Species::Mime_Jr;
		case 440: return Species::Happiny;
		case 441: return Species::Chatot;
		case 442: return Species::Spiritomb;
		case 443: return Species::Gible;
		case 444: return Species::Gabite;
		case 445: return Species::Garchomp;
		case 446: return Species::Munchlax;
		case 447: return Species::Riolu;
		case 448: return Species::Lucario;
		case 449: return Species::Hippopotas;
		case 450: return Species::Hippowdon;
		case 451: return Species::Skorupi;
		case 452: return Species::Drapion;
		case 453: return Species::Croagunk;
		case 454: return Species::Toxicroak;
		case 455: return Species::Carnivine;
		case 456: return Species::Finneon;
		case 457: return Species::Lumineon;
		case 458: return Species::Mantyke;
		case 459: return Species::Snover;
		case 460: return Species::Abomasnow;
		case 461: return Species::Weavile;
		case 462: return Species::Magnezone;
		case 463: return Species::Lickilicky;
		case 464: return Species::Rhyperior;
		case 465: return Species::Tangrowth;
		case 466: return Species::Electivire;
		case 467: return Species::Magmortar;
		case 468: return Species::Togekiss;
		case 469: return Species::Yanmega;
		case 470: return Species::Leafeon;
		case 471: return Species::Glaceon;
		case 472: return Species::Gliscor;
		case 473: return Species::Mamoswine;
		case 474: return Species::Porygon_Z;
		case 475: return Species::Gallade;
		case 476: return Species::Probopass;
		case 477: return Species::Dusknoir;
		case 478: return Species::Froslass;
		case 479:
			static_assert(bounded::max_value<decltype(species.forme)> <= 5_bi);
			switch (species.forme.value()) {
				case 0: return Species::Rotom;
				case 1: return Species::Rotom_Mow;
				case 2: return Species::Rotom_Heat;
				case 3: return Species::Rotom_Frost;
				case 4: return Species::Rotom_Wash;
				case 5: return Species::Rotom_Fan;
				default: bounded::unreachable();
			}
		case 480: return Species::Uxie;
		case 481: return Species::Mesprit;
		case 482: return Species::Azelf;
		case 483: return Species::Dialga;
		case 484: return Species::Palkia;
		case 485: return Species::Heatran;
		case 486: return Species::Regigigas;
		case 487:
			switch (species.forme.value()) {
				case 0: return Species::Giratina_Altered;
				case 1: return Species::Giratina_Origin;
				default: throw InvalidFormeID("Giratina", species.forme);
			}
		case 488: return Species::Cresselia;
		case 489: return Species::Phione;
		case 490: return Species::Manaphy;
		case 491: return Species::Darkrai;
		case 492:
			switch (species.forme.value()) {
				case 0: return Species::Shaymin_Land;
				case 1: return Species::Shaymin_Sky;
				default: throw InvalidFormeID("Shaymin", species.forme);
			}
		case 493: return Species::Arceus;
		
		default: {
			static_assert(bounded::max_value<decltype(species.id)> == 493);
			bounded::unreachable();
		}
	}
}

namespace {

constexpr SpeciesIDs::ID to_id_only(Species const species) {
	switch (species) {
		case Species::Bulbasaur: return 1_bi;
		case Species::Ivysaur: return 2_bi;
		case Species::Venusaur: return 3_bi;
		case Species::Charmander: return 4_bi;
		case Species::Charmeleon: return 5_bi;
		case Species::Charizard: return 6_bi;
		case Species::Squirtle: return 7_bi;
		case Species::Wartortle: return 8_bi;
		case Species::Blastoise: return 9_bi;
		case Species::Caterpie: return 10_bi;
		case Species::Metapod: return 11_bi;
		case Species::Butterfree: return 12_bi;
		case Species::Weedle: return 13_bi;
		case Species::Kakuna: return 14_bi;
		case Species::Beedrill: return 15_bi;
		case Species::Pidgey: return 16_bi;
		case Species::Pidgeotto: return 17_bi;
		case Species::Pidgeot: return 18_bi;
		case Species::Rattata: return 19_bi;
		case Species::Raticate: return 20_bi;
		case Species::Spearow: return 21_bi;
		case Species::Fearow: return 22_bi;
		case Species::Ekans: return 23_bi;
		case Species::Arbok: return 24_bi;
		case Species::Pikachu: return 25_bi;
		case Species::Raichu: return 26_bi;
		case Species::Sandshrew: return 27_bi;
		case Species::Sandslash: return 28_bi;
		case Species::Nidoran_F: return 29_bi;
		case Species::Nidorina: return 30_bi;
		case Species::Nidoqueen: return 31_bi;
		case Species::Nidoran_M: return 32_bi;
		case Species::Nidorino: return 33_bi;
		case Species::Nidoking: return 34_bi;
		case Species::Clefairy: return 35_bi;
		case Species::Clefable: return 36_bi;
		case Species::Vulpix: return 37_bi;
		case Species::Ninetales: return 38_bi;
		case Species::Jigglypuff: return 39_bi;
		case Species::Wigglytuff: return 40_bi;
		case Species::Zubat: return 41_bi;
		case Species::Golbat: return 42_bi;
		case Species::Oddish: return 43_bi;
		case Species::Gloom: return 44_bi;
		case Species::Vileplume: return 45_bi;
		case Species::Paras: return 46_bi;
		case Species::Parasect: return 47_bi;
		case Species::Venonat: return 48_bi;
		case Species::Venomoth: return 49_bi;
		case Species::Diglett: return 50_bi;
		case Species::Dugtrio: return 51_bi;
		case Species::Meowth: return 52_bi;
		case Species::Persian: return 53_bi;
		case Species::Psyduck: return 54_bi;
		case Species::Golduck: return 55_bi;
		case Species::Mankey: return 56_bi;
		case Species::Primeape: return 57_bi;
		case Species::Growlithe: return 58_bi;
		case Species::Arcanine: return 59_bi;
		case Species::Poliwag: return 60_bi;
		case Species::Poliwhirl: return 61_bi;
		case Species::Poliwrath: return 62_bi;
		case Species::Abra: return 63_bi;
		case Species::Kadabra: return 64_bi;
		case Species::Alakazam: return 65_bi;
		case Species::Machop: return 66_bi;
		case Species::Machoke: return 67_bi;
		case Species::Machamp: return 68_bi;
		case Species::Bellsprout: return 69_bi;
		case Species::Weepinbell: return 70_bi;
		case Species::Victreebel: return 71_bi;
		case Species::Tentacool: return 72_bi;
		case Species::Tentacruel: return 73_bi;
		case Species::Geodude: return 74_bi;
		case Species::Graveler: return 75_bi;
		case Species::Golem: return 76_bi;
		case Species::Ponyta: return 77_bi;
		case Species::Rapidash: return 78_bi;
		case Species::Slowpoke: return 79_bi;
		case Species::Slowbro: return 80_bi;
		case Species::Magnemite: return 81_bi;
		case Species::Magneton: return 82_bi;
		case Species::Farfetchd: return 83_bi;
		case Species::Doduo: return 84_bi;
		case Species::Dodrio: return 85_bi;
		case Species::Seel: return 86_bi;
		case Species::Dewgong: return 87_bi;
		case Species::Grimer: return 88_bi;
		case Species::Muk: return 89_bi;
		case Species::Shellder: return 90_bi;
		case Species::Cloyster: return 91_bi;
		case Species::Gastly: return 92_bi;
		case Species::Haunter: return 93_bi;
		case Species::Gengar: return 94_bi;
		case Species::Onix: return 95_bi;
		case Species::Drowzee: return 96_bi;
		case Species::Hypno: return 97_bi;
		case Species::Krabby: return 98_bi;
		case Species::Kingler: return 99_bi;
		case Species::Voltorb: return 100_bi;
		case Species::Electrode: return 101_bi;
		case Species::Exeggcute: return 102_bi;
		case Species::Exeggutor: return 103_bi;
		case Species::Cubone: return 104_bi;
		case Species::Marowak: return 105_bi;
		case Species::Hitmonlee: return 106_bi;
		case Species::Hitmonchan: return 107_bi;
		case Species::Lickitung: return 108_bi;
		case Species::Koffing: return 109_bi;
		case Species::Weezing: return 110_bi;
		case Species::Rhyhorn: return 111_bi;
		case Species::Rhydon: return 112_bi;
		case Species::Chansey: return 113_bi;
		case Species::Tangela: return 114_bi;
		case Species::Kangaskhan: return 115_bi;
		case Species::Horsea: return 116_bi;
		case Species::Seadra: return 117_bi;
		case Species::Goldeen: return 118_bi;
		case Species::Seaking: return 119_bi;
		case Species::Staryu: return 120_bi;
		case Species::Starmie: return 121_bi;
		case Species::Mr_Mime: return 122_bi;
		case Species::Scyther: return 123_bi;
		case Species::Jynx: return 124_bi;
		case Species::Electabuzz: return 125_bi;
		case Species::Magmar: return 126_bi;
		case Species::Pinsir: return 127_bi;
		case Species::Tauros: return 128_bi;
		case Species::Magikarp: return 129_bi;
		case Species::Gyarados: return 130_bi;
		case Species::Lapras: return 131_bi;
		case Species::Ditto: return 132_bi;
		case Species::Eevee: return 133_bi;
		case Species::Vaporeon: return 134_bi;
		case Species::Jolteon: return 135_bi;
		case Species::Flareon: return 136_bi;
		case Species::Porygon: return 137_bi;
		case Species::Omanyte: return 138_bi;
		case Species::Omastar: return 139_bi;
		case Species::Kabuto: return 140_bi;
		case Species::Kabutops: return 141_bi;
		case Species::Aerodactyl: return 142_bi;
		case Species::Snorlax: return 143_bi;
		case Species::Articuno: return 144_bi;
		case Species::Zapdos: return 145_bi;
		case Species::Moltres: return 146_bi;
		case Species::Dratini: return 147_bi;
		case Species::Dragonair: return 148_bi;
		case Species::Dragonite: return 149_bi;
		case Species::Mewtwo: return 150_bi;
		case Species::Mew: return 151_bi;
		case Species::Chikorita: return 152_bi;
		case Species::Bayleef: return 153_bi;
		case Species::Meganium: return 154_bi;
		case Species::Cyndaquil: return 155_bi;
		case Species::Quilava: return 156_bi;
		case Species::Typhlosion: return 157_bi;
		case Species::Totodile: return 158_bi;
		case Species::Croconaw: return 159_bi;
		case Species::Feraligatr: return 160_bi;
		case Species::Sentret: return 161_bi;
		case Species::Furret: return 162_bi;
		case Species::Hoothoot: return 163_bi;
		case Species::Noctowl: return 164_bi;
		case Species::Ledyba: return 165_bi;
		case Species::Ledian: return 166_bi;
		case Species::Spinarak: return 167_bi;
		case Species::Ariados: return 168_bi;
		case Species::Crobat: return 169_bi;
		case Species::Chinchou: return 170_bi;
		case Species::Lanturn: return 171_bi;
		case Species::Pichu: return 172_bi;
		case Species::Cleffa: return 173_bi;
		case Species::Igglybuff: return 174_bi;
		case Species::Togepi: return 175_bi;
		case Species::Togetic: return 176_bi;
		case Species::Natu: return 177_bi;
		case Species::Xatu: return 178_bi;
		case Species::Mareep: return 179_bi;
		case Species::Flaaffy: return 180_bi;
		case Species::Ampharos: return 181_bi;
		case Species::Bellossom: return 182_bi;
		case Species::Marill: return 183_bi;
		case Species::Azumarill: return 184_bi;
		case Species::Sudowoodo: return 185_bi;
		case Species::Politoed: return 186_bi;
		case Species::Hoppip: return 187_bi;
		case Species::Skiploom: return 188_bi;
		case Species::Jumpluff: return 189_bi;
		case Species::Aipom: return 190_bi;
		case Species::Sunkern: return 191_bi;
		case Species::Sunflora: return 192_bi;
		case Species::Yanma: return 193_bi;
		case Species::Wooper: return 194_bi;
		case Species::Quagsire: return 195_bi;
		case Species::Espeon: return 196_bi;
		case Species::Umbreon: return 197_bi;
		case Species::Murkrow: return 198_bi;
		case Species::Slowking: return 199_bi;
		case Species::Misdreavus: return 200_bi;
		case Species::Unown: return 201_bi;
		case Species::Wobbuffet: return 202_bi;
		case Species::Girafarig: return 203_bi;
		case Species::Pineco: return 204_bi;
		case Species::Forretress: return 205_bi;
		case Species::Dunsparce: return 206_bi;
		case Species::Gligar: return 207_bi;
		case Species::Steelix: return 208_bi;
		case Species::Snubbull: return 209_bi;
		case Species::Granbull: return 210_bi;
		case Species::Qwilfish: return 211_bi;
		case Species::Scizor: return 212_bi;
		case Species::Shuckle: return 213_bi;
		case Species::Heracross: return 214_bi;
		case Species::Sneasel: return 215_bi;
		case Species::Teddiursa: return 216_bi;
		case Species::Ursaring: return 217_bi;
		case Species::Slugma: return 218_bi;
		case Species::Magcargo: return 219_bi;
		case Species::Swinub: return 220_bi;
		case Species::Piloswine: return 221_bi;
		case Species::Corsola: return 222_bi;
		case Species::Remoraid: return 223_bi;
		case Species::Octillery: return 224_bi;
		case Species::Delibird: return 225_bi;
		case Species::Mantine: return 226_bi;
		case Species::Skarmory: return 227_bi;
		case Species::Houndour: return 228_bi;
		case Species::Houndoom: return 229_bi;
		case Species::Kingdra: return 230_bi;
		case Species::Phanpy: return 231_bi;
		case Species::Donphan: return 232_bi;
		case Species::Porygon2: return 233_bi;
		case Species::Stantler: return 234_bi;
		case Species::Smeargle: return 235_bi;
		case Species::Tyrogue: return 236_bi;
		case Species::Hitmontop: return 237_bi;
		case Species::Smoochum: return 238_bi;
		case Species::Elekid: return 239_bi;
		case Species::Magby: return 240_bi;
		case Species::Miltank: return 241_bi;
		case Species::Blissey: return 242_bi;
		case Species::Raikou: return 243_bi;
		case Species::Entei: return 244_bi;
		case Species::Suicune: return 245_bi;
		case Species::Larvitar: return 246_bi;
		case Species::Pupitar: return 247_bi;
		case Species::Tyranitar: return 248_bi;
		case Species::Lugia: return 249_bi;
		case Species::Ho_Oh: return 250_bi;
		case Species::Celebi: return 251_bi;
		case Species::Treecko: return 252_bi;
		case Species::Grovyle: return 253_bi;
		case Species::Sceptile: return 254_bi;
		case Species::Torchic: return 255_bi;
		case Species::Combusken: return 256_bi;
		case Species::Blaziken: return 257_bi;
		case Species::Mudkip: return 258_bi;
		case Species::Marshtomp: return 259_bi;
		case Species::Swampert: return 260_bi;
		case Species::Poochyena: return 261_bi;
		case Species::Mightyena: return 262_bi;
		case Species::Zigzagoon: return 263_bi;
		case Species::Linoone: return 264_bi;
		case Species::Wurmple: return 265_bi;
		case Species::Silcoon: return 266_bi;
		case Species::Beautifly: return 267_bi;
		case Species::Cascoon: return 268_bi;
		case Species::Dustox: return 269_bi;
		case Species::Lotad: return 270_bi;
		case Species::Lombre: return 271_bi;
		case Species::Ludicolo: return 272_bi;
		case Species::Seedot: return 273_bi;
		case Species::Nuzleaf: return 274_bi;
		case Species::Shiftry: return 275_bi;
		case Species::Taillow: return 276_bi;
		case Species::Swellow: return 277_bi;
		case Species::Wingull: return 278_bi;
		case Species::Pelipper: return 279_bi;
		case Species::Ralts: return 280_bi;
		case Species::Kirlia: return 281_bi;
		case Species::Gardevoir: return 282_bi;
		case Species::Surskit: return 283_bi;
		case Species::Masquerain: return 284_bi;
		case Species::Shroomish: return 285_bi;
		case Species::Breloom: return 286_bi;
		case Species::Slakoth: return 287_bi;
		case Species::Vigoroth: return 288_bi;
		case Species::Slaking: return 289_bi;
		case Species::Nincada: return 290_bi;
		case Species::Ninjask: return 291_bi;
		case Species::Shedinja: return 292_bi;
		case Species::Whismur: return 293_bi;
		case Species::Loudred: return 294_bi;
		case Species::Exploud: return 295_bi;
		case Species::Makuhita: return 296_bi;
		case Species::Hariyama: return 297_bi;
		case Species::Azurill: return 298_bi;
		case Species::Nosepass: return 299_bi;
		case Species::Skitty: return 300_bi;
		case Species::Delcatty: return 301_bi;
		case Species::Sableye: return 302_bi;
		case Species::Mawile: return 303_bi;
		case Species::Aron: return 304_bi;
		case Species::Lairon: return 305_bi;
		case Species::Aggron: return 306_bi;
		case Species::Meditite: return 307_bi;
		case Species::Medicham: return 308_bi;
		case Species::Electrike: return 309_bi;
		case Species::Manectric: return 310_bi;
		case Species::Plusle: return 311_bi;
		case Species::Minun: return 312_bi;
		case Species::Volbeat: return 313_bi;
		case Species::Illumise: return 314_bi;
		case Species::Roselia: return 315_bi;
		case Species::Gulpin: return 316_bi;
		case Species::Swalot: return 317_bi;
		case Species::Carvanha: return 318_bi;
		case Species::Sharpedo: return 319_bi;
		case Species::Wailmer: return 320_bi;
		case Species::Wailord: return 321_bi;
		case Species::Numel: return 322_bi;
		case Species::Camerupt: return 323_bi;
		case Species::Torkoal: return 324_bi;
		case Species::Spoink: return 325_bi;
		case Species::Grumpig: return 326_bi;
		case Species::Spinda: return 327_bi;
		case Species::Trapinch: return 328_bi;
		case Species::Vibrava: return 329_bi;
		case Species::Flygon: return 330_bi;
		case Species::Cacnea: return 331_bi;
		case Species::Cacturne: return 332_bi;
		case Species::Swablu: return 333_bi;
		case Species::Altaria: return 334_bi;
		case Species::Zangoose: return 335_bi;
		case Species::Seviper: return 336_bi;
		case Species::Lunatone: return 337_bi;
		case Species::Solrock: return 338_bi;
		case Species::Barboach: return 339_bi;
		case Species::Whiscash: return 340_bi;
		case Species::Corphish: return 341_bi;
		case Species::Crawdaunt: return 342_bi;
		case Species::Baltoy: return 343_bi;
		case Species::Claydol: return 344_bi;
		case Species::Lileep: return 345_bi;
		case Species::Cradily: return 346_bi;
		case Species::Anorith: return 347_bi;
		case Species::Armaldo: return 348_bi;
		case Species::Feebas: return 349_bi;
		case Species::Milotic: return 350_bi;
		case Species::Castform: return 351_bi;
		case Species::Kecleon: return 352_bi;
		case Species::Shuppet: return 353_bi;
		case Species::Banette: return 354_bi;
		case Species::Duskull: return 355_bi;
		case Species::Dusclops: return 356_bi;
		case Species::Tropius: return 357_bi;
		case Species::Chimecho: return 358_bi;
		case Species::Absol: return 359_bi;
		case Species::Wynaut: return 360_bi;
		case Species::Snorunt: return 361_bi;
		case Species::Glalie: return 362_bi;
		case Species::Spheal: return 363_bi;
		case Species::Sealeo: return 364_bi;
		case Species::Walrein: return 365_bi;
		case Species::Clamperl: return 366_bi;
		case Species::Huntail: return 367_bi;
		case Species::Gorebyss: return 368_bi;
		case Species::Relicanth: return 369_bi;
		case Species::Luvdisc: return 370_bi;
		case Species::Bagon: return 371_bi;
		case Species::Shelgon: return 372_bi;
		case Species::Salamence: return 373_bi;
		case Species::Beldum: return 374_bi;
		case Species::Metang: return 375_bi;
		case Species::Metagross: return 376_bi;
		case Species::Regirock: return 377_bi;
		case Species::Regice: return 378_bi;
		case Species::Registeel: return 379_bi;
		case Species::Latias: return 380_bi;
		case Species::Latios: return 381_bi;
		case Species::Kyogre: return 382_bi;
		case Species::Groudon: return 383_bi;
		case Species::Rayquaza: return 384_bi;
		case Species::Jirachi: return 385_bi;
		case Species::Deoxys_Normal: return 386_bi;
		case Species::Deoxys_Attack: return 386_bi;
		case Species::Deoxys_Defense: return 386_bi;
		case Species::Deoxys_Speed: return 386_bi;
		case Species::Turtwig: return 387_bi;
		case Species::Grotle: return 388_bi;
		case Species::Torterra: return 389_bi;
		case Species::Chimchar: return 390_bi;
		case Species::Monferno: return 391_bi;
		case Species::Infernape: return 392_bi;
		case Species::Piplup: return 393_bi;
		case Species::Prinplup: return 394_bi;
		case Species::Empoleon: return 395_bi;
		case Species::Starly: return 396_bi;
		case Species::Staravia: return 397_bi;
		case Species::Staraptor: return 398_bi;
		case Species::Bidoof: return 399_bi;
		case Species::Bibarel: return 400_bi;
		case Species::Kricketot: return 401_bi;
		case Species::Kricketune: return 402_bi;
		case Species::Shinx: return 403_bi;
		case Species::Luxio: return 404_bi;
		case Species::Luxray: return 405_bi;
		case Species::Budew: return 406_bi;
		case Species::Roserade: return 407_bi;
		case Species::Cranidos: return 408_bi;
		case Species::Rampardos: return 409_bi;
		case Species::Shieldon: return 410_bi;
		case Species::Bastiodon: return 411_bi;
		case Species::Burmy: return 412_bi;
		case Species::Wormadam_Plant: return 413_bi;
		case Species::Wormadam_Sandy: return 413_bi;
		case Species::Wormadam_Trash: return 413_bi;
		case Species::Mothim: return 414_bi;
		case Species::Combee: return 415_bi;
		case Species::Vespiquen: return 416_bi;
		case Species::Pachirisu: return 417_bi;
		case Species::Buizel: return 418_bi;
		case Species::Floatzel: return 419_bi;
		case Species::Cherubi: return 420_bi;
		case Species::Cherrim: return 421_bi;
		case Species::Shellos: return 422_bi;
		case Species::Gastrodon: return 423_bi;
		case Species::Ambipom: return 424_bi;
		case Species::Drifloon: return 425_bi;
		case Species::Drifblim: return 426_bi;
		case Species::Buneary: return 427_bi;
		case Species::Lopunny: return 428_bi;
		case Species::Mismagius: return 429_bi;
		case Species::Honchkrow: return 430_bi;
		case Species::Glameow: return 431_bi;
		case Species::Purugly: return 432_bi;
		case Species::Chingling: return 433_bi;
		case Species::Stunky: return 434_bi;
		case Species::Skuntank: return 435_bi;
		case Species::Bronzor: return 436_bi;
		case Species::Bronzong: return 437_bi;
		case Species::Bonsly: return 438_bi;
		case Species::Mime_Jr: return 439_bi;
		case Species::Happiny: return 440_bi;
		case Species::Chatot: return 441_bi;
		case Species::Spiritomb: return 442_bi;
		case Species::Gible: return 443_bi;
		case Species::Gabite: return 444_bi;
		case Species::Garchomp: return 445_bi;
		case Species::Munchlax: return 446_bi;
		case Species::Riolu: return 447_bi;
		case Species::Lucario: return 448_bi;
		case Species::Hippopotas: return 449_bi;
		case Species::Hippowdon: return 450_bi;
		case Species::Skorupi: return 451_bi;
		case Species::Drapion: return 452_bi;
		case Species::Croagunk: return 453_bi;
		case Species::Toxicroak: return 454_bi;
		case Species::Carnivine: return 455_bi;
		case Species::Finneon: return 456_bi;
		case Species::Lumineon: return 457_bi;
		case Species::Mantyke: return 458_bi;
		case Species::Snover: return 459_bi;
		case Species::Abomasnow: return 460_bi;
		case Species::Weavile: return 461_bi;
		case Species::Magnezone: return 462_bi;
		case Species::Lickilicky: return 463_bi;
		case Species::Rhyperior: return 464_bi;
		case Species::Tangrowth: return 465_bi;
		case Species::Electivire: return 466_bi;
		case Species::Magmortar: return 467_bi;
		case Species::Togekiss: return 468_bi;
		case Species::Yanmega: return 469_bi;
		case Species::Leafeon: return 470_bi;
		case Species::Glaceon: return 471_bi;
		case Species::Gliscor: return 472_bi;
		case Species::Mamoswine: return 473_bi;
		case Species::Porygon_Z: return 474_bi;
		case Species::Gallade: return 475_bi;
		case Species::Probopass: return 476_bi;
		case Species::Dusknoir: return 477_bi;
		case Species::Froslass: return 478_bi;
		case Species::Rotom: return 479_bi;
		case Species::Rotom_Heat: return 479_bi;
		case Species::Rotom_Wash: return 479_bi;
		case Species::Rotom_Frost: return 479_bi;
		case Species::Rotom_Fan: return 479_bi;
		case Species::Rotom_Mow: return 479_bi;
		case Species::Uxie: return 480_bi;
		case Species::Mesprit: return 481_bi;
		case Species::Azelf: return 482_bi;
		case Species::Dialga: return 483_bi;
		case Species::Palkia: return 484_bi;
		case Species::Heatran: return 485_bi;
		case Species::Regigigas: return 486_bi;
		case Species::Giratina_Altered: return 487_bi;
		case Species::Giratina_Origin: return 487_bi;
		case Species::Cresselia: return 488_bi;
		case Species::Phione: return 489_bi;
		case Species::Manaphy: return 490_bi;
		case Species::Darkrai: return 491_bi;
		case Species::Shaymin_Land: return 492_bi;
		case Species::Shaymin_Sky: return 492_bi;
		case Species::Arceus: return 493_bi;
		default: throw UnsupportedSpecies(species);
	}
}

constexpr SpeciesIDs::Forme to_forme(Species const species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Giratina_Origin:
		case Species::Rotom_Mow:
		case Species::Shaymin_Sky:
		case Species::Wormadam_Sandy:
			return 1_bi;
		case Species::Deoxys_Defense:
		case Species::Rotom_Heat:
		case Species::Wormadam_Trash:
			return 2_bi;
		case Species::Deoxys_Speed:
		case Species::Rotom_Frost:
			return 3_bi;
		case Species::Rotom_Wash:
			return 4_bi;
		case Species::Rotom_Fan:
			return 5_bi;
		default:
			return 0_bi;
	}
}

}	// namespace

SpeciesIDs species_to_id(Species const species) {
	return { to_id_only(species), to_forme(species) };
}

Ability id_to_ability(AbilityID const id) {
	switch (id.value()) {
		case 1: return Ability::Stench;
		case 2: return Ability::Drizzle;
		case 3: return Ability::Speed_Boost;
		case 4: return Ability::Battle_Armor;
		case 5: return Ability::Sturdy;
		case 6: return Ability::Damp;
		case 7: return Ability::Limber;
		case 8: return Ability::Sand_Veil;
		case 9: return Ability::Static;
		case 10: return Ability::Volt_Absorb;
		case 11: return Ability::Water_Absorb;
		case 12: return Ability::Oblivious;
		case 13: return Ability::Cloud_Nine;
		case 14: return Ability::Compound_Eyes;
		case 15: return Ability::Insomnia;
		case 16: return Ability::Color_Change;
		case 17: return Ability::Immunity;
		case 18: return Ability::Flash_Fire;
		case 19: return Ability::Shield_Dust;
		case 20: return Ability::Own_Tempo;
		case 21: return Ability::Suction_Cups;
		case 22: return Ability::Intimidate;
		case 23: return Ability::Shadow_Tag;
		case 24: return Ability::Rough_Skin;
		case 25: return Ability::Wonder_Guard;
		case 26: return Ability::Levitate;
		case 27: return Ability::Effect_Spore;
		case 28: return Ability::Synchronize;
		case 29: return Ability::Clear_Body;
		case 30: return Ability::Natural_Cure;
		case 31: return Ability::Lightning_Rod;
		case 32: return Ability::Serene_Grace;
		case 33: return Ability::Swift_Swim;
		case 34: return Ability::Chlorophyll;
		case 35: return Ability::Illuminate;
		case 36: return Ability::Trace;
		case 37: return Ability::Huge_Power;
		case 38: return Ability::Poison_Point;
		case 39: return Ability::Inner_Focus;
		case 40: return Ability::Magma_Armor;
		case 41: return Ability::Water_Veil;
		case 42: return Ability::Magnet_Pull;
		case 43: return Ability::Soundproof;
		case 44: return Ability::Rain_Dish;
		case 45: return Ability::Sand_Stream;
		case 46: return Ability::Pressure;
		case 47: return Ability::Thick_Fat;
		case 48: return Ability::Early_Bird;
		case 49: return Ability::Flame_Body;
		case 50: return Ability::Run_Away;
		case 51: return Ability::Keen_Eye;
		case 52: return Ability::Hyper_Cutter;
		case 53: return Ability::Pickup;
		case 54: return Ability::Truant;
		case 55: return Ability::Hustle;
		case 56: return Ability::Cute_Charm;
		case 57: return Ability::Plus;
		case 58: return Ability::Minus;
		case 59: return Ability::Forecast;
		case 60: return Ability::Sticky_Hold;
		case 61: return Ability::Shed_Skin;
		case 62: return Ability::Guts;
		case 63: return Ability::Marvel_Scale;
		case 64: return Ability::Liquid_Ooze;
		case 65: return Ability::Overgrow;
		case 66: return Ability::Blaze;
		case 67: return Ability::Torrent;
		case 68: return Ability::Swarm;
		case 69: return Ability::Rock_Head;
		case 70: return Ability::Drought;
		case 71: return Ability::Arena_Trap;
		case 72: return Ability::Vital_Spirit;
		case 73: return Ability::White_Smoke;
		case 74: return Ability::Pure_Power;
		case 75: return Ability::Shell_Armor;
		case 76: return Ability::Air_Lock;
		case 77: return Ability::Tangled_Feet;
		case 78: return Ability::Motor_Drive;
		case 79: return Ability::Rivalry;
		case 80: return Ability::Steadfast;
		case 81: return Ability::Snow_Cloak;
		case 82: return Ability::Gluttony;
		case 83: return Ability::Anger_Point;
		case 84: return Ability::Unburden;
		case 85: return Ability::Heatproof;
		case 86: return Ability::Simple;
		case 87: return Ability::Dry_Skin;
		case 88: return Ability::Download;
		case 89: return Ability::Iron_Fist;
		case 90: return Ability::Poison_Heal;
		case 91: return Ability::Adaptability;
		case 92: return Ability::Skill_Link;
		case 93: return Ability::Hydration;
		case 94: return Ability::Solar_Power;
		case 95: return Ability::Quick_Feet;
		case 96: return Ability::Normalize;
		case 97: return Ability::Sniper;
		case 98: return Ability::Magic_Guard;
		case 99: return Ability::No_Guard;
		case 100: return Ability::Stall;
		case 101: return Ability::Technician;
		case 102: return Ability::Leaf_Guard;
		case 103: return Ability::Klutz;
		case 104: return Ability::Mold_Breaker;
		case 105: return Ability::Super_Luck;
		case 106: return Ability::Aftermath;
		case 107: return Ability::Anticipation;
		case 108: return Ability::Forewarn;
		case 109: return Ability::Unaware;
		case 110: return Ability::Tinted_Lens;
		case 111: return Ability::Filter;
		case 112: return Ability::Slow_Start;
		case 113: return Ability::Scrappy;
		case 114: return Ability::Storm_Drain;
		case 115: return Ability::Ice_Body;
		case 116: return Ability::Solid_Rock;
		case 117: return Ability::Snow_Warning;
		case 118: return Ability::Honey_Gather;
		case 119: return Ability::Frisk;
		case 120: return Ability::Reckless;
		case 121: return Ability::Multitype;
		case 122: return Ability::Flower_Gift;
		case 123: return Ability::Bad_Dreams;
		default: {
			static_assert(bounded::max_value<decltype(id)> == 123);
			bounded::unreachable();
		}
	}
}

AbilityID ability_to_id(Ability const ability) {
	switch (ability) {
		case Ability::Adaptability: return 91_bi;
		case Ability::Aftermath: return 106_bi;
		case Ability::Air_Lock: return 76_bi;
		case Ability::Anger_Point: return 83_bi;
		case Ability::Anticipation: return 107_bi;
		case Ability::Arena_Trap: return 71_bi;
		case Ability::Bad_Dreams: return 123_bi;
		case Ability::Battle_Armor: return 4_bi;
		case Ability::Blaze: return 66_bi;
		case Ability::Chlorophyll: return 34_bi;
		case Ability::Clear_Body: return 29_bi;
		case Ability::Cloud_Nine: return 13_bi;
		case Ability::Color_Change: return 16_bi;
		case Ability::Compound_Eyes: return 14_bi;
		case Ability::Cute_Charm: return 56_bi;
		case Ability::Damp: return 6_bi;
		case Ability::Download: return 88_bi;
		case Ability::Drizzle: return 2_bi;
		case Ability::Drought: return 70_bi;
		case Ability::Dry_Skin: return 87_bi;
		case Ability::Early_Bird: return 48_bi;
		case Ability::Effect_Spore: return 27_bi;
		case Ability::Filter: return 111_bi;
		case Ability::Flame_Body: return 49_bi;
		case Ability::Flash_Fire: return 18_bi;
		case Ability::Flower_Gift: return 122_bi;
		case Ability::Forecast: return 59_bi;
		case Ability::Forewarn: return 108_bi;
		case Ability::Frisk: return 119_bi;
		case Ability::Gluttony: return 82_bi;
		case Ability::Guts: return 62_bi;
		case Ability::Heatproof: return 85_bi;
		case Ability::Honey_Gather: return 118_bi;
		case Ability::Huge_Power: return 37_bi;
		case Ability::Hustle: return 55_bi;
		case Ability::Hydration: return 93_bi;
		case Ability::Hyper_Cutter: return 52_bi;
		case Ability::Ice_Body: return 115_bi;
		case Ability::Illuminate: return 35_bi;
		case Ability::Immunity: return 17_bi;
		case Ability::Inner_Focus: return 39_bi;
		case Ability::Insomnia: return 15_bi;
		case Ability::Intimidate: return 22_bi;
		case Ability::Iron_Fist: return 89_bi;
		case Ability::Keen_Eye: return 51_bi;
		case Ability::Klutz: return 103_bi;
		case Ability::Leaf_Guard: return 102_bi;
		case Ability::Levitate: return 26_bi;
		case Ability::Lightning_Rod: return 31_bi;
		case Ability::Limber: return 7_bi;
		case Ability::Liquid_Ooze: return 64_bi;
		case Ability::Magic_Guard: return 98_bi;
		case Ability::Magma_Armor: return 40_bi;
		case Ability::Magnet_Pull: return 42_bi;
		case Ability::Marvel_Scale: return 63_bi;
		case Ability::Minus: return 58_bi;
		case Ability::Mold_Breaker: return 104_bi;
		case Ability::Motor_Drive: return 78_bi;
		case Ability::Multitype: return 121_bi;
		case Ability::Natural_Cure: return 30_bi;
		case Ability::No_Guard: return 99_bi;
		case Ability::Normalize: return 96_bi;
		case Ability::Oblivious: return 12_bi;
		case Ability::Overgrow: return 65_bi;
		case Ability::Own_Tempo: return 20_bi;
		case Ability::Pickup: return 53_bi;
		case Ability::Plus: return 57_bi;
		case Ability::Poison_Heal: return 90_bi;
		case Ability::Poison_Point: return 38_bi;
		case Ability::Pressure: return 46_bi;
		case Ability::Pure_Power: return 74_bi;
		case Ability::Quick_Feet: return 95_bi;
		case Ability::Rain_Dish: return 44_bi;
		case Ability::Reckless: return 120_bi;
		case Ability::Rivalry: return 79_bi;
		case Ability::Rock_Head: return 69_bi;
		case Ability::Rough_Skin: return 24_bi;
		case Ability::Run_Away: return 50_bi;
		case Ability::Sand_Stream: return 45_bi;
		case Ability::Sand_Veil: return 8_bi;
		case Ability::Scrappy: return 113_bi;
		case Ability::Serene_Grace: return 32_bi;
		case Ability::Shadow_Tag: return 23_bi;
		case Ability::Shed_Skin: return 61_bi;
		case Ability::Shell_Armor: return 75_bi;
		case Ability::Shield_Dust: return 19_bi;
		case Ability::Simple: return 86_bi;
		case Ability::Skill_Link: return 92_bi;
		case Ability::Slow_Start: return 112_bi;
		case Ability::Sniper: return 97_bi;
		case Ability::Snow_Cloak: return 81_bi;
		case Ability::Snow_Warning: return 117_bi;
		case Ability::Solar_Power: return 94_bi;
		case Ability::Solid_Rock: return 116_bi;
		case Ability::Soundproof: return 43_bi;
		case Ability::Speed_Boost: return 3_bi;
		case Ability::Stall: return 100_bi;
		case Ability::Static: return 9_bi;
		case Ability::Steadfast: return 80_bi;
		case Ability::Stench: return 1_bi;
		case Ability::Sticky_Hold: return 60_bi;
		case Ability::Storm_Drain: return 114_bi;
		case Ability::Sturdy: return 5_bi;
		case Ability::Suction_Cups: return 21_bi;
		case Ability::Super_Luck: return 105_bi;
		case Ability::Swarm: return 68_bi;
		case Ability::Swift_Swim: return 33_bi;
		case Ability::Synchronize: return 28_bi;
		case Ability::Tangled_Feet: return 77_bi;
		case Ability::Technician: return 101_bi;
		case Ability::Thick_Fat: return 47_bi;
		case Ability::Tinted_Lens: return 110_bi;
		case Ability::Torrent: return 67_bi;
		case Ability::Trace: return 36_bi;
		case Ability::Truant: return 54_bi;
		case Ability::Unaware: return 109_bi;
		case Ability::Unburden: return 84_bi;
		case Ability::Vital_Spirit: return 72_bi;
		case Ability::Volt_Absorb: return 10_bi;
		case Ability::Water_Absorb: return 11_bi;
		case Ability::Water_Veil: return 41_bi;
		case Ability::White_Smoke: return 73_bi;
		case Ability::Wonder_Guard: return 25_bi;
		default: throw std::runtime_error(std::string(to_string(ability)) + " not supported in Pokemon Online");
	}
}

struct InvalidPart : std::runtime_error {
	InvalidPart(std::string const & type, uint16_t id, uint8_t part):
		std::runtime_error("Invalid conversion to " + type + " ID: " + std::to_string(static_cast<unsigned>(id)) + " with part ID: " + std::to_string(static_cast <unsigned>(part)) + ".\n") {
	}
};

Ability battle_id_to_ability (uint16_t id, uint8_t part) {
	// 6: BAD_DREAMS
	// 18: %s's %a activates!
	// 21: %s changed its type to %t!
	// 30: %s's %a prevents its stat from being lowered!
	// 31: %s's %a prevented its stats from being lowered!
	// 32: %s's %a heals it!
	// 33: %s's %a cures it!|%s didn't get paralyzed because of its %a!|%s stayed awake because of its %a!|%s didn't get frozen because of its %a!|%s didn't get burnt because of its %a!|%s didn't get poisoned because of its %a!
	// 38: %s's %a took the attack!|%s's %a raised its special attack!|%s's %a made the attack useless!
	// 40: %s has %a!
	// 50: %s's %a hurts %f
	// 68: %s's %a raised its attack!|%s's %a made the attack useless!
	// 70: %s's %a absorbs the attack!|%s's %a made the attack useless!
	switch (id) {
		case 2: return Ability::Aftermath;
		case 3: return Ability::Anger_Point;
		case 4: return Ability::Anticipation;
		case 6: return Ability::Bad_Dreams;
		case 9: return Ability::Color_Change;
		case 11: return Ability::Cute_Charm;
		case 12: return Ability::Inner_Focus;
		case 13: return Ability::Download;
		case 14: switch (part) {
			case 0: return Ability::Snow_Warning;
			case 1: return Ability::Drizzle;
			case 2: return Ability::Sand_Stream;
			case 3: return Ability::Drought;
			default: throw InvalidPart("ability", id, part);
		}
		case 15: return Ability::Dry_Skin;
		case 16: return Ability::Effect_Spore;
		case 19: return Ability::Flash_Fire;
		case 22: return Ability::Forewarn;
		case 23: return Ability::Frisk;
		case 24: return Ability::Shield_Dust;
		case 29: return Ability::Hydration;
		case 34: return Ability::Intimidate;
		case 37: return Ability::Leaf_Guard;
		case 41: return Ability::Motor_Drive;
		case 44: return Ability::Own_Tempo;
		case 45: return Ability::Poison_Heal;
		case 46: return Ability::Pressure;
		case 54: return Ability::Shed_Skin;
		case 55: return Ability::Slow_Start;
		case 56: return Ability::Solar_Power;
		case 57: return Ability::Soundproof;
		case 58: return Ability::Speed_Boost;
		case 60: return Ability::Steadfast;
		case 61: return Ability::Synchronize;
		case 66: return Ability::Trace;
		case 67: return Ability::Truant;
		case 71: return Ability::Wonder_Guard;
		default: throw std::runtime_error("Invalid ability ID: " + std::to_string(static_cast<int>(id)));
	}
}

Gender id_to_gender(GenderID const id) {
	switch (id.value()) {
		case 0: return Gender::genderless;
		case 1: return Gender::male;
		case 2: return Gender::female;
		default: {
			static_assert(bounded::max_value<decltype(id)> == 2);
			bounded::unreachable();
		}
	}
}

GenderID gender_to_id(Gender const gender) {
	switch (gender) {
		case Gender::genderless: return 0_bi;
		case Gender::male: return 1_bi;
		case Gender::female: return 2_bi;
	}
}

Item id_to_item(ItemID const id) {
	switch (id.value()) {
		case 0: return Item::None;
		case 1: return Item::Big_Root;
		case 2: return Item::Blue_Scarf;
		case 3: return Item::Bright_Powder;
		case 4: return Item::Choice_Band;
		case 5: return Item::Choice_Scarf;
		case 6: return Item::Choice_Specs;
		case 7: return Item::Destiny_Knot;
		case 8: return Item::Expert_Belt;
		case 9: return Item::Focus_Band;
		case 10: return Item::Focus_Sash;
		case 11: return Item::Full_Incense;
		case 12: return Item::Green_Scarf;
		case 13: return Item::Lagging_Tail;
		case 14: return Item::Lax_Incense;
		case 15: return Item::Leftovers;
		case 16: return Item::Luck_Incense;
		case 17: return Item::Mental_Herb;
		case 18: return Item::Metal_Powder;
		case 19: return Item::Muscle_Band;
		case 20: return Item::Odd_Incense;
		case 21: return Item::Pink_Scarf;
		case 22: return Item::Power_Herb;
		case 23: return Item::Pure_Incense;
		case 24: return Item::Quick_Powder;
		case 25: return Item::Reaper_Cloth;
		case 26: return Item::Red_Scarf;
		case 27: return Item::Rock_Incense;
		case 28: return Item::Rose_Incense;
		case 29: return Item::Sea_Incense;
		case 30: return Item::Shed_Shell;
		case 31: return Item::Silk_Scarf;
		case 32: return Item::Silver_Powder;
		case 33: return Item::Smooth_Rock;
		case 34: return Item::Soft_Sand;
		case 35: return Item::Soothe_Bell;
		case 36: return Item::Wave_Incense;
		case 37: return Item::White_Herb;
		case 38: return Item::Wide_Lens;
		case 39: return Item::Wise_Glasses;
		case 40: return Item::Yellow_Scarf;
		case 41: return Item::Zoom_Lens;
		case 42: return Item::Amulet_Coin;
		case 43: return Item::Antidote;
		case 44: return Item::Awakening;
		case 45: return Item::Berry_Juice;
		case 46: return Item::Big_Pearl;
		case 47: return Item::Big_Mushroom;
		case 48: return Item::Black_Belt;
		case 49: return Item::Black_Flute;
		case 50: return Item::Black_Sludge;
		case 51: return Item::Black_Glasses;
		case 52: return Item::Blue_Flute;
		case 53: return Item::Blue_Shard;
		case 54: return Item::Burn_Heal;
		case 55: return Item::Calcium;
		case 56: return Item::Carbos;
		case 57: return Item::Charcoal;
		case 58: return Item::Cleanse_Tag;
		case 59: return Item::Damp_Mulch;
		case 60: return Item::Deep_Sea_Scale;
		case 61: return Item::Dire_Hit;
		case 62: return Item::Dragon_Scale;
		case 63: return Item::Elixir;
		case 64: return Item::Energy_Root;
		case 65: return Item::Energy_Powder;
		case 66: return Item::Escape_Rope;
		case 67: return Item::Ether;
		case 68: return Item::Everstone;
		case 69: return Item::Exp_Share;
		case 70: return Item::Fire_Stone;
		case 71: return Item::Flame_Orb;
		case 72: return Item::Fluffy_Tail;
		case 73: return Item::Fresh_Water;
		case 74: return Item::Full_Heal;
		case 75: return Item::Full_Restore;
		case 76: return Item::Gooey_Mulch;
		case 77: return Item::Green_Shard;
		case 78: return Item::Growth_Mulch;
		case 79: return Item::Guard_Spec;
		case 80: return Item::Heal_Powder;
		case 81: return Item::Heart_Scale;
		case 82: return Item::Honey;
		case 83: return Item::HP_Up;
		case 84: return Item::Hyper_Potion;
		case 85: return Item::Ice_Heal;
		case 86: return Item::Iron;
		case 87: return Item::Kings_Rock;
		case 88: return Item::Lava_Cookie;
		case 89: return Item::Leaf_Stone;
		case 90: return Item::Lemonade;
		case 91: return Item::Life_Orb;
		case 92: return Item::Light_Ball;
		case 93: return Item::Light_Clay;
		case 94: return Item::Lucky_Egg;
		case 95: return Item::Magnet;
		case 96: return Item::Max_Elixir;
		case 97: return Item::Max_Ether;
		case 98: return Item::Max_Potion;
		case 99: return Item::Max_Repel;
		case 100: return Item::Max_Revive;
		case 101: return Item::Metal_Coat;
		case 102: return Item::Metronome;
		case 103: return Item::Miracle_Seed;
		case 104: return Item::Moomoo_Milk;
		case 105: return Item::Moon_Stone;
		case 106: return Item::Mystic_Water;
		case 107: return Item::Never_Melt_Ice;
		case 108: return Item::Nugget;
		case 109: return Item::Old_Gateau;
		case 110: return Item::Paralyze_Heal;
		case 111: return Item::Pearl;
		case 112: return Item::Poke_Doll;
		case 113: return Item::Potion;
		case 114: return Item::PP_Max;
		case 115: return Item::PP_Up;
		case 116: return Item::Protein;
		case 117: return Item::Rare_Candy;
		case 118: return Item::Razor_Fang;
		case 119: return Item::Red_Flute;
		case 120: return Item::Red_Shard;
		case 121: return Item::Repel;
		case 122: return Item::Revival_Herb;
		case 123: return Item::Revive;
		case 124: return Item::Sacred_Ash;
		case 125: return Item::Scope_Lens;
		case 126: return Item::Shell_Bell;
		case 127: return Item::Shoal_Salt;
		case 128: return Item::Shoal_Shell;
		case 129: return Item::Smoke_Ball;
		case 130: return Item::Soda_Pop;
		case 131: return Item::Soul_Dew;
		case 132: return Item::Spell_Tag;
		case 133: return Item::Stable_Mulch;
		case 134: return Item::Star_Piece;
		case 135: return Item::Stardust;
		case 136: return Item::Sun_Stone;
		case 137: return Item::Super_Potion;
		case 138: return Item::Super_Repel;
		case 139: return Item::Thunder_Stone;
		case 140: return Item::Tiny_Mushroom;
		case 141: return Item::Toxic_Orb;
		case 142: return Item::Twisted_Spoon;
		case 143: return Item::Up_Grade;
		case 144: return Item::Water_Stone;
		case 145: return Item::White_Flute;
		case 146: return Item::X_Accuracy;
		case 147: return Item::X_Attack;
		case 148: return Item::X_Defend;
		case 149: return Item::X_Special;
		case 150: return Item::X_Sp_Def;
		case 151: return Item::X_Speed;
		case 152: return Item::Yellow_Flute;
		case 153: return Item::Yellow_Shard;
		case 154: return Item::Zinc;
		case 155: return Item::Icy_Rock;
		case 156: return Item::Lucky_Punch;
		case 157: return Item::Dubious_Disc;
		case 158: return Item::Sharp_Beak;
		case 159: return Item::Adamant_Orb;
		case 160: return Item::Damp_Rock;
		case 161: return Item::Heat_Rock;
		case 162: return Item::Lustrous_Orb;
		case 163: return Item::Macho_Brace;
		case 164: return Item::Stick;
		case 165: return Item::Dragon_Fang;
		case 166: return Item::Poison_Barb;
		case 167: return Item::Power_Anklet;
		case 168: return Item::Power_Band;
		case 169: return Item::Power_Belt;
		case 170: return Item::Power_Bracer;
		case 171: return Item::Power_Lens;
		case 172: return Item::Power_Weight;
		case 173: return Item::Dawn_Stone;
		case 174: return Item::Dusk_Stone;
		case 175: return Item::Electirizer;
		case 176: return Item::Magmarizer;
		case 177: return Item::Odd_Keystone;
		case 178: return Item::Oval_Stone;
		case 179: return Item::Protector;
		case 180: return Item::Quick_Claw;
		case 181: return Item::Razor_Claw;
		case 182: return Item::Shiny_Stone;
		case 183: return Item::Sticky_Barb;
		case 184: return Item::Deep_Sea_Tooth;
		case 185: return Item::Draco_Plate;
		case 186: return Item::Dread_Plate;
		case 187: return Item::Earth_Plate;
		case 188: return Item::Fist_Plate;
		case 189: return Item::Flame_Plate;
		case 190: return Item::Grip_Claw;
		case 191: return Item::Icicle_Plate;
		case 192: return Item::Insect_Plate;
		case 193: return Item::Iron_Plate;
		case 194: return Item::Meadow_Plate;
		case 195: return Item::Mind_Plate;
		case 196: return Item::Sky_Plate;
		case 197: return Item::Splash_Plate;
		case 198: return Item::Spooky_Plate;
		case 199: return Item::Stone_Plate;
		case 200: return Item::Thick_Club;
		case 201: return Item::Toxic_Plate;
		case 202: return Item::Zap_Plate;
		case 203: return Item::Armor_Fossil;
		case 204: return Item::Claw_Fossil;
		case 205: return Item::Dome_Fossil;
		case 206: return Item::Hard_Stone;
		case 207: return Item::Helix_Fossil;
		case 208: return Item::Old_Amber;
		case 209: return Item::Rare_Bone;
		case 210: return Item::Root_Fossil;
		case 211: return Item::Skull_Fossil;
		case 212: return Item::Iron_Ball;
		case 213: return Item::Griseous_Orb;
		case 214: return Item::Air_Mail;
		case 215: return Item::Bloom_Mail;
		case 216: return Item::Brick_Mail;
		case 217: return Item::Bubble_Mail;
		case 218: return Item::Flame_Mail;
		case 219: return Item::Grass_Mail;
		case 220: throw std::runtime_error("case 220 intentionally missing");
		case 221: return Item::Heart_Mail;
		case 222: return Item::Mosaic_Mail;
		case 223: return Item::Snow_Mail;
		case 224: return Item::Space_Mail;
		case 225: return Item::Steel_Mail;
		case 226: return Item::Tunnel_Mail;
		default: {
			static_assert(bounded::max_value<decltype(id)> == 226);
			bounded::unreachable();
		}
	}
}

ItemID item_to_id(Item const item) {
	switch (item) {
		case Item::None: return 0_bi;
		case Item::Big_Root: return 1_bi;
		case Item::Blue_Scarf: return 2_bi;
		case Item::Bright_Powder: return 3_bi;
		case Item::Choice_Band: return 4_bi;
		case Item::Choice_Scarf: return 5_bi;
		case Item::Choice_Specs: return 6_bi;
		case Item::Destiny_Knot: return 7_bi;
		case Item::Expert_Belt: return 8_bi;
		case Item::Focus_Band: return 9_bi;
		case Item::Focus_Sash: return 10_bi;
		case Item::Full_Incense: return 11_bi;
		case Item::Green_Scarf: return 12_bi;
		case Item::Lagging_Tail: return 13_bi;
		case Item::Lax_Incense: return 14_bi;
		case Item::Leftovers: return 15_bi;
		case Item::Luck_Incense: return 16_bi;
		case Item::Mental_Herb: return 17_bi;
		case Item::Metal_Powder: return 18_bi;
		case Item::Muscle_Band: return 19_bi;
		case Item::Odd_Incense: return 20_bi;
		case Item::Pink_Scarf: return 21_bi;
		case Item::Power_Herb: return 22_bi;
		case Item::Pure_Incense: return 23_bi;
		case Item::Quick_Powder: return 24_bi;
		case Item::Reaper_Cloth: return 25_bi;
		case Item::Red_Scarf: return 26_bi;
		case Item::Rock_Incense: return 27_bi;
		case Item::Rose_Incense: return 28_bi;
		case Item::Sea_Incense: return 29_bi;
		case Item::Shed_Shell: return 30_bi;
		case Item::Silk_Scarf: return 31_bi;
		case Item::Silver_Powder: return 32_bi;
		case Item::Smooth_Rock: return 33_bi;
		case Item::Soft_Sand: return 34_bi;
		case Item::Soothe_Bell: return 35_bi;
		case Item::Wave_Incense: return 36_bi;
		case Item::White_Herb: return 37_bi;
		case Item::Wide_Lens: return 38_bi;
		case Item::Wise_Glasses: return 39_bi;
		case Item::Yellow_Scarf: return 40_bi;
		case Item::Zoom_Lens: return 41_bi;
		case Item::Amulet_Coin: return 42_bi;
		case Item::Antidote: return 43_bi;
		case Item::Awakening: return 44_bi;
		case Item::Berry_Juice: return 45_bi;
		case Item::Big_Pearl: return 46_bi;
		case Item::Big_Mushroom: return 47_bi;
		case Item::Black_Belt: return 48_bi;
		case Item::Black_Flute: return 49_bi;
		case Item::Black_Sludge: return 50_bi;
		case Item::Black_Glasses: return 51_bi;
		case Item::Blue_Flute: return 52_bi;
		case Item::Blue_Shard: return 53_bi;
		case Item::Burn_Heal: return 54_bi;
		case Item::Calcium: return 55_bi;
		case Item::Carbos: return 56_bi;
		case Item::Charcoal: return 57_bi;
		case Item::Cleanse_Tag: return 58_bi;
		case Item::Damp_Mulch: return 59_bi;
		case Item::Deep_Sea_Scale: return 60_bi;
		case Item::Dire_Hit: return 61_bi;
		case Item::Dragon_Scale: return 62_bi;
		case Item::Elixir: return 63_bi;
		case Item::Energy_Root: return 64_bi;
		case Item::Energy_Powder: return 65_bi;
		case Item::Escape_Rope: return 66_bi;
		case Item::Ether: return 67_bi;
		case Item::Everstone: return 68_bi;
		case Item::Exp_Share: return 69_bi;
		case Item::Fire_Stone: return 70_bi;
		case Item::Flame_Orb: return 71_bi;
		case Item::Fluffy_Tail: return 72_bi;
		case Item::Fresh_Water: return 73_bi;
		case Item::Full_Heal: return 74_bi;
		case Item::Full_Restore: return 75_bi;
		case Item::Gooey_Mulch: return 76_bi;
		case Item::Green_Shard: return 77_bi;
		case Item::Growth_Mulch: return 78_bi;
		case Item::Guard_Spec: return 79_bi;
		case Item::Heal_Powder: return 80_bi;
		case Item::Heart_Scale: return 81_bi;
		case Item::Honey: return 82_bi;
		case Item::HP_Up: return 83_bi;
		case Item::Hyper_Potion: return 84_bi;
		case Item::Ice_Heal: return 85_bi;
		case Item::Iron: return 86_bi;
		case Item::Kings_Rock: return 87_bi;
		case Item::Lava_Cookie: return 88_bi;
		case Item::Leaf_Stone: return 89_bi;
		case Item::Lemonade: return 90_bi;
		case Item::Life_Orb: return 91_bi;
		case Item::Light_Ball: return 92_bi;
		case Item::Light_Clay: return 93_bi;
		case Item::Lucky_Egg: return 94_bi;
		case Item::Magnet: return 95_bi;
		case Item::Max_Elixir: return 96_bi;
		case Item::Max_Ether: return 97_bi;
		case Item::Max_Potion: return 98_bi;
		case Item::Max_Repel: return 99_bi;
		case Item::Max_Revive: return 100_bi;
		case Item::Metal_Coat: return 101_bi;
		case Item::Metronome: return 102_bi;
		case Item::Miracle_Seed: return 103_bi;
		case Item::Moomoo_Milk: return 104_bi;
		case Item::Moon_Stone: return 105_bi;
		case Item::Mystic_Water: return 106_bi;
		case Item::Never_Melt_Ice: return 107_bi;
		case Item::Nugget: return 108_bi;
		case Item::Old_Gateau: return 109_bi;
		case Item::Paralyze_Heal: return 110_bi;
		case Item::Pearl: return 111_bi;
		case Item::Poke_Doll: return 112_bi;
		case Item::Potion: return 113_bi;
		case Item::PP_Max: return 114_bi;
		case Item::PP_Up: return 115_bi;
		case Item::Protein: return 116_bi;
		case Item::Rare_Candy: return 117_bi;
		case Item::Razor_Fang: return 118_bi;
		case Item::Red_Flute: return 119_bi;
		case Item::Red_Shard: return 120_bi;
		case Item::Repel: return 121_bi;
		case Item::Revival_Herb: return 122_bi;
		case Item::Revive: return 123_bi;
		case Item::Sacred_Ash: return 124_bi;
		case Item::Scope_Lens: return 125_bi;
		case Item::Shell_Bell: return 126_bi;
		case Item::Shoal_Salt: return 127_bi;
		case Item::Shoal_Shell: return 128_bi;
		case Item::Smoke_Ball: return 129_bi;
		case Item::Soda_Pop: return 130_bi;
		case Item::Soul_Dew: return 131_bi;
		case Item::Spell_Tag: return 132_bi;
		case Item::Stable_Mulch: return 133_bi;
		case Item::Star_Piece: return 134_bi;
		case Item::Stardust: return 135_bi;
		case Item::Sun_Stone: return 136_bi;
		case Item::Super_Potion: return 137_bi;
		case Item::Super_Repel: return 138_bi;
		case Item::Thunder_Stone: return 139_bi;
		case Item::Tiny_Mushroom: return 140_bi;
		case Item::Toxic_Orb: return 141_bi;
		case Item::Twisted_Spoon: return 142_bi;
		case Item::Up_Grade: return 143_bi;
		case Item::Water_Stone: return 144_bi;
		case Item::White_Flute: return 145_bi;
		case Item::X_Accuracy: return 146_bi;
		case Item::X_Attack: return 147_bi;
		case Item::X_Defend: return 148_bi;
		case Item::X_Special: return 149_bi;
		case Item::X_Sp_Def: return 150_bi;
		case Item::X_Speed: return 151_bi;
		case Item::Yellow_Flute: return 152_bi;
		case Item::Yellow_Shard: return 153_bi;
		case Item::Zinc: return 154_bi;
		case Item::Icy_Rock: return 155_bi;
		case Item::Lucky_Punch: return 156_bi;
		case Item::Dubious_Disc: return 157_bi;
		case Item::Sharp_Beak: return 158_bi;
		case Item::Adamant_Orb: return 159_bi;
		case Item::Damp_Rock: return 160_bi;
		case Item::Heat_Rock: return 161_bi;
		case Item::Lustrous_Orb: return 162_bi;
		case Item::Macho_Brace: return 163_bi;
		case Item::Stick: return 164_bi;
		case Item::Dragon_Fang: return 165_bi;
		case Item::Poison_Barb: return 166_bi;
		case Item::Power_Anklet: return 167_bi;
		case Item::Power_Band: return 168_bi;
		case Item::Power_Belt: return 169_bi;
		case Item::Power_Bracer: return 170_bi;
		case Item::Power_Lens: return 171_bi;
		case Item::Power_Weight: return 172_bi;
		case Item::Dawn_Stone: return 173_bi;
		case Item::Dusk_Stone: return 174_bi;
		case Item::Electirizer: return 175_bi;
		case Item::Magmarizer: return 176_bi;
		case Item::Odd_Keystone: return 177_bi;
		case Item::Oval_Stone: return 178_bi;
		case Item::Protector: return 179_bi;
		case Item::Quick_Claw: return 180_bi;
		case Item::Razor_Claw: return 181_bi;
		case Item::Shiny_Stone: return 182_bi;
		case Item::Sticky_Barb: return 183_bi;
		case Item::Deep_Sea_Tooth: return 184_bi;
		case Item::Draco_Plate: return 185_bi;
		case Item::Dread_Plate: return 186_bi;
		case Item::Earth_Plate: return 187_bi;
		case Item::Fist_Plate: return 188_bi;
		case Item::Flame_Plate: return 189_bi;
		case Item::Grip_Claw: return 190_bi;
		case Item::Icicle_Plate: return 191_bi;
		case Item::Insect_Plate: return 192_bi;
		case Item::Iron_Plate: return 193_bi;
		case Item::Meadow_Plate: return 194_bi;
		case Item::Mind_Plate: return 195_bi;
		case Item::Sky_Plate: return 196_bi;
		case Item::Splash_Plate: return 197_bi;
		case Item::Spooky_Plate: return 198_bi;
		case Item::Stone_Plate: return 199_bi;
		case Item::Thick_Club: return 200_bi;
		case Item::Toxic_Plate: return 201_bi;
		case Item::Zap_Plate: return 202_bi;
		case Item::Armor_Fossil: return 203_bi;
		case Item::Claw_Fossil: return 204_bi;
		case Item::Dome_Fossil: return 205_bi;
		case Item::Hard_Stone: return 206_bi;
		case Item::Helix_Fossil: return 207_bi;
		case Item::Old_Amber: return 208_bi;
		case Item::Rare_Bone: return 209_bi;
		case Item::Root_Fossil: return 210_bi;
		case Item::Skull_Fossil: return 211_bi;
		case Item::Iron_Ball: return 212_bi;
		case Item::Griseous_Orb: return 213_bi;
		case Item::Air_Mail: return 214_bi;
		case Item::Bloom_Mail: return 215_bi;
		case Item::Brick_Mail: return 216_bi;
		case Item::Bubble_Mail: return 217_bi;
		case Item::Flame_Mail: return 218_bi;
		case Item::Grass_Mail: return 219_bi;
		// Intentionally missing 220
		case Item::Heart_Mail: return 221_bi;
		case Item::Mosaic_Mail: return 222_bi;
		case Item::Snow_Mail: return 223_bi;
		case Item::Space_Mail: return 224_bi;
		case Item::Steel_Mail: return 225_bi;
		case Item::Tunnel_Mail: return 226_bi;
		default: return 0_bi;
	}
}

Item battle_id_to_item(uint16_t id, uint8_t part) {
	switch (id) {
		case 3: return Item::White_Herb;
		case 4: return Item::Focus_Band;
		case 5: return Item::Focus_Sash;
		case 7: return Item::Mental_Herb;
		case 11: return Item::Power_Herb;
		case 12: return Item::Leftovers;
		case 16: return Item::Black_Sludge;
		case 17: return Item::Quick_Claw;
		case 18: return Item::Berry_Juice;
		case 19: return (part == 0) ? Item::Flame_Orb : Item::Toxic_Orb;
		case 21: return Item::Life_Orb;
		case 24: return Item::Shell_Bell;
		case 29: return Item::Sticky_Barb;
		default: throw InvalidPart("item", id, part);
	}
}

Moves id_to_move(MoveID const id) {
	return static_cast<Moves>(id + bounded::integer(Moves::Regular_Begin) - 1_bi);
}

MoveID move_to_id(Moves const move) {
	auto const move_id = bounded::integer(move) - bounded::integer(Moves::Regular_Begin) + 1_bi;
	BOUNDED_ASSERT(move_id > 0_bi);
	return MoveID(move_id);
}

Nature id_to_nature(NatureID const id) {
	switch (id.value()) {
		case 0: return Nature::Hardy;
		case 1: return Nature::Lonely;
		case 2: return Nature::Brave;
		case 3: return Nature::Adamant;
		case 4: return Nature::Naughty;
		case 5: return Nature::Bold;
		case 6: return Nature::Docile;
		case 7: return Nature::Relaxed;
		case 8: return Nature::Impish;
		case 9: return Nature::Lax;
		case 10: return Nature::Timid;
		case 11: return Nature::Hasty;
		case 12: return Nature::Serious;
		case 13: return Nature::Jolly;
		case 14: return Nature::Naive;
		case 15: return Nature::Modest;
		case 16: return Nature::Mild;
		case 17: return Nature::Quiet;
		case 18: return Nature::Bashful;
		case 19: return Nature::Rash;
		case 20: return Nature::Calm;
		case 21: return Nature::Gentle;
		case 22: return Nature::Sassy;
		case 23: return Nature::Careful;
		case 24: return Nature::Quirky;
		default: {
			static_assert(bounded::max_value<decltype(id)> == 24);
			bounded::unreachable();
		}
	}
}

NatureID nature_to_id(Nature const nature) {
	switch (nature) {
		case Nature::Adamant: return 3_bi;
		case Nature::Bashful: return 18_bi;
		case Nature::Bold: return 5_bi;
		case Nature::Brave: return 2_bi;
		case Nature::Calm: return 20_bi;
		case Nature::Careful: return 23_bi;
		case Nature::Docile: return 6_bi;
		case Nature::Gentle: return 21_bi;
		case Nature::Hardy: return 0_bi;
		case Nature::Hasty: return 11_bi;
		case Nature::Impish: return 8_bi;
		case Nature::Jolly: return 13_bi;
		case Nature::Lax: return 9_bi;
		case Nature::Lonely: return 1_bi;
		case Nature::Mild: return 16_bi;
		case Nature::Modest: return 15_bi;
		case Nature::Naive: return 14_bi;
		case Nature::Naughty: return 4_bi;
		case Nature::Quiet: return 17_bi;
		case Nature::Quirky: return 24_bi;
		case Nature::Rash: return 19_bi;
		case Nature::Relaxed: return 7_bi;
		case Nature::Sassy: return 22_bi;
		case Nature::Serious: return 12_bi;
		case Nature::Timid: return 10_bi;
	}
}

} // namespace po
} // namespace technicalmachine
