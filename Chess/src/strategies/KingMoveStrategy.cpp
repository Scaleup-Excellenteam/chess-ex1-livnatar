
#include "strategies/KingMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"

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
//------------------------------------------------------------------------
/**
 * Generates all valid one-square moves for a king from the given position.
 * The king can move one square in any direction, and may capture enemy pieces.
 * Castling logic is not implemented in this function.
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the king (row, column).
 * @param isWhite    Indicates whether the king is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'K' for King).
 * @return A vector of valid moves including captures; excludes illegal positions and castling.
 */
std::vector<Move> KingMoveStrategy::generateMoves(const ChessBoard& board,
                                                  const std::pair<int, int>& from,
                                                  bool isWhite,
                                                  char pieceType) const {

    std::vector<Move> validMoves;
    int row = from.first;
    int col = from.second;

    // All 8 possible directions the king can move
    const std::pair<int, int> directions[8] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };

    // Generate moves in each of the 8 directions
    for (const auto& dir : directions) {

        int newRow = row + dir.first;
        int newCol = col + dir.second;

        // Check if the new position is on the board
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {

            std::pair<int, int> to = { newRow, newCol };

            if (!board.isOccupied(newRow, newCol)) {
                // Empty square, add as a valid move
                validMoves.emplace_back(from, to, 0, pieceType);
            }
            else {
                // Occupied square
                const ChessPiece* piece = board.getPieceAt(newRow, newCol);
                if (piece && piece->getColor() != isWhite) {
                    // Enemy piece, can capture
                    validMoves.emplace_back(from, to, 0, pieceType);
                }
            }
        }
    }

    /*
     TODO: Castling implementation would go here
     Check if king hasn't moved and if relevant rook hasn't moved
     Check if squares between king and rook are empty
     Check if king is not in check and doesn't pass through check
    */

    return validMoves;
}