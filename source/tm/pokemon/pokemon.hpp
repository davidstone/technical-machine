// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/happiness.hpp>
#include <tm/pokemon/hidden_power.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/move/container.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stats.hpp>

#include <tm/ability.hpp>
#include <tm/compress.hpp>
#include <tm/exists_if.hpp>
#include <tm/gender.hpp>
#include <tm/generation.hpp>
#include <tm/held_item.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/maybe_find.hpp>

#include <cstdint>

namespace technicalmachine {

template<Generation generation>
struct Pokemon {
	Pokemon(Species const species, Level const level, Gender const gender, Item const item, Ability const ability, CombinedStats<generation> const stat_inputs, RegularMoves regular_moves_, Happiness const happiness = Happiness()):
		m_regular_moves(regular_moves_),
		m_stats(BaseStats(generation, species), level, stat_inputs),

		m_species(species),
		m_item(item),
		m_ability(ability),
		m_gender(gender),
		m_nature(stat_inputs.nature),

		m_level(level),

		m_happiness(happiness),

		m_hidden_power([&]() -> bounded::optional<HiddenPower<generation>> {
			if constexpr (generation == Generation::one) {
				return bounded::none;
			} else if (containers::any(m_regular_moves, [](Move const move) { return move.name() == Moves::Hidden_Power; })) {
				return HiddenPower<generation>(stat_inputs.dvs_or_ivs);
			} else {
				return bounded::none;
			}
		}()),
		
		m_has_been_seen(false),

		m_ability_is_known(true),
		m_item_is_known(true),
		m_stats_are_known(true)
	{
	}

	Pokemon(Species const species, Level const level, Gender const gender):
		Pokemon(
			species,
			level,
			gender,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			RegularMoves(),
			Happiness()
		)
	{
		m_ability_is_known = false;
		m_item_is_known = false;
		m_stats_are_known = false;
	}

	auto hp() const {
		return m_stats.hp();
	}
	auto stat(SplitSpecialRegularStat const stat_name) const {
		return m_stats[stat_name];
	}

	auto advance_status_from_move(Ability const ability, bool const clear_status) & {
		m_status.advance_from_move(ability, clear_status);
	}

	auto has_been_seen() const -> bool {
		return m_has_been_seen;
	}
	void mark_as_seen() & {
		m_has_been_seen = true;
	}

	auto regular_moves() const -> RegularMoves {
		return m_regular_moves;
	}
	auto add_move(Move const move) & -> void {
		if (containers::any_equal(regular_moves(), move.name()) or !is_regular(move.name())) {
			return;
		}
		m_regular_moves.push_back(move);
		if constexpr (generation != Generation::one) {
			if (move.name() == Moves::Hidden_Power) {
				if (!m_stats_are_known) {
					// TODO: Don't just give them Hidden Power Dark
					set_ivs_and_evs(default_combined_stats<generation>);
					m_stats_are_known = false;
					m_hidden_power = HiddenPower<generation>(default_combined_stats<generation>.dvs_or_ivs);
				} else {
					m_hidden_power = HiddenPower<generation>(calculate_ivs_and_evs(*this).dvs_or_ivs);
				}
			}
		}
	}

	void reduce_pp(Moves const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & {
		auto const maybe_move = containers::maybe_find(m_regular_moves, move_name);
		if (!maybe_move) {
			return;
		}
		auto & move = *maybe_move;
		move.reduce_pp(amount);
		activate_pp_restore_berry(move, embargo, magic_room);
	}

	void set_hp(auto const hp) & {
		m_stats.hp() = hp;
	}

	auto initial_ability() const {
		if constexpr (exists<decltype(m_ability)>) {
			return m_ability;
		} else {
			return Ability::Honey_Gather;
		}
	}
	void set_initial_ability(Ability const ability) & {
		if constexpr (exists<decltype(m_ability)>) {
			m_ability = ability;
			m_ability_is_known = true;
		}
	}

	auto gender() const {
		if constexpr (exists<decltype(m_gender)>) {
			return m_gender;
		} else {
			return Gender::genderless;
		}
	}

	auto happiness() const -> Happiness {
		if constexpr (exists<decltype(m_happiness)>) {
			return m_happiness;
		} else {
			return Happiness();
		}
	}

	auto hidden_power() const -> bounded::optional<HiddenPower<generation>> {
		if constexpr (exists<decltype(m_hidden_power)>) {
			return m_hidden_power;
		} else {
			return bounded::none;
		}
	}

	auto item(bool const embargo, bool const magic_room) const -> Item {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.get(generation, embargo, magic_room);
		} else {
			return Item::None;
		}
	}
	auto remove_item() & -> bounded::optional<Item> {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.remove();
		} else {
			return bounded::none;
		}
	}
	auto destroy_item() & -> bool {
		if constexpr (exists<decltype(m_item)>) {
			return m_item.destroy();
		} else {
			return false;
		}
	}
	auto recycle_item() & -> void {
		if constexpr (exists<decltype(m_item)>) {
			m_item.recycle();
		}
	}
	auto set_item(Item const item) & -> void {
		if constexpr (exists<decltype(m_item)>) {
			m_item = HeldItem(item);
			m_item_is_known = true;
		}
	}

