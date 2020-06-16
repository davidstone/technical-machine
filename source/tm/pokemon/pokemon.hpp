// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/pokemon/happiness.hpp>
#include <tm/pokemon/hidden_power.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/held_item.hpp>
#include <tm/operators.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <tm/move/container.hpp>

#include <tm/stat/nature.hpp>
#include <tm/stat/stats.hpp>

#include <bounded/assert.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Generation : std::uint8_t;

struct Pokemon {
	Pokemon(Generation, TeamSize my_team_size, Species species, Level level, Gender gender, Happiness happiness = Happiness{});
	Pokemon(Generation, TeamSize my_team_size, Species species, Level level, Gender gender, Item item, Ability ability, Nature nature, Happiness happiness = Happiness{});
	
	auto hp() const {
		return stats.hp();
	}
	auto stat(RegularStat const stat_name) const {
		return stats[stat_name];
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

	auto all_moves() const -> MoveContainer const & {
		return m_moves;
	}
	auto all_moves() -> MoveContainer & {
		return m_moves;
	}

	void set_hp(auto const hp) & {
		stats.hp() = hp;
	}

	auto initial_ability() const {
		return m_ability;
	}
	void set_initial_ability(Ability const ability) & {
		m_ability = ability;
		m_ability_is_known = true;
	}

	auto gender() const {
		return m_gender;
	}

	auto happiness() const -> Happiness {
		return m_happiness;
	}

	auto hidden_power() const -> bounded::optional<HiddenPower> {
		return m_hidden_power;
	}

	auto item(Generation const generation, bool const embargo, bool const magic_room) const -> Item {
		return m_item.get(generation, embargo, magic_room);
	}
	auto unmodified_item() const -> Item {
		return m_item.get(Generation::eight, false, false);
	}
	auto remove_item() & {
		return m_item.remove();
	}
	auto destroy_item() & {
		return m_item.destroy();
	}
	auto recycle_item() & -> void {
		m_item.recycle();
	}
	auto set_item(Item const item) & -> void {
		m_item = HeldItem(item);
		m_item_is_known = true;
	}

	auto level() const -> Level {
		return m_level;
	}

	auto nature() const -> Nature {
		return m_nature;
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
	auto set_nature(Nature const nature) -> void {
		m_nature = nature;
		m_nature_is_known = true;
	}
	auto nature_is_known() const -> bool {
		return m_nature_is_known;
	}

	auto set_ev(Generation const generation, PermanentStat const stat_name, IV const iv, EV const ev) -> void {
		if (stat_name == PermanentStat::hp) {
			auto const base_stats = BaseStats(generation, species());
			stats.hp() = HP(base_stats, level(), iv, ev);
		} else {
			auto & stat = stats[RegularStat(stat_name)];
			stat = Stat(stat.base(), iv, ev);
		}
	}


	friend auto operator==(Pokemon const &, Pokemon const &) -> bool = default;
private:
	MoveContainer m_moves;
	
	Stats stats;

	Species m_species;
	HeldItem m_item;
	Ability m_ability;
	Gender m_gender;
	Status m_status;
	Nature m_nature;

	Level m_level;
	Happiness m_happiness;
	bounded::optional<HiddenPower> m_hidden_power;

	bool m_has_been_seen : 1;
	
	bool m_ability_is_known : 1;
	bool m_item_is_known : 1;
	bool m_nature_is_known : 1;
};

inline decltype(auto) regular_moves(Pokemon const & pokemon) {
	return pokemon.all_moves().regular();
}
inline decltype(auto) regular_moves(Pokemon & pokemon) {
	return pokemon.all_moves().regular();
}

inline auto get_hidden_power_type(Pokemon const pokemon) {
	auto const hidden_power = pokemon.hidden_power();
	return BOUNDED_CONDITIONAL(hidden_power, hidden_power->type(), bounded::none);
}

inline auto hp_ratio(Pokemon const pokemon) {
	auto const hp = pokemon.hp();
	return rational(hp.current(), hp.max());
}

}	// namespace technicalmachine
