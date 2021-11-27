// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/netbattle/read_team_file.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>

#include <tm/pokemon/is_genderless.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/ingame_id_to_nature.hpp>

#include <tm/string_conversions/species.hpp>

#include <tm/bit_view.hpp>
#include <tm/bytes_in_file.hpp>
#include <tm/constant_generation.hpp>
#include <tm/generation.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/generate.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/range_view.hpp>
#include <containers/vector.hpp>

#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine::nb {
namespace {

using namespace std::string_view_literals;

enum class FileVersion { four_zero, four_one, five_zero };

using SpeciesID = bounded::integer<0, 509>;
constexpr auto id_to_species(SpeciesID const id, FileVersion const version) {
	switch (id.value()) {
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
		case 386: return Species::Deoxys_Normal;

		// Generation 4 or alternate forms in 3
		case 387: return version == FileVersion::five_zero ? Species::Turtwig : Species::Deoxys_Attack;
		case 388: return version == FileVersion::five_zero ? Species::Grotle : Species::Deoxys_Defense;
		case 389: return version == FileVersion::five_zero ? Species::Torterra : Species::Deoxys_Speed;

		// Generation 4
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
		case 413: return Species::Wormadam_Plant;
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
		case 479: return Species::Rotom;
		case 480: return Species::Uxie;
		case 481: return Species::Mesprit;
		case 482: return Species::Azelf;
		case 483: return Species::Dialga;
		case 484: return Species::Palkia;
		case 485: return Species::Heatran;
		case 486: return Species::Regigigas;
		// TODO: Giratina_Origin is handled purely by the item
		case 487: return Species::Giratina_Altered;
		case 488: return Species::Cresselia;
		case 489: return Species::Phione;
		case 490: return Species::Manaphy;
		case 491: return Species::Darkrai;
		case 492: return Species::Shaymin_Land;
		case 493: return Species::Arceus;

		case 494:
		case 495:
		case 496:
		case 497:
		case 498:
			throw std::runtime_error("Used unused Species ID");

		// Alternate forms, version 5.0
		case 499: return Species::Deoxys_Attack;
		case 500: return Species::Deoxys_Defense;
		case 501: return Species::Deoxys_Speed;
		case 502: return Species::Wormadam_Sandy;
		case 503: return Species::Wormadam_Trash;
		case 504: return Species::Rotom_Mow;
		case 505: return Species::Rotom_Frost;
		case 506: return Species::Rotom_Heat;
		case 507: return Species::Rotom_Fan;
		case 508: return Species::Rotom_Wash;
		case 509: return Species::Shaymin_Sky;

		default:
			static_assert(numeric_traits::max_value<decltype(id)> == 509);
			bounded::unreachable();
	}
}

using ItemID = bounded::integer<0, 127>;
constexpr auto id_to_item(ItemID const id, bool const prefer_gen_4_item) {
	switch (id.value()) {
		case 0: return !prefer_gen_4_item ? Item::None : Item::Micle_Berry;

		// Generation 2 mostly
		case 1: return !prefer_gen_4_item ? Item::Berry : Item::Enigma_Berry;
		case 2: return !prefer_gen_4_item ? Item::Berry_Juice : Item::Rowap_Berry;
		case 3: return !prefer_gen_4_item ? Item::Bitter_Berry : Item::Jaboca_Berry;
		case 4: return !prefer_gen_4_item ? Item::Burnt_Berry : Item::Babiri_Berry;
		case 5: return !prefer_gen_4_item ? Item::Gold_Berry : Item::Charti_Berry;
		case 6: return !prefer_gen_4_item ? Item::Ice_Berry : Item::Chilan_Berry;
		case 7: return !prefer_gen_4_item ? Item::Mint_Berry : Item::Chople_Berry;
		case 8: return !prefer_gen_4_item ? Item::MiracleBerry : Item::Coba_Berry;
		case 9: return !prefer_gen_4_item ? Item::MysteryBerry : Item::Colbur_Berry;
		case 10: return !prefer_gen_4_item ? Item::PRZCureBerry : Item::Haban_Berry;
		case 11: return !prefer_gen_4_item ? Item::PSNCureBerry : Item::Kasib_Berry;
		case 12: return !prefer_gen_4_item ? Item::Berserk_Gene : Item::Kebia_Berry;
		case 13: return !prefer_gen_4_item ? Item::Black_Belt : Item::Occa_Berry;
		case 14: return !prefer_gen_4_item ?  Item::Black_Glasses : Item::Passho_Berry;
		case 15: return !prefer_gen_4_item ?  Item::Bright_Powder : Item::Payapa_Berry;
		case 16: return !prefer_gen_4_item ?  Item::Charcoal : Item::Rindo_Berry;
		case 17: return !prefer_gen_4_item ?  Item::Dragon_Fang : Item::Shuca_Berry;
		case 18: return !prefer_gen_4_item ?  Item::Focus_Band : Item::Tanga_Berry;
		case 19: return !prefer_gen_4_item ?  Item::Hard_Stone : Item::Wacan_Berry;
		case 20: return !prefer_gen_4_item ?  Item::Kings_Rock : Item::Yache_Berry;
		case 21: return !prefer_gen_4_item ?  Item::Leftovers : Item::Flame_Plate;
		case 22: return !prefer_gen_4_item ?  Item::Light_Ball : Item::Griseous_Orb;
		case 23: return Item::Lucky_Punch;
		case 24: return Item::Magnet;
		case 25: return Item::Metal_Coat;
		case 26: return Item::Metal_Powder;
		case 27: return Item::Miracle_Seed;
		case 28: return Item::Mystic_Water;
		case 29: return Item::Never_Melt_Ice;
		case 30: return Item::Pink_Bow;
		case 31: return Item::Poison_Barb;
		case 32: return Item::Polkadot_Bow;
		case 33: return Item::Quick_Claw;
		case 34: return Item::Scope_Lens;
		case 35: return Item::Sharp_Beak;
		case 36: return Item::Silver_Powder;
		case 37: return Item::Soft_Sand;
		case 38: return Item::Spell_Tag;
		case 39: return Item::Stick;
		case 40: return Item::Thick_Club;
		case 41: return Item::Twisted_Spoon;

		// Generation 3
		case 42: return Item::Cheri_Berry;
		case 43: return Item::Chesto_Berry;
		case 44: return Item::Pecha_Berry;
		case 45: return Item::Rawst_Berry;
		case 46: return Item::Aspear_Berry;
		case 47: return Item::Leppa_Berry;
		case 48: return Item::Oran_Berry;
		case 49: return Item::Persim_Berry;
		case 50: return Item::Lum_Berry;
		case 51: return Item::Sitrus_Berry;
		case 52: return Item::Figy_Berry;
		case 53: return Item::Iapapa_Berry;
		case 54: return Item::Mago_Berry;
		case 55: return Item::Wiki_Berry;
		case 56: return Item::Aguav_Berry;
		case 57: return Item::Liechi_Berry;
		case 58: return Item::Ganlon_Berry;
		case 59: return Item::Salac_Berry;
		case 60: return Item::Petaya_Berry;
		case 61: return Item::Apicot_Berry;
		case 62: return Item::Lansat_Berry;
		case 63: return Item::Starf_Berry;
		case 64: return Item::Choice_Band;
		case 65: return Item::Deep_Sea_Scale;
		case 66: return Item::Deep_Sea_Tooth;
		case 67: return Item::Lax_Incense;
		case 68: return Item::Macho_Brace;
		case 69: return Item::Mental_Herb;
		case 70: return Item::Sea_Incense;
		case 71: return Item::Shell_Bell;
		case 72: return Item::Silk_Scarf;
		case 73: return Item::Soul_Dew;
		case 74: return Item::White_Herb;

		// Generation 4 mostly
		case 75: return Item::Adamant_Orb;
		case 76: return Item::Big_Root;
		case 77: return Item::Choice_Scarf;
		case 78: return Item::Choice_Specs;
		case 79: return Item::Damp_Rock;
		case 80: return Item::Destiny_Knot;
		case 81: return Item::Draco_Plate;
		case 82: return Item::Dread_Plate;
		case 83: return Item::Earth_Plate;
		case 84: return Item::Expert_Belt;
		case 85: return Item::Fist_Plate;
		case 86: return Item::Flame_Orb;
		case 87: return Item::Focus_Sash;
		case 88: return Item::Full_Incense;
		case 89: return Item::Grip_Claw;
		case 90: return Item::Heat_Rock;
		case 91: return Item::Icicle_Plate;
		case 92: return Item::Icy_Rock;
		case 93: return Item::Insect_Plate;
		case 94: return Item::Iron_Ball;
		case 95: return Item::Iron_Plate;
		case 96: return Item::Lagging_Tail;
		case 97: return Item::Life_Orb;
		case 98: return Item::Light_Clay;
		case 99: return Item::Lustrous_Orb;
		case 100: return Item::Meadow_Plate;
		case 101: return Item::Metronome;
		case 102: return Item::Mind_Plate;
		case 103: return Item::Muscle_Band;
		case 104: return Item::Odd_Incense;
		case 105: return Item::Power_Herb;
		case 106: return Item::Quick_Powder;
		case 107: return Item::Razor_Claw;
		case 108: return Item::Razor_Fang;
		case 109: return Item::Rock_Incense;
		case 110: return Item::Rose_Incense;
		case 111: return Item::Shed_Shell;
		case 112: return Item::Shoal_Salt;
		case 113: return Item::Sky_Plate;
		case 114: return Item::Smooth_Rock;
		case 115: return Item::Splash_Plate;
		case 116: return Item::Spooky_Plate;
		case 117: return Item::Sticky_Barb;
		case 118: return Item::Stone_Plate;
		case 119: return Item::Toxic_Orb;
		case 120: return Item::Toxic_Plate;
		case 121: return Item::Wave_Incense;
		case 122: return Item::Wide_Lens;
		case 123: return Item::Wise_Glasses;
		case 124: return Item::Zap_Plate;
		case 125: return Item::Zoom_Lens;
		case 126: return Item::Black_Sludge;
		case 127: return Item::Custap_Berry;

		default:
			static_assert(numeric_traits::max_value<decltype(id)> == 127_bi);
			bounded::unreachable();
	}
}

constexpr auto id_to_ability(Species const species, bounded::integer<0, 1> const id) -> Ability {
	auto invalid = [=]() -> Ability {
		throw std::runtime_error(containers::concatenate<std::string>(to_string(species), " does not have two abilities"sv));
	};
	switch (species) {
		case Species::Bulbasaur: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Ivysaur: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Venusaur: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Charmander: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Charmeleon: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Charizard: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Squirtle: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Wartortle: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Blastoise: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Caterpie: return id == 0_bi ? Ability::Shield_Dust : invalid();
		case Species::Metapod: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Butterfree: return id == 0_bi ? Ability::Compound_Eyes : invalid();
		case Species::Weedle: return id == 0_bi ? Ability::Shield_Dust : invalid();
		case Species::Kakuna: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Beedrill: return id == 0_bi ? Ability::Swarm : invalid();
		case Species::Pidgey: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Pidgeotto: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Pidgeot: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Rattata: return id == 0_bi ? Ability::Run_Away : Ability::Guts;
		case Species::Raticate: return id == 0_bi ? Ability::Run_Away : Ability::Guts;
		case Species::Spearow: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Fearow: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Ekans: return id == 0_bi ? Ability::Intimidate : Ability::Shed_Skin;
		case Species::Arbok: return id == 0_bi ? Ability::Intimidate : Ability::Shed_Skin;
		case Species::Pikachu: return id == 0_bi ? Ability::Static : invalid();
		case Species::Raichu: return id == 0_bi ? Ability::Static : invalid();
		case Species::Sandshrew: return id == 0_bi ? Ability::Sand_Veil : invalid();
		case Species::Sandslash: return id == 0_bi ? Ability::Sand_Veil : invalid();
		case Species::Nidoran_F: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Nidorina: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Nidoqueen: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Nidoran_M: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Nidorino: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Nidoking: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Clefairy: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Clefable: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Vulpix: return id == 0_bi ? Ability::Flash_Fire : invalid();
		case Species::Ninetales: return id == 0_bi ? Ability::Flash_Fire : invalid();
		case Species::Jigglypuff: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Wigglytuff: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Zubat: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Golbat: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Oddish: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Gloom: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Vileplume: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Paras: return id == 0_bi ? Ability::Effect_Spore : invalid();
		case Species::Parasect: return id == 0_bi ? Ability::Effect_Spore : invalid();
		case Species::Venonat: return id == 0_bi ? Ability::Compound_Eyes : invalid();
		case Species::Venomoth: return id == 0_bi ? Ability::Shield_Dust : invalid();
		case Species::Diglett: return id == 0_bi ? Ability::Sand_Veil : Ability::Arena_Trap;
		case Species::Dugtrio: return id == 0_bi ? Ability::Sand_Veil : Ability::Arena_Trap;
		case Species::Meowth: return id == 0_bi ? Ability::Pickup : invalid();
		case Species::Persian: return id == 0_bi ? Ability::Limber : invalid();
		case Species::Psyduck: return id == 0_bi ? Ability::Damp : Ability::Cloud_Nine;
		case Species::Golduck: return id == 0_bi ? Ability::Damp : Ability::Cloud_Nine;
		case Species::Mankey: return id == 0_bi ? Ability::Vital_Spirit : invalid();
		case Species::Primeape: return id == 0_bi ? Ability::Vital_Spirit : invalid();
		case Species::Growlithe: return id == 0_bi ? Ability::Intimidate : Ability::Flash_Fire;
		case Species::Arcanine: return id == 0_bi ? Ability::Intimidate : Ability::Flash_Fire;
		case Species::Poliwag: return id == 0_bi ? Ability::Water_Absorb : Ability::Damp;
		case Species::Poliwhirl: return id == 0_bi ? Ability::Water_Absorb : Ability::Damp;
		case Species::Poliwrath: return id == 0_bi ? Ability::Water_Absorb : Ability::Damp;
		case Species::Abra: return id == 0_bi ? Ability::Synchronize : Ability::Inner_Focus;
		case Species::Kadabra: return id == 0_bi ? Ability::Synchronize : Ability::Inner_Focus;
		case Species::Alakazam: return id == 0_bi ? Ability::Synchronize : Ability::Inner_Focus;
		case Species::Machop: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Machoke: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Machamp: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Bellsprout: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Weepinbell: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Victreebel: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Tentacool: return id == 0_bi ? Ability::Clear_Body : Ability::Liquid_Ooze;
		case Species::Tentacruel: return id == 0_bi ? Ability::Clear_Body : Ability::Liquid_Ooze;
		case Species::Geodude: return id == 0_bi ? Ability::Rock_Head : Ability::Sturdy;
		case Species::Graveler: return id == 0_bi ? Ability::Rock_Head : Ability::Sturdy;
		case Species::Golem: return id == 0_bi ? Ability::Rock_Head : Ability::Sturdy;
		case Species::Ponyta: return id == 0_bi ? Ability::Run_Away : Ability::Flash_Fire;
		case Species::Rapidash: return id == 0_bi ? Ability::Run_Away : Ability::Flash_Fire;
		case Species::Slowpoke: return id == 0_bi ? Ability::Oblivious : Ability::Own_Tempo;
		case Species::Slowbro: return id == 0_bi ? Ability::Oblivious : Ability::Own_Tempo;
		case Species::Magnemite: return id == 0_bi ? Ability::Magnet_Pull : Ability::Sturdy;
		case Species::Magneton: return id == 0_bi ? Ability::Magnet_Pull : Ability::Sturdy;
		case Species::Farfetchd: return id == 0_bi ? Ability::Keen_Eye : Ability::Inner_Focus;
		case Species::Doduo: return id == 0_bi ? Ability::Run_Away : Ability::Early_Bird;
		case Species::Dodrio: return id == 0_bi ? Ability::Run_Away : Ability::Early_Bird;
		case Species::Seel: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Dewgong: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Grimer: return id == 0_bi ? Ability::Stench : Ability::Sticky_Hold;
		case Species::Muk: return id == 0_bi ? Ability::Stench : Ability::Sticky_Hold;
		case Species::Shellder: return id == 0_bi ? Ability::Shell_Armor : invalid();
		case Species::Cloyster: return id == 0_bi ? Ability::Shell_Armor : invalid();
		case Species::Gastly: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Haunter: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Gengar: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Onix: return id == 0_bi ? Ability::Rock_Head : Ability::Sturdy;
		case Species::Drowzee: return id == 0_bi ? Ability::Insomnia : invalid();
		case Species::Hypno: return id == 0_bi ? Ability::Insomnia : invalid();
		case Species::Krabby: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Shell_Armor;
		case Species::Kingler: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Shell_Armor;
		case Species::Voltorb: return id == 0_bi ? Ability::Soundproof : Ability::Static;
		case Species::Electrode: return id == 0_bi ? Ability::Soundproof : Ability::Static;
		case Species::Exeggcute: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Exeggutor: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Cubone: return id == 0_bi ? Ability::Rock_Head : Ability::Lightning_Rod;
		case Species::Marowak: return id == 0_bi ? Ability::Rock_Head : Ability::Lightning_Rod;
		case Species::Hitmonlee: return id == 0_bi ? Ability::Limber : invalid();
		case Species::Hitmonchan: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Lickitung: return id == 0_bi ? Ability::Own_Tempo : Ability::Oblivious;
		case Species::Koffing: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Weezing: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Rhyhorn: return id == 0_bi ? Ability::Lightning_Rod : Ability::Rock_Head;
		case Species::Rhydon: return id == 0_bi ? Ability::Lightning_Rod : Ability::Rock_Head;
		case Species::Chansey: return id == 0_bi ? Ability::Natural_Cure : Ability::Serene_Grace;
		case Species::Tangela: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Kangaskhan: return id == 0_bi ? Ability::Early_Bird : invalid();
		case Species::Horsea: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Seadra: return id == 0_bi ? Ability::Poison_Point : invalid();
		case Species::Goldeen: return id == 0_bi ? Ability::Swift_Swim : Ability::Water_Veil;
		case Species::Seaking: return id == 0_bi ? Ability::Swift_Swim : Ability::Water_Veil;
		case Species::Staryu: return id == 0_bi ? Ability::Illuminate : Ability::Natural_Cure;
		case Species::Starmie: return id == 0_bi ? Ability::Illuminate : Ability::Natural_Cure;
		case Species::Mr_Mime: return id == 0_bi ? Ability::Soundproof : invalid();
		case Species::Scyther: return id == 0_bi ? Ability::Swarm : invalid();
		case Species::Jynx: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Electabuzz: return id == 0_bi ? Ability::Static : invalid();
		case Species::Magmar: return id == 0_bi ? Ability::Flame_Body : invalid();
		case Species::Pinsir: return id == 0_bi ? Ability::Hyper_Cutter : invalid();
		case Species::Tauros: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Magikarp: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Gyarados: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Lapras: return id == 0_bi ? Ability::Water_Absorb : Ability::Shell_Armor;
		case Species::Ditto: return id == 0_bi ? Ability::Limber : invalid();
		case Species::Eevee: return id == 0_bi ? Ability::Run_Away : invalid();
		case Species::Vaporeon: return id == 0_bi ? Ability::Water_Absorb : invalid();
		case Species::Jolteon: return id == 0_bi ? Ability::Volt_Absorb : invalid();
		case Species::Flareon: return id == 0_bi ? Ability::Flash_Fire : invalid();
		case Species::Porygon: return id == 0_bi ? Ability::Trace : invalid();
		case Species::Omanyte: return id == 0_bi ? Ability::Swift_Swim : Ability::Shell_Armor;
		case Species::Omastar: return id == 0_bi ? Ability::Swift_Swim : Ability::Shell_Armor;
		case Species::Kabuto: return id == 0_bi ? Ability::Swift_Swim : Ability::Battle_Armor;
		case Species::Kabutops: return id == 0_bi ? Ability::Swift_Swim : Ability::Battle_Armor;
		case Species::Aerodactyl: return id == 0_bi ? Ability::Rock_Head : Ability::Pressure;
		case Species::Snorlax: return id == 0_bi ? Ability::Immunity : Ability::Thick_Fat;
		case Species::Articuno: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Zapdos: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Moltres: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Dratini: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Dragonair: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Dragonite: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Mewtwo: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Mew: return id == 0_bi ? Ability::Synchronize : invalid();
		case Species::Chikorita: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Bayleef: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Meganium: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Cyndaquil: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Quilava: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Typhlosion: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Totodile: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Croconaw: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Feraligatr: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Sentret: return id == 0_bi ? Ability::Run_Away : Ability::Keen_Eye;
		case Species::Furret: return id == 0_bi ? Ability::Run_Away : Ability::Keen_Eye;
		case Species::Hoothoot: return id == 0_bi ? Ability::Insomnia : Ability::Keen_Eye;
		case Species::Noctowl: return id == 0_bi ? Ability::Insomnia : Ability::Keen_Eye;
		case Species::Ledyba: return id == 0_bi ? Ability::Swarm : Ability::Early_Bird;
		case Species::Ledian: return id == 0_bi ? Ability::Swarm : Ability::Early_Bird;
		case Species::Spinarak: return id == 0_bi ? Ability::Swarm : Ability::Insomnia;
		case Species::Ariados: return id == 0_bi ? Ability::Swarm : Ability::Insomnia;
		case Species::Crobat: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Chinchou: return id == 0_bi ? Ability::Volt_Absorb : Ability::Illuminate;
		case Species::Lanturn: return id == 0_bi ? Ability::Volt_Absorb : Ability::Illuminate;
		case Species::Pichu: return id == 0_bi ? Ability::Static : invalid();
		case Species::Cleffa: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Igglybuff: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Togepi: return id == 0_bi ? Ability::Hustle : Ability::Serene_Grace;
		case Species::Togetic: return id == 0_bi ? Ability::Hustle : Ability::Serene_Grace;
		case Species::Natu: return id == 0_bi ? Ability::Synchronize : Ability::Early_Bird;
		case Species::Xatu: return id == 0_bi ? Ability::Synchronize : Ability::Early_Bird;
		case Species::Mareep: return id == 0_bi ? Ability::Static : invalid();
		case Species::Flaaffy: return id == 0_bi ? Ability::Static : invalid();
		case Species::Ampharos: return id == 0_bi ? Ability::Static : invalid();
		case Species::Bellossom: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Marill: return id == 0_bi ? Ability::Thick_Fat : Ability::Huge_Power;
		case Species::Azumarill: return id == 0_bi ? Ability::Thick_Fat : Ability::Huge_Power;
		case Species::Sudowoodo: return id == 0_bi ? Ability::Sturdy : Ability::Rock_Head;
		case Species::Politoed: return id == 0_bi ? Ability::Water_Absorb : Ability::Damp;
		case Species::Hoppip: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Skiploom: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Jumpluff: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Aipom: return id == 0_bi ? Ability::Run_Away : Ability::Pickup;
		case Species::Sunkern: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Sunflora: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Yanma: return id == 0_bi ? Ability::Speed_Boost : Ability::Compound_Eyes;
		case Species::Wooper: return id == 0_bi ? Ability::Damp : Ability::Water_Absorb;
		case Species::Quagsire: return id == 0_bi ? Ability::Damp : Ability::Water_Absorb;
		case Species::Espeon: return id == 0_bi ? Ability::Synchronize : invalid();
		case Species::Umbreon: return id == 0_bi ? Ability::Synchronize : invalid();
		case Species::Murkrow: return id == 0_bi ? Ability::Insomnia : invalid();
		case Species::Slowking: return id == 0_bi ? Ability::Oblivious : Ability::Own_Tempo;
		case Species::Misdreavus: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Unown: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Wobbuffet: return id == 0_bi ? Ability::Shadow_Tag : invalid();
		case Species::Girafarig: return id == 0_bi ? Ability::Inner_Focus : Ability::Early_Bird;
		case Species::Pineco: return id == 0_bi ? Ability::Sturdy : invalid();
		case Species::Forretress: return id == 0_bi ? Ability::Sturdy : invalid();
		case Species::Dunsparce: return id == 0_bi ? Ability::Serene_Grace : Ability::Run_Away;
		case Species::Gligar: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Sand_Veil;
		case Species::Steelix: return id == 0_bi ? Ability::Rock_Head : Ability::Sturdy;
		case Species::Snubbull: return id == 0_bi ? Ability::Intimidate : Ability::Run_Away;
		case Species::Granbull: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Qwilfish: return id == 0_bi ? Ability::Poison_Point : Ability::Swift_Swim;
		case Species::Scizor: return id == 0_bi ? Ability::Swarm : invalid();
		case Species::Shuckle: return id == 0_bi ? Ability::Sturdy : invalid();
		case Species::Heracross: return id == 0_bi ? Ability::Swarm : Ability::Guts;
		case Species::Sneasel: return id == 0_bi ? Ability::Inner_Focus : Ability::Keen_Eye;
		case Species::Teddiursa: return id == 0_bi ? Ability::Pickup : invalid();
		case Species::Ursaring: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Slugma: return id == 0_bi ? Ability::Magma_Armor : Ability::Flame_Body;
		case Species::Magcargo: return id == 0_bi ? Ability::Magma_Armor : Ability::Flame_Body;
		case Species::Swinub: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Piloswine: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Corsola: return id == 0_bi ? Ability::Hustle : Ability::Natural_Cure;
		case Species::Remoraid: return id == 0_bi ? Ability::Hustle : invalid();
		case Species::Octillery: return id == 0_bi ? Ability::Suction_Cups : invalid();
		case Species::Delibird: return id == 0_bi ? Ability::Vital_Spirit : Ability::Hustle;
		case Species::Mantine: return id == 0_bi ? Ability::Swift_Swim : Ability::Water_Absorb;
		case Species::Skarmory: return id == 0_bi ? Ability::Keen_Eye : Ability::Sturdy;
		case Species::Houndour: return id == 0_bi ? Ability::Early_Bird : Ability::Flash_Fire;
		case Species::Houndoom: return id == 0_bi ? Ability::Early_Bird : Ability::Flash_Fire;
		case Species::Kingdra: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Phanpy: return id == 0_bi ? Ability::Pickup : invalid();
		case Species::Donphan: return id == 0_bi ? Ability::Sturdy : invalid();
		case Species::Porygon2: return id == 0_bi ? Ability::Trace : invalid();
		case Species::Stantler: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Smeargle: return id == 0_bi ? Ability::Own_Tempo : invalid();
		case Species::Tyrogue: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Hitmontop: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Smoochum: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Elekid: return id == 0_bi ? Ability::Static : invalid();
		case Species::Magby: return id == 0_bi ? Ability::Flame_Body : invalid();
		case Species::Miltank: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Blissey: return id == 0_bi ? Ability::Natural_Cure : Ability::Serene_Grace;
		case Species::Raikou: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Entei: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Suicune: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Larvitar: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Pupitar: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Tyranitar: return id == 0_bi ? Ability::Sand_Stream : invalid();
		case Species::Lugia: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Ho_Oh: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Celebi: return id == 0_bi ? Ability::Natural_Cure : invalid();
		case Species::Treecko: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Grovyle: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Sceptile: return id == 0_bi ? Ability::Overgrow : invalid();
		case Species::Torchic: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Combusken: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Blaziken: return id == 0_bi ? Ability::Blaze : invalid();
		case Species::Mudkip: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Marshtomp: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Swampert: return id == 0_bi ? Ability::Torrent : invalid();
		case Species::Poochyena: return id == 0_bi ? Ability::Run_Away : invalid();
		case Species::Mightyena: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Zigzagoon: return id == 0_bi ? Ability::Pickup : invalid();
		case Species::Linoone: return id == 0_bi ? Ability::Pickup : invalid();
		case Species::Wurmple: return id == 0_bi ? Ability::Shield_Dust : invalid();
		case Species::Silcoon: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Beautifly: return id == 0_bi ? Ability::Swarm : invalid();
		case Species::Cascoon: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Dustox: return id == 0_bi ? Ability::Shield_Dust : invalid();
		case Species::Lotad: return id == 0_bi ? Ability::Swift_Swim : Ability::Rain_Dish;
		case Species::Lombre: return id == 0_bi ? Ability::Swift_Swim : Ability::Rain_Dish;
		case Species::Ludicolo: return id == 0_bi ? Ability::Swift_Swim : Ability::Rain_Dish;
		case Species::Seedot: return id == 0_bi ? Ability::Chlorophyll : Ability::Early_Bird;
		case Species::Nuzleaf: return id == 0_bi ? Ability::Chlorophyll : Ability::Early_Bird;
		case Species::Shiftry: return id == 0_bi ? Ability::Chlorophyll : Ability::Early_Bird;
		case Species::Taillow: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Swellow: return id == 0_bi ? Ability::Guts : invalid();
		case Species::Wingull: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Pelipper: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Ralts: return id == 0_bi ? Ability::Synchronize : Ability::Trace;
		case Species::Kirlia: return id == 0_bi ? Ability::Synchronize : Ability::Trace;
		case Species::Gardevoir: return id == 0_bi ? Ability::Synchronize : Ability::Trace;
		case Species::Surskit: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Masquerain: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Shroomish: return id == 0_bi ? Ability::Effect_Spore : invalid();
		case Species::Breloom: return id == 0_bi ? Ability::Effect_Spore : invalid();
		case Species::Slakoth: return id == 0_bi ? Ability::Truant : invalid();
		case Species::Vigoroth: return id == 0_bi ? Ability::Vital_Spirit : invalid();
		case Species::Slaking: return id == 0_bi ? Ability::Truant : invalid();
		case Species::Nincada: return id == 0_bi ? Ability::Compound_Eyes : invalid();
		case Species::Ninjask: return id == 0_bi ? Ability::Speed_Boost : invalid();
		case Species::Shedinja: return id == 0_bi ? Ability::Wonder_Guard : invalid();
		case Species::Whismur: return id == 0_bi ? Ability::Soundproof : invalid();
		case Species::Loudred: return id == 0_bi ? Ability::Soundproof : invalid();
		case Species::Exploud: return id == 0_bi ? Ability::Soundproof : invalid();
		case Species::Makuhita: return id == 0_bi ? Ability::Thick_Fat : Ability::Guts;
		case Species::Hariyama: return id == 0_bi ? Ability::Thick_Fat : Ability::Guts;
		case Species::Azurill: return id == 0_bi ? Ability::Thick_Fat : Ability::Huge_Power;
		case Species::Nosepass: return id == 0_bi ? Ability::Sturdy : Ability::Magnet_Pull;
		case Species::Skitty: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Delcatty: return id == 0_bi ? Ability::Cute_Charm : invalid();
		case Species::Sableye: return id == 0_bi ? Ability::Keen_Eye : invalid();
		case Species::Mawile: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Intimidate;
		case Species::Aron: return id == 0_bi ? Ability::Sturdy : Ability::Rock_Head;
		case Species::Lairon: return id == 0_bi ? Ability::Sturdy : Ability::Rock_Head;
		case Species::Aggron: return id == 0_bi ? Ability::Sturdy : Ability::Rock_Head;
		case Species::Meditite: return id == 0_bi ? Ability::Pure_Power : invalid();
		case Species::Medicham: return id == 0_bi ? Ability::Pure_Power : invalid();
		case Species::Electrike: return id == 0_bi ? Ability::Static : Ability::Lightning_Rod;
		case Species::Manectric: return id == 0_bi ? Ability::Static : Ability::Lightning_Rod;
		case Species::Plusle: return id == 0_bi ? Ability::Plus : invalid();
		case Species::Minun: return id == 0_bi ? Ability::Minus : invalid();
		case Species::Volbeat: return id == 0_bi ? Ability::Illuminate : Ability::Swarm;
		case Species::Illumise: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Roselia: return id == 0_bi ? Ability::Natural_Cure : Ability::Poison_Point;
		case Species::Gulpin: return id == 0_bi ? Ability::Liquid_Ooze : Ability::Sticky_Hold;
		case Species::Swalot: return id == 0_bi ? Ability::Liquid_Ooze : Ability::Sticky_Hold;
		case Species::Carvanha: return id == 0_bi ? Ability::Rough_Skin : invalid();
		case Species::Sharpedo: return id == 0_bi ? Ability::Rough_Skin : invalid();
		case Species::Wailmer: return id == 0_bi ? Ability::Water_Veil : Ability::Oblivious;
		case Species::Wailord: return id == 0_bi ? Ability::Water_Veil : Ability::Oblivious;
		case Species::Numel: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Camerupt: return id == 0_bi ? Ability::Magma_Armor : invalid();
		case Species::Torkoal: return id == 0_bi ? Ability::White_Smoke : invalid();
		case Species::Spoink: return id == 0_bi ? Ability::Thick_Fat : Ability::Own_Tempo;
		case Species::Grumpig: return id == 0_bi ? Ability::Thick_Fat : Ability::Own_Tempo;
		case Species::Spinda: return id == 0_bi ? Ability::Own_Tempo : invalid();
		case Species::Trapinch: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Arena_Trap;
		case Species::Vibrava: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Flygon: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Cacnea: return id == 0_bi ? Ability::Sand_Veil : invalid();
		case Species::Cacturne: return id == 0_bi ? Ability::Sand_Veil : invalid();
		case Species::Swablu: return id == 0_bi ? Ability::Natural_Cure : invalid();
		case Species::Altaria: return id == 0_bi ? Ability::Natural_Cure : invalid();
		case Species::Zangoose: return id == 0_bi ? Ability::Immunity : invalid();
		case Species::Seviper: return id == 0_bi ? Ability::Shed_Skin : invalid();
		case Species::Lunatone: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Solrock: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Barboach: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Whiscash: return id == 0_bi ? Ability::Oblivious : invalid();
		case Species::Corphish: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Shell_Armor;
		case Species::Crawdaunt: return id == 0_bi ? Ability::Hyper_Cutter : Ability::Shell_Armor;
		case Species::Baltoy: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Claydol: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Lileep: return id == 0_bi ? Ability::Suction_Cups : invalid();
		case Species::Cradily: return id == 0_bi ? Ability::Suction_Cups : invalid();
		case Species::Anorith: return id == 0_bi ? Ability::Battle_Armor : invalid();
		case Species::Armaldo: return id == 0_bi ? Ability::Battle_Armor : invalid();
		case Species::Feebas: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Milotic: return id == 0_bi ? Ability::Marvel_Scale : invalid();
		case Species::Castform: return id == 0_bi ? Ability::Forecast : invalid();
		case Species::Kecleon: return id == 0_bi ? Ability::Color_Change : invalid();
		case Species::Shuppet: return id == 0_bi ? Ability::Insomnia : invalid();
		case Species::Banette: return id == 0_bi ? Ability::Insomnia : invalid();
		case Species::Duskull: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Dusclops: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Tropius: return id == 0_bi ? Ability::Chlorophyll : invalid();
		case Species::Chimecho: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Absol: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Wynaut: return id == 0_bi ? Ability::Shadow_Tag : invalid();
		case Species::Snorunt: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Glalie: return id == 0_bi ? Ability::Inner_Focus : invalid();
		case Species::Spheal: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Sealeo: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Walrein: return id == 0_bi ? Ability::Thick_Fat : invalid();
		case Species::Clamperl: return id == 0_bi ? Ability::Shell_Armor : invalid();
		case Species::Huntail: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Gorebyss: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Relicanth: return id == 0_bi ? Ability::Swift_Swim : Ability::Rock_Head;
		case Species::Luvdisc: return id == 0_bi ? Ability::Swift_Swim : invalid();
		case Species::Bagon: return id == 0_bi ? Ability::Rock_Head : invalid();
		case Species::Shelgon: return id == 0_bi ? Ability::Rock_Head : invalid();
		case Species::Salamence: return id == 0_bi ? Ability::Intimidate : invalid();
		case Species::Beldum: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Metang: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Metagross: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Regirock: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Regice: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Registeel: return id == 0_bi ? Ability::Clear_Body : invalid();
		case Species::Latias: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Latios: return id == 0_bi ? Ability::Levitate : invalid();
		case Species::Kyogre: return id == 0_bi ? Ability::Drizzle : invalid();
		case Species::Groudon: return id == 0_bi ? Ability::Drought : invalid();
		case Species::Rayquaza: return id == 0_bi ? Ability::Air_Lock : invalid();
		case Species::Jirachi: return id == 0_bi ? Ability::Serene_Grace : invalid();
		case Species::Deoxys_Normal: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Deoxys_Attack: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Deoxys_Defense: return id == 0_bi ? Ability::Pressure : invalid();
		case Species::Deoxys_Speed: return id == 0_bi ? Ability::Pressure : invalid();
		default: throw std::runtime_error(containers::concatenate<std::string>(to_string(species), " not supported in NetBattle team files"sv));
	}
}

constexpr auto id_to_gender(Species const species, bounded::integer<0, 1> const id) -> Gender {
	if (is_genderless(species)) {
		if (id == 1_bi) {
			throw std::runtime_error(containers::concatenate<std::string>("Tried to give a gender to "sv, to_string(species)));
		}
		return Gender::genderless;
	}
	switch (id.value()) {
		case 0: return Gender::male;
		case 1: return Gender::female;
		default: bounded::unreachable();
	}
}

using MoveID = bounded::integer<0, 467>;
constexpr auto id_to_move(MoveID const id) -> bounded::optional<Moves> {
	switch (id.value()) {
		case 0: return bounded::none;

		// Generation 1 and 2
		case 1: return Moves::Absorb;
		case 2: return Moves::Acid;
		case 3: return Moves::Acid_Armor;
		case 4: return Moves::Aeroblast;
		case 5: return Moves::Agility;
		case 6: return Moves::Amnesia;
		case 7: return Moves::Ancient_Power;
		case 8: return Moves::Attract;
		case 9: return Moves::Aurora_Beam;
		case 10: return Moves::Barrage;
		case 11: return Moves::Barrier;
		case 12: return Moves::Baton_Pass;
		case 13: return Moves::Beat_Up;
		case 14: return Moves::Belly_Drum;
		case 15: return Moves::Bide;
		case 16: return Moves::Bind;
		case 17: return Moves::Bite;
		case 18: return Moves::Blizzard;
		case 19: return Moves::Body_Slam;
		case 20: return Moves::Bone_Club;
		case 21: return Moves::Bone_Rush;
		case 22: return Moves::Bonemerang;
		case 23: return Moves::Bubble;
		case 24: return Moves::Bubble_Beam;
		case 25: return Moves::Charm;
		case 26: return Moves::Clamp;
		case 27: return Moves::Comet_Punch;
		case 28: return Moves::Confuse_Ray;
		case 29: return Moves::Confusion;
		case 30: return Moves::Constrict;
		case 31: return Moves::Conversion;
		case 32: return Moves::Conversion_2;
		case 33: return Moves::Cotton_Spore;
		case 34: return Moves::Counter;
		case 35: return Moves::Crabhammer;
		case 36: return Moves::Cross_Chop;
		case 37: return Moves::Crunch;
		case 38: return Moves::Curse;
		case 39: return Moves::Cut;
		case 40: return Moves::Defense_Curl;
		case 41: return Moves::Destiny_Bond;
		case 42: return Moves::Detect;
		case 43: return Moves::Dig;
		case 44: return Moves::Disable;
		case 45: return Moves::Dizzy_Punch;
		case 46: return Moves::Double_Kick;
		case 47: return Moves::Double_Team;
		case 48: return Moves::Double_Edge;
		case 49: return Moves::Double_Slap;
		case 50: return Moves::Dragon_Rage;
		case 51: return Moves::Dragon_Breath;
		case 52: return Moves::Dream_Eater;
		case 53: return Moves::Drill_Peck;
		case 54: return Moves::Dynamic_Punch;
		case 55: return Moves::Earthquake;
		case 56: return Moves::Egg_Bomb;
		case 57: return Moves::Ember;
		case 58: return Moves::Encore;
		case 59: return Moves::Endure;
		case 60: return Moves::Explosion;
		case 61: return Moves::Extreme_Speed;
		case 62: return Moves::Feint_Attack;
		case 63: return Moves::False_Swipe;
		case 64: return Moves::Fire_Blast;
		case 65: return Moves::Fire_Punch;
		case 66: return Moves::Fire_Spin;
		case 67: return Moves::Fissure;
		case 68: return Moves::Flail;
		case 69: return Moves::Flame_Wheel;
		case 70: return Moves::Flamethrower;
		case 71: return Moves::Flash;
		case 72: return Moves::Fly;
		case 73: return Moves::Focus_Energy;
		case 74: return Moves::Foresight;
		case 75: return Moves::Frustration;
		case 76: return Moves::Fury_Attack;
		case 77: return Moves::Fury_Cutter;
		case 78: return Moves::Fury_Swipes;
		case 79: return Moves::Future_Sight;
		case 80: return Moves::Giga_Drain;
		case 81: return Moves::Glare;
		case 82: return Moves::Growl;
		case 83: return Moves::Growth;
		case 84: return Moves::Guillotine;
		case 85: return Moves::Gust;
		case 86: return Moves::Harden;
		case 87: return Moves::Haze;
		case 88: return Moves::Headbutt;
		case 89: return Moves::Heal_Bell;
		case 90: return Moves::High_Jump_Kick;
		case 91: return Moves::Hidden_Power;
		case 92: return Moves::Horn_Attack;
		case 93: return Moves::Horn_Drill;
		case 94: return Moves::Hydro_Pump;
		case 95: return Moves::Hyper_Beam;
		case 96: return Moves::Hyper_Fang;
		case 97: return Moves::Hypnosis;
		case 98: return Moves::Ice_Beam;
		case 99: return Moves::Ice_Punch;
		case 100: return Moves::Icy_Wind;
		case 101: return Moves::Iron_Tail;
		case 102: return Moves::Jump_Kick;
		case 103: return Moves::Karate_Chop;
		case 104: return Moves::Kinesis;
		case 105: return Moves::Leech_Life;
		case 106: return Moves::Leech_Seed;
		case 107: return Moves::Leer;
		case 108: return Moves::Lick;
		case 109: return Moves::Light_Screen;
		case 110: return Moves::Lock_On;
		case 111: return Moves::Lovely_Kiss;
		case 112: return Moves::Low_Kick;
		case 113: return Moves::Mach_Punch;
		case 114: return Moves::Magnitude;
		case 115: return Moves::Mean_Look;
		case 116: return Moves::Meditate;
		case 117: return Moves::Mega_Drain;
		case 118: return Moves::Mega_Kick;
		case 119: return Moves::Mega_Punch;
		case 120: return Moves::Megahorn;
		case 121: return Moves::Metal_Claw;
		case 122: return Moves::Metronome;
		case 123: return Moves::Milk_Drink;
		case 124: return Moves::Mimic;
		case 125: return Moves::Mind_Reader;
		case 126: return Moves::Minimize;
		case 127: return Moves::Mirror_Coat;
		case 128: return Moves::Mirror_Move;
		case 129: return Moves::Mist;
		case 130: return Moves::Moonlight;
		case 131: return Moves::Morning_Sun;
		case 132: return Moves::Mud_Slap;
		case 133: return Moves::Night_Shade;
		case 134: return Moves::Nightmare;
		case 135: return Moves::Octazooka;
		case 136: return Moves::Outrage;
		case 137: return Moves::Pain_Split;
		case 138: return Moves::Pay_Day;
		case 139: return Moves::Peck;
		case 140: return Moves::Perish_Song;
		case 141: return Moves::Petal_Dance;
		case 142: return Moves::Pin_Missile;
		case 143: return Moves::Poison_Gas;
		case 144: return Moves::Poison_Sting;
		case 145: return Moves::Poison_Powder;
		case 146: return Moves::Pound;
		case 147: return Moves::Powder_Snow;
		case 148: return Moves::Present;
		case 149: return Moves::Protect;
		case 150: return Moves::Psybeam;
		case 151: return Moves::Psych_Up;
		case 152: return Moves::Psychic;
		case 153: return Moves::Psywave;
		case 154: return Moves::Pursuit;
		case 155: return Moves::Quick_Attack;
		case 156: return Moves::Rage;
		case 157: return Moves::Rain_Dance;
		case 158: return Moves::Rapid_Spin;
		case 159: return Moves::Razor_Leaf;
		case 160: return Moves::Razor_Wind;
		case 161: return Moves::Recover;
		case 162: return Moves::Reflect;
		case 163: return Moves::Rest;
		case 164: return Moves::Return;
		case 165: return Moves::Reversal;
		case 166: return Moves::Roar;
		case 167: return Moves::Rock_Slide;
		case 168: return Moves::Rock_Smash;
		case 169: return Moves::Rock_Throw;
		case 170: return Moves::Rolling_Kick;
		case 171: return Moves::Rollout;
		case 172: return Moves::Sacred_Fire;
		case 173: return Moves::Safeguard;
		case 174: return Moves::Sandstorm;
		case 175: return Moves::Sand_Attack;
		case 176: return Moves::Scary_Face;
		case 177: return Moves::Scratch;
		case 178: return Moves::Screech;
		case 179: return Moves::Seismic_Toss;
		case 180: return Moves::Self_Destruct;
		case 181: return Moves::Shadow_Ball;
		case 182: return Moves::Sharpen;
		case 183: return Moves::Sing;
		case 184: return Moves::Sketch;
		case 185: return Moves::Skull_Bash;
		case 186: return Moves::Sky_Attack;
		case 187: return Moves::Slam;
		case 188: return Moves::Slash;
		case 189: return Moves::Sleep_Powder;
		case 190: return Moves::Sleep_Talk;
		case 191: return Moves::Sludge;
		case 192: return Moves::Sludge_Bomb;
		case 193: return Moves::Smog;
		case 194: return Moves::Smokescreen;
		case 195: return Moves::Snore;
		case 196: return Moves::Soft_Boiled;
		case 197: return Moves::Solar_Beam;
		case 198: return Moves::Sonic_Boom;
		case 199: return Moves::Spark;
		case 200: return Moves::Spider_Web;
		case 201: return Moves::Spike_Cannon;
		case 202: return Moves::Spikes;
		case 203: return Moves::Spite;
		case 204: return Moves::Splash;
		case 205: return Moves::Spore;
		case 206: return Moves::Steel_Wing;
		case 207: return Moves::Stomp;
		case 208: return Moves::Strength;
		case 209: return Moves::String_Shot;
		case 210: return Moves::Struggle;
		case 211: return Moves::Stun_Spore;
		case 212: return Moves::Submission;
		case 213: return Moves::Substitute;
		case 214: return Moves::Sunny_Day;
		case 215: return Moves::Super_Fang;
		case 216: return Moves::Supersonic;
		case 217: return Moves::Surf;
		case 218: return Moves::Swagger;
		case 219: return Moves::Sweet_Kiss;
		case 220: return Moves::Sweet_Scent;
		case 221: return Moves::Swift;
		case 222: return Moves::Swords_Dance;
		case 223: return Moves::Synthesis;
		case 224: return Moves::Tackle;
		case 225: return Moves::Tail_Whip;
		case 226: return Moves::Take_Down;
		case 227: return Moves::Teleport;
		case 228: return Moves::Thief;
		case 229: return Moves::Thrash;
		case 230: return Moves::Thunder;
		case 231: return Moves::Thunder_Wave;
		case 232: return Moves::Thunderbolt;
		case 233: return Moves::Thunder_Punch;
		case 234: return Moves::Thunder_Shock;
		case 235: return Moves::Toxic;
		case 236: return Moves::Transform;
		case 237: return Moves::Tri_Attack;
		case 238: return Moves::Triple_Kick;
		case 239: return Moves::Twineedle;
		case 240: return Moves::Twister;
		case 241: return Moves::Vice_Grip;
		case 242: return Moves::Vine_Whip;
		case 243: return Moves::Vital_Throw;
		case 244: return Moves::Water_Gun;
		case 245: return Moves::Waterfall;
		case 246: return Moves::Whirlpool;
		case 247: return Moves::Whirlwind;
		case 248: return Moves::Wing_Attack;
		case 249: return Moves::Withdraw;
		case 250: return Moves::Wrap;
		case 251: return Moves::Zap_Cannon;

		// Generation 3
		case 252: return Moves::Aerial_Ace;
		case 253: return Moves::Air_Cutter;
		case 254: return Moves::Arm_Thrust;
		case 255: return Moves::Aromatherapy;
		case 256: return Moves::Assist;
		case 257: return Moves::Astonish;
		case 258: return Moves::Blast_Burn;
		case 259: return Moves::Blaze_Kick;
		case 260: return Moves::Block;
		case 261: return Moves::Bounce;
		case 262: return Moves::Brick_Break;
		case 263: return Moves::Bulk_Up;
		case 264: return Moves::Bullet_Seed;
		case 265: return Moves::Calm_Mind;
		case 266: return Moves::Camouflage;
		case 267: return Moves::Charge;
		case 268: return Moves::Cosmic_Power;
		case 269: return Moves::Covet;
		case 270: return Moves::Crush_Claw;
		case 271: return Moves::Dive;
		case 272: return Moves::Doom_Desire;
		case 273: return Moves::Dragon_Claw;
		case 274: return Moves::Dragon_Dance;
		case 275: return Moves::Endeavor;
		case 276: return Moves::Eruption;
		case 277: return Moves::Extrasensory;
		case 278: return Moves::Facade;
		case 279: return Moves::Fake_Out;
		case 280: return Moves::Fake_Tears;
		case 281: return Moves::Feather_Dance;
		case 282: return Moves::Flatter;
		case 283: return Moves::Focus_Punch;
		case 284: return Moves::Follow_Me;
		case 285: return Moves::Frenzy_Plant;
		case 286: return Moves::Grass_Whistle;
		case 287: return Moves::Grudge;
		case 288: return Moves::Hail;
		case 289: return Moves::Heat_Wave;
		case 290: return Moves::Helping_Hand;
		case 291: return Moves::Howl;
		case 292: return Moves::Hydro_Cannon;
		case 293: return Moves::Hyper_Voice;
		case 294: return Moves::Ice_Ball;
		case 295: return Moves::Icicle_Spear;
		case 296: return Moves::Imprison;
		case 297: return Moves::Ingrain;
		case 298: return Moves::Iron_Defense;
		case 299: return Moves::Knock_Off;
		case 300: return Moves::Leaf_Blade;
		case 301: return Moves::Luster_Purge;
		case 302: return Moves::Magic_Coat;
		case 303: return Moves::Magical_Leaf;
		case 304: return Moves::Memento;
		case 305: return Moves::Metal_Sound;
		case 306: return Moves::Meteor_Mash;
		case 307: return Moves::Mist_Ball;
		case 308: return Moves::Mud_Shot;
		case 309: return Moves::Mud_Sport;
		case 310: return Moves::Muddy_Water;
		case 311: return Moves::Nature_Power;
		case 312: return Moves::Needle_Arm;
		case 313: return Moves::Odor_Sleuth;
		case 314: return Moves::Overheat;
		case 315: return Moves::Poison_Fang;
		case 316: return Moves::Poison_Tail;
		case 317: return Moves::Psycho_Boost;
		case 318: return Moves::Recycle;
		case 319: return Moves::Refresh;
		case 320: return Moves::Revenge;
		case 321: return Moves::Rock_Blast;
		case 322: return Moves::Rock_Tomb;
		case 323: return Moves::Role_Play;
		case 324: return Moves::Sand_Tomb;
		case 325: return Moves::Secret_Power;
		case 326: return Moves::Shadow_Punch;
		case 327: return Moves::Sheer_Cold;
		case 328: return Moves::Shock_Wave;
		case 329: return Moves::Signal_Beam;
		case 330: return Moves::Silver_Wind;
		case 331: return Moves::Skill_Swap;
		case 332: return Moves::Sky_Uppercut;
		case 333: return Moves::Slack_Off;
		case 334: return Moves::Smelling_Salts;
		case 335: return Moves::Snatch;
		case 336: return Moves::Spit_Up;
		case 337: return Moves::Stockpile;
		case 338: return Moves::Superpower;
		case 339: return Moves::Swallow;
		case 340: return Moves::Tail_Glow;
		case 341: return Moves::Taunt;
		case 342: return Moves::Teeter_Dance;
		case 343: return Moves::Tickle;
		case 344: return Moves::Torment;
		case 345: return Moves::Trick;
		case 346: return Moves::Uproar;
		case 347: return Moves::Volt_Tackle;
		case 348: return Moves::Water_Pulse;
		case 349: return Moves::Water_Sport;
		case 350: return Moves::Water_Spout;
		case 351: return Moves::Weather_Ball;
		case 352: return Moves::Will_O_Wisp;
		case 353: return Moves::Wish;
		case 354: return Moves::Yawn;

		// Generation 4
		case 355: return Moves::Acupressure;
		case 356: return Moves::Air_Slash;
		case 357: return Moves::Aqua_Jet;
		case 358: return Moves::Aqua_Ring;
		case 359: return Moves::Aqua_Tail;
		case 360: return Moves::Assurance;
		case 361: return Moves::Attack_Order;
		case 362: return Moves::Aura_Sphere;
		case 363: return Moves::Avalanche;
		case 364: return Moves::Brave_Bird;
		case 365: return Moves::Brine;
		case 366: return Moves::Bug_Bite;
		case 367: return Moves::Bug_Buzz;
		case 368: return Moves::Bullet_Punch;
		case 369: return Moves::Captivate;
		case 370: return Moves::Charge_Beam;
		case 371: return Moves::Chatter;
		case 372: return Moves::Close_Combat;
		case 373: return Moves::Copycat;
		case 374: return Moves::Cross_Poison;
		case 375: return Moves::Crush_Grip;
		case 376: return Moves::Dark_Pulse;
		case 377: return Moves::Dark_Void;
		case 378: return Moves::Defend_Order;
		case 379: return Moves::Defog;
		case 380: return Moves::Discharge;
		case 381: return Moves::Double_Hit;
		case 382: return Moves::Draco_Meteor;
		case 383: return Moves::Dragon_Pulse;
		case 384: return Moves::Dragon_Rush;
		case 385: return Moves::Drain_Punch;
		case 386: return Moves::Earth_Power;
		case 387: return Moves::Embargo;
		case 388: return Moves::Energy_Ball;
		case 389: return Moves::Feint;
		case 390: return Moves::Fire_Fang;
		case 391: return Moves::Flare_Blitz;
		case 392: return Moves::Flash_Cannon;
		case 393: return Moves::Fling;
		case 394: return Moves::Focus_Blast;
		case 395: return Moves::Force_Palm;
		case 396: return Moves::Gastro_Acid;
		case 397: return Moves::Giga_Impact;
		case 398: return Moves::Grass_Knot;
		case 399: return Moves::Gravity;
		case 400: return Moves::Guard_Swap;
		case 401: return Moves::Gunk_Shot;
		case 402: return Moves::Gyro_Ball;
		case 403: return Moves::Hammer_Arm;
		case 404: return Moves::Head_Smash;
		case 405: return Moves::Heal_Block;
		case 406: return Moves::Heal_Order;
		case 407: return Moves::Healing_Wish;
		case 408: return Moves::Heart_Swap;
		case 409: return Moves::Ice_Fang;
		case 410: return Moves::Ice_Shard;
		case 411: return Moves::Iron_Head;
		case 412: return Moves::Judgment;
		case 413: return Moves::Last_Resort;
		case 414: return Moves::Lava_Plume;
		case 415: return Moves::Leaf_Storm;
		case 416: return Moves::Lucky_Chant;
		case 417: return Moves::Lunar_Dance;
		case 418: return Moves::Magma_Storm;
		case 419: return Moves::Magnet_Bomb;
		case 420: return Moves::Magnet_Rise;
		case 421: return Moves::Me_First;
		case 422: return Moves::Metal_Burst;
		case 423: return Moves::Miracle_Eye;
		case 424: return Moves::Mirror_Shot;
		case 425: return Moves::Mud_Bomb;
		case 426: return Moves::Nasty_Plot;
		case 427: return Moves::Natural_Gift;
		case 428: return Moves::Night_Slash;
		case 429: return Moves::Ominous_Wind;
		case 430: return Moves::Payback;
		case 431: return Moves::Pluck;
		case 432: return Moves::Poison_Jab;
		case 433: return Moves::Power_Gem;
		case 434: return Moves::Power_Swap;
		case 435: return Moves::Power_Trick;
		case 436: return Moves::Power_Whip;
		case 437: return Moves::Psycho_Cut;
		case 438: return Moves::Psycho_Shift;
		case 439: return Moves::Punishment;
		case 440: return Moves::Roar_of_Time;
		case 441: return Moves::Rock_Climb;
		case 442: return Moves::Rock_Polish;
		case 443: return Moves::Rock_Wrecker;
		case 444: return Moves::Roost;
		case 445: return Moves::Seed_Bomb;
		case 446: return Moves::Seed_Flare;
		case 447: return Moves::Shadow_Claw;
		case 448: return Moves::Shadow_Force;
		case 449: return Moves::Shadow_Sneak;
		case 450: return Moves::Spacial_Rend;
		case 451: return Moves::Stealth_Rock;
		case 452: return Moves::Stone_Edge;
		case 453: return Moves::Sucker_Punch;
		case 454: return Moves::Switcheroo;
		case 455: return Moves::Tailwind;
		case 456: return Moves::Thunder_Fang;
		case 457: return Moves::Toxic_Spikes;
		case 458: return Moves::Trick_Room;
		case 459: return Moves::Trump_Card;
		case 460: return Moves::U_turn;
		case 461: return Moves::Vacuum_Wave;
		case 462: return Moves::Wake_Up_Slap;
		case 463: return Moves::Wood_Hammer;
		case 464: return Moves::Worry_Seed;
		case 465: return Moves::Wring_Out;
		case 466: return Moves::X_Scissor;
		case 467: return Moves::Zen_Headbutt;
		default: bounded::unreachable();
	}
}

struct Parser {
	constexpr explicit Parser(containers::vector<std::byte> const & buffer):
		m_view(buffer),
		m_file_version(pop_header())
	{
	}

