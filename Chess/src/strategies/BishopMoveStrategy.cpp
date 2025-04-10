
#include "strategies/BishopMoveStrategy.h"
#include "board/ChessBoard.h"

//------------------------------------------------------------------------
/**
 * Checks if a bishop move is valid (diagonal and not blocked).
 *
 * @param board The current chess board.
 * @param from  Source position.
 * @param to    Destination position.
 * @return MOVE_SUCCESS if valid, MOVE_INVALID_OR_BLOCKED otherwise.
 */
int BishopMoveStrategy::checkMovement(const ChessBoard& board,
                                      const std::pair<int, int>& from,
                                      const std::pair<int, int>& to) const {

    int startRow = from.first;
    int startCol = from.second;
    int endRow = to.first;
    int endCol = to.second;

    // Check if the move is diagonal: difference between rows == difference between cols
    int rowDiff = abs(endRow - startRow);
    int colDiff = abs(endCol - startCol);

    if (rowDiff != colDiff) {
        return MOVE_INVALID_OR_BLOCKED;  // Bishops can only move diagonally
    }

    //Direction of movement
    int rowStep = (endRow > startRow) ? 1 : -1;
    int colStep = (endCol > startCol) ? 1 : -1;
    
    int row = startRow + rowStep;
    int col = startCol + colStep;

    //Check for clear path
    while (row != endRow) {
        
        if (board.isOccupied(row, col)) {
            return MOVE_INVALID_OR_BLOCKED;
        }
        row += rowStep;
        col += colStep;
    }

    // Path is clear - valid move
    return MOVE_SUCCESS;
}


