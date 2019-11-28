// Copyright (C) 2017 David Stone
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

#include <tm/move/moves.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <utility>

namespace technicalmachine {

using namespace bounded::literal;

struct PP {
	using pp_ups_type = bounded::checked_integer<0, 3>;

	constexpr PP(Generation const generation, Moves const move, pp_ups_type const pp_ups):
		m_max(calculate_max(base_pp(generation, move), pp_ups)),
		m_current(m_max ? bounded::optional<current_type>(*m_max) : bounded::none)
	{
	}

	constexpr auto is_empty() const {
		return m_current and *m_current == 0_bi;
	}

	constexpr auto restore(auto const value) & {
		BOUNDED_ASSERT(*m_current == 0_bi);
		*m_current = value;
	}

	constexpr auto has_unlimited_pp() const {
		return !m_max;
	}

	constexpr auto decrement(Ability const foe_ability) & {
		if (has_unlimited_pp()) {
			return;
		}
		// I think it is always an error to try to decrement a move without PP.
		BOUNDED_ASSERT(*m_current != 0_bi);
		*m_current -= BOUNDED_CONDITIONAL(uses_extra_pp(foe_ability), 2_bi, 1_bi);
	}

	constexpr auto trump_card_power() const -> bounded::integer<40, 200> {
		// Should be safe because we assume we are using Trump Card
		switch (m_current->value()) {
			case 0: return 200_bi;
			case 1: return 80_bi;
			case 2: return 60_bi;
			case 3: return 50_bi;
			default: return 40_bi;
		}
	}

	// Assumes max PP is the same because it assumes the same Move on the same
	// Pokemon
	friend constexpr auto operator==(PP const lhs, PP const rhs) {
		return lhs.m_current == rhs.m_current;
	}

private:
	using base_type = bounded::integer<1, 40>;
	
	// It is not allowed to use a static function with deduced return type to
	// define a typedef in the same class
	using max_type = decltype(std::declval<base_type>() * (std::declval<pp_ups_type>() + 5_bi) / 5_bi);

	static constexpr auto calculate_max(bounded::optional<base_type> const base, pp_ups_type const pp_ups) -> bounded::optional<max_type> {
		return base ? bounded::optional(*base * (pp_ups + 5_bi) / 5_bi) : bounded::none;
	}

	// clamped_integer simplifies situations like Pressure and Leppa
	using current_type = bounded::clamped_integer<0, static_cast<int>(bounded::max_value<max_type>)>;
	// TODO: Use optional<pair> instead of pair<optional>
	bounded::optional<max_type> m_max;
	bounded::optional<current_type> m_current;
	
