// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

#include <tm/clients/pokemon_showdown/sockets.hpp>

#include <boost/asio/connect.hpp>
#include <boost/beast/http.hpp>

#include <cstddef>

namespace technicalmachine {
namespace ps {

Sockets::Sockets(std::string_view const host, std::string_view const port, std::string_view const resource):
	m_socket(make_connected_socket(host, port)),
	m_websocket(m_socket)
{
	m_websocket.handshake(
		boost::string_view(host.data(), host.size()),
		boost::string_view(resource.data(), resource.size())
	);
}

auto Sockets::make_connected_socket(std::string_view const host, std::string_view const port) -> tcp::socket {
	auto socket = tcp::socket(m_io);
	auto resolver = tcp::resolver(m_io);
	boost::asio::connect(socket, resolver.resolve(host, port));
	return socket;
}

auto Sockets::read_message() -> DelimitedBufferView<std::string_view> {
	m_buffer.consume(static_cast<std::size_t>(-1));
	m_websocket.read(m_buffer);

	auto const asio_buffer = m_buffer.data();
	auto const sv = std::string_view(static_cast<char const *>(asio_buffer.data()), asio_buffer.size());

	return {sv, '\n'};
}

void Sockets::write_message(std::string_view const message) {
	m_websocket.write(boost::asio::buffer(message));
}

auto Sockets::authenticate(std::string_view const host, std::string_view const port, http::request<http::string_body> const & request) -> http::response<http::string_body> {
	auto socket = make_connected_socket(host, port);

	http::write(socket, request);
	
	auto buffer = boost::beast::flat_buffer{};
	auto response = http::response<http::string_body>{};
	http::read(socket, buffer, response);
	return response;
}

} // namespace ps
} // namespace technicalmachine
