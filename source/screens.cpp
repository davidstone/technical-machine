// Copyright (C) 2015 David Stone
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

#include "screens.hpp"

namespace technicalmachine {

auto Screens::decrement() -> void {
	m_light_screen.decrement();
	m_reflect.decrement();
	m_lucky_chant.decrement();
	m_mist.decrement();
	m_safeguard.decrement();
	m_tailwind.decrement();
}

auto Screens::activate_light_screen(bool is_extended) -> void {
	m_light_screen.activate(is_extended);
}

auto Screens::activate_reflect(bool is_extended) -> void {
	m_reflect.activate(is_extended);
}

auto Screens::activate_lucky_chant() -> void {
	m_lucky_chant.activate();
}

auto Screens::activate_mist() -> void {
	m_mist.activate();
}

auto Screens::activate_safeguard() -> void {
	m_safeguard.activate();
}

auto Screens::activate_tailwind() -> void {
	m_tailwind.activate();
}

auto Screens::shatter() -> void {
	m_light_screen = LightScreenEffect{};
	m_reflect = ReflectEffect{};
}

auto operator==(Screens const & lhs, Screens const & rhs) -> bool {
	return lhs.light_screen() == rhs.light_screen() and
			lhs.reflect() == rhs.reflect() and
			lhs.lucky_chant() == rhs.lucky_chant() and
			lhs.mist() == rhs.mist() and
			lhs.safeguard() == rhs.safeguard() and
			lhs.tailwind() == rhs.tailwind();
}

}	// namespace technicalmachine
