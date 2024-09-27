// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.client;

import tm.clients.ps.client_message_handler;
import tm.clients.ps.room_message_block;
import tm.clients.ps.sockets;

import tm.strategy.strategy;

import tm.settings_file;

import std_module;

namespace technicalmachine::ps {

export struct Client {
	Client(SettingsFile settings, Strategy strategy):
		m_sockets(settings.host, settings.port, settings.resource),
		m_impl(
			std::move(settings),
			std::move(strategy),
			[&](std::string_view const output) { m_sockets.write_message(output); },
			[&](auto const & ... args) { return m_sockets.authenticate(args...); }
		)
	{
	}

	[[noreturn]] void run() {
		while (true) {
			m_impl.handle_messages(RoomMessageBlock(m_sockets.read_message()));
		}
	}

private:
	Sockets m_sockets;
	ClientMessageHandler m_impl;
};

} // namespace technicalmachine::ps
