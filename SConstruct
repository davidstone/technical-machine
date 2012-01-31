# SCons file
# Copyright (C) 2012 David Stone
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

import os
import glob
import shutil

# I compile the same object file for multiple executables. I want all debug
# objects in one folder and all optimized objects in a different folder,
# regardless of the final executable they are used for. I also do not want to
# worry about whether a particular object file is used in multiple executables.
# Therefore, I turn off the warning about multiple environments using the same
# objects (which is only raised if the end result for those objects is the
# same).

SetOption('warn', 'no-duplicate-environment')


warnings = ['-Wall', '-Wextra', '-pedantic', '-Wformat=2', '-Wstrict-overflow=3', '-Wno-unused', '-Werror']
full_optimizations = ['-O3', '-march=native', '-funsafe-loop-optimizations', '-flto']
cc_flags = warnings
cxx_flags = ['-std=c++0x']
link_flags = warnings + ['-fwhole-program']
optimized_link_flags = ['-s'] + full_optimizations

default = DefaultEnvironment(CCFLAGS = cc_flags, CXXFLAGS = cxx_flags, LINKFLAGS = link_flags)

debug = default.Clone()
debug.Append(CCFLAGS = ['-g'])
debug.VariantDir('build/debug', 'src', duplicate = 0)

optimized = default.Clone()
optimized.Append(CCFLAGS = full_optimizations, CPPDEFINES = ['NDEBUG'], LINKFLAGS = optimized_link_flags)
optimized.VariantDir('build/optimized', 'src', duplicate = 0)

def prepend_str_conversion_dir (source):
	return 'string_conversions/' + source
string_conversion_sources = ['ability_string.cpp', 'gender_string.cpp', 'item_string.cpp', 'move_string.cpp', 'nature_string.cpp', 'pokemon_string.cpp', 'status_string.cpp']
string_conversion_sources = map(prepend_str_conversion_dir, string_conversion_sources)

