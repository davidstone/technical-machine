// Type information
// Copyright (C) 2014 David Stone
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

#ifndef TYPE__TYPE_HPP_
#define TYPE__TYPE_HPP_

#include "../status.hpp"
#include "../move/moves_forward.hpp"

namespace technicalmachine {
class Effectiveness;
class Pokemon;
class Weather;

enum class Type : uint8_t {
	Bug,
	Dark,
	Dragon,
	Electric,
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

auto get_type(Moves move, Pokemon const & pokemon) -> Type;

constexpr auto is_boosted_by_flash_fire(Type const type) {
	return type == Type::Fire;
}
constexpr auto is_immune_to_hail(Type const type) {
	return type == Type::Ice;
}
inline auto is_immune_to_sandstorm(Type const type) {
	switch (type) {
		case Type::Ground:
		case Type::Rock:
		case Type::Steel:
			return true;
		default:
			return false;
	}
}


auto is_strengthened_by_weather(Type type, Weather weather) -> bool;
auto is_weakened_by_weather(Type type, Weather weather) -> bool;

template<Statuses status>
constexpr auto blocks_status(Type) {
	return false;
}
template<>
constexpr auto blocks_status<Statuses::burn>(Type const type) {
	return type == Type::Fire;
}
template<>
constexpr auto blocks_status<Statuses::freeze>(Type const type) {
	return type == Type::Ice;
}
template<>
constexpr auto blocks_status<Statuses::poison>(Type const type) {
	return type == Type::Poison or type == Type::Steel;
}
template<>
constexpr auto blocks_status<Statuses::poison_toxic>(Type const type) {
	return blocks_status<Statuses::poison>(type);
}


}	// namespace technicalmachine
#endif	// TYPE__TYPE_HPP_
