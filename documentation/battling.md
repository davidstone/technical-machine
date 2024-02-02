# Battling

Here, "Battling" is defined as everything that comes after both trainers have sent out their first Pokemon. This excludes team building, and it also excludes the team selection process in versions of the game that have team reveal. Technical Machine currently does not support anything special from team reveal.

## Overview

Technical Machine assigns a probability to each of the opponent's possible selections, evaluates the expected score of each of its own selections, and then makes selection that performs best against that distribution, on average. How long it spends on this is determined by two search depth parameters. This is an extension of a minimax search algorithm to let it work in a simultaneous move game.

## Action Prediction

To estimate the foe's actions, Technical Machine currently runs an expectiminimax algorithm from the foe's perspective and weights by the evaluated score. This is equivalent to the foe trying to maximize their score in a game in which Technical Machine gets to know their selections before it determines its own selections. The worst selection the foe can make as determined by this scoring algorithm is assumed to never be used (unless all selections score the same), and the the remaining selections are weighted by their differences in relative scores.

The exact weighting algorithm is just some arbitrary math. and the ability of this algorithm to predict the actual actions of the foe is quite poor -- it seems to do worse on average than random guessing. It also takes a long time to come up with this answer, which limits Technical Machine's ability to search deeper.

In the near future, Technical Machine will use a much simpler algorithm for action prediction, most likely based on hand-tuned heuristics like "Moves that do more damage are more likely to be used". Longer term, this looks like an area with good potential for traditional machine learning techniques.

## Searching the game state

After generating the estimated probability distribution of the foe's selections, Technical Machine tries to determine the expected value of each of its own selections. It does this with a series of searches.

The first search looks at every possible game state that results from each combination of actions up to some number of turns in the future. The number of turns to look ahead is specified by a constant depth parameter.

After Technical Machine has looked that number of turns ahead, it begins its second layer of searching. For this, it considers every 1v1 match-up of remaining Pokemon on each team. If the Pokemon is not currently out, it is assumed to switch in without the opponent acting or any end-of-turn effects advancing. Then for each 1v1 match-up, the original search algorithm repeats (predict foe selections, evaluate position for each of Technical Machine's selections) up to some number of turns in the future. For each of those terminal states, Technical Machine evaluates the score of the game state using a traditional evaluation function. If one side has more Pokemon remaining than the other side, the side with more Pokemon is assumed to have a match-up against a Pokemon it beats 100% of the time for purposes of scoring to account for this advantage.

## Evaluation

The actual evaluation function is [quite simple](../settings/4/OU/evaluate.json). The only factors it considers are remaining HP%, whether the Pokemon has been revealed, and entry hazards. Each of these values are given a weight. The existing weights were picked arbitrarily and then tuned once to make Stealth Rock less valuable, but otherwise have remained the same since Technical Machine's inception.

The evaluation function used to account for many more factors (things like stat boosts, Reflect, status, etc). However, removing those factors from consideration speeds up evaluation and can be accounted for more organically by just searching to a greater depth.

In the future, rather than giving a Pokemon a score bonus for not being revealed, one possible avenue is to give a bonus for maximizing the error on team prediction. In other words, hiding that you have a Snorlax in Generation 2 is worth very little because the other player already assumes that, but hiding whether it has Belly Drum or Curse might be worth a lot more.

## Long-Term Thinking

Also referred to as strategy, long-term thinking is being able to realize that if I could take out Blissey, Calm Mind Raikou can sweep their team easily, therefore I can sacrifice anything on my team (other than Raikou) to take out Blissey and win. Long-term thinking is the ability to accept any short-term loss of material for a powerful enough positional gain. Forms of AI that attempt to search large portions of the game are bad at this, because there simply isn't enough time to search deep enough to find these positional advantages.

The purpose of that second-level evaluation function that considers all the 1v1 matchups is my attempt at solving this problem. It teaches Technical Machine to value Pokemon that can break through many Pokemon on the foe's team, and to not value Pokemon that lose to the Pokemon on the foe's team.

## Transposition Table

A problem with the search algorithm I use is that it can evaluate the same position many times if it is possible to arrive at the same position through different means. This is known as a "transposition" in chess. Technical Machine has a transposition table to help solve this problem. TM creates a hash for each team and the battle field. It then uses this hash to index into a hash table that stores the score at that state and the depth. If the depth saved is at least as deep as the depth of the current search, TM can use the saved value instead of computing it again.
