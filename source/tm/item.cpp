// Item functions
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

#include <tm/item.hpp>
#include <algorithm>

namespace technicalmachine {
using namespace bounded::literal;

bounded::integer<0, 80> berry_power(Item const item) {
	switch (item) {
		case Item::Aguav_Berry:
		case Item::Aspear_Berry:
		case Item::Babiri_Berry:
		case Item::Charti_Berry:
		case Item::Cheri_Berry:
		case Item::Chesto_Berry:
		case Item::Chilan_Berry:
		case Item::Chople_Berry:
		case Item::Coba_Berry:
		case Item::Colbur_Berry:
		case Item::Figy_Berry:
		case Item::Haban_Berry:
		case Item::Iapapa_Berry:
		case Item::Kasib_Berry:
		case Item::Kebia_Berry:
		case Item::Leppa_Berry:
		case Item::Lum_Berry:
		case Item::Mago_Berry:
		case Item::Occa_Berry:
		case Item::Oran_Berry:
		case Item::Passho_Berry:
		case Item::Payapa_Berry:
		case Item::Pecha_Berry:
		case Item::Persim_Berry:
		case Item::Rawst_Berry:
		case Item::Razz_Berry:
		case Item::Rindo_Berry:
		case Item::Shuca_Berry:
		case Item::Sitrus_Berry:
		case Item::Tanga_Berry:
		case Item::Wacan_Berry:
		case Item::Wiki_Berry:
		case Item::Yache_Berry:
			return 60_bi;
		case Item::Bluk_Berry:
		case Item::Cornn_Berry:
		case Item::Grepa_Berry:
		case Item::Hondew_Berry:
		case Item::Kelpsy_Berry:
		case Item::Magost_Berry:
		case Item::Nanab_Berry:
		case Item::Nomel_Berry:
		case Item::Pamtre_Berry:
		case Item::Pinap_Berry:
		case Item::Pomeg_Berry:
		case Item::Qualot_Berry:
		case Item::Rabuta_Berry:
		case Item::Spelon_Berry:
		case Item::Tamato_Berry:
		case Item::Wepear_Berry:
			return 70_bi;
		case Item::Apicot_Berry:
		case Item::Belue_Berry:
		case Item::Custap_Berry:
		case Item::Durin_Berry:
		case Item::Enigma_Berry:
		case Item::Ganlon_Berry:
		case Item::Jaboca_Berry:
		case Item::Lansat_Berry:
		case Item::Liechi_Berry:
		case Item::Micle_Berry:
		case Item::Petaya_Berry:
		case Item::Rowap_Berry:
		case Item::Salac_Berry:
		case Item::Starf_Berry:
		case Item::Watmel_Berry:
			return 80_bi;
		default:
			return 0_bi;
	}
}

bounded::integer<0, 130> fling_power(Item const item) {
	switch (item) {
		case Item::Iron_Ball:
			return 130_bi;
		case Item::Hard_Stone:
		case Item::Armor_Fossil:
		case Item::Claw_Fossil:
		case Item::Dome_Fossil:
		case Item::Helix_Fossil:
		case Item::Old_Amber:
		case Item::Rare_Bone:
		case Item::Root_Fossil:
		case Item::Skull_Fossil:
			return 100_bi;
		case Item::DeepSeaTooth:
		case Item::Draco_Plate:
		case Item::Dread_Plate:
		case Item::Earth_Plate:
		case Item::Fist_Plate:
		case Item::Flame_Plate:
		case Item::Grip_Claw:
		case Item::Icicle_Plate:
		case Item::Insect_Plate:
		case Item::Iron_Plate:
		case Item::Meadow_Plate:
		case Item::Mind_Plate:
		case Item::Sky_Plate:
		case Item::Splash_Plate:
		case Item::Spooky_Plate:
		case Item::Stone_Plate:
		case Item::Thick_Club:
		case Item::Toxic_Plate:
		case Item::Zap_Plate:
			return 90_bi;
		case Item::Quick_Claw:
		case Item::Razor_Claw:
		case Item::Sticky_Barb:
		case Item::Dawn_Stone:
		case Item::Dusk_Stone:
		case Item::Electirizer:
		case Item::Magmarizer:
		case Item::Odd_Keystone:
		case Item::Oval_Stone:
		case Item::Protector:
		case Item::Shiny_Stone:
			return 80_bi;
		case Item::Dragon_Fang:
		case Item::Poison_Barb:
		case Item::Power_Anklet:
		case Item::Power_Band:
		case Item::Power_Belt:
		case Item::Power_Bracer:
		case Item::Power_Lens:
		case Item::Power_Weight:
			return 70_bi;
		case Item::Adamant_Orb:
		case Item::Damp_Rock:
		case Item::Heat_Rock:
		case Item::Lustrous_Orb:
		case Item::Macho_Brace:
		case Item::Stick:
			return 60_bi;
		case Item::Sharp_Beak:
		case Item::Dubious_Disc:
			return 50_bi;
		case Item::Icy_Rock:
		case Item::Lucky_Punch:
			return 40_bi;
		case Item::Berry_Juice:
		case Item::Black_Belt:
		case Item::Black_Sludge:
		case Item::BlackGlasses:
		case Item::Charcoal:
		case Item::DeepSeaScale:
		case Item::Flame_Orb:
		case Item::Kings_Rock:
		case Item::Life_Orb:
		case Item::Light_Ball:
		case Item::Light_Clay:
		case Item::Magnet:
		case Item::Metal_Coat:
		case Item::Metronome:
		case Item::Miracle_Seed:
		case Item::Mystic_Water:
		case Item::NeverMeltIce:
		case Item::Razor_Fang:
		case Item::Scope_Lens:
		case Item::Shell_Bell:
		case Item::Soul_Dew:
		case Item::Spell_Tag:
		case Item::Toxic_Orb:
		case Item::TwistedSpoon:
		case Item::Amulet_Coin:
		case Item::Antidote:
		case Item::Awakening:
		case Item::Big_Mushroom:
		case Item::Big_Pearl:
		case Item::Black_Flute:
		case Item::Blue_Flute:
		case Item::Blue_Shard:
		case Item::Burn_Heal:
		case Item::Calcium:
		case Item::Carbos:
		case Item::Cleanse_Tag:
		case Item::Damp_Mulch:
		case Item::Dire_Hit:
		case Item::Dragon_Scale:
		case Item::Elixir:
		case Item::Energy_Root:
		case Item::EnergyPowder:
		case Item::Escape_Rope:
		case Item::Ether:
		case Item::Everstone:
		case Item::Exp_Share:
		case Item::Fire_Stone:
		case Item::Fluffy_Tail:
		case Item::Fresh_Water:
		case Item::Full_Heal:
		case Item::Full_Restore:
		case Item::Gooey_Mulch:
		case Item::Green_Shard:
		case Item::Growth_Mulch:
		case Item::Guard_Spec:
		case Item::Heal_Powder:
		case Item::Heart_Scale:
		case Item::Honey:
		case Item::HP_Up:
		case Item::Hyper_Potion:
		case Item::Ice_Heal:
		case Item::Iron:
		case Item::Lava_Cookie:
		case Item::Leaf_Stone:
		case Item::Lemonade:
		case Item::Lucky_Egg:
		case Item::Max_Elixir:
		case Item::Max_Ether:
		case Item::Max_Potion:
		case Item::Max_Repel:
		case Item::Max_Revive:
		case Item::MooMoo_Milk:
		case Item::Moon_Stone:
		case Item::Nugget:
		case Item::Old_Gateau:
		case Item::Parlyz_Heal:
		case Item::Pearl:
		case Item::Poke_Doll:
		case Item::Potion:
		case Item::PP_Max:
		case Item::PP_Up:
		case Item::Protein:
		case Item::Rare_Candy:
		case Item::Red_Flute:
		case Item::Red_Shard:
		case Item::Repel:
		case Item::Revival_Herb:
		case Item::Revive:
		case Item::Sacred_Ash:
		case Item::Shoal_Salt:
		case Item::Shoal_Shell:
		case Item::Smoke_Ball:
		case Item::Soda_Pop:
		case Item::Stable_Mulch:
		case Item::Star_Piece:
		case Item::Stardust:
		case Item::Sun_Stone:
		case Item::Super_Potion:
		case Item::Super_Repel:
		case Item::Thunderstone:
		case Item::TinyMushroom:
		case Item::Up_Grade:
		case Item::Water_Stone:
		case Item::White_Flute:
		case Item::X_Accuracy:
		case Item::X_Attack:
		case Item::X_Defend:
		case Item::X_Sp_Def:
		case Item::X_Special:
		case Item::X_Speed:
		case Item::Yellow_Flute:
		case Item::Yellow_Shard:
		case Item::Zinc:
			return 30_bi;
		case Item::Big_Root:
		case Item::Blue_Scarf:
		case Item::BrightPowder:
		case Item::Choice_Band:
		case Item::Choice_Scarf:
		case Item::Choice_Specs:
		case Item::Destiny_Knot:
		case Item::Expert_Belt:
		case Item::Focus_Band:
		case Item::Focus_Sash:
		case Item::Full_Incense:
		case Item::Green_Scarf:
		case Item::Lagging_Tail:
		case Item::Lax_Incense:
		case Item::Leftovers:
		case Item::Luck_Incense:
		case Item::Mental_Herb:
		case Item::Metal_Powder:
		case Item::Muscle_Band:
		case Item::Odd_Incense:
		case Item::Pink_Scarf:
		case Item::Power_Herb:
		case Item::Pure_Incense:
		case Item::Quick_Powder:
		case Item::Reaper_Cloth:
		case Item::Red_Scarf:
		case Item::Rock_Incense:
		case Item::Rose_Incense:
		case Item::Sea_Incense:
		case Item::Shed_Shell:
		case Item::Silk_Scarf:
		case Item::SilverPowder:
		case Item::Smooth_Rock:
		case Item::Soft_Sand:
		case Item::Soothe_Bell:
		case Item::Wave_Incense:
		case Item::White_Herb:
		case Item::Wide_Lens:
		case Item::Wise_Glasses:
		case Item::Yellow_Scarf:
		case Item::Zoom_Lens:
		case Item::Aguav_Berry:
		case Item::Aspear_Berry:
		case Item::Babiri_Berry:
		case Item::Charti_Berry:
		case Item::Cheri_Berry:
		case Item::Chesto_Berry:
		case Item::Chilan_Berry:
		case Item::Chople_Berry:
		case Item::Coba_Berry:
		case Item::Colbur_Berry:
		case Item::Figy_Berry:
		case Item::Haban_Berry:
		case Item::Iapapa_Berry:
		case Item::Kasib_Berry:
		case Item::Kebia_Berry:
		case Item::Leppa_Berry:
		case Item::Lum_Berry:
		case Item::Mago_Berry:
		case Item::Occa_Berry:
		case Item::Oran_Berry:
		case Item::Passho_Berry:
		case Item::Payapa_Berry:
		case Item::Pecha_Berry:
		case Item::Persim_Berry:
		case Item::Rawst_Berry:
		case Item::Razz_Berry:
		case Item::Rindo_Berry:
		case Item::Shuca_Berry:
		case Item::Sitrus_Berry:
		case Item::Tanga_Berry:
		case Item::Wacan_Berry:
		case Item::Wiki_Berry:
		case Item::Yache_Berry:
		case Item::Bluk_Berry:
		case Item::Cornn_Berry:
		case Item::Grepa_Berry:
		case Item::Hondew_Berry:
		case Item::Kelpsy_Berry:
		case Item::Magost_Berry:
		case Item::Nanab_Berry:
		case Item::Nomel_Berry:
		case Item::Pamtre_Berry:
		case Item::Pinap_Berry:
		case Item::Pomeg_Berry:
		case Item::Qualot_Berry:
		case Item::Rabuta_Berry:
		case Item::Spelon_Berry:
		case Item::Tamato_Berry:
		case Item::Wepear_Berry:
		case Item::Apicot_Berry:
		case Item::Belue_Berry:
		case Item::Custap_Berry:
		case Item::Durin_Berry:
		case Item::Enigma_Berry:
		case Item::Ganlon_Berry:
		case Item::Jaboca_Berry:
		case Item::Lansat_Berry:
		case Item::Liechi_Berry:
		case Item::Micle_Berry:
		case Item::Petaya_Berry:
		case Item::Rowap_Berry:
		case Item::Salac_Berry:
		case Item::Starf_Berry:
		case Item::Watmel_Berry:
			return 10_bi;
		default:
			return 0_bi;
	}
}

bool blocks_trick(Item const item) {
	switch (item) {
		case Item::Air_Mail:
		case Item::Bloom_Mail:
		case Item::Brick_Mail:
		case Item::Bubble_Mail:
		case Item::Flame_Mail:
		case Item::Grass_Mail:
		case Item::Heart_Mail:
		case Item::Mosaic_Mail:
		case Item::Snow_Mail:
		case Item::Space_Mail:
		case Item::Steel_Mail:
		case Item::Tunnel_Mail:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
