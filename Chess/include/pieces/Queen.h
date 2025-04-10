
#pragma once
#include "ChessPiece.h"

class Queen : public ChessPiece {

public:

    Queen(bool isWhite, char pieceType, const std::pair<int, int>& pos);

    // Delete copy constructor and assignment
    Queen(const Queen&) = delete;
    Queen& operator=(const Queen&) = delete;
    
    // Add move constructor and assignment
    Queen(Queen&&) noexcept = default;
    Queen& operator=(Queen&&) noexcept = default;

    int checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const override;

private:
   
    // Helper methods to check different movement types
    bool isPathClearHorizontal(const ChessBoard& board, int startRow, int startCol, int endCol) const;
    bool isPathClearVertical(const ChessBoard& board, int startCol, int startRow, int endRow) const;
    bool isPathClearDiagonal(const ChessBoard& board, int startRow, int startCol, int endRow, int endCol) const;
};