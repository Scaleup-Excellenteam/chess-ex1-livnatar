
#include "ChessPiece.h"

//------------------------------------------------------------------------
ChessPiece::ChessPiece(bool isWhite, char pieceType, std::pair<int, int> pos)
    : m_isWhite(isWhite), m_pieceType(type), m_position(pos) {};

//------------------------------------------------------------------------
void ChessPiece::setPosition(std::pair<int, int> newPos) {
    m_position = newPos;
}

//------------------------------------------------------------------------
std::pair<int, int> ChessPiece::getPosition() const {
    return position;
}

//------------------------------------------------------------------------
char ChessPiece::getPieceType() const {
    return m_pieceType;
}

//------------------------------------------------------------------------
bool ChessPiece::getColor() const {
    return m_isWhite;
}