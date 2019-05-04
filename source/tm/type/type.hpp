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

#include <tm/enum.hpp>
#include <tm/status.hpp>

namespace technicalmachine {
enum class Generation;
enum class Moves : std::uint16_t;
struct Weather;

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

auto get_type(Generation generation, Moves move, Type hidden_power) -> Type;

constexpr auto is_boosted_by_flash_fire(Type const type) {
	return type == Type::Fire;
}
constexpr auto is_immune_to_hail(Type const type) {
	return type == Type::Ice;
}
constexpr auto is_immune_to_sandstorm(Type const type) {
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

constexpr auto blocks_status(Type const type, Statuses const status) {
	switch (status) {
		case Statuses::burn:
			return type == Type::Fire;
		case Statuses::freeze:
			return type == Type::Ice;
		case Statuses::poison:
		case Statuses::toxic:
			return type == Type::Poison or type == Type::Steel;
		default:
			return false;
	}
}

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Type> : technicalmachine::enum_numeric_limits<technicalmachine::Type::Typeless> {};

}	// namespace std
