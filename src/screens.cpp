// All Screens a team can set up
// Copyright (C) 2012 David Stone
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

void Screens::decrement() {
	m_light_screen.decrement();
	m_reflect.decrement();
	m_lucky_chant.decrement();
	m_mist.decrement();
	m_safeguard.decrement();
	m_tailwind.decrement();
}

bool Screens::light_screen() const {
	return static_cast<bool>(m_light_screen);
}

bool Screens::reflect() const {
	return static_cast<bool>(m_reflect);
}

bool Screens::lucky_chant() const {
	return static_cast<bool>(m_lucky_chant);
}

bool Screens::mist() const {
	return static_cast<bool>(m_mist);
}

bool Screens::safeguard() const {
	return static_cast<bool>(m_safeguard);
}

bool Screens::tailwind() const {
	return static_cast<bool>(m_tailwind);
}

void Screens::activate_light_screen(bool is_extended) {
	m_light_screen.activate(is_extended);
}

void Screens::activate_reflect(bool is_extended) {
	m_reflect.activate(is_extended);
}

void Screens::activate_lucky_chant() {
	m_lucky_chant.activate();
}

void Screens::activate_mist() {
	m_mist.activate();
}

void Screens::activate_safeguard() {
	m_safeguard.activate();
}

void Screens::activate_tailwind() {
	m_tailwind.activate();
}

void Screens::shatter() {
	m_light_screen.clear();
	m_reflect.clear();
}

Screens::hash_type Screens::hash() const {
	return m_light_screen.hash() + m_light_screen.max_hash() *
			(m_reflect.hash() + m_reflect.max_hash() *
			(m_lucky_chant.hash() + m_lucky_chant.max_hash() *
			(m_mist.hash() + m_mist.max_hash() *
			(m_safeguard.hash() + m_safeguard.max_hash() *
			(m_tailwind.hash() + m_tailwind.max_hash())))));
}

Screens::hash_type Screens::max_hash() {
	return ReflectLightScreen::max_hash() * ReflectLightScreen::max_hash() * Screen::max_hash() * Screen::max_hash() * Screen::max_hash() * Tailwind::max_hash();
}

bool operator==(Screens const & lhs, Screens const & rhs) {
	return lhs.m_light_screen == rhs.m_light_screen and
			lhs.m_reflect == rhs.m_reflect and
			lhs.m_lucky_chant == rhs.m_lucky_chant and
			lhs.m_mist == rhs.m_mist and
			lhs.m_safeguard == rhs.m_safeguard and
			lhs.m_tailwind == rhs.m_tailwind;
}

bool operator!=(Screens const & lhs, Screens const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
