
#include "pieces/Pawn.h"

//------------------------------------------------------------------------
Pawn::Pawn(bool isWhite, char pieceType, const std::pair<int, int>& pos)
    : ChessPiece(isWhite, pieceType, pos) {}
