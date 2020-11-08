// Copyright (C) 2018 David Stone
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

	auto decrement_pp(Ability const target) & {
		m_pp.decrement(target);
	}

	auto restore_pp(auto const value) & {
		m_pp.restore(value);
	}

	friend auto operator==(Move const &, Move const &) -> bool = default;
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

auto is_usable_while_frozen(Moves move) -> bool;

}	// namespace technicalmachine
