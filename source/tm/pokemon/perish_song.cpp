// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.perish_song;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
using PerishSong = EndOfTurnCounter<
	generation >= Generation::two,
	3,
	Resettable::no
>;

static_assert(!PerishSong<Generation::three>().is_active());

constexpr auto perish_song_is_active_after(auto const turns) {
	auto perish_song = PerishSong<Generation::three>();
	perish_song.activate();
	for ([[maybe_unused]] auto const n : containers::integer_range(turns)) {
		perish_song.advance_one_turn();
	}
	return perish_song.is_active();
}

static_assert(perish_song_is_active_after(0_bi));
static_assert(perish_song_is_active_after(1_bi));
static_assert(perish_song_is_active_after(2_bi));
static_assert(perish_song_is_active_after(3_bi));
static_assert(!perish_song_is_active_after(4_bi));

static_assert([]{
	auto perish_song = PerishSong<Generation::three>();
	perish_song.activate();
	perish_song.advance_one_turn();
	perish_song.activate();
	for ([[maybe_unused]] auto const n : containers::integer_range(3_bi)) {
		perish_song.advance_one_turn();
	}
	return !perish_song.is_active();
}());

} // namespace technicalmachine
