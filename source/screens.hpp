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

#include "operators.hpp"
#include "screen.hpp"

namespace technicalmachine {

struct Screens {
	constexpr auto decrement() {
		m_light_screen.decrement();
		m_reflect.decrement();
		m_lucky_chant.decrement();
		m_mist.decrement();
		m_safeguard.decrement();
		m_tailwind.decrement();
	}

	constexpr auto light_screen() const noexcept {
		return m_light_screen;
	}
	constexpr auto reflect() const noexcept {
		return m_reflect;
	}
	constexpr auto lucky_chant() const noexcept {
		return m_lucky_chant;
	}
	constexpr auto mist() const noexcept {
		return m_mist;
	}
	constexpr auto safeguard() const noexcept {
		return m_safeguard;
	}
	constexpr auto tailwind() const noexcept {
		return m_tailwind;
	}

	constexpr auto activate_light_screen(bool is_extended = false) {
		m_light_screen.activate(is_extended);
	}

	constexpr auto activate_reflect(bool is_extended = false) {
		m_reflect.activate(is_extended);
	}

	constexpr auto activate_lucky_chant() {
		m_lucky_chant.activate();
	}

	constexpr auto activate_mist() {
		m_mist.activate();
	}

	constexpr auto activate_safeguard() {
		m_safeguard.activate();
	}

	constexpr auto activate_tailwind() {
		m_tailwind.activate();
	}

	constexpr auto shatter() {
		m_light_screen = LightScreenEffect{};
		m_reflect = ReflectEffect{};
	}

private:
	LightScreenEffect m_light_screen;
	ReflectEffect m_reflect;
	LuckyChantEffect m_lucky_chant;
	MistEffect m_mist;
	SafeguardEffect m_safeguard;
	TailwindEffect m_tailwind;
};

constexpr auto compare(Screens const lhs, Screens const rhs) {
	auto as_tuple = [](auto const value) {
		return containers::make_tuple(
			value.light_screen(),
			value.reflect(),
			value.lucky_chant(),
			value.mist(),
			value.safeguard(),
			value.tailwind()
		);
	};
	return compare(as_tuple(lhs), as_tuple(rhs));
}


}	// namespace technicalmachine
