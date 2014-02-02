// Predict foe's team
// Copyright (C) 2014 David Stone
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

#include "detailed_stats.hpp"
#include "random_team.hpp"
#include "team_predictor.hpp"

#include "ev_optimizer/ev_optimizer.hpp"

#include "ui/input_constants.hpp"
#include "ui/pokemon_inputs.hpp"

#include "../team.hpp"

#include "../pokemon/max_pokemon_per_team.hpp"
#include "../pokemon/pokemon.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Multiline_Output.H>

#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

using namespace technicalmachine;

// A GUI version of the team predictor.

namespace {

class Data {
public:
	Data():
		random_engine(rd()),
		team_ptr(new Team())
		{
	}
	void reset() {
		team_ptr.reset(new Team());
	}
	template<typename... Args>
	void add(Args && ... args) {
		input.emplace_back(std::forward<Args>(args)...);
	}
	Team const & team() const {
		return *team_ptr;
	}
	Team & team() {
		return *team_ptr;
	}
	std::vector<PokemonInputs *> input;
	Fl_Int_Input * random_input;
	Fl_Multiline_Output * output;
	DetailedStats detailed;
	std::random_device rd;
	std::mt19937 random_engine;
private:
	std::unique_ptr<Team> team_ptr;
};

constexpr int number_of_stats = 6;
constexpr int button_width = input_width;
constexpr int button_height = input_height;

constexpr int input_lines_per_pokemon = 4;
constexpr int input_lines_for_random = 2;
constexpr int input_lines_for_button = 1;
constexpr int total_input_lines = max_pokemon_per_team * input_lines_per_pokemon + input_lines_for_random + input_lines_for_button;
constexpr int total_input_height = total_input_lines * (input_height + padding);

constexpr int output_width = 400;
constexpr int height_per_line = 16;
constexpr int output_lines_for_ability = 1;
constexpr int output_lines_for_nature_and_evs = 2;
constexpr int output_lines_for_moves = 4;
constexpr int output_lines_per_pokemon = 1 + output_lines_for_ability + output_lines_for_nature_and_evs + output_lines_for_moves;
constexpr int output_lines_per_team = max_pokemon_per_team * output_lines_per_pokemon;
constexpr int output_team_padding = 10;
constexpr int output_team_height = output_lines_per_team * height_per_line + output_team_padding;
constexpr int output_height = output_team_height;
constexpr int output_x_position = left_padding + ev_input_width * number_of_stats + left_padding;

constexpr int window_width = output_x_position + output_width + padding;
constexpr int window_height = padding + ((total_input_height > output_height) ? total_input_height : output_height + padding);

class PokemonInputValues {
public:
	PokemonInputValues(PokemonInputs const & inputs):
		species(inputs.species()),
		nature(inputs.nature()),
		evs{
			inputs.hp(),
			inputs.atk(),
			inputs.def(),
			inputs.spa(),
			inputs.spd(),
			inputs.spe()
		},
		moves(inputs.moves())
		{
	}
	void add_to_team(Team & team) const {
		team.add_pokemon(species, Level(100_bi), Gender{}, item, ability, nature);
		Pokemon & pokemon = team.replacement();
		get_hp(pokemon).ev = EV(evs[0]);
		for (auto const stat : regular_stats()) {
			get_stat(pokemon, stat).ev = EV(evs[static_cast<unsigned>(stat) + 1u]);
		}
		for (auto const move : moves) {
			pokemon.move.add(move);
		}
	}
private:
	Species species;
	Item item;
	Ability ability;
	Nature nature;
	std::array<EV, number_of_stats> evs;
	std::vector<Moves> moves;
};

class Input {
public:
	template<typename... Args>
	void emplace_back(Args &&... args) {
		pokemon.emplace_back(std::forward<Args>(args)...);
	}
private:
	std::vector<PokemonInputValues> pokemon;
};


unsigned max_random(Data const & data) {
	unsigned const remaining_pokemon = max_pokemon_per_team - data.team().all_pokemon().size();
	try {
		return std::min(boost::lexical_cast<unsigned>(data.random_input->value()), remaining_pokemon);
	}
	catch (boost::bad_lexical_cast const &) {
		return 0u;
	}
}

void generate_random_team(Data & data) {
	random_team(data.team(), data.random_engine, max_random(data));
}

void function (Fl_Widget * w, void * d) {
	Data & data = *reinterpret_cast <Data *> (d);
	bool using_lead = false;
	for (PokemonInputs * inputs : data.input) {
		if (data.team().all_pokemon().size() >= max_pokemon_per_team)
			break;
		if (!inputs->is_valid()) {
			continue;
		}
		PokemonInputValues input(*inputs);
		input.add_to_team(data.team());
		if (inputs == data.input.front()) {
			using_lead = true;
		}
	}
	generate_random_team(data);
	Team team = predict_team(data.detailed, data.team(), data.random_engine, using_lead);
	data.output->value(to_string(team, false).c_str());
	data.reset();
}

}	// unnamed namespace

int main () {
	Fl_Window win (window_width, window_height, "Team Predictor");
		int button_number = 0;
		PokemonInputs input0(button_number);
		PokemonInputs input1(button_number);
		PokemonInputs input2(button_number);
		PokemonInputs input3(button_number);
		PokemonInputs input4(button_number);
		PokemonInputs input5(button_number);
		button_number += 1;
		Fl_Int_Input random_input(left_padding, y_position(button_number), input_width, input_height, "Max random Pokemon");
		random_input.align(FL_ALIGN_TOP);
		// FLTK has a poor interface
		random_input.value("6");
		++button_number;
		Fl_Return_Button calculate (left_padding, y_position(button_number), button_width, button_height, "Calculate");
		++button_number;
		Fl_Multiline_Output output (output_x_position, padding, output_width, output_height);
	win.end();

	Data data;
	
	data.add(&input0);
	data.add(&input1);
	data.add(&input2);
	data.add(&input3);
	data.add(&input4);
	data.add(&input5);
	data.random_input = &random_input;
	data.output = &output;

	calculate.callback (function, &data);
	win.show();

	return Fl::run();
}
