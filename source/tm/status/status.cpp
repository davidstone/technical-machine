// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.status.status;

import tm.status.status_name;

import tm.ability;
import tm.compress;
import tm.generation;
import tm.probability;
import tm.saturating_add;

import bounded;
import numeric_traits;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

struct Clear {
	friend auto operator==(Clear, Clear) -> bool = default;
};
struct Burn {
	friend auto operator==(Burn, Burn) -> bool = default;
};
struct Freeze {
	friend auto operator==(Freeze, Freeze) -> bool = default;
};
struct Paralysis {
	friend auto operator==(Paralysis, Paralysis) -> bool = default;
};
struct Poison {
	friend auto operator==(Poison, Poison) -> bool = default;
};
struct Toxic {
	friend auto operator==(Toxic, Toxic) -> bool = default;
};
struct Sleep {
	bounded::integer<0, 4> turns_slept = 0_bi;
	friend auto operator==(Sleep, Sleep) -> bool = default;
	friend constexpr auto compress(Sleep const value) {
		return compress(value.turns_slept);
	}
};
struct Rest {
	bounded::integer<0, 2> turns_slept = 0_bi;
	friend auto operator==(Rest, Rest) -> bool = default;
	friend constexpr auto compress(Rest const value) {
		return compress(value.turns_slept);
	}
};

using SleepCounter = bounded::integer<0, 4>;

// It's possible to acquire Early Bird in the middle of a sleep
constexpr auto early_bird_probability(SleepCounter const turns_slept) -> Probability {
	switch (turns_slept.value()) {
		case 0: return Probability(1.0 / 4.0);
		case 1: return Probability(1.0 / 2.0);
		case 2: return Probability(2.0 / 3.0);
		default: return Probability(1.0); // case 3, 4
	}
}

constexpr auto non_early_bird_probability(
	Generation const generation,
	SleepCounter const turns_slept
) -> Probability {
	auto const adjusted_turns = BOUNDED_CONDITIONAL(generation == Generation::one, turns_slept - 1_bi, turns_slept);
	return (adjusted_turns == 0_bi) ?
		Probability(0.0) :
		Probability(1.0 / static_cast<double>(numeric_traits::max_value<SleepCounter> + 1_bi - adjusted_turns));
}

constexpr auto awaken_probability(
	Generation const generation,
	SleepCounter const turns_slept,
	Ability const ability
) -> Probability {
	return wakes_up_early(ability) ?
		early_bird_probability(turns_slept) :
		non_early_bird_probability(generation, turns_slept)
	;
}

// TODO: Make sure this is accurate with Early Bird
constexpr auto rest_awaken_probability(
	Generation const generation,
	bounded::integer<0, 2> const turns_slept
) -> Probability {
	auto const max_duration = BOUNDED_CONDITIONAL(generation == Generation::one, 1_bi, numeric_traits::max_value<decltype(turns_slept)>);
	return turns_slept == max_duration ? Probability(1.0) : Probability(0.0);
}

export struct Status {
	constexpr Status() = default;

	explicit constexpr Status(StatusName const status):
		m_state([=] {
			switch (status) {
				case StatusName::clear: return State(Clear{});
				case StatusName::burn: return State(Burn{});
				case StatusName::freeze: return State(Freeze{});
				case StatusName::paralysis: return State(Paralysis{});
				case StatusName::poison: return State(Poison{});
				case StatusName::toxic: return State(Toxic{});
				case StatusName::sleep: return State(Sleep{});
				case StatusName::rest: return State(Rest{});
			}
		}())
	{
	}

	constexpr auto name() const {
		return static_cast<StatusName>(m_state.index().integer());
	}

	constexpr auto operator=(StatusName const status) & -> Status & {
		*this = Status(status);
		return *this;
	}

	constexpr auto advance_from_move(Ability const ability, bool const clear) & -> void {
		if (clear) {
			m_state = Clear{};
		} else {
			auto advance_sleep = [&](auto & sleep) {
				saturating_add(sleep.turns_slept, BOUNDED_CONDITIONAL(wakes_up_early(ability), 2_bi, 1_bi));
			};
			tv::visit(m_state, tv::overload(
				[=](Sleep & sleep) { advance_sleep(sleep); },
				[=](Rest & sleep) { advance_sleep(sleep); },
				[](auto) {}
			));
		}
	}

	// If the current status is sleep or rest, returns the probability the
	// status can change from sleeping to awake on this move. If the current
	// status is freeze, returns the probability of thawing. Returns 0.0 if the
	// Pokemon is not asleep or frozen or if, due to the sleep counter, they
	// will definitely not awaken.
	constexpr auto probability_of_clearing(Generation const generation, Ability const ability) const -> Probability {
		return tv::visit(m_state, tv::overload(
			[=](Sleep const sleep) {
				return awaken_probability(generation, sleep.turns_slept, ability);
			},
			[=](Rest const sleep) {
				return rest_awaken_probability(generation, sleep.turns_slept);
			},
			[=](Freeze) {
				return generation == Generation::one ? Probability(0.0) : Probability(0.2);
			},
			[](auto) {
				return Probability(0.0);
			}
		));
	}

	friend auto operator==(Status, Status) -> bool = default;
	friend constexpr auto compress(Status const value) {
		return compress(value.m_state);
	}

private:
	using State = tv::variant<
		Clear,
		Burn,
		Freeze,
		Paralysis,
		Poison,
		Toxic,
		Sleep,
		Rest
	>;
	State m_state{Clear{}};
};


export constexpr auto is_clear(Status const status) {
	return status.name() == StatusName::clear;
}
export constexpr auto is_frozen(Status const status) {
	return status.name() == StatusName::freeze;
}
export constexpr auto is_sleeping(Status const status) {
	switch (status.name()) {
		case StatusName::rest:
		case StatusName::sleep:
			return true;
		default:
			return false;
	}
}

export constexpr auto boosts_smellingsalt(Status const status) {
	return status.name() == StatusName::paralysis;
}

export constexpr bool boosts_defense(Ability const ability, Status const status) {
	return ability == Ability::Marvel_Scale and !is_clear(status);
}

} // namespace technicalmachine
