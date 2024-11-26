// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.end_of_turn_counter;

import tm.compress;
import tm.exists_if;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export enum class Resettable { no, yes };

export template<bool is_supported_this_generation, int max_turns, Resettable resettable = Resettable::yes>
struct EndOfTurnCounter {
	constexpr auto is_active() const -> bool {
		if constexpr (is_supported_this_generation) {
			return static_cast<bool>(m_turns_active);
		} else {
			return false;
		}
	}
	constexpr auto activate() & -> void {
		if constexpr (is_supported_this_generation) {
			if (resettable == Resettable::no and m_turns_active) {
				return;
			}
			m_turns_active = 0_bi;
		}
	}
	constexpr auto advance_one_turn() & -> void {
		if constexpr (is_supported_this_generation) {
			if (m_turns_active and *m_turns_active != bounded::constant<max_turns>) {
				++*m_turns_active;
			} else {
				m_turns_active = tv::none;
			}
		}
	}
	friend auto operator==(EndOfTurnCounter, EndOfTurnCounter) -> bool = default;
	friend constexpr auto compress(EndOfTurnCounter const value) {
		return compress(value.m_turns_active);
	}

private:
	using Counter = tv::optional<bounded::integer<0, max_turns>>;
	[[no_unique_address]] ExistsIf<Counter, is_supported_this_generation> m_turns_active{tv::none};
};

namespace {

using Example = EndOfTurnCounter<true, 2>;

static_assert(!Example().is_active());

constexpr auto example_is_active_after(auto const turns) {
	auto example = Example();
	example.activate();
	for (auto const _ : containers::integer_range(turns)) {
		example.advance_one_turn();
	}
	return example.is_active();
}

static_assert(example_is_active_after(0_bi));
static_assert(example_is_active_after(1_bi));
static_assert(example_is_active_after(2_bi));
static_assert(!example_is_active_after(3_bi));

} // namespace
} // namespace technicalmachine
