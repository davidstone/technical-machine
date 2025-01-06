// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.pokemon.pokemon;

import tm.move.initial_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.pp;
import tm.move.regular_moves;

import tm.pokemon.any_pokemon;
import tm.pokemon.happiness;
import tm.pokemon.hidden_power;
import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.status.status;
import tm.status.status_name;

import tm.ability;
import tm.compress;
import tm.exists_if;
import tm.gender;
import tm.generation;
import tm.held_item;
import tm.item;
import tm.rational;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct Pokemon {
	constexpr Pokemon(Species const species, Level const level, Gender const gender, Item const item, Ability const ability, CombinedStatsFor<generation> const stat_inputs, RegularMoves regular_moves_, Happiness const happiness = Happiness()):
		m_regular_moves(regular_moves_),
		m_stats(BaseStats(generation, species), level, stat_inputs),

		m_species(species),
		m_item(item),
		m_ability(ability),
		m_gender(gender),
		m_nature(stat_inputs.nature),

		m_level(level),

		m_happiness(happiness),

		m_hidden_power(calculate_hidden_power<generation>(stat_inputs.dvs_or_ivs, m_regular_moves)),
		
		m_has_been_seen(false)
	{
	}

	explicit constexpr Pokemon(SeenPokemon<generation> const & other):
		m_regular_moves(other.regular_moves()),
		m_stats(other.stats()),
		m_species(other.species()),
		m_item(other.actual_item()),
		m_ability(other.initial_ability()),
		m_gender(other.gender()),
		m_status(other.status()),
		m_nature(other.nature()),
		m_level(other.level()),
		m_happiness(252_bi),
		m_hidden_power(other.hidden_power()),
		m_has_been_seen(true)
	{
	}

	explicit constexpr Pokemon(InitialPokemon<special_style_for(generation)> const & other):
		Pokemon(
			other.species,
			other.level,
			other.gender,
			other.item,
			other.ability,
			other.stats,
			RegularMoves(containers::transform(other.moves, [](InitialMove const move) {
				return Move(generation, move.name, move.pp_ups);
			})),
			other.happiness
		)
	{
	}

	constexpr auto hp() const {
		return m_stats.hp();
	}
	constexpr auto stats() const -> Stats<stat_style_for(generation)> {
		return m_stats;
	}

	constexpr auto advance_status_from_move(Ability const ability, bool const clear_status) & {
		m_status.advance_from_move(ability, clear_status);
	}

	constexpr auto has_been_seen() const -> bool {
		return m_has_been_seen;
	}
	constexpr void mark_as_seen() & {
		m_has_been_seen = true;
	}

	constexpr auto regular_moves() const -> RegularMoves {
		return m_regular_moves;
	}

	constexpr void reduce_pp(MoveName const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & {
		auto const maybe_move = containers::maybe_find(m_regular_moves, move_name);
		if (!maybe_move) {
			return;
		}
		auto & move = *maybe_move;
		move.reduce_pp(amount);
		activate_pp_restore_berry(move, embargo, magic_room);
	}

	constexpr void set_hp(bounded::bounded_integer auto const hp) & {
		m_stats.hp() = hp;
	}

	constexpr auto initial_ability() const {
		if constexpr (exists<decltype(m_ability)>) {
			return m_ability;
		} else {
			return Ability::Honey_Gather;
		}
	}

	constexpr auto gender() const {
		if constexpr (exists<decltype(m_gender)>) {
			return m_gender;
		} else {
			return Gender::genderless;
		}
	}

	constexpr auto happiness() const -> Happiness {
		if constexpr (exists<decltype(m_happiness)>) {
			return m_happiness;
		} else {
			return Happiness();
		}
	}

	constexpr auto hidden_power() const -> tv::optional<HiddenPower<generation>> {
		if constexpr (exists<decltype(m_hidden_power)>) {
			return m_hidden_power;
		} else {
			return tv::none;
		}
	}

	constexpr auto item(bool const embargo, bool const magic_room) const -> Item {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.get(generation, embargo, magic_room);
		} else {
			return Item::None;
		}
	}
	constexpr auto remove_item() & -> tv::optional<Item> {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.remove();
		} else {
			return tv::none;
		}
	}
	constexpr auto destroy_item() & -> bool {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.destroy();
		} else {
			return false;
		}
	}
	constexpr auto recycle_item() & -> void {
		if constexpr (exists<decltype(m_item)>) {
			m_item.recycle();
		}
	}
	constexpr auto set_item(Item const item) & -> void {
		if constexpr (exists<decltype(m_item)>) {
			m_item = HeldItem(item);
		}
	}

	constexpr auto level() const -> Level {
		return m_level;
	}

	constexpr auto nature() const -> Nature {
		if constexpr (exists<decltype(m_nature)>) {
			return m_nature;
		} else {
			return Nature::Hardy;
		}
	}
	constexpr auto species() const -> Species {
		return m_species;
	}

	constexpr auto status() const -> Status {
		return m_status;
	}
	constexpr void set_status(StatusName const status) & {
		m_status = status;
	}

	friend auto operator==(Pokemon, Pokemon) -> bool = default;
	friend constexpr auto compress(Pokemon const value) {
		return compress_combine(
			value.m_regular_moves,
			value.m_stats.hp().current(),
			value.m_item,
			value.m_status,
			value.m_has_been_seen
		);
	}

private:
	constexpr auto activate_pp_restore_berry(Move & move, bool const embargo, bool const magic_room) & -> void {
		if (no_pp(move.pp())) {
			switch (item(embargo, magic_room)) {
				case Item::Leppa_Berry:
					move.restore_pp(10_bi);
					remove_item();
					break;
				case Item::MysteryBerry:
					move.restore_pp(5_bi);
					remove_item();
					break;
				default:
					break;
			}
		}
	}

	constexpr explicit Pokemon(bounded::tombstone_tag, auto const make):
		m_regular_moves(make()),
		m_stats(bounded::tombstone_traits<Stats<stat_style_for(generation)>>::make(0_bi)),
		m_species(),
		m_item(bounded::tombstone_traits<HeldItem>::make(0_bi)),
		m_ability(),
		m_gender(),
		m_status(),
		m_nature(),
		m_level(bounded::tombstone_traits<Level>::make(0_bi)),
		m_happiness(),
		m_hidden_power(),
		m_has_been_seen()
	{
	}

	RegularMoves m_regular_moves;

	Stats<stat_style_for(generation)> m_stats;

	Species m_species;
	[[no_unique_address]] ExistsIf<HeldItem, generation >= Generation::two> m_item;
	[[no_unique_address]] ExistsIf<Ability, generation >= Generation::three> m_ability;
	[[no_unique_address]] ExistsIf<Gender, generation >= Generation::two> m_gender;
	Status m_status;
	[[no_unique_address]] ExistsIf<Nature, generation >= Generation::three> m_nature;

	Level m_level;
	[[no_unique_address]] ExistsIf<Happiness, generation >= Generation::two> m_happiness;
	[[no_unique_address]] ExistsIf<tv::optional<HiddenPower<generation>>, generation >= Generation::two> m_hidden_power;

	bool m_has_been_seen;

	friend bounded::tombstone<Pokemon<generation>>;
	friend bounded::tombstone_member<&Pokemon<generation>::m_regular_moves>;
};

template<Generation generation>
constexpr auto is_real_pokemon<Pokemon<generation>> = true;

} // namespace technicalmachine

template<technicalmachine::Generation generation>
struct bounded::tombstone<technicalmachine::Pokemon<generation>> : bounded::tombstone_member<&technicalmachine::Pokemon<generation>::m_regular_moves> {
};
