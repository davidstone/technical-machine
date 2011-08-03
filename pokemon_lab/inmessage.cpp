// Pokemon Lab incoming messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>
#include <vector>
#include "../evaluate.h"
#include "../map.h"
#include "../species.h"
#include "../team.h"
#include "../weather.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "inmessage.h"
#include "connect.h"
#undef SING

namespace technicalmachine {
namespace pl {

InMessage::InMessage ():
	index (0) {
}

void InMessage::reset (unsigned bytes) {
	buffer.clear();
	buffer.resize (bytes);
	index = 0;
}

void InMessage::read_header (boost::asio::ip::tcp::socket & socket, BotClient * client) {
	reset (5);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& InMessage::read_body, this, boost::ref (socket), client));
}

void InMessage::read_body (boost::asio::ip::tcp::socket & socket, BotClient * client) {
	// extract the message type and length components
	Message code = static_cast <InMessage::Message> (read_byte ());
	uint32_t bytes = read_int ();

	reset (bytes);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& BotClient::handle_message, client, code, boost::ref (*this)));
}

uint32_t InMessage::read_bytes (int bytes) {
	uint32_t data = 0;
	for (int n = 0; n != bytes; ++n) {
		data += buffer [index] << (8 * (bytes - n - 1));
		++index;
	}
	return data;
}

uint8_t InMessage::read_byte () {
	return read_bytes (1);
}

uint16_t InMessage::read_short () {
	return read_bytes (2);
}

uint32_t InMessage::read_int () {
	return read_bytes (4);
}

std::string InMessage::read_string () {
	unsigned short length = read_short ();
	std::string data = "";
	for (unsigned n = 0; n != length; ++n) {
		data += buffer [index];
		++index;
	}
	return data;
}

void InMessage::skip () {
	buffer.clear();
}

species InMessage::pl_to_tm_species (int id) {
	static species const pokemon_lab_to_technical_machine_species [] = { BULBASAUR, IVYSAUR, VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, RATTATA, RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, RAICHU, SANDSHREW, SANDSLASH, NIDORAN_F, NIDORINA, NIDOQUEEN, NIDORAN_M, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, DUGTRIO, MEOWTH, PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, GROWLITHE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MACHOP, MACHOKE, MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, GRAVELER, GOLEM, PONYTA, RAPIDASH, SLOWPOKE, SLOWBRO, MAGNEMITE, MAGNETON, FARFETCHD, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, MUK, SHELLDER, CLOYSTER, GASTLY, HAUNTER, GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, VOLTORB, ELECTRODE, EXEGGCUTE, EXEGGUTOR, CUBONE, MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, TAUROS, MAGIKARP, GYARADOS, LAPRAS, DITTO, EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, SNORLAX, ARTICUNO, ZAPDOS, MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, MISDREAVUS, UNOWN, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, SNUBBULL, GRANBULL, QWILFISH, SCIZOR, SHUCKLE, HERACROSS, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MAWILE, ARON, LAIRON, AGGRON, MEDITITE, MEDICHAM, ELECTRIKE, MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, KECLEON, SHUPPET, BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, WYNAUT, SNORUNT, GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELGON, SALAMENCE, BELDUM, METANG, METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, LATIOS, KYOGRE, GROUDON, RAYQUAZA, JIRACHI, DEOXYS_M, DEOXYS_A, DEOXYS_D, DEOXYS_S, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, WORMADAM_P, WORMADAM_S, WORMADAM_T, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, SHELLOS, GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, UXIE, MESPRIT, AZELF, DIALGA, PALKIA, HEATRAN, REGIGIGAS, GIRATINA_A, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN_L, ARCEUS, ROTOM_F, ROTOM_C, ROTOM_H, ROTOM_S, ROTOM_W, GIRATINA_O, SHAYMIN_S };
	return pokemon_lab_to_technical_machine_species [id];
}

}		// namespace pl
}		// namespace technicalmachine
