// Connect to an arbitrary Pokemon sim
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

#ifndef NETWORK_CONNECT_H_
#define NETWORK_CONNECT_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

#include "../battle.h"
#include "../evaluate.h"
#include "../species.h"

namespace technicalmachine {
namespace network {
class GenericBattleSettings;

class GenericClient {
	public:
		std::string username;
	protected:
		std::string password;
		std::string host;
		std::string port;
	private:
		std::string time_format;
		std::vector <std::string> highlights;
		std::vector <std::string> response;
		std::vector <std::string> trusted_users;
	protected:
		// Battles that have not yet begun
		std::map <std::string, std::shared_ptr <GenericBattle>> challenges;
		// Battles currently underway
		std::map <uint8_t, std::shared_ptr <GenericBattle>> battles;
		std::map <std::string, uint32_t> channels;
	public:
		boost::asio::io_service io;
		Score score;
		int detailed [END_SPECIES][7];
	protected:
		int depth;
	public:
		boost::scoped_ptr <boost::asio::ip::tcp::socket> socket;
		int chattiness;

		explicit GenericClient (int set_depth);
		virtual ~GenericClient () { }
	private:
		void load_highlights ();
		void load_responses ();
		void load_trusted_users ();
		bool is_trusted (std::string const & user) const;
		void load_account_info ();
		void load_settings ();
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
		void handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message);
	protected:
		void handle_server_message (std::string const & sender, std::string const & message) const;
		void handle_incoming_challenge (std::string const & opponent, GenericBattleSettings const & settings);
		void add_pending_challenge (std::shared_ptr <GenericBattle> const & battle);
		void handle_challenge_withdrawn (std::string const & opponent);
		void handle_battle_begin (uint32_t battle_id, std::string const & opponent, uint8_t party = -1);
		void pause_at_start_of_battle ();
		virtual void handle_finalize_challenge (std::string const & opponent, bool accepted, bool challenger) = 0;
	private:
		bool is_highlighted (std::string const & message) const;
	protected:
		void handle_private_message (std::string const & sender, std::string const & message);
	private:
		void do_request (std::string const & user, std::string const & request);
		virtual void send_battle_challenge (std::string const & opponent) = 0;
	public:
		void send_channel_message (std::string channel, std::string const & message);
		virtual void send_channel_message (uint32_t channel_id, std::string const & message) = 0;
		virtual void send_private_message (std::string const & user, std::string const & message) = 0;
		std::string get_response () const;
	protected:
		enum Result {
			WON = 1,
			LOST = -1,
			TIED = 0
		};
		void handle_battle_end (GenericBattle & battle, uint32_t battle_id, Result result);
};
} //namespace network
} // namespace technicalmachine
#endif // NETWORK_CONNECT_H_
