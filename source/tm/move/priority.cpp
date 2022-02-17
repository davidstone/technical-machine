// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/priority.hpp>

#include <tm/move/moves.hpp>

#include <tm/constant_generation.hpp>

namespace technicalmachine {

using namespace bounded::literal;
namespace {

using PriorityInteger = bounded::integer<-6, 6>;

constexpr auto priority_impl(constant_gen_t<Generation::one>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Quick_Attack:
			return 1_bi;
		case Moves::Counter:
			return -1_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::two>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Protect:
			return 3_bi;
		case Moves::Extreme_Speed:
		case Moves::Mach_Punch:
		case Moves::Quick_Attack:
			return 1_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
		case Moves::Roar:
		case Moves::Whirlwind:
		case Moves::Vital_Throw:
			return -1_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::three>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Magic_Coat:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Follow_Me:
		case Moves::Protect:
			return 3_bi;
		case Moves::Extreme_Speed:
		case Moves::Fake_Out:
		case Moves::Mach_Punch:
		case Moves::Quick_Attack:
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Focus_Punch:
			return -2_bi;
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::four>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Magic_Coat:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Follow_Me:
		case Moves::Protect:
			return 3_bi;
		case Moves::Feint:
			return 2_bi;
		case Moves::Aqua_Jet:
		case Moves::Bide:
		case Moves::Bullet_Punch:
		case Moves::Extreme_Speed:
		case Moves::Fake_Out:
		case Moves::Ice_Shard:
		case Moves::Mach_Punch:
		case Moves::Quick_Attack:
		case Moves::Shadow_Sneak:
		case Moves::Sucker_Punch:
		case Moves::Vacuum_Wave:
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Focus_Punch:
			return -2_bi;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		case Moves::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::five>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Magic_Coat:
		case Moves::Protect:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Fake_Out:
		case Moves::Follow_Me:
		case Moves::Quick_Guard:
		case Moves::Rage_Powder:
		case Moves::Wide_Guard:
			return 3_bi;
		case Moves::Extreme_Speed:
		case Moves::Feint:
			return 2_bi;
		case Moves::Ally_Switch:
		case Moves::Aqua_Jet:
		case Moves::Bide:
		case Moves::Bullet_Punch:
		case Moves::Ice_Shard:
		case Moves::Mach_Punch:
		case Moves::Quick_Attack:
		case Moves::Shadow_Sneak:
		case Moves::Sucker_Punch:
		case Moves::Vacuum_Wave:
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Focus_Punch:
			return -2_bi;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Circle_Throw:
		case Moves::Dragon_Tail:
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		case Moves::Magic_Room:
		case Moves::Trick_Room:
		case Moves::Wonder_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::six>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Kings_Shield:
		case Moves::Magic_Coat:
		case Moves::Protect:
		case Moves::Spiky_Shield:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Crafty_Shield:
		case Moves::Fake_Out:
		case Moves::Quick_Guard:
		case Moves::Wide_Guard:
			return 3_bi;
		case Moves::Extreme_Speed:
		case Moves::Feint:
		case Moves::Follow_Me:
		case Moves::Rage_Powder:
			return 2_bi;
		case Moves::Ally_Switch:
		case Moves::Aqua_Jet:
		case Moves::Baby_Doll_Eyes:
		case Moves::Bide:
		case Moves::Bullet_Punch:
		case Moves::Ice_Shard:
		case Moves::Ion_Deluge:
		case Moves::Mach_Punch:
		case Moves::Powder:
		case Moves::Quick_Attack:
		case Moves::Shadow_Sneak:
		case Moves::Sucker_Punch:
		case Moves::Vacuum_Wave:
		case Moves::Water_Shuriken:
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Focus_Punch:
			return -2_bi;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Circle_Throw:
		case Moves::Dragon_Tail:
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		case Moves::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::seven>, Moves const move) -> PriorityInteger {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Baneful_Bunker:
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Kings_Shield:
		case Moves::Magic_Coat:
		case Moves::Protect:
		case Moves::Spiky_Shield:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Crafty_Shield:
		case Moves::Fake_Out:
		case Moves::Quick_Guard:
		case Moves::Wide_Guard:
		case Moves::Spotlight:
			return 3_bi;
		case Moves::Ally_Switch:
		case Moves::Extreme_Speed:
		case Moves::Feint:
		case Moves::First_Impression:
		case Moves::Follow_Me:
		case Moves::Rage_Powder:
			return 2_bi;
		case Moves::Accelerock:
		case Moves::Aqua_Jet:
		case Moves::Baby_Doll_Eyes:
		case Moves::Bide:
		case Moves::Bullet_Punch:
		case Moves::Ice_Shard:
		case Moves::Ion_Deluge:
		case Moves::Mach_Punch:
		case Moves::Powder:
		case Moves::Quick_Attack:
		case Moves::Shadow_Sneak:
		case Moves::Sucker_Punch:
		case Moves::Vacuum_Wave:
		case Moves::Water_Shuriken:
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Beak_Blast:
		case Moves::Focus_Punch:
		case Moves::Shell_Trap:
			return -2_bi;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Circle_Throw:
		case Moves::Dragon_Tail:
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		case Moves::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}

constexpr auto priority_impl(constant_gen_t<Generation::eight>, Moves const move) -> PriorityInteger {
	return priority_impl(constant_gen<Generation::seven>, move);
}

constexpr auto get_priority(Generation const generation, Moves const move) {
	return constant_generation(generation, [=](auto const g) { return priority_impl(g, move); });
}

} // namespace

Priority::Priority(Generation const generation, Moves const move):
	priority(get_priority(generation, move)) {
}

} // namespace technicalmachine
