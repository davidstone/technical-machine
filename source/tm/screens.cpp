// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.screens;

import tm.compress;
import tm.generation;
import tm.screen;

namespace technicalmachine {

export template<Generation generation>
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

	friend auto operator==(Screens, Screens) -> bool = default;
	friend constexpr auto compress(Screens<generation> const value) {
		return compress_combine(
			value.light_screen(),
			value.reflect(),
			value.lucky_chant(),
			value.mist(),
			value.safeguard(),
			value.tailwind()
		);
	}

private:
	LightScreen m_light_screen;
	Reflect m_reflect;
	[[no_unique_address]] LuckyChant<generation> m_lucky_chant;
	Mist m_mist;
	[[no_unique_address]] Safeguard<generation> m_safeguard;
	[[no_unique_address]] Tailwind<generation> m_tailwind;
};

} // namespace technicalmachine
