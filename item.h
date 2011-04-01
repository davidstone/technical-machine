// Item header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>

namespace technicalmachine {

/*
MAIL collectively represents all types of mail, as they have identical effects.

POWER_ITEMS represents all items beginning with the word "Power", with the exception of Power Herb, as they have identical effects.

WAVE_INCENSE represents Sea Incense as well as Wave Incense, so chosen because they have identical effects and "Wave Incense" does not contain a homophone. A quick informal poll in #stark also shows support for Wave Incense over Sea Incense.

LAGGING_TAIL represents Full Incense as well as Lagging Tail, so chosen because they have identical effects and "Lagging Tail" does not contain the word "Incense" thus reducing the risk of a typo. It also sounds cooler and is the example item I've been using since I learned of these items, so is quicker to come to mind.

KINGS_ROCK represents King's Rock as well as Razor Fang. King's Rock was chosen because it is the item most people refer to when they consider the effect.

OTHER represents those items that can be held, but otherwise have no competitive effect. These items cannot be used with Fling. The OTHER items followed by a number represent those items that can be held and used with Fling (with the number representing Fling's power used with that item), but otherwise have no competitive effect.

Every other item listed has a unique competitive effect. For instance, Stone Plate, Hard Stone, and Rock Incense all have different powers when used with Fling. END_ITEM is used to make iterating over the list easier.

I used METRONOME_ITEM (along with METRONOME_MOVE) instead of just making one of them METRONOME and the other METRONOME_WHATEVER so that I will never get confused as to what to type. Attempting to use METRONOME will create an error on compile, allowing me to fix it.

There are 162 unique items.
*/

enum items { NO_ITEM, ADAMANT_ORB, AGUAV_BERRY, APICOT_BERRY, ASPEAR_BERRY, BABIRI_BERRY, BELUE_BERRY, BERRY_JUICE, BIG_ROOT, BLACK_BELT, BLACK_SLUDGE, BLACKGLASSES, BLUK_BERRY, BRIGHTPOWDER, CHARCOAL, CHARTI_BERRY, CHERI_BERRY, CHESTO_BERRY, CHILAN_BERRY, CHOICE_BAND, CHOICE_SCARF, CHOICE_SPECS, CHOPLE_BERRY, COBA_BERRY, COLBUR_BERRY, CORNN_BERRY, CUSTAP_BERRY, DAMP_ROCK, DEEPSEASCALE, DEEPSEATOOTH, DESTINY_KNOT, DRACO_PLATE, DRAGON_FANG, DREAD_PLATE, DURIN_BERRY, EARTH_PLATE, ENIGMA_BERRY, EXPERT_BELT, FIGY_BERRY, FIST_PLATE, FLAME_ORB, FLAME_PLATE, FOCUS_BAND, FOCUS_SASH, GANLON_BERRY, GREPA_BERRY, GRIP_CLAW, GRISEOUS_ORB, HABAN_BERRY, HARD_STONE, HEAT_ROCK, HONDEW_BERRY, IAPAPA_BERRY, ICICLE_PLATE, ICY_ROCK, INSECT_PLATE, IRON_BALL, IRON_PLATE, JABOCA_BERRY, KASIB_BERRY, KEBIA_BERRY, KELPSY_BERRY, KINGS_ROCK, LAGGING_TAIL, LANSAT_BERRY, LAX_INCENSE, LEFTOVERS, LEPPA_BERRY, LIECHI_BERRY, LIFE_ORB, LIGHT_BALL, LIGHT_CLAY, LUCKY_PUNCH, LUM_BERRY, LUSTROUS_ORB, MACHO_BRACE, MAGNET, MAGO_BERRY, MAGOST_BERRY, MAIL, MEADOW_PLATE, MENTAL_HERB, METAL_COAT, METAL_POWDER, METRONOME_ITEM, MICLE_BERRY, MIND_PLATE, MIRACLE_SEED, MUSCLE_BAND, MYSTIC_WATER, NANAB_BERRY, NEVERMELTICE, NOMEL_BERRY, OCCA_BERRY, ODD_INCENSE, ORAN_BERRY, PAMTRE_BERRY, PASSHO_BERRY, PAYAPA_BERRY, PECHA_BERRY, PERSIM_BERRY, PETAYA_BERRY, PINAP_BERRY, POISON_BARB, POMEG_BERRY, POWER_HERB, POWER_ITEMS, QUALOT_BERRY, QUICK_CLAW, QUICK_POWDER, RABUTA_BERRY, RAWST_BERRY, RAZOR_CLAW, RAZZ_BERRY, RINDO_BERRY, ROCK_INCENSE, ROSE_INCENSE, ROWAP_BERRY, SALAC_BERRY, SCOPE_LENS, SHARP_BEAK, SHED_SHELL, SHELL_BELL, SHUCA_BERRY, SILK_SCARF, SILVERPOWDER, SITRUS_BERRY, SKY_PLATE, SMOOTH_ROCK, SOFT_SAND, SOUL_DEW, SPELL_TAG, SPELON_BERRY, SPLASH_PLATE, SPOOKY_PLATE, STARF_BERRY, STICK, STICKY_BARB, STONE_PLATE, TAMATO_BERRY, TANGA_BERRY, THICK_CLUB, TOXIC_ORB, TOXIC_PLATE, TWISTEDSPOON, WACAN_BERRY, WATMEL_BERRY, WAVE_INCENSE, WEPEAR_BERRY, WHITE_HERB, WIDE_LENS, WIKI_BERRY, WISE_GLASSES, YACHE_BERRY, ZAP_PLATE, ZOOM_LENS, OTHER, OTHER10, OTHER30, OTHER50, OTHER80, OTHER100, END_ITEM };

const std::string item_name [] = { "No Item", "Adamant Orb ", "Aguav Berry", "Apicot Berry", "Aspear Berry", "Babiri Berry", "Belue Berry", "Berry Juice", "Big Root", "Black Belt", "Black Sludge", "BlackGlasses", "Bluk Berry", "BrightPowder", "Charcoal", "Charti Berry", "Cheri Berry", "Chesto Berry", "Chilan Berry", "Choice Band", "Choice Scarf", "Choice Specs", "Chople Berry", "Coba Berry", "Colbur Berry", "Cornn Berry", "Custap Berry", "Damp Rock", "DeepSeaScale", "DeepSeaTooth", "Destiny Knot", "Draco Plate", "Dragon Fang", "Dread Plate", "Durin Berry", "Earth Plate", "Enigma Berry", "Expert Belt", "Figy Berry", "Fist Plate", "Flame Orb", "Flame Plate", "Focus Band", "Focus Sash", "Ganlon Berry", "Grepa Berry", "Grip Claw", "Griseous Orb", "Haban Berry", "Hard Stone", "Heat Rock", "Hondew Berry", "Iapapa Berry", "Icicle Plate", "Icy Rock", "Insect Plate", "Iron Ball", "Iron Plate", "Jaboca Berry", "Kasib Berry", "Kebia Berry", "Kelpsy Berry", "King's Rock", "Lagging Tail", "Lansat Berry", "Lax Incense", "Leftovers", "Leppa Berry", "Liechi Berry", "Life Orb", "Light Ball", "Light Clay", "Lucky Punch", "Lum Berry", "Lustrous Orb", "Macho Brace", "Magnet", "Mago Berry", "Magost Berry", "Mail", "Meadow Plate", "Mental Herb", "Metal Coat", "Metal Powder", "Metronome", "Micle Berry", "Mind Plate", "Miracle Seed", "Muscle Band", "Mystic Water", "Nanab Berry", "NeverMeltIce", "Nomel Berry", "Occa Berry", "Odd Incense", "Oran Berry", "Pamtre Berry", "Passho Berry", "Payapa Berry", "Pecha Berry", "Persim Berry", "Petaya Berry", "Pinap Berry", "Poison Barb", "Pomeg Berry", "Power Herb", "Power Item", "Qualot Berry", "Quick Claw", "Quick Powder", "Rabuta Berry", "Rawst Berry", "Razor Claw", "Razz Berry", "Rindo Berry", "Rock Incense", "Rose Incense", "Rowap Berry", "Salac Berry", "Scope Lens", "Sharp Beak", "Shed Shell", "Shell Bell", "Shuca Berry", "Silk Scarf", "SilverPowder", "Sitrus Berry", "Sky Plate", "Smooth Rock", "Soft Sand", "Soul Dew", "Spell Tag", "Spelon Berry", "Splash Plate", "Spooky Plate", "Starf Berry", "Stick", "Sticky Barb", "Stone Plate", "Tamato Berry", "Tanga Berry", "Thick Club", "Toxic Orb", "Toxic Plate", "TwistedSpoon", "Wacan Berry", "Watmel Berry", "Wave Incense", "Wepear Berry", "White Herb", "Wide Lens", "Wiki Berry", "Wise Glasses", "Yache Berry", "Zap Plate", "Zoom Lens", "Other", "Other10", "Other30", "Other50", "Other80", "Other100" };

void set_item_map (std::map <std::string, items> &items_map);

}
#endif
