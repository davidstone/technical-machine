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
#include <bounded/optional.hpp>

#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Ability : std::uint8_t;
struct ActivePokemon;
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
struct Team;
struct Weather;

inline auto initial_stat(StatNames const stat_name, Stat const stat, Level const level, Nature const nature) {
	auto const pre_nature = (2_bi * stat.base() + stat.iv().value() + stat.ev().value() / 4_bi) * level() / 100_bi + 5_bi;
	return pre_nature * boost(nature, stat_name);
}

using attack_type = bounded::integer<1, 7368>;
using special_attack_type = bounded::integer<1, 4536>;
auto calculate_attack(Generation, ActivePokemon attacker, Ability other_ability, Weather, bool critical_hit) -> attack_type;
auto calculate_special_attack(Generation, ActivePokemon attacker, Ability other_ability, Weather, bool critical_hit) -> special_attack_type;

using defense_type = bounded::integer<1, 3684>;
using special_defense_type = bounded::integer<1, 3684>;
auto calculate_defense(Generation, ActivePokemon defender, Ability other_ability, Weather, bool critical_hit = false, bool is_self_KO = false) -> defense_type;
auto calculate_special_defense(Generation, ActivePokemon defender, Ability attacker_ability, Weather, bool critical_hit = false) -> special_defense_type;

using speed_type = bounded::integer<1, 12096>;
auto calculate_speed(Generation, Team const &, Ability other_ability, Weather) -> speed_type;

using Faster = bounded::optional<std::pair<Team const &, Team const &>>;
auto faster_pokemon(Generation, Team const & team1, Team const & team2, Weather) -> Faster;

struct OrderElement {
	Team const & team;
	Moves move;
};
using Order = bounded::optional<std::pair<OrderElement, OrderElement>>;
auto order(Generation generation, Team const & team1, Moves move1, Team const & team2, Moves move2, Weather weather) -> Order;

}	// namespace technicalmachine
