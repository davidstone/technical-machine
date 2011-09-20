// Interface for the stand-alone log analyzer / team extractor
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <fstream>
#include <iostream>
#include <string>
#include "analyze_logs.h"
#include "pokemon.h"
#include "team.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>

using namespace technicalmachine;

struct Data {
	Fl_Input* input;
	std::string output;
	Fl_Multiline_Output* multiout;
	std::string log_path;
	Weather weather;
};

void load_log (Fl_Widget* w, void* data) {
	Data* d = reinterpret_cast<Data*> (data);
	std::ifstream file ((d->log_path + d->input->value()).c_str());
	if (!file.is_open() or ((static_cast<std::string> (d->input->value())).size() == 0))
		std::cerr << d->input->value() << " is not a valid file.\n";
	else {
		std::string line;
		Team player1 (false);
		player1.player = "";
		Team player2 (false);
		player2.player = "";
		Team* previous;
		for (getline (file, line); !file.eof(); getline (file, line))
			analyze_line (player1, player2, previous, d->weather, line);
		file.close ();
		d->output = "";
		output (d->output, player1);
		output (d->output, player2);
		d->multiout->value (d->output.c_str());
	}
}

int main (int argc, char* argv[]) {
	Fl_Window win (1125, 900, "Analyze Logs");
		Fl_Multiline_Input inputpaste (80, 10, 400, 770, "Paste log");
		Fl_Return_Button calculatepaste (80, 785, 400, 30, "Pasted");
		Fl_Input inputfile (80, 825, 400, 30, "Open file");
		Fl_Return_Button calculatefile (80, 860, 400, 30, "File");
		Fl_Multiline_Output output (540, 10, 500, 880, "Output");
	win.end();

	Data data;
	data.input = &inputfile;
	data.multiout = &output;
	data.log_path = "/home/david/Shoddy logs/";
	
	calculatefile.callback (load_log, &data);
	win.show();

	return Fl::run();
}
