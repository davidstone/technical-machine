// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/iv.hpp>

#include <tm/type/type.hpp>

#include <tm/generation.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/unreachable.hpp>

namespace technicalmachine {

template<Generation generation>
struct HiddenPower {
	using Power =
		std::conditional_t<generation <= Generation::one, bounded::constant_t<0>,
		std::conditional_t<generation <= Generation::two, bounded::integer<31, 70>,
		std::conditional_t<generation <= Generation::five, bounded::integer<30, 70>,
		bounded::constant_t<60>
	>>>;

	constexpr HiddenPower(DVs const dvs) requires(generation == Generation::two):
		m_power(calculate_power(dvs)),
		m_type(calculate_type(dvs))
	{
	}
	constexpr HiddenPower(IVs const ivs) requires(generation >= Generation::three):
		m_power(calculate_power(ivs)),
		m_type(calculate_type(ivs))
	{
	}
	
	constexpr auto power() const {
		return m_power;
	}
	constexpr auto type() const {
		return m_type;
	}
	
	friend auto operator==(HiddenPower, HiddenPower) -> bool = default;
private:
	friend bounded::tombstone_traits<HiddenPower>;

	constexpr HiddenPower(bounded::none_t, auto const index):
		m_power(bounded::tombstone_traits<Power>::make(index)),
		m_type(Type::Typeless)
	{
	}
	
	static constexpr auto sum_stats(IVs const ivs, auto const transform) {
		return
			transform(0_bi, ivs.hp) +
			transform(1_bi, ivs.atk) +
			transform(2_bi, ivs.def) +
			transform(3_bi, ivs.spe) +
			transform(4_bi, ivs.spa) +
			transform(5_bi, ivs.spd);
	}

	// http://www.psypokes.com/gsc/dvguide.php
	static constexpr auto calculate_power(DVs const dvs) -> Power {
		auto const bit = [](DV const dv) { return BOUNDED_CONDITIONAL(dv.value() < 8_bi, 0_bi, 1_bi); };
		auto const x =
			(bit(dvs.atk) << 3_bi) +
			(bit(dvs.def) << 2_bi) +
			(bit(dvs.spe) << 1_bi) +
			(bit(dvs.spc) << 0_bi);
		auto const y = bounded::min(dvs.spc.value(), 3_bi);
		return (5_bi * x + y) / 2_bi + 31_bi;
	}
	
	static constexpr auto calculate_power(IVs const ivs) -> Power {
		if constexpr (generation <= Generation::five) {
			auto transform = [](auto const index, IV const iv) { return ((iv.value() / 2_bi) % 2_bi) << index; };
			return sum_stats(ivs, transform) * 40_bi / 63_bi + 30_bi;
		} else {
			return 60_bi;
		}
	}
	
	static constexpr auto calculate_type(DVs const dvs) -> Type {
		constexpr auto get_base_four_digit = [](DV const dv) { return dv.value() % 4_bi; };
		return calculate_type_impl(
			(get_base_four_digit(dvs.atk) * 4_bi) +
			(get_base_four_digit(dvs.def) * 1_bi)
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

	[[no_unique_address]] Power m_power;
	Type m_type;
};

} // namespace technicalmachine
namespace bounded {

template<technicalmachine::Generation generation>
struct tombstone_traits<technicalmachine::HiddenPower<generation>> {
	using base = tombstone_traits<typename technicalmachine::HiddenPower<generation>::Power>;
	static constexpr auto spare_representations = base::spare_representations;

	static constexpr auto make(auto const index) noexcept {
		return technicalmachine::HiddenPower<generation>(bounded::none, index);
	}
	static constexpr auto index(technicalmachine::HiddenPower<generation> const value) {
		return base::index(value.power());
	}
};

} // namespace bounded