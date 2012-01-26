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

// What follows are generic functions and data for connection to any Pokemon sim. Specific functions and data are found in the respective sim's folder.

#include "connect.hpp"

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

#include "battle_settings.hpp"

#include "../battle.hpp"
#include "../evaluate.hpp"
#include "../load_stats.hpp"
#include "../teampredictor.hpp"

#include "../pokemon_lab/write_team_file.hpp"

namespace technicalmachine {
namespace network {

GenericClient::GenericClient (int set_depth):
	random_engine (rd ()),
	detailed ({{ 0 }}),
	depth (set_depth)
	{
	load_highlights ();
	load_responses ();
	load_trusted_users ();
	detailed_stats (detailed);
	load_account_info ();
	while (username.empty()) {
		std::cerr << "Add a username and password entry to settings/settings.txt and hit enter.";
		std::cin.get ();
		load_account_info ();
	}
	load_settings ();
	connect ();
}

namespace {

static void create_unsorted_vector (std::string const & file_name, std::vector <std::string> & unsorted) {
	// First, clear the vector so this can be used to reload an already filled
	// vector as well. This allows the user to reload settings from a file
	// without restarting TM.
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

static void create_sorted_vector (std::string const & file_name, std::vector <std::string> & sorted) {
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
}	// anonymous namespace

void GenericClient::load_highlights () {
	create_unsorted_vector ("settings/highlights.txt", highlights);
}

void GenericClient::load_responses () {
	create_unsorted_vector ("settings/responses.txt", response);
}

void GenericClient::load_trusted_users () {
	create_sorted_vector ("settings/trusted_users.txt", trusted_users);
}

bool GenericClient::is_trusted (std::string const & user) const {
	// I sort the std::vector of trusted users as soon as I load them to make
	// this legal and as fast as possible.
	return std::binary_search (trusted_users.begin(), trusted_users.end (), user);
}

void GenericClient::load_account_info () {
	std::ifstream file ("settings/settings.txt");
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
	// This is broken off from load_account_info so allow this to be reloaded
	// while the program is running.
	std::ifstream file ("settings/settings.txt");
	std::string line;
	std::string const delimiter = ": ";
	std::string const comment = "//";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.substr (0, comment.length ()) != comment and !line.empty ()) {
			size_t position = line.find (delimiter);
			std::string const data = line.substr (0, position);
			if (data == "chattiness")
				chattiness = std::stoi (line.substr (position + delimiter.length()));
			else if (data == "time format")
				time_format = line.substr (position + delimiter.length());
		}
	}
	file.close();
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

void GenericClient::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) {
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

void GenericClient::add_pending_challenge (std::shared_ptr <GenericBattle> const & battle) {
	challenges.insert (std::pair <std::string, std::shared_ptr <GenericBattle>> (battle->foe.player, battle));
}

void GenericClient::handle_challenge_withdrawn (std::string const & opponent) {
	challenges.erase (opponent);
}

void GenericClient::handle_battle_begin (uint32_t battle_id, std::string const & opponent, uint8_t party) {
 	std::shared_ptr <GenericBattle> battle = challenges.find (opponent)->second;
 	if (battle->party == static_cast <uint8_t> (-1))
		battle->party = party;
	battles.insert (std::pair <uint32_t, std::shared_ptr <GenericBattle>> (battle_id, battle));
	challenges.erase (opponent);
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

void GenericClient::handle_battle_end (uint32_t battle_id, Result result) {
	auto const it = battles.find (battle_id);
	if (it != battles.end ()) {
		GenericBattle & battle = *it->second;
		std::string verb;
		switch (result) {
			case WON:
				verb = "Won";
				break;
			case LOST:
				verb = "Lost";
				break;
			case TIED:
				verb = "Tied";
				break;
		}
		print_with_time_stamp (std::cout, verb + " a battle vs. " + battle.foe.player);
		if (result == LOST) {
			std::string foe_team_file = "teams/foe/";
			foe_team_file += get_random_string ();
			foe_team_file += get_extension ();
			// TODO: add support for other formats
			Team predicted = battle.foe;
			predict_team (detailed, predicted, battle.ai.size);
			pl::write_team (predicted, foe_team_file);
		}
		battles.erase (battle_id);
	}
}

std::string GenericClient::get_random_string () {
	constexpr unsigned range = 36;
	static constexpr char legal_characters [range] = "ABCDEFGHIJKLMNOPQSTUVWXYZ0123456789";
	std::uniform_int_distribution <unsigned> distribution { 0, range - 1 };
	std::string str;
	str.resize (8);
	for (unsigned n = 0; n != 8; ++n)
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

std::string GenericClient::get_response () {
	if (response.size() > 0) {
		std::uniform_int_distribution <unsigned> distribution { 0, static_cast <unsigned> (response.size() - 1) };
		return response [distribution (random_engine)];
	}
	else {
		return "";
	}
}

void GenericClient::send_channel_message (std::string channel, std::string const & message) {
	uint32_t const channel_id = channels.find (channel)->second;
	send_channel_message (channel_id, message);
}

namespace {
static size_t set_target_and_find_message_begin (std::string const & request, std::string const & delimiter, size_t delimiter_position, std::string & target) {
	// This function is used to determine the target of a message (such as who
	// to PM, what channel to join, etc.). The target may be one word with or
	// without quotes, or multiple words with quotes. After it sets target, it
	// returns the position of the beginning of the message.
	size_t const quote1 = request.find ("\"");
	size_t quote2 = std::string::npos;
	if (quote1 == delimiter_position + delimiter.length())
		quote2 = request.find ("\"", quote1 + 1);
	size_t message_begin = std::string::npos;
	if (quote2 != std::string::npos) {
		target = request.substr (quote1 + 1, quote2 - quote1 - 1);
		if (request.length() > quote2 + 1 + delimiter.length())
			message_begin = quote2 + 1 + delimiter.length();
	}
	else {
		message_begin = request.find (delimiter, delimiter_position + 1);
		target = request.substr (delimiter_position + delimiter.length (), message_begin - delimiter_position - delimiter.length());
	}
	return message_begin;
}
}	// anonymous namespace

void GenericClient::handle_private_message (std::string const & sender, std::string const & message) {
	print_with_time_stamp (std::cout, "<PM> " + sender + ": " + message);
	if (is_trusted (sender))
		do_request (sender, message);
}

void GenericClient::do_request (std::string const & user, std::string const & request) {
	if (request.length() > 1 and request [0] == '!') {
		std::string const delimiter = " ";
		size_t const delimiter_position = request.find (delimiter);
		std::string const command = request.substr (1, delimiter_position - 1);
		// I may replace this with a version that hashes the command and
		// switches over the hash instead, as this currently compares a lot of
		// strings, which could theoretically become slow with many commands.
		// Probably not a concern, though.
		if (command == "challenge") {
			if (request.length () >= delimiter_position + delimiter.length()) {
				std::string const opponent = request.substr (delimiter_position + delimiter.length ());
				send_battle_challenge (opponent);
			}
		}
		else if (command == "depth") {
			if (request.length () >= delimiter_position + delimiter.length()) {
				int const new_depth = std::stoi (request.substr (delimiter_position + delimiter.length ()));
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
			if (request.length () >= delimiter_position + delimiter.length()) {
				std::string const channel = request.substr (delimiter_position + delimiter.length ());
				join_channel (channel);
			}
		}
		else if (command == "message") {
			if (request.length () >= delimiter_position + delimiter.length()) {
				std::string target_channel;
				size_t message_begin = set_target_and_find_message_begin (request, delimiter, delimiter_position, target_channel);
				if (message_begin != std::string::npos) {
					std::string message = request.substr (message_begin);
					send_channel_message (target_channel, message);
				}
			}
		}
		else if (command == "part") {
			if (request.length () >= delimiter_position + delimiter.length()) {
				std::string const channel = request.substr (delimiter_position + delimiter.length ());
				part_channel (channel);
			}
		}
		else if (command == "pm") {
			if (request.length () >= delimiter_position + delimiter.length()) {
				std::string target;
				size_t message_begin = set_target_and_find_message_begin (request, delimiter, delimiter_position, target);
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

} // namespace network
} // namespace technicalmachine
