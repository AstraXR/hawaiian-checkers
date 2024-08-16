/**
* CISC 187 - Data Structures in C++ 
* Professor Dave Parillo
* Author: Sophia Su
* ID: 0005623258
*/ 

#include "game.h"
#include "options.h"
#include "computer_player.h"

#include <algorithm>
#include <cctype> 
#include <iomanip>
#include <iostream> 
#include <iostream>
#include <random> 
#include <stdexcept>
#include <vector>  

/**
 * The Game class represents a game of Konane.
 * @param options The game settings and rules.
 */ 
Game::Game(const Options& options) 
    : player1_(options.Player1()), player2_(options.Player2()), current_player_(options.Player1()), 
      num_rows_(options.NumRows()), 
      num_cols_(options.NumCols()), 
      prev_row_(-1), prev_col_(-1)  {
}

/**
 * Prompts the user to enter the names of the players.
 * If -c was specified, the computer will be the second player.
 */
void Game::PromptForPlayerNames() {
  std::cout << "Enter name for Player 1: ";
  std::cin >> player1_;
  current_player_ = player1_;
  if (player2_ == "Koko the Computer") {
    std::cout << "\n";
    std::cout << "Hi, I'm Koko the Computer! I'll be playing against you! \n\n";
  } else {
    std::cout << "Enter name for Player 2: ";
    std::cin >> player2_;
    std::cout << "\n";
  }
  return;
}

/**
 * Starts the game. Game ends when there are no valid moves for the current player.
 */
void Game::Play() {

  std::string playAgain;
  do {
    InitializeBoard();
    Welcome();
    PromptForPlayerNames();
    std::pair<int, int> source;
    std::pair<int, int> destination;

    DrawBoard();
    std::cout << player1_ << ", you are black. " << player1_ << " moves first. \n";
    std::cout << player2_ << ", you are white. " << player2_ << " moves second. \n";
    
    do {
      ShowAllValidMoves();  
      do {
          std::string stoneColor = StoneToString(GetPlayerStone(current_player_));
          source = GetSource(current_player_ + "'s turn. Select your " + stoneColor + " stone: ");  
          prev_row_ = source.first;  // assign to prev_row_ and prev_col_
          prev_col_ = source.second;  
      } while (!IsValidStone());
    
      destination = GetDestinationAndMakeMove(source); 
      source = destination;
      
      // Check if the current player can make another jump with the same stone
      while (CanMakeMultipleJumps(source.first, source.second)) {
        ShowRemainingValidMoves();
        std::string input;
        std::string currentPosition = GetColumnLabel(source.second) + std::to_string(num_rows_ - source.first);
        std::cout << "Your current position is [" << currentPosition << "]. "
                  << "You can make another jump with the same stone.\n"
                  << "Would you like to continue jumping? (Y/N): ";
        std::cin >> input; 
        ShowRemainingValidMoves();
        if (input.find_first_of("nN") == 0) {
          break;
        }
        
        destination = GetDestinationAndMakeMove(source);
        source = destination;
      }

      SwitchPlayer();
    } while (!IsGameOver());
    Congratulations();
    std::cin >> playAgain;
  } while (playAgain.find_first_of("yY") == 0); 
}

/**
 *  Displays a funny welcome message to the players! :)
 */
