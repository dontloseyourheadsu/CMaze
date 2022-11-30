# CMaze

## Description

Terminal maze game written in C.

## Objective

Clear the maze by taking the player to the goal without going through the walls. You can collect coins to get a better score. Try to get the max score!

## Symbols

- Goal - ‘#’
- Player - ‘o’
- Wall - ‘-’
- Path - ‘ ‘
- Path to solution - ‘\*’
- Fog - ‘░’
- Coins - ‘$’

## Gameplay

You can move by entering the following commands:

- Walk Up - ‘w’
- Walk Right - ‘d’
- Walk Down - ‘s’
- Walk Left - ‘a’

You can input several inputs during a turn, but that doesn’t mean you will be able to cross walls.

The game has 4 types of levels:

- Easy (9x9 maze)
- Medium (10x20 maze)
- Hard (29x29 maze)
- Fog (9x9 maze). There is fog and you can’t see beyond one space.

Collect coins to improve your score.

In case you aren’t able to complete the game, you can access the solution generator by entering ‘g’. This will show you the path from any point you are at and you can always ask the path again. But be aware that you won't get all the points if you use this aid.
