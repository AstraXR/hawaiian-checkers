/**
 * CISC 187 - Data Structures in C++ 
 * Professor Dave Parillo
 * @author Sophia Su
 * ID: 0005623258
 */ 

#include "options.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <string>

/**
 * Constructor for the Options class
 * @param argc The number of command-line arguments
 */
Options::Options(int argc, char* argv[]) {
  play_with_computer_ = false;
  srand(time(0));
  num_rows_ = rand() % 28 + 3;
  num_cols_ = rand() % 27 + 4;
  player1_ = "";
  player2_ = "";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h") {
      DisplayHelp();
      exit(0);
    } else if (arg == "-c") {
      play_with_computer_ = true;
    } else if (arg == "-n" || arg == "-m") {
      int value;
      if (i + 1 < argc && argv[i+1][0] != '-') {
        try {
          value = std::stoi(argv[++i]);
        } catch (const std::invalid_argument& e) {
          std::cerr << "\nOops! Invalid argument for " << arg << ": " << e.what() << "\n";
          DisplayHelp();
          exit(1);
        }
      } else {
        value = (arg == "-n") ? rand() % 28 + 3 : rand() % 27 + 4;
      }
      if (arg == "-n") {
        if (value < 3 || value > 30) {
          std::cerr << "\nThe number of rows must be between 3 and 30.\n";
          DisplayHelp();
          exit(1);
        }
        num_rows_ = value;
      } else {  // arg == "-m"
        if (value < 4 || value > 30) {
          std::cerr << "\nThe number of columns must be between 4 and 30.\n";
          DisplayHelp();
          exit(1);
        }
        num_cols_ = value;
      }
    } else {
      std::cerr << "\nOopsies! Unknown option: " << arg << "\n";
      DisplayHelp();
      exit(1);
    }
    if (play_with_computer_) {
      player2_ = "Koko the Computer";
    }
  }
}

/**
 * Displays a beautiful help message and the rules of Konane
 */
void Options::DisplayHelp() const {
  std::cout << R"(
==============================================================================
                       __ .-~-.   .~``~,._
                     .~  `     \ /     .  `\
                     |     \    |   .'     |
               _      \     `.  |  /    __/
            .~` `'. .--;.       ,.O  -~`   `\
            \  '-. |     `-  o.O/o, __       |
             '-.,__ \    .-~' `\|o `  ~.    /_
               _.--'/   `    ,  /  \        | `~-.,
              /     |       /  :    '._    / -.    `\
            /   .-' '.___.;   `      \`--'\    `    |
            |          /    \         /     '.__,.--,/
            '--..,___.'      `~--'--~'

==============================================================================
                            KONANE RULES            
==============================================================================
The game is played on a rectangular board with black and white stones placed 
in an alternating pattern. There is one black and one white stone removed.
The objective of the game is to make the last move. On one's turn, a player 
uses their piece to vertically or horizontally jump over an opponent's piece. 
Any piece that is jumped over is then removed from the board. Only horizontal 
and vertical jumps are allowed. Multiple jumps are allowed if the player has 
more valid moves with the same stone.  The player who has no valid moves 
left loses the game. Thus, the player who makes the last move wins!
==============================================================================
                            KONANE OPTIONS            
==============================================================================

Usage: game [-h] [-c] [-n ROWS] [-m COLS]

Options:
  -h        Display this help message and exit.
  -c        Play against the computer.
  -n ROWS   Set the number of rows on the game board (default: random).
            The number of rows must be between 3 and 30.
  -m COLS   Set the number of columns on the game board (default: random).
            The number of columns must be between 4 and 30.

                           Enjoy the game!
==============================================================================
)";
}

/**
 * @return Returns the name of player 1.
 */ 
std::string Options::Player1() const { 
  return player1_; 
}

/**
 * @return Returns the name of player 2.
 */ 
std::string Options::Player2() const { 
  return player2_; 
}

/**
 * @return Returns True if the user chose to play against Koko, false otherwise.
 */ 
bool Options::PlayWithComputer() const { 
  return play_with_computer_; 
}

/**
 * @return Returns the number of rows on the game board.
 */ 
int Options::NumRows() const { 
  return num_rows_; 
} 

/**
 * @return Returns the number of columns on the game board.
 */ 
int Options::NumCols() const { 
  return num_cols_; 
}
