# List of sources
# Copyright (C) 2014 David Stone
#
# This program is free software: you can redistribute it and / or modify
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
	"""Remove redundant specification of a directory for multiple sources"""
	return map(lambda source: directory + '/' + source, sources)

string_conversion_sources = prepend_dir('string_conversions', [
	'ability.cpp',
	'gender.cpp',
	'invalid_string_conversion.cpp',
	'item.cpp',
	'move.cpp',
	'nature.cpp',
	'pokemon.cpp',
	'status.cpp',
])

bide_sources = prepend_dir('bide', [
	'bide.cpp',
	'damage.cpp',
	'duration.cpp',
])

move_sources = prepend_dir('move', [
	'move.cpp',
	'accuracy.cpp',
	'base_power.cpp',
	'global_move.cpp',
	'is_switch.cpp',
	'max_moves_per_pokemon.cpp',
	'pp.cpp',
	'priority.cpp',
])

pokemon_sources = prepend_dir('pokemon', [
	'collection.cpp',
	'container.cpp',
	'max_pokemon_per_team.cpp',
	'pokemon_not_found.cpp',
	'happiness.cpp',
	'level.cpp',
	'pokemon.cpp',
])
pokemon_sources += move_sources

active_pokemon_sources = prepend_dir('pokemon', [
	'active_pokemon.cpp',
	'active_pokemon_flags.cpp',
	'confusion.cpp',
	'delayed_attack.cpp',
	'disable.cpp',
	'embargo.cpp',
	'encore.cpp',
	'end_of_turn_counter.cpp',
	'heal_block.cpp',
	'last_used_move.cpp',
	'magnet_rise.cpp',
	'partial_trap.cpp',
	'perish_song.cpp',
	'rampage.cpp',
	'slow_start.cpp',
	'stockpile.cpp',
	'substitute.cpp',
	'taunt.cpp',
	'toxic.cpp',
	'uproar.cpp',
	'vanish.cpp',
	'yawn.cpp',
])
active_pokemon_sources += bide_sources + pokemon_sources + ['random_damage.cpp']

stat_sources = prepend_dir('stat', [
	'calculate.cpp',
	'chance_to_hit.cpp',
	'ev.cpp',
	'hp.cpp',
	'iv.cpp',
	'nature.cpp',
	'stage.cpp',
	'stat.cpp',
	'stat_names.cpp',
	'stats.cpp',
])

team_sources = [
	'entry_hazards.cpp',
	'screen.cpp',
	'screens.cpp',
	'team.cpp',
	'wish.cpp',
] + active_pokemon_sources

network = prepend_dir('clients/network', [
	'buffer_overrun.cpp',
	'client.cpp',
	'inmessage.cpp',
	'invalid_channel.cpp',
	'invalid_packet.cpp',
	'outmessage.cpp',
])
pokemon_lab = prepend_dir('clients/pokemon_lab', [
	'read_team_file.cpp',
	'write_team_file.cpp',
])
pokemon_online = prepend_dir('clients/pokemon_online', [
	'battle.cpp',
	'battle_settings.cpp',
	'client.cpp',
	'conversion.cpp',
	'inmessage.cpp',
	'invalid_team_file.cpp',
	'invalid_user.cpp',
	'outmessage.cpp',
	'read_team_file.cpp',
	'read_user_info.cpp',
	'write_team_file.cpp',
])
pokemon_showdown = prepend_dir('clients/pokemon_showdown', [
	'battle.cpp',
	'battle_settings.cpp',
	'client.cpp',
	'inmessage.cpp',
	'outmessage.cpp',
])
clients = prepend_dir('clients', [
	'battle.cpp',
	'battle_result.cpp',
	'battles.cpp',
	'client.cpp',
	'invalid_team_file_format.cpp',
	'no_pending_challenges.cpp',
	'party.cpp',
	'updated_hp.cpp',
]) + network + pokemon_lab + pokemon_online + pokemon_showdown

ev_optimizer_sources = prepend_dir('team_predictor/ev_optimizer', [
	'combine.cpp',
	'defensive.cpp',
	'defensive_data_point.cpp',
	'ev_optimizer.cpp',
	'offensive.cpp',
	'single_classification_evs.cpp',
	'speed.cpp',
])

evaluate_sources = prepend_dir('evaluate', [
	'evaluate.cpp',
	'expectiminimax.cpp',
	'move_scores.cpp',
	'ranked.cpp',
	'reorder.cpp',
	'transposition.cpp',
])

team_builder_ui_sources = prepend_dir('team_predictor/ui', [
	'ev_inputs.cpp',
	'move_inputs.cpp',
	'nature_input.cpp',
	'pokemon_inputs.cpp',
	'species_input.cpp',
])

