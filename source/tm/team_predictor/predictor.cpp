// Copyright (C) 2019 David Stone
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

#include <tm/team_predictor/generate_team_builder_ui.hpp>
#include <tm/team_predictor/random_team.hpp>
#include <tm/team_predictor/team_predictor.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/team.hpp>

#include <tm/buffer_view.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <bounded/optional.hpp>
#include <bounded/to_integer.hpp>

#include <containers/integer_range.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include <random>
#include <string>
#include <unordered_map>
#include <utility>

#include <iostream>

namespace {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

} // namespace

namespace technicalmachine {
namespace {

using namespace bounded::literal;

auto get_expected_base(std::string_view const input, std::string_view const expected_key) {
	auto [key, value] = split(input, '=');
	if (key != expected_key) {
		throw std::runtime_error("Expected " + std::string(expected_key) + " but got " + std::string(key));
	}
	return value;
}

template<typename T>
auto get_expected(std::string_view const input, std::string const & key, std::string const & index) {
	auto const value = get_expected_base(input, key + index);
	return BOUNDED_CONDITIONAL(value == "Select+" + key, bounded::none, from_string<T>(value));
}

template<typename T>
auto get_expected_integer_wrapper(std::string_view const input, std::string_view const key) {
	return T(bounded::to_integer<typename T::value_type>(get_expected_base(input, key)));
}

struct HTMLTeam {
	Generation generation;
	Team team;
};
auto parse_html_team(std::string_view str) -> HTMLTeam {
	auto buffer = BufferView(str, '&');

	constexpr auto is_me = true;
	auto team = Team(max_pokemon_per_team, is_me);

	auto const generation = Generation(bounded::to_integer<1, 8>(get_expected_base(buffer.next(), "generation")));

	for (auto const index : containers::integer_range(max_pokemon_per_team)) {
		auto const index_str = bounded::to_string(index);
		auto get_integer_wrapper = [&]<typename T>(bounded::detail::types<T>, char const * const key) {
			return get_expected_integer_wrapper<T>(buffer.next(), key + index_str);
		};
		auto get = [&]<typename T>(bounded::detail::types<T>, std::string const & key) {
			return get_expected<T>(buffer.next(), key, index_str);
		};
		auto get_ev = [&](std::string const & key) {
			return get_integer_wrapper(bounded::detail::types<EV>(), key);
		};

		auto species = get(bounded::detail::types<Species>(), "species");
		auto level = get_integer_wrapper(bounded::detail::types<Level>(), "level");
		auto gender = get(bounded::detail::types<Gender>(), "gender");
		auto item = get(bounded::detail::types<Item>(), "item");
		auto ability = get(bounded::detail::types<Ability>(), "ability");
		auto nature = get(bounded::detail::types<Nature>(), "nature");
		auto hp = get_ev("hp");
		auto atk = get_ev("atk");
		auto def = get_ev("def");
		auto spa = get_ev("spa");
		auto spd = get_ev("spd");
		auto spe = get_ev("spe");

		auto moves = containers::static_vector<Moves, max_moves_per_pokemon.value()>();
		for (auto const move_index : containers::integer_range(max_moves_per_pokemon)) {
			auto const move = get_expected<Moves>(buffer.next(), "move", bounded::to_string(index) + "_" + bounded::to_string(move_index));
			if (move) {
				push_back(moves, *move);
			}
		}

		if (hp.value() + atk.value() + def.value() + spa.value() + spd.value() + spe.value() > max_total_evs(generation)) {
			throw std::runtime_error("Too many EVs");
		}

		if (!species) {
			continue;
		}
		auto & pokemon = team.add_pokemon(
			generation,
			*species,
			level,
			gender ? *gender : Gender::genderless
		);
		if (item) {
			pokemon.set_item(*item);
		}
		if (ability) {
			pokemon.set_initial_ability(*ability);
		}
		if (nature) {
			set_nature(pokemon, *nature);
		}
		for (auto const move : moves) {
			add_seen_move(all_moves(pokemon), generation, move);
		}
		set_hp_ev(generation, pokemon, hp);
		set_stat_ev(pokemon, StatNames::ATK, atk);
		set_stat_ev(pokemon, StatNames::DEF, def);
		set_stat_ev(pokemon, StatNames::SPA, spa);
		set_stat_ev(pokemon, StatNames::SPD, spd);
		set_stat_ev(pokemon, StatNames::SPE, spe);
	}

	return {generation, team};
}

struct Data {
	Data(AllUsageStats const & all_usage_stats):
		m_all_usage_stats(all_usage_stats)
	{
	}

