// Predict foe's team
// Copyright (C) 2015 David Stone
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

#include <containers/array/array.hpp>
#include <containers/vector/vector.hpp>

#include <fltk/FL/Fl.H>
#include <fltk/FL/Fl_Window.H>
#include <fltk/FL/Fl_Return_Button.H>
#include <fltk/FL/Fl_Int_Input.H>
#include <fltk/FL/Fl_Multiline_Output.H>

#include <random>

using namespace technicalmachine;
using namespace bounded::literal;

// A GUI version of the team predictor.

namespace {

using AllPokemonInputs = containers::array<PokemonInputs, max_pokemon_per_team.value()>;

struct Data {
	explicit Data(AllPokemonInputs const & inputs_, Fl_Multiline_Output & output_):
		inputs(inputs_),
		output(output_),
		random_engine(rd()),
		m_team(max_pokemon_per_team)
		{
	}
	Team & team() {
		return m_team;
	}
	AllPokemonInputs const & inputs;
	Fl_Multiline_Output & output;
	DetailedStats detailed;
	std::random_device rd;
	std::mt19937 random_engine;
private:
	Team m_team;
};

constexpr int number_of_stats = 6;
constexpr int button_height = input_height;

constexpr int input_lines_per_pokemon = 4;
constexpr int total_input_lines = static_cast<int>(max_pokemon_per_team) * input_lines_per_pokemon;
constexpr int total_input_height = total_input_lines * (input_height + padding);

constexpr int output_width = 400;
constexpr int height_per_line = 16;
constexpr int output_lines_for_ability = 1;
constexpr int output_lines_for_nature_and_evs = 2;
constexpr int output_lines_for_moves = 4;
constexpr int output_lines_per_pokemon = 1 + output_lines_for_ability + output_lines_for_nature_and_evs + output_lines_for_moves;
constexpr int output_lines_per_team = static_cast<int>(max_pokemon_per_team) * output_lines_per_pokemon;
constexpr int output_team_padding = 10;
constexpr int output_team_height = output_lines_per_team * height_per_line + output_team_padding;
constexpr int output_height = output_team_height;
constexpr int output_x_position = left_padding + ev_input_width * number_of_stats + left_padding;
constexpr int total_output_height = output_height + padding + button_height;

constexpr int window_width = output_x_position + output_width + padding;
constexpr int window_height = padding + bounded::max(total_input_height, total_output_height) + padding;

struct PokemonInputValues {
	PokemonInputValues(PokemonInputs const & inputs):
		species(inputs.species()),
		// TODO: load actual item?
		item(Item::No_Item),
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
		team.add_pokemon(species, Level(100_bi), Gender::genderless, item, ability, nature);
		Pokemon & pokemon = team.replacement();
		set_hp_ev(pokemon, evs[0_bi]);
		for (auto const stat : regular_stats()) {
			set_stat_ev(pokemon, stat, containers::at(evs, bounded::integer(stat) + 1_bi));
		}
		for (auto const move : moves) {
			all_moves(pokemon).emplace_back(move);
		}
	}
private:
	Species species;
	Item item;
	Ability ability;
	Nature nature;
	containers::array<EV, number_of_stats> evs;
	containers::static_vector<Moves, max_moves_per_pokemon.value()> moves;
};


void generate_random_team(Data & data) {
	random_team(data.team(), data.random_engine);
}

void function (Fl_Widget *, void * d) {
	auto & data = *reinterpret_cast<Data *> (d);
	bool using_lead = false;
	for (PokemonInputs const & inputs : data.inputs) {
		if (size(data.team().all_pokemon()) >= max_pokemon_per_team)
			break;
		if (!inputs.is_valid()) {
			continue;
		}
		PokemonInputValues input(inputs);
		input.add_to_team(data.team());
		if (&inputs == &front(data.inputs)) {
			using_lead = true;
		}
	}
	generate_random_team(data);
	Team team = predict_team(data.detailed, data.team(), data.random_engine, using_lead);
	data.output.value(to_string(team, false).c_str());
	data.team() = Team(max_pokemon_per_team);
}

}	// namespace

int main () {
	Fl_Window win(window_width, window_height, "Team Predictor");
		int button_number = 0;
		AllPokemonInputs all_pokemon_inputs{{
			{button_number, PokemonInputs::construct},
			{button_number, PokemonInputs::construct},
			{button_number, PokemonInputs::construct},
			{button_number, PokemonInputs::construct},
			{button_number, PokemonInputs::construct},
			{button_number, PokemonInputs::construct}
		}};
		Fl_Multiline_Output output(output_x_position, padding, output_width, output_height);
		Fl_Return_Button calculate(output_x_position, padding + output_height + padding, output_width, button_height, "Calculate");
	end(win);

	Data data(all_pokemon_inputs, output);

	calculate.callback (function, &data);
	win.show();

	return Fl::run();
}