ai_sources = [
	'ai.cpp',
	'ability.cpp',
	'block.cpp',
	'damage.cpp',
	'endofturn.cpp',
	'gender.cpp',
	'hash.cpp',
	'heal.cpp',
	'item.cpp',
	'invalid_settings_file.cpp',
	'phazing_in_same_pokemon.cpp',
	'settings_file.cpp',
	'status.cpp',
	'switch.cpp',
	'variable.cpp',
	'weather.cpp',
]
ai_sources += team_sources
ai_sources += clients
ai_sources += prepend_dir('cryptography', [
	'hex.cpp',
	'md5.cpp',
	'rijndael.cpp',
	'sha2.cpp',
])
ai_sources += evaluate_sources
ai_sources += prepend_dir('move', [
	'collection.cpp',
	'container.cpp',
	'power.cpp',
	'shared.cpp',
	'use_move.cpp',
])
ai_sources += stat_sources
ai_sources += prepend_dir('team_predictor', [
	'detailed_stats.cpp',
	'estimate.cpp',
	'load_stats.cpp',
	'multiplier.cpp',
	'team_predictor.cpp',
])
ai_sources += ev_optimizer_sources
ai_sources += prepend_dir('type', [
	'collection.cpp',
	'effectiveness.cpp',
	'type.cpp',
])
ai_sources += string_conversion_sources
ai_libraries = [
	'pthread',
	'boost_system',
	'boost_filesystem',
]

predict_sources = [
	'ability.cpp',
	'damage.cpp',
	'gender.cpp',
	'heal.cpp',
	'item.cpp',
	'invalid_settings_file.cpp',
	'phazing_in_same_pokemon.cpp',
	'status.cpp',
	'variable.cpp',
	'weather.cpp',
]
predict_sources += team_sources
predict_sources += prepend_dir('move', [
	'collection.cpp',
	'container.cpp',
	'power.cpp',
	'shared.cpp',
])
predict_sources += stat_sources
predict_sources += prepend_dir('clients/', [
	'invalid_team_file_format.cpp',
])
predict_sources += prepend_dir('clients/pokemon_lab', [
	'read_team_file.cpp',
])
predict_sources += prepend_dir('clients/pokemon_online', [
	'conversion.cpp',
	'invalid_team_file.cpp',
	'read_team_file.cpp',
])
predict_sources += prepend_dir('team_predictor', [
	'detailed_stats.cpp',
	'estimate.cpp',
	'load_stats.cpp',
	'multiplier.cpp',
	'predictor.cpp',
	'random_team.cpp',
	'team_predictor.cpp',
])
predict_sources += ev_optimizer_sources
predict_sources += team_builder_ui_sources
predict_sources += prepend_dir('type', [
	'collection.cpp',
	'effectiveness.cpp',
	'type.cpp',
])
predict_sources += string_conversion_sources
predict_libraries = [
	'fltk',
	'boost_filesystem',
	'boost_system',
]

test_sources = prepend_dir('test', [
	'test.cpp',
	'damage.cpp',
	'ev_optimizer.cpp',
	'incorrect_calculation.cpp',
	'stat.cpp',
	'status.cpp',
	'string_conversion.cpp',
])
test_sources += prepend_dir('test/collections', [
	'collection.cpp',
	'invalid_collection.cpp',
	'move_collection.cpp',
	'move_container.cpp',
	'variable_collection.cpp',
])
test_sources += prepend_dir('test/pokemon_lab', [
	'team_file.cpp',
	'test.cpp',
])
test_sources += prepend_dir('test/pokemon_online', [
	'conversion.cpp',
	'team_file.cpp',
	'test.cpp',
])
test_sources += [
	'ability.cpp',
	'damage.cpp',
	'gender.cpp',
	'heal.cpp',
	'item.cpp',
	'invalid_settings_file.cpp',
	'phazing_in_same_pokemon.cpp',
	'status.cpp',
	'variable.cpp',
	'weather.cpp',
]
test_sources += team_sources
test_sources += prepend_dir('move', [
	'collection.cpp',
	'container.cpp',
	'power.cpp',
	'shared.cpp',
])
test_sources += stat_sources
test_sources += ev_optimizer_sources
test_sources += prepend_dir('clients/', [
	'invalid_team_file_format.cpp',
])
test_sources += prepend_dir('clients/pokemon_lab', [
	'read_team_file.cpp',
	'write_team_file.cpp',
])
test_sources += prepend_dir('clients/pokemon_online', [
	'conversion.cpp',
	'invalid_team_file.cpp',
	'read_team_file.cpp',
	'write_team_file.cpp',
])
test_sources += prepend_dir('type', [
	'collection.cpp',
	'effectiveness.cpp',
	'type.cpp',
])
test_sources += string_conversion_sources
test_libraries = [
	'boost_filesystem',
	'boost_system',
]

ai = ('ai', ai_sources, ai_libraries)
predict = ('predict', predict_sources, predict_libraries)
test = ('test', test_sources, test_libraries)

base_sources = [ai, predict, test]
