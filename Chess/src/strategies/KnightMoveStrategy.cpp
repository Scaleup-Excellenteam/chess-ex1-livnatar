
#include "strategies/KnightMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"

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

//------------------------------------------------------------------------
/**
 * Generates all valid L-shaped moves for a knight from the given position.
 * The knight moves in an 'L' shape: two squares in one direction and then one square perpendicular.
 * It can jump over other pieces and capture enemy pieces at the destination.
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the knight (row, column).
 * @param isWhite    Indicates whether the knight is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'N' for Knight).
 * @return A vector of valid moves including captures; excludes out-of-bounds positions.
 */
std::vector<Move> KnightMoveStrategy::generateMoves(const ChessBoard& board,
                                                    const std::pair<int, int>& from,
                                                    bool isWhite,
                                                    char pieceType) const {

    std::vector<Move> validMoves;
    int row = from.first;
    int col = from.second;

    // All possible knight moves (L-shape)
    const std::pair<int, int> knightMoves[8] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    // Generate moves in each of the 8 directions
    for (const auto& move : knightMoves) {
        
        int newRow = row + move.first;
        int newCol = col + move.second ;

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

    return validMoves;
}

