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

#ifndef EVALUATE_HPP_
#define EVALUATE_HPP_

#include <cstdint>

namespace technicalmachine {

class Pokemon;
class Status;
class Team;
class Weather;

// All variables within Score do not change during use. I do not declare them
// const because the values are determined by reading a text file, so they
// aren't quite known at initialization time. Can be updated between battles.
class Score {
	public:
		// Highest score. 100% chance to win. No particular significance to
		// this number other than being larger than any score a non-winning
		// position can have.
		constexpr static int64_t VICTORY = 65536;
		
		int transposition_table;	
	private:
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
		
		int atk_stage;
		int def_stage;
		int spa_stage;
		int spd_stage;
		int spe_stage;
		int focus_energy;
	
		int baton_pass;
		int no_pp;
	public:
		Score ();
		void load_evaluation_constants ();
		// ai and foe are both logically constant. I change the active Pokemon
		// in each of them, but I change it back before the function returns.
		int64_t evaluate (Team & ai, Team & foe, Weather const & weather) const;
	private:
		int64_t score_team (Team const & team) const;
		int64_t score_pokemon (Team const & ai, Team const & foe, Weather const & weather) const;
		int64_t score_status (Team const & team) const;
		int64_t score_move (Team const & ai, Team const & foe, Weather const & weather) const;
	public:
		static int64_t win (Team const & team);
};

} // namespace technicalmachine
#endif // EVALUATE_HPP_