	template<typename T>
	constexpr auto pop_integer(auto const bits) {
		if (bits > m_view.remaining_bits()) {
			throw std::runtime_error("Attempted to pop too many bits.");
		}
		return bounded::check_in_range<T>(m_view.pop_integer(bits));
	}
	constexpr auto pop_integer(auto const bits) {
		return pop_integer<bounded::integer<0, bounded::normalize<(1_bi << bits) - 1_bi>>>(bits);
	}
	constexpr auto pop_string(auto const size) -> std::string_view {
		if (size * bounded::char_bit > m_view.remaining_bits()) {
			throw std::runtime_error("Attempted to pop too many bytes.");
		}
		return m_view.pop_string(size);
	}
	constexpr auto pop_sized_string() -> std::string_view {
		return pop_string(pop_integer(8_bi));
	}
	constexpr auto skip(auto const bits) {
		m_view.skip(bits);
	}

	constexpr auto file_version() const {
		return m_file_version;
	}

private:
	constexpr auto pop_header() -> FileVersion {
		auto const file_version_string = pop_string(7_bi);
		if (file_version_string == " PNB4.0") {
			return FileVersion::four_zero;
		} else if (file_version_string == " PNB4.1") {
			return FileVersion::four_one;
		} else if (file_version_string == " PNB5.0") {
			return FileVersion::five_zero;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>("Version "sv, file_version_string, " not supported"sv));
		}
	}
	BitView m_view;
	FileVersion m_file_version;
};

