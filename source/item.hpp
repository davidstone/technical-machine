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

#pragma once

#include "enum.hpp"

#include <cstdint>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

enum class Item : uint16_t {
	No_Item, Adamant_Orb, Aguav_Berry, Air_Mail, Amulet_Coin,
	Antidote, Apicot_Berry, Armor_Fossil, Aspear_Berry, Awakening,
	Babiri_Berry, Belue_Berry, Berry_Juice, Bicycle, Big_Mushroom,
	Big_Pearl, Big_Root, Black_Belt, Black_Flute, Black_Sludge,
	BlackGlasses, Bloom_Mail, Blue_Flute, Blue_Scarf, Blue_Shard,
	Bluk_Berry, Brick_Mail, BrightPowder, Bubble_Mail, Burn_Heal,
	Calcium, Carbos, Charcoal, Charti_Berry, Cheri_Berry,
	Cherish_Ball, Chesto_Berry, Chilan_Berry, Choice_Band, Choice_Scarf,
	Choice_Specs, Chople_Berry, Claw_Fossil, Cleanse_Tag, Coba_Berry,
	Coin_Case, Colbur_Berry, Cornn_Berry, Coupon_1, Coupon_2,
	Coupon_3, Custap_Berry, Damp_Mulch, Damp_Rock, Dawn_Stone,
	DeepSeaScale, DeepSeaTooth, Destiny_Knot, Dire_Hit, Dive_Ball,
	Dome_Fossil, Draco_Plate, Dragon_Fang, Dragon_Scale, Dread_Plate,
	Dubious_Disc, Durin_Berry, Dusk_Ball, Dusk_Stone, Earth_Plate,
	Electirizer, Elixir, Energy_Root, EnergyPowder, Enigma_Berry,
	Escape_Rope, Ether, Everstone, Exp_Share, Expert_Belt,
	Explorer_Kit, Fashion_Case, Figy_Berry, Fire_Stone, Fist_Plate,
	Flame_Mail, Flame_Orb, Flame_Plate, Fluffy_Tail, Focus_Band,
	Focus_Sash, Fresh_Water, Full_Heal, Full_Incense, Full_Sleep_Restore,
	Galactic_Key, Ganlon_Berry, Good_Rod, Gooey_Mulch, Grass_Mail,
	Great_Ball, Green_Scarf, Green_Shard, Grepa_Berry, Grip_Claw,
	Griseous_Orb, Growth_Mulch, Guard_Spec, Haban_Berry, Hard_Stone,
	Heal_Ball, Heal_Powder, Heart_Mail, Heart_Scale, Heat_Rock,
	Helix_Fossil, HM01, HM02, HM03, HM04,
	HM05, HM06, HM07, HM08, Hondew_Berry,
	Honey, HP_Up, Hyper_Potion, Iapapa_Berry, Ice_Heal,
	Icicle_Plate, Icy_Rock, Insect_Plate, Iron, Iron_Ball,
	Iron_Plate, Jaboca_Berry, Journal, Kasib_Berry, Kebia_Berry,
	Kelpsy_Berry, Kings_Rock, Lagging_Tail, Lansat_Berry, Lava_Cookie,
	Lax_Incense, Leaf_Stone, Leftovers, Lemonade, Leppa_Berry,
	Liechi_Berry, Life_Orb, Light_Ball, Light_Clay, Loot_Sack,
	Luck_Incense, Lucky_Egg, Lucky_Punch, Lum_Berry, Lunar_Wing,
	Lustrous_Orb, Luxury_Ball, Macho_Brace, Magmarizer, Magnet,
	Mago_Berry, Magost_Berry, Master_Ball, Max_Elixir, Max_Ether,
	Max_Potion, Max_Repel, Max_Revive, Meadow_Plate, Member_Card,
	Mental_Herb, Metal_Coat, Metal_Powder, Metronome, Micle_Berry,
	Mind_Plate, Miracle_Seed, MooMoo_Milk, Moon_Stone, Mosaic_Mail,
	Muscle_Band, Mystic_Water, Nanab_Berry, Nest_Ball, Net_Ball,
	NeverMeltIce, Nomel_Berry, Nugget, Oaks_Letter, Occa_Berry,
	Odd_Incense, Odd_Keystone, Old_Amber, Old_Charm, Old_Gateau,
	Old_Rod, Oran_Berry, Oval_Stone, Pal_Pad, Pamtre_Berry,
	Parcel, Parlyz_Heal, Passho_Berry, Payapa_Berry, Pearl,
	Pecha_Berry, Persim_Berry, Petaya_Berry, Pinap_Berry, Pink_Scarf,
	Poffin_Case, Point_Card, Poison_Barb, Poke_Ball, Poke_Doll,
	Poke_Radar, Pomeg_Berry, Potion, Power_Anklet, Power_Band,
	Power_Belt, Power_Bracer, Power_Herb, Power_Lens, Power_Weight,
	PP_Max, PP_Up, Premier_Ball, Protector, Protein,
	Pure_Incense, Qualot_Berry, Quick_Ball, Quick_Claw, Quick_Powder,
	Rabuta_Berry, Rare_Bone, Rare_Candy, Rawst_Berry, Razor_Claw,
	Razor_Fang, Razz_Berry, Reaper_Cloth, Red_Flute, Red_Scarf,
	Red_Shard, Repeat_Ball, Repel, Revival_Herb, Revive,
	Rindo_Berry, Rock_Incense, Root_Fossil, Rose_Incense, Rowap_Berry,
	Sacred_Ash, Safari_Ball, Salac_Berry, Scope_Lens, Sea_Incense,
	Seal_Bag, Seal_Case, SecretPotion, Sharp_Beak, Shed_Shell,
	Shell_Bell, Shiny_Stone, Shoal_Salt, Shoal_Shell, Shuca_Berry,
	Silk_Scarf, SilverPowder, Sitrus_Berry, Skull_Fossil, Sky_Plate,
	Smoke_Ball, Smooth_Rock, Snow_Mail, Soda_Pop, Soft_Sand,
	Soothe_Bell, Soul_Dew, Space_Mail, Spell_Tag, Spelon_Berry,
	Splash_Plate, Spooky_Plate, Sprayduck, Stable_Mulch, Star_Piece,
	Stardust, Starf_Berry, Steel_Mail, Stick, Sticky_Barb,
	Stone_Plate, Storage_Key, Suite_Key, Sun_Stone, Super_Potion,
	Super_Repel, Super_Rod, Tamato_Berry, Tanga_Berry, Thick_Club,
	Thunderstone, Timer_Ball, TinyMushroom, TM01, TM02,
	TM03, TM04, TM05, TM06, TM07,
	TM08, TM09, TM10, TM11, TM12,
	TM13, TM14, TM15, TM16, TM17,
	TM18, TM19, TM20, TM21, TM22,
	TM23, TM24, TM25, TM26, TM27,
	TM28, TM29, TM30, TM31, TM32,
	TM33, TM34, TM35, TM36, TM37,
	TM38, TM39, TM40, TM41, TM42,
	TM43, TM44, TM45, TM46, TM47,
	TM48, TM49, TM50, TM51, TM52,
	TM53, TM54, TM55, TM56, TM57,
	TM58, TM59, TM60, TM61, TM62,
	TM63, TM64, TM65, TM66, TM67,
	TM68, TM69, TM70, TM71, TM72,
	TM73, TM74, TM75, TM76, TM77,
	TM78, TM79, TM80, TM81, TM82,
	TM83, TM84, TM85, TM86, TM87,
	TM88, TM89, TM90, TM91, TM92,
	Town_Map, Toxic_Orb, Toxic_Plate, Tunnel_Mail, TwistedSpoon,
	Ultra_Ball, Up_Grade, VS_Seeker, Wacan_Berry, Water_Stone,
	Watmel_Berry, Wave_Incense, Wepear_Berry, White_Flute, White_Herb,
	Wide_Lens, Wiki_Berry, Wise_Glasses, Works_Key, X_Accuracy,
	X_Attack, X_Defend, X_Sp_Def, X_Special, X_Speed,
	Yache_Berry, Yellow_Flute, Yellow_Scarf, Yellow_Shard, Zap_Plate,
	Zinc, Zoom_Lens, END
};

bool is_set(Item item);
bool allows_switching(Item item);
bool boosts_super_effective_moves(Item item);
bool causes_recoil(Item item);
bool grounds(Item item);
bool is_choice_item(Item item);
bool was_lost(Item item);
void remove(Item & item);
bounded::integer<0, 80> berry_power(Item item);		// Returns 0 for non-berries
bounded::integer<0, 130> fling_power(Item item);
bool blocks_trick(Item item);
bool extends_hail(Item item);
bool extends_rain(Item item);
bool extends_sand(Item item);
bool extends_sun(Item item);
bool extends_light_screen(Item item);
bool extends_reflect(Item item);
void steal(Item & mine, Item & other);

}	// namespace technicalmachine

namespace bounded {

template<>
struct basic_numeric_limits<technicalmachine::Item> : technicalmachine::basic_numeric_limits<technicalmachine::Item> {};

}	// namespace bounded

