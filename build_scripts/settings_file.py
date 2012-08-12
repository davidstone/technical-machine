# Create Technical Machine's settings file
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

settings_env = Environment()

def check_if_exists (dependency, target, prev_ni):
	return not os.path.exists(str(target))

settings_env.Decider(check_if_exists)
source_settings_file_suffix = '.template'
for settings_file in Glob('../source/settings/*' + source_settings_file_suffix, strings=True):
	target_file = os.path.basename(settings_file) [:-len(source_settings_file_suffix)]
	NoClean(settings_env.InstallAs('../settings/' + target_file, settings_file))

def create_team_directory (target, source, env):
	dir_str = str (target [0])
	os.makedirs(dir_str)

team_dir_builder = Builder(action = create_team_directory, target_factory=Dir)
team_dir_env = Environment(BUILDERS = { 'Create' : team_dir_builder })
NoClean(team_dir_env.Create('../teams/foe', []))
team_dir_env.Decider(check_if_exists)