auto parse_moves(Generation const generation, Parser & parser) -> RegularMoves {
	auto pop_move = [&] {
		return id_to_move(parser.pop_integer<MoveID>(9_bi));
	};
	// filter does not properly handle input iterators
	auto moves = RegularMoves();
	for (auto const move_name : containers::generate_n(max_moves_per_pokemon, pop_move)) {
		if (!move_name) {
			continue;
		}
		moves.push_back(Move(generation, *move_name));
	}
	return moves;
}

template<typename T>
constexpr auto parse_ivs_or_evs(Parser & parser, auto const bits) {
	auto pop = [&] {
		return T(parser.pop_integer<typename T::value_type>(bits));
	};
	auto const hp = pop();
	auto const attack = pop();
	auto const defense = pop();
	auto const speed = pop();
	auto const special_attack = pop();
	auto const special_defense = pop();
	return GenericStats{hp, attack, defense, special_attack, special_defense, speed};
}

template<Generation generation>
constexpr auto parse_ivs(Parser & parser) {
	constexpr auto bits = 5_bi;
	if constexpr (generation <= Generation::two) {
		auto const stored = parse_ivs_or_evs<DV>(parser, bits);
		auto const result = DVs{stored.atk(), stored.def(), stored.spe(), stored.spa()};
		if (result.hp() != stored.hp()) {
			throw std::runtime_error(containers::concatenate<std::string>("Invalid DVs. Calculated HP DV of "sv, to_string(result.hp().value()), " but received "sv, to_string(stored.hp().value())));
		}
		return result;
	} else {
		return parse_ivs_or_evs<IV>(parser, bits);
	}
}

