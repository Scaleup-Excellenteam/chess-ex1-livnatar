
#pragma once
#include "GameConstants.h"
#include <utility>
#include <vector>

class ChessBoard;
class Move;

class MoveStrategy {
public:
    
    virtual ~MoveStrategy() = default;

    // Movement validation method that all strategies must implement
    virtual int checkMovement(const ChessBoard& board,
                              const std::pair<int, int>& from,
                              const std::pair<int, int>& to) const = 0;

    // Generate all valid moves for a piece at given position
    virtual std::vector<Move> generateMoves(const ChessBoard& board,
                                            const std::pair<int, int>& from,
                                            bool isWhite,
                                            char pieceType) const = 0;
};