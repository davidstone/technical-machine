// Ability data structure
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include "ability.h"
#include "pokemon.h"
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

Ability::Ability ():
	name (END_ABILITY) {
}

Ability::Ability (Abilities ability):
	name (ability) {
}

bool Ability::blocks_switching (Team const & switcher, Weather const weather) const {
	switch (name) {
		case SHADOW_TAG:
			if (switcher.pokemon->ability.name != Ability::SHADOW_TAG)
				return true;
			return false;
		case ARENA_TRAP:
			if (grounded (switcher, weather))
				return true;
			return false;
		case MAGNET_PULL:
			if (is_type (switcher, STEEL))
				return true;
			return false;
		default:
			return false;
	}
}

bool Ability::is_set () const {
	return name != END_ABILITY;
}

std::string Ability::get_name () const {
	/*
	Stench replaces Honey Gather, Illuminate, Pickup, and Run Away.

	Air Lock replaces Cloud Nine.

	Pure Power replaces Huge Power.

	Battle Armor replaces Shell Armor.

	Insomnia replaces Vital Spirit.

	Clear Body replaces White Smoke.

	Solid Rock replaces Filter.
	*/

	static std::string const ability_name [] = { "Adaptability", "Aftermath", "Air Lock", "Anger Point", "Anticipation", "Arena Trap", "Bad Dreams", "Battle Armor", "Blaze", "Chlorophyll", "Clear Body", "Color Change", "Compoundeyes", "Cute Charm", "Damp", "Download", "Drizzle", "Drought", "Dry Skin", "Early Bird", "Effect Spore", "Flame Body", "Flash Fire", "Flower Gift", "Forecast", "Forewarn", "Frisk", "Gluttony", "Guts", "Heatproof", "Hustle", "Hydration", "Hyper Cutter", "Ice Body", "Immunity", "Inner Focus", "Insomnia", "Intimidate", "Iron Fist", "Keen Eye", "Klutz", "Leaf Guard", "Levitate", "Lightningrod", "Limber", "Liquid Ooze", "Magic Guard", "Magma Armor", "Magnet Pull", "Marvel Scale", "Minus", "Mold Breaker", "Motor Drive", "Multitype", "Natural Cure", "No Guard", "Normalize", "Oblivious", "Overgrow", "Own Tempo", "Plus", "Poison Heal", "Poison Point", "Pressure", "Pure Power", "Quick Feet", "Rain Dish", "Reckless", "Rivalry", "Rock Head", "Rough Skin", "Sand Stream", "Sand Veil", "Scrappy", "Serene Grace", "Shadow Tag", "Shed Skin", "Shield Dust", "Simple", "Skill Link", "Slow Start", "Sniper", "Snow Cloak", "Snow Warning", "Solar Power", "Solid Rock", "Soundproof", "Speed Boost", "Stall", "Static", "Steadfast", "Stench", "Sticky Hold", "Storm Drain", "Sturdy", "Suction Cups", "Super Luck", "Swarm", "Swift Swim", "Synchronize", "Tangled Feet", "Technician", "Thick Fat", "Tinted Lens", "Torrent", "Trace", "Truant", "Unaware", "Unburden", "Volt Absorb", "Water Absorb", "Water Veil", "Wonder Guard", "END_ABILITY" };
	return ability_name [name];
}

}
