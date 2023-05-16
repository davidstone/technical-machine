# Battling

Here, "Battling" is defined as everything that comes after both trainers have sent out their first Pokemon. This excludes team building, and it also excludes the team selection process in versions of the game that have team reveal. Technical Machine currently does not support anything special from team reveal.

## Minimax / Search

Technical Machine's backbone is the search algorithm. The search algorithm I use essentially considers every possible combination of moves that the two players can make on a given turn. Then it looks at every combination of moves that can be used from each of the possibilities that arose from looking at every outcome of the previous turn. This process continues until it has searched deep enough to arrive at a good conclusion. This is the strategy that chess programs tend to use, and is guaranteed to find the 'best' solution for the depth it can search, but the limit of this sort of strategy is that the game tree branches a lot, making a complete search take a very long time to complete.

If the search algorithm finds a guaranteed win no matter what the opponent does, then it should obviously use the moves that lead to that state. However, this is a rare condition, especially in Pokemon, and so an AI requires something more sophisticated. Technical Machine assigns a probability to each of the opponent's possible actions, and then makes the move that performs best against that distribution, on average. This is an extension of a minimax search.

Minimax algorithms rely on an evaluation function. The evaluation function is a way to score the game when it's somewhere in between "guaranteed win" and "guaranteed loss". A sample evaluation function could look something like giving 10 points for every Pokemon that's alive on a team, and 1 point for every % HP that each remaining Pokemon has. More complex evaluation functions can give more accurate estimates of the state of the game.

Technical Machine's evaluation function is a more complex function. The first level of evaluation is actually a second search, but this time it evaluates every 1v1 matchup between the two teams. Each of these search paths are scored by a second level evaluation function, which is super simple (it looks at whether the Pokemon is still alive, how much HP it has, and entry hazards, plus a bonus if that Pokemon has not actually been revealed yet in battle).

## Action Prediction

Predicting which action the foe will select is generally seen as the largest part of skill in Pokemon. Technical Machine does a crude form of prediction by assuming the opponent will analyze the battle the same as Technical Machine, but at one level shallower of search. It then weights all possible moves by the evaluated score resulting from each move, from the foe's perspective (which would in turn involve the foe predicting its foe at one level shallower than that).

## Long-Term Thinking

Also referred to as strategy, long-term thinking is being able to realize that if I could take out Blissey, Calm Mind Raikou can sweep their team easily, therefore I can sacrifice anything on my team (other than Raikou) to take out Blissey and win. Long-term thinking is the ability to accept any short-term loss of material for a powerful enough positional gain. Forms of AI that attempt to search large portions of the game are bad at this, because there simply isn't enough time to search deep enough to find these positional advantages.

The purpose of that second-level evaluation function, that considers all the 1v1 matchups, is my attempt at solving this problem. It teaches Technical Machine to value Pokemon that can break through many Pokemon on the foe's team, and to not value Pokemon that lose to the Pokemon on the foe's team.

## Transposition Table

A problem with the search algorithm I use is that it can evaluate the same position many times if it is possible to arrive at the same position through different means. This is known as a "transposition" in chess. Technical Machine has a transposition table to help solve this problem. TM creates a hash for each team and the battle field. It then uses this hash to index into a hash table that stores the score at that state and the depth. If the depth saved is at least as deep as the depth of the current search, TM can use the saved value instead of computing it again.
