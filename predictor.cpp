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
#include "movefunction.h"
#include "pokemon.h"
#include "statfunction.h"
#include "team.h"
#include "teampredictor.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

namespace tm {

struct Data {
	std::vector<Fl_Input*> input;
	std::vector<Fl_Output*> output;
	Map map;
 	int detailed [END_SPECIES][7];
};

void function (Fl_Widget* w, void* data) {
	Data* d = reinterpret_cast<Data*> (data);
	
	teams team;
	bool using_lead;
	std::map<std::string, species>::const_iterator it = d->map.specie.find ((*d->input.begin())->value());
	if (it != d->map.specie.end()) {
		using_lead = true;
		pokemon member;
		member.name = it->second;
		team.member.push_back (member);
	}
	else
		using_lead = false;
	for (std::vector<Fl_Input*>::const_iterator in = d->input.begin() + 1; in != d->input.end(); ++in) {
		it = d->map.specie.find ((*in)->value());
		if (it != d->map.specie.end()) {
			pokemon member;
			member.name = it->second;
			team.member.push_back (member);
		}
	}
	
	if (team.member.size() > 0) {
		predict (d->detailed, team, using_lead);
		for (unsigned n = 0; n != d->output.size(); ++n) {
			std::string output = pokemon_name [team.member.at (n).name] + ": ";
			for (std::vector<moves>::const_iterator move = team.member.at(n).moveset.begin(); move->name != STRUGGLE; ++move)
				output += move_name [move->name] + ", ";
			output.erase (output.end() - 2, output.end());
			d->output.at (n)->value (output.c_str());
		}
	}
	else {
		for (std::vector<Fl_Output*>::iterator out = d->output.begin(); out != d->output.end(); ++out)
			(*out)->value ("");
	}
}

int main () {
	Fl_Window win (800, 280, "Team Predictor");
		Fl_Input input0 (60, 10, 120, 30, "Input");
		Fl_Input input1 (60, 50, 120, 30);
		Fl_Input input2 (60, 90, 120, 30);
		Fl_Input input3 (60, 130, 120, 30);
		Fl_Input input4 (60, 170, 120, 30);
		Fl_Input input5 (60, 210, 120, 30);
		Fl_Output output0 (260, 10, 500, 30, "Output");
		Fl_Output output1 (260, 50, 500, 30);
		Fl_Output output2 (260, 90, 500, 30);
		Fl_Output output3 (260, 130, 500, 30);
		Fl_Output output4 (260, 170, 500, 30);
		Fl_Output output5 (260, 210, 500, 30);
		Fl_Return_Button calculate (60, 250, 700, 30, "Calculate");
	win.end();

	Data data;
	set_species_map (data.map.specie);
	set_ability_map (data.map.ability);
	set_item_map (data.map.item);
	set_nature_map (data.map.nature);
	set_move_map (data.map.move);
	detailed_stats (data.map, data.detailed);
	
	data.input.push_back (&input0);
	data.input.push_back (&input1);
	data.input.push_back (&input2);
	data.input.push_back (&input3);
	data.input.push_back (&input4);
	data.input.push_back (&input5);
	data.output.push_back (&output0);
	data.output.push_back (&output1);
	data.output.push_back (&output2);
	data.output.push_back (&output3);
	data.output.push_back (&output4);
	data.output.push_back (&output5);

	calculate.callback (function, &data);
	win.show();

	return Fl::run();
}

void predict (int detailed [][7], teams &team, bool using_lead) {
	std::vector<double> overall;
	overall_stats (overall);
	double total = 961058;	// Total number of teams
	double multiplier [END_SPECIES][END_SPECIES];
	team_stats (overall, total, multiplier);
	std::vector<double> lead;
	if (using_lead)
		lead_stats (lead);
	else {
		for (int n = 0; n != END_SPECIES; ++n)
			lead.push_back (1);
	}
	
	std::vector<double> estimate;
	for (unsigned n = 0; n != END_SPECIES; ++n)
		estimate.push_back ((overall.at (n) / total) * lead.at (n));

	for (std::vector<pokemon>::const_iterator it = team.member.begin(); it != team.member.end(); ++it) {
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [it->name] [n];
	}
	predict_pokemon (team, estimate, detailed, multiplier);
}

}
