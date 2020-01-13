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

#include <tm/move/priority.hpp>

#include <tm/move/moves.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {

using namespace bounded::literal;
namespace {

using PriorityInteger = bounded::integer<-6, 6>;

constexpr auto gen_one_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_two_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_three_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_four_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_five_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_six_priority(Moves const move) -> PriorityInteger {
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

constexpr auto gen_seven_priority(Moves const move) -> PriorityInteger {
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

constexpr auto get_priority(Generation const generation, Moves const move) {
	switch (generation) {
		case Generation::one: return gen_one_priority(move);
		case Generation::two: return gen_two_priority(move);
		case Generation::three: return gen_three_priority(move);
		case Generation::four: return gen_four_priority(move);
		case Generation::five: return gen_five_priority(move);
		case Generation::six: return gen_six_priority(move);
		case Generation::seven: return gen_seven_priority(move);
		case Generation::eight: return gen_seven_priority(move);
	}
}

}	// namespace

Priority::Priority(Generation const generation, Moves const move) :
	priority(get_priority(generation, move)) {
}

}	// namespace technicalmachine
