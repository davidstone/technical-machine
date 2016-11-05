// Calculate a Pokemon's current stat
// Copyright (C) 2016 David Stone
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

#include <bounded/integer.hpp>

#include "nature.hpp"
#include "stat.hpp"
#include "stat_names.hpp"

#include "../pokemon/level.hpp"

namespace technicalmachine {
using namespace bounded::literal;

struct ActivePokemon;
struct Team;
struct Weather;

auto initial_stat(StatNames const stat_name, Stat const stat, Level const level, Nature const nature) {
	auto const pre_nature = (2_bi * stat.base() + stat.iv().value() + stat.ev().value() / 4_bi) * level() / 100_bi + 5_bi;
	return pre_nature * boost(nature, stat_name);
}

using attack_type = bounded::integer<1, 7368>;
using special_attack_type = bounded::integer<1, 4536>;
auto calculate_attacking_stat(ActivePokemon attacker, Weather weather, bool critical_hit) -> std::common_type_t<attack_type, special_attack_type>;
auto calculate_attack(ActivePokemon attacker, Weather weather, bool critical_hit) -> attack_type;
auto calculate_special_attack(ActivePokemon attacker, Weather weather, bool critical_hit) -> special_attack_type;

using defense_type = bounded::integer<1, 3684>;
using special_defense_type = bounded::integer<1, 3684>;
auto calculate_defending_stat(ActivePokemon attacker, ActivePokemon defender, Weather weather, bool critical_hit) -> std::common_type_t<defense_type, special_defense_type>;
auto calculate_defense(ActivePokemon defender, Weather weather, bool critical_hit = false, bool is_self_KO = false) -> defense_type;
auto calculate_special_defense(ActivePokemon defender, Weather weather, bool critical_hit = false) -> special_defense_type;

using speed_type = bounded::integer<1, 12096>;
auto calculate_speed(Team const & team, Weather weather) -> speed_type;
auto order(Team const & team1, Team const & team2, Weather weather) -> std::pair<Team const *, Team const *>;
auto faster_pokemon(Team const & team1, Team const & team2, Weather weather) -> std::pair<Team const *, Team const *>;

}	// namespace technicalmachine
