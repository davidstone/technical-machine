// Evaluate header
// Copyright (C) 2012 David Stone
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

#include <array>
#include <cstdint>

#include "../stat/stat.hpp"

namespace technicalmachine {

class ActivePokemon;
class Pokemon;
class Status;
class Team;
class Weather;

class Evaluate {
	public:
		Evaluate();
		void load();
		// ai and foe are both logically constant. I change the active Pokemon
		// in each of them, but I change it back before the function returns.
		int64_t operator()(Team & ai, Team & foe, Weather const & weather) const;
		// Both of these return victory if the battle is won. Returns -victory
		// if the battle is lost. Returns 0 otherwise.
		static int64_t win(Team const & team);
		static int64_t sleep_clause(Team const & team);
		// Highest score. 100% chance to win. No particular significance to
		// this number other than being larger than any score a non-winning
		// position can have.
		constexpr static int64_t victory = 30240;
	private:
		int64_t score_team(Team const & team) const;
		int64_t score_active_pokemon(ActivePokemon const & active) const;
		int64_t score_all_pokemon(Team & team, Team const & other, Weather const & weather) const;
		int64_t score_pokemon(Team const & team, Team const & other, Weather const & weather) const;
		int64_t baton_passable_score(ActivePokemon const & pokemon) const;
		int64_t score_status(ActivePokemon const & pokemon) const;
		int64_t score_move (ActivePokemon const & pokemon, Team const & other, Weather const & weather) const;

		int light_screen;
		int lucky_chant;
		int mist;
		int reflect;
		int safeguard;
		int tailwind;

		int wish;

		int spikes;
		int stealth_rock;
		int toxic_spikes;

		int members;
		int hp;
		int hidden;
		int aqua_ring;
		int curse;
		int imprison;
		int ingrain;
		int leech_seed;
		int loaf;
		int magnet_rise;
		int nightmare;
		int substitute;
		int substitute_hp;
		int torment;
		int trapped;

		int burn;
		int freeze;
		int paralysis;
		int poison;
		int sleep;
		
		std::array<int, Stat::END> stage;
		int focus_energy;
	
		int baton_pass;
		int no_pp;
};

} // namespace technicalmachine
#endif // EVALUATE__EVALUATE_HPP_
