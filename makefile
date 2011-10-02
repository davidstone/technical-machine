aiobjects = ai.o ability.o analyze_logs.o block.o damage.o endofturn.o evaluate.o expectiminimax.o gender.o heal.o item.o load_stats.o move.o pokemon.o reorder_moves.o stat.o status.o switch.o team.o teampredictor.o transposition.o type.o weather.o crypt/get_md5.o crypt/get_sha2.o crypt/hex.o crypt/md5.o crypt/rijndael.o crypt/sha2.o pokemon_lab/battle.o pokemon_lab/connect.o pokemon_lab/file.o pokemon_lab/inmessage.o pokemon_lab/outmessage.o pokemon_online/file.o

predictobjects = predictor.o ability.o block.o damage.o gender.o item.o load_stats.o heal.o move.o pokemon.o stat.o status.o switch.o team.o teampredictor.o type.o weather.o pokemon_lab/file.o pokemon_online/file.o

rdcobjects = rdc.o ability.o block.o damage.o gender.o heal.o item.o load_stats.o move.o pokemon.o reversedamage.o stat.o status.o switch.o team.o teampredictor.o type.o unknown.o weather.o pokemon_lab/file.o pokemon_online/file.o

analogobjects = analyze_logs_main.o analyze_logs.o damage.o item.o move.o pokemon.o stat.o status.o team.o type.o weather.o

analogguiobjects = analyze_logs_gui.o analyze_logs.o damage.o move.o pokemon.o stat.o status.o team.o weather.o

warnings = -Wall -Wextra -pedantic -Wformat=2 -Wno-unused
fulloptimizations = -DNDEBUG -O3 -march=native -ffast-math -funsafe-loop-optimizations
tournament = $(fulloptimizations) -flto -fwhole-program

ai : $(aiobjects)
	$(CXX) -o ai $(aiobjects) -lpthread -lboost_system -lboost_filesystem $(CXXFLAGS)
ai : optimizations = -g

aio : $(aiobjects)
	$(CXX) -o aio $(aiobjects) -lpthread -lboost_system $(CXXFLAGS)
aio : optimizations = $(fulloptimizations)

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

ability.o: ability.cpp ability.h pokemon.h active.h gender.h item.h \
 species.h stat.h status.h type.h team.h move.h weather.h
ai.o: ai.cpp analyze_logs.h gender.h move.h active.h type.h species.h \
 evaluate.h expectiminimax.h item.h load_stats.h ability.h stat.h \
 pokemon.h status.h team.h teampredictor.h weather.h \
 pokemon_lab/connect.h pokemon_lab/inmessage.h
analyze_logs_gui.o: analyze_logs_gui.cpp analyze_logs.h gender.h move.h \
 active.h type.h species.h pokemon.h item.h stat.h status.h team.h
analyze_logs_main.o: analyze_logs_main.cpp analyze_logs.h gender.h move.h \
 active.h type.h species.h pokemon.h item.h stat.h status.h team.h
analyze_logs.o: analyze_logs.cpp analyze_logs.h gender.h move.h active.h \
 type.h species.h ability.h endofturn.h item.h stat.h pokemon.h \
 status.h switch.h team.h weather.h
block.o: block.cpp block.h ability.h move.h active.h type.h pokemon.h \
 gender.h item.h species.h stat.h status.h team.h weather.h
damage.o: damage.cpp damage.h ability.h gender.h item.h move.h active.h \
 type.h pokemon.h species.h stat.h status.h team.h weather.h
endofturn.o: endofturn.cpp endofturn.h ability.h damage.h heal.h move.h \
 active.h type.h pokemon.h gender.h item.h species.h stat.h status.h \
 team.h weather.h
evaluate.o: evaluate.cpp evaluate.h ability.h move.h active.h type.h \
 pokemon.h gender.h item.h species.h stat.h status.h team.h weather.h
expectiminimax.o: expectiminimax.cpp expectiminimax.h move.h active.h \
 type.h ability.h block.h endofturn.h evaluate.h reorder_moves.h \
 pokemon.h gender.h item.h species.h stat.h status.h switch.h team.h \
 transposition.h weather.h
gender.o: gender.cpp gender.h
heal.o: heal.cpp heal.h ability.h damage.h pokemon.h active.h gender.h \
 item.h species.h stat.h status.h type.h
load_stats.o: load_stats.cpp ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h pokemon.h status.h
move.o: move.cpp move.h active.h type.h ability.h block.h damage.h \
 evaluate.h heal.h pokemon.h gender.h item.h species.h stat.h status.h \
 switch.h team.h weather.h
