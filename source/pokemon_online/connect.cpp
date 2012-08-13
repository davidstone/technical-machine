// Connect to Pokemon Online
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

#include "connect.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "battle.hpp"
#include "battle_settings.hpp"
#include "conversion.hpp"
#include "inmessage.hpp"
#include "invalid_user.hpp"
#include "outmessage.hpp"
#include "read_user_info.hpp"

#include "../team.hpp"

#include "../cryptography/md5.hpp"

#include "../network/invalid_channel.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
namespace po {

Client::Client (unsigned set_depth):
	network::GenericClient (set_depth),
	team(generate_team(Battle::pokemon_per_team())) {
	log_in ();
}


void Client::log_in () {
	OutMessage msg (OutMessage::LOG_IN);
	msg.write_team(team, username());
	msg.write_color ();
	bool const ladder_enabled = true;
	msg.write_byte (ladder_enabled);
	bool const show_team = true;
	msg.write_byte (show_team);
	send_message(msg);
}

void Client::run() {
	InMessage msg;
	read_header(msg);

	io.run();
}

void Client::send_keep_alive_message () {
	OutMessage msg (OutMessage::KEEP_ALIVE);
	send_message(msg);
}

namespace {
// Apparently unused in this file...
enum Action {
	CANCEL = 0,
	ATTACK = 1,
	SWITCH = 2,
	REARRANGE = 3,
	CENTER_MOVE = 4,
	DRAW = 5
};
}	// unnamed namespace

void Client::handle_message (InMessage::Message code, InMessage & msg) {
	switch (code) {
		case InMessage::LOG_IN:
			handle_log_in (msg);
			break;
		case InMessage::LOG_OUT:
			handle_log_out (msg);
			break;
		case InMessage::PLAYERS_LIST:
			handle_players_list (msg);
			break;
		case InMessage::SEND_TEAM:
			handle_send_team (msg);
			break;
		case InMessage::CHALLENGE_STUFF:
			handle_challenge_stuff (msg);
			break;
		case InMessage::BATTLE_BEGIN:
			parse_battle_begin (msg);
			break;
		case InMessage::BATTLE_FINISHED:
			parse_battle_finished (msg);
			break;
		case InMessage::BATTLE_MESSAGE:
			handle_battle_message (msg);
			break;
		case InMessage::KEEP_ALIVE:
			// This currently doesn't do anything on the server...
			send_keep_alive_message ();
			break;
		case InMessage::ASK_FOR_PASS:
			parse_ask_for_pass (msg);
			break;
		case InMessage::REGISTER:
			send_registration_message ();
			break;
		case InMessage::PLAYER_KICK:
			handle_player_kick (msg);
			break;
		case InMessage::PLAYER_BAN:
			handle_player_ban (msg);
			break;
		case InMessage::SERV_NUM_CHANGE:
			handle_serv_num_change (msg);
			break;
		case InMessage::SERV_DESC_CHANGE:
			handle_serv_desc_change (msg);
			break;
		case InMessage::SERV_NAME_CHANGE:
			handle_serv_name_change (msg);
			break;
		case InMessage::SEND_PM:
			parse_private_message (msg);
			break;
		case InMessage::AWAY:
			// Someone has gone away.
			break;
		case InMessage::GET_USER_INFO:
			handle_get_user_info (msg);
			break;
		case InMessage::GET_USER_ALIAS:
			handle_get_user_alias (msg);
			break;
		case InMessage::GET_BANLIST:
			handle_get_banlist (msg);
			break;
		case InMessage::CP_BAN:
			handle_cp_ban (msg);
			break;
		case InMessage::CP_UNBAN:
			handle_cp_unban (msg);
			break;
		case InMessage::SPECTATE_BATTLE:
			handle_spectate_battle (msg);
			break;
		case InMessage::SPECTATING_BATTLE_MESSAGE:
			handle_spectating_battle_message (msg);
			break;
		case InMessage::BATTLE_CHAT:
			handle_battle_chat (msg);
			break;
		case InMessage::SPECTATING_BATTLE_CHAT:
			handle_spectating_battle_chat (msg);
			break;
		case InMessage::SPECTATING_BATTLE_FINISHED:
			handle_spectating_battle_finished (msg);
			break;
		case InMessage::LADDER_CHANGE:
			handle_ladder_change (msg);
			break;
		case InMessage::SHOW_TEAM_CHANGE:
			handle_show_team_change (msg);
			break;
		case InMessage::VERSION_CONTROL:
			parse_version_control (msg);
			break;
		case InMessage::TIER_SELECTION:
			handle_tier_selection (msg);
			break;
		case InMessage::SERV_MAX_CHANGE:
			handle_serv_max_change (msg);
			break;
		case InMessage::FIND_BATTLE:
			handle_find_battle (msg);
			break;
		case InMessage::SHOW_RANKINGS:
			handle_show_rankings (msg);
			break;
		case InMessage::ANNOUNCEMENT:
			parse_announcement (msg);
			break;
		case InMessage::CP_TBAN:
			handle_cp_tban (msg);
			break;
		case InMessage::CP_TUNBAN:
			handle_cp_tunban (msg);
			break;
		case InMessage::PLAYER_TBAN:
			handle_player_tban (msg);
			break;
		case InMessage::GET_TBAN_LIST:
			handle_get_tban_list (msg);
			break;
		case InMessage::BATTLE_LIST:
			handle_battle_list (msg);
			break;
		case InMessage::CHANNELS_LIST:
			handle_channels_list (msg);
			break;
		case InMessage::CHANNEL_PLAYERS:
			handle_channel_players (msg);
			break;
		case InMessage::JOIN_CHANNEL:
			handle_join_channel (msg);
			break;
		case InMessage::LEAVE_CHANNEL:
			handle_leave_channel (msg);
			break;
		case InMessage::CHANNEL_BATTLE:
			handle_channel_battle (msg);
			break;
		case InMessage::ADD_CHANNEL:
			parse_add_channel (msg);
			break;
		case InMessage::REMOVE_CHANNEL:
			parse_remove_channel (msg);
			break;
		case InMessage::CHAN_NAME_CHANGE:
			handle_channel_name_change (msg);
			break;
		case InMessage::CHANNEL_MESSAGE:
			parse_channel_message (msg);
			break;
		case InMessage::SERVER_MESSAGE:
			parse_server_message (msg);
			break;
		case InMessage::HTML_MESSAGE:
			handle_html_message (msg);
			break;
		case InMessage::HTML_CHANNEL:
			handle_html_channel (msg);
			break;
		case InMessage::SERVER_NAME:
			parse_server_name (msg);
			break;
		case InMessage::SPECIAL_PASS:
			handle_special_pass (msg);
			break;
		case InMessage::SERVER_LIST_END:
			handle_server_list_end (msg);
			break;
		case InMessage::SET_IP:
			handle_set_ip (msg);
			break;
		default:
			handle_unknown_message_code (code);
			break;
	}
	read_header(msg);
}

namespace {

void print_message_bytes (InMessage & msg) {
	while (msg.index != msg.buffer.size ())
		std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
}

}	// unnamed namespace

void Client::handle_log_in (InMessage & msg) {
	User const user (msg);
	if (user.name == username())
		my_id = user.id;
	else
		print_with_time_stamp (std::cerr, "Server claims my username is " + user.name);
}

void Client::handle_log_out (InMessage & msg) {
	// Only sent to us if we have sent the user a PM.
	uint32_t const user_id = msg.read_int ();
	remove_player (user_id);
}

void Client::handle_players_list (InMessage & msg) {
	// We get this when a logs in or completes a rated battle.
	User const user (msg);
	add_player (user.id, user.name);
}

void Client::handle_send_team (InMessage & msg) {
	// We get this when a user changes their team.
	User const user (msg);
	if (user.id == my_id) {
		// If I get this message about me, it may mean that I just
		// changed my team to the team I wish to use for my pending
		// challenge. Once I get this message to verify that my team
		// change went through, it is safe to send my challenge.
		send_battle_challenge_with_current_team ();
	}
}

namespace {
enum ChallengeDescription {
	SENT = 0,
	ACCEPTED = 1,
	CANCELED = 2,
	BUSY = 3,
	REFUSED = 4,
	INVALID_TEAM = 5,
	DIFFERENT_GENERATION = 6
};
}	// unnamed namespace

void Client::handle_challenge_stuff (InMessage & msg) {
	ChallengeDescription const description = static_cast <ChallengeDescription> (msg.read_byte ());
	uint32_t user_id = msg.read_int ();
	uint32_t const clauses = msg.read_int ();
	uint8_t const mode = msg.read_byte ();
	std::string const & user = get_user_name (user_id);
	if (description == SENT) {
		// Received when they challenge me.
		BattleSettings const settings (clauses, mode);
		handle_incoming_challenge (user, settings);
	}
	else {
		// Received when I challenge them and they refuse / are unable to battle.
		// ACCEPTED is never sent to me.
		constexpr bool accepted = false;
		handle_finalize_challenge (user, accepted);
	}
}

namespace {
class BattlePokemon {
	public:
		std::pair <uint16_t, uint8_t> id;
		std::string nickname;
		uint16_t max_hp;
		uint16_t hp;
		Gender gender;
		bool shiny;
		uint8_t level;
		Item item;
		Ability ability;
		uint8_t happiness;
		std::vector<Moves> moves;
		explicit BattlePokemon (InMessage & msg):
			id (msg.read_short (), msg.read_byte ()),
			nickname (msg.read_string ()),
			max_hp (msg.read_short ()),
			hp (msg.read_short ()),
			gender (id_to_gender (msg.read_byte ())),
			shiny (msg.read_byte ()),
			level (msg.read_byte ()),
			item (id_to_item (msg.read_short ())),
			ability (id_to_ability (msg.read_short ())),
			happiness (msg.read_byte ())
			{
			for (unsigned n = 0; n != 5; ++n) {
				// Something to do with stats. Probably boosts.
				uint16_t const st = msg.read_short ();
			}
			for (unsigned n = 0; n != 4; ++n) {
				moves.emplace_back(id_to_move(msg.read_short()));
				uint8_t const pp = msg.read_byte ();
				uint8_t const total_pp = msg.read_byte ();
			}
			for (unsigned n = 0; n != 6; ++n) {
				// PO uses a QList of int, so hopefully their int is always 32-bit.
				uint32_t const ev = msg.read_int ();
			}
			for (unsigned n = 0; n != 6; ++n) {
				uint32_t const dv = msg.read_int ();
			}
		}
};

class BattleTeam {
	public:
		std::vector <BattlePokemon> pokemon;
		explicit BattleTeam (InMessage & msg) {
			for (unsigned n = 0; n != 6; ++n) {
				pokemon.emplace_back(msg);
			}
		}
};
}	// unnamed namespace

void Client::parse_battle_begin (InMessage & msg) {
	uint32_t const battle_id = msg.read_int ();
	uint32_t const user_id1 = msg.read_int ();
	uint32_t const user_id2 = msg.read_int ();
	if (user_id1 == 0) {
		BattleConfiguration configuration (msg);
		// The server then sends me my own team.
		BattleTeam battle_team (msg);
		handle_battle_begin (battle_id, get_user_name (user_id2));
	}
	else {
		// Another battle has begun on the server.
	}
}

void Client::parse_battle_finished (InMessage & msg) {
	uint32_t const battle_id = msg.read_int ();
	uint8_t const result_code = msg.read_byte ();
	// I completely ignore Close because I determine when I want to
	// close a battle, not my foe. Technical Machine never sends the
	// Close message to allow spectators to talk as long as possible.
	// The server automatically removes the battle when my foe closes
	// it, but that can't be helped.
	if (result_code != 3) {
		uint32_t const winner = msg.read_int ();
		uint32_t const loser = msg.read_int ();
		if (winner == my_id or loser == my_id) {
			handle_battle_end(battle_id, get_result(result_code, winner));
		}
	}
}

void Client::handle_battle_message (InMessage & msg) {
	uint32_t const battle_id = msg.read_int ();
	uint32_t length = msg.read_int ();
	uint8_t const command = msg.read_byte ();
	uint8_t const player = msg.read_byte ();
	length -= (sizeof (command) + sizeof (player));
	auto & battle = static_cast<Battle &>(find_battle (battle_id));
	battle.handle_message (*this, battle_id, command, player, msg);
}

void Client::parse_ask_for_pass (InMessage & msg) {
	std::string const & salt = msg.read_string ();
	authenticate (salt);
}

void Client::handle_player_kick (InMessage & msg) const {
	handle_unimplemented_message (msg, "PLAYER_KICK");
}

void Client::handle_player_ban (InMessage & msg) const {
	handle_unimplemented_message (msg, "PLAYER_BAN");
}

void Client::handle_serv_num_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "SERV_NUM_CHANGE");
}

