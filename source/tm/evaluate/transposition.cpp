// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.transposition;

import tm.evaluate.compressed_battle;
import tm.evaluate.depth;
import tm.evaluate.scored_selection;
import tm.evaluate.state;

import tm.move.move_name;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto table_size = bounded::constant<1 << 13>;
using TableIndex = bounded::integer<0, bounded::normalize<table_size - 1_bi>>;

template<bounded::bounded_integer Output>
constexpr auto update_hash(Output & output, bounded::bounded_integer auto input) -> void {
	while (input != 0_bi) {
		auto const temp = input % table_size;
		input /= table_size;
		output = bounded::assume_in_range<Output>(output.value() xor temp.value());
	}
}

export template<Generation generation>
struct TranspositionTable {
	auto add_score(State<generation> const & state, Depth const depth, ScoredSelections selections) -> void {
		auto const compressed_battle = compress_battle(state);
		auto _ = std::scoped_lock(m_mutex);
		auto & value = (*m_data)[index(compressed_battle)];
		value.compressed_battle = compressed_battle;
		value.depth = depth;
		value.selections = selections;
	}

	auto get_score(State<generation> const & state, Depth const depth) const -> tv::optional<ScoredSelections> {
		auto const compressed_battle = compress_battle(state);
		auto _ = std::scoped_lock(m_mutex);
		auto const & value = (*m_data)[index(compressed_battle)];
		if (value.depth >= depth and value.compressed_battle == compressed_battle) {
			return value.selections;
		}
		return tv::none;
	}

private:
	template<std::size_t... indexes>
	static constexpr auto hash(CompressedBattle<generation> const & compressed_battle, std::index_sequence<indexes...>) -> TableIndex {
		auto result = TableIndex(0_bi);
		(..., update_hash(result, compressed_battle[bounded::constant<indexes>]));
		return result;
	}
	static constexpr auto index(CompressedBattle<generation> const & compressed_battle) -> TableIndex {
		return hash(
			compressed_battle,
			bounded::make_index_sequence(tv::tuple_size<CompressedBattle<generation>>)
		);
	}

	struct Value {
		CompressedBattle<generation> compressed_battle = {};
		Depth depth = Depth(0_bi, 0_bi);
		ScoredSelections selections;
	};
	using Data = containers::array<Value, table_size>;
	static_assert(std::same_as<TableIndex, containers::index_type<Data>>);

	std::unique_ptr<Data> m_data = std::make_unique<Data>();
	mutable std::mutex m_mutex;
};

} // namespace technicalmachine