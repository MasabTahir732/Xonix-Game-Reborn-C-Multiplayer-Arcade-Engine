<div align="center">



\# 👾 Xonix Reborn: C++ Multiplayer Arcade Engine



\*A terminal-based arcade game built entirely in C++ showcasing complex data structures, state management, and real-time collision detection.\*



!\[C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge\&logo=c%2B%2B\&logoColor=white)

!\[Data Structures](https://img.shields.io/badge/Data%20Structures-Grid%20Traversal-orange?style=for-the-badge)

!\[Game Dev](https://img.shields.io/badge/Game%20Logic-Collision%20Detection-green?style=for-the-badge)



</div>



\---



\## 📖 Overview

This project is a fully functional clone of the classic arcade game \*\*Xonix\*\*, developed from scratch in C++. It serves as a practical implementation of core Data Structures and Algorithms (DSA). The game engine handles real-time rendering, dynamic boundary filling, and enemy AI without relying on external game engines like Unity or Unreal.



\## 🎮 Watch the Gameplay

\*(Upload a quick demo of your game to YouTube, make it Unlisted, and paste the link here!)\*



\## ✨ Core Systems \& Architecture

The project is built using a highly modular, object-oriented C++ architecture:



\* \*\*Game State Management:\*\* Handles dynamic switching between the `Main Menu`, `Login\_Page`, `Inventory\_Page`, and `LeaderBoardPage`.

\* \*\*Real-Time Data Structures:\*\* Implements complex grid traversal algorithms to detect enclosed areas, manage player trails, and process immediate boundary fills.

\* \*\*Multiplayer \& Matchmaking:\*\* Features custom logic for player pairing (`Match\_making.h`) and localized multiplayer modes (`Multiplayer\_Page.h`).

\* \*\*Persistence Engine:\*\* Uses file I/O (`SaveGame.h`, `saved\_game.txt`) to maintain player progress, friend lists (`Friends.h`), and persistent high scores.



\## 🚀 Getting Started



\### 1. Clone the Repository

```bash

git clone \[https://github.com/MasabTahir732/Xonix-Game-Reborn-C-Multiplayer-Arcade-Engine.git](https://github.com/MasabTahir732/Xonix-Game-Reborn-C-Multiplayer-Arcade-Engine.git)

cd Xonix-Game-Reborn-C-Multiplayer-Arcade-Engine


\### 2. Compile and Run
Ensure you have a standard C++ compiler installed. Compile the main application:

```bash
g++ main.cpp -o xonix_game
./xonix_game
