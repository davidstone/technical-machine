// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/status.hpp>

#include <tm/ability.hpp>
#include <tm/heal.hpp>
#include <tm/saturating_add.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <bounded/assert.hpp>
#include <bounded/detail/overload.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {
using namespace bounded::literal;

Status::Status(Statuses const status):
	m_state([=] {
		switch (status) {
			case Statuses::clear: return State(Clear{});
			case Statuses::burn: return State(Burn{});
			case Statuses::freeze: return State(Freeze{});
			case Statuses::paralysis: return State(Paralysis{});
			case Statuses::poison: return State(Poison{});
			case Statuses::toxic: return State(Toxic{});
			case Statuses::sleep: return State(Sleep{});
			case Statuses::rest: return State(Rest{});
		}
	}())
{
}

auto lowers_speed(Status const status, Ability const ability) -> bool {
	return status.name() == Statuses::paralysis and !blocks_paralysis_speed_penalty(ability);
}
auto boosts_facade(Status const status) -> bool {
	switch (status.name()) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::toxic:
			return true;
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::sleep:
		case Statuses::rest:
			return false;
	}
}

auto Status::advance_from_move(Ability const ability, bool const clear) & -> void {
	if (clear) {
		m_state = Clear{};
	} else {
		auto advance_sleep = [&](auto & sleep) {
			saturating_add(sleep.turns_slept, BOUNDED_CONDITIONAL(wakes_up_early(ability), 2_bi, 1_bi));
		};
		bounded::visit(m_state, bounded::overload(
			[=](Sleep & sleep) { advance_sleep(sleep); },
			[=](Rest & sleep) { advance_sleep(sleep); },
			[](auto) {}
		));
	}
}

namespace {

using SleepCounter = bounded::integer<0, 4>;

// It's possible to acquire Early Bird in the middle of a sleep
constexpr auto early_bird_probability(SleepCounter const turns_slept) {
	switch (turns_slept.value()) {
		case 0:
			return 1.0 / 4.0;
		case 1:
			return 1.0 / 2.0;
		case 2:
			return 2.0 / 3.0;
		default: // case 3, 4
			return 1.0;
	}
}

constexpr auto non_early_bird_probability(Generation const generation, SleepCounter const turns_slept) {
	auto const adjusted_turns = BOUNDED_CONDITIONAL(generation == Generation::one, turns_slept - 1_bi, turns_slept);
	return (adjusted_turns == 0_bi) ?
		0.0 :
		1.0 / static_cast<double>(numeric_traits::max_value<SleepCounter> + 1_bi - adjusted_turns);
}

constexpr auto awaken_probability(Generation const generation, SleepCounter const turns_slept, Ability const ability) {
	return wakes_up_early(ability) ?
		early_bird_probability(turns_slept) :
		non_early_bird_probability(generation, turns_slept)
	;
}

// TODO: Make sure this is accurate with Early Bird
constexpr auto rest_awaken_probability(Generation const generation, bounded::integer<0, 2> const turns_slept) {
	auto const max_duration = BOUNDED_CONDITIONAL(generation == Generation::one, 1_bi, numeric_traits::max_value<decltype(turns_slept)>);
	return turns_slept == max_duration ? 1.0 : 0.0;
}

} // namespace

auto Status::probability_of_clearing(Generation const generation, Ability const ability) const -> double {
	return bounded::visit(m_state, bounded::overload(
		[=](Sleep const sleep) { return awaken_probability(generation, sleep.turns_slept, ability); },
		[=](Rest const sleep) { return rest_awaken_probability(generation, sleep.turns_slept); },
		[=](Freeze) { return generation == Generation::one ? 0.0 : 0.2; },
		[](auto) { return 0.0; }
	));
}

bool blocks_status(Ability const ability, Ability const other_ability, Statuses const status, Weather const weather) {
	auto is_sunny = [&] {
		return weather.sun(weather_is_blocked_by_ability(ability, other_ability));
	};
	auto const ability_blocked = other_ability == Ability::Mold_Breaker;
	switch (status) {
		case Statuses::burn:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Leaf_Guard:
					return is_sunny();
				case Ability::Water_Veil:
					return true;
				default:
					return false;
			}
		case Statuses::freeze:
			return is_sunny() or (!ability_blocked and ability == Ability::Magma_Armor);
		case Statuses::paralysis:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Leaf_Guard:
					return is_sunny();
				case Ability::Limber:
					return true;
				default:
					return false;
			}
		case Statuses::poison:
		case Statuses::toxic:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Immunity:
					return true;
				case Ability::Leaf_Guard:
					return is_sunny();
				default:
					return false;
			}
		case Statuses::sleep:
		case Statuses::rest:
			if (ability_blocked) {
				return false;
			}
			switch (ability) {
				case Ability::Insomnia:
				case Ability::Vital_Spirit:
					return true;
				case Ability::Leaf_Guard:
					return is_sunny();
				default:
					return false;
			}
		case Statuses::clear:
			return false;
	}
}

} // namespace technicalmachine
