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

#include "battle_result.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
enum class Species : uint16_t;
class DetailedStats;
class Gender;
namespace network {
class GenericClient;
class OutMessage;
} // namespace network

class GenericBattle {
	public:
		std::string opponent;
	protected:
		std::mt19937 random_engine;
		Team ai;
		Team foe;
		std::vector <Species> slot_memory;
	private:
		Weather weather;
		Team * first;
		Team * last;
		unsigned depth;
		bool move_damage;
	protected:
		uint8_t party;
		GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, std::string const & team_file_name);
		GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, Team const & team);
		virtual ~GenericBattle() {}
	public:
		bool is_me (uint32_t other_party) const;
		void set_if_party_unknown (uint8_t new_party);
		void write_team (network::OutMessage & msg, std::string const & username = std::string());
		Team predict_foe_team (DetailedStats const & detailed) const;
		void handle_begin_turn (uint16_t turn_count) const;
		void handle_request_action (network::GenericClient & client, network::OutMessage & msg, uint32_t battle_id, bool can_switch, std::vector <uint8_t> const & attacks_allowed, bool forced = false);
		void handle_use_move (uint8_t moving_party, uint8_t slot, Moves move_name);
		void handle_send_out (uint8_t switching_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level);
		void handle_hp_change (uint8_t party_changing_hp, uint8_t slot, uint16_t change_in_hp, uint16_t remaining_hp, uint16_t denominator);
		void handle_set_pp (uint8_t party_changing_pp, uint8_t slot, uint8_t pp);
		void handle_fainted (uint8_t fainting_party, uint8_t slot);
		void handle_end (network::GenericClient & client, Result const result) const;
	protected:
		void update_from_previous_turn (network::GenericClient & client, uint32_t battle_id);
		Moves determine_action (network::GenericClient & client);
		uint8_t switch_slot (Moves move) const;
		virtual uint16_t max_damage_precision () const;
		void initialize_turn ();
		virtual uint8_t get_target () const = 0;
		static constexpr uint8_t unknown_party = 255;
	private:
		void correct_hp_and_report_errors (Team & team);
		void normalize_hp ();
		static void initialize_team (Team & team);
		void do_turn ();
		GenericBattle (GenericBattle const &) = delete;
		GenericBattle & operator= (GenericBattle const &) = delete;
};

}	// namespace technicalmachine
#endif	// BATTLE_HPP_
