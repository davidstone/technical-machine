// Connect to an arbitrary networked Pokemon sim
// Copyright (C) 2014 David Stone
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

// What follows are generic functions and data for connection to any Pokemon
// sim. Specific functions and data are found in the respective sim's folder.

#include "client.hpp"

#include <cstdint>
#include <ctime>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "inmessage.hpp"
#include "outmessage.hpp"

#include "../battle.hpp"
#include "../battle_settings.hpp"

#include "../../exit_program.hpp"
#include "../../settings_file.hpp"

#include "../../evaluate/evaluate.hpp"

namespace technicalmachine {
namespace network {
namespace {

std::vector<std::string> load_highlights ();
std::vector<std::string> load_trusted_users ();
void handle_exit_command();

}	// unnamed namespace

Client::Client(unsigned const depth):
	Base(depth),
	highlights (load_highlights ()),
	trusted_users (load_trusted_users ())
	{
	load_settings (false);
	while (username().empty()) {
		std::cerr << "Add a username and password entry to " + Settings::file_name() + " and hit enter.";
		std::cin.get ();
		load_settings (false);
	}
	connect ();
}

namespace {

std::vector<std::string> create_unsorted_vector (std::string const & file_name) {
	std::vector <std::string> unsorted;
	std::ifstream file (file_name);
	std::string line;
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ())
			unsorted.emplace_back(line);
	}
	file.close();
	return unsorted;
}

std::vector<std::string> create_sorted_vector (std::string const & file_name) {
	// The sorted vector is used to allow std::binary_search to be used on the
	// vector for fast searching. I use a sorted std::vector instead of a
	// std::set because it has faster performance uses less memory. My use
	// pattern is distinct insertion period after which the entire vector can
	// be sorted (faster than inserting into a std::set), followed by lookups
	// (std::binary_search of a std::vector is faster than std::set.find).
	// Analysis of this can be found here: http://lafstern.org/matt/col1.pdf

	// I don't use a sorted vector all of the time because for my other
	// structures, I am either searching for every element in my list against
	// some other text, or am I just picking an element at random, so the
	// sorting would add nothing.

	std::vector<std::string> sorted = create_unsorted_vector (file_name);
	std::sort (sorted.begin(), sorted.end());
	return sorted;
}

std::vector<std::string> load_highlights () {
	return create_unsorted_vector ("settings/highlights.txt");
}

std::vector<std::string> load_trusted_users () {
	return create_sorted_vector ("settings/trusted_users.txt");
}

}	// unnamed namespace

bool Client::is_trusted (std::string const & user) const {
	// I sort the std::vector of trusted users as soon as I load them to make
	// this legal and as fast as possible.
	return std::binary_search (trusted_users.begin(), trusted_users.end (), user);
}

void Client::load_settings (bool const reloading) {
	auto settings = reloading ? Base::load_settings(true) : Settings();
	chattiness = settings.chattiness;
	
	if (!reloading) {
		Server & server = settings.servers.front();
		host = server.host;
		port = server.port;
		current_username = server.username;
		if (server.password.empty()) {
			server.password = random_string(31);
			settings.write();
		}
		current_password = server.password;
	}
}

void Client::send_message(OutMessage & msg) {
	msg.send(*socket);
}

void Client::read_header(InMessage & msg) {
	msg.read_header(*socket, this);
}

void Client::connect () {
	socket = std::make_unique<boost::asio::ip::tcp::socket>(io);
	boost::asio::ip::tcp::resolver resolver (io);
	boost::asio::ip::tcp::resolver::query query (host, port);

	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve (query);
	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error and endpoint_iterator != end) {
		socket->close ();
		socket->connect (*endpoint_iterator++, error);
	}

	if (error) {
		print_with_time_stamp (std::cerr, "Error connecting: " + error.message () + ". Waiting a few seconds and trying again.");
		reconnect ();
	}
}

void Client::reconnect () {
	// Wait a few seconds before reconnecting.
	boost::asio::deadline_timer pause (io, boost::posix_time::seconds (5));
	pause.wait ();
	std::cerr << "Reconnecting.\n";
	connect ();
}

void Client::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) const {
	std::string msg = message;
	boost::to_lower (msg);
	if (is_highlighted (msg)) {
		print_with_time_stamp (std::cout, user + ": " + message);
	}
}

void Client::handle_server_message (std::string const & sender, std::string const & message) const {
	print_with_time_stamp (std::cout, "~~" + sender + "~~: " + message);
}


std::string const & Client::username() const {
	return current_username;
}

std::string const & Client::password() const {
	return current_password;
}

