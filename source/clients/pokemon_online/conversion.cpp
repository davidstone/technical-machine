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

#include "conversion.hpp"

#include "../../ability.hpp"
#include "../../gender.hpp"
#include "../../item.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

#include "../../stat/stat.hpp"

#include "../../string_conversions/pokemon.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace technicalmachine {
namespace po {

namespace {

struct InvalidSpeciesID : std::runtime_error {
	explicit InvalidSpeciesID(unsigned const id):
		std::runtime_error("Invalid species ID " + std::to_string(id))
	{
	}
};

struct InvalidFormeId : std::runtime_error {
	InvalidFormeId(std::string const & species, unsigned const forme):
		std::runtime_error("Invalid forme ID " + std::to_string(forme) + " for " + species)
	{
	}
};

struct UnsupportedSpecies : std::runtime_error {
	explicit UnsupportedSpecies(Species const species):
		std::runtime_error(to_string(species) + " not supported in Pokemon online.")
	{
	}
};

}	// namespace

Species id_to_species (unsigned const id, unsigned const forme) {
	switch (id) {
		case 0: return Species::END;	// "Missingno."

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
			switch (forme) {
				case 0: return Species::Deoxys_Mediocre;
				case 1: return Species::Deoxys_Attack;
				case 2: return Species::Deoxys_Defense;
				case 3: return Species::Deoxys_Speed;
				default: throw InvalidFormeId("Deoxys", forme);
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
			switch (forme) {
				case 0: return Species::Wormadam_Plant;
				case 1: return Species::Wormadam_Sandy;
				case 2: return Species::Wormadam_Trash;
				default: throw InvalidFormeId("Wormadam", forme);
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
			switch (forme) {
				case 0: return Species::Rotom;
				case 1: return Species::Rotom_Mow;
				case 2: return Species::Rotom_Heat;
				case 3: return Species::Rotom_Frost;
				case 4: return Species::Rotom_Wash;
				case 5: return Species::Rotom_Fan;
				default: throw InvalidFormeId("Rotom", forme);
			}
		case 480: return Species::Uxie;
		case 481: return Species::Mesprit;
		case 482: return Species::Azelf;
		case 483: return Species::Dialga;
		case 484: return Species::Palkia;
		case 485: return Species::Heatran;
		case 486: return Species::Regigigas;
		case 487:
			switch (forme) {
				case 0: return Species::Giratina_Altered;
				case 1: return Species::Giratina_Origin;
				default: throw InvalidFormeId("Giratina", forme);
			}
		case 488: return Species::Cresselia;
		case 489: return Species::Phione;
		case 490: return Species::Manaphy;
		case 491: return Species::Darkrai;
		case 492:
			switch (forme) {
				case 0: return Species::Shaymin_Land;
				case 1: return Species::Shaymin_Sky;
				default: throw InvalidFormeId("Shaymin", forme);
			}
		case 493: return Species::Arceus;
		
		default: throw InvalidSpeciesID(id);
	}
}

namespace {

constexpr auto to_id_only(Species const species) {
	switch (species) {
		case Species::Bulbasaur: return 1;
		case Species::Ivysaur: return 2;
		case Species::Venusaur: return 3;
		case Species::Charmander: return 4;
		case Species::Charmeleon: return 5;
		case Species::Charizard: return 6;
		case Species::Squirtle: return 7;
		case Species::Wartortle: return 8;
		case Species::Blastoise: return 9;
		case Species::Caterpie: return 10;
		case Species::Metapod: return 11;
		case Species::Butterfree: return 12;
		case Species::Weedle: return 13;
		case Species::Kakuna: return 14;
		case Species::Beedrill: return 15;
		case Species::Pidgey: return 16;
		case Species::Pidgeotto: return 17;
		case Species::Pidgeot: return 18;
		case Species::Rattata: return 19;
		case Species::Raticate: return 20;
		case Species::Spearow: return 21;
		case Species::Fearow: return 22;
		case Species::Ekans: return 23;
		case Species::Arbok: return 24;
		case Species::Pikachu: return 25;
		case Species::Raichu: return 26;
		case Species::Sandshrew: return 27;
		case Species::Sandslash: return 28;
		case Species::Nidoran_F: return 29;
		case Species::Nidorina: return 30;
		case Species::Nidoqueen: return 31;
		case Species::Nidoran_M: return 32;
		case Species::Nidorino: return 33;
		case Species::Nidoking: return 34;
		case Species::Clefairy: return 35;
		case Species::Clefable: return 36;
		case Species::Vulpix: return 37;
		case Species::Ninetales: return 38;
		case Species::Jigglypuff: return 39;
		case Species::Wigglytuff: return 40;
		case Species::Zubat: return 41;
		case Species::Golbat: return 42;
		case Species::Oddish: return 43;
		case Species::Gloom: return 44;
		case Species::Vileplume: return 45;
		case Species::Paras: return 46;
		case Species::Parasect: return 47;
		case Species::Venonat: return 48;
		case Species::Venomoth: return 49;
		case Species::Diglett: return 50;
		case Species::Dugtrio: return 51;
		case Species::Meowth: return 52;
		case Species::Persian: return 53;
		case Species::Psyduck: return 54;
		case Species::Golduck: return 55;
		case Species::Mankey: return 56;
		case Species::Primeape: return 57;
		case Species::Growlithe: return 58;
		case Species::Arcanine: return 59;
		case Species::Poliwag: return 60;
		case Species::Poliwhirl: return 61;
		case Species::Poliwrath: return 62;
		case Species::Abra: return 63;
		case Species::Kadabra: return 64;
		case Species::Alakazam: return 65;
		case Species::Machop: return 66;
		case Species::Machoke: return 67;
		case Species::Machamp: return 68;
		case Species::Bellsprout: return 69;
		case Species::Weepinbell: return 70;
		case Species::Victreebel: return 71;
		case Species::Tentacool: return 72;
		case Species::Tentacruel: return 73;
		case Species::Geodude: return 74;
		case Species::Graveler: return 75;
		case Species::Golem: return 76;
		case Species::Ponyta: return 77;
		case Species::Rapidash: return 78;
		case Species::Slowpoke: return 79;
		case Species::Slowbro: return 80;
		case Species::Magnemite: return 81;
		case Species::Magneton: return 82;
		case Species::Farfetchd: return 83;
		case Species::Doduo: return 84;
		case Species::Dodrio: return 85;
		case Species::Seel: return 86;
		case Species::Dewgong: return 87;
		case Species::Grimer: return 88;
		case Species::Muk: return 89;
		case Species::Shellder: return 90;
		case Species::Cloyster: return 91;
		case Species::Gastly: return 92;
		case Species::Haunter: return 93;
		case Species::Gengar: return 94;
		case Species::Onix: return 95;
		case Species::Drowzee: return 96;
		case Species::Hypno: return 97;
		case Species::Krabby: return 98;
		case Species::Kingler: return 99;
		case Species::Voltorb: return 100;
		case Species::Electrode: return 101;
		case Species::Exeggcute: return 102;
		case Species::Exeggutor: return 103;
		case Species::Cubone: return 104;
		case Species::Marowak: return 105;
		case Species::Hitmonlee: return 106;
		case Species::Hitmonchan: return 107;
		case Species::Lickitung: return 108;
		case Species::Koffing: return 109;
		case Species::Weezing: return 110;
		case Species::Rhyhorn: return 111;
		case Species::Rhydon: return 112;
		case Species::Chansey: return 113;
		case Species::Tangela: return 114;
		case Species::Kangaskhan: return 115;
		case Species::Horsea: return 116;
		case Species::Seadra: return 117;
		case Species::Goldeen: return 118;
		case Species::Seaking: return 119;
		case Species::Staryu: return 120;
		case Species::Starmie: return 121;
		case Species::Mr_Mime: return 122;
		case Species::Scyther: return 123;
		case Species::Jynx: return 124;
		case Species::Electabuzz: return 125;
		case Species::Magmar: return 126;
		case Species::Pinsir: return 127;
		case Species::Tauros: return 128;
		case Species::Magikarp: return 129;
		case Species::Gyarados: return 130;
		case Species::Lapras: return 131;
		case Species::Ditto: return 132;
		case Species::Eevee: return 133;
		case Species::Vaporeon: return 134;
		case Species::Jolteon: return 135;
		case Species::Flareon: return 136;
		case Species::Porygon: return 137;
		case Species::Omanyte: return 138;
		case Species::Omastar: return 139;
		case Species::Kabuto: return 140;
		case Species::Kabutops: return 141;
		case Species::Aerodactyl: return 142;
		case Species::Snorlax: return 143;
		case Species::Articuno: return 144;
		case Species::Zapdos: return 145;
		case Species::Moltres: return 146;
		case Species::Dratini: return 147;
		case Species::Dragonair: return 148;
		case Species::Dragonite: return 149;
		case Species::Mewtwo: return 150;
		case Species::Mew: return 151;
		case Species::Chikorita: return 152;
		case Species::Bayleef: return 153;
		case Species::Meganium: return 154;
		case Species::Cyndaquil: return 155;
		case Species::Quilava: return 156;
		case Species::Typhlosion: return 157;
		case Species::Totodile: return 158;
		case Species::Croconaw: return 159;
		case Species::Feraligatr: return 160;
		case Species::Sentret: return 161;
		case Species::Furret: return 162;
		case Species::Hoothoot: return 163;
		case Species::Noctowl: return 164;
		case Species::Ledyba: return 165;
		case Species::Ledian: return 166;
		case Species::Spinarak: return 167;
		case Species::Ariados: return 168;
		case Species::Crobat: return 169;
		case Species::Chinchou: return 170;
		case Species::Lanturn: return 171;
		case Species::Pichu: return 172;
		case Species::Cleffa: return 173;
		case Species::Igglybuff: return 174;
		case Species::Togepi: return 175;
		case Species::Togetic: return 176;
		case Species::Natu: return 177;
		case Species::Xatu: return 178;
		case Species::Mareep: return 179;
		case Species::Flaaffy: return 180;
		case Species::Ampharos: return 181;
		case Species::Bellossom: return 182;
		case Species::Marill: return 183;
		case Species::Azumarill: return 184;
		case Species::Sudowoodo: return 185;
		case Species::Politoed: return 186;
		case Species::Hoppip: return 187;
		case Species::Skiploom: return 188;
		case Species::Jumpluff: return 189;
		case Species::Aipom: return 190;
		case Species::Sunkern: return 191;
		case Species::Sunflora: return 192;
		case Species::Yanma: return 193;
		case Species::Wooper: return 194;
		case Species::Quagsire: return 195;
		case Species::Espeon: return 196;
		case Species::Umbreon: return 197;
		case Species::Murkrow: return 198;
		case Species::Slowking: return 199;
		case Species::Misdreavus: return 200;
		case Species::Unown: return 201;
		case Species::Wobbuffet: return 202;
		case Species::Girafarig: return 203;
		case Species::Pineco: return 204;
		case Species::Forretress: return 205;
		case Species::Dunsparce: return 206;
		case Species::Gligar: return 207;
		case Species::Steelix: return 208;
		case Species::Snubbull: return 209;
		case Species::Granbull: return 210;
		case Species::Qwilfish: return 211;
		case Species::Scizor: return 212;
		case Species::Shuckle: return 213;
		case Species::Heracross: return 214;
		case Species::Sneasel: return 215;
		case Species::Teddiursa: return 216;
		case Species::Ursaring: return 217;
		case Species::Slugma: return 218;
		case Species::Magcargo: return 219;
		case Species::Swinub: return 220;
		case Species::Piloswine: return 221;
		case Species::Corsola: return 222;
		case Species::Remoraid: return 223;
		case Species::Octillery: return 224;
		case Species::Delibird: return 225;
		case Species::Mantine: return 226;
		case Species::Skarmory: return 227;
		case Species::Houndour: return 228;
		case Species::Houndoom: return 229;
		case Species::Kingdra: return 230;
		case Species::Phanpy: return 231;
		case Species::Donphan: return 232;
		case Species::Porygon2: return 233;
		case Species::Stantler: return 234;
		case Species::Smeargle: return 235;
		case Species::Tyrogue: return 236;
		case Species::Hitmontop: return 237;
		case Species::Smoochum: return 238;
		case Species::Elekid: return 239;
		case Species::Magby: return 240;
		case Species::Miltank: return 241;
		case Species::Blissey: return 242;
		case Species::Raikou: return 243;
		case Species::Entei: return 244;
		case Species::Suicune: return 245;
		case Species::Larvitar: return 246;
		case Species::Pupitar: return 247;
		case Species::Tyranitar: return 248;
		case Species::Lugia: return 249;
		case Species::Ho_Oh: return 250;
		case Species::Celebi: return 251;
		case Species::Treecko: return 252;
		case Species::Grovyle: return 253;
		case Species::Sceptile: return 254;
		case Species::Torchic: return 255;
		case Species::Combusken: return 256;
		case Species::Blaziken: return 257;
		case Species::Mudkip: return 258;
		case Species::Marshtomp: return 259;
		case Species::Swampert: return 260;
		case Species::Poochyena: return 261;
		case Species::Mightyena: return 262;
		case Species::Zigzagoon: return 263;
		case Species::Linoone: return 264;
		case Species::Wurmple: return 265;
		case Species::Silcoon: return 266;
		case Species::Beautifly: return 267;
		case Species::Cascoon: return 268;
		case Species::Dustox: return 269;
		case Species::Lotad: return 270;
		case Species::Lombre: return 271;
		case Species::Ludicolo: return 272;
		case Species::Seedot: return 273;
		case Species::Nuzleaf: return 274;
		case Species::Shiftry: return 275;
		case Species::Taillow: return 276;
		case Species::Swellow: return 277;
		case Species::Wingull: return 278;
		case Species::Pelipper: return 279;
		case Species::Ralts: return 280;
		case Species::Kirlia: return 281;
		case Species::Gardevoir: return 282;
		case Species::Surskit: return 283;
		case Species::Masquerain: return 284;
		case Species::Shroomish: return 285;
		case Species::Breloom: return 286;
		case Species::Slakoth: return 287;
		case Species::Vigoroth: return 288;
		case Species::Slaking: return 289;
		case Species::Nincada: return 290;
		case Species::Ninjask: return 291;
		case Species::Shedinja: return 292;
		case Species::Whismur: return 293;
		case Species::Loudred: return 294;
		case Species::Exploud: return 295;
		case Species::Makuhita: return 296;
		case Species::Hariyama: return 297;
		case Species::Azurill: return 298;
		case Species::Nosepass: return 299;
		case Species::Skitty: return 300;
		case Species::Delcatty: return 301;
		case Species::Sableye: return 302;
		case Species::Mawile: return 303;
		case Species::Aron: return 304;
		case Species::Lairon: return 305;
		case Species::Aggron: return 306;
		case Species::Meditite: return 307;
		case Species::Medicham: return 308;
		case Species::Electrike: return 309;
		case Species::Manectric: return 310;
		case Species::Plusle: return 311;
		case Species::Minun: return 312;
		case Species::Volbeat: return 313;
		case Species::Illumise: return 314;
		case Species::Roselia: return 315;
		case Species::Gulpin: return 316;
		case Species::Swalot: return 317;
		case Species::Carvanha: return 318;
		case Species::Sharpedo: return 319;
		case Species::Wailmer: return 320;
		case Species::Wailord: return 321;
		case Species::Numel: return 322;
		case Species::Camerupt: return 323;
		case Species::Torkoal: return 324;
		case Species::Spoink: return 325;
		case Species::Grumpig: return 326;
		case Species::Spinda: return 327;
		case Species::Trapinch: return 328;
		case Species::Vibrava: return 329;
		case Species::Flygon: return 330;
		case Species::Cacnea: return 331;
		case Species::Cacturne: return 332;
		case Species::Swablu: return 333;
		case Species::Altaria: return 334;
		case Species::Zangoose: return 335;
		case Species::Seviper: return 336;
		case Species::Lunatone: return 337;
		case Species::Solrock: return 338;
		case Species::Barboach: return 339;
		case Species::Whiscash: return 340;
		case Species::Corphish: return 341;
		case Species::Crawdaunt: return 342;
		case Species::Baltoy: return 343;
		case Species::Claydol: return 344;
		case Species::Lileep: return 345;
		case Species::Cradily: return 346;
		case Species::Anorith: return 347;
		case Species::Armaldo: return 348;
		case Species::Feebas: return 349;
		case Species::Milotic: return 350;
		case Species::Castform: return 351;
		case Species::Kecleon: return 352;
		case Species::Shuppet: return 353;
		case Species::Banette: return 354;
		case Species::Duskull: return 355;
		case Species::Dusclops: return 356;
		case Species::Tropius: return 357;
		case Species::Chimecho: return 358;
		case Species::Absol: return 359;
		case Species::Wynaut: return 360;
		case Species::Snorunt: return 361;
		case Species::Glalie: return 362;
		case Species::Spheal: return 363;
		case Species::Sealeo: return 364;
		case Species::Walrein: return 365;
		case Species::Clamperl: return 366;
		case Species::Huntail: return 367;
		case Species::Gorebyss: return 368;
		case Species::Relicanth: return 369;
		case Species::Luvdisc: return 370;
		case Species::Bagon: return 371;
		case Species::Shelgon: return 372;
		case Species::Salamence: return 373;
		case Species::Beldum: return 374;
		case Species::Metang: return 375;
		case Species::Metagross: return 376;
		case Species::Regirock: return 377;
		case Species::Regice: return 378;
		case Species::Registeel: return 379;
		case Species::Latias: return 380;
		case Species::Latios: return 381;
		case Species::Kyogre: return 382;
		case Species::Groudon: return 383;
		case Species::Rayquaza: return 384;
		case Species::Jirachi: return 385;
		case Species::Deoxys_Mediocre: return 386;
		case Species::Deoxys_Attack: return 386;
		case Species::Deoxys_Defense: return 386;
		case Species::Deoxys_Speed: return 386;
		case Species::Turtwig: return 387;
		case Species::Grotle: return 388;
		case Species::Torterra: return 389;
		case Species::Chimchar: return 390;
		case Species::Monferno: return 391;
		case Species::Infernape: return 392;
		case Species::Piplup: return 393;
		case Species::Prinplup: return 394;
		case Species::Empoleon: return 395;
		case Species::Starly: return 396;
		case Species::Staravia: return 397;
		case Species::Staraptor: return 398;
		case Species::Bidoof: return 399;
		case Species::Bibarel: return 400;
		case Species::Kricketot: return 401;
		case Species::Kricketune: return 402;
		case Species::Shinx: return 403;
		case Species::Luxio: return 404;
		case Species::Luxray: return 405;
		case Species::Budew: return 406;
		case Species::Roserade: return 407;
		case Species::Cranidos: return 408;
		case Species::Rampardos: return 409;
		case Species::Shieldon: return 410;
		case Species::Bastiodon: return 411;
		case Species::Burmy: return 412;
		case Species::Wormadam_Plant: return 413;
		case Species::Wormadam_Sandy: return 413;
		case Species::Wormadam_Trash: return 413;
		case Species::Mothim: return 414;
		case Species::Combee: return 415;
		case Species::Vespiquen: return 416;
		case Species::Pachirisu: return 417;
		case Species::Buizel: return 418;
		case Species::Floatzel: return 419;
		case Species::Cherubi: return 420;
		case Species::Cherrim: return 421;
		case Species::Shellos: return 422;
		case Species::Gastrodon: return 423;
		case Species::Ambipom: return 424;
		case Species::Drifloon: return 425;
		case Species::Drifblim: return 426;
		case Species::Buneary: return 427;
		case Species::Lopunny: return 428;
		case Species::Mismagius: return 429;
		case Species::Honchkrow: return 430;
		case Species::Glameow: return 431;
		case Species::Purugly: return 432;
		case Species::Chingling: return 433;
		case Species::Stunky: return 434;
		case Species::Skuntank: return 435;
		case Species::Bronzor: return 436;
		case Species::Bronzong: return 437;
		case Species::Bonsly: return 438;
		case Species::Mime_Jr: return 439;
		case Species::Happiny: return 440;
		case Species::Chatot: return 441;
		case Species::Spiritomb: return 442;
		case Species::Gible: return 443;
		case Species::Gabite: return 444;
		case Species::Garchomp: return 445;
		case Species::Munchlax: return 446;
		case Species::Riolu: return 447;
		case Species::Lucario: return 448;
		case Species::Hippopotas: return 449;
		case Species::Hippowdon: return 450;
		case Species::Skorupi: return 451;
		case Species::Drapion: return 452;
		case Species::Croagunk: return 453;
		case Species::Toxicroak: return 454;
		case Species::Carnivine: return 455;
		case Species::Finneon: return 456;
		case Species::Lumineon: return 457;
		case Species::Mantyke: return 458;
		case Species::Snover: return 459;
		case Species::Abomasnow: return 460;
		case Species::Weavile: return 461;
		case Species::Magnezone: return 462;
		case Species::Lickilicky: return 463;
		case Species::Rhyperior: return 464;
		case Species::Tangrowth: return 465;
		case Species::Electivire: return 466;
		case Species::Magmortar: return 467;
		case Species::Togekiss: return 468;
		case Species::Yanmega: return 469;
		case Species::Leafeon: return 470;
		case Species::Glaceon: return 471;
		case Species::Gliscor: return 472;
		case Species::Mamoswine: return 473;
		case Species::Porygon_Z: return 474;
		case Species::Gallade: return 475;
		case Species::Probopass: return 476;
		case Species::Dusknoir: return 477;
		case Species::Froslass: return 478;
		case Species::Rotom: return 479;
		case Species::Rotom_Heat: return 479;
		case Species::Rotom_Wash: return 479;
		case Species::Rotom_Frost: return 479;
		case Species::Rotom_Fan: return 479;
		case Species::Rotom_Mow: return 479;
		case Species::Uxie: return 480;
		case Species::Mesprit: return 481;
		case Species::Azelf: return 482;
		case Species::Dialga: return 483;
		case Species::Palkia: return 484;
		case Species::Heatran: return 485;
		case Species::Regigigas: return 486;
		case Species::Giratina_Altered: return 487;
		case Species::Giratina_Origin: return 487;
		case Species::Cresselia: return 488;
		case Species::Phione: return 489;
		case Species::Manaphy: return 490;
		case Species::Darkrai: return 491;
		case Species::Shaymin_Land: return 492;
		case Species::Shaymin_Sky: return 492;
		case Species::Arceus: return 493;
		case Species::END: assert(false);
		default: throw UnsupportedSpecies(species);
	}
}

constexpr auto to_forme(Species const species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Giratina_Origin:
		case Species::Rotom_Mow:
		case Species::Shaymin_Sky:
		case Species::Wormadam_Sandy:
			return 1;
		case Species::Deoxys_Defense:
		case Species::Rotom_Heat:
		case Species::Wormadam_Trash:
			return 2;
		case Species::Deoxys_Speed:
		case Species::Rotom_Frost:
			return 3;
		case Species::Rotom_Wash:
			return 4;
		case Species::Rotom_Fan:
			return 5;
		default:
			return 0;
	}
}

}	// namespace

std::pair<uint16_t, uint8_t> species_to_id(Species const species) {
	return std::make_pair(to_id_only(species), to_forme(species));
}

Ability::Abilities id_to_ability(unsigned const id) {
	switch (id) {
		case 0: return Ability::END;
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
		case 14: return Ability::Compoundeyes;
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
		case 31: return Ability::Lightningrod;
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
		default: throw std::runtime_error("Invalid ability ID in PO conversion");
	}
}

unsigned ability_to_id(Ability::Abilities const ability) {
	switch (ability) {
		case Ability::Adaptability: return 91;
		case Ability::Aftermath: return 106;
		case Ability::Air_Lock: return 76;
		case Ability::Anger_Point: return 83;
		case Ability::Anticipation: return 107;
		case Ability::Arena_Trap: return 71;
		case Ability::Bad_Dreams: return 123;
		case Ability::Battle_Armor: return 4;
		case Ability::Blaze: return 66;
		case Ability::Chlorophyll: return 34;
		case Ability::Clear_Body: return 29;
		case Ability::Cloud_Nine: return 13;
		case Ability::Color_Change: return 16;
		case Ability::Compoundeyes: return 14;
		case Ability::Cute_Charm: return 56;
		case Ability::Damp: return 6;
		case Ability::Download: return 88;
		case Ability::Drizzle: return 2;
		case Ability::Drought: return 70;
		case Ability::Dry_Skin: return 87;
		case Ability::Early_Bird: return 48;
		case Ability::Effect_Spore: return 27;
		case Ability::Filter: return 111;
		case Ability::Flame_Body: return 49;
		case Ability::Flash_Fire: return 18;
		case Ability::Flower_Gift: return 122;
		case Ability::Forecast: return 59;
		case Ability::Forewarn: return 108;
		case Ability::Frisk: return 119;
		case Ability::Gluttony: return 82;
		case Ability::Guts: return 62;
		case Ability::Heatproof: return 85;
		case Ability::Honey_Gather: return 118;
		case Ability::Huge_Power: return 37;
		case Ability::Hustle: return 55;
		case Ability::Hydration: return 93;
		case Ability::Hyper_Cutter: return 52;
		case Ability::Ice_Body: return 115;
		case Ability::Illuminate: return 35;
		case Ability::Immunity: return 17;
		case Ability::Inner_Focus: return 39;
		case Ability::Insomnia: return 15;
		case Ability::Intimidate: return 22;
		case Ability::Iron_Fist: return 89;
		case Ability::Keen_Eye: return 51;
		case Ability::Klutz: return 103;
		case Ability::Leaf_Guard: return 102;
		case Ability::Levitate: return 26;
		case Ability::Lightningrod: return 31;
		case Ability::Limber: return 7;
		case Ability::Liquid_Ooze: return 64;
		case Ability::Magic_Guard: return 98;
		case Ability::Magma_Armor: return 40;
		case Ability::Magnet_Pull: return 42;
		case Ability::Marvel_Scale: return 63;
		case Ability::Minus: return 58;
		case Ability::Mold_Breaker: return 104;
		case Ability::Motor_Drive: return 78;
		case Ability::Multitype: return 121;
		case Ability::Natural_Cure: return 30;
		case Ability::No_Guard: return 99;
		case Ability::Normalize: return 96;
		case Ability::Oblivious: return 12;
		case Ability::Overgrow: return 65;
		case Ability::Own_Tempo: return 20;
		case Ability::Pickup: return 53;
		case Ability::Plus: return 57;
		case Ability::Poison_Heal: return 90;
		case Ability::Poison_Point: return 38;
		case Ability::Pressure: return 46;
		case Ability::Pure_Power: return 74;
		case Ability::Quick_Feet: return 95;
		case Ability::Rain_Dish: return 44;
		case Ability::Reckless: return 120;
		case Ability::Rivalry: return 79;
		case Ability::Rock_Head: return 69;
		case Ability::Rough_Skin: return 24;
		case Ability::Run_Away: return 50;
		case Ability::Sand_Stream: return 45;
		case Ability::Sand_Veil: return 8;
		case Ability::Scrappy: return 113;
		case Ability::Serene_Grace: return 32;
		case Ability::Shadow_Tag: return 23;
		case Ability::Shed_Skin: return 61;
		case Ability::Shell_Armor: return 75;
		case Ability::Shield_Dust: return 19;
		case Ability::Simple: return 86;
		case Ability::Skill_Link: return 92;
		case Ability::Slow_Start: return 112;
		case Ability::Sniper: return 97;
		case Ability::Snow_Cloak: return 81;
		case Ability::Snow_Warning: return 117;
		case Ability::Solar_Power: return 94;
		case Ability::Solid_Rock: return 116;
		case Ability::Soundproof: return 43;
		case Ability::Speed_Boost: return 3;
		case Ability::Stall: return 100;
		case Ability::Static: return 9;
		case Ability::Steadfast: return 80;
		case Ability::Stench: return 1;
		case Ability::Sticky_Hold: return 60;
		case Ability::Storm_Drain: return 114;
		case Ability::Sturdy: return 5;
		case Ability::Suction_Cups: return 21;
		case Ability::Super_Luck: return 105;
		case Ability::Swarm: return 68;
		case Ability::Swift_Swim: return 33;
		case Ability::Synchronize: return 28;
		case Ability::Tangled_Feet: return 77;
		case Ability::Technician: return 101;
		case Ability::Thick_Fat: return 47;
		case Ability::Tinted_Lens: return 110;
		case Ability::Torrent: return 67;
		case Ability::Trace: return 36;
		case Ability::Truant: return 54;
		case Ability::Unaware: return 109;
		case Ability::Unburden: return 84;
		case Ability::Vital_Spirit: return 72;
		case Ability::Volt_Absorb: return 10;
		case Ability::Water_Absorb: return 11;
		case Ability::Water_Veil: return 41;
		case Ability::White_Smoke: return 73;
		case Ability::Wonder_Guard: return 25;
		case Ability::END: assert(false);
	};
}

struct InvalidPart : std::runtime_error {
	InvalidPart (uint16_t id, uint8_t part):
		std::runtime_error ("Invalid conversion to ability ID: " + std::to_string (static_cast<unsigned> (id)) + " with part ID: " + std::to_string (static_cast <unsigned> (part)) + ".\n") {
	}
};

Ability::Abilities battle_id_to_ability (uint16_t id, uint8_t part) {
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
			default: throw InvalidPart(id, part);
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
		default: return Ability::END;
	}
}

Gender::Genders id_to_gender(unsigned id) {
	switch (id) {
		case 0: return Gender::GENDERLESS;
		case 1: return Gender::MALE;
		case 2: return Gender::FEMALE;
		default: throw std::runtime_error("Invalid Gender ID in PO conversion");
	}
}

unsigned gender_to_id (Gender::Genders gender) {
	switch (gender) {
		case Gender::GENDERLESS: return 0;
		case Gender::MALE: return 1;
		case Gender::FEMALE: return 2;
		case Gender::END: assert(false);
	}
}

Item id_to_item(unsigned id) {
	switch (id) {
		case 0: return Item::No_Item;
		case 1: return Item::Big_Root;
		case 2: return Item::Blue_Scarf;
		case 3: return Item::BrightPowder;
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
		case 32: return Item::SilverPowder;
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
		case 51: return Item::BlackGlasses;
		case 52: return Item::Blue_Flute;
		case 53: return Item::Blue_Shard;
		case 54: return Item::Burn_Heal;
		case 55: return Item::Calcium;
		case 56: return Item::Carbos;
		case 57: return Item::Charcoal;
		case 58: return Item::Cleanse_Tag;
		case 59: return Item::Damp_Mulch;
		case 60: return Item::DeepSeaScale;
		case 61: return Item::Dire_Hit;
		case 62: return Item::Dragon_Scale;
		case 63: return Item::Elixir;
		case 64: return Item::Energy_Root;
		case 65: return Item::EnergyPowder;
		case 66: return Item::Escape_Rope;
		case 67: return Item::Ether;
		case 68: return Item::Everstone;
		case 69: return Item::Exp_Share;
		case 70: return Item::Fire_Stone;
		case 71: return Item::Flame_Orb;
		case 72: return Item::Fluffy_Tail;
		case 73: return Item::Fresh_Water;
		case 74: return Item::Full_Heal;
		case 75: return Item::Full_Sleep_Restore;
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
		case 104: return Item::MooMoo_Milk;
		case 105: return Item::Moon_Stone;
		case 106: return Item::Mystic_Water;
		case 107: return Item::NeverMeltIce;
		case 108: return Item::Nugget;
		case 109: return Item::Old_Gateau;
		case 110: return Item::Parlyz_Heal;
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
		case 139: return Item::Thunderstone;
		case 140: return Item::TinyMushroom;
		case 141: return Item::Toxic_Orb;
		case 142: return Item::TwistedSpoon;
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
		case 184: return Item::DeepSeaTooth;
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
		// case 220 intentionally missing
		case 221: return Item::Heart_Mail;
		case 222: return Item::Mosaic_Mail;
		case 223: return Item::Snow_Mail;
		case 224: return Item::Space_Mail;
		case 225: return Item::Steel_Mail;
		case 226: return Item::Tunnel_Mail;
		default: throw std::runtime_error("Invalid item ID in PO conversion");
	}
}

unsigned item_to_id(Item item) {
	switch (item) {
		case Item::No_Item: return 0;
		case Item::Big_Root: return 1;
		case Item::Blue_Scarf: return 2;
		case Item::BrightPowder: return 3;
		case Item::Choice_Band: return 4;
		case Item::Choice_Scarf: return 5;
		case Item::Choice_Specs: return 6;
		case Item::Destiny_Knot: return 7;
		case Item::Expert_Belt: return 8;
		case Item::Focus_Band: return 9;
		case Item::Focus_Sash: return 10;
		case Item::Full_Incense: return 11;
		case Item::Green_Scarf: return 12;
		case Item::Lagging_Tail: return 13;
		case Item::Lax_Incense: return 14;
		case Item::Leftovers: return 15;
		case Item::Luck_Incense: return 16;
		case Item::Mental_Herb: return 17;
		case Item::Metal_Powder: return 18;
		case Item::Muscle_Band: return 19;
		case Item::Odd_Incense: return 20;
		case Item::Pink_Scarf: return 21;
		case Item::Power_Herb: return 22;
		case Item::Pure_Incense: return 23;
		case Item::Quick_Powder: return 24;
		case Item::Reaper_Cloth: return 25;
		case Item::Red_Scarf: return 26;
		case Item::Rock_Incense: return 27;
		case Item::Rose_Incense: return 28;
		case Item::Sea_Incense: return 29;
		case Item::Shed_Shell: return 30;
		case Item::Silk_Scarf: return 31;
		case Item::SilverPowder: return 32;
		case Item::Smooth_Rock: return 33;
		case Item::Soft_Sand: return 34;
		case Item::Soothe_Bell: return 35;
		case Item::Wave_Incense: return 36;
		case Item::White_Herb: return 37;
		case Item::Wide_Lens: return 38;
		case Item::Wise_Glasses: return 39;
		case Item::Yellow_Scarf: return 40;
		case Item::Zoom_Lens: return 41;
		case Item::Amulet_Coin: return 42;
		case Item::Antidote: return 43;
		case Item::Awakening: return 44;
		case Item::Berry_Juice: return 45;
		case Item::Big_Pearl: return 46;
		case Item::Big_Mushroom: return 47;
		case Item::Black_Belt: return 48;
		case Item::Black_Flute: return 49;
		case Item::Black_Sludge: return 50;
		case Item::BlackGlasses: return 51;
		case Item::Blue_Flute: return 52;
		case Item::Blue_Shard: return 53;
		case Item::Burn_Heal: return 54;
		case Item::Calcium: return 55;
		case Item::Carbos: return 56;
		case Item::Charcoal: return 57;
		case Item::Cleanse_Tag: return 58;
		case Item::Damp_Mulch: return 59;
		case Item::DeepSeaScale: return 60;
		case Item::Dire_Hit: return 61;
		case Item::Dragon_Scale: return 62;
		case Item::Elixir: return 63;
		case Item::Energy_Root: return 64;
		case Item::EnergyPowder: return 65;
		case Item::Escape_Rope: return 66;
		case Item::Ether: return 67;
		case Item::Everstone: return 68;
		case Item::Exp_Share: return 69;
		case Item::Fire_Stone: return 70;
		case Item::Flame_Orb: return 71;
		case Item::Fluffy_Tail: return 72;
		case Item::Fresh_Water: return 73;
		case Item::Full_Heal: return 74;
		case Item::Full_Sleep_Restore: return 75;
		case Item::Gooey_Mulch: return 76;
		case Item::Green_Shard: return 77;
		case Item::Growth_Mulch: return 78;
		case Item::Guard_Spec: return 79;
		case Item::Heal_Powder: return 80;
		case Item::Heart_Scale: return 81;
		case Item::Honey: return 82;
		case Item::HP_Up: return 83;
		case Item::Hyper_Potion: return 84;
		case Item::Ice_Heal: return 85;
		case Item::Iron: return 86;
		case Item::Kings_Rock: return 87;
		case Item::Lava_Cookie: return 88;
		case Item::Leaf_Stone: return 89;
		case Item::Lemonade: return 90;
		case Item::Life_Orb: return 91;
		case Item::Light_Ball: return 92;
		case Item::Light_Clay: return 93;
		case Item::Lucky_Egg: return 94;
		case Item::Magnet: return 95;
		case Item::Max_Elixir: return 96;
		case Item::Max_Ether: return 97;
		case Item::Max_Potion: return 98;
		case Item::Max_Repel: return 99;
		case Item::Max_Revive: return 100;
		case Item::Metal_Coat: return 101;
		case Item::Metronome: return 102;
		case Item::Miracle_Seed: return 103;
		case Item::MooMoo_Milk: return 104;
		case Item::Moon_Stone: return 105;
		case Item::Mystic_Water: return 106;
		case Item::NeverMeltIce: return 107;
		case Item::Nugget: return 108;
		case Item::Old_Gateau: return 109;
		case Item::Parlyz_Heal: return 110;
		case Item::Pearl: return 111;
		case Item::Poke_Doll: return 112;
		case Item::Potion: return 113;
		case Item::PP_Max: return 114;
		case Item::PP_Up: return 115;
		case Item::Protein: return 116;
		case Item::Rare_Candy: return 117;
		case Item::Razor_Fang: return 118;
		case Item::Red_Flute: return 119;
		case Item::Red_Shard: return 120;
		case Item::Repel: return 121;
		case Item::Revival_Herb: return 122;
		case Item::Revive: return 123;
		case Item::Sacred_Ash: return 124;
		case Item::Scope_Lens: return 125;
		case Item::Shell_Bell: return 126;
		case Item::Shoal_Salt: return 127;
		case Item::Shoal_Shell: return 128;
		case Item::Smoke_Ball: return 129;
		case Item::Soda_Pop: return 130;
		case Item::Soul_Dew: return 131;
		case Item::Spell_Tag: return 132;
		case Item::Stable_Mulch: return 133;
		case Item::Star_Piece: return 134;
		case Item::Stardust: return 135;
		case Item::Sun_Stone: return 136;
		case Item::Super_Potion: return 137;
		case Item::Super_Repel: return 138;
		case Item::Thunderstone: return 139;
		case Item::TinyMushroom: return 140;
		case Item::Toxic_Orb: return 141;
		case Item::TwistedSpoon: return 142;
		case Item::Up_Grade: return 143;
		case Item::Water_Stone: return 144;
		case Item::White_Flute: return 145;
		case Item::X_Accuracy: return 146;
		case Item::X_Attack: return 147;
		case Item::X_Defend: return 148;
		case Item::X_Special: return 149;
		case Item::X_Sp_Def: return 150;
		case Item::X_Speed: return 151;
		case Item::Yellow_Flute: return 152;
		case Item::Yellow_Shard: return 153;
		case Item::Zinc: return 154;
		case Item::Icy_Rock: return 155;
		case Item::Lucky_Punch: return 156;
		case Item::Dubious_Disc: return 157;
		case Item::Sharp_Beak: return 158;
		case Item::Adamant_Orb: return 159;
		case Item::Damp_Rock: return 160;
		case Item::Heat_Rock: return 161;
		case Item::Lustrous_Orb: return 162;
		case Item::Macho_Brace: return 163;
		case Item::Stick: return 164;
		case Item::Dragon_Fang: return 165;
		case Item::Poison_Barb: return 166;
		case Item::Power_Anklet: return 167;
		case Item::Power_Band: return 168;
		case Item::Power_Belt: return 169;
		case Item::Power_Bracer: return 170;
		case Item::Power_Lens: return 171;
		case Item::Power_Weight: return 172;
		case Item::Dawn_Stone: return 173;
		case Item::Dusk_Stone: return 174;
		case Item::Electirizer: return 175;
		case Item::Magmarizer: return 176;
		case Item::Odd_Keystone: return 177;
		case Item::Oval_Stone: return 178;
		case Item::Protector: return 179;
		case Item::Quick_Claw: return 180;
		case Item::Razor_Claw: return 181;
		case Item::Shiny_Stone: return 182;
		case Item::Sticky_Barb: return 183;
		case Item::DeepSeaTooth: return 184;
		case Item::Draco_Plate: return 185;
		case Item::Dread_Plate: return 186;
		case Item::Earth_Plate: return 187;
		case Item::Fist_Plate: return 188;
		case Item::Flame_Plate: return 189;
		case Item::Grip_Claw: return 190;
		case Item::Icicle_Plate: return 191;
		case Item::Insect_Plate: return 192;
		case Item::Iron_Plate: return 193;
		case Item::Meadow_Plate: return 194;
		case Item::Mind_Plate: return 195;
		case Item::Sky_Plate: return 196;
		case Item::Splash_Plate: return 197;
		case Item::Spooky_Plate: return 198;
		case Item::Stone_Plate: return 199;
		case Item::Thick_Club: return 200;
		case Item::Toxic_Plate: return 201;
		case Item::Zap_Plate: return 202;
		case Item::Armor_Fossil: return 203;
		case Item::Claw_Fossil: return 204;
		case Item::Dome_Fossil: return 205;
		case Item::Hard_Stone: return 206;
		case Item::Helix_Fossil: return 207;
		case Item::Old_Amber: return 208;
		case Item::Rare_Bone: return 209;
		case Item::Root_Fossil: return 210;
		case Item::Skull_Fossil: return 211;
		case Item::Iron_Ball: return 212;
		case Item::Griseous_Orb: return 213;
		case Item::Air_Mail: return 214;
		case Item::Bloom_Mail: return 215;
		case Item::Brick_Mail: return 216;
		case Item::Bubble_Mail: return 217;
		case Item::Flame_Mail: return 218;
		case Item::Grass_Mail: return 219;
		// Intentionally missing 220
		case Item::Heart_Mail: return 221;
		case Item::Mosaic_Mail: return 222;
		case Item::Snow_Mail: return 223;
		case Item::Space_Mail: return 224;
		case Item::Steel_Mail: return 225;
		case Item::Tunnel_Mail: return 226;
		case Item::Aguav_Berry:
		case Item::Apicot_Berry:
		case Item::Aspear_Berry:
		case Item::Babiri_Berry:
		case Item::Belue_Berry:
		case Item::Bicycle:
		case Item::Bluk_Berry:
		case Item::Charti_Berry:
		case Item::Cheri_Berry:
		case Item::Cherish_Ball:
		case Item::Chesto_Berry:
		case Item::Chilan_Berry:
		case Item::Chople_Berry:
		case Item::Coba_Berry:
		case Item::Coin_Case:
		case Item::Colbur_Berry:
		case Item::Cornn_Berry:
		case Item::Coupon_1:
		case Item::Coupon_2:
		case Item::Coupon_3:
		case Item::Custap_Berry:
		case Item::Dive_Ball:
		case Item::Durin_Berry:
		case Item::Dusk_Ball:
		case Item::Enigma_Berry:
		case Item::Explorer_Kit:
		case Item::Fashion_Case:
		case Item::Figy_Berry:
		case Item::Galactic_Key:
		case Item::Ganlon_Berry:
		case Item::Good_Rod:
		case Item::Great_Ball:
		case Item::Grepa_Berry:
		case Item::Haban_Berry:
		case Item::Heal_Ball:
		case Item::HM01:
		case Item::HM02:
		case Item::HM03:
		case Item::HM04:
		case Item::HM05:
		case Item::HM06:
		case Item::HM07:
		case Item::HM08:
		case Item::Hondew_Berry:
		case Item::Iapapa_Berry:
		case Item::Jaboca_Berry:
		case Item::Journal:
		case Item::Kasib_Berry:
		case Item::Kebia_Berry:
		case Item::Kelpsy_Berry:
		case Item::Lansat_Berry:
		case Item::Leppa_Berry:
		case Item::Liechi_Berry:
		case Item::Loot_Sack:
		case Item::Lum_Berry:
		case Item::Lunar_Wing:
		case Item::Luxury_Ball:
		case Item::Mago_Berry:
		case Item::Magost_Berry:
		case Item::Master_Ball:
		case Item::Member_Card:
		case Item::Micle_Berry:
		case Item::Nanab_Berry:
		case Item::Nest_Ball:
		case Item::Net_Ball:
		case Item::Nomel_Berry:
		case Item::Oaks_Letter:
		case Item::Occa_Berry:
		case Item::Old_Charm:
		case Item::Old_Rod:
		case Item::Oran_Berry:
		case Item::Pal_Pad:
		case Item::Pamtre_Berry:
		case Item::Parcel:
		case Item::Passho_Berry:
		case Item::Payapa_Berry:
		case Item::Pecha_Berry:
		case Item::Persim_Berry:
		case Item::Petaya_Berry:
		case Item::Pinap_Berry:
		case Item::Poffin_Case:
		case Item::Point_Card:
		case Item::Poke_Ball:
		case Item::Poke_Radar:
		case Item::Pomeg_Berry:
		case Item::Premier_Ball:
		case Item::Qualot_Berry:
		case Item::Quick_Ball:
		case Item::Rabuta_Berry:
		case Item::Rawst_Berry:
		case Item::Razz_Berry:
		case Item::Repeat_Ball:
		case Item::Rindo_Berry:
		case Item::Rowap_Berry:
		case Item::Safari_Ball:
		case Item::Salac_Berry:
		case Item::Seal_Bag:
		case Item::Seal_Case:
		case Item::SecretPotion:
		case Item::Shuca_Berry:
		case Item::Sitrus_Berry:
		case Item::Spelon_Berry:
		case Item::Sprayduck:
		case Item::Starf_Berry:
		case Item::Storage_Key:
		case Item::Suite_Key:
		case Item::Super_Rod:
		case Item::Tamato_Berry:
		case Item::Tanga_Berry:
		case Item::Timer_Ball:
		case Item::TM01:
		case Item::TM02:
		case Item::TM03:
		case Item::TM04:
		case Item::TM05:
		case Item::TM06:
		case Item::TM07:
		case Item::TM08:
		case Item::TM09:
		case Item::TM10:
		case Item::TM11:
		case Item::TM12:
		case Item::TM13:
		case Item::TM14:
		case Item::TM15:
		case Item::TM16:
		case Item::TM17:
		case Item::TM18:
		case Item::TM19:
		case Item::TM20:
		case Item::TM21:
		case Item::TM22:
		case Item::TM23:
		case Item::TM24:
		case Item::TM25:
		case Item::TM26:
		case Item::TM27:
		case Item::TM28:
		case Item::TM29:
		case Item::TM30:
		case Item::TM31:
		case Item::TM32:
		case Item::TM33:
		case Item::TM34:
		case Item::TM35:
		case Item::TM36:
		case Item::TM37:
		case Item::TM38:
		case Item::TM39:
		case Item::TM40:
		case Item::TM41:
		case Item::TM42:
		case Item::TM43:
		case Item::TM44:
		case Item::TM45:
		case Item::TM46:
		case Item::TM47:
		case Item::TM48:
		case Item::TM49:
		case Item::TM50:
		case Item::TM51:
		case Item::TM52:
		case Item::TM53:
		case Item::TM54:
		case Item::TM55:
		case Item::TM56:
		case Item::TM57:
		case Item::TM58:
		case Item::TM59:
		case Item::TM60:
		case Item::TM61:
		case Item::TM62:
		case Item::TM63:
		case Item::TM64:
		case Item::TM65:
		case Item::TM66:
		case Item::TM67:
		case Item::TM68:
		case Item::TM69:
		case Item::TM70:
		case Item::TM71:
		case Item::TM72:
		case Item::TM73:
		case Item::TM74:
		case Item::TM75:
		case Item::TM76:
		case Item::TM77:
		case Item::TM78:
		case Item::TM79:
		case Item::TM80:
		case Item::TM81:
		case Item::TM82:
		case Item::TM83:
		case Item::TM84:
		case Item::TM85:
		case Item::TM86:
		case Item::TM87:
		case Item::TM88:
		case Item::TM89:
		case Item::TM90:
		case Item::TM91:
		case Item::TM92:
		case Item::Town_Map:
		case Item::Ultra_Ball:
		case Item::VS_Seeker:
		case Item::Wacan_Berry:
		case Item::Watmel_Berry:
		case Item::Wepear_Berry:
		case Item::Wiki_Berry:
		case Item::Works_Key:
		case Item::Yache_Berry:
			return 0;
		case Item::END:
			assert(false);
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
		default: return Item::END;
	}
}

Moves id_to_move (unsigned id) {
	return (id == 0) ? Moves::END : static_cast<Moves>(id + static_cast<unsigned>(Moves::Regular_Begin) - 1);
}

unsigned move_to_id (Moves move) {
	auto const move_id = static_cast<unsigned>(move) - static_cast<unsigned>(Moves::Regular_Begin) + 1u;
	assert(move_id > 0);
	return (move == Moves::END) ? 0 : move_id;
}

Nature id_to_nature(unsigned id) {
	switch (id) {
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
		default: throw std::runtime_error("Invalid nature ID in PO conversion");
	}
}

unsigned nature_to_id(Nature const nature) {
	switch (nature) {
		case Nature::Adamant: return 3;
		case Nature::Bashful: return 18;
		case Nature::Bold: return 5;
		case Nature::Brave: return 2;
		case Nature::Calm: return 20;
		case Nature::Careful: return 23;
		case Nature::Docile: return 6;
		case Nature::Gentle: return 21;
		case Nature::Hardy: return 0;
		case Nature::Hasty: return 11;
		case Nature::Impish: return 8;
		case Nature::Jolly: return 13;
		case Nature::Lax: return 9;
		case Nature::Lonely: return 1;
		case Nature::Mild: return 16;
		case Nature::Modest: return 15;
		case Nature::Naive: return 14;
		case Nature::Naughty: return 4;
		case Nature::Quiet: return 17;
		case Nature::Quirky: return 24;
		case Nature::Rash: return 19;
		case Nature::Relaxed: return 7;
		case Nature::Sassy: return 22;
		case Nature::Serious: return 12;
		case Nature::Timid: return 10;
		case Nature::END: assert(false);
	}
}
} // namespace po
} // namespace technicalmachine
