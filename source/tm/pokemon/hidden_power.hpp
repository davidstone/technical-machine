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

#include <tm/stat/base_stats.hpp>
#include <tm/stat/iv.hpp>

#include <tm/type/type.hpp>

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

// TODO: Make this a template based on the generation so we don't need to store
// the power
struct HiddenPower {
	template<typename... IVs>
	constexpr HiddenPower(Generation const generation, IVs const ... ivs):
		m_power(calculate_power(generation, std::make_index_sequence<sizeof...(ivs)>{}, ivs...)),
		m_type(calculate_type(std::make_index_sequence<sizeof...(ivs)>{}, ivs...))
	{
	}
	
	constexpr auto power() const {
		return m_power;
	}
	constexpr auto type() const {
		return m_type;
	}
	
private:
	using Power = bounded::integer<30, 70>;

	template<std::size_t... indexes, typename... IVs>
	static constexpr auto calculate_power(Generation const generation, std::index_sequence<indexes...>, IVs... ivs) -> Power {
		// TODO: This is probably best expressed with bit operations
		return BOUNDED_CONDITIONAL(generation >= Generation::six,
			60_bi,
			(... + (((ivs.value() / 2_bi) % 2_bi) << bounded::constant<indexes>)) * 40_bi / 63_bi + 30_bi
		);
	}
	
	template<std::size_t... indexes, typename... IVs>
	static constexpr auto calculate_type(std::index_sequence<indexes...>, IVs... ivs) -> Type {
		// TODO: This is probably best expressed with bit operations
		auto const index = (... + ((ivs.value() % 2_bi) << bounded::constant<indexes>)) * 15_bi / 63_bi;
		switch (index.value()) {
			case 0: return Type::Fighting;
			case 1: return Type::Flying;
			case 2: return Type::Poison;
			case 3: return Type::Ground;
			case 4: return Type::Rock;
			case 5: return Type::Bug;
			case 6: return Type::Ghost;
			case 7: return Type::Steel;
			case 8: return Type::Fire;
			case 9: return Type::Water;
			case 10: return Type::Grass;
			case 11: return Type::Electric;
			case 12: return Type::Psychic;
			case 13: return Type::Ice;
			case 14: return Type::Dragon;
			case 15: return Type::Dark;
			default: static_assert(index.max() == 15); BOUNDED_ASSERT_OR_ASSUME(false);
		}
	}

	Power m_power;
	Type m_type;
};

}	// namespace technicalmachine
