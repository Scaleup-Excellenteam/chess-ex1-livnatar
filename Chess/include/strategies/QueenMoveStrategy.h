#pragma once
#include "strategies/MoveStrategy.h"
#include <memory>

class QueenMoveStrategy : public MoveStrategy {

public:
    QueenMoveStrategy(std::shared_ptr<MoveStrategy> rookStrategy,
                      std::shared_ptr<MoveStrategy> bishopStrategy);

    int checkMovement(const ChessBoard& board,
                      const std::pair<int, int>& from,
                      const std::pair<int, int>& to) const override;

private:
    std::shared_ptr<MoveStrategy> m_rookStrategy;
    std::shared_ptr<MoveStrategy> m_bishopStrategy;
};