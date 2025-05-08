#pragma once
#include "strategies/MoveStrategy.h"

class KnightMoveStrategy : public MoveStrategy {
public:
    int checkMovement(const ChessBoard& board,
        const std::pair<int, int>& from,
        const std::pair<int, int>& to) const override;
    
    std::vector<Move> generateMoves(const ChessBoard& board,
        const std::pair<int, int>& from,
        bool isWhite,
        char pieceType) const override;
};