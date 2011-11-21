# Copyright (C) 2011 David Stone
#
# This file is part of Technical Machine.
#
# Technical Machine is free software: you can redistribute it and / or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

aiobjects = \
	ai.o ability.o analyze_logs.o battle.o block.o damage.o endofturn.o evaluate.o expectiminimax.o gender.o heal.o item.o load_stats.o move.o pokemon.o reorder_moves.o stat.o status.o switch.o team.o teampredictor.o transposition.o type.o weather.o \
	crypt/get_md5.o crypt/get_sha2.o crypt/hex.o crypt/md5.o crypt/rijndael.o crypt/sha2.o \
	network/connect.o network/inmessage.o network/outmessage.o \
	pokemon_lab/battle.o pokemon_lab/battle_settings.o pokemon_lab/connect.o pokemon_lab/conversion.o pokemon_lab/file.o pokemon_lab/inmessage.o pokemon_lab/outmessage.o \
	pokemon_online/battle.o pokemon_online/battle_settings.o pokemon_online/connect.o pokemon_online/conversion.o pokemon_online/file.o pokemon_online/inmessage.o pokemon_online/outmessage.o pokemon_online/read_user_info.o

predictobjects = predictor.o ability.o block.o damage.o gender.o item.o load_stats.o heal.o move.o pokemon.o stat.o status.o switch.o team.o teampredictor.o type.o weather.o \
	pokemon_lab/conversion.o pokemon_lab/file.o \
	pokemon_online/conversion.o pokemon_online/file.o

rdcobjects = rdc.o ability.o block.o damage.o gender.o heal.o item.o load_stats.o move.o pokemon.o reversedamage.o stat.o status.o switch.o team.o teampredictor.o type.o unknown.o weather.o \
	pokemon_lab/conversion.o pokemon_lab/file.o \
	pokemon_online/conversion.o pokemon_online/file.o

warnings = -Wall -Wextra -pedantic -Wformat=2 -Wno-unused
fulloptimizations = -DNDEBUG -O3 -march=native -funsafe-loop-optimizations
tournament = $(fulloptimizations) -flto -fwhole-program

ai : $(aiobjects)
	$(CXX) -o ai $(aiobjects) -lpthread -lboost_system -lboost_filesystem $(CXXFLAGS)
ai : optimizations = -g

aio : $(aiobjects)
	$(CXX) -o aio $(aiobjects) -lpthread -lboost_system -lboost_filesystem $(CXXFLAGS)
aio : optimizations = $(fulloptimizations)

predict : $(predictobjects)
	$(CXX) -o predict $(predictobjects) -lfltk -lboost_filesystem -lboost_system $(CXXFLAGS)
predict : optimizations = -g

predicto : $(predictobjects)
	$(CXX) -o predict $(predictobjects) -lfltk -lboost_filesystem -lboost_system $(CXXFLAGS)
predicto : optimizations = $(fulloptimizations)

rdc : $(rdcobjects)
	$(CXX) -o rdc $(rdcobjects) -lboost_filesystem -lboost_system $(CXXFLAGS)
rdc : optimizations = -g

rdco : $(rdcobjects)
	$(CXX) -o rdco $(rdcobjects) -lboost_filesystem -lboost_system $(CXXFLAGS)
rdco : optimizations = $(fulloptimizations)

CXXFLAGS = $(warnings) $(optimizations) -std=c++0x
CFLAGS = $(warnings) $(optimizations) -std=c99

ability.o: ability.cpp ability.h pokemon.h active.h gender.h item.h \
 species.h stat.h status.h type.h team.h move.h weather.h
ai.o: ai.cpp analyze_logs.h gender.h move.h active.h type.h species.h \
 evaluate.h expectiminimax.h item.h load_stats.h ability.h stat.h \
 pokemon.h status.h team.h teampredictor.h weather.h \
 pokemon_lab/connect.h pokemon_lab/inmessage.h
analyze_logs.o: analyze_logs.cpp analyze_logs.h gender.h move.h active.h \
 type.h species.h ability.h endofturn.h item.h stat.h pokemon.h \
 status.h switch.h team.h weather.h
battle.o: battle.cpp battle.h analyze_logs.h move.h active.h type.h \
 species.h gender.h team.h pokemon.h ability.h item.h stat.h status.h \
 weather.h expectiminimax.h teampredictor.h network/connect.h \
 evaluate.h
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

