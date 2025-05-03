
#include "strategies/RookMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"

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
//------------------------------------------------------------------------
/**
 * Generates all valid horizontal and vertical moves for a rook from the given position.
 * The rook moves in straight lines along rows and columns and can capture enemy pieces in its path.
 * The move generation stops when the rook is blocked by a piece (either ally or enemy).
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the rook (row, column).
 * @param isWhite    Indicates whether the rook is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'R' for Rook).
 * @return A vector of valid moves including captures; excludes blocked paths and out-of-bounds positions.
 */
std::vector<Move> RookMoveStrategy::generateMoves(const ChessBoard& board,
                                                  const std::pair<int, int>& from,
                                                  bool isWhite,
                                                  char pieceType) const{
    
    std::vector<Move> validMoves;
    int row = from.first;
    int col = from.second;

    // Directions: up, right, down, left
    const std::pair<int, int> directions[4] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

    // Generate moves in each of the four directions
    for (const auto& dir : directions) {
        int dRow = dir.first;
        int dCol = dir.second;

        for (int i = 1; i < 8; ++i) {
            
            int newRow = row + dRow * i;
            int newCol = col + dCol * i;

            // Check if new position is on the board
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


