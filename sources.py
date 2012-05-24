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

def prepend_str_conversion_dir (source):
	return 'string_conversions/' + source

string_conversion_sources = ['ability_string.cpp', 'gender_string.cpp', 'item_string.cpp', 'move_string.cpp', 'nature_string.cpp', 'pokemon_string.cpp', 'status_string.cpp']
string_conversion_sources = map(prepend_str_conversion_dir, string_conversion_sources)

ai_sources = ['ai.cpp', 'ability.cpp', 'battle.cpp', 'battle_result.cpp', 'block.cpp', 'damage.cpp', 'detailed_stats.cpp', 'endofturn.cpp', 'evaluate.cpp', 'expectiminimax.cpp', 'gender.cpp', 'heal.cpp', 'item.cpp', 'load_stats.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'settings_file.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'switch.cpp', 'team.cpp', 'team_predictor.cpp', 'transposition.cpp', 'type.cpp', 'type_collection.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
ai_sources += ['cryptography/hex.cpp', 'cryptography/md5.cpp', 'cryptography/rijndael.cpp', 'cryptography/sha2.cpp']
ai_sources += ['move/move.cpp', 'move/collection.cpp', 'move/container.cpp', 'move/power.cpp', 'move/pp.cpp', 'move/ranked.cpp', 'move/reorder.cpp', 'move/shared.cpp', 'move/times_used.cpp', 'move/use_move.cpp']
ai_sources += ['network/connect.cpp', 'network/inmessage.cpp', 'network/outmessage.cpp']
ai_sources += ['pokemon_lab/battle.cpp', 'pokemon_lab/battle_settings.cpp', 'pokemon_lab/connect.cpp', 'pokemon_lab/conversion.cpp', 'pokemon_lab/inmessage.cpp', 'pokemon_lab/outmessage.cpp', 'pokemon_lab/read_team_file.cpp', 'pokemon_lab/write_team_file.cpp']
ai_sources += ['pokemon_online/battle.cpp', 'pokemon_online/battle_settings.cpp', 'pokemon_online/connect.cpp', 'pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp', 'pokemon_online/inmessage.cpp', 'pokemon_online/outmessage.cpp', 'pokemon_online/read_user_info.cpp', 'pokemon_online/write_team_file.cpp']
ai_sources += string_conversion_sources
ai_libraries = ['pthread', 'boost_system', 'boost_filesystem']

predict_sources = ['predictor.cpp', 'ability.cpp', 'detailed_stats.cpp', 'gender.cpp', 'item.cpp', 'load_stats.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'team_predictor.cpp', 'type.cpp', 'type_collection.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
predict_sources += ['move/move.cpp', 'move/collection.cpp', 'move/container.cpp', 'move/pp.cpp', 'move/ranked.cpp', 'move/reorder.cpp', 'move/shared.cpp', 'move/times_used.cpp']
predict_sources += ['pokemon_lab/read_team_file.cpp']
predict_sources += ['pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp']
predict_sources += string_conversion_sources
predict_libraries = ['fltk', 'boost_filesystem', 'boost_system']

test_sources = ['test/test.cpp', 'test/damage.cpp', 'test/stat.cpp', 'test/string_conversion.cpp']
test_sources += ['test/collections/collection.cpp', 'test/collections/move_collection.cpp', 'test/collections/move_container.cpp', 'test/collections/variable_collection.cpp']
test_sources += ['test/pokemon_lab/conversion.cpp', 'test/pokemon_lab/team_file.cpp', 'test/pokemon_lab/test.cpp']
test_sources += ['test/pokemon_online/conversion.cpp', 'test/pokemon_online/team_file.cpp', 'test/pokemon_online/test.cpp']
test_sources += ['ability.cpp', 'damage.cpp', 'gender.cpp', 'item.cpp', 'nature.cpp', 'pokemon.cpp', 'pokemon_collection.cpp', 'screen.cpp', 'stage.cpp', 'stat.cpp', 'status.cpp', 'team.cpp', 'type.cpp', 'type_collection.cpp', 'variable.cpp', 'variable_collection.cpp', 'weather.cpp']
test_sources += ['move/move.cpp', 'move/collection.cpp', 'move/container.cpp', 'move/power.cpp', 'move/pp.cpp', 'move/ranked.cpp', 'move/reorder.cpp', 'move/shared.cpp', 'move/times_used.cpp']
test_sources += ['pokemon_lab/conversion.cpp', 'pokemon_lab/read_team_file.cpp', 'pokemon_lab/write_team_file.cpp']
test_sources += ['pokemon_online/conversion.cpp', 'pokemon_online/read_team_file.cpp', 'pokemon_online/write_team_file.cpp']
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
