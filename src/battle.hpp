// Generic battle
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

#ifndef BATTLE_HPP_
#define BATTLE_HPP_

#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include "gender.hpp"
#include "move.hpp"
#include "species.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace network {
class GenericClient;
class OutMessage;
} // namespace network

class GenericBattle {
	protected:
		std::mt19937 random_engine;
	public:
		std::string opponent;
		Team ai;
		Team foe;
	protected:
		Weather weather;
		std::vector <Species> slot_memory;
		unsigned depth;
		// active and inactive keep track of the Pokemon that are the "major" Pokemon of that message. This helps keep track of information on future messages so I can do things like assign critical hits to the right move.
		Team * active;
		Team * inactive;
		Team * first;
		Team * last;
		bool move_damage;
	public:
		uint8_t party;
	protected:
		GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, std::string const & team_file_name);
		GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, Team const & team);
		virtual ~GenericBattle() {}
	public:
		void handle_begin_turn (uint16_t turn_count) const;
		void handle_request_action (network::GenericClient & client, network::OutMessage & msg, uint32_t battle_id, bool can_switch, std::vector <uint8_t> const & attacks_allowed, bool forced = false);
	protected:
		void update_from_previous_turn (network::GenericClient & client, uint32_t battle_id);
		Move::Moves determine_action (network::GenericClient & client);
	public:
		void handle_use_move (uint8_t moving_party, uint8_t slot, Move::Moves move_name);
		void handle_withdraw (uint8_t switching_party, uint8_t slot, std::string const & nickname) const; // does nothing
		void handle_send_out (uint8_t switching_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level);
	public:
		void handle_health_change (uint8_t party_changing_health, uint8_t slot, int16_t change_in_health, int16_t remaining_health, int16_t denominator);
	private:
		void correct_hp_and_report_errors (Team & team);
		void normalize_hp ();
	public:
		void handle_set_pp (uint8_t party_changing_pp, uint8_t slot, uint8_t pp);
		void handle_fainted (uint8_t fainting_party, uint8_t slot);
	protected:
		uint8_t switch_slot (Move::Moves move) const;
		virtual unsigned get_max_damage_precision () const;
		void initialize_turn ();
	private:
		static void initialize_team (Team & team);
		void do_turn ();
	protected:
		virtual uint8_t get_target () const = 0;
		GenericBattle (GenericBattle const &) = delete;
		GenericBattle & operator= (GenericBattle const &) = delete;
		static constexpr uint8_t unknown_party = 255;
};

}	// namespace technicalmachine
#endif	// BATTLE_HPP_
