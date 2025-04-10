
#pragma once
#include "GameConstants.h"
#include <utility>

class ChessBoard;

class MoveStrategy {
public:
    
    virtual ~MoveStrategy() = default;

    // Movement validation method that all strategies must implement
    virtual int checkMovement(const ChessBoard& board,
                              const std::pair<int, int>& from,
                              const std::pair<int, int>& to) const = 0;
};