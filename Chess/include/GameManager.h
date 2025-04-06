
#pragma once
#include <string>
#include <memory>
#include "ChessBoard.h"
#include "GameConstants.h"

class GameManager {

public:
    GameManager(const std::string& boardStr);

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    // Processes a move in chess notation (e.g., "a7c6") and returns status code
    int checkMovement(const std::string& move);

private:
    std::unique_ptr<ChessBoard> m_chessBoard;
    bool m_isWhiteTurn; // true = White's turn, false = Black's turn
    
    // TODO (future): Add vectors to store captured pieces 
    // for use in UI display, undo functionality, or game history tracking

   
    std::pair<int, int> convertPosition(const std::string& pos) const;
    std::pair<int, int> findKingPosition(bool isWhiteKing) const;
    void switchTurn();
    bool isCheck() const;
    int checkOpponentInCheck();
};
