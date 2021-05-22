// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/iv.hpp>

#include <tm/type/type.hpp>

#include <tm/generation.hpp>

#include <bounded/detail/conditional.hpp>
#include <bounded/optional.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace detail {

// These attempt to choose an optimal spread. Some other spreads would do better
// on particular Pokemon.

// http://www.psypokes.com/gsc/dvguide.php
constexpr auto hidden_power_dvs(bounded::optional<Type> const type, bool const has_physical_move) {
	auto const odd_odd = DV(BOUNDED_CONDITIONAL(has_physical_move, 15_bi, 11_bi));
	auto const odd_even = DV(BOUNDED_CONDITIONAL(has_physical_move, 14_bi, 10_bi));
	auto const even_odd = DV(BOUNDED_CONDITIONAL(has_physical_move, 13_bi, 9_bi));
	auto const even_even = DV(BOUNDED_CONDITIONAL(has_physical_move, 12_bi, 8_bi));
	if (!type) {
		constexpr auto min_with_max_hp = DV(1_bi);
		constexpr auto max = DV(15_bi);
		return DVs(
			has_physical_move ? max : min_with_max_hp, // Atk
			max, // Def
			max, // Spe
			max // Spc
		);
	}
	switch (*type) {
		case Type::Bug: return DVs(
			even_odd, // Atk
			DV(13_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Dark: return DVs(
			odd_odd, // Atk
			DV(15_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Dragon: return DVs(
			odd_odd, // Atk
			DV(14_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Electric: return DVs(
			odd_even, // Atk
			DV(15_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Fairy: throw std::runtime_error("No Hidden Power Fairy");
		case Type::Fighting: return DVs(
			even_even, // Atk
			DV(12_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Fire: return DVs(
			odd_even, // Atk
			DV(12_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Flying: return DVs(
			even_even, // Atk
			DV(13_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Ghost: return DVs(
			even_odd, // Atk
			DV(14_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Grass: return DVs(
			odd_even, // Atk
			DV(14_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Ground: return DVs(
			even_even, // Atk
			DV(15_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Ice: return DVs(
			odd_odd, // Atk
			DV(13_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Normal: throw std::runtime_error("No Hidden Power Normal");
		case Type::Poison: return DVs(
			even_even, // Atk
			DV(14_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Psychic: return DVs(
			odd_odd, // Atk
			DV(12_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Rock: return DVs(
			even_odd, // Atk
			DV(12_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Steel: return DVs(
			even_odd, // Atk
			DV(15_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Water: return DVs(
			odd_even, // Atk
			DV(13_bi), // Def
			DV(15_bi), // Spe
			DV(15_bi) // Spc
		);
		case Type::Typeless: throw std::runtime_error("No Hidden Power Typeless");
	}
}

// https://www.smogon.com/forums/threads/hidden-power-iv-combinations.78083/
constexpr auto hidden_power_ivs(bounded::optional<Type> const type, bool const has_physical_move) {
	constexpr auto min_even = IV(2_bi);
	constexpr auto max_even = IV(30_bi);
	constexpr auto min_odd = IV(3_bi);
	constexpr auto max_odd = IV(31_bi);
	auto const even = has_physical_move ? max_even : min_even;
	auto const odd = has_physical_move ? max_odd : min_odd;
	if (!type) {
		constexpr auto min = IV(numeric_traits::min_value<IV::value_type>);
		constexpr auto max = IV(numeric_traits::max_value<IV::value_type>);
		return IVs(
			max, // HP
			has_physical_move ? max : min, // Atk
			max, // Def
			max, // SpA
			max, // SpD
			max // Spe
		);
	}
	switch (*type) {
		case Type::Bug: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(30_bi), // Def
			IV(31_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Dark: return IVs(
			IV(31_bi), // HP
			odd, // Atk
			IV(31_bi), // Def
			IV(31_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Dragon: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(31_bi), // Def
			IV(31_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Electric: return IVs(
			IV(31_bi), // HP
			odd, // Atk
			IV(31_bi), // Def
			IV(30_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Fairy: throw std::runtime_error("No Hidden Power Fairy");
		case Type::Fighting: return IVs(
			IV(31_bi), // HP
			odd, // Atk
			IV(30_bi), // Def
			IV(30_bi), // SpA
			IV(30_bi), // SpD
			IV(30_bi) // Spe
		);
		case Type::Fire: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(31_bi), // Def
			IV(30_bi), // SpA
			IV(31_bi), // SpD
			IV(30_bi) // Spe
		);
		case Type::Flying: return IVs(
			IV(30_bi), // HP
			even, // Atk
			IV(30_bi), // Def
			IV(30_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Ghost: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(31_bi), // Def
			IV(31_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Grass: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(31_bi), // Def
			IV(30_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Ground: return IVs(
			IV(31_bi), // HP
			BOUNDED_CONDITIONAL(has_physical_move, max_odd, min_even), // Atk
			IV(31_bi), // Def
			IV(30_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Ice: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(30_bi), // Def
			IV(31_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Normal: throw std::runtime_error("No Hidden Power Normal");
		case Type::Poison: return IVs(
			IV(31_bi), // HP
			odd, // Atk
			IV(30_bi), // Def
			IV(30_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Psychic: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(31_bi), // Def
			IV(31_bi), // SpA
			IV(31_bi), // SpD
			IV(30_bi) // Spe
		);
		case Type::Rock: return IVs(
			IV(31_bi), // HP
			BOUNDED_CONDITIONAL(has_physical_move, max_odd, min_even), // Atk
			IV(30_bi), // Def
			IV(31_bi), // SpA
			IV(30_bi), // SpD
			IV(30_bi) // Spe
		);
		case Type::Steel: return IVs(
			IV(31_bi), // HP
			odd, // Atk
			IV(31_bi), // Def
			IV(31_bi), // SpA
			IV(30_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Water: return IVs(
			IV(31_bi), // HP
			even, // Atk
			IV(30_bi), // Def
			IV(30_bi), // SpA
			IV(31_bi), // SpD
			IV(31_bi) // Spe
		);
		case Type::Typeless: throw std::runtime_error("No Hidden Power Typeless");
	}
}

} // namespace detail

template<Generation generation>
constexpr auto hidden_power_ivs(bounded::optional<Type> const type, bool const has_physical_move) {
	if constexpr (generation <= Generation::two) {
		return detail::hidden_power_dvs(type, has_physical_move);
	} else {
		return detail::hidden_power_ivs(type, has_physical_move);
	}
}

}	// namespace technicalmachine
