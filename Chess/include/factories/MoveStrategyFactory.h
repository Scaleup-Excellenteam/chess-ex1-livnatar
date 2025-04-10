
#pragma once
#include "strategies/MoveStrategy.h"
#include <memory>
#include <map>

class MoveStrategyFactory {
public:
    static void initialize();
    static std::shared_ptr<MoveStrategy> createMoveStrategy(char pieceType);

private:
    static std::map<char, std::shared_ptr<MoveStrategy>> m_strategies;
};