// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.web_server.websocket_server;

import tm.web_server.web_server;

import tm.boost_networking;

import containers;
import std_module;

namespace technicalmachine {

export using OnWebsocketConnect = containers::trivial_inplace_function<
	auto() const -> containers::string,
	sizeof(void *)
>;
export using OnWebsocketMessage = containers::trivial_inplace_function<
	auto(std::string_view) const -> containers::string,
	sizeof(void *) * 2
>;

namespace {
using namespace std::string_view_literals;

struct Connection {
	explicit Connection(
		tcp::socket socket,
		[[clang::lifetimebound]] OnWebsocketConnect const & on_connect,
		[[clang::lifetimebound]] OnWebsocketMessage const & on_message
	):
		m_socket(std::move(socket)),
		m_on_connect(on_connect),
		m_on_message(on_message)
	{
		accept_handshake();
	}

	Connection(Connection &&) = delete;

	auto is_open() const -> bool {
		return m_socket.is_open();
	}

private:
	auto accept_handshake() -> void {
		m_socket.async_accept([&](boost::beast::error_code const ec) {
			if (ec) {
				std::cerr << "Websocket accept error: " << ec.message() << '\n';
				return;
			}
			write(m_on_connect.get()());
		});
	}
	auto write(std::string_view const str) -> void {
		auto buffer = m_buffer.prepare(str.size());
		containers::copy(
			str,
			std::span<char>(
				static_cast<char *>(buffer.data()),
				buffer.size()
			)
		);
		websocket_async_write(
			m_socket,
			buffer,
			[&](boost::beast::error_code const ec, std::size_t) {
				if (ec) {
					std::cerr << "Websocket write error: " << ec.message() << '\n';
					websocket_close(m_socket);
					return;
				}
				m_buffer.clear();
				read();
			}
		);
	}
	auto read() -> void {
		websocket_async_read(
			m_socket,
			m_buffer,
			[&](boost::beast::error_code const ec, std::size_t) {
				if (ec) {
					std::cerr << "Websocket read error: " << ec.message() << '\n';
					websocket_close(m_socket);
					return;
				}
				auto const response = m_on_message.get()(std::string_view(
					static_cast<char const *>(m_buffer.data().data()),
					m_buffer.data().size()
				));
				m_buffer.clear();
				write(response);
			}
		);
	}
	InsecureWebsocket m_socket;
	boost::beast::flat_buffer m_buffer;
	std::reference_wrapper<OnWebsocketConnect const> m_on_connect;
	std::reference_wrapper<OnWebsocketMessage const> m_on_message;
};

struct MakeConnection {
	MakeConnection(
		OnWebsocketConnect on_connect,
		OnWebsocketMessage on_message
	):
		m_on_connect(std::move(on_connect)),
		m_on_message(std::move(on_message))
	{
	}
	auto operator()(tcp::socket socket) const {
		return std::make_unique<Connection>(
			std::move(socket),
			m_on_connect,
			m_on_message
		);
	}
private:
	OnWebsocketConnect m_on_connect;
	OnWebsocketMessage m_on_message;
};

} // namespace

export struct WebSocketServer {
	WebSocketServer(
		[[clang::lifetimebound]] boost::asio::io_context & context,
		unsigned short const port,
		OnWebsocketConnect on_connect,
		OnWebsocketMessage on_message
	):
		m_server(
			context,
			port,
			MakeConnection(std::move(on_connect), std::move(on_message))
		)
	{
	}
private:
	WebServer<MakeConnection> m_server;
};

} // namespace technicalmachine
