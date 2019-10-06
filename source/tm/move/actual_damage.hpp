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

#include <tm/move/executed_move.hpp>
#include <tm/move/other_move.hpp>

#include <tm/stat/hp.hpp>

#include <tm/weather.hpp>

#include <bounded/detail/variant/variant.hpp>

namespace technicalmachine {

enum class Generation;
struct Team;

struct ActualDamage {
	struct Unknown {};
	struct Capped {
		bounded::integer<0, HP::max_value / 4> value;
	};
	struct Known {
		HP::current_type value;
	};

	constexpr ActualDamage(Unknown const value_):
		m_value(value_)
	{
	}
	constexpr ActualDamage(Capped const value_):
		m_value(value_)
	{
	}
	constexpr ActualDamage(Known const value_):
		m_value(value_)
	{
	}
	
	auto value(Generation generation, Team const & user, ExecutedMove move, Team const & other, OtherMove other_move, Weather weather) const -> HP::current_type;
private:
	bounded::variant<
		Unknown,
		Capped,
		Known
	> m_value;
};

}	// namespace technicalmachine