void Game::Welcome() {
  std::cout << R"(
===============================================================================================================

                                                .-'''-.             
                                               ' heyo `.
                                               ' hooman! `.
                                                ` , . , '  `..
___       __    ______                                        `..                    .                             
__ |     / /_______  /__________________ ________                ~               | \/|  
__ | /| / /_  _ \_  /_  ___/  __ \_  __ `__ \  _ \     (\   _                  ) )|/|   
__ |/ |/ / /  __/  / / /__ / /_/ /  / / / / /  __/        (/            _----. /.'.' 
____/|__/  \___//_/  \___/ \____//_/ /_/ /_/\___/    .-._________..      .' @ _\  .'
                                                     '.._______.   '.   /    (_| .')  
                _____                                   '._____.  /   '-/      | _.'      
                __  /______                                '.______ (         ) ) \  
                _  __/  __ \                                 '..____ '._       )  )
                / /_ / /_/ /                                   .' __.--\  , ,  // ((
                \__/ \____/                                    '.'     |  \/   (_.'(        
                                                                        '   \ .'
        ______ __                               ______                  \   (
        ___  //_/__________________ _______________  /                   \   '.
        __  ,<  _  __ \_  __ \  __ `/_  __ \  _ \_  /                     \ \ '.)
        _  /| | / /_/ /  / / / /_/ /_  / / /  __//_/                       '-'-'
        /_/ |_| \____//_/ /_/\__,_/ /_/ /_/\___/(_)    
                                                       
                                                    
==============================================================================================================="    
)" << "\n\n";
}

/**
 * Prompts the user to enter the sorce of the stone.
 * @param prompt The prompt to display to the user.
 * @return The row and column of the source.
 */
std::pair<int, int> Game::GetSource(const std::string& prompt) {
  std::string input;
  std::cout << prompt;
  std::cin >> input;

  // Convert the input to uppercase
  for (char& c : input) {
    c = std::toupper(c);
  }

  // Validate the input
  while (!IsValidInput(input)) {
    std::cout << "Invalid input (e.g. 'e3').\n" << prompt; 
    std::cin >> input;
    // Convert the input to uppercase
    for (char& c : input) {
      c = std::toupper(c);
    }
  }

  // Extract the column letter(s) and row number from the input
  int i = 0;
  while (i < input.length() && std::isalpha(input[i])) {
    ++i;
  }
  std::string columnLetter = input.substr(0, i);
  std::string rowNumber = input.substr(i);

  // Convert from letters to column index and from number to row index
  int col = ConvertLetterToColumnIndex(columnLetter);
  int row = ConvertNumberToRowIndex(rowNumber);

  // Ensure the column and row are within the valid range
  if (col < 0 || col >= num_cols_ || row < 0 || row >= num_rows_) {
    std::cout << "Your move is out of bounds. Try again.\n";
    return GetSource(prompt);
  }

  return std::make_pair(row, col);
}

/**
 * Check if the input move from user is valid.
 * @param input The input to validate.
 * @return True if the input is valid, false otherwise.
 */
bool Game::IsValidInput(const std::string& input) {
  // Check the length of the input
  if (input.length() < 2 || input.length() > 4) {
    return false;
  }

  // Check the first character(s) for the column
  int i = 0;
  while (i < input.length() && std::isalpha(input[i])) {
    ++i;
  }
  if (i == 0 || i > 2) {  // No column specified or column is more than two letters
    return false;
  }

  // Check the remaining character(s) for the row
  while (i < input.length() && std::isdigit(input[i])) {
    ++i;
  }
  if (i != input.length() || i > 4) {  // Not all characters were digits or row is more than two digits
    return false;
  }

  return true;
}

/**
 * Converts the column letter(s) to a zero-based index.
 * @param columnLetter The column letter(s) to convert.
 * @return The zero-based index of the column.
 */
int Game::ConvertLetterToColumnIndex(const std::string& columnLetter) {
  int columnIndex = 0;
  for (char c : columnLetter) { 
    columnIndex = columnIndex * 26 + (std::toupper(c) - 'A' + 1);
  }
  return columnIndex - 1;  // Subtract 1 to make it zero-based
}

/**
 * Converts row number to zero-based index.
 * @param rowNumber The row number to convert.
 * @return The zero-based index of the row.
 */
int Game::ConvertNumberToRowIndex(const std::string& rowNumber) {
  return num_rows_ - std::stoi(rowNumber);  // subtract from total to reverse order
}

/**
 * Checks if the stone at the given row and column is valid.
 * @param bool printErrors Whether to print error messages.
 * @return bool True if the stone is valid, false otherwise.
 */
bool Game::IsValidStone(bool printErrors) const {
  // Check if the row and column are within the bounds of the board
  if (prev_row_ < 0 || prev_row_ >= num_rows_ || prev_col_ < 0 || prev_col_ >= num_cols_) {
    if (printErrors) {
      std::cout << "The spot you selected is out of bounds.\n";
    }
    return false;
  }

  // Check if the cell at the given row and column is empty
  if (board_[prev_row_][prev_col_] == Stone::kEmpty) {
    if (printErrors) {
      std::cout << "This spot is empty, there is no stone. Try again.\n";
    }
    return false;
  }

  // Check if the stone is the correct color
  Game::Stone currentStone = (current_player_ == player1_) ? Stone::kBlack : Stone::kWhite;
  if (board_[prev_row_][prev_col_] != currentStone) {
    if (printErrors) {
     std::cout << "You picked the wrong color: " << StoneToString(board_[prev_row_][prev_col_]) 
               << ". Please try again.\n";
    }
    return false;
  }

  // Check if the stone is two away from any empty stone
  for (int row = 0; row < num_rows_; ++row) {
    for (int col = 0; col < num_cols_; ++col) {
      if (board_[row][col] == Stone::kEmpty) {
        if ((abs(row - prev_row_) == 2 && abs(col - prev_col_) == 0) || 
            (abs(row - prev_row_) == 0 && abs(col - prev_col_) == 2)) {
          // Calculate the position of the stone that should be jumped over
          int middle_row = (row + prev_row_) / 2;
          int middle_col = (col + prev_col_) / 2;
          // Check if the stone at the middle position is an opponent's stone
          if (board_[middle_row][middle_col] != Stone::kEmpty && board_[middle_row][middle_col] != currentStone) {
            return true;
          }
        }
      }
    }
  }
  // Checks if the stone is jumping overan opponent's stone
  Game::Stone oppositeStone = (currentStone == Stone::kBlack) ? Stone::kWhite : Stone::kBlack;
  if (printErrors) {
    std::cout << "You must be jumping over a " << StoneToString(oppositeStone) 
              << " stone into an empty space! Try again.\n";
  }
  return false;
}

/**
 * Returns the stone colur of the given player.
 * @param player The name of the player.
 * @return The stone color of the player.
 */
Game::Stone Game::GetPlayerStone(const std::string& player) const {
  if (player == player1_) {
    return Stone::kBlack;
  } else if (player == player2_) {
    return Stone::kWhite;
  } else {
    return Stone::kEmpty;
  }
}

/**
 * Returns the string representation of the stone.
 * @param stone The stone to convert to a string.
 */
std::string Game::StoneToString(Game::Stone stone) const{
    switch (stone) {
        case Game::Stone::kEmpty:
            return "empty";
        case Game::Stone::kBlack:
            return "black";
        case Game::Stone::kWhite:
            return "white";
        default:
            return "unknown";
    }
}

/**
 * Prints all valid first moves for the current player.
 */
void Game::ShowAllValidMoves() {
  std::vector<std::string> validMoves;

  for (int row = 0; row < num_rows_; ++row) {
    for (int col = 0; col < num_cols_; ++col) {
      prev_row_ = row;
      prev_col_ = col;
      if (IsValidStone(false)) {
        std::string currentPosition = GetColumnLabel(col) + std::to_string(num_rows_ - row);
        validMoves.push_back(currentPosition);
      }
    }
  }
  std::cout << "\n" << current_player_ << ", your valid moves are: ";
  for (const auto& move : validMoves) {
    std::cout << "[" << move;
    if (move != validMoves.back()) {
      std::cout << "], ";
    } else {
      std::cout << "].\n";
    }
  }
}

/**
 * Prints the remaining valid moves for the one stone selected by the player.
 */
void Game::ShowRemainingValidMoves() {
    // Direction vectors for vertical and horizontal directions
    std::vector<std::string> validMoves;
    std::pair<int, int> directions[] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    // Check all directions
    for (const auto& dir : directions) {
        int new_row = prev_row_ + dir.first;
        int new_col = prev_col_ + dir.second;

        // If the jump is valid
        if (IsValidJump(new_row, new_col, false)) {
            std::string currentPosition = GetColumnLabel(new_col) + std::to_string(num_rows_ - new_row);
            validMoves.push_back(currentPosition);
        }
    }
    std::cout << "\n" << current_player_ << ", your remaining valid moves are: ";
    for (const auto& move : validMoves) {
        std::cout << "[" << move;
        if (move != validMoves.back()) {
            std::cout << "], ";
        } else {
            std::cout << "].\n";
        }
    }
}

/**
 * Returns the destination of the stone and makes the move.
 * @param source The source row and column of the stone.
 * @return The destination row and column of the stone.
 */
std::pair<int, int> Game::GetDestinationAndMakeMove(std::pair<int, int> source) {
  std::pair<int, int> destination;
  do {
    destination = GetSource("Enter destination: ");
    std::cout << "\n";
  } while (!IsValidJump(destination.first, destination.second));

  // Determine the stone for the move
  Stone stone = (current_player_ == player1_) ? Stone::kBlack : Stone::kWhite;
  
  MakeMove(destination.first, destination.second);
  // Update prev_row_ and prev_col_ after making the move
  prev_row_ = destination.first;
  prev_col_ = destination.second;
  return destination;
}

/**
 * Makes the move on the board and draws the board.
 * @param row The row of the destination.
 * @param col The column of the destination.
 */
void Game::MakeMove(int row, int col) {
  // Make the move
  if (current_player_ == player1_) {
    board_[row][col] = Stone::kBlack;
  } else {
    board_[row][col] = Stone::kWhite;
  }
  if ((abs(row - prev_row_) == 2 && col == prev_col_)) {
    // remove stone in between the jump
    board_[(row + prev_row_) / 2][col] = Stone::kEmpty;
  }
  else if ((abs(col - prev_col_) == 2 && row == prev_row_)) {
    // remove stone in between the jump
    board_[row][(col + prev_col_) / 2] = Stone::kEmpty;
  }
  // Remove the jumped stone if a jump occurred
  board_[prev_row_][prev_col_] = Stone::kEmpty;
  DrawBoard();
}

/**
 * Checks if the jump is valid.
 * @param row The row of the destination.
 * @param col The column of the destination.
 * @param bool printErrors Whether to print error messages.
 * @return bool True if the jump is valid, false otherwise.
 */
bool Game::IsValidJump(int row, int col, bool printErrors) {
   // Check if row and col are within the valid range of the board
  if (row < 0 || row >= board_.size() || col < 0 || col >= board_[0].size()) {
    if (printErrors) {
      std::cout << "Invalid position: (" << row << ", " << col << ")\n";
    }
    return false;
  }
  if (board_[row][col] == Stone::kEmpty) {
    //std::cout << "empty: (" << row << ", " << col << ")\n";
    if ((abs(row - prev_row_) == 2 && col == prev_col_) || (abs(col - prev_col_) == 2 && row == prev_row_)) {
      // Calculate the position of the stone that should be jumped over
      int middle_row = (row + prev_row_) / 2;
      int middle_col = (col + prev_col_) / 2;
      // Check if the stone at the middle position is an opponent's stone
      Game::Stone currentStone = (current_player_ == player1_) ? Stone::kBlack : Stone::kWhite;
      Game::Stone oppositeStone = (currentStone == Stone::kBlack) ? Stone::kWhite : Stone::kBlack;
      
      if (board_[middle_row][middle_col] == oppositeStone) {
        return true;
      } else {
        if (printErrors) {
          std::cout << "There is no opponent's stone to jump over. Try again.\n";
        }
        return false;
      }
    } else { 
      if (printErrors) {
        std::cout << "Although this is an empty spot, you did not jump two spaces away. Try again.\n";
      }
      return false;
    }
  } else {
    if (printErrors) {
      std::cout << "The destination is not empty. Try again.\n";
    }
    return false;
  }
  return false;
}

/**
 * Checks if the current stone can make multiple jumps.
 * @param row The row of the current stone.
 * @param col The column of the current stone.
 * @return bool True if the stone can make multiple jumps, false otherwise.
 */
bool Game::CanMakeMultipleJumps(int row, int col) { //current location
    // Direction vectors for vertical and horizontal directions
    prev_row_ = row;
    prev_col_ = col;
    std::pair<int, int> directions[] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    // Check all directions
    for (const auto& dir : directions) {
        int new_row = row + dir.first;
        int new_col = col + dir.second;

        // If the jump is valid
        if (IsValidJump(new_row, new_col, false)) {
            return true;
        }
    }

    // If no valid jumps were found
    return false;
}

/**
 * If two human player game, it switches the current player.
 * If one human player and one Koko the Computer player game, it switches the current player and makes a move for Koko.
 * If Koko has no valid moves, you win!
 */
void Game::SwitchPlayer() {
  current_player_ = (current_player_ == player1_) ? player2_ : player1_;
  if (current_player_ == "Koko the Computer") {
    ComputerPlayer cp;
    cp.SetStone(Stone::kWhite);
    auto move = cp.GetRandomMove(*this);
    if (move.first.first == -1) {
      computer_game_over_ = true;
      return;
    }
    std::cout << "\nKoko the Computer made a move from [" << GetColumnLabel(move.first.second) << std::to_string(num_rows_ - move.first.first) << "] to [" << GetColumnLabel(move.second.second) << std::to_string(num_rows_ - move.second.first) << "].\n\n";
    cp.MakeMove(*this, move);
    current_player_ = (current_player_ == player1_) ? player2_ : player1_;
  }
}

/**
 * Checks if the game is over.
 * @return bool True if the game is over, false otherwise.
 */
bool Game::IsGameOver() {
  if (computer_game_over_) {
    winner_ = player1_;
    return true;
  }

  // Determine the stone for the current player
  Game::Stone currentStone = (current_player_ == player1_) ? Stone::kBlack : Stone::kWhite;

  // If the current player has no valid moves, the game is over
  if (!HasValidMoves(currentStone)) {
    // Set the winner to the other player
    winner_ = (current_player_ == player1_) ? player2_ : player1_;
    return true;
  }
  // Otherwise, the game is not over
  return false;
}

/**
 * Checks if the current stone has any valid moves.
 * @param currentStone The stone to check for valid moves.
 * @return bool True if the stone has valid moves, false otherwise.
 */
bool Game::HasValidMoves(Game::Stone currentStone) {
  for (int row = 0; row < num_rows_; ++row) {
    for (int col = 0; col < num_cols_; ++col) {
      if (board_[row][col] == currentStone) {
        // Check all possible jumps for the current stone
        if (CanMakeMultipleJumps(row, col)) {
              return true;
        }
      }
    }
  }
  return false;
}

/**
 * Returns the winner of the game.
 * @return std::string The winner of the game.
 */
std::string Game::GetWinner() const {
  if (winner_ == "Koko the Computer") {
    return "Koko";
  }
  return winner_;
}

/**
 * Prints an awesome congratulations message to the players!
 */
void Game::Congratulations() {
  std::cout << R"(
=============================================================
                     Game over!
=============================================================
               THE KONANE GAME WINNER IS...
=============================================================
                  _______________
                 |@@@@|     |####|
                 |@@@@|     |####|
                 |@@@@|     |####|
                 \@@@@|     |####/
                  \@@@|     |###/
                   `@@|_____|##'
                        (O)
                     .-'''''-.
                   .'  * * *  `.
                  :  *       *  :
                 : ~             :)" << "\n                 :      " 
                << GetWinner() <<
             R"(     :
                 : ~           ~ :
                  :  *       *  :
                   `.  * * *  .'
                     `-.....-'
