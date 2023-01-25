// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>

export module tm.team_predictor.connection;

import tm.boost_beast_http;
import tm.team_predictor.generate_team_builder_ui;
import tm.team_predictor.mutable_buffer;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

// TODO: Change the UI and protocol based on generation?
constexpr auto default_query_string = "generation=8&species0=Select+species&level0=100&gender0=Select+gender&item0=Select+item&ability0=Select+ability&nature0=Select+nature&hp0=0&atk0=0&def0=0&spa0=0&spd0=0&spe0=0&move0_0=Select+move&move0_1=Select+move&move0_2=Select+move&move0_3=Select+move&species1=Select+species&level1=100&gender1=Select+gender&item1=Select+item&ability1=Select+ability&nature1=Select+nature&hp1=0&atk1=0&def1=0&spa1=0&spd1=0&spe1=0&move1_0=Select+move&move1_1=Select+move&move1_2=Select+move&move1_3=Select+move&species2=Select+species&level2=100&gender2=Select+gender&item2=Select+item&ability2=Select+ability&nature2=Select+nature&hp2=0&atk2=0&def2=0&spa2=0&spd2=0&spe2=0&move2_0=Select+move&move2_1=Select+move&move2_2=Select+move&move2_3=Select+move&species3=Select+species&level3=100&gender3=Select+gender&item3=Select+item&ability3=Select+ability&nature3=Select+nature&hp3=0&atk3=0&def3=0&spa3=0&spd3=0&spe3=0&move3_0=Select+move&move3_1=Select+move&move3_2=Select+move&move3_3=Select+move&species4=Select+species&level4=100&gender4=Select+gender&item4=Select+item&ability4=Select+ability&nature4=Select+nature&hp4=0&atk4=0&def4=0&spa4=0&spd4=0&spe4=0&move4_0=Select+move&move4_1=Select+move&move4_2=Select+move&move4_3=Select+move&species5=Select+species&level5=100&gender5=Select+gender&item5=Select+item&ability5=Select+ability&nature5=Select+nature&hp5=0&atk5=0&def5=0&spa5=0&spd5=0&spe5=0&move5_0=Select+move&move5_1=Select+move&move5_2=Select+move&move5_3=Select+move"sv;

export struct Connection {
	explicit Connection(boost::asio::ip::tcp::socket socket):
		m_socket(std::move(socket))
	{
	}

	// The return value is valid until the next call to `read()`.
	auto read() -> std::string_view {
		m_buffer.clear();
		m_http_body = http_read(m_socket, m_buffer);
		return m_http_body.empty() ? default_query_string : std::string_view(m_http_body);
	}
	auto write(std::string_view const original_query, std::string_view const str) -> void {
		auto response = prepare_http_response();
		auto const buffer = response.body().prepare(response.body().capacity());
		auto const size = generate_team_builder_ui(
			original_query,
			str,
			mutable_buffer(
				static_cast<char *>(buffer.data()),
				bounded::assume_in_range<containers::range_size_t<mutable_buffer>>(buffer.size())
			)
		);
		response.body().commit(static_cast<std::size_t>(size));
		http_write(m_socket, response);
	}

private:
	boost::asio::ip::tcp::socket m_socket;
	boost::beast::flat_buffer m_buffer;
	std::string m_http_body;
};

} // namespace technicalmachine
