

#include "pieces/King.h"

//------------------------------------------------------------------------
King::King(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}
