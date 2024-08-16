
# Hawaiian Checkers - Konane Game

## Description

This project implements the Konane game, also known as Hawaiian Checkers. It's a two-player strategy game played on a rectangular board with black and white stones.

## Features

- Play against another human player or against Koko the Computer
- Customizable board size (3-30 rows, 4-30 columns)
- Command-line interface with various options
- Implements game rules including multiple jumps

## Files

- `main.cpp`: Entry point of the game
- `game.h` and `game.cpp`: Implements the game logic and board representation
- `options.h` and `options.cpp`: Handles command-line options and game settings
- `computer_player.h` and `computer_player.cpp`: Implements the AI player (Koko the Computer)

## How to Compile

```bash
g++ -std=c++17 main.cpp game.cpp options.cpp computer_player.cpp -o konane
```

## How to Run

```bash
./konane [-h] [-c] [-n ROWS] [-m COLS]
```

## Command-line Options

- `-h`: Display help message and game rules
- `-c`: Play against the computer
- `-n ROWS`: Set the number of rows (3-30, default: random)
- `-m COLS`: Set the number of columns (4-30, default: random)

## Game Rules

1. The game is played on a rectangular board with black and white stones in an alternating pattern.
2. One black and one white stone are initially removed.
3. Players take turns jumping over opponent's pieces, removing them from the board.
4. Only horizontal and vertical jumps are allowed.
5. Multiple jumps are allowed in a single turn if possible.
6. The player who makes the last move wins.

## Acknowledgments

- Professor Dave Parillo, CISC 187 - Data Structures in C++

