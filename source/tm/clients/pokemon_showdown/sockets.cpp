// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

#include <openssl/ssl.h>

export module tm.clients.ps.sockets;

import containers;
import std_module;

namespace technicalmachine::ps {

using namespace std::string_view_literals;

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
using Websocket = boost::beast::websocket::stream<boost::beast::ssl_stream<tcp::socket>>;

auto make_ssl_context() -> ssl::context {
    auto context = ssl::context(ssl::context::tlsv13_client);
	context.set_default_verify_paths();
	return context;
}

export struct Sockets {
	Sockets(std::string_view const host, std::string_view const port, std::string_view const resource):
		m_ssl(make_ssl_context()),
		m_websocket(m_io, m_ssl)
	{
		auto resolver = tcp::resolver(m_io);
		boost::asio::connect(boost::beast::get_lowest_layer(m_websocket), resolver.resolve(host, port));
		// Set SNI Hostname (many hosts need this to handshake successfully)
		if(!SSL_set_tlsext_host_name(m_websocket.next_layer().native_handle(), std::string(host).c_str())) {
			throw boost::beast::system_error(
				boost::beast::error_code(
					static_cast<int>(::ERR_get_error()),
					boost::asio::error::get_ssl_category()
				),
				"Failed to set SNI Hostname"
			);
		}

        m_websocket.next_layer().handshake(ssl::stream_base::client);

		m_websocket.handshake(
			std::string_view(containers::concatenate<containers::string>(host, ":"sv, port)),
			resource
		);
	}

	Sockets(Sockets &&) = delete;

	auto read_message() -> std::string_view {
		m_buffer.consume(static_cast<std::size_t>(-1));
		m_websocket.read(m_buffer);

		auto const asio_buffer = m_buffer.data();
		auto const sv = std::string_view(static_cast<char const *>(asio_buffer.data()), asio_buffer.size());

		return sv;
	}

	auto write_message(std::string_view const message) -> void {
		m_websocket.write(boost::asio::buffer(message));
	}

	auto authenticate(std::string_view const host, std::string_view const port, http::request<http::string_body> const & request) -> http::response<http::string_body> {
		auto socket = tcp::socket(m_io);
		auto resolver = tcp::resolver(m_io);
		boost::asio::connect(socket, resolver.resolve(host, port));

		http::write(socket, request);

		auto buffer = boost::beast::flat_buffer();
		auto response = http::response<http::string_body>();
		http::read(socket, buffer, response);
		return response;
	}

private:
	boost::beast::flat_buffer m_buffer;
	boost::asio::io_context m_io;
    ssl::context m_ssl;
	Websocket m_websocket;
};

} // namespace technicalmachine::ps
