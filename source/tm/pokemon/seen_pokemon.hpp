// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>
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

#include <tm/string_conversions/species.hpp>

#include <tm/ability.hpp>
#include <tm/compress.hpp>
#include <tm/exists_if.hpp>
#include <tm/gender.hpp>
#include <tm/generation.hpp>
#include <tm/held_item.hpp>
#include <tm/status.hpp>
#include <tm/visible_hp.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/maybe_find.hpp>

#include <cstdint>

namespace technicalmachine {

auto has_hidden_power(any_pokemon auto const & pokemon) -> bool {
	return containers::maybe_find_if(pokemon.regular_moves(), [](Move const move) {
		return move.name() == Moves::Hidden_Power;
	});
}


template<Generation generation>
struct SeenPokemon {
	SeenPokemon(Species const species, containers::string nickname, Level const level, Gender const gender, MaxVisibleHP const hp_resolution = MaxVisibleHP(100_bi)):
		m_nickname(std::move(nickname)),
		m_species(species),
		m_gender(gender),
		m_level(level),
		m_hp{CurrentVisibleHP(hp_resolution.value()), hp_resolution},
		m_stats(BaseStats(generation, m_species), m_level, default_combined_stats<generation>)
	{
	}
	SeenPokemon(Species const species, Level const level, Gender const gender, MaxVisibleHP const hp_resolution = MaxVisibleHP(100_bi)):
		SeenPokemon(species, containers::string(to_string(species)), level, gender, hp_resolution)
	{
	}

	constexpr auto nickname() const -> std::string_view {
		return m_nickname;
	}

	auto stat(SplitSpecialRegularStat const stat_name) const {
		return m_stats[stat_name];
	}
	constexpr auto visible_hp() const {
		return m_hp;
	}
	constexpr auto hp() const {
		auto const max_hp = m_stats.hp().max();
		auto temp = HP(max_hp);
		temp = to_real_hp(max_hp, visible_hp()).value;
		return temp;
	}
	constexpr auto set_hp(CurrentVisibleHP const visible_hp) & -> void {
		// TODO: Do something smart with actual
		if (visible_hp > m_hp.max) {
			throw std::runtime_error(containers::concatenate<std::string>("Tried to set a visible HP of "sv, to_string(visible_hp.value()), ", is greater than visible max HP of "sv, to_string(m_hp.max.value())));
		}
		m_hp.current = visible_hp;
	}
	constexpr auto set_hp(bounded::bounded_integer auto const given_real_hp) & -> void {
		auto const max_real_hp = m_stats.hp().max();
		auto const real_hp = bounded::clamp(given_real_hp, 0_bi, max_real_hp);
		set_hp(CurrentVisibleHP(bounded::assume_in_range(m_hp.max.value() * real_hp / max_real_hp, 0_bi, m_hp.max.value())));
	}

	constexpr auto advance_status_from_move(Ability const ability, bool const clear_status) & {
		m_status.advance_from_move(ability, clear_status);
	}

	constexpr void mark_as_seen() & {
	}

	constexpr auto regular_moves() const -> RegularMoves {
		return m_regular_moves;
	}
	auto add_move(Move const move) & -> void {
		if (containers::any_equal(regular_moves(), move.name()) or !is_regular(move.name())) {
			return;
		}
		m_regular_moves.push_back(move);
	}

	constexpr auto reduce_pp(Moves const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & -> void {
		auto const maybe_move = containers::maybe_find(m_regular_moves, move_name);
		if (!maybe_move) {
			return;
		}
		auto & move = *maybe_move;
		move.reduce_pp(amount);
		activate_pp_restore_berry(move, embargo, magic_room);
	}

	constexpr auto initial_ability() const -> Ability {
		if constexpr (exists<decltype(m_ability)>) {
			// TODO: ???
			return m_ability ? *m_ability : Ability::Honey_Gather;
		} else {
			return Ability::Honey_Gather;
		}
	}
	constexpr auto set_initial_ability(Ability const ability) & -> void {
		if constexpr (exists<decltype(m_ability)>) {
			m_ability = ability;
		}
	}

	constexpr auto gender() const -> Gender {
		if constexpr (exists<decltype(m_gender)>) {
			return m_gender;
		} else {
			return Gender::genderless;
		}
	}

	// TODO: ???
	auto happiness() const -> Happiness {
		return Happiness();
	}

	constexpr auto item(bool const embargo, bool const magic_room) const -> Item {
		if constexpr (exists<decltype(m_item)>) {
			// TODO: ???
			return m_item ? m_item->get(generation, embargo, magic_room) : Item::None;
		} else {
			return Item::None;
		}
	}
	constexpr auto actual_item() const {
		// TODO: ???
		if constexpr (exists<decltype(m_item)>) {
			return m_item ? *m_item : HeldItem(Item::None);
		} else {
			return Item::None;
		}
	}
	constexpr auto remove_item() & -> bounded::optional<Item> {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			return m_item->remove();
		} else {
			return bounded::none;
		}
	}
	constexpr auto destroy_item() & -> bool {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			return m_item->destroy();
		} else {
			return false;
		}
	}
	constexpr auto recycle_item() & -> void {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			m_item->recycle();
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

	constexpr auto species() const -> Species {
		return m_species;
	}

	constexpr auto status() const -> Status {
		return m_status;
	}
	constexpr auto set_status(Statuses const status) & -> void {
		m_status = status;
	}

	constexpr auto ability_is_known() const -> bool {
		if constexpr (exists<decltype(m_ability)>) {
			return static_cast<bool>(m_ability);
		} else {
			return true;
		}
	}
	constexpr auto item_is_known() const -> bool {
		if constexpr (exists<decltype(m_item)>) {
			return static_cast<bool>(m_item);
		} else {
			return true;
		}
	}

	constexpr auto nature() const -> Nature {
		if constexpr (exists<decltype(m_nature)>) {
			return m_nature ? *m_nature : Nature::Hardy;
		} else {
			return Nature::Hardy;
		}
	}

	auto set_ivs_and_evs(CombinedStats<generation> const stat_inputs) -> void {
		if constexpr (exists<decltype(m_nature)>) {
			m_nature = stat_inputs.nature;
		}
		m_stats = Stats<generation>(BaseStats(generation, species()), level(), stat_inputs);
	}

	auto hidden_power() const -> bounded::optional<HiddenPower<generation>> {
		// TODO: ???
		return has_hidden_power(*this) ?
			HiddenPower<generation>(numeric_traits::max_value<typename HiddenPower<generation>::Power>, Type::Dark) :
			bounded::optional<HiddenPower<generation>>();
	}

	friend auto operator==(SeenPokemon, SeenPokemon) -> bool = default;
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

	containers::string m_nickname;
	RegularMoves m_regular_moves;
	
	Species m_species;
	[[no_unique_address]] ExistsIf<bounded::optional<HeldItem>, generation >= Generation::two> m_item;
	[[no_unique_address]] ExistsIf<bounded::optional<Ability>, generation >= Generation::three> m_ability;
	[[no_unique_address]] ExistsIf<Gender, generation >= Generation::two> m_gender;
	[[no_unique_address]] ExistsIf<bounded::optional<Nature>, generation >= Generation::three> m_nature;
	Status m_status;

	Level m_level;

	VisibleHP m_hp;
	// TODO: Make this a set of possible ranges. The current HP is never used
	// from this. When the representation is changed to store possible ranges
	// instead of an exact value, that useless state will go away.
	Stats<generation> m_stats;

	// TODO: Possible Hidden Power types
};

} // namespace technicalmachine
