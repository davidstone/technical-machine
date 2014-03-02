// All Screens a team can set up
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

#ifndef SCREENS_HPP_
#define SCREENS_HPP_

#include <cstdint>
#include "screen.hpp"

namespace technicalmachine {

class Screens {
public:
	auto decrement() -> void;

	auto light_screen() const -> LightScreenEffect const &;
	auto reflect() const -> ReflectEffect const &;
	auto lucky_chant() const -> LuckyChantEffect const &;
	auto mist() const -> MistEffect const &;
	auto safeguard() const -> SafeguardEffect const &;
	auto tailwind() const -> TailwindEffect const &;

	auto activate_light_screen(bool is_extended = false) -> void;
	auto activate_reflect(bool is_extended = false) -> void;
	auto activate_lucky_chant() -> void;
	auto activate_mist() -> void;
	auto activate_safeguard() -> void;
	auto activate_tailwind() -> void;

	auto shatter() -> void;
	typedef uint64_t hash_type;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	LightScreenEffect m_light_screen;
	ReflectEffect m_reflect;
	LuckyChantEffect m_lucky_chant;
	MistEffect m_mist;
	SafeguardEffect m_safeguard;
	TailwindEffect m_tailwind;
};
auto operator==(Screens const & lhs, Screens const & rhs) -> bool;
auto operator!=(Screens const & lhs, Screens const & rhs) -> bool;

}	// namespace technicalmachine
#endif	// SCREENS_HPP_
