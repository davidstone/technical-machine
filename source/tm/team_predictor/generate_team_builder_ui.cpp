// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/generate_team_builder_ui.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species.hpp>
#include <tm/stat/nature.hpp>
#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/ability.hpp>
#include <tm/buffer_view.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/integer_range.hpp>
#include <containers/string.hpp>

namespace technicalmachine {
namespace {

using namespace bounded::literal;
using namespace std::string_view_literals;

template<typename Range>
auto add_dropdown_with_id(std::ostream & output, std::string_view const type, std::string_view const id, Range range, std::string_view const default_value) {
	output <<
		"<select name=\"" << id << "\" id=\"" << id << "\">";
	auto add_option = [&](std::string_view const value) {
		output << "<option" << (value == default_value ? " selected" : "") << ">" << value << "</option>";
	};
	add_option(containers::concatenate<containers::string>("Select "sv, type));
	for (auto const value : range) {
		add_option(to_string(value));
	}
	output << "</select>";
}

template<typename Range>
auto add_dropdown(std::ostream & output, std::string_view const type, std::string_view const index, Range range, std::string_view const default_value) {
	add_dropdown_with_id(output, type, containers::concatenate<containers::string>(type, index), std::move(range), default_value);
}

template<typename Enum>
auto add_dropdown(std::ostream & output, std::string_view const type, std::string_view const index, std::string_view const default_value) {
	add_dropdown(output, type, index, containers::enum_range<Enum>(), default_value);
}

void add_level(std::ostream & output, std::string_view const id, std::string_view const default_value) {
	output <<
		"<label for=\"" << id << "\">Level: </label>"
		"<input type=\"number\" min=\"1\" max=\"100\" value=\"" << default_value << "\" name=\"" << id << "\">";
}

void add_ev(std::ostream & output, std::string_view const id, std::string_view const label, std::string_view const default_value) {
	output <<
		R"(<label for=")" << id << R"(">)" << label << R"( EVs: </label>)"
		R"(<input type="number" min="0" max="255" style="width: 3em" value=")" << default_value << R"(" name=")" << id << R"(">)";
}

} // namespace

void generate_team_builder_ui(std::ostream & output, std::string_view const query_string, std::string_view const generated) {
	output <<
		"<!DOCTYPE html>"
		"<html>"
		"<head><title>Team Builder</title></head>"
		"<body>"
		"<form method=\"post\">";
	auto query_buffer = BufferView(query_string);
	auto next_default = [&]{
		pop_to_delimiter(query_buffer, '=');
		return pop_to_delimiter(query_buffer, '&');
	};
	add_dropdown_with_id(output, "generation", "generation", containers::enum_range<Generation>(), next_default());
	output << "<br>";
	for (auto const pokemon_index : containers::integer_range(max_pokemon_per_team)) {
		auto const index_str = bounded::to_string(pokemon_index);
		add_dropdown<Species>(output, "species", index_str, next_default());
		add_level(output, "level" + index_str, next_default());
		add_dropdown<Gender>(output, "gender", index_str, next_default());
		add_dropdown<Item>(output, "item", index_str, next_default());
		add_dropdown<Ability>(output, "ability", index_str, next_default());
		add_dropdown<Nature>(output, "nature", index_str, next_default());
		output << "<br>";
		add_ev(output, "hp" + index_str, "HP", next_default());
		output << "    ";
		add_ev(output, "atk" + index_str, "Atk", next_default());
		output << "    ";
		add_ev(output, "def" + index_str, "Def", next_default());
		output << "    ";
		add_ev(output, "spa" + index_str, "SpA", next_default());
		output << "    ";
		add_ev(output, "spd" + index_str, "SpD", next_default());
		output << "    ";
		add_ev(output, "spe" + index_str, "Spe", next_default());
		output << "<br>";
		for (auto const move_index : containers::integer_range(max_moves_per_pokemon)) {
			add_dropdown(
				output,
				"move",
				index_str + "_" + bounded::to_string(move_index),
				containers::filter(containers::enum_range<Moves>(), is_regular),
				next_default()
			);
		}
		output << "<br><br>";
	}
	output <<
		"<input type=\"submit\" value=\"Submit\">"
		"</form>";
	output << "<pre>" << generated << "</pre>";
	output << "</body></html>";
}

} // namespace technicalmachine
