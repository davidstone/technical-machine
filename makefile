aiobjects = ai.o analyze_logs.o block.o damage.o endofturn.o evaluate.o expectiminimax.o heal.o load_stats.o map.o move.o pokemon.o reorder_moves.o stat.o status.o switch.o team.o teampredictor.o transposition.o type.o weather.o crypt/get_md5.o crypt/get_sha2.o crypt/hex.o crypt/md5.o crypt/rijndael.o crypt/sha2.o pokemon_lab/connect.o pokemon_lab/file.cpp pokemon_lab/inmessage.o pokemon_lab/outmessage.o pokemon_online/file.o

predictobjects = predictor.o block.o damage.o load_stats.o heal.o map.o move.o pokemon.o stat.o status.o switch.o team.o teampredictor.o type.o weather.o pokemon_lab/file.o pokemon_online/file.o

rdcobjects = rdc.o block.o damage.o heal.o load_stats.o map.o move.o pokemon.o reversedamage.o stat.o status.o switch.o team.o teampredictor.o type.o unknown.o weather.o pokemon_lab/file.o pokemon_online/file.o

analogobjects = analyze_logs_main.o analyze_logs.o damage.o move.o pokemon.o stat.o status.o team.o type.o weather.o

analogguiobjects = analyze_logs_gui.o analyze_logs.o damage.o move.o pokemon.o stat.o status.o team.o weather.o

connectobjects = block.o damage.o heal.o map.o move.o pokemon.o stat.o status.o switch.o team.o type.o weather.o crypt/get_md5.o crypt/get_sha2.o crypt/hex.o crypt/md5.o crypt/rijndael.o crypt/sha2.o pokemon_lab/connect.o pokemon_lab/file.cpp pokemon_lab/inmessage.o pokemon_lab/outmessage.o pokemon_online/file.o

warnings = -Wall -Wextra -pedantic -Wformat=2 -Wno-unused
fulloptimizations = -O3 -march=native -ffast-math -DNDEBUG -funsafe-loop-optimizations

CXX = g++

ai : $(aiobjects)
	$(CXX) -o ai $(aiobjects) -lpthread -lboost_system $(CXXFLAGS)
ai : optimizations = -g

aio : $(aiobjects)
	$(CXX) -o aio $(aiobjects) -lpthread -lboost_system $(CXXFLAGS)
aio : optimizations = $(fulloptimizations)

connect : $(connectobjects)
	$(CXX) -o connect $(connectobjects) -lpthread -lboost_system $(CXXFLAGS)
connect : optimizations = -g -Wno-unused

predict : $(predictobjects)
	$(CXX) -o predict $(predictobjects) -l fltk $(CXXFLAGS)
predict : optimizations = -g

predicto : $(predictobjects)
	$(CXX) -o predict $(predictobjects) -l fltk $(CXXFLAGS)
predicto : optimizations = $(fulloptimizations)

rdc : $(rdcobjects)
	$(CXX) -o rdc $(rdcobjects) $(CXXFLAGS)
rdc : optimizations = -g

rdco : $(rdcobjects)
	$(CXX) -o rdco $(rdcobjects) $(CXXFLAGS)
rdco : optimizations = $(fulloptimizations)

analog : $(analogobjects)
	$(CXX) -o analog $(analogobjects) $(CXXFLAGS)
analog : optimizations = -g

analoggui : $(analogguiobjects)
	$(CXX) -o analoggui $(analogguiobjects) $(CXXFLAGS) -l fltk
analoggui : optimizations = -g

CXXFLAGS = $(warnings) $(optimizations) -std=c++0x
CFLAGS = $(warnings) $(optimizations) -std=c99

ai.o: ai.cpp analyze_logs.h evaluate.h expectiminimax.h move.h active.h \
 type.h item.h load_stats.h species.h map.h ability.h gender.h stat.h \
 pokemon.h status.h team.h teampredictor.h weather.h
analyze_logs_gui.o: analyze_logs_gui.cpp analyze_logs.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h \
 status.h team.h
analyze_logs_main.o: analyze_logs_main.cpp analyze_logs.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h \
 status.h team.h
analyze_logs.o: analyze_logs.cpp analyze_logs.h ability.h endofturn.h \
 map.h gender.h item.h move.h active.h type.h stat.h species.h \
 pokemon.h status.h switch.h team.h weather.h
block.o: block.cpp block.h move.h active.h type.h pokemon.h ability.h \
 gender.h item.h species.h stat.h status.h team.h weather.h
damage.o: damage.cpp damage.h ability.h gender.h item.h move.h active.h \
 type.h pokemon.h species.h stat.h status.h team.h weather.h
endofturn.o: endofturn.cpp endofturn.h ability.h damage.h heal.h move.h \
 active.h type.h pokemon.h gender.h item.h species.h stat.h status.h \
 team.h weather.h
