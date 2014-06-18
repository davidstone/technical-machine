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

#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

class ActivePokemon;
class EntryHazards;
class Pokemon;
class Status;
class Team;
class Weather;

// 100% chance to win
constexpr auto victory = 40715248_bi;

class Evaluate {
public:
	// +1 gives me room to create a value that will always be overwritten
	using type = bounded::integer<-static_cast<intmax_t>(victory + 1_bi), static_cast<intmax_t>(victory + 1_bi)>;
	Evaluate();
	auto operator()(Team const & ai, Team const & foe, Weather weather) const -> type;
	// Both of these return victory if the battle is won. Returns -victory
	// if the battle is lost. Returns 0 otherwise.
	static auto win(Team const & team) -> type;
	static auto sleep_clause(Team const & team) -> type;

	// Arbitrary values
	using value_type = bounded::integer<-4096, 4096>;

	auto light_screen() const { return m_light_screen; }
	auto lucky_chant() const { return m_lucky_chant; }
	auto mist() const { return m_mist; }
	auto reflect() const { return m_reflect; }
	auto safeguard() const { return m_safeguard; }
	auto tailwind() const { return m_tailwind; }
	auto wish() const { return m_wish; }
	auto spikes() const { return m_spikes; }
	auto stealth_rock() const { return m_stealth_rock; }
	auto toxic_spikes() const { return m_toxic_spikes; }
	auto members() const { return m_members; }
	auto hp() const { return m_hp; }
	auto hidden() const { return m_hidden; }
	auto aqua_ring() const { return m_aqua_ring; }
	auto curse() const { return m_curse; }
	auto imprison() const { return m_imprison; }
	auto ingrain() const { return m_ingrain; }
	auto leech_seed() const { return m_leech_seed; }
	auto loaf() const { return m_loaf; }
	auto magnet_rise() const { return m_magnet_rise; }
	auto nightmare() const { return m_nightmare; }
	auto substitute() const { return m_substitute; }
	auto substitute_hp() const { return m_substitute_hp; }
	auto torment() const { return m_torment; }
	auto trapped() const { return m_trapped; }
	auto burn() const { return m_burn; }
	auto freeze() const { return m_freeze; }
	auto paralysis() const { return m_paralysis; }
	auto poison() const { return m_poison; }
	auto sleep() const { return m_sleep; }
	auto toxic() const { return m_toxic; }
	auto focus_energy() const { return m_focus_energy; }
	auto baton_pass() const { return m_baton_pass; }
	auto no_pp() const { return m_no_pp; }
	auto stage() const { return m_stage; }
private:
	value_type m_light_screen;
	value_type m_lucky_chant;
	value_type m_mist;
	value_type m_reflect;
	value_type m_safeguard;
	value_type m_tailwind;

	value_type m_wish;

	value_type m_spikes;
	value_type m_stealth_rock;
	value_type m_toxic_spikes;

	value_type m_members;
	value_type m_hp;
	value_type m_hidden;
	value_type m_aqua_ring;
	value_type m_curse;
	value_type m_imprison;
	value_type m_ingrain;
	value_type m_leech_seed;
	value_type m_loaf;
	value_type m_magnet_rise;
	value_type m_nightmare;
	value_type m_substitute;
	value_type m_substitute_hp;
	value_type m_torment;
	value_type m_trapped;

	value_type m_burn;
	value_type m_freeze;
	value_type m_paralysis;
	value_type m_poison;
	value_type m_sleep;
	value_type m_toxic;
	
	bounded::array<value_type, Stage::number_of_stats.value()> m_stage;
	value_type m_focus_energy;

	value_type m_baton_pass;
	value_type m_no_pp;
};

} // namespace technicalmachine
#endif // EVALUATE__EVALUATE_HPP_
