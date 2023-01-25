// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.entry_hazards;

import tm.compress;
import tm.generation;
import tm.exists_if;
import tm.saturating_add;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

// Using an entry hazard puts them down on the opponent's side of the field.

export template<Generation generation>
struct EntryHazards {
	constexpr auto spikes() const {
		return m_spikes;
	}
	constexpr auto stealth_rock() const -> bool {
		return m_stealth_rock;
	}
	constexpr auto toxic_spikes() const {
		return m_toxic_spikes;
	}
	constexpr auto add_spikes() & {
		saturating_increment(m_spikes);
	}
	constexpr auto add_toxic_spikes() & {
		saturating_increment(m_toxic_spikes);
	}
	constexpr auto clear_toxic_spikes() & {
		m_toxic_spikes = 0_bi;
	}
	constexpr auto add_stealth_rock() & {
		m_stealth_rock = true;
	}

	friend auto operator==(EntryHazards, EntryHazards) -> bool = default;

private:
	static constexpr bool spikes_exist = generation >= Generation::two;
	static constexpr auto max_spikes = generation == Generation::two ? 1 : 3;
	static constexpr bool toxic_spikes_exist = generation >= Generation::four;
	static constexpr bool stealth_rock_exists = generation >= Generation::four;
	[[no_unique_address]] IntegerIf<bounded::integer<0, max_spikes>, spikes_exist> m_spikes = 0_bi;
	[[no_unique_address]] IntegerIf<bounded::integer<0, 2>, toxic_spikes_exist> m_toxic_spikes = 0_bi;
	[[no_unique_address]] BoolIf<stealth_rock_exists, struct stealth_rock> m_stealth_rock;
};

export template<Generation generation>
constexpr auto compress(EntryHazards<generation> const value) {
	return compress_combine(value.spikes(), value.stealth_rock(), value.toxic_spikes());
}

} // namespace technicalmachine