	auto team_string(std::string_view const input_data) -> containers::string {
		constexpr auto using_lead = false;
		try {
			auto [generation, team] = parse_html_team(input_data);
			auto const & usage_stats = m_all_usage_stats[generation];
			random_team(generation, usage_stats, team, m_random_engine);
			return to_string(predict_team(
				generation,
				usage_stats,
				LeadStats(using_lead),
				team,
				m_random_engine
			), false);
		} catch (std::exception const & ex) {
			return containers::string(ex.what());
		}
	}

private:
	AllUsageStats const & m_all_usage_stats;
	std::mt19937 m_random_engine{std::random_device()()};
};

struct Connection {
	explicit Connection(AllUsageStats const & all_usage_stats, tcp::socket socket):
		m_data(all_usage_stats),
		m_socket(std::move(socket))
	{
	}

	void process() {
		auto request = Request();
		auto buffer = beast::flat_static_buffer<8192>();
		http::read(m_socket, buffer, request);
		auto const & body = request.body();
		auto const query_string = body.empty() ? default_query_string : std::string_view(body);
		auto response = create_response(request.version(), query_string);
		http::write(m_socket, response);
	}

private:
	using Request = http::request<http::string_body>;
	using Response = http::response<http::dynamic_body>;

	static constexpr auto default_query_string = std::string_view("generation=4&species0=Select+species&level0=100&gender0=Select+gender&item0=Select+item&ability0=Select+ability&nature0=Select+nature&hp0=0&atk0=0&def0=0&spa0=0&spd0=0&spe0=0&move0_0=Select+move&move0_1=Select+move&move0_2=Select+move&move0_3=Select+move&species1=Select+species&level1=100&gender1=Select+gender&item1=Select+item&ability1=Select+ability&nature1=Select+nature&hp1=0&atk1=0&def1=0&spa1=0&spd1=0&spe1=0&move1_0=Select+move&move1_1=Select+move&move1_2=Select+move&move1_3=Select+move&species2=Select+species&level2=100&gender2=Select+gender&item2=Select+item&ability2=Select+ability&nature2=Select+nature&hp2=0&atk2=0&def2=0&spa2=0&spd2=0&spe2=0&move2_0=Select+move&move2_1=Select+move&move2_2=Select+move&move2_3=Select+move&species3=Select+species&level3=100&gender3=Select+gender&item3=Select+item&ability3=Select+ability&nature3=Select+nature&hp3=0&atk3=0&def3=0&spa3=0&spd3=0&spe3=0&move3_0=Select+move&move3_1=Select+move&move3_2=Select+move&move3_3=Select+move&species4=Select+species&level4=100&gender4=Select+gender&item4=Select+item&ability4=Select+ability&nature4=Select+nature&hp4=0&atk4=0&def4=0&spa4=0&spd4=0&spe4=0&move4_0=Select+move&move4_1=Select+move&move4_2=Select+move&move4_3=Select+move&species5=Select+species&level5=100&gender5=Select+gender&item5=Select+item&ability5=Select+ability&nature5=Select+nature&hp5=0&atk5=0&def5=0&spa5=0&spd5=0&spe5=0&move5_0=Select+move&move5_1=Select+move&move5_2=Select+move&move5_3=Select+move");

	auto create_response(unsigned const version, std::string_view const query_string) -> Response {
		auto response = Response();
		response.version(version);
		response.keep_alive(false);

		response.result(http::status::ok);
		response.set(http::field::server, "Beast");
		response.set(http::field::content_type, "text/html");
		auto output = beast::ostream(response.body());
		generate_team_builder_ui(output, query_string, m_data.team_string(query_string));

		return response;
	}

	Data m_data;
	tcp::socket m_socket;
};

} // namespace
} // namespace technicalmachine

int main() {
	auto const all_usage_stats = std::make_unique<technicalmachine::AllUsageStats>();

	auto const address = boost::asio::ip::make_address("127.0.0.1");
	auto const port = static_cast<unsigned short>(46923);

	auto ioc = boost::asio::io_context();

	auto acceptor = tcp::acceptor(ioc, {address, port});
	while (true) {
		auto socket = tcp::socket(ioc);
		acceptor.accept(socket);
		auto connection = technicalmachine::Connection(*all_usage_stats, std::move(socket));
		connection.process();
	}
}
