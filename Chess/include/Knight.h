
#pragma once
#include "ChessPiece.h"

class Knight : public ChessPiece {

public:

    Knight(bool isWhite, char pieceType, const std::pair<int, int>& pos);

    // Delete copy constructor and assignment
    Knight(const Knight&) = delete;
    Knight& operator=(const Knight&) = delete;
   
    // Add move constructor and assignment
    Knight(Knight&&) noexcept = default;
    Knight& operator=(Knight&&) noexcept = default;

    int checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const override;
};