# List of sources
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

def prepend_dir(directory, sources):
	return map(lambda source: directory + '/' + source, sources)

string_conversion_sources = prepend_dir('string_conversions', ['ability_string.cpp', 'gender_string.cpp', 'item_string.cpp', 'move_string.cpp', 'nature_string.cpp', 'pokemon_string.cpp', 'status_string.cpp'])

ai_sources = ['ai.cpp', 'ability.cpp', 'battle.cpp', 'battle_result.cpp', 'block.cpp', 'damage.cpp', 'detailed_stats.cpp', 'endofturn.cpp', 'evaluate.cpp', 'expectiminimax.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'load_stats.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'settings_file.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'switch.cpp', 'team.cpp', 'team_predictor.cpp', 'transposition.cpp', 'type.cpp', 'type_collection.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
ai_sources += prepend_dir('cryptography', ['hex.cpp', 'md5.cpp', 'rijndael.cpp', 'sha2.cpp'])
ai_sources += prepend_dir('move', ['move.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'power.cpp', 'pp.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp', 'use_move.cpp'])
ai_sources += prepend_dir('network', ['connect.cpp', 'inmessage.cpp', 'outmessage.cpp'])
ai_sources += prepend_dir('pokemon_lab', ['battle.cpp', 'battle_settings.cpp', 'connect.cpp', 'conversion.cpp', 'inmessage.cpp', 'outmessage.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
ai_sources += prepend_dir('pokemon_online', ['battle.cpp', 'battle_settings.cpp', 'connect.cpp', 'conversion.cpp', 'read_team_file.cpp', 'inmessage.cpp', 'outmessage.cpp', 'read_user_info.cpp', 'write_team_file.cpp'])
ai_sources += string_conversion_sources
ai_libraries = ['pthread', 'boost_system', 'boost_filesystem']

predict_sources = ['predictor.cpp', 'ability.cpp', 'detailed_stats.cpp', 'gender.cpp', 'item.cpp', 'load_stats.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'team_predictor.cpp', 'type.cpp', 'type_collection.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
predict_sources += prepend_dir('move', ['move.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'pp.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp'])
predict_sources += prepend_dir('pokemon_lab', ['read_team_file.cpp'])
predict_sources += prepend_dir('pokemon_online', ['conversion.cpp', 'read_team_file.cpp'])
predict_sources += string_conversion_sources
predict_libraries = ['fltk', 'boost_filesystem', 'boost_system']

test_sources = prepend_dir('test', ['test.cpp', 'damage.cpp', 'stat.cpp', 'string_conversion.cpp'])
test_sources += prepend_dir('test/collections', ['collection.cpp', 'move_collection.cpp', 'move_container.cpp', 'variable_collection.cpp'])
test_sources += prepend_dir('test/pokemon_lab', ['conversion.cpp', 'team_file.cpp', 'test.cpp'])
test_sources += prepend_dir('test/pokemon_online', ['conversion.cpp', 'team_file.cpp', 'test.cpp'])
test_sources += ['ability.cpp', 'damage.cpp', 'gender.cpp', 'item.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'type.cpp', 'type_collection.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
test_sources += prepend_dir('move', ['move.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'power.cpp', 'pp.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp'])
test_sources += prepend_dir('pokemon_lab', ['conversion.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
test_sources += prepend_dir('pokemon_online', ['conversion.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
test_sources += string_conversion_sources
test_libraries = ['boost_filesystem', 'boost_system']

ai = ('ai', ai_sources, ai_libraries)
predict = ('predict', predict_sources, predict_libraries)
test = ('test', test_sources, test_libraries)

def generate_sources (sources, version):
	temp = []
	for source in sources:
		temp += ['build/' + version + '/' + source]
	return temp
