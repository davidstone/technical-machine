// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/buffer_view.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <string_view>

namespace technicalmachine {
namespace ps {
namespace http = boost::beast::http;

struct Sockets {
	using tcp = boost::asio::ip::tcp;

	Sockets(std::string_view host, std::string_view port, std::string_view resource);
	Sockets(Sockets &&) = delete;

	auto read_message() -> DelimitedBufferView<std::string_view>;
	void write_message(std::string_view message);
	auto authenticate(std::string_view host, std::string_view port, http::request<http::string_body> const & request) -> http::response<http::string_body>;

private:
	auto make_connected_socket(std::string_view host, std::string_view port) -> tcp::socket;
	boost::beast::flat_buffer m_buffer;
	boost::asio::io_service m_io;
	tcp::socket m_socket;
	boost::beast::websocket::stream<tcp::socket &> m_websocket;
};

} // namespace ps
} // namespace technicalmachine