void Client::handle_serv_desc_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "SERV_DESC_CHANGE");
}

void Client::handle_serv_name_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "SERV_NAME_CHANGE");
}

void Client::parse_private_message (InMessage & msg) {
	uint32_t const user_id = msg.read_int ();
	std::string const message = msg.read_string ();
	handle_private_message (user_id, message);
}

void Client::handle_get_user_info (InMessage & msg) const {
	handle_unimplemented_message (msg, "GET_USER_INFO");
}

void Client::handle_get_user_alias (InMessage & msg) const {
	handle_unimplemented_message (msg, "GET_USER_ALIAS");
}

void Client::handle_get_banlist (InMessage & msg) const {
	handle_unimplemented_message (msg, "GET_BANLIST");
}

void Client::handle_cp_ban (InMessage & msg) const {
	handle_unimplemented_message (msg, "CP_BAN");
}

void Client::handle_cp_unban (InMessage & msg) const {
	handle_unimplemented_message (msg, "CP_UNBAN");
}

void Client::handle_spectate_battle (InMessage & msg) const {
	uint32_t const battle_id = msg.read_int ();
	BattleConfiguration configuration (msg);
}

void Client::handle_spectating_battle_message (InMessage & msg) const {
	handle_unimplemented_message (msg, "SPECTATING_BATTLE_MESSAGE");
}

