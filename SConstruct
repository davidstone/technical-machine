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
from gcc_warnings import warnings, warnings_debug, warnings_optimized
from gcc_optimizations import optimizations, preprocessor_optimizations, linker_optimizations
from sources import ai, predict, test, generate_sources

SetOption('warn', 'no-duplicate-environment')
SetOption('max_drift', 2)
SetOption('implicit_cache', 1)
num_cpu = int(os.environ.get('NUMBER_OF_PROCESSORS', 4))
SetOption('num_jobs', num_cpu * 3 / 2)
AddOption('--verbose', dest = 'verbose', action = "store_true", help = 'Print the full compiler command output.')
Decider('MD5-timestamp')


version_flags = ['-std=c++0x']
cc_flags = warnings + ['-g']
cxx_flags = version_flags
link_flags = warnings + version_flags

default = DefaultEnvironment()
default.Append(CCFLAGS = cc_flags, CXXFLAGS = cxx_flags, LINKFLAGS = link_flags)

if not GetOption('verbose'):
	default['CXXCOMSTR'] = 'Compiling $TARGET'
	default['LINKCOMSTR'] = 'Linking $TARGET'

debug = default.Clone()
debug.Append(CCFLAGS = warnings_debug)
debug.VariantDir('build/debug', 'src', duplicate = 0)

optimized = default.Clone()
optimized.Append(CCFLAGS = optimizations + warnings_optimized, CPPDEFINES = preprocessor_optimizations, LINKFLAGS = linker_optimizations + warnings_optimized)
optimized.VariantDir('build/optimized', 'src', duplicate = 0)

def create_program (base, versions):
	env = { 'debug':debug, 'optimized':optimized }
	suffix = { 'debug':'', 'optimized':'o' }
	name, base_sources, libraries = base
	for version in versions:
		sources = generate_sources (base_sources, version)
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
