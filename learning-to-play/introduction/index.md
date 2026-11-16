---
layout: chapter
title: "Introduction"
subtitle: ""
chapter: 1
permalink: /learning-to-play/introduction/
---

*The story of Atelier.*

In the spring of 2026 I was teaching an object-oriented programming course and was facing another semester with the same game project: I loosely define a game and the students implement it. The approach worked and students liked the freedom and creativity it required. 

In the beginning of each semester, I would pick and develop the game myself in order to evaluate its appropriateness for the class. This time I chose Reversi. While implementing the game mechanics, I created simple agents to play the game to ensure the mechanics worked. Then I started creating more complex agents and realized that creating the agents was where I was having fun: how to you model a strategy?

I decided to take that approach with the class. I implemented a series of games (Nim, Tic-Tac-Toe, and Mancala) and the students built agents to play and test. It was a simple study, just logging wins, losses, and draws (where appropriate). 

Each game was a standalone system. This involved writing redundant code that ran the games and reported the results. After the semester I decided to integrate the games into a common framework, now called *The Arena*. The Arena executes the game and manages Arena-level statistics. Games implement their unique mechanics and manage game level statistics.

Between May 2026 and November 2026, The Arena become stable and numerous games were integrated:

- Nim
- Tic-Tac-Toe
- Four in a Row (Connect Four)
- Reversi (Othello)
- Mancala
- Battleship
- Mexican Train Dominoes
- Backgammon
- Potion Commotion (novel)
- Fold or Fortune (novel)
- Bold Claims (Liar's Dice variant)


## GAMESTRAT: Games, Chance, and Strategy

In the fall of 2026 I was invited to create a course to execute in spring 2027. The course - GAMESTRAT: Games, Chance, and Strategy -  was designed around the study of board games. The approach was 
- Students play the physical game
  - Randomly at first, using dice
  - Then develop strategies to test against random play.
- Once a strategy was considered stable, students were invited to implement it in code (or via AI) and see how it performed in The Arena.

To further support the course, I created *The Arcade*. The Arcade consists of playable versions of each of the games included in the course (and a few more). The agents in The Arcade are the result of studying games with the framework.

Together, The Arena and The Arcade make up *Atelier* 
- The Arena provides a place to test strategies computationally 
- The Arcade provides a place to experience those strategies through play

## What Atelier Isn't
Atelier is not about perfection. It's about understanding how small changes impact an outcome. It was born out of wondering what strategy might be "good enough".

In reflection on game play, I realized, for me, games are supposed to be fun. I don't want to play on autopilot, but I also don't want to expend too much energy. So, given a game's mechanics, what might simply be *good enough*? I might characterize "good enough" as being an interesting opponent. 

## Agents
At the heart of the study is the *agent*. An agent represents a player in a game and essentially is the embodiment of a strategy encoded into an algorithm implemented in such a way it fits into the framework. There are four categories of agents: Naive, Enthusiast, Strategist, and Expert. 

Keep in the mind, the agent category is more about the approach, not necessarily categories of performance. A Naive agent can beat a Strategist from time to time. On average, though, 

- Expert beats Strategist
- Strategist beats Enthusiast
- Enthusiast beats Naive

### Naive Agents
Every game study begins in the same place with an agent we refer to as *Naive*. A Naive agent is one that 

- knows the rules
- knows how to make a move
- can notice a winning move (called *endgame aware*)

There can be multiple naive agents implemented for a game. The main characteristic is that they no what a move is but don't typically distinguish one move from another. In fact, most games started with an agent that would take a win if it could recognize it but otherwise would randomly select an action from the set of all legal moves. 

Another type of naive agent might simply choose the same move or type of move every time. For example, a Naive Nim might always take 1 stick unless it can win.

The Naive agent was born from physical game play. Students would first play a game using some type of random number generator, typically a die. That is, rather than deciding a move, the would roll a die and take the associated action. For example, when playing Nim, if there were more than 3 sticks in the pile, students would roll a 4-sided die (reroll on a 4). The purpose of this is students could observe the game without influencing it. By watching random moves yield bad results helped build an initial strategy.

### Enthusiast Agents
The next level agents are called *Enthusiasts*. An Enthusiast agent recognizes important game mechanics and prioritizes them without necessarily understanding their strategic implications. Different Enthusiasts may prioritize different mechanics, representing different approaches to learning the game.

For example, in Mancala there are two strategic moves: capture your opponents seeds and earn an extra turn. One Enthusiast might prioritize capturing seeds over the extra turn whereas another might do the opposite.

### Strategist Agents
Agents that leverage the relationships between game mechanics and and their consequences are referred to as *Strategists*. Agents might attempt to anticipate consequences rather than simply react to opportunities.

For example, in Reversi, an Enthusiast might recognize edge squares are valuable but given multiple edge options may choose randomly between them. A Strategist may take into account the number of moves it leaves its opponent (mobility).

### Expert Agents
The most sophisticated strategies are encoded in *Expert* agents. An Expert agent possesses a deep understanding of the game and employs an optimal or near-optimal strategy.

We do not encounter many Expert agents in this journey. The goal was not to develop an Expert, but rather to understand the impact of incremental changing strategic elements.

