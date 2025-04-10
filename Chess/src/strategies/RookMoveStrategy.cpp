
#include "strategies/RookMoveStrategy.h"
#include "board/ChessBoard.h"


//------------------------------------------------------------------------
/**
 * Validates rook's movement (horizontal or vertical).
 *
 * @param board The chess board.
 * @param from  Starting position.
 * @param to    Target position.
 * @return MOVE_SUCCESS if valid, MOVE_INVALID_OR_BLOCKED otherwise.
 */
int RookMoveStrategy::checkMovement(const ChessBoard& board, 
	                                const std::pair<int, int>& from, 
	                                const std::pair<int, int>& to) const {

    // Check if the move is horizontal or vertical
    bool isHorizontalMove = (from.first == to.first);
    bool isVerticalMove = (from.second == to.second);

    if (!isHorizontalMove && !isVerticalMove) {
        return MOVE_INVALID_OR_BLOCKED; // Rooks can only move horizontally or vertically
    }

    // Determine direction of movement
    int startRow = from.first;
    int startCol = from.second;
    int endRow = to.first;
    int endCol = to.second;

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


