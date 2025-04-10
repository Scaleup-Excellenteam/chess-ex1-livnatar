
#pragma once
#include "strategies/MoveStrategy.h"

class RookMoveStrategy : public MoveStrategy {
public:
    int checkMovement(const ChessBoard& board,
                      const std::pair<int, int>& from,
                      const std::pair<int, int>& to) const override;
};