evaluate.o: evaluate.cpp evaluate.h move.h active.h type.h pokemon.h \
 ability.h gender.h item.h species.h stat.h status.h team.h weather.h
expectiminimax.o: expectiminimax.cpp expectiminimax.h move.h active.h \
 type.h block.h endofturn.h evaluate.h reorder_moves.h pokemon.h \
 ability.h gender.h item.h species.h stat.h status.h switch.h team.h \
 transposition.h weather.h
heal.o: heal.cpp heal.h pokemon.h ability.h active.h gender.h item.h \
 move.h type.h species.h stat.h status.h
load_stats.o: load_stats.cpp map.h ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h pokemon.h status.h
map.o: map.cpp map.h ability.h gender.h item.h move.h active.h type.h \
 stat.h species.h
move.o: move.cpp move.h active.h type.h ability.h block.h \
 damage.h heal.h pokemon.h gender.h item.h species.h stat.h \
 status.h switch.h team.h weather.h
pokemon.o: pokemon.cpp active.h pokemon.h ability.h \
 gender.h item.h move.h type.h species.h stat.h status.h
predictor.o: predictor.cpp load_stats.h species.h pokemon.h ability.h \
 active.h gender.h item.h move.h type.h stat.h status.h team.h \
 teampredictor.h
rdc.o: rdc.cpp ability.h item.h load_stats.h species.h move.h active.h \
 type.h pokemon.h gender.h stat.h status.h reversedamage.h \
 switch.h team.h teampredictor.h unknown.h weather.h
reorder_moves.o: reorder_moves.cpp reorder_moves.h move.h active.h type.h
reversedamage.o: reversedamage.cpp reversedamage.h damage.h item.h stat.h \
 species.h team.h active.h weather.h unknown.h
stat.o: stat.cpp ability.h item.h move.h active.h type.h pokemon.h \
 gender.h species.h stat.h status.h team.h weather.h
status.o: status.cpp pokemon.h ability.h active.h gender.h item.h move.h \
 type.h species.h stat.h status.h team.h weather.h
switch.o: switch.cpp heal.h pokemon.h ability.h active.h gender.h item.h \
 move.h type.h species.h stat.h status.h switch.h team.h weather.h
team.o: team.cpp ability.h gender.h item.h move.h active.h type.h \
 pokemon.h species.h stat.h status.h switch.h team.h \
 teampredictor.h pokemon_lab/file.h pokemon_online/file.h
teampredictor.o: teampredictor.cpp load_stats.h species.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h stat.h status.h team.h \
 teampredictor.h
transposition.o: transposition.cpp evaluate.h expectiminimax.h move.h \
 active.h type.h pokemon.h ability.h gender.h item.h species.h stat.h \
 status.h team.h transposition.h weather.h
type.o: type.cpp pokemon.h ability.h active.h gender.h item.h move.h \
 type.h species.h stat.h status.h team.h weather.h
unknown.o: unknown.cpp unknown.h item.h stat.h species.h
weather.o: weather.cpp weather.h
crypt/get_md5.o: crypt/get_md5.cpp crypt/get_md5.h crypt/hex.h crypt/md5.h
crypt/get_sha2.o: crypt/get_sha2.cpp crypt/get_sha2.h crypt/hex.h crypt/sha2.h
crypt/hex.o: crypt/hex.cpp crypt/hex.h
crypt/md5.o: crypt/md5.c crypt/md5.h
crypt/rijndael.o: crypt/rijndael.cpp crypt/rijndael.h
crypt/sha2.o: crypt/sha2.c crypt/sha2.h
connect.o: pokemon_lab/connect.cpp crypt/get_md5.h \
 crypt/get_sha2.h crypt/rijndael.h load_stats.h map.h ability.h gender.h \
 item.h move.h active.h type.h stat.h pokemon.h \
 status.h evaluate.h species.h team.h weather.h \
 pokemon_lab/connect.h pokemon_lab/inmessage.h pokemon_lab/outmessage.h
pokemon_lab/file.o: pokemon_lab/file.cpp map.h ability.h gender.h item.h \
 move.h active.h type.h stat.h species.h pokemon.h status.h team.h \
 pokemon_lab/file.h
inmessage.o: pokemon_lab/inmessage.cpp evaluate.h \
 map.h ability.h gender.h item.h move.h active.h \
 type.h stat.h species.h team.h weather.h \
 pokemon_lab/inmessage.h pokemon_lab/connect.h
pokemon_lab/outmessage.o: pokemon_lab/outmessage.cpp pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h \
 status.h team.h pokemon_lab/outmessage.h
pokemon_online/file.o: pokemon_online/file.cpp pokemon.h ability.h active.h \
 gender.h item.h move.h type.h species.h stat.h status.h team.h \
 pokemon_online/file.h


.PHONY: clean
clean:
	find . -name "*.o" -print0 | xargs --null rm
