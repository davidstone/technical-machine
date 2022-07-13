// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/calculate_damage.hpp>
#include <tm/move/category.hpp>
#include <tm/move/executed_move.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/will_be_recharge_turn.hpp>

#include <tm/pokemon/substitute.hpp>

#include <tm/stat/hp.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <bounded/overload.hpp>
#include <bounded/detail/variant/variant.hpp>

namespace technicalmachine {

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

	template<any_team UserTeam>	
	auto value(UserTeam const & user, ExecutedMove<UserTeam> const executed, bool const move_weakened_from_item, any_team auto const & other, OtherMove const other_move, Weather const weather) const -> damage_type {
		auto calculate = [&] {
			auto const substitute = substitute_interaction(generation_from<UserTeam>, executed.move.name);
			auto const no_damage =
				!is_damaging(executed.move.name) or
				(other.pokemon().substitute() and substitute != Substitute::bypassed) or
				will_be_recharge_turn(user.pokemon(), executed.move.name, other.pokemon().ability(), weather);
			return no_damage ? 0_bi : calculate_damage(user, executed, move_weakened_from_item, other, other_move, weather);
		};

		return bounded::visit(m_value, bounded::overload(
			[=](Unknown) { return calculate(); },
			[=](Capped const capped) -> damage_type { return bounded::min(calculate(), capped.value); },
			[](Known const known) -> damage_type { return known.value; }
		));
	}

private:
	bounded::variant<
		Unknown,
		Capped,
		Known
	> m_value;
};

} // namespace technicalmachine
