// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.perish_song;

import tm.pokemon.any_pokemon;
import tm.pokemon.end_of_turn_counter;
import tm.pokemon.faint;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
struct PerishSong {
	constexpr auto activate() & -> void {
		if (!m_counter.is_active()) {
			m_counter.activate();
		}
	}
	constexpr auto advance_one_turn(any_mutable_active_pokemon auto const pokemon) & -> void {
		if (!m_counter.is_active()) {
			return;
		}
		m_counter.advance_one_turn();
		if (!m_counter.is_active()) {
			faint(pokemon);
		}
	}
	friend auto operator==(PerishSong, PerishSong) -> bool = default;
	friend constexpr auto compress(PerishSong const value) {
		return compress(value.m_counter);
	}
private:
	[[no_unique_address]] EndOfTurnCounter<generation >= Generation::two, 3> m_counter;
};

} // namespace technicalmachine
