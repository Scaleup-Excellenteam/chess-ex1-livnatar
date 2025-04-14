#include "strategies/PawnMoveStrategy.h"
#include "board/ChessBoard.h"
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