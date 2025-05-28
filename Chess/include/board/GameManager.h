
#pragma once
#include <string>
#include <memory>
#include "board/ChessBoard.h"
#include "GameConstants.h"
#include "move/MoveRecommender.h"

/*
Input Handling Note:
The Chess class provided handles most user input and cannot be modified.
However, it does not catch all invalid inputs (such as wrong format or out-of-bounds moves).
To ensure robust error handling, GameManager throws an InvalidPositionException 
for invalid move formats or positions.
All such exceptions are caught internally: an error message is printed to the user, 
and code 11 ("no piece at source") is returned as a fallback for any unexpected or malformed input.
This approach keeps the game running smoothly within the project’s constraints.
*/


class GameManager{

public:
    GameManager(const std::string& boardStr);

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int checkMovement(const std::string& move);

    void showRecommendations();
    void promptUserForRecommendationTurns();
   
    // Accessor methods for future (UI display...)
    //const std::vector<std::unique_ptr<ChessPiece>>& getCapturedWhitePieces() const;
    //const std::vector<std::unique_ptr<ChessPiece>>& getCapturedBlackPieces() const;

private:
    std::unique_ptr<ChessBoard> m_chessBoard;
    bool m_isWhiteTurn;
    
    std::vector<std::unique_ptr<ChessPiece>> m_capturedWhitePieces;
    std::vector<std::unique_ptr<ChessPiece>> m_capturedBlackPieces;
    std::unique_ptr<ChessPiece> m_lastCaptured;

    MoveRecommender m_recommender;
    int m_recommendationDepth;

   
    std::pair<int, int> convertPosition(const std::string& pos) const;
    std::pair<int, int> findKingPosition(bool isWhiteKing) const;
    void switchTurn();
    bool isCheck() const;
    int checkOpponentInCheck();
    void makeMove(const std::pair<int, int>& from, const std::pair<int, int>& to);
    void undoLastMove(const std::pair<int, int>& from, const std::pair<int, int>& to);
    void setRecommendationDepth(int turns);
};
