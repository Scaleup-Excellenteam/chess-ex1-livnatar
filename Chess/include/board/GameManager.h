
#pragma once
#include <string>
#include <memory>
#include "board/ChessBoard.h"
#include "GameConstants.h"


/*
Input Handling Note:
The Chess class provided handles user input and can't be modified.
However, it doesn't catch all invalid inputs (e.g., wrong format or out-of-bounds).
Since I can't add new response codes, I chose to return code 11 ("no piece at source")
as a fallback for any unexpected or malformed input. This keeps the game running
smoothly within the project’s limits.
*/


class GameManager {

public:
    GameManager(const std::string& boardStr);

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int checkMovement(const std::string& move);
   
    // Accessor methods for future (UI display...)
    //const std::vector<std::unique_ptr<ChessPiece>>& getCapturedWhitePieces() const;
    //const std::vector<std::unique_ptr<ChessPiece>>& getCapturedBlackPieces() const;

private:
    std::unique_ptr<ChessBoard> m_chessBoard;
    bool m_isWhiteTurn;
    
    std::vector<std::unique_ptr<ChessPiece>> m_capturedWhitePieces;
    std::vector<std::unique_ptr<ChessPiece>> m_capturedBlackPieces;
    std::unique_ptr<ChessPiece> m_lastCaptured;
   
    std::pair<int, int> convertPosition(const std::string& pos) const;
    std::pair<int, int> findKingPosition(bool isWhiteKing) const;
    void switchTurn();
    bool isCheck() const;
    int checkOpponentInCheck();
    void makeMove(const std::pair<int, int>& from, const std::pair<int, int>& to);
    void undoLastMove(const std::pair<int, int>& from, const std::pair<int, int>& to);
};
