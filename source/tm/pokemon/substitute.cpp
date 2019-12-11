// Copyright (C) 2018 David Stone
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

#include <tm/pokemon/substitute.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {
namespace {

constexpr auto gen1_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::Disable:
		case Moves::Leech_Seed:
		case Moves::Super_Fang:
		case Moves::Transform:
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Wrap:
			return true;
		default:
			return false;
	}
}

constexpr auto gen2_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Foresight:
		case Moves::Mean_Look:
		case Moves::Mimic:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Spider_Web:
		case Moves::Spite:
		case Moves::Transform:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

constexpr auto gen3_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Tickle:
		case Moves::Torment:
		case Moves::Transform:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

constexpr auto gen4_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Foresight:
		case Moves::Guard_Swap:
		case Moves::Heart_Swap:
		case Moves::Miracle_Eye:
		case Moves::Odor_Sleuth:
		case Moves::Power_Swap:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Torment:
		case Moves::Transform:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

constexpr auto gen5_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::After_You:
		case Moves::Attract:
		case Moves::Conversion_2:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Foresight:
		case Moves::Guard_Swap:
		case Moves::Heart_Swap:
		case Moves::Miracle_Eye:
		case Moves::Odor_Sleuth:
		case Moves::Power_Swap:
		case Moves::Psych_Up:
		case Moves::Reflect_Type:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Torment:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

constexpr auto gen6_bypasses_substitute(Moves const move) {
	switch (move) {
		case Moves::After_You:
		case Moves::Attract:
		case Moves::Bestow:
		case Moves::Boomburst:
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::Clanging_Scales:
		case Moves::Clangorous_Soulblaze:
		case Moves::Confide:
		case Moves::Conversion_2:
		case Moves::Disable:
		case Moves::Disarming_Voice:
		case Moves::Echoed_Voice:
		case Moves::Encore:
		case Moves::Foresight:
		case Moves::Grass_Whistle:
		case Moves::Growl:
		case Moves::Guard_Swap:
		case Moves::Heart_Swap:
		case Moves::Hyperspace_Fury:
		case Moves::Hyperspace_Hole:
		case Moves::Hyper_Voice:
		case Moves::Instruct:
		case Moves::Metal_Sound:
		case Moves::Miracle_Eye:
		case Moves::Noble_Roar:
		case Moves::Odor_Sleuth:
		case Moves::Parting_Shot:
		case Moves::Play_Nice:
		case Moves::Powder:
		case Moves::Power_Swap:
		case Moves::Psych_Up:
		case Moves::Reflect_Type:
		case Moves::Relic_Song:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Round:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Skill_Swap:
		case Moves::Snarl:
		case Moves::Snore:
		case Moves::Sparkling_Aria:
		case Moves::Spectral_Thief:
		case Moves::Speed_Swap:
		case Moves::Spite:
		case Moves::Supersonic:
		case Moves::Taunt:
		case Moves::Torment:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

constexpr auto gen7_bypasses_substitute(Moves const move) {
	return gen6_bypasses_substitute(move);
}

constexpr auto gen8_bypasses_substitute(Moves const move) {
	return gen7_bypasses_substitute(move); // TODO
}

constexpr auto move_bypasses_substitute(Generation const generation, Moves const move) {
	switch (generation) {
		case Generation::one: return gen1_bypasses_substitute(move);
		case Generation::two: return gen2_bypasses_substitute(move);
		case Generation::three: return gen3_bypasses_substitute(move);
		case Generation::four: return gen4_bypasses_substitute(move);
		case Generation::five: return gen5_bypasses_substitute(move);
		case Generation::six: return gen6_bypasses_substitute(move);
		case Generation::seven: return gen7_bypasses_substitute(move);
		case Generation::eight: return gen8_bypasses_substitute(move);
	}
}

} // namespace

auto blocked_by_substitute(Generation const generation, Moves const move) -> bool {
	switch (move_target(generation, move)) {
		case Target::user:
		case Target::all_allies:
		case Target::adjacent_ally:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::all:
		case Target::field:
			return false;
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::adjacent:
		case Target::any:
		case Target::all_adjacent:
			return !move_bypasses_substitute(generation, move);
		case Target::user_or_adjacent_ally:
			// TODO: Generation 5 allows it only when used on itself
			return generation <= Generation::four;
	}
	// Gen 2
	// Fails if user has Substitute
	#if 0
		case Moves::Counter:
		case Moves::Mirror_Coat:
		case Moves::Protect:
		case Moves::Detect:
		case Moves::Endure:
	}
	#endif
	// Partially blocked
	#if 0
	switch (move) {
		case Moves::Curse: (Ghost, effect only)
		case Moves::Swagger: (confusion only)
	}
	#endif
	// Cancel partial trap
	// Boost Rage even if Substitute hit
	// Gen 3
	// Fails
	#if 0
	switch (move) {
		case Moves::Curse: (Ghost, effect only)
	}
	#endif
	// Block Intimidate
	// Absorb Uproar
	// Gen 4
	// Fails
	#if 0
	switch (move) {
		case Moves::Curse: (Ghost, effect only)
	}
	#endif
	// Block Intimidate
	// Block confusion from berries
	// Blocks Intimidate even if U-turn breaks the Substitute
	// Blocks Toxic Spikes poisoning
	// Defog doesn't lower evasiveness
	// Absorb Uproar
	// Gen 5
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
	// Gen 6
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
}

namespace {

constexpr auto gen1_damage_blocked_by_substitute(Moves) {
	return false;
}

constexpr auto gen2_damage_blocked_by_substitute(Moves const move) {
	switch (move) {
		case Moves::Absorb:
		case Moves::Dream_Eater:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			return true;
		default:
			return false;
	}
}

constexpr auto gen3_damage_blocked_by_substitute(Moves const move) {
	return move == Moves::Dream_Eater;
}

constexpr auto gen4_damage_blocked_by_substitute(Moves const move) {
	return move == Moves::Dream_Eater;
}

constexpr auto gen5_damage_blocked_by_substitute(Moves) {
	return false;
}

constexpr auto gen6_damage_blocked_by_substitute(Moves) {
	return false;
}

constexpr auto gen7_damage_blocked_by_substitute(Moves) {
	return false;
}

constexpr auto gen8_damage_blocked_by_substitute(Moves) {
	return false;
}

} // namespace

auto damage_blocked_by_substitute(Generation const generation, Moves const move) -> bool {
	switch (generation) {
		case Generation::one: return gen1_damage_blocked_by_substitute(move);
		case Generation::two: return gen2_damage_blocked_by_substitute(move);
		case Generation::three: return gen3_damage_blocked_by_substitute(move);
		case Generation::four: return gen4_damage_blocked_by_substitute(move);
		case Generation::five: return gen5_damage_blocked_by_substitute(move);
		case Generation::six: return gen6_damage_blocked_by_substitute(move);
		case Generation::seven: return gen7_damage_blocked_by_substitute(move);
		case Generation::eight: return gen8_damage_blocked_by_substitute(move);
	}
}

} // namespace technicalmachine
