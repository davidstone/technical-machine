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

// What follows are generic functions and data for connection to any Pokemon
// sim. Specific functions and data are found in the respective sim's folder.

#include "connect.hpp"

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

#include "battle_settings.hpp"

#include "../battle.hpp"
#include "../evaluate.hpp"
#include "../exit_program.hpp"
#include "../settings_file.hpp"

namespace technicalmachine {
namespace network {
namespace {

std::vector<std::string> load_highlights ();
std::vector<std::string> load_responses ();
std::vector<std::string> load_trusted_users ();
void handle_exit_command();

}	// unnamed namespace

GenericClient::GenericClient (unsigned set_depth):
	random_engine (rd ()),
	highlights (load_highlights ()),
	responses (load_responses ()),
	trusted_users (load_trusted_users ()),
	depth (set_depth)
	{
	load_settings (false);
	while (username.empty()) {
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

std::vector<std::string> load_responses () {
	return create_unsorted_vector ("settings/responses.txt");
}

std::vector<std::string> load_trusted_users () {
	return create_sorted_vector ("settings/trusted_users.txt");
}

}	// unnamed namespace

bool GenericClient::is_trusted (std::string const & user) const {
	// I sort the std::vector of trusted users as soon as I load them to make
	// this legal and as fast as possible.
	return std::binary_search (trusted_users.begin(), trusted_users.end (), user);
}

void GenericClient::load_settings (bool const reloading) {
	Settings settings;
	team_file_name = settings.team_file;
	chattiness = settings.chattiness;
	time_format = settings.time_format;
	
	if (!reloading) {
		Server & server = settings.servers.front();
		host = server.host;
		port = server.port;
		username = server.username;
		if (server.password.empty()) {
			server.password = get_random_string (31);
			settings.write();
		}
		password = server.password;
	}
}

void GenericClient::connect () {
	socket.reset (new boost::asio::ip::tcp::socket (io));
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

void GenericClient::reconnect () {
	// Wait a few seconds before reconnecting.
	boost::asio::deadline_timer pause (io, boost::posix_time::seconds (5));
	pause.wait ();
	std::cerr << "Reconnecting.\n";
	connect ();
}

void GenericClient::print_with_time_stamp (std::ostream & stream, std::string const & message) const {
	stream << "[" + time_stamp () + "] " + message + "\n";
}

std::string GenericClient::time_stamp () const {
	// There does not appear to be an easy way to format the current time with
	// a format string. This seems like a major limitation of boost::date_time
	// and / or boost::posix_time, as well as the std header chrono.
	std::string result;
	if (!time_format.empty ()) {
		// probably_big_enough is a guess at how big the time stamp will be.
		// It is OK if it is wrong.
		constexpr unsigned probably_big_enough = 30;
		result.resize (probably_big_enough);
		time_t current_time = time (nullptr);
		tm * timeptr = localtime (&current_time);
		while (strftime (&result [0], result.size (), time_format.c_str(), timeptr) == 0)
			result.resize (result.size () * 2);
	}
	return result;
}

void GenericClient::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) const {
	std::string msg = message;
	boost::to_lower (msg);
	if (is_highlighted (msg)) {
		print_with_time_stamp (std::cout, user + ": " + message);
	}
}

void GenericClient::handle_server_message (std::string const & sender, std::string const & message) const {
	print_with_time_stamp (std::cout, "~~" + sender + "~~: " + message);
}


void GenericClient::handle_incoming_challenge (std::string const & opponent, GenericBattleSettings const & settings) {
	bool const accepted = settings.are_acceptable () and is_trusted (opponent);
	constexpr bool challenger = false;
	handle_finalize_challenge (opponent, accepted, challenger);
}

void GenericClient::add_pending_challenge (GenericBattle * battle) {
	battles.add_pending_challenge(battle);
}

void GenericClient::handle_challenge_withdrawn (std::string const & opponent) {
	battles.handle_challenge_withdrawn(opponent);
}

void GenericClient::handle_battle_begin (uint32_t battle_id, std::string const & opponent, uint8_t party) {
	GenericBattle & battle = battles.handle_begin(battle_id, opponent);
	battle.set_if_party_unknown (party);
//	pause_at_start_of_battle ();
}

void GenericClient::pause_at_start_of_battle () {
	// The bot pauses before it sends actions at the start of the battle to
	// give spectators a chance to join.
	boost::asio::deadline_timer pause (io, boost::posix_time::seconds (10));
	pause.wait ();
}

namespace {

std::string get_extension () {
	// TODO: add support for other formats
	return ".sbt";
}

}	// unnamed namespace

void GenericClient::handle_battle_end (uint32_t const battle_id, Result const result) {
	GenericBattle const & battle = battles.find(battle_id);
	battle.handle_end (*this, result);
	battles.handle_end(battle_id);
}

std::string GenericClient::generate_team_file_name () {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	std::string foe_team_file;
	do {
		foe_team_file = "teams/foe/";
		foe_team_file += get_random_string (8);
		foe_team_file += get_extension ();
	} while (boost::filesystem::exists (foe_team_file));
	return foe_team_file;
}

std::string GenericClient::get_random_string (unsigned size) {
	constexpr unsigned range = 36;
	static constexpr char legal_characters [] = "abcdefghijklmnopqrstuvwxyz0123456789";
	std::uniform_int_distribution <unsigned> distribution { 0, range - 1 };
	std::string str;
	str.resize (size);
	for (unsigned n = 0; n != size; ++n)
		str [n] = legal_characters [distribution (random_engine)];
	return str;
}

bool GenericClient::is_highlighted (std::string const & message) const {
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

void GenericClient::send_channel_message (std::string channel, std::string const & message) {
	uint32_t const channel_id = channels.find (channel)->second;
	send_channel_message (channel_id, message);
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

void GenericClient::handle_private_message(std::string const & sender, std::string const & message) {
	print_with_time_stamp(std::cout, "<PM> " + sender + ": " + message);
	if (is_trusted(sender) and is_valid_command_structure(message))
		do_request(sender, message);
}

void GenericClient::do_request (std::string const & user, std::string const & request) {
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

void GenericClient::handle_challenge_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const opponent = request.substr (start);
	send_battle_challenge (opponent);
}

void GenericClient::handle_depth_change_command (std::string const & user, std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	try {
		depth = boost::lexical_cast<unsigned> (request.substr (start));
		if (depth > 3) {
			// Hopefully this will happen rarely enough that declaring
			// big_message static would be a pessimization. There is no need to
			// store that extra memory for a non-performance critical section
			// of code.
			std::string const big_message = "Warning: very large depth requested. Battles will probably time out. Enter a value between 0 and 3 inclusive or proceed at your own risk.";
			std::cerr << big_message + "\n";
			send_private_message (user, big_message);
		}
	}
	catch (boost::bad_lexical_cast const &) {
		std::string const invalid_depth = "Invalid depth requested. Please enter a number between 0 and 3 inclusive.";
		send_private_message (user, invalid_depth);
	}
}

void GenericClient::handle_join_channel_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const channel = request.substr (start);
	join_channel (channel);
}

void GenericClient::handle_send_message_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string target_channel;
	size_t message_begin = set_target_and_find_message_begin (request, start, target_channel);
	if (message_begin != std::string::npos) {
		std::string message = request.substr (message_begin);
		send_channel_message (target_channel, message);
	}
}

void GenericClient::handle_part_channel_command (std::string const & request, size_t start) {
	if (request.length () <= start)
		return;
	std::string const channel = request.substr (start);
	part_channel (channel);
}

void GenericClient::handle_send_pm_command (std::string const & request, size_t start) {
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

void GenericClient::handle_reload_settings_command () {
	highlights = load_highlights ();
	responses = load_responses ();
	trusted_users = load_trusted_users ();
	load_settings (true);
	score.load_evaluation_constants ();
}

void GenericClient::taunt_foe(uint32_t const battle_id) {
	std::uniform_int_distribution<unsigned> distribution { 0, chattiness - 1 };
	if (distribution(random_engine) == 0)
		send_channel_message(battle_id, get_response());
}

std::string GenericClient::get_response () {
	if (!responses.empty()) {
		std::uniform_int_distribution<unsigned> distribution { 0, static_cast<unsigned>(responses.size() - 1) };
		return responses[distribution(random_engine)];
	}
	else {
		return "";
	}
}

}	// namespace network
}	// namespace technicalmachine