=============================================================
                   CONGRATULATIONS!
=============================================================
            You were both amazing players!
=============================================================
          Would you like to play again? (Y/N): 
)";
}

/**
 * @return int The number of rows of the board.
 */
int Game::GetNumRows() const {
    return num_rows_;
}

/**
 * @return int The number of columns of the board.
 */
int Game::GetNumCols() const {
    return num_cols_;
}

/**
 * @return The game board
 */
const std::vector<std::vector<Game::Stone>>& Game::GetBoard() const {
    return board_;
}

/**
 *  Sets the stone at the given row and column.
 */
void Game::SetStone(int row, int col, Stone stone) {
    board_[row][col] = stone;
}

/**
 *  Draws the board.
 */
void Game::DrawBoard() const {
  int max_digits = std::to_string(num_rows_).length();
  int row_number = num_rows_;  // Start from the bottom of the board

  // Print column letters
  std::cout << std::string(max_digits + 2, ' ');  // Extra space for row numbers
  for (int j = 0; j < num_cols_; ++j) {
    std::cout << "  " << GetColumnLabel(j) << " ";
  }
  std::cout << '\n';

  // Print row of underscores
  std::cout << std::string(max_digits + 2, ' ');  // Extra space for row numbers
  for (int j = 0; j < num_cols_; ++j) {
    std::cout << "____";
  }
  std::cout << "_\n";

  // Print rows
  for (const auto& row : board_) {
    // Print row number with adjusted spacing
    std::cout << std::setw(max_digits) << std::right << row_number-- << " |";  // Decrement row_number

    // Print row contents
    for (const auto& cell : row) {
      std::cout << "|";  // Print vertical line before each cell
      switch (cell) {
        case Stone::kEmpty:
          std::cout << " . ";
          break;
        case Stone::kBlack:
          std::cout << "\033[1;37m ● \033[0m";  // Black color
          break;
        case Stone::kWhite:
          std::cout << "\033[1;30m ○ \033[0m";  // White color
          break;
      }
    }
    std::cout << "|\n";  // Print vertical line after each row

    // Print horizontal line after each row
    std::cout << std::string(max_digits + 2, ' ') << "+";  // Extra space for row numbers
    for (int j = 0; j < num_cols_; ++j) {
      std::cout << "---+";
    }
    std::cout << "\n";
  }
}

