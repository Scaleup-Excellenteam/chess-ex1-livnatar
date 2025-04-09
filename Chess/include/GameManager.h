
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
