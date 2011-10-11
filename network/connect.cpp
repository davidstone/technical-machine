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

// What follows are generic functions and data for connection to any Pokemon sim. Specific functions and data are found in the respective sim's folder.

#include "connect.h"

#include <cstdint>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

#include "../evaluate.h"
#include "../load_stats.h"

namespace technicalmachine {
namespace network {

GenericClient::GenericClient (int depth_):
	detailed ({{ 0 }}),
	depth (depth_),
	timer (io, boost::posix_time::seconds (45)),
	socket (io)
	{
	srand (static_cast <unsigned> (time (0)));
	load_highlights ();
	load_responses ();
	load_trusted_users ();
	detailed_stats (detailed);
	std::string host;
	std::string port;
	load_account_info (host, port);
	load_settings ();
	connect (host, port);
}

void create_unsorted_vector (std::string const & file_name, std::vector <std::string> & unsorted) {
	unsorted.clear ();
	std::ifstream file (file_name);
	std::string line;
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ())
			unsorted.push_back (line);
	}
	file.close();
}

void GenericClient::load_highlights () {
	create_unsorted_vector ("highlights.txt", highlights);
}

void GenericClient::load_responses () {
	create_unsorted_vector ("responses.txt", response);
}

void create_sorted_vector (std::string const & file_name, std::vector <std::string> & sorted) {
	// The sorted vector is used to allow std::binary_search to be used on the vector for fast searching.
	sorted.clear ();
	std::ifstream file (file_name);
	std::string line;
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ()) {
			boost::algorithm::trim (line);
			sorted.push_back (line);
		}
	}
	std::sort (sorted.begin(), sorted.end());
	file.close();
}

void GenericClient::load_trusted_users () {
	create_sorted_vector ("trusted_users.txt", trusted_users);
}

bool GenericClient::is_trusted (std::string const & user) const {
	// I sort the std::vector of trusted users as soon as I load them to make this legal and as fast as possible.
	return std::binary_search (trusted_users.begin(), trusted_users.end (), user);
}

void GenericClient::load_account_info (std::string & host, std::string & port) {
	std::ifstream file ("settings.txt");
	std::string line;
	std::string const delimiter = ": ";
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ()) {
			size_t position = line.find (delimiter);
			std::string const data = line.substr (0, position);
			if (data == "host")
				host = line.substr (position + delimiter.length());
			else if (data == "port")
				port = line.substr (position + delimiter.length());
			else if (data == "username")
				username = line.substr (position + delimiter.length());
			else if (data == "password")
				password = line.substr (position + delimiter.length());
		}
	}
	file.close();
}

void GenericClient::load_settings () {
	std::ifstream file ("settings.txt");
	std::string line;
	std::string const delimiter = ": ";
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ()) {
			size_t position = line.find (delimiter);
			std::string const data = line.substr (0, position);
			if (data == "chattiness")
				chattiness = boost::lexical_cast <int> (line.substr (position + delimiter.length()));
			else if (data == "time format")
				time_format = line.substr (position + delimiter.length());
		}
	}
	file.close();
}

void GenericClient::connect (std::string const & host, std::string const & port) {
	boost::asio::ip::tcp::resolver resolver (io);
	boost::asio::ip::tcp::resolver::query query (host, port);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve (query);
	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error and endpoint_iterator != end) {
		socket.close();
		socket.connect (*endpoint_iterator++, error);
	}
}

void GenericClient::reset_timer (unsigned timer_length) {
	send_keep_alive_message ();
	timer.expires_from_now (boost::posix_time::seconds (timer_length));
	timer.async_wait (boost::bind (& GenericClient::reset_timer, this, timer_length));
}

void GenericClient::print_with_time_stamp (std::string const & message) const {
	std::cout << "[" + time_stamp () + "] " + message + "\n";
}

std::string GenericClient::time_stamp () const {
	char result [20];
	time_t timer = time (nullptr);
	tm * timeptr = localtime (&timer);
	strftime (result, 20, time_format.c_str(), timeptr);
	return std::string (result);
}

