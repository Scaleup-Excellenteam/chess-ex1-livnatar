
#pragma once
#include <utility>

class ChessBoard;

class ChessPiece {
public:

    ChessPiece(bool isWhite, char pieceType, std::pair<int, int> pos);
    virtual ~ChessPiece() = default;
    virtual int checkMovement(const ChessBoard& board, std::pair<int, int> newPos) const = 0;
    std::pair<int, int> getPosition() const;
    char getPieceType() const;
    bool getColor() const;

protected:
    bool m_isWhite;
    char m_pieceType;
    std::pair<int, int> m_position;

    void setPosition(std::pair<int, int> newPos);
};