// Private methods

/**
 *  Initializes the board.
 */
void Game::InitializeBoard() {
  // Create an empty board
  board_ = std::vector<std::vector<Stone>>(num_rows_, std::vector<Stone>(num_cols_, Stone::kEmpty));

  // Fill the board with alternating black and white stones
  for (int row = 0; row < num_rows_; ++row) {
    for (int col = 0; col < num_cols_; ++col) {
      if ((row + col) % 2 == 0) {
        board_[row][col] = Stone::kBlack;
      } else {
        board_[row][col] = Stone::kWhite;
      }
    }
  }

  // Randomly remove one black stone and one white stone from the board
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, num_rows_ - 1);

  // Remove one black stone
  int black_row, black_col;
  while (true) {
    black_row = dis(gen);
    black_col = dis(gen);
    if (board_[black_row][black_col] == Stone::kBlack) {
      board_[black_row][black_col] = Stone::kEmpty;
      std::string currentPosition = GetColumnLabel(black_col) + std::to_string(num_rows_ - black_row);
      std::cout << "\nKonane removed a black stone at [" << currentPosition << "] ";
      break;
    }
  }

    // Collect all white stones
  std::vector<std::pair<int, int>> white_stones;
  for (int i = 0; i < board_.size(); ++i) {
      for (int j = 0; j < board_[i].size(); ++j) {
          if (board_[i][j] == Stone::kWhite) {
              white_stones.push_back({i, j});
          }
      }
  }

  // Shuffle the vector
  std::shuffle(white_stones.begin(), white_stones.end(), gen);

  // Try to find a white stone that is not adjacent or diagonal to the black stone
  // syntax valid for C++17
  for (const auto& [row, col] : white_stones) {
      if (abs(row - black_row) > 1 || abs(col - black_col) > 1) {
          board_[row][col] = Stone::kEmpty;
          std::string currentPosition = GetColumnLabel(col) + std::to_string(num_rows_ - row);
          std::cout << "and removed a white stone at [" << currentPosition << "]!\n\n";
          return;
      }
  }

  // If no such stone is found, remove the first white stone
  auto [row, col] = white_stones[0];
  board_[row][col] = Stone::kEmpty;
  std::string currentPosition = GetColumnLabel(col) + std::to_string(num_rows_ - row);
  std::cout << "and removed a white stone at [" << currentPosition << "]!\n";
}

/**
 *  Returns the column label for the given column index.
 * @param col The column index.
 * @return std::string The column label.
 */
std::string Game::GetColumnLabel(int col) const {
  if (col < 26) {
    // Single letter labels for columns A-Z
    return std::string(1, 'A' + col);
  } else {
    // Two-letter labels for columns AA-ZZ
    char first_letter = 'A' + (col / 26) - 1;
    char second_letter = 'A' + (col % 26);
    return std::string(1, first_letter) + std::string(1, second_letter);
  }
}