void Client::handle_battle_chat (InMessage & msg) const {
	handle_unimplemented_message (msg, "BATTLE_CHAT");
}

void Client::handle_spectating_battle_chat (InMessage & msg) const {
	handle_unimplemented_message (msg, "SPECTATING_BATTLE_CHAT");
}

void Client::handle_spectating_battle_finished (InMessage & msg) const {
	handle_unimplemented_message (msg, "SPECTATING_BATTLE_FINISHED");
}

void Client::handle_show_team_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "SHOW_TEAM_CHANGE");
}

void Client::handle_ladder_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "LADDER_CHANGE");
}

void Client::parse_version_control (InMessage & msg) const {
	std::string const server_version = msg.read_string ();
	handle_version_control (server_version);
}

void Client::handle_tier_selection (InMessage & msg) const {
	// Unfortunately, the tier list contains strings, which can be variable-length.
	// Just ignore this and read until the buffer is empty.
	uint32_t const bytes_in_tier_list = msg.read_int ();
	std::vector <std::pair <uint8_t, std::string>> tiers;
	while (msg.index != msg.buffer.size ()) {
		uint8_t const tier_level = msg.read_byte ();
		std::string const tier_name = msg.read_string ();
		tiers.emplace_back(tier_level, tier_name);
	}
}

