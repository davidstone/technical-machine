# A Pokemon Artificial Intelligence

Technical Machine has primarily been tested in generation 4. However, I have added support for most generation 5 Pokemon / moves, and it could theoretically play older generations as well. In theory, the program should play the strongest in generation 1 because of its simplicity. There are many fewer configurations of teams, and fewer options per turn.

## Team Building and Team Prediction

The first problem a Pokemon program needs to solve is the issue of team building. Technical Machine's primary method of generating new teams is to steal the teams of the foes that beat it, but it also has a team improvement component. Full details can be read on the [team building page](http://doublewise.net/pokemon/team_building/). It uses the same concepts to predict what the foe's team is.

## Battling

Here, "Battling" is defined as everything that comes after both trainers have sent out their first Pokemon. This excludes team building, and it also excludes the team selection process in versions of the game that have team reveal. Technical Machine currently does not support anything special from team reveal. Technical Machine has [some example battles](http://doublewise.net/pokemon/battles/).

### Long-Term Thinking

Also referred to as strategy, long-term thinking is being able to realize that if I could take out Blissey, Calm Mind Raikou can sweep their team easily, therefore I can sacrifice anything on my team (other than Raikou) to take out Blissey and win. Long-term thinking is the ability to accept any short-term loss of material for a powerful enough positional gain. Forms of AI that attempt to search large portions of the game are bad at this, because there simply isn't enough time to search deep enough to find these positional advantages. A smarter evaluation function helps with this a bit.

### Minimax / Search

Technical Machine's backbone is the search algorithm. The search algorithm I use essentially considers every possible combination of moves that the two players can make on a given turn. Then it looks at every combination of moves that can be used from each of the possibilities that arose from looking at every outcome of the previous turn. This process continues until it has searched deep enough to arrive at a good conclusion. This is the strategy that modern chess programs use, and is guaranteed to find the 'best' solution for the depth it can search, but the limit of this sort of strategy is that the game tree branches a lot, making a complete search take a very long time to complete. Technical Machine can currently search 4 turns ahead in about 5 seconds, and 5 turns ahead in about 1-5 minutes. With some fairly simple optimizations, it should be straightforward to reduce that down to being able to search 5 turns ahead in about 20 seconds, which should be sufficient for typical play vs. humans.

If the search algorithm finds a guaranteed win no matter what the opponent does, then it should obviously use the moves that lead to that state. However, this is a rare condition, especially in Pokemon, and so an AI requires something more sophisticated. The algorithm I use is minimax. Minimax algorithms rely on an evaluation function. The evaluation function is a way to score the game when it's somewhere in between "guaranteed win" and "guaranteed loss". A sample evaluation function could look something like giving 10 points for every Pokemon that's alive on a team, and 1 point for every % HP that each remaining Pokemon has. More complex evaluation functions can give more accurate estimates of the state of the game, with conditional evaluation functions giving the program the ability to engage in long-term thinking. By "conditional" evaluation function, I mean parts of the function that, for example, evaluate how well each Pokemon on one team performs against each individual Pokemon on the other team, and giving that Pokemon a bonus dependent on how well that match up goes, as a sort of way to reward removing powerful opponents and keeping powerful allies. The downfall of such an evaluation function is that it would be slow, which would limit the maximum depth of the AI's search (how far ahead it can look).

Currently, my evaluation function is a simple function. It just looks at several key variables and gives them a score if they are present. My ultimate goal is to apply an evolutionary algorithm to my evaluation function so that I can try lots of settings for the specific value of any one factor and have my program battle against itself many times to find out which settings are the best. After several self-battles, I would have it battle against a human expert, with those human battles weighted more than its self-battles. The purpose of the human-battle is to prevent it from becoming the "neighborhood champion", where it is only good in comparison to itself. By forcing it to battle against an expert, it prevents it from getting stuck at a local maximum, where it found a strategy that's only good against the sorts of strategies the AI is likely to use. The purpose of having it battle against itself is simply because it can battle much faster than any human. The self-play strategy was used to create [TD-Gammon](http://www.research.ibm.com/massive/tdl.html), a backgammon program of approximately equal strength to the best human backgammon players in the world.

### Expectiminimax

Pokemon is a game with random variables in it, so a modification of the simple minimax algorithm is required to account for this. The traditional minimax algorithm makes the play such that the opponent's best response is the worst. In other words, if both players have two moves available, with the AI having moves A and B, and the foe having moves 1 and 2, if the combination of A1 gives a score of 1, and A2 gives a score of 200, the AI will assume the foe will use move 1, giving a score of 1. If B1 gives a score of 3, and B2 gives a score of 2, the AI will assume the foe will use move 2, giving a score of 2. Therefore, the AI will use move B, as even if the foe makes the best counter to my move, my score is the best it can be. Expectiminimax is a slight alteration to this in that if there is a 40% chance the move will give a score of 10, and a 60% it will give a score of 50, then the expected score is .4 * 10 + .6 * 50 = 34.

### Transposition Table

A problem with the search algorithm I use is that it can evaluate the same position many times if it is possible to arrive at the same position through different means. This is known as a "transposition" in chess. Technical Machine has a transposition table to help solve this problem. TM creates a hash for each team and the battle field. It then uses this hash to index an array that stores the score at that state and the depth. If the depth saved is at least as deep as the depth of the current search, TM can use the saved value instead of computing it again.

### Action Prediction

Predicting which action the foe will select is generally seen as the largest part of skill in Pokemon. My program, however, currently does not do any prediction. Its moves are entirely the result of the expectiminimax function and the evaluation function. This seems like the hardest part of Pokemon to tackle, but not having any action prediction would seem to make master-level play impossible. A plan for the future is for Technical Machine to use the concept of [yomi layers](http://doublewise.net/pokemon/yomi.php).

# Technical Machine installation instructions

For a full overview of the program, see http://doublewise.net/pokemon/

## Prerequisites

* clang 7.0+ and libc++ (clang's standard library)
* Boost 1.67.0+
* fltk is needed the stand-alone team predictor / builder. It is not needed to actually run the battling AI.
* CMake 3.14+
* A build tool for CMake to generate (Ninja is recommended)

## Building (Linux)

* `cd path/to/technical-machine`
* `mkdir build`
* `cd build`
* `cmake .. -G"Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release` (you can use `-G"Unix Makefiles"` instead if you do not install ninja)
* `ninja`
* Fill in server information for the server you want to connect to, as found in "build/settings/settings.xml".
* Run the program with `./ai [depth]`, where depth is an optional value that tells Technical Machine how many turns ahead to look (defaults to 2).

### Build targets

ai
:	The AI with maximum optimizations. Accepts a single, optional command-line argument, the depth to search. Higher depth means stronger play, but it also takes longer to search. If no value is entered, 2 is assumed. Recommended at 1 through 5, depending on how fast your computer is and whether you built in release mode (the default). If you set the depth of search to 0, Technical Machine moves randomly.

predict
:	Team predictor. Enter in Pokemon already seen and it shows its prediction of the remaining team. If a Pokemon is put into the first slot, it is assumed to be the lead Pokemon. If no Pokemon is put in that slot, no lead stats are used.

tm_test
:	Runs some tests to verify there are no regressions. Should be run before committing anything.