ai_sources = ['ai.cpp', 'ability.cpp', 'battle.cpp', 'block.cpp', 'damage.cpp', 'endofturn.cpp', 'evaluate.cpp', 'expectiminimax.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'load_stats.cpp', 'move.cpp', 'move_power.cpp', 'nature.cpp', 'pokemon.cpp', 'reorder_moves.cpp', 'stat.cpp', 'status.cpp', 'switch.cpp', 'team.cpp', 'team_predictor.cpp', 'transposition.cpp', 'type.cpp', 'use_move.cpp', 'weather.cpp']
ai_sources += ['cryptography/hex.cpp', 'cryptography/md5.cpp', 'cryptography/rijndael.cpp', 'cryptography/sha2.cpp']
ai_sources += ['network/connect.cpp', 'network/inmessage.cpp', 'network/outmessage.cpp']
ai_sources += ['pokemon_lab/battle.cpp', 'pokemon_lab/battle_settings.cpp', 'pokemon_lab/connect.cpp', 'pokemon_lab/conversion.cpp', 'pokemon_lab/inmessage.cpp', 'pokemon_lab/outmessage.cpp', 'pokemon_lab/read_team_file.cpp', 'pokemon_lab/write_team_file.cpp']
ai_sources += ['pokemon_online/battle.cpp', 'pokemon_online/battle_settings.cpp', 'pokemon_online/connect.cpp', 'pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp', 'pokemon_online/inmessage.cpp', 'pokemon_online/outmessage.cpp', 'pokemon_online/read_user_info.cpp', 'pokemon_online/write_team_file.cpp']
ai_sources += string_conversion_sources
ai_libraries = ['pthread', 'boost_system', 'boost_filesystem']

predict_sources = ['predictor.cpp', 'ability.cpp', 'gender.cpp', 'item.cpp', 'load_stats.cpp', 'move.cpp', 'nature.cpp', 'pokemon.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'team_predictor.cpp', 'type.cpp']
predict_sources += ['pokemon_lab/read_team_file.cpp']
predict_sources += ['pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp']
predict_sources += string_conversion_sources
predict_libraries = ['fltk', 'boost_filesystem', 'boost_system']

#rdc_sources = ['rdc.cpp', 'ability.cpp', 'block.cpp', 'damage.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'load_stats.cpp', 'move.cpp', 'move_power.cpp', 'nature.cpp', 'pokemon.cpp', 'reversedamage.cpp', 'stat.cpp', 'status.cpp', 'switch.cpp', 'team.cpp', 'team_predictor.cpp', 'type.cpp', 'unknown.cpp', 'weather.cpp']
#rdc_sources += ['pokemon_lab/conversion.cpp', 'pokemon_lab/read_team_file.cpp']
#rdc_sources += ['pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp']
#rdc_sources += string_conversion_sources
#rdc_libraries = ['boost_filesystem', 'boost_system']

test_sources = ['test/test.cpp', 'test/damage.cpp', 'test/stat.cpp', 'test/string_conversion.cpp', 'test/pokemon_lab/conversion.cpp', 'test/pokemon_lab/team_file.cpp', 'test/pokemon_lab/test.cpp', 'test/pokemon_online/conversion.cpp', 'test/pokemon_online/team_file.cpp', 'test/pokemon_online/test.cpp']
test_sources += ['ability.cpp', 'damage.cpp', 'gender.cpp', 'item.cpp', 'move.cpp', 'move_power.cpp', 'nature.cpp', 'pokemon.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'type.cpp', 'weather.cpp']
test_sources += ['pokemon_lab/conversion.cpp', 'pokemon_lab/read_team_file.cpp', 'pokemon_lab/write_team_file.cpp']
test_sources += ['pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp', 'pokemon_online/write_team_file.cpp']
test_sources += string_conversion_sources
test_libraries = ['boost_filesystem', 'boost_system']

ai_debug_sources = []
for source in ai_sources:
	ai_debug_sources += ['build/debug/' + source]
ai_optimized_sources = []
for source in ai_sources:
	ai_optimized_sources += ['build/optimized/' + source]
predict_debug_sources = []
for source in predict_sources:
	predict_debug_sources += ['build/debug/' + source]
#rdc_debug_sources = []
#for source in rdc_sources:
#	rdc_debug_sources += ['build/debug/' + source]
#rdc_optimized_sources = []
#for source in rdc_sources:
#	rdc_optimized_sources += ['build/optimized/' + source]
test_debug_sources = []
for source in test_sources:
	test_debug_sources += ['build/debug/' + source]
test_optimized_sources = []
for source in test_sources:
	test_optimized_sources += ['build/optimized/' + source]

ai_debug = debug.Clone(LIBS = ai_libraries)
ai_debug.Program('ai', ai_debug_sources)

ai_optimized = optimized.Clone(LIBS = ai_libraries)
ai_optimized.Program('aio', ai_optimized_sources)

predict = debug.Clone(LIBS = predict_libraries)
predict.Program('predict', predict_debug_sources)

#rdc_debug = debug.Clone(LIBS = rdc_libraries)
#rdc_debug.Program('rdc', rdc_debug_sources)

#rdc_optimized = optimized.Clone(LIBS = rdc_libraries)
#rdc_optimized.Program('rdco', rdc_optimized_sources)

test_debug = debug.Clone(LIBS = test_libraries)
test_debug.Program('test', test_debug_sources)

test_optimized = optimized.Clone(LIBS=test_libraries)
test_optimized.Program('testo', test_optimized_sources)

num_cpu = int(os.environ.get('NUMBER_OF_PROCESSORS', 3))
Decider('MD5-timestamp')
SetOption('num_jobs', num_cpu * 3 / 2)

def check_if_exists (dependency, target, prev_ni):
	return not os.path.exists(str(target))

settings_env = Environment()
settings_env.Decider(check_if_exists)
for settings_file in Glob('src/settings/*.template', strings=True):
	target_file = os.path.basename(settings_file) [:-9]
	NoClean(settings_env.InstallAs('settings/' + target_file, settings_file))

def create_team_directory (target, source, env):
	dir_str = str (target [0])
	os.makedirs(dir_str)

team_dir_builder = Builder(action = create_team_directory, target_factory=Dir)
team_dir_env = Environment(BUILDERS = { 'Create' : team_dir_builder })
NoClean(team_dir_env.Create('teams/foe', []))
team_dir_env.Decider(check_if_exists)
