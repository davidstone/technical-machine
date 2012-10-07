// Predict foe's team
// Copyright (C) 2012 David Stone
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

#include <vector>

#include "detailed_stats.hpp"
#include "team_predictor.hpp"

#include "../string_conversions/invalid_string_conversion.hpp"
#include "../string_conversions/conversion.hpp"
#include "../team.hpp"

#include "../pokemon/pokemon.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>

using namespace technicalmachine;

// A GUI version of the team predictor.

namespace {

class Data {
	public:
		std::vector<Fl_Input *> input;
		Fl_Multiline_Output * output;
		DetailedStats detailed;
};

void function (Fl_Widget * w, void * d) {
	Data & data = *reinterpret_cast <Data *> (d);
	
	Team team;
	bool using_lead = false;
	for (Fl_Input * in : data.input) {
		Species species;
		try {
			species = from_string<Species>(in->value());
			constexpr unsigned level = 100;
			Gender const gender(Gender::MALE);
			team.add_pokemon(species, level, gender);
		}
		catch (InvalidFromStringConversion const &) {
			species = Species::END;
		}
		if (in == data.input.front ()) {
			using_lead = (species != Species::END);
		}
	}
	
	if (!team.all_pokemon().is_empty()) {
		team = predict_team (data.detailed, team, using_lead);
		data.output->value (team.to_string(false).c_str());
	}
	else
		data.output->value ("");
}
}	// unnamed namespace

int main () {
	constexpr int left_padding = 50;
	constexpr int padding = 10;
	constexpr int input_width = 120;
	constexpr int input_height = 30;
	constexpr int button_width = input_width;
	constexpr int button_height = input_height;
	auto const y_position = [](int const button_number) {
		return (1 + button_number) * padding + button_number * input_height;
	};
	
	constexpr int output_width = 320;
	constexpr int height_per_line = 16;
	constexpr int lines_for_ability = 1;
	constexpr int lines_for_moves = 4;
	constexpr int lines_per_pokemon = 1 + lines_for_ability + lines_for_moves;
	constexpr int pokemon_per_team = 6;
	constexpr int lines_per_team = pokemon_per_team * lines_per_pokemon;
	constexpr int team_padding = 10;
	constexpr int team_height = lines_per_team * height_per_line + team_padding;
	constexpr int output_height = team_height;
	constexpr int output_x_position = left_padding + input_width + left_padding;
	
	constexpr int window_width = output_x_position + output_width + padding;
	constexpr int window_height = padding + output_height + padding;
	Fl_Window win (window_width, window_height, "Team Predictor");
		Fl_Input input0 (left_padding, y_position(0), input_width, input_height, "Input");
		Fl_Input input1 (left_padding, y_position(1), input_width, input_height);
		Fl_Input input2 (left_padding, y_position(2), input_width, input_height);
		Fl_Input input3 (left_padding, y_position(3), input_width, input_height);
		Fl_Input input4 (left_padding, y_position(4), input_width, input_height);
		Fl_Input input5 (left_padding, y_position(5), input_width, input_height);
		Fl_Return_Button calculate (left_padding, y_position(6), button_width, button_height, "Calculate");
		Fl_Multiline_Output output (output_x_position, padding, output_width, output_height);
	win.end();

	Data data;
	
	data.input.emplace_back(&input0);
	data.input.emplace_back(&input1);
	data.input.emplace_back(&input2);
	data.input.emplace_back(&input3);
	data.input.emplace_back(&input4);
	data.input.emplace_back(&input5);
	data.output = &output;

	calculate.callback (function, &data);
	win.show();

	return Fl::run();
}