void Client::handle_serv_max_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "SERV_MAX_CHANGE");
}

void Client::handle_find_battle (InMessage & msg) const {
	handle_unimplemented_message (msg, "FIND_BATTLE");
}

void Client::handle_show_rankings (InMessage & msg) const {
	handle_unimplemented_message (msg, "SHOW_RANKINGS");
}

void Client::parse_announcement (InMessage & msg) const {
	std::string const announcement = msg.read_string ();
	handle_announcement (announcement);
}

void Client::handle_cp_tban (InMessage & msg) const {
	handle_unimplemented_message (msg, "CP_TBAN");
}

void Client::handle_cp_tunban (InMessage & msg) const {
	handle_unimplemented_message (msg, "CP_TUNBAN");
}

void Client::handle_player_tban (InMessage & msg) const {
	handle_unimplemented_message (msg, "PLAYER_TBAN");
}

void Client::handle_get_tban_list (InMessage & msg) const {
	handle_unimplemented_message (msg, "GET_TBAN_LIST");
}

void Client::handle_battle_list (InMessage & msg) {
	// For some reason, PO associates battles with channels
	uint32_t const channel_id = msg.read_int ();
	uint32_t const number_of_battles = msg.read_int ();
	for (unsigned n = 0; n != number_of_battles; ++n)
		add_battle (msg);
}

