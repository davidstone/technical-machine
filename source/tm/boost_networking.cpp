// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/stream_traits.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

export module tm.boost_networking;

import containers;

namespace boost::asio::error {

export using ::boost::asio::error::get_ssl_category;

} // namespace boost::asio::error

namespace boost::asio::ip {

export using ::boost::asio::ip::tcp;

} // namespace boost::asio::ip

namespace boost::asio::ssl {

export using ::boost::asio::ssl::context;
export using ::boost::asio::ssl::stream_base;

} // namespace boost::asio::ssl

namespace boost::asio {

export using ::boost::asio::buffer;
export using ::boost::asio::connect;
export using ::boost::asio::io_context;

} // namespace boost::asio

namespace boost::beast::http {

export using ::boost::beast::http::async_read;
export using ::boost::beast::http::async_write;
export using ::boost::beast::http::empty_body;
export using ::boost::beast::http::error;
export using ::boost::beast::http::field;
export using ::boost::beast::http::read;
export using ::boost::beast::http::request;
export using ::boost::beast::http::response;
export using ::boost::beast::http::status;
export using ::boost::beast::http::string_body;
export using ::boost::beast::http::verb;
export using ::boost::beast::http::write;

} // namespace boost::beast::http

namespace boost::beast {

export using ::boost::beast::async_write;
export using ::boost::beast::error_code;
export using ::boost::beast::flat_buffer;
export using ::boost::beast::get_lowest_layer;
export using ::boost::beast::system_error;

} // namespace boost::beast

namespace technicalmachine {

export namespace http = boost::beast::http;
export using boost::asio::ip::tcp;

export auto http_read(tcp::socket & socket, boost::beast::flat_buffer & buffer) -> http::request<http::string_body> {
	auto request = http::request<http::string_body>();
	http::read(socket, buffer, request);
	return request;
}

export auto http_write(tcp::socket & socket, http::response<http::basic_dynamic_body<boost::beast::flat_buffer>> const & response) -> void {
	http::write(socket, response);
}

export auto create_http_post(std::string_view const host, std::string_view const target, std::string_view const data) -> http::request<http::string_body> {
	constexpr auto version = 11U;
	auto request = http::request<http::string_body>(http::verb::post, target, version, data);
	request.set(http::field::host, host);
	request.set(http::field::user_agent, "Technical Machine");
	request.set(http::field::content_type, "application/x-www-form-urlencoded");
	request.prepare_payload();
	return request;
}

export using Websocket = boost::beast::websocket::stream<boost::beast::ssl_stream<tcp::socket>>;
export using InsecureWebsocket = boost::beast::websocket::stream<tcp::socket>;

// Work around boost headers declaring some functions `static`
export auto websocket_read(Websocket & websocket, boost::beast::flat_buffer & buffer) -> void {
	websocket.read(buffer);
}
// Work around boost headers declaring some functions `static`
export auto websocket_write(Websocket & websocket, std::string_view const str) -> void {
	websocket.write(boost::asio::buffer(str));
}

using Continuation = auto(boost::beast::error_code, std::size_t) const -> void;

// Work around boost headers declaring some functions `static`
export auto websocket_async_read(
	InsecureWebsocket & websocket,
	boost::beast::flat_buffer & buffer,
	containers::trivial_inplace_function<Continuation, sizeof(void *)> const continuation
) -> void {
	websocket.async_read(buffer, continuation);
}
// Work around boost headers declaring some functions `static`
export auto websocket_async_write(
	InsecureWebsocket & websocket,
	boost::asio::const_buffer const buffer,
	containers::trivial_inplace_function<Continuation, sizeof(void *)> const continuation
) -> void {
	websocket.async_write(buffer, continuation);
}

export auto websocket_close(InsecureWebsocket & socket) -> void {
	socket.close(boost::beast::websocket::close_reason());
}

} // namespace technicalmachine

namespace technicalmachine::ps {

export using AuthenticationSignature = auto(
	std::string_view,
	std::string_view,
	http::request<http::string_body> const &
) const -> http::response<http::string_body>;

} // namespace technicalmachine::ps
