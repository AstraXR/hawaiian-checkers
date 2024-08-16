/**
 * CISC 187 - Data Structures in C++ 
 * Professor Dave Parillo
 * @author Sophia Su
 * ID: 0005623258
 */ 

#include "computer_player.h"
#include "game.h"
#include <limits>

/**
 * Constructor for Koko the Computer
 */
ComputerPlayer::ComputerPlayer(): stone_(Game::Stone::kEmpty) {}

/**
 * Set the stone for Koko
 */
void ComputerPlayer::SetStone(Game::Stone stone) {
    stone_ = stone;
}

/**
 * Get a random move for Koko
 * @param game The game object
 * @return A pair of pairs representing the source and destination of the move
 */
std::pair<std::pair<int, int>, std::pair<int, int>> ComputerPlayer::GetRandomMove(Game& game) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> validMoves;
    
    // Generate all valid moves
    for (int sourceRow = 0; sourceRow < game.GetNumRows(); ++sourceRow) {
        for (int sourceCol = 0; sourceCol < game.GetNumCols(); ++sourceCol) {
            if (game.GetBoard().at(sourceRow).at(sourceCol) == stone_) {
                for (int destRow = 0; destRow < game.GetNumRows(); ++destRow) {
                    for (int destCol = 0; destCol < game.GetNumCols(); ++destCol) {
                        if (game.GetBoard().at(destRow).at(destCol) == Game::Stone::kEmpty && IsValidJump(game, sourceRow, sourceCol, destRow, destCol)) {
                            validMoves.push_back(std::make_pair(std::make_pair(sourceRow, sourceCol), std::make_pair(destRow, destCol)));
                        }
                    }
                }
            }
        }
    }
    // If there are no valid moves for Koko, return an invalid move to signal that the game is over! You won!
    if (validMoves.empty()) {
       return std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1));
    } else { // Select a move randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
        int randomIndex = dis(gen);
        return validMoves.at(randomIndex);
    }
}

/**
 * Check if Koko's jump is valid
 * @param game The game object
 * @param sourceRow The row of the source position
 * @param sourceCol The column of the source position
 * @param destRow The row of the destination position
 * @param destCol The column of the destination position
 * @return True if the jump is valid, false otherwise
 */
bool ComputerPlayer::IsValidJump(Game& game, int sourceRow, int sourceCol, int destRow, int destCol) {
   // Check if row and col are within the valid range of the board
  const std::vector<std::vector<Game::Stone>> board = game.GetBoard();
  if (sourceRow < 0 || sourceRow >= board.size() || sourceCol < 0 || sourceCol >= board[0].size()) {
    return false;
  }
  if (board.at(destRow).at(destCol) == Game::Stone::kEmpty) {
    if ((abs(destRow - sourceRow) == 2 && destCol == sourceCol) || (abs(destCol - sourceCol) == 2 && destRow == sourceRow)) {
      // Calculate the position of the stone that should be jumped over
      int middle_row = (destRow + sourceRow) / 2;
      int middle_col = (destCol + sourceCol) / 2;
      // Check if the stone at the middle position is an opponent's stone
      

      if (board.at(middle_row).at(middle_col) == Game::Stone::kBlack) {
        return true;
      } else {
          return false;
      }
    } else { 
        return false;
    }
  } else {
      return false;
  }
  return false;
}

/**
 * Make Koko's move
 * @param game The game object
 * @param move A pair of pairs representing the source and destination of the move
 */
void ComputerPlayer::MakeMove(Game& game, const std::pair<std::pair<int, int>, std::pair<int, int>>& move) {
    int sourceRow = move.first.first;
    int sourceCol = move.first.second;
    int destRow = move.second.first;
    int destCol = move.second.second;
    // Make the move
    if ((abs(destRow - sourceRow) == 2 && destCol == sourceCol)) {
        // remove stone in between the jump
        game.SetStone((destRow + sourceRow) / 2, destCol, Game::Stone::kEmpty);
    }
    else if ((abs(destCol - sourceCol) == 2 && destRow == sourceRow)) {
        // remove stone in between the jump
        game.SetStone(destRow, (destCol + sourceCol) / 2, Game::Stone::kEmpty);
    }
    // Remove the jumped stone if a jump occurred
    game.SetStone(sourceRow, sourceCol, Game::Stone::kEmpty);
    game.SetStone(destRow, destCol, stone_);
    game.DrawBoard();
}

