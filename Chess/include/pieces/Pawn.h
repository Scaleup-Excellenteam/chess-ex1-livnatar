
#pragma once
#include "ChessPiece.h"

class Pawn : public ChessPiece {

public:

    Pawn(bool isWhite, char pieceType, const std::pair<int, int>& pos);

    // Delete copy constructor and assignment
    Pawn(const Pawn&) = delete;
    Pawn& operator=(const Pawn&) = delete;

    // Add move constructor and assignment
    Pawn(Pawn&&) noexcept = default;
    Pawn& operator=(Pawn&&) noexcept = default;
};