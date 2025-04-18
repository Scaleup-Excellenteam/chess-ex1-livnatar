
#pragma once

// Status codes for move validation

// Move cannot be performed - each response code represents a different reason
const int MOVE_NO_PIECE_IN_SOURCE = 11;  // No piece in the source square
const int MOVE_OPPONENT_PIECE_IN_SOURCE = 12;  // Opponent's piece in the source square
const int MOVE_FRIENDLY_PIECE_IN_DEST = 13;  // Destination square contains a friendly piece
const int MOVE_INVALID_OR_BLOCKED = 21;  // Invalid move or blocked path (e.g., a rook moving diagonally)
const int MOVE_CAUSES_CHECK = 31;  // Move results in a check for the moving player

// Move can be performed
const int MOVE_SUCCESS = 42;  // Valid move: piece moves from source to destination, and turn changes
const int MOVE_SUCCESS_CHECK = 41;  // Valid move that results in a check: same as MOVE_SUCCESS, but also displays a check message

