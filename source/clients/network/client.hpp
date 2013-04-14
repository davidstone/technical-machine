// Connect to an arbitrary networked Pokemon sim
// Copyright (C) 2013 David Stone
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

#ifndef CLIENTS__NETWORK__CLIENT_HPP_
#define CLIENTS__NETWORK__CLIENT_HPP_

#include <cstdint>
#include <string>
#include <vector>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "../battle.hpp"
#include "../client.hpp"

namespace technicalmachine {
class BattleSettings;
class Party;
enum class Result;
namespace network {
class InMessage;
class OutMessage;

class Client : public ::technicalmachine::Client {
	typedef ::technicalmachine::Client Base;
public:
	void handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) const;
	void send_message(OutMessage & msg);
protected:
	explicit Client(unsigned depth);
	void reconnect ();
	virtual void send_channel_message(std::string const & channel, std::string const & message) = 0;
	virtual void send_channel_message(uint32_t channel_id, std::string const & message) = 0;
	virtual void send_private_message(std::string const & user, std::string const & message) = 0;
	void read_header(InMessage & msg);
	template<typename SimulatorInMessage>
	void run_main_service() {
		SimulatorInMessage msg;
		read_header(msg);
		io.run();
	}
	std::string const & username() const;
	std::string const & password() const;
	void handle_server_message (std::string const & sender, std::string const & message) const;
	template<typename Battle, typename ... Args>
	Battle const & add_pending_challenge(Args && ... args) {
		return Base::add_pending_challenge<Battle>(std::forward<Args>(args)...);
	}
	template<typename Timer>
	std::unique_ptr<Timer> make_timer() {
		return std::unique_ptr<Timer>(new Timer(io));
	}
	void handle_private_message (std::string const & sender, std::string const & message);
private:
	bool is_trusted (std::string const & user) const;
	void load_settings (bool reloading);
	void connect ();
	virtual void send_keep_alive_message () = 0;
	virtual void join_channel (std::string const & channel) = 0;
	virtual void part_channel (std::string const & channel) = 0;
	bool is_highlighted (std::string const & message) const;
	void do_request (std::string const & user, std::string const & request);
	void handle_challenge_command (std::string const & request, size_t start);
	void handle_depth_change_command (std::string const & user, std::string const & request, size_t start);
	void handle_join_channel_command (std::string const & request, size_t start);
	void handle_send_message_command (std::string const & request, size_t start);
	void handle_part_channel_command (std::string const & request, size_t start);
	void handle_send_pm_command (std::string const & request, size_t start);
	void handle_reload_settings_command ();

	boost::asio::io_service io;
	std::string host;
	std::string port;
	std::unique_ptr<boost::asio::ip::tcp::socket> socket;
	std::string current_username;
	std::string current_password;
	std::vector <std::string> highlights;
	std::vector <std::string> trusted_users;
	unsigned chattiness;
};

}	// namespace network
}	// namespace technicalmachine
#endif	// CLIENTS__NETWORK__CLIENT_HPP_