	static constexpr auto base_pp(Generation const generation, Moves const move) -> bounded::optional<base_type> {
		using bounded::none;
		switch (move) {
			case Moves::Pass: return none;
			case Moves::Switch0: return none;
			case Moves::Switch1: return none;
			case Moves::Switch2: return none;
			case Moves::Switch3: return none;
			case Moves::Switch4: return none;
			case Moves::Switch5: return none;
			case Moves::Hit_Self: return none;
			case Moves::Pound: return 35_bi;
			case Moves::Karate_Chop: return 25_bi;
			case Moves::Double_Slap: return 10_bi;
			case Moves::Comet_Punch: return 15_bi;
			case Moves::Mega_Punch: return 20_bi;
			case Moves::Pay_Day: return 20_bi;
			case Moves::Fire_Punch: return 15_bi;
			case Moves::Ice_Punch: return 15_bi;
			case Moves::Thunder_Punch: return 15_bi;
			case Moves::Scratch: return 35_bi;
			case Moves::Vice_Grip: return 30_bi;
			case Moves::Guillotine: return 5_bi;
			case Moves::Razor_Wind: return 10_bi;
			case Moves::Swords_Dance: return BOUNDED_CONDITIONAL(generation <= Generation::five, 30_bi, 20_bi);
			case Moves::Cut: return 30_bi;
			case Moves::Gust: return 35_bi;
			case Moves::Wing_Attack: return 35_bi;
			case Moves::Whirlwind: return 20_bi;
			case Moves::Fly: return 15_bi;
			case Moves::Bind: return 20_bi;
			case Moves::Slam: return 20_bi;
			case Moves::Vine_Whip:
				switch (generation) {
					case Generation::one:
					case Generation::two:
					case Generation::three:
						return 10_bi;
					case Generation::four:
					case Generation::five:
						return 15_bi;
					case Generation::six:
					case Generation::seven:
						return 25_bi;
				}
			return BOUNDED_CONDITIONAL(generation <= Generation::three, 10_bi, 15_bi);
			case Moves::Stomp: return 20_bi;
			case Moves::Double_Kick: return 30_bi;
			case Moves::Mega_Kick: return 5_bi;
			case Moves::Jump_Kick: return BOUNDED_CONDITIONAL(generation <= Generation::four, 25_bi, 10_bi);
			case Moves::Rolling_Kick: return 15_bi;
			case Moves::Sand_Attack: return 15_bi;
			case Moves::Headbutt: return 15_bi;
			case Moves::Horn_Attack: return 25_bi;
			case Moves::Fury_Attack: return 20_bi;
			case Moves::Horn_Drill: return 5_bi;
			case Moves::Tackle: return 35_bi;
			case Moves::Body_Slam: return 15_bi;
			case Moves::Wrap: return 20_bi;
			case Moves::Take_Down: return 20_bi;
			case Moves::Thrash: return BOUNDED_CONDITIONAL(generation <= Generation::four, 20_bi, 10_bi);
			case Moves::Double_Edge: return 15_bi;
			case Moves::Tail_Whip: return 30_bi;
			case Moves::Poison_Sting: return 35_bi;
			case Moves::Twineedle: return 20_bi;
			case Moves::Pin_Missile: return 20_bi;
			case Moves::Leer: return 30_bi;
			case Moves::Bite: return 25_bi;
			case Moves::Growl: return 40_bi;
			case Moves::Roar: return 20_bi;
			case Moves::Sing: return 15_bi;
			case Moves::Supersonic: return 20_bi;
			case Moves::Sonic_Boom: return 20_bi;
			case Moves::Disable: return 20_bi;
			case Moves::Acid: return 30_bi;
			case Moves::Ember: return 25_bi;
			case Moves::Flamethrower: return 15_bi;
			case Moves::Mist: return 30_bi;
			case Moves::Water_Gun: return 25_bi;
			case Moves::Hydro_Pump: return 5_bi;
			case Moves::Surf: return 15_bi;
			case Moves::Ice_Beam: return 10_bi;
			case Moves::Blizzard: return 5_bi;
			case Moves::Psybeam: return 20_bi;
			case Moves::Bubble_Beam: return 20_bi;
			case Moves::Aurora_Beam: return 20_bi;
			case Moves::Hyper_Beam: return 5_bi;
			case Moves::Peck: return 35_bi;
			case Moves::Drill_Peck: return 20_bi;
			case Moves::Submission: return BOUNDED_CONDITIONAL(generation <= Generation::five, 25_bi, 20_bi);
			case Moves::Low_Kick: return 20_bi;
			case Moves::Counter: return 20_bi;
			case Moves::Seismic_Toss: return 20_bi;
			case Moves::Strength: return 15_bi;
			case Moves::Absorb: return BOUNDED_CONDITIONAL(generation <= Generation::three, 20_bi, 25_bi);
			case Moves::Mega_Drain: return BOUNDED_CONDITIONAL(generation <= Generation::three, 10_bi, 15_bi);
			case Moves::Leech_Seed: return 10_bi;
			case Moves::Growth: return BOUNDED_CONDITIONAL(generation <= Generation::five, 40_bi, 20_bi);
			case Moves::Razor_Leaf: return 25_bi;
			case Moves::Solar_Beam: return 10_bi;
			case Moves::Poison_Powder: return 35_bi;
			case Moves::Stun_Spore: return 30_bi;
			case Moves::Sleep_Powder: return 15_bi;
			case Moves::Petal_Dance: return BOUNDED_CONDITIONAL(generation <= Generation::four, 20_bi, 10_bi);
			case Moves::String_Shot: return 40_bi;
			case Moves::Dragon_Rage: return 10_bi;
			case Moves::Fire_Spin: return 15_bi;
			case Moves::Thunder_Shock: return 30_bi;
			case Moves::Thunderbolt: return 15_bi;
			case Moves::Thunder_Wave: return 20_bi;
			case Moves::Thunder: return 10_bi;
			case Moves::Rock_Throw: return 15_bi;
			case Moves::Earthquake: return 10_bi;
			case Moves::Fissure: return 5_bi;
			case Moves::Dig: return 10_bi;
			case Moves::Toxic: return 10_bi;
			case Moves::Confusion: return 25_bi;
			case Moves::Psychic: return 10_bi;
			case Moves::Hypnosis: return 20_bi;
			case Moves::Meditate: return 40_bi;
			case Moves::Agility: return 30_bi;
			case Moves::Quick_Attack: return 30_bi;
			case Moves::Rage: return 20_bi;
			case Moves::Teleport: return 20_bi;
			case Moves::Night_Shade: return 15_bi;
			case Moves::Mimic: return 10_bi;
			case Moves::Screech: return 40_bi;
			case Moves::Double_Team: return 15_bi;
			case Moves::Recover: return BOUNDED_CONDITIONAL(generation <= Generation::three, 20_bi, 10_bi);
			case Moves::Harden: return 30_bi;
			case Moves::Minimize: return BOUNDED_CONDITIONAL(generation <= Generation::five, 20_bi, 10_bi);
			case Moves::Smokescreen: return 20_bi;
			case Moves::Confuse_Ray: return 10_bi;
			case Moves::Withdraw: return 40_bi;
			case Moves::Defense_Curl: return 40_bi;
			case Moves::Barrier: return BOUNDED_CONDITIONAL(generation <= Generation::five, 30_bi, 20_bi);
			case Moves::Light_Screen: return 30_bi;
			case Moves::Haze: return 30_bi;
			case Moves::Reflect: return 20_bi;
			case Moves::Focus_Energy: return 30_bi;
			case Moves::Bide: return 10_bi;
			case Moves::Metronome: return 10_bi;
			case Moves::Mirror_Move: return 20_bi;
			case Moves::Self_Destruct: return 5_bi;
			case Moves::Egg_Bomb: return 10_bi;
			case Moves::Lick: return 30_bi;
			case Moves::Smog: return 20_bi;
			case Moves::Sludge: return 20_bi;
			case Moves::Bone_Club: return 20_bi;
			case Moves::Fire_Blast: return 5_bi;
			case Moves::Waterfall: return 15_bi;
			case Moves::Clamp: return BOUNDED_CONDITIONAL(generation <= Generation::four, 10_bi, 15_bi);
			case Moves::Swift: return 20_bi;
			case Moves::Skull_Bash: return BOUNDED_CONDITIONAL(generation <= Generation::five, 15_bi, 10_bi);
			case Moves::Spike_Cannon: return 15_bi;
			case Moves::Constrict: return 35_bi;
			case Moves::Amnesia: return 20_bi;
			case Moves::Kinesis: return 15_bi;
			case Moves::Soft_Boiled: return 10_bi;
			case Moves::High_Jump_Kick: return BOUNDED_CONDITIONAL(generation <= Generation::four, 20_bi, 10_bi);
			case Moves::Glare: return 30_bi;
			case Moves::Dream_Eater: return 15_bi;
			case Moves::Poison_Gas: return 40_bi;
			case Moves::Barrage: return 20_bi;
			case Moves::Leech_Life: return BOUNDED_CONDITIONAL(generation <= Generation::five, 15_bi, 10_bi);
			case Moves::Lovely_Kiss: return 10_bi;
			case Moves::Sky_Attack: return 5_bi;
			case Moves::Transform: return 10_bi;
			case Moves::Bubble: return 30_bi;
			case Moves::Dizzy_Punch: return 10_bi;
			case Moves::Spore: return 15_bi;
			case Moves::Flash: return 20_bi;
			case Moves::Psywave: return 15_bi;
			case Moves::Splash: return 40_bi;
			case Moves::Acid_Armor: return BOUNDED_CONDITIONAL(generation <= Generation::five, 40_bi, 20_bi);
			case Moves::Crabhammer: return 10_bi;
			case Moves::Explosion: return 5_bi;
			case Moves::Fury_Swipes: return 15_bi;
			case Moves::Bonemerang: return 10_bi;
			case Moves::Rest: return 10_bi;
			case Moves::Rock_Slide: return 10_bi;
			case Moves::Hyper_Fang: return 15_bi;
			case Moves::Sharpen: return 30_bi;
			case Moves::Conversion: return 30_bi;
			case Moves::Tri_Attack: return 10_bi;
			case Moves::Super_Fang: return 10_bi;
			case Moves::Slash: return 20_bi;
			case Moves::Substitute: return 10_bi;
			case Moves::Struggle: return none;
			case Moves::Sketch: return 1_bi;
			case Moves::Triple_Kick: return 10_bi;
			case Moves::Thief: return BOUNDED_CONDITIONAL(generation <= Generation::five, 10_bi, 25_bi);
			case Moves::Spider_Web: return 10_bi;
			case Moves::Mind_Reader: return 5_bi;
			case Moves::Nightmare: return 15_bi;
			case Moves::Flame_Wheel: return 25_bi;
			case Moves::Snore: return 15_bi;
			case Moves::Curse: return 10_bi;
			case Moves::Flail: return 15_bi;
			case Moves::Conversion_2: return 30_bi;
			case Moves::Aeroblast: return 5_bi;
			case Moves::Cotton_Spore: return 40_bi;
			case Moves::Reversal: return 15_bi;
			case Moves::Spite: return 10_bi;
			case Moves::Powder_Snow: return 25_bi;
			case Moves::Protect: return 10_bi;
			case Moves::Mach_Punch: return 30_bi;
			case Moves::Scary_Face: return 10_bi;
			case Moves::Feint_Attack: return 20_bi;
			case Moves::Sweet_Kiss: return 10_bi;
			case Moves::Belly_Drum: return 10_bi;
			case Moves::Sludge_Bomb: return 10_bi;
			case Moves::Mud_Slap: return 10_bi;
			case Moves::Octazooka: return 10_bi;
			case Moves::Spikes: return 20_bi;
			case Moves::Zap_Cannon: return 5_bi;
			case Moves::Foresight: return 40_bi;
			case Moves::Destiny_Bond: return 5_bi;
			case Moves::Perish_Song: return 5_bi;
			case Moves::Icy_Wind: return 15_bi;
			case Moves::Detect: return 5_bi;
			case Moves::Bone_Rush: return 10_bi;
			case Moves::Lock_On: return 5_bi;
			case Moves::Outrage: return BOUNDED_CONDITIONAL(generation <= Generation::four, 15_bi, 10_bi);
			case Moves::Sandstorm: return 10_bi;
			case Moves::Giga_Drain: return BOUNDED_CONDITIONAL(generation <= Generation::three, 5_bi, 10_bi);
			case Moves::Endure: return 10_bi;
			case Moves::Charm: return 20_bi;
			case Moves::Rollout: return 20_bi;
			case Moves::False_Swipe: return 40_bi;
			case Moves::Swagger: return 15_bi;
			case Moves::Milk_Drink: return 10_bi;
			case Moves::Spark: return 20_bi;
			case Moves::Fury_Cutter: return 20_bi;
			case Moves::Steel_Wing: return 25_bi;
			case Moves::Mean_Look: return 5_bi;
			case Moves::Attract: return 15_bi;
			case Moves::Sleep_Talk: return 10_bi;
			case Moves::Heal_Bell: return 5_bi;
			case Moves::Return: return 20_bi;
			case Moves::Present: return 15_bi;
			case Moves::Frustration: return 20_bi;
			case Moves::Safeguard: return 25_bi;
			case Moves::Pain_Split: return 20_bi;
			case Moves::Sacred_Fire: return 5_bi;
			case Moves::Magnitude: return 30_bi;
			case Moves::Dynamic_Punch: return 5_bi;
			case Moves::Megahorn: return 10_bi;
			case Moves::Dragon_Breath: return 20_bi;
			case Moves::Baton_Pass: return 40_bi;
			case Moves::Encore: return 5_bi;
			case Moves::Pursuit: return 20_bi;
			case Moves::Rapid_Spin: return 40_bi;
			case Moves::Sweet_Scent: return 20_bi;
			case Moves::Iron_Tail: return 15_bi;
			case Moves::Metal_Claw: return 35_bi;
			case Moves::Vital_Throw: return 10_bi;
			case Moves::Morning_Sun: return 5_bi;
			case Moves::Synthesis: return 5_bi;
			case Moves::Moonlight: return 5_bi;
			case Moves::Hidden_Power: return 15_bi;
			case Moves::Cross_Chop: return 5_bi;
			case Moves::Twister: return 20_bi;
			case Moves::Rain_Dance: return 5_bi;
			case Moves::Sunny_Day: return 5_bi;
			case Moves::Crunch: return 15_bi;
			case Moves::Mirror_Coat: return 20_bi;
			case Moves::Psych_Up: return 10_bi;
			case Moves::Extreme_Speed: return 5_bi;
			case Moves::Ancient_Power: return 5_bi;
			case Moves::Shadow_Ball: return 15_bi;
			case Moves::Future_Sight: return BOUNDED_CONDITIONAL(generation <= Generation::four, 15_bi, 10_bi);
			case Moves::Rock_Smash: return 15_bi;
			case Moves::Whirlpool: return 15_bi;
			case Moves::Beat_Up: return 10_bi;
			case Moves::Fake_Out: return 10_bi;
			case Moves::Uproar: return 10_bi;
			case Moves::Stockpile: return BOUNDED_CONDITIONAL(generation <= Generation::three, 20_bi, 10_bi);
			case Moves::Spit_Up: return 10_bi;
			case Moves::Swallow: return 10_bi;
			case Moves::Heat_Wave: return 10_bi;
			case Moves::Hail: return 10_bi;
			case Moves::Torment: return 15_bi;
			case Moves::Flatter: return 15_bi;
			case Moves::Will_O_Wisp: return 15_bi;
			case Moves::Memento: return 10_bi;
			case Moves::Facade: return 20_bi;
			case Moves::Focus_Punch: return 20_bi;
			case Moves::Smelling_Salts: return 10_bi;
			case Moves::Follow_Me: return 20_bi;
			case Moves::Nature_Power: return 20_bi;
			case Moves::Charge: return 20_bi;
			case Moves::Taunt: return 20_bi;
			case Moves::Helping_Hand: return 20_bi;
			case Moves::Trick: return 10_bi;
			case Moves::Role_Play: return 10_bi;
			case Moves::Wish: return 10_bi;
			case Moves::Assist: return 20_bi;
			case Moves::Ingrain: return 20_bi;
			case Moves::Superpower: return 5_bi;
			case Moves::Magic_Coat: return 15_bi;
			case Moves::Recycle: return 10_bi;
			case Moves::Revenge: return 10_bi;
			case Moves::Brick_Break: return 15_bi;
			case Moves::Yawn: return 10_bi;
			case Moves::Knock_Off: return 20_bi;
			case Moves::Endeavor: return 5_bi;
			case Moves::Eruption: return 5_bi;
			case Moves::Skill_Swap: return 10_bi;
			case Moves::Imprison: return 10_bi;
			case Moves::Refresh: return 20_bi;
			case Moves::Grudge: return 5_bi;
			case Moves::Snatch: return 10_bi;
			case Moves::Secret_Power: return 20_bi;
			case Moves::Dive: return 10_bi;
			case Moves::Arm_Thrust: return 20_bi;
			case Moves::Camouflage: return 20_bi;
			case Moves::Tail_Glow: return 20_bi;
			case Moves::Luster_Purge: return 5_bi;
			case Moves::Mist_Ball: return 5_bi;
			case Moves::Feather_Dance: return 15_bi;
			case Moves::Teeter_Dance: return 20_bi;
			case Moves::Blaze_Kick: return 10_bi;
			case Moves::Mud_Sport: return 15_bi;
			case Moves::Ice_Ball: return 20_bi;
			case Moves::Needle_Arm: return 15_bi;
			case Moves::Slack_Off: return 10_bi;
			case Moves::Hyper_Voice: return 10_bi;
			case Moves::Poison_Fang: return 15_bi;
			case Moves::Crush_Claw: return 10_bi;
			case Moves::Blast_Burn: return 5_bi;
			case Moves::Hydro_Cannon: return 5_bi;
			case Moves::Meteor_Mash: return 10_bi;
			case Moves::Astonish: return 15_bi;
			case Moves::Weather_Ball: return 10_bi;
			case Moves::Aromatherapy: return 5_bi;
			case Moves::Fake_Tears: return 20_bi;
			case Moves::Air_Cutter: return 25_bi;
			case Moves::Overheat: return 5_bi;
			case Moves::Odor_Sleuth: return 40_bi;
			case Moves::Rock_Tomb: return BOUNDED_CONDITIONAL(generation <= Generation::five, 10_bi, 15_bi);
			case Moves::Silver_Wind: return 5_bi;
			case Moves::Metal_Sound: return 40_bi;
			case Moves::Grass_Whistle: return 15_bi;
			case Moves::Tickle: return 20_bi;
			case Moves::Cosmic_Power: return 20_bi;
			case Moves::Water_Spout: return 5_bi;
			case Moves::Signal_Beam: return 15_bi;
			case Moves::Shadow_Punch: return 20_bi;
			case Moves::Extrasensory: return BOUNDED_CONDITIONAL(generation <= Generation::five, 30_bi, 20_bi);
			case Moves::Sky_Uppercut: return 15_bi;
			case Moves::Sand_Tomb: return 15_bi;
			case Moves::Sheer_Cold: return 5_bi;
			case Moves::Muddy_Water: return 10_bi;
			case Moves::Bullet_Seed: return 30_bi;
			case Moves::Aerial_Ace: return 20_bi;
			case Moves::Icicle_Spear: return 30_bi;
			case Moves::Iron_Defense: return 15_bi;
			case Moves::Block: return 5_bi;
			case Moves::Howl: return 40_bi;
			case Moves::Dragon_Claw: return 15_bi;
			case Moves::Frenzy_Plant: return 5_bi;
			case Moves::Bulk_Up: return 20_bi;
			case Moves::Bounce: return 5_bi;
			case Moves::Mud_Shot: return 15_bi;
			case Moves::Poison_Tail: return 25_bi;
			case Moves::Covet: return BOUNDED_CONDITIONAL(generation <= Generation::five, 40_bi, 25_bi);
			case Moves::Volt_Tackle: return 15_bi;
			case Moves::Magical_Leaf: return 20_bi;
			case Moves::Water_Sport: return 15_bi;
			case Moves::Calm_Mind: return 20_bi;
			case Moves::Leaf_Blade: return 15_bi;
			case Moves::Dragon_Dance: return 20_bi;
			case Moves::Rock_Blast: return 10_bi;
			case Moves::Shock_Wave: return 20_bi;
			case Moves::Water_Pulse: return 20_bi;
			case Moves::Doom_Desire: return 5_bi;
			case Moves::Psycho_Boost: return 5_bi;
			case Moves::Roost: return 10_bi;
			case Moves::Gravity: return 5_bi;
			case Moves::Miracle_Eye: return 40_bi;
			case Moves::Wake_Up_Slap: return 10_bi;
			case Moves::Hammer_Arm: return 10_bi;
			case Moves::Gyro_Ball: return 5_bi;
			case Moves::Healing_Wish: return 10_bi;
			case Moves::Brine: return 10_bi;
			case Moves::Natural_Gift: return 15_bi;
			case Moves::Feint: return 10_bi;
			case Moves::Pluck: return 20_bi;
			case Moves::Tailwind: return BOUNDED_CONDITIONAL(generation <= Generation::five, 30_bi, 15_bi);
			case Moves::Acupressure: return 30_bi;
			case Moves::Metal_Burst: return 10_bi;
			case Moves::U_turn: return 20_bi;
			case Moves::Close_Combat: return 5_bi;
			case Moves::Payback: return 10_bi;
			case Moves::Assurance: return 10_bi;
			case Moves::Embargo: return 15_bi;
			case Moves::Fling: return 10_bi;
			case Moves::Psycho_Shift: return 10_bi;
			case Moves::Trump_Card: return 5_bi;
			case Moves::Heal_Block: return 15_bi;
			case Moves::Wring_Out: return 5_bi;
			case Moves::Power_Trick: return 10_bi;
			case Moves::Gastro_Acid: return 10_bi;
			case Moves::Lucky_Chant: return 30_bi;
			case Moves::Me_First: return 20_bi;
			case Moves::Copycat: return 20_bi;
			case Moves::Power_Swap: return 10_bi;
			case Moves::Guard_Swap: return 10_bi;
			case Moves::Punishment: return 5_bi;
			case Moves::Last_Resort: return 5_bi;
			case Moves::Worry_Seed: return 10_bi;
			case Moves::Sucker_Punch: return 5_bi;
			case Moves::Toxic_Spikes: return 20_bi;
			case Moves::Heart_Swap: return 10_bi;
			case Moves::Aqua_Ring: return 20_bi;
			case Moves::Magnet_Rise: return 10_bi;
			case Moves::Flare_Blitz: return 15_bi;
			case Moves::Force_Palm: return 10_bi;
			case Moves::Aura_Sphere: return 20_bi;
			case Moves::Rock_Polish: return 20_bi;
			case Moves::Poison_Jab: return 20_bi;
			case Moves::Dark_Pulse: return 15_bi;
			case Moves::Night_Slash: return 15_bi;
			case Moves::Aqua_Tail: return 10_bi;
			case Moves::Seed_Bomb: return 15_bi;
			case Moves::Air_Slash: return BOUNDED_CONDITIONAL(generation <= Generation::five, 20_bi, 15_bi);
			case Moves::X_Scissor: return 15_bi;
			case Moves::Bug_Buzz: return 10_bi;
			case Moves::Dragon_Pulse: return 10_bi;
			case Moves::Dragon_Rush: return 10_bi;
			case Moves::Power_Gem: return 20_bi;
			case Moves::Drain_Punch: return BOUNDED_CONDITIONAL(generation <= Generation::four, 5_bi, 10_bi);
			case Moves::Vacuum_Wave: return 30_bi;
			case Moves::Focus_Blast: return 5_bi;
			case Moves::Energy_Ball: return 10_bi;
			case Moves::Brave_Bird: return 15_bi;
			case Moves::Earth_Power: return 10_bi;
			case Moves::Switcheroo: return 10_bi;
			case Moves::Giga_Impact: return 5_bi;
			case Moves::Nasty_Plot: return 20_bi;
			case Moves::Bullet_Punch: return 30_bi;
			case Moves::Avalanche: return 10_bi;
			case Moves::Ice_Shard: return 30_bi;
			case Moves::Shadow_Claw: return 15_bi;
			case Moves::Thunder_Fang: return 15_bi;
			case Moves::Ice_Fang: return 15_bi;
			case Moves::Fire_Fang: return 15_bi;
			case Moves::Shadow_Sneak: return 30_bi;
			case Moves::Mud_Bomb: return 10_bi;
			case Moves::Psycho_Cut: return 20_bi;
			case Moves::Zen_Headbutt: return 15_bi;
			case Moves::Mirror_Shot: return 10_bi;
			case Moves::Flash_Cannon: return 10_bi;
			case Moves::Rock_Climb: return 20_bi;
			case Moves::Defog: return 15_bi;
			case Moves::Trick_Room: return 5_bi;
			case Moves::Draco_Meteor: return 5_bi;
			case Moves::Discharge: return 15_bi;
			case Moves::Lava_Plume: return 15_bi;
			case Moves::Leaf_Storm: return 5_bi;
			case Moves::Power_Whip: return 10_bi;
			case Moves::Rock_Wrecker: return 5_bi;
			case Moves::Cross_Poison: return 20_bi;
			case Moves::Gunk_Shot: return 5_bi;
			case Moves::Iron_Head: return 15_bi;
			case Moves::Magnet_Bomb: return 20_bi;
			case Moves::Stone_Edge: return 5_bi;
			case Moves::Captivate: return 20_bi;
			case Moves::Stealth_Rock: return 20_bi;
			case Moves::Grass_Knot: return 20_bi;
			case Moves::Chatter: return 20_bi;
			case Moves::Judgment: return 10_bi;
			case Moves::Bug_Bite: return 20_bi;
			case Moves::Charge_Beam: return 10_bi;
			case Moves::Wood_Hammer: return 15_bi;
			case Moves::Aqua_Jet: return 20_bi;
			case Moves::Attack_Order: return 15_bi;
			case Moves::Defend_Order: return 10_bi;
			case Moves::Heal_Order: return 10_bi;
			case Moves::Head_Smash: return 5_bi;
			case Moves::Double_Hit: return 10_bi;
			case Moves::Roar_of_Time: return 5_bi;
			case Moves::Spacial_Rend: return 5_bi;
			case Moves::Lunar_Dance: return 10_bi;
			case Moves::Crush_Grip: return 5_bi;
			case Moves::Magma_Storm: return 5_bi;
			case Moves::Dark_Void: return 10_bi;
			case Moves::Seed_Flare: return 5_bi;
			case Moves::Ominous_Wind: return 5_bi;
			case Moves::Shadow_Force: return 5_bi;
			case Moves::Hone_Claws: return 15_bi;
			case Moves::Wide_Guard: return 10_bi;
			case Moves::Guard_Split: return 10_bi;
			case Moves::Power_Split: return 10_bi;
			case Moves::Wonder_Room: return 10_bi;
			case Moves::Psyshock: return 10_bi;
			case Moves::Venoshock: return 10_bi;
			case Moves::Autotomize: return 15_bi;
			case Moves::Rage_Powder: return 20_bi;
			case Moves::Telekinesis: return 15_bi;
			case Moves::Magic_Room: return 10_bi;
			case Moves::Smack_Down: return 15_bi;
			case Moves::Storm_Throw: return 10_bi;
			case Moves::Flame_Burst: return 15_bi;
			case Moves::Sludge_Wave: return 10_bi;
			case Moves::Quiver_Dance: return 20_bi;
			case Moves::Heavy_Slam: return 10_bi;
			case Moves::Synchronoise: return BOUNDED_CONDITIONAL(generation <= Generation::five, 15_bi, 10_bi);
			case Moves::Electro_Ball: return 10_bi;
			case Moves::Soak: return 20_bi;
			case Moves::Flame_Charge: return 20_bi;
			case Moves::Coil: return 20_bi;
			case Moves::Low_Sweep: return 20_bi;
			case Moves::Acid_Spray: return 20_bi;
			case Moves::Foul_Play: return 15_bi;
			case Moves::Simple_Beam: return 15_bi;
			case Moves::Entrainment: return 15_bi;
			case Moves::After_You: return 15_bi;
			case Moves::Round: return 15_bi;
			case Moves::Echoed_Voice: return 15_bi;
			case Moves::Chip_Away: return 20_bi;
			case Moves::Clear_Smog: return 15_bi;
			case Moves::Stored_Power: return 10_bi;
			case Moves::Quick_Guard: return 15_bi;
			case Moves::Ally_Switch: return 15_bi;
			case Moves::Scald: return 15_bi;
			case Moves::Shell_Smash: return 15_bi;
			case Moves::Heal_Pulse: return 10_bi;
			case Moves::Hex: return 10_bi;
			case Moves::Sky_Drop: return 10_bi;
			case Moves::Shift_Gear: return 10_bi;
			case Moves::Circle_Throw: return 10_bi;
			case Moves::Incinerate: return 15_bi;
			case Moves::Quash: return 15_bi;
			case Moves::Acrobatics: return 15_bi;
			case Moves::Reflect_Type: return 15_bi;
			case Moves::Retaliate: return 5_bi;
			case Moves::Final_Gambit: return 5_bi;
			case Moves::Bestow: return 15_bi;
			case Moves::Inferno: return 5_bi;
			case Moves::Water_Pledge: return 10_bi;
			case Moves::Fire_Pledge: return 10_bi;
			case Moves::Grass_Pledge: return 10_bi;
			case Moves::Volt_Switch: return 20_bi;
			case Moves::Struggle_Bug: return 20_bi;
			case Moves::Bulldoze: return 20_bi;
			case Moves::Frost_Breath: return 10_bi;
			case Moves::Dragon_Tail: return 10_bi;
			case Moves::Work_Up: return 30_bi;
			case Moves::Electroweb: return 15_bi;
			case Moves::Wild_Charge: return 15_bi;
			case Moves::Drill_Run: return 10_bi;
			case Moves::Dual_Chop: return 15_bi;
			case Moves::Heart_Stamp: return 25_bi;
			case Moves::Horn_Leech: return 10_bi;
			case Moves::Sacred_Sword: return BOUNDED_CONDITIONAL(generation <= Generation::five, 20_bi, 15_bi);
			case Moves::Razor_Shell: return 10_bi;
			case Moves::Heat_Crash: return 10_bi;
			case Moves::Leaf_Tornado: return 10_bi;
			case Moves::Steamroller: return 20_bi;
			case Moves::Cotton_Guard: return 10_bi;
			case Moves::Night_Daze: return 10_bi;
			case Moves::Psystrike: return 10_bi;
			case Moves::Tail_Slap: return 10_bi;
			case Moves::Hurricane: return 10_bi;
			case Moves::Head_Charge: return 15_bi;
			case Moves::Gear_Grind: return 15_bi;
			case Moves::Searing_Shot: return 5_bi;
			case Moves::Techno_Blast: return 5_bi;
			case Moves::Relic_Song: return 10_bi;
			case Moves::Secret_Sword: return 10_bi;
			case Moves::Glaciate: return 10_bi;
			case Moves::Bolt_Strike: return 5_bi;
			case Moves::Blue_Flare: return 5_bi;
			case Moves::Fiery_Dance: return 10_bi;
			case Moves::Freeze_Shock: return 5_bi;
			case Moves::Ice_Burn: return 5_bi;
			case Moves::Snarl: return 15_bi;
			case Moves::Icicle_Crash: return 10_bi;
			case Moves::V_create: return 5_bi;
			case Moves::Fusion_Flare: return 5_bi;
			case Moves::Fusion_Bolt: return 5_bi;
			case Moves::Flying_Press: return 10_bi;
			case Moves::Mat_Block: return 10_bi;
			case Moves::Belch: return 10_bi;
			case Moves::Rototiller: return 10_bi;
			case Moves::Sticky_Web: return 20_bi;
			case Moves::Fell_Stinger: return 25_bi;
			case Moves::Phantom_Force: return 10_bi;
			case Moves::Trick_or_Treat: return 20_bi;
			case Moves::Noble_Roar: return 30_bi;
			case Moves::Ion_Deluge: return 25_bi;
			case Moves::Parabolic_Charge: return 20_bi;
			case Moves::Forests_Curse: return 20_bi;
			case Moves::Petal_Blizzard: return 15_bi;
			case Moves::Freeze_Dry: return 20_bi;
			case Moves::Disarming_Voice: return 15_bi;
			case Moves::Parting_Shot: return 20_bi;
			case Moves::Topsy_Turvy: return 20_bi;
			case Moves::Draining_Kiss: return 10_bi;
			case Moves::Crafty_Shield: return 10_bi;
			case Moves::Flower_Shield: return 10_bi;
			case Moves::Grassy_Terrain: return 10_bi;
			case Moves::Misty_Terrain: return 10_bi;
			case Moves::Electrify: return 20_bi;
			case Moves::Play_Rough: return 10_bi;
			case Moves::Fairy_Wind: return 30_bi;
			case Moves::Moonblast: return 15_bi;
			case Moves::Boomburst: return 10_bi;
			case Moves::Fairy_Lock: return 10_bi;
			case Moves::Kings_Shield: return 10_bi;
			case Moves::Play_Nice: return 20_bi;
			case Moves::Confide: return 20_bi;
			case Moves::Diamond_Storm: return 5_bi;
			case Moves::Steam_Eruption: return 5_bi;
			case Moves::Hyperspace_Hole: return 5_bi;
			case Moves::Water_Shuriken: return 20_bi;
			case Moves::Mystical_Fire: return 10_bi;
			case Moves::Spiky_Shield: return 10_bi;
			case Moves::Aromatic_Mist: return 20_bi;
			case Moves::Eerie_Impulse: return 15_bi;
			case Moves::Venom_Drench: return 20_bi;
			case Moves::Powder: return 20_bi;
			case Moves::Geomancy: return 10_bi;
			case Moves::Magnetic_Flux: return 20_bi;
			case Moves::Happy_Hour: return 30_bi;
			case Moves::Electric_Terrain: return 10_bi;
			case Moves::Dazzling_Gleam: return 10_bi;
			case Moves::Celebrate: return 40_bi;
			case Moves::Hold_Hands: return 40_bi;
			case Moves::Baby_Doll_Eyes: return 30_bi;
			case Moves::Nuzzle: return 20_bi;
			case Moves::Hold_Back: return 40_bi;
			case Moves::Infestation: return 20_bi;
			case Moves::Power_Up_Punch: return 20_bi;
			case Moves::Oblivion_Wing: return 10_bi;
			case Moves::Thousand_Arrows: return 10_bi;
			case Moves::Thousand_Waves: return 10_bi;
			case Moves::Lands_Wrath: return 10_bi;
			case Moves::Light_of_Ruin: return 5_bi;
			case Moves::Origin_Pulse: return 10_bi;
			case Moves::Precipice_Blades: return 10_bi;
			case Moves::Dragon_Ascent: return 5_bi;
			case Moves::Hyperspace_Fury: return 5_bi;
			case Moves::Breakneck_Blitz: return 1_bi;
			case Moves::All_Out_Pummeling: return 1_bi;
			case Moves::Supersonic_Skystrike: return 1_bi;
			case Moves::Acid_Downpour: return 1_bi;
			case Moves::Tectonic_Rage: return 1_bi;
			case Moves::Continental_Crush: return 1_bi;
			case Moves::Savage_Spin_Out: return 1_bi;
			case Moves::Never_Ending_Nightmare: return 1_bi;
			case Moves::Corkscrew_Crash: return 1_bi;
			case Moves::Inferno_Overdrive: return 1_bi;
			case Moves::Hydro_Vortex: return 1_bi;
			case Moves::Bloom_Doom: return 1_bi;
			case Moves::Gigavolt_Havoc: return 1_bi;
			case Moves::Shattered_Psyche: return 1_bi;
			case Moves::Subzero_Slammer: return 1_bi;
			case Moves::Devastating_Drake: return 1_bi;
			case Moves::Black_Hole_Eclipse: return 1_bi;
			case Moves::Twinkle_Tackle: return 1_bi;
			case Moves::Catastropika: return 1_bi;
			case Moves::Shore_Up: return 10_bi;
			case Moves::First_Impression: return 10_bi;
			case Moves::Baneful_Bunker: return 10_bi;
			case Moves::Spirit_Shackle: return 10_bi;
			case Moves::Darkest_Lariat: return 10_bi;
			case Moves::Sparkling_Aria: return 10_bi;
			case Moves::Ice_Hammer: return 10_bi;
			case Moves::Floral_Healing: return 10_bi;
			case Moves::High_Horsepower: return 10_bi;
			case Moves::Strength_Sap: return 10_bi;
			case Moves::Solar_Blade: return 10_bi;
			case Moves::Leafage: return 40_bi;
			case Moves::Spotlight: return 15_bi;
			case Moves::Toxic_Thread: return 20_bi;
			case Moves::Laser_Focus: return 30_bi;
			case Moves::Gear_Up: return 20_bi;
			case Moves::Throat_Chop: return 15_bi;
			case Moves::Pollen_Puff: return 15_bi;
			case Moves::Anchor_Shot: return 20_bi;
			case Moves::Psychic_Terrain: return 10_bi;
			case Moves::Lunge: return 15_bi;
			case Moves::Fire_Lash: return 15_bi;
			case Moves::Power_Trip: return 10_bi;
			case Moves::Burn_Up: return 5_bi;
			case Moves::Speed_Swap: return 10_bi;
			case Moves::Smart_Strike: return 10_bi;
			case Moves::Purify: return 20_bi;
			case Moves::Revelation_Dance: return 15_bi;
			case Moves::Core_Enforcer: return 10_bi;
			case Moves::Trop_Kick: return 15_bi;
			case Moves::Instruct: return 15_bi;
			case Moves::Beak_Blast: return 15_bi;
			case Moves::Clanging_Scales: return 5_bi;
			case Moves::Dragon_Hammer: return 15_bi;
			case Moves::Brutal_Swing: return 20_bi;
			case Moves::Aurora_Veil: return 20_bi;
			case Moves::Sinister_Arrow_Raid: return 1_bi;
			case Moves::Malicious_Moonsault: return 1_bi;
			case Moves::Oceanic_Operetta: return 1_bi;
			case Moves::Guardian_of_Alola: return 1_bi;
			case Moves::Soul_Stealing_7_Star_Strike: return 1_bi;
			case Moves::Stoked_Sparksurfer: return 1_bi;
			case Moves::Pulverizing_Pancake: return 1_bi;
			case Moves::Extreme_Evoboost: return 1_bi;
			case Moves::Genesis_Supernova: return 1_bi;
			case Moves::Shell_Trap: return 5_bi;
			case Moves::Fleur_Cannon: return 5_bi;
			case Moves::Psychic_Fangs: return 10_bi;
			case Moves::Stomping_Tantrum: return 10_bi;
			case Moves::Shadow_Bone: return 10_bi;
			case Moves::Accelerock: return 20_bi;
			case Moves::Liquidation: return 10_bi;
			case Moves::Prismatic_Laser: return 10_bi;
			case Moves::Spectral_Thief: return 10_bi;
			case Moves::Sunsteel_Strike: return 5_bi;
			case Moves::Moongeist_Beam: return 5_bi;
			case Moves::Tearful_Look: return 20_bi;
			case Moves::Zing_Zap: return 10_bi;
			case Moves::Natures_Madness: return 10_bi;
			case Moves::Multi_Attack: return 10_bi;
			case Moves::m10000000_Volt_Thunderbolt: return 1_bi;
			case Moves::Mind_Blown: return 5_bi;
			case Moves::Plasma_Fists: return 15_bi;
			case Moves::Photon_Geyser: return 5_bi;
			case Moves::Light_That_Burns_the_Sky: return 1_bi;
			case Moves::Searing_Sunraze_Smash: return 1_bi;
			case Moves::Menacing_Moonraze_Maelstrom: return 1_bi;
			case Moves::Lets_Snuggle_Forever: return 1_bi;
			case Moves::Splintered_Stormshards: return 1_bi;
			case Moves::Clangorous_Soulblaze: return 1_bi;
			case Moves::Zippy_Zap: return 15_bi;
			case Moves::Splishy_Splash: return 15_bi;
			case Moves::Floaty_Fall: return 15_bi;
			case Moves::Pika_Papow: return 20_bi;
			case Moves::Bouncy_Bubble: return 15_bi;
			case Moves::Buzzy_Buzz: return 15_bi;
			case Moves::Sizzly_Slide: return 15_bi;
			case Moves::Glitzy_Glow: return 15_bi;
			case Moves::Baddy_Bad: return 15_bi;
			case Moves::Sappy_Seed: return 15_bi;
			case Moves::Freezy_Frost: return 15_bi;
			case Moves::Sparkly_Swirl: return 15_bi;
			case Moves::Veevee_Volley: return 20_bi;
			case Moves::Double_Iron_Bash: return 5_bi;
		}
	}
};

}	// namespace technicalmachine
