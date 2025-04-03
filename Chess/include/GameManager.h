
#pragma once
#include "ChessBoard.h"
#include "GameConstants.h"
#include <string>

class GameManager {
public:
    GameManager(const std::string& boardStr);

    // Processes a move in chess notation (e.g., "a7c6") and returns status code
    int checkMovement(const std::string& move);

private:
    ChessBoard m_chessBoard;

    // Converts chess notation (e.g., "a7") to board indices (row, col)
    std::pair<int, int> convertPosition(const std::string& pos) const;
};



