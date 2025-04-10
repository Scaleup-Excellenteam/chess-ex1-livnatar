
#include "pieces/ChessPiece.h"
#include "board/ChessBoard.h"
#include "factories/MoveStrategyFactory.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
ChessPiece::ChessPiece(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : m_isWhite(isWhite), m_pieceType(pieceType), m_position(pos) {

    m_moveStrategy = MoveStrategyFactory::createMoveStrategy(pieceType);
};

//------------------------------------------------------------------------
void ChessPiece::setPosition(const std::pair<int, int>& newPos) {
    m_position = newPos;
}

//------------------------------------------------------------------------
std::pair<int, int> ChessPiece::getPosition() const {
    return m_position;
}

//------------------------------------------------------------------------
char ChessPiece::getPieceType() const {
    return m_pieceType;
}

//------------------------------------------------------------------------
bool ChessPiece::getColor() const {
    return m_isWhite;
}
//------------------------------------------------------------------------

int ChessPiece::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
   
    if (!m_moveStrategy) {
       
        return MOVE_INVALID_OR_BLOCKED; // No strategy defined - fallback for any unexpected or malformed input
    }

    return m_moveStrategy->checkMovement(board, m_position, newPos);
}