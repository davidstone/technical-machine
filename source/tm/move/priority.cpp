// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.priority;

import tm.move.action;
import tm.move.move_name;

import tm.constant_generation;
import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

using PriorityInteger = bounded::integer<-6, 6>;

constexpr auto priority_impl(constant_gen_t<Generation::one>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Quick_Attack:
			return 1_bi;
		case MoveName::Counter:
			return -1_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::two>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Protect:
			return 3_bi;
		case MoveName::Extreme_Speed:
		case MoveName::Mach_Punch:
		case MoveName::Quick_Attack:
			return 1_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
		case MoveName::Roar:
		case MoveName::Whirlwind:
		case MoveName::Vital_Throw:
			return -1_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::three>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Helping_Hand:
			return 5_bi;
		case MoveName::Magic_Coat:
		case MoveName::Snatch:
			return 4_bi;
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Follow_Me:
		case MoveName::Protect:
			return 3_bi;
		case MoveName::Extreme_Speed:
		case MoveName::Fake_Out:
		case MoveName::Mach_Punch:
		case MoveName::Quick_Attack:
			return 1_bi;
		case MoveName::Vital_Throw:
			return -1_bi;
		case MoveName::Focus_Punch:
			return -2_bi;
		case MoveName::Revenge:
			return -3_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
			return -4_bi;
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return -5_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::four>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Helping_Hand:
			return 5_bi;
		case MoveName::Magic_Coat:
		case MoveName::Snatch:
			return 4_bi;
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Follow_Me:
		case MoveName::Protect:
			return 3_bi;
		case MoveName::Feint:
			return 2_bi;
		case MoveName::Aqua_Jet:
		case MoveName::Bide:
		case MoveName::Bullet_Punch:
		case MoveName::Extreme_Speed:
		case MoveName::Fake_Out:
		case MoveName::Ice_Shard:
		case MoveName::Mach_Punch:
		case MoveName::Quick_Attack:
		case MoveName::Shadow_Sneak:
		case MoveName::Sucker_Punch:
		case MoveName::Vacuum_Wave:
			return 1_bi;
		case MoveName::Vital_Throw:
			return -1_bi;
		case MoveName::Focus_Punch:
			return -2_bi;
		case MoveName::Avalanche:
		case MoveName::Revenge:
			return -3_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
			return -4_bi;
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return -5_bi;
		case MoveName::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::five>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Helping_Hand:
			return 5_bi;
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Magic_Coat:
		case MoveName::Protect:
		case MoveName::Snatch:
			return 4_bi;
		case MoveName::Fake_Out:
		case MoveName::Follow_Me:
		case MoveName::Quick_Guard:
		case MoveName::Rage_Powder:
		case MoveName::Wide_Guard:
			return 3_bi;
		case MoveName::Extreme_Speed:
		case MoveName::Feint:
			return 2_bi;
		case MoveName::Ally_Switch:
		case MoveName::Aqua_Jet:
		case MoveName::Bide:
		case MoveName::Bullet_Punch:
		case MoveName::Ice_Shard:
		case MoveName::Mach_Punch:
		case MoveName::Quick_Attack:
		case MoveName::Shadow_Sneak:
		case MoveName::Sucker_Punch:
		case MoveName::Vacuum_Wave:
			return 1_bi;
		case MoveName::Vital_Throw:
			return -1_bi;
		case MoveName::Focus_Punch:
			return -2_bi;
		case MoveName::Avalanche:
		case MoveName::Revenge:
			return -3_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
			return -4_bi;
		case MoveName::Circle_Throw:
		case MoveName::Dragon_Tail:
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return -5_bi;
		case MoveName::Magic_Room:
		case MoveName::Trick_Room:
		case MoveName::Wonder_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::six>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Helping_Hand:
			return 5_bi;
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Kings_Shield:
		case MoveName::Magic_Coat:
		case MoveName::Protect:
		case MoveName::Spiky_Shield:
		case MoveName::Snatch:
			return 4_bi;
		case MoveName::Crafty_Shield:
		case MoveName::Fake_Out:
		case MoveName::Quick_Guard:
		case MoveName::Wide_Guard:
			return 3_bi;
		case MoveName::Extreme_Speed:
		case MoveName::Feint:
		case MoveName::Follow_Me:
		case MoveName::Rage_Powder:
			return 2_bi;
		case MoveName::Ally_Switch:
		case MoveName::Aqua_Jet:
		case MoveName::Baby_Doll_Eyes:
		case MoveName::Bide:
		case MoveName::Bullet_Punch:
		case MoveName::Ice_Shard:
		case MoveName::Ion_Deluge:
		case MoveName::Mach_Punch:
		case MoveName::Powder:
		case MoveName::Quick_Attack:
		case MoveName::Shadow_Sneak:
		case MoveName::Sucker_Punch:
		case MoveName::Vacuum_Wave:
		case MoveName::Water_Shuriken:
			return 1_bi;
		case MoveName::Vital_Throw:
			return -1_bi;
		case MoveName::Focus_Punch:
			return -2_bi;
		case MoveName::Avalanche:
		case MoveName::Revenge:
			return -3_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
			return -4_bi;
		case MoveName::Circle_Throw:
		case MoveName::Dragon_Tail:
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return -5_bi;
		case MoveName::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::seven>, MoveName const move) -> PriorityInteger {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return 6_bi;
		case MoveName::Helping_Hand:
			return 5_bi;
		case MoveName::Baneful_Bunker:
		case MoveName::Detect:
		case MoveName::Endure:
		case MoveName::Kings_Shield:
		case MoveName::Magic_Coat:
		case MoveName::Protect:
		case MoveName::Spiky_Shield:
		case MoveName::Snatch:
			return 4_bi;
		case MoveName::Crafty_Shield:
		case MoveName::Fake_Out:
		case MoveName::Quick_Guard:
		case MoveName::Wide_Guard:
		case MoveName::Spotlight:
			return 3_bi;
		case MoveName::Ally_Switch:
		case MoveName::Extreme_Speed:
		case MoveName::Feint:
		case MoveName::First_Impression:
		case MoveName::Follow_Me:
		case MoveName::Rage_Powder:
			return 2_bi;
		case MoveName::Accelerock:
		case MoveName::Aqua_Jet:
		case MoveName::Baby_Doll_Eyes:
		case MoveName::Bide:
		case MoveName::Bullet_Punch:
		case MoveName::Ice_Shard:
		case MoveName::Ion_Deluge:
		case MoveName::Mach_Punch:
		case MoveName::Powder:
		case MoveName::Quick_Attack:
		case MoveName::Shadow_Sneak:
		case MoveName::Sucker_Punch:
		case MoveName::Vacuum_Wave:
		case MoveName::Water_Shuriken:
			return 1_bi;
		case MoveName::Vital_Throw:
			return -1_bi;
		case MoveName::Beak_Blast:
		case MoveName::Focus_Punch:
		case MoveName::Shell_Trap:
			return -2_bi;
		case MoveName::Avalanche:
		case MoveName::Revenge:
			return -3_bi;
		case MoveName::Counter:
		case MoveName::Mirror_Coat:
			return -4_bi;
		case MoveName::Circle_Throw:
		case MoveName::Dragon_Tail:
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return -5_bi;
		case MoveName::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::eight>, MoveName const move) -> PriorityInteger {
	return priority_impl(constant_gen<Generation::seven>, move);
}

constexpr auto get_priority(Generation const generation, Action const action) {
	return tv::visit(action, tv::overload(
		[](UnusedSwitch) -> PriorityInteger { return 6_bi; },
		[&](MoveName const move) {
			return constant_generation(generation, [=](auto const g) { return priority_impl(g, move); });
		}
	));
}

export struct Priority {
	explicit constexpr Priority(Generation const generation, Action const action):
		priority(get_priority(generation, action)) {
	}

	friend auto operator<=>(Priority, Priority) = default;
private:
	bounded::integer<-6, 6> priority;
};

} // namespace technicalmachine
