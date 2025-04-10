
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
};