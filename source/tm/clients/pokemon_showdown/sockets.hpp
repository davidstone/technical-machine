// Copyright (C) 2020 David Stone
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
