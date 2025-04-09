
#pragma once
#include "ChessPiece.h"

class Rook : public ChessPiece {

public:

    Rook(bool isWhite, char pieceType, const std::pair<int, int>& pos);
    
    // Delete copy constructor and assignment
    Rook(const Rook&) = delete;
    Rook& operator=(const Rook&) = delete;
    
    // Add move constructor and assignment
    Rook(Rook&&) noexcept = default;
    Rook& operator=(Rook&&) noexcept = default;
   
    int checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const override;
};