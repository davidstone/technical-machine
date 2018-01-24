# Technical Machine installation instructions

For a full overview of the program, see http://doublewise.net/pokemon/

## Prerequisites

* clang version 5.0.1 or newer
* gcc install of 7.2.0 for the standard library
* Boost 1.65.0
* fltk is needed the stand-alone team predictor / builder. It is not needed to actually run the battling AI.
* CMake 3.10.2
* Ninja (tested on 1.8.2)

## Building (Linux)

* `cd path/to/technical-machine`
* `mkdir build`
* `cd build`
* `cmake .. -G"Ninja" -DCMAKE_CXX_COMPILER=clang++` (you can also use `-G"Unix Makefiles"` instead)
* `ninja`
* Fill in server information for the server you want to connect to, as found in "build/settings/settings.xml".
* Run the program with `./ai [depth]`, where depth is an optional value that tells Technical Machine how many turns ahead to look (defaults to 2). The depth can be updated by sending a PM to the bot (if you are a trusted user, set in `trusted_users.txt`) with the command `!depth #`.

### Build targets

ai
:	The AI with maximum optimizations. Accepts a single, optional command-line argument, the depth to search. Higher depth means stronger play, but it also takes longer to search. If no value is entered, 2 is assumed. Recommended at 1 through 5, depending on how fast your computer is and whether you built in release mode (the default). If you set the depth of search to 0, Technical Machine moves randomly.

predict
:	Team predictor. Enter in Pokemon already seen and it shows its prediction of the remaining team. If a Pokemon is put into the first slot, it is assumed to be the lead Pokemon. If no Pokemon is put in that slot, no lead stats are used.

tm_test
:	Runs some tests to verify there are no regressions. Should be run before committing anything.

# Commands

Commands can be sent to Technical Machine on any server it is logged in to (and only apply on that server). It only accepts commands via PMs from trusted users (determined by reading `settings/trusted_users.txt`).

!challenge
:	Format is `!challenge username`

!depth
:	Format is `!depth #` (where `#` represents the new depth as an integer). Sets Technical Machine's depth of search to `#`. This only applies to future challenges / battles. Existing battles are fixed in their depth.

!exit
:	Technical Machine logs out of the server.

!join
:	Format is `!join channel_name` (with no '#' symbol)

!message
:	Sends a message to a channel. Format is `!message channel_name msg` where `channel` is a one word channel name (with no '#' symbol) and `message` is any number of words. For channels that may optionally contain spaces, format is `!message "channel name" msg`

!part
:	Format is `!part channel_name` (with no '#' symbol)

!pm
:	Format is `!pm username message` where `username` is a one word username. For usernames that may optionally contain spaces, format is `!pm "user name" message`

!quit
:	Same as !exit

!reload
:	Reloads all relevant text files. This allows updating TM's set of canned responses, how often it uses those responses, and evaluation constants, for instance.
