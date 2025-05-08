
#include "strategies/QueenMoveStrategy.h"
#include "board/ChessBoard.h"
#include "move/Move.h"

//------------------------------------------------------------------------
/**
 * Constructs a QueenMoveStrategy using existing rook and bishop strategies.
 */
QueenMoveStrategy::QueenMoveStrategy(std::shared_ptr<MoveStrategy> rookStrategy,
                                     std::shared_ptr<MoveStrategy> bishopStrategy)
    : m_rookStrategy(rookStrategy), m_bishopStrategy(bishopStrategy) {}

//------------------------------------------------------------------------
/**
 * Validates queen's move by delegating to rook and bishop strategies.
 *
 * @param board The chess board.
 * @param from  Starting position.
 * @param to    Target position.
 * @return MOVE_SUCCESS if valid, MOVE_INVALID_OR_BLOCKED otherwise.
 */
int QueenMoveStrategy::checkMovement(const ChessBoard& board,
    const std::pair<int, int>& from,
    const std::pair<int, int>& to) const {

    // Try the rook movement (horizontal/vertical)
    int rookResult = m_rookStrategy->checkMovement(board, from, to);
    if (rookResult == MOVE_SUCCESS) {
        return MOVE_SUCCESS;
    }

    // If rook movement failed, try bishop movement (diagonal)
    return m_bishopStrategy->checkMovement(board, from, to);

}
//------------------------------------------------------------------------
/**
 * Generates all valid moves for a queen from the given position.
 * The queen combines the movement capabilities of both a rook (horizontal/vertical)
 * and a bishop (diagonal), so this function delegates to both movement strategies.
 *
 * @param board      The current state of the chess board.
 * @param from       The current position of the queen (row, column).
 * @param isWhite    Indicates whether the queen is white or black.
 * @param pieceType  The character representing the piece type (e.g., 'Q' for Queen).
 * @return A vector of valid moves including captures; combines rook and bishop moves.
 */
std::vector<Move> QueenMoveStrategy::generateMoves(const ChessBoard& board,
                                                   const std::pair<int, int>& from,
                                                   bool isWhite,
                                                   char pieceType) const {

    // Queen combines the movement of Rook and Bishop
    std::vector<Move> validMoves;

    // Get rook-like moves
    std::vector<Move> rookMoves = m_rookStrategy->generateMoves(board, from, isWhite, pieceType);
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());

    // Get bishop-like moves
    std::vector<Move> bishopMoves = m_bishopStrategy->generateMoves(board, from, isWhite, pieceType);
    validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return validMoves;
}