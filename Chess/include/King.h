
#pragma once
#include "ChessPiece.h"

class King : public ChessPiece {

public:

    King(bool isWhite, char pieceType, const std::pair<int, int>& pos);

    // Delete copy constructor and assignment
    King(const King&) = delete;
    King& operator=(const King&) = delete;
   
    // Add move constructor and assignment
    King(King&&) noexcept = default;
    King& operator=(King&&) noexcept = default;

    int checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const override;
};