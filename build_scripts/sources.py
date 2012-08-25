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

network = prepend_dir('network', ['battles.cpp', 'client.cpp', 'inmessage.cpp', 'outmessage.cpp'])
pokemon_lab = prepend_dir('pokemon_lab', ['battle.cpp', 'battle_settings.cpp', 'client.cpp', 'conversion.cpp', 'inmessage.cpp', 'outmessage.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
pokemon_online = prepend_dir('pokemon_online', ['battle.cpp', 'battle_settings.cpp', 'client.cpp', 'conversion.cpp', 'read_team_file.cpp', 'inmessage.cpp', 'outmessage.cpp', 'read_user_info.cpp', 'write_team_file.cpp'])
clients = prepend_dir('clients', ['battle.cpp', 'battle_result.cpp', 'party.cpp'] + network + pokemon_lab + pokemon_online)

ai_sources = ['ai.cpp', 'ability.cpp', 'block.cpp', 'damage.cpp', 'endofturn.cpp', 'entry_hazards.cpp', 'evaluate.cpp', 'expectiminimax.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'screen.cpp', 'screens.cpp', 'settings_file.cpp', 'status.cpp', 'substitute.cpp', 'switch.cpp', 'taunt.cpp', 'team.cpp', 'toxic.cpp', 'transposition.cpp', 'uproar.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp', 'wish.cpp', 'yawn.cpp']
ai_sources += prepend_dir('bide', ['bide.cpp', 'damage.cpp', 'duration.cpp'])
ai_sources += clients
ai_sources += prepend_dir('cryptography', ['hex.cpp', 'md5.cpp', 'rijndael.cpp', 'sha2.cpp'])
ai_sources += prepend_dir('move', ['move.cpp', 'accuracy.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'power.cpp', 'pp.cpp', 'priority.cpp', 'random.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp', 'use_move.cpp'])
ai_sources += prepend_dir('pokemon', ['pokemon.cpp', 'collection.cpp', 'level.cpp', 'seen.cpp'])
ai_sources += prepend_dir('stat', ['chance_to_hit.cpp', 'nature.cpp', 'stage.cpp', 'stat.cpp'])
ai_sources += prepend_dir('team_predictor', ['detailed_stats.cpp', 'load_stats.cpp', 'team_predictor.cpp'])
ai_sources += prepend_dir('type', ['collection.cpp', 'type.cpp'])
ai_sources += string_conversion_sources
ai_libraries = ['pthread', 'boost_system', 'boost_filesystem']

predict_sources = ['ability.cpp', 'damage.cpp', 'entry_hazards.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'screen.cpp', 'screens.cpp', 'substitute.cpp', 'status.cpp', 'taunt.cpp', 'team.cpp', 'toxic.cpp', 'uproar.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp', 'wish.cpp', 'yawn.cpp']
predict_sources += prepend_dir('bide', ['bide.cpp', 'damage.cpp', 'duration.cpp'])
predict_sources += prepend_dir('move', ['move.cpp', 'accuracy.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'power.cpp', 'pp.cpp', 'priority.cpp', 'random.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp'])
predict_sources += prepend_dir('pokemon', ['pokemon.cpp', 'collection.cpp', 'level.cpp', 'seen.cpp'])
predict_sources += prepend_dir('stat', ['chance_to_hit.cpp', 'nature.cpp', 'stage.cpp', 'stat.cpp'])
predict_sources += prepend_dir('clients/pokemon_lab', ['read_team_file.cpp'])
predict_sources += prepend_dir('clients/pokemon_online', ['conversion.cpp', 'read_team_file.cpp'])
predict_sources += prepend_dir('team_predictor', ['detailed_stats.cpp', 'load_stats.cpp', 'predictor.cpp', 'team_predictor.cpp'])
predict_sources += prepend_dir('type', ['collection.cpp', 'type.cpp'])
predict_sources += string_conversion_sources
predict_libraries = ['fltk', 'boost_filesystem', 'boost_system']

test_sources = prepend_dir('test', ['test.cpp', 'damage.cpp', 'stat.cpp', 'string_conversion.cpp'])
test_sources += prepend_dir('test/collections', ['collection.cpp', 'move_collection.cpp', 'move_container.cpp', 'variable_collection.cpp'])
test_sources += prepend_dir('test/pokemon_lab', ['conversion.cpp', 'team_file.cpp', 'test.cpp'])
test_sources += prepend_dir('test/pokemon_online', ['conversion.cpp', 'team_file.cpp', 'test.cpp'])
test_sources += ['ability.cpp', 'damage.cpp', 'entry_hazards.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'screen.cpp', 'screens.cpp', 'status.cpp', 'substitute.cpp', 'taunt.cpp', 'team.cpp', 'toxic.cpp', 'uproar.cpp', 'vanish.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp', 'wish.cpp', 'yawn.cpp']
test_sources += prepend_dir('bide', ['bide.cpp', 'damage.cpp', 'duration.cpp'])
test_sources += prepend_dir('move', ['move.cpp', 'accuracy.cpp', 'classification.cpp', 'collection.cpp', 'container.cpp', 'disable.cpp', 'power.cpp', 'pp.cpp', 'priority.cpp', 'random.cpp', 'ranked.cpp', 'reorder.cpp', 'shared.cpp', 'times_used.cpp'])
test_sources += prepend_dir('stat', ['chance_to_hit.cpp', 'nature.cpp', 'stage.cpp', 'stat.cpp'])
test_sources += prepend_dir('pokemon', ['pokemon.cpp', 'collection.cpp', 'level.cpp', 'seen.cpp'])
test_sources += prepend_dir('clients/pokemon_lab', ['conversion.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
test_sources += prepend_dir('clients/pokemon_online', ['conversion.cpp', 'read_team_file.cpp', 'write_team_file.cpp'])
test_sources += prepend_dir('type', ['collection.cpp', 'type.cpp'])
test_sources += string_conversion_sources
test_libraries = ['boost_filesystem', 'boost_system']

ai = ('ai', ai_sources, ai_libraries)
predict = ('predict', predict_sources, predict_libraries)
test = ('test', test_sources, test_libraries)

def generate_sources (sources, version, compiler_name):
	temp = []
	for source in sources:
		temp += ['build/' + compiler_name + '/' + version + '/' + source]
	return temp
