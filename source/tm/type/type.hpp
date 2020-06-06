// Type information
// Copyright (C) 2016 David Stone
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

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

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
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::Type> = technicalmachine::Type();

template<>
inline constexpr auto max_value<technicalmachine::Type> = technicalmachine::Type::Typeless;

}	// namespace bounded
namespace technicalmachine {

// If `move` is Hidden Power, type must not be `none`
auto get_type(Generation generation, Moves move, bounded::optional<Type> hidden_power) -> Type;

constexpr auto is_boosted_by_flash_fire(Type const type) {
	return type == Type::Fire;
}

}	// namespace technicalmachine
