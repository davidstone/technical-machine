# Technical Machine installation instructions

For a full overview of the program, see http://doublewise.net/pokemon/

## Prerequisites

* gcc version 4.7.0 or newer, with c++ support. gcc version 4.6.x and older will not work, as Technical Machine makes use of some features in C++11 that are not supported in older versions of gcc. Listed as gcc-c++ in Fedora repositories. Unfortunately, other compilers, such as clang and MSVC++, will not work because they do not support all of the C++11 features yet.
* Boost. Listed as boost-devel in Fedora repositories. Technical Machine is currently tested against version 1.48.0.
* fltk is needed the stand-alone team predictor / builder. It is not needed to actually run the battling AI. Listed as fltk-devel.
* SCons is used for the build process.

## Building (Linux)

* `cd path/to/technical-machine`
* `scons`. The default target (by simply typing scons) will build all versions of all programs. Various stand-alone parts can be made by typing `scons target`, where target is what you want to build. In general, the debug version of any target is made by appending "-debug" to the target name. For instance, if you want to just build the core AI, you type `scons ai`. If you want to only build the debug version, you type `scons ai-debug`.
* Fill in server information for the server you want to connect to, as found in "settings/settings.xml".
* Run the program with `./ai [depth]` or `./ai-debug [depth]`, where depth is an optional value that tells Technical Machine how many turns ahead to look (defaults to 2). The depth can be updated by sending a PM to the bot (if you are a trusted user, set in `trusted_users.txt`) with the command `!depth #`.

### Build targets

ai
:	The AI with maximum optimizations. Accepts a single, optional command-line argument, the depth to search. Higher depth means stronger play, but it also takes longer to search. If no value is entered, 2 is assumed. Recommended at 1 through 3. If you are very patient and have a fast computer, Technical Machine can search to a depth of 4 in a reasonable amount of time. If you set the depth of search to 0, Technical Machine moves randomly.

ai-debug
:	The AI with no optimizations and all asserts. Accepts a single command-line argument as above. Recommended at 1 or 2.

predict
predict-debug
:	GUI team predictor. Enter in Pokemon already seen and it shows its prediction of the remaining team. If a Pokemon is put into the first slot, it is assumed to be the lead Pokemon. If no Pokemon is put in that slot, no lead stats are used.

test
:	Runs some tests to verify there are no regressions. Should be run before committing anything.

test-debug
:	Tests with no optimizations enabled, to verify that optimizations do not change behavior.

# Commands

Commands can be sent to Technical Machine on any server it is logged in to (and only apply on that server). It only accepts commands via PMs from trusted users (determined by reading `settings/trusted_users.txt`).

!challenge
:	Format is `!challenge username`

!depth
:	Format is `!depth #` (where `#` represents the new depth as an integer). Sets Technical Machine's depth of search to `#`. This only applies to future challenges / battles. Existing battles are fixed in their depth.

!join
:	Format is `!join channel_name` (with no '#' symbol)

!message
:	Sends a message to a channel. Format is `!message channel_name msg` where `channel` is a one word channel name (with no '#' symbol) and `message` is any number of words. For channels that may optionally contain spaces, format is `!message "channel name" msg`

!part
:	Format is `!part channel_name` (with no '#' symbol)

!pm
:	Format is `!pm username message` where `username` is a one word username. For usernames that may optionally contain spaces, format is `!pm "user name" message`

!reload
:	Reloads all relevant text files. This allows updating TM's set of canned responses, how often it uses those responses, and evaluation constants, for instance.
