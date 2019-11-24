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

#include <tm/stat/dv.hpp>
#include <tm/stat/iv.hpp>

#include <tm/type/type.hpp>

#include <tm/generation.hpp>

#include <bounded/integer.hpp>
#include <bounded/unreachable.hpp>

namespace technicalmachine {

// TODO: Make this a template based on the generation so we don't need to store
// the power?
struct HiddenPower {
	constexpr HiddenPower(Generation const generation, DVs const dvs):
		m_power(calculate_power(dvs)),
		m_type(calculate_type(dvs))
	{
		BOUNDED_ASSERT_OR_ASSUME(generation <= Generation::two);
	}
	constexpr HiddenPower(Generation const generation, IVs const ivs):
		m_power(calculate_power(generation, ivs)),
		m_type(calculate_type(ivs))
	{
		BOUNDED_ASSERT_OR_ASSUME(generation >= Generation::three);
	}
	
	constexpr auto power() const {
		return m_power;
	}
	constexpr auto type() const {
		return m_type;
	}
	
private:
	using Power = bounded::integer<30, 70>;
	
	static constexpr auto sum_stats(IVs const ivs, auto const transform) {
		return
			transform(0_bi, ivs.hp) +
			transform(1_bi, ivs.attack) +
			transform(2_bi, ivs.defense) +
			transform(3_bi, ivs.speed) +
			transform(4_bi, ivs.special_attack) +
			transform(5_bi, ivs.special_defense);
	}

	// http://www.psypokes.com/gsc/dvguide.php
	static constexpr auto calculate_power(DVs const dvs) -> Power {
		auto const bit = [](DV const dv) { return BOUNDED_CONDITIONAL(dv.value() < 8_bi, 0_bi, 1_bi); };
		auto const x =
			(bit(dvs.attack) << 3_bi) +
			(bit(dvs.defense) << 2_bi) +
			(bit(dvs.speed) << 1_bi) +
			(bit(dvs.special) << 0_bi);
		auto const y = bounded::min(dvs.special.value(), 3_bi);
		return (5_bi * x + y) / 2_bi + 31_bi;
	}
	
	static constexpr auto calculate_power(Generation const generation, IVs const ivs) -> Power {
		auto transform = [](auto const index, IV const iv) { return ((iv.value() / 2_bi) % 2_bi) << index; };
		return BOUNDED_CONDITIONAL(generation >= Generation::six,
			60_bi,
			sum_stats(ivs, transform) * 40_bi / 63_bi + 30_bi
		);
	}
	
	static constexpr auto calculate_type(DVs const dvs) -> Type {
		auto get_low_bit = [](auto const index, DV const dv) { return (dv.value() >> index) % 2_bi; };
		return calculate_type_impl(
			(get_low_bit(1_bi, dvs.attack) << 3_bi) +
			(get_low_bit(0_bi, dvs.attack) << 2_bi) +
			(get_low_bit(1_bi, dvs.defense) << 1_bi) +
			(get_low_bit(0_bi, dvs.defense) << 0_bi)
		);
	}

	static constexpr auto calculate_type(IVs const ivs) -> Type {
		auto transform = [](auto const index, IV const iv) { return (iv.value() % 2_bi) << index; };
		return calculate_type_impl(sum_stats(ivs, transform) * 15_bi / 63_bi);
	}

	static constexpr auto calculate_type_impl(bounded::integer<0, 15> const index) -> Type {
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
			default: bounded::unreachable();
		}
	}

	Power m_power;
	Type m_type;
};

}	// namespace technicalmachine
