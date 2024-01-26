// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.move.action;

import tm.move.move_name;
import tm.move.pass;
import tm.move.switch_;

import tm.pokemon.max_pokemon_per_team;

import bounded;
import numeric_traits;
import std_module;
import tv;

using namespace bounded::literal;

namespace technicalmachine {

using Index = tv::variant_index<MoveName, Switch, Pass>;

constexpr auto pass_index = bounded::constant<numeric_traits::max_value<MoveName>> + 1_bi;

export struct Action {
	constexpr Action(MoveName const move):
		m_value(move)
	{
	}
	constexpr Action(Switch const switch_):
		m_value(switch_.value())
	{
	}
	constexpr Action(Pass):
		m_value(pass_index)
	{
	}

	constexpr auto index() const -> Index {
		return
			m_value <= numeric_traits::max_value<TeamIndex> ? Index(1_bi) :
			m_value <= bounded::constant<numeric_traits::max_value<MoveName>> ? Index(0_bi) :
			Index(2_bi);
	}

	constexpr auto operator[](bounded::type_t<MoveName> index_) const -> MoveName {
		BOUNDED_ASSERT(index() == index_);
		return MoveName(m_value);
	}
	constexpr auto operator[](bounded::constant_t<0>) const -> MoveName {
		return (*this)[bounded::type<MoveName>];
	}
	constexpr auto operator[](bounded::type_t<Switch>) const -> Switch {
		return Switch(bounded::assume_in_range<TeamIndex>(m_value));
	}
	constexpr auto operator[](bounded::constant_t<1>) const -> Switch {
		return (*this)[bounded::type<Switch>];
	}
	constexpr auto operator[](bounded::type_t<Pass> index_) const -> Pass {
		BOUNDED_ASSERT(index() == index_);
		return pass;
	}
	constexpr auto operator[](bounded::constant_t<2>) const -> Pass {
		return (*this)[bounded::type<Pass>];
	}

	friend constexpr auto operator==(Action, Action) -> bool = default;

private:
	static_assert(bounded::constant<numeric_traits::min_value<MoveName>> == numeric_traits::max_value<TeamIndex> + 1_bi);
	using Integer = bounded::integer<
		0,
		bounded::normalize<pass_index>
	>;
	[[no_unique_address]] Integer m_value;

	constexpr explicit Action(bounded::tombstone_tag, auto const make):
		m_value(make())
	{
	}
	friend bounded::tombstone_traits<Action>;
	friend bounded::tombstone_traits_composer<&Action::m_value>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::Action> : bounded::tombstone_traits_composer<&technicalmachine::Action::m_value> {
};
