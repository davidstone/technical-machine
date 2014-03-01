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

auto Screens::light_screen() const -> ReflectLightScreen const & {
	return m_light_screen;
}

auto Screens::reflect() const -> ReflectLightScreen const & {
	return m_reflect;
}

auto Screens::lucky_chant() const -> Screen const & {
	return m_lucky_chant;
}

auto Screens::mist() const -> Screen const & {
	return m_mist;
}

auto Screens::safeguard() const -> Screen const & {
	return m_safeguard;
}

auto Screens::tailwind() const -> Tailwind const & {
	return m_tailwind;
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
	m_light_screen.clear();
	m_reflect.clear();
}

auto Screens::hash() const -> hash_type {
	return light_screen().hash() + light_screen().max_hash() *
			(reflect().hash() + reflect().max_hash() *
			(lucky_chant().hash() + lucky_chant().max_hash() *
			(mist().hash() + mist().max_hash() *
			(safeguard().hash() + safeguard().max_hash() *
			(tailwind().hash() + tailwind().max_hash())))));
}

auto Screens::max_hash() -> hash_type {
	return ReflectLightScreen::max_hash() * ReflectLightScreen::max_hash() * Screen::max_hash() * Screen::max_hash() * Screen::max_hash() * Tailwind::max_hash();
}

auto operator==(Screens const & lhs, Screens const & rhs) -> bool {
	return lhs.light_screen() == rhs.light_screen() and
			lhs.reflect() == rhs.reflect() and
			lhs.lucky_chant() == rhs.lucky_chant() and
			lhs.mist() == rhs.mist() and
			lhs.safeguard() == rhs.safeguard() and
			lhs.tailwind() == rhs.tailwind();
}

auto operator!=(Screens const & lhs, Screens const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
