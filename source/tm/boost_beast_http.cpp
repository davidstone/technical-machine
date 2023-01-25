// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

export module tm.boost_beast_http;

import std_module;

namespace technicalmachine {

namespace http = boost::beast::http;

export auto prepare_http_response() -> http::response<http::basic_dynamic_body<boost::beast::flat_buffer>> {
	auto response = http::response<http::basic_dynamic_body<boost::beast::flat_buffer>>();
	response.body().reserve(1ULL << 20ULL);
	// response.version(version);
	response.keep_alive(false);
	response.result(http::status::ok);
	response.set(http::field::server, "Beast");
	response.set(http::field::content_type, "text/html");
	return response;
}

export auto http_read(boost::asio::ip::tcp::socket & socket, boost::beast::flat_buffer & buffer) -> std::string {
	auto request = http::request<http::string_body>();
	http::read(socket, buffer, request);
	return std::move(request).body();
}

export auto http_write(boost::asio::ip::tcp::socket & socket, http::response<http::basic_dynamic_body<boost::beast::flat_buffer>> const & response) -> void {
	http::write(socket, response);
}

export auto create_http_post(std::string_view const host, std::string_view const target, std::string_view const data) -> http::request<http::string_body> {
	constexpr auto version = 11U;
	auto request = http::request<http::string_body>(http::verb::post, target, version, data);
	request.set(http::field::host, host);
	request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request.set(http::field::content_type, "application/x-www-form-urlencoded");
	request.prepare_payload();
	return request;
}

namespace ps {

export using AuthenticationSignature = http::response<http::string_body>(
	std::string_view,
	std::string_view,
	http::request<http::string_body> const &
) const;

} // namespace ps
} // namespace technicalmachine
