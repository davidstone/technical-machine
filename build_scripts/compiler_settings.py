# Determine the correct settings for the compiler being used.
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

compiler_name = GetOption('compiler')
compiler_command = GetOption('compiler_command')
if compiler_name == None and compiler_command != None:
	compiler_name = os.path.basename(compiler_command)
elif compiler_command == None:
	compiler_command = compiler_name
if compiler_name == None:
	compiler_name = DefaultEnvironment()['CXX']
	compiler_command = compiler_name
# should probably add some extra stuff here to account for things like g++-4.7

def is_compiler (compiler):
	return compiler_name == compiler

if is_compiler('g++') or is_compiler('gcc'):
	from build_scripts.gcc.std import cxx_std
	from build_scripts.gcc.warnings import warnings, warnings_debug, warnings_optimized
	from build_scripts.gcc.optimizations import optimizations, preprocessor_optimizations, linker_optimizations
elif is_compiler('clang++') or is_compiler('clang'):
	from build_scripts.clang.std import cxx_std
	from build_scripts.clang.warnings import warnings, warnings_debug, warnings_optimized
	from build_scripts.clang.optimizations import optimizations, preprocessor_optimizations, linker_optimizations

cc_flags = { 'default': warnings + ['-g'], 'debug': warnings_debug, 'optimized': optimizations + warnings_optimized }
cxx_flags = { 'default': cxx_std, 'debug': [], 'optimized': [] }
link_flags = { 'default': warnings + cxx_std, 'debug': [], 'optimized': linker_optimizations + warnings_optimized }
cpp_flags = { 'default': [], 'debug': [], 'optimized': preprocessor_optimizations }

flags = { 'cc': cc_flags, 'cxx': cxx_flags, 'link': link_flags, 'cpp': cpp_flags }

Export('flags', 'compiler_command')
