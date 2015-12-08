// Pokemon Online battle logic
// Copyright (C) 2015 David Stone
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

#include "outmessage.hpp"
#include "../battle.hpp"

#include <random>
#include <string>

namespace technicalmachine {
namespace po {

struct Client;
struct InMessage;

struct Battle final : ::technicalmachine::Battle {
	template<typename ... Args>
	Battle(std::string const & foe_name, TeamSize const team_size, std::random_device::result_type seed, unsigned const battle_depth, bool const challenger, Args && ... args):
		::technicalmachine::Battle::Battle(foe_name, team_size, seed, battle_depth, std::forward<Args>(args)...),
		action (OutMessage::BATTLE_MESSAGE)
		{
		set_party_if_unknown(Party(BOUNDED_CONDITIONAL(challenger, 0_bi, 1_bi)));
	}
	void handle_message (Client & client, uint32_t battle_id, uint8_t command, Party party, InMessage & msg);
private:
	void parse_begin_turn (InMessage & msg) const;
	void parse_send_out (InMessage & msg, Party party);
	void parse_use_attack (InMessage & msg, Party party);
	void parse_straight_damage (InMessage & msg);
	void parse_hp_change (InMessage & msg, Party party);
	void parse_pp_change (InMessage & msg);
	void parse_effectiveness (InMessage & msg);
	void parse_number_of_hits (InMessage & msg);
	void parse_stat_change (InMessage & msg);
	void parse_status_message (InMessage & msg);
	void parse_status_change (InMessage & msg);
	void parse_abs_status_change (InMessage & msg);
	void parse_already_statused (InMessage & msg);
	void parse_recoil (InMessage & msg);
	void parse_weather_message (InMessage & msg);
	void parse_ability_message (InMessage & msg, Party party);
	void parse_item_message (InMessage & msg, Party party);
	void parse_move_message (InMessage & msg);
	void parse_substitute (InMessage & msg);
	void parse_dynamic_info (InMessage & msg);
	void parse_dynamic_stats (InMessage & msg);
	void parse_spectating (InMessage & msg);
	void parse_temporary_pokemon_change (InMessage & msg);
	void parse_clock_start (InMessage & msg);
	void parse_clock_stop (InMessage & msg);
	void handle_rated (Client & client, InMessage & msg);
	void parse_tier_section (InMessage & msg);
	void parse_spectator_chat (Client const & client, InMessage & msg, uint32_t battle_id);
	void parse_point_estimate (InMessage & msg);
	void parse_offer_choice (Client & client, InMessage & msg, uint32_t battle_id);
	void handle_make_your_choice (Client & client);
	void handle_cancel_move ();
	void parse_rearrange_team (InMessage & msg);
	void parse_spot_shifts (InMessage & msg);
	void parse_battle_end (InMessage & msg);
	VisibleFoeHP max_damage_precision() const  final;
	Party last_attacker;
	OutMessage action;
};

}	// namespace po
}	// namespace technicalmachine
