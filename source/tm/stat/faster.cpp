// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.faster;

import tm.move.move_name;
import tm.move.priority;

import tm.stat.calculate_speed;

import tm.environment;
import tm.generation;
import tm.team;

import operators;
import tv;
import std_module;

namespace technicalmachine {

export template<Generation generation>
struct Faster : operators::arrow<Faster<generation>> {
	Faster(Team<generation> const & team1, Team<generation> const & team2, Environment const environment) {
		auto result = before_trick_room(team1, team2, environment);
		if (!result) {
			return;
		}
		tv::insert(m_teams, environment.trick_room() ? pair(result->second, result->first) : pair(result->first, result->second));
	}

	constexpr explicit operator bool() const {
		return static_cast<bool>(m_teams);
	}
	constexpr auto const & operator*() const {
		return *m_teams;
	}

private:
	using pair = std::pair<Team<generation> const &, Team<generation> const &>;

	Faster() = default;
	constexpr Faster(Team<generation> const & faster, Team<generation> const & slower):
		m_teams(pair(faster, slower))
	{
	}
	static auto before_trick_room(Team<generation> const & team1, Team<generation> const & team2, Environment const environment) -> Faster {
		auto const speed1 = calculate_speed(team1, team2.pokemon().ability(), environment);
		auto const speed2 = calculate_speed(team2, team1.pokemon().ability(), environment);

		return
			speed1 > speed2 ? Faster(team1, team2) :
			speed1 < speed2 ? Faster(team2, team1) :
			Faster();
	}


	tv::optional<pair> m_teams;
};

export template<Generation generation>
struct Order : operators::arrow<Order<generation>> {
	Order(Team<generation> const & team1, MoveName const move1, Team<generation> const & team2, MoveName const move2, Environment const environment):
		Order([&] {
			auto const priority1 = Priority(generation, move1);
			auto const priority2 = Priority(generation, move2);

			auto const lhs = Element{team1, move1};
			auto const rhs = Element{team2, move2};

			auto const lhs_first = Order(lhs, rhs);
			auto const rhs_first = Order(rhs, lhs);

			if (priority1 > priority2) {
				return lhs_first;
			} else if (priority1 < priority2) {
				return rhs_first;
			} else if (auto const ordered = Faster(team1, team2, environment)) {
				return (std::addressof(ordered->first) == std::addressof(team1)) ? lhs_first : rhs_first;
			} else {
				return Order();
			}
		}())
	{
	}


	constexpr explicit operator bool() const {
		return static_cast<bool>(m_elements);
	}
	constexpr auto const & operator*() const {
		return *m_elements;
	}

private:
	struct Element {
		Team<generation> const & team;
		MoveName move;
	};
	using pair = std::pair<Element, Element>;

	Order() = default;
	constexpr Order(Element const lhs, Element const rhs):
		m_elements(pair(lhs, rhs))
	{
	}

	tv::optional<pair> m_elements;
};

} // namespace technicalmachine
