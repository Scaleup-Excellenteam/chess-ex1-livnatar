
#pragma once
#include <utility> 
#include <memory>
#include <vector>
#include "board/IBoardState.h"
#include "move/Move.h"

class MoveStrategy;

class ChessPiece {
public:

    ChessPiece(bool isWhite, char pieceType, const std::pair<int, int>& pos);
    virtual ~ChessPiece() = default;
    
    // Delete copy constructor and assignment
    ChessPiece(const ChessPiece&) = delete;
    ChessPiece& operator=(const ChessPiece&) = delete;
    
    // Add move constructor and assignment
    ChessPiece(ChessPiece&&) noexcept = default;
    ChessPiece& operator=(ChessPiece&&) noexcept = default;
    
    int checkMovement(const IBoardState& board, const std::pair<int, int>& newPos) const;
    std::vector<Move> generateValidMoves(const IBoardState& board) const;
    std::pair<int, int> getPosition() const;
    char getPieceType() const;
    bool getColor() const;
    void setPosition(const std::pair<int, int>& newPos);

protected:
    bool m_isWhite;
    char m_pieceType;
    std::pair<int, int> m_position;
    std::shared_ptr<MoveStrategy> m_moveStrategy;
};