void Client::handle_channels_list (InMessage & msg) {
	for (uint32_t number = msg.read_int (); number != 0; --number) {
		uint32_t const channel_id = msg.read_int ();
		std::string const channel_name = msg.read_string ();
		handle_add_channel (channel_name, channel_id);
	}
}

void Client::handle_channel_players (InMessage & msg) {
	uint32_t const channel_id = msg.read_int ();
	uint32_t const number_of_players = msg.read_int ();
	std::vector <uint32_t> players;
	players.reserve (number_of_players);
	for (unsigned n = 0; n != number_of_players; ++n)
		players.emplace_back(msg.read_int());
}

void Client::handle_join_channel (InMessage & msg) {
	uint32_t const channel_id = msg.read_int ();
	uint32_t const user_id = msg.read_int ();
}

void Client::handle_leave_channel (InMessage & msg) {
	uint32_t const channel_id = msg.read_int ();
	uint32_t const user_id = msg.read_int ();
	potentially_remove_player (channel_id, user_id);
}

void Client::handle_channel_battle (InMessage & msg) const {
	print_with_time_stamp (std::cerr, "CHANNEL_BATTLE");
	uint32_t const battle_id = msg.read_int ();
	uint32_t const id1 = msg.read_int ();
	uint32_t const id2 = msg.read_int ();
	print_message_bytes (msg);
}

void Client::parse_add_channel (InMessage & msg) {
	std::string const channel_name = msg.read_string ();
	uint32_t const channel_id = msg.read_int ();
	handle_add_channel (channel_name, channel_id);
}

void Client::parse_remove_channel (InMessage & msg) {
	uint32_t const channel_id = msg.read_int ();
	handle_remove_channel (channel_id);
}

void Client::handle_channel_name_change (InMessage & msg) const {
	handle_unimplemented_message (msg, "CHANNEL_NAME_CHANGE");
}

namespace {
void get_speaker_and_message (InMessage & msg, std::string & speaker, std::string & message) {
	// The server sends one string of the form "speaker: message" rather than sending the speaker and the message separately.
	std::string const speaker_and_message = msg.read_string ();
	std::string const delimiter = ": ";
	size_t delimiter_position = speaker_and_message.find (delimiter);
	speaker = speaker_and_message.substr (0, delimiter_position);
	message = speaker_and_message.substr (delimiter_position + delimiter.size());
}
}	// unnamed namespace

void Client::parse_channel_message (InMessage & msg) const {
	uint32_t const channel_id = msg.read_int ();
	std::string speaker;
	std::string message;
	get_speaker_and_message (msg, speaker, message);
	handle_channel_message (channel_id, speaker, message);
}

void Client::parse_server_message (InMessage & msg) const {
	std::string speaker;		// by default, "~~Server~~"
	std::string message;
	get_speaker_and_message (msg, speaker, message);
	handle_server_message (speaker, message);
}

void Client::handle_html_message (InMessage & msg) const {
	std::string const message = msg.read_string ();
	// Possibly do some sort of HTML parsing at some point.
	// For now, just print it.
	print_with_time_stamp (std::cerr, message);
}

void Client::handle_html_channel (InMessage & msg) const {
	handle_unimplemented_message (msg, "HTML_CHANNEL");
}

void Client::parse_server_name (InMessage & msg) const {
	std::string const server_name = msg.read_string ();
	handle_server_name (server_name);
}

void Client::handle_special_pass (InMessage & msg) const {
	handle_unimplemented_message (msg, "SPECIAL_PASS");
}

void Client::handle_server_list_end (InMessage & msg) const {
	handle_unimplemented_message (msg, "SERVER_LIST_END");
}

void Client::handle_set_ip (InMessage & msg) const {
	handle_unimplemented_message (msg, "SET_IP");
}

void Client::handle_unknown_message_code (InMessage::Message const code) const {
	print_with_time_stamp (std::cerr, "Unknown code: " + std::to_string (static_cast<unsigned> (code)));
}

void Client::handle_unimplemented_message (InMessage & msg, std::string const & message_name) const {
	print_with_time_stamp (std::cerr, message_name);
	print_message_bytes (msg);
}

