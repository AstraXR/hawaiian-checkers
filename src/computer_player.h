#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "game.h"
#include <string>

/**
 * @file computer_player.h
 * @brief This file contains the declaration of the ComputerPlayer class, which is responsible for Koko the Computer's moves in the Konane game.
 */
/**
 * @class ComputerPlayer
 * @brief This class represents Koko the Computer in the Konane game.
 */
class ComputerPlayer {
public:

    /**
     * @brief Constructor for Koko the Computer
     */
    ComputerPlayer();

    /**
     * @brief Set the stone for Koko
     * @param stone The stone for Koko
     */
    void SetStone(Game::Stone stone);

    /**
     * @brief Get a random move for Koko
     * @param game The game object
     * @return A pair of pairs representing the source and destination of the move
     */
    std::pair<std::pair<int, int>, std::pair<int, int>> GetRandomMove(Game& game);

    /**
     * @brief Check if Koko's jump is valid
     * @param game The game object
     * @param sourceRow The row of the source position
     * @param sourceCol The column of the source position
     * @param destRow The row of the destination position
     * @param destCol The column of the destination position
     * @return True if the jump is valid, false otherwise
     */
    bool IsValidJump(Game& game, int sourceRow, int sourceCol, int destRow, int destCol);

    /**
     * @brief Make a move for Koko
     * @param game The game object
     * @param move A pair of pairs representing the source and destination of the move
     */
    void MakeMove(Game& game, const std::pair<std::pair<int, int>, std::pair<int, int>>& move);
    
    /**
     * @brief Get the stone for Koko
     */
    Game::Stone stone_;
};

#endif // COMPUTER_PLAYER_H