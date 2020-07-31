// Copyright (C) 2019 David Stone
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

#include <tm/move/side_effects.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Moves : std::uint16_t;

template<Generation generation>
struct UsedMove {
	constexpr UsedMove(
		Moves const selected_,
		Moves const executed_,
		bool const critical_hit_,
		bool const miss_,
		typename SideEffect<generation>::Function const side_effect_
	):
		side_effect(side_effect_),
		selected(selected_),
		executed(executed_),
		critical_hit(critical_hit_),
		miss(miss_)
	{
	}

	constexpr UsedMove(Moves const selected_, typename SideEffect<generation>::Function const side_effect_):
		side_effect(side_effect_),
		selected(selected_),
		executed(selected),
		critical_hit(false),
		miss(false)
	{
	}

	typename SideEffect<generation>::Function side_effect;
	Moves selected;
	Moves executed;
	bool critical_hit;
	bool miss;
};

}	// namespace technicalmachine
