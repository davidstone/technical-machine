// Generic battle
// Copyright (C) 2011 David Stone
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

#ifndef BATTLE_H_
#define BATTLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "analyze_logs.h"
#include "gender.h"
#include "species.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

namespace network {
class GenericClient;
}

class GenericBattle {
	public:
		Team ai;
		Team foe;
	protected:
		Weather weather;
		Log log;
		int depth;
	public:
		uint8_t party;
	protected:
		GenericBattle (std::string const & opponent, int battle_depth);
	public:
		void handle_begin_turn (uint16_t turn_count);
	protected:
		void update_from_previous_turn (network::GenericClient & client, uint32_t battle_id);
		Move::Moves determine_action (network::GenericClient & client);
	public:
		void handle_use_move (uint8_t moving_party, uint8_t slot, std::string const & nickname, int16_t move_id);
		void handle_withdraw (uint8_t party, uint8_t slot, std::string const & nickname);
		void handle_send_out (uint8_t party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level);
		void handle_health_change (uint8_t party_changing_health, uint8_t slot, int16_t change_in_health, int16_t remaining_health, int16_t denominator);
	private:
		void correct_hp_and_report_errors (Team & team);
	public:
		void handle_set_pp (uint8_t party_changing_pp, uint8_t slot, uint8_t pp);
		void handle_fainted (uint8_t fainting_party, uint8_t slot, std::string const & nickname);
};

} // namespace technicalmachine
#endif // BATTLE_H_
