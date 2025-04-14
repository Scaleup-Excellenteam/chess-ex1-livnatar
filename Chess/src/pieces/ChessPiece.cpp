
#include "pieces/ChessPiece.h"
#include "board/ChessBoard.h"
#include "factories/MoveStrategyFactory.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
/**
 * Constructor for a chess piece.
 *
 * @param isWhite   True if the piece is white, false if black.
 * @param pieceType Character representing the type of the piece (e.g., 'K' for King).
 * @param pos       Initial position on the board.
 */
ChessPiece::ChessPiece(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : m_isWhite(isWhite), m_pieceType(pieceType), m_position(pos) {

    m_moveStrategy = MoveStrategyFactory::createMoveStrategy(pieceType);
};

//------------------------------------------------------------------------
/**
 * Sets the piece's new position.
 *
 * @param newPos New coordinates on the board.
 */
void ChessPiece::setPosition(const std::pair<int, int>& newPos) {
    m_position = newPos;
}

//------------------------------------------------------------------------
/**
 * Returns the current position of the piece.
 *
 * @return Pair of integers representing the row and column.
 */
std::pair<int, int> ChessPiece::getPosition() const {
    return m_position;
}

//------------------------------------------------------------------------
/**
 * Returns the type of the piece.
 *
 * @return Character representing the type (e.g., 'P' for Pawn, 'K' for King).
 */
char ChessPiece::getPieceType() const {
    return m_pieceType;
}

//------------------------------------------------------------------------
/**
 * Returns the color of the piece.
 *
 * @return True if the piece is white, false if black.
 */
bool ChessPiece::getColor() const {
    return m_isWhite;
}
//------------------------------------------------------------------------
/**
 * Validates whether the move to a new position is allowed for this piece.
 * Delegates the validation to the appropriate movement strategy.
 *
 * @param board   The current state of the chess board.
 * @param newPos  The position to move to.
 * @return MOVE_SUCCESS if the move is valid, otherwise MOVE_INVALID_OR_BLOCKED.
 */
int ChessPiece::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
   
    if (!m_moveStrategy) {
        return MOVE_INVALID_OR_BLOCKED; // No strategy defined - fallback for any unexpected or malformed input
    }

    return m_moveStrategy->checkMovement(board, m_position, newPos);
}