
#include "strategies/KingMoveStrategy.h"
#include "board/ChessBoard.h"

//------------------------------------------------------------------------
/**
 * Validates a king's move (1 square in any direction).
 *
 * @param board The current chess board.
 * @param from  Starting position.
 * @param to    Target position.
 * @return MOVE_SUCCESS if valid, MOVE_INVALID_OR_BLOCKED otherwise.
 */
int KingMoveStrategy::checkMovement(const ChessBoard& board,
                                    const std::pair<int, int>& from,
                                    const std::pair<int, int>& to) const {

    // Get current position
    int currRow = from.first;
    int currCol = from.second;

    int newRow = to.first;
    int newCol = to.second;

    int rowDiff = abs(newRow - currRow);
    int colDiff = abs(newCol - currCol);

    // Kings can only move one square in any direction
    if (rowDiff > 1 || colDiff > 1) {

       /* Note: Castling implementation would be checked here
        To implement castling, we would need:
        1. Check if king has moved before (requires move tracking)
        2. Check if rook has moved before (requires move tracking)
        3. Check if path between king and rook is clear
        4. Check if king is in check or would pass through check
        5. Handle the king moving two squares and the rook jumping over
       
        For example:
        if (isCastlingMove(board, from, to)) {
            if (canCastle(board, from, to)) {
                // Would need to move the rook as well in *Chess* class
                return MOVE_CASTLING; // New constant code response
            }
        }
       */

        return MOVE_INVALID_OR_BLOCKED;
    }

    // If we're not moving at all (same position), that's invalid
    if (rowDiff == 0 && colDiff == 0) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    return MOVE_SUCCESS;
}