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

#include <tm/move/move.hpp>

#include <cstdint>

#include <tm/move/base_power.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/move/moves.hpp>

namespace technicalmachine {

auto is_damaging(Moves const move) -> bool {
	auto const power = base_power(move);
	return !static_cast<bool>(power) or *power != 0_bi;
}

auto is_phaze(Moves const move) -> bool {
	switch (move) {
		case Moves::Roar:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

auto is_usable_while_frozen(Moves const move) -> bool {
	switch (move) {
		case Moves::Flame_Wheel:
		case Moves::Sacred_Fire:
			return true;
		default:
			return false;
	}
}

auto is_regular(Moves const move) -> bool {
	switch (move) {
		case Moves::Pass:
		case Moves::Hit_Self:
		case Moves::Struggle:
			return false;
		default:
			return !is_switch(move);
	}
}


auto is_physical(Moves const move) -> bool {
	switch (move) {
		case Moves::Hit_Self:
		case Moves::Pound:
		case Moves::Karate_Chop:
		case Moves::DoubleSlap:
		case Moves::Comet_Punch:
		case Moves::Mega_Punch:
		case Moves::Pay_Day:
		case Moves::Fire_Punch:
		case Moves::Ice_Punch:
		case Moves::ThunderPunch:
		case Moves::Scratch:
		case Moves::ViceGrip:
		case Moves::Guillotine:
		case Moves::Cut:
		case Moves::Wing_Attack:
		case Moves::Fly:
		case Moves::Bind:
		case Moves::Slam:
		case Moves::Vine_Whip:
		case Moves::Stomp:
		case Moves::Double_Kick:
		case Moves::Mega_Kick:
		case Moves::Jump_Kick:
		case Moves::Rolling_Kick:
		case Moves::Headbutt:
		case Moves::Horn_Attack:
		case Moves::Fury_Attack:
		case Moves::Horn_Drill:
		case Moves::Tackle:
		case Moves::Body_Slam:
		case Moves::Wrap:
		case Moves::Take_Down:
		case Moves::Thrash:
		case Moves::Double_Edge:
		case Moves::Poison_Sting:
		case Moves::Twineedle:
		case Moves::Pin_Missile:
		case Moves::Bite:
		case Moves::Peck:
		case Moves::Drill_Peck:
		case Moves::Submission:
		case Moves::Low_Kick:
		case Moves::Counter:
		case Moves::Seismic_Toss:
		case Moves::Strength:
		case Moves::Razor_Leaf:
		case Moves::Rock_Throw:
		case Moves::Earthquake:
		case Moves::Fissure:
		case Moves::Dig:
		case Moves::Quick_Attack:
		case Moves::Rage:
		case Moves::Bide:
		case Moves::Selfdestruct:
		case Moves::Egg_Bomb:
		case Moves::Lick:
		case Moves::Bone_Club:
		case Moves::Waterfall:
		case Moves::Clamp:
		case Moves::Skull_Bash:
		case Moves::Spike_Cannon:
		case Moves::Constrict:
		case Moves::Hi_Jump_Kick:
		case Moves::Barrage:
		case Moves::Leech_Life:
		case Moves::Sky_Attack:
		case Moves::Dizzy_Punch:
		case Moves::Crabhammer:
		case Moves::Explosion:
		case Moves::Fury_Swipes:
		case Moves::Bonemerang:
		case Moves::Rock_Slide:
		case Moves::Hyper_Fang:
		case Moves::Super_Fang:
		case Moves::Slash:
		case Moves::Struggle:
		case Moves::Triple_Kick:
		case Moves::Thief:
		case Moves::Flame_Wheel:
		case Moves::Flail:
		case Moves::Reversal:
		case Moves::Mach_Punch:
		case Moves::Faint_Attack:
		case Moves::Bone_Rush:
		case Moves::Outrage:
		case Moves::Rollout:
		case Moves::False_Swipe:
		case Moves::Spark:
		case Moves::Fury_Cutter:
		case Moves::Steel_Wing:
		case Moves::Return:
		case Moves::Present:
		case Moves::Frustration:
		case Moves::Sacred_Fire:
		case Moves::Magnitude:
		case Moves::DynamicPunch:
		case Moves::Megahorn:
		case Moves::Pursuit:
		case Moves::Rapid_Spin:
		case Moves::Iron_Tail:
		case Moves::Metal_Claw:
		case Moves::Vital_Throw:
		case Moves::Cross_Chop:
		case Moves::Crunch:
		case Moves::ExtremeSpeed:
		case Moves::Rock_Smash:
		case Moves::Beat_Up:
		case Moves::Fake_Out:
		case Moves::Facade:
		case Moves::Focus_Punch:
		case Moves::SmellingSalt:
		case Moves::Superpower:
		case Moves::Revenge:
		case Moves::Brick_Break:
		case Moves::Knock_Off:
		case Moves::Endeavor:
		case Moves::Secret_Power:
		case Moves::Dive:
		case Moves::Arm_Thrust:
		case Moves::Blaze_Kick:
		case Moves::Ice_Ball:
		case Moves::Needle_Arm:
		case Moves::Poison_Fang:
		case Moves::Crush_Claw:
		case Moves::Meteor_Mash:
		case Moves::Astonish:
		case Moves::Rock_Tomb:
		case Moves::Shadow_Punch:
		case Moves::Sky_Uppercut:
		case Moves::Sand_Tomb:
		case Moves::Bullet_Seed:
		case Moves::Aerial_Ace:
		case Moves::Icicle_Spear:
		case Moves::Dragon_Claw:
		case Moves::Bounce:
		case Moves::Poison_Tail:
		case Moves::Covet:
		case Moves::Volt_Tackle:
		case Moves::Leaf_Blade:
		case Moves::Rock_Blast:
		case Moves::Wake_Up_Slap:
		case Moves::Hammer_Arm:
		case Moves::Gyro_Ball:
		case Moves::Natural_Gift:
		case Moves::Feint:
		case Moves::Pluck:
		case Moves::Metal_Burst:
		case Moves::U_turn:
		case Moves::Close_Combat:
		case Moves::Payback:
		case Moves::Assurance:
		case Moves::Fling:
		case Moves::Punishment:
		case Moves::Last_Resort:
		case Moves::Sucker_Punch:
		case Moves::Flare_Blitz:
		case Moves::Force_Palm:
		case Moves::Poison_Jab:
		case Moves::Night_Slash:
		case Moves::Aqua_Tail:
		case Moves::Seed_Bomb:
		case Moves::X_Scissor:
		case Moves::Dragon_Rush:
		case Moves::Drain_Punch:
		case Moves::Brave_Bird:
		case Moves::Giga_Impact:
		case Moves::Bullet_Punch:
		case Moves::Avalanche:
		case Moves::Ice_Shard:
		case Moves::Shadow_Claw:
		case Moves::Thunder_Fang:
		case Moves::Ice_Fang:
		case Moves::Fire_Fang:
		case Moves::Shadow_Sneak:
		case Moves::Psycho_Cut:
		case Moves::Zen_Headbutt:
		case Moves::Rock_Climb:
		case Moves::Power_Whip:
		case Moves::Rock_Wrecker:
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Iron_Head:
		case Moves::Magnet_Bomb:
		case Moves::Stone_Edge:
		case Moves::Bug_Bite:
		case Moves::Wood_Hammer:
		case Moves::Aqua_Jet:
		case Moves::Attack_Order:
		case Moves::Head_Smash:
		case Moves::Double_Hit:
		case Moves::Crush_Grip:
		case Moves::Shadow_Force:
		case Moves::Smack_Down:
		case Moves::Storm_Throw:
		case Moves::Heavy_Slam:
		case Moves::Flame_Charge:
		case Moves::Low_Sweep:
		case Moves::Foul_Play:
		case Moves::Chip_Away:
		case Moves::Sky_Drop:
		case Moves::Circle_Throw:
		case Moves::Acrobatics:
		case Moves::Retaliate:
		case Moves::Bulldoze:
		case Moves::Dragon_Tail:
		case Moves::Wild_Charge:
		case Moves::Drill_Run:
		case Moves::Dual_Chop:
		case Moves::Heart_Stamp:
		case Moves::Horn_Leech:
		case Moves::Sacred_Sword:
		case Moves::Razor_Shell:
		case Moves::Heat_Crash:
		case Moves::Steamroller:
		case Moves::Tail_Slap:
		case Moves::Head_Charge:
		case Moves::Gear_Grind:
		case Moves::Bolt_Strike:
		case Moves::Freeze_Shock:
		case Moves::Icicle_Crash:
		case Moves::V_create:
		case Moves::Fusion_Bolt:
			return true;
		default:
			return false;
	}
}

auto is_special(Moves const move) -> bool {
	switch (move) {
		case Moves::Razor_Wind:
		case Moves::Gust:
		case Moves::SonicBoom:
		case Moves::Acid:
		case Moves::Ember:
		case Moves::Flamethrower:
		case Moves::Water_Gun:
		case Moves::Hydro_Pump:
		case Moves::Surf:
		case Moves::Ice_Beam:
		case Moves::Blizzard:
		case Moves::Psybeam:
		case Moves::BubbleBeam:
		case Moves::Aurora_Beam:
		case Moves::Hyper_Beam:
		case Moves::Absorb:
		case Moves::Mega_Drain:
		case Moves::SolarBeam:
		case Moves::Petal_Dance:
		case Moves::Dragon_Rage:
		case Moves::Fire_Spin:
		case Moves::ThunderShock:
		case Moves::Thunderbolt:
		case Moves::Thunder:
		case Moves::Confusion:
		case Moves::Psychic:
		case Moves::Night_Shade:
		case Moves::Smog:
		case Moves::Sludge:
		case Moves::Fire_Blast:
		case Moves::Swift:
		case Moves::Dream_Eater:
		case Moves::Bubble:
		case Moves::Psywave:
		case Moves::Tri_Attack:
		case Moves::Snore:
		case Moves::Aeroblast:
		case Moves::Powder_Snow:
		case Moves::Sludge_Bomb:
		case Moves::Mud_Slap:
		case Moves::Octazooka:
		case Moves::Zap_Cannon:
		case Moves::Icy_Wind:
		case Moves::Giga_Drain:
		case Moves::DragonBreath:
		case Moves::Hidden_Power:
		case Moves::Twister:
		case Moves::Mirror_Coat:
		case Moves::AncientPower:
		case Moves::Shadow_Ball:
		case Moves::Future_Sight:
		case Moves::Whirlpool:
		case Moves::Uproar:
		case Moves::Spit_Up:
		case Moves::Heat_Wave:
		case Moves::Eruption:
		case Moves::Luster_Purge:
		case Moves::Mist_Ball:
		case Moves::Hyper_Voice:
		case Moves::Blast_Burn:
		case Moves::Hydro_Cannon:
		case Moves::Weather_Ball:
		case Moves::Air_Cutter:
		case Moves::Overheat:
		case Moves::Silver_Wind:
		case Moves::Water_Spout:
		case Moves::Signal_Beam:
		case Moves::Extrasensory:
		case Moves::Sheer_Cold:
		case Moves::Muddy_Water:
		case Moves::Frenzy_Plant:
		case Moves::Mud_Shot:
		case Moves::Magical_Leaf:
		case Moves::Shock_Wave:
		case Moves::Water_Pulse:
		case Moves::Doom_Desire:
		case Moves::Psycho_Boost:
		case Moves::Brine:
		case Moves::Trump_Card:
		case Moves::Wring_Out:
		case Moves::Aura_Sphere:
		case Moves::Dark_Pulse:
		case Moves::Air_Slash:
		case Moves::Bug_Buzz:
		case Moves::Dragon_Pulse:
		case Moves::Power_Gem:
		case Moves::Vacuum_Wave:
		case Moves::Focus_Blast:
		case Moves::Energy_Ball:
		case Moves::Earth_Power:
		case Moves::Mud_Bomb:
		case Moves::Mirror_Shot:
		case Moves::Flash_Cannon:
		case Moves::Draco_Meteor:
		case Moves::Discharge:
		case Moves::Lava_Plume:
		case Moves::Leaf_Storm:
		case Moves::Grass_Knot:
		case Moves::Chatter:
		case Moves::Judgment:
		case Moves::Charge_Beam:
		case Moves::Roar_of_Time:
		case Moves::Spacial_Rend:
		case Moves::Magma_Storm:
		case Moves::Seed_Flare:
		case Moves::Ominous_Wind:
		case Moves::Psyshock:
		case Moves::Venoshock:
		case Moves::Flame_Burst:
		case Moves::Sludge_Wave:
		case Moves::Synchronoise:
		case Moves::Electro_Ball:
		case Moves::Acid_Spray:
		case Moves::Round:
		case Moves::Echoed_Voice:
		case Moves::Clear_Smog:
		case Moves::Stored_Power:
		case Moves::Scald:
		case Moves::Hex:
		case Moves::Incinerate:
		case Moves::Final_Gambit:
		case Moves::Inferno:
		case Moves::Water_Pledge:
		case Moves::Fire_Pledge:
		case Moves::Grass_Pledge:
		case Moves::Volt_Switch:
		case Moves::Struggle_Bug:
		case Moves::Frost_Breath:
		case Moves::Electroweb:
		case Moves::Leaf_Tornado:
		case Moves::Night_Daze:
		case Moves::Psystrike:
		case Moves::Hurricane:
		case Moves::Searing_Shot:
		case Moves::Techno_Blast:
		case Moves::Relic_Song:
		case Moves::Secret_Sword:
		case Moves::Glaciate:
		case Moves::Blue_Flare:
		case Moves::Fiery_Dance:
		case Moves::Ice_Burn:
		case Moves::Snarl:
		case Moves::Fusion_Flare:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
