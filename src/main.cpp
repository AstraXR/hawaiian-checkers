/**
 * CISC 187 - Data Structures in C++ 
 * Professor Dave Parillo
 * @author Astra Su
 * ID: 0005623258
 */ 

#include "game.h"
#include "options.h"

#include <iostream>
#include <string>

// Entry point of the game.
int main(int argc, char* argv[]) {
  // Parse command-line arguments and initialize game options.
  Options options(argc, argv);

  // Initialize the game with the specified number of rows and columns.
  // Player names are also set in the Game constructor.
  Game game(options);

  // Start the game.
  game.Play();

  return 0;
}
