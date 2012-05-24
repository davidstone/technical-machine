// Moves classification (physical, special, or other)
// Copyright (C) 2012 David Stone
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

#include "classification.hpp"

#include <cstdint>

namespace technicalmachine {

enum class Classification::Cached : int8_t { special = -1, neither = 0, physical = 1 };

Classification::Classification(Moves const move):
	cached(classification(move)) {
}

bool Classification::is_physical() const {
	return cached == Cached::physical;
}

bool Classification::is_special() const {
	return cached == Cached::special;
}

Classification::Cached Classification::classification (Moves const move) {
	static constexpr Cached lookup [] = {
		Cached::special,	// Absorb 
		Cached::special,	// Acid 
		Cached::neither,	// Acid Armor 
		Cached::neither,	// Acupressure 
		Cached::physical,	// Aerial Ace 
		Cached::special,	// Aeroblast 
		Cached::neither,	// Agility 
		Cached::special,	// Air Cutter 
		Cached::special,	// Air Slash 
		Cached::neither,	// Amnesia 
		Cached::special,	// AncientPower 
		Cached::physical,	// Aqua Jet 
		Cached::neither,	// Aqua Ring 
		Cached::physical,	// Aqua Tail 
		Cached::physical,	// Arm Thrust 
		Cached::neither,	// Aromatherapy 
		Cached::neither,	// Assist 
		Cached::physical,	// Assurance 
		Cached::physical,	// Astonish 
		Cached::physical,	// Attack Order 
		Cached::neither,	// Attract 
		Cached::special,	// Aura Sphere 
		Cached::special,	// Aurora Beam 
		Cached::physical,	// Avalanche 
		Cached::physical,	// Barrage 
		Cached::neither,	// Barrier 
		Cached::neither,	// Baton Pass 
		Cached::physical,	// Beat Up 
		Cached::neither,	// Belly Drum 
		Cached::physical,	// Bide 
		Cached::physical,	// Bind 
		Cached::physical,	// Bite 
		Cached::special,	// Blast Burn 
		Cached::physical,	// Blaze Kick 
		Cached::special,	// Blizzard 
		Cached::neither,	// Block 
		Cached::physical,	// Body Slam 
		Cached::physical,	// Bone Club 
		Cached::physical,	// Bonemerang 
		Cached::physical,	// Bone Rush 
		Cached::physical,	// Bounce 
		Cached::physical,	// Brave Bird 
		Cached::physical,	// Brick Break 
		Cached::special,	// Brine 
		Cached::special,	// Bubble 
		Cached::special,	// BubbleBeam 
		Cached::physical,	// Bug Bite 
		Cached::special,	// Bug Buzz 
		Cached::neither,	// Bulk Up 
		Cached::physical,	// Bullet Punch 
		Cached::physical,	// Bullet Seed 
		Cached::neither,	// Calm Mind 
		Cached::neither,	// Camouflage 
		Cached::neither,	// Captivate 
		Cached::neither,	// Charge 
		Cached::special,	// Charge Beam 
		Cached::neither,	// Charm 
		Cached::special,	// Chatter 
		Cached::physical,	// Clamp 
		Cached::physical,	// Close Combat 
		Cached::physical,	// Comet Punch 
		Cached::neither,	// Confuse Ray 
		Cached::special,	// Confusion 
		Cached::physical,	// Constrict 
		Cached::neither,	// Conversion 
		Cached::neither,	// Conversion 2 
		Cached::neither,	// Copycat 
		Cached::neither,	// Cosmic Power 
		Cached::neither,	// Cotton Spore 
		Cached::physical,	// Counter 
		Cached::physical,	// Covet 
		Cached::physical,	// Crabhammer 
		Cached::physical,	// Cross Chop 
		Cached::physical,	// Cross Poison 
		Cached::physical,	// Crunch 
		Cached::physical,	// Crush Claw 
		Cached::physical,	// Crush Grip 
		Cached::neither,	// Curse 
		Cached::physical,	// Cut 
		Cached::special,	// Dark Pulse 
		Cached::neither,	// Dark Void 
		Cached::neither,	// Defend Order 
		Cached::neither,	// Defense Curl 
		Cached::neither,	// Defog 
		Cached::neither,	// Destiny Bond 
		Cached::neither,	// Detect 
		Cached::physical,	// Dig 
		Cached::neither,	// Disable 
		Cached::special,	// Discharge 
		Cached::physical,	// Dive 
		Cached::physical,	// Dizzy Punch 
		Cached::special,	// Doom Desire 
		Cached::physical,	// Double-Edge 
		Cached::physical,	// Double Hit 
		Cached::physical,	// Double Kick 
		Cached::physical,	// DoubleSlap 
		Cached::neither,	// Double Team 
		Cached::special,	// Draco Meteor 
		Cached::special,	// DragonBreath 
		Cached::physical,	// Dragon Claw 
		Cached::neither,	// Dragon Dance 
		Cached::special,	// Dragon Pulse 
		Cached::special,	// Dragon Rage 
		Cached::physical,	// Dragon Rush 
		Cached::physical,	// Drain Punch 
		Cached::special,	// Dream Eater 
		Cached::physical,	// Drill Peck 
		Cached::physical,	// DynamicPunch 
		Cached::special,	// Earth Power 
		Cached::physical,	// Earthquake 
		Cached::physical,	// Egg Bomb 
		Cached::neither,	// Embargo 
		Cached::special,	// Ember 
		Cached::neither,	// Encore 
		Cached::physical,	// Endeavor 
		Cached::neither,	// Endure 
		Cached::special,	// Energy Ball 
		Cached::special,	// Eruption 
		Cached::physical,	// Explosion 
		Cached::special,	// Extrasensory 
		Cached::physical,	// ExtremeSpeed 
		Cached::physical,	// Facade 
		Cached::physical,	// Faint Attack 
		Cached::physical,	// Fake Out 
		Cached::neither,	// Fake Tears 
		Cached::physical,	// False Swipe 
		Cached::neither,	// FeatherDance 
		Cached::physical,	// Feint 
		Cached::special,	// Fire Blast 
		Cached::physical,	// Fire Fang 
		Cached::physical,	// Fire Punch 
		Cached::special,	// Fire Spin 
		Cached::physical,	// Fissure 
		Cached::physical,	// Flail 
		Cached::special,	// Flamethrower 
		Cached::physical,	// Flame Wheel 
		Cached::physical,	// Flare Blitz 
		Cached::neither,	// Flash 
		Cached::special,	// Flash Cannon 
		Cached::neither,	// Flatter 
		Cached::physical,	// Fling 
		Cached::physical,	// Fly 
		Cached::special,	// Focus Blast 
		Cached::neither,	// Focus Energy 
		Cached::physical,	// Focus Punch 
		Cached::neither,	// Follow Me 
		Cached::physical,	// Force Palm 
		Cached::neither,	// Foresight 
		Cached::special,	// Frenzy Plant 
		Cached::physical,	// Frustration 
		Cached::physical,	// Fury Attack 
		Cached::physical,	// Fury Cutter 
		Cached::physical,	// Fury Swipes 
		Cached::special,	// Future Sight 
		Cached::neither,	// Gastro Acid 
		Cached::special,	// Giga Drain 
		Cached::physical,	// Giga Impact 
		Cached::neither,	// Glare 
		Cached::special,	// Grass Knot 
		Cached::neither,	// GrassWhistle 
		Cached::neither,	// Gravity 
		Cached::neither,	// Growl 
		Cached::neither,	// Growth 
		Cached::neither,	// Grudge 
		Cached::neither,	// Guard Swap 
		Cached::physical,	// Guillotine 
		Cached::physical,	// Gunk Shot 
		Cached::special,	// Gust 
		Cached::physical,	// Gyro Ball 
		Cached::neither,	// Hail 
		Cached::physical,	// Hammer Arm 
		Cached::neither,	// Harden 
		Cached::neither,	// Haze 
		Cached::physical,	// Headbutt 
		Cached::physical,	// Head Smash 
		Cached::neither,	// Heal Bell 
		Cached::neither,	// Heal Block 
		Cached::neither,	// Healing Wish 
		Cached::neither,	// Heal Order 
		Cached::neither,	// Heart Swap 
		Cached::special,	// Heat Wave 
		Cached::neither,	// Helping Hand 
		Cached::special,	// Hidden Power 
		Cached::physical,	// Hi Jump Kick 
		Cached::physical,	// Horn Attack 
		Cached::physical,	// Horn Drill 
		Cached::neither,	// Howl 
		Cached::special,	// Hydro Cannon 
		Cached::special,	// Hydro Pump 
		Cached::special,	// Hyper Beam 
		Cached::physical,	// Hyper Fang 
		Cached::special,	// Hyper Voice 
		Cached::neither,	// Hypnosis 
		Cached::physical,	// Ice Ball 
		Cached::special,	// Ice Beam 
		Cached::physical,	// Ice Fang 
		Cached::physical,	// Ice Punch 
		Cached::physical,	// Ice Shard 
		Cached::physical,	// Icicle Spear 
		Cached::special,	// Icy Wind 
		Cached::neither,	// Imprison 
		Cached::neither,	// Ingrain 
		Cached::neither,	// Iron Defense 
		Cached::physical,	// Iron Head 
		Cached::physical,	// Iron Tail 
		Cached::special,	// Judgment 
		Cached::physical,	// Jump Kick 
		Cached::physical,	// Karate Chop 
		Cached::neither,	// Kinesis 
		Cached::physical,	// Knock Off 
		Cached::physical,	// Last Resort 
		Cached::special,	// Lava Plume 
		Cached::physical,	// Leaf Blade 
		Cached::special,	// Leaf Storm 
		Cached::physical,	// Leech Life 
		Cached::neither,	// Leech Seed 
		Cached::neither,	// Leer 
		Cached::physical,	// Lick 
		Cached::neither,	// Light Screen 
		Cached::neither,	// Lock-On 
		Cached::neither,	// Lovely Kiss 
		Cached::physical,	// Low Kick 
		Cached::neither,	// Lucky Chant 
		Cached::neither,	// Lunar Dance 
		Cached::special,	// Luster Purge 
		Cached::physical,	// Mach Punch 
		Cached::special,	// Magical Leaf 
		Cached::neither,	// Magic Coat 
		Cached::special,	// Magma Storm 
		Cached::physical,	// Magnet Bomb 
		Cached::neither,	// Magnet Rise 
		Cached::physical,	// Magnitude 
		Cached::neither,	// Mean Look 
		Cached::neither,	// Meditate 
		Cached::neither,	// Me First 
		Cached::special,	// Mega Drain 
		Cached::physical,	// Megahorn 
		Cached::physical,	// Mega Kick 
		Cached::physical,	// Mega Punch 
		Cached::neither,	// Memento 
		Cached::physical,	// Metal Burst 
		Cached::physical,	// Metal Claw 
		Cached::neither,	// Metal Sound 
		Cached::physical,	// Meteor Mash 
		Cached::neither,	// Metronome 
		Cached::neither,	// Milk Drink 
		Cached::neither,	// Mimic 
		Cached::neither,	// Mind Reader 
		Cached::neither,	// Minimize 
		Cached::neither,	// Miracle Eye 
		Cached::special,	// Mirror Coat 
		Cached::neither,	// Mirror Move 
		Cached::special,	// Mirror Shot 
		Cached::neither,	// Mist 
		Cached::special,	// Mist Ball 
		Cached::neither,	// Moonlight 
		Cached::neither,	// Morning Sun 
		Cached::special,	// Mud Bomb 
		Cached::special,	// Muddy Water 
		Cached::special,	// Mud Shot 
		Cached::special,	// Mud-Slap 
		Cached::neither,	// Mud Sport 
		Cached::neither,	// Nasty Plot 
		Cached::physical,	// Natural Gift 
		Cached::neither,	// Nature Power 
		Cached::physical,	// Needle Arm 
		Cached::neither,	// Nightmare 
		Cached::special,	// Night Shade 
		Cached::physical,	// Night Slash 
		Cached::special,	// Octazooka 
		Cached::neither,	// Odor Sleuth 
		Cached::special,	// Ominous Wind 
		Cached::physical,	// Outrage 
		Cached::special,	// Overheat 
		Cached::neither,	// Pain Split 
		Cached::physical,	// Payback 
		Cached::physical,	// Pay Day 
		Cached::physical,	// Peck 
		Cached::neither,	// Perish Song 
		Cached::special,	// Petal Dance 
		Cached::physical,	// Pin Missile 
		Cached::physical,	// Pluck 
		Cached::physical,	// Poison Fang 
		Cached::neither,	// Poison Gas 
		Cached::physical,	// Poison Jab 
		Cached::neither,	// PoisonPowder 
		Cached::physical,	// Poison Sting 
		Cached::physical,	// Poison Tail 
		Cached::physical,	// Pound 
		Cached::special,	// Powder Snow 
		Cached::special,	// Power Gem 
		Cached::neither,	// Power Swap 
		Cached::neither,	// Power Trick 
		Cached::physical,	// Power Whip 
		Cached::physical,	// Present 
		Cached::neither,	// Protect 
		Cached::special,	// Psybeam 
		Cached::special,	// Psychic 
		Cached::special,	// Psycho Boost 
		Cached::physical,	// Psycho Cut 
		Cached::neither,	// Psycho Shift 
		Cached::neither,	// Psych Up 
		Cached::special,	// Psywave 
		Cached::physical,	// Punishment 
		Cached::physical,	// Pursuit 
		Cached::physical,	// Quick Attack 
		Cached::physical,	// Rage 
		Cached::neither,	// Rain Dance 
		Cached::physical,	// Rapid Spin 
		Cached::physical,	// Razor Leaf 
		Cached::special,	// Razor Wind 
		Cached::neither,	// Recover 
		Cached::neither,	// Recycle 
		Cached::neither,	// Reflect 
		Cached::neither,	// Refresh 
		Cached::neither,	// Rest 
		Cached::physical,	// Return 
		Cached::physical,	// Revenge 
		Cached::physical,	// Reversal 
		Cached::neither,	// Roar 
		Cached::special,	// Roar of Time 
		Cached::physical,	// Rock Blast 
		Cached::physical,	// Rock Climb 
		Cached::neither,	// Rock Polish 
		Cached::physical,	// Rock Slide 
		Cached::physical,	// Rock Smash 
		Cached::physical,	// Rock Throw 
		Cached::physical,	// Rock Tomb 
		Cached::physical,	// Rock Wrecker 
		Cached::neither,	// Role Play 
		Cached::physical,	// Rolling Kick 
		Cached::physical,	// Rollout 
		Cached::neither,	// Roost 
		Cached::physical,	// Sacred Fire 
		Cached::neither,	// Safeguard 
		Cached::neither,	// Sand-Attack 
		Cached::neither,	// Sandstorm 
		Cached::physical,	// Sand Tomb 
		Cached::neither,	// Scary Face 
		Cached::physical,	// Scratch 
		Cached::neither,	// Screech 
		Cached::physical,	// Secret Power 
		Cached::physical,	// Seed Bomb 
		Cached::special,	// Seed Flare 
		Cached::physical,	// Seismic Toss 
		Cached::physical,	// Selfdestruct 
		Cached::special,	// Shadow Ball 
		Cached::physical,	// Shadow Claw 
		Cached::physical,	// Shadow Force 
		Cached::physical,	// Shadow Punch 
		Cached::physical,	// Shadow Sneak 
		Cached::neither,	// Sharpen 
		Cached::special,	// Sheer Cold 
		Cached::special,	// Shock Wave 
		Cached::special,	// Signal Beam 
		Cached::special,	// Silver Wind 
		Cached::neither,	// Sing 
		Cached::neither,	// Sketch 
		Cached::neither,	// Skill Swap 
		Cached::physical,	// Skull Bash 
		Cached::physical,	// Sky Attack 
		Cached::physical,	// Sky Uppercut 
		Cached::neither,	// Slack Off 
		Cached::physical,	// Slam 
		Cached::physical,	// Slash 
		Cached::neither,	// Sleep Powder 
		Cached::neither,	// Sleep Talk 
		Cached::special,	// Sludge 
		Cached::special,	// Sludge Bomb 
		Cached::physical,	// SmellingSalt 
		Cached::special,	// Smog 
		Cached::neither,	// SmokeScreen 
		Cached::neither,	// Snatch 
		Cached::special,	// Snore 
		Cached::neither,	// Softboiled 
		Cached::special,	// SolarBeam 
		Cached::special,	// SonicBoom 
		Cached::special,	// Spacial Rend 
		Cached::physical,	// Spark 
		Cached::neither,	// Spider Web 
		Cached::physical,	// Spike Cannon 
		Cached::neither,	// Spikes 
		Cached::neither,	// Spite 
		Cached::special,	// Spit Up 
		Cached::neither,	// Splash 
		Cached::neither,	// Spore 
		Cached::neither,	// Stealth Rock 
		Cached::physical,	// Steel Wing 
		Cached::neither,	// Stockpile 
		Cached::physical,	// Stomp 
		Cached::physical,	// Stone Edge 
		Cached::physical,	// Strength 
		Cached::neither,	// String Shot 
		Cached::physical,	// Struggle 
		Cached::neither,	// Stun Spore 
		Cached::physical,	// Submission 
		Cached::neither,	// Substitute 
		Cached::physical,	// Sucker Punch 
		Cached::neither,	// Sunny Day 
		Cached::physical,	// Super Fang 
		Cached::physical,	// Superpower 
		Cached::neither,	// Supersonic 
		Cached::special,	// Surf 
		Cached::neither,	// Swagger 
		Cached::neither,	// Swallow 
		Cached::neither,	// Sweet Kiss 
		Cached::neither,	// Sweet Scent 
		Cached::special,	// Swift 
		Cached::neither,	// Switcheroo 
		Cached::neither,	// Swords Dance 
		Cached::neither,	// Synthesis 
		Cached::physical,	// Tackle 
		Cached::neither,	// Tail Glow 
		Cached::neither,	// Tail Whip 
		Cached::neither,	// Tailwind 
		Cached::physical,	// Take Down 
		Cached::neither,	// Taunt 
		Cached::neither,	// Teeter Dance 
		Cached::neither,	// Teleport 
		Cached::physical,	// Thief 
		Cached::physical,	// Thrash 
		Cached::special,	// Thunder 
		Cached::special,	// Thunderbolt 
		Cached::physical,	// Thunder Fang 
		Cached::physical,	// ThunderPunch 
		Cached::special,	// ThunderShock 
		Cached::neither,	// Thunder Wave 
		Cached::neither,	// Tickle 
		Cached::neither,	// Torment 
		Cached::neither,	// Toxic 
		Cached::neither,	// Toxic Spikes 
		Cached::neither,	// Transform 
		Cached::special,	// Tri Attack 
		Cached::neither,	// Trick 
		Cached::neither,	// Trick Room 
		Cached::physical,	// Triple Kick 
		Cached::special,	// Trump Card 
		Cached::physical,	// Twineedle 
		Cached::special,	// Twister 
		Cached::special,	// Uproar 
		Cached::physical,	// U-turn 
		Cached::special,	// Vacuum Wave 
		Cached::physical,	// ViceGrip 
		Cached::physical,	// Vine Whip 
		Cached::physical,	// Vital Throw 
		Cached::physical,	// Volt Tackle 
		Cached::physical,	// Wake-Up Slap 
		Cached::physical,	// Waterfall 
		Cached::special,	// Water Gun 
		Cached::special,	// Water Pulse 
		Cached::neither,	// Water Sport 
		Cached::special,	// Water Spout 
		Cached::special,	// Weather Ball 
		Cached::special,	// Whirlpool 
		Cached::neither,	// Whirlwind 
		Cached::neither,	// Will-O-Wisp 
		Cached::physical,	// Wing Attack 
		Cached::neither,	// Wish 
		Cached::neither,	// Withdraw 
		Cached::physical,	// Wood Hammer 
		Cached::neither,	// Worry Seed 
		Cached::physical,	// Wrap 
		Cached::special,	// Wring Out 
		Cached::physical,	// X-Scissor 
		Cached::neither,	// Yawn 
		Cached::special,	// Zap Cannon 
		Cached::physical	// Zen Headbutt 
	};
	return lookup[static_cast<unsigned>(move)];
}

}	// namespace technicalmachine
