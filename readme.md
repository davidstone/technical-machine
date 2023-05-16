# Technical Machine, a Pokemon AI

Technical Machine has partial support for all generations. However, generations 1-4 are the most complete -- later generations assume many new moves have no side effects.

## Team Building and Team Prediction

The first problem a Pokemon program needs to solve is the issue of team building. Full details can be read on the [team building documentation](documentation/team_building.md). It uses the same concepts to predict what the foe's team is.

## Battling

After deciding on a team, it's time to battle! You can read the full details on [how Technical Machine decides which action to take in battle](documentation/battling.md).

# Technical Machine installation instructions

See [full build instructions](documentation/building.md).

Once built, fill in server information for the server you want to connect to, as found in `build/settings/settings.json`. Then you can run TM with `./build/ai [general-depth] [single-depth]`, where general-depth and single-depth are optional values that tell Technical Machine how many turns ahead to look (defaults to 2, 0).

## settings.json

If the "team" setting is left blank, Technical Machine will generate a team of its own for every battle. If a team file is specified, Technical Machine will use that file. If a directory is specified, Technical Machine will randomly use a file inside that directory, recursively. For instance, if you have team files in folders based on tiers, then to use any OU team you would put "teams/ou/", but to use your stall team you would put "teams/ou/stall.sbt". For directories, the terminating '/' is optional. Relative paths are relative to your executable, not the `settings.json` file.

### Build targets

ai
:	The AI with maximum optimizations. Accepts two arguments. The first is an integer for how far ahead to search before evaluating each position. The second is an integer for how far ahead to consider each possible 1v1 matchup after doing the previous general search. Higher depth means stronger play, but it also takes much longer to search. If no value is entered, 2, 0 is assumed. Recommended that the two numbers add up to no more than 4, depending on how fast your computer is and whether you built in release mode.

file_converter
:   Converts team files in one format to another format. Supported input file types are NetBattle, NetBattle Supremacy, Shoddy Battle, Pokemon Lab, and Pokemon Online. Supported output types are print a string representation, save a string representation to files, Pokemon Lab, and Pokemon Online.

predict
:	Team predictor. Go to localhost:46923 in your web browser after running this to use the team builder / predictor. Enter in Pokemon already seen and it shows its prediction of the remaining team. If a Pokemon is put into the first slot, it is assumed to be the lead Pokemon. If no Pokemon is put in that slot, no lead stats are used.

ps_regression_test
:   Runs through old logs to make sure nothing is obviously broken.

ps_usage_stats_create_teams_file
:   Parses Pokemon Showdown log files to generate a single file containing all teams found in all battles parsed. Output is a binary file in the "tmmt" (Technical Machine Multi-Team) binary format, which is not stable between TM versions.

ps_usage_stats
:   Reads a tmmt file and writes out usage stats in the "tmus" (Technical Machine Usage Stats) binary format. Note that the format of a tmmt file can change with any new version of TM, and thus `ps_usage_stats_create_teams_file` should be rerun after pulling in new changes to the code, or arbitrarily bad behavior could occur.

ps_usage_stats_create_derivative_stats
:   Reads a tmus file. Generates many human-readable representations of subsets of the data.

tm_test
:	Runs some tests to verify there are no regressions. Should be run before committing anything.
