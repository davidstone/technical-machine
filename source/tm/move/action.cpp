// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.action;

import tm.move.move_name;
import tm.move.switch_;

import bounded;
import numeric_traits;
import std_module;
import tv;

using namespace bounded::literal;

namespace technicalmachine {

export struct UnusedSwitch {
	UnusedSwitch() = delete;
	friend constexpr auto operator==(UnusedSwitch, UnusedSwitch) -> bool = default;
};

export struct Action {
	constexpr Action(MoveName const move):
		m_value(move)
	{
	}
	[[noreturn]] Action(UnusedSwitch) {
		std::unreachable();
	}

	constexpr auto index() const -> tv::variant_index<MoveName, UnusedSwitch> {
		return tv::variant_index<MoveName, UnusedSwitch>(0_bi);
	}

	constexpr auto operator[](bounded::constant_t<0>) const -> MoveName {
		return MoveName(m_value);
	}
	constexpr auto operator[](bounded::type_t<MoveName>) const -> MoveName {
		return (*this)[0_bi];
	}
	auto operator[](bounded::constant_t<1>) const -> UnusedSwitch {
		std::unreachable();
	}
	constexpr auto operator[](bounded::type_t<UnusedSwitch>) const -> UnusedSwitch {
		return (*this)[1_bi];
	}

	friend constexpr auto operator==(Action, Action) -> bool = default;

private:
	using Integer = bounded::integer<
		0,
		bounded::normalize<numeric_traits::max_value<MoveName>>
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

namespace technicalmachine {

export constexpr auto get_move(Action const action) -> MoveName {
	return tv::visit(action, tv::overload(
		[](MoveName const move) -> MoveName { return move; },
		[](UnusedSwitch) -> MoveName { std::unreachable(); }
	));
}

} // namespace technicalmachine
