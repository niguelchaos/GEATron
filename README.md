# GEATron

## Introduction
Academic project of DIT455, Game Engine Architecture.
The source code contains a recreation in C++ of the light-cycle minigame from Tron, the arcade videogame in 1982. More information can be found here: [Tron(1982) Arcade Game](https://en.wikipedia.org/wiki/Tron_(video_game))
Due to time constraints(~3 weeks), not everything is true to the original. This replica is a 2-player game, unlike the original single-player.

## Project Details
Largely created from scratch, a great emphasis was placed on creating the game with basic architecture of modern engines. As such, concepts such as gameobjects, components, messages and certain data structures were used.
This game specifically uses a uniform grid to keep track of wall states.
An effort was made to decouple rendering components from logic, though there are still clear issues.
