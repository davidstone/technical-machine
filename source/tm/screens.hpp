// Copyright (C) 2018 David Stone
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

#include <tm/operators.hpp>
#include <tm/screen.hpp>

namespace technicalmachine {

struct Screens {
	constexpr auto decrement() & {
		m_light_screen.decrement();
		m_reflect.decrement();
		m_lucky_chant.decrement();
		m_mist.decrement();
		m_safeguard.decrement();
		m_tailwind.decrement();
	}

	constexpr auto light_screen() const {
		return m_light_screen;
	}
	constexpr auto reflect() const {
		return m_reflect;
	}
	constexpr auto lucky_chant() const {
		return m_lucky_chant;
	}
	constexpr auto mist() const {
		return m_mist;
	}
	constexpr auto safeguard() const {
		return m_safeguard;
	}
	constexpr auto tailwind() const {
		return m_tailwind;
	}

	constexpr auto activate_light_screen(bool is_extended = false) & {
		m_light_screen.activate(is_extended);
	}

	constexpr auto activate_reflect(bool is_extended = false) & {
		m_reflect.activate(is_extended);
	}

	constexpr auto activate_lucky_chant() & {
		m_lucky_chant.activate();
	}

	constexpr auto activate_mist() & {
		m_mist.activate();
	}

	constexpr auto activate_safeguard() & {
		m_safeguard.activate();
	}

	constexpr auto activate_tailwind() & {
		m_tailwind.activate();
	}

	constexpr auto shatter() & {
		m_light_screen = LightScreen();
		m_reflect = Reflect();
	}

	friend auto operator==(Screens const &, Screens const &) -> bool = default;
	
private:
	LightScreen m_light_screen;
	Reflect m_reflect;
	LuckyChant m_lucky_chant;
	Mist m_mist;
	Safeguard m_safeguard;
	Tailwind m_tailwind;
};

}	// namespace technicalmachine
