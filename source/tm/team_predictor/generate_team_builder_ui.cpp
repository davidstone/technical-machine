// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string_view>

export module tm.team_predictor.generate_team_builder_ui;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.stat.nature;

import tm.team_predictor.mutable_buffer;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.generation;
import tm.string_conversions.item;
import tm.string_conversions.nature;
import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.ability;
import tm.buffer_view;
import tm.gender;
import tm.generation;
import tm.item;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

auto assume_array_sized(auto const s) {
	return bounded::assume_in_range<containers::range_size_t<mutable_buffer>>(s);
};

constexpr auto remove_prefix(mutable_buffer & buffer, containers::range_size_t<mutable_buffer> const size) -> void {
	buffer = mutable_buffer(
		containers::begin(buffer) + size,
		assume_array_sized(containers::size(buffer) - size)
	);
}
constexpr auto write_data_impl(mutable_buffer & buffer, std::string_view const data) -> void {
	containers::copy(data, buffer);
	remove_prefix(buffer, assume_array_sized(containers::size(data)));
}
constexpr auto write_data(mutable_buffer & buffer, std::same_as<std::string_view> auto ... data) -> void {
	auto const size_to_write = assume_array_sized((... + containers::size(data)));
	if (size_to_write > containers::size(buffer)) {
		throw std::runtime_error("Buffer overrun");
	}
	(..., write_data_impl(buffer, data));
}

template<typename Range>
auto add_dropdown_with_id(mutable_buffer & buffer, std::string_view const type, std::string_view const id, Range range, std::string_view const default_value) {
	write_data(buffer, "<select name=\""sv, id, "\" id=\""sv, id, "\">"sv);
	auto add_option = [&](std::string_view const value) {
		write_data(buffer, "<option"sv, (value == default_value ? " selected"sv : ""sv), ">"sv, value, "</option>"sv);
	};
	add_option(containers::concatenate<containers::string>("Select "sv, type));
	for (auto const value : range) {
		add_option(to_string(value));
	}
	write_data(buffer, "</select>"sv);
}

template<typename Range>
auto add_dropdown(mutable_buffer & buffer, std::string_view const type, std::string_view const index, Range range, std::string_view const default_value) {
	add_dropdown_with_id(buffer, type, containers::concatenate<containers::string>(type, index), std::move(range), default_value);
}

template<typename Enum>
auto add_dropdown(mutable_buffer & buffer, std::string_view const type, std::string_view const index, std::string_view const default_value) {
	add_dropdown(buffer, type, index, containers::enum_range<Enum>(), default_value);
}

void add_level(mutable_buffer & buffer, std::string_view const id, std::string_view const default_value) {
	write_data(buffer,
		"<label for=\""sv, id, "\">Level: </label>"
		"<input type=\"number\" min=\"1\" max=\"100\" value=\""sv, default_value, "\" name=\""sv, id, "\">"sv
	);
}

void add_ev(mutable_buffer & buffer, std::string_view const id, std::string_view const label, std::string_view const default_value) {
	write_data(buffer,
		R"(<label for=")"sv, id, R"(">)"sv, label, R"( EVs: </label>)"
		R"(<input type="number" min="0" max="255" style="width: 3em" value=")"sv, default_value, R"(" name=")"sv, id, R"(">)"sv
	);
}

} // namespace

export auto generate_team_builder_ui(std::string_view const query_string, std::string_view const generated, mutable_buffer buffer) -> containers::range_size_t<mutable_buffer> {
	auto const initial_size = containers::size(buffer);
	write_data(buffer,
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head><title>Team Builder</title></head>\n"
		"<body>\n"
		"<form method=\"post\">\n"sv
	);
	auto query_buffer = BufferView(query_string);
	auto next_default = [&] {
		pop_to_delimiter(query_buffer, '=');
		return pop_to_delimiter(query_buffer, '&');
	};
	add_dropdown_with_id(buffer, "generation", "generation", containers::enum_range<Generation>(), next_default());
	write_data(buffer, "<br>\n"sv);
	for (auto const pokemon_index : containers::integer_range(max_pokemon_per_team)) {
		auto const index_str = containers::to_string(pokemon_index);
		add_dropdown<Species>(buffer, "species", index_str, next_default());
		add_level(buffer, containers::concatenate<containers::string>("level"sv, index_str), next_default());
		add_dropdown<Gender>(buffer, "gender", index_str, next_default());
		add_dropdown<Item>(buffer, "item", index_str, next_default());
		add_dropdown<Ability>(buffer, "ability", index_str, next_default());
		add_dropdown<Nature>(buffer, "nature", index_str, next_default());
		write_data(buffer, "<br>"sv);
		add_ev(buffer, containers::concatenate<containers::string>("hp"sv, index_str), "HP", next_default());
		write_data(buffer, "    "sv);
		add_ev(buffer, containers::concatenate<containers::string>("atk"sv, index_str), "Atk", next_default());
		write_data(buffer, "    "sv);
		add_ev(buffer, containers::concatenate<containers::string>("def"sv, index_str), "Def", next_default());
		write_data(buffer, "    "sv);
		add_ev(buffer, containers::concatenate<containers::string>("spa"sv, index_str), "SpA", next_default());
		write_data(buffer, "    "sv);
		add_ev(buffer, containers::concatenate<containers::string>("spd"sv, index_str), "SpD", next_default());
		write_data(buffer, "    "sv);
		add_ev(buffer, containers::concatenate<containers::string>("spe"sv, index_str), "Spe", next_default());
		write_data(buffer, "<br>"sv);
		for (auto const move_index : containers::integer_range(max_moves_per_pokemon)) {
			add_dropdown(
				buffer,
				"move",
				containers::concatenate<containers::string>(index_str, "_"sv, containers::to_string(move_index)),
				containers::filter(containers::enum_range<MoveName>(), is_regular),
				next_default()
			);
		}
		write_data(buffer, "<br><br>\n"sv);
	}
	write_data(buffer,
		"<input type=\"submit\" value=\"Submit\">\n"
		"</form>\n"sv
	);
	write_data(buffer, "<pre>"sv, generated, "</pre>\n"sv);
	write_data(buffer, "</body>\n</html>"sv);
	return assume_array_sized(initial_size - containers::size(buffer));
}

} // namespace technicalmachine
