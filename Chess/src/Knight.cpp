
#include "Knight.h"
#include "ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
Knight::Knight(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------

int Knight::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {
   
    // Get current position
    int startRow = m_position.first;
    int startCol = m_position.second;
    int endRow = newPos.first;
    int endCol = newPos.second;

    int rowDiff = abs(endRow - startRow);
    int colDiff = abs(endCol - startCol);

    // Knight moves in an L-shape: 2 squares in one direction and 1 in the other
    bool isValidKnightMove = (rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1);

    if (!isValidKnightMove) {
        return MOVE_INVALID_OR_BLOCKED;
    }

    return MOVE_SUCCESS;
}
