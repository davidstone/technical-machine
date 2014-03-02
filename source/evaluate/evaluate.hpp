// Evaluate header
// Copyright (C) 2014 David Stone
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

#ifndef EVALUATE__EVALUATE_HPP_
#define EVALUATE__EVALUATE_HPP_

#include "../stat/stage.hpp"
#include "../stat/stat_names.hpp"

#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>

#include <array>
#include <cstdint>

namespace technicalmachine {

class ActivePokemon;
class EntryHazards;
class Pokemon;
class Status;
class Team;
class Weather;

class Evaluate {
public:
	Evaluate();
	void load();
	int64_t operator()(Team const & ai, Team const & foe, Weather const & weather) const;
	// Both of these return victory if the battle is won. Returns -victory
	// if the battle is lost. Returns 0 otherwise.
	static int64_t win(Team const & team);
	static int64_t sleep_clause(Team const & team);
	// Highest score. 100% chance to win. No particular significance to
	// this number other than being larger than any score a non-winning
	// position can have.
	constexpr static int64_t victory = 30240;

	// Arbitrary values
	using value_type = bounded_integer::native_integer<-4096, 4096>;
	using underlying_type = bounded_integer::equivalent_type<value_type, bounded_integer::throw_policy>;
	using stage_type = bounded_integer::array<underlying_type, Stage::number_of_stats.value()>;

	auto light_screen() const -> value_type { return m_light_screen; }
	auto lucky_chant() const -> value_type { return m_lucky_chant; }
	auto mist() const -> value_type { return m_mist; }
	auto reflect() const -> value_type { return m_reflect; }
	auto safeguard() const -> value_type { return m_safeguard; }
	auto tailwind() const -> value_type { return m_tailwind; }
	auto wish() const -> value_type { return m_wish; }
	auto spikes() const -> value_type { return m_spikes; }
	auto stealth_rock() const -> value_type { return m_stealth_rock; }
	auto toxic_spikes() const -> value_type { return m_toxic_spikes; }
	auto members() const -> value_type { return m_members; }
	auto hp() const -> value_type { return m_hp; }
	auto hidden() const -> value_type { return m_hidden; }
	auto aqua_ring() const -> value_type { return m_aqua_ring; }
	auto curse() const -> value_type { return m_curse; }
	auto imprison() const -> value_type { return m_imprison; }
	auto ingrain() const -> value_type { return m_ingrain; }
	auto leech_seed() const -> value_type { return m_leech_seed; }
	auto loaf() const -> value_type { return m_loaf; }
	auto magnet_rise() const -> value_type { return m_magnet_rise; }
	auto nightmare() const -> value_type { return m_nightmare; }
	auto substitute() const -> value_type { return m_substitute; }
	auto substitute_hp() const -> value_type { return m_substitute_hp; }
	auto torment() const -> value_type { return m_torment; }
	auto trapped() const -> value_type { return m_trapped; }
	auto burn() const -> value_type { return m_burn; }
	auto freeze() const -> value_type { return m_freeze; }
	auto paralysis() const -> value_type { return m_paralysis; }
	auto poison() const -> value_type { return m_poison; }
	auto sleep() const -> value_type { return m_sleep; }
	auto toxic() const -> value_type { return m_toxic; }
	auto focus_energy() const -> value_type { return m_focus_energy; }
	auto baton_pass() const -> value_type { return m_baton_pass; }
	auto no_pp() const -> value_type { return m_no_pp; }
	auto stage() const -> stage_type { return m_stage; }
private:
	int64_t score_team(Team const & team) const;
	int64_t score_all_pokemon(Team const & team, Team const & other, Weather const & weather) const;
	int64_t score_pokemon(Pokemon const & pokemon, EntryHazards const & entry_hazards, Team const & other, Weather const & weather, int toxic_counter = 1) const;
	int64_t score_active_pokemon(ActivePokemon const & active) const;
	int64_t baton_passable_score(ActivePokemon const & pokemon) const;
	int64_t score_status(Pokemon const & pokemon, int toxic_counter) const;
	int64_t score_move (Pokemon const & pokemon, Team const & other, Weather const & weather) const;

	underlying_type m_light_screen;
	underlying_type m_lucky_chant;
	underlying_type m_mist;
	underlying_type m_reflect;
	underlying_type m_safeguard;
	underlying_type m_tailwind;

	underlying_type m_wish;

	underlying_type m_spikes;
	underlying_type m_stealth_rock;
	underlying_type m_toxic_spikes;

	underlying_type m_members;
	underlying_type m_hp;
	underlying_type m_hidden;
	underlying_type m_aqua_ring;
	underlying_type m_curse;
	underlying_type m_imprison;
	underlying_type m_ingrain;
	underlying_type m_leech_seed;
	underlying_type m_loaf;
	underlying_type m_magnet_rise;
	underlying_type m_nightmare;
	underlying_type m_substitute;
	underlying_type m_substitute_hp;
	underlying_type m_torment;
	underlying_type m_trapped;

	underlying_type m_burn;
	underlying_type m_freeze;
	underlying_type m_paralysis;
	underlying_type m_poison;
	underlying_type m_sleep;
	underlying_type m_toxic;
	
	stage_type m_stage;
	underlying_type m_focus_energy;

	underlying_type m_baton_pass;
	underlying_type m_no_pp;
};

} // namespace technicalmachine
#endif // EVALUATE__EVALUATE_HPP_
