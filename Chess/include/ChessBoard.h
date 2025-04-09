
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ChessPiece.h"
#include "GameConstants.h" 

class ChessBoard {
public:

    ChessBoard(const std::string& boardStr);

    // Delete copy constructor and assignment
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator=(const ChessBoard&) = delete;
    
    // Add move constructor and assignment
    ChessBoard(ChessBoard&& other) noexcept = default;
    ChessBoard& operator=(ChessBoard&& other) noexcept = default;
   
    ~ChessBoard() = default;


    bool isOccupied(int row, int col) const;
    const ChessPiece* getPieceAt(int row, int col) const;
    int checkMovement(const std::pair<int, int>& from, const std::pair<int, int>& to, bool isWhiteTurn) const;
    void movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to);
    void printBoard() const;

private:
    std::vector<std::vector<std::unique_ptr<ChessPiece>>> m_board;
    void setupBoard(const std::string& boardStr);
};
