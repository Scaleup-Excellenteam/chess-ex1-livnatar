
#pragma once
#include "ChessPiece.h"

class Bishop : public ChessPiece {

public:

    Bishop(bool isWhite, char pieceType, const std::pair<int, int>& pos);

    // Delete copy constructor and assignment
    Bishop(const Bishop&) = delete;
    Bishop& operator=(const Bishop&) = delete;
    
    // Add move constructor and assignment
    Bishop(Bishop&&) noexcept = default;
    Bishop& operator=(Bishop&&) noexcept = default;
};