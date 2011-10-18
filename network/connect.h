// Connect to an arbitrary Pokemon sim
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef NETWORK_CONNECT_H_
#define NETWORK_CONNECT_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "../evaluate.h"
#include "../species.h"

namespace technicalmachine {
namespace network {

class GenericClient {
	protected:
		std::string username;
		std::string password;
	private:
		std::string time_format;
		std::vector <std::string> highlights;
		std::vector <std::string> response;
		std::vector <std::string> trusted_users;
	public:
		int detailed [END_SPECIES][7];
		Score score;
	protected:
//		std::map <std::string, Battle> challenges;		// Battles that have not yet begun
//		std::map <uint8_t, Battle> battles;			// Battles currently underway
		int depth;
	public:
		boost::asio::io_service io;
	private:
		boost::asio::deadline_timer timer;
	public:
		boost::asio::ip::tcp::socket socket;
		int chattiness;
		explicit GenericClient (int depth_);
		virtual ~GenericClient () { }
	private:
		void load_highlights ();
		void load_responses ();
		void load_trusted_users ();
		bool is_trusted (std::string const & user) const;
		void load_account_info (std::string & host, std::string & port);
		void load_settings ();
		void connect (std::string const & host, std::string const & port);
	public:
//		void run ();
	protected:
		void reset_timer (unsigned timer_length);
		virtual void send_keep_alive_message () = 0;
	private:
		virtual void join_channel (std::string const & channel) = 0;
		virtual void part_channel (std::string const & channel) = 0;
	protected:
		void print_with_time_stamp (std::string const & message) const;
	private:
		std::string time_stamp () const;
	protected:
		void handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message);
	private:
		bool is_highlighted (std::string const & message) const;
	protected:
		void handle_private_message (std::string const & sender, std::string const & message);
	private:
		void do_request (std::string const & user, std::string const & request);
		virtual void send_battle_challenge (std::string const & opponent) = 0;
	public:
		virtual void send_channel_message (std::string channel, std::string const & message) = 0;
		virtual void send_channel_message (uint32_t channel_id, std::string const & message) = 0;
		virtual void send_private_message (std::string const & user, std::string const & message) = 0;
		std::string get_response () const;
	private:
		size_t set_target_and_find_message_begin (std::string const & request, std::string const & delimiter, size_t delimiter_position, std::string & target);
	protected:
		void pause_at_start_of_battle ();
};
}
}
#endif  // NETWORK_CONNECT_H_
