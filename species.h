// Species enum
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SPECIES_H_
#define SPECIES_H_

namespace technicalmachine {

/*
I decided to go with alphabetical listing over Pokedex listing due to the issue of alternate forms. Alternate forms (like Deoxys-A vs. Deoxys-S) mean that I have two choices: I can have one be in the middle of the list, and the other be at the other end, or I can stick the forms together. Either way, it ruins the idea of referencing Pokemon by #, so to avoid any mistakes, I just went alphabetical.

Hyphens and spaces were replaced by underscores. Dots and apostrophes were removed.

The following are my format for species:

Deoxys-A(ttack), Deoxys-D(efense), Deoxys-M(ediocre), Deoxys-S(peed). All of these are standard except Deoxys-M, which is usually refered as simply Deoxys. On Shoddy Battle, A = f, D = l, S = e, and M has no letter. Pokemon Online uses my standard abbreviations, except with Deoxys-M being just Deoxys.

Giratina-A(ltered), Giratina-O(rigin). Both of these are somewhat standard. Giratina-A is sometimes refered to as simply Giratina. On Shoddy, Giratina-A has no letter, and Giratina-O is Giratina-o. Pokemon Online uses Giratina and Giratina-O.

The weaker Rotom form is Rotom. The appliance Rotom forms use their standard one-letter abbreviation. This is the same as Shoddy Battle, except Shoddy does not capitalize the form letter. For reference: Rotom-C(ut), Rotom-F(ridge), Rotom-H(eat), Rotom-S(pin), Rotom-W(ash). Pokemon Online uses the same as I do.

Shaymin-L(and), Shaymin-S(ky). Shoddy uses Shaymin and Shaymin-s. Pokemon Online uses Shaymin and Shaymin-S.

Wormadam-P(lant) = Grass-type, Wormadam-S(andy) = Ground-type, Wormadam-T(rash) = Steel-type. These names are based on the in-game Pokedex descriptions of the forms. On Shoddy Battle, P = no letter, S = g, T = s. Pokemon Online uses Wormadam, Wormadam-G, and Wormadam-S.

Pokemon Online refers to Porygon2 as Porygon 2.
*/

enum Species { ABOMASNOW, ABRA, ABSOL, AERODACTYL, AGGRON, AIPOM, ALAKAZAM, ALTARIA, AMBIPOM, AMPHAROS, ANORITH, ARBOK, ARCANINE, ARCEUS, ARIADOS, ARMALDO, ARON, ARTICUNO, AZELF, AZUMARILL, AZURILL, BAGON, BALTOY, BANETTE, BARBOACH, BASTIODON, BAYLEEF, BEAUTIFLY, BEEDRILL, BELDUM, BELLOSSOM, BELLSPROUT, BIBAREL, BIDOOF, BLASTOISE, BLAZIKEN, BLISSEY, BONSLY, BRELOOM, BRONZONG, BRONZOR, BUDEW, BUIZEL, BULBASAUR, BUNEARY, BURMY, BUTTERFREE, CACNEA, CACTURNE, CAMERUPT, CARNIVINE, CARVANHA, CASCOON, CASTFORM, CATERPIE, CELEBI, CHANSEY, CHARIZARD, CHARMANDER, CHARMELEON, CHATOT, CHERRIM, CHERUBI, CHIKORITA, CHIMCHAR, CHIMECHO, CHINCHOU, CHINGLING, CLAMPERL, CLAYDOL, CLEFABLE, CLEFAIRY, CLEFFA, CLOYSTER, COMBEE, COMBUSKEN, CORPHISH, CORSOLA, CRADILY, CRANIDOS, CRAWDAUNT, CRESSELIA, CROAGUNK, CROBAT, CROCONAW, CUBONE, CYNDAQUIL, DARKRAI, DELCATTY, DELIBIRD, DEOXYS_A, DEOXYS_D, DEOXYS_M, DEOXYS_S, DEWGONG, DIALGA, DIGLETT, DITTO, DODRIO, DODUO, DONPHAN, DRAGONAIR, DRAGONITE, DRAPION, DRATINI, DRIFBLIM, DRIFLOON, DROWZEE, DUGTRIO, DUNSPARCE, DUSCLOPS, DUSKNOIR, DUSKULL, DUSTOX, EEVEE, EKANS, ELECTABUZZ, ELECTIVIRE, ELECTRIKE, ELECTRODE, ELEKID, EMPOLEON, ENTEI, ESPEON, EXEGGCUTE, EXEGGUTOR, EXPLOUD, FARFETCHD, FEAROW, FEEBAS, FERALIGATR, FINNEON, FLAAFFY, FLAREON, FLOATZEL, FLYGON, FORRETRESS, FROSLASS, FURRET, GABITE, GALLADE, GARCHOMP, GARDEVOIR, GASTLY, GASTRODON, GENGAR, GEODUDE, GIBLE, GIRAFARIG, GIRATINA_A, GIRATINA_O, GLACEON, GLALIE, GLAMEOW, GLIGAR, GLISCOR, GLOOM, GOLBAT, GOLDEEN, GOLDUCK, GOLEM, GOREBYSS, GRANBULL, GRAVELER, GRIMER, GROTLE, GROUDON, GROVYLE, GROWLITHE, GRUMPIG, GULPIN, GYARADOS, HAPPINY, HARIYAMA, HAUNTER, HEATRAN, HERACROSS, HIPPOPOTAS, HIPPOWDON, HITMONCHAN, HITMONLEE, HITMONTOP, HO_OH, HONCHKROW, HOOTHOOT, HOPPIP, HORSEA, HOUNDOOM, HOUNDOUR, HUNTAIL, HYPNO, IGGLYBUFF, ILLUMISE, INFERNAPE, IVYSAUR, JIGGLYPUFF, JIRACHI, JOLTEON, JUMPLUFF, JYNX, KABUTO, KABUTOPS, KADABRA, KAKUNA, KANGASKHAN, KECLEON, KINGDRA, KINGLER, KIRLIA, KOFFING, KRABBY, KRICKETOT, KRICKETUNE, KYOGRE, LAIRON, LANTURN, LAPRAS, LARVITAR, LATIAS, LATIOS, LEAFEON, LEDIAN, LEDYBA, LICKILICKY, LICKITUNG, LILEEP, LINOONE, LOMBRE, LOPUNNY, LOTAD, LOUDRED, LUCARIO, LUDICOLO, LUGIA, LUMINEON, LUNATONE, LUVDISC, LUXIO, LUXRAY, MACHAMP, MACHOKE, MACHOP, MAGBY, MAGCARGO, MAGIKARP, MAGMAR, MAGMORTAR, MAGNEMITE, MAGNETON, MAGNEZONE, MAKUHITA, MAMOSWINE, MANAPHY, MANECTRIC, MANKEY, MANTINE, MANTYKE, MAREEP, MARILL, MAROWAK, MARSHTOMP, MASQUERAIN, MAWILE, MEDICHAM, MEDITITE, MEGANIUM, MEOWTH, MESPRIT, METAGROSS, METANG, METAPOD, MEW, MEWTWO, MIGHTYENA, MILOTIC, MILTANK, MIME_JR, MINUN, MISDREAVUS, MISMAGIUS, MOLTRES, MONFERNO, MOTHIM, MR_MIME, MUDKIP, MUK, MUNCHLAX, MURKROW, NATU, NIDOKING, NIDOQUEEN, NIDORAN_F, NIDORAN_M, NIDORINA, NIDORINO, NINCADA, NINETALES, NINJASK, NOCTOWL, NOSEPASS, NUMEL, NUZLEAF, OCTILLERY, ODDISH, OMANYTE, OMASTAR, ONIX, PACHIRISU, PALKIA, PARAS, PARASECT, PELIPPER, PERSIAN, PHANPY, PHIONE, PICHU, PIDGEOT, PIDGEOTTO, PIDGEY, PIKACHU, PILOSWINE, PINECO, PINSIR, PIPLUP, PLUSLE, POLITOED, POLIWAG, POLIWHIRL, POLIWRATH, PONYTA, POOCHYENA, PORYGON, PORYGON_Z, PORYGON2, PRIMEAPE, PRINPLUP, PROBOPASS, PSYDUCK, PUPITAR, PURUGLY, QUAGSIRE, QUILAVA, QWILFISH, RAICHU, RAIKOU, RALTS, RAMPARDOS, RAPIDASH, RATICATE, RATTATA, RAYQUAZA, REGICE, REGIGIGAS, REGIROCK, REGISTEEL, RELICANTH, REMORAID, RHYDON, RHYHORN, RHYPERIOR, RIOLU, ROSELIA, ROSERADE, ROTOM, ROTOM_C, ROTOM_F, ROTOM_H, ROTOM_S, ROTOM_W, SABLEYE, SALAMENCE, SANDSHREW, SANDSLASH, SCEPTILE, SCIZOR, SCYTHER, SEADRA, SEAKING, SEALEO, SEEDOT, SEEL, SENTRET, SEVIPER, SHARPEDO, SHAYMIN_L, SHAYMIN_S, SHEDINJA, SHELGON, SHELLDER, SHELLOS, SHIELDON, SHIFTRY, SHINX, SHROOMISH, SHUCKLE, SHUPPET, SILCOON, SKARMORY, SKIPLOOM, SKITTY, SKORUPI, SKUNTANK, SLAKING, SLAKOTH, SLOWBRO, SLOWKING, SLOWPOKE, SLUGMA, SMEARGLE, SMOOCHUM, SNEASEL, SNORLAX, SNORUNT, SNOVER, SNUBBULL, SOLROCK, SPEAROW, SPHEAL, SPINARAK, SPINDA, SPIRITOMB, SPOINK, SQUIRTLE, STANTLER, STARAPTOR, STARAVIA, STARLY, STARMIE, STARYU, STEELIX, STUNKY, SUDOWOODO, SUICUNE, SUNFLORA, SUNKERN, SURSKIT, SWABLU, SWALOT, SWAMPERT, SWELLOW, SWINUB, TAILLOW, TANGELA, TANGROWTH, TAUROS, TEDDIURSA, TENTACOOL, TENTACRUEL, TOGEKISS, TOGEPI, TOGETIC, TORCHIC, TORKOAL, TORTERRA, TOTODILE, TOXICROAK, TRAPINCH, TREECKO, TROPIUS, TURTWIG, TYPHLOSION, TYRANITAR, TYROGUE, UMBREON, UNOWN, URSARING, UXIE, VAPOREON, VENOMOTH, VENONAT, VENUSAUR, VESPIQUEN, VIBRAVA, VICTREEBEL, VIGOROTH, VILEPLUME, VOLBEAT, VOLTORB, VULPIX, WAILMER, WAILORD, WALREIN, WARTORTLE, WEAVILE, WEEDLE, WEEPINBELL, WEEZING, WHISCASH, WHISMUR, WIGGLYTUFF, WINGULL, WOBBUFFET, WOOPER, WORMADAM_P, WORMADAM_S, WORMADAM_T, WURMPLE, WYNAUT, XATU, YANMA, YANMEGA, ZANGOOSE, ZAPDOS, ZIGZAGOON, ZUBAT, END_SPECIES };

}
#endif		// SPECIES_H_
