// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.substitute;

import tm.move.move_name;
import tm.move.target;

import tm.stat.current_hp;
import tm.stat.max_hp;

import tm.compress;
import tm.constant_generation;
import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto generation_bonus(Generation const generation) {
	return BOUNDED_CONDITIONAL(generation == Generation::one, 1_bi, 0_bi);
}

using hp_type = decltype(bounded::declval<CurrentHP>() / 4_bi + generation_bonus(Generation()));

export struct Substitute {
	enum class Interaction {
		absorbs,
		bypassed,
		causes_failure
	};
	using enum Interaction;

	constexpr auto create(
		Generation const generation,
		CurrentHP const current_hp,
		MaxHP const max_hp
	) & -> tv::optional<hp_type> {
		if (static_cast<bool>(*this)) {
			return tv::none;
		}
		constexpr auto divisor = 4_bi;
		auto const can_use_substitute = generation == Generation::one ?
			current_hp * divisor >= max_hp :
			current_hp * divisor > max_hp and max_hp != 1_bi;
		if (!can_use_substitute) {
			return tv::none;
		}
		auto const lost_hp = max_hp / divisor;
		m_hp = lost_hp + generation_bonus(generation);
		return lost_hp;
	}

	constexpr auto damage(auto const damage_done) {
		BOUNDED_ASSERT(damage_done >= 0_bi);
		auto const original_hp = m_hp;
		m_hp = bounded::max(m_hp - damage_done, 0_bi);
		return bounded::assume_in_range<CurrentHP>(original_hp - m_hp);
	}

	constexpr auto hp() const -> hp_type {
		return m_hp;
	}

	explicit constexpr operator bool() const {
		return hp() != 0_bi;
	}

	friend auto operator==(Substitute, Substitute) -> bool = default;
	friend constexpr auto compress(Substitute const value) {
		return value.m_hp;
	}

private:
	hp_type m_hp = 0_bi;
};