	auto level() const -> Level {
		return m_level;
	}

	auto nature() const -> Nature {
		if constexpr (exists<decltype(m_nature)>) {
			return m_nature;
		} else {
			return Nature::Hardy;
		}
	}
	auto species() const -> Species {
		return m_species;
	}

	auto status() const -> Status {
		return m_status;
	}
	void set_status(Statuses const status) & {
		m_status = status;
	}

	auto ability_is_known() const -> bool {
		return m_ability_is_known;
	}
	auto item_is_known() const -> bool {
		return m_item_is_known;
	}

	auto set_ivs_and_evs(CombinedStats<generation> const stat_inputs) -> void {
		if constexpr (exists<decltype(m_nature)>) {
			m_nature = stat_inputs.nature;
			m_stats_are_known = true;
		}
		m_stats = with_new_ivs_and_evs(hp(), BaseStats(generation, species()), level(), stat_inputs);
	}

	friend auto operator==(Pokemon, Pokemon) -> bool = default;
	friend auto compress(Pokemon const value) {
		return compress_combine(
			value.m_regular_moves,
			value.m_stats.hp().current(),
			value.m_item,
			value.m_status,
			value.m_has_been_seen
		);
	}
private:
	auto activate_pp_restore_berry(Move & move, bool const embargo, bool const magic_room) & -> void {
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

	RegularMoves m_regular_moves;
	
	Stats<generation> m_stats;

	Species m_species;
	[[no_unique_address]] ExistsIf<HeldItem, generation >= Generation::two> m_item;
	[[no_unique_address]] ExistsIf<Ability, generation >= Generation::three> m_ability;
	[[no_unique_address]] ExistsIf<Gender, generation >= Generation::two> m_gender;
	Status m_status;
	[[no_unique_address]] ExistsIf<Nature, generation >= Generation::three> m_nature;

	Level m_level;
	[[no_unique_address]] ExistsIf<Happiness, generation >= Generation::two> m_happiness;
	[[no_unique_address]] ExistsIf<bounded::optional<HiddenPower<generation>>, generation >= Generation::two> m_hidden_power;

	bool m_has_been_seen : 1;
	
	bool m_ability_is_known : 1;
	bool m_item_is_known : 1;
	bool m_stats_are_known : 1;
};

template<Generation generation>
auto calculate_ivs_and_evs(Pokemon<generation> const pokemon) {
	auto const nature = pokemon.nature();
	auto const stats = Stats<generation>{
		pokemon.hp(),
		pokemon.stat(SplitSpecialRegularStat::atk),
		pokemon.stat(SplitSpecialRegularStat::def),
		pokemon.stat(SplitSpecialRegularStat::spa),
		pokemon.stat(SplitSpecialRegularStat::spd),
		pokemon.stat(SplitSpecialRegularStat::spe)
	};
	return calculate_ivs_and_evs(
		pokemon.species(),
		pokemon.level(),
		stats,
		pokemon.hidden_power(),
		containers::enum_range(nature, nature)
	);
}

extern template struct Pokemon<Generation::one>;
extern template struct Pokemon<Generation::two>;
extern template struct Pokemon<Generation::three>;
extern template struct Pokemon<Generation::four>;
extern template struct Pokemon<Generation::five>;
extern template struct Pokemon<Generation::six>;
extern template struct Pokemon<Generation::seven>;
extern template struct Pokemon<Generation::eight>;

auto get_hidden_power_type(auto const pokemon) {
	auto const hidden_power = pokemon.hidden_power();
	return BOUNDED_CONDITIONAL(hidden_power, hidden_power->type(), bounded::none);
}

auto hp_ratio(auto const pokemon) {
	auto const hp = pokemon.hp();
	return rational(hp.current(), hp.max());
}

} // namespace technicalmachine
