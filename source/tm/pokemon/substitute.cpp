// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/substitute.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>

#include <tm/constant_generation.hpp>

namespace technicalmachine {
namespace {

auto target_based(Generation const generation, Moves const move) {
	switch (move_target(generation, move)) {
		case Target::user:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::adjacent_ally:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::all:
		case Target::field:
			return Substitute::bypassed;
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::adjacent:
		case Target::any:
		case Target::all_adjacent:
			return Substitute::absorbs;
		case Target::user_or_adjacent_ally:
			// TODO: Generation 5 allows it only when used on itself
			return generation <= Generation::four ?
				Substitute::causes_failure :
				Substitute::bypassed;
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::one>, Moves const move) {
	switch (move) {
		case Moves::Disable:
		case Moves::Leech_Seed:
		case Moves::Super_Fang:
		case Moves::Transform:
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Wrap:
			return Substitute::bypassed;
		default:
			return target_based(Generation::one, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::two>, Moves const move) {
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
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Explosion:
		case Moves::Foresight:
		case Moves::Mean_Look:
		case Moves::Mimic:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Self_Destruct:
		case Moves::Spider_Web:
		case Moves::Spite:
		case Moves::Transform:
		case Moves::Whirlwind:
			return Substitute::bypassed;
		case Moves::Absorb:
		case Moves::Dream_Eater:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::two, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::three>, Moves const move) {
	// Fails
	#if 0
	switch (move) {
		case Moves::Curse: (Ghost, effect only)
	}
	#endif
	// Block Intimidate
	// Absorb Uproar
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Explosion:
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Self_Destruct:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Tickle:
		case Moves::Torment:
		case Moves::Transform:
		case Moves::Whirlwind:
			return Substitute::bypassed;
		case Moves::Dream_Eater:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::three, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::four>, Moves const move) {
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
	switch (move) {
		case Moves::Attract:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Explosion:
		case Moves::Foresight:
		case Moves::Guard_Swap:
		case Moves::Heart_Swap:
		case Moves::Miracle_Eye:
		case Moves::Odor_Sleuth:
		case Moves::Power_Swap:
		case Moves::Psych_Up:
		case Moves::Roar:
		case Moves::Role_Play:
		case Moves::Self_Destruct:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Torment:
		case Moves::Transform:
		case Moves::Whirlwind:
			return Substitute::bypassed;
		case Moves::Dream_Eater:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::four, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::five>, Moves const move) {
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
	switch (move) {
		case Moves::After_You:
		case Moves::Attract:
		case Moves::Conversion_2:
		case Moves::Disable:
		case Moves::Encore:
		case Moves::Explosion:
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
		case Moves::Self_Destruct:
		case Moves::Skill_Swap:
		case Moves::Spite:
		case Moves::Taunt:
		case Moves::Torment:
		case Moves::Whirlwind:
			return Substitute::bypassed;
		default:
			return target_based(Generation::five, move);
	}
}

constexpr auto latest_substitute_interaction(Generation const generation, Moves const move) {
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
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
		case Moves::Explosion:
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
		case Moves::Self_Destruct:
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
			return Substitute::bypassed;
		default:
			return target_based(generation, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::six>, Moves const move) {
	return latest_substitute_interaction(Generation::six, move);
}
constexpr auto substitute_interaction_impl(constant_gen_t<Generation::seven>, Moves const move) {
	return latest_substitute_interaction(Generation::seven, move);
}
constexpr auto substitute_interaction_impl(constant_gen_t<Generation::eight>, Moves const move) {
	return latest_substitute_interaction(Generation::eight, move);
}

} // namespace

auto substitute_interaction(Generation const generation, Moves const move) -> Substitute::Interaction {
	return constant_generation(generation, [=](auto const g) { return substitute_interaction_impl(g, move); });
}

} // namespace technicalmachine
