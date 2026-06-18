# XONIX — Classic Arcade Game Recreation in C++ & SFML

---

## **Overview**

A complete implementation of the classic **Xonix** arcade game developed using **C++** and the **SFML (Simple and Fast Multimedia Library)** framework.

This project was developed as a team assignment for a Programming Fundamentals course and introduces core game development concepts such as event handling, collision detection, game state management, file handling, and real-time gameplay systems.

The game features multiple game modes, difficulty levels, power-ups, enemy AI, score tracking, and leaderboard functionality.

---

## **Tech Stack**

**Language:** C++

**Library:** SFML

**Concepts Used:**

* File Handling
* Arrays & Grid-Based Systems
* Collision Detection
* Game State Management
* Event Handling
* Object-Oriented Programming

---

## **Game Modes**

### Single Player Mode

Play against computer-controlled enemies while capturing territory and maximizing your score.

### Two Player Mode

Compete with another player on the same grid and attempt to control the largest area.

---

## **Gameplay**

When the game starts, players are welcomed with:

```text
Welcome to Xonix!

1 → Single Player Mode
2 → Two Player Mode
```

Players spawn at opposite corners of the map.

The outer boundary of the grid is already established, and players must move strategically to capture additional territory while avoiding enemy collisions.

The objective is to expand controlled territory, collect power-ups, survive enemy encounters, and achieve the highest possible score.

---

## **Features**

* Single Player Mode
* Two Player Mode
* Multiple Difficulty Levels
* Dynamic Enemy Movement Patterns
* Territory Capture Mechanics
* Power-Up System
* Score Tracking
* Collision Detection
* Interactive Menus
* Background Music
* File-Based Leaderboard System

---

## **Programming Concepts Demonstrated**

This project showcases several fundamental programming and software development concepts:

* Real-Time Game Loop Design
* Event-Driven Programming
* File Input and Output
* Data Management Using Arrays
* Collision Detection Algorithms
* State-Based Game Architecture
* Audio and Graphics Integration
* Multiplayer Gameplay Logic

---

## **How to Run**

### Requirements

* C++ Compiler (G++)
* SFML Library

### Compile

```bash
g++ main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

### Run

```bash
./game
```

---

## **Project Goals**

The primary objective of this project was to apply Programming Fundamentals concepts in the development of a complete interactive application.

Through this project, team members gained practical experience with:

* C++ Application Development
* Game Programming Fundamentals
* Team-Based Software Development
* Debugging and Testing
* User Interaction Design
* Managing Larger Codebases

---

## **Course Context**

Developed as a Programming Fundamentals course project to introduce students to software engineering practices and game development using C++ and SFML.
