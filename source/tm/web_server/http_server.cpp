// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.web_server.http_server;

import tm.web_server.extension_to_content_type;
import tm.web_server.normalize_requested_file;
import tm.web_server.web_server;

import tm.boost_networking;
import tm.bytes_in_file;
import tm.get_directory;

import containers;
import std_module;
import tv;

namespace technicalmachine {
namespace {
using namespace std::string_view_literals;

auto get_client_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_CLIENT_DIRECTORY"));
}

auto make_response(
	std::filesystem::path const & root,
	http::request<http::empty_body> const & request
) -> http::response<http::string_body> {
	auto make = [&](
		http::status const status,
		std::string_view const body,
		std::string_view const content_type
	) {
		auto response = http::response<http::string_body>(status, request.version());
		response.keep_alive(request.keep_alive());
		response.result(http::status::ok);
		response.set(http::field::server, "Beast");
		response.set(http::field::content_type, content_type);
		response.body() = std::string(body);
		response.prepare_payload();
		return response;
	};
	try {
		if (request.method() != http::verb::get) {
			throw std::runtime_error("Unknown HTTP-method");
		}

		auto const path = normalize_requested_file(
			root,
			std::filesystem::path(request.target())
		);
		auto const content_type = extension_to_content_type(path.extension());

		auto const bytes = bytes_in_file(path);
		auto const size = static_cast<std::size_t>(containers::size(bytes));

		auto const body = std::string_view(
			reinterpret_cast<char const *>(containers::data(bytes)),
			size
		);
		return make(http::status::ok, body, content_type);
	} catch (std::exception const & ex) {
		return make(http::status::bad_request, ex.what(), "text/html"sv);
	}
}

struct Connection {
	explicit Connection(
		tcp::socket socket,
		[[clang::lifetimebound]] std::filesystem::path const & root
	):
		m_socket(std::move(socket)),
		m_root(root)
	{
		do_read();
	}

	Connection(Connection &&) = delete;

	auto is_open() const -> bool {
		return m_socket.is_open();
	}

private:
	auto do_read() -> void {
		m_request = {};
		http::async_read(
			m_socket,
			m_buffer,
			m_request,
			[&](boost::beast::error_code const ec, std::size_t) {
				if (ec) {
					if (ec != http::error::end_of_stream) {
						std::cerr << "read error: " << ec.message() << "\n";
					}
					m_socket.close();
					return;
				}
				m_buffer.clear();
				m_response = make_response(m_root, m_request);
				send_response();
			}
		);
	}

	auto send_response() -> void {
		http::async_write(
			m_socket,
			m_response,
			[&](boost::beast::error_code const ec, std::size_t) {
				if (ec) {
					std::cerr << "write error: " << ec.message() << "\n";
					m_socket.close();
					return;
				}
				if (m_response.keep_alive()) {
					do_read();
				} else {
					m_socket.close();
				}
			}
		);
	}

	tcp::socket m_socket;
	boost::beast::flat_buffer m_buffer;
	http::request<http::empty_body> m_request;
	http::response<http::string_body> m_response;
	std::reference_wrapper<std::filesystem::path const> m_root;
};

struct MakeConnection {
	auto operator()(tcp::socket socket) const {
		return std::make_unique<Connection>(std::move(socket), m_root);
	}
private:
	std::filesystem::path m_root = get_client_directory();
};

} // namespace

export struct HTTPServer {
	HTTPServer(
		[[clang::lifetimebound]] boost::asio::io_context & context,
		unsigned short const port
	):
		m_server(context, port, MakeConnection())
	{
	}
private:
	WebServer<MakeConnection> m_server;
};

} // namespace technicalmachine
