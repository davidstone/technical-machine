// Type information
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <cstdint>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;

enum class Type : std::uint8_t {
	Bug,
	Dark,
	Dragon,
	Electric,
	Fairy,
	Fighting,
	Fire,
	Flying,
	Ghost,
	Grass,
	Ground,
	Ice,
	Normal,
	Poison,
	Psychic,
	Rock,
	Steel,
	Water,
	Typeless
};

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::Type> = technicalmachine::Type();

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::Type> = technicalmachine::Type::Typeless;

namespace technicalmachine {

// If `move` is Hidden Power, type must not be `none`
auto get_type(Generation generation, Moves move, bounded::optional<Type> hidden_power) -> Type;

constexpr auto is_boosted_by_flash_fire(Type const type) {
	return type == Type::Fire;
}

} // namespace technicalmachine