network/connect.o: network/connect.cpp network/connect.h evaluate.h \
 species.h network/battle_settings.h load_stats.h
network/inmessage.o: network/inmessage.cpp network/inmessage.h \
 network/buffer_overrun.h network/invalid_packet.h
network/outmessage.o: network/outmessage.cpp network/outmessage.h

pokemon_lab/battle.o: pokemon_lab/battle.cpp pokemon_lab/battle.h \
 analyze_logs.h active.h type.h species.h team.h ability.h gender.h item.h \
 stat.h status.h weather.h expectiminimax.h move.h pokemon.h \
 teampredictor.h pokemon_lab/connect.h pokemon_lab/inmessage.h \
 network/inmessage.h network/connect.h evaluate.h pokemon_lab/outmessage.h \
 network/outmessage.h
pokemon_lab/battle_settings.o: pokemon_lab/battle_settings.cpp \
 pokemon_lab/battle_settings.h network/battle_settings.h
pokemon_lab/connect.o: pokemon_lab/connect.cpp pokemon_lab/connect.h \
 pokemon_lab/battle.h battle.h analyze_logs.h move.h active.h type.h species.h \
 gender.h team.h pokemon.h ability.h item.h stat.h status.h weather.h \
 pokemon_lab/inmessage.h network/inmessage.h network/connect.h \
 evaluate.h pokemon_lab/battle_settings.h network/battle_settings.h \
 pokemon_lab/outmessage.h network/outmessage.h crypt/get_md5.h \
 crypt/get_sha2.h crypt/rijndael.h pokemon_lab/conversion.h
pokemon_lab/conversion.o: pokemon_lab/conversion.cpp pokemon_lab/conversion.h \
 move.h active.h type.h species.h
pokemon_lab/file.o: pokemon_lab/file.cpp pokemon_lab/file.h pokemon.h \
 ability.h active.h gender.h item.h move.h type.h species.h stat.h status.h \
 team.h
pokemon_lab/inmessage.o: pokemon_lab/inmessage.cpp pokemon_lab/inmessage.h \
 network/inmessage.h pokemon_lab/connect.h pokemon_lab/battle.h analyze_logs.h \
 move.h active.h type.h species.h team.h pokemon.h ability.h gender.h item.h \
 stat.h status.h weather.h network/connect.h evaluate.h
pokemon_lab/outmessage.o: pokemon_lab/outmessage.cpp pokemon_lab/outmessage.h \
 species.h network/outmessage.h ability.h active.h gender.h item.h move.h type.h \
 stat.h status.h team.h pokemon.h

pokemon_online/battle.o: pokemon_online/battle.cpp pokemon_online/battle.h \
 battle.h analyze_logs.h move.h active.h type.h species.h gender.h ability.h \
 item.h stat.h status.h weather.h pokemon.h team.h
pokemon_online/battle_settings.o: pokemon_online/battle_settings.cpp \
 pokemon_online/battle_settings.h network/battle_settings.h
pokemon_online/connect.o: pokemon_online/connect.cpp pokemon_online/connect.h \
 pokemon_online/inmessage.h network/inmessage.h network/connect.h battle.h \
 analyze_logs.h move.h active.h type.h species.h gender.h team.h pokemon.h \
 ability.h item.h stat.h status.h weather.h evaluate.h pokemon_online/battle.h \
 pokemon_online/battle_settings.h network/battle_settings.h pokemon_online/outmessage.h \
 network/outmessage.h crypt/get_md5.h
pokemon_online/conversion.o: pokemon_online/conversion.cpp pokemon_online/conversion.h \
 ability.h item.h move.h active.h type.h species.h
pokemon_online/file.o: pokemon_online/file.cpp pokemon_online/file.h \
 ability.h active.h type.h species.h stat.h pokemon.h gender.h \
 item.h status.h team.h move.h
pokemon_online/inmessage.o: pokemon_online/inmessage.cpp pokemon_online/inmessage.h \
 network/inmessage.h pokemon_online/connect.h network/connect.h evaluate.h \
 species.h network/invalid_packet.h
pokemon_online/outmessage.o: pokemon_online/outmessage.cpp pokemon_online/outmessage.h \
 species.h network/outmessage.h
pokemon_online/read_user_info.o: pokemon_online/read_user_info.cpp \
 pokemon_online/read_user_info.h pokemon_online/inmessage.h \
 network/inmessage.h

.PHONY: clean
clean:
	find . -name "*.o" -print0 | xargs --null rm