void Client::authenticate (std::string const & salt) {
	OutMessage msg (OutMessage::ASK_FOR_PASS);
	// Pokemon Online bases its hash on lowercase a-f.
	std::string hash = cryptography::md5(password());
	boost::algorithm::to_lower (hash);
	hash = cryptography::md5 (hash + salt);
	boost::algorithm::to_lower (hash);
	msg.write_string (hash);
	send_message(msg);
}

void Client::add_player (uint32_t user_id, std::string const & user_name) {
	// It's safe to call this on a user that already exists. It will just rewrite the same information.
	user_id_to_name [user_id] = user_name;
	user_name_to_id [user_name] = user_id;
}

void Client::remove_player (uint32_t user_id) {
	// I can get to this code when I leave a channel thanks to my quick fix below.
	if (user_id != my_id) {
		auto const it = user_id_to_name.find (user_id);
		if (it != user_id_to_name.end ()) {
			std::string const user_name = it->second;
			user_id_to_name.erase (it);
			user_name_to_id.erase (user_name);
		}
		else {
			std::string const message = "Server requested removing non-existant player: " + std::to_string (user_id);
			print_with_time_stamp (std::cerr, message);
		}
	}
}

void Client::potentially_remove_player (uint32_t channel_id, uint32_t user_id) {
	// Due to Pokemon Online using user ids instead of user names, I have to keep a map.
	// However, I do not receive a "player logged off" message unless I've sent them a PM.
	// This means that I could create some complicated "hasKnowledgeOf" function like PO,
	// but it would still be wrong and a lot more complicated. For now, I'm assuming all
	// players are in channel 0. TODO: come back and make this slightly more accurate.
	if (channel_id == 0)
		remove_player (user_id);
}

void Client::send_battle_challenge (std::string const & opponent) {
	// Due to Pokemon Online's team registration feature, I only support one
	// queued challenge at a time. This seems to be a better option than
	// removing the ability to randomly select a team. I could support multiple
	// challenges and mostly randomized teams, but that would be much more
	// work. It would require using a queue instead of a map.
	try {
		if (!challenges_are_queued() and get_user_id(opponent)) {
			constexpr bool challenger = true;
			auto const & battle = add_pending_challenge<Battle>(opponent, challenger);
			OutMessage msg (OutMessage::SEND_TEAM);
			battle.write_team(msg, username());
			send_message(msg);
		}
	}
	catch (InvalidUser const & error) {
		print_with_time_stamp (std::cerr, error.what ());
	}
}

void Client::send_battle_challenge_with_current_team () {
	try {
		if (challenges_are_queued()) {
			OutMessage msg (OutMessage::CHALLENGE_STUFF);
			uint32_t const user_id = get_user_id(first_challenger());
			uint8_t const generation = 4;		// ???
			std::vector <uint32_t> clauses {
				BattleSettings::FREEZE_CLAUSE,
				BattleSettings::SLEEP_CLAUSE,
				BattleSettings::SPECIES_CLAUSE,
			};
			BattleSettings const settings (clauses, BattleSettings::SINGLES);
			msg.write_challenge (user_id, generation, settings);
			send_message(msg);
		}
	}
	catch (InvalidUser const & error) {
		handle_challenge_withdrawn();
		std::string message = error.what();
		message += " They may have logged out.";
		print_with_time_stamp(std::cerr, message);
	}
}

void Client::handle_finalize_challenge (std::string const & opponent, bool accepted, bool unused) {
	OutMessage msg (OutMessage::CHALLENGE_STUFF);
	std::string verb;
	// See the description of send_battle_challenge() for why I make sure
	// challenges is empty
	if (accepted and !challenges_are_queued()) {
		// They challenged me.
		constexpr bool challenger = false;
		add_pending_challenge<Battle>(team, opponent, challenger);
		msg.write_byte (ACCEPTED);
		verb = "Accepted";
	}
	else {
		handle_challenge_withdrawn();
		msg.write_byte (REFUSED);
		verb = "Rejected";
	}
	// This call to get_user_id is safe and should never throw. opponent has
	// already been validated.
	msg.write_int (get_user_id (opponent));
	send_message(msg);
	print_with_time_stamp (std::cout, verb + " challenge vs. " + opponent);
}

void Client::handle_remove_challenge (std::string const & opponent) {
}

void Client::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	send_message(msg);
}

