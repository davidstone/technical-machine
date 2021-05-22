// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/hidden_power.hpp>
#include <tm/stat/hidden_power_ivs.hpp>

#include <containers/integer_range.hpp>

namespace technicalmachine {
namespace {

template<Generation generation>
constexpr void test_impl() {
	for (auto const type : containers::enum_range<Type>()) {
		switch (type) {
			case Type::Fairy:
			case Type::Normal:
			case Type::Typeless:
				continue;
			default:
				break;
		}
		for (auto const has_physical_move : { false, true }) {
			auto const hidden_power = HiddenPower<generation>(hidden_power_ivs<generation>(type, has_physical_move));
			BOUNDED_ASSERT(hidden_power.power() == 70_bi);
			BOUNDED_ASSERT(hidden_power.type() == type);
		}
	}
}

constexpr bool test() {
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

	test_impl<Generation::two>();
	test_impl<Generation::three>();

	return true;
}

static_assert(test());

} // namespace
} // namespace technicalmachine