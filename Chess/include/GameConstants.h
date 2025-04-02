
#pragma once

// Status codes for move validation
constexpr int MOVE_SUCCESS = 42;
constexpr int INVALID_MOVE = -1;
constexpr int NO_PIECE = -2;

// Additional status codes can be added here later

// Enum for piece types (if needed in the future)
enum class PieceType {
    NONE,  // Empty square
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