template<Generation generation>
constexpr auto parse_evs(Parser & parser) {
	auto const stored = parse_ivs_or_evs<EV>(parser, 8_bi);
	if constexpr (generation <= Generation::two) {
		// NetBattle technically preserves but ignores the EV values populated
		// in old generations. Given that this matters only when moving Pokemon
		// forward to later generations, I do not think that information needs
		// to able to round trip.
		return OldGenEVs{EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi)};
	} else {
		return stored;
	}
}

template<Generation generation>
auto parse_pokemon(Parser & parser) -> bounded::optional<KnownPokemon<generation>> {
	constexpr auto nickname_bytes = 15_bi;
	auto const padded_nickname = parser.pop_string(nickname_bytes);
	auto const nickname = std::string_view(
		padded_nickname.begin(),
		containers::find_if_not(containers::reversed(padded_nickname), bounded::equal_to(' ')).base()
	);
	constexpr auto species_bits = 9_bi;
	auto const species_id = parser.pop_integer<SpeciesID>(species_bits);
	if (species_id == 0_bi) {
		constexpr auto total_pokemon_bit_length = 35_bi * bounded::char_bit;
		parser.skip(total_pokemon_bit_length - nickname_bytes * bounded::char_bit - species_bits);
		return bounded::none;
	}
	auto const species = id_to_species(bounded::increase_min<1>(species_id), parser.file_version());
	[[maybe_unused]] auto const game_version = parser.pop_integer(3_bi);
	auto const level = Level(parser.pop_integer<Level::value_type>(7_bi));
	auto const item_id = parser.pop_integer<ItemID>(7_bi);
	auto const nature = ingame_id_to_nature(parser.pop_integer<IngameNatureID>(5_bi));
	auto const ability = id_to_ability(species, parser.pop_integer(1_bi));
	auto const gender = id_to_gender(species, parser.pop_integer(1_bi));

	[[maybe_unused]] auto const shiny = parser.pop_integer(1_bi);
	[[maybe_unused]] auto const box = parser.pop_integer(4_bi);
	[[maybe_unused]] auto const unown_letter = parser.pop_integer(5_bi);

	auto const moves = parse_moves(generation, parser);
	auto const ivs = parse_ivs<generation>(parser);
	auto const evs = parse_evs<generation>(parser);

	[[maybe_unused]] auto const unused = parser.pop_integer(1_bi);
	[[maybe_unused]] auto const is_generation_4 = parser.pop_integer(1_bi);
	auto const prefer_generation_4_item = parser.pop_integer(1_bi) == 1_bi ? true : false;
	auto const item = id_to_item(item_id, prefer_generation_4_item);

	return KnownPokemon<generation>(
		species,
		containers::string(nickname),
		level,
		gender,
		item,
		ability,
		CombinedStats<generation>{nature, ivs, evs},
		moves
	);
}

