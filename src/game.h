#ifndef GAME_H
#define GAME_H

#include "options.h"

#include <string>
#include <vector>
#include <memory>

/**
 * @file game.h
 * @brief This file contains the declaration of the Game class, which is responsible for the Konane gameplay and user interaction.
 */

/**
 * @class Game
 * @brief This class represents a game of Konane.
 */
class Game {
 public:

  /**
   * @enum Stone
   * @brief This enum represents the different types of stones in the game.
   */
  enum class Stone { kEmpty, kBlack, kWhite };

  /**
   * @brief Constructor for the Game class
   * @param options The game options
   */
  Game(const Options& options); 

  /**
   * @brief Prompt the players for their names
   */
  void PromptForPlayerNames();

  /**
   * @brief Start the game loop. End when game is over.
   */
  void Play();

  /** 
   * @brief Display the funny welcome message!
   */
  void Welcome();

  /**
   * @brief Get the source position of the stone from the user
   * @param prompt The prompt message
   * @return A pair of integers representing the row and column of the source position
   */
  std::pair<int, int> GetSource(const std::string& prompt);

  /**
    * @brief Validate the source position of the stone
    * @param input The input string as column(s)row from user
    * @return True if the input is valid, false otherwise
    */
  bool IsValidInput(const std::string& input);

  /**
   * @brief Convert the column letter to the column index
   * @param columnLetter The column letter
   * @return The column index
   */
  int ConvertLetterToColumnIndex(const std::string& columnLetter);

  /**
   * @brief Convert the row number to the row index
   * @param rowNumber The row number
   * @return The row index
   */
  int ConvertNumberToRowIndex(const std::string& rowNumber);

  /**
   * @brief Checks if the stone is valid
   * @param printErrors A boolean flag indicating whether to print errors
   * @return True if the stone is valid, false otherwise
   */
  bool IsValidStone(bool printErrors = true) const;

  /**
  * @brief Get the player's stone
  * @param player The name of the player
  * @return The stone of the player
  */
  Stone GetPlayerStone(const std::string& player) const;

  /**
  * @brief Convert the stone to a string
  * @param stone The stone
  * @return The string representation of the stone
  */
  std::string StoneToString(Stone stone) const;

  /**
   * @brief Show all valid moves for the current player
   */
  void ShowAllValidMoves();

  /**
   * @brief Show the remaining valid moves for the current player
   */
  void ShowRemainingValidMoves();

  /**
   * @brief Get the destination position of the stone and make move
   * @param source The source position of the stone
   * @return A pair of integers representing the row and column of the destination position 
   */
  std::pair<int, int> GetDestinationAndMakeMove(std::pair<int, int> source);

   /**
   * @brief Make the move
   * @param row The destination row of the stone
   * @param col The destination column of the stone
   */
  void MakeMove(int row, int col);

  /**
    * @brief Check if the jump is valid
    * @param row The destination row of the stone
    * @param col The destination column of the stone
    * @param printErrors A boolean flag indicating whether to print errors
    * @return True if the jump is valid, false otherwise
    */
  bool IsValidJump(int row, int col, bool printErrors = true);

  /**
    * @brief Check if the player can make multiple jumps
    * @param row The source row of the stone
    * @param col The source column of the stone
    * @return True if the player can make multiple jumps, false otherwise
    */
  bool CanMakeMultipleJumps(int row, int col);
 
  /**
   * @brief Switch the player
   * If two human player game, it switches the current player.
   * If one human player and one Koko the Computer player game, it switches the current player and makes a move for Koko.
   * If Koko has no valid moves, you win!
   */
  void SwitchPlayer();

  /**
   * @brief Check if the game is over
   * @return True if the game is over, false otherwise
   */
  bool IsGameOver();

  /**
   * @brief Check if the player has valid moves
   * @param stone The stone of the player
   * @return True if the player has valid moves, false otherwise
   */
  bool HasValidMoves(Stone stone);

 /**
   * @brief Get the winner of the game
   * @return The name of the winner
   */
  std::string GetWinner() const;

  /**
   * @brief Display an awesome congratulations message!
   */
  void Congratulations();

  /**
   * @brief Get the number of rows on the game board
   * @return The number of rows
   */
  int GetNumRows() const;

  /**
   * @brief Get the number of columns on the game board
   * @return The number of columns
   */
  int GetNumCols() const;

  /**
    * @brief Get the game board
    * @return The game board
    */
  const std::vector<std::vector<Stone>>& GetBoard() const;

  /**
    * @brief Set the stone on the game board
    * @param row The row of the stone
    * @param col The column of the stone
    * @param stone The stone to set
    */
  void SetStone(int row, int col, Stone stone);

  /**
    * @brief Draw the game board
    */
  void DrawBoard() const;

 private:

 /**
   * @brief Initialize the game board
   */
  void InitializeBoard();

  /**
    * @brief Get the column label
    * @param col The column index
    * @return The column label
    */
  std::string GetColumnLabel(int col) const;


  // A boolean flag indicating whether the game is played with a computer.
  bool computer_game_over_ = false;

  // The number of rows in the game board.
  int num_rows_;

  // The number of columns in the game board.
  int num_cols_;

  // The source row of the stone
  int prev_row_;

  // The source column of the stone
  int prev_col_;

  // The name of player 1.
  std::string player1_;

  // The name of player 2.
  std::string player2_;

  // The name of the current player.
  std::string current_player_;

  // The name of the winner.
  std::string winner_;

  // The game board.
  std::vector<std::vector<Stone>> board_;
};

#endif  // GAME_H