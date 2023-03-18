// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.actual_damage;

import tm.move.calculate_damage;
import tm.move.category;
import tm.move.damage_type;
import tm.move.executed_move;
import tm.move.other_move;
import tm.move.will_be_recharge_turn;

import tm.pokemon.substitute;

import tm.stat.current_hp;

import tm.any_team;
import tm.environment;
import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export struct ActualDamage {
	struct Unknown {};
	struct Capped {
		bounded::integer<0, bounded::builtin_max_value<CurrentHP> / 4> value;
	};
	struct Known {
		CurrentHP value;
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
	auto value(UserTeam const & user, ExecutedMove<UserTeam> const executed, bool const move_weakened_from_item, any_team auto const & other, OtherMove const other_move, Environment const environment) const -> damage_type {
		auto calculate = [&] {
			auto const substitute = substitute_interaction(generation_from<UserTeam>, executed.move.name);
			auto const no_damage =
				!is_damaging(executed.move.name) or
				(other.pokemon().substitute() and substitute != Substitute::bypassed) or
				will_be_recharge_turn(user.pokemon(), executed.move.name, other.pokemon().ability(), environment);
			return no_damage ? 0_bi : calculate_damage(user, executed, move_weakened_from_item, other, other_move, environment);
		};

		return tv::visit(m_value, tv::overload(
			[=](Unknown) { return calculate(); },
			[=](Capped const capped) -> damage_type { return bounded::min(calculate(), capped.value); },
			[](Known const known) -> damage_type { return known.value; }
		));
	}

private:
	tv::variant<
		Unknown,
		Capped,
		Known
	> m_value;
};

export struct FlaggedActualDamage {
	ActualDamage value;
	bool did_any_damage;
};

} // namespace technicalmachine
