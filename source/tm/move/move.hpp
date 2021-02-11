// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/pp.hpp>

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
using namespace bounded::literal;

struct Move {
	explicit Move(Generation const generation, Moves const move, PP::pp_ups_type const pp_ups = 3_bi) :
		m_name(move),
		m_pp(generation, move, pp_ups)
	{
	}

	auto name() const {
		return m_name;
	}

	auto pp() const {
		return m_pp;
	}

	auto decrement_pp(bool const uses_extra_pp) & {
		m_pp.reduce(BOUNDED_CONDITIONAL(uses_extra_pp, 2_bi, 1_bi));
	}

	auto restore_pp(auto const value) & {
		m_pp.restore(value);
	}

	friend auto operator==(Move, Move) -> bool = default;
	// Pokemon don't change their moves throughout a battle, so we don't need
	// to include move name
	friend auto compress(Move const value) {
		return compress(value.m_pp);
	}
private:
	Moves m_name;
	PP m_pp;
};

inline auto operator==(Move const lhs, Moves const rhs) -> bool {
	return lhs.name() == rhs;
}

auto is_regular(Moves move) -> bool;

auto is_phaze(Moves name) -> bool;

auto thaws_user(Moves move) -> bool;

}	// namespace technicalmachine
