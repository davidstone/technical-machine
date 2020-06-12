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

#include <tm/stat/stat_names.hpp>

#include <tm/type/type.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <operators/arrow.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Ability : std::uint16_t;
struct ActivePokemon;
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
struct Team;
struct Weather;

using attack_type = bounded::integer<1, 7368>;
using special_attack_type = bounded::integer<1, 4536>;
auto calculate_attack(Generation, ActivePokemon attacker, Type move_type, Ability other_ability, Weather, bool critical_hit) -> attack_type;
auto calculate_special_attack(Generation, ActivePokemon attacker, Type move_type, Ability other_ability, Weather, bool critical_hit) -> special_attack_type;

using defense_type = bounded::integer<1, 3684>;
using special_defense_type = bounded::integer<1, 3684>;
auto calculate_defense(Generation, ActivePokemon defender, Moves, Weather, bool critical_hit = false) -> defense_type;
auto calculate_special_defense(Generation, ActivePokemon defender, Ability attacker_ability, Weather, bool critical_hit = false) -> special_defense_type;

using speed_type = bounded::integer<1, 12096>;
auto calculate_speed(Generation, Team const &, Ability other_ability, Weather) -> speed_type;

struct Faster : operators::arrow<Faster> {
	Faster(Generation, Team const & team1, Team const & team2, Weather);
	constexpr explicit operator bool() const {
		return static_cast<bool>(m_teams);
	}
	constexpr auto const & operator*() const {
		return *m_teams;
	}
private:
	using pair = std::pair<Team const &, Team const &>;

	Faster() = default;
	constexpr Faster(Team const & faster, Team const & slower):
		m_teams(pair(faster, slower))
	{
	}
	static auto before_trick_room(Generation, Team const & team1, Team const & team2, Weather) -> Faster;

	bounded::optional<pair> m_teams;
};

struct Order : operators::arrow<Order> {
	Order(Generation, Team const & team1, Moves move1, Team const & team2, Moves move2, Weather);

	constexpr explicit operator bool() const {
		return static_cast<bool>(m_elements);
	}
	constexpr auto const & operator*() const {
		return *m_elements;
	}
private:
	struct Element {
		Team const & team;
		Moves move;
	};
	using pair = std::pair<Element, Element>;

	Order() = default;
	constexpr Order(Element const lhs, Element const rhs):
		m_elements(pair(lhs, rhs))
	{
	}

	bounded::optional<pair> m_elements;
};

}	// namespace technicalmachine
