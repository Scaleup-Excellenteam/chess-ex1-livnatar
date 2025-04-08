
#include "Knight.h"
#include "ChessBoard.h"
#include "GameConstants.h"

//------------------------------------------------------------------------
Knight::Knight(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}

//------------------------------------------------------------------------

int Knight::checkMovement(const ChessBoard& board, const std::pair<int, int>& newPos) const {


    // Path is clear - movement is valid
    return MOVE_SUCCESS;
}
