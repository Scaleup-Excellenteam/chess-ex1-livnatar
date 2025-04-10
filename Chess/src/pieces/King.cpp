

#include "pieces/King.h"
#include "board/ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
King::King(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------

int King::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
   
    // Get current position
    int currRow = m_position.first;
    int currCol = m_position.second;
    
    int newRow = newPos.first;
    int newCol = newPos.second;

    int rowDiff = abs(newRow - currRow);
    int colDiff = abs(newCol - currCol);

    // Kings can only move one square in any direction
    // This means the maximum difference in both row and column should be 1
    if (rowDiff > 1 || colDiff > 1) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    // If we're not moving at all (same position), that's invalid
    if (rowDiff == 0 && colDiff == 0) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    // Castling would be handled here


    return MOVE_SUCCESS;
}
