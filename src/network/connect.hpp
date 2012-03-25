// Connect to an arbitrary Pokemon sim
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

#ifndef NETWORK_CONNECT_HPP_
#define NETWORK_CONNECT_HPP_

#include <cstdint>
#include <map>
#include <random>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

#include "../battle.hpp"
#include "../battle_result.hpp"
#include "../evaluate.hpp"
#include "../species.hpp"

namespace technicalmachine {
namespace network {
class GenericBattleSettings;

class GenericClient {
	public:
		std::string username;
		std::string team_file_name;
	protected:
		std::string password;
		std::string host;
		std::string port;
		std::random_device rd;
		std::mt19937 random_engine;
	private:
		std::string time_format;
		std::vector <std::string> highlights;
		std::vector <std::string> responses;
		std::vector <std::string> trusted_users;
	protected:
		// Battles that have not yet begun
		std::map <std::string, std::shared_ptr <GenericBattle>> challenges;
		// Battles currently underway
		std::map <uint32_t, std::shared_ptr <GenericBattle>> battles;
		std::map <std::string, uint32_t> channels;
	public:
		boost::asio::io_service io;
		Score score;
		int detailed [Species::END][7];
	protected:
		unsigned depth;
	public:
		boost::scoped_ptr <boost::asio::ip::tcp::socket> socket;
		unsigned chattiness;

		explicit GenericClient (unsigned set_depth);
		virtual ~GenericClient () { }
	private:
		bool is_trusted (std::string const & user) const;
		void load_settings (bool reloading);
		void connect ();
	public:
		void reconnect ();
	protected:
		virtual void send_keep_alive_message () = 0;
	private:
		virtual void join_channel (std::string const & channel) = 0;
		virtual void part_channel (std::string const & channel) = 0;
	protected:
		void print_with_time_stamp (std::ostream & stream, std::string const & message) const;
	private:
		std::string time_stamp () const;
	public:
		void handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) const;
	protected:
		void handle_server_message (std::string const & sender, std::string const & message) const;
		void handle_incoming_challenge (std::string const & opponent, GenericBattleSettings const & settings);
		void add_pending_challenge (std::shared_ptr <GenericBattle> const & battle);
		void handle_challenge_withdrawn (std::string const & opponent);
		void handle_battle_begin (uint32_t battle_id, std::string const & opponent, uint8_t party = 0xFF);
		void pause_at_start_of_battle ();
		virtual void handle_finalize_challenge (std::string const & opponent, bool accepted, bool challenger) = 0;
	private:
		bool is_highlighted (std::string const & message) const;
	public:
		void send_channel_message (std::string channel, std::string const & message);
		virtual void send_channel_message (uint32_t channel_id, std::string const & message) = 0;
		virtual void send_private_message (std::string const & user, std::string const & message) = 0;
		std::string get_response ();
	protected:
		void handle_battle_end (uint32_t battle_id, Result result);
	private:
		std::string generate_team_file_name ();
		std::string get_random_string (unsigned size);
	protected:
		void handle_private_message (std::string const & sender, std::string const & message);
	private:
		void do_request (std::string const & user, std::string const & request);
		void handle_challenge_command (std::string const & request, size_t start);
		void handle_depth_change_command (std::string const & user, std::string const & request, size_t start);
		void handle_join_channel_command (std::string const & request, size_t start);
		void handle_send_message_command (std::string const & request, size_t start);
		void handle_part_channel_command (std::string const & request, size_t start);
		void handle_send_pm_command (std::string const & request, size_t start);
		void handle_reload_settings_command ();
		virtual void send_battle_challenge (std::string const & opponent) = 0;
};

}	// namespace network
}	// namespace technicalmachine
#endif	// NETWORK_CONNECT_HPP_
