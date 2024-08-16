#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <iostream>
#include <random>
#include <string>

/**
 * @file options.h
 * @brief This file contains the declaration of the Options class, which processes command line arguments and stores game settings.
 * It also displays the help message and rules of Konane.
 */
/**
 * @class Options
 * @brief The Options class processes command line arguments and stores game settings. 
 */
class Options {
 public:
  
  /**
   * Constructor for the Options class
   */
  Options(int argc, char* argv[]);

  /**
   * Parse the command line arguments and set the appropriate options
   */
  void ParseCommandLineArguments(int argc, char* argv[]);

  /**
   * Display the help message and rules of Konane
   */
  void DisplayHelp() const;

  /**
   * @return The name of player 1
   */
  std::string Player1() const;

  /**
   * @return The name of player 2
   */
  std::string Player2() const;

  /**
   * @return True if the game is played with the computer, false otherwise
   */
  bool PlayWithComputer() const;

  /**
   * @return The number of rows
   */
  int NumRows() const;

  /**
   * @return The number of columns
   */
  int NumCols() const;

 private:
  // A boolean flag indicating whether the game is played with a computer.
  bool play_with_computer_;

  // The number of rows in the game board.
  int num_rows_;

  // The number of columns in the game board.
  int num_cols_;

  // The name of the first player.
  std::string player1_;

  // The name of the second player.
  std::string player2_;
};

#endif  // OPTIONS_H_