constexpr auto target_based(Generation const generation, MoveName const move) {
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

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::one>, MoveName const move) {
	switch (move) {
		case MoveName::Disable:
		case MoveName::Leech_Seed:
		case MoveName::Super_Fang:
		case MoveName::Transform:
		case MoveName::Bind:
		case MoveName::Clamp:
		case MoveName::Fire_Spin:
		case MoveName::Wrap:
			return Substitute::bypassed;
		default:
			return target_based(Generation::one, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::two>, MoveName const move) {
	// Fails if user has Substitute
	#if 0
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
		case MoveName::Protect:
		case MoveName::Detect:
		case MoveName::Endure:
	}
	#endif
	// Partially blocked
	#if 0
	switch (move) {
		case MoveName::Curse: (Ghost, effect only)
		case MoveName::Swagger: (confusion only)
	}
	#endif
	// Cancel partial trap
	// Boost Rage even if Substitute hit
	switch (move) {
		case MoveName::Attract:
		case MoveName::Disable:
		case MoveName::Encore:
		case MoveName::Foresight:
		case MoveName::Mean_Look:
		case MoveName::Mimic:
		case MoveName::Psych_Up:
		case MoveName::Roar:
		case MoveName::Spider_Web:
		case MoveName::Spite:
		case MoveName::Transform:
		case MoveName::Whirlwind:
			return Substitute::bypassed;
		case MoveName::Absorb:
		case MoveName::Dream_Eater:
		case MoveName::Giga_Drain:
		case MoveName::Leech_Life:
		case MoveName::Mega_Drain:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::two, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::three>, MoveName const move) {
	// Fails
	#if 0
	switch (move) {
		case MoveName::Curse: (Ghost, effect only)
	}
	#endif
	// Block Intimidate
	// Absorb Uproar
	switch (move) {
		case MoveName::Attract:
		case MoveName::Disable:
		case MoveName::Encore:
		case MoveName::Foresight:
		case MoveName::Odor_Sleuth:
		case MoveName::Psych_Up:
		case MoveName::Roar:
		case MoveName::Role_Play:
		case MoveName::Skill_Swap:
		case MoveName::Spite:
		case MoveName::Taunt:
		case MoveName::Tickle:
		case MoveName::Torment:
		case MoveName::Transform:
		case MoveName::Whirlwind:
			return Substitute::bypassed;
		case MoveName::Dream_Eater:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::three, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::four>, MoveName const move) {
	// Fails
	#if 0
	switch (move) {
		case MoveName::Curse: (Ghost, effect only)
	}
	#endif
	// Block Intimidate
	// Block confusion from berries
	// Blocks Intimidate even if U-turn breaks the Substitute
	// Blocks Toxic Spikes poisoning
	// Defog doesn't lower evasiveness
	// Absorb Uproar
	switch (move) {
		case MoveName::Attract:
		case MoveName::Disable:
		case MoveName::Encore:
		case MoveName::Foresight:
		case MoveName::Guard_Swap:
		case MoveName::Heart_Swap:
		case MoveName::Miracle_Eye:
		case MoveName::Odor_Sleuth:
		case MoveName::Power_Swap:
		case MoveName::Psych_Up:
		case MoveName::Roar:
		case MoveName::Role_Play:
		case MoveName::Skill_Swap:
		case MoveName::Spite:
		case MoveName::Taunt:
		case MoveName::Torment:
		case MoveName::Transform:
		case MoveName::Whirlwind:
			return Substitute::bypassed;
		case MoveName::Dream_Eater:
			return Substitute::causes_failure;
		default:
			return target_based(Generation::four, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::five>, MoveName const move) {
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
	switch (move) {
		case MoveName::After_You:
		case MoveName::Attract:
		case MoveName::Conversion_2:
		case MoveName::Disable:
		case MoveName::Encore:
		case MoveName::Foresight:
		case MoveName::Guard_Swap:
		case MoveName::Heart_Swap:
		case MoveName::Miracle_Eye:
		case MoveName::Odor_Sleuth:
		case MoveName::Power_Swap:
		case MoveName::Psych_Up:
		case MoveName::Reflect_Type:
		case MoveName::Roar:
		case MoveName::Role_Play:
		case MoveName::Skill_Swap:
		case MoveName::Spite:
		case MoveName::Taunt:
		case MoveName::Torment:
		case MoveName::Whirlwind:
			return Substitute::bypassed;
		default:
			return target_based(Generation::five, move);
	}
}

constexpr auto latest_substitute_interaction(Generation const generation, MoveName const move) {
	// Block Intimidate
	// Block Imposter
	// Absorb Uproar
	switch (move) {
		case MoveName::After_You:
		case MoveName::Attract:
		case MoveName::Bestow:
		case MoveName::Boomburst:
		case MoveName::Bug_Buzz:
		case MoveName::Chatter:
		case MoveName::Clanging_Scales:
		case MoveName::Clangorous_Soulblaze:
		case MoveName::Confide:
		case MoveName::Conversion_2:
		case MoveName::Disable:
		case MoveName::Disarming_Voice:
		case MoveName::Echoed_Voice:
		case MoveName::Encore:
		case MoveName::Foresight:
		case MoveName::Grass_Whistle:
		case MoveName::Growl:
		case MoveName::Guard_Swap:
		case MoveName::Heart_Swap:
		case MoveName::Hyperspace_Fury:
		case MoveName::Hyperspace_Hole:
		case MoveName::Hyper_Voice:
		case MoveName::Instruct:
		case MoveName::Metal_Sound:
		case MoveName::Miracle_Eye:
		case MoveName::Noble_Roar:
		case MoveName::Odor_Sleuth:
		case MoveName::Parting_Shot:
		case MoveName::Play_Nice:
		case MoveName::Powder:
		case MoveName::Power_Swap:
		case MoveName::Psych_Up:
		case MoveName::Reflect_Type:
		case MoveName::Relic_Song:
		case MoveName::Roar:
		case MoveName::Role_Play:
		case MoveName::Round:
		case MoveName::Screech:
		case MoveName::Sing:
		case MoveName::Skill_Swap:
		case MoveName::Snarl:
		case MoveName::Snore:
		case MoveName::Sparkling_Aria:
		case MoveName::Spectral_Thief:
		case MoveName::Speed_Swap:
		case MoveName::Spite:
		case MoveName::Supersonic:
		case MoveName::Taunt:
		case MoveName::Torment:
		case MoveName::Whirlwind:
			return Substitute::bypassed;
		default:
			return target_based(generation, move);
	}
}

constexpr auto substitute_interaction_impl(constant_gen_t<Generation::six>, MoveName const move) {
	return latest_substitute_interaction(Generation::six, move);
}
constexpr auto substitute_interaction_impl(constant_gen_t<Generation::seven>, MoveName const move) {
	return latest_substitute_interaction(Generation::seven, move);
}
constexpr auto substitute_interaction_impl(constant_gen_t<Generation::eight>, MoveName const move) {
	return latest_substitute_interaction(Generation::eight, move);
}

export constexpr auto substitute_interaction(Generation const generation, MoveName const move) -> Substitute::Interaction {
	return constant_generation(generation, [=](auto const g) { return substitute_interaction_impl(g, move); });
}

} // namespace technicalmachine
