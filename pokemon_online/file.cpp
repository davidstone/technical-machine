// Load Pokemon Online teams
// Copyright (C) 2011 David Stone
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

#include <boost/lexical_cast.hpp>
#include "file.h"
#include "../ability.h"
#include "../pokemon.h"
#include "../species.h"
#include "../team.h"

namespace technicalmachine {

namespace po {

void load_team (Team & team, std::string const & name, unsigned size) {
	// This doesn't work right, but I can't find the PO team file format spec.
	Species const pokemon_converter [] = { END_SPECIES, BULBASAUR, IVYSAUR, VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, RATTATA, RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, RAICHU, SANDSHREW, SANDSLASH, NIDORAN_F, NIDORINA, NIDOQUEEN, NIDORAN_M, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, DUGTRIO, MEOWTH, PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, GROWLITHE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MACHOP, MACHOKE, MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, GRAVELER, GOLEM, PONYTA, RAPIDASH, SLOWPOKE, SLOWBRO, MAGNEMITE, MAGNETON, FARFETCHD, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, MUK, SHELGON, CLOYSTER, GASTLY, HAUNTER, GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, VOLTORB, ELECTRODE, EXEGGCUTE, EXEGGUTOR, CUBONE, MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, TAUROS, MAGIKARP, GYARADOS, LAPRAS, DITTO, EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, SNORLAX, ARTICUNO, ZAPDOS, MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, MISDREAVUS, UNOWN, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, SNUBBULL, GRANBULL, QWILFISH, SCIZOR, SHUCKLE, HERACROSS, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MAWILE, ARON, LAIRON, AGGRON, MEDITITE, MEDICHAM, ELECTRIKE, MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, KECLEON, SHUPPET, BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, WYNAUT, SNORUNT, GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELLDER, SALAMENCE, BELDUM, METANG, METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, LATIOS, KYOGRE, GROUDON, RAYQUAZA, JIRACHI, DEOXYS_A, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, WORMADAM_P, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, SHELLOS, GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, UXIE, MESPRIT, AZELF, DIALGA, PALKIA, HEATRAN, REGIGIGAS, GIRATINA_A, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN_L, ARCEUS };
	Ability::Abilities const ability_converter [] = { Ability::STENCH, Ability::DRIZZLE, Ability::SPEED_BOOST, Ability::BATTLE_ARMOR, Ability::STURDY, Ability::DAMP, Ability::LIMBER, Ability::SAND_VEIL, Ability::STATIC, Ability::VOLT_ABSORB, Ability::WATER_ABSORB, Ability::OBLIVIOUS, Ability::AIR_LOCK, Ability::COMPOUNDEYES, Ability::INSOMNIA, Ability::COLOR_CHANGE, Ability::IMMUNITY, Ability::FLASH_FIRE, Ability::SHIELD_DUST, Ability::OWN_TEMPO, Ability::SUCTION_CUPS, Ability::INTIMIDATE, Ability::SHADOW_TAG, Ability::ROUGH_SKIN, Ability::WONDER_GUARD, Ability::LEVITATE, Ability::EFFECT_SPORE, Ability::SYNCHRONIZE, Ability::CLEAR_BODY, Ability::NATURAL_CURE, Ability::LIGHTNINGROD, Ability::SERENE_GRACE, Ability::SWIFT_SWIM, Ability::CHLOROPHYLL, Ability::STENCH, Ability::TRACE, Ability::PURE_POWER, Ability::POISON_POINT, Ability::INNER_FOCUS, Ability::MAGMA_ARMOR, Ability::WATER_VEIL, Ability::MAGNET_PULL, Ability::SOUNDPROOF, Ability::RAIN_DISH, Ability::SAND_STREAM, Ability::PRESSURE, Ability::THICK_FAT, Ability::EARLY_BIRD, Ability::FLAME_BODY, Ability::STENCH, Ability::KEEN_EYE, Ability::HYPER_CUTTER, Ability::STENCH, Ability::TRUANT, Ability::HUSTLE, Ability::CUTE_CHARM, Ability::PLUS, Ability::MINUS, Ability::FORECAST, Ability::STICKY_HOLD, Ability::SHED_SKIN, Ability::GUTS, Ability::MARVEL_SCALE, Ability::LIQUID_OOZE, Ability::OVERGROW, Ability::BLAZE, Ability::TORRENT, Ability::SWARM, Ability::ROCK_HEAD, Ability::DROUGHT, Ability::ARENA_TRAP, Ability::INSOMNIA, Ability::CLEAR_BODY, Ability::PURE_POWER, Ability::BATTLE_ARMOR, Ability::AIR_LOCK, Ability::TANGLED_FEET, Ability::MOTOR_DRIVE, Ability::RIVALRY, Ability::STEADFAST, Ability::SNOW_CLOAK, Ability::GLUTTONY, Ability::ANGER_POINT, Ability::UNBURDEN, Ability::HEATPROOF, Ability::SIMPLE, Ability::DRY_SKIN, Ability::DOWNLOAD, Ability::IRON_FIST, Ability::POISON_HEAL, Ability::ADAPTABILITY, Ability::SKILL_LINK, Ability::HYDRATION, Ability::SOLAR_POWER, Ability::QUICK_FEET, Ability::NORMALIZE, Ability::SNIPER, Ability::MAGIC_GUARD, Ability::NO_GUARD, Ability::STALL, Ability::TECHNICIAN, Ability::LEAF_GUARD, Ability::KLUTZ, Ability::MOLD_BREAKER, Ability::SUPER_LUCK, Ability::AFTERMATH, Ability::ANTICIPATION, Ability::FOREWARN, Ability::UNAWARE, Ability::TINTED_LENS, Ability::SOLID_ROCK, Ability::SLOW_START, Ability::SCRAPPY, Ability::STORM_DRAIN, Ability::ICE_BODY, Ability::SOLID_ROCK, Ability::SNOW_WARNING, Ability::STENCH, Ability::FRISK, Ability::RECKLESS, Ability::MULTITYPE, Ability::FLOWER_GIFT, Ability::BAD_DREAMS };
	Item::Items const item_converter [] = { Item::NO_ITEM, Item::BIG_ROOT, Item::OTHER10, Item::BRIGHTPOWDER, Item::CHOICE_BAND, Item::CHOICE_SCARF, Item::CHOICE_SPECS, Item::DESTINY_KNOT, Item::EXPERT_BELT, Item::FOCUS_BAND, Item::FOCUS_SASH, Item::LAGGING_TAIL, Item::OTHER10, Item::LAGGING_TAIL, Item::LAX_INCENSE, Item::LEFTOVERS, Item::OTHER10, Item::MENTAL_HERB, Item::METAL_POWDER, Item::MUSCLE_BAND, Item::ODD_INCENSE, Item::OTHER10, Item::POWER_HERB, Item::OTHER10, Item::QUICK_POWDER, Item::OTHER10, Item::OTHER10, Item::ROCK_INCENSE, Item::ROSE_INCENSE, Item::WAVE_INCENSE, Item::SHED_SHELL, Item::SILK_SCARF, Item::SILVERPOWDER, Item::SMOOTH_ROCK, Item::SOFT_SAND, Item::OTHER10, Item::WAVE_INCENSE, Item::WHITE_HERB, Item::WIDE_LENS, Item::WISE_GLASSES, Item::OTHER10, Item::ZOOM_LENS, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::BERRY_JUICE, Item::OTHER30, Item::OTHER30, Item::BLACK_BELT, Item::OTHER30, Item::BLACK_SLUDGE, Item::BLACKGLASSES, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::CHARCOAL, Item::OTHER30, Item::OTHER30, Item::DEEPSEASCALE, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::FLAME_ORB, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::KINGS_ROCK, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::LIFE_ORB, Item::LIGHT_BALL, Item::LIGHT_CLAY, Item::OTHER30, Item::MAGNET, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::METAL_COAT, Item::METRONOME, Item::MIRACLE_SEED, Item::OTHER30, Item::OTHER30, Item::MYSTIC_WATER, Item::NEVERMELTICE, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::KINGS_ROCK, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::SCOPE_LENS, Item::SHELL_BELL, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::SOUL_DEW, Item::SPELL_TAG, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::TOXIC_ORB, Item::TWISTEDSPOON, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::OTHER30, Item::ICY_ROCK, Item::LUCKY_PUNCH, Item::OTHER50, Item::SHARP_BEAK, Item::ADAMANT_ORB, Item::DAMP_ROCK, Item::HEAT_ROCK, Item::LUSTROUS_ORB, Item::MACHO_BRACE, Item::STICK, Item::DRAGON_FANG, Item::POISON_BARB, Item::POWER_ANKLET, Item::POWER_BAND, Item::POWER_BELT, Item::POWER_BRACER, Item::POWER_LENS, Item::POWER_WEIGHT, Item::OTHER80, Item::OTHER80, Item::OTHER80, Item::OTHER80, Item::OTHER80, Item::OTHER80, Item::OTHER80, Item::QUICK_CLAW, Item::RAZOR_CLAW, Item::OTHER80, Item::STICKY_BARB, Item::DEEPSEATOOTH, Item::DRACO_PLATE, Item::DREAD_PLATE, Item::EARTH_PLATE, Item::FIST_PLATE, Item::FLAME_PLATE, Item::GRIP_CLAW, Item::ICICLE_PLATE, Item::INSECT_PLATE, Item::IRON_PLATE, Item::MEADOW_PLATE, Item::MIND_PLATE, Item::SKY_PLATE, Item::SPLASH_PLATE, Item::SPOOKY_PLATE, Item::STONE_PLATE, Item::THICK_CLUB, Item::TOXIC_PLATE, Item::ZAP_PLATE, Item::OTHER100, Item::OTHER100, Item::OTHER100, Item::HARD_STONE, Item::OTHER100, Item::OTHER100, Item::OTHER100, Item::OTHER100, Item::OTHER100, Item::IRON_BALL, Item::GRISEOUS_ORB, Item::AIR_MAIL, Item::AIR_MAIL, Item::AIR_MAIL, Item::AIR_MAIL, Item::AIR_MAIL, Item::AIR_MAIL, Item::CHERI_BERRY, Item::CHESTO_BERRY, Item::PECHA_BERRY, Item::RAWST_BERRY, Item::ASPEAR_BERRY, Item::LEPPA_BERRY, Item::ORAN_BERRY, Item::PERSIM_BERRY, Item::LUM_BERRY, Item::SITRUS_BERRY, Item::FIGY_BERRY, Item::WIKI_BERRY, Item::MAGO_BERRY, Item::AGUAV_BERRY, Item::IAPAPA_BERRY, Item::RAZZ_BERRY, Item::BLUK_BERRY, Item::NANAB_BERRY, Item::WEPEAR_BERRY, Item::PINAP_BERRY, Item::POMEG_BERRY, Item::KELPSY_BERRY, Item::QUALOT_BERRY, Item::HONDEW_BERRY, Item::GREPA_BERRY, Item::TAMATO_BERRY, Item::CORNN_BERRY, Item::MAGOST_BERRY, Item::RABUTA_BERRY, Item::NOMEL_BERRY, Item::SPELON_BERRY, Item::PAMTRE_BERRY, Item::WATMEL_BERRY, Item::DURIN_BERRY, Item::BELUE_BERRY, Item::OCCA_BERRY, Item::PASSHO_BERRY, Item::WACAN_BERRY, Item::RINDO_BERRY, Item::YACHE_BERRY, Item::CHOPLE_BERRY, Item::KEBIA_BERRY, Item::SHUCA_BERRY, Item::COBA_BERRY, Item::PAYAPA_BERRY, Item::TANGA_BERRY, Item::CHARTI_BERRY, Item::KASIB_BERRY, Item::HABAN_BERRY, Item::COLBUR_BERRY, Item::BABIRI_BERRY, Item::CHILAN_BERRY, Item::LIECHI_BERRY, Item::GANLON_BERRY, Item::SALAC_BERRY, Item::PETAYA_BERRY, Item::APICOT_BERRY, Item::LANSAT_BERRY, Item::STARF_BERRY, Item::ENIGMA_BERRY, Item::MICLE_BERRY, Item::CUSTAP_BERRY, Item::JABOCA_BERRY, Item::ROWAP_BERRY };
	Move::Moves const move_converter [] = { Move::END_MOVE, Move::POUND, Move::KARATE_CHOP, Move::DOUBLESLAP, Move::COMET_PUNCH, Move::MEGA_PUNCH, Move::PAY_DAY, Move::FIRE_PUNCH, Move::ICE_PUNCH, Move::THUNDERPUNCH, Move::SCRATCH, Move::VICEGRIP, Move::GUILLOTINE, Move::RAZOR_WIND, Move::SWORDS_DANCE, Move::CUT, Move::GUST, Move::WING_ATTACK, Move::WHIRLWIND, Move::FLY, Move::BIND, Move::SLAM, Move::VINE_WHIP, Move::STOMP, Move::DOUBLE_KICK, Move::MEGA_KICK, Move::JUMP_KICK, Move::ROLLING_KICK, Move::SAND_ATTACK, Move::HEADBUTT, Move::HORN_ATTACK, Move::FURY_ATTACK, Move::HORN_DRILL, Move::TACKLE, Move::BODY_SLAM, Move::WRAP, Move::TAKE_DOWN, Move::THRASH, Move::DOUBLE_EDGE, Move::TAIL_WHIP, Move::POISON_STING, Move::TWINEEDLE, Move::PIN_MISSILE, Move::LEER, Move::BITE, Move::GROWL, Move::ROAR, Move::SING, Move::SUPERSONIC, Move::SONICBOOM, Move::DISABLE, Move::ACID, Move::EMBER, Move::FLAMETHROWER, Move::MIST, Move::WATER_GUN, Move::HYDRO_PUMP, Move::SURF, Move::ICE_BEAM, Move::BLIZZARD, Move::PSYBEAM, Move::BUBBLEBEAM, Move::AURORA_BEAM, Move::HYPER_BEAM, Move::PECK, Move::DRILL_PECK, Move::SUBMISSION, Move::LOW_KICK, Move::COUNTER, Move::SEISMIC_TOSS, Move::STRENGTH, Move::ABSORB, Move::MEGA_DRAIN, Move::LEECH_SEED, Move::GROWTH, Move::RAZOR_LEAF, Move::SOLARBEAM, Move::POISONPOWDER, Move::STUN_SPORE, Move::SLEEP_POWDER, Move::PETAL_DANCE, Move::STRING_SHOT, Move::DRAGON_RAGE, Move::FIRE_SPIN, Move::THUNDERSHOCK, Move::THUNDERBOLT, Move::THUNDER_WAVE, Move::THUNDER, Move::ROCK_THROW, Move::EARTHQUAKE, Move::FISSURE, Move::DIG, Move::TOXIC, Move::CONFUSION, Move::PSYCHIC, Move::HYPNOSIS, Move::MEDITATE, Move::AGILITY, Move::QUICK_ATTACK, Move::RAGE, Move::TELEPORT, Move::NIGHT_SHADE, Move::MIMIC, Move::SCREECH, Move::DOUBLE_TEAM, Move::RECOVER, Move::HARDEN, Move::MINIMIZE, Move::SMOKESCREEN, Move::CONFUSE_RAY, Move::WITHDRAW, Move::DEFENSE_CURL, Move::BARRIER, Move::LIGHT_SCREEN, Move::HAZE, Move::REFLECT, Move::FOCUS_ENERGY, Move::BIDE, Move::METRONOME, Move::MIRROR_MOVE, Move::SELFDESTRUCT, Move::EGG_BOMB, Move::LICK, Move::SMOG, Move::SLUDGE, Move::BONE_CLUB, Move::FIRE_BLAST, Move::WATERFALL, Move::CLAMP, Move::SWIFT, Move::SKULL_BASH, Move::SPIKE_CANNON, Move::CONSTRICT, Move::AMNESIA, Move::KINESIS, Move::SOFTBOILED, Move::HI_JUMP_KICK, Move::GLARE, Move::DREAM_EATER, Move::POISON_GAS, Move::BARRAGE, Move::LEECH_LIFE, Move::LOVELY_KISS, Move::SKY_ATTACK, Move::TRANSFORM, Move::BUBBLE, Move::DIZZY_PUNCH, Move::SPORE, Move::FLASH, Move::PSYWAVE, Move::SPLASH, Move::ACID_ARMOR, Move::CRABHAMMER, Move::EXPLOSION, Move::FURY_SWIPES, Move::BONEMERANG, Move::REST, Move::ROCK_SLIDE, Move::HYPER_FANG, Move::SHARPEN, Move::CONVERSION, Move::TRI_ATTACK, Move::SUPER_FANG, Move::SLASH, Move::SUBSTITUTE, Move::STRUGGLE, Move::SKETCH, Move::TRIPLE_KICK, Move::THIEF, Move::SPIDER_WEB, Move::MIND_READER, Move::NIGHTMARE, Move::FLAME_WHEEL, Move::SNORE, Move::CURSE, Move::FLAIL, Move::CONVERSION2, Move::AEROBLAST, Move::COTTON_SPORE, Move::REVERSAL, Move::SPITE, Move::POWDER_SNOW, Move::PROTECT, Move::MACH_PUNCH, Move::SCARY_FACE, Move::FAINT_ATTACK, Move::SWEET_KISS, Move::BELLY_DRUM, Move::SLUDGE_BOMB, Move::MUD_SLAP, Move::OCTAZOOKA, Move::SPIKES, Move::ZAP_CANNON, Move::FORESIGHT, Move::DESTINY_BOND, Move::PERISH_SONG, Move::ICY_WIND, Move::DETECT, Move::BONE_RUSH, Move::LOCK_ON, Move::OUTRAGE, Move::SANDSTORM, Move::GIGA_DRAIN, Move::ENDURE, Move::CHARM, Move::ROLLOUT, Move::FALSE_SWIPE, Move::SWAGGER, Move::MILK_DRINK, Move::SPARK, Move::FURY_CUTTER, Move::STEEL_WING, Move::MEAN_LOOK, Move::ATTRACT, Move::SLEEP_TALK, Move::HEAL_BELL, Move::RETURN, Move::PRESENT, Move::FRUSTRATION, Move::SAFEGUARD, Move::PAIN_SPLIT, Move::SACRED_FIRE, Move::MAGNITUDE, Move::DYNAMICPUNCH, Move::MEGAHORN, Move::DRAGONBREATH, Move::BATON_PASS, Move::ENCORE, Move::PURSUIT, Move::RAPID_SPIN, Move::SWEET_SCENT, Move::IRON_TAIL, Move::METAL_CLAW, Move::VITAL_THROW, Move::MORNING_SUN, Move::SYNTHESIS, Move::MOONLIGHT, Move::HIDDEN_POWER, Move::CROSS_CHOP, Move::TWISTER, Move::RAIN_DANCE, Move::SUNNY_DAY, Move::CRUNCH, Move::MIRROR_COAT, Move::PSYCH_UP, Move::EXTREMESPEED, Move::ANCIENTPOWER, Move::SHADOW_BALL, Move::FUTURE_SIGHT, Move::ROCK_SMASH, Move::WHIRLPOOL, Move::BEAT_UP, Move::FAKE_OUT, Move::UPROAR, Move::STOCKPILE, Move::SPIT_UP, Move::SWALLOW, Move::HEAT_WAVE, Move::HAIL, Move::TORMENT, Move::FLATTER, Move::WILL_O_WISP, Move::MEMENTO, Move::FACADE, Move::FOCUS_PUNCH, Move::SMELLINGSALT, Move::FOLLOW_ME, Move::NATURE_POWER, Move::CHARGE, Move::TAUNT, Move::HELPING_HAND, Move::TRICK, Move::ROLE_PLAY, Move::WISH, Move::ASSIST, Move::INGRAIN, Move::SUPERPOWER, Move::MAGIC_COAT, Move::RECYCLE, Move::REVENGE, Move::BRICK_BREAK, Move::YAWN, Move::KNOCK_OFF, Move::ENDEAVOR, Move::ERUPTION, Move::SKILL_SWAP, Move::IMPRISON, Move::REFRESH, Move::GRUDGE, Move::SNATCH, Move::SECRET_POWER, Move::DIVE, Move::ARM_THRUST, Move::CAMOUFLAGE, Move::TAIL_GLOW, Move::LUSTER_PURGE, Move::MIST_BALL, Move::FEATHERDANCE, Move::TEETER_DANCE, Move::BLAZE_KICK, Move::MUD_SPORT, Move::ICE_BALL, Move::NEEDLE_ARM, Move::SLACK_OFF, Move::HYPER_VOICE, Move::POISON_FANG, Move::CRUSH_CLAW, Move::BLAST_BURN, Move::HYDRO_CANNON, Move::METEOR_MASH, Move::ASTONISH, Move::WEATHER_BALL, Move::AROMATHERAPY, Move::FAKE_TEARS, Move::AIR_CUTTER, Move::OVERHEAT, Move::ODOR_SLEUTH, Move::ROCK_TOMB, Move::SILVER_WIND, Move::METAL_SOUND, Move::GRASSWHISTLE, Move::TICKLE, Move::COSMIC_POWER, Move::WATER_SPOUT, Move::SIGNAL_BEAM, Move::SHADOW_PUNCH, Move::EXTRASENSORY, Move::SKY_UPPERCUT, Move::SAND_TOMB, Move::SHEER_COLD, Move::MUDDY_WATER, Move::BULLET_SEED, Move::AERIAL_ACE, Move::ICICLE_SPEAR, Move::IRON_DEFENSE, Move::BLOCK, Move::HOWL, Move::DRAGON_CLAW, Move::FRENZY_PLANT, Move::BULK_UP, Move::BOUNCE, Move::MUD_SHOT, Move::POISON_TAIL, Move::COVET, Move::VOLT_TACKLE, Move::MAGICAL_LEAF, Move::WATER_SPORT, Move::CALM_MIND, Move::LEAF_BLADE, Move::DRAGON_DANCE, Move::ROCK_BLAST, Move::SHOCK_WAVE, Move::WATER_PULSE, Move::DOOM_DESIRE, Move::PSYCHO_BOOST, Move::ROOST, Move::GRAVITY, Move::MIRACLE_EYE, Move::WAKE_UP_SLAP, Move::HAMMER_ARM, Move::GYRO_BALL, Move::HEALING_WISH, Move::BRINE, Move::NATURAL_GIFT, Move::FEINT, Move::PLUCK, Move::TAILWIND, Move::ACUPRESSURE, Move::METAL_BURST, Move::U_TURN, Move::CLOSE_COMBAT, Move::PAYBACK, Move::ASSURANCE, Move::EMBARGO, Move::FLING, Move::PSYCHO_SHIFT, Move::TRUMP_CARD, Move::HEAL_BLOCK, Move::WRING_OUT, Move::POWER_TRICK, Move::GASTRO_ACID, Move::LUCKY_CHANT, Move::ME_FIRST, Move::COPYCAT, Move::POWER_SWAP, Move::GUARD_SWAP, Move::PUNISHMENT, Move::LAST_RESORT, Move::WORRY_SEED, Move::SUCKER_PUNCH, Move::TOXIC_SPIKES, Move::HEART_SWAP, Move::AQUA_RING, Move::MAGNET_RISE, Move::FLARE_BLITZ, Move::FORCE_PALM, Move::AURA_SPHERE, Move::ROCK_POLISH, Move::POISON_JAB, Move::DARK_PULSE, Move::NIGHT_SLASH, Move::AQUA_TAIL, Move::SEED_BOMB, Move::AIR_SLASH, Move::X_SCISSOR, Move::BUG_BUZZ, Move::DRAGON_PULSE, Move::DRAGON_RUSH, Move::POWER_GEM, Move::DRAIN_PUNCH, Move::VACUUM_WAVE, Move::FOCUS_BLAST, Move::ENERGY_BALL, Move::BRAVE_BIRD, Move::EARTH_POWER, Move::SWITCHEROO, Move::GIGA_IMPACT, Move::NASTY_PLOT, Move::BULLET_PUNCH, Move::AVALANCHE, Move::ICE_SHARD, Move::SHADOW_CLAW, Move::THUNDER_FANG, Move::ICE_FANG, Move::FIRE_FANG, Move::SHADOW_SNEAK, Move::MUD_BOMB, Move::PSYCHO_CUT, Move::ZEN_HEADBUTT, Move::MIRROR_SHOT, Move::FLASH_CANNON, Move::ROCK_CLIMB, Move::DEFOG, Move::TRICK_ROOM, Move::DRACO_METEOR, Move::DISCHARGE, Move::LAVA_PLUME, Move::LEAF_STORM, Move::POWER_WHIP, Move::ROCK_WRECKER, Move::CROSS_POISON, Move::GUNK_SHOT, Move::IRON_HEAD, Move::MAGNET_BOMB, Move::STONE_EDGE, Move::CAPTIVATE, Move::STEALTH_ROCK, Move::GRASS_KNOT, Move::CHATTER, Move::JUDGMENT, Move::BUG_BITE, Move::CHARGE_BEAM, Move::WOOD_HAMMER, Move::AQUA_JET, Move::ATTACK_ORDER, Move::DEFEND_ORDER, Move::HEAL_ORDER, Move::HEAD_SMASH, Move::DOUBLE_HIT, Move::ROAR_OF_TIME, Move::SPACIAL_REND, Move::LUNAR_DANCE, Move::CRUSH_GRIP, Move::MAGMA_STORM, Move::DARK_VOID, Move::SEED_FLARE, Move::OMINOUS_WIND, Move::SHADOW_FORCE };
	std::ifstream file (name.c_str());
	std::string line;
	getline (file, line);
	for (unsigned n = 0; n != 6; ++n)
		load_pokemon (team, file, pokemon_converter, ability_converter, item_converter, move_converter, size);
	file.close ();
	team.size = team.pokemon.set.size ();
}

void load_pokemon (Team & team, std::ifstream & file, Species const pokemon_converter [], Ability::Abilities const ability_converter [], Item::Items const item_converter [], Move::Moves const move_converter [], unsigned size) {
	std::string line;
	getline (file, line);
	getline (file, line);
	Pokemon member (pokemon_converter [converter ("Num=\"", "\"", line)], team.size);
	int forme = converter ("Forme=\"", "\"", line);
	switch (member.name) {
		case DEOXYS_A:
			switch (forme) {
				case 0:
					member.name = DEOXYS_M;
					break;
				// case 1: is assumed
				case 1:
					break;
				case 2:
					member.name = DEOXYS_D;
					break;
				case 3:
					member.name = DEOXYS_S;
					break;
			}
			break;
		case GIRATINA_A:
			// Giratina-O has a forme value of 1
			member.name = static_cast<Species> (member.name + forme);
			break;
		case ROTOM:
			switch (forme) {
				case 1:
					member.name = ROTOM_C;
					break;
				case 2:
					member.name = ROTOM_H;
					break;
				case 3:
					member.name = ROTOM_F;
					break;
				case 4:
					member.name = ROTOM_W;
					break;
				case 5:
					member.name = ROTOM_S;
					break;
			}
			break;
		case SHAYMIN_L:
			// Shaymin-S has a forme value of 1
			member.name = static_cast<Species> (member.name + forme);
			break;
		case WORMADAM_P:
			// Wormadam-P is 0, Wormadam-S is 1, Wormadam-T is 2
			member.name = static_cast<Species> (member.name + forme);
		default:
			break;
	}

	member.ability = ability_converter [converter ("Ability=\"", "\"", line)];

	member.item = item_converter [converter ("Item=\"", "\"", line)];
	
	member.nature.name = static_cast <Nature::Natures> (converter ("Nature=\"", "\"", line));
	
	member.level = converter ("Lvl=\"", "\"", line);
	
	std::string const data = "Nickname=\"";
	size_t const x = data.length();
	size_t const a = line.find (data);
	size_t const b = line.find ("\"", a + x);
	member.nickname = line.substr (a + x, b - a - x);
	
	member.happiness = converter ("Happiness=\"", "\"", line);
	
	member.gender.from_simulator_int (converter ("Gender=\"", "\"", line));

	for (unsigned n = 0; n != 4; ++n) {
		getline (file, line);
		Move move (move_converter [converter ("<Move>", "</Move>", line)], 3, size);
		member.move.set.push_back (move);
	}
	getline (file, line);
	member.hp.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.atk.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.def.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spa.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spd.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.spe.iv = converter ("<DV>", "</DV>", line);
	getline (file, line);
	member.hp.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.atk.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.def.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spa.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spd.ev = converter ("<EV>", "</EV>", line);
	getline (file, line);
	member.spe.ev = converter ("<EV>", "</EV>", line);
	if (member.name != END_SPECIES and member.move.set.size() != 0)
		team.pokemon.set.push_back (member);
}

unsigned converter (std::string const & data, std::string const & end, std::string const & line) {
	size_t const x = data.length();
	size_t const a = line.find (data);
	size_t const b = line.find (end, a + x);
	return boost::lexical_cast<unsigned> (line.substr (a + x, b - a - x));
}

}

}