void Client::part_channel (std::string const & channel) {
	auto const it = channel_to_id.find (channel);
	if (it != channel_to_id.end ()) {
		OutMessage msg (OutMessage::LEAVE_CHANNEL);
		msg.write_int (it->second);
		send_message(msg);
	}
}

void Client::handle_add_channel (std::string const & channel_name, uint32_t channel_id) {
	channel_to_id.insert (std::pair <std::string, uint32_t> (channel_name, channel_id));
	id_to_channel.insert (std::pair <uint32_t, std::string> (channel_id, channel_name));
}

void Client::handle_remove_channel (uint32_t channel_id) {
	auto const it = id_to_channel.find (channel_id);
	if (it != id_to_channel.end()) {
		channel_to_id.erase (it->second);
		id_to_channel.erase (it);
	}
}

void Client::add_battle (InMessage & msg) {
	// Not sure if any of this is relevant. If it is, all_battles will be added to the class.
	std::multimap <uint32_t, uint32_t> all_battles;
	uint32_t const battle_id = msg.read_int ();
	uint32_t const player1 = msg.read_int ();
	uint32_t const player2 = msg.read_int ();
	std::pair <uint32_t, uint32_t> const match1 (player1, battle_id);
	all_battles.insert (match1);
	std::pair <uint32_t, uint32_t> const match2 (player2, battle_id);
	all_battles.insert (match2);
}

void Client::remove_battle (InMessage & msg) {
}

void Client::send_channel_message(std::string const & channel, std::string const & message) {
	auto const it = channel_to_id.find(channel);
	if (it == channel_to_id.end())
		throw network::InvalidChannel(channel);
	uint32_t const channel_id = it->second;
	send_channel_message(channel_id, message);
}

void Client::send_channel_message (uint32_t channel_id, std::string const & message) {
	OutMessage msg (OutMessage::CHANNEL_MESSAGE);
	msg.write_int (channel_id);
	msg.write_string (message);
	send_message(msg);
}

void Client::send_private_message (std::string const & user, std::string const & message) {
	try {
		send_private_message (get_user_id (user), message);
	}
	catch (InvalidUser const & error) {
		print_with_time_stamp (std::cerr, error.what ());
	}
}

void Client::send_private_message (uint32_t user_id, std::string const & message) {
	OutMessage msg (OutMessage::SEND_PM);
	msg.write_int (user_id);
	msg.write_string (message);
	send_message(msg);
}

void Client::send_registration_message () {
	OutMessage msg (OutMessage::REGISTER);
	msg.write_string(password());
	send_message(msg);
}

void Client::handle_version_control (std::string const & server_version) const {
	// Pretend to be the most recent version because this is the standard I'm coding against.
	std::string const version = "1.0.53";
	if (version != server_version) {
		print_with_time_stamp (std::cerr, "Server version is: " + server_version);
		print_with_time_stamp (std::cerr, "User's 'version': " + version);
	}
}

void Client::handle_server_name (std::string const & server_name) const {
	print_with_time_stamp (std::cout, "Server name: " + server_name);
}

void Client::handle_announcement (std::string const & announcement) const {
	print_with_time_stamp (std::cout, announcement);
}

void Client::handle_private_message (uint32_t user_id, std::string const & message) {
	std::string const user = get_user_name (user_id);
	GenericClient::handle_private_message (user, message);
}

uint32_t Client::get_user_id (std::string const & name) const {
	auto const it = user_name_to_id.find (name);
	if (it != user_name_to_id.end ())
		return it->second;
	else
		throw InvalidUser (name);
}

std::string Client::get_user_name (uint32_t id) const {
	auto const it = user_id_to_name.find (id);
	if (it != user_id_to_name.end ())
		return it->second;
	else
		throw InvalidUser (id);
}

void Client::write_team() {
	team = generate_team(Battle::pokemon_per_team());
	OutMessage team_msg(OutMessage::SEND_TEAM);
	team_msg.write_team(team, username());
	send_message(team_msg);
}

Result Client::get_result (uint8_t code, uint32_t winner) const {
	// I completely ignore Close because I determine when I want to close a
	// battle, not my foe. I also ignore Forfeit -- Technical Machine never
	// forfeits; if the foe forfeits, then it is a win.
	if (code != 2)
		return (winner == my_id) ? Result::won : Result::lost;
	else
		return Result::tied;
}

}	// namespace po
}	// namespace technicalmachine