template<Generation generation>
auto parse_team(Parser & parser) -> KnownTeam<generation> {
	auto all_pokemon = typename KnownPokemonCollection<generation>::Container();
	for ([[maybe_unused]] auto const pokemon_index : containers::integer_range(max_pokemon_per_team)) {
		auto const pokemon = parse_pokemon<generation>(parser);
		if (pokemon) {
			containers::push_back(all_pokemon, *pokemon);
		}
	}
	return KnownTeam<generation>(std::move(all_pokemon));
}

using GameVersion = bounded::integer<0, 8>;
constexpr auto game_version_to_generation(GameVersion const game_version) {
	switch (game_version.value()) {
		case 0: return Generation::one; // "RBY with Trades"
		case 1: return Generation::two; // "GSC with Trades"
		case 2: return Generation::three; // "Ru/Sa Only"
		case 3: return Generation::three; // "Full Advance"
		case 4: return Generation::three; // "Full Advance + XD Moves"
		case 5: return Generation::one; // "True RBY"
		case 6: return Generation::two; // "True GSC"
		case 7: return Generation::four; // "True Diamond + Pearl"
		case 8: return Generation::four; // "Full Generation 4"
		default: bounded::unreachable();
	}
}

} // namespace

auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<KnownTeam> {
	try {
		auto const bytes = bytes_in_file(std::ifstream(team_file, std::ios_base::binary));
		auto parser = Parser(bytes);
		[[maybe_unused]] auto const username = parser.pop_sized_string();
		[[maybe_unused]] auto const extra_info = parser.pop_sized_string();
		[[maybe_unused]] auto const win_message = parser.pop_sized_string();
		if (parser.file_version() == FileVersion::five_zero) {
			[[maybe_unused]] auto const tie_message = parser.pop_sized_string();
		}
		[[maybe_unused]] auto const lose_message = parser.pop_sized_string();
		auto const generation = game_version_to_generation(parser.pop_integer<GameVersion>(8_bi));
		[[maybe_unused]] auto const avatar = parser.pop_integer(8_bi);
		[[maybe_unused]] auto const sprite_type = parser.pop_integer(8_bi);
		return constant_generation(generation, [&]<Generation g>(constant_gen_t<g>) {
			return GenerationGeneric<KnownTeam>(parse_team<g>(parser));
		});
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse NetBattle team file \""sv, team_file.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

} // namespace technicalmachine::nb