void GenericClient::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) {
	std::string msg = message;
	boost::to_lower (msg);
	if (is_highlighted (msg)) {
		print_with_time_stamp (message);
	}
}

bool GenericClient::is_highlighted (std::string const & message) const {
	for (std::vector<std::string>::const_iterator it = highlights.begin(); it != highlights.end(); ++it) {
		if (message.find (*it) != std::string::npos)
			return true;
	}
	return false;
}

std::string GenericClient::get_response () const {
	return response [rand() % response.size()];
}

size_t GenericClient::set_target_and_find_message_begin (std::string const & request, std::string const & delimiter, size_t found, std::string & target) {
	size_t const quote1 = request.find ("\"");
	size_t quote2 = std::string::npos;
	if (quote1 == found + delimiter.length())
		quote2 = request.find ("\"", quote1 + 1);
	size_t message_begin = std::string::npos;
	if (quote2 != std::string::npos) {
		target = request.substr (quote1 + 1, quote2 - quote1 - 1);
		if (request.length() > quote2 + 1 + delimiter.length())
			message_begin = quote2 + 1 + delimiter.length();
	}
	else {
		message_begin = request.find (delimiter, found + 1);
		target = request.substr (found + delimiter.length (), message_begin - found - delimiter.length());
	}
	return message_begin;
}

void GenericClient::handle_private_message (std::string const & sender, std::string const & message) {
	print_with_time_stamp ("<PM> " + sender + ": " + message);
	if (is_trusted (sender))
		do_request (sender, message);
}

void GenericClient::do_request (std::string const & user, std::string const & request) {
	if (request.length() > 1 and request [0] == '!') {
		std::string const delimiter = " ";
		size_t const found = request.find (delimiter);
		std::string const command = request.substr (1, found - 1);
		if (command == "challenge") {
			if (request.length () >= found + delimiter.length()) {
				std::string const opponent = request.substr (found + delimiter.length ());
				send_battle_challenge (opponent);
			}
		}
		else if (command == "depth") {
			if (request.length () >= found + delimiter.length()) {
				int const new_depth = boost::lexical_cast <int> (request.substr (found + delimiter.length ()));
				if (new_depth < 1) {
					std::string const message = "Invalid depth requested. Please enter a number between 1 and 3 inclusive.";
					send_private_message (user, message);
				}
				else {
					depth = new_depth;
					if (new_depth > 3) {
						std::string const message = "Warning: very large depth requested. Battles will probably time out. Enter a value between 1 and 3 inclusive or proceed at your own risk.";
						std::cerr << message + "\n";
						send_private_message (user, message);
					}
				}
			}
		}
		else if (command == "join") {
			if (request.length () >= found + delimiter.length()) {
				std::string const channel = request.substr (found + delimiter.length ());
				join_channel (channel);
			}
		}
		else if (command == "message") {
			if (request.length () >= found + delimiter.length()) {
				std::string target_channel;
				size_t message_begin = set_target_and_find_message_begin (request, delimiter, found, target_channel);
				if (message_begin != std::string::npos) {
					std::string message = request.substr (message_begin);
					send_channel_message (target_channel, message);
				}
			}
		}
		else if (command == "part") {
			if (request.length () >= found + delimiter.length()) {
				std::string const channel = request.substr (found + delimiter.length ());
				part_channel (channel);
			}
		}
		else if (command == "pm") {
			if (request.length () >= found + delimiter.length()) {
				std::string target;
				size_t message_begin = set_target_and_find_message_begin (request, delimiter, found, target);
				if (message_begin != std::string::npos) {
					std::string message = request.substr (message_begin);
					send_private_message (target, message);
				}
			}
		}
		else if (command == "reload") {
			load_highlights ();
			load_responses ();
			load_trusted_users ();
			load_settings ();
			score.load_evaluation_constants ();
		}
	}
}

void GenericClient::pause_at_start_of_battle () {
	// The bot pauses before it sends actions at the start of the battle to give spectators a chance to join.
	boost::asio::deadline_timer timer (io, boost::posix_time::seconds (10));
	timer.wait ();
}

}
}
