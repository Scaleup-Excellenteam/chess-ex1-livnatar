#include "strategies/PawnMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"
#include <cstdlib> 


//------------------------------------------------------------------------
/**
 * Checks if a pawn move is valid.
 * Supports forward movement, diagonal captures, and placeholders for promotion and en passant.
 *
 * @param board The chess board.
 * @param from  Current position of the pawn.
 * @param to    Target position to move to.
 * @return MOVE_SUCCESS if the move is valid, otherwise MOVE_INVALID_OR_BLOCKED.
 */
int PawnMoveStrategy::checkMovement(const ChessBoard& board,
                                    const std::pair<int, int>& from,
                                    const std::pair<int, int>& to) const {
    
    const ChessPiece* piece = board.getPieceAt(from.first, from.second);
    if (!piece) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    bool isWhite = piece->getColor();
    int colDiff = std::abs(to.second - from.second);

    // Forward movement with no capture
    if (colDiff == 0) {
        if (canMoveForward(board, isWhite, from, to) || canMoveTwoSquares(board, isWhite, from, to)) {

            /*
             Note: Pawn *promotion* detection could be implemented here, but since the Chess class
             cannot be modified, the promoted piece would not be visually represented on the board.
             -> A full implementation would require changes to the Chess class to update the displayed piece.
             if ((isWhite && to.first == 7) || (!isWhite && to.first == 0)) {
                 // Promotion logic here
            }
            */

            return MOVE_SUCCESS;
        }
    }
    // Diagonal capture
    else if (colDiff == 1) {
        if (canCaptureDiagonally(board, isWhite, from, to)) {

            /*
             Note: Similar promotion check would be needed here for diagonal captures
             if ((isWhite && to.first == 7) || (!isWhite && to.first == 0)) {
                 // Promotion logic would go here
             }
            */

            return MOVE_SUCCESS;
        }

        /*
         Note: *En passant* capture could be implemented here, and would require tracking the last move
         in the ChessBoard class and updating the Chess display(cannot modified) to show the captured pawn is removed.
         if (canCaptureEnPassant(board, isWhite, from, to)) {
             return MOVE_SUCCESS;
         }
        */
    }

    return MOVE_INVALID_OR_BLOCKED;
}

//------------------------------------------------------------------------
/**
 * Checks if a pawn can move forward one square.
 *
 * @param board      The chess board.
 * @param isWhite    True if the pawn is white.
 * @param from       Current position.
 * @param to         Target position.
 * @return true if move is valid.
 */
bool PawnMoveStrategy::canMoveForward(const ChessBoard& board,
                                      bool isWhite,
                                      const std::pair<int, int>& from,
                                      const std::pair<int, int>& to) const {
    
    int forwardDirection = isWhite ? 1 : -1;
    int rowDiff = to.first - from.first;

    // One square forward
    if (rowDiff == forwardDirection && 
        to.second == from.second && 
        !board.isOccupied(to.first, to.second)) {
       
        return true;
    }
    return false;
}

//------------------------------------------------------------------------
/**
 * Checks if a pawn can move forward two squares from its initial position.
 *
 * @param board      The chess board.
 * @param isWhite    True if the pawn is white.
 * @param from       Current position.
 * @param to         Target position.
 * @return true if two-square move is valid.
 */
bool PawnMoveStrategy::canMoveTwoSquares(const ChessBoard& board,
                                         bool isWhite,
                                         const std::pair<int, int>& from,
                                         const std::pair<int, int>& to) const {
    
    int forwardDirection = isWhite ? 1 : -1;
    int rowDiff = to.first - from.first;
  
    bool inStartPosition = (isWhite && from.first == 1) || (!isWhite && from.first == 6);

    // Two squares from starting position
    if (rowDiff == 2 * forwardDirection &&
        to.second == from.second &&
        inStartPosition) {
        
        int midRow = from.first + forwardDirection;
        return (!board.isOccupied(midRow, from.second) &&
                !board.isOccupied(to.first, to.second)     );
    }
    return false;
}

//------------------------------------------------------------------------
/**
 * Checks if a pawn can capture a piece diagonally.
 *
 * @param board      The chess board.
 * @param isWhite    True if the pawn is white.
 * @param from       Current position.
 * @param to         Target capture position.
 * @return true if diagonal capture is allowed.
 */
bool PawnMoveStrategy::canCaptureDiagonally(const ChessBoard& board,
                                            bool isWhite,
                                            const std::pair<int, int>& from,
                                            const std::pair<int, int>& to) const {
   
    int forwardDirection = isWhite ? 1 : -1;
    int rowDiff = to.first - from.first;

    if (rowDiff == forwardDirection &&
        std::abs(to.second - from.second) == 1 &&
        board.isOccupied(to.first, to.second)) {

        const ChessPiece* targetPiece = board.getPieceAt(to.first, to.second);
        return (targetPiece && targetPiece->getColor() != isWhite);
    }
    return false;
}
//------------------------------------------------------------------------
/**
 * Generates all valid moves for a pawn from the given position.
 * Handles forward movement (1 or 2 squares from the starting rank) and diagonal captures.
 * En passant and promotion are noted but not implemented in this function.
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the pawn (row, column).
 * @param isWhite    Indicates whether the pawn is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'P' for Pawn).
 * @return A vector of valid moves including captures and double-step advances from the initial position.
 */
