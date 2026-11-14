---
layout: chapter
title: "Nim"
subtitle: "Discovering an Incomplete Strategy"
chapter: 1
permalink: /learning-to-play/nim/
---

Nim is rather simple game. A pile of sticks is placed between two players. They take turns taking 1, 2, or 3 sticks. The player to pick up the last stick wins. Other versions of the game exist involving multiple piles or playing such that the person to pick up the last stick loses.

First, you may have noticed we didn't say anything about the number of sticks in the initial pile. That can be any positive numnber. As we will see, the probability of winning depends on the number of sticks to start. Thus, we will discuss results of agents playing games with piles varying between 4 and 99 sticks. 

The cases where the starting number of sticks is less than 4 are well understood: Since all agents are endgame aware, if there are less than 4 sticks, the first agent to play will take all the sticks and win.

## Playing Naively
We begin with the simplest of agents: NaiveRandom. The agent plays by the following rule:

```
if 3 or fewer sticks remain
   take them all
else 
   randomly take 1, 2, or 3
```

For the first experiment, we let NaiveRandom play against itself. The results are shown in Figure 1. 

<figure class="numbered">
  <img src="nr_vs_nr.svg"
       alt="NaiveRandom vs NaiveRandom win rates">
  <figcaption>
    Win rate for NaiveRandom against NaiveRandom as a function of the initial pile size.
  </figcaption>
</figure>

Already, we see the impact of the starting number of sticks.

## A First Strategy
A natural next step a new player might attempt is simply taking a set number of sticks until the endgame. The agent that always takes *N* sticks is called, NaiveContsant*N*. Figure 2 shows the probability of NaiveRandom, going first, winning against each of the NaiveConstant*N*s. 

<figure class="numbered">
  <img src="nr_vs_nc1_nc2_nc3.svg"
       alt="NaiveRandom vs NaiveConstantN win rates for NaiveRandom">
  <figcaption>
    Win rate for NaiveRandom against NativeConstantN when going first.
  </figcaption>
</figure>

Since seating order can affect the outcome of games, we also need to inspect the games where NaiveRandom goes second. Figure 3 shows the probability of NaiveConstant*N*, going first, winning against NaiveRandom.

<figure class="numbered">
  <img src="nc1_nc2_nc3_vs_nr.svg"
       alt="NaiveConstantN vs NaiveRandom win rates for NaiveRandom">
  <figcaption>
    Win rate for NaiveRandom against NativeConstantN when going second.
  </figcaption>
</figure>

Of course, we should also see how each NaiveConstant*N* agent performs against itself and the other NaiveConstant*N* agents. Figure 4 provides interactive graphs illustrating the match ups (hover over the labels to isolate graphs).

<figure id="fig-constant-strategies">
  <iframe
    src="{{ 'nim_constants_interactive.html' }}"
    width="100%"
    height="1460"
    style="border: none;"
    title="Nim Constant Strategy Comparisons">
  </iframe>

  <figcaption>
    <strong>Figure 4.</strong>
    First-player win probabilities for the three constant
    strategies against one another, for initial pile sizes
    4 through 99.
  </figcaption>
</figure>

## Pile Residues
The next step involves considering the state of the pile that is left for the opponent. Since we are taking 1-3 sticks, this will leave the piles in residues mod 4. This brings us to our first Enthusiasts: EnthusiastResidue*N* will make choices to leave the stick pile with residue mod 4. Thus, we have EnthusiastResidue0 (eR0), EnthusiastResidue1 (eR1), EnthusiastsResidue2 (eR2), and EnthusiastResidue3 (eR3). When multiple moves are available, the choice is random.

<figure id="fig-constant-strategies">
  <iframe
    src="{{ 'nim_tournament_explorer.html' }}"
    width="100%"
    height="1650"
    style="border: none;"
    title="Exploring the Nim Strategy Tournament">
  </iframe>

  <figcaption>
    <strong>Figure 4.</strong>
    Interactive results from a tournament of eight Nim-playing agents over initial pile sizes of 4–99. Each cell in the heat map represents the first player's win percentage against the second player, with color indicating the degree of advantage or disadvantage. Select a cell to examine the results by initial pile size, or adjust the pile-size range to investigate how the relative performance of strategies changes. Diagonal cells are omitted because self-play results were not included in the tournament data.
  </figcaption>
</figure>

Clearly, something stands out: leaving your opponent a pile containing a multiple of 4 sticks is a strong strategy. If you are familiar with Nim, you likely knew that. But the interesting thing we see here is that while it is an optimal strategy, it is not infallible. That is where things get interesting.

Before exploring that apparent contradiction, we should explain why leaving your opponent a multiple of 4 sticks is optimal. Recall that a player may remove 1, 2, or 3 sticks on each turn, and the player who takes the last stick wins. If you leave your opponent a multiple of 4, they must remove 1, 2, or 3 sticks. You can then remove 3, 2, or 1 sticks, respectively, so that the two moves together remove exactly 4 sticks. This returns your opponent to another multiple of 4. By repeating this process, you eventually leave your opponent with 4 sticks. Whatever they remove, you can take the remaining sticks and win. Of course, this strategy requires that you first have an opportunity to leave a multiple of 4. If you begin with a multiple of 4 yourself, a sufficiently clever opponent can use the same strategy against you.

## Piles of Other Multiples
What we observed is leaving your opponent a pile that is a multiple of 4 is advantageous. However, we can leave piles in other multiples. The other natural choices are multiples of 2 and multiples of 3 but we will also examine multiples of 8. 