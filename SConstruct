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
import multiprocessing
import shutil

from build_scripts.sources import ai, predict, test, generate_sources

SetOption('warn', 'no-duplicate-environment')
SetOption('max_drift', 2)
SetOption('implicit_cache', 1)
SetOption('num_jobs', multiprocessing.cpu_count())

AddOption('--compiler', dest = 'compiler', type = 'string', action = 'store', help = 'Name of the compiler to use.')
AddOption('--compiler-command', dest = 'compiler_command', type = 'string', action = 'store', help = 'Command to launch the compiler.')
AddOption('--verbose', dest = 'verbose', action = "store_true", help = 'Print the full compiler command output.')

Decider('MD5-timestamp')

SConscript('build_scripts/compiler_settings.py')
Import('flags', 'compiler_command', 'compiler_name')

default = DefaultEnvironment()

if not GetOption('verbose'):
	default['CXXCOMSTR'] = 'Compiling $TARGET'
	default['LINKCOMSTR'] = 'Linking $TARGET'

default.Replace(CXX = compiler_command)

def setup_environment_flags(version):
	environment = default.Clone()
	environment.Append(CCFLAGS = flags['cc'][version])
	environment.Append(CXXFLAGS = flags['cxx'][version])
	environment.Append(LINKFLAGS = flags['link'][version])
	environment.Append(CPPDEFINES = flags['cpp'][version])
	if version != 'default':
		build_root = 'build/' + compiler_name + '/'
		environment.VariantDir(build_root + version, 'src', duplicate = 0)
	return environment

default = setup_environment_flags('default')
debug = setup_environment_flags('debug')
optimized = setup_environment_flags('optimized')

def create_program (base, versions):
	env = { 'debug':debug, 'optimized':optimized }
	suffix = { 'debug':'', 'optimized':'o' }
	name, base_sources, libraries = base
	for version in versions:
		sources = generate_sources (base_sources, version, compiler_name)
		executable_name = name + suffix [version]
		env[version].Clone(LIBS = libraries).Program(executable_name, sources)

create_program (ai, ['debug', 'optimized'])
create_program (predict, ['debug'])
create_program (test, ['debug', 'optimized'])

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