std::vector<Move> PawnMoveStrategy::generateMoves(const ChessBoard& board,
                                                  const std::pair<int, int>& from,
                                                  bool isWhite,
                                                  char pieceType) const {
   
    std::vector<Move> validMoves;

    // Validate starting position
    if (!isValidPosition(from.first, from.second)) {
        std::cerr << "Invalid pawn position: (" << from.first << "," << from.second << ")" << std::endl;
        return validMoves;
    }

    int row = from.first;
    int col = from.second;
    int direction = isWhite ? 1 : -1;

    // Add forward moves
    addForwardMoves(board, validMoves, from, row, col, direction, isWhite, pieceType);

    // Add capture moves
    addCaptureMoves(board, validMoves, from, row, col, direction, isWhite, pieceType);

    // TODO: Implement promotion and en passant

    return validMoves;
}
//------------------------------------------------------------------------
/**
 * Adds all valid forward moves for a pawn from its current position.
 * A pawn can move one square forward if the square is empty. If on its starting rank,
 * and the square two steps ahead is also empty, it can move two squares forward.
 *
 * @param board      The current state of the chess board.
 * @param validMoves The vector to which valid forward moves will be added.
 * @param from       The current position of the pawn (row, column).
 * @param row        The row index of the current pawn position.
 * @param col        The column index of the current pawn position.
 * @param direction  The direction of movement (1 for white, -1 for black).
 * @param isWhite    Indicates whether the pawn is white.
 * @param pieceType  The character representing the pawn piece type (e.g., 'P').
 */
void PawnMoveStrategy::addForwardMoves(const ChessBoard& board,
                                       std::vector<Move>& validMoves,
                                       const std::pair<int, int>& from,
                                       int row, int col, int direction,
                                       bool isWhite, char pieceType) const {
    // Single step forward
    int newRow = row + direction;
    if (!isValidPosition(newRow, col)) {
        return;
    }

    if (!board.isOccupied(newRow, col)) {
        validMoves.emplace_back(from, std::make_pair(newRow, col), 0, pieceType);

        // Two squares forward from starting position
        bool onStartingRank = (isWhite && row == 1) || (!isWhite && row == 6);
        int twoStepRow = row + (2 * direction);

        if (onStartingRank && isValidPosition(twoStepRow, col) && !board.isOccupied(twoStepRow, col)) {
            validMoves.emplace_back(from, std::make_pair(twoStepRow, col), 0, pieceType);
        }
    }
}
//------------------------------------------------------------------------
/**
 * Adds all valid capture moves for a pawn from its current position.
 * Pawns can capture one square diagonally forward (left or right) if an opponent's piece is present.
 * This function also provides a placeholder for en passant capture logic.
 *
 * @param board      The current state of the chess board.
 * @param validMoves The vector to which valid capture moves will be added.
 * @param from       The current position of the pawn (row, column).
 * @param row        The row index of the current pawn position.
 * @param col        The column index of the current pawn position.
 * @param direction  The direction of movement (1 for white, -1 for black).
 * @param isWhite    Indicates whether the pawn is white.
 * @param pieceType  The character representing the pawn piece type (e.g., 'P').
 */
void PawnMoveStrategy::addCaptureMoves(const ChessBoard& board,
                                       std::vector<Move>& validMoves,
                                       const std::pair<int, int>& from,
                                       int row, int col, int direction,
                                       bool isWhite, char pieceType) const {
    
    int newRow = row + direction;
    if (!isValidPosition(newRow, col)) {
        return;
    }

    for (int colOffset : {-1, 1}) {
        int newCol = col + colOffset;
        if (!isValidPosition(newRow, newCol)) {
            continue;
        }

        try {
            if (board.isOccupied(newRow, newCol)) {
                const ChessPiece* piece = board.getPieceAt(newRow, newCol);
                if (piece && piece->getColor() != isWhite) {
                    validMoves.emplace_back(from, std::make_pair(newRow, newCol), 0, pieceType);
                }
            }
            // TODO: En passant implementation would go here
        }
        catch (const std::exception& e) {
            std::cerr << "Error checking capture position: " << e.what() << std::endl;
        }
    }
}
//------------------------------------------------------------------------
/*
Note: Implementations for future EnPassant and Promotion enhancements

bool PawnMoveStrategy::canCaptureEnPassant(const ChessBoard& board, 
                                        bool isWhite,
                                        const std::pair<int, int>& from,
                                        const std::pair<int, int>& to) const {
    // Implementation for en passant would go here
    // Would require access to the last move made
    return false;
}

bool PawnMoveStrategy::isPromotionMove(bool isWhite, const std::pair<int, int>& to) const {
    // Check if pawn reached the last rank
    return (isWhite && to.first == 7) || (!isWhite && to.first == 0);
}
*/