pokemon.o: pokemon.cpp ability.h active.h move.h type.h pokemon.h \
 gender.h item.h species.h stat.h status.h
predictor.o: predictor.cpp ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h load_stats.h pokemon.h status.h team.h \
 teampredictor.h
rdc.o: rdc.cpp ability.h item.h load_stats.h species.h move.h active.h \
 type.h pokemon.h gender.h stat.h status.h reversedamage.h switch.h \
 team.h teampredictor.h unknown.h weather.h
reorder_moves.o: reorder_moves.cpp reorder_moves.h move.h active.h type.h
reversedamage.o: reversedamage.cpp reversedamage.h damage.h item.h stat.h \
 species.h team.h active.h move.h type.h weather.h unknown.h
stat.o: stat.cpp ability.h item.h move.h active.h type.h pokemon.h \
 gender.h species.h stat.h status.h team.h weather.h
status.o: status.cpp status.h ability.h pokemon.h active.h gender.h \
 item.h species.h stat.h type.h team.h move.h weather.h
switch.o: switch.cpp switch.h ability.h heal.h pokemon.h active.h \
 gender.h item.h species.h stat.h status.h type.h team.h move.h weather.h
team.o: team.cpp ability.h gender.h item.h move.h active.h type.h \
 pokemon.h species.h stat.h status.h switch.h team.h teampredictor.h \
 pokemon_lab/file.h pokemon_online/file.h
teampredictor.o: teampredictor.cpp teampredictor.h species.h ability.h \
 load_stats.h pokemon.h active.h gender.h item.h stat.h status.h type.h \
 team.h move.h
transposition.o: transposition.cpp transposition.h ability.h evaluate.h \
 expectiminimax.h move.h active.h type.h pokemon.h gender.h item.h \
 species.h stat.h status.h team.h weather.h
type.o: type.cpp type.h ability.h pokemon.h active.h gender.h item.h \
 species.h stat.h status.h team.h move.h weather.h
weather.o: weather.cpp weather.h
crypt/get_md5.o: crypt/get_md5.cpp crypt/get_md5.h crypt/hex.h crypt/md5.h
crypt/get_sha2.o: crypt/get_sha2.cpp crypt/get_sha2.h crypt/hex.h crypt/sha2.h
crypt/hex.o: crypt/hex.cpp crypt/hex.h
crypt/md5.o: crypt/md5.c crypt/md5.h
crypt/rijndael.o: crypt/rijndael.cpp crypt/rijndael.h
crypt/sha2.o: crypt/sha2.c crypt/sha2.h
battle.o: pokemon_lab/battle.cpp pokemon_lab/battle.h analyze_logs.h \
 move.h active.h type.h species.h team.h weather.h expectiminimax.h \
 pokemon.h ability.h gender.h item.h stat.h status.h teampredictor.h connect.h \
 evaluate.h pokemon_lab/inmessage.h pokemon_lab/outmessage.h
pokemon_lab/connect.o: pokemon_lab/connect.cpp crypt/get_md5.h \
 crypt/get_sha2.h crypt/rijndael.h analyze_logs.h gender.h move.h active.h \
 type.h evaluate.h expectiminimax.h load_stats.h ability.h item.h \
 stat.h pokemon.h status.h species.h team.h teampredictor.h weather.h \
 pokemon_lab/connect.h pokemon_lab/inmessage.h pokemon_lab/outmessage.h
pokemon_lab/file.o: pokemon_lab/file.cpp ability.h gender.h item.h move.h \
 active.h type.h stat.h species.h pokemon.h status.h team.h pokemon_lab/file.h
pokemon_lab/inmessage.o: pokemon_lab/inmessage.cpp analyze_logs.h \
 gender.h move.h active.h type.h evaluate.h ability.h item.h stat.h \
 species.h team.h weather.h pokemon_lab/inmessage.h pokemon_lab/connect.h
pokemon_lab/outmessage.o: pokemon_lab/outmessage.cpp ability.h \
 pokemon.h active.h gender.h item.h species.h stat.h status.h type.h team.h \
 move.h pokemon_lab/outmessage.h
pokemon_online/file.o: pokemon_online/file.cpp pokemon_online/file.h \
 ability.h active.h type.h species.h stat.h pokemon.h gender.h \
 item.h status.h team.h move.h


.PHONY: clean
clean:
	find . -name "*.o" -print0 | xargs --null rm
