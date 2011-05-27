aiobjects = ai.o analyze_logs.o block.o damage.o endofturn.o evaluate.o expectiminimax.o heal.o load_stats.o map.o move.o pokemon.o stat.o status.o switch.o team.o teampredictor.o type.o weather.o

predictobjects = predictor.o block.o damage.o heal.o move.o pokemon.o stat.o status.o switch.o team.o teampredictor.o type.o weather.o

rdcobjects = rdc.o block.o damage.o heal.o load_stats.o map.o move.o pokemon.o reversedamage.o stat.o status.o switch.o team.o teampredictor.o type.o unknown.o weather.o

analogobjects = analyze_logs_main.o analyze_logs.o damage.o move.o pokemon.o stat.o status.o team.o type.o weather.o

analogguiobjects = analyze_logs_gui.o analyze_logs.o damage.o move.o pokemon.o stat.o status.o team.o weather.o

warnings = -Wall -Wextra -pedantic -Wformat=2 -Wno-unused
fulloptimizations = -O3 -march=native -ffast-math -DNDEBUG -fno-rtti

ai : $(aiobjects)
	g++ -o ai $(aiobjects) $(CXXFLAGS)
ai : optimizations = -g

aio : $(aiobjects)
	g++ -o aio $(aiobjects) $(CXXFLAGS)
aio : optimizations = $(fulloptimizations)

predict : $(predictobjects)
	g++ -o predict $(predictobjects) -l fltk $(CXXFLAGS)
predict : optimizations = $(fulloptimizations)

rdc : $(rdcobjects)
	g++ -o rdc $(rdcobjects) $(CXXFLAGS)
rdc : optimizations = -g

rdco : $(rdcobjects)
	g++ -o rdco $(rdcobjects) $(CXXFLAGS)
rdco : optimizations = $(fulloptimizations)

analog : $(analogobjects)
	g++ -o analog $(analogobjects) $(CXXFLAGS)
analog : optimizations = -g

analoggui : $(analogguiobjects)
	g++ -o analoggui $(analogguiobjects) $(CXXFLAGS) -l fltk
analoggui : optimizations = -g

CXXFLAGS = $(warnings) $(optimizations)

ai.o: ai.cpp load_stats.h map.h ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h pokemon.h status.h movefunction.h \
 team.h weather.h statfunction.h teampredictor.h analyze_logs.h \
 expectiminimax.h evaluate.h state.h
analyze_logs_gui.o: analyze_logs_gui.cpp analyze_logs.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h \
 status.h team.h map.h weather.h movefunction.h
analyze_logs_main.o: analyze_logs_main.cpp analyze_logs.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h \
 status.h team.h map.h weather.h movefunction.h
analyze_logs.o: analyze_logs.cpp analyze_logs.h pokemon.h ability.h \
 active.h gender.h item.h move.h type.h species.h stat.h status.h team.h \
 map.h weather.h endofturn.h movefunction.h switch.h
block.o: block.cpp block.h move.h active.h type.h pokemon.h ability.h \
 gender.h item.h species.h stat.h status.h team.h map.h weather.h \
 statfunction.h typefunction.h
damage.o: damage.cpp damage.h pokemon.h ability.h active.h gender.h \
 item.h move.h type.h species.h stat.h status.h team.h map.h weather.h \
 typefunction.h
endofturn.o: endofturn.cpp endofturn.h pokemon.h ability.h active.h \
 gender.h item.h move.h type.h species.h stat.h status.h team.h map.h \
 weather.h damage.h heal.h statfunction.h statusfunction.h typefunction.h
evaluate.o: evaluate.cpp evaluate.h move.h active.h type.h pokemon.h \
 ability.h gender.h item.h species.h stat.h status.h team.h map.h \
 weather.h typefunction.h
expectiminimax.o: expectiminimax.cpp expectiminimax.h evaluate.h move.h \
 active.h type.h pokemon.h ability.h gender.h item.h species.h stat.h \
 status.h team.h map.h weather.h state.h block.h endofturn.h \
 movefunction.h statfunction.h switch.h transposition.h
heal.o: heal.cpp heal.h pokemon.h ability.h active.h gender.h item.h \
 move.h type.h species.h stat.h status.h
load_stats.o: load_stats.cpp map.h ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h pokemon.h status.h
map.o: map.cpp map.h ability.h gender.h item.h move.h active.h type.h \
 stat.h species.h
move.o: move.cpp move.h active.h type.h movefunction.h pokemon.h \
 ability.h gender.h item.h species.h stat.h status.h team.h map.h \
 weather.h block.h damage.h heal.h statfunction.h statusfunction.h \
 switch.h typefunction.h
pokemon.o: pokemon.cpp active.h movefunction.h pokemon.h ability.h \
 gender.h item.h move.h type.h species.h stat.h status.h team.h map.h \
 weather.h statfunction.h
predictor.o: predictor.cpp movefunction.h pokemon.h ability.h active.h \
 gender.h item.h move.h type.h species.h stat.h status.h team.h map.h \
 weather.h statfunction.h teampredictor.h
rdc.o: rdc.cpp ability.h item.h move.h active.h type.h movefunction.h \
 pokemon.h gender.h species.h stat.h status.h team.h map.h weather.h \
 reversedamage.h unknown.h statfunction.h switch.h teampredictor.h
reversedamage.o: reversedamage.cpp reversedamage.h team.h active.h map.h \
 ability.h gender.h item.h move.h type.h stat.h species.h pokemon.h \
 status.h weather.h unknown.h damage.h statfunction.h
stat.o: stat.cpp ability.h item.h move.h active.h type.h pokemon.h \
 gender.h species.h stat.h status.h statfunction.h team.h map.h weather.h \
 typefunction.h
status.o: status.cpp pokemon.h ability.h active.h gender.h item.h move.h \
 type.h species.h stat.h status.h statusfunction.h team.h map.h weather.h \
 typefunction.h
switch.o: switch.cpp heal.h pokemon.h ability.h active.h gender.h item.h \
 move.h type.h species.h stat.h status.h statfunction.h team.h map.h \
 weather.h statusfunction.h switch.h typefunction.h
team.o: team.cpp ability.h gender.h item.h move.h active.h type.h \
 movefunction.h pokemon.h species.h stat.h status.h team.h map.h \
 weather.h statfunction.h switch.h teampredictor.h
teampredictor.o: teampredictor.cpp load_stats.h map.h ability.h gender.h \
 item.h move.h active.h type.h stat.h species.h pokemon.h status.h team.h \
 teampredictor.h
type.o: type.cpp team.h active.h map.h ability.h gender.h item.h move.h \
 type.h stat.h species.h pokemon.h status.h typefunction.h weather.h
unknown.o: unknown.cpp unknown.h item.h stat.h species.h
weather.o: weather.cpp weather.h

.PHONY: clean
clean:
	rm *.o
