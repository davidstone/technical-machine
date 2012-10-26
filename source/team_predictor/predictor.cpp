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

#include <algorithm>
#include <random>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "detailed_stats.hpp"
#include "random_team.hpp"
#include "team_predictor.hpp"

#include "ui/input_constants.hpp"
#include "ui/species_input.hpp"

#include "../string_conversions/invalid_string_conversion.hpp"
#include "../string_conversions/conversion.hpp"
#include "../team.hpp"

#include "../pokemon/pokemon.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Multiline_Output.H>

using namespace technicalmachine;

// A GUI version of the team predictor.

namespace {
class PokemonInputs;
constexpr unsigned pokemon_per_team = 6;

class Data {
	public:
		void reset() {
			team = Team();
		}
		template<typename... Args>
		void add(Args && ... args) {
			input.emplace_back(std::forward<Args>(args)...);
		}
		std::vector<PokemonInputs *> input;
		Fl_Int_Input * random_input;
		Fl_Multiline_Output * output;
		DetailedStats detailed;
		Team team;
};

constexpr int pokemon_indent = 20;
constexpr int ev_input_width = 60;
constexpr int number_of_stats = 6;
constexpr int button_width = input_width;
constexpr int button_height = input_height;

constexpr int input_lines_per_pokemon = 3;
constexpr int input_lines_for_random = 2;
constexpr int input_lines_for_button = 1;
constexpr int total_input_lines = pokemon_per_team * input_lines_per_pokemon + input_lines_for_random + input_lines_for_button;
constexpr int total_input_height = total_input_lines * (input_height + padding);

constexpr int output_width = 400;
constexpr int height_per_line = 16;
constexpr int output_lines_for_ability = 1;
constexpr int output_lines_for_nature_and_evs = 2;
constexpr int output_lines_for_moves = 4;
constexpr int output_lines_per_pokemon = 1 + output_lines_for_ability + output_lines_for_nature_and_evs + output_lines_for_moves;
constexpr int output_lines_per_team = pokemon_per_team * output_lines_per_pokemon;
constexpr int output_team_padding = 10;
constexpr int output_team_height = output_lines_per_team * height_per_line + output_team_padding;
constexpr int output_height = output_team_height;
constexpr int output_x_position = left_padding + ev_input_width * number_of_stats + left_padding;

constexpr int window_width = output_x_position + output_width + padding;
constexpr int window_height = padding + ((total_input_height > output_height) ? total_input_height : output_height + padding);

class EVInput {
	public:
		explicit EVInput(int const button_number, int const ev, char const * label = ""):
			input(left_padding + pokemon_indent + ev_input_width * (ev), y_position(button_number), ev_input_width, input_height, label)
			{
		}
		unsigned value() const {
			return boost::lexical_cast<unsigned>(input.value());
		}
	private:
		Fl_Int_Input input;
};

class EVInputs {
	public:
		explicit EVInputs(int const button_number):
			hp(button_number, 0, "EVs"),
			atk(button_number, 1),
			def(button_number, 2),
			spa(button_number, 3),
			spd(button_number, 4),
			spe(button_number, 5)
			{
		}
		EVInput hp;
		EVInput atk;
		EVInput def;
		EVInput spa;
		EVInput spd;
		EVInput spe;
};

class MoveInput {
	public:
		MoveInput(int const button_number, int const x_position, char const * label = ""):
			input(left_padding + pokemon_indent + x_position * width, y_position(button_number), width, input_height, label)
			{
		}
		Moves value() const {
			return from_string<Moves>(input.value());
		}
	private:
		static constexpr int width = 90;
		Fl_Input input;
};

class MoveInputs {
	public:
		explicit MoveInputs(int const button_number):
			input0(button_number, 0, "Moves"),
			input1(button_number, 1),
			input2(button_number, 2),
			input3(button_number, 3)
			{
		}
		MoveInput input0;
		MoveInput input1;
		MoveInput input2;
		MoveInput input3;
};

class PokemonInputs {
	public:
		explicit PokemonInputs(int & button_number):
			m_species(button_number++),
			m_evs(button_number++),
			m_moves(button_number++)
			{
		}
		Species species() const {
			return m_species.value();
		}
		unsigned hp() const {
			return m_evs.hp.value();
		}
		unsigned atk() const {
			return m_evs.atk.value();
		}
		unsigned def() const {
			return m_evs.def.value();
		}
		unsigned spa() const {
			return m_evs.spa.value();
		}
		unsigned spd() const {
			return m_evs.spd.value();
		}
		unsigned spe() const {
			return m_evs.spe.value();
		}
		std::vector<Moves> moves() const {
			return {
				m_moves.input0.value(),
				m_moves.input1.value(),
				m_moves.input2.value(),
				m_moves.input3.value()
			};
		}
	private:
		SpeciesInput m_species;
		EVInputs m_evs;
		MoveInputs m_moves;
};

class PokemonInputValues {
	public:
		PokemonInputValues(PokemonInputs const & inputs):
			species(inputs.species()),
			hp(inputs.hp()),
			atk(inputs.atk()),
			def(inputs.def()),
			spa(inputs.spa()),
			spd(inputs.spd()),
			spe(inputs.spe()),
			moves(inputs.moves())
			{
		}
		void add_to_team(Team & team) const {
			team.add_pokemon(species, 100, Gender(), item, ability, nature);
			team.pokemon().hp().ev.set_value(hp);
			team.pokemon().atk().ev.set_value(atk);
			team.pokemon().def().ev.set_value(def);
			team.pokemon().spa().ev.set_value(spa);
			team.pokemon().spd().ev.set_value(spd);
			team.pokemon().spe().ev.set_value(spe);
		}
		Species species;
		Item item;
		Ability ability;
		Nature nature;
		unsigned hp;
		unsigned atk;
		unsigned def;
		unsigned spa;
		unsigned spd;
		unsigned spe;
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
	unsigned const remaining_pokemon = pokemon_per_team - data.team.all_pokemon().size();
	try {
		return std::min(boost::lexical_cast<unsigned>(data.random_input->value()), remaining_pokemon);
	}
	catch (boost::bad_lexical_cast const &) {
		return 0u;
	}
}

void generate_random_team(Data & data) {
	static std::random_device rd;
	static std::mt19937 random_engine(rd());
	random_team(data.team, random_engine, max_random(data));
}

void function (Fl_Widget * w, void * d) {
	Data & data = *reinterpret_cast <Data *> (d);
	bool using_lead = false;
	for (PokemonInputs * inputs : data.input) {
		if (data.team.all_pokemon().size() >= pokemon_per_team)
			break;
		Species species;
		try {
			species = inputs->species();
			constexpr unsigned level = 100;
			Gender const gender(Gender::MALE);
			data.team.add_pokemon(species, level, gender);
		}
		catch (InvalidFromStringConversion const &) {
			species = Species::END;
		}
		if (inputs == data.input.front ()) {
			using_lead = (species != Species::END);
		}
	}
	generate_random_team(data);
	data.output->value(predict_team(data.detailed, data.team, using_lead).to_string(false).c_str());
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
