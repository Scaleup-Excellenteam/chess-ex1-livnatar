
#include "strategies/KnightMoveStrategy.h"
#include "board/ChessBoard.h"

//------------------------------------------------------------------------
/**
 * Validates a knight's move (L-shape: 2 by 1).
 *
 * @param board The current chess board.
 * @param from  Starting position.
 * @param to    Target position.
 * @return MOVE_SUCCESS if valid, MOVE_INVALID_OR_BLOCKED otherwise.
 */
int KnightMoveStrategy::checkMovement(const ChessBoard& board,
                                      const std::pair<int, int>& from,
                                      const std::pair<int, int>& to) const {

    // Get current position
    int startRow = from.first;
    int startCol = from.second;
    int endRow = to.first;
    int endCol = to.second;

    int rowDiff = abs(endRow - startRow);
    int colDiff = abs(endCol - startCol);

    // Knight moves in an L-shape: 2 squares in one direction and 1 in the other
    bool isValidKnightMove = (rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1);

    if (!isValidKnightMove) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    return MOVE_SUCCESS;

}


