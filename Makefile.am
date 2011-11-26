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

warnings = -Wall -Wextra -pedantic -Wformat=2 -Wno-unused
fulloptimizations = -DNDEBUG -O3 -march=native -funsafe-loop-optimizations
tournament = $(fulloptimizations) -flto -fwhole-program

CXXFLAGS = $(warnings) -g -std=c++0x
CFLAGS = $(warnings) -g -std=c99

bin_PROGRAMS = ai
ai_SOURCES=src/ai.cpp src/ability.cpp src/analyze_logs.cpp src/battle.cpp src/block.cpp src/damage.cpp src/endofturn.cpp src/evaluate.cpp src/expectiminimax.cpp src/gender.cpp src/heal.cpp src/item.cpp src/load_stats.cpp src/move.cpp src/pokemon.cpp src/reorder_moves.cpp src/stat.cpp src/status.cpp src/switch.cpp src/team.cpp src/teampredictor.cpp src/transposition.cpp src/type.cpp src/weather.cpp \
	src/crypt/get_md5.cpp src/crypt/get_sha2.cpp src/crypt/hex.cpp src/crypt/md5.c src/crypt/rijndael.cpp src/crypt/sha2.c \
	src/network/connect.cpp src/network/inmessage.cpp src/network/outmessage.cpp \
	src/pokemon_lab/battle.cpp src/pokemon_lab/battle_settings.cpp src/pokemon_lab/connect.cpp src/pokemon_lab/conversion.cpp src/pokemon_lab/file.cpp src/pokemon_lab/inmessage.cpp src/pokemon_lab/outmessage.cpp \
	src/pokemon_online/battle.cpp src/pokemon_online/battle_settings.cpp src/pokemon_online/connect.cpp src/pokemon_online/conversion.cpp src/pokemon_online/file.cpp src/pokemon_online/inmessage.cpp src/pokemon_online/outmessage.cpp src/pokemon_online/read_user_info.cpp
