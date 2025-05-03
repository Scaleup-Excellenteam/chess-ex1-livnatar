
#include "strategies/BishopMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"

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

//------------------------------------------------------------------------
/**
 * Generates all valid diagonal moves for a bishop from the given position.
 * The bishop can move in all four diagonal directions until it is blocked
 * by another piece or the edge of the board.
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the bishop (row, column).
 * @param isWhite    Indicates whether the bishop is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'B' for Bishop).
 * @return A vector of valid moves including captures, excluding blocked paths.
 */
std::vector<Move> BishopMoveStrategy::generateMoves(const ChessBoard& board,
                                                    const std::pair<int, int>& from,
                                                    bool isWhite,
                                                    char pieceType) const {

    std::vector<Move> validMoves;
    int row = from.first;
    int col = from.second;

    // Directions: north-east, south-east, south-west, north-west
    const std::pair<int, int> directions[4] = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };

    // Generate moves in each of the four directions
    for (const auto& dir : directions) {
        int dRow = dir.first;
        int dCol = dir.second;

        for (int i = 1; i < 8; ++i) {
            int newRow = row + dRow * i;
            int newCol = col + dCol * i;

            // Check if we're still on the board
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
                break;
            }

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
                // Stop looking in this direction (blocked)
                break;
            }
        }
    }

    return validMoves;
}