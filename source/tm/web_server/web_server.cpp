// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.web_server.web_server;

import tm.boost_networking;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

auto make_acceptor(
	[[clang::lifetimebound]] boost::asio::io_context & context,
	unsigned short const port
) -> tcp::acceptor {
	auto acceptor = tcp::acceptor(context);
	auto const protocol = tcp::v4();
	acceptor.open(protocol);
	acceptor.set_option(tcp::socket::reuse_address(true));
	acceptor.bind(tcp::endpoint(protocol, port));
	acceptor.listen();
	return acceptor;
}

template<typename T>
concept connection = requires(T const & c) { c->is_open(); };

template<typename Function>
concept connection_factory = connection<std::invoke_result_t<Function, tcp::socket &&>>;

export template<connection_factory MakeConnection>
struct WebServer {
	WebServer(
		[[clang::lifetimebound]] boost::asio::io_context & context,
		unsigned short const port,
		MakeConnection make_connection
	):
		m_acceptor(make_acceptor(context, port)),
		m_make_connection(std::move(make_connection))
	{
		accept_connection();
	}
private:
	using ConnectionPtr = std::invoke_result_t<MakeConnection, tcp::socket &&>;
	auto accept_connection() -> void {
		m_acceptor.async_accept([&](
			boost::beast::error_code const ec,
			tcp::socket socket
		) -> void {
			containers::erase_if(m_connections, [](ConnectionPtr const & c) {
				return !c->is_open();
			});
			if (ec) {
				std::cerr << "accept error: " << ec.message() << "\n";
			} else {
				containers::push_back(
					m_connections,
					m_make_connection(std::move(socket))
				);
			}
			accept_connection();
		});
	}

	tcp::acceptor m_acceptor;
	MakeConnection m_make_connection;
	containers::vector<ConnectionPtr> m_connections;
};

} // namespace technicalmachine
