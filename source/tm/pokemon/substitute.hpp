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

#include <tm/compress.hpp>
#include <tm/operators.hpp>
#include <tm/stat/hp.hpp>

#include <bounded/integer.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
using namespace bounded::literal;

struct Substitute {
private:
	using hp_type = decltype(std::declval<HP::current_type>() / 4_bi);
public:
	enum class Interaction { absorbs, bypassed, causes_failure };
	// TODO: using enum
	static constexpr auto absorbs = Interaction::absorbs;
	static constexpr auto bypassed = Interaction::bypassed;
	static constexpr auto causes_failure = Interaction::causes_failure;

	constexpr auto create(HP::current_type const total_hp) -> hp_type {
		if (static_cast<bool>(*this)) {
			return 0_bi;
		}
		m_hp = total_hp / 4_bi;
		return m_hp;
	}

	constexpr auto damage(auto const damage_done) {
		BOUNDED_ASSERT(damage_done >= 0_bi);
		auto const original_hp = m_hp;
		m_hp = bounded::max(m_hp - damage_done, 0_bi);
		return HP::current_type(original_hp - m_hp);
	}

	constexpr auto hp() const -> hp_type {
		return m_hp;
	}

	explicit constexpr operator bool() const {
		return hp() != 0_bi;
	}

	friend auto operator==(Substitute const &, Substitute const &) -> bool = default;
	friend constexpr auto compress(Substitute const value) {
		return value.m_hp;
	}
private:
	hp_type m_hp = 0_bi;
};

auto substitute_interaction(Generation, Moves) -> Substitute::Interaction;

}	// namespace technicalmachine
