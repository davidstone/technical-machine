// Predict foe's team
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>
#include <vector>
#include "load_stats.h"
#include "pokemon.h"
#include "team.h"
#include "teampredictor.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>

using namespace technicalmachine;

// A GUI version of the team predictor.

struct Data {
	std::vector<Fl_Input*> input;
	Fl_Multiline_Output* output;
	Map map;
	int detailed [END_SPECIES][7];
};

void function (Fl_Widget* w, void* data) {
	Data* d = reinterpret_cast<Data*> (data);
	
	Team team (false, d->map);
	bool using_lead;
	std::map<std::string, species>::const_iterator it = d->map.specie.find ((*d->input.begin())->value());
	if (it != d->map.specie.end()) {
		using_lead = true;
		Pokemon member (it->second, team.size);
		team.pokemon.set.push_back (member);
	}
	else
		using_lead = false;
	for (std::vector<Fl_Input*>::const_iterator in = d->input.begin() + 1; in != d->input.end(); ++in) {
		it = d->map.specie.find ((*in)->value());
		if (it != d->map.specie.end()) {
			Pokemon member (it->second, team.size);
			team.pokemon.set.push_back (member);
		}
	}
	
	if (team.pokemon.set.size() > 0) {
		predict_team (d->detailed, team, using_lead);
		std::string out;
		team.output (out);
		d->output->value (out.c_str());
	}
	else
		d->output->value ("");
}

int main () {
	Fl_Window win (520, 730, "Team Predictor");
		Fl_Input input0 (50, 10, 120, 30, "Input");
		Fl_Input input1 (50, 50, 120, 30);
		Fl_Input input2 (50, 90, 120, 30);
		Fl_Input input3 (50, 130, 120, 30);
		Fl_Input input4 (50, 170, 120, 30);
		Fl_Input input5 (50, 210, 120, 30);
		Fl_Return_Button calculate (50, 250, 120, 30, "Calculate");
		Fl_Multiline_Output output (250, 10, 260, 710, "Output");
	win.end();

	Data data;
	detailed_stats (data.map, data.detailed);
	
	data.input.push_back (&input0);
	data.input.push_back (&input1);
	data.input.push_back (&input2);
	data.input.push_back (&input3);
	data.input.push_back (&input4);
	data.input.push_back (&input5);
	data.output = &output;

	calculate.callback (function, &data);
	win.show();

	return Fl::run();
}
