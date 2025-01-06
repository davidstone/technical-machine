// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.hidden_power;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.stat.iv;
import tm.stat.stat_style;

import tm.type.type;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export using HiddenPowerDVPower = bounded::integer<31, 70>;
export using HiddenPowerIVPower = bounded::integer<30, 70>;

constexpr auto sum_stats(IVs const ivs, auto const transform) {
	return
		transform(0_bi, ivs.hp()) +
		transform(1_bi, ivs.atk()) +
		transform(2_bi, ivs.def()) +
		transform(3_bi, ivs.spe()) +
		transform(4_bi, ivs.spa()) +
		transform(5_bi, ivs.spd());
}

constexpr auto calculate_type_impl(bounded::integer<0, 15> const index) -> Type {
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
		default: std::unreachable();
	}
}

constexpr auto calculate_type(DVs const dvs) -> Type {
	constexpr auto get_base_four_digit = [](DV const dv) { return dv.value() % 4_bi; };
	return calculate_type_impl(
		(get_base_four_digit(dvs.atk()) * 4_bi) +
		(get_base_four_digit(dvs.def()) * 1_bi)
	);
}

constexpr auto calculate_type(IVs const ivs) -> Type {
	auto transform = [](auto const index, IV const iv) { return (iv.value() % 2_bi) << index; };
	return calculate_type_impl(sum_stats(ivs, transform) * 15_bi / 63_bi);
}

constexpr auto is_valid_type(Type const type) -> bool {
	switch (type) {
		case Type::Bug:
		case Type::Dark:
		case Type::Dragon:
		case Type::Electric:
		case Type::Fighting:
		case Type::Fire:
		case Type::Flying:
		case Type::Ghost:
		case Type::Grass:
		case Type::Ground:
		case Type::Ice:
		case Type::Poison:
		case Type::Psychic:
		case Type::Rock:
		case Type::Steel:
		case Type::Water:
			return true;
		case Type::Normal:
		case Type::Fairy:
		case Type::Typeless:
			return false;
	}
}

// http://www.psypokes.com/gsc/dvguide.php
constexpr auto calculate_power(DVs const dvs) {
	auto const bit = [](DV const dv) { return BOUNDED_CONDITIONAL(dv.value() < 8_bi, 0_bi, 1_bi); };
	auto const x =
		(bit(dvs.atk()) << 3_bi) +
		(bit(dvs.def()) << 2_bi) +
		(bit(dvs.spe()) << 1_bi) +
		(bit(dvs.spc()) << 0_bi);
	auto const y = bounded::min(dvs.spc().value(), 3_bi);
	return (5_bi * x + y) / 2_bi + 31_bi;
}

constexpr auto calculate_power(IVs const ivs) {
	auto transform = [](auto const index, IV const iv) { return ((iv.value() / 2_bi) % 2_bi) << index; };
	return sum_stats(ivs, transform) * 40_bi / 63_bi + 30_bi;
}

constexpr auto dv_power_generation(Generation const generation) -> bool {
	return generation == Generation::two;
}
constexpr auto iv_power_generation(Generation const generation) -> bool {
	switch (generation) {
		case Generation::three:
		case Generation::four:
		case Generation::five:
			return true;
		default:
			return false;
	}
}
constexpr auto fixed_power_generation(Generation const generation) -> bool {
	switch (generation) {
		case Generation::one:
		case Generation::two:
		case Generation::three:
		case Generation::four:
		case Generation::five:
			return false;
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return true;
	}
}

template<Generation generation>
using HiddenPowerPower =
	std::conditional_t<dv_power_generation(generation), HiddenPowerDVPower,
	std::conditional_t<iv_power_generation(generation), HiddenPowerIVPower,
	std::conditional_t<fixed_power_generation(generation), bounded::constant_t<60>,
	bounded::constant_t<0>
>>>;

template<Generation generation>
constexpr auto is_valid_power(HiddenPowerPower<generation> const power) {
	if constexpr (generation <= Generation::two) {
		auto const remainder = power % 5_bi;
		return remainder != 3_bi and remainder != 4_bi;
	} else {
		return true;
	}
}

export template<Generation generation>
struct HiddenPower {
	using Power = HiddenPowerPower<generation>;

	constexpr HiddenPower(Power const power, Type const type):
		m_power(power),
		m_type(type)
	{
		BOUNDED_ASSERT(is_valid_type(type));
		BOUNDED_ASSERT(is_valid_power<generation>(power));
	}
	constexpr explicit HiddenPower(DVs const dvs) requires(dv_power_generation(generation)):
		m_power(calculate_power(dvs)),
		m_type(calculate_type(dvs))
	{
	}
	constexpr explicit HiddenPower(IVs const ivs) requires(iv_power_generation(generation)):
		m_power(calculate_power(ivs)),
		m_type(calculate_type(ivs))
	{
	}
	constexpr explicit HiddenPower(IVs const ivs) requires(fixed_power_generation(generation)):
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
	constexpr HiddenPower(bounded::tombstone_tag, auto const make):
		m_power(make()),
		m_type(Type::Typeless)
	{
	}

	[[no_unique_address]] Power m_power;
	Type m_type;

	friend bounded::tombstone<HiddenPower<generation>>;
	friend bounded::tombstone_member<&HiddenPower<generation>::m_power>;
};

export template<Generation generation>
constexpr auto calculate_hidden_power(
	DVsOrIVs<special_input_style_for(generation)> dvs_or_ivs,
	RegularMoves const moves
) -> tv::optional<HiddenPower<generation>> {
	if constexpr (generation == Generation::one) {
		return tv::none;
	} else if (containers::any(moves, [](Move const move) { return move.name() == MoveName::Hidden_Power; })) {
		return HiddenPower<generation>(dvs_or_ivs);
	} else {
		return tv::none;
	}
}

} // namespace technicalmachine

template<technicalmachine::Generation generation>
struct bounded::tombstone<technicalmachine::HiddenPower<generation>> : bounded::tombstone_member<&technicalmachine::HiddenPower<generation>::m_power> {
};

namespace technicalmachine {
namespace {

constexpr bool test() {
	{
		constexpr auto hp = HiddenPower<Generation::two>(DVs(
			DV(10_bi),
			DV(13_bi),
			DV(15_bi),
			DV(15_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Water);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Bug);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Dark);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Dark);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Dragon);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Electric);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Fire);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(30_bi),
			IV(30_bi),
			IV(30_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Flying);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Ghost);
	}
	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Ghost);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Grass);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Ground);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Ice);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Poison);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(30_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Psychic);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi),
			IV(30_bi),
			IV(30_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Rock);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(30_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Steel);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(31_bi),
			IV(30_bi),
			IV(30_bi),
			IV(30_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Water);
	}

	{
		constexpr auto hp = HiddenPower<Generation::three>(IVs(
			IV(29_bi),
			IV(31_bi),
			IV(30_bi),
			IV(29_bi),
			IV(30_bi),
			IV(29_bi)
		));
		static_assert(hp.power() == 54_bi);
		static_assert(hp.type() == Type::Ghost);
	}

	{
		constexpr auto hp = HiddenPower<Generation::four>(IVs(
			IV(31_bi),
			IV(3_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		));
		static_assert(hp.power() == 70_bi);
		static_assert(hp.type() == Type::Dark);
	}

	return true;
}

static_assert(test());

} // namespace
} // namespace technicalmachine