bool Client::is_highlighted (std::string const & message) const {
	// Easiest way I've thought of to see if anything in highlights is in the
	// message is to do a search in the message on each of the elements in
	// highlights. A problem with this approach are that most people want to
	// search for words, not just strings of characters. For instance, if I
	// have "tm" in highlights, I usually don't want to be alerted to someone
	// saying "atm". Fixing this problem probably requires some sort of regex
	// or a fancy word boundary definition.
	for (std::string const & highlight : highlights) {
		if (message.find (highlight) != std::string::npos)
			return true;
	}
	return false;
}

namespace {

bool is_valid_command_structure (std::string const & request) {
	constexpr char command_character = '!';
	return request.length() > 1 and request.front () == command_character;
}

size_t set_target_and_find_message_begin (std::string const & request, size_t start, std::string & target) {
	// This function is used to determine the target of a message (such as who
	// to PM, what channel to join, etc.). The target may be one word with or
	// without quotes, or multiple words with quotes. After it sets target, it
	// returns the position of the beginning of the message.
	size_t const quote1 = request.find ("\"");
	size_t const quote2 = (quote1 == start) ?
		request.find ("\"", quote1 + 1) :
		std::string::npos;
	size_t message_begin = std::string::npos;
	if (quote2 != std::string::npos) {
		target = request.substr (quote1 + 1, quote2 - quote1 - 1);
		if (request.length() > quote2 + 1 + 1)
			message_begin = quote2 + 1 + 1;
	}
	else {
		message_begin = request.find (' ', start);
		target = request.substr (start, message_begin - start);
	}
	return message_begin;
}

}	// unnamed namespace

void Client::handle_private_message(std::string const & sender, std::string const & message) {
	print_with_time_stamp(std::cout, "<PM> " + sender + ": " + message);
	if (is_trusted(sender) and is_valid_command_structure(message))
		do_request(sender, message);
}

void Client::do_request (std::string const & user, std::string const & request) {
	size_t const delimiter_position = request.find (' ');
	std::string const command = request.substr (1, delimiter_position - 1);
	// I may replace this with a version that hashes the command and switches
	// over the hash instead, as this currently compares a lot of strings,
	// which could theoretically become slow with many commands. Probably not a
	// concern, though.
	if (command == "challenge")
		handle_challenge_command (request, delimiter_position + 1);
	else if (command == "depth")
		handle_depth_change_command (user, request, delimiter_position + 1);
	else if (command == "exit" or command == "quit")
		handle_exit_command();
	else if (command == "join")
		handle_join_channel_command (request, delimiter_position + 1);
	else if (command == "message")
		handle_send_message_command (request, delimiter_position + 1);
	else if (command == "part")
		handle_part_channel_command (request, delimiter_position + 1);
	else if (command == "pm")
		handle_send_pm_command (request, delimiter_position + 1);
	else if (command == "reload")
		handle_reload_settings_command ();
}

void Client::handle_challenge_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const opponent = request.substr (start);
	send_battle_challenge (opponent);
}

void Client::handle_depth_change_command (std::string const & user, std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	constexpr unsigned max_depth = 4;
	try {
		auto const depth = boost::lexical_cast<unsigned>(request.substr(start));
		if (depth > max_depth) {
			// Hopefully this will happen rarely enough that declaring
			// big_message static would be a pessimization. There is no need to
			// store that extra memory for a non-performance critical section
			// of code.
			std::string const big_message = "Warning: very large depth requested. Battles will probably time out. Enter a value between 0 and " + std::to_string(max_depth) + " inclusive or proceed at your own risk.";
			std::cerr << big_message + "\n";
			send_private_message (user, big_message);
		}
		set_depth(depth);
	}
	catch (boost::bad_lexical_cast const &) {
		std::string const invalid_depth = "Invalid depth requested. Please enter a number between 0 and " + std::to_string(max_depth) + " inclusive.";
		send_private_message (user, invalid_depth);
	}
}

void Client::handle_join_channel_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const channel = request.substr (start);
	join_channel (channel);
}

void Client::handle_send_message_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string target_channel;
	size_t message_begin = set_target_and_find_message_begin (request, start, target_channel);
	if (message_begin != std::string::npos) {
		std::string message = request.substr (message_begin);
		send_channel_message (target_channel, message);
	}
}

void Client::handle_part_channel_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const channel = request.substr (start);
	part_channel (channel);
}

void Client::handle_send_pm_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string target;
	size_t message_begin = set_target_and_find_message_begin (request, start, target);
	if (message_begin != std::string::npos) {
		std::string message = request.substr (message_begin);
		send_private_message (target, message);
	}
}

namespace {
void handle_exit_command() {
	throw ExitProgram();
}

}	// unnamed namespace

void Client::handle_reload_settings_command () {
	highlights = load_highlights ();
	trusted_users = load_trusted_users ();
	load_settings (true);
}

}	// namespace network
}	// namespace technicalmachine
