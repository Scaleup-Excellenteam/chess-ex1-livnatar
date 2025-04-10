
#include "strategies/KnightMoveStrategy.h"
#include "board/ChessBoard.h"

//------------------------------------------------------------------------
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


