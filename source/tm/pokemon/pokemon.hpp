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
#include <tm/pokemon/species.hpp>

#include <tm/ability.hpp>
#include <tm/compress.hpp>
#include <tm/gender.hpp>
#include <tm/generation.hpp>
#include <tm/held_item.hpp>
#include <tm/operators.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <tm/move/container.hpp>

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stats.hpp>

#include <bounded/assert.hpp>

#include <cstdint>

namespace technicalmachine {

template<Generation generation>
struct Pokemon {
	Pokemon(Species const species, Level const level, Gender const gender, Item const item, Ability const ability, CombinedStats<IVAndEV> stats, RegularMoves regular_moves_, Happiness const happiness = Happiness()):
		m_regular_moves(regular_moves_),
		m_stats(BaseStats(generation, species), stats, level),

		m_species(species),
		m_item(item),
		m_ability(ability),
		m_gender(gender),
		m_nature(stats.nature),

		m_level(level),

		m_happiness(happiness),

		// TODO: Make this none if there is no way to call Hidden Power
		// TODO: Use the IVs provided
		m_hidden_power([=]{
			constexpr auto dv = DV(15_bi);
			constexpr auto iv = IV(31_bi);
			return generation <= Generation::two ?
				HiddenPower(generation, DVs{dv, dv, dv, dv}) :
				HiddenPower(generation, IVs{iv, iv, iv, iv, iv, iv});
		}()),
		
		m_has_been_seen(false),

		m_ability_is_known(true),
		m_item_is_known(true),
		m_nature_is_known(true)
	{
	}

	Pokemon(Species const species, Level const level, Gender const gender) : 
		Pokemon::Pokemon(
			species,
			level,
			gender,
			Item::None,
			Ability::Honey_Gather,
			CombinedStats<IVAndEV>{
				Nature::Hardy,
				{default_iv(generation), EV(0_bi)},
				{default_iv(generation), EV(0_bi)},
				{default_iv(generation), EV(0_bi)},
				{default_iv(generation), EV(0_bi)},
				{default_iv(generation), EV(0_bi)},
				{default_iv(generation), EV(0_bi)},
			},
			RegularMoves(),
			Happiness()
		)
	{
		m_ability_is_known = false;
		m_item_is_known = false;
		m_nature_is_known = false;
	}
	
	auto hp() const {
		return m_stats.hp();
	}
	auto stat(RegularStat const stat_name) const {
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

	auto regular_moves() const -> RegularMoves const & {
		return m_regular_moves;
	}
	auto regular_moves() -> RegularMoves & {
		return m_regular_moves;
	}

	void set_hp(auto const hp) & {
		m_stats.hp() = hp;
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

	auto item(bool const embargo, bool const magic_room) const -> Item {
		return m_item.get(generation, embargo, magic_room);
	}
	auto unmodified_item() const -> Item {
		return m_item.get(generation, false, false);
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

	auto set_ev(PermanentStat const stat_name, IV const iv, EV const ev) -> void {
		auto const base_stats = BaseStats(generation, species());
		if (stat_name == PermanentStat::hp) {
			m_stats.hp() = HP(base_stats, level(), iv, ev);
		} else {
			auto const regular = RegularStat(stat_name);
			auto & stat = m_stats[regular];
			stat = initial_stat(regular, base_stats[regular], nature(), iv, ev, level());
		}
	}

	friend auto operator==(Pokemon const &, Pokemon const &) -> bool = default;
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
	RegularMoves m_regular_moves;
	
	Stats m_stats;

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

auto get_hidden_power_type(auto const pokemon) {
	auto const hidden_power = pokemon.hidden_power();
	return BOUNDED_CONDITIONAL(hidden_power, hidden_power->type(), bounded::none);
}

auto hp_ratio(auto const pokemon) {
	auto const hp = pokemon.hp();
	return rational(hp.current(), hp.max());
}

}	// namespace technicalmachine
