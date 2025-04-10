
#include "strategies/QueenMoveStrategy.h"
#include "board/ChessBoard.h"


QueenMoveStrategy::QueenMoveStrategy(std::shared_ptr<MoveStrategy> rookStrategy,
                                     std::shared_ptr<MoveStrategy> bishopStrategy)
    : m_rookStrategy(rookStrategy), m_bishopStrategy(bishopStrategy) {}

//------------------------------------------------------------------------
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