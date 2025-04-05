
#include "Rook.h"
#include "ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
Rook::Rook(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------

int Rook::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
    
    // Check if the move is horizontal or vertical - a valid rook move must keep either the row or column the same
    bool isHorizontalMove = (m_position.first == newPos.first);
    bool isVerticalMove = (m_position.second == newPos.second);

    if (!isHorizontalMove && !isVerticalMove) {
        return MOVE_INVALID_OR_BLOCKED; // Rooks can only move horizontally or vertically
    }

    // Determine direction of movement
    int startRow = m_position.first;
    int startCol = m_position.second;
    int endRow = newPos.first;
    int endCol = newPos.second;

    // Check for clear path 
    if (isHorizontalMove) {
        
        // Moving along a row 
        int step = (endCol > startCol) ? 1 : -1;
        for (int col = startCol + step; col != endCol; col += step) {
            if (board.isOccupied(startRow, col)) {
                return MOVE_INVALID_OR_BLOCKED;
            }
        }
    }
    else {
        
        // Moving along a column 
        int step = (endRow > startRow) ? 1 : -1;
        for (int row = startRow + step; row != endRow; row += step) {
            if (board.isOccupied(row, startCol)) {
                return MOVE_INVALID_OR_BLOCKED;
            }
        }
    }

    // Path is clear - movement is valid
    return MOVE_SUCCESS;
}
