// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>

export module tm.visible_hp;

import tm.stat.allowed_hp;
import tm.stat.current_hp;
import tm.stat.max_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

export struct CurrentVisibleHP {
	constexpr explicit CurrentVisibleHP(CurrentHP const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(CurrentVisibleHP, CurrentVisibleHP) = default;

private:
	CurrentHP m_value;
};

export struct MaxVisibleHP {
	constexpr explicit MaxVisibleHP(MaxHP const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(MaxVisibleHP, MaxVisibleHP) = default;

private:
	MaxHP m_value;
};

export constexpr auto operator<=>(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() <=> rhs.value();
}
export constexpr auto operator==(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() == rhs.value();
}

// For the AI's Pokemon, this is exact. For the foe's Pokemon, this is a rounded
// fraction out of 42 or 100.
export struct VisibleHP {
	CurrentVisibleHP current;
	MaxVisibleHP max;
};

export auto to_real_hp(MaxHP const max_hp, VisibleHP const visible_hp) -> AllowedHP {
	if (visible_hp.current == CurrentVisibleHP(0_bi)) {
		return AllowedHP(0_bi);
	}
	constexpr auto expected_max_visible_hp = MaxVisibleHP(100_bi);
	if (expected_max_visible_hp != visible_hp.max) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Received an invalid max HP. Expected: "sv,
			containers::to_string(expected_max_visible_hp.value()),
			" but got "sv,
			containers::to_string(visible_hp.max.value())
		));
	}
	auto compute_value = [=](auto const visible_current) {
		return bounded::assume_in_range<CurrentHP>(max_hp * visible_current / visible_hp.max.value());
	};
	// TODO: Put in correct bounds on this for the foe Pokemon
	return AllowedHP(
		compute_value(visible_hp.current.value() - 1_bi),
		compute_value(visible_hp.current.value()),
		compute_value(bounded::min(visible_hp.current.value() + 1_bi, visible_hp.max.value()))
	);
}

} // namespace technicalmachine
