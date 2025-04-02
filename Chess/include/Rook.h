#pragma once
#include <ChessPiece.h>


class Rook : public ChessPiece {

public:

    Rook(bool isWhite, char pieceType, std::pair<int, int> pos);
    int isMoveValid(std::string position